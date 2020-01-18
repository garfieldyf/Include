///////////////////////////////////////////////////////////////////////////////
// memutil.inl
//
// Author : Garfield
// Creation Date : 2010/12/21

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __MEMUTIL_INL__
#define __MEMUTIL_INL__

#ifndef __MEMUTIL_H__
    #error memutil.inl requires memutil.h to be included first
#endif

namespace stdutil {

///////////////////////////////////////////////////////////////////////////////
// Implementation of the buffer_ptr class
//

template <typename _Ty>
__INLINE__ buffer_ptr<_Ty>::buffer_ptr()
    : size(0), data(NULL)
{
}

template <typename _Ty>
__INLINE__ buffer_ptr<_Ty>::buffer_ptr(const_pointer _Ptr, uint32_t _Size)
    : size(_Size), data(_Ptr)
{
}

template <typename _Ty>
__INLINE__ buffer_ptr<_Ty>::buffer_ptr(const_pointer _First, const_pointer _Last)
    : size(_Last - _First), data(_First)
{
}

template <typename _Ty>
__INLINE__ void buffer_ptr<_Ty>::clear()
{
    data = NULL;
    size = 0;
}

template <typename _Ty>
__INLINE__ void buffer_ptr<_Ty>::set(const_pointer _Ptr, uint32_t _Size)
{
    data = _Ptr;
    size = _Size;
}

template <typename _Ty>
__INLINE__ void buffer_ptr<_Ty>::set(const_pointer _First, const_pointer _Last)
{
    data = _First;
    size = _Last - _First;
}

template <typename _Ty>
__INLINE__ bool buffer_ptr<_Ty>::operator<(const buffer_ptr<_Ty>& that) const
{
    assert(!empty());
    assert(!that.empty());

    return (compare(that.data, that.size) < 0);
}

template <typename _Ty>
__INLINE__ bool buffer_ptr<_Ty>::operator>(const buffer_ptr<_Ty>& that) const
{
    assert(!empty());
    assert(!that.empty());

    return (compare(that.data, that.size) > 0);
}

template <typename _Ty>
__INLINE__ typename buffer_ptr<_Ty>::const_reference buffer_ptr<_Ty>::operator[](uint32_t _Index) const
{
    assert(!empty());
    assert(_Index < count());

    return data[_Index];
}

template <typename _Ty>
__INLINE__ bool buffer_ptr<_Ty>::operator==(const buffer_ptr<_Ty>& that) const
{
    assert(!empty());
    assert(!that.empty());

    return (compare(that.data, that.size) == 0);
}

template <typename _Ty>
__INLINE__ int buffer_ptr<_Ty>::compare(const buffer_ptr<_Ty>& that) const
{
    assert(!empty());
    assert(!that.empty());

    return compare(that.data, that.size);
}

template <typename _Ty>
__INLINE__ int buffer_ptr<_Ty>::compare(const_pointer _Ptr, uint32_t _Size) const
{
    assert(_Ptr);
    assert(!empty());

    int _Result = ::memcmp(data, _Ptr, __Min(size, _Size));
    return (_Result != 0 ? _Result : (int)(size - _Size));
}

#ifdef _CRT_DEBUG_DUMP
template <typename _Ty> template <typename _Pr>
__INLINE__ void buffer_ptr<_Ty>::Dump(const _Pr& _Printer) const
{
    _Printer("buffer_ptr<_Ty> [ size = %u , data = %p ]\n", size, data);
}
#endif  // _CRT_DEBUG_DUMP

template <typename _Ty>
__INLINE__ bool buffer_ptr<_Ty>::empty() const
{
    return (data == NULL);
}

template <typename _Ty>
__INLINE__ uint32_t buffer_ptr<_Ty>::count() const
{
    return (size / sizeof(_Ty));
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the char_sequence class
//

__INLINE__ char_sequence::char_sequence()
{
}

__INLINE__ char_sequence::char_sequence(const char* _Str)
    : _Mybase(_Str, ::strlen(_Str))
{
}

__INLINE__ char_sequence::char_sequence(const char* _Str, uint32_t _Length)
    : _Mybase(_Str, _Length)
{
}

__INLINE__ char_sequence::char_sequence(const char* _First, const char* _Last)
    : _Mybase(_First, _Last)
{
}

__INLINE__ bool char_sequence::operator==(const char* _Str) const
{
    assert(_Str);
    assert(!empty());

    return (compare(_Str, ::strlen(_Str)) == 0);
}

#ifdef _CRT_DEBUG_DUMP
template <typename _Pr>
__INLINE__ void char_sequence::Dump(const _Pr& _Printer) const
{
    if (empty())
        _Printer("char_sequence [ length = 0 , data = NULL ]\n");
    else
        _Printer("char_sequence [ length = %u , data = %.*s ]\n", size, size, data);
}
#endif  // _CRT_DEBUG_DUMP

#if defined(_STRING_) || defined(_STLP_STRING) || defined(_GLIBCXX_STRING) || defined(_LIBCPP_STRING)
__INLINE__ std::string char_sequence::to_string() const
{
    assert(!empty());
    return std::string(data, size);
}
#endif  // defined(_STRING_) || defined(_STLP_STRING) || defined(_GLIBCXX_STRING) || defined(_LIBCPP_STRING)

__INLINE__ uint32_t char_sequence::to_string(char* _Str, uint32_t _Length) const
{
    assert(_Str);
    assert(!empty());

    return ::__snprintf(_Str, _Length, "%.*s", size, data);
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the heap_buffer class
//

template <typename _Ty>
__INLINE__ heap_buffer<_Ty>::heap_buffer()
{
    _Assign(NULL);
}

template <typename _Ty>
__INLINE__ heap_buffer<_Ty>::~heap_buffer()
{
    ::free(data);
    LOGI("free memory [ data = %p, size = %u, capacity = %u ]\n", data, size, capacity());
}

template <typename _Ty>
__INLINE__ typename heap_buffer<_Ty>::pointer heap_buffer<_Ty>::allocate(uint32_t _Count)
{
    return allocateBytes(sizeof(_Ty) * _Count);
}

template <typename _Ty>
__INLINE__ typename heap_buffer<_Ty>::pointer heap_buffer<_Ty>::allocateBytes(uint32_t _Size)
{
    if (capacity() < _Size)
    {
    #ifdef WINVER
        data = (pointer)__realloc(data, _Size);
    #else
        data = (pointer)__realloc(data, maxsize = _Size);
    #endif  // WINVER

    #ifndef NDEBUG
        if (data != NULL)
            LOGI("data = %p, request size = %u, allocate size = %u\n", data, _Size, capacity());
        else
            LOGE("Couldn't allocate memory (size = %u)\n", _Size);
    #endif  // NDEBUG
    }

    return data;
}

template <typename _Ty>
__INLINE__ bool heap_buffer<_Ty>::append(const_pointer _Data, uint32_t _Count, uint32_t _Offset/* = 0*/)
{
    assert(_Data);
    assert(_Count);

    return appendBytes(_Data, _Count * sizeof(_Ty), _Offset * sizeof(_Ty));
}

template <typename _Ty>
__INLINE__ bool heap_buffer<_Ty>::appendBytes(const void* _Data, uint32_t _Size, uint32_t _Offset/* = 0*/)
{
    assert(_Data);

    const uint32_t _Capacity = capacity(), _NewSize = size + _Size;
    if (_Capacity < _NewSize)
    {
    #ifdef WINVER
        data = (pointer)__realloc(data, __Max(_Capacity * 3 / 2, _NewSize), size);
    #else
        data = (pointer)__realloc(data, maxsize = __Max(_Capacity * 3 / 2, _NewSize), size);
    #endif  // WINVER
    }

    const bool _Successful = (data != NULL);
    if (_Successful)
    {
        ::__memcpy((char*)data + size, (char*)_Data + _Offset, _Size);
        size += _Size;
    }

    return _Successful;
}

template <typename _Ty>
__INLINE__ void heap_buffer<_Ty>::clear()
{
    if (data != NULL)
    {
        LOGI("free memory [ data = %p, size = %u, capacity = %u ]\n", data, size, capacity());
        ::free(data);
        _Assign(NULL);
    }
}

template <typename _Ty>
__INLINE__ void heap_buffer<_Ty>::attach(pointer _Data, uint32_t _Size)
{
#ifndef NDEBUG
    if (_Data == NULL) assert(_Size == 0);
    if (_Data != NULL) assert(_Size != 0);
    if (_Data != NULL && data != NULL) assert(_Data != data);
#endif  // NDEBUG

    ::free(data);
    _Assign(_Data, _Size, _Size);
}

#ifdef _CRT_DEBUG_DUMP
template <typename _Ty> template <typename _Pr>
__INLINE__ void heap_buffer<_Ty>::Dump(const _Pr& _Printer) const
{
    _Printer("heap_buffer<_Ty> [ data = %p, count = %u, size = %u, capacity = %u ]\n", data, count(), size, capacity());
}
#endif  // _CRT_DEBUG_DUMP

template <typename _Ty>
__INLINE__ bool heap_buffer<_Ty>::empty() const
{
    return (data == NULL);
}

template <typename _Ty>
__INLINE__ uint32_t heap_buffer<_Ty>::count() const
{
    return (size / sizeof(_Ty));
}

template <typename _Ty>
__INLINE__ uint32_t heap_buffer<_Ty>::capacity() const
{
#ifdef WINVER
    return ::_msize(data);
#else
    return maxsize;
#endif  // WINVER
}

template <typename _Ty>
__INLINE__ void heap_buffer<_Ty>::_Assign(pointer _Data, uint32_t _Size/* = 0*/, uint32_t _Capacity/* = 0*/)
{
#ifdef WINVER
    UNUSED_PARAMETER(_Capacity);
#else
    maxsize = _Capacity;
#endif  // WINVER

    data = _Data;
    size = _Size;
}

}  // namespace stdutil

#endif  // __MEMUTIL_INL__