///////////////////////////////////////////////////////////////////////////////
// scmgrutil.h
//
// Copyright(c) 2012, Garfield. All rights reserved.
// Author  : Garfield
// Version : 1.0
// Finish Time : 2009/4/13

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __SCMGRUTIL_H__
#define __SCMGRUTIL_H__

#include <winsvc.h>
#include "atlmiscutil.h"

///////////////////////////////////////////////////////////////////////////////
// Classes in this file:
//
// CServiceManager
// CService
// CServiceStatus
// CServiceConfig
// CServiceDescription
// CServiceSidInfo
// CServicePreshutdownInfo
// CServiceFailureActions
// CServiceDelayedInfo
// CServicePrivilegesInfo
// CServiceFailureActionsFlag
// CServiceNotifyImpl<T>
// CServiceStatusEnumeration

namespace atlutil {

///////////////////////////////////////////////////////////////////////////////
// Interface of the CServiceHandleTraits class
//

struct CServiceHandleTraits : public atlutil::CHandleTraits
{
    typedef SC_HANDLE HANDLET_PTR;
    static BOOL WINAPI Destroy(HANDLET_PTR hHandle);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CServiceManager class
//

class CServiceManager : public atlutil::CHandle<CServiceHandleTraits>
{
// Constructors
public:
    CServiceManager();

// Operations
public:
    SC_HANDLE Open(DWORD dwDesiredAccess = SC_MANAGER_ALL_ACCESS, LPCTSTR pszDatabaseName = NULL, LPCTSTR pszMachineName = NULL);

#if defined(__ATLSTR_H__) || defined(__AFXSTR_H__)
    CString GetServiceName(LPCTSTR pszDisplayName);
    CString GetServiceDisplayName(LPCTSTR pszServiceName);
#endif

    BOOL GetServiceName(LPCTSTR pszDisplayName, LPTSTR pszServiceName, DWORD& dwBufferLength);
    BOOL GetServiceDisplayName(LPCTSTR pszServiceName, LPTSTR pszDisplayName, DWORD& dwBufferLength);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CService class
//

class CService : public atlutil::CHandle<CServiceHandleTraits>
{
// Constructors
public:
    CService();

// Operations
public:
    SC_HANDLE Create(SC_HANDLE hSCManager, LPCTSTR pszServiceName, LPCTSTR pszBinaryPathName, LPCTSTR pszDisplayName = NULL, DWORD dwDesiredAccess = SERVICE_ALL_ACCESS, DWORD dwServiceType = SERVICE_WIN32_SHARE_PROCESS, DWORD dwStartType = SERVICE_AUTO_START, DWORD dwErrorControl = SERVICE_ERROR_NORMAL, LPDWORD pdwTagId = NULL, LPCTSTR pszDependencies = NULL, LPCTSTR pszServiceStartName = NULL, LPCTSTR pszLoadOrderGroup = NULL, LPCTSTR pszPassword = NULL);
    SC_HANDLE Open(SC_HANDLE hSCManager, LPCTSTR pszServiceName, DWORD dwDesiredAccess = SERVICE_ALL_ACCESS);

    BOOL Delete();
    BOOL Start(LPCTSTR* ppszServiceArgs = NULL, DWORD dwServiceArgs = 0);
    BOOL Control(SERVICE_STATUS& status, DWORD dwControl = SERVICE_CONTROL_STOP);

