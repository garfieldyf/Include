///////////////////////////////////////////////////////////////////////////////
// atlsyncutil.h
//
// Copyright(c) 2018, Garfield. All rights reserved.
// Author  : Garfield
// Version : 2.0
// Creation Date : 2010/5/22

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __ATLSYNCUTIL_H__
#define __ATLSYNCUTIL_H__

#ifndef _WIN32_WCE
#include <process.h>
#endif

#include <atlsync.h>
#include <atlsimpcoll.h>
#include "atlmiscutil.h"

///////////////////////////////////////////////////////////////////////////////
// Classes in this file:
//
// CMailSlot
// CNamedPipe
// ITimerHandler
// CTimerTask
// CAsyncTimerTask
// CAsyncTaskImpl<T>
// CMessageThread
// IMessageHandlerImpl<T>
// CInitOnceImpl<T>
// CSharedSRWLock
// CExclusiveSRWLock
// CConditionVariable
// CThreadPool
// CThreadPoolIo
// CThreadPoolWork
// CThreadPoolWait
// CThreadPoolTimer
// CThreadPoolCleanupGroup
// CTPCallbackInstance
// CTPCallbackEnvironment
// CTPIoCallbackImpl<T>
// CTPWorkCallbackImpl<T>
// CTPWaitCallbackImpl<T>
// CTPTimerCallbackImpl<T>
// CTPSimpleCallbackImpl<T>
// CThreadPoolCleanupGroupImpl<T>

namespace atlutil {

///////////////////////////////////////////////////////////////////////////////
// Interface of the CMailSlot class
//

class CMailSlot : public CHandle<CFileHandleTraits>
{
// Constructors
public:
    CMailSlot();

// Operations
public:
    HANDLE Create(LPCTSTR pszName, DWORD dwReadTimeout = MAILSLOT_WAIT_FOREVER, DWORD dwMaxMessageSize = 0, LPSECURITY_ATTRIBUTES psa = NULL);
    BOOL Read(LPVOID pBuffer, DWORD dwNumberOfBytesToRead, DWORD& dwNumberOfBytesRead) const;

// Attributes
public:
    BOOL SetInfo(DWORD dwReadTimeout);
    BOOL GetInfo(LPDWORD pdwMessageCount = NULL, LPDWORD pdwNextSize = NULL, LPDWORD pdwMaxMessageSize = NULL, LPDWORD pdwReadTimeout = NULL) const;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CNamedPipe class
//

class CNamedPipe : public CHandle<CFileHandleTraits>
{
// Constructors
public:
    CNamedPipe();

// Operations
public:
    HANDLE Create(LPCTSTR pszName, DWORD dwInBufferSize, DWORD dwOutBufferSize, DWORD dwOpenMode = PIPE_ACCESS_DUPLEX | FILE_FLAG_WRITE_THROUGH, DWORD dwPipeMode = PIPE_TYPE_BYTE | PIPE_WAIT, DWORD dwMaxInstances = 1, DWORD dwDefaultTimeOut = NMPWAIT_USE_DEFAULT_WAIT, LPSECURITY_ATTRIBUTES psa = NULL);
    HANDLE Open(LPCTSTR pszName, DWORD dwDesiredAccess = GENERIC_READ, DWORD dwShareMode = FILE_SHARE_READ, DWORD dwFlagsAndAttributes = FILE_FLAG_WRITE_THROUGH, LPSECURITY_ATTRIBUTES psa = NULL);

    BOOL Connect(LPOVERLAPPED pOverlapped = NULL) const;
    BOOL Disconnect() const;

    BOOL Read(LPVOID pBuffer, DWORD dwNumberOfBytesToRead, LPDWORD pdwNumberOfBytesRead, LPOVERLAPPED pOverlapped = NULL) const;
    BOOL Write(LPCVOID pBuffer, DWORD dwNumberOfBytesToWrite, LPDWORD pdwNumberOfBytesWritten, LPOVERLAPPED pOverlapped = NULL);

