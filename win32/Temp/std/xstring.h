////////////////////////////////////////////////////////////////////////////////////
// xstring.h : 头文件
//
// Copyright(c) 2006, Garfield. All rights reserved.
// Author  : Garfield
// Version : 1.0
// Finish Time : 2006/12/25

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __XSTRING_H__
#define __XSTRING_H__

#ifndef _INC_TCHAR
#include <tchar.h>
#endif

namespace strutil {

    /***
    *
    *   _TCHAR* strcpy(const _TCHAR* _Source, _TCHAR* _Dest)
    *
    *   功能:
    *       字符串拷贝
    *
    *   参数:
    *       const _TCHAR* _Source 源字符串常量指针
    *       _TCHAR* _Dest         目标字符串指针
    *
    *   返回值:
    *       _TCHAR* 目标字符串指针
    *
    *   注意:
    *       将 _Source 中的内容拷贝到 _Dest 中, _Dest 中必须有足够的空间
    *
    *******************************************************************************/

    inline _TCHAR* strcpy(const _TCHAR* _Source, _TCHAR* _Dest) throw()
    {
        _ASSERTE(_Dest != NULL && _Source != NULL);

        _TCHAR* _RetVal = _Dest;
        while (*_Dest++ = *_Source++)
            ;

        return _RetVal;
    }


    /***
    *
    *   size_t strlen(const _TCHAR* _Source)
    *
    *   功能:
    *       字符串长度
    *
    *   参数:
    *       const _TCHAR* _Source 源字符串常量指针
    *
    *   返回值:
    *       size_t 目标字符串长度
    *
    *   注意:
    *       目标字符串长度不包含 '\0'
    *
    *******************************************************************************/

    inline size_t strlen(const _TCHAR* _Source) throw()
    {
        _ASSERTE(_Source != NULL);

        size_t _Length = 0;
        while (*_Source++)
            ++_Length;

        return _Length;
    }


    /***
    *
    *   int strcmp(const _TCHAR* _Source, const _TCHAR* _Dest)
    *
    *   功能:
    *       比较字符串
    *
    *   参数:
    *       const _TCHAR* _Source 源字符串常量指针
    *       const _TCHAR* _Dest   目标字符串常量指针
    *
    *   返回值:
    *       int _Source > _Dest 返回 1
    *           _Source = _Dest 返回 0
    *           _Source < _Dest 返回 -1
    *
    *******************************************************************************/

    inline int strcmp(const _TCHAR* _Source, const _TCHAR* _Dest) throw()
    {
        _ASSERTE(_Source != NULL && _Dest != NULL);

        int _Result = 0;
        while (!(_Result = *_Source - *_Dest) && *_Dest)
            ++_Source, ++_Dest;

        return (_Result > 0) ? 1 : ((_Result < 0) ? -1 : 0);
    }


    /***
    *
    *   const _TCHAR* strchr(const _TCHAR* _Source, _TCHAR _Val)
    *   _TCHAR* strchr(_TCHAR* _Source, _TCHAR _Val)
    *
    *   功能:
    *       查找字符
    *
    *   参数:
    *       const _TCHAR* _Source 源字符串常量指针
    *       _TCHAR* _Source       源字符串指针
    *       _TCHAR _Val           待查找的字符
    *
    *   返回值:
    *       const _TCHAR*, _TCHAR* 待查找的字符在源字符串中的位置(指针)
    *
    *   注意:
    *       该函数从源字符串中查找第一个符合要求的字符. 若找到, 则返回在源字符串中
    *       的位置, 否则返回 NULL
    *
    *******************************************************************************/

    inline const _TCHAR* strchr(const _TCHAR* _Source, _TCHAR _Val) throw()
    {
        _ASSERTE(_Source != NULL);

        while (*_Source && *_Source != _Val)
            ++_Source;

        return (*_Source == _Val) ? _Source : NULL;
    }

    inline _TCHAR* strchr(_TCHAR* _Source, _TCHAR _Val) throw()
    {
        return const_cast<_TCHAR*>(strchr(static_cast<const _TCHAR*>(_Source), _Val));
    }


