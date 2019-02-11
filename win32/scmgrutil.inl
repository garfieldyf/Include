///////////////////////////////////////////////////////////////////////////////
// scmgrutil.inl
//
// Copyright(c) 2019, Garfield. All rights reserved.
// Author  : Garfield
// Version : 1.0
// Creation Date : 2009/4/13

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __SCMGRUTIL_INL__
#define __SCMGRUTIL_INL__

#ifndef __SCMGRUTIL_H__
    #error scmgrutil.inl requires scmgrutil.h to be included first
#endif

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
        strServiceName.ReleaseBuffer(dwBufferLength);
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
        strDisplayName.ReleaseBuffer(dwBufferLength);
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

inline BOOL CServiceStatus::Query(SC_HANDLE hService, SC_STATUS_TYPE InfoLevel/* = SC_STATUS_PROCESS_INFO*/)
{
    ATLASSERT(hService);

    DWORD cbSize = sizeof(SERVICE_STATUS_PROCESS);
    return ::QueryServiceStatusEx(hService, InfoLevel, (LPBYTE)this, cbSize, &cbSize);
}

inline BOOL CServiceStatus::Verify(SC_HANDLE hService, DWORD dwVerifiedState, DWORD dwVerifiedTime/* = 10*/, SC_STATUS_TYPE InfoLevel/* = SC_STATUS_PROCESS_INFO*/)
{
    ATLASSERT(hService);
    ATLASSERT(dwVerifiedState <= SERVICE_PAUSED);
    ATLASSERT(dwVerifiedState >= SERVICE_STOPPED);

    BOOL bSuccessful = Query(hService, InfoLevel);
    for (; bSuccessful && dwCurrentState != dwVerifiedState && dwVerifiedTime > 0; --dwVerifiedTime)
    {
        ::Sleep(200);
        bSuccessful = Query(hService, InfoLevel);
    }

    return (dwCurrentState == dwVerifiedState);
}

