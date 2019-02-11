///////////////////////////////////////////////////////////////////////////////
// regutil.h
//
// Copyright(c) 2019, Garfield. All rights reserved.
// Author  : Garfield
// Version : 1.0
// Creation Date : 2009/6/12

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __REGUTIL_H__
#define __REGUTIL_H__

#include "atlmiscutil.h"

///////////////////////////////////////////////////////////////////////////////
// Classes in this file:
//
// CRegKey

namespace atlutil {

///////////////////////////////////////////////////////////////////////////////
// Interface of the CRegKeyTraits class
//

struct CRegKeyTraits : public CHandleTraits
{
    typedef HKEY HANDLET_PTR;
    static BOOL WINAPI Destroy(HANDLET_PTR hHandle);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CRegKey class
//

class CRegKey : public CHandle<CRegKeyTraits>
{
// Constructors
public:
    CRegKey();

// Operations
public:
    LONG Create(LPCTSTR pszAppKey, HKEY hRoot = HKEY_CURRENT_USER, DWORD dwOptions = REG_OPTION_NON_VOLATILE, REGSAM samDesired = KEY_READ | KEY_WRITE, LPDWORD pdwDisposition = NULL, LPTSTR pClass = REG_NONE, LPSECURITY_ATTRIBUTES psa = NULL);
    LONG QueryValue(LPCTSTR pszName, DWORD& cbSize, LPDWORD pdwType = NULL) const;

    LONG GetValue(LPCTSTR pszName, LPVOID pValue, DWORD cbSize) const;
    LONG SetValue(LPCTSTR pszName, LPCVOID pValue, DWORD cbSize, DWORD dwType = REG_DWORD);

#if defined(__ATLSTR_H__) || defined(__AFXSTR_H__)
    LONG GetStringValue(LPCTSTR pszName, CString& strValue) const;
#endif
    LONG GetStringValue(LPCTSTR pszName, LPTSTR pszValue, UINT nLength) const;

    LONG SetStringValue(LPCTSTR pszName, LPCTSTR pszValue, DWORD dwType = REG_SZ);
    LONG SetMultiStringValue(LPCTSTR pszName, LPCTSTR pszValue, DWORD cbSize = 0);

    LONG GetBinaryValue(LPCTSTR pszName, LPVOID pValue, DWORD& cbSize) const;
    LONG SetBinaryValue(LPCTSTR pszName, LPCVOID pValue, DWORD cbSize);

    template <typename _Ty> LONG GetIntegerValue(LPCTSTR pszName, _Ty& _Value) const;
    template <typename _Ty> LONG SetIntegerValue(LPCTSTR pszName, _Ty _Value, DWORD dwType = REG_DWORD);
};

}  // namespace atlutil

#include "regutil.inl"

#endif  // __REGUTIL_H__