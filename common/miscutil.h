///////////////////////////////////////////////////////////////////////////////
// miscutil.h
//
// Author : Garfield
// Creation Date : 2010/3/22

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __MISCUTIL_H__
#define __MISCUTIL_H__

#include "platform.h"

///////////////////////////////////////////////////////////////////////////////
// Macros in this file:
//

#ifdef WINVER
#define __memcpy                CopyMemory
#define __strncpy               lstrcpynA
#define __strtoll               _strtoi64
#define __tstrlen               lstrlen
#define __tstrncmp              _tcsncmp
#define __snprintf              sprintf_s
#define __sntprintf             _tssprintf
#else
#define __memcpy                memcpy
#define __strncpy               strlcpy
#define __strtoll               strtoll
#define __tstrlen               strlen
#define __tstrncmp              strncmp
#define __snprintf              snprintf
#define __sntprintf             snprintf
#endif  // WINVER


///////////////////////////////////////////////////////////////////////////////
// Global functions in this file:
//
// __Min<_Ty>()
// __Max<_Ty>()
// __swap<_Ty>()
// __fopen()
// __realloc()
// __localtime()
// __to_digit()
// __to_byte_array()
// __to_hex_string<_Ty>()


///////////////////////////////////////////////////////////////////////////////
// Global functions
//

template <typename _Ty>
__STATIC_INLINE__ const _Ty& __Min(const _Ty& _Left, const _Ty& _Right)
{
    return (_Left < _Right ? _Left : _Right);
}

template <typename _Ty>
__STATIC_INLINE__ const _Ty& __Max(const _Ty& _Left, const _Ty& _Right)
{
    return (_Left > _Right ? _Left : _Right);
}

template <typename _Ty>
__STATIC_INLINE__ void __swap(_Ty& _Left, _Ty& _Right)
{
    _Ty _Tmp(_Left);
    _Left  = _Right;
    _Right = _Tmp;
}

__STATIC_INLINE__ FILE* __fopen(const char* _Filename, const char* _Mode)
{
#ifdef WINVER
    FILE* _File = NULL;
    ::fopen_s(&_File, _Filename, _Mode);
    return _File;
#else
    return ::fopen(_Filename, _Mode);
#endif  // WINVER
}

__STATIC_INLINE__ void* __realloc(void* _Memory, uint32_t _NewSize, uint32_t _CopySize = 0)
{
    assert(_NewSize > 0);

    void* _NewMemory = ::realloc(_Memory, _NewSize);
    if (_NewMemory == NULL)
    {
        LOGW("Couldn't reallocate memory (memory = %p, size = %u)\n", _Memory, _NewSize);
        if ((_NewMemory = ::malloc(_NewSize)) != NULL && _CopySize != 0)
            ::__memcpy(_NewMemory, _Memory, _CopySize);

        __check_error(_NewMemory == NULL, "Couldn't allocate memory (size = %u)\n", _NewSize);
        ::free(_Memory);
    }

    return _NewMemory;
}

#if defined(_INC_TIME) || defined(TIME64_H) || defined(_TIME_H_)
__STATIC_INLINE__ bool __localtime(struct tm& _Tm, int64_t _Time)
{
#ifdef WINVER
    return (::_localtime64_s(&_Tm, &_Time) == 0);
#else
#ifdef __LP64__
    return (::localtime_r(&_Time, &_Tm) != NULL);
#else
    return (::localtime64_r(&_Time, &_Tm) != NULL);
#endif  // __LP64__
#endif  // WINVER
}
#endif  // defined(_INC_TIME) || defined(TIME64_H) || defined(_TIME_H_)

/*
__STATIC_INLINE__ bool __startswith(const TCHAR* _Str, const TCHAR* _Prefix, int32_t _Length = INVALID_LENGTH)
{
    assert(_Str);
    assert(_Prefix);

    const int32_t _PrefixLength = ::__tstrlen(_Prefix);
    if (_Length == INVALID_LENGTH)
        _Length = ::__tstrlen(_Str);

    return (_Length >= _PrefixLength && ::__tstrncmp(_Str, _Prefix, _PrefixLength) == 0);
}

__STATIC_INLINE__ bool __endswith(const TCHAR* _Str, const TCHAR* _Suffix, int32_t _Length = INVALID_LENGTH)
{
    assert(_Str);
    assert(_Suffix);

    const int32_t _SuffixLength = ::__tstrlen(_Suffix);
    if (_Length == INVALID_LENGTH)
        _Length = ::__tstrlen(_Str);

    return (_Length >= _SuffixLength && ::__tstrncmp(_Str + (_Length - _SuffixLength), _Suffix, _SuffixLength) == 0);
}
*/

__STATIC_INLINE__ unsigned __to_digit(TCHAR _Hex)
{
    return (_Hex - (_Hex >= _T('0') && _Hex <= _T('9') ? _T('0') : (_Hex >= _T('a') && _Hex <= _T('f') ? _T('a') - 10 : _T('A') - 10)));
}

__STATIC_INLINE__ void __to_byte_array(unsigned char* _Out, const TCHAR* _Data, int32_t _First = 0, int32_t _Last = INVALID_LENGTH, int32_t _Offset = 0)
{
    assert(_Out);
    assert(_Data);

    if (_Last == INVALID_LENGTH)
        _Last = ::__tstrlen(_Data);

    for (; _First < _Last; _First += 2, ++_Offset)
        _Out[_Offset] = (unsigned char)((__to_digit(_Data[_First]) << 4) + __to_digit(_Data[_First + 1]));
}

template <typename _Ty>
__STATIC_INLINE__ void __to_hex_string(const _Ty* _Data, int32_t _Length, TCHAR* _Out, bool _LowerCase = true)
{
    assert(_Out);
    assert(_Data);

    _Out[0] = _T('\0');
    if (_Length > 0)
    {
        TCHAR _Format[32];
        ::__sntprintf(_Format, _countof(_Format), (_LowerCase ? _T("%%%02dx") : _T("%%%02dX")), sizeof(_Ty) * 2);

        for (int32_t i = 0; i < _Length; ++i)
            _Out += ::__sntprintf(_Out, sizeof(_Ty) * 2 + 1, _Format, _Data[i]);
    }
}

#endif  // __MISCUTIL_H__