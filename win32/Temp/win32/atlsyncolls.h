///////////////////////////////////////////////////////////////////////////////
// atlsyncolls.h
//
// Copyright(c) 2012, Garfield. All rights reserved.
// Author  : Garfield
// Version : 2.0
// Finish Time : 2007/3/22

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __ATLSYNCOLLS_H__
#define __ATLSYNCOLLS_H__

#include <deque>
#include <map>
#include <atlcoll.h>
#include "atlsyncutil.h"

///////////////////////////////////////////////////////////////////////////////
// Classes in this file:
//
// CConcurrentArray<_Ty, _Traits>
// CConcurrentMap<_Key, _Value, _KeyTraits, _ValTraits>
// concurrent_deque<_Ty, _Container>
// concurrent_map<_Key, _Value, _Container>
// block_deque<_Ty, _Container>
// priority_block_queue<_Ty, _Container, _Comparator>
// priority_block_stack<_Ty, _Container, _Comparator>

namespace atlutil {

///////////////////////////////////////////////////////////////////////////////
// Interface of the CConcurrentArray class
//

template <typename _Ty, typename _Traits = ATL::CElementTraits<_Ty> >
class CConcurrentArray
{
public:
    typedef ATL::CAtlArray<_Ty, _Traits> _Container;
    typedef typename _Container::INARGTYPE  INARGTYPE;
    typedef typename _Container::OUTARGTYPE OUTARGTYPE;

// Constructors
public:
    CConcurrentArray();

// Operations
public:
    void RemoveAll();
    void RemoveAt(size_t nIndex, size_t nCount = 1);

    _Ty& operator[](size_t nIndex);
    const _Ty& operator[](size_t nIndex) const;

    size_t Add(INARGTYPE element);
    void SetAtGrow(size_t nIndex, INARGTYPE element);
    void InsertAt(size_t nIndex, INARGTYPE element, size_t nCount = 1);

    void Copy(const _Container& _Source);
    void Copy(const CConcurrentArray<_Ty, _Traits>& _Source);

    size_t Append(const _Container& _Source);
    size_t Append(const CConcurrentArray<_Ty, _Traits>& _Source);

    void InsertArrayAt(size_t nStart, const _Container& _Source);
    void InsertArrayAt(size_t nStart, const CConcurrentArray<_Ty, _Traits>& _Source);

// Attributes
public:
    bool IsEmpty() const;

    _Ty* GetData();
    const _Ty* GetData() const;

    size_t GetCount() const;
    bool SetCount(size_t nNewSize, int nGrowBy = -1);

// Data members
public:
    _Container _Cont;
    mutable ATL::CCriticalSection _CritSec;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CConcurrentMap class
//

template <typename _Key, typename _Value, typename _KeyTraits = ATL::CElementTraits<_Key>, typename _ValTraits = ATL::CElementTraits<_Value> >
class CConcurrentMap
{
public:
    typedef ATL::CAtlMap<_Key, _Value, _KeyTraits, _ValTraits> _Container;
    typedef typename _Container::KINARGTYPE  KINARGTYPE;
    typedef typename _Container::VINARGTYPE  VINARGTYPE;
    typedef typename _Container::KOUTARGTYPE KOUTARGTYPE;
    typedef typename _Container::VOUTARGTYPE VOUTARGTYPE;
    typedef typename _Container::CPair CPair;

// Constructors
public:
    explicit CConcurrentMap(UINT nBins = 17, float fOptimalLoad = 0.75f, float fLoThreshold = 0.25f, float fHiThreshold = 2.25f, UINT nBlockSize = 10);

// Operations
public:
    POSITION GetStartPosition() const;

    CPair* GetNext(POSITION& pos);
    const CPair* GetNext(POSITION& pos) const;

    void RemoveAll();
    void RemoveAt(POSITION pos);
    bool RemoveKey(KINARGTYPE key);

    _Value& operator[](KINARGTYPE key);
    const _Key& GetNextKey(POSITION& pos) const;

    _Value& GetNextValue(POSITION& pos);
    const _Value& GetNextValue(POSITION& pos) const;

    CPair* Lookup(KINARGTYPE key);
    const CPair* Lookup(KINARGTYPE key) const;
    bool Lookup(KINARGTYPE key, VOUTARGTYPE value) const;

// Attributes
public:
    bool IsEmpty() const;
    size_t GetCount() const;

// Data members
public:
    _Container _Cont;
    mutable ATL::CCriticalSection _CritSec;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the concurrent_deque class
//

template <typename _Ty, typename _Container = std::deque<_Ty> >
class concurrent_deque
{
public:
    typedef _Container container_type;
    typedef typename _Container::size_type size_type;
    typedef typename _Container::value_type value_type;
    typedef typename _Container::reference reference;
    typedef typename _Container::const_reference const_reference;

// Constructors
public:
    concurrent_deque();
    concurrent_deque(const concurrent_deque& that);

// Operations
public:
    void push_front(const value_type& _Val);
    void push_back(const value_type& _Val);

    void pop_front();
    void pop_back();

    reference front();
    const_reference front() const;

    reference back();
    const_reference back() const;

    void clear();
    void erase(size_type _Pos);
    void resize(size_type _Newsize);

    reference operator[](size_type _Pos);
    const_reference operator[](size_type _Pos) const;
    concurrent_deque& operator=(const concurrent_deque& that);

// Attributes
public:
    bool empty() const;
    size_type size() const;

// Data members
public:
    _Container _Cont;
    mutable ATL::CCriticalSection _CritSec;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the concurrent_map class
//

template <typename _Key, typename _Value, typename _Container = std::map<_Key, _Value> >
class concurrent_map
{
public:
    typedef _Container container_type;
    typedef typename _Container::_Pairib _Pairib;
    typedef typename _Container::size_type size_type;
    typedef typename _Container::value_type value_type;
    typedef typename _Container::key_type key_type;
    typedef typename _Container::mapped_type mapped_type;
    typedef typename _Container::iterator iterator;
    typedef typename _Container::const_iterator const_iterator;
    typedef typename _Container::reference reference;
    typedef typename _Container::const_reference const_reference;

// Constructors
public:
    concurrent_map();
    concurrent_map(const concurrent_map& that);

// Operations
public:
    iterator begin();
    const_iterator begin() const;

    iterator end();
    const_iterator end() const;

    _Pairib insert(const value_type& _Val);
    _Pairib insert(const key_type& _Keyval, const mapped_type& _Val);

    void clear();
    size_type erase(const key_type& _Keyval);

    iterator find(const key_type& _Keyval);
    const_iterator find(const key_type& _Keyval) const;

    mapped_type& operator[](const key_type& _Keyval);
    concurrent_map& operator=(const concurrent_map& that);

// Attributes
public:
    bool empty() const;
    size_type size() const;

// Data members
public:
    _Container _Cont;
    mutable ATL::CCriticalSection _CritSec;
};


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
    CConditionVariable _CondVar;
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


#ifdef __STDCOLLS_H__
///////////////////////////////////////////////////////////////////////////////
// Interface of the _Priority_block_base class
//

template <typename _Container>
class _Priority_block_base : public _Block_base<_Container>
{
public:
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
#endif  // __STDCOLLS_H__

}  // namespace atlutil

#include "atlsyncolls.inl"

#endif  // __ATLSYNCOLLS_H__