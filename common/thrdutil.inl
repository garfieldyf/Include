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

template <typename _Callable, ThreadBase::_Check_callable_t<_Callable>>
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

template <typename _Callable, ThreadBase::_Check_callable_t<_Callable>>
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
// Implementation of the LooperThread class
//

__INLINE__ void LooperThread::start()
{
    if (!mRunning.exchange(true) && mEventFd.create() != -1) {
        mThread = std::thread(&LooperThread::run, this);
    }
}

__INLINE__ void LooperThread::stop()
{
    if (mRunning.exchange(false)) {
        mEventFd.write(1);
        mThread.join();
        mEventFd.close();
        mTaskQueue.clear();  // Clears all pending tasks.
    }
}

template <typename _Callable, ThreadBase::_Check_callable_t<_Callable>>
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
        mTaskQueue.push(std::move(runnable), delayMillis);
        mEventFd.write(1);
    } else {
        LOGE("The LooperThread has not started.\n");
    }
#else
    if (running) {
        mTaskQueue.push(std::forward<_Callable>(callable), delayMillis);
        mEventFd.write(1);
    }
#endif  // NDEBUG

    return running;
}

__INLINE__ void LooperThread::run()
{
    LOGD("LooperThread::start()\n");
    Task task;
    while (nextTask(task)) {    // might block
        task.mRunnable();
    }

    LOGD("LooperThread::stop()\n");
}

__INLINE__ void LooperThread::pollWait(int timeout)
{
    struct pollfd pfd = { mEventFd, POLLIN };
    const int result = ::poll(&pfd, 1, timeout);

#ifndef NDEBUG
    if (result == -1) {
        logError("The poll wait failed");
        assert(false);
    }
#endif  // NDEBUG

    if (result > 0 && pfd.fd == mEventFd && (pfd.revents & POLLIN)) {
        uint64_t value;
        mEventFd.read(value);
    }
}

__INLINE__ bool LooperThread::nextTask(Task& outTask)
{
    while (mRunning) {
        const int timeout = mTaskQueue.pop(outTask);
        if (timeout == 0) {
            // Got a task.
            break;
        }

        // The next task is not ready. Waiting
        // a timeout to wake up when it is ready.
        pollWait(timeout);
    }

    return mRunning;
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


///////////////////////////////////////////////////////////////////////////////
// Implementation of the LooperThread::TaskQueue class
//

__INLINE__ void LooperThread::TaskQueue::clear()
{
    MutexLock lock(mMutex);

#ifndef NDEBUG
    const size_t size = super::size();
    if (size > 0) {
        LOGW("The number of %zu pending tasks will be discard.\n", size);
    }
#endif // NDEBUG

    super::clear();
}

template <typename _Callable>
__INLINE__ void LooperThread::TaskQueue::push(_Callable&& callable, uint32_t delayMillis)
{
    MutexLock lock(mMutex);
    super::emplace(std::forward<_Callable>(callable), delayMillis);
}

__INLINE__ int LooperThread::TaskQueue::pop(Task& outTask)
{
    MutexLock lock(mMutex);
    int timeout;
    if (super::empty()) {
        // No more tasks.
        timeout = -1;
    } else {
        const Task& task = super::top();
        if ((timeout = task.getTimeout()) == 0) {
            outTask = const_cast<Task&&>(task);
            super::pop();
        }
    }

    return timeout;
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the LocalSocketThread class
//

template <typename _Callback>
__INLINE__ void LocalSocketThread::start(const char* name, _Callback&& callback)
{
    if (!mRunning.exchange(true) && mSocket.connect(name) == 0) {
        mCallback = std::forward<_Callback>(callback);
        mThread = std::thread(&LocalSocketThread::run, this);
        LOGD("LocalSocketThread::start()\n");
    }
}

__INLINE__ void LocalSocketThread::stop()
{
    if (mRunning.exchange(false)) {
        mSocket.close();
        mThread.join();
        LOGD("LocalSocketThread::stop()\n");
    }
}

__INLINE__ ssize_t LocalSocketThread::send(const void* buf, size_t size) const
{
    assert(buf);
    assert(size > 0);

#ifndef NDEBUG
    ssize_t sendBytes = -1;
    if (mRunning && !mSocket.isEmpty()) {
        sendBytes = mSocket.write(buf, size);
    } else {
        LOGE("The LocalSocketThread has not started.\n");
    }

    return sendBytes;
#else
    return (mRunning && !mSocket.isEmpty() ? mSocket.write(buf, size) : -1);
#endif  // NDEBUG
}

__INLINE__ void LocalSocketThread::run()
{
    uint8_t buf[8192];
    while (true) {
        const ssize_t size = mSocket.read(buf, sizeof(buf));
        if (size == 0) {
            break;
        }

        mCallback(buf, size);
    }
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the LocalServerThread class
//

template <typename _Callback>
__INLINE__ void LocalServerThread::start(const char* name, _Callback&& callback)
{
    if (!mRunning.exchange(true) && mServer.listen(name) == 0) {
        mCallback = std::forward<_Callback>(callback);
        mThread = std::thread(&LocalServerThread::run, this);
        LOGD("LocalServerThread::start()\n");
    }
}

__INLINE__ void LocalServerThread::stop()
{
    if (mRunning.exchange(false)) {
        mSocket.close();
        mServer.close();
        mThread.join();
        LOGD("LocalServerThread::stop()\n");
    }
}

__INLINE__ void LocalServerThread::run()
{
    const int sockFd = mServer.accept();
    if (sockFd != -1) {
        mSocket.attach(sockFd);
        LocalSocketThread::run();
    }
}

}  // namespace stdutil

#endif  // __THRDUTIL_INL__