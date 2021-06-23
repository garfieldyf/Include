///////////////////////////////////////////////////////////////////////////////
// thrdutil.h
//
// Author : Garfield
// Creation Date : 2020/4/20

#ifndef __THRDUTIL_H__
#define __THRDUTIL_H__

#include "syncutil.h"
#include "stdcolls.h"

///////////////////////////////////////////////////////////////////////////////
// Classes in this file:
//
// Looper
// EventLooper
// LooperThread
// ThreadPool<_ThrdCount>

namespace stdutil {

///////////////////////////////////////////////////////////////////////////////
// Type definitions
//

template <uint32_t _ThrdCount = 1>
class ThreadPool;

using Runnable = std::function<void()>;

template <typename _Callable>
using _Enable_if_callable_t = std::enable_if_t<!std::is_same<std::decay_t<_Callable>, std::nullptr_t>::value, int>;


///////////////////////////////////////////////////////////////////////////////
// Interface of the Looper class
//

class Looper final
{
// Constructors/Destructor
public:
    constexpr Looper();
    ~Looper();

    Looper(const Looper&) = delete;
    Looper& operator=(const Looper&) = delete;

// Operations
public:
    /**
     * Initializes the current thread as a looper.
     * @return Returns true if this looper was successfully initialized.
     * Returns false if this looper was already initialized.
     */
    bool prepare();

    /**
     * Runs the task queue in the thread. Be sure to call quit() to end the loop.
     */
    void run();

    /**
     * Quits this looper. All pending tasks will be removed from the task queue.
     * @return Returns true if this looper was successfully quitted. Returns false
     * if this looper was already quitted.
     */
    bool quit();

    /**
     * Posts a callable to the end of the task queue. The callable will be run 
     * on the the thread.
     * @param callable The callable that will be executed, Maybe a pointer to
     * function, pointer to member function, lambda expression, or any kind of
     * move-constructible function object.
     * @return Returns true if the callable was successfully added to the task
     * queue. Returns false on failure, usually because this looper processing
     * the task queue is quitting.
     */
    template <typename _Callable, _Enable_if_callable_t<_Callable> = 0>
    bool post(_Callable&& callable);

    /**
     * Posts a callable to the beginning of the task queue. The callable will 
     * be run on the thread.
     * @param callable The callable that will be executed, Maybe a pointer to
     * function, pointer to member function, lambda expression, or any kind of
     * move-constructible function object.
     * @return Returns true if the callable was successfully added to the task
     * queue. Returns false on failure, usually because this looper processing
     * the task queue is quitting.
     */
    template <typename _Callable, _Enable_if_callable_t<_Callable> = 0>
    bool postAtFront(_Callable&& callable);

// Attributes
public:
    /**
     * Tests if this looper is running.
     * @return true if this looper is running, false otherwise.
     */
    bool isRunning() const;

    /**
     * Returns the looper associated with the main thread of the application.
     * @return A reference to the Looper.
     */
    static Looper& getMainLooper();

// Implementation
private:
    template <uint32_t _ThrdCount>
    friend class ThreadPool;

// Data members
private:
    uint32_t mThreadCount;
    std::atomic_bool mRunning;
    blocking_deque<Runnable> mTaskQueue;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the EventLooper class
//

class EventLooper final
{
// Constructors/Destructor
public:
    constexpr EventLooper();
    ~EventLooper();

    EventLooper(const EventLooper&) = delete;
    EventLooper& operator=(const EventLooper&) = delete;

// Operations
public:
    /**
     * Initializes the current thread as a looper.
     * @return Returns true if this looper was successfully initialized.
     * Returns false if this looper was already initialized.
     */
    bool prepare();

    /**
     * Runs the task queue in this thread. Be sure to call quit() to end the loop.
     */
    void run();

    /**
     * Quits this looper. All pending tasks will be removed from the task queue.
     * @return Returns true if this looper was successfully quitted. Returns false
     * if this looper was already quitted.
     */
    bool quit();

    /**
     * Posts a callable to the task queue, to be run after the specified amount of time elapses.
     * @param callable The callable that will be executed, Maybe a pointer to function, pointer
     * to member function, lambda expression, or any kind of move-constructible function object.
     * @param delayMillis The delay in milliseconds until the callable will be executed.
     * @return Returns true if the callable was successfully added to the task queue. Returns false
     * on failure, usually because this looper processing the task queue is quitting.
     */
    template <typename _Callable, _Enable_if_callable_t<_Callable> = 0>
    bool post(_Callable&& callable, uint32_t delayMillis = 0);

// Attributes
public:
    /**
     * Tests if this looper is running.
     * @return true if this looper is running, false otherwise.
     */
    bool isRunning() const;

    /**
     * Returns the looper associated with the main thread of the application.
     * @return A reference to the EventLooper.
     */
    static EventLooper& getMainLooper();

// Nested classes
private:
    using MutexLock = std::lock_guard<std::mutex>;
    using TimePoint = std::chrono::steady_clock::time_point;

    ///////////////////////////////////////////////////////////////////////////
    // Interface of the Task class
    //

    class Task final
    {
    // Constructors
    public:
        Task() = default;
        Task(Task&&) = default;
        Task& operator=(Task&&) = default;

