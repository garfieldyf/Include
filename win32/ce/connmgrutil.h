///////////////////////////////////////////////////////////////////////////////
// connmgrutil.h
//
// Copyright(c) 2019, Garfield. All rights reserved.
// Author  : Garfield
// Version : 1.0
// Creation Date : 2009/12/14

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __CONNMGRUTIL_H__
#define __CONNMGRUTIL_H__

#ifndef _WIN32_WCE
    #error connmgrutil.h compiles under Windows CE only
#endif

#include <connmgr.h>
#include "atlmiscutil.h"
#include <connmgr_status.h>

///////////////////////////////////////////////////////////////////////////////
// Classes in this file:
//
// CConnectionManager
// CConnMgrDestInfo
// CConnMgrDetailedStatus
//
// Global functions in this file:
//
// ConnMgrApiReady()

namespace WCE {

///////////////////////////////////////////////////////////////////////////////
// Interface of the CConnectionManagerTraits class
//

struct CConnectionManagerTraits : public atlutil::CHandleTraits
{
    static BOOL WINAPI Destroy(HANDLET_PTR hHandle);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CConnectionManager class
//

class CConnectionManager : public atlutil::CHandle<CConnectionManagerTraits>
{
// Constructors
public:
    CConnectionManager();

// Operations
public:
    HRESULT EstablishConnection(const CONNMGR_CONNECTIONINFO& connInfo);
    DWORD EstablishConnectionSync(const CONNMGR_CONNECTIONINFO& connInfo, DWORD dwTimeout = 30000);

    void ReleaseConnection(LONG lCache = 0L);
    HRESULT SetConnectionPriority(DWORD dwPriority);
    HRESULT ProviderMessage(const GUID& guidProvider, DWORD dwMsg1, DWORD dwMsg2, PBYTE pParams, ULONG cbParamSize, LPDWORD pdwIndex = NULL);

// Attributes
public:
    DWORD GetConnectionStatus();
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CConnMgrDestInfo class
//

class CConnMgrDestInfo : public CONNMGR_DESTINATION_INFO
{
// Constructors
public:
    CConnMgrDestInfo();

// Operations
public:
    HRESULT EnumDestinations();

#ifdef _CRT_DEBUG_DUMP
    template <typename _Pr> void Dump(const _Pr& _Printer) const;
#endif

// Data members
public:
    int m_nIndex;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CConnMgrDetailedStatus class
//

class CConnMgrDetailedStatus : public atlutil::CHeapBufferImpl<CONNMGR_CONNECTION_DETAILED_STATUS>
{
// Constructors
public:
    CConnMgrDetailedStatus();

// Operations
public:
    CONNMGR_CONNECTION_DETAILED_STATUS* QueryStatus();
};


///////////////////////////////////////////////////////////////////////////////
// Global functions
//

inline DWORD WINAPI ConnMgrApiReady(DWORD dwTimeout = 5000)
{
    DWORD dwResult = WAIT_FAILED;
    if (HANDLE hEvent = ::ConnMgrApiReadyEvent())
    {
        dwResult = ::WaitForSingleObject(hEvent, dwTimeout);
        ATLVERIFY(::CloseHandle(hEvent));
    }

    return dwResult;
}

}  // namespace WCE

#include "connmgrutil.inl"

#endif  // __CONNMGRUTIL_H__