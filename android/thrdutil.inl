///////////////////////////////////////////////////////////////////////////////
// thrdutil.inl
//
// Author : Garfield
// Creation Date : 2020/4/20

#ifndef __THRDUTIL_INL__
#define __THRDUTIL_INL__

#ifndef __THRDUTIL_H__
    #error thrdutil.inl requires thrdutil.h to be included first
#endif

__BEGIN_NAMESPACE

///////////////////////////////////////////////////////////////////////////////
// Implementation of the LooperThread class
//

__INLINE__ LooperThread::LooperThread()
    : mRunning(false)
{
}

__INLINE__ LooperThread::~LooperThread()
{
    assert_log(!mRunning, "The LooperThread has not stopped.");
}

__INLINE__ void LooperThread::start()
{
    if (!mRunning.exchange(true)) {
        mThread = std::thread(&LooperThread::run, this);
    }
}

__INLINE__ void LooperThread::stop()
{
    if (mRunning.exchange(false)) {
        // Posts an empty task to exit this thread.
        mTaskQueue.emplace_front(nullptr);
        mThread.join();
        LOGD("The pending tasks [size = %zu]\n", mTaskQueue.size());
        mTaskQueue.clear();
    }
}

template <typename _Callable>
__INLINE__ void LooperThread::post(_Callable&& callable)
{
    assert_log(typeid(callable) != typeid(std::nullptr_t), "LooperThread::post() does not accept a nullptr.");

    if (mRunning) {
        mTaskQueue.emplace_back(std::forward<_Callable>(callable));
#ifndef NDEBUG
    } else {
        LOGE("The LooperThread has not started.\n");
#endif  // NDEBUG
    }
}

template <typename _Callable>
__INLINE__ void LooperThread::postAtFront(_Callable&& callable)
{
    assert_log(typeid(callable) != typeid(std::nullptr_t), "LooperThread::postAtFront() does not accept a nullptr.");

    if (mRunning) {
        mTaskQueue.emplace_front(std::forward<_Callable>(callable));
#ifndef NDEBUG
    } else {
        LOGE("The LooperThread has not started.\n");
#endif  // NDEBUG
    }
}

__INLINE__ void LooperThread::run()
{
    LOGD("LooperThread::start()\n");
    Runnable task;
    while (mTaskQueue.pop_front(task)) {    // might block
        // Exit the run, if the task is empty.
        if (!task) {
            break;
        }

        // Run the task.
        task();
    }

    LOGD("LooperThread::stop()\n");
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the Epoll class
//

__INLINE__ Epoll::Epoll()
    : mEpollFd(-1), mEventFd(-1)
{
}

__INLINE__ Epoll::~Epoll()
{
    close();
}

__INLINE__ int Epoll::open()
{
    assert(mEpollFd == -1);
    assert(mEventFd == -1);

    mEpollFd = ::epoll_create(1);
    mEventFd = ::eventfd(0, EFD_NONBLOCK);

    int result = -1;
    if (mEpollFd != -1 && mEventFd != -1) {
        struct epoll_event event = { 0 };
        event.events  = EPOLLIN;
        event.data.fd = mEventFd;
        result = ::epoll_ctl(mEpollFd, EPOLL_CTL_ADD, mEventFd, &event);
    }

    __check_error2(mEpollFd == -1 || mEventFd == -1 || result == -1, "The epoll open failed");
    return result;
}

__INLINE__ void Epoll::close()
{
    if (mEpollFd != -1) {
        LOGD("The epoll was closed [%d, %d]\n", mEpollFd, mEventFd);
        ::close(mEpollFd);
        ::close(mEventFd);
        mEpollFd = mEventFd = -1;
    }
}

__INLINE__ void Epoll::notify()
{
    uint64_t value = 1;
    verify(::write(mEventFd, &value, sizeof(uint64_t)), sizeof(uint64_t));
}

__INLINE__ void Epoll::wait(int timeout)
{
    struct epoll_event event;
    int result = ::epoll_wait(mEpollFd, &event, 1, timeout);
    __check_error2(result == -1, "The epoll wait failed");

    if (result > 0 && event.data.fd == mEventFd && (event.events & EPOLLIN)) {
        uint64_t value;
        verify(::read(mEventFd, &value, sizeof(uint64_t)), sizeof(uint64_t));
    }
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the HandlerThread class
//

__INLINE__ HandlerThread::HandlerThread()
    : mRunning(false)
{
}

__INLINE__ HandlerThread::~HandlerThread()
{
    assert_log(!mRunning, "The HandlerThread has not stopped.");
}

__INLINE__ void HandlerThread::start()
{
    if (!mRunning.exchange(true) && mEpoll.open() == 0) {
        mThread = std::thread(&HandlerThread::run, this);
    }
}

__INLINE__ void HandlerThread::stop()
{
    if (mRunning.exchange(false)) {
        mEpoll.notify();
        mThread.join();
        mEpoll.close();
        LOGD("The pending tasks [size = %zu]\n", mTaskQueue.size());
        mTaskQueue.clear();
    }
}

template <typename _Callable>
__INLINE__ void HandlerThread::post(_Callable&& callable, uint32_t delayMillis/* = 0*/)
{
    assert_log(typeid(callable) != typeid(std::nullptr_t), "HandlerThread::post() does not accept a nullptr.");

    if (mRunning) {
        {
            MutexLock lock(mMutex);
            mTaskQueue.emplace(std::forward<_Callable>(callable), delayMillis);
        }

        mEpoll.notify();
#ifndef NDEBUG
    } else {
        LOGE("The HandlerThread has not started.\n");
#endif  // NDEBUG
    }
}

__INLINE__ void HandlerThread::run()
{
    LOGD("HandlerThread::start()\n");
    Task task;
    for (;;) {
        do {
            int timeout = nextTask(task);   // might block
            if (timeout == -2) {
                // Got a task.
                break;
            }

            // Wait for events on the epoll.
            mEpoll.wait(timeout);
        } while (mRunning);

        // Exit the run, if this thread has stopped.
        if (!mRunning) {
            break;
        }

        // Run the task.
        task.mRunnable();
    }

    LOGD("HandlerThread::stop()\n");
}

__INLINE__ int HandlerThread::nextTask(Task& outTask)
{
    MutexLock lock(mMutex);
    int timeout = -2;
    if (mTaskQueue.empty()) {
        // No more tasks.
        timeout = -1;
    } else {
        const auto now = std::chrono::steady_clock::now();
        const Task& task = mTaskQueue.top();
        if (now < task.mWhen) {
            // Next task is not ready. Set a timeout to wake up when it is ready.
            timeout = task.getTimeout(now);
        } else {
            // Got a task.
            outTask = std::move(const_cast<Task&>(task));
            mTaskQueue.pop();
        }
    }

    return timeout;
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the Task class
//

__INLINE__ HandlerThread::Task::Task(Runnable&& runnable, uint32_t delayMillis)
    : mWhen(std::chrono::steady_clock::now() + std::chrono::milliseconds(delayMillis))
    , mRunnable(std::move(runnable))
{
}

__INLINE__ bool HandlerThread::Task::operator>(const Task& right) const
{
    return (mWhen > right.mWhen);
}

__INLINE__ int HandlerThread::Task::getTimeout(const TimePoint& now) const
{
    const uint64_t timeout = std::chrono::duration_cast<std::chrono::milliseconds>(mWhen - now).count();
    return static_cast<int>(std::min(timeout, static_cast<uint64_t>(INT_MAX)));
}

__END_NAMESPACE

#endif  // __THRDUTIL_INL__