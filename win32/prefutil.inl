///////////////////////////////////////////////////////////////////////////////
// prefutil.inl
//
// Copyright(c) 2019, Garfield. All rights reserved.
// Author  : Garfield
// Version : 1.0
// Creation Date : 2010/4/26

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __PREFUTIL_INL__
#define __PREFUTIL_INL__

#ifndef __PREFUTIL_H__
    #error prefutil.inl requires prefutil.h to be included first
#endif

namespace stdutil {

///////////////////////////////////////////////////////////////////////////////
// Implementation of the CProperties class
//

inline CProperties::CProperties()
{
}

inline POSITION CProperties::GetStartPosition() const
{
    return m_mapProperties.GetStartPosition();
}

inline const CProperties::CProperty* CProperties::GetNext(POSITION& pos) const
{
    ATLASSERT(pos);
    return m_mapProperties.GetNext(pos);
}

inline void CProperties::RemoveAll()
{
    return m_mapProperties.RemoveAll();
}

inline BOOL CProperties::Remove(LPCTSTR pszName)
{
    ATLASSERT(pszName);
    return m_mapProperties.RemoveKey(pszName);
}

#ifdef _OBJBASE_H_
inline void CProperties::SetGUID(LPCTSTR pszName, const GUID& guidValue)
{
    ATLASSERT(pszName);

    OLECHAR szValue[64];
    ATLVERIFY(::StringFromGUID2(guidValue, szValue, _countof(szValue)));
    m_mapProperties[pszName] = szValue;
}

inline BOOL CProperties::GetGUID(LPCTSTR pszName, GUID& guidValue) const
{
    ATLASSERT(pszName);

    const CProperty* pProperty = m_mapProperties.Lookup(pszName);
    return (pProperty != NULL && SUCCEEDED(::CLSIDFromString((LPOLESTR)(LPCTSTR)pProperty->m_value, &guidValue)));
}
#endif  // _OBJBASE_H_

inline void CProperties::SetDate(LPCTSTR pszName, const SYSTEMTIME& dtValue)
{
    ATLASSERT(pszName);

    CString strValue;
    strValue.Format(_T("%hu/%hu/%hu %hu:%hu:%hu.%hu %hu"), dtValue.wYear, dtValue.wMonth, dtValue.wDay, dtValue.wHour, dtValue.wMinute, dtValue.wSecond, dtValue.wMilliseconds, dtValue.wDayOfWeek);
    m_mapProperties[pszName] = strValue;
}

inline BOOL CProperties::GetDate(LPCTSTR pszName, SYSTEMTIME& dtValue) const
{
    ATLASSERT(pszName);

    BOOL bSuccessful = FALSE;
    if (const CProperty* pProperty = m_mapProperties.Lookup(pszName))
        bSuccessful = (::_tsscanf(pProperty->m_value, _T("%hu/%hu/%hu %hu:%hu:%hu.%hu %hu"), &dtValue.wYear, &dtValue.wMonth, &dtValue.wDay, &dtValue.wHour, &dtValue.wMinute, &dtValue.wSecond, &dtValue.wMilliseconds, &dtValue.wDayOfWeek) == 8);

    return bSuccessful;
}

#ifdef __ATLCOMTIME_H__
inline void CProperties::SetDate(LPCTSTR pszName, const COleDateTime& dtValue)
{
    ATLASSERT(pszName);
    SetBinary(pszName, (const BYTE*)&dtValue, sizeof(COleDateTime));
}

inline BOOL CProperties::GetDate(LPCTSTR pszName, COleDateTime& dtValue) const
{
    ATLASSERT(pszName);
    return (GetBinary(pszName, (LPBYTE)&dtValue, sizeof(COleDateTime)) == sizeof(COleDateTime));
}
#endif  // __ATLCOMTIME_H__

inline void CProperties::SetString(LPCTSTR pszName, LPCTSTR pszValue)
{
    ATLASSERT(pszName);
    m_mapProperties[pszName] = pszValue;
}

inline LPCTSTR CProperties::GetString(LPCTSTR pszName, LPCTSTR pszDefault/* = _T("")*/) const
{
    ATLASSERT(pszName);

    if (const CProperty* pProperty = m_mapProperties.Lookup(pszName))
        pszDefault = pProperty->m_value;

    return pszDefault;
}

inline void CProperties::SetBinary(LPCTSTR pszName, const BYTE* pValue, DWORD cbSize)
{
    ATLASSERT(pValue);
    ATLASSERT(pszName);
    ATLASSERT(cbSize > 0);

    CString strValue;
    if (LPTSTR pszBuffer = strValue.GetBuffer(cbSize << 1))
    {
        ::__tohexstring(pValue, cbSize, pszBuffer);
        strValue.ReleaseBuffer(cbSize << 1);
    }

    m_mapProperties[pszName] = strValue;
}

inline DWORD CProperties::GetBinary(LPCTSTR pszName, LPBYTE pValue, DWORD cbSize) const
{
    ATLASSERT(pszName);

    DWORD dwByteSize = 0;
    if (const CProperty* pProperty = m_mapProperties.Lookup(pszName))
    {
        ATLASSERT(pProperty->m_value.GetLength() % 2 == 0);

        dwByteSize = pProperty->m_value.GetLength() >> 1;
        if (pValue != NULL && cbSize >= dwByteSize)
            ::__tobytearray(pValue, (LPCTSTR)pProperty->m_value, 0, pProperty->m_value.GetLength());
    }

    return dwByteSize;
}

inline void CProperties::SetInteger(LPCTSTR pszName, LONGLONG nValue)
{
    ATLASSERT(pszName);

    CString strValue;
    strValue.Format(_T("%I64d"), nValue);
    m_mapProperties[pszName] = strValue;
}

inline LONGLONG CProperties::GetInteger(LPCTSTR pszName, LONGLONG nDefault/* = 0*/) const
{
    ATLASSERT(pszName);

    if (const CProperty* pProperty = m_mapProperties.Lookup(pszName))
        nDefault = ::_ttoi64(pProperty->m_value);

    return nDefault;
}

template <typename _Ty>
inline void CProperties::SetInteger(LPCTSTR pszName, _Ty _Value)
{
    SetInteger(pszName, (LONGLONG)_Value);
}

template <typename _Ty>
inline _Ty CProperties::GetInteger(LPCTSTR pszName, _Ty _Default) const
{
#pragma warning(push)
#pragma warning(disable: 4800)
    return (_Ty)GetInteger(pszName, (LONGLONG)_Default);
#pragma warning(pop)
}

template <typename _Ty>
inline void CProperties::SetDecimal(LPCTSTR pszName, _Ty _Value, int nDigit/* = 6*/)
{
    ATLASSERT(pszName);

    CString strValue;
    strValue.Format(_T("%.*f"), nDigit, _Value);
    m_mapProperties[pszName] = strValue;
}

template <typename _Ty>
inline _Ty CProperties::GetDecimal(LPCTSTR pszName, _Ty _Default) const
{
    ATLASSERT(pszName);

    if (const CProperty* pProperty = m_mapProperties.Lookup(pszName))
        _Default = (_Ty)::_tstof(pProperty->m_value);

    return _Default;
}

#ifdef _CRT_DEBUG_DUMP
template <typename _Pr>
inline void CProperties::Dump(const _Pr& _Printer) const
{
    _Printer(_T("\n[ PROPERTY COUNT = %u ]\n[\n"), m_mapProperties.GetCount());
    for (POSITION pos = m_mapProperties.GetStartPosition(); pos != NULL; )
    {
        const CProperty* pProperty = m_mapProperties.GetNext(pos);
        ATLASSERT(pProperty);

        _Printer(_T("\t%s = %s\n"), pProperty->m_key, pProperty->m_value);
    }

    _Printer(_T("]\n"));
}
#endif  // _CRT_DEBUG_DUMP

inline DWORD CProperties::GetCount() const
{
    return m_mapProperties.GetCount();
}


#if defined(__msxml2_h__) && defined(__ATLCOMCLI_H__)
///////////////////////////////////////////////////////////////////////////////
// Implementation of the CPreferences class
//

inline CPreferences::CPreferences()
{
}

inline HRESULT CPreferences::Load(const VARIANT& vtSource)
{
    ATLASSERT(vtSource.vt != VT_EMPTY);
    CComPtr<IXMLDOMDocument> spDocument;
    m_vtSource = vtSource;

    HRESULT hr = spDocument.CoCreateInstance(__uuidof(DOMDocument));
    if (SUCCEEDED(hr))
    {
        _COM_VERIFY(spDocument->put_async(VARIANT_FALSE));
        _COM_VERIFY(spDocument->put_validateOnParse(VARIANT_FALSE));
        _COM_VERIFY(spDocument->put_preserveWhiteSpace(VARIANT_TRUE));

        VARIANT_BOOL bSuccessful = VARIANT_FALSE;
        hr = spDocument->load(m_vtSource, &bSuccessful);
        if (bSuccessful)
            hr = LoadPreferences(spDocument);
    }

    return hr;
}

inline HRESULT CPreferences::Save() const
{
    ATLASSERT(m_vtSource.vt != VT_EMPTY);

    CComPtr<IXMLDOMDocument> spDocument;
    HRESULT hr = spDocument.CoCreateInstance(__uuidof(DOMDocument));
    if (SUCCEEDED(hr))
    {
        VARIANT_BOOL bSuccessful = VARIANT_FALSE;
        hr = spDocument->loadXML(CComBSTR(OLESTR("<?xml version='1.0' encoding='utf-8'?><preferences />")), &bSuccessful);
        if (bSuccessful)
            hr = SavePreferences(spDocument);
    }

    return hr;
}

inline HRESULT CPreferences::LoadPreferences(IXMLDOMDocument* pDocument)
{
    ATLASSERT(pDocument);

    CComPtr<IXMLDOMNodeList> spPreferences;
    HRESULT hr = pDocument->selectNodes(CComBSTR(OLESTR("//preferences/*")), &spPreferences);
    if (SUCCEEDED(hr))
    {
        long lCount = 0;
        _COM_VERIFY(spPreferences->get_length(&lCount));

        for (long i = 0; i < lCount; ++i)
        {
            CComPtr<IXMLDOMNode> spItem;
            if (SUCCEEDED(spPreferences->get_item(i, &spItem)))
            {
                CComBSTR bstrName;
                _COM_VERIFY(spItem->get_nodeName(&bstrName));

                CComBSTR bstrValue;
                _COM_VERIFY(spItem->get_text(&bstrValue));

                m_mapProperties[bstrName] = bstrValue;
            }
        }
    }

    return hr;
}

inline HRESULT CPreferences::SavePreferences(IXMLDOMDocument* pDocument) const
{
    ATLASSERT(pDocument);
    ATLASSERT(m_vtSource.vt != VT_EMPTY);

    CComPtr<IXMLDOMElement> spPreferences;
    if (SUCCEEDED(pDocument->get_documentElement(&spPreferences)))
    {
        for (POSITION pos = m_mapProperties.GetStartPosition(); pos != NULL; )
        {
            const CProperty* pProperty = m_mapProperties.GetNext(pos);
            ATLASSERT(pProperty);

            CComPtr<IXMLDOMElement> spItem;
            if (SUCCEEDED(pDocument->createElement(CComBSTR(pProperty->m_key), &spItem)))
            {
                _COM_VERIFY(spItem->put_text(CComBSTR(pProperty->m_value)));
                _COM_VERIFY(spPreferences->appendChild(spItem, NULL));
            }
        }
    }

    return pDocument->save(m_vtSource);
}
#endif  // defined(__msxml2_h__) && defined(__ATLCOMCLI_H__)

}  // namespace stdutil

#endif  // __PREFUTIL_INL__