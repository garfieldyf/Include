///////////////////////////////////////////////////////////////////////////////
// iocputil.h
//
// Copyright(c) 2019, Garfield. All rights reserved.
// Author  : Garfield
// Version : 2.0
// Creation Date : 2007/12/21

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __IOCPUTIL_H__
#define __IOCPUTIL_H__

#ifdef _WIN32_WCE
    #error iocputil.h compiles under Windows only
#endif

#include <process.h>
#include <atlcoll.h>
#include "atlmiscutil.h"

///////////////////////////////////////////////////////////////////////////////
// Classes in this file:
//
// IIOCompletionEvent
// CIOCompletionPort

namespace stdutil {

///////////////////////////////////////////////////////////////////////////////
// Interface of the IIOCompletionEvent class
//

interface ATL_NO_VTABLE IIOCompletionEvent
{
    virtual void OnIOComplete(DWORD dwNumberOfBytesTransferred, LPOVERLAPPED pOverlapped, DWORD dwErrorCode) = 0;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CThreadPool class
//

class CThreadPool
{
// Constructors/Destructor
public:
    CThreadPool();
    ~CThreadPool();

// Operations
public:
    BOOL Create(HANDLE hCompletionPort, DWORD dwThreadCount = 0);
    void Close(HANDLE hCompletionPort, DWORD dwTimeout = INFINITE);

// Attributes
public:
    DWORD GetThreadCount() const;
    HANDLE GetThread(DWORD dwIndex) const;

#if (_WIN32_WINNT >= 0x0600)
    DWORD GetThreadId(DWORD dwIndex) const;
#endif

// Implementation
protected:
    static DWORD WINAPI ThreadProc(LPVOID pParameter);

// Data members
protected:
    ATL::CAtlArray<HANDLE> m_arrThreads;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CIOCompletionPort class
//

class CIOCompletionPort : public atlutil::CHandle<>
{
// Constructors
public:
    CIOCompletionPort();

// Operations
public:
    HANDLE Create(DWORD dwNumberOfConcurrentThreads = 0);
    BOOL Associate(HANDLE hHandle, IIOCompletionEvent* pEvent);

    BOOL CreateThreadPool(DWORD dwThreadCount = 0);
    void CloseThreadPool(DWORD dwTimeout = INFINITE);

    void ProcessSurplusPacket();
    BOOL PostQueuedStatus(DWORD dwNumberOfBytesTransferred, IIOCompletionEvent* pEvent, LPOVERLAPPED pOverlapped);
    BOOL GetQueuedStatus(LPDWORD pNumberOfBytesTransferred, IIOCompletionEvent** ppEvent, LPOVERLAPPED* ppOverlapped, DWORD dwMilliseconds = INFINITE);

// Attributes
public:
    DWORD GetThreadCount() const;
    HANDLE GetThread(DWORD dwIndex) const;

#if (_WIN32_WINNT >= 0x0600)
    DWORD GetThreadId(DWORD dwIndex) const;
#endif

// Implementation
private:
    HANDLE Detach();
    void Attach(HANDLE);

// Data members
protected:
    CThreadPool m_threads;
};

}  // namespace stdutil

#include "iocputil.inl"

#endif  // __IOCPUTIL_H__