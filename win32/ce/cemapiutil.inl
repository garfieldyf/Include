///////////////////////////////////////////////////////////////////////////////
// cemapiutil.inl
//
// Copyright(c) 2019, Garfield. All rights reserved.
// Author  : Garfield
// Version : 1.0
// Creation Date : 2009/12/14

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __CEMAPIUTIL_INL__
#define __CEMAPIUTIL_INL__

#ifndef __CEMAPIUTIL_H__
    #error cemapiutil.inl requires cemapiutil.h to be included first
#endif

namespace WCE {

///////////////////////////////////////////////////////////////////////////////
// Type definition
//

typedef CSPropArrayTraits<ADRENTRY, ADRLIST> CAdrListTraits;
typedef CSPropArrayTraits<SPropProblem, SPropProblemArray> CSPropProblemArrayTraits;


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CSRowSetTraits class
//

inline BOOL WINAPI CSRowSetTraits::Destroy(HANDLET_PTR hHandle)
{
    ::FreeProws(hHandle);
    return TRUE;
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CSRowSet class
//

inline CSRowSet::CSRowSet()
{
}

inline LPSRowSet* CSRowSet::GetAddress()
{
    Destroy();
    return &m_hHandle;
}

inline ULONG CSRowSet::GetRows() const
{
    ATLASSERT(m_hHandle);
    return m_hHandle->cRows;
}

inline ULONG CSRowSet::GetCols() const
{
    ATLASSERT(m_hHandle);
    ATLASSERT(m_hHandle->cRows > 0);

    return m_hHandle->aRow[0].cValues;
}

inline ULONG CSRowSet::GetPropTag(ULONG ulRow, ULONG ulCol) const
{
    ATLASSERT(m_hHandle);
    ATLASSERT(m_hHandle->cRows > 0);
    ATLASSERT(ulRow < m_hHandle->cRows);
    ATLASSERT(m_hHandle->aRow[0].cValues > 0);
    ATLASSERT(ulCol < m_hHandle->aRow[0].cValues);

    return m_hHandle->aRow[ulRow].lpProps[ulCol].ulPropTag;
}

inline ULONG CSRowSet::GetPropType(ULONG ulRow, ULONG ulCol) const
{
    ATLASSERT(m_hHandle);
    ATLASSERT(m_hHandle->cRows > 0);
    ATLASSERT(ulRow < m_hHandle->cRows);
    ATLASSERT(m_hHandle->aRow[0].cValues > 0);
    ATLASSERT(ulCol < m_hHandle->aRow[0].cValues);

    return PROP_TYPE(m_hHandle->aRow[ulRow].lpProps[ulCol].ulPropTag);
}

inline LONG CSRowSet::GetLong(ULONG ulRow, ULONG ulCol) const
{
    ATLASSERT(m_hHandle);
    ATLASSERT(m_hHandle->cRows > 0);
    ATLASSERT(ulRow < m_hHandle->cRows);
    ATLASSERT(m_hHandle->aRow[0].cValues > 0);
    ATLASSERT(ulCol < m_hHandle->aRow[0].cValues);
    ATLASSERT(PROP_TYPE(m_hHandle->aRow[ulRow].lpProps[ulCol].ulPropTag) == PT_LONG || PROP_TYPE(m_hHandle->aRow[ulRow].lpProps[ulCol].ulPropTag) == PT_ERROR);

    return m_hHandle->aRow[ulRow].lpProps[ulCol].Value.l;
}

inline BOOL CSRowSet::GetBool(ULONG ulRow, ULONG ulCol) const
{
    ATLASSERT(m_hHandle);
    ATLASSERT(m_hHandle->cRows > 0);
    ATLASSERT(ulRow < m_hHandle->cRows);
    ATLASSERT(m_hHandle->aRow[0].cValues > 0);
    ATLASSERT(ulCol < m_hHandle->aRow[0].cValues);
    ATLASSERT(PROP_TYPE(m_hHandle->aRow[ulRow].lpProps[ulCol].ulPropTag) == PT_BOOLEAN);

    return m_hHandle->aRow[ulRow].lpProps[ulCol].Value.b;
}

inline short CSRowSet::GetShort(ULONG ulRow, ULONG ulCol) const
{
    ATLASSERT(m_hHandle);
    ATLASSERT(m_hHandle->cRows > 0);
    ATLASSERT(ulRow < m_hHandle->cRows);
    ATLASSERT(m_hHandle->aRow[0].cValues > 0);
    ATLASSERT(ulCol < m_hHandle->aRow[0].cValues);
    ATLASSERT(PROP_TYPE(m_hHandle->aRow[ulRow].lpProps[ulCol].ulPropTag) == PT_SHORT);

    return m_hHandle->aRow[ulRow].lpProps[ulCol].Value.i;
}

inline float CSRowSet::GetFloat(ULONG ulRow, ULONG ulCol) const
{
    ATLASSERT(m_hHandle);
    ATLASSERT(m_hHandle->cRows > 0);
    ATLASSERT(ulRow < m_hHandle->cRows);
    ATLASSERT(m_hHandle->aRow[0].cValues > 0);
    ATLASSERT(ulCol < m_hHandle->aRow[0].cValues);
    ATLASSERT(PROP_TYPE(m_hHandle->aRow[ulRow].lpProps[ulCol].ulPropTag) == PT_R4);

    return m_hHandle->aRow[ulRow].lpProps[ulCol].Value.flt;
}

inline double CSRowSet::GetDouble(ULONG ulRow, ULONG ulCol) const
{
    ATLASSERT(m_hHandle);
    ATLASSERT(m_hHandle->cRows > 0);
    ATLASSERT(ulRow < m_hHandle->cRows);
    ATLASSERT(m_hHandle->aRow[0].cValues > 0);
    ATLASSERT(ulCol < m_hHandle->aRow[0].cValues);
    ATLASSERT(PROP_TYPE(m_hHandle->aRow[ulRow].lpProps[ulCol].ulPropTag) == PT_DOUBLE || PROP_TYPE(m_hHandle->aRow[ulRow].lpProps[ulCol].ulPropTag) == PT_APPTIME);

    return m_hHandle->aRow[ulRow].lpProps[ulCol].Value.dbl;
}

inline LONGLONG CSRowSet::GetInt64(ULONG ulRow, ULONG ulCol) const
{
    ATLASSERT(m_hHandle);
    ATLASSERT(m_hHandle->cRows > 0);
    ATLASSERT(ulRow < m_hHandle->cRows);
    ATLASSERT(m_hHandle->aRow[0].cValues > 0);
    ATLASSERT(ulCol < m_hHandle->aRow[0].cValues);
    ATLASSERT(PROP_TYPE(m_hHandle->aRow[ulRow].lpProps[ulCol].ulPropTag) == PT_I8);

    return m_hHandle->aRow[ulRow].lpProps[ulCol].Value.li.QuadPart;
}

inline ULONG CSRowSet::GetSize(ULONG ulRow, ULONG ulCol) const
{
    ATLASSERT(m_hHandle);
    ATLASSERT(m_hHandle->cRows > 0);
    ATLASSERT(ulRow < m_hHandle->cRows);
    ATLASSERT(m_hHandle->aRow[0].cValues > 0);
    ATLASSERT(ulCol < m_hHandle->aRow[0].cValues);
    ATLASSERT(PROP_TYPE(m_hHandle->aRow[ulRow].lpProps[ulCol].ulPropTag) == PT_BINARY);

    return m_hHandle->aRow[ulRow].lpProps[ulCol].Value.bin.cb;
}

inline LPBYTE CSRowSet::GetBinary(ULONG ulRow, ULONG ulCol) const
{
    ATLASSERT(m_hHandle);
    ATLASSERT(m_hHandle->cRows > 0);
    ATLASSERT(ulRow < m_hHandle->cRows);
    ATLASSERT(m_hHandle->aRow[0].cValues > 0);
    ATLASSERT(ulCol < m_hHandle->aRow[0].cValues);
    ATLASSERT(PROP_TYPE(m_hHandle->aRow[ulRow].lpProps[ulCol].ulPropTag) == PT_BINARY);

    return m_hHandle->aRow[ulRow].lpProps[ulCol].Value.bin.lpb;
}

inline LPCSTR CSRowSet::GetStringA(ULONG ulRow, ULONG ulCol) const
{
    ATLASSERT(m_hHandle);
    ATLASSERT(m_hHandle->cRows > 0);
    ATLASSERT(ulRow < m_hHandle->cRows);
    ATLASSERT(m_hHandle->aRow[0].cValues > 0);
    ATLASSERT(ulCol < m_hHandle->aRow[0].cValues);
    ATLASSERT(PROP_TYPE(m_hHandle->aRow[ulRow].lpProps[ulCol].ulPropTag) == PT_STRING8);

    return m_hHandle->aRow[ulRow].lpProps[ulCol].Value.lpszA;
}

inline LPCWSTR CSRowSet::GetStringW(ULONG ulRow, ULONG ulCol) const
{
    ATLASSERT(m_hHandle);
    ATLASSERT(m_hHandle->cRows > 0);
    ATLASSERT(ulRow < m_hHandle->cRows);
    ATLASSERT(m_hHandle->aRow[0].cValues > 0);
    ATLASSERT(ulCol < m_hHandle->aRow[0].cValues);
    ATLASSERT(PROP_TYPE(m_hHandle->aRow[ulRow].lpProps[ulCol].ulPropTag) == PT_UNICODE);

    return m_hHandle->aRow[ulRow].lpProps[ulCol].Value.lpszW;
}

inline const GUID* CSRowSet::GetGUID(ULONG ulRow, ULONG ulCol) const
{
    ATLASSERT(m_hHandle);
    ATLASSERT(m_hHandle->cRows > 0);
    ATLASSERT(ulRow < m_hHandle->cRows);
    ATLASSERT(m_hHandle->aRow[0].cValues > 0);
    ATLASSERT(ulCol < m_hHandle->aRow[0].cValues);
    ATLASSERT(PROP_TYPE(m_hHandle->aRow[ulRow].lpProps[ulCol].ulPropTag) == PT_CLSID);

    return m_hHandle->aRow[ulRow].lpProps[ulCol].Value.lpguid;
}

inline const CURRENCY& CSRowSet::GetCurrency(ULONG ulRow, ULONG ulCol) const
{
    ATLASSERT(m_hHandle);
    ATLASSERT(m_hHandle->cRows > 0);
    ATLASSERT(ulRow < m_hHandle->cRows);
    ATLASSERT(m_hHandle->aRow[0].cValues > 0);
    ATLASSERT(ulCol < m_hHandle->aRow[0].cValues);
    ATLASSERT(PROP_TYPE(m_hHandle->aRow[ulRow].lpProps[ulCol].ulPropTag) == PT_CURRENCY);

    return m_hHandle->aRow[ulRow].lpProps[ulCol].Value.cur;
}

inline const FILETIME& CSRowSet::GetDateTime(ULONG ulRow, ULONG ulCol) const
{
    ATLASSERT(m_hHandle);
    ATLASSERT(m_hHandle->cRows > 0);
    ATLASSERT(ulRow < m_hHandle->cRows);
    ATLASSERT(m_hHandle->aRow[0].cValues > 0);
    ATLASSERT(ulCol < m_hHandle->aRow[0].cValues);
    ATLASSERT(PROP_TYPE(m_hHandle->aRow[ulRow].lpProps[ulCol].ulPropTag) == PT_SYSTIME);

    return m_hHandle->aRow[ulRow].lpProps[ulCol].Value.ft;
}

inline BOOL CSRowSet::GetDateTime(ULONG ulRow, ULONG ulCol, SYSTEMTIME& st) const
{
    ATLASSERT(m_hHandle);
    ATLASSERT(m_hHandle->cRows > 0);
    ATLASSERT(ulRow < m_hHandle->cRows);
    ATLASSERT(m_hHandle->aRow[0].cValues > 0);
    ATLASSERT(ulCol < m_hHandle->aRow[0].cValues);
    ATLASSERT(PROP_TYPE(m_hHandle->aRow[ulRow].lpProps[ulCol].ulPropTag) == PT_SYSTIME);

    FILETIME ft;
    return (::FileTimeToLocalFileTime(&m_hHandle->aRow[ulRow].lpProps[ulCol].Value.ft, &ft) && ::FileTimeToSystemTime(&ft, &st));
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CSPropValue class
//

inline CSPropValue::CSPropValue()
    : m_ulValues(0), m_pPropVal(NULL)
{
}

inline CSPropValue::~CSPropValue()
{
    Empty();
}

inline LPSPropValue* CSPropValue::GetAddress()
{
    Empty();
    return &m_pPropVal;
}

inline void CSPropValue::Empty()
{
    if (m_pPropVal != NULL && ::MAPIFreeBuffer(m_pPropVal) == S_OK)
        m_pPropVal = NULL, m_ulValues = 0;
}

inline ULONG CSPropValue::GetPropTag(ULONG ulIndex) const
{
    ATLASSERT(m_pPropVal);
    ATLASSERT(ulIndex < m_ulValues);

    return m_pPropVal[ulIndex].ulPropTag;
}

inline ULONG CSPropValue::GetPropType(ULONG ulIndex) const
{
    ATLASSERT(m_pPropVal);
    ATLASSERT(ulIndex < m_ulValues);

    return PROP_TYPE(m_pPropVal[ulIndex].ulPropTag);
}

inline LONG CSPropValue::GetLong(ULONG ulIndex) const
{
    ATLASSERT(m_pPropVal);
    ATLASSERT(ulIndex < m_ulValues);
    ATLASSERT(PROP_TYPE(m_pPropVal[ulIndex].ulPropTag) == PT_LONG || PROP_TYPE(m_pPropVal[ulIndex].ulPropTag) == PT_ERROR);

    return m_pPropVal[ulIndex].Value.l;
}

inline BOOL CSPropValue::GetBool(ULONG ulIndex) const
{
    ATLASSERT(m_pPropVal);
    ATLASSERT(ulIndex < m_ulValues);
    ATLASSERT(PROP_TYPE(m_pPropVal[ulIndex].ulPropTag) == PT_BOOLEAN);

    return m_pPropVal[ulIndex].Value.b;
}

inline short CSPropValue::GetShort(ULONG ulIndex) const
{
    ATLASSERT(m_pPropVal);
    ATLASSERT(ulIndex < m_ulValues);
    ATLASSERT(PROP_TYPE(m_pPropVal[ulIndex].ulPropTag) == PT_SHORT);

    return m_pPropVal[ulIndex].Value.i;
}

inline float CSPropValue::GetFloat(ULONG ulIndex) const
{
    ATLASSERT(m_pPropVal);
    ATLASSERT(ulIndex < m_ulValues);
    ATLASSERT(PROP_TYPE(m_pPropVal[ulIndex].ulPropTag) == PT_R4);

    return m_pPropVal[ulIndex].Value.flt;
}

inline double CSPropValue::GetDouble(ULONG ulIndex) const
{
    ATLASSERT(m_pPropVal);
    ATLASSERT(ulIndex < m_ulValues);
    ATLASSERT(PROP_TYPE(m_pPropVal[ulIndex].ulPropTag) == PT_DOUBLE || PROP_TYPE(m_pPropVal[ulIndex].ulPropTag) == PT_APPTIME);

    return m_pPropVal[ulIndex].Value.dbl;
}

inline LONGLONG CSPropValue::GetInt64(ULONG ulIndex) const
{
    ATLASSERT(m_pPropVal);
    ATLASSERT(ulIndex < m_ulValues);
    ATLASSERT(PROP_TYPE(m_pPropVal[ulIndex].ulPropTag) == PT_I8);

    return m_pPropVal[ulIndex].Value.li.QuadPart;
}

inline ULONG CSPropValue::GetSize(ULONG ulIndex) const
{
    ATLASSERT(m_pPropVal);
    ATLASSERT(ulIndex < m_ulValues);
    ATLASSERT(PROP_TYPE(m_pPropVal[ulIndex].ulPropTag) == PT_BINARY);

    return m_pPropVal[ulIndex].Value.bin.cb;
}

inline LPBYTE CSPropValue::GetBinary(ULONG ulIndex) const
{
    ATLASSERT(m_pPropVal);
    ATLASSERT(ulIndex < m_ulValues);
    ATLASSERT(PROP_TYPE(m_pPropVal[ulIndex].ulPropTag) == PT_BINARY);

    return m_pPropVal[ulIndex].Value.bin.lpb;
}

inline LPCSTR CSPropValue::GetStringA(ULONG ulIndex) const
{
    ATLASSERT(m_pPropVal);
    ATLASSERT(ulIndex < m_ulValues);
    ATLASSERT(PROP_TYPE(m_pPropVal[ulIndex].ulPropTag) == PT_STRING8);

    return m_pPropVal[ulIndex].Value.lpszA;
}

inline LPCWSTR CSPropValue::GetStringW(ULONG ulIndex) const
{
    ATLASSERT(m_pPropVal);
    ATLASSERT(ulIndex < m_ulValues);
    ATLASSERT(PROP_TYPE(m_pPropVal[ulIndex].ulPropTag) == PT_UNICODE);

    return m_pPropVal[ulIndex].Value.lpszW;
}

inline const GUID* CSPropValue::GetGUID(ULONG ulIndex) const
{
    ATLASSERT(m_pPropVal);
    ATLASSERT(ulIndex < m_ulValues);
    ATLASSERT(PROP_TYPE(m_pPropVal[ulIndex].ulPropTag) == PT_CLSID);

    return m_pPropVal[ulIndex].Value.lpguid;
}

inline const CURRENCY& CSPropValue::GetCurrency(ULONG ulIndex) const
{
    ATLASSERT(m_pPropVal);
    ATLASSERT(ulIndex < m_ulValues);
    ATLASSERT(PROP_TYPE(m_pPropVal[ulIndex].ulPropTag) == PT_CURRENCY);

    return m_pPropVal[ulIndex].Value.cur;
}

inline const FILETIME& CSPropValue::GetDateTime(ULONG ulIndex) const
{
    ATLASSERT(m_pPropVal);
    ATLASSERT(ulIndex < m_ulValues);
    ATLASSERT(PROP_TYPE(m_pPropVal[ulIndex].ulPropTag) == PT_SYSTIME);

    return m_pPropVal[ulIndex].Value.ft;
}

inline BOOL CSPropValue::GetDateTime(ULONG ulIndex, SYSTEMTIME& st) const
{
    ATLASSERT(m_pPropVal);
    ATLASSERT(ulIndex < m_ulValues);
    ATLASSERT(PROP_TYPE(m_pPropVal[ulIndex].ulPropTag) == PT_SYSTIME);

    FILETIME ft;
    return (::FileTimeToLocalFileTime(&m_pPropVal[ulIndex].Value.ft, &ft) && ::FileTimeToSystemTime(&ft, &st));
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CSPropArray class
//

template <typename _PropArrayTraits, ULONG cValues>
inline CSPropArray<_PropArrayTraits, cValues>::CSPropArray()
    : m_ulValues(cValues)
{
}

template <typename _PropArrayTraits, ULONG cValues>
inline CSPropArray<_PropArrayTraits, cValues>::CSPropArray(_RefProp _Value)
    : m_ulValues(cValues)
{
    ATLASSERT(cValues > 0);
    m_Props[0] = _Value;
}

template <typename _PropArrayTraits, ULONG cValues>
inline CSPropArray<_PropArrayTraits, cValues>::CSPropArray(_RefProp _Value, _RefProp _Value2)
    : m_ulValues(cValues)
{
    ATLASSERT(cValues > 1);

    m_Props[0] = _Value;
    m_Props[1] = _Value2;
}

template <typename _PropArrayTraits, ULONG cValues>
inline CSPropArray<_PropArrayTraits, cValues>::CSPropArray(_RefProp _Value, _RefProp _Value2, _RefProp _Value3)
    : m_ulValues(cValues)
{
    ATLASSERT(cValues > 2);

    m_Props[0] = _Value;
    m_Props[1] = _Value2;
    m_Props[2] = _Value3;
}

template <typename _PropArrayTraits, ULONG cValues>
inline CSPropArray<_PropArrayTraits, cValues>::operator LPPropArray()
{
    return reinterpret_cast<LPPropArray>(this);
}

template <typename _PropArrayTraits, ULONG cValues>
inline CSPropArray<_PropArrayTraits, cValues>::operator LPCPropArray() const
{
    return reinterpret_cast<LPCPropArray>(this);
}


#ifndef __ATLCOMUTIL_H__
///////////////////////////////////////////////////////////////////////////////
// Implementation of the IMAPIAdviseSinkImpl class
//

template <typename T>
inline HRESULT IMAPIAdviseSinkImpl<T>::IMAPIAdviseSink_Advise(IMsgStore* pStore, ULONG ulEventMask)
{
    ATLASSERT(pSession);
    ATLASSERT(m_dwCookie == INVALID_COOKIE_VALUE);

    return pStore->Advise(0, NULL, ulEventMask, (IMAPIAdviseSink*)this, &m_dwCookie);       // must be C style cast
}

template <typename T>
inline HRESULT IMAPIAdviseSinkImpl<T>::IMAPIAdviseSink_Advise(IMAPISession* pSession, ULONG ulEventMask)
{
    ATLASSERT(pSession);
    ATLASSERT(m_dwCookie == INVALID_COOKIE_VALUE);

    return pSession->Advise(0, NULL, ulEventMask, (IMAPIAdviseSink*)this, &m_dwCookie);     // must be C style cast
}

template <typename T>
inline HRESULT IMAPIAdviseSinkImpl<T>::IMAPIAdviseSink_Unadvise(IMsgStore* pStore)
{
    ATLASSERT(pSession);
    ATLASSERT(m_dwCookie != INVALID_COOKIE_VALUE);

    return pStore->Unadvise(m_dwCookie);
}

template <typename T>
inline HRESULT IMAPIAdviseSinkImpl<T>::IMAPIAdviseSink_Unadvise(IMAPISession* pSession)
{
    ATLASSERT(pSession);
    ATLASSERT(m_dwCookie != INVALID_COOKIE_VALUE);

    return pSession->Unadvise(m_dwCookie);
}

template <typename T>
inline STDMETHODIMP_(ULONG) IMAPIAdviseSinkImpl<T>::OnNotify(ULONG /*cNotif*/, LPNOTIFICATION pNotifications)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);

    ULONG ulResult = 0;
    switch (pNotifications->ulEventType)
    {
    case fnevExtended:
        ulResult = pThis->IMAPIAdviseSink_Extended(&pNotifications->info.ext);
        break;

    case fnevNewMail:
        ulResult = pThis->IMAPIAdviseSink_NewMail(&pNotifications->info.newmail);
        break;

    case fnevObjectMoved:
        ulResult = pThis->IMAPIAdviseSink_ObjectMoved(&pNotifications->info.obj);
        break;

    case fnevObjectCopied:
        ulResult = pThis->IMAPIAdviseSink_ObjectCopied(&pNotifications->info.obj);
        break;

    case fnevObjectCreated:
        ulResult = pThis->IMAPIAdviseSink_ObjectCreated(&pNotifications->info.obj);
        break;

    case fnevObjectDeleted:
        ulResult = pThis->IMAPIAdviseSink_ObjectDeleted(&pNotifications->info.obj);
        break;

    case fnevCriticalError:
        ulResult = pThis->IMAPIAdviseSink_CriticalError(&pNotifications->info.err);
        break;

    case fnevTableModified:
        ulResult = pThis->IMAPIAdviseSink_TableModified(&pNotifications->info.tab);
        break;

    case fnevObjectModified:
        ulResult = pThis->IMAPIAdviseSink_ObjectModified(&pNotifications->info.obj);
        break;

    case fnevSearchComplete:
        ulResult = pThis->IMAPIAdviseSink_SearchComplete(&pNotifications->info.obj);
        break;
    }

    return ulResult;
}

template <typename T>
inline ULONG IMAPIAdviseSinkImpl<T>::IMAPIAdviseSink_NewMail(NEWMAIL_NOTIFICATION* /*pNotifications*/)
{
    return 0;
}

template <typename T>
inline ULONG IMAPIAdviseSinkImpl<T>::IMAPIAdviseSink_Extended(EXTENDED_NOTIFICATION* /*pNotifications*/)
{
    return 0;
}

template <typename T>
inline ULONG IMAPIAdviseSinkImpl<T>::IMAPIAdviseSink_ObjectMoved(OBJECT_NOTIFICATION* /*pNotifications*/)
{
    return 0;
}

template <typename T>
inline ULONG IMAPIAdviseSinkImpl<T>::IMAPIAdviseSink_CriticalError(ERROR_NOTIFICATION* /*pNotifications*/)
{
    return 0;
}

template <typename T>
inline ULONG IMAPIAdviseSinkImpl<T>::IMAPIAdviseSink_TableModified(TABLE_NOTIFICATION* /*pNotifications*/)
{
    return 0;
}

template <typename T>
inline ULONG IMAPIAdviseSinkImpl<T>::IMAPIAdviseSink_ObjectCopied(OBJECT_NOTIFICATION* /*pNotifications*/)
{
    return 0;
}

template <typename T>
inline ULONG IMAPIAdviseSinkImpl<T>::IMAPIAdviseSink_ObjectCreated(OBJECT_NOTIFICATION* /*pNotifications*/)
{
    return 0;
}

template <typename T>
inline ULONG IMAPIAdviseSinkImpl<T>::IMAPIAdviseSink_ObjectDeleted(OBJECT_NOTIFICATION* /*pNotifications*/)
{
    return 0;
}

template <typename T>
inline ULONG IMAPIAdviseSinkImpl<T>::IMAPIAdviseSink_ObjectModified(OBJECT_NOTIFICATION* /*pNotifications*/)
{
    return 0;
}

template <typename T>
inline ULONG IMAPIAdviseSinkImpl<T>::IMAPIAdviseSink_SearchComplete(OBJECT_NOTIFICATION* /*pNotifications*/)
{
    return 0;
}
#endif  // __ATLCOMUTIL_H__


///////////////////////////////////////////////////////////////////////////////
// Implementation of the IMailRuleClientImpl class
//

template <typename T>
inline HRESULT WINAPI IMailRuleClientImpl<T>::IMailRuleClient_RegisterServer()
{
    OLECHAR szClsID[64];
    ATLVERIFY(::StringFromGUID2(T::GetObjectCLSID(), szClsID, _countof(szClsID)));

    TCHAR szModulePath[MAX_PATH];
    ATLVERIFY(::GetModuleFileName(ATL::_AtlBaseModule.GetModuleInstance(), szModulePath, _countof(szModulePath)));

    HRESULT hr = sysutil::CoRegisterServer(szModulePath, szClsID);
    if (SUCCEEDED(hr))
        hr = ::RegistrySetDWORD(HKEY_LOCAL_MACHINE, _T("Software\\Microsoft\\Inbox\\Svc\\SMS\\Rules"), szClsID, 1);

    return hr;
}

template <typename T>
inline HRESULT WINAPI IMailRuleClientImpl<T>::IMailRuleClient_UnregisterServer()
{
    OLECHAR szClsID[64];
    ATLVERIFY(::StringFromGUID2(T::GetObjectCLSID(), szClsID, _countof(szClsID)));

    TCHAR szSubKey[64];
    ::_tssprintf(szSubKey, _countof(szSubKey), _T("CLSID\\%s"), szClsID);
    _COND_VERIFY(::RegDeleteKey(HKEY_CLASSES_ROOT, szSubKey), ERROR_SUCCESS);

    return ::RegistryDeleteValue(HKEY_LOCAL_MACHINE, _T("Software\\Microsoft\\Inbox\\Svc\\SMS\\Rules"), szClsID);
}

template <typename T>
inline STDMETHODIMP IMailRuleClientImpl<T>::Initialize(IMsgStore* /*pStore*/, MRCACCESS* pmaDesired)
{
    *pmaDesired = MRC_ACCESS_WRITE;
    return S_OK;
}

template <typename T>
inline STDMETHODIMP IMailRuleClientImpl<T>::ProcessMessage(IMsgStore* pStore, ULONG cbMsg, LPENTRYID pMsg, ULONG cbDestFolder, LPENTRYID pDestFolder, ULONG* pulEventType, MRCHANDLED* pHandled)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);

    return pThis->IMailRuleClient_ProcessMessage(pStore, cbMsg, pMsg, cbDestFolder, pDestFolder, pulEventType, pHandled);
}

template <typename T>
inline HRESULT IMailRuleClientImpl<T>::IMailRuleClient_ProcessMessage(IMsgStore* /*pStore*/, ULONG /*cbMsg*/, LPENTRYID /*pMsg*/, ULONG /*cbDestFolder*/, LPENTRYID /*pDestFolder*/, ULONG* /*pulEventType*/, MRCHANDLED* /*pHandled*/)
{
    return S_OK;
}

}  // namespace WCE

#endif  // __CEMAPIUTIL_INL__