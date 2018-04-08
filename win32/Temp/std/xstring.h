////////////////////////////////////////////////////////////////////////////////////
// xstring.h : ͷ�ļ�
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
    *   ����:
    *       �ַ�������
    *
    *   ����:
    *       const _TCHAR* _Source Դ�ַ�������ָ��
    *       _TCHAR* _Dest         Ŀ���ַ���ָ��
    *
    *   ����ֵ:
    *       _TCHAR* Ŀ���ַ���ָ��
    *
    *   ע��:
    *       �� _Source �е����ݿ����� _Dest ��, _Dest �б������㹻�Ŀռ�
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
    *   ����:
    *       �ַ�������
    *
    *   ����:
    *       const _TCHAR* _Source Դ�ַ�������ָ��
    *
    *   ����ֵ:
    *       size_t Ŀ���ַ�������
    *
    *   ע��:
    *       Ŀ���ַ������Ȳ����� '\0'
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
    *   ����:
    *       �Ƚ��ַ���
    *
    *   ����:
    *       const _TCHAR* _Source Դ�ַ�������ָ��
    *       const _TCHAR* _Dest   Ŀ���ַ�������ָ��
    *
    *   ����ֵ:
    *       int _Source > _Dest ���� 1
    *           _Source = _Dest ���� 0
    *           _Source < _Dest ���� -1
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
    *   ����:
    *       �����ַ�
    *
    *   ����:
    *       const _TCHAR* _Source Դ�ַ�������ָ��
    *       _TCHAR* _Source       Դ�ַ���ָ��
    *       _TCHAR _Val           �����ҵ��ַ�
    *
    *   ����ֵ:
    *       const _TCHAR*, _TCHAR* �����ҵ��ַ���Դ�ַ����е�λ��(ָ��)
    *
    *   ע��:
    *       �ú�����Դ�ַ����в��ҵ�һ������Ҫ����ַ�. ���ҵ�, �򷵻���Դ�ַ�����
    *       ��λ��, ���򷵻� NULL
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
    *   ����:
    *       ��������ַ�
    *
    *   ����:
    *       const _TCHAR* _Source Դ�ַ�������ָ��
    *       _TCHAR* _Source       Դ�ַ���ָ��
    *       _TCHAR _Val           �����ҵ��ַ�
    *
    *   ����ֵ:
    *       const _TCHAR*, _TCHAR* �����ҵ��ַ���Դ�ַ����е�λ��(ָ��)
    *
    *   ע��:
    *       �ú�����Դ�ַ����ĺ�����ҵ�һ������Ҫ����ַ�. ���ҵ�, �򷵻���Դ�ַ���
    *       �е�λ��, ���򷵻� NULL
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
    *   ����:
    *       �ַ�������
    *
    *   ����:
    *       const _TCHAR* _Source Դ�ַ�������ָ��
    *       _TCHAR* _Dest         Ŀ���ַ���ָ��
    *
    *   ����ֵ:
    *       _TCHAR* Ŀ���ַ���ָ��
    *
    *   ע��:
    *       �� _Source �е��������ӵ� _Dest �ĺ���, _Dest �б������㹻�Ŀռ�
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
    *   ����:
    *       �����ַ���
    *
    *   ����:
    *       const _TCHAR* _Source Դ�ַ�������ָ��
    *       _TCHAR* _Source       Դ�ַ���ָ��
    *       const _TCHAR* _Search �����ҵ��ַ�������ָ��
    *
    *   ����ֵ:
    *       const _TCHAR*, _TCHAR* �����ҵ��ַ�����Դ�ַ����е���ʼλ��(ָ��)
    *
    *   ע��:
    *       �ú�����Դ�ַ����в��ҵ�һ������Ҫ����ַ���. �������ҵ��ַ���Ϊ��
    *       �򷵻�Դ�ַ���ָ��. ���ҵ�, �򷵻���Դ�ַ����е���ʼλ��, ���򷵻� NULL
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