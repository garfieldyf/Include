///////////////////////////////////////////////////////////////////////////////
// atlsyncolls.inl
//
// Copyright(c) 2012, Garfield. All rights reserved.
// Author  : Garfield
// Version : 2.0
// Finish Time : 2007/3/22

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __ATLSYNCOLLS_INL__
#define __ATLSYNCOLLS_INL__

#ifndef __ATLSYNCOLLS_H__
    #error atlsyncolls.inl requires atlsyncolls.h to be included first
#endif

namespace atlutil {

///////////////////////////////////////////////////////////////////////////////
// Implementation of the CConcurrentArray class
//

template <typename _Ty, typename _Traits>
inline CConcurrentArray<_Ty, _Traits>::CConcurrentArray()
{
}

template <typename _Ty, typename _Traits>
inline void CConcurrentArray<_Ty, _Traits>::RemoveAll()
{
    ATL::CCritSecLock lock(_CritSec);
    _Cont.RemoveAll();
}

template <typename _Ty, typename _Traits>
inline void CConcurrentArray<_Ty, _Traits>::RemoveAt(size_t nIndex, size_t nCount/* = 1*/)
{
    ATL::CCritSecLock lock(_CritSec);
    _Cont.RemoveAt(nIndex, nCount);
}

template <typename _Ty, typename _Traits>
inline _Ty& CConcurrentArray<_Ty, _Traits>::operator[](size_t nIndex)
{
    ATL::CCritSecLock lock(_CritSec);
    return _Cont[nIndex];
}

template <typename _Ty, typename _Traits>
inline const _Ty& CConcurrentArray<_Ty, _Traits>::operator[](size_t nIndex) const
{
    ATL::CCritSecLock lock(_CritSec);
    return _Cont[nIndex];
}

template <typename _Ty, typename _Traits>
inline size_t CConcurrentArray<_Ty, _Traits>::Add(INARGTYPE element)
{
    ATL::CCritSecLock lock(_CritSec);
    return _Cont.Add(element);
}

template <typename _Ty, typename _Traits>
inline void CConcurrentArray<_Ty, _Traits>::SetAtGrow(size_t nIndex, INARGTYPE element)
{
    ATL::CCritSecLock lock(_CritSec);
    _Cont.SetAtGrow(nIndex, element);
}

template <typename _Ty, typename _Traits>
inline void CConcurrentArray<_Ty, _Traits>::InsertAt(size_t nIndex, INARGTYPE element, size_t nCount/* = 1*/)
{
    ATL::CCritSecLock lock(_CritSec);
    _Cont.InsertAt(nIndex, element, nCount);
}

template <typename _Ty, typename _Traits>
inline void CConcurrentArray<_Ty, _Traits>::Copy(const _Container& _Source)
{
    ATL::CCritSecLock lock(_CritSec);
    _Cont.Copy(_Source);
}

template <typename _Ty, typename _Traits>
inline void CConcurrentArray<_Ty, _Traits>::Copy(const CConcurrentArray<_Ty, _Traits>& _Source)
{
    ATL::CCritSecLock lock(_CritSec);
    _Cont.Copy(_Source._Cont);
}

template <typename _Ty, typename _Traits>
inline size_t CConcurrentArray<_Ty, _Traits>::Append(const _Container& _Source)
{
    ATL::CCritSecLock lock(_CritSec);
    return _Cont.Append(_Source);
}

template <typename _Ty, typename _Traits>
inline size_t CConcurrentArray<_Ty, _Traits>::Append(const CConcurrentArray<_Ty, _Traits>& _Source)
{
    ATL::CCritSecLock lock(_CritSec);
    return _Cont.Append(_Source._Cont);
}

template <typename _Ty, typename _Traits>
inline void CConcurrentArray<_Ty, _Traits>::InsertArrayAt(size_t nStart, const _Container& _Source)
{
    ATL::CCritSecLock lock(_CritSec);
    _Cont.InsertArrayAt(nStart, &_Source);
}

template <typename _Ty, typename _Traits>
inline void CConcurrentArray<_Ty, _Traits>::InsertArrayAt(size_t nStart, const CConcurrentArray<_Ty, _Traits>& _Source)
{
    ATL::CCritSecLock lock(_CritSec);
    _Cont.InsertArrayAt(nStart, &_Source._Cont);
}

template <typename _Ty, typename _Traits>
inline bool CConcurrentArray<_Ty, _Traits>::IsEmpty() const
{
    ATL::CCritSecLock lock(_CritSec);
    return _Cont.IsEmpty();
}

template <typename _Ty, typename _Traits>
inline _Ty* CConcurrentArray<_Ty, _Traits>::GetData()
{
    ATL::CCritSecLock lock(_CritSec);
    return _Cont.GetData();
}

template <typename _Ty, typename _Traits>
inline const _Ty* CConcurrentArray<_Ty, _Traits>::GetData() const
{
    ATL::CCritSecLock lock(_CritSec);
    return _Cont.GetData();
}

template <typename _Ty, typename _Traits>
inline size_t CConcurrentArray<_Ty, _Traits>::GetCount() const
{
    ATL::CCritSecLock lock(_CritSec);
    return _Cont.GetCount();
}

template <typename _Ty, typename _Traits>
inline bool CConcurrentArray<_Ty, _Traits>::SetCount(size_t nNewSize, int nGrowBy/* = -1*/)
{
    ATL::CCritSecLock lock(_CritSec);
    return _Cont.SetCount(nNewSize, nGrowBy);
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CConcurrentMap class
//

template <typename _Key, typename _Value, typename _KeyTraits, typename _ValTraits>
inline CConcurrentMap<_Key, _Value, _KeyTraits, _ValTraits>::CConcurrentMap(UINT nBins/* = 17*/, float fOptimalLoad/* = 0.75f*/, float fLoThreshold/* = 0.25f*/, float fHiThreshold/* = 2.25f*/, UINT nBlockSize/* = 10*/)
    : _Cont(nBins, fOptimalLoad, fLoThreshold, fHiThreshold, nBlockSize)
{
}

template <typename _Key, typename _Value, typename _KeyTraits, typename _ValTraits>
inline POSITION CConcurrentMap<_Key, _Value, _KeyTraits, _ValTraits>::GetStartPosition() const
{
    ATL::CCritSecLock lock(_CritSec);
    return _Cont.GetStartPosition();
}

template <typename _Key, typename _Value, typename _KeyTraits, typename _ValTraits>
inline typename CConcurrentMap<_Key, _Value, _KeyTraits, _ValTraits>::CPair* CConcurrentMap<_Key, _Value, _KeyTraits, _ValTraits>::GetNext(POSITION& pos)
{
    ATL::CCritSecLock lock(_CritSec);
    return _Cont.GetNext(pos);
}

template <typename _Key, typename _Value, typename _KeyTraits, typename _ValTraits>
inline typename const CConcurrentMap<_Key, _Value, _KeyTraits, _ValTraits>::CPair* CConcurrentMap<_Key, _Value, _KeyTraits, _ValTraits>::GetNext(POSITION& pos) const
{
    ATL::CCritSecLock lock(_CritSec);
    return _Cont.GetNext(pos);
}

template <typename _Key, typename _Value, typename _KeyTraits, typename _ValTraits>
inline void CConcurrentMap<_Key, _Value, _KeyTraits, _ValTraits>::RemoveAll()
{
    ATL::CCritSecLock lock(_CritSec);
    _Cont.RemoveAll();
}

template <typename _Key, typename _Value, typename _KeyTraits, typename _ValTraits>
inline void CConcurrentMap<_Key, _Value, _KeyTraits, _ValTraits>::RemoveAt(POSITION pos)
{
    ATL::CCritSecLock lock(_CritSec);
    _Cont.RemoveAtPos(pos);
}

template <typename _Key, typename _Value, typename _KeyTraits, typename _ValTraits>
inline bool CConcurrentMap<_Key, _Value, _KeyTraits, _ValTraits>::RemoveKey(KINARGTYPE key)
{
    ATL::CCritSecLock lock(_CritSec);
    return _Cont.RemoveKey(key);
}

template <typename _Key, typename _Value, typename _KeyTraits, typename _ValTraits>
inline _Value& CConcurrentMap<_Key, _Value, _KeyTraits, _ValTraits>::operator[](KINARGTYPE key)
{
    ATL::CCritSecLock lock(_CritSec);
    return _Cont[key];
}

template <typename _Key, typename _Value, typename _KeyTraits, typename _ValTraits>
inline const _Key& CConcurrentMap<_Key, _Value, _KeyTraits, _ValTraits>::GetNextKey(POSITION& pos) const
{
    ATL::CCritSecLock lock(_CritSec);
    return _Cont.GetNextKey(pos);
}

template <typename _Key, typename _Value, typename _KeyTraits, typename _ValTraits>
inline _Value& CConcurrentMap<_Key, _Value, _KeyTraits, _ValTraits>::GetNextValue(POSITION& pos)
{
    ATL::CCritSecLock lock(_CritSec);
    return _Cont.GetNextValue(pos);
}

template <typename _Key, typename _Value, typename _KeyTraits, typename _ValTraits>
inline const _Value& CConcurrentMap<_Key, _Value, _KeyTraits, _ValTraits>::GetNextValue(POSITION& pos) const
{
    ATL::CCritSecLock lock(_CritSec);
    return _Cont.GetNextValue(pos);
}

template <typename _Key, typename _Value, typename _KeyTraits, typename _ValTraits>
inline typename CConcurrentMap<_Key, _Value, _KeyTraits, _ValTraits>::CPair* CConcurrentMap<_Key, _Value, _KeyTraits, _ValTraits>::Lookup(KINARGTYPE key)
{
    ATL::CCritSecLock lock(_CritSec);
    return _Cont.Lookup(key);
}

template <typename _Key, typename _Value, typename _KeyTraits, typename _ValTraits>
inline typename const CConcurrentMap<_Key, _Value, _KeyTraits, _ValTraits>::CPair* CConcurrentMap<_Key, _Value, _KeyTraits, _ValTraits>::Lookup(KINARGTYPE key) const
{
    ATL::CCritSecLock lock(_CritSec);
    return _Cont.Lookup(key);
}

template <typename _Key, typename _Value, typename _KeyTraits, typename _ValTraits>
inline bool CConcurrentMap<_Key, _Value, _KeyTraits, _ValTraits>::Lookup(KINARGTYPE key, VOUTARGTYPE value) const
{
    ATL::CCritSecLock lock(_CritSec);
    return _Cont.Lookup(key, value);
}

template <typename _Key, typename _Value, typename _KeyTraits, typename _ValTraits>
inline bool CConcurrentMap<_Key, _Value, _KeyTraits, _ValTraits>::IsEmpty() const
{
    ATL::CCritSecLock lock(_CritSec);
    return _Cont.IsEmpty();
}

template <typename _Key, typename _Value, typename _KeyTraits, typename _ValTraits>
inline size_t CConcurrentMap<_Key, _Value, _KeyTraits, _ValTraits>::GetCount() const
{
    ATL::CCritSecLock lock(_CritSec);
    return _Cont.GetCount();
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the concurrent_deque class
//

template <typename _Ty, typename _Container>
inline concurrent_deque<_Ty, _Container>::concurrent_deque()
{
}

template <typename _Ty, typename _Container>
inline concurrent_deque<_Ty, _Container>::concurrent_deque(const concurrent_deque<_Ty, _Container>& that)
    : _Cont(that._Cont)
{
}

template <typename _Ty, typename _Container>
inline void concurrent_deque<_Ty, _Container>::push_front(const value_type& _Val)
{
    ATL::CCritSecLock lock(_CritSec);
    _Cont.push_front(_Val);
}

template <typename _Ty, typename _Container>
inline void concurrent_deque<_Ty, _Container>::push_back(const value_type& _Val)
{
    ATL::CCritSecLock lock(_CritSec);
    _Cont.push_back(_Val);
}

template <typename _Ty, typename _Container>
inline void concurrent_deque<_Ty, _Container>::pop_front()
{
    ATL::CCritSecLock lock(_CritSec);
    _Cont.pop_front();
}

template <typename _Ty, typename _Container>
inline void concurrent_deque<_Ty, _Container>::pop_back()
{
    ATL::CCritSecLock lock(_CritSec);
    _Cont.pop_back();
}

template <typename _Ty, typename _Container>
inline typename concurrent_deque<_Ty, _Container>::reference concurrent_deque<_Ty, _Container>::front()
{
    ATL::CCritSecLock lock(_CritSec);
    return _Cont.front();
}

template <typename _Ty, typename _Container>
inline typename concurrent_deque<_Ty, _Container>::const_reference concurrent_deque<_Ty, _Container>::front() const
{
    ATL::CCritSecLock lock(_CritSec);
    return _Cont.front();
}

template <typename _Ty, typename _Container>
inline typename concurrent_deque<_Ty, _Container>::reference concurrent_deque<_Ty, _Container>::back()
{
    ATL::CCritSecLock lock(_CritSec);
    return _Cont.back();
}

template <typename _Ty, typename _Container>
inline typename concurrent_deque<_Ty, _Container>::const_reference concurrent_deque<_Ty, _Container>::back() const
{
    ATL::CCritSecLock lock(_CritSec);
    return _Cont.back();
}

template <typename _Ty, typename _Container>
inline void concurrent_deque<_Ty, _Container>::clear()
{
    ATL::CCritSecLock lock(_CritSec);
    _Cont.clear();
}

template <typename _Ty, typename _Container>
inline void concurrent_deque<_Ty, _Container>::erase(size_type _Pos)
{
    ATL::CCritSecLock lock(_CritSec);
    _Cont.erase(_Cont.begin() + _Pos);
}

template <typename _Ty, typename _Container>
inline void concurrent_deque<_Ty, _Container>::resize(size_type _Newsize)
{
    ATL::CCritSecLock lock(_CritSec);
    _Cont.resize(_Newsize);
}

template <typename _Ty, typename _Container>
inline typename concurrent_deque<_Ty, _Container>::reference concurrent_deque<_Ty, _Container>::operator[](size_type _Pos)
{
    ATL::CCritSecLock lock(_CritSec);
    return _Cont[_Pos];
}

template <typename _Ty, typename _Container>
inline typename concurrent_deque<_Ty, _Container>::const_reference concurrent_deque<_Ty, _Container>::operator[](size_type _Pos) const
{
    ATL::CCritSecLock lock(_CritSec);
    return _Cont[_Pos];
}

template <typename _Ty, typename _Container>
inline concurrent_deque<_Ty, _Container>& concurrent_deque<_Ty, _Container>::operator=(const concurrent_deque<_Ty, _Container>& that)
{
    ATL::CCritSecLock lock(_CritSec);

    _Cont = that._Cont;
    return *this;
}

template <typename _Ty, typename _Container>
inline bool concurrent_deque<_Ty, _Container>::empty() const
{
    ATL::CCritSecLock lock(_CritSec);
    return _Cont.empty();
}

template <typename _Ty, typename _Container>
inline typename concurrent_deque<_Ty, _Container>::size_type concurrent_deque<_Ty, _Container>::size() const
{
    ATL::CCritSecLock lock(_CritSec);
    return _Cont.size();
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the concurrent_map class
//

template <typename _Key, typename _Value, typename _Container>
inline concurrent_map<_Key, _Value, _Container>::concurrent_map()
{
}

template <typename _Key, typename _Value, typename _Container>
inline concurrent_map<_Key, _Value, _Container>::concurrent_map(const concurrent_map<_Key, _Value, _Container>& that)
    : _Cont(that._Cont)
{
}

template <typename _Key, typename _Value, typename _Container>
inline typename concurrent_map<_Key, _Value, _Container>::iterator concurrent_map<_Key, _Value, _Container>::begin()
{
    ATL::CCritSecLock lock(_CritSec);
    return _Cont.begin();
}

template <typename _Key, typename _Value, typename _Container>
inline typename concurrent_map<_Key, _Value, _Container>::const_iterator concurrent_map<_Key, _Value, _Container>::begin() const
{
    ATL::CCritSecLock lock(_CritSec);
    return _Cont.begin();
}

template <typename _Key, typename _Value, typename _Container>
inline typename concurrent_map<_Key, _Value, _Container>::iterator concurrent_map<_Key, _Value, _Container>::end()
{
    ATL::CCritSecLock lock(_CritSec);
    return _Cont.end();
}

template <typename _Key, typename _Value, typename _Container>
inline typename concurrent_map<_Key, _Value, _Container>::const_iterator concurrent_map<_Key, _Value, _Container>::end() const
{
    ATL::CCritSecLock lock(_CritSec);
    return _Cont.end();
}

template <typename _Key, typename _Value, typename _Container>
inline typename concurrent_map<_Key, _Value, _Container>::_Pairib concurrent_map<_Key, _Value, _Container>::insert(const value_type& _Val)
{
    ATL::CCritSecLock lock(_CritSec);
    return _Cont.insert(_Val);
}

template <typename _Key, typename _Value, typename _Container>
inline typename concurrent_map<_Key, _Value, _Container>::_Pairib concurrent_map<_Key, _Value, _Container>::insert(const key_type& _Keyval, const mapped_type& _Val)
{
    ATL::CCritSecLock lock(_CritSec);
    return _Cont.insert(value_type(_Keyval, _Val));
}

template <typename _Key, typename _Value, typename _Container>
inline void concurrent_map<_Key, _Value, _Container>::clear()
{
    ATL::CCritSecLock lock(_CritSec);
    _Cont.clear();
}

template <typename _Key, typename _Value, typename _Container>
inline typename concurrent_map<_Key, _Value, _Container>::size_type concurrent_map<_Key, _Value, _Container>::erase(const key_type& _Keyval)
{
    ATL::CCritSecLock lock(_CritSec);
    return _Cont.erase(_Keyval);
}

template <typename _Key, typename _Value, typename _Container>
inline typename concurrent_map<_Key, _Value, _Container>::iterator concurrent_map<_Key, _Value, _Container>::find(const key_type& _Keyval)
{
    ATL::CCritSecLock lock(_CritSec);
    return _Cont.find(_Keyval);
}

template <typename _Key, typename _Value, typename _Container>
inline typename concurrent_map<_Key, _Value, _Container>::const_iterator concurrent_map<_Key, _Value, _Container>::find(const key_type& _Keyval) const
{
    ATL::CCritSecLock lock(_CritSec);
    return _Cont.find(_Keyval);
}

template <typename _Key, typename _Value, typename _Container>
inline typename concurrent_map<_Key, _Value, _Container>::mapped_type& concurrent_map<_Key, _Value, _Container>::operator[](const key_type& _Keyval)
{
    ATL::CCritSecLock lock(_CritSec);
    return _Cont[_Keyval];
}

template <typename _Key, typename _Value, typename _Container>
inline concurrent_map<_Key, _Value, _Container>& concurrent_map<_Key, _Value, _Container>::operator=(const concurrent_map<_Key, _Value, _Container>& that)
{
    ATL::CCritSecLock lock(_CritSec);

    _Cont = that._Cont;
    return *this;
}

template <typename _Key, typename _Value, typename _Container>
inline bool concurrent_map<_Key, _Value, _Container>::empty() const
{
    ATL::CCritSecLock lock(_CritSec);
    return _Cont.empty();
}

template <typename _Key, typename _Value, typename _Container>
inline typename concurrent_map<_Key, _Value, _Container>::size_type concurrent_map<_Key, _Value, _Container>::size() const
{
    ATL::CCritSecLock lock(_CritSec);
    return _Cont.size();
}


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


#ifdef __STDCOLLS_H__
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
#endif  // __STDCOLLS_H__

}  // namespace atlutil

#endif  // __ATLSYNCOLLS_INL__