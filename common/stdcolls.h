///////////////////////////////////////////////////////////////////////////////
// stdcolls.h
//
// Author : Garfield
// Creation Date : 2020/4/12

#ifndef __STDCOLLS_H__
#define __STDCOLLS_H__

#include "platform.h"
#include <queue>
#include <mutex>
#include <condition_variable>

///////////////////////////////////////////////////////////////////////////////
// Classes in this file:
//
// deque<_Ty, _Alloc>
// vector<_Ty, _Alloc>
// blocking_deque<_Ty, _Alloc>
// priority_queue<_Ty, _Container, _Comparator>
// priority_blocking_queue<_Ty, _Container, _Comparator>

namespace stdutil {

///////////////////////////////////////////////////////////////////////////////
// Interface of the container class
//

template <typename _TBase>
class container final : public _TBase
{
public:
    using value_type = typename _TBase::value_type;
    using _TBase::begin;
    using _TBase::end;
    using _TBase::erase;

// Constructors
public:
    container() = default;
    template <typename _Iter>
    container(_Iter _First, _Iter _Last);
    container(std::initializer_list<value_type> _List);

    container(container&&) = default;
    container& operator=(container&&) = default;

    container(const container&) = default;
    container& operator=(const container&) = default;

public:
    /**
     * Removes the specified element from this container.
     * @param _Val The element that is to be removed.
     * @return true if removes the element successful, false otherwise.
     */
    bool erase(const value_type& _Val);

    /**
     * Removes the specified element from this container.
     * @param _Pred The predicate to apply to all elements.
     * @return true if removes the element successful, false otherwise.
     */
    template <typename _Predicate>
    bool erase_if(_Predicate _Pred);

    /**
     * Removes from this container all the elements that compare equal to _Val.
     * @param _Val The element that is to be removed.
     * @return true if removes elements successful, false otherwise.
     */
    bool remove(const value_type& _Val);

    /**
     * Removes from this container all the elements for which predicate _Pred returns true.
     * @param _Pred The predicate to apply to all elements.
     * @return true if removes elements successful, false otherwise.
     */
    template <typename _Predicate>
    bool remove_if(_Predicate _Pred);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the deque / vector class
//

template <typename _Ty, typename _Alloc = std::allocator<_Ty>>
using deque = container<std::deque<_Ty, _Alloc>>;

template <typename _Ty, typename _Alloc = std::allocator<_Ty>>
using vector = container<std::vector<_Ty, _Alloc>>;


///////////////////////////////////////////////////////////////////////////////
// Interface of the _Blocking_container class
//

template <typename _Container>
class _Blocking_container
{
public:
    using size_type  = typename _Container::size_type;
    using reference  = typename _Container::reference;
    using value_type = typename _Container::value_type;
    using const_reference = typename _Container::const_reference;

// Constructors
protected:
    _Blocking_container() = default;
    _Blocking_container(const _Blocking_container&) = delete;
    _Blocking_container& operator=(const _Blocking_container&) = delete;

// Operations
public:
    /**
     * Removes all elements from this container, leaving it empty.
     */
    void clear();

    /**
     * Requests this container to reduce its memory usage to fit its size.
     */
    void shrink_to_fit();

    /**
     * Removes the specified element from this container.
     * @param _Val The element that is to be removed.
     * @return true if removes the element successful, false otherwise.
     */
    bool erase(const value_type& _Val);

    /**
     * Removes the specified element from this container.
     * @param _Pred The predicate to apply to all elements.
     * @return true if removes the element successful, false otherwise.
     */
    template <typename _Predicate>
    bool erase_if(_Predicate _Pred);

    /**
     * Removes from this container all the elements that compare equal to _Val.
     * @param _Val The element that is to be removed.
     * @return true if removes elements successful, false otherwise.
     */
    bool remove(const value_type& _Val);

    /**
     * Removes from this container all the elements for which predicate _Pred returns true.
     * @param _Pred The predicate to apply to all elements.
     * @return true if removes elements successful, false otherwise.
     */
    template <typename _Predicate>
    bool remove_if(_Predicate _Pred);

    /**
     * Swaps the contents of this container with another container.
     * @param _Right The container to swap contents with.
     */
    void swap(_Blocking_container& _Right);

// Attributes
public:
    /**
     * Tests if this container is empty.
     * @return true if this container is empty, false otherwise.
     */
    bool empty() const;

    /**
     * Returns the number of elements in this container.
     * @return The number of elements.
     */
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

template <typename _Ty, typename _Alloc = std::allocator<_Ty>>
class blocking_deque : public _Blocking_container<deque<_Ty, _Alloc>>
{
public:
    using value_type = typename deque<_Ty, _Alloc>::value_type;

// Constructors
public:
    blocking_deque() = default;

// Operations
public:
    /**
     * Adds an element constructed in place to the beginning of this container.
     * @param _Args The arguments forwarded to construct the element.
     */
    template <typename... _ValArgs>
    void push_front(_ValArgs&&... _Args);

