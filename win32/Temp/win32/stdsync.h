///////////////////////////////////////////////////////////////////////////////
// stdsync.h
//
// Copyright(c) 2010, Garfield. All rights reserved.
// Author  : Garfield
// Version : 2.0
// Finish Time : 2010/5/22

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __STDSYNC_H__
#define __STDSYNC_H__

#ifndef _WIN32_WCE
#include <process.h>
#endif

#include "stddefx.h"

///////////////////////////////////////////////////////////////////////////////
// Classes in this file:
//
// CAsyncTaskImpl<T>
// ITimerEvent
// CTimerTask
// IMessageEvent
// CMessageThread

namespace stdutil {

///////////////////////////////////////////////////////////////////////////////
// Interface of the CAsyncTaskBase class
//

class ATL_NO_VTABLE CAsyncTaskBase
{
// Constructors/Destructor
protected:
    CAsyncTaskBase();
    ~CAsyncTaskBase();

// Operations
public:
    void CancelTask();
    DWORD PauseTask() const;
    DWORD ResumeTask() const;

// Attributes
public:
    BOOL IsTaskCancelled() const;

// Implementation
protected:
    void FinishTask(DWORD dwTimeout = 5000);

// Data members
protected:
    LONG m_lCancel;
    HANDLE m_hThread;
};


#ifdef __ATLWIN_H__
///////////////////////////////////////////////////////////////////////////////
// Interface of the CAsyncTaskImpl class
//

template <typename T>
class ATL_NO_VTABLE CAsyncTaskImpl : public CAsyncTaskBase
{
// Constructors
protected:
    CAsyncTaskImpl();

// Implementation
public:
    BEGIN_MSG_MAP(CAsyncTaskImpl<T>)
        MESSAGE_HANDLER(WM_ASYNCTAST_FINISH, OnAsyncTaskFinish)
    END_MSG_MAP()

// Overridables
public:
    HRESULT OnPrepareTask();
    HRESULT OnExecuteTask();
    void OnFinishTask(HRESULT hrResult);

// Operations
public:
    BOOL ExecuteTask(DWORD dwCreationFlags = 0, SIZE_T dwStackSize = 0, LPSECURITY_ATTRIBUTES psa = NULL);

// Implementation
protected:
    static DWORD WINAPI AsyncTaskProc(LPVOID pParameter);

// Message map functions
public:
    LRESULT OnAsyncTaskFinish(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
};
#endif  // __ATLWIN_H__


///////////////////////////////////////////////////////////////////////////////
// Interface of the CThreadBase class
//

class CThreadBase
{
// Constructors/Destructor
protected:
    CThreadBase();
    ~CThreadBase();

// Operations
public:
    operator HANDLE() const;

    DWORD Resume() const;
    DWORD Suspend() const;

// Attributes
public:
    int GetPriority() const;
    BOOL SetPriority(int nPriority);

// Data members
protected:
    HANDLE m_hThread;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the ITimerEvent class
//

interface ATL_NO_VTABLE ITimerEvent
{
    virtual void OnTimerEvent(DWORD dwEventId) = 0;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CTimerTask class
//

class CTimerTask : public CThreadBase
{
// Constructors/Destructor
public:
    CTimerTask();
    ~CTimerTask();

// Operations
public:
    BOOL Start(ITimerEvent* pEvent, DWORD dwEventId, DWORD dwDelay, BOOL bFixedDelay = TRUE, DWORD dwCreationFlags = 0, SIZE_T dwStackSize = 0, LPSECURITY_ATTRIBUTES psa = NULL);

    BOOL Cancel() const;
    BOOL Stop(DWORD dwTimeout = INFINITE);

// Attributes
public:
    BOOL IsCancelled() const;
    DWORD GetEventId() const;

// Implementation
protected:
    static DWORD WINAPI FixedTimerProc(LPVOID pParameter);
    static DWORD WINAPI DelayTimerProc(LPVOID pParameter);

// Data members
protected:
    HANDLE m_hEvent;
    DWORD m_dwDelay;
    DWORD m_dwEventId;
    ITimerEvent* m_pEvent;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the IMessageEvent class
//

interface ATL_NO_VTABLE IMessageEvent
{
    virtual void OnProcessMessage(const MSG& msg) = 0;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CMessageThread class
//

class CMessageThread : public CThreadBase
{
// Constructors
public:
    CMessageThread();

// Operations
public:
    BOOL Create(IMessageEvent* pEvent, DWORD dwCreationFlags = 0, SIZE_T dwStackSize = 0, LPSECURITY_ATTRIBUTES psa = NULL);
    BOOL Quit(DWORD dwTimeout = INFINITE);

    void Cancel();
    BOOL PostMessage(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0L) const;

// Attributes
public:
    BOOL IsCancelled() const;
    DWORD GetThreadId() const;

// Implementation
protected:
    static DWORD WINAPI ThreadProc(LPVOID pParameter);

// Data members
protected:
    LONG m_lCancel;
    DWORD m_dwThreadId;
    IMessageEvent* m_pEvent;
};

}  // namespace stdutil

#include "stdsync.inl"

#endif  // __STDSYNC_H__