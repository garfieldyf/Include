///////////////////////////////////////////////////////////////////////////////
// stdcolls.inl
//
// Copyright(c) 2012, Garfield. All rights reserved.
// Author  : Garfield
// Version : 1.0
// Finish Time : 2010/4/12

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __STDCOLLS_INL__
#define __STDCOLLS_INL__

#ifndef __STDCOLLS_H__
    #error stdcolls.inl requires stdcolls.h to be included first
#endif

namespace stdutil {

///////////////////////////////////////////////////////////////////////////////
// Implementation of the _Priority_base class
//

template <typename _Container, typename _Comparator>
inline _Priority_base<_Container, _Comparator>::_Priority_base()
{
}

template <typename _Container, typename _Comparator>
inline void _Priority_base<_Container, _Comparator>::clear()
{
    _Cont.clear();
}

template <typename _Container, typename _Comparator>
inline void _Priority_base<_Container, _Comparator>::pop()
{
    _Cont.pop_front();
}

template <typename _Container, typename _Comparator>
inline typename _Priority_base<_Container, _Comparator>::reference _Priority_base<_Container, _Comparator>::top()
{
    return _Cont.front();
}

template <typename _Container, typename _Comparator>
inline typename _Priority_base<_Container, _Comparator>::const_reference _Priority_base<_Container, _Comparator>::top() const
{
    return _Cont.front();
}

template <typename _Container, typename _Comparator>
inline bool _Priority_base<_Container, _Comparator>::empty() const
{
    return _Cont.empty();
}

template <typename _Container, typename _Comparator>
inline typename _Priority_base<_Container, _Comparator>::size_type _Priority_base<_Container, _Comparator>::size() const
{
    return _Cont.size();
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the priority_queue class
//

template <typename _Ty, typename _Container, typename _Comparator>
inline priority_queue<_Ty, _Container, _Comparator>::priority_queue()
{
}

template <typename _Ty, typename _Container, typename _Comparator>
inline void priority_queue<_Ty, _Container, _Comparator>::push(const value_type& _Val)
{
    _Cont.insert(std::upper_bound(_Cont.begin(), _Cont.end(), _Val, _Comp), _Val);
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the priority_stack class
//

template <typename _Ty, typename _Container, typename _Comparator>
inline priority_stack<_Ty, _Container, _Comparator>::priority_stack()
{
}

template <typename _Ty, typename _Container, typename _Comparator>
inline void priority_stack<_Ty, _Container, _Comparator>::push(const value_type& _Val)
{
    _Cont.insert(std::lower_bound(_Cont.begin(), _Cont.end(), _Val, _Comp), _Val);
}


#ifdef __ATLSYNC_H__
///////////////////////////////////////////////////////////////////////////////
// Implementation of the _Block_base class
//

template <typename _Container>
inline _Block_base<_Container>::_Block_base()
#ifndef _ATL_BLOCK_DEQUE_USE_CONDITION_VARIABLE
    // Creates an auto-reset and initial state is nonsignaled event object.
    : _Event(FALSE, FALSE)
#endif  // _ATL_BLOCK_DEQUE_USE_CONDITION_VARIABLE
{
}

template <typename _Container>
inline bool _Block_base<_Container>::empty() const
{
    ATL::CCritSecLock lock(_CritSec);
    return _Cont.empty();
}

template <typename _Container>
inline typename _Block_base<_Container>::size_type _Block_base<_Container>::size() const
{
    ATL::CCritSecLock lock(_CritSec);
    return _Cont.size();
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the block_deque class
//

template <typename _Ty, typename _Container>
inline block_deque<_Ty, _Container>::block_deque()
{
}

template <typename _Ty, typename _Container>
inline void block_deque<_Ty, _Container>::push_front(const value_type& _Val)
{
    ATL::CCritSecLock lock(_CritSec);
    _Cont.push_front(_Val);
    lock.Unlock();

#ifndef _ATL_BLOCK_DEQUE_USE_CONDITION_VARIABLE
    ATLASSERT(_Event.m_h);
    ATLVERIFY(_Event.Set());
#else
    _CondVar.Wake();
#endif  // _ATL_BLOCK_DEQUE_USE_CONDITION_VARIABLE
}

template <typename _Ty, typename _Container>
inline void block_deque<_Ty, _Container>::push_back(const value_type& _Val)
{
    ATL::CCritSecLock lock(_CritSec);
    _Cont.push_back(_Val);
    lock.Unlock();

#ifndef _ATL_BLOCK_DEQUE_USE_CONDITION_VARIABLE
    ATLASSERT(_Event.m_h);
    ATLVERIFY(_Event.Set());
#else
    _CondVar.Wake();
#endif  // _ATL_BLOCK_DEQUE_USE_CONDITION_VARIABLE
}

template <typename _Ty, typename _Container>
inline bool block_deque<_Ty, _Container>::front(reference _Val, unsigned _Timeout/* = 0*/)
{
    return _Front(_Val, _Timeout, false);
}

template <typename _Ty, typename _Container>
inline bool block_deque<_Ty, _Container>::back(reference _Val, unsigned _Timeout/* = 0*/)
{
    return _Back(_Val, _Timeout, false);
}

template <typename _Ty, typename _Container>
inline bool block_deque<_Ty, _Container>::pop_front(reference _Val, unsigned _Timeout/* = INFINITE*/)
{
    return _Front(_Val, _Timeout);
}

template <typename _Ty, typename _Container>
inline bool block_deque<_Ty, _Container>::pop_back(reference _Val, unsigned _Timeout/* = INFINITE*/)
{
    return _Back(_Val, _Timeout);
}

template <typename _Ty, typename _Container>
inline bool block_deque<_Ty, _Container>::_Front(reference _Val, unsigned _Timeout, bool _Remove/* = true*/)
{
#ifndef _ATL_BLOCK_DEQUE_USE_CONDITION_VARIABLE
    ATLASSERT(_Event.m_h);

    bool _Successful = (::WaitForSingleObject(_Event.m_h, _Timeout) == WAIT_OBJECT_0);
    if (_Successful)
    {
        // Gets the first element of the container and removes it, if the _Remove parameter is true.
        ATL::CCritSecLock lock(_CritSec);
        _Val = _Cont.front();
        if (_Remove)
            _Cont.pop_front();

        // Set the event object to signaled, if container is not empty.
        if (_Cont.size() != 0)
            ATLVERIFY(_Event.Set());
    }
#else
    ATL::CCritSecLock lock(_CritSec);

    // Sleeping _Timeout milliseconds, if container is empty.
    bool _Successful = (_Cont.size() != 0 || _CondVar.Sleep(_CritSec, _Timeout));
    if (_Successful)
    {
        // Gets the first element of the container and removes it, if the _Remove parameter is true.
        _Val = _Cont.front();
        if (_Remove)
            _Cont.pop_front();
    }
#endif  // _ATL_BLOCK_DEQUE_USE_CONDITION_VARIABLE

    return _Successful;
}

template <typename _Ty, typename _Container>
inline bool block_deque<_Ty, _Container>::_Back(reference _Val, unsigned _Timeout, bool _Remove/* = true*/)
{
#ifndef _ATL_BLOCK_DEQUE_USE_CONDITION_VARIABLE
    ATLASSERT(_Event.m_h);

    bool _Successful = (::WaitForSingleObject(_Event.m_h, _Timeout) == WAIT_OBJECT_0);
    if (_Successful)
    {
        // Gets the last element of the container and removes it, if the _Remove parameter is true.
        ATL::CCritSecLock lock(_CritSec);
        _Val = _Cont.back();
        if (_Remove)
            _Cont.pop_back();

        // Set the event object to signaled, if container is not empty.
        if (_Cont.size() != 0)
            ATLVERIFY(_Event.Set());
    }
#else
    ATL::CCritSecLock lock(_CritSec);

    // Sleeping _Timeout milliseconds, if container is empty.
    bool _Successful = (_Cont.size() != 0 || _CondVar.Sleep(_CritSec, _Timeout));
    if (_Successful)
    {
        // Gets the last element of the container and removes it, if the _Remove parameter is true.
        _Val = _Cont.back();
        if (_Remove)
            _Cont.pop_back();
    }
#endif  // _ATL_BLOCK_DEQUE_USE_CONDITION_VARIABLE

    return _Successful;
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the _Priority_block_base class
//

template <typename _Container>
inline _Priority_block_base<_Container>::_Priority_block_base()
{
}

template <typename _Container>
inline void _Priority_block_base<_Container>::push(const value_type& _Val)
{
    ATL::CCritSecLock lock(_CritSec);
    _Cont.push(_Val);
    lock.Unlock();

#ifndef _ATL_BLOCK_DEQUE_USE_CONDITION_VARIABLE
    ATLASSERT(_Event.m_h);
    ATLVERIFY(_Event.Set());
#else
    _CondVar.Wake();
#endif  // _ATL_BLOCK_DEQUE_USE_CONDITION_VARIABLE
}

template <typename _Container>
inline bool _Priority_block_base<_Container>::top(reference _Val, unsigned _Timeout/* = 0*/)
{
    return _Top(_Val, _Timeout, false);
}

template <typename _Container>
inline bool _Priority_block_base<_Container>::pop(reference _Val, unsigned _Timeout/* = INFINITE*/)
{
    return _Top(_Val, _Timeout);
}

template <typename _Container>
inline typename const _Priority_block_base<_Container>::comparator_type& _Priority_block_base<_Container>::comparator() const
{
    return _Cont._Comp;
}

template <typename _Container>
inline bool _Priority_block_base<_Container>::_Top(reference _Val, unsigned _Timeout, bool _Remove/* = true*/)
{
#ifndef _ATL_BLOCK_DEQUE_USE_CONDITION_VARIABLE
    ATLASSERT(_Event.m_h);

    bool _Successful = (::WaitForSingleObject(_Event.m_h, _Timeout) == WAIT_OBJECT_0);
    if (_Successful)
    {
        // Gets an element at the top of the container and removes it, if the _Remove parameter is true.
        ATL::CCritSecLock lock(_CritSec);
        _Val = _Cont.top();
        if (_Remove)
            _Cont.pop();

        // Set the event object to signaled, if container is not empty.
        if (_Cont.size() != 0)
            ATLVERIFY(_Event.Set());
    }
#else
    ATL::CCritSecLock lock(_CritSec);

    // Sleeping _Timeout milliseconds, if container is empty.
    bool _Successful = (_Cont.size() != 0 || _CondVar.Sleep(_CritSec, _Timeout));
    if (_Successful)
    {
        // Gets an element at the top of the container and removes it, if the _Remove parameter is true.
        _Val = _Cont.top();
        if (_Remove)
            _Cont.pop();
    }
#endif  // _ATL_BLOCK_DEQUE_USE_CONDITION_VARIABLE

    return _Successful;
}
#endif  // __ATLSYNC_H__

}  // namespace stdutil

#endif  // __STDCOLLS_INL__