///////////////////////////////////////////////////////////////////////////////
// stdcolls.h
//
// Copyright(c) 2018, Garfield. All rights reserved.
// Author  : Garfield
// Version : 1.2
// Creation Date : 2010/4/12

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __STDCOLLS_H__
#define __STDCOLLS_H__

#include "platform.h"
#include <list>
#include <deque>
#include <algorithm>
#include <functional>

///////////////////////////////////////////////////////////////////////////////
// Classes in this file:
//
// pool<_Ty, _Container>
// synchronized_pool<_Container>
// priority_queue<_Ty, _Comparator>
// priority_stack<_Ty, _Comparator>
// pointer_traits<_Ty>
// primitive_traits<_Ty>
// blocking_deque<_Ty, _Traits, _Container>

namespace stdutil {

///////////////////////////////////////////////////////////////////////////////
// Interface of the pool class
//

template <typename _Ty>
class pool
{
    DECLARE_NONCOPYABLE(pool);

public:
    typedef std::list<_Ty*> _Container;
    typedef typename _Container::iterator iterator;
    typedef typename _Container::value_type pointer;
    typedef typename _Container::size_type size_type;

// Constructors/Destructor
public:
    pool();
    ~pool();

// Operations
public:
    pointer obtain();
    void clear();
    void recycle(pointer _Val);

// Attributes
public:
    bool empty() const;
    size_type size() const;

// Data members
private:
    _Container _Cont;
};


#if defined(__ATLSYNC_H__) || defined(__SYSUTILS_H__)
///////////////////////////////////////////////////////////////////////////////
// Interface of the synchronized_pool class
//

template <typename _Container>
class synchronized_pool
{
    DECLARE_NONCOPYABLE(synchronized_pool);

public:
    typedef _Container container_type;
    typedef typename _Container::pointer pointer;
    typedef typename _Container::iterator iterator;
    typedef typename _Container::size_type size_type;

// Constructors
public:
    synchronized_pool();

// Operations
public:
    pointer obtain();
    void clear();
    void recycle(pointer _Val);

// Attributes
public:
    bool empty() const;
    size_type size() const;

// Data members
private:
    _Container _Cont;

#ifdef WINVER
    mutable ATL::CCriticalSection _CritSec;
#else
    mutable __NS::Mutex _Mutex;
#endif  // WINVER
};
#endif  // defined(__ATLSYNC_H__) || defined(__SYSUTILS_H__)


///////////////////////////////////////////////////////////////////////////////
// Interface of the _Priority_base class
//

template <typename _Ty, typename _Comparator>
class ATL_NO_VTABLE _Priority_base : public std::deque<_Ty>
{
public:
    typedef std::deque<_Ty> _Mybase;
    typedef _Comparator comparator_type;
    typedef typename _Mybase::iterator iterator;
    typedef typename _Mybase::reference reference;
    typedef typename _Mybase::size_type size_type;
    typedef typename _Mybase::const_iterator const_iterator;
    typedef typename _Mybase::const_reference const_reference;

// Constructors
protected:
    _Priority_base();

// Operations
public:
    const_iterator find(const_reference _Val) const;

    iterator remove(const_reference _Val);
    size_type remove_n(const_reference _Val);

// Implementation
private:
    void push_front(const_reference _Val);
    void push_back(const_reference _Val);

// Data members
public:
    _Comparator comp;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the priority_queue class
//

template <typename _Ty, typename _Comparator = std::greater<_Ty> >
class priority_queue : public _Priority_base<_Ty, _Comparator>
{
public:
    typedef _Priority_base<_Ty, _Comparator> _Mybase;
    typedef typename _Mybase::iterator iterator;
    typedef typename _Mybase::reference reference;
    typedef typename _Mybase::size_type size_type;
    typedef typename _Mybase::const_iterator const_iterator;
    typedef typename _Mybase::const_reference const_reference;
    typedef typename _Mybase::comparator_type comparator_type;

// Constructors
public:
    priority_queue();

// Operations
public:
    void push(const_reference _Val);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the priority_stack class
//

template <typename _Ty, typename _Comparator = std::greater<_Ty> >
class priority_stack : public _Priority_base<_Ty, _Comparator>
{
public:
    typedef _Priority_base<_Ty, _Comparator> _Mybase;
    typedef typename _Mybase::iterator iterator;
    typedef typename _Mybase::reference reference;
    typedef typename _Mybase::size_type size_type;
    typedef typename _Mybase::const_iterator const_iterator;
    typedef typename _Mybase::const_reference const_reference;
    typedef typename _Mybase::comparator_type comparator_type;

// Constructors
public:
    priority_stack();

// Operations
public:
    void push(const_reference _Val);
};


#if defined(__ATLSYNC_H__) || defined(__SYSUTILS_H__)
///////////////////////////////////////////////////////////////////////////////
// Interface of the pointer_traits class
//

template <typename _Ty>
struct pointer_traits
{
    static void destroy(_Ty* _Val);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the primitive_traits class
//

template <typename _Ty>
struct primitive_traits
{
    static void destroy(_Ty& _Val);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the _Blocking_deque class
//

template <typename _Ty, typename _Container = std::deque<_Ty> >
class ATL_NO_VTABLE _Blocking_deque
{
    DECLARE_NONCOPYABLE(_Blocking_deque);

public:
    typedef _Container container_type;
    typedef typename _Container::iterator iterator;
    typedef typename _Container::reference reference;
    typedef typename _Container::size_type size_type;
    typedef typename _Container::const_reference const_reference;

// Constructors
protected:
    _Blocking_deque();

// Operations
public:
    void push_front(const_reference _Val);
    void push_back(const_reference _Val);

    bool find(const_reference _Val) const;
    template <typename _Pr> bool find(_Pr _Pred) const;

    int pop_front(reference _Val, unsigned _Timeout = INFINITE);
    int pop_back(reference _Val, unsigned _Timeout = INFINITE);

// Attributes
public:
    bool empty() const;
    size_type size() const;

// Data members
protected:
    _Container _Cont;

#ifdef WINVER
#ifdef _ATL_NEW_BLOCKING_DEQUE
    mutable SRWLOCK _SRWLock;
    atlutil::CConditionVariable _CondVar;
#else
    ATL::CEvent _Event;
    mutable ATL::CCriticalSection _CritSec;
#endif  // _ATL_NEW_BLOCKING_DEQUE
#else
    __NS::Condition _Condition;
    mutable __NS::Mutex _Mutex;
#endif  // WINVER
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the blocking_deque class
//

template <typename _Ty, typename _Traits = primitive_traits<_Ty>, typename _Container = std::deque<_Ty> >
class blocking_deque : public _Blocking_deque<_Ty, _Container>
{
public:
    typedef _Traits traits_type;
    typedef _Blocking_deque<_Ty, _Container> _Mybase;
    typedef typename _Mybase::iterator iterator;
    typedef typename _Mybase::reference reference;
    typedef typename _Mybase::size_type size_type;
    typedef typename _Mybase::container_type container_type;
    typedef typename _Mybase::const_reference const_reference;

// Constructors/Destructor
public:
    blocking_deque();
    ~blocking_deque();

// Operations
public:
    void clear();

    bool remove(const_reference _Val);
    template <typename _Pr> bool remove(_Pr _Pred);

    size_type remove_n(const_reference _Val);
    template <typename _Pr> size_type remove_n(_Pr _Pred);

// Implementation
private:
    iterator _Remove(iterator _Where);
};
#endif  // defined(__ATLSYNC_H__) || defined(__SYSUTILS_H__)

}  // namespace stdutil

#include "stdcolls.inl"

#endif  // __STDCOLLS_H__