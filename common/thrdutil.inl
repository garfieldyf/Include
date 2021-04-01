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

namespace stdutil {

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

#ifndef NDEBUG
    if (mEpollFd == -1 || mEventFd == -1 || result == -1) {
        LOGE("The epoll open failed - errno = %d\n", errno);
        assert(false);
    }
#endif  // NDEBUG

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
    ::write(mEventFd, &value, sizeof(uint64_t));
}

__INLINE__ void Epoll::wait(int timeout)
{
    struct epoll_event event;
    int result = ::epoll_wait(mEpollFd, &event, 1, timeout);

#ifndef NDEBUG
    if (result == -1) {
        LOGE("The epoll wait failed - errno = %d\n", errno);
        assert(false);
    }
#endif  // NDEBUG

    if (result > 0 && event.data.fd == mEventFd && (event.events & EPOLLIN)) {
        uint64_t value;
        ::read(mEventFd, &value, sizeof(uint64_t));
    }
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the LooperThread class
//

__INLINE__ LooperThread::LooperThread()
    : mRunning(false)
{
}

__INLINE__ LooperThread::~LooperThread()
{
#ifndef NDEBUG
    if (mRunning) {
        LOGE("The LooperThread has not stopped.\n");
        assert(false);
    }
#endif  // NDEBUG
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
        mTaskQueue.push_front(nullptr);
        mThread.join();
        LOGD("The pending tasks [size = %zu]\n", mTaskQueue.size());
        mTaskQueue.clear();
    }
}

template <typename _Callable>
__INLINE__ bool LooperThread::post(_Callable&& callable)
{
    const bool running = mRunning;

#ifndef NDEBUG
    Runnable task = std::forward<_Callable>(callable);
    if (!task) {
        LOGE("LooperThread::post() does not accept a nullptr.\n");
        assert(false);
    }

    if (running) {
        mTaskQueue.push_back(std::move(task));
    } else {
        LOGE("The LooperThread has not started.\n");
    }
#else
    if (running) {
        mTaskQueue.push_back(std::forward<_Callable>(callable));
    }
#endif  // NDEBUG

    return running;
}

template <typename _Callable>
__INLINE__ bool LooperThread::postAtFront(_Callable&& callable)
{
    const bool running = mRunning;

#ifndef NDEBUG
    Runnable task = std::forward<_Callable>(callable);
    if (!task) {
        LOGE("LooperThread::postAtFront() does not accept a nullptr.\n");
        assert(false);
    }

    if (running) {
        mTaskQueue.push_front(std::move(task));
    } else {
        LOGE("The LooperThread has not started.\n");
    }
#else
    if (running) {
        mTaskQueue.push_front(std::forward<_Callable>(callable));
    }
#endif  // NDEBUG

    return running;
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
// Implementation of the HandlerThread class
//

__INLINE__ HandlerThread::HandlerThread()
    : mRunning(false)
{
}

__INLINE__ HandlerThread::~HandlerThread()
{
#ifndef NDEBUG
    if (mRunning) {
        LOGE("The HandlerThread has not stopped.\n");
        assert(false);
    }
#endif  // NDEBUG
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
__INLINE__ bool HandlerThread::post(_Callable&& callable, uint32_t delayMillis/* = 0*/)
{
    const bool running = mRunning;

#ifndef NDEBUG
    Runnable task = std::forward<_Callable>(callable);
    if (!task) {
        LOGE("HandlerThread::post() does not accept a nullptr.\n");
        assert(false);
    }

    if (running) {
        {
            MutexLock lock(mMutex);
            mTaskQueue.emplace(std::move(task), delayMillis);
        }

        mEpoll.notify();
    } else {
        LOGE("The HandlerThread has not started.\n");
    }
#else
    if (running) {
        {
            MutexLock lock(mMutex);
            mTaskQueue.emplace(std::forward<_Callable>(callable), delayMillis);
        }

        mEpoll.notify();
    }
#endif  // NDEBUG

    return running;
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
            timeout = task.getWakeupTime(now);
        } else {
            // Got a task.
            outTask = std::move(const_cast<Task&>(task));
            mTaskQueue.pop();
        }
    }

    return timeout;
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the HandlerThread::Task class
//

template <typename _Callable>
__INLINE__ HandlerThread::Task::Task(_Callable&& callable, uint32_t delayMillis)
    : mWhen(std::chrono::steady_clock::now() + std::chrono::milliseconds(delayMillis))
    , mRunnable(std::forward<_Callable>(callable))
{
}

__INLINE__ bool HandlerThread::Task::operator>(const Task& right) const
{
    return (mWhen > right.mWhen);
}

__INLINE__ int HandlerThread::Task::getWakeupTime(const TimePoint& now) const
{
    const uint64_t timeout = std::chrono::duration_cast<std::chrono::milliseconds>(mWhen - now).count();
    return static_cast<int>(std::min(timeout, static_cast<uint64_t>(INT_MAX)));
}

}  // namespace stdutil

#endif  // __THRDUTIL_INL__