    BOOL ChangeConfig(DWORD dwStartType, DWORD dwServiceType, DWORD dwErrorControl, LPCTSTR pszDisplayName, LPCTSTR pszServiceStartName, LPCTSTR pszPassword = NULL, LPCTSTR pszDependencies = NULL, LPCTSTR pszBinaryPathName = NULL, LPCTSTR pszLoadOrderGroup = NULL, LPDWORD pdwTagId = NULL);
    BOOL ChangeConfig2(DWORD dwInfoLevel, LPVOID pvConfigInfo);
    BOOL SetSecurity(PSECURITY_DESCRIPTOR pSecurityDescriptor, SECURITY_INFORMATION dwSecurityInformation = OWNER_SECURITY_INFORMATION);

#if (_WIN32_WINNT >= 0x0600)
    BOOL ControlEx(DWORD dwControl, LPVOID pControlParams, DWORD dwInfoLevel = SERVICE_CONTROL_STATUS_REASON_INFO);
#endif
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CServiceStatus class
//

class CServiceStatus : public SERVICE_STATUS_PROCESS
{
// Constructors
public:
    CServiceStatus();

// Operations
public:
    BOOL Query(SC_HANDLE hService);

#ifdef _CRT_DEBUG_DUMP
    template <typename _Pr> void Dump(const _Pr& _Printer) const;
#endif

// Attributes
public:
    BOOL IsPaused() const;
    BOOL IsRunning() const;
    BOOL IsStopped() const;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CServiceConfig class
//

class CServiceConfig : public atlutil::CBufferImpl<QUERY_SERVICE_CONFIG>
{
// Constructors
public:
    CServiceConfig();

// Operations
public:
    BOOL Query(SC_HANDLE hService);

#ifdef _CRT_DEBUG_DUMP
    template <typename _Pr> void Dump(const _Pr& _Printer) const;
#endif
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CServiceConfigInfo class
//

template <typename _Ty, DWORD dwInfoLevel>
class CServiceConfigInfo : public atlutil::CBufferImpl<_Ty>
{
// Constructors
public:
    CServiceConfigInfo();

// Operations
public:
    BOOL Query(SC_HANDLE hService);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CServiceConfigInfoT class
//

template <typename TBase, DWORD dwInfoLevel>
class CServiceConfigInfoT : public TBase
{
// Constructors
public:
    CServiceConfigInfoT();

// Operations
public:
    BOOL Query(SC_HANDLE hService);
};


#if (_WIN32_WINNT >= 0x0600)
///////////////////////////////////////////////////////////////////////////////
// Interface of the CServiceNotify class
//

class CServiceNotify : public SERVICE_NOTIFY
{
// Constructors/Destructor
public:
    CServiceNotify();
    ~CServiceNotify();

// Operations
public:
    BOOL Register(SC_HANDLE hSCManager, LPCTSTR pszServiceName, DWORD dwNotifyMask, PFN_SC_NOTIFY_CALLBACK pfnNotifyCallback, LPVOID pContext);
    void Unregister(DWORD dwTimeout);

    DWORD GetServiceStatus();
    SC_HANDLE GetService() const;

// Implementation
protected:
    static DWORD WINAPI ThreadProc(LPVOID pParameter);

// Data members
protected:
    HANDLE m_hEvent;
    HANDLE m_hThread;
    DWORD  m_nNotifyMask;
    SC_HANDLE m_hService;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CServiceNotifyImpl class
//

template <typename T>
class ATL_NO_VTABLE CServiceNotifyImpl
{
// Constructors/Destructor
protected:
    CServiceNotifyImpl();
    ~CServiceNotifyImpl();

// Overridables
public:
    void ServiceNotify_OnStatusChange(SC_HANDLE hService, const SERVICE_NOTIFY& svcNotify);

// Operations
public:
    BOOL ServiceNotify_NotifyStatusChange(SC_HANDLE hSCManager, LPCTSTR pszServiceName, DWORD dwNotifyMask = SERVICE_NOTIFY_RUNNING | SERVICE_NOTIFY_STOPPED);
    void ServiceNotify_Release();

#ifdef _CRT_DEBUG_DUMP
    template <typename _Pr> void ServiceNotify_Dump(const _Pr& _Printer) const;
#endif

// Implementation
protected:
    static void CALLBACK OnServiceNotify(LPVOID pContext);

// Data members
protected:
    CServiceNotify m_svcNotify;
};
#endif  // (_WIN32_WINNT >= 0x0600)


///////////////////////////////////////////////////////////////////////////////
// Interface of the CServiceStatusEnumeration class
//

class CServiceStatusEnumeration : public atlutil::CBufferImpl<ENUM_SERVICE_STATUS_PROCESS>
{
// Constructors
public:
    CServiceStatusEnumeration();

// Operations
public:
    BOOL EnumStatus(SC_HANDLE hSCManager, DWORD dwServiceType = SERVICE_WIN32, DWORD dwServiceState = SERVICE_STATE_ALL, LPCTSTR pszGroupName = NULL, SC_ENUM_TYPE InfoLevel = SC_ENUM_PROCESS_INFO);

#ifdef _CRT_DEBUG_DUMP
    template <typename _Pr> void Dump(const _Pr& _Printer) const;
#endif

// Data members
public:
    DWORD m_dwCount;
};

}  // namespace atlutil

#include "scmgrutil.inl"

#endif  // __SCMGRUTIL_H__