#ifdef _CRT_DEBUG_DUMP
template <typename _Pr>
inline void CServiceStatus::Dump(const _Pr& _Printer) const
{
    _Printer(_T("\n[ SERVICES STATUS ]\n[\n\tService Type\t\t\t\t= 0x%08lx\n\tCurrent State\t\t\t\t= 0x%08lx\n\tControls Accepted\t\t\t= 0x%08lx\n\
                \tWin32 Exit Code\t\t\t\t= 0x%08lx\n\tService Specific Exit Code\t= 0x%08lx\n\tCheck Point\t\t\t\t\t= 0x%08lx\n\tWait Hint\t\t\t\t\t= 0x%08lx\n\
                \tProcess Id\t\t\t\t\t= 0x%08lx\n\tService Flags\t\t\t\t= 0x%08lx\n]\n"), dwServiceType, dwCurrentState, dwControlsAccepted, dwWin32ExitCode,
                dwServiceSpecificExitCode, dwCheckPoint, dwWaitHint, dwProcessId, dwServiceFlags);
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

inline BOOL CServiceStatus::IsPending() const
{
    return (dwCurrentState == SERVICE_START_PENDING || dwCurrentState == SERVICE_STOP_PENDING || dwCurrentState == SERVICE_CONTINUE_PENDING || dwCurrentState == SERVICE_PAUSE_PENDING);
}

inline BOOL CServiceStatus::IsStopped() const
{
    return (dwCurrentState == SERVICE_STOPPED);
}

inline BOOL CServiceStatus::IsStopPending() const
{
    return (dwCurrentState == SERVICE_STOP_PENDING);
}

inline BOOL CServiceStatus::IsStartPending() const
{
    return (dwCurrentState == SERVICE_START_PENDING);
}

inline BOOL CServiceStatus::IsPausePending() const
{
    return (dwCurrentState == SERVICE_PAUSE_PENDING);
}

inline BOOL CServiceStatus::IsContinuePending() const
{
    return (dwCurrentState == SERVICE_CONTINUE_PENDING);
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

    DWORD cbSize = 0;
    BOOL bSuccessful = ((!::QueryServiceConfig(hService, NULL, 0, &cbSize) && ::GetLastError() == ERROR_INSUFFICIENT_BUFFER) && m_buffer.allocateBytes(cbSize) != NULL && ::QueryServiceConfig(hService, m_buffer.data, cbSize, &cbSize));
    if (!bSuccessful)
        m_buffer.clear();

    return bSuccessful;
}

#ifdef _CRT_DEBUG_DUMP
template <typename _Pr>
inline void CServiceConfig::Dump(const _Pr& _Printer) const
{
    if (!IsEmpty())
    {
        _Printer(_T("\n[ SERVICES CONFIG ]\n[\n\tService Type\t\t= 0x%08lx\n\tStart Type\t\t\t= 0x%08lx\n\tError Control\t\t= 0x%08lx\n\tBinary Path Name\
                    \t= %s\n\tLoad Order Group\t= %s\n\tTag ID\t\t\t\t= %u\n\tDependencies\t\t= "), m_buffer.data->dwServiceType, m_buffer.data->dwStartType,
                    m_buffer.data->dwErrorControl, m_buffer.data->lpBinaryPathName, m_buffer.data->lpLoadOrderGroup, m_buffer.data->dwTagId);

        if (m_buffer.data->lpDependencies != NULL)
        {
            LPCTSTR pszDependencies = m_buffer.data->lpDependencies;
            while (*pszDependencies != _T('\0'))
            {
                _Printer(_T("%s "), pszDependencies);
                pszDependencies += ::lstrlen(pszDependencies) + 1;
            }
        }
        _Printer(_T("\n\tService Start Name\t= %s\n\tDisplay Name\t\t= %s\n]\n"), m_buffer.data->lpServiceStartName, m_buffer.data->lpDisplayName);
    }
    else
    {
        _Printer(_T("\n[ THE SERVICES CONFIG IS EMPTY! ]\n"));
    }
}
#endif  // _CRT_DEBUG_DUMP


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CServiceConfigInfo class
//

template <typename _Ty, DWORD t_dwInfoLevel>
inline CServiceConfigInfo<_Ty, t_dwInfoLevel>::CServiceConfigInfo()
{
}

template <typename _Ty, DWORD t_dwInfoLevel>
inline BOOL CServiceConfigInfo<_Ty, t_dwInfoLevel>::Query(SC_HANDLE hService)
{
    ATLASSERT(hService);

    DWORD cbSize = 0;
    BOOL bSuccessful = ((!::QueryServiceConfig2(hService, t_dwInfoLevel, NULL, 0, &cbSize) && ::GetLastError() == ERROR_INSUFFICIENT_BUFFER) && m_buffer.allocateBytes(cbSize) != NULL && ::QueryServiceConfig2(hService, t_dwInfoLevel, (LPBYTE)m_buffer.data, cbSize, &cbSize));
    if (!bSuccessful)
        m_buffer.clear();

    return bSuccessful;
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CServiceConfigInfoT class
//

template <typename TBase, DWORD t_dwInfoLevel>
inline CServiceConfigInfoT<TBase, t_dwInfoLevel>::CServiceConfigInfoT()
{
}

template <typename TBase, DWORD t_dwInfoLevel>
inline BOOL CServiceConfigInfoT<TBase, t_dwInfoLevel>::Query(SC_HANDLE hService)
{
    ATLASSERT(hService);

    DWORD cbSize = sizeof(TBase);
    return ::QueryServiceConfig2(hService, t_dwInfoLevel, (LPBYTE)this, cbSize, &cbSize);
}


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

    DWORD cbSize = 0, dwResumeHandle = 0;
    BOOL bSuccessful = ((!::EnumServicesStatusEx(hSCManager, InfoLevel, dwServiceType, dwServiceState, NULL, 0, &cbSize, &m_dwCount, &dwResumeHandle, pszGroupName) && ::GetLastError() == ERROR_MORE_DATA) && m_buffer.allocateBytes(cbSize) != NULL && ::EnumServicesStatusEx(hSCManager, InfoLevel, dwServiceType, dwServiceState, (LPBYTE)m_buffer.data, cbSize, &cbSize, &m_dwCount, &dwResumeHandle, pszGroupName));
    if (!bSuccessful)
        m_buffer.clear();

    return bSuccessful;
}

#ifdef _CRT_DEBUG_DUMP
template <typename _Pr>
inline void CServiceStatusEnumeration::Dump(const _Pr& _Printer) const
{
    if (!IsEmpty())
    {
        _Printer(_T("\n[ SERVICES COUNT = %u ]\n"), m_dwCount);
        for (DWORD i = 0; i < m_dwCount; ++i)
        {
            _Printer(_T("[\n\tService Name\t\t\t\t= %s\n\tService Display Name\t\t= %s\n\tService Type\t\t\t\t= 0x%08lx\n\tCurrent State\t\t\t\t= 0x%08lx\n\
                        \tControls Accepted\t\t\t= 0x%08lx\n\tWin32 Exit Code\t\t\t\t= 0x%08lx\n\tService Specific Exit Code\t= 0x%08lx\n\tCheck Point\t\t\t\t\t= 0x%08lx\n\
                        \tWait Hint\t\t\t\t\t= 0x%08lx\n\tProcess Id\t\t\t\t\t= 0x%08lx\n\tService Flags\t\t\t\t= 0x%08lx\n]\n"), m_buffer.data[i].lpServiceName,
                        m_buffer.data[i].lpDisplayName, m_buffer.data[i].ServiceStatusProcess.dwServiceType, m_buffer.data[i].ServiceStatusProcess.dwCurrentState,
                        m_buffer.data[i].ServiceStatusProcess.dwControlsAccepted, m_buffer.data[i].ServiceStatusProcess.dwWin32ExitCode, m_buffer.data[i].ServiceStatusProcess.dwServiceSpecificExitCode,
                        m_buffer.data[i].ServiceStatusProcess.dwCheckPoint, m_buffer.data[i].ServiceStatusProcess.dwWaitHint, m_buffer.data[i].ServiceStatusProcess.dwProcessId,
                        m_buffer.data[i].ServiceStatusProcess.dwServiceFlags);
        }
        _Printer(_T("\n"));
    }
    else
    {
        _Printer(_T("\n[ THE SERVICE STATUS ENUMERATION IS EMPTY! ]\n"));
    }
}
#endif  // _CRT_DEBUG_DUMP


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CServiceCtrlHandlerImpl class
//

template <typename T>
inline CServiceCtrlHandlerImpl<T>::CServiceCtrlHandlerImpl()
    : m_hServiceStatus(NULL)
{
}

template <typename T>
inline DWORD CServiceCtrlHandlerImpl<T>::SCHandler_OnPause()
{
    return NO_ERROR;
}

template <typename T>
inline DWORD CServiceCtrlHandlerImpl<T>::SCHandler_OnStop()
{
    return NO_ERROR;
}

template <typename T>
inline DWORD CServiceCtrlHandlerImpl<T>::SCHandler_OnContinue()
{
    return NO_ERROR;
}

template <typename T>
inline DWORD CServiceCtrlHandlerImpl<T>::SCHandler_OnShutdown()
{
    return NO_ERROR;
}

template <typename T>
inline DWORD CServiceCtrlHandlerImpl<T>::SCHandler_OnPreShutdown()
{
    return NO_ERROR;
}

template <typename T>
inline DWORD CServiceCtrlHandlerImpl<T>::SCHandler_OnInterrogate()
{
    return NO_ERROR;
}

template <typename T>
inline DWORD CServiceCtrlHandlerImpl<T>::SCHandler_OnParamChange()
{
    return NO_ERROR;
}

template <typename T>
inline SERVICE_STATUS_HANDLE CServiceCtrlHandlerImpl<T>::SCHandler_Register(LPCTSTR pszServiceName)
{
    ATLASSERT(pszServiceName);
    ATLASSERT(SCHandler_IsEmpty());

    return (m_hServiceStatus = ::RegisterServiceCtrlHandlerEx(pszServiceName, T::SCHandler_ServiceControl, static_cast<T*>(this)));
}

template <typename T>
inline BOOL CServiceCtrlHandlerImpl<T>::SCHandler_IsEmpty() const
{
    return (m_hServiceStatus == NULL);
}

template <typename T>
inline DWORD WINAPI CServiceCtrlHandlerImpl<T>::SCHandler_ServiceControl(DWORD dwControl, DWORD /*dwEventType*/, LPVOID /*pEventData*/, LPVOID pContext)
{
    T* pThis = reinterpret_cast<T*>(pContext);
    ATLASSERT(pThis);

    DWORD dwRetCode = NO_ERROR;
    switch (dwControl)
    {
    case SERVICE_CONTROL_STOP:
        dwRetCode = pThis->SCHandler_OnStop();
        break;

    case SERVICE_CONTROL_PAUSE:
        dwRetCode = pThis->SCHandler_OnPause();
        break;

    case SERVICE_CONTROL_CONTINUE:
        dwRetCode = pThis->SCHandler_OnContinue();
        break;

    case SERVICE_CONTROL_SHUTDOWN:
        dwRetCode = pThis->SCHandler_OnShutdown();
        break;

    case SERVICE_CONTROL_INTERROGATE:
        dwRetCode = pThis->SCHandler_OnInterrogate();
        break;

    case SERVICE_CONTROL_PARAMCHANGE:
        dwRetCode = pThis->SCHandler_OnParamChange();
        break;

    case SERVICE_CONTROL_PRESHUTDOWN:
        dwRetCode = pThis->SCHandler_OnPreShutdown();
        break;
    }

    return dwRetCode;
}

}  // namespace atlutil

#endif  // __SCMGRUTIL_INL__