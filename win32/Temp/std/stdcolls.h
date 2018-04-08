///////////////////////////////////////////////////////////////////////////////
// stdcolls.h
//
// Copyright(c) 2012, Garfield. All rights reserved.
// Author  : Garfield
// Version : 1.0
// Finish Time : 2010/4/12

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __STDCOLLS_H__
#define __STDCOLLS_H__

#include <deque>
#include <list>
#include <algorithm>
#include <functional>

///////////////////////////////////////////////////////////////////////////////
// Classes in this file:
//
// priority_queue<_Ty, _Container, _Comparator>
// priority_stack<_Ty, _Container, _Comparator>
// block_deque<_Ty, _Container>
// priority_block_queue<_Ty, _Container, _Comparator>
// priority_block_stack<_Ty, _Container, _Comparator>

namespace stdutil {

///////////////////////////////////////////////////////////////////////////////
// Interface of the _Priority_base class
//

template <typename _Container, typename _Comparator>
class _Priority_base
{
public:
    typedef _Container container_type;
    typedef _Comparator comparator_type;
    typedef typename _Container::size_type size_type;
    typedef typename _Container::value_type value_type;
    typedef typename _Container::reference reference;
    typedef typename _Container::const_reference const_reference;

// Constructors
protected:
    _Priority_base();

// Operations
public:
    void clear();
    void pop();

    reference top();
    const_reference top() const;

// Attributes
public:
    bool empty() const;
    size_type size() const;

// Data members
public:
    _Container  _Cont;
    _Comparator _Comp;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the priority_queue class
//

template <typename _Ty, typename _Container = std::list<_Ty>, typename _Comparator = std::greater<typename _Container::value_type> >
class priority_queue : public _Priority_base<_Container, _Comparator>
{
public:
    typedef _Priority_base<_Container, _Comparator> _Mybase;
    typedef typename _Mybase::size_type size_type;
    typedef typename _Mybase::value_type value_type;
    typedef typename _Mybase::reference reference;
    typedef typename _Mybase::const_reference const_reference;
    typedef typename _Mybase::container_type container_type;
    typedef typename _Mybase::comparator_type comparator_type;

// Constructors
public:
    priority_queue();

// Operations
public:
    void push(const value_type& _Val);

// Implementation
private:
    using _Mybase::_Cont;
    using _Mybase::_Comp;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the priority_stack class
//

template <typename _Ty, typename _Container = std::list<_Ty>, typename _Comparator = std::greater<typename _Container::value_type> >
class priority_stack : public _Priority_base<_Container, _Comparator>
{
public:
    typedef _Priority_base<_Container, _Comparator> _Mybase;
    typedef typename _Mybase::size_type size_type;
    typedef typename _Mybase::value_type value_type;
    typedef typename _Mybase::reference reference;
    typedef typename _Mybase::const_reference const_reference;
    typedef typename _Mybase::container_type container_type;
    typedef typename _Mybase::comparator_type comparator_type;

// Constructors
public:
    priority_stack();

// Operations
public:
    void push(const value_type& _Val);

// Implementation
private:
    using _Mybase::_Cont;
    using _Mybase::_Comp;
};


#ifdef __ATLSYNC_H__
///////////////////////////////////////////////////////////////////////////////
// Interface of the _Block_base class
//

template <typename _Container>
class _Block_base
{
public:
    typedef _Container container_type;
    typedef typename _Container::size_type size_type;
    typedef typename _Container::reference reference;
    typedef typename _Container::value_type value_type;

// Constructors
protected:
    _Block_base();

// Attributes
public:
    bool empty() const;
    size_type size() const;

// Implementation
private:
    _Block_base(const _Block_base& that);
    _Block_base& operator=(const _Block_base& that);

// Data members
protected:
#ifndef _ATL_BLOCK_DEQUE_USE_CONDITION_VARIABLE
    ATL::CEvent _Event;
#else
    atlutil::CConditionVariable _CondVar;
#endif

    _Container _Cont;
    mutable ATL::CCriticalSection _CritSec;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the block_deque class
//

template <typename _Ty, typename _Container = std::deque<_Ty> >
class block_deque : public _Block_base<_Container>
{
public:
    typedef _Block_base<_Container> _Mybase;
    typedef typename _Mybase::size_type size_type;
    typedef typename _Mybase::reference reference;
    typedef typename _Mybase::value_type value_type;
    typedef typename _Mybase::container_type container_type;

// Constructors
public:
    block_deque();

// Operations
public:
    void push_front(const value_type& _Val);
    void push_back(const value_type& _Val);

    bool front(reference _Val, unsigned _Timeout = 0);
    bool back(reference _Val, unsigned _Timeout = 0);

    bool pop_front(reference _Val, unsigned _Timeout = INFINITE);
    bool pop_back(reference _Val, unsigned _Timeout = INFINITE);

// Implementation
protected:
    bool _Front(reference _Val, unsigned _Timeout, bool _Remove = true);
    bool _Back(reference _Val, unsigned _Timeout, bool _Remove = true);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the _Priority_block_base class
//

template <typename _Container>
class _Priority_block_base : public _Block_base<_Container>
{
public:
    typedef _Block_base<_Container> _Mybase;
    typedef typename _Mybase::size_type size_type;
    typedef typename _Mybase::reference reference;
    typedef typename _Mybase::value_type value_type;
    typedef typename _Mybase::container_type container_type;
    typedef typename _Container::comparator_type comparator_type;

// Constructors
protected:
    _Priority_block_base();

// Operations
public:
    void push(const value_type& _Val);
    bool top(reference _Val, unsigned _Timeout = 0);
    bool pop(reference _Val, unsigned _Timeout = INFINITE);

// Attributes
public:
    const comparator_type& comparator() const;

// Implementation
protected:
    bool _Top(reference _Val, unsigned _Timeout, bool _Remove = true);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the priority_block_queue class
//

template <typename _Ty, typename _Container = std::list<_Ty>, typename _Comparator = std::greater<typename _Container::value_type> >
class priority_block_queue : public _Priority_block_base<stdutil::priority_queue<_Ty, _Container, _Comparator> >
{
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the priority_block_stack class
//

template <typename _Ty, typename _Container = std::list<_Ty>, typename _Comparator = std::greater<typename _Container::value_type> >
class priority_block_stack : public _Priority_block_base<stdutil::priority_stack<_Ty, _Container, _Comparator> >
{
};
#endif  // __ATLSYNC_H__

}  // namespace stdutil

#include "stdcolls.inl"

#endif  // __STDCOLLS_H__