        Task(const Task&) = delete;
        Task& operator=(const Task&) = delete;

        template <typename _Callable>
        Task(_Callable&& callable, uint32_t delayMillis);

    // Operations
    public:
        /**
         * Returns the timeout in milliseconds since std::steady_clock::now().
         * @return The timeout in milliseconds, -1 causes wait to indefinitely.
         */
        int getTimeout() const;

        /**
         * Tests if this task is greater than right.
         * @return true if this task is greater than right, false otherwise.
         */
        bool operator>(const Task& right) const;

    // Data members
    public:
        TimePoint mWhen;
        Runnable mRunnable;
    };

    ///////////////////////////////////////////////////////////////////////////
    // Interface of the TaskQueue class
    //

    class TaskQueue final : private priority_queue<Task, vector<Task>, std::greater<Task>>
    {
    // Constructors
    public:
        constexpr TaskQueue() = default;

    // Operations
    public:
        /**
         * Removes all tasks from this queue, leaving it empty.
         */
        void clear();

        /**
         * Adds a callable to this queue, to be run after the 
         * specified amount of time elapses.
         * @param callable The callable that will be executed.
         * @param delayMillis The delay in milliseconds until 
         * the callable will be executed.
         */
        template <typename _Callable>
        void push(_Callable&& callable, uint32_t delayMillis);

        /**
         * Retrieves and removes the task on top of this queue.
         * @param outTask The outTask to store the returned task.
         * @return The waiting timeout in milliseconds.
         */
        int pop(Task& outTask);

    // Implementation
    private:
        using _Base = priority_queue<Task, vector<Task>, std::greater<Task>>;

    // Data members
    private:
        std::mutex mMutex;
    };

// Data members
private:
    Event mEvent;
    TaskQueue mTaskQueue;
    std::atomic_bool mRunning;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the LooperThread class
//

class LooperThread final
{
// Constructors
public:
    LooperThread() = default;

// Operations
public:
    /**
     * Starts this thread to begin execution.
     */
    void start();

    /**
     * Forces this thread to stop executing. All pending
     * tasks will be removed from the task queue.
     */
    void stop();

    /**
     * Posts a callable to the task queue, to be run after the specified amount of time elapses.
     * @param callable The callable that will be executed, Maybe a pointer to function, pointer
     * to member function, lambda expression, or any kind of move-constructible function object.
     * @param delayMillis The delay in milliseconds until the callable will be executed.
     * @return Returns true if the callable was successfully added to the task queue. Returns false
     * on failure, usually because this thread was stopped.
     */
    template <typename _Callable, _Enable_if_callable_t<_Callable> = 0>
    bool post(_Callable&& callable, uint32_t delayMillis = 0);

// Attributes
public:
    /**
     * Returns the looper associated with this thread.
     * @return A reference to the EventLooper.
     */
    EventLooper& getLooper();

    /**
     * Tests if this thread is running.
     * @return true if this thread is running, false otherwise.
     */
    bool isRunning() const;

    /**
     * Returns this thread id.
     * @return An object of type std::thread::id of this thread.
     */
    std::thread::id getId() const;

// Data members
private:
    EventLooper mLooper;
    std::thread mThread;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the ThreadPool class
//

template <uint32_t _ThrdCount>
class ThreadPool final
{
    static_assert(_ThrdCount >= 1 && _ThrdCount <= 8, "Invalid template argument for ThreadPool: _ThrdCount must be >= 1 and <= 8");

// Constructors
public:
    ThreadPool();

// Operations
public:
    /**
     * Starts this thread pool to begin execution.
     */
    void start();

    /**
     * Forces this thread pool to stop executing. All
     * pending tasks will be removed from the task queue.
     */
    void stop();

    /**
     * Posts a callable to the end of the task queue. The callable will be run 
     * on the thread.
     * @param callable The callable that will be executed, Maybe a pointer to
     * function, pointer to member function, lambda expression, or any kind of
     * move-constructible function object.
     * @return Returns true if the callable was successfully added to the task
     * queue. Returns false on failure, usually because this pool was stopped.
     */
    template <typename _Callable, _Enable_if_callable_t<_Callable> = 0>
    bool post(_Callable&& callable);

    /**
     * Posts a callable to the beginning of the task queue. The callable will 
     * be run on the thread.
     * @param callable The callable that will be executed, Maybe a pointer to
     * function, pointer to member function, lambda expression, or any kind of
     * move-constructible function object.
     * @return Returns true if the callable was successfully added to the task
     * queue. Returns false on failure, usually because this pool was stopped.
     */
    template <typename _Callable, _Enable_if_callable_t<_Callable> = 0>
    bool postAtFront(_Callable&& callable);

// Attributes
public:
    /**
     * Returns the looper associated with this thread pool.
     * @return A reference to the Looper.
     */
    Looper& getLooper();

    /**
     * Tests if this thread pool is running.
     * @return true if this thread pool is running, false otherwise.
     */
    bool isRunning() const;

    /**
     * Returns the number of threads in this thread pool.
     * @return The number of threads.
     */
    constexpr uint32_t getThreadCount() const;

// Data members
private:
    Looper mLooper;
    std::thread mThreads[_ThrdCount];
};

}  // namespace stdutil

#include "thrdutil.inl"

#endif  // __THRDUTIL_H__