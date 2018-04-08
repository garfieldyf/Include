///////////////////////////////////////////////////////////////////////////////
// stdsputil.h
//
// Copyright(c) 2018, Garfield. All rights reserved.
// Author  : Garfield
// Version : 1.0
// Creation Date : 2010/10/9

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __STDSPUTIL_H__
#define __STDSPUTIL_H__

#include "platform.h"

///////////////////////////////////////////////////////////////////////////////
// Classes in this file:
//
// shared_ptr<_Ty>

namespace stdutil {

///////////////////////////////////////////////////////////////////////////////
// Interface of the shared_ptr class
//

template <typename _Ty>
class shared_ptr
{
public:
    typedef _Ty* pointer;
    typedef _Ty& reference;

// Constructors/Destructor
public:
    explicit shared_ptr(pointer _Ptr = NULL);
    shared_ptr(const shared_ptr<_Ty>& that);
    ~shared_ptr();

    template <typename _Other>
    friend class shared_ptr;

    template <typename _Other>
    explicit shared_ptr(_Other* _Ptr);

    template <typename _Other>
    shared_ptr(const shared_ptr<_Other>& that);

#if defined(_MEMORY_) || defined(_STLP_MEMORY) || defined(_GLIBCXX_MEMORY)
    template <typename _Other>
    shared_ptr(std::auto_ptr<_Other>& that);
#endif

// Operations
public:
    shared_ptr<_Ty>& operator=(pointer _Ptr);
    shared_ptr<_Ty>& operator=(const shared_ptr<_Ty>& that);

    template <typename _Other>
    shared_ptr<_Ty>& operator=(_Other* _Ptr);

    template <typename _Other>
    shared_ptr<_Ty>& operator=(const shared_ptr<_Other>& that);

#if defined(_MEMORY_) || defined(_STLP_MEMORY) || defined(_GLIBCXX_MEMORY)
    template <typename _Other>
    shared_ptr<_Ty>& operator=(std::auto_ptr<_Other>& that);
#endif

    void release();
    pointer get() const;

    operator pointer() const;
    pointer operator->() const;
    reference operator*() const;

    bool operator==(const shared_ptr<_Ty>& that) const;
    bool operator!=(const shared_ptr<_Ty>& that) const;

    template <typename _Other>
    bool operator==(const shared_ptr<_Other>& that) const;

    template <typename _Other>
    bool operator!=(const shared_ptr<_Other>& that) const;

#ifdef _CRT_DEBUG_DUMP
    template <typename _Pr> void Dump(const _Pr& _Printer) const;
#endif

// Implementation
private:
    void _AddRef();

    template <typename _Other>
    shared_ptr<_Ty>& _Assign(_Other* __Pointer, unsigned* __RefCount = NULL);

// Data members
private:
    pointer _Pointer;
    unsigned* _RefCount;
};

}  // namespace stdutil

#include "stdsputil.inl"

#endif  // __STDSPUTIL_H__