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
        struct epoll_event ev = { 0 };
        ev.events  = EPOLLIN;
        ev.data.fd = mEventFd;
        result = ::epoll_ctl(mEpollFd, EPOLL_CTL_ADD, mEventFd, &ev);
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
    uint64_t counter = 1;
    verify(::write(mEventFd, &counter, sizeof(uint64_t)), sizeof(uint64_t));
}

__INLINE__ void Epoll::wait(int timeout)
{
    struct epoll_event ev;
    int result = ::epoll_wait(mEpollFd, &ev, 1, timeout);
    __check_error2(result == -1, "The epoll wait failed");

    if (result > 0 && ev.data.fd == mEventFd && (ev.events & EPOLLIN)) {
        uint64_t counter;
        verify(::read(mEventFd, &counter, sizeof(uint64_t)), sizeof(uint64_t));
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
    assert_log(!mRunning, "The LooperThread is not stop.\n");
}

__INLINE__ void LooperThread::start()
{
    if (!mRunning.exchange(true) && mEpoll.open() == 0) {
        mThread = std::thread(&LooperThread::run, this);
    }
}

__INLINE__ void LooperThread::stop()
{
    if (mRunning.exchange(false)) {
        mEpoll.notify();
        mThread.join();
        mEpoll.close();
        mTaskQueue.clear();
    }
}

template <typename _Callable>
__INLINE__ void LooperThread::post(_Callable&& callable, uint32_t delayMillis/* = 0*/)
{
    if (mRunning) {
        {
            MutexLock lock(mMutex);
            mTaskQueue.emplace(std::forward<_Callable>(callable), delayMillis);
        }

        mEpoll.notify();
    }
}

__INLINE__ void LooperThread::post(std::nullptr_t, uint32_t /*delayMillis = 0*/)
{
    assert_log(false, "LooperThread::post() does not accept a nullptr.\n");
}

__INLINE__ void LooperThread::run()
{
    LOGD("LooperThread::start()\n");
    while (true) {
        Task task;
        nextTask(task);     // might block
        if (!mRunning) {
            break;
        }

        task.mRunnable();
    }

    LOGD("LooperThread::stop()\n");
}

__INLINE__ void LooperThread::nextTask(Task& outTask)
{
    int timeout = 0;
    while (true) {
        mEpoll.wait(timeout);
        if (!mRunning) {
            break;
        }

        MutexLock lock(mMutex);
        if (mTaskQueue.empty()) {
            // No more tasks.
            timeout = -1;
        } else {
            const auto now = std::chrono::steady_clock::now();
            const Task& task = mTaskQueue.top();
            if (now < task.mWhen) {
                // Next task is not ready. Set a timeout to wake up when it is ready.
                timeout = task.duration(now);
            } else {
                // Got a task.
                outTask = std::move(const_cast<Task&>(task));
                mTaskQueue.pop();
                break;
            }
        }
    }
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the Task class
//

__INLINE__ LooperThread::Task::Task(Runnable&& runnable, uint32_t delayMillis)
    : mWhen(std::chrono::steady_clock::now() + std::chrono::milliseconds(delayMillis))
    , mRunnable(std::move(runnable))
{
}

__INLINE__ bool LooperThread::Task::operator>(const Task& right) const
{
    return (mWhen > right.mWhen);
}

__INLINE__ int LooperThread::Task::duration(const TimePoint& now) const
{
    const uint64_t timeout = std::chrono::duration_cast<std::chrono::milliseconds>(mWhen - now).count();
    return static_cast<int>(std::min(timeout, static_cast<uint64_t>(INT_MAX)));
}

__END_NAMESPACE

#endif  // __THRDUTIL_INL__