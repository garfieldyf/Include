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
// Implementation of the Looper class
//

__INLINE__ constexpr Looper::Looper()
    : mThreadCount(1), mRunning(false)
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
    return !mRunning.exchange(true);
}

__INLINE__ void Looper::run()
{
    Runnable task;
    while (mTaskQueue.pop_front(task)) {    // might block
        // Exit the run, if the task is empty.
        if (!task) {
            break;
        }

        // Run the task.
        task();
    }

    if (mThreadCount == 1) {
        clear();    // Removes all pending tasks.
    }
}

__INLINE__ bool Looper::quit()
{
    const bool result = mRunning.exchange(false);
    if (result) {
        for (uint32_t i = 0; i < mThreadCount; ++i) {
            // Posts an empty task to end the loop.
            mTaskQueue.push_front(nullptr);
        }
    }

    return result;
}

template <typename _Callable, _Enable_if_callable_t<_Callable>>
__INLINE__ bool Looper::post(_Callable&& callable)
{
    const bool running = mRunning;

#ifndef NDEBUG
    Runnable task = std::forward<_Callable>(callable);
    if (!task) {
        LOGE("Looper::post() does not accept an empty callable.\n");
        assert(false);
    }

    if (running) {
        mTaskQueue.push_back(std::move(task));
    } else {
        LOGE("The Looper has not initialized. Did not call Looper::prepare().\n");
    }
#else
    if (running) {
        mTaskQueue.push_back(std::forward<_Callable>(callable));
    }
#endif  // NDEBUG

    return running;
}

