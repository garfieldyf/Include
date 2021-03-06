///////////////////////////////////////////////////////////////////////////////
// sysutil.h
//
// Author : Garfield
// Creation Date : 2011/6/23

#ifndef __SYSUTILS_H__
#define __SYSUTILS_H__

#include "stdutil.h"
#include <pthread.h>
#include <sys/syscall.h>
#include <sys/resource.h>

///////////////////////////////////////////////////////////////////////////////
// Classes in this file:
//
// Mutex
// MutexLock
// RWLock
// ReadLock
// WriteLock
// Condition
// ThreadAttr
// ThreadKey<_Ty>
//
// Global functions in this file:
//
// initOnce()
// getThreadId()
// getThreadPriority()
// setThreadPriority()

__BEGIN_NAMESPACE

///////////////////////////////////////////////////////////////////////////////
// Interface of the Mutex class
//

class Mutex : public pthread_mutex_t
{
    DECLARE_NONCOPYABLE(Mutex);

// Constructors/Destructor
public:
    explicit Mutex(int pshared = PTHREAD_PROCESS_PRIVATE, int type = PTHREAD_MUTEX_NORMAL);
    ~Mutex();

// Operations
public:
    int lock(unsigned timeout = INFINITE);
    int unlock();
    int tryLock();

    operator pthread_mutex_t*();
    operator const pthread_mutex_t*() const;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the MutexLock class
//

class MutexLock
{
    DECLARE_NONCOPYABLE(MutexLock);

// Constructors/Destructor
public:
    explicit MutexLock(Mutex& mutex);
    ~MutexLock();

// Data members
private:
    Mutex& mMutex;
};


#if (__ANDROID_API__ >= 9)
///////////////////////////////////////////////////////////////////////////////
// Interface of the RWLock class
//

class RWLock : public pthread_rwlock_t
{
    DECLARE_NONCOPYABLE(RWLock);

// Constructors/Destructor
public:
    explicit RWLock(int pshared = PTHREAD_PROCESS_PRIVATE);
    ~RWLock();

// Operations
public:
    int readLock(const timespec& abstime);
    int readLock(unsigned timeout = INFINITE);

    int writeLock(const timespec& abstime);
    int writeLock(unsigned timeout = INFINITE);

    int unlock();
    int tryReadLock();
    int tryWriteLock();

    operator pthread_rwlock_t*();
    operator const pthread_rwlock_t*() const;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the RWAutoLock class
//

class ATL_NO_VTABLE RWAutoLock
{
    DECLARE_NONCOPYABLE(RWAutoLock);

// Constructors/Destructor
protected:
    explicit RWAutoLock(RWLock& rwlock);
    ~RWAutoLock();

// Data members
protected:
    RWLock& mRWLock;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the ReadLock class
//

class ReadLock : public RWAutoLock
{
// Constructors
public:
    explicit ReadLock(RWLock& rwlock);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the WriteLock class
//

class WriteLock : public RWAutoLock
{
// Constructors
public:
    explicit WriteLock(RWLock& rwlock);
};
#endif  // (__ANDROID_API__ >= 9)


///////////////////////////////////////////////////////////////////////////////
// Interface of the Condition class
//

class Condition : public pthread_cond_t
{
    DECLARE_NONCOPYABLE(Condition);

// Constructors/Destructor
public:
    explicit Condition(int pshared = PTHREAD_PROCESS_PRIVATE);
    ~Condition();

// Operations
public:
    operator pthread_cond_t*();
    operator const pthread_cond_t*() const;

    int notify();
    int notifyAll();
    int wait(pthread_mutex_t& mutex, unsigned timeout = INFINITE);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the ThreadAttr class
//

class ThreadAttr : public pthread_attr_t
{
    DECLARE_NONCOPYABLE(ThreadAttr);

// Constructors/Destructor
public:
    ThreadAttr();
    explicit ThreadAttr(int scope, int detachstate = PTHREAD_CREATE_JOINABLE);
    ~ThreadAttr();

// Operations
public:
    int get(pthread_t tid);

    operator pthread_attr_t*();
    operator const pthread_attr_t*() const;

#ifndef NDEBUG
    void dump() const;
#endif

// Attributes
public:
    int getScope() const;
    int setScope(int scope);

    int setGuardSize(size_t size);
    int getGuardSize(size_t& size) const;

    int setDetachState(int state);
    int getDetachState(int& state) const;

    int setSchedPolicy(int policy);
    int getSchedPolicy(int& policy) const;

    int setSchedParam(const sched_param& param);
    int getSchedParam(sched_param& param) const;

    int setStackSize(size_t size);
    int getStackSize(size_t& size) const;

    int setStack(void* addr, size_t size);
    int getStack(void*& addr, size_t& size) const;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the ThreadKeyBase class
//

class ATL_NO_VTABLE ThreadKeyBase
{
    DECLARE_NONCOPYABLE(ThreadKeyBase);

// Constructors/Destructor
protected:
    constexpr ThreadKeyBase();
    ~ThreadKeyBase();

// Operations
public:
    int create(void (*destroy)(void*) = NULL);
    void destroy();

// Data members
protected:
    pthread_key_t mKey;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the ThreadKey class
//

template <typename _Ty>
class ThreadKey : public ThreadKeyBase
{
// Attributes
public:
    _Ty getValue() const;
    int setValue(const _Ty value) const;
};


///////////////////////////////////////////////////////////////////////////////
// Global functions
//

__STATIC_INLINE__ int initOnce(pthread_once_t& control, void (*initialize)())
{
    return __verify(::pthread_once(&control, initialize), "Call pthread_once failed");
}

__STATIC_INLINE__ int getThreadId()
{
    return ::syscall(__NR_gettid);
}

__STATIC_INLINE__ int getThreadPriority(int tid)
{
#ifndef NDEBUG
    errno = 0;
    const int priority = ::getpriority(PRIO_PROCESS, tid);
    __verify(errno, "Couldn't get thread priority (tid = %d)", tid);
    return priority;
#else
    return ::getpriority(PRIO_PROCESS, tid);
#endif  // NDEBUG
}

__STATIC_INLINE__ int getThreadPriority()
{
    return getThreadPriority(getThreadId());
}

__STATIC_INLINE__ int setThreadPriority(int tid, int priority)
{
    return __verify(::setpriority(PRIO_PROCESS, tid, priority), "Couldn't set thread priority (tid = %d, priority = %d)", tid, priority);
}

__STATIC_INLINE__ int setThreadPriority(int priority)
{
    return setThreadPriority(getThreadId(), priority);
}

__END_NAMESPACE

#include "sysutil.inl"

#endif  // __SYSUTILS_H__