    /***
    *
    *   const _TCHAR* strrchr(const _TCHAR* _Source, _TCHAR _Val)
    *   _TCHAR* strrchr(_TCHAR* _Source, _TCHAR _Val)
    *
    *   功能:
    *       反向查找字符
    *
    *   参数:
    *       const _TCHAR* _Source 源字符串常量指针
    *       _TCHAR* _Source       源字符串指针
    *       _TCHAR _Val           待查找的字符
    *
    *   返回值:
    *       const _TCHAR*, _TCHAR* 待查找的字符在源字符串中的位置(指针)
    *
    *   注意:
    *       该函数从源字符串的后面查找第一个符合要求的字符. 若找到, 则返回在源字符串
    *       中的位置, 否则返回 NULL
    *
    *******************************************************************************/

    inline const _TCHAR* strrchr(const _TCHAR* _Source, _TCHAR _Val) throw()
    {
        _ASSERTE(_Source != NULL);

        const _TCHAR* _Start = _Source;
        while (*_Source++)
            ;
        while (--_Source != _Start && *_Source != _Val)
            ;

        return (*_Source == _Val) ? _Source : NULL;
    }

    inline _TCHAR* strrchr(_TCHAR* _Source, _TCHAR _Val) throw()
    {
        return const_cast<_TCHAR*>(strrchr(static_cast<const _TCHAR*>(_Source), _Val));
    }


    /***
    *
    *   _TCHAR* strcat(const _TCHAR* _Source, _TCHAR* _Dest)
    *
    *   功能:
    *       字符串连接
    *
    *   参数:
    *       const _TCHAR* _Source 源字符串常量指针
    *       _TCHAR* _Dest         目标字符串指针
    *
    *   返回值:
    *       _TCHAR* 目标字符串指针
    *
    *   注意:
    *       将 _Source 中的内容连接到 _Dest 的后面, _Dest 中必须有足够的空间
    *
    *******************************************************************************/

    inline _TCHAR* strcat(const _TCHAR* _Source, _TCHAR* _Dest) throw()
    {
        _ASSERTE(_Source != NULL && _Dest != NULL);

        _TCHAR* _RetVal = _Dest;
        while (*_Dest)
            ++_Dest;

        while (*_Dest++ = *_Source++)
            ;
        
        return _RetVal;
    }


    /***
    *
    *   const _TCHAR* strstr(const _TCHAR* _Source, const _TCHAR* _Search)
    *   _TCHAR* strstr(_TCHAR* _Source, const _TCHAR* _Search)
    *
    *   功能:
    *       查找字符串
    *
    *   参数:
    *       const _TCHAR* _Source 源字符串常量指针
    *       _TCHAR* _Source       源字符串指针
    *       const _TCHAR* _Search 待查找的字符串常量指针
    *
    *   返回值:
    *       const _TCHAR*, _TCHAR* 待查找的字符串在源字符串中的起始位置(指针)
    *
    *   注意:
    *       该函数从源字符串中查找第一个符合要求的字符串. 若待查找的字符串为空
    *       则返回源字符串指针. 若找到, 则返回在源字符串中的起始位置, 否则返回 NULL
    *
    *******************************************************************************/

    inline const _TCHAR* strstr(const _TCHAR* _Source, const _TCHAR* _Search) throw()
    {
        _ASSERTE(_Source != NULL && _Search != NULL);

        const _TCHAR* _Str1 = NULL;
        const _TCHAR* _Str2 = NULL;

        while (*_Source)
        {
            _Str1 = _Source, _Str2 = _Search;

            while (*_Str1 && *_Str2 && !(*_Str1 - *_Str2))
                ++_Str1, ++_Str2;

            if (*_Str2 == _T('\0'))
                return _Source;

            ++_Source;
        }

        return NULL;
    }

    inline _TCHAR* strstr(_TCHAR* _Source, const _TCHAR* _Search) throw()
    {
        return const_cast<_TCHAR*>(strstr(static_cast<const _TCHAR*>(_Source), _Search));
    }

} // namespace strutil

#endif  // __XSTRING_H__