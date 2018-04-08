///////////////////////////////////////////////////////////////////////////////
// thdutil.inl
//
// Copyright(c) 2018, Garfield. All rights reserved.
// Author  : Garfield
// Version : 1.5
// Creation Date : 2012/8/12

#ifndef __THDUTIL_INL__
#define __THDUTIL_INL__

#ifndef __THDUTIL_H__
    #error thdutil.inl requires thdutil.h to be included first
#endif

__BEGIN_NAMESPACE

///////////////////////////////////////////////////////////////////////////////
// Implementation of the ThreadBase class
//

__INLINE__ ThreadBase::ThreadBase()
    : id(0)
{
}

__INLINE__ ThreadBase::~ThreadBase()
{
    __check_error(Thread_isAlive(), "The thread (id = 0x%08lx) was NOT quit\n", id);
}

__INLINE__ void* ThreadBase::Thread_run()
{
    LOGI("Thread (id = 0x%08lx) is running\n", id);
    return NULL;
}

#if (__ANDROID_API__ >= 9)
__INLINE__ int ThreadBase::Thread_setName(const char* name) const
{
    assert(Thread_isAlive());
    return __android_check_error(::pthread_setname_np(id, name), "Couldn't set thread name (id = 0x%08lx, name = %s)", id, name);
}
#endif  // (__ANDROID_API__ >= 9)

__INLINE__ int ThreadBase::Thread_join(void** retval/* = NULL*/) const
{
    assert_log(::pthread_self() != id, "Thread_join() can NOT call on self thread\n");
    return __android_check_error((Thread_isAlive() ? ::pthread_join(id, retval) : 0), "The thread (id = 0x%08lx) join failed", id);
}

__INLINE__ bool ThreadBase::Thread_isAlive() const
{
    return (id != 0);
}

__INLINE__ int ThreadBase::Thread_getCpuClockId(clockid_t& clockid) const
{
    assert(Thread_isAlive());
    return __android_check_error(::pthread_getcpuclockid(id, &clockid), "Couldn't get cpu clock id (id = 0x%08lx)", id);
}

__INLINE__ int ThreadBase::Thread_getPriority() const
{
    return getThreadPriority();
}

__INLINE__ int ThreadBase::Thread_setPriority(int priority) const
{
    return setThreadPriority(priority);
}

__INLINE__ int ThreadBase::Thread_setSchedParam(int poilcy, const sched_param& param)
{
    assert(Thread_isAlive());
    return __android_check_error(::pthread_setschedparam(id, poilcy, &param), "Couldn't set sched param (id = 0x%08lx, poilcy = %d, priority = %d)", id, poilcy, param.sched_priority);
}

