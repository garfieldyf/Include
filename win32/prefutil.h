///////////////////////////////////////////////////////////////////////////////
// prefutil.h
//
// Copyright(c) 2019, Garfield. All rights reserved.
// Author  : Garfield
// Version : 1.0
// Creation Date : 2010/4/26

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __PREFUTIL_H__
#define __PREFUTIL_H__

#include <atlstr.h>
#include <atlcoll.h>
#include "miscutil.h"

///////////////////////////////////////////////////////////////////////////////
// Classes in this file:
//
// CProperties
// CPreferences

namespace stdutil {

///////////////////////////////////////////////////////////////////////////////
// Interface of the CProperties class
//

class CProperties
{
public:
    typedef ATL::CAtlMap<CString, CString, ATL::CStringElementTraits<CString>, ATL::CStringElementTraits<CString> > CMapProperties;
    typedef ATL::CAtlMap<CString, CString, ATL::CStringElementTraits<CString>, ATL::CStringElementTraits<CString> >::CPair CProperty;

// Constructors
public:
    CProperties();

// Operations
public:
    POSITION GetStartPosition() const;
    const CProperty* GetNext(POSITION& pos) const;

    void RemoveAll();
    BOOL Remove(LPCTSTR pszName);

#ifdef _OBJBASE_H_
    void SetGUID(LPCTSTR pszName, const GUID& guidValue);
    BOOL GetGUID(LPCTSTR pszName, GUID& guidValue) const;
#endif

    void SetDate(LPCTSTR pszName, const SYSTEMTIME& dtValue);
    BOOL GetDate(LPCTSTR pszName, SYSTEMTIME& dtValue) const;

#ifdef __ATLCOMTIME_H__
    void SetDate(LPCTSTR pszName, const COleDateTime& dtValue);
    BOOL GetDate(LPCTSTR pszName, COleDateTime& dtValue) const;
#endif

    void SetString(LPCTSTR pszName, LPCTSTR pszValue);
    LPCTSTR GetString(LPCTSTR pszName, LPCTSTR pszDefault = _T("")) const;

    void SetBinary(LPCTSTR pszName, const BYTE* pValue, DWORD cbSize);
    DWORD GetBinary(LPCTSTR pszName, LPBYTE pValue, DWORD cbSize) const;

    void SetInteger(LPCTSTR pszName, LONGLONG nValue);
    LONGLONG GetInteger(LPCTSTR pszName, LONGLONG nDefault = 0) const;

    template <typename _Ty> void SetInteger(LPCTSTR pszName, _Ty _Value);
    template <typename _Ty> _Ty GetInteger(LPCTSTR pszName, _Ty _Default) const;

    template <typename _Ty> void SetDecimal(LPCTSTR pszName, _Ty _Value, int nDigit = 6);
    template <typename _Ty> _Ty GetDecimal(LPCTSTR pszName, _Ty _Default) const;

#ifdef _CRT_DEBUG_DUMP
    template <typename _Pr> void Dump(const _Pr& _Printer) const;
#endif

// Attributes
public:
    DWORD GetCount() const;

// Data members
protected:
    CMapProperties m_mapProperties;
};


#if defined(__msxml2_h__) && defined(__ATLCOMCLI_H__)
///////////////////////////////////////////////////////////////////////////////
// Interface of the CPreferences class
//

class CPreferences : public CProperties
{
// Constructors
public:
    CPreferences();

// Operations
public:
    HRESULT Load(const VARIANT& vtSource);
    HRESULT Save() const;

// Implementation
protected:
    HRESULT LoadPreferences(IXMLDOMDocument* pDocument);
    HRESULT SavePreferences(IXMLDOMDocument* pDocument) const;

// Data members
public:
    CComVariant m_vtSource;
};
#endif  // defined(__msxml2_h__) && defined(__ATLCOMCLI_H__)

}  // namespace stdutil

#include "prefutil.inl"

#endif  // __PREFUTIL_H__