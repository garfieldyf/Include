///////////////////////////////////////////////////////////////////////////////
// stdcolls.inl
//
// Copyright(c) 2018, Garfield. All rights reserved.
// Author  : Garfield
// Version : 1.2
// Creation Date : 2010/4/12

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
// Implementation of the pool class
//

template <typename _Ty>
__INLINE__ pool<_Ty>::pool()
{
}

template <typename _Ty>
__INLINE__ pool<_Ty>::~pool()
{
    clear();
}

template <typename _Ty>
__INLINE__ typename pool<_Ty>::pointer pool<_Ty>::obtain()
{
    pointer _Val = NULL;
    if (_Cont.empty()) {
        _Val = new _Ty;
    } else {
        _Val = _Cont.front();
        _Cont.pop_front();
    }

    return _Val;
}

template <typename _Ty>
__INLINE__ void pool<_Ty>::clear()
{
    for (iterator _First = _Cont.begin(), _Last = _Cont.end(); _First != _Last; ++_First)
        delete (*_First);

    _Cont.clear();
}

template <typename _Ty>
__INLINE__ void pool<_Ty>::recycle(pointer _Val)
{
    assert(_Val);
    _Cont.push_front(_Val);
}

template <typename _Ty>
__INLINE__ bool pool<_Ty>::empty() const
{
    return _Cont.empty();
}

template <typename _Ty>
__INLINE__ typename pool<_Ty>::size_type pool<_Ty>::size() const
{
    return _Cont.size();
}


#if defined(__ATLSYNC_H__) || defined(__SYSUTILS_H__)
///////////////////////////////////////////////////////////////////////////////
// Implementation of the synchronized_pool class
//

template <typename _Container>
__INLINE__ synchronized_pool<_Container>::synchronized_pool()
{
}

template <typename _Container>
__INLINE__ typename synchronized_pool<_Container>::pointer synchronized_pool<_Container>::obtain()
{
#ifdef WINVER
    ATL::CCritSecLock lock(_CritSec);
#else
    __NS::MutexLock lock(_Mutex);
#endif  // WINVER

    return _Cont.obtain();
}

template <typename _Container>
__INLINE__ void synchronized_pool<_Container>::clear()
{
#ifdef WINVER
    ATL::CCritSecLock lock(_CritSec);
#else
    __NS::MutexLock lock(_Mutex);
#endif  // WINVER

    _Cont.clear();
}

template <typename _Container>
__INLINE__ void synchronized_pool<_Container>::recycle(pointer _Val)
{
    assert(_Val);

#ifdef WINVER
    ATL::CCritSecLock lock(_CritSec);
#else
    __NS::MutexLock lock(_Mutex);
#endif  // WINVER

    _Cont.recycle(_Val);
}

template <typename _Container>
__INLINE__ bool synchronized_pool<_Container>::empty() const
{
#ifdef WINVER
    ATL::CCritSecLock lock(_CritSec);
#else
    __NS::MutexLock lock(_Mutex);
#endif  // WINVER

    return _Cont.empty();
}

template <typename _Container>
__INLINE__ typename synchronized_pool<_Container>::size_type synchronized_pool<_Container>::size() const
{
#ifdef WINVER
    ATL::CCritSecLock lock(_CritSec);
#else
    __NS::MutexLock lock(_Mutex);
#endif  // WINVER

    return _Cont.size();
}
#endif  // defined(__ATLSYNC_H__) || defined(__SYSUTILS_H__)


///////////////////////////////////////////////////////////////////////////////
// Implementation of the _Priority_base class
//

template <typename _Ty, typename _Comparator>
__INLINE__ _Priority_base<_Ty, _Comparator>::_Priority_base()
{
}

