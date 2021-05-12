///////////////////////////////////////////////////////////////////////////////
// thrdutil.h
//
// Author : Garfield
// Creation Date : 2020/4/20

#ifndef __THRDUTIL_H__
#define __THRDUTIL_H__

#include <thread>
#include "ipcutil.h"
#include "stdcolls.h"

///////////////////////////////////////////////////////////////////////////////
// Classes in this file:
//
// WorkerThread
// LooperThread
// LocalSocketThread
// LocalServerThread

namespace stdutil {

///////////////////////////////////////////////////////////////////////////////
// Interface of the ThreadBase class
//

class ThreadBase
{
// Constructors/Destructor
protected:
    ThreadBase();
    ~ThreadBase();

    ThreadBase(const ThreadBase&) = delete;
    ThreadBase& operator=(const ThreadBase&) = delete;

// Attributes
public:
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

// Implementation
protected:
    template <typename _Callable>
    using _Check_callable_t = std::enable_if_t<!std::is_same<std::decay_t<_Callable>, std::nullptr_t>::value, int>;

// Data members
protected:
    std::thread mThread;
    std::atomic_bool mRunning;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the WorkerThread class
//

class WorkerThread final : public ThreadBase
{
// Constructors
public:
    WorkerThread() = default;

// Operations
public:
    /**
     * Starts this thread to begin execution.
     */
    void start();

    /**
     * Forces this thread to stop executing. All remaining
     * tasks will be removed from the task queue.
     */
    void stop();

    /**
     * Posts a callable to the end of the task queue. The callable will be run 
     * on this thread.
     * @param callable The callable that will be executed, Maybe a pointer to
     * function, pointer to member function, lambda expression, or any kind of
     * move-constructible function object.
     * @return Returns true if the callable was successfully added to the task
     * queue. Returns false on failure, usually because this thread was stopped.
     */
    template <typename _Callable, _Check_callable_t<_Callable> = 0>
    bool post(_Callable&& callable);

    /**
     * Posts a callable to the beginning of the task queue. The callable will be 
     * run on this thread.
     * @param callable The callable that will be executed, Maybe a pointer to
     * function, pointer to member function, lambda expression, or any kind of
     * move-constructible function object.
     * @return Returns true if the callable was successfully added to the task
     * queue. Returns false on failure, usually because this thread was stopped.
     */
    template <typename _Callable, _Check_callable_t<_Callable> = 0>
    bool postAtFront(_Callable&& callable);

// Implementation
private:
    /**
     * This thread start entry point.
     */
    void run();

    using Runnable  = std::function<void()>;
    using TaskQueue = blocking_deque<Runnable>;

// Data members
private:
    TaskQueue mTaskQueue;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the LooperThread class
//

class LooperThread final : public ThreadBase
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
     * Forces this thread to stop executing. All remaining
     * tasks will be removed from the task queue.
     */
    void stop();

    /**
     * Posts a callable to the task queue, to be run after the specified amount 
     * of time elapses.
     * @param callable The callable that will be executed, Maybe a pointer to
     * function, pointer to member function, lambda expression, or any kind of
     * move-constructible function object.
     * @param delayMillis The delay in milliseconds until the callable will be
     * executed.
     * @return Returns true if the callable was successfully added to the task
     * queue. Returns false on failure, usually because this thread was stopped.
     */
    template <typename _Callable, _Check_callable_t<_Callable> = 0>
    bool post(_Callable&& callable, uint32_t delayMillis = 0);

// Implementation
private:
    class Task;

    /**
     * This thread start entry point.
     */
    void run();

    /**
     * Retrieves and removes the task on top of the task queue.
     * @param outTask The outTask to store the returned task.
     * @return returns true if retrieves successful, false otherwise.
     */
    bool nextTask(Task& outTask);

// Nested classes
private:
    using Runnable  = std::function<void()>;
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
        template <typename _Callable>
        Task(_Callable&& callable, uint32_t delayMillis);

        Task(Task&&) = default;
        Task& operator=(Task&&) = default;

        Task(const Task&) = delete;
        Task& operator=(const Task&) = delete;

    // Operations
    public:
        /**
         * Returns the timeout in milliseconds since std::steady_clock::now().
         * @return The timeout in milliseconds.
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
        TaskQueue() = default;

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
        using super = priority_queue<Task, vector<Task>, std::greater<Task>>;

    // Data members
    private:
        std::mutex mMutex;
    };

// Data members
private:
    EventFd mEventFd;
    TaskQueue mTaskQueue;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the LocalSocketThread class
//

class LocalSocketThread : public ThreadBase
{
public:
    using Callback = std::function<void(const uint8_t*, ssize_t)>;

// Constructors
public:
    LocalSocketThread() = default;

public:
    /**
     * Starts this thread to begin execution.
     * @param name The name of the address.
     * @param callback The callback to handle the receive data, or nullptr.
     */
    template <typename _Callback>
    void start(const char* name, _Callback&& callback);

    /**
     * Forces this thread to stop executing.
     */
    void stop();

    /**
     * Sends data to a connected socket.
     * @param buf A pointer to a buffer containing the data.
     * @param size The number of bytes of the buffer.
     * @return The total number of bytes sent, -1 otherwise.
     */
    ssize_t send(const void* buf, size_t size) const;

// Implementation
protected:
    /**
     * This thread start entry point.
     */
    void run();

// Data members
protected:
    Callback mCallback;
    LocalSocket mSocket;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the LocalServerThread class
//

class LocalServerThread final : public LocalSocketThread
{
// Constructors
public:
    LocalServerThread() = default;

// Operations
public:
    /**
     * Starts this thread to begin execution.
     * @param name The name of the address.
     * @param callback The callback to handle the receive data, or nullptr.
     */
    template <typename _Callback>
    void start(const char* name, _Callback&& callback);

    /**
     * Forces this thread to stop executing.
     */
    void stop();

// Implementation
private:
    /**
     * This thread start entry point.
     */
    void run();

// Data members
private:
    LocalSocket mServer;
};

}  // namespace stdutil

#include "thrdutil.inl"

#endif  // __THRDUTIL_H__