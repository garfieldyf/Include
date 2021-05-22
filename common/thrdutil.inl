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
// Implementation of the WorkerThread class
//

__INLINE__ WorkerThread::WorkerThread()
    : mRunning(false)
{
}

__INLINE__ WorkerThread::~WorkerThread()
{
#ifndef NDEBUG
    if (mRunning) {
        LOGE("The WorkerThread has not stopped.\n");
        assert(false);
    }
#endif  // NDEBUG
}

__INLINE__ void WorkerThread::start()
{
    if (!mRunning.exchange(true)) {
        mThread = std::thread(&WorkerThread::run, this);
    }
}

__INLINE__ void WorkerThread::stop()
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

template <typename _Callable, _Enable_if_callable_t<_Callable>>
__INLINE__ bool WorkerThread::post(_Callable&& callable)
{
    const bool running = mRunning;

#ifndef NDEBUG
    Runnable task = std::forward<_Callable>(callable);
    if (!task) {
        LOGE("The WorkerThread::post() does not accept an empty callable.\n");
        assert(false);
    }

    if (running) {
        mTaskQueue.push_back(std::move(task));
    } else {
        LOGE("The WorkerThread has not started.\n");
    }
#else
    if (running) {
        mTaskQueue.push_back(std::forward<_Callable>(callable));
    }
#endif  // NDEBUG

    return running;
}

template <typename _Callable, _Enable_if_callable_t<_Callable>>
__INLINE__ bool WorkerThread::postAtFront(_Callable&& callable)
{
    const bool running = mRunning;

#ifndef NDEBUG
    Runnable task = std::forward<_Callable>(callable);
    if (!task) {
        LOGE("The WorkerThread::postAtFront() does not accept an empty callable.\n");
        assert(false);
    }

    if (running) {
        mTaskQueue.push_front(std::move(task));
    } else {
        LOGE("The WorkerThread has not started.\n");
    }
#else
    if (running) {
        mTaskQueue.push_front(std::forward<_Callable>(callable));
    }
#endif  // NDEBUG

    return running;
}

__INLINE__ bool WorkerThread::isRunning() const
{
    return mRunning;
}

__INLINE__ std::thread::id WorkerThread::getId() const
{
    return mThread.get_id();
}

__INLINE__ void WorkerThread::run()
{
    LOGD("WorkerThread::start()\n");
    Runnable task;
    while (mTaskQueue.pop_front(task)) {    // might block
        // Exit the run, if the task is empty.
        if (!task) {
            break;
        }

        // Run the task.
        task();
    }

    LOGD("WorkerThread::stop()\n");
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the Looper class
//

__INLINE__ constexpr Looper::Looper()
    : mRunning(false)
{
}

__INLINE__ Looper::~Looper()
{
#ifndef NDEBUG
    if (mRunning) {
        LOGE("The Looper has not quitted.\n");
        assert(false);
    }
#endif  // NDEBUG
}

__INLINE__ bool Looper::prepare()
{
    return (!mRunning.exchange(true) && mEventFd.create() != -1);
}

__INLINE__ void Looper::run()
{
    Task task;
    while (nextTask(task)) {  // might block
        task.runnable();
    }
}

template <typename _Callback>
__INLINE__ void Looper::quit(_Callback callback)
{
    if (mRunning.exchange(false)) {
        mEventFd.write();
        callback();
        mEventFd.close();
        mTaskQueue.clear();  // Clears all pending tasks.
    }
}

template <typename _Callable, _Enable_if_callable_t<_Callable>>
__INLINE__ bool Looper::post(_Callable&& callable, uint32_t delayMillis/* = 0*/)
{
    const bool running = mRunning;

#ifndef NDEBUG
    Runnable runnable = std::forward<_Callable>(callable);
    if (!runnable) {
        LOGE("The Looper::post() does not accept an empty callable.\n");
        assert(false);
    }

    if (running) {
        mTaskQueue.push(std::move(runnable), delayMillis);
        mEventFd.write();
    } else {
        LOGE("The Looper has not initialized.\n");
    }
#else
    if (running) {
        mTaskQueue.push(std::forward<_Callable>(callable), delayMillis);
        mEventFd.write();
    }
#endif  // NDEBUG

    return running;
}

__INLINE__ bool Looper::isRunning() const
{
    return mRunning;
}

__INLINE__ Looper& Looper::getMainLooper()
{
    static Looper sMainLooper;
    return sMainLooper;
}

__INLINE__ bool Looper::nextTask(Task& outTask)
{
    while (mRunning) {
        const int timeout = mTaskQueue.pop(outTask);
        if (timeout == 0) {
            // Got a task.
            break;
        }

        // The next task is not ready. Waiting
        // a timeout to wake up when it is ready.
        mEventFd.poll(timeout);
    }

    return mRunning;
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the Looper::Task class
//

template <typename _Callable>
__INLINE__ Looper::Task::Task(_Callable&& callable, uint32_t delayMillis)
    : when(std::chrono::steady_clock::now() + std::chrono::milliseconds(delayMillis))
    , runnable(std::forward<_Callable>(callable))
{
}

__INLINE__ int Looper::Task::getTimeout() const
{
    int64_t timeout = std::chrono::duration_cast<std::chrono::milliseconds>(when - std::chrono::steady_clock::now()).count();
    if (timeout < 0) {
        timeout = 0;
    } else if (timeout > INT_MAX) {
        timeout = INT_MAX;
    }

    return static_cast<int>(timeout);
}

__INLINE__ bool Looper::Task::operator>(const Task& right) const
{
    return (when > right.when);
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the Looper::TaskQueue class
//

__INLINE__ void Looper::TaskQueue::clear()
{
    MutexLock lock(mMutex);

#ifndef NDEBUG
    const size_t size = _Base::size();
    if (size > 0) {
        LOGW("The number of %zu pending tasks will be discard.\n", size);
    }
#endif // NDEBUG

    _Base::clear();
}

template <typename _Callable>
__INLINE__ void Looper::TaskQueue::push(_Callable&& callable, uint32_t delayMillis)
{
    MutexLock lock(mMutex);
    _Base::emplace(std::forward<_Callable>(callable), delayMillis);
}

__INLINE__ int Looper::TaskQueue::pop(Task& outTask)
{
    MutexLock lock(mMutex);
    int timeout = -1;   // Waiting to indefinitely.
    if (!_Base::empty()) {
        const Task& task = _Base::top();
        if ((timeout = task.getTimeout()) == 0) {
            outTask = const_cast<Task&&>(task);
            _Base::pop();
        }
    }

    return timeout;
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the LooperThread class
//

__INLINE__ void LooperThread::start()
{
    if (mLooper.prepare()) {
        mThread = std::thread(&Looper::run, &mLooper);
        LOGD("LooperThread::start()\n");
    }
}

__INLINE__ void LooperThread::stop()
{
    mLooper.quit([this]() {
        mThread.join();
        LOGD("LooperThread::stop()\n");
    });
}

template <typename _Callable, _Enable_if_callable_t<_Callable>>
__INLINE__ bool LooperThread::post(_Callable&& callable, uint32_t delayMillis/* = 0*/)
{
    return mLooper.post(std::forward<_Callable>(callable), delayMillis);
}

__INLINE__ Looper& LooperThread::getLooper()
{
    return mLooper;
}

__INLINE__ bool LooperThread::isRunning() const
{
    return mLooper.isRunning();
}

__INLINE__ std::thread::id LooperThread::getId() const
{
    return mThread.get_id();
}

}  // namespace stdutil

#endif  // __THRDUTIL_INL__