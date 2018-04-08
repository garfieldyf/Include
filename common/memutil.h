///////////////////////////////////////////////////////////////////////////////
// memutil.h
//
// Copyright(c) 2018, Garfield. All rights reserved.
// Author  : Garfield
// Version : 1.0
// Creation Date : 2010/12/21

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __MEMUTIL_H__
#define __MEMUTIL_H__

#include "miscutil.h"

///////////////////////////////////////////////////////////////////////////////
// Classes in this file:
//
// buffer_ptr<_Ty>
// char_sequence
// heap_buffer<_Ty>

namespace stdutil {

///////////////////////////////////////////////////////////////////////////////
// Interface of the buffer_ptr class
//

template <typename _Ty>
class buffer_ptr
{
public:
    typedef _Ty* pointer;
    typedef const _Ty* const_pointer;
    typedef const _Ty& const_reference;

// Constructors
public:
    buffer_ptr();
    buffer_ptr(const_pointer _Ptr, uint32_t _Size);
    buffer_ptr(const_pointer _First, const_pointer _Last);

// Operations
public:
    void clear();

    void set(const_pointer _Ptr, uint32_t _Size);
    void set(const_pointer _First, const_pointer _Last);

    bool operator<(const buffer_ptr<_Ty>& that) const;
    bool operator>(const buffer_ptr<_Ty>& that) const;

    const_reference operator[](uint32_t _Index) const;
    bool operator==(const buffer_ptr<_Ty>& that) const;

    int compare(const buffer_ptr<_Ty>& that) const;
    int compare(const_pointer _Ptr, uint32_t _Size) const;

#ifdef _CRT_DEBUG_DUMP
    template <typename _Pr> void Dump(const _Pr& _Printer) const;
#endif

// Attributes
public:
    bool empty() const;
    uint32_t count() const;

// Data members
public:
    uint32_t size;          // The data size in bytes.
    const_pointer data;     // The data pointer.
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the char_sequence class
//

class char_sequence : public buffer_ptr<char>
{
public:
    typedef buffer_ptr<char> _Mybase;

// Constructors
public:
    char_sequence();
    char_sequence(const char* _Str);
    char_sequence(const char* _Str, uint32_t _Length);
    char_sequence(const char* _First, const char* _Last);

// Operations
public:
    bool operator==(const char* _Str) const;

#ifdef _CRT_DEBUG_DUMP
    template <typename _Pr> void Dump(const _Pr& _Printer) const;
#endif

// Attributes
public:
#if defined(_STRING_) || defined(_STLP_STRING) || defined(_GLIBCXX_STRING)
    std::string to_string() const;
#endif
    uint32_t to_string(char* _Str, uint32_t _Length) const;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the heap_buffer class
//

template <typename _Ty>
class heap_buffer
{
    DECLARE_NONCOPYABLE(heap_buffer);

public:
    typedef _Ty* pointer;
    typedef const _Ty* const_pointer;

// Constructors/Destructor
public:
    heap_buffer();
    ~heap_buffer();

// Operations
public:
    pointer allocate(uint32_t _Count);
    pointer allocateBytes(uint32_t _Size);

    bool append(const_pointer _Data, uint32_t _Count, uint32_t _Offset = 0);
    bool appendBytes(const void* _Data, uint32_t _Size, uint32_t _Offset = 0);

    void clear();
    void attach(pointer _Data, uint32_t _Size);

#ifdef _CRT_DEBUG_DUMP
    template <typename _Pr> void Dump(const _Pr& _Printer) const;
#endif

// Attributes
public:
    bool empty() const;
    uint32_t count() const;
    uint32_t capacity() const;

// Implementation
private:
    void _Assign(pointer _Data, uint32_t _Size = 0, uint32_t _Capacity = 0);

// Data members
public:
    pointer data;
    uint32_t size;

#ifndef WINVER
    uint32_t maxsize;
#endif
};

}  // namespace stdutil

#include "memutil.inl"

#endif  // __MEMUTIL_H__