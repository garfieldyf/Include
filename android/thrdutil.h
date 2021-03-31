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

__BEGIN_NAMESPACE

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
     * Posts a callable to the end of the task queue. The 
     * callable will be run on this thread.
     * @param callable The callable that will be executed.
     */
    template <typename _Callable>
    void post(_Callable&& callable);

    /**
     * Posts a callable to the beginning of the task queue. 
     * The runnable will be run on this thread.
     * @param callable The callable that will be executed.
     */
    template <typename _Callable>
    void postAtFront(_Callable&& callable);

// Implementation
private:
    void run();

    using Runnable  = std::function<void()>;
    using TaskQueue = stdutil::blocking_deque<Runnable>;

// Data members
private:
    std::thread mThread;
    TaskQueue mTaskQueue;
    std::atomic_bool mRunning;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the Epoll  class
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
    int open();
    void close();

    void notify();
    void wait(int timeout);

// Data members
private:
    int mEpollFd;
    int mEventFd;
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
     * Posts a callable to the task queue, to be run after 
     * the specified amount of time elapses.
     * @param callable The callable that will be executed.
     * @param delayMillis The delay in milliseconds until
     * the callable will be executed.
     */
    template <typename _Callable>
    void post(_Callable&& callable, uint32_t delayMillis = 0);

// Implementation
private:
    class Task;
    void run();
    int nextTask(Task& outTask);

    using Runnable  = std::function<void()>;
    using MutexLock = std::lock_guard<std::mutex>;
    using TimePoint = std::chrono::steady_clock::time_point;
    using TaskQueue = stdutil::priority_queue<Task, std::greater<Task>>;

// Data members
private:
    Epoll mEpoll;
    std::mutex mMutex;
    std::thread mThread;
    TaskQueue mTaskQueue;
    std::atomic_bool mRunning;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the Task class
//

class HandlerThread::Task final
{
// Constructors
public:
    Task() = default;
    Task(Runnable&& runnable, uint32_t delayMillis);

    Task(Task&&) = default;
    Task& operator=(Task&&) = default;

    Task(const Task&) = delete;
    Task& operator=(const Task&) = delete;

// Operations
public:
    friend class HandlerThread;
    bool operator>(const Task& right) const;
    int getTimeout(const TimePoint& now) const;

// Data members
private:
    TimePoint mWhen;
    Runnable mRunnable;
};

__END_NAMESPACE

#include "thrdutil.inl"

#endif  // __THRDUTIL_H__