__INLINE__ int ThreadBase::Thread_getSchedParam(int& poilcy, sched_param& param) const
{
    assert(Thread_isAlive());
    return __android_check_error(::pthread_getschedparam(id, &poilcy, &param), "Couldn't get sched param (id = 0x%08lx)", id);
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the ThreadImpl class
//

template <typename T>
__INLINE__ ThreadImpl<T>::ThreadImpl()
{
}

template <typename T>
__INLINE__ int ThreadImpl<T>::Thread_create(const pthread_attr_t* attr/* = NULL*/)
{
    assert(!Thread_isAlive());
    return __android_check_error(::pthread_create(&id, attr, T::Thread_proc, static_cast<T*>(this)), "Couldn't create thread");
}

template <typename T>
__INLINE__ void* ThreadImpl<T>::Thread_proc(void* parameter)
{
    T* _This = reinterpret_cast<T*>(parameter);
    assert(_This);

    void* retval = _This->Thread_run();
    _This->id = 0;  // Clear thread id.

    return retval;
}


#ifdef __STDCOLLS_H__
///////////////////////////////////////////////////////////////////////////////
// Implementation of the Message class
//

__INLINE__ Message::Message(uint32_t msg)
    : message(msg)
{
}

__INLINE__ Message::~Message()
{
}

#ifndef NDEBUG
__INLINE__ void Message::dump() const
{
    LOGI("Message [ message = %u ]\n", message);
}
#endif  // NDEBUG


///////////////////////////////////////////////////////////////////////////////
// Implementation of the MessageComparator class
//

__INLINE__ MessageComparator::MessageComparator(uint32_t msg)
    : message(msg)
{
}

__INLINE__ bool MessageComparator::operator()(const Message* msg) const
{
    return (msg->message == message);
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the MessageQueue class
//

__INLINE__ MessageQueue::MessageQueue()
{
}

__INLINE__ int MessageQueue::next(Message*& msg, uint32_t timeout/* = INFINITE*/)
{
    return mMessages.pop_front(msg, timeout);
}

__INLINE__ void MessageQueue::quit(bool postAtFront/* = true*/)
{
    if (postAtFront)
        mMessages.push_front(NULL);
    else
        mMessages.push_back(NULL);
}

__INLINE__ void MessageQueue::enqueueMessage(Message* msg)
{
    assert_log(msg, "Invalid parameter - The msg is null");
    mMessages.push_back(msg);
}

__INLINE__ void MessageQueue::enqueueMessage(uint32_t message)
{
    mMessages.push_back(new Message(message));
}

__INLINE__ void MessageQueue::enqueueMessageAtFront(Message* msg)
{
    assert_log(msg, "Invalid parameter - The msg is null");
    mMessages.push_front(msg);
}

__INLINE__ void MessageQueue::enqueueMessageAtFront(uint32_t message)
{
    mMessages.push_front(new Message(message));
}

__INLINE__ void MessageQueue::removeAllMessages()
{
    mMessages.clear();
}

__INLINE__ bool MessageQueue::hasMessages(uint32_t message) const
{
    return mMessages.find(MessageComparator(message));
}

__INLINE__ size_t MessageQueue::removeMessages(uint32_t message, bool removeAll/* = true*/)
{
    const MessageComparator comp(message);
    return (removeAll ? mMessages.remove_n(comp) : (size_t)mMessages.remove(comp));
}

__INLINE__ size_t MessageQueue::getMessageCount() const
{
    return mMessages.size();
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the MessageThreadBase class
//

__INLINE__ MessageThreadBase::MessageThreadBase()
{
}

__INLINE__ void MessageThreadBase::MsgThread_sendMessage(Message* msg)
{
    assert(msg);
    mQueue.enqueueMessage(msg);
}

__INLINE__ void MessageThreadBase::MsgThread_sendMessage(uint32_t message)
{
    mQueue.enqueueMessage(message);
}

__INLINE__ void MessageThreadBase::MsgThread_sendMessageAtFront(Message* msg)
{
    assert(msg);
    mQueue.enqueueMessageAtFront(msg);
}

__INLINE__ void MessageThreadBase::MsgThread_sendMessageAtFront(uint32_t message)
{
    mQueue.enqueueMessageAtFront(message);
}

__INLINE__ bool MessageThreadBase::MsgThread_hasMessages(uint32_t message) const
{
    return mQueue.hasMessages(message);
}

__INLINE__ size_t MessageThreadBase::MsgThread_removeMessages(uint32_t message, bool removeAll/* = true*/)
{
    return mQueue.removeMessages(message, removeAll);
}

__INLINE__ MessageQueue* MessageThreadBase::MsgThread_getQueue()
{
    return &mQueue;
}

__INLINE__ const MessageQueue* MessageThreadBase::MsgThread_getQueue() const
{
    return &mQueue;
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the MessageThreadHandle class
//

__INLINE__ MessageThreadHandle::MessageThreadHandle()
    : mQueue(NULL)
{
}

__INLINE__ void MessageThreadHandle::MsgThread_sendMessage(Message* msg)
{
    assert(msg);
    assert(mQueue);

    mQueue->enqueueMessage(msg);
}

__INLINE__ void MessageThreadHandle::MsgThread_sendMessage(uint32_t message)
{
    assert(mQueue);
    mQueue->enqueueMessage(message);
}

__INLINE__ void MessageThreadHandle::MsgThread_sendMessageAtFront(Message* msg)
{
    assert(msg);
    assert(mQueue);

    mQueue->enqueueMessageAtFront(msg);
}

__INLINE__ void MessageThreadHandle::MsgThread_sendMessageAtFront(uint32_t message)
{
    assert(mQueue);
    mQueue->enqueueMessageAtFront(message);
}

__INLINE__ bool MessageThreadHandle::MsgThread_hasMessages(uint32_t message) const
{
    assert(mQueue);
    return mQueue->hasMessages(message);
}

__INLINE__ size_t MessageThreadHandle::MsgThread_removeMessages(uint32_t message, bool removeAll/* = true*/)
{
    assert(mQueue);
    return mQueue->removeMessages(message, removeAll);
}

__INLINE__ MessageQueue* MessageThreadHandle::MsgThread_getQueue()
{
    return mQueue;
}

__INLINE__ const MessageQueue* MessageThreadHandle::MsgThread_getQueue() const
{
    return mQueue;
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the MessageThreadImpl class
//

template <typename T, typename TBase>
__INLINE__ MessageThreadImpl<T, TBase>::MessageThreadImpl(uint32_t aliveTime/* = INFINITE*/)
    : mAliveTime(aliveTime)
{
}

template <typename T, typename TBase>
__INLINE__ void MessageThreadImpl<T, TBase>::MsgThread_beginMessageLoop()
{
    LOGI("MsgThread_beginMessageLoop\n");
}

template <typename T, typename TBase>
__INLINE__ void MessageThreadImpl<T, TBase>::MsgThread_endMessageLoop()
{
    LOGI("MsgThread_endMessageLoop\n");
}

template <typename T, typename TBase>
__INLINE__ bool MessageThreadImpl<T, TBase>::MsgThread_handleMessage(Message* /*msg*/)
{
    // Return true, destroy the msg.
    // Return false, do nothing. The msg parameter can be reuse.
    LOGI("MsgThread_handleMessage\n");
    return true;
}

template <typename T, typename TBase>
__INLINE__ int MessageThreadImpl<T, TBase>::MsgThread_create(MessageQueue* queue, const pthread_attr_t* attr/* = NULL*/)
{
    assert(queue);
    assert(!this->Thread_isAlive());

    this->mQueue = queue;
    return this->Thread_create(attr);
}

template <typename T, typename TBase>
__INLINE__ void MessageThreadImpl<T, TBase>::Thread_quit(bool postAtFront/* = true*/, bool join/* = true*/)
{
    if (this->Thread_isAlive())
    {
        this->mQueue.quit(postAtFront);
        if (join) this->Thread_join();
    }
}

template <typename T, typename TBase>
__INLINE__ void* MessageThreadImpl<T, TBase>::Thread_run()
{
    T* _This = static_cast<T*>(this);
    assert(_This);

    _This->MsgThread_beginMessageLoop();    // Begin message loop.
    _This->MsgThread_runMessageLoop();      // Run message loop.
    _This->MsgThread_endMessageLoop();      // End message loop.

    LOGI("Exit thread run (id = 0x%08lx)\n", _This->id);
    return NULL;
}

template <typename T, typename TBase>
__INLINE__ void MessageThreadImpl<T, TBase>::MsgThread_runMessageLoop()
{
    T* _This = static_cast<T*>(this);
    assert(_This);

    MessageQueue* msgQueue = _This->MsgThread_getQueue();
    assert(msgQueue);

    // Run message loop.
    Message* msg = NULL;

#ifndef NDEBUG
    int errnum = 0;
    while ((errnum = msgQueue->next(msg, mAliveTime)) == 0)     // might block.
#else
    while (msgQueue->next(msg, mAliveTime) == 0)    // might block.
#endif  // NDEBUG
    {
        // Exit message loop, if the msg is NULL.
        if (msg == NULL)
            break;

        // Destroy message, if MsgThread_handleMessage returns true.
        if (_This->MsgThread_handleMessage(msg))
            MessageQueue::MessageTraits::destroy(msg);
    }

#ifndef NDEBUG
    if (errnum == ETIMEDOUT)
    {
        LOGD("Get message from message queue time out, timeout = %u\n", mAliveTime);
    }
    else if (errnum != 0)
    {
        char error[MAX_PATH];
        ::strerror_r(errnum, error, _countof(error));
        LOGE("Couldn't obtain message from message queue, errno = %d, error = %s\n", errnum, error);
    }
#endif  // NDEBUG
}
#endif  // __STDCOLLS_H__

__END_NAMESPACE

#endif  // __THDUTIL_INL__