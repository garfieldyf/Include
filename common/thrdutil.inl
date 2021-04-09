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
// Implementation of the ThreadBase class
//

__INLINE__ ThreadBase::ThreadBase()
    : mRunning(false)
{
}

__INLINE__ ThreadBase::~ThreadBase()
{
#ifndef NDEBUG
    if (mRunning) {
        LOGE("This thread has not stopped.\n");
        assert(false);
    }
#endif  // NDEBUG
}

__INLINE__ bool ThreadBase::isRunning() const
{
    return mRunning;
}

__INLINE__ std::thread::id ThreadBase::getId() const
{
    return mThread.get_id();
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the TaskThread class
//

__INLINE__ void TaskThread::start()
{
    if (!mRunning.exchange(true)) {
        mThread = std::thread(&TaskThread::run, this);
    }
}

__INLINE__ void TaskThread::stop()
{
    if (mRunning.exchange(false)) {
        // Posts an empty task to exit this thread.
        mTaskQueue.push_front(nullptr);
        mThread.join();

    #ifndef NDEBUG
        const size_t size = mTaskQueue.size();
        if (size > 0) {
            LOGW("The number of %zu pending tasks will be discard.\n", size);
        }
    #endif  // NDEBUG

        // Clears all pending tasks.
        mTaskQueue.clear();
    }
}

template <typename _Callable>
__INLINE__ bool TaskThread::post(_Callable&& callable)
{
    const bool running = mRunning;

#ifndef NDEBUG
    Runnable task = std::forward<_Callable>(callable);
    if (!task) {
        LOGE("TaskThread::post() does not accept an empty callable.\n");
        assert(false);
    }

    if (running) {
        mTaskQueue.push_back(std::move(task));
    } else {
        LOGE("The TaskThread has not started.\n");
    }
#else
    if (running) {
        mTaskQueue.push_back(std::forward<_Callable>(callable));
    }
#endif  // NDEBUG

    return running;
}

template <typename _Callable>
__INLINE__ bool TaskThread::postAtFront(_Callable&& callable)
{
    const bool running = mRunning;

#ifndef NDEBUG
    Runnable task = std::forward<_Callable>(callable);
    if (!task) {
        LOGE("TaskThread::postAtFront() does not accept an empty callable.\n");
        assert(false);
    }

    if (running) {
        mTaskQueue.push_front(std::move(task));
    } else {
        LOGE("The TaskThread has not started.\n");
    }
#else
    if (running) {
        mTaskQueue.push_front(std::forward<_Callable>(callable));
    }
#endif  // NDEBUG

    return running;
}

__INLINE__ void TaskThread::run()
{
    LOGD("TaskThread::start()\n");
    Runnable task;
    while (mTaskQueue.pop_front(task)) {    // might block
        // Exit the run, if the task is empty.
        if (!task) {
            break;
        }

        // Run the task.
        task();
    }

    LOGD("TaskThread::stop()\n");
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
#ifndef NDEBUG
    if (mEpollFd != -1) {
        LOGE("The epoll has not closed.\n");
        assert(false);
    }
#endif  // NDEBUG
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
    const int result = ::epoll_wait(mEpollFd, &event, 1, timeout);

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

    #ifndef NDEBUG
        const size_t size = mTaskQueue.size();
        if (size > 0) {
            LOGW("The number of %zu pending tasks will be discard.\n", size);
        }
    #endif  // NDEBUG

        // Clears all pending tasks.
        mTaskQueue.clear();
    }
}

template <typename _Callable>
__INLINE__ bool LooperThread::post(_Callable&& callable, uint32_t delayMillis/* = 0*/)
{
    const bool running = mRunning;

#ifndef NDEBUG
    Runnable runnable = std::forward<_Callable>(callable);
    if (!runnable) {
        LOGE("LooperThread::post() does not accept an empty callable.\n");
        assert(false);
    }

    if (running) {
        {
            MutexLock lock(mMutex);
            mTaskQueue.emplace(std::move(runnable), delayMillis);
        }

        mEpoll.notify();
    } else {
        LOGE("The LooperThread has not started.\n");
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

__INLINE__ void LooperThread::run()
{
    LOGD("LooperThread::start()\n");
    Task task;
    for (;;) {
        do {
            const int timeout = nextTask(task);   // might block
            if (timeout == 0) {
                // Got a task.
                break;
            }

            // The next task is not ready. Waiting a timeout to wake up when it is ready.
            mEpoll.wait(timeout);
        } while (mRunning);

        // Exit the run, if this thread has stopped.
        if (!mRunning) {
            break;
        }

        // Run the task.
        task.mRunnable();
    }

    LOGD("LooperThread::stop()\n");
}

__INLINE__ int LooperThread::nextTask(Task& outTask)
{
    MutexLock lock(mMutex);
    int timeout;
    if (mTaskQueue.empty()) {
        // No more tasks.
        timeout = -1;
    } else {
        const Task& task = mTaskQueue.top();
        if ((timeout = task.getTimeout()) == 0) {
            outTask = const_cast<Task&&>(task);
            mTaskQueue.pop();
        }
    }

    return timeout;
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the LooperThread::Task class
//

template <typename _Callable>
__INLINE__ LooperThread::Task::Task(_Callable&& callable, uint32_t delayMillis)
    : mWhen(std::chrono::steady_clock::now() + std::chrono::milliseconds(delayMillis))
    , mRunnable(std::forward<_Callable>(callable))
{
}

__INLINE__ int LooperThread::Task::getTimeout() const
{
    int64_t timeout = std::chrono::duration_cast<std::chrono::milliseconds>(mWhen - std::chrono::steady_clock::now()).count();
    if (timeout < 0) {
        timeout = 0;
    } else if (timeout > INT_MAX) {
        timeout = INT_MAX;
    }

    return static_cast<int>(timeout);
}

__INLINE__ bool LooperThread::Task::operator>(const Task& right) const
{
    return (mWhen > right.mWhen);
}

}  // namespace stdutil

#endif  // __THRDUTIL_INL__