template <typename _Ty, typename _Comparator>
__INLINE__ typename _Priority_base<_Ty, _Comparator>::const_iterator _Priority_base<_Ty, _Comparator>::find(const_reference _Val) const
{
    const_iterator _Last  = this->end();
    const_iterator _Where = std::lower_bound(this->begin(), _Last, _Val, comp);

    return (_Where != _Last && !comp(_Val, *_Where) ? _Where : _Last);
}

template <typename _Ty, typename _Comparator>
__INLINE__ typename _Priority_base<_Ty, _Comparator>::iterator _Priority_base<_Ty, _Comparator>::remove(const_reference _Val)
{
    iterator _Last  = this->end();
    iterator _Where = std::lower_bound(this->begin(), _Last, _Val, comp);

    return (_Where != _Last && !comp(_Val, *_Where) ? this->erase(_Where) : _Last);
}

template <typename _Ty, typename _Comparator>
__INLINE__ typename _Priority_base<_Ty, _Comparator>::size_type _Priority_base<_Ty, _Comparator>::remove_n(const_reference _Val)
{
    size_type _Count = 0;
    for (iterator _Where = this->begin(); (_Where = std::lower_bound(_Where, this->end(), _Val, comp)) != this->end() && !comp(_Val, *_Where); )
        _Where = this->erase(_Where), ++_Count;

    return _Count;
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the priority_queue class
//

template <typename _Ty, typename _Comparator>
__INLINE__ priority_queue<_Ty, _Comparator>::priority_queue()
{
}

template <typename _Ty, typename _Comparator>
__INLINE__ void priority_queue<_Ty, _Comparator>::push(const_reference _Val)
{
    insert(std::upper_bound(this->begin(), this->end(), _Val, this->comp), _Val);
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the priority_stack class
//

template <typename _Ty, typename _Comparator>
__INLINE__ priority_stack<_Ty, _Comparator>::priority_stack()
{
}

template <typename _Ty, typename _Comparator>
__INLINE__ void priority_stack<_Ty, _Comparator>::push(const_reference _Val)
{
    insert(std::lower_bound(this->begin(), this->end(), _Val, this->comp), _Val);
}


#if defined(__ATLSYNC_H__) || defined(__SYSUTILS_H__)
///////////////////////////////////////////////////////////////////////////////
// Implementation of the pointer_traits class
//

template <typename _Ty>
__INLINE__ void pointer_traits<_Ty>::destroy(_Ty* _Val)
{
    delete _Val;
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the primitive_traits class
//

template <typename _Ty>
__INLINE__ void primitive_traits<_Ty>::destroy(_Ty& /*_Val*/)
{
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the _Blocking_deque class
//

template <typename _Ty, typename _Container>
__INLINE__ _Blocking_deque<_Ty, _Container>::_Blocking_deque()
{
#ifdef WINVER
#ifdef _ATL_NEW_BLOCKING_DEQUE
    ::InitializeSRWLock(&_SRWLock);
#else
    // Creates an auto-reset and initial state is nonsignaled event object.
    ATLVERIFY(_Event.Create(NULL, FALSE, FALSE, NULL));
#endif  // _ATL_NEW_BLOCKING_DEQUE
#endif  // WINVER
}

template <typename _Ty, typename _Container>
__INLINE__ void _Blocking_deque<_Ty, _Container>::push_front(const_reference _Val)
{
#ifdef WINVER
#ifdef _ATL_NEW_BLOCKING_DEQUE
    atlutil::CExclusiveSRWLock lock(_SRWLock);
    _Cont.push_front(_Val);
    _CondVar.Wake();
#else
    ATL::CCritSecLock lock(_CritSec);
    _Cont.push_front(_Val);

    ATLASSERT(_Event.m_h);
    ATLVERIFY(_Event.Set());
#endif  // _ATL_NEW_BLOCKING_DEQUE
#else
    __NS::MutexLock lock(_Mutex);
    _Cont.push_front(_Val);
    _Condition.notify();
#endif  // WINVER
}

template <typename _Ty, typename _Container>
__INLINE__ void _Blocking_deque<_Ty, _Container>::push_back(const_reference _Val)
{
#ifdef WINVER
#ifdef _ATL_NEW_BLOCKING_DEQUE
    atlutil::CExclusiveSRWLock lock(_SRWLock);
    _Cont.push_back(_Val);
    _CondVar.Wake();
#else
    ATL::CCritSecLock lock(_CritSec);
    _Cont.push_back(_Val);

    ATLASSERT(_Event.m_h);
    ATLVERIFY(_Event.Set());
#endif  // _ATL_NEW_BLOCKING_DEQUE
#else
    __NS::MutexLock lock(_Mutex);
    _Cont.push_back(_Val);
    _Condition.notify();
#endif  // WINVER
}

template <typename _Ty, typename _Container>
__INLINE__ bool _Blocking_deque<_Ty, _Container>::find(const_reference _Val) const
{
#ifdef WINVER
#ifdef _ATL_NEW_BLOCKING_DEQUE
    atlutil::CSharedSRWLock lock(_SRWLock);
#else
    ATL::CCritSecLock lock(_CritSec);
#endif  // _ATL_NEW_BLOCKING_DEQUE
#else
    __NS::MutexLock lock(_Mutex);
#endif  // WINVER

    return (std::find(_Cont.begin(), _Cont.end(), _Val) != _Cont.end());
}

template <typename _Ty, typename _Container> template <typename _Pr>
__INLINE__ bool _Blocking_deque<_Ty, _Container>::find(_Pr _Pred) const
{
#ifdef WINVER
#ifdef _ATL_NEW_BLOCKING_DEQUE
    atlutil::CSharedSRWLock lock(_SRWLock);
#else
    ATL::CCritSecLock lock(_CritSec);
#endif  // _ATL_NEW_BLOCKING_DEQUE
#else
    __NS::MutexLock lock(_Mutex);
#endif  // WINVER

    return (std::find_if(_Cont.begin(), _Cont.end(), _Pred) != _Cont.end());
}

template <typename _Ty, typename _Container>
__INLINE__ int _Blocking_deque<_Ty, _Container>::pop_front(reference _Val, unsigned _Timeout/* = INFINITE*/)
{
#ifdef WINVER
#ifdef _ATL_NEW_BLOCKING_DEQUE
    atlutil::CExclusiveSRWLock lock(_SRWLock);

    // Waiting for _Timeout milliseconds, if container is empty.
    BOOL _Result = TRUE;
    while (_Result && _Cont.empty())
        _Result = _CondVar.Sleep(_SRWLock, _Timeout);

    // Popup the first element of the container, if wait successful.
    if (_Result)
    {
        _Val = _Cont.front();
        _Cont.pop_front();
    }
#else
    ATLASSERT(_Event.m_h);

    // Waiting for _Timeout milliseconds, if event is nonsignaled.
    DWORD _Result = ::WaitForSingleObject(_Event.m_h, _Timeout);
    if (_Result == WAIT_OBJECT_0)
    {
        // Popup the first element of the container, if wait successful.
        ATL::CCritSecLock lock(_CritSec);
        _Val = _Cont.front();
        _Cont.pop_front();

        // Set the event to signaled, if container is not empty.
        if (_Cont.size() != 0)
            ATLVERIFY(_Event.Set());
    }
#endif  // _ATL_NEW_BLOCKING_DEQUE
#else
    __NS::MutexLock lock(_Mutex);

    // Waiting for _Timeout milliseconds, if container is empty.
    int _Result = 0;
    while (_Result == 0 && _Cont.empty())
        _Result = _Condition.wait(_Mutex, _Timeout);

    // Popup the first element of the container, if wait successful.
    if (_Result == 0)
    {
        _Val = _Cont.front();
        _Cont.pop_front();
    }
#endif  // WINVER

    return (int)_Result;
}

template <typename _Ty, typename _Container>
__INLINE__ int _Blocking_deque<_Ty, _Container>::pop_back(reference _Val, unsigned _Timeout/* = INFINITE*/)
{
#ifdef WINVER
#ifdef _ATL_NEW_BLOCKING_DEQUE
    atlutil::CExclusiveSRWLock lock(_SRWLock);

    // Waiting for _Timeout milliseconds, if container is empty.
    BOOL _Result = TRUE;
    while (_Result && _Cont.empty())
        _Result = _CondVar.Sleep(_SRWLock, _Timeout);

    // Popup the last element of the container, if wait successful.
    if (_Result)
    {
        _Val = _Cont.back();
        _Cont.pop_back();
    }
#else
    ATLASSERT(_Event.m_h);

    // Waiting for _Timeout milliseconds, if event is nonsignaled.
    DWORD _Result = ::WaitForSingleObject(_Event.m_h, _Timeout);
    if (_Result == WAIT_OBJECT_0)
    {
        // Popup the last element of the container, if wait successful.
        ATL::CCritSecLock lock(_CritSec);
        _Val = _Cont.back();
        _Cont.pop_back();

        // Set the event to signaled, if container is not empty.
        if (_Cont.size() != 0)
            ATLVERIFY(_Event.Set());
    }
#endif  // _ATL_NEW_BLOCKING_DEQUE
#else
    __NS::MutexLock lock(_Mutex);

    // Waiting for _Timeout milliseconds, if container is empty.
    int _Result = 0;
    while (_Result == 0 && _Cont.empty())
        _Result = _Condition.wait(_Mutex, _Timeout);

    // Popup the last element of the container, if wait successful.
    if (_Result == 0)
    {
        _Val = _Cont.back();
        _Cont.pop_back();
    }
#endif  // WINVER

    return (int)_Result;
}

template <typename _Ty, typename _Container>
__INLINE__ bool _Blocking_deque<_Ty, _Container>::empty() const
{
#ifdef WINVER
#ifdef _ATL_NEW_BLOCKING_DEQUE
    atlutil::CSharedSRWLock lock(_SRWLock);
#else
    ATL::CCritSecLock lock(_CritSec);
#endif  // _ATL_NEW_BLOCKING_DEQUE
#else
    __NS::MutexLock lock(_Mutex);
#endif  // WINVER

    return _Cont.empty();
}

template <typename _Ty, typename _Container>
__INLINE__ typename _Blocking_deque<_Ty, _Container>::size_type _Blocking_deque<_Ty, _Container>::size() const
{
#ifdef WINVER
#ifdef _ATL_NEW_BLOCKING_DEQUE
    atlutil::CSharedSRWLock lock(_SRWLock);
#else
    ATL::CCritSecLock lock(_CritSec);
#endif  // _ATL_NEW_BLOCKING_DEQUE
#else
    __NS::MutexLock lock(_Mutex);
#endif  // WINVER

    return _Cont.size();
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the blocking_deque class
//

template <typename _Ty, typename _Traits, typename _Container>
__INLINE__ blocking_deque<_Ty, _Traits, _Container>::blocking_deque()
{
}

template <typename _Ty, typename _Traits, typename _Container>
__INLINE__ blocking_deque<_Ty, _Traits, _Container>::~blocking_deque()
{
    clear();
}

template <typename _Ty, typename _Traits, typename _Container>
__INLINE__ void blocking_deque<_Ty, _Traits, _Container>::clear()
{
#ifdef WINVER
#ifdef _ATL_NEW_BLOCKING_DEQUE
    atlutil::CExclusiveSRWLock lock(_SRWLock);
#else
    ATL::CCritSecLock lock(_CritSec);
#endif  // _ATL_NEW_BLOCKING_DEQUE
#else
    __NS::MutexLock lock(this->_Mutex);
#endif  // WINVER

    for (iterator _First = this->_Cont.begin(), _Last = this->_Cont.end(); _First != _Last; ++_First)
        _Traits::destroy(*_First);

    this->_Cont.clear();
}

template <typename _Ty, typename _Traits, typename _Container>
__INLINE__ bool blocking_deque<_Ty, _Traits, _Container>::remove(const_reference _Val)
{
#ifdef WINVER
#ifdef _ATL_NEW_BLOCKING_DEQUE
    atlutil::CExclusiveSRWLock lock(_SRWLock);
#else
    ATL::CCritSecLock lock(_CritSec);
#endif  // _ATL_NEW_BLOCKING_DEQUE
#else
    __NS::MutexLock lock(this->_Mutex);
#endif  // WINVER

    iterator _Where  = std::find(this->_Cont.begin(), this->_Cont.end(), _Val);
    bool _Successful = (_Where != this->_Cont.end());
    if (_Successful)
        _Remove(_Where);

    return _Successful;
}

template <typename _Ty, typename _Traits, typename _Container> template <typename _Pr>
__INLINE__ bool blocking_deque<_Ty, _Traits, _Container>::remove(_Pr _Pred)
{
#ifdef WINVER
#ifdef _ATL_NEW_BLOCKING_DEQUE
    atlutil::CExclusiveSRWLock lock(_SRWLock);
#else
    ATL::CCritSecLock lock(_CritSec);
#endif  // _ATL_NEW_BLOCKING_DEQUE
#else
    __NS::MutexLock lock(this->_Mutex);
#endif  // WINVER

    iterator _Where  = std::find_if(this->_Cont.begin(), this->_Cont.end(), _Pred);
    bool _Successful = (_Where != this->_Cont.end());
    if (_Successful)
        _Remove(_Where);

    return _Successful;
}

template <typename _Ty, typename _Traits, typename _Container>
__INLINE__ typename blocking_deque<_Ty, _Traits, _Container>::size_type blocking_deque<_Ty, _Traits, _Container>::remove_n(const_reference _Val)
{
#ifdef WINVER
#ifdef _ATL_NEW_BLOCKING_DEQUE
    atlutil::CExclusiveSRWLock lock(_SRWLock);
#else
    ATL::CCritSecLock lock(_CritSec);
#endif  // _ATL_NEW_BLOCKING_DEQUE
#else
    __NS::MutexLock lock(this->_Mutex);
#endif  // WINVER

    size_type _Count = 0;
    for (iterator _Where = this->_Cont.begin(); (_Where = std::find(_Where, this->_Cont.end(), _Val)) != this->_Cont.end(); )
        _Where = _Remove(_Where), ++_Count;

    return _Count;
}

template <typename _Ty, typename _Traits, typename _Container> template <typename _Pr>
__INLINE__ typename blocking_deque<_Ty, _Traits, _Container>::size_type blocking_deque<_Ty, _Traits, _Container>::remove_n(_Pr _Pred)
{
#ifdef WINVER
#ifdef _ATL_NEW_BLOCKING_DEQUE
    atlutil::CExclusiveSRWLock lock(_SRWLock);
#else
    ATL::CCritSecLock lock(_CritSec);
#endif  // _ATL_NEW_BLOCKING_DEQUE
#else
    __NS::MutexLock lock(this->_Mutex);
#endif  // WINVER

    size_type _Count = 0;
    for (iterator _Where = this->_Cont.begin(); (_Where = std::find_if(_Where, this->_Cont.end(), _Pred)) != this->_Cont.end(); )
        _Where = _Remove(_Where), ++_Count;

    return _Count;
}

template <typename _Ty, typename _Traits, typename _Container>
__INLINE__ typename blocking_deque<_Ty, _Traits, _Container>::iterator blocking_deque<_Ty, _Traits, _Container>::_Remove(iterator _Where)
{
    _Traits::destroy(*_Where);
    return this->_Cont.erase(_Where);
}
#endif  // defined(__ATLSYNC_H__) || defined(__SYSUTILS_H__)

}  // namespace stdutil

#endif  // __STDCOLLS_INL__