    /**
     * Adds an element constructed in place to the end of this container.
     * @param _Args The arguments forwarded to construct the element.
     */
    template <typename... _ValArgs>
    void push_back(_ValArgs&&... _Args);

    /**
     * Retrieves and removes the element at the beginning of this container, waiting 
     * up to the specified _Timeout if necessary for an element to become available.
     * @param _Val The _Val to store the returned result.
     * @param _Timeout How long in milliseconds to wait before giving up, -1 causes
     * wait to indefinitely.
     * @return true if retrieve the element successful, false otherwise.
     */
    bool pop_front(value_type& _Val, uint32_t _Timeout = -1);

    /**
     * Retrieves and removes the element at the end of this container, waiting up 
     * to the specified _Timeout if necessary for an element to become available.
     * @param _Val The _Val to store the returned result.
     * @param _Timeout How long in milliseconds to wait before giving up, -1 causes
     * wait to indefinitely.
     * @return true if retrieve the element successful, false otherwise.
     */
    bool pop_back(value_type& _Val, uint32_t _Timeout = -1);

// Implementation
private:
    using super = _Blocking_container<deque<_Ty, _Alloc>>;
    using super::_Pop;
    using super::_Mycont;
    using super::_Mycond;
    using super::_Mymutex;
    using typename super::mutex_lock;
    using typename super::unique_lock;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the priority_queue class
//

template <typename _Ty, typename _Container = vector<_Ty>, typename _Comparator = std::less<_Ty>>
class priority_queue : public std::priority_queue<_Ty, _Container, _Comparator>
{
public:
    using value_type = typename _Container::value_type;

// Constructors
public:
    priority_queue() = default;
    template <typename _Iter>
    priority_queue(_Iter _First, _Iter _Last);
    priority_queue(std::initializer_list<_Ty> _List, const _Comparator& _Comp = _Comparator());

    priority_queue(priority_queue&&) = default;
    priority_queue& operator=(priority_queue&&) = default;

    priority_queue(const priority_queue&) = default;
    priority_queue& operator=(const priority_queue&) = default;

// Operations
public:
    /**
     * Removes all elements from this container, leaving it empty.
     */
    void clear();

    /**
     * Requests this container to reduce its memory usage to fit its size.
     */
    void shrink_to_fit();

    /**
     * Removes the specified element from this container.
     * @param _Val The element that is to be removed.
     * @return true if removes the element successful, false otherwise.
     */
    bool erase(const value_type& _Val);

    /**
     * Removes the specified element from this container.
     * @param _Pred The predicate to apply to all elements.
     * @return true if removes the element successful, false otherwise.
     */
    template <typename _Predicate>
    bool erase_if(_Predicate _Pred);

    /**
     * Removes from this container all the elements that compare equal to _Val.
     * @param _Val The element that is to be removed.
     * @return true if removes elements successful, false otherwise.
     */
    bool remove(const value_type& _Val);

    /**
     * Removes from this container all the elements for which predicate _Pred returns true.
     * @param _Pred The predicate to apply to all elements.
     * @return true if removes elements successful, false otherwise.
     */
    template <typename _Predicate>
    bool remove_if(_Predicate _Pred);

// Implementation
private:
    using super = std::priority_queue<_Ty, _Container, _Comparator>;
    using super::c;
    using super::comp;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the priority_blocking_queue class
//

template <typename _Ty, typename _Container = vector<_Ty>, typename _Comparator = std::less<_Ty>>
class priority_blocking_queue : public _Blocking_container<priority_queue<_Ty, _Container, _Comparator>>
{
public:
    using value_type = typename priority_queue<_Ty, _Container, _Comparator>::value_type;

// Constructors
public:
    priority_blocking_queue() = default;

// Operations
public:
    /**
     * Inserts an element constructed in place to this container.
     * @param _Args The arguments forwarded to construct the element.
     */
    template <typename... _ValArgs>
    void push(_ValArgs&&... _Args);

    /**
     * Retrieves and removes the element on top of this container, waiting up to 
     * the specified _Timeout if necessary for an element to become available.
     * @param _Val The _Val to store the returned result.
     * @param _Timeout How long in milliseconds to wait before giving up, -1 causes
     * wait to indefinitely.
     * @return true if retrieve the element successful, false otherwise.
     */
    bool pop(value_type& _Val, uint32_t _Timeout = -1);

// Implementation
private:
    using super = _Blocking_container<priority_queue<_Ty, _Container, _Comparator>>;
    using super::_Pop;
    using super::_Mycont;
    using super::_Mycond;
    using super::_Mymutex;
    using typename super::mutex_lock;
    using typename super::unique_lock;
};

}  // namespace stdutil

#include "stdcolls.inl"

#endif  // __STDCOLLS_H__