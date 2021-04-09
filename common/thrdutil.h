///////////////////////////////////////////////////////////////////////////////
// thrdutil.h
//
// Author : Garfield
// Creation Date : 2020/4/20

#ifndef __THRDUTIL_H__
#define __THRDUTIL_H__

#include "platform.h"
#include "stdcolls.h"
#include <thread>
#include <sys/epoll.h>
#include <sys/eventfd.h>

///////////////////////////////////////////////////////////////////////////////
// Classes in this file:
//
// TaskThread
// LooperThread

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

// Data members
protected:
    std::thread mThread;
    std::atomic_bool mRunning;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the TaskThread class
//

class TaskThread final : public ThreadBase
{
// Constructors
public:
    TaskThread() = default;

// Operations
public:
    /**
     * Starts this thread to begin execution.
     */
    void start();

    /**
     * Forces this thread to stop executing.
     */
    void stop();

    /**
     * Posts a callable to the end of the task queue. The callable will be run 
     * on this thread.
     * @param callable The callable that will be executed, Maybe a pointer to
     * function, pointer to member function, lambda expression, or any kind of
     * move-constructible function object.
     * @return Returns true if the callable was successfully added into the task
     * queue. Returns false on failure, usually because this thread was exited.
     */
    template <typename _Callable>
    bool post(_Callable&& callable);

    /**
     * Posts a callable to the beginning of the task queue. The callable will be 
     * run on this thread.
     * @param callable The callable that will be executed, Maybe a pointer to
     * function, pointer to member function, lambda expression, or any kind of
     * move-constructible function object.
     * @return Returns true if the callable was successfully added into the task
     * queue. Returns false on failure, usually because this thread was exited.
     */
    template <typename _Callable>
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
// Interface of the Epoll class
//

class Epoll final
{
// Constructors/Destructor
public:
    Epoll();
    ~Epoll();

    Epoll(const Epoll&) = delete;
    Epoll& operator=(const Epoll&) = delete;

// Operations
public:
    /**
     * Opens a new epoll file descriptor.
     * @return returns 0 if opens successful, -1 otherwise.
     */
    int open();

    /**
     * Closes this epoll file descriptor.
     */
    void close();

    /**
     * Unblocks a thread currently waiting for this epoll.
     */
    void notify();

    /**
     * Blocks a thread, and sets a timeout after which the thread unblocks.
     * @param timeout The waiting timeout in milliseconds, -1 causes wait to 
     * indefinitely.
     */
    void wait(int timeout);

// Data members
private:
    int mEpollFd;
    int mEventFd;
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
     * Forces this thread to stop executing.
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
     * @return Returns true if the callable was successfully added in to the task
     * queue. Returns false on failure, usually because this thread was exited.
     */
    template <typename _Callable>
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
     * @return The waiting timeout in milliseconds.
     */
    int nextTask(Task& outTask);

    using Runnable  = std::function<void()>;
    using MutexLock = std::lock_guard<std::mutex>;
    using TimePoint = std::chrono::steady_clock::time_point;
    using TaskQueue = priority_queue<Task, std::greater<Task>>;

// Data members
private:
    Epoll mEpoll;
    std::mutex mMutex;
    TaskQueue mTaskQueue;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the LooperThread::Task class
//

class LooperThread::Task final
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

}  // namespace stdutil

#include "thrdutil.inl"

#endif  // __THRDUTIL_H__