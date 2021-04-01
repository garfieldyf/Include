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
// LooperThread
// HandlerThread

namespace stdutil {

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

class LooperThread final
{
// Constructors/Destructor
public:
    LooperThread();
    ~LooperThread();

    LooperThread(const LooperThread&) = delete;
    LooperThread& operator=(const LooperThread&) = delete;

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
     * @param callable The callable that will be executed.
     * @return Returns true if the callable was successfully added into the task
     * queue. Returns false on failure, usually because this thread is exiting.
     */
    template <typename _Callable>
    bool post(_Callable&& callable);

    /**
     * Posts a callable to the beginning of the task queue. The callable will be 
     * run on this thread.
     * @param callable The callable that will be executed.
     * @return Returns true if the callable was successfully added into the task
     * queue. Returns false on failure, usually because this thread is exiting.
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
    std::thread mThread;
    TaskQueue mTaskQueue;
    std::atomic_bool mRunning;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the HandlerThread class
//

class HandlerThread final
{
// Constructors/Destructor
public:
    HandlerThread();
    ~HandlerThread();

    HandlerThread(const HandlerThread&) = delete;
    HandlerThread& operator=(const HandlerThread&) = delete;

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
     * @param callable The callable that will be executed.
     * @param delayMillis The delay in milliseconds until the callable will be
     * executed.
     * @return Returns true if the callable was successfully added in to the task
     * queue. Returns false on failure, usually because this thread is exiting.
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
    std::thread mThread;
    TaskQueue mTaskQueue;
    std::atomic_bool mRunning;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the HandlerThread::Task class
//

class HandlerThread::Task final
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
    friend class HandlerThread;

    /**
     * Tests if this task is greater than right.
     * @return true if this task is greater than
     * right, false otherwise.
     */
    bool operator>(const Task& right) const;

    /**
     * Returns the wake up time in milliseconds since now.
     * @param now The current time of the std::steady_clock.
     * @return The wake up time in milliseconds.
     */
    int getWakeupTime(const TimePoint& now) const;

// Data members
private:
    TimePoint mWhen;
    Runnable mRunnable;
};

}  // namespace stdutil

#include "thrdutil.inl"

#endif  // __THRDUTIL_H__