template <typename _Callable, _Enable_if_callable_t<_Callable>>
__INLINE__ bool Looper::postAtFront(_Callable&& callable)
{
    const bool running = mRunning;

#ifndef NDEBUG
    Runnable task = std::forward<_Callable>(callable);
    if (!task) {
        LOGE("Looper::postAtFront() does not accept an empty callable.\n");
        assert(false);
    }

    if (running) {
        mTaskQueue.push_front(std::move(task));
    } else {
        LOGE("The Looper has not initialized. Did not call Looper::prepare().\n");
    }
#else
    if (running) {
        mTaskQueue.push_front(std::forward<_Callable>(callable));
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

__INLINE__ void Looper::clear()
{
#ifndef NDEBUG
    const size_t size = mTaskQueue.size();
    if (size > 0) {
        LOGW("The number of %zu pending tasks will be discard.\n", size);
    }
#endif  // NDEBUG

    mTaskQueue.clear();
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the EventLooper class
//

__INLINE__ constexpr EventLooper::EventLooper()
    : mRunning(false)
{
}

__INLINE__ EventLooper::~EventLooper()
{
#ifndef NDEBUG
    if (mRunning) {
        LOGE("The EventLooper has not quitted.\n");
        assert(false);
    }
#endif  // NDEBUG
}

__INLINE__ bool EventLooper::prepare()
{
    return (!mRunning.exchange(true) && mEventFd.create() != -1);
}

__INLINE__ void EventLooper::run()
{
    Task task;
    while (nextTask(task)) {  // might block
        task.runnable();
    }

    // Closes the eventfd and removes all pending tasks.
    mEventFd.close();
    mTaskQueue.clear();
}

__INLINE__ bool EventLooper::quit()
{
    const bool result = mRunning.exchange(false);
    if (result) {
        // Wakes up the waiting thread to end the loop.
        mEventFd.write();
    }

    return result;
}

template <typename _Callable, _Enable_if_callable_t<_Callable>>
__INLINE__ bool EventLooper::post(_Callable&& callable, uint32_t delayMillis/* = 0*/)
{
    const bool running = mRunning;

#ifndef NDEBUG
    Runnable runnable = std::forward<_Callable>(callable);
    if (!runnable) {
        LOGE("EventLooper::post() does not accept an empty callable.\n");
        assert(false);
    }

    if (running) {
        mTaskQueue.push(std::move(runnable), delayMillis);
        mEventFd.write();   // Wakes up the waiting thread.
    } else {
        LOGE("The EventLooper has not initialized. Did not call EventLooper::prepare().\n");
    }
#else
    if (running) {
        mTaskQueue.push(std::forward<_Callable>(callable), delayMillis);
        mEventFd.write();   // Wakes up the waiting thread.
    }
#endif  // NDEBUG

    return running;
}

__INLINE__ bool EventLooper::isRunning() const
{
    return mRunning;
}

__INLINE__ EventLooper& EventLooper::getMainLooper()
{
    static EventLooper sMainLooper;
    return sMainLooper;
}

__INLINE__ bool EventLooper::nextTask(Task& outTask)
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
// Implementation of the EventLooper::Task class
//

template <typename _Callable>
__INLINE__ EventLooper::Task::Task(_Callable&& callable, uint32_t delayMillis)
    : when(std::chrono::steady_clock::now() + std::chrono::milliseconds(delayMillis))
    , runnable(std::forward<_Callable>(callable))
{
}

__INLINE__ int EventLooper::Task::getTimeout() const
{
    using namespace std::chrono;
    int64_t timeout = duration_cast<milliseconds>(when - steady_clock::now()).count();
    if (timeout < 0) {
        timeout = 0;
    } else if (timeout > INT_MAX) {
        timeout = INT_MAX;
    }

    return static_cast<int>(timeout);
}

__INLINE__ bool EventLooper::Task::operator>(const Task& right) const
{
    return (when > right.when);
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the EventLooper::TaskQueue class
//

__INLINE__ void EventLooper::TaskQueue::clear()
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
__INLINE__ void EventLooper::TaskQueue::push(_Callable&& callable, uint32_t delayMillis)
{
    MutexLock lock(mMutex);
    _Base::emplace(std::forward<_Callable>(callable), delayMillis);
}

__INLINE__ int EventLooper::TaskQueue::pop(Task& outTask)
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
        mThread = std::thread(&EventLooper::run, &mLooper);
        LOGD("LooperThread::start()\n");
    }
}

__INLINE__ void LooperThread::stop()
{
    if (mLooper.quit()) {
    #ifndef NDEBUG
        if (mThread.get_id() == std::this_thread::get_id()) {
            LOGE("LooperThread::stop() can NOT call on self thread.\n");
            assert(false);
        }
    #endif  // NDEBUG

        mThread.join();
        LOGD("LooperThread::stop()\n");
    }
}

template <typename _Callable, _Enable_if_callable_t<_Callable>>
__INLINE__ bool LooperThread::post(_Callable&& callable, uint32_t delayMillis/* = 0*/)
{
    return mLooper.post(std::forward<_Callable>(callable), delayMillis);
}

__INLINE__ EventLooper& LooperThread::getLooper()
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


///////////////////////////////////////////////////////////////////////////////
// Implementation of the ThreadPool class
//

template <uint32_t _ThreadCount>
__INLINE__ ThreadPool<_ThreadCount>::ThreadPool()
{
    mLooper.mThreadCount = _ThreadCount;
}

template <uint32_t _ThreadCount>
__INLINE__ void ThreadPool<_ThreadCount>::start()
{
    if (mLooper.prepare()) {
        for (std::thread& thread : mThreads) {
            thread = std::thread(&Looper::run, &mLooper);
        }

        LOGD("ThreadPool::start()\n");
    }
}

template <uint32_t _ThreadCount>
__INLINE__ void ThreadPool<_ThreadCount>::stop()
{
    if (mLooper.quit()) {
        for (std::thread& thread : mThreads) {
            thread.join();
        }

        // Removes all pending tasks.
        mLooper.clear();
        LOGD("ThreadPool::stop()\n");
    }
}

template <uint32_t _ThreadCount> template <typename _Callable, _Enable_if_callable_t<_Callable>>
__INLINE__ bool ThreadPool<_ThreadCount>::post(_Callable&& callable)
{
    return mLooper.post(std::forward<_Callable>(callable));
}

template <uint32_t _ThreadCount> template <typename _Callable, _Enable_if_callable_t<_Callable>>
__INLINE__ bool ThreadPool<_ThreadCount>::postAtFront(_Callable&& callable)
{
    return mLooper.postAtFront(std::forward<_Callable>(callable));
}

template <uint32_t _ThreadCount>
__INLINE__ Looper& ThreadPool<_ThreadCount>::getLooper()
{
    return mLooper;
}

template <uint32_t _ThreadCount>
__INLINE__ bool ThreadPool<_ThreadCount>::isRunning() const
{
    return mLooper.isRunning();
}

template <uint32_t _ThreadCount>
__INLINE__ constexpr uint32_t ThreadPool<_ThreadCount>::getThreadCount() const
{
    return _ThreadCount;
}

}  // namespace stdutil

#endif  // __THRDUTIL_INL__