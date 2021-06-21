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
// Global functions
//

#ifndef NDEBUG
__STATIC_INLINE__ void _Check_size(size_t size)
{
    if (size > 0) {
        LOGW("The number of %zu pending tasks will be discard.\n", size);
    }
}

__STATIC_INLINE__ void _Check_running(bool running, const char* classname)
{
    if (running) {
        LOGE("The %s has not quitted.\n", classname);
        assert(false);
    }
}

__STATIC_INLINE__ void _Check_joinable(std::thread& thread, const char* classname)
{
    if (thread.get_id() == std::this_thread::get_id()) {
        LOGE("%s::stop() can NOT call on self thread - RESOURCE_DEADLOCK_WOULD_OCCUR.\n", classname);
        assert(false);
    }
}

template <typename _Callable>
__STATIC_INLINE__ Runnable _Build_task(_Callable&& callable, const char* prefix)
{
    Runnable task = std::forward<_Callable>(callable);
    if (!task) {
        LOGE("%s does not accept an empty callable.\n", prefix);
        assert(false);
    }

    return task;
}
#else
#define _Check_size(_Size)                          ((void)0)
#define _Check_running(_Running, _Classname)        ((void)0)
#define _Check_joinable(_Thread, _Classname)        ((void)0)
#endif  // NDEBUG


///////////////////////////////////////////////////////////////////////////////
// Implementation of the Looper class
//

__INLINE__ constexpr Looper::Looper()
    : mThreadCount(1), mRunning(false)
{
}

__INLINE__ Looper::~Looper()
{
    _Check_running(mRunning, "Looper");
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
        // Removes all pending tasks.
        _Check_size(mTaskQueue.size());
        mTaskQueue.clear();
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
    if (running) {
        mTaskQueue.push_back(_Build_task(std::forward<_Callable>(callable), "Looper::post()"));
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
    if (running) {
        mTaskQueue.push_front(_Build_task(std::forward<_Callable>(callable), "Looper::postAtFront()"));
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


///////////////////////////////////////////////////////////////////////////////
// Implementation of the EventLooper class
//

__INLINE__ constexpr EventLooper::EventLooper()
    : mRunning(false)
{
}

__INLINE__ EventLooper::~EventLooper()
{
    _Check_running(mRunning, "EventLooper");
}

__INLINE__ bool EventLooper::prepare()
{
    return (!mRunning.exchange(true) && mEventFd.create() != -1);
}

__INLINE__ void EventLooper::run()
{
    Task task;
    while (mRunning) {
        const int timeout = mTaskQueue.pop(task);
        if (timeout == 0) {
            // Got a task, run it.
            task.runnable();
        } else {
            // The task is not ready. Waiting a
            // timeout to wake up when it is ready.
            mEventFd.wait(timeout);
        }
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
        mEventFd.notify();
    }

    return result;
}

template <typename _Callable, _Enable_if_callable_t<_Callable>>
__INLINE__ bool EventLooper::post(_Callable&& callable, uint32_t delayMillis/* = 0*/)
{
    const bool running = mRunning;

#ifndef NDEBUG
    if (running) {
        mTaskQueue.push(_Build_task(std::forward<_Callable>(callable), "EventLooper::post()"), delayMillis);
        mEventFd.notify();  // Wakes up the waiting thread.
    } else {
        LOGE("The EventLooper has not initialized. Did not call EventLooper::prepare().\n");
    }
#else
    if (running) {
        mTaskQueue.push(std::forward<_Callable>(callable), delayMillis);
        mEventFd.notify();  // Wakes up the waiting thread.
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
    _Check_size(_Base::size());
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
    int timeout = -1;   // Waits to indefinitely.
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
        _Check_joinable(mThread, "LooperThread");
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

template <uint32_t _ThrdCount>
__INLINE__ ThreadPool<_ThrdCount>::ThreadPool()
{
    mLooper.mThreadCount = _ThrdCount;
}

template <uint32_t _ThrdCount>
__INLINE__ void ThreadPool<_ThrdCount>::start()
{
    if (mLooper.prepare()) {
        for (std::thread& thread : mThreads) {
            thread = std::thread(&Looper::run, &mLooper);
        }

        LOGD("ThreadPool::start()\n");
    }
}

template <uint32_t _ThrdCount>
__INLINE__ void ThreadPool<_ThrdCount>::stop()
{
    if (mLooper.quit()) {
        for (std::thread& thread : mThreads) {
            _Check_joinable(thread, "ThreadPool");
            thread.join();
        }

        if _CONSTEXPR (_ThrdCount > 1) {
            // Removes all pending tasks.
            _Check_size(mLooper.mTaskQueue.size());
            mLooper.mTaskQueue.clear();
        }

        LOGD("ThreadPool::stop()\n");
    }
}

template <uint32_t _ThrdCount> template <typename _Callable, _Enable_if_callable_t<_Callable>>
__INLINE__ bool ThreadPool<_ThrdCount>::post(_Callable&& callable)
{
    return mLooper.post(std::forward<_Callable>(callable));
}

template <uint32_t _ThrdCount> template <typename _Callable, _Enable_if_callable_t<_Callable>>
__INLINE__ bool ThreadPool<_ThrdCount>::postAtFront(_Callable&& callable)
{
    return mLooper.postAtFront(std::forward<_Callable>(callable));
}

template <uint32_t _ThrdCount>
__INLINE__ Looper& ThreadPool<_ThrdCount>::getLooper()
{
    return mLooper;
}

template <uint32_t _ThrdCount>
__INLINE__ bool ThreadPool<_ThrdCount>::isRunning() const
{
    return mLooper.isRunning();
}

template <uint32_t _ThrdCount>
__INLINE__ constexpr uint32_t ThreadPool<_ThrdCount>::getThreadCount() const
{
    return _ThrdCount;
}

}  // namespace stdutil

#endif  // __THRDUTIL_INL__