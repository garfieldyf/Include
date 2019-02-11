///////////////////////////////////////////////////////////////////////////////
// cemsgutil.h
//
// Copyright(c) 2019, Garfield. All rights reserved.
// Author  : Garfield
// Version : 1.0
// Creation Date : 2010/10/21

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __CEMSGUTIL_H__
#define __CEMSGUTIL_H__

#ifndef _WIN32_WCE
    #error cemsgutil.h compiles under Windows CE only
#endif

#include <atlalloc.h>
#include "atlmiscutil.h"

///////////////////////////////////////////////////////////////////////////////
// Classes in this file:
//
// CMessageQueue
// CMessageLoop
// CMessageThread
// IMessageHandler

namespace WCE {

///////////////////////////////////////////////////////////////////////////////
// Interface of the CMessageQueueTraits class
//

struct CMessageQueueTraits : public atlutil::CHandleTraits
{
    static BOOL WINAPI Destroy(HANDLET_PTR hHandle);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CMessageQueue class
//

class CMessageQueue : public atlutil::CHandle<CMessageQueueTraits>
{
// Constructors
public:
    CMessageQueue();

// Operations
public:
    HANDLE Create(DWORD cbMaxMessage, BOOL bReadAccess = TRUE, DWORD dwMaxMessages = 0, DWORD dwFlags = MSGQUEUE_NOPRECOMMIT, LPCWSTR pszName = NULL);
    HANDLE Open(HANDLE hProccess, HANDLE hMsgQueue, BOOL bReadAccess = TRUE);

    BOOL Read(LPVOID pBuffer, DWORD cbBufferSize, DWORD& dwFlags, DWORD& dwNumberOfBytesRead, DWORD dwTimeout = INFINITE) const;
    BOOL Write(LPCVOID pBuffer,DWORD cbBufferSize, DWORD dwFlags = 0, DWORD dwTimeout = INFINITE) const;

// Attributes
public:
    DWORD GetMaxMessageSize() const;
    BOOL GetQueueInfo(MSGQUEUEINFO& msgQueueInfo) const;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the IMessageHandler class
//

interface ATL_NO_VTABLE IMessageHandler
{
    virtual void OnProcessMessage(const MSGQUEUE_MESSAGE* pMsg, DWORD cbSize, DWORD dwFlags) = 0;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CMessageLoop class
//

class CMessageLoop
{
// Constructors
public:
    CMessageLoop();

// Operations
public:
    BOOL Create(LPCWSTR pszMsgQueueName);
    BOOL Create(HANDLE hMsgQueue, HANDLE hProccess = ::GetCurrentProcess());

    void Destroy();
    void Run(IMessageHandler* pHandler);

// Data members
protected:
    CMessageQueue m_msgQueue;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CMessageThread class
//

class CMessageThread
{
// Constructors/Destructor
public:
    CMessageThread();
    ~CMessageThread();

// Operations
public:
    BOOL Create(IMessageHandler* pHandler, DWORD cbMaxMessage, DWORD dwMaxMessages = 0, DWORD dwFlags = MSGQUEUE_NOPRECOMMIT, DWORD dwThreadFlags = 0);
    BOOL Quit(DWORD dwTimeout = INFINITE);

    BOOL PostMessage(UINT message, DWORD dwFlags = 0, DWORD dwTimeout = INFINITE) const;
    BOOL PostMessage(const MSGQUEUE_MESSAGE* pMsg, DWORD cbSize, DWORD dwFlags = 0, DWORD dwTimeout = INFINITE) const;

    operator HANDLE() const;

// Attributes
public:
    int GetPriority() const;
    BOOL SetPriority(int nPriority);

// Implementation
protected:
    static DWORD WINAPI ThreadProc(LPVOID pParameter);

// Data members
protected:
    HANDLE m_hThread;
    CMessageQueue m_msgQueue;
    IMessageHandler* m_pHandler;
};

}  // namespace WCE

#include "cemsgutil.inl"

#endif  // __CEMSGUTIL_H__