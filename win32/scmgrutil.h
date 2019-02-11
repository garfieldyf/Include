///////////////////////////////////////////////////////////////////////////////
// scmgrutil.h
//
// Copyright(c) 2019, Garfield. All rights reserved.
// Author  : Garfield
// Version : 1.0
// Creation Date : 2009/4/13

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __SCMGRUTIL_H__
#define __SCMGRUTIL_H__

#ifdef _WIN32_WCE
    #error scmgrutil.h compiles under Windows only
#endif

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
// CServiceStatusEnumeration
// CServiceCtrlHandlerImpl<T>

namespace atlutil {

///////////////////////////////////////////////////////////////////////////////
// Interface of the CServiceHandleTraits class
//

struct CServiceHandleTraits : public CHandleTraits
{
    typedef SC_HANDLE HANDLET_PTR;
    static BOOL WINAPI Destroy(HANDLET_PTR hHandle);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CServiceManager class
//

class CServiceManager : public CHandle<CServiceHandleTraits>
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

class CService : public CHandle<CServiceHandleTraits>
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
    BOOL Query(SC_HANDLE hService, SC_STATUS_TYPE InfoLevel = SC_STATUS_PROCESS_INFO);
    BOOL Verify(SC_HANDLE hService, DWORD dwVerifiedState, DWORD dwVerifiedTime = 10, SC_STATUS_TYPE InfoLevel = SC_STATUS_PROCESS_INFO);

#ifdef _CRT_DEBUG_DUMP
    template <typename _Pr> void Dump(const _Pr& _Printer) const;
#endif

// Attributes
public:
    BOOL IsPaused() const;
    BOOL IsRunning() const;
    BOOL IsPending() const;
    BOOL IsStopped() const;

    BOOL IsStopPending() const;
    BOOL IsStartPending() const;
    BOOL IsPausePending() const;
    BOOL IsContinuePending() const;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CServiceConfig class
//

class CServiceConfig : public CHeapBufferImpl<QUERY_SERVICE_CONFIG>
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

template <typename _Ty, DWORD t_dwInfoLevel>
class CServiceConfigInfo : public CHeapBufferImpl<_Ty>
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

template <typename TBase, DWORD t_dwInfoLevel>
class CServiceConfigInfoT : public TBase
{
// Constructors
public:
    CServiceConfigInfoT();

// Operations
public:
    BOOL Query(SC_HANDLE hService);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CServiceStatusEnumeration class
//

class CServiceStatusEnumeration : public CHeapBufferImpl<ENUM_SERVICE_STATUS_PROCESS>
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


///////////////////////////////////////////////////////////////////////////////
// Interface of the CServiceCtrlHandlerImpl class
//

template <typename T>
class ATL_NO_VTABLE CServiceCtrlHandlerImpl
{
// Constructors
protected:
    CServiceCtrlHandlerImpl();

// Overridables
public:
    DWORD SCHandler_OnPause();
    DWORD SCHandler_OnStop();
    DWORD SCHandler_OnContinue();

    DWORD SCHandler_OnShutdown();
    DWORD SCHandler_OnPreShutdown();

    DWORD SCHandler_OnInterrogate();
    DWORD SCHandler_OnParamChange();

// Operations
public:
    SERVICE_STATUS_HANDLE SCHandler_Register(LPCTSTR pszServiceName);

// Attributes
public:
    BOOL SCHandler_IsEmpty() const;

// Implementation
protected:
    static DWORD WINAPI SCHandler_ServiceControl(DWORD dwControl, DWORD dwEventType, LPVOID pEventData, LPVOID pContext);

// Data members
public:
    SERVICE_STATUS_HANDLE m_hServiceStatus;
};

}  // namespace atlutil

#include "scmgrutil.inl"

#endif  // __SCMGRUTIL_H__