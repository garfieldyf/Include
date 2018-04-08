///////////////////////////////////////////////////////////////////////////////
// thdutil.h
//
// Copyright(c) 2018, Garfield. All rights reserved.
// Author  : Garfield
// Version : 1.5
// Creation Date : 2012/8/12

#ifndef __THDUTIL_H__
#define __THDUTIL_H__

#include "sysutil.h"

///////////////////////////////////////////////////////////////////////////////
// Classes in this file:
//
// ThreadImpl<T>
// Message
// MessageQueue
// MessageThreadBase
// MessageThreadHandle
// MessageThreadImpl<T, TBase>

__BEGIN_NAMESPACE

///////////////////////////////////////////////////////////////////////////////
// Interface of the ThreadBase class
//

class ATL_NO_VTABLE ThreadBase
{
    DECLARE_NONCOPYABLE(ThreadBase);

// Constructors/Destructor
protected:
    ThreadBase();
    ~ThreadBase();

// Overridables
public:
    void* Thread_run();

// Operations
public:
#if (__ANDROID_API__ >= 9)
    int Thread_setName(const char* name) const;
#endif
    int Thread_join(void** retval = NULL) const;

// Attributes
public:
    bool Thread_isAlive() const;
    int Thread_getCpuClockId(clockid_t& clockid) const;

    int Thread_getPriority() const;
    int Thread_setPriority(int priority) const;

    int Thread_setSchedParam(int poilcy, const sched_param& param);
    int Thread_getSchedParam(int& poilcy, sched_param& param) const;

// Data members
public:
    pthread_t id;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the ThreadImpl class
//

template <typename T>
class ATL_NO_VTABLE ThreadImpl : public ThreadBase
{
// Constructors
protected:
    ThreadImpl();

// Operations
public:
    int Thread_create(const pthread_attr_t* attr = NULL);

// Implementation
protected:
    static void* Thread_proc(void* parameter);
};


#ifdef __STDCOLLS_H__
///////////////////////////////////////////////////////////////////////////////
// Interface of the Message class
//

class Message
{
// Constructors/Destructor
public:
    explicit Message(uint32_t msg);
    virtual ~Message();

// Operations
public:
#ifndef NDEBUG
    virtual void dump() const;
#endif

// Data members
public:
    uint32_t message;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the MessageComparator class
//

class MessageComparator
{
// Constructors
public:
    explicit MessageComparator(uint32_t msg);

// Operations
public:
    bool operator()(const Message* msg) const;

// Data members
private:
    const uint32_t message;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the MessageQueue class
//

class MessageQueue
{
    DECLARE_NONCOPYABLE(MessageQueue);

// Constructors
public:
    MessageQueue();

public:
    typedef stdutil::pointer_traits<Message> MessageTraits;

// Operations
public:
    int next(Message*& msg, uint32_t timeout = INFINITE);
    void quit(bool postAtFront = true);

    void enqueueMessage(Message* msg);
    void enqueueMessage(uint32_t message);

    void enqueueMessageAtFront(Message* msg);
    void enqueueMessageAtFront(uint32_t message);

    void removeAllMessages();
    bool hasMessages(uint32_t message) const;
    size_t removeMessages(uint32_t message, bool removeAll = true);

// Attributes
public:
    size_t getMessageCount() const;

// Data members
private:
    stdutil::blocking_deque<Message*, MessageTraits> mMessages;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the MessageThreadBase class
//

class ATL_NO_VTABLE MessageThreadBase
{
// Constructors
protected:
    MessageThreadBase();

// Operations
public:
    void MsgThread_sendMessage(Message* msg);
    void MsgThread_sendMessage(uint32_t message);

    void MsgThread_sendMessageAtFront(Message* msg);
    void MsgThread_sendMessageAtFront(uint32_t message);

    bool MsgThread_hasMessages(uint32_t message) const;
    size_t MsgThread_removeMessages(uint32_t message, bool removeAll = true);

// Attributes
public:
    MessageQueue* MsgThread_getQueue();
    const MessageQueue* MsgThread_getQueue() const;

// Data members
protected:
    MessageQueue mQueue;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the MessageThreadHandle class
//

class ATL_NO_VTABLE MessageThreadHandle
{
// Constructors
protected:
    MessageThreadHandle();

// Operations
public:
    void MsgThread_sendMessage(Message* msg);
    void MsgThread_sendMessage(uint32_t message);

    void MsgThread_sendMessageAtFront(Message* msg);
    void MsgThread_sendMessageAtFront(uint32_t message);

    bool MsgThread_hasMessages(uint32_t message) const;
    size_t MsgThread_removeMessages(uint32_t message, bool removeAll = true);

// Attributes
public:
    MessageQueue* MsgThread_getQueue();
    const MessageQueue* MsgThread_getQueue() const;

// Data members
protected:
    MessageQueue* mQueue;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the MessageThreadImpl class
//

template <typename T, typename TBase = MessageThreadBase>
class ATL_NO_VTABLE MessageThreadImpl : public ThreadImpl<T>, public TBase
{
// Constructors
protected:
    MessageThreadImpl(uint32_t aliveTime = INFINITE);

// Overridables
public:
    void MsgThread_beginMessageLoop();
    void MsgThread_endMessageLoop();
    bool MsgThread_handleMessage(Message* msg);

// Operations
public:
    int MsgThread_create(MessageQueue* queue, const pthread_attr_t* attr = NULL);
    void Thread_quit(bool postAtFront = true, bool join = true);

// Overrides
public:
    void* Thread_run();

// Implementation
protected:
    void MsgThread_runMessageLoop();

// Data members
protected:
    uint32_t mAliveTime;
};
#endif  // __STDCOLLS_H__

__END_NAMESPACE

#include "thdutil.inl"

#endif  // __THDUTIL_H__