///////////////////////////////////////////////////////////////////////////////
// stdsputil.inl
//
// Copyright(c) 2019, Garfield. All rights reserved.
// Author  : Garfield
// Version : 1.0
// Creation Date : 2010/10/9

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __STDSPUTIL_INL__
#define __STDSPUTIL_INL__

#ifndef __STDSPUTIL_H__
    #error stdsputil.inl requires stdsputil.h to be included first
#endif

namespace stdutil {

///////////////////////////////////////////////////////////////////////////////
// Implementation of the shared_ptr class
//

template <typename _Ty>
__INLINE__ shared_ptr<_Ty>::shared_ptr(pointer _Ptr/* = NULL*/)
    : _Pointer(_Ptr), _RefCount(NULL)
{
    _AddRef();
}

template <typename _Ty>
__INLINE__ shared_ptr<_Ty>::shared_ptr(const shared_ptr<_Ty>& that)
    : _Pointer(that._Pointer), _RefCount(that._RefCount)
{
    _AddRef();
}

template <typename _Ty> template <typename _Other>
__INLINE__ shared_ptr<_Ty>::shared_ptr(_Other* _Ptr)
    : _Pointer(_Ptr), _RefCount(NULL)
{
    _AddRef();
}

template <typename _Ty> template <typename _Other>
__INLINE__ shared_ptr<_Ty>::shared_ptr(const shared_ptr<_Other>& that)
    : _Pointer(that._Pointer), _RefCount(that._RefCount)
{
    _AddRef();
}

#if defined(_MEMORY_) || defined(_STLP_MEMORY) || defined(_GLIBCXX_MEMORY)
template <typename _Ty> template <typename _Other>
__INLINE__ shared_ptr<_Ty>::shared_ptr(std::auto_ptr<_Other>& that)
    : _Pointer(that.release()), _RefCount(NULL)
{
    _AddRef();
}
#endif  // defined(_MEMORY_) || defined(_STLP_MEMORY) || defined(_GLIBCXX_MEMORY)

template <typename _Ty>
__INLINE__ shared_ptr<_Ty>::~shared_ptr()
{
    release();
}

template <typename _Ty>
__INLINE__ shared_ptr<_Ty>& shared_ptr<_Ty>::operator=(pointer _Ptr)
{
    return _Assign(_Ptr);
}

template <typename _Ty>
__INLINE__ shared_ptr<_Ty>& shared_ptr<_Ty>::operator=(const shared_ptr<_Ty>& that)
{
    return _Assign(that._Pointer, that._RefCount);
}

template <typename _Ty> template <typename _Other>
__INLINE__ shared_ptr<_Ty>& shared_ptr<_Ty>::operator=(_Other* _Ptr)
{
    return _Assign(_Ptr);
}

template <typename _Ty> template <typename _Other>
__INLINE__ shared_ptr<_Ty>& shared_ptr<_Ty>::operator=(const shared_ptr<_Other>& that)
{
    return _Assign(that._Pointer, that._RefCount);
}

#if defined(_MEMORY_) || defined(_STLP_MEMORY) || defined(_GLIBCXX_MEMORY)
template <typename _Ty> template <typename _Other>
__INLINE__ shared_ptr<_Ty>& shared_ptr<_Ty>::operator=(std::auto_ptr<_Other>& that)
{
    return _Assign(that.release());
}
#endif  // defined(_MEMORY_) || defined(_STLP_MEMORY) || defined(_GLIBCXX_MEMORY)

template <typename _Ty>
__INLINE__ void shared_ptr<_Ty>::release()
{
    if (_Pointer != NULL)
    {
        if (--*_RefCount == 0)
        {
            delete _Pointer;
            delete _RefCount;
        }

        _Pointer = NULL, _RefCount = NULL;
    }
}

template <typename _Ty>
__INLINE__ typename shared_ptr<_Ty>::pointer shared_ptr<_Ty>::get() const
{
    return _Pointer;
}

template <typename _Ty>
__INLINE__ shared_ptr<_Ty>::operator pointer() const
{
    return _Pointer;
}

template <typename _Ty>
__INLINE__ typename shared_ptr<_Ty>::pointer shared_ptr<_Ty>::operator->() const
{
    assert(_Pointer);
    return _Pointer;
}

template <typename _Ty>
__INLINE__ typename shared_ptr<_Ty>::reference shared_ptr<_Ty>::operator*() const
{
    assert(_Pointer);
    return *_Pointer;
}

template <typename _Ty>
__INLINE__ bool shared_ptr<_Ty>::operator==(const shared_ptr<_Ty>& that) const
{
    return (_Pointer == that._Pointer);
}

template <typename _Ty>
__INLINE__ bool shared_ptr<_Ty>::operator!=(const shared_ptr<_Ty>& that) const
{
    return (_Pointer != that._Pointer);
}

template <typename _Ty> template <typename _Other>
__INLINE__ bool shared_ptr<_Ty>::operator==(const shared_ptr<_Other>& that) const
{
    return (_Pointer == that._Pointer);
}

template <typename _Ty> template <typename _Other>
__INLINE__ bool shared_ptr<_Ty>::operator!=(const shared_ptr<_Other>& that) const
{
    return (_Pointer != that._Pointer);
}

#ifdef _CRT_DEBUG_DUMP
template <typename _Ty> template <typename _Pr>
__INLINE__ void shared_ptr<_Ty>::Dump(const _Pr& _Printer) const
{
    _Printer("shared_ptr<_Ty> [ Pointer = %p, RefCount = %u ]\n", _Pointer, (_RefCount != NULL ? *_RefCount : 0));
}
#endif  // _CRT_DEBUG_DUMP

template <typename _Ty>
__INLINE__ void shared_ptr<_Ty>::_AddRef()
{
    if (_Pointer != NULL)
    {
        if (_RefCount != NULL)
            ++*_RefCount;
        else
            _RefCount = new unsigned(1);
    }
#ifndef NDEBUG
    else
    {
        assert(_RefCount == NULL);
    }
#endif  // NDEBUG
}

template <typename _Ty> template <typename _Other>
__INLINE__ shared_ptr<_Ty>& shared_ptr<_Ty>::_Assign(_Other* __Pointer, unsigned* __RefCount/* = NULL*/)
{
    if (_Pointer != __Pointer)
    {
        release();
        _Pointer  = __Pointer;
        _RefCount = __RefCount;
        _AddRef();
    }

    return *this;
}

}  // namespace stdutil

#endif  // __STDSPUTIL_INL__