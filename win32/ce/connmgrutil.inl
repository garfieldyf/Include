///////////////////////////////////////////////////////////////////////////////
// connmgrutil.inl
//
// Author : Garfield
// Creation Date : 2009/12/14

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __CONNMGRUTIL_INL__
#define __CONNMGRUTIL_INL__

#ifndef __CONNMGRUTIL_H__
    #error connmgrutil.inl requires connmgrutil.h to be included first
#endif

namespace WCE {

///////////////////////////////////////////////////////////////////////////////
// Implementation of the CConnectionManagerTraits class
//

inline BOOL WINAPI CConnectionManagerTraits::Destroy(HANDLET_PTR hHandle)
{
    return SUCCEEDED(::ConnMgrReleaseConnection(hHandle, 0L));
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CConnectionManager class
//

inline CConnectionManager::CConnectionManager()
{
}

inline HRESULT CConnectionManager::EstablishConnection(const CONNMGR_CONNECTIONINFO& connInfo)
{
    ATLASSERT(m_hHandle == NULL);
    return ::ConnMgrEstablishConnection((CONNMGR_CONNECTIONINFO*)&connInfo, &m_hHandle);
}

inline DWORD CConnectionManager::EstablishConnectionSync(const CONNMGR_CONNECTIONINFO& connInfo, DWORD dwTimeout/* = 30000*/)
{
    ATLASSERT(m_hHandle == NULL);

    DWORD dwStatus = CONNMGR_STATUS_UNKNOWN;
    _COM_VERIFY(::ConnMgrEstablishConnectionSync((CONNMGR_CONNECTIONINFO*)&connInfo, &m_hHandle, dwTimeout, &dwStatus));

    return dwStatus;
}

inline void CConnectionManager::ReleaseConnection(LONG lCache/* = 0L*/)
{
    if (m_hHandle != NULL && SUCCEEDED(::ConnMgrReleaseConnection(m_hHandle, lCache)))
        m_hHandle = NULL;
}

inline HRESULT CConnectionManager::SetConnectionPriority(DWORD dwPriority)
{
    ATLASSERT(m_hHandle);
    return ::ConnMgrSetConnectionPriority(m_hHandle, dwPriority);
}

inline HRESULT CConnectionManager::ProviderMessage(const GUID& guidProvider, DWORD dwMsg1, DWORD dwMsg2, PBYTE pParams, ULONG cbParamSize, LPDWORD pdwIndex/* = NULL*/)
{
    ATLASSERT(pParams);
    return ::ConnMgrProviderMessage(m_hHandle, &guidProvider, pdwIndex, dwMsg1, dwMsg2, pParams, cbParamSize);
}

inline DWORD CConnectionManager::GetConnectionStatus()
{
    ATLASSERT(m_hHandle);

    DWORD dwStatus = CONNMGR_STATUS_UNKNOWN;
    _COM_VERIFY(::ConnMgrConnectionStatus(m_hHandle, &dwStatus));

    return dwStatus;
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CConnMgrDestInfo class
//

inline CConnMgrDestInfo::CConnMgrDestInfo()
    : m_nIndex(-1)
{
}

inline HRESULT CConnMgrDestInfo::EnumDestinations()
{
    return ::ConnMgrEnumDestinations(++m_nIndex, this);
}

#ifdef _CRT_DEBUG_DUMP
template <typename _Pr>
inline void CConnMgrDestInfo::Dump(const _Pr& _Printer) const
{
    OLECHAR szClsId[64];
    ATLVERIFY(::StringFromGUID2(guid, szClsId, _countof(szClsId)));
    _Printer(_T("CONNECTION MANAGER DESTINATION INFO:\n\t[ Guid   = %s ]\n\t[ Desc   = %s ]\n\t[ Secure = %s ]\n\n"), szClsId, szDescription, (fSecure ? _T("TRUE") : _T("FALSE")));
}
#endif  // _CRT_DEBUG_DUMP


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CConnMgrDetailedStatus class
//

inline CConnMgrDetailedStatus::CConnMgrDetailedStatus()
{
}

inline CONNMGR_CONNECTION_DETAILED_STATUS* CConnMgrDetailedStatus::QueryStatus()
{
    DWORD cbSize = 0;
    if (::ConnMgrQueryDetailedStatus(NULL, &cbSize) != CO_E_INSUFFICIENT_BUFFER || m_buffer.allocateBytes(cbSize) == NULL || FAILED(::ConnMgrQueryDetailedStatus(m_buffer.data, &cbSize)))
        m_buffer.clear();

    return m_buffer.data;
}

}  // namespace WCE

#endif  // __CONNMGRUTIL_INL__