    BOOL Transact(LPVOID pWriteBuffer, DWORD dwWriteBufferSize, LPVOID pReadBuffer, DWORD dwReadBufferSize, LPDWORD pdwBytesRead, LPOVERLAPPED pOverlapped = NULL) const;
    BOOL Peek(LPVOID pBuffer = NULL, DWORD dwBufferSize = 0, LPDWORD pdwBytesRead = NULL, LPDWORD pdwTotalBytesAvail = NULL, LPDWORD pdwBytesLeftThisMessage = NULL) const;

    static BOOL WINAPI Wait(LPCTSTR pszName, DWORD dwTimeout = NMPWAIT_WAIT_FOREVER);
    static BOOL WINAPI Call(LPCTSTR pszName, LPVOID pWriteBuffer, DWORD dwWriteBufferSize, LPVOID pReadBuffer, DWORD dwReadBufferSize, LPDWORD pdwBytesRead, DWORD dwTimeout = NMPWAIT_WAIT_FOREVER);

// Attributes
public:
#if (_WIN32_WINNT >= 0x0600)
    BOOL GetServerProcessId(ULONG& ulProcessId) const;
    BOOL GetClientProcessId(ULONG& ulProcessId) const;

    BOOL GetServerSessionId(ULONG& ulSessionId) const;
    BOOL GetClientSessionId(ULONG& ulSessionId) const;

#if defined(__ATLSTR_H__) || defined(__AFXSTR_H__)
    BOOL GetClientComputerName(CString& strComputerName) const;
#endif

    BOOL GetClientComputerName(LPTSTR pszComputerName, ULONG ulNameLength) const;
#endif

    BOOL GetInfo(LPDWORD pdwFlags = NULL, LPDWORD pdwOutBufferSize = NULL, LPDWORD pdwInBufferSize = NULL, LPDWORD pdwMaxInstances = NULL) const;
    BOOL SetState(LPDWORD pdwMode = NULL, LPDWORD pdwMaxCollectionCount = NULL, LPDWORD pdwCollectDataTimeout = NULL);
    BOOL GetState(LPDWORD pdwState = NULL, LPDWORD pdwCurrentInstances = NULL, LPDWORD pdwMaxCollectionCount = NULL, LPDWORD pdwCollectDataTimeout = NULL, LPTSTR pszUserName = NULL, DWORD dwUserNameSize = 0) const;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the ITimerHandler class
//

interface ATL_NO_VTABLE ITimerHandler
{
    virtual void OnTimer(UINT_PTR nIDEvent, DWORD dwTime) = 0;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CTimerHandlers class
//

class CTimerHandlers
{
// Constructors
public:
    CTimerHandlers();

// Operations
public:
    BOOL Insert(UINT_PTR nIDEvent, ITimerHandler* pHandler);
    BOOL Remove(UINT_PTR nIDEvent);

    ITimerHandler* Lookup(UINT_PTR nIDEvent) const;

#ifdef _CRT_DEBUG_DUMP
    template <typename _Pr> void Dump(const _Pr& _Printer);
#endif

// Implementation
private:
    CTimerHandlers(const CTimerHandlers& that);
    CTimerHandlers& operator=(const CTimerHandlers& that);

// Data members
protected:
    ATL::CSimpleMap<UINT_PTR, ITimerHandler*> m_mapHandlers;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CAsyncTimerHandlers class
//

class CAsyncTimerHandlers : CTimerHandlers
{
// Constructors
public:
    CAsyncTimerHandlers();

// Operations
public:
    BOOL Insert(UINT_PTR nIDEvent, ITimerHandler* pHandler);
    BOOL Remove(UINT_PTR nIDEvent);

    ITimerHandler* Lookup(UINT_PTR nIDEvent) const;

#ifdef _CRT_DEBUG_DUMP
    template <typename _Pr> void Dump(const _Pr& _Printer);
#endif

// Data members
protected:
    mutable ATL::CCriticalSection m_cs;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CTimerTaskBase class
//

class ATL_NO_VTABLE CTimerTaskBase
{
// Constructors/Destructor
protected:
    CTimerTaskBase();
    ~CTimerTaskBase();

// Operations
public:
    BOOL SetDelay(UINT nDelay);

// Attributes
public:
    BOOL IsStopped() const;

// Implementation
private:
    CTimerTaskBase(const CTimerTaskBase& that);
    CTimerTaskBase& operator=(const CTimerTaskBase& that);

// Data members
public:
    UINT_PTR m_nIDEvent;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CTimerTaskT class
//

template <typename _TimerHandlers>
class CTimerTaskT : public CTimerTaskBase
{
// Constructors
public:
    CTimerTaskT();

// Operations
public:
    UINT_PTR Start(ITimerHandler* pHandler, UINT nDelay);
    void Stop();

#ifdef _CRT_DEBUG_DUMP
    template <typename _Pr> static void Dump(const _Pr& _Printer);
#endif

// Implementation
protected:
    static void CALLBACK TimerProc(HWND hWnd, UINT uMsg, UINT nIDEvent, DWORD dwTime);

// Data members
protected:
    static _TimerHandlers _Handlers;
};


#ifdef __ATLWIN_H__
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
    void AsyncTask_Cancel();

// Attributes
public:
    BOOL AsyncTask_IsCancelled() const;

// Implementation
protected:
    void AsyncTask_Finish(DWORD dwTimeout = 5000);

// Data members
protected:
    HANDLE m_hThread;
    volatile BOOL m_bCancel;
};


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
    HRESULT AsyncTask_OnBeforeExecute();
    HRESULT AsyncTask_OnExecute();
    void AsyncTask_OnAfterExecute(HRESULT hrResult);

// Operations
public:
    BOOL AsyncTask_Execute(DWORD dwCreationFlags = 0, LPSECURITY_ATTRIBUTES psa = NULL);

// Implementation
protected:
    static DWORD WINAPI AsyncTaskProc(LPVOID pParameter);

// Message map functions
public:
    LRESULT OnAsyncTaskFinish(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
};
#endif  // __ATLWIN_H__


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
    HANDLE Create(LPTHREAD_START_ROUTINE pStartAddress, LPVOID pParameter, DWORD dwCreationFlags = 0, LPSECURITY_ATTRIBUTES psa = NULL);
    BOOL Quit(DWORD dwTimeout = INFINITE, int nExitCode = 0);

    DWORD Resume() const;
    DWORD Suspend() const;

    operator HANDLE() const;
    BOOL PostMessage(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0L) const;

// Attributes
public:
    int GetPriority() const;
    BOOL SetPriority(int nPriority);

// Data members
public:
    HANDLE m_hThread;
    DWORD  m_dwThreadId;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the IMessageHandlerImpl class
//

template <typename T>
class ATL_NO_VTABLE IMessageHandlerImpl
{
// Overridables
public:
    void MsgHandler_OnProcessMessage(const MSG& msg);

// Operations
public:
    HANDLE MsgHandler_CreateThread(CMessageThread& thread, DWORD dwCreationFlags = 0, LPSECURITY_ATTRIBUTES psa = NULL);

// Implementation
protected:
    static DWORD WINAPI MsgHandler_Run(LPVOID pParameter);
};


#if (_WIN32_WINNT >= 0x0600)
///////////////////////////////////////////////////////////////////////////////
// Interface of the CInitOnceImpl class
//

template <typename T>
class ATL_NO_VTABLE CInitOnceImpl
{
// Constructors
protected:
    CInitOnceImpl();

// Overridables
public:
    BOOL OnInitCallback(LPINIT_ONCE pInitOnce, LPVOID* pContext);

// Operations
public:
    LPVOID ExecuteOnce();

// Implementation
protected:
    static BOOL CALLBACK InitOnceCallback(LPINIT_ONCE pInitOnce, LPVOID pParameter, LPVOID* pContext);

// Data members
protected:
    INIT_ONCE m_InitOnce;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CSharedSRWLock class
//

class CSharedSRWLock
{
// Constructors/Destructor
public:
    explicit CSharedSRWLock(SRWLOCK& SRWLock, BOOL bInitialLock = TRUE);
    ~CSharedSRWLock();

// Operations
public:
    void Lock();
    void Unlock();

// Implementation
private:
    CSharedSRWLock(const CSharedSRWLock& that);
    CSharedSRWLock& operator=(const CSharedSRWLock& that);

// Data members
private:
    BOOL m_bLocked;
    SRWLOCK& m_SRWLock;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CExclusiveSRWLock class
//

class CExclusiveSRWLock
{
// Constructors/Destructor
public:
    explicit CExclusiveSRWLock(SRWLOCK& SRWLock, BOOL bInitialLock = TRUE);
    ~CExclusiveSRWLock();

// Operations
public:
    void Lock();
    void Unlock();

// Implementation
private:
    CExclusiveSRWLock(const CExclusiveSRWLock& that);
    CExclusiveSRWLock& operator=(const CExclusiveSRWLock& that);

// Data members
private:
    BOOL m_bLocked;
    SRWLOCK& m_SRWLock;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CConditionVariable class
//

class CConditionVariable : public CONDITION_VARIABLE
{
// Constructors
public:
    CConditionVariable();

// Operations
public:
    BOOL Sleep(CRITICAL_SECTION& cs, DWORD dwMilliseconds = INFINITE);
    BOOL Sleep(SRWLOCK& SRWLock, DWORD dwMilliseconds = INFINITE, ULONG ulFlags = 0);

    void Wake();
    void WakeAll();

// Implementation
private:
    CConditionVariable(const CConditionVariable& that);
    CConditionVariable& operator=(const CConditionVariable& that);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CThreadPoolTraits class
//

struct CThreadPoolTraits : public CHandleTraits
{
    typedef PTP_POOL HANDLET_PTR;
    static BOOL WINAPI Destroy(HANDLET_PTR hHandle);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CThreadPool class
//

class CThreadPool : public CHandle<CThreadPoolTraits>
{
// Constructors
public:
    CThreadPool();

// Operations
public:
    PTP_POOL Create(LPVOID pReserved = NULL);

    BOOL SetThreadMinimum(DWORD dwMinThreads);
    void SetThreadMaximum(DWORD dwMaxThreads);

#if (_WIN32_WINNT >= 0x0601)
    BOOL SetStackInformation(const TP_POOL_STACK_INFORMATION& stackInfo);
    BOOL QueryStackInformation(TP_POOL_STACK_INFORMATION& stackInfo) const;
#endif
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CThreadPoolIoTraits class
//

struct CThreadPoolIoTraits : public CHandleTraits
{
    typedef PTP_IO HANDLET_PTR;
    static BOOL WINAPI Destroy(HANDLET_PTR hHandle);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CThreadPoolIo class
//

class CThreadPoolIo : public CHandle<CThreadPoolIoTraits>
{
// Constructors
public:
    CThreadPoolIo();

// Operations
public:
    PTP_IO Create(HANDLE hHandle, PTP_WIN32_IO_CALLBACK pfnIoCallback, LPVOID pContext = NULL, PTP_CALLBACK_ENVIRON pEnvironment = NULL);

    void StartIo();
    void CancelIo();
    void Wait(BOOL bCancelPendingCallbacks = FALSE);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CThreadPoolWork class
//

class CThreadPoolWork
{
// Constructors
public:
    CThreadPoolWork(PTP_WORK pWork = NULL);

// Operations
public:
    PTP_WORK Create(PTP_WORK_CALLBACK pfnWorkCallback, LPVOID pContext = NULL, PTP_CALLBACK_ENVIRON pEnvironment = NULL);
    void Close();

    CThreadPoolWork& operator=(PTP_WORK pWork);
    operator PTP_WORK() const;

    void Submit();
    void Wait(BOOL bCancelPendingCallbacks = FALSE);

// Attributes
public:
    BOOL IsEmpty() const;

// Data members
public:
    PTP_WORK m_pWork;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CThreadPoolWait class
//

class CThreadPoolWait
{
// Constructors
public:
    CThreadPoolWait(PTP_WAIT pWait = NULL);

// Operations
public:
    PTP_WAIT Create(PTP_WAIT_CALLBACK pfnWaitCallback, LPVOID pContext = NULL, PTP_CALLBACK_ENVIRON pEnvironment = NULL);
    void Close();

    CThreadPoolWait& operator=(PTP_WAIT pWait);
    operator PTP_WAIT() const;

    void Wait(BOOL bCancelPendingCallbacks = FALSE);
    void SetWait(HANDLE hObject, LPFILETIME pftTimeout = NULL);

// Attributes
public:
    BOOL IsEmpty() const;

// Data members
public:
    PTP_WAIT m_pWait;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CThreadPoolTimer class
//

class CThreadPoolTimer
{
// Constructors
public:
    CThreadPoolTimer(PTP_TIMER pTimer = NULL);

// Operations
public:
    PTP_TIMER Create(PTP_TIMER_CALLBACK pfnTimerCallback, LPVOID pContext = NULL, PTP_CALLBACK_ENVIRON pEnvironment = NULL);
    void Close();

    CThreadPoolTimer& operator=(PTP_TIMER pTimer);
    operator PTP_TIMER() const;

    void Wait(BOOL bCancelPendingCallbacks = FALSE);
    void SetTimer(DWORD dwPeriod, LPFILETIME pftDueTime = NULL, DWORD dwWindowLength = 0);

// Attributes
public:
    BOOL IsEmpty() const;
    BOOL IsTimerSet() const;

// Data members
public:
    PTP_TIMER m_pTimer;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CThreadPoolCleanupGroupTraits class
//

struct CThreadPoolCleanupGroupTraits : public CHandleTraits
{
    typedef PTP_CLEANUP_GROUP HANDLET_PTR;
    static BOOL WINAPI Destroy(HANDLET_PTR hHandle);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CThreadPoolCleanupGroup class
//

class CThreadPoolCleanupGroup : public CHandle<CThreadPoolCleanupGroupTraits>
{
// Constructors
public:
    CThreadPoolCleanupGroup();

// Operations
public:
    PTP_CLEANUP_GROUP Create();
    void CloseMembers(LPVOID pCleanupContext = NULL, BOOL bCancelPendingCallbacks = FALSE);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CTPCallbackInstance class
//

class CTPCallbackInstance
{
// Constructors
public:
    CTPCallbackInstance(PTP_CALLBACK_INSTANCE pInstance = NULL);

// Operations
public:
    CTPCallbackInstance& operator=(PTP_CALLBACK_INSTANCE pInstance);
    operator PTP_CALLBACK_INSTANCE();

    BOOL CallbackMayRunLong();
    void DisassociateCurrentThreadFromCallback();

    void FreeLibraryWhenCallbackReturns(HMODULE hModule);
    void LeaveCriticalSectionWhenCallbackReturns(LPCRITICAL_SECTION pcs);

    void SetEventWhenCallbackReturns(HANDLE hEvent);
    void ReleaseMutexWhenCallbackReturns(HANDLE hMutex);
    void ReleaseSemaphoreWhenCallbackReturns(HANDLE hSemaphore, DWORD dwIncSemaphoreCount);

// Attributes
public:
    BOOL IsEmpty() const;

// Data members
public:
    PTP_CALLBACK_INSTANCE m_pInstance;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CTPCallbackEnvironment class
//

class CTPCallbackEnvironment : public TP_CALLBACK_ENVIRON
{
// Constructors
public:
    CTPCallbackEnvironment();

// Operations
public:
    void Destroy();
    operator PTP_CALLBACK_ENVIRON();

    void SetCallbackRunsLong();
    void SetCallbackLibrary(LPVOID pModule);

#if (_WIN32_WINNT >= 0x0601)
    void SetCallbackPersistent();
    void SetCallbackPriority(TP_CALLBACK_PRIORITY nPriority);
#endif

    void SetCallbackPool(PTP_POOL pPool = NULL);
    void SetCallbackCleanupGroup(PTP_CLEANUP_GROUP pCleanupGroup, PTP_CLEANUP_GROUP_CANCEL_CALLBACK pfnCancelCallback = NULL);

// Implementation
private:
    CTPCallbackEnvironment(const CTPCallbackEnvironment& that);
    CTPCallbackEnvironment& operator=(const CTPCallbackEnvironment& that);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CTPIoCallbackImpl class
//

template <typename T>
class ATL_NO_VTABLE CTPIoCallbackImpl
{
// Overridables
public:
    void TPIo_OnCallback(CTPCallbackInstance instance, PTP_IO pIo, ULONG_PTR dwNumberOfBytesTransferred, LPVOID pOverlapped, ULONG ulIoResult);

// Operations
public:
    PTP_IO TPIo_CreateIo(CThreadPoolIo& TPIo, HANDLE hHandle, PTP_CALLBACK_ENVIRON pEnvironment = NULL);

// Implementation
protected:
    static void CALLBACK TPIo_Callback(PTP_CALLBACK_INSTANCE pInstance, LPVOID pContext, LPVOID pOverlapped, ULONG ulIoResult, ULONG_PTR dwNumberOfBytesTransferred, PTP_IO pIo);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CTPWorkCallbackImpl class
//

template <typename T>
class ATL_NO_VTABLE CTPWorkCallbackImpl
{
// Overridables
public:
    void TPWork_OnCallback(CTPCallbackInstance instance, PTP_WORK pWork);

// Operations
public:
    PTP_WORK TPWork_CreateWork(PTP_CALLBACK_ENVIRON pEnvironment = NULL);

// Implementation
protected:
    static void CALLBACK TPWork_Callback(PTP_CALLBACK_INSTANCE pInstance, LPVOID pContext, PTP_WORK pWork);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CTPWaitCallbackImpl class
//

template <typename T>
class ATL_NO_VTABLE CTPWaitCallbackImpl
{
// Overridables
public:
    void TPWait_OnCallback(CTPCallbackInstance instance, PTP_WAIT pWait, TP_WAIT_RESULT dwWaitResult);

// Operations
public:
    PTP_WAIT TPWait_CreateWait(PTP_CALLBACK_ENVIRON pEnvironment = NULL);

// Implementation
protected:
    static void CALLBACK TPWait_Callback(PTP_CALLBACK_INSTANCE pInstance, LPVOID pContext, PTP_WAIT pWait, TP_WAIT_RESULT dwWaitResult);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CTPTimerCallbackImpl class
//

template <typename T>
class ATL_NO_VTABLE CTPTimerCallbackImpl
{
// Overridables
public:
    void TPTimer_OnCallback(CTPCallbackInstance instance, PTP_TIMER pTimer);

// Operations
public:
    PTP_TIMER TPTimer_CreateTimer(PTP_CALLBACK_ENVIRON pEnvironment = NULL);

// Implementation
protected:
    static void CALLBACK TPTimer_Callback(PTP_CALLBACK_INSTANCE pInstance, LPVOID pContext, PTP_TIMER pTimer);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CTPSimpleCallbackImpl class
//

template <typename T>
class ATL_NO_VTABLE CTPSimpleCallbackImpl
{
// Overridables
public:
    void TPSimple_OnCallback(CTPCallbackInstance instance);

// Operations
public:
    BOOL TPSimple_Submit(PTP_CALLBACK_ENVIRON pEnvironment = NULL);

// Implementation
protected:
    static void CALLBACK TPSimple_Callback(PTP_CALLBACK_INSTANCE pInstance, LPVOID pContext);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CThreadPoolCleanupGroupImpl class
//

template <typename T>
class ATL_NO_VTABLE CThreadPoolCleanupGroupImpl
{
// Overridables
public:
    void TPCleanupGroup_OnCallback(LPVOID pObjectContext);

// Operations
public:
    PTP_CLEANUP_GROUP TPCleanupGroup_Create();
    void TPCleanupGroup_Close();

    void TPCleanupGroup_Register(PTP_CALLBACK_ENVIRON pEnvironment = NULL);
    void TPCleanupGroup_CloseMembers(BOOL bCancelPendingCallbacks = FALSE);

// Attributes
public:
    BOOL TPCleanupGroup_IsEmpty() const;

// Implementation
protected:
    static void CALLBACK TPCleanupGroup_Callback(LPVOID pObjectContext, LPVOID pCleanupContext);

// Data members
protected:
    CThreadPoolCleanupGroup m_cleanupGroup;
};
#endif  // (_WIN32_WINNT >= 0x0600)

}  // namespace atlutil

#include "atlsyncutil.inl"

#endif  // __ATLSYNCUTIL_H__