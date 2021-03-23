///////////////////////////////////////////////////////////////////////////////
// stdcolls.h
//
// Author : Garfield
// Creation Date : 2020/4/12

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __STDCOLLS_H__
#define __STDCOLLS_H__

#include <queue>
#include <stack>
#include <mutex>
#include <condition_variable>

///////////////////////////////////////////////////////////////////////////////
// Classes in this file:
//
// queue<_Ty>
// stack<_Ty>
// priority_queue<_Ty, _Comparator>
// blocking_deque<_Ty>
// priority_blocking_queue<_Ty, _Comparator>

namespace stdutil {

///////////////////////////////////////////////////////////////////////////////
// Interface of the queue class
//

template <typename _Ty>
class queue : public std::queue<_Ty>
{
public:
    using _Mybase = std::queue<_Ty>;
    using container_type = typename _Mybase::container_type;

// Constructors
public:
    queue() = default;
    explicit queue(container_type&& _Cont);
    explicit queue(const container_type& _Cont);

    queue(queue&&) = default;
    queue& operator=(queue&&) = default;

    queue(const queue&) = default;
    queue& operator=(const queue&) = default;

// Operations
public:
    void clear();
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the stack class
//

template <typename _Ty>
class stack : public std::stack<_Ty>
{
public:
    using _Mybase = std::stack<_Ty>;
    using container_type = typename _Mybase::container_type;

// Constructors
public:
    stack() = default;
    explicit stack(container_type&& _Cont);
    explicit stack(const container_type& _Cont);

    stack(stack&&) = default;
    stack& operator=(stack&&) = default;

    stack(const stack&) = default;
    stack& operator=(const stack&) = default;

// Operations
public:
    void clear();
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the priority_queue class
//

template <typename _Ty, typename _Comparator = std::less<_Ty>>
class priority_queue : public std::priority_queue<_Ty, std::vector<_Ty>, _Comparator>
{
public:
    using _Mybase = std::priority_queue<_Ty, std::vector<_Ty>, _Comparator>;
    using container_type = typename _Mybase::container_type;

// Constructors
public:
    priority_queue() = default;
    priority_queue(const _Comparator& _Comp, container_type&& _Cont);
    priority_queue(const _Comparator& _Comp, const container_type& _Cont);

    priority_queue(priority_queue&&) = default;
    priority_queue& operator=(priority_queue&&) = default;

    priority_queue(const priority_queue&) = default;
    priority_queue& operator=(const priority_queue&) = default;

// Operations
public:
    void clear();
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the _Blocking_container class
//

template <typename _Container>
class _Blocking_container
{
public:
    using size_type  = typename _Container::size_type;
    using value_type = typename _Container::value_type;

// Constructors
protected:
    _Blocking_container() = default;
    _Blocking_container(const _Blocking_container&) = delete;
    _Blocking_container& operator=(const _Blocking_container&) = delete;

// Operations
public:
    void clear();

// Attributes
public:
    bool empty() const;
    size_type size() const;

// Implementation
protected:
    using mutex_lock  = std::lock_guard<std::mutex>;
    using unique_lock = std::unique_lock<std::mutex>;

    template <typename _Predicate>
    bool _Pop(value_type& _Val, uint32_t _Timeout, _Predicate _Pred);

// Data members
protected:
    _Container _Mycont;
    mutable std::mutex _Mymutex;
    std::condition_variable _Mycond;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the blocking_deque class
//

template <typename _Ty>
class blocking_deque : public _Blocking_container<std::deque<_Ty>>
{
    using _Mybase = _Blocking_container<std::deque<_Ty>>;
    using _Mybase::_Mycont;
    using _Mybase::_Mycond;
    using _Mybase::_Mymutex;
    using value_type  = typename _Mybase::value_type;
    using mutex_lock  = typename _Mybase::mutex_lock;
    using unique_lock = typename _Mybase::unique_lock;

// Constructors
public:
    blocking_deque() = default;

// Operations
public:
    void push_front(value_type&& _Val);
    void push_front(const value_type& _Val);

    void push_back(value_type&& _Val);
    void push_back(const value_type& _Val);

    template <typename... _ValArgs>
    void emplace_front(_ValArgs&&... _Args);

    template <typename... _ValArgs>
    void emplace_back(_ValArgs&&... _Args);

    bool pop_front(value_type& _Val, uint32_t _Timeout = -1);
    bool pop_back(value_type& _Val, uint32_t _Timeout = -1);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the priority_blocking_queue class
//

template <typename _Ty, typename _Comparator = std::less<_Ty>>
class priority_blocking_queue : public _Blocking_container<priority_queue<_Ty, _Comparator>>
{
    using _Mybase = _Blocking_container<priority_queue<_Ty, _Comparator>>;
    using _Mybase::_Mycont;
    using _Mybase::_Mycond;
    using _Mybase::_Mymutex;
    using value_type  = typename _Mybase::value_type;
    using mutex_lock  = typename _Mybase::mutex_lock;
    using unique_lock = typename _Mybase::unique_lock;

// Constructors
public:
    priority_blocking_queue() = default;

// Operations
public:
    void push(value_type&& _Val);
    void push(const value_type& _Val);

    template <typename... _ValArgs>
    void emplace(_ValArgs&&... _Args);

    bool pop(value_type& _Val, uint32_t _Timeout = -1);
};

}  // namespace stdutil

#include "stdcolls.inl"

#endif  // __STDCOLLS_H__