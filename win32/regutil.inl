///////////////////////////////////////////////////////////////////////////////
// regutil.inl
//
// Copyright(c) 2018, Garfield. All rights reserved.
// Author  : Garfield
// Version : 1.0
// Creation Date : 2009/6/12

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __REGUTIL_INL__
#define __REGUTIL_INL__

#ifndef __REGUTIL_H__
    #error regutil.inl requires regutil.h to be included first
#endif

namespace atlutil {

///////////////////////////////////////////////////////////////////////////////
// Implementation of the CRegKeyTraits class
//

inline BOOL WINAPI CRegKeyTraits::Destroy(HANDLET_PTR hHandle)
{
    return (::RegCloseKey(hHandle) == ERROR_SUCCESS);
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CRegKey class
//

inline CRegKey::CRegKey()
{
}

inline LONG CRegKey::Create(LPCTSTR pszAppKey, HKEY hRoot/* = HKEY_CURRENT_USER*/, DWORD dwOptions/* = REG_OPTION_NON_VOLATILE*/, REGSAM samDesired/* = KEY_READ | KEY_WRITE*/, LPDWORD pdwDisposition/* = NULL*/, LPTSTR pClass/* = REG_NONE*/, LPSECURITY_ATTRIBUTES psa/* = NULL*/)
{
    ATLASSERT(IsEmpty());
    ATLASSERT(pszAppKey);

    return ::RegCreateKeyEx(hRoot, pszAppKey, 0, pClass, dwOptions, samDesired, psa, &m_hHandle, pdwDisposition);
}

inline LONG CRegKey::QueryValue(LPCTSTR pszName, DWORD& cbSize, LPDWORD pdwType/* = NULL*/) const
{
    ATLASSERT(!IsEmpty());
    return ::RegQueryValueEx(m_hHandle, pszName, NULL, pdwType, NULL, &cbSize);
}

inline LONG CRegKey::GetValue(LPCTSTR pszName, LPVOID pValue, DWORD cbSize) const
{
    ATLASSERT(!IsEmpty());
    return ::RegQueryValueEx(m_hHandle, pszName, NULL, NULL, (LPBYTE)pValue, &cbSize);
}

inline LONG CRegKey::SetValue(LPCTSTR pszName, LPCVOID pValue, DWORD cbSize, DWORD dwType/* = REG_DWORD*/)
{
    ATLASSERT(!IsEmpty());
    return ::RegSetValueEx(m_hHandle, pszName, 0, dwType, (const BYTE*)pValue, cbSize);
}

#if defined(__ATLSTR_H__) || defined(__AFXSTR_H__)
inline LONG CRegKey::GetStringValue(LPCTSTR pszName, CString& strValue) const
{
    ATLASSERT(!IsEmpty());

    DWORD cbSize = 0;
    LONG lResult = ::RegQueryValueEx(m_hHandle, pszName, NULL, NULL, NULL, &cbSize);

    if (lResult == ERROR_SUCCESS)
    {
        lResult = ::RegQueryValueEx(m_hHandle, pszName, NULL, NULL, (LPBYTE)strValue.GetBuffer(cbSize / sizeof(TCHAR)), &cbSize);
        strValue.ReleaseBuffer(cbSize / sizeof(TCHAR));
    }

    return lResult;
}
#endif  // defined(__ATLSTR_H__) || defined(__AFXSTR_H__)

inline LONG CRegKey::SetMultiStringValue(LPCTSTR pszName, LPCTSTR pszValue, DWORD cbSize/* = 0*/)
{
    ATLASSERT(!IsEmpty());

    if (cbSize == 0)
    {
        LPCTSTR pszTemp = pszValue;
        for (int nLength = 0; nLength != 1; pszTemp += nLength)
        {
            nLength = ::lstrlen(pszTemp) + 1;
            cbSize += nLength * sizeof(TCHAR);
        }
    }

    return ::RegSetValueEx(m_hHandle, pszName, 0, REG_MULTI_SZ, (const BYTE*)pszValue, cbSize);
}

inline LONG CRegKey::GetStringValue(LPCTSTR pszName, LPTSTR pszValue, UINT nLength) const
{
    ATLASSERT(!IsEmpty());

    DWORD cbSize = nLength * sizeof(TCHAR);
    return ::RegQueryValueEx(m_hHandle, pszName, NULL, NULL, (LPBYTE)pszValue, &cbSize);
}

inline LONG CRegKey::SetStringValue(LPCTSTR pszName, LPCTSTR pszValue, DWORD dwType/* = REG_SZ*/)
{
    ATLASSERT(pszValue);
    ATLASSERT(!IsEmpty());
    ATLASSERT(dwType == REG_SZ || dwType == REG_EXPAND_SZ);

    return ::RegSetValueEx(m_hHandle, pszName, 0, dwType, (const BYTE*)pszValue, (::lstrlen(pszValue) + 1) * sizeof(TCHAR));
}

inline LONG CRegKey::GetBinaryValue(LPCTSTR pszName, LPVOID pValue, DWORD& cbSize) const
{
    ATLASSERT(!IsEmpty());
    return ::RegQueryValueEx(m_hHandle, pszName, NULL, NULL, (LPBYTE)pValue, &cbSize);
}

inline LONG CRegKey::SetBinaryValue(LPCTSTR pszName, LPCVOID pValue, DWORD cbSize)
{
    ATLASSERT(!IsEmpty());
    return ::RegSetValueEx(m_hHandle, pszName, 0, REG_BINARY, (const BYTE*)pValue, cbSize);
}

template <typename _Ty>
inline LONG CRegKey::GetIntegerValue(LPCTSTR pszName, _Ty& _Value) const
{
    ATLASSERT(sizeof(_Ty) == sizeof(DWORD) || sizeof(_Ty) == sizeof(ULONGLONG));
    return GetValue(pszName, &_Value, sizeof(_Ty));
}

template <typename _Ty>
inline LONG CRegKey::SetIntegerValue(LPCTSTR pszName, _Ty _Value, DWORD dwType/* = REG_DWORD*/)
{
    ATLASSERT(!IsEmpty());
    return ::RegSetValueEx(m_hHandle, pszName, 0, dwType, (const BYTE*)&_Value, sizeof(_Ty));
}

}  // namespace atlutil

#endif  // __REGUTIL_INL__