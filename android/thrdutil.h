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

__BEGIN_NAMESPACE

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
// Interface of the LooperThread class
//

class LooperThread
{
public:
    using Runnable = std::function<void()>;

// Constructors/Destructor
public:
    LooperThread();
    ~LooperThread();

// Operations
public:
    void start();
    void stop();

    template <typename _Callable>
    void post(_Callable&& callable, uint32_t delayMillis = 0);
    void post(std::nullptr_t, uint32_t delayMillis = 0);

// Implementation
private:
    class Task;
    void run();
    void nextTask(Task& outTask);

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

class LooperThread::Task final
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
    friend class LooperThread;
    bool operator>(const Task& right) const;
    int duration(const TimePoint& now) const;

// Data members
private:
    TimePoint mWhen;
    Runnable mRunnable;
};

__END_NAMESPACE

#include "thrdutil.inl"

#endif  // __THRDUTIL_H__