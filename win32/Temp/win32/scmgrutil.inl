///////////////////////////////////////////////////////////////////////////////
// scmgrutil.inl
//
// Copyright(c) 2012, Garfield. All rights reserved.
// Author  : Garfield
// Version : 1.0
// Finish Time : 2009/4/13

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __SCMGRUTIL_INL__
#define __SCMGRUTIL_INL__

namespace atlutil {

///////////////////////////////////////////////////////////////////////////////
// Type definition
//

typedef CServiceConfigInfo<SERVICE_DESCRIPTION, SERVICE_CONFIG_DESCRIPTION> CServiceDescription;
typedef CServiceConfigInfo<SERVICE_FAILURE_ACTIONS, SERVICE_CONFIG_FAILURE_ACTIONS> CServiceFailureActions;

#if (_WIN32_WINNT >= 0x0600)
typedef CServiceConfigInfoT<SERVICE_SID_INFO, SERVICE_CONFIG_SERVICE_SID_INFO> CServiceSidInfo;
typedef CServiceConfigInfoT<SERVICE_PRESHUTDOWN_INFO, SERVICE_CONFIG_PRESHUTDOWN_INFO> CServicePreshutdownInfo;
typedef CServiceConfigInfoT<SERVICE_DELAYED_AUTO_START_INFO, SERVICE_CONFIG_DELAYED_AUTO_START_INFO> CServiceDelayedInfo;
typedef CServiceConfigInfoT<SERVICE_FAILURE_ACTIONS_FLAG, SERVICE_CONFIG_FAILURE_ACTIONS_FLAG> CServiceFailureActionsFlag;
typedef CServiceConfigInfo<SERVICE_REQUIRED_PRIVILEGES_INFO, SERVICE_CONFIG_REQUIRED_PRIVILEGES_INFO> CServicePrivilegesInfo;
#endif  // (_WIN32_WINNT >= 0x0600)


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CServiceHandleTraits class
//

inline BOOL WINAPI CServiceHandleTraits::Destroy(HANDLET_PTR hHandle)
{
    return ::CloseServiceHandle(hHandle);
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CServiceManager class
//

inline CServiceManager::CServiceManager()
{
}

inline SC_HANDLE CServiceManager::Open(DWORD dwDesiredAccess/* = SC_MANAGER_ALL_ACCESS*/, LPCTSTR pszDatabaseName/* = NULL*/, LPCTSTR pszMachineName/* = NULL*/)
{
    ATLASSERT(IsEmpty());
    return (m_hHandle = ::OpenSCManager(pszMachineName, pszDatabaseName, dwDesiredAccess));
}

#if defined(__ATLSTR_H__) || defined(__AFXSTR_H__)
inline CString CServiceManager::GetServiceName(LPCTSTR pszDisplayName)
{
    ATLASSERT(!IsEmpty());
    ATLASSERT(pszDisplayName);

    CString strServiceName;
    DWORD dwBufferLength = 0;

    if (!::GetServiceKeyName(m_hHandle, pszDisplayName, NULL, &dwBufferLength) && ::GetLastError() == ERROR_INSUFFICIENT_BUFFER)
    {
        ATLASSERT(dwBufferLength != 0);
        ATLVERIFY(::GetServiceKeyName(m_hHandle, pszDisplayName, strServiceName.GetBuffer(++dwBufferLength), &dwBufferLength));
        strServiceName.ReleaseBufferSetLength(dwBufferLength);
    }

    return strServiceName;
}

inline CString CServiceManager::GetServiceDisplayName(LPCTSTR pszServiceName)
{
    ATLASSERT(!IsEmpty());
    ATLASSERT(pszServiceName);

    CString strDisplayName;
    DWORD dwBufferLength = 0;

    if (!::GetServiceDisplayName(m_hHandle, pszServiceName, NULL, &dwBufferLength) && ::GetLastError() == ERROR_INSUFFICIENT_BUFFER)
    {
        ATLASSERT(dwBufferLength != 0);
        ATLVERIFY(::GetServiceDisplayName(m_hHandle, pszServiceName, strDisplayName.GetBuffer(++dwBufferLength), &dwBufferLength));
        strDisplayName.ReleaseBufferSetLength(dwBufferLength);
    }

    return strDisplayName;
}
#endif  // defined(__ATLSTR_H__) || defined(__AFXSTR_H__)

inline BOOL CServiceManager::GetServiceName(LPCTSTR pszDisplayName, LPTSTR pszServiceName, DWORD& dwBufferLength)
{
    ATLASSERT(!IsEmpty());
    ATLASSERT(pszDisplayName);

    return ::GetServiceKeyName(m_hHandle, pszDisplayName, pszServiceName, &dwBufferLength);
}

inline BOOL CServiceManager::GetServiceDisplayName(LPCTSTR pszServiceName, LPTSTR pszDisplayName, DWORD& dwBufferLength)
{
    ATLASSERT(!IsEmpty());
    ATLASSERT(pszServiceName);

    return ::GetServiceDisplayName(m_hHandle, pszServiceName, pszDisplayName, &dwBufferLength);
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CService class
//

inline CService::CService()
{
}

inline SC_HANDLE CService::Create(SC_HANDLE hSCManager, LPCTSTR pszServiceName, LPCTSTR pszBinaryPathName, LPCTSTR pszDisplayName/* = NULL*/, DWORD dwDesiredAccess/* = SERVICE_ALL_ACCESS*/, DWORD dwServiceType/* = SERVICE_WIN32_SHARE_PROCESS*/, DWORD dwStartType/* = SERVICE_AUTO_START*/, DWORD dwErrorControl/* = SERVICE_ERROR_NORMAL*/, LPDWORD pdwTagId/* = NULL*/, LPCTSTR pszDependencies/* = NULL*/, LPCTSTR pszServiceStartName/* = NULL*/, LPCTSTR pszLoadOrderGroup/* = NULL*/, LPCTSTR pszPassword/* = NULL*/)
{
    ATLASSERT(IsEmpty());
    ATLASSERT(hSCManager);
    ATLASSERT(pszServiceName);

    return (m_hHandle = ::CreateService(hSCManager, pszServiceName, pszDisplayName, dwDesiredAccess, dwServiceType, dwStartType, dwErrorControl, pszBinaryPathName, pszLoadOrderGroup, pdwTagId, pszDependencies, pszServiceStartName, pszPassword));
}

inline SC_HANDLE CService::Open(SC_HANDLE hSCManager, LPCTSTR pszServiceName, DWORD dwDesiredAccess/* = SERVICE_ALL_ACCESS*/)
{
    ATLASSERT(IsEmpty());
    ATLASSERT(hSCManager);
    ATLASSERT(pszServiceName);

    return (m_hHandle = ::OpenService(hSCManager, pszServiceName, dwDesiredAccess));
}

inline BOOL CService::Delete()
{
    ATLASSERT(!IsEmpty());
    return ::DeleteService(m_hHandle);
}

inline BOOL CService::Start(LPCTSTR* ppszServiceArgs/* = NULL*/, DWORD dwServiceArgs/* = 0*/)
{
    ATLASSERT(!IsEmpty());
    return ::StartService(m_hHandle, dwServiceArgs, ppszServiceArgs);
}

inline BOOL CService::Control(SERVICE_STATUS& status, DWORD dwControl/* = SERVICE_CONTROL_STOP*/)
{
    ATLASSERT(!IsEmpty());
    return ::ControlService(m_hHandle, dwControl, &status);
}

inline BOOL CService::ChangeConfig(DWORD dwStartType, DWORD dwServiceType, DWORD dwErrorControl, LPCTSTR pszDisplayName, LPCTSTR pszServiceStartName, LPCTSTR pszPassword/* = NULL*/, LPCTSTR pszDependencies/* = NULL*/, LPCTSTR pszBinaryPathName/* = NULL*/, LPCTSTR pszLoadOrderGroup/* = NULL*/, LPDWORD pdwTagId/* = NULL*/)
{
    ATLASSERT(!IsEmpty());
    return ::ChangeServiceConfig(m_hHandle, dwServiceType, dwStartType, dwErrorControl, pszBinaryPathName, pszLoadOrderGroup, pdwTagId, pszDependencies, pszServiceStartName, pszPassword, pszDisplayName);
}

inline BOOL CService::ChangeConfig2(DWORD dwInfoLevel, LPVOID pvConfigInfo)
{
    ATLASSERT(!IsEmpty());
    return ::ChangeServiceConfig2(m_hHandle, dwInfoLevel, pvConfigInfo);
}

inline BOOL CService::SetSecurity(PSECURITY_DESCRIPTOR pSecurityDescriptor, SECURITY_INFORMATION dwSecurityInformation/* = OWNER_SECURITY_INFORMATION*/)
{
    ATLASSERT(!IsEmpty());
    return ::SetServiceObjectSecurity(m_hHandle, dwSecurityInformation, pSecurityDescriptor);
}

#if (_WIN32_WINNT >= 0x0600)
inline BOOL CService::ControlEx(DWORD dwControl, LPVOID pControlParams, DWORD dwInfoLevel/* = SERVICE_CONTROL_STATUS_REASON_INFO*/)
{
    ATLASSERT(!IsEmpty());
    return ::ControlServiceEx(m_hHandle, dwControl, dwInfoLevel, pControlParams);
}
#endif  // (_WIN32_WINNT >= 0x0600)


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CServiceStatus class
//

inline CServiceStatus::CServiceStatus()
{
}

inline BOOL CServiceStatus::Query(SC_HANDLE hService)
{
    ATLASSERT(hService);

    DWORD cbBufferSize = sizeof(SERVICE_STATUS_PROCESS);
    return ::QueryServiceStatusEx(hService, SC_STATUS_PROCESS_INFO, (LPBYTE)this, cbBufferSize, &cbBufferSize);
}

#ifdef _CRT_DEBUG_DUMP
template <typename _Pr>
inline void CServiceStatus::Dump(const _Pr& _Printer) const
{
    _Printer(_T("\n[ SERVICES STATUS ]\n[\n\tService Type\t\t\t\t= 0x%08lx\n\tCurrent State\t\t\t\t= 0x%08lx\n\tControls Accepted\t\t\t= 0x%08lx\n\tWin32 Exit Code\t\t\t\t= 0x%08lx\n\tService Specific Exit Code\t= 0x%08lx\n\tCheck Point\t\t\t\t\t= 0x%08lx\n\tWait Hint\t\t\t\t\t= 0x%08lx\n\tProcess Id\t\t\t\t\t= 0x%08lx\n\tService Flags\t\t\t\t= 0x%08lx\n]\n"),
             dwServiceType, dwCurrentState, dwControlsAccepted, dwWin32ExitCode, dwServiceSpecificExitCode, dwCheckPoint, dwWaitHint, dwProcessId, dwServiceFlags);
}
#endif  // _CRT_DEBUG_DUMP

inline BOOL CServiceStatus::IsPaused() const
{
    return (dwCurrentState == SERVICE_PAUSED);
}

inline BOOL CServiceStatus::IsRunning() const
{
    return (dwCurrentState == SERVICE_RUNNING);
}

inline BOOL CServiceStatus::IsStopped() const
{
    return (dwCurrentState == SERVICE_STOPPED);
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CServiceConfig class
//

inline CServiceConfig::CServiceConfig()
{
}

inline BOOL CServiceConfig::Query(SC_HANDLE hService)
{
    ATLASSERT(hService);

    Free();
    BOOL bSuccessful = FALSE;
    DWORD cbBufferSize = 0;

    if (!::QueryServiceConfig(hService, NULL, 0, &cbBufferSize) && ::GetLastError() == ERROR_INSUFFICIENT_BUFFER)
    {
        if (Allocate(cbBufferSize))
            bSuccessful = ::QueryServiceConfig(hService, m_pBuffer, cbBufferSize, &cbBufferSize);
    }

    return bSuccessful;
}

#ifdef _CRT_DEBUG_DUMP
template <typename _Pr>
inline void CServiceConfig::Dump(const _Pr& _Printer) const
{
    _Printer(_T("\n[ SERVICES CONFIG ]\n[\n"));
    if (m_pBuffer != NULL)
    {
        _Printer(_T("\tService Type\t\t= 0x%08lx\n\tStart Type\t\t\t= 0x%08lx\n\tError Control\t\t= 0x%08lx\n\tBinary Path Name\t= %s\n\tLoad Order Group\t= %s\n\tTag ID\t\t\t\t= %u\n\tDependencies\t\t= "),
                 m_pBuffer->dwServiceType, m_pBuffer->dwStartType, m_pBuffer->dwErrorControl, m_pBuffer->lpBinaryPathName, m_pBuffer->lpLoadOrderGroup, m_pBuffer->dwTagId);

        if (m_pBuffer->lpDependencies != NULL)
        {
            LPCTSTR pszDependencies = m_pBuffer->lpDependencies;
            while (*pszDependencies != _T('\0'))
            {
                _Printer(_T("%s "), pszDependencies);
                pszDependencies += ::lstrlen(pszDependencies) + 1;
            }
        }
        _Printer(_T("\n\tService Start Name\t= %s\n\tDisplay Name\t\t= %s\n"), m_pBuffer->lpServiceStartName, m_pBuffer->lpDisplayName);
    }
    _Printer(_T("]\n"));
}
#endif  // _CRT_DEBUG_DUMP


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CServiceConfigInfo class
//

template <typename _Ty, DWORD dwInfoLevel>
inline CServiceConfigInfo<_Ty, dwInfoLevel>::CServiceConfigInfo()
{
}

template <typename _Ty, DWORD dwInfoLevel>
inline BOOL CServiceConfigInfo<_Ty, dwInfoLevel>::Query(SC_HANDLE hService)
{
    ATLASSERT(hService);

    Free();
    BOOL bSuccessful = FALSE;
    DWORD cbBufferSize = 0;

    if (!::QueryServiceConfig2(hService, dwInfoLevel, NULL, 0, &cbBufferSize) && ::GetLastError() == ERROR_INSUFFICIENT_BUFFER)
    {
        if (Allocate(cbBufferSize))
            bSuccessful = ::QueryServiceConfig2(hService, dwInfoLevel, (LPBYTE)m_pBuffer, cbBufferSize, &cbBufferSize);
    }

    return bSuccessful;
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CServiceConfigInfoT class
//

template <typename TBase, DWORD dwInfoLevel>
inline CServiceConfigInfoT<TBase, dwInfoLevel>::CServiceConfigInfoT()
{
}

template <typename TBase, DWORD dwInfoLevel>
inline BOOL CServiceConfigInfoT<TBase, dwInfoLevel>::Query(SC_HANDLE hService)
{
    ATLASSERT(hService);

    DWORD cbBufferSize = sizeof(TBase);
    return ::QueryServiceConfig2(hService, dwInfoLevel, (LPBYTE)this, cbBufferSize, &cbBufferSize);
}


#if (_WIN32_WINNT >= 0x0600)
///////////////////////////////////////////////////////////////////////////////
// Implementation of the CServiceNotify class
//

inline CServiceNotify::CServiceNotify()
    : m_hEvent(::CreateEvent(NULL, FALSE, FALSE, NULL))
    , m_hThread(NULL), m_hService(NULL), m_nNotifyMask(0)
{
    ATLASSERT(m_hEvent);
    dwVersion = SERVICE_NOTIFY_STATUS_CHANGE;
}

inline CServiceNotify::~CServiceNotify()
{
    ATLASSERT(m_hThread == NULL);
    ATLASSERT(m_hService == NULL);
    ATLVERIFY(::CloseHandle(m_hEvent));
}

inline BOOL CServiceNotify::Register(SC_HANDLE hSCManager, LPCTSTR pszServiceName, DWORD dwNotifyMask, PFN_SC_NOTIFY_CALLBACK pfnNotifyCallback, LPVOID pContext)
{
    ATLASSERT(pContext);
    ATLASSERT(hSCManager);
    ATLASSERT(pszServiceName);
    ATLASSERT(pfnNotifyCallback);
    ATLASSERT(m_hThread == NULL);
    ATLASSERT(m_hService == NULL);

    BOOL bSuccessful = FALSE;
    if ((m_hService = ::OpenService(hSCManager, pszServiceName, SERVICE_ALL_ACCESS)) != NULL)
    {
        this->pContext = pContext;
        this->m_nNotifyMask = dwNotifyMask;
        this->pfnNotifyCallback = pfnNotifyCallback;

        bSuccessful = ((m_hThread = _CreateThread(NULL, 0, ThreadProc, this, 0, NULL)) != NULL);
    }

    return bSuccessful;
}

inline void CServiceNotify::Unregister(DWORD dwTimeout)
{
    if (m_hThread != NULL)
    {
        ATLVERIFY(::SetEvent(m_hEvent));
        _COND_VERIFY(::WaitForSingleObject(m_hThread, dwTimeout), WAIT_OBJECT_0);

        ATLVERIFY(::CloseHandle(m_hThread));
        m_hThread = NULL;
        ATLTRACE(_T("CServiceNotify::Unregister().\n"));
    }
}

inline DWORD CServiceNotify::GetServiceStatus()
{
    ATLASSERT(m_hService);
    return ::NotifyServiceStatusChange(m_hService, m_nNotifyMask, this);
}

inline SC_HANDLE CServiceNotify::GetService() const
{
    return m_hService;
}

inline DWORD WINAPI CServiceNotify::ThreadProc(LPVOID pParameter)
{
    CServiceNotify* pThis = reinterpret_cast<CServiceNotify*>(pParameter);
    ATLASSERT(pThis);

    do
    {
        ATLTRACE(_T("begin register service status change notification.\n"));
        // Register service status change notification.
        if (::NotifyServiceStatusChange(pThis->m_hService, pThis->m_nNotifyMask, pThis) != ERROR_SUCCESS)
            break;
        ATLTRACE(_T("end register service status change notification.\n"));
    }
    while (::WaitForSingleObjectEx(pThis->m_hEvent, INFINITE, TRUE) != WAIT_OBJECT_0);

    // Close service handle.
    ATLVERIFY(::CloseServiceHandle(pThis->m_hService));
    pThis->m_hService = NULL;
    ATLTRACE(_T("exit thread.\n"));

    return 0;
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CServiceNotifyImpl class
//

template <typename T>
inline CServiceNotifyImpl<T>::CServiceNotifyImpl()
{
}

template <typename T>
inline CServiceNotifyImpl<T>::~CServiceNotifyImpl()
{
    //ServiceNotify_Release();
}

template <typename T>
inline void CServiceNotifyImpl<T>::ServiceNotify_OnStatusChange(SC_HANDLE /*hService*/, const SERVICE_NOTIFY& /*svcNotify*/)
{
}

template <typename T>
inline BOOL CServiceNotifyImpl<T>::ServiceNotify_NotifyStatusChange(SC_HANDLE hSCManager, LPCTSTR pszServiceName, DWORD dwNotifyMask/* = SERVICE_NOTIFY_RUNNING | SERVICE_NOTIFY_STOPPED*/)
{
    ATLASSERT(hSCManager);
    ATLASSERT(pszServiceName);

    return m_svcNotify.Register(hSCManager, pszServiceName, dwNotifyMask, T::OnServiceNotify, static_cast<T*>(this));
}

template <typename T>
inline void CServiceNotifyImpl<T>::ServiceNotify_Release()
{
    m_svcNotify.Unregister(5000);
}

#ifdef _CRT_DEBUG_DUMP
template <typename T> template <typename _Pr>
inline void CServiceNotifyImpl<T>::ServiceNotify_Dump(const _Pr& /*_Printer*/) const
{
}
#endif  // _CRT_DEBUG_DUMP

template <typename T>
inline void CALLBACK CServiceNotifyImpl<T>::OnServiceNotify(LPVOID pContext)
{
    T* pThis = reinterpret_cast<T*>(pContext);
    ATLASSERT(pThis);

    if (pThis->m_svcNotify.GetServiceStatus() == ERROR_SUCCESS)
        pThis->ServiceNotify_OnStatusChange(pThis->m_svcNotify.GetService(), pThis->m_svcNotify);
}
#endif  // (_WIN32_WINNT >= 0x0600)


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CServiceStatusEnumeration class
//

inline CServiceStatusEnumeration::CServiceStatusEnumeration()
    : m_dwCount(0)
{
}

inline BOOL CServiceStatusEnumeration::EnumStatus(SC_HANDLE hSCManager, DWORD dwServiceType/* = SERVICE_WIN32*/, DWORD dwServiceState/* = SERVICE_STATE_ALL*/, LPCTSTR pszGroupName/* = NULL*/, SC_ENUM_TYPE InfoLevel/* = SC_ENUM_PROCESS_INFO*/)
{
    ATLASSERT(hSCManager);
    ATLASSERT(m_pBuffer == NULL);

    BOOL bSuccessful = FALSE;
    DWORD cbBufferSize = 0, dwResumeHandle = 0;
    if (!::EnumServicesStatusEx(hSCManager, InfoLevel, dwServiceType, dwServiceState, NULL, 0, &cbBufferSize, &m_dwCount, &dwResumeHandle, pszGroupName) && ::GetLastError() == ERROR_MORE_DATA)
    {
        if (Allocate(cbBufferSize))
            bSuccessful = ::EnumServicesStatusEx(hSCManager, InfoLevel, dwServiceType, dwServiceState, (LPBYTE)m_pBuffer, cbBufferSize, &cbBufferSize, &m_dwCount, &dwResumeHandle, pszGroupName);
    }

    return bSuccessful;
}

#ifdef _CRT_DEBUG_DUMP
template <typename _Pr>
inline void CServiceStatusEnumeration::Dump(const _Pr& _Printer) const
{
    _Printer(_T("\n[ SERVICES COUNT = %u ]\n"), m_dwCount);
    if (m_pBuffer != NULL)
    {
        for (DWORD i = 0; i < m_dwCount; ++i)
        {
            _Printer(_T("[\n\tService Name\t\t\t\t= %s\n\tService Display Name\t\t= %s\n\tService Type\t\t\t\t= 0x%08lx\n\tCurrent State\t\t\t\t= 0x%08lx\n\tControls Accepted\t\t\t= 0x%08lx\n\tWin32 Exit Code\t\t\t\t= 0x%08lx\n\tService Specific Exit Code\t= 0x%08lx\n\tCheck Point\t\t\t\t\t= 0x%08lx\n\tWait Hint\t\t\t\t\t= 0x%08lx\n\tProcess Id\t\t\t\t\t= 0x%08lx\n\tService Flags\t\t\t\t= 0x%08lx\n]\n"),
                     m_pBuffer[i].lpServiceName, m_pBuffer[i].lpDisplayName, m_pBuffer[i].ServiceStatusProcess.dwServiceType, m_pBuffer[i].ServiceStatusProcess.dwCurrentState, m_pBuffer[i].ServiceStatusProcess.dwControlsAccepted, m_pBuffer[i].ServiceStatusProcess.dwWin32ExitCode,
                     m_pBuffer[i].ServiceStatusProcess.dwServiceSpecificExitCode, m_pBuffer[i].ServiceStatusProcess.dwCheckPoint, m_pBuffer[i].ServiceStatusProcess.dwWaitHint, m_pBuffer[i].ServiceStatusProcess.dwProcessId, m_pBuffer[i].ServiceStatusProcess.dwServiceFlags);
        }
    }
    _Printer(_T("\n"));
}
#endif  // _CRT_DEBUG_DUMP

}  // namespace atlutil

#endif  // __SCMGRUTIL_INL__