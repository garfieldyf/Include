///////////////////////////////////////////////////////////////////////////////
// sysutil.inl
//
// Copyright(c) 2018, Garfield. All rights reserved.
// Author  : Garfield
// Version : 1.0
// Creation Date : 2011/6/23

#ifndef __SYSUTILS_INL__
#define __SYSUTILS_INL__

#ifndef __SYSUTILS_H__
    #error sysutil.inl requires sysutil.h to be included first
#endif

__BEGIN_NAMESPACE

///////////////////////////////////////////////////////////////////////////////
// Implementation of the Mutex class
//

__INLINE__ Mutex::Mutex(pthread_mutexattr_t* attr/* = NULL*/)
{
    verify(::pthread_mutex_init(this, attr), 0);
}

__INLINE__ Mutex::Mutex(int pshared, int type)
{
    MutexAttr attr(pshared, type);
    verify(::pthread_mutex_init(this, attr), 0);
}

__INLINE__ Mutex::~Mutex()
{
    verify(::pthread_mutex_destroy(this), 0);
    LOGI("The mutex destroy (mutex = %p)\n", this);
}

__INLINE__ int Mutex::lock(unsigned timeout/* = INFINITE*/)
{
#ifdef __LP64__
    return (timeout == INFINITE ? __android_check_error(::pthread_mutex_lock(this), "Couldn't lock mutex") : ::pthread_mutex_timedlock(this, timespec_t::toTimespec(timeout)));
#else
    return (timeout == INFINITE ? __android_check_error(::pthread_mutex_lock(this), "Couldn't lock mutex") : ::pthread_mutex_lock_timeout_np(this, timeout));
#endif  // __LP64__
}

__INLINE__ int Mutex::unlock()
{
    return __android_check_error(::pthread_mutex_unlock(this), "Couldn't unlock mutex");
}

__INLINE__ int Mutex::tryLock()
{
    return ::pthread_mutex_trylock(this);
}

__INLINE__ Mutex::operator pthread_mutex_t*()
{
    return this;
}

__INLINE__ Mutex::operator const pthread_mutex_t*() const
{
    return this;
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the MutexLock class
//

__INLINE__ MutexLock::MutexLock(Mutex& mutex)
    : mMutex(mutex)
{
    mMutex.lock();
}

__INLINE__ MutexLock::~MutexLock()
{
    mMutex.unlock();
}


#if (__ANDROID_API__ >= 9)
///////////////////////////////////////////////////////////////////////////////
// Implementation of the RWLock class
//

__INLINE__ RWLock::RWLock(const pthread_rwlockattr_t* attr/* = NULL*/)
{
    verify(::pthread_rwlock_init(this, attr), 0);
}

__INLINE__ RWLock::RWLock(int pshared)
{
    RWLockAttr attr(pshared);
    verify(::pthread_rwlock_init(this, attr), 0);
}

__INLINE__ RWLock::~RWLock()
{
    verify(::pthread_rwlock_destroy(this), 0);
    LOGI("The RWLock destroy (rwlock = %p)\n", this);
}

__INLINE__ int RWLock::readLock(const timespec& abstime)
{
    return ::pthread_rwlock_timedrdlock(this, &abstime);
}

__INLINE__ int RWLock::readLock(unsigned timeout/* = INFINITE*/)
{
    return (timeout == INFINITE ? __android_check_error(::pthread_rwlock_rdlock(this), "Couldn't lock to read") : ::pthread_rwlock_timedrdlock(this, timespec_t::toTimespec(timeout)));
}

__INLINE__ int RWLock::writeLock(const timespec& abstime)
{
    return ::pthread_rwlock_timedwrlock(this, &abstime);
}

__INLINE__ int RWLock::writeLock(unsigned timeout/* = INFINITE*/)
{
    return (timeout == INFINITE ? __android_check_error(::pthread_rwlock_wrlock(this), "Couldn't lock to write") : ::pthread_rwlock_timedwrlock(this, timespec_t::toTimespec(timeout)));
}

__INLINE__ int RWLock::unlock()
{
    return __android_check_error(::pthread_rwlock_unlock(this), "Couldn't unlock RWLock");
}

__INLINE__ int RWLock::tryReadLock()
{
    return ::pthread_rwlock_tryrdlock(this);
}

__INLINE__ int RWLock::tryWriteLock()
{
    return ::pthread_rwlock_trywrlock(this);
}

__INLINE__ RWLock::operator pthread_rwlock_t*()
{
    return this;
}

__INLINE__ RWLock::operator const pthread_rwlock_t*() const
{
    return this;
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the RWAutoLock class
//

__INLINE__ RWAutoLock::RWAutoLock(RWLock& rwlock)
    : mRWLock(rwlock)
{
}

__INLINE__ RWAutoLock::~RWAutoLock()
{
    mRWLock.unlock();
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the ReadLock class
//

__INLINE__ ReadLock::ReadLock(RWLock& rwlock)
    : RWAutoLock(rwlock)
{
    mRWLock.readLock();
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the WriteLock class
//

__INLINE__ WriteLock::WriteLock(RWLock& rwlock)
    : RWAutoLock(rwlock)
{
    mRWLock.writeLock();
}
#endif  // (__ANDROID_API__ >= 9)


///////////////////////////////////////////////////////////////////////////////
// Implementation of the Condition class
//

__INLINE__ Condition::Condition(pthread_condattr_t* attr/* = NULL*/)
{
    verify(::pthread_cond_init(this, attr), 0);
}

__INLINE__ Condition::Condition(int pshared)
{
    CondAttr attr(pshared);
    verify(::pthread_cond_init(this, attr), 0);
}

__INLINE__ Condition::~Condition()
{
    verify(::pthread_cond_destroy(this), 0);
    LOGI("The condition destroy (condition = %p)\n", this);
}

__INLINE__ Condition::operator pthread_cond_t*()
{
    return this;
}

__INLINE__ Condition::operator const pthread_cond_t*() const
{
    return this;
}

__INLINE__ int Condition::notify()
{
    return __android_check_error(::pthread_cond_signal(this), "Condition couldn't notify");
}

__INLINE__ int Condition::notifyAll()
{
    return __android_check_error(::pthread_cond_broadcast(this), "Condition couldn't notify all");
}

__INLINE__ int Condition::wait(pthread_mutex_t& mutex, unsigned timeout/* = INFINITE*/)
{
#ifdef __LP64__
    return (timeout == INFINITE ? __android_check_error(::pthread_cond_wait(this, &mutex), "Couldn't wait condition") : ::pthread_cond_timedwait(this, &mutex, timespec_t::toTimespec(timeout)));
#else
    return (timeout == INFINITE ? __android_check_error(::pthread_cond_wait(this, &mutex), "Couldn't wait condition") : ::pthread_cond_timeout_np(this, &mutex, timeout));
#endif  // __LP64__
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CondAttr class
//

__INLINE__ CondAttr::CondAttr(int pshared/* = PTHREAD_PROCESS_PRIVATE*/)
{
    verify(::pthread_condattr_init(&mAttr), 0);
    verify(setShared(pshared), 0);
}

__INLINE__ CondAttr::~CondAttr()
{
    verify(::pthread_condattr_destroy(&mAttr), 0);
}

__INLINE__ CondAttr::operator pthread_condattr_t*()
{
    return &mAttr;
}

__INLINE__ CondAttr::operator const pthread_condattr_t*() const
{
    return &mAttr;
}

#ifndef NDEBUG
__INLINE__ void CondAttr::dump() const
{
    int pshared = -1;
    verify(getShared(pshared), 0);
    LOGI("CondAttr [ pshared = %d ]\n", pshared);
}
#endif  // NDEBUG

__INLINE__ int CondAttr::setShared(int pshared)
{
    return __android_check_error(::pthread_condattr_setpshared(&mAttr, pshared), "Couldn't set condition pshared (pshared = %d)", pshared);
}

__INLINE__ int CondAttr::getShared(int& pshared) const
{
    return __android_check_error(::pthread_condattr_getpshared(&mAttr, &pshared), "Couldn't get condition pshared");
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the MutexAttr class
//

__INLINE__ MutexAttr::MutexAttr(int pshared/* = PTHREAD_PROCESS_PRIVATE*/, int type/* = PTHREAD_MUTEX_NORMAL*/)
{
    verify(::pthread_mutexattr_init(&mAttr), 0);
    verify(setType(type), 0);
    verify(setShared(pshared), 0);
}

__INLINE__ MutexAttr::~MutexAttr()
{
    verify(::pthread_mutexattr_destroy(&mAttr), 0);
}

__INLINE__ MutexAttr::operator pthread_mutexattr_t*()
{
    return &mAttr;
}

__INLINE__ MutexAttr::operator const pthread_mutexattr_t*() const
{
    return &mAttr;
}

#ifndef NDEBUG
__INLINE__ void MutexAttr::dump() const
{
    int type = -1, pshared = -1;
    verify(getType(type), 0);
    verify(getShared(pshared), 0);

    LOGI("MutexAttr [ type = %d, pshared = %d ]\n", type, pshared);
}
#endif  // NDEBUG

__INLINE__ int MutexAttr::setType(int type)
{
    return __android_check_error(::pthread_mutexattr_settype(&mAttr, type), "Couldn't set mutex type (type = %d)", type);
}

__INLINE__ int MutexAttr::getType(int& type) const
{
    return __android_check_error(::pthread_mutexattr_gettype(&mAttr, &type), "Couldn't get mutex type");
}

__INLINE__ int MutexAttr::setShared(int pshared)
{
    return __android_check_error(::pthread_mutexattr_setpshared(&mAttr, pshared), "Couldn't set mutex pshared (pshared = %d)", pshared);
}

__INLINE__ int MutexAttr::getShared(int& pshared) const
{
    return __android_check_error(::pthread_mutexattr_getpshared(&mAttr, &pshared), "Couldn't get mutex pshared");
}


#if (__ANDROID_API__ >= 9)
///////////////////////////////////////////////////////////////////////////////
// Implementation of the RWLockAttr class
//

__INLINE__ RWLockAttr::RWLockAttr(int pshared/* = PTHREAD_PROCESS_PRIVATE*/)
{
    verify(::pthread_rwlockattr_init(&mAttr), 0);
    verify(setShared(pshared), 0);
}

__INLINE__ RWLockAttr::~RWLockAttr()
{
    verify(::pthread_rwlockattr_destroy(&mAttr), 0);
}

__INLINE__ RWLockAttr::operator pthread_rwlockattr_t*()
{
    return &mAttr;
}

__INLINE__ RWLockAttr::operator const pthread_rwlockattr_t*() const
{
    return &mAttr;
}

#ifndef NDEBUG
__INLINE__ void RWLockAttr::dump() const
{
    int pshared = -1;
    verify(getShared(pshared), 0);

    LOGI("RWLockAttr [ pshared = %d ]\n", pshared);
}
#endif  // NDEBUG

__INLINE__ int RWLockAttr::setShared(int pshared)
{
    return __android_check_error(::pthread_rwlockattr_setpshared(&mAttr, pshared), "Couldn't set RWLock pshared (pshared = %d)", pshared);
}

__INLINE__ int RWLockAttr::getShared(int& pshared) const
{
    return __android_check_error(::pthread_rwlockattr_getpshared(&mAttr, &pshared), "Couldn't get RWLock pshared");
}
#endif  // (__ANDROID_API__ >= 9)


///////////////////////////////////////////////////////////////////////////////
// Implementation of the ThreadAttr class
//

__INLINE__ ThreadAttr::ThreadAttr()
{
}

__INLINE__ ThreadAttr::ThreadAttr(int scope, int detachstate/* = PTHREAD_CREATE_JOINABLE*/)
{
    verify(::pthread_attr_init(this), 0);
    verify(setScope(scope), 0);
    verify(setDetachState(detachstate), 0);
}

__INLINE__ ThreadAttr::~ThreadAttr()
{
    verify(::pthread_attr_destroy(this), 0);
}

__INLINE__ ThreadAttr::operator pthread_attr_t*()
{
    return this;
}

__INLINE__ ThreadAttr::operator const pthread_attr_t*() const
{
    return this;
}

__INLINE__ int ThreadAttr::get(pthread_t tid)
{
    return __android_check_error(::pthread_getattr_np(tid, this), "Couldn't get thread (tid = 0x%08lx) attr", tid);
}

#ifndef NDEBUG
__INLINE__ void ThreadAttr::dump() const
{
    void* stackaddr = NULL;
    struct sched_param param;
    size_t guardsize = 0, stacksize = 0;
    int detachstate = 0, schedpolicy = 0;

    verify(getSchedParam(param), 0);
    verify(getGuardSize(guardsize), 0);
    verify(getDetachState(detachstate), 0);
    verify(getSchedPolicy(schedpolicy), 0);
    verify(getStack(stackaddr, stacksize), 0);
    LOGI("ThreadAttr [ scope = %d, stack addr = %p, stack size = " _PRId ", guard size = " _PRId ", detach state = %d, sched policy = %d, sched priority = %d ]\n",
         getScope(), stackaddr, stacksize, guardsize, detachstate, schedpolicy, param.sched_priority);
}
#endif  // NDEBUG

__INLINE__ int ThreadAttr::getScope() const
{
#ifdef __LP64__
    int scope = -1;
    verify(::pthread_attr_getscope(this, &scope), 0);
    return scope;
#else
    return ::pthread_attr_getscope(this);
#endif  // __LP64__
}

__INLINE__ int ThreadAttr::setScope(int scope)
{
    return __android_check_error(::pthread_attr_setscope(this, scope), "Couldn't set thread scope (scope = %d)", scope);
}

__INLINE__ int ThreadAttr::setGuardSize(size_t size)
{
    return __android_check_error(::pthread_attr_setguardsize(this, size), "Couldn't set thread guard size (size = " _PRId ")", size);
}

__INLINE__ int ThreadAttr::getGuardSize(size_t& size) const
{
    return __android_check_error(::pthread_attr_getguardsize(this, &size), "Couldn't get thread guard size");
}

__INLINE__ int ThreadAttr::setDetachState(int state)
{
    return __android_check_error(::pthread_attr_setdetachstate(this, state), "Couldn't set thread detach state (state = %d)", state);
}

__INLINE__ int ThreadAttr::getDetachState(int& state) const
{
    return __android_check_error(::pthread_attr_getdetachstate(this, &state), "Couldn't get thread detach state");
}

__INLINE__ int ThreadAttr::setSchedPolicy(int policy)
{
    return __android_check_error(::pthread_attr_setschedpolicy(this, policy), "Couldn't set thread schedpolicy (policy = %d)", policy);
}

__INLINE__ int ThreadAttr::getSchedPolicy(int& policy) const
{
    return __android_check_error(::pthread_attr_getschedpolicy(this, &policy), "Couldn't get thread schedpolicy");
}

__INLINE__ int ThreadAttr::setSchedParam(const sched_param& param)
{
    return __android_check_error(::pthread_attr_setschedparam(this, &param), "Couldn't set thread sched param");
}

__INLINE__ int ThreadAttr::getSchedParam(sched_param& param) const
{
    return __android_check_error(::pthread_attr_getschedparam(this, &param), "Couldn't get thread sched param");
}

__INLINE__ int ThreadAttr::setStackSize(size_t size)
{
    return __android_check_error(::pthread_attr_setstacksize(this, size), "Couldn't set thread stack size (size = " _PRId ")", size);
}

__INLINE__ int ThreadAttr::getStackSize(size_t& size) const
{
    return __android_check_error(::pthread_attr_getstacksize(this, &size), "Couldn't get thread stack size");
}

__INLINE__ int ThreadAttr::setStack(void* addr, size_t size)
{
    return __android_check_error(::pthread_attr_setstack(this, addr, size), "Couldn't set thread stack info (addr = %p, size = " _PRId ")", addr, size);
}

__INLINE__ int ThreadAttr::getStack(void*& addr, size_t& size) const
{
    return __android_check_error(::pthread_attr_getstack(this, &addr, &size), "Couldn't get thread stack info");
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the ThreadKeyBase class
//

__INLINE__ ThreadKeyBase::ThreadKeyBase()
    : mKey(-1)
{
}

__INLINE__ ThreadKeyBase::~ThreadKeyBase()
{
    destroy();
}

__INLINE__ int ThreadKeyBase::create(void (*destroy)(void*)/* = NULL*/)
{
    return __android_check_error(::pthread_key_create(&mKey, destroy), "Couldn't create thread-local storage key");
}

__INLINE__ void ThreadKeyBase::destroy()
{
    if (mKey != -1 && ::pthread_key_delete(mKey) == 0)
    {
        LOGI("The thread-local storage key was deleted (mKey = %d)\n", mKey);
        mKey = -1;
    }
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the ThreadKey class
//

template <typename _Ty>
__INLINE__ _Ty ThreadKey<_Ty>::getValue() const
{
    return (_Ty)::pthread_getspecific(mKey);
}

template <typename _Ty>
__INLINE__ int ThreadKey<_Ty>::setValue(const _Ty value) const
{
    return __android_check_error(::pthread_setspecific(mKey, (const void*)value), "Couldn't set thread-local storage value");
}

__END_NAMESPACE

#endif  // __SYSUTILS_INL__