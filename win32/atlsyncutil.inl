///////////////////////////////////////////////////////////////////////////////
// atlsyncutil.inl
//
// Copyright(c) 2018, Garfield. All rights reserved.
// Author  : Garfield
// Version : 2.0
// Creation Date : 2010/5/22

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __ATLSYNCUTIL_INL__
#define __ATLSYNCUTIL_INL__

#ifndef __ATLSYNCUTIL_H__
    #error atlsyncutil.inl requires atlsyncutil.h to be included first
#endif

namespace atlutil {

///////////////////////////////////////////////////////////////////////////////
// Type definition
//

typedef CTimerTaskT<CTimerHandlers> CTimerTask;
typedef CTimerTaskT<CAsyncTimerHandlers> CAsyncTimerTask;


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CMailSlot class
//

inline CMailSlot::CMailSlot()
{
}

inline HANDLE CMailSlot::Create(LPCTSTR pszName, DWORD dwReadTimeout/* = MAILSLOT_WAIT_FOREVER*/, DWORD dwMaxMessageSize/* = 0*/, LPSECURITY_ATTRIBUTES psa/* = NULL*/)
{
    ATLASSERT(pszName);
    ATLASSERT(IsEmpty());

    return (m_hHandle = ::CreateMailslot(pszName, dwMaxMessageSize, dwReadTimeout, psa));
}

inline BOOL CMailSlot::Read(LPVOID pBuffer, DWORD dwNumberOfBytesToRead, DWORD& dwNumberOfBytesRead) const
{
    ATLASSERT(pBuffer);
    ATLASSERT(!IsEmpty());

    return ::ReadFile(m_hHandle, pBuffer, dwNumberOfBytesToRead, &dwNumberOfBytesRead, NULL);
}

inline BOOL CMailSlot::SetInfo(DWORD dwReadTimeout)
{
    ATLASSERT(!IsEmpty());
    return ::SetMailslotInfo(m_hHandle, dwReadTimeout);
}

inline BOOL CMailSlot::GetInfo(LPDWORD pdwMessageCount/* = NULL*/, LPDWORD pdwNextSize/* = NULL*/, LPDWORD pdwMaxMessageSize/* = NULL*/, LPDWORD pdwReadTimeout/* = NULL*/) const
{
    ATLASSERT(!IsEmpty());
    return ::GetMailslotInfo(m_hHandle, pdwMaxMessageSize, pdwNextSize, pdwMessageCount, pdwReadTimeout);
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CNamedPipe class
//

inline CNamedPipe::CNamedPipe()
{
}

inline HANDLE CNamedPipe::Create(LPCTSTR pszName, DWORD dwInBufferSize, DWORD dwOutBufferSize, DWORD dwOpenMode/* = PIPE_ACCESS_DUPLEX | FILE_FLAG_WRITE_THROUGH*/, DWORD dwPipeMode/* = PIPE_TYPE_BYTE | PIPE_WAIT*/, DWORD dwMaxInstances/* = 1*/, DWORD dwDefaultTimeOut/* = NMPWAIT_USE_DEFAULT_WAIT*/, LPSECURITY_ATTRIBUTES psa/* = NULL*/)
{
    ATLASSERT(pszName);
    ATLASSERT(IsEmpty());

    return (m_hHandle = ::CreateNamedPipe(pszName, dwOpenMode, dwPipeMode, dwMaxInstances, dwOutBufferSize, dwInBufferSize, dwDefaultTimeOut, psa));
}

inline HANDLE CNamedPipe::Open(LPCTSTR pszName, DWORD dwDesiredAccess/* = GENERIC_READ*/, DWORD dwShareMode/* = FILE_SHARE_READ*/, DWORD dwFlagsAndAttributes/* = FILE_FLAG_WRITE_THROUGH*/, LPSECURITY_ATTRIBUTES psa/* = NULL*/)
{
    ATLASSERT(pszName);
    ATLASSERT(IsEmpty());

    return (m_hHandle = ::CreateFile(pszName, dwDesiredAccess, dwShareMode, psa, OPEN_EXISTING, dwFlagsAndAttributes, NULL));
}

inline BOOL CNamedPipe::Connect(LPOVERLAPPED pOverlapped/* = NULL*/) const
{
    ATLASSERT(!IsEmpty());
    return ::ConnectNamedPipe(m_hHandle, pOverlapped);
}

inline BOOL CNamedPipe::Disconnect() const
{
    ATLASSERT(!IsEmpty());
    return ::DisconnectNamedPipe(m_hHandle);
}

inline BOOL CNamedPipe::Read(LPVOID pBuffer, DWORD dwNumberOfBytesToRead, LPDWORD pdwNumberOfBytesRead, LPOVERLAPPED pOverlapped/* = NULL*/) const
{
    ATLASSERT(!IsEmpty());
    return ::ReadFile(m_hHandle, pBuffer, dwNumberOfBytesToRead, pdwNumberOfBytesRead, pOverlapped);
}

inline BOOL CNamedPipe::Write(LPCVOID pBuffer, DWORD dwNumberOfBytesToWrite, LPDWORD pdwNumberOfBytesWritten, LPOVERLAPPED pOverlapped/* = NULL*/)
{
    ATLASSERT(!IsEmpty());
    return ::WriteFile(m_hHandle, pBuffer, dwNumberOfBytesToWrite, pdwNumberOfBytesWritten, pOverlapped);
}

inline BOOL CNamedPipe::Transact(LPVOID pWriteBuffer, DWORD dwWriteBufferSize, LPVOID pReadBuffer, DWORD dwReadBufferSize, LPDWORD pdwBytesRead, LPOVERLAPPED pOverlapped/* = NULL*/) const
{
    ATLASSERT(!IsEmpty());
    return ::TransactNamedPipe(m_hHandle, pWriteBuffer, dwWriteBufferSize, pReadBuffer, dwReadBufferSize, pdwBytesRead, pOverlapped);
}

inline BOOL CNamedPipe::Peek(LPVOID pBuffer/* = NULL*/, DWORD dwBufferSize/* = 0*/, LPDWORD pdwBytesRead/* = NULL*/, LPDWORD pdwTotalBytesAvail/* = NULL*/, LPDWORD pdwBytesLeftThisMessage/* = NULL*/) const
{
    ATLASSERT(!IsEmpty());
    return ::PeekNamedPipe(m_hHandle, pBuffer, dwBufferSize, pdwBytesRead, pdwTotalBytesAvail, pdwBytesLeftThisMessage);
}

inline BOOL WINAPI CNamedPipe::Wait(LPCTSTR pszName, DWORD dwTimeout/* = NMPWAIT_WAIT_FOREVER*/)
{
    ATLASSERT(pszName);
    return ::WaitNamedPipe(pszName, dwTimeout);
}

inline BOOL WINAPI CNamedPipe::Call(LPCTSTR pszName, LPVOID pWriteBuffer, DWORD dwWriteBufferSize, LPVOID pReadBuffer, DWORD dwReadBufferSize, LPDWORD pdwBytesRead, DWORD dwTimeout/* = NMPWAIT_WAIT_FOREVER*/)
{
    ATLASSERT(pszName);
    return ::CallNamedPipe(pszName, pWriteBuffer, dwWriteBufferSize, pReadBuffer, dwReadBufferSize, pdwBytesRead, dwTimeout);
}

#if (_WIN32_WINNT >= 0x0600)
inline BOOL CNamedPipe::GetServerProcessId(ULONG& ulProcessId) const
{
    ATLASSERT(!IsEmpty());
    return ::GetNamedPipeServerProcessId(m_hHandle, &ulProcessId);
}

inline BOOL CNamedPipe::GetClientProcessId(ULONG& ulProcessId) const
{
    ATLASSERT(!IsEmpty());
    return ::GetNamedPipeClientProcessId(m_hHandle, &ulProcessId);
}

inline BOOL CNamedPipe::GetServerSessionId(ULONG& ulSessionId) const
{
    ATLASSERT(!IsEmpty());
    return ::GetNamedPipeServerSessionId(m_hHandle, &ulSessionId);
}

inline BOOL CNamedPipe::GetClientSessionId(ULONG& ulSessionId) const
{
    ATLASSERT(!IsEmpty());
    return ::GetNamedPipeClientSessionId(m_hHandle, &ulSessionId);
}

#if defined(__ATLSTR_H__) || defined(__AFXSTR_H__)
inline BOOL CNamedPipe::GetClientComputerName(CString& strComputerName) const
{
    ATLASSERT(!IsEmpty());

    BOOL bSuccessful = ::GetNamedPipeClientComputerName(m_hHandle, strComputerName.GetBuffer(MAX_PATH), sizeof(TCHAR) * MAX_PATH);
    strComputerName.ReleaseBuffer();

    return bSuccessful;
}
#endif  // defined(__ATLSTR_H__) || defined(__AFXSTR_H__)

inline BOOL CNamedPipe::GetClientComputerName(LPTSTR pszComputerName, ULONG ulNameLength) const
{
    ATLASSERT(!IsEmpty());
    ATLASSERT(pszComputerName);

    return ::GetNamedPipeClientComputerName(m_hHandle, pszComputerName, ulNameLength);
}
#endif  // (_WIN32_WINNT >= 0x0600)

inline BOOL CNamedPipe::GetInfo(LPDWORD pdwFlags/* = NULL*/, LPDWORD pdwOutBufferSize/* = NULL*/, LPDWORD pdwInBufferSize/* = NULL*/, LPDWORD pdwMaxInstances/* = NULL*/) const
{
    ATLASSERT(!IsEmpty());
    return ::GetNamedPipeInfo(m_hHandle, pdwFlags, pdwOutBufferSize, pdwInBufferSize, pdwMaxInstances);
}

inline BOOL CNamedPipe::SetState(LPDWORD pdwMode/* = NULL*/, LPDWORD pdwMaxCollectionCount/* = NULL*/, LPDWORD pdwCollectDataTimeout/* = NULL*/)
{
    ATLASSERT(!IsEmpty());
    return ::SetNamedPipeHandleState(m_hHandle, pdwMode, pdwMaxCollectionCount, pdwCollectDataTimeout);
}

inline BOOL CNamedPipe::GetState(LPDWORD pdwState/* = NULL*/, LPDWORD pdwCurrentInstances/* = NULL*/, LPDWORD pdwMaxCollectionCount/* = NULL*/, LPDWORD pdwCollectDataTimeout/* = NULL*/, LPTSTR pszUserName/* = NULL*/, DWORD dwUserNameSize/* = 0*/) const
{
    ATLASSERT(!IsEmpty());
    return ::GetNamedPipeHandleState(m_hHandle, pdwState, pdwCurrentInstances, pdwMaxCollectionCount, pdwCollectDataTimeout, pszUserName, dwUserNameSize);
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CTimerHandlers class
//

inline CTimerHandlers::CTimerHandlers()
{
}

inline BOOL CTimerHandlers::Insert(UINT_PTR nIDEvent, ITimerHandler* pHandler)
{
    ATLASSERT(pHandler);
    ATLASSERT(nIDEvent != 0);

    const int nIndex = m_mapHandlers.FindKey(0);
    return (nIndex != -1 ? m_mapHandlers.SetAtIndex(nIndex, nIDEvent, pHandler) : m_mapHandlers.Add(nIDEvent, pHandler));
}

inline BOOL CTimerHandlers::Remove(UINT_PTR nIDEvent)
{
    ATLASSERT(nIDEvent != 0);

    const int nIndex = m_mapHandlers.FindKey(nIDEvent);
    return (nIndex != -1 && m_mapHandlers.SetAtIndex(nIndex, 0, NULL));
}

inline ITimerHandler* CTimerHandlers::Lookup(UINT_PTR nIDEvent) const
{
    ATLASSERT(nIDEvent != 0);
    return m_mapHandlers.Lookup(nIDEvent);
}

#ifdef _CRT_DEBUG_DUMP
template <typename _Pr>
inline void CTimerHandlers::Dump(const _Pr& _Printer)
{
    _Printer(_T("\n[ EVENT HANDLER COUNT = %u ]\n"), m_mapHandlers.GetSize());
    for (int i = 0; i < m_mapHandlers.GetSize(); ++i)
        _Printer(_T("[ Event Id = %u, Event Handler = 0x%p ]\n"), m_mapHandlers.GetKeyAt(i), m_mapHandlers.GetValueAt(i));

    _Printer(_T("\n"));
}
#endif  // _CRT_DEBUG_DUMP


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CAsyncTimerHandlers class
//

inline CAsyncTimerHandlers::CAsyncTimerHandlers()
{
}

inline BOOL CAsyncTimerHandlers::Insert(UINT_PTR nIDEvent, ITimerHandler* pHandler)
{
    ATLASSERT(pHandler);
    ATLASSERT(nIDEvent != 0);

    ATL::CCritSecLock lock(m_cs);
    return CTimerHandlers::Insert(nIDEvent, pHandler);
}

inline BOOL CAsyncTimerHandlers::Remove(UINT_PTR nIDEvent)
{
    ATLASSERT(nIDEvent != 0);

    ATL::CCritSecLock lock(m_cs);
    return CTimerHandlers::Remove(nIDEvent);
}

inline ITimerHandler* CAsyncTimerHandlers::Lookup(UINT_PTR nIDEvent) const
{
    ATLASSERT(nIDEvent != 0);

    ATL::CCritSecLock lock(m_cs);
    return CTimerHandlers::Lookup(nIDEvent);
}

#ifdef _CRT_DEBUG_DUMP
template <typename _Pr>
inline void CAsyncTimerHandlers::Dump(const _Pr& _Printer)
{
    ATL::CCritSecLock lock(m_cs);
    CTimerHandlers::Dump(_Printer);
}
#endif  // _CRT_DEBUG_DUMP


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CTimerTaskBase class
//

inline CTimerTaskBase::CTimerTaskBase()
    : m_nIDEvent(0)
{
}

inline CTimerTaskBase::~CTimerTaskBase()
{
    ATLASSERT(IsStopped());
}

inline BOOL CTimerTaskBase::SetDelay(UINT nDelay)
{
    ATLASSERT(!IsStopped());
    return (::SetTimer(NULL, m_nIDEvent, nDelay, NULL) == m_nIDEvent);
}

inline BOOL CTimerTaskBase::IsStopped() const
{
    return (m_nIDEvent == 0);
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CTimerTaskT class
//

template <typename _TimerHandlers>
__declspec(selectany) _TimerHandlers CTimerTaskT<_TimerHandlers>::_Handlers;

template <typename _TimerHandlers>
inline CTimerTaskT<_TimerHandlers>::CTimerTaskT()
{
}

template <typename _TimerHandlers>
inline UINT_PTR CTimerTaskT<_TimerHandlers>::Start(ITimerHandler* pHandler, UINT nDelay)
{
    ATLASSERT(pHandler);
    ATLASSERT(IsStopped());

    if ((m_nIDEvent = ::SetTimer(NULL, 0, nDelay, TimerProc)) != 0)
        ATLVERIFY(_Handlers.Insert(m_nIDEvent, pHandler));

    return m_nIDEvent;
}

template <typename _TimerHandlers>
inline void CTimerTaskT<_TimerHandlers>::Stop()
{
    if (m_nIDEvent != 0)
    {
        ATLVERIFY(::KillTimer(NULL, m_nIDEvent));
        ATLVERIFY(_Handlers.Remove(m_nIDEvent));
        m_nIDEvent = 0;
    }
}

#ifdef _CRT_DEBUG_DUMP
template <typename _TimerHandlers> template <typename _Pr>
inline void CTimerTaskT<_TimerHandlers>::Dump(const _Pr& _Printer)
{
    _Handlers.Dump(_Printer);
}
#endif  // _CRT_DEBUG_DUMP

template <typename _TimerHandlers>
inline void CALLBACK CTimerTaskT<_TimerHandlers>::TimerProc(HWND /*hWnd*/, UINT /*uMsg*/, UINT nIDEvent, DWORD dwTime)
{
    if (ITimerHandler* pHandler = _Handlers.Lookup(nIDEvent))
        pHandler->OnTimer(nIDEvent, dwTime);
}


#ifdef __ATLWIN_H__
///////////////////////////////////////////////////////////////////////////////
// Implementation of the CAsyncTaskBase class
//

inline CAsyncTaskBase::CAsyncTaskBase()
    : m_bCancel(FALSE), m_hThread(NULL)
{
}

inline CAsyncTaskBase::~CAsyncTaskBase()
{
    ATLASSERT(!m_bCancel);
    ATLASSERT(m_hThread == NULL);
}

inline void CAsyncTaskBase::AsyncTask_Cancel()
{
    m_bCancel = TRUE;
}

inline BOOL CAsyncTaskBase::AsyncTask_IsCancelled() const
{
    return m_bCancel;
}

inline void CAsyncTaskBase::AsyncTask_Finish(DWORD dwTimeout/* = 5000*/)
{
    ATLASSERT(m_hThread);

    _COND_VERIFY(::WaitForSingleObject(m_hThread, dwTimeout), WAIT_OBJECT_0);
    ATLVERIFY(::CloseHandle(m_hThread));

    m_hThread = NULL;
    m_bCancel = FALSE;
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CAsyncTaskImpl class
//

template <typename T>
inline CAsyncTaskImpl<T>::CAsyncTaskImpl()
{
}

template <typename T>
inline HRESULT CAsyncTaskImpl<T>::AsyncTask_OnBeforeExecute()
{
    // Runs on the UI thread before AsyncTask_OnExecute().
    // Return S_OK, if successful
    // Return COM-defined error code, if failed
    return S_OK;
}

template <typename T>
inline HRESULT CAsyncTaskImpl<T>::AsyncTask_OnExecute()
{
    // Override this method to perform a computation on a background thread.
    // Return S_OK, if successful
    // Return E_ABORT, if cancelled
    // Return COM-defined error code, if failed
    return S_OK;
}

template <typename T>
inline void CAsyncTaskImpl<T>::AsyncTask_OnAfterExecute(HRESULT /*hrResult*/)
{
    // Runs on the UI thread after AsyncTask_OnExecute().
}

template <typename T>
inline BOOL CAsyncTaskImpl<T>::AsyncTask_Execute(DWORD dwCreationFlags/* = 0*/, LPSECURITY_ATTRIBUTES psa/* = NULL*/)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);
    ATLASSERT(!m_bCancel);
    ATLASSERT(m_hThread == NULL);

    HRESULT hr = pThis->AsyncTask_OnBeforeExecute();
    if (SUCCEEDED(hr))
        m_hThread = _CreateThread(psa, 0, T::AsyncTaskProc, pThis, dwCreationFlags, NULL);
    else
        pThis->AsyncTask_OnAfterExecute(hr);

    return (m_hThread != NULL);
}

template <typename T>
inline DWORD WINAPI CAsyncTaskImpl<T>::AsyncTaskProc(LPVOID pParameter)
{
    T* pThis = reinterpret_cast<T*>(pParameter);
    ATLASSERT(pThis);
    ATLASSERT(::IsWindow(pThis->m_hWnd));

    ATLVERIFY(::PostMessage(pThis->m_hWnd, WM_ASYNCTAST_FINISH, 0, pThis->AsyncTask_OnExecute()));
    ATLTRACE(_T("Thread exit - CAsyncTaskImpl<T>::AsyncTaskProc()\n"));

    return 0;
}

template <typename T>
inline LRESULT CAsyncTaskImpl<T>::OnAsyncTaskFinish(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);

    pThis->AsyncTask_OnAfterExecute(lParam);
    pThis->AsyncTask_Finish();

    return 0L;
}
#endif  // __ATLWIN_H__


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CMessageThread class
//

inline CMessageThread::CMessageThread()
    : m_hThread(NULL), m_dwThreadId(0)
{
}

inline CMessageThread::~CMessageThread()
{
    ATLASSERT(m_hThread == NULL);
}

inline HANDLE CMessageThread::Create(LPTHREAD_START_ROUTINE pStartAddress, LPVOID pParameter, DWORD dwCreationFlags/* = 0*/, LPSECURITY_ATTRIBUTES psa/* = NULL*/)
{
    ATLASSERT(pParameter);
    ATLASSERT(pStartAddress);
    ATLASSERT(m_hThread == NULL);

    return (m_hThread = _CreateThread(psa, 0, pStartAddress, pParameter, dwCreationFlags, &m_dwThreadId));
}

inline BOOL CMessageThread::Quit(DWORD dwTimeout/* = INFINITE*/, int nExitCode/* = 0*/)
{
    BOOL bSuccessful = TRUE;
    if (m_hThread != NULL)
    {
        ATLVERIFY(::PostThreadMessage(m_dwThreadId, WM_QUIT, nExitCode, 0L));
        bSuccessful = (::WaitForSingleObject(m_hThread, dwTimeout) == WAIT_OBJECT_0);

        ATLVERIFY(::CloseHandle(m_hThread));
        m_hThread = NULL, m_dwThreadId = 0;
        ATLTRACE(_T("CMessageThread::Quit() - quit the message loop.\n"));
    }

    return bSuccessful;
}

inline DWORD CMessageThread::Resume() const
{
    ATLASSERT(m_hThread);
    return ::ResumeThread(m_hThread);
}

inline DWORD CMessageThread::Suspend() const
{
    ATLASSERT(m_hThread);
    return SuspendThread(m_hThread);
}

inline CMessageThread::operator HANDLE() const
{
    return m_hThread;
}

inline BOOL CMessageThread::PostMessage(UINT uMsg, WPARAM wParam/* = 0*/, LPARAM lParam/* = 0L*/) const
{
    ATLASSERT(m_hThread);
    ATLASSERT(uMsg != WM_QUIT);

    return ::PostThreadMessage(m_dwThreadId, uMsg, wParam, lParam);
}

inline int CMessageThread::GetPriority() const
{
    ATLASSERT(m_hThread);
    return ::GetThreadPriority(m_hThread);
}

inline BOOL CMessageThread::SetPriority(int nPriority)
{
    ATLASSERT(m_hThread);
    return ::SetThreadPriority(m_hThread, nPriority);
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the IMessageHandlerImpl class
//

template <typename T>
inline void IMessageHandlerImpl<T>::MsgHandler_OnProcessMessage(const MSG& /*msg*/)
{
}

template <typename T>
inline HANDLE IMessageHandlerImpl<T>::MsgHandler_CreateThread(CMessageThread& thread, DWORD dwCreationFlags/* = 0*/, LPSECURITY_ATTRIBUTES psa/* = NULL*/)
{
    ATLASSERT(thread.m_hThread == NULL);
    return thread.Create(T::MsgHandler_Run, static_cast<T*>(this), dwCreationFlags, psa);
}

template <typename T>
inline DWORD CALLBACK IMessageHandlerImpl<T>::MsgHandler_Run(LPVOID pParameter)
{
    T* pThis = reinterpret_cast<T*>(pParameter);
    ATLASSERT(pThis);

    MSG msg;
    while (::GetMessage(&msg, NULL, 0, 0) > 0)
        pThis->MsgHandler_OnProcessMessage(msg);

    ATLTRACE(_T("IMessageHandlerImpl<T>::MsgHandler_Run() - exiting.\n"));
    return msg.wParam;
}


#if (_WIN32_WINNT >= 0x0600)
///////////////////////////////////////////////////////////////////////////////
// Implementation of the CInitOnceImpl class
//

template <typename T>
inline CInitOnceImpl<T>::CInitOnceImpl()
{
    ::InitOnceInitialize(&m_InitOnce);
}

template <typename T>
inline BOOL CInitOnceImpl<T>::OnInitCallback(LPINIT_ONCE /*pInitOnce*/, LPVOID* /*pContext*/)
{
    ATLASSERT(FALSE && _T("MUST BE IMPLEMENTED IN A DERIVED CLASS."));
    return TRUE;
}

template <typename T>
inline LPVOID CInitOnceImpl<T>::ExecuteOnce()
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);

    LPVOID pContext = NULL;
    ::InitOnceExecuteOnce(&m_InitOnce, T::InitOnceCallback, pThis, &pContext);

    return pContext;
}

template <typename T>
inline BOOL CALLBACK CInitOnceImpl<T>::InitOnceCallback(LPINIT_ONCE pInitOnce, LPVOID pParameter, LPVOID* pContext)
{
    T* pThis = reinterpret_cast<T*>(pParameter);
    ATLASSERT(pThis);

    return pThis->OnInitCallback(pInitOnce, pContext);
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CSharedSRWLock class
//

inline CSharedSRWLock::CSharedSRWLock(SRWLOCK& SRWLock, BOOL bInitialLock/* = TRUE*/)
    : m_bLocked(FALSE), m_SRWLock(SRWLock)
{
    if (bInitialLock) Lock();
}

inline CSharedSRWLock::~CSharedSRWLock()
{
    if (m_bLocked) Unlock();
}

inline void CSharedSRWLock::Lock()
{
    ATLASSERT(!m_bLocked);

    m_bLocked = TRUE;
    ::AcquireSRWLockShared(&m_SRWLock);
}

inline void CSharedSRWLock::Unlock()
{
    ATLASSERT(m_bLocked);

    m_bLocked = FALSE;
    ::ReleaseSRWLockShared(&m_SRWLock);
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CExclusiveSRWLock class
//

inline CExclusiveSRWLock::CExclusiveSRWLock(SRWLOCK& SRWLock, BOOL bInitialLock/* = TRUE*/)
    : m_bLocked(FALSE), m_SRWLock(SRWLock)
{
    if (bInitialLock) Lock();
}

inline CExclusiveSRWLock::~CExclusiveSRWLock()
{
    if (m_bLocked) Unlock();
}

inline void CExclusiveSRWLock::Lock()
{
    ATLASSERT(!m_bLocked);

    m_bLocked = TRUE;
    ::AcquireSRWLockExclusive(&m_SRWLock);
}

inline void CExclusiveSRWLock::Unlock()
{
    ATLASSERT(m_bLocked);

    m_bLocked = FALSE;
    ::ReleaseSRWLockExclusive(&m_SRWLock);
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CConditionVariable class
//

inline CConditionVariable::CConditionVariable()
{
    ::InitializeConditionVariable(this);
}

inline BOOL CConditionVariable::Sleep(CRITICAL_SECTION& cs, DWORD dwMilliseconds/* = INFINITE*/)
{
    return ::SleepConditionVariableCS(this, &cs, dwMilliseconds);
}

inline BOOL CConditionVariable::Sleep(SRWLOCK& SRWLock, DWORD dwMilliseconds/* = INFINITE*/, ULONG ulFlags/* = 0*/)
{
    return ::SleepConditionVariableSRW(this, &SRWLock, dwMilliseconds, ulFlags);
}

inline void CConditionVariable::Wake()
{
    ::WakeConditionVariable(this);
}

inline void CConditionVariable::WakeAll()
{
    ::WakeAllConditionVariable(this);
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CThreadPoolTraits class
//

inline BOOL WINAPI CThreadPoolTraits::Destroy(HANDLET_PTR hHandle)
{
    ::CloseThreadpool(hHandle);
    return TRUE;
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CThreadPool class
//

inline CThreadPool::CThreadPool()
{
}

inline PTP_POOL CThreadPool::Create(LPVOID pReserved/* = NULL*/)
{
    ATLASSERT(IsEmpty());
    return (m_hHandle = ::CreateThreadpool(pReserved));
}

inline BOOL CThreadPool::SetThreadMinimum(DWORD dwMinThreads)
{
    ATLASSERT(!IsEmpty());
    return ::SetThreadpoolThreadMinimum(m_hHandle, dwMinThreads);
}

inline void CThreadPool::SetThreadMaximum(DWORD dwMaxThreads)
{
    ATLASSERT(!IsEmpty());
    ::SetThreadpoolThreadMaximum(m_hHandle, dwMaxThreads);
}

#if (_WIN32_WINNT >= 0x0601)
inline BOOL CThreadPool::SetStackInformation(const TP_POOL_STACK_INFORMATION& stackInfo)
{
    ATLASSERT(!IsEmpty());
    return ::SetThreadpoolStackInformation(m_hHandle, (PTP_POOL_STACK_INFORMATION)&stackInfo);
}

inline BOOL CThreadPool::QueryStackInformation(TP_POOL_STACK_INFORMATION& stackInfo) const
{
    ATLASSERT(!IsEmpty());
    return ::QueryThreadpoolStackInformation(m_hHandle, &stackInfo);
}
#endif  // (_WIN32_WINNT >= 0x0601)


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CThreadPoolIoTraits class
//

inline BOOL WINAPI CThreadPoolIoTraits::Destroy(HANDLET_PTR hHandle)
{
    ::CloseThreadpoolIo(hHandle);
    return TRUE;
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CThreadPoolIo class
//

inline CThreadPoolIo::CThreadPoolIo()
{
}

inline PTP_IO CThreadPoolIo::Create(HANDLE hHandle, PTP_WIN32_IO_CALLBACK pfnIoCallback, LPVOID pContext/* = NULL*/, PTP_CALLBACK_ENVIRON pEnvironment/* = NULL*/)
{
    ATLASSERT(hHandle);
    ATLASSERT(IsEmpty());
    ATLASSERT(pfnIoCallback);

    return (m_hHandle = ::CreateThreadpoolIo(hHandle, pfnIoCallback, pContext, pEnvironment));
}

inline void CThreadPoolIo::StartIo()
{
    ATLASSERT(!IsEmpty());
    ::StartThreadpoolIo(m_hHandle);
}

inline void CThreadPoolIo::CancelIo()
{
    ATLASSERT(!IsEmpty());
    ::CancelThreadpoolIo(m_hHandle);
}

inline void CThreadPoolIo::Wait(BOOL bCancelPendingCallbacks/* = FALSE*/)
{
    ATLASSERT(!IsEmpty());
    ::WaitForThreadpoolIoCallbacks(m_hHandle, bCancelPendingCallbacks);
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CThreadPoolWork class
//

inline CThreadPoolWork::CThreadPoolWork(PTP_WORK pWork/* = NULL*/)
    : m_pWork(pWork)
{
}

inline PTP_WORK CThreadPoolWork::Create(PTP_WORK_CALLBACK pfnWorkCallback, LPVOID pContext/* = NULL*/, PTP_CALLBACK_ENVIRON pEnvironment/* = NULL*/)
{
    ATLASSERT(IsEmpty());
    ATLASSERT(pfnWorkCallback);

    return (m_pWork = ::CreateThreadpoolWork(pfnWorkCallback, pContext, pEnvironment));
}

inline void CThreadPoolWork::Close()
{
    if (m_pWork != NULL)
    {
        ::CloseThreadpoolWork(m_pWork);
        m_pWork = NULL;
    }
}

inline CThreadPoolWork& CThreadPoolWork::operator=(PTP_WORK pWork)
{
    m_pWork = pWork;
    return *this;
}

inline CThreadPoolWork::operator PTP_WORK() const
{
    return m_pWork;
}

inline void CThreadPoolWork::Submit()
{
    ATLASSERT(!IsEmpty());
    ::SubmitThreadpoolWork(m_pWork);
}

inline void CThreadPoolWork::Wait(BOOL bCancelPendingCallbacks/* = FALSE*/)
{
    ATLASSERT(!IsEmpty());
    ::WaitForThreadpoolWorkCallbacks(m_pWork, bCancelPendingCallbacks);
}

inline BOOL CThreadPoolWork::IsEmpty() const
{
    return (m_pWork == NULL);
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CThreadPoolWait class
//

inline CThreadPoolWait::CThreadPoolWait(PTP_WAIT pWait/* = NULL*/)
    : m_pWait(pWait)
{
}

inline PTP_WAIT CThreadPoolWait::Create(PTP_WAIT_CALLBACK pfnWaitCallback, LPVOID pContext/* = NULL*/, PTP_CALLBACK_ENVIRON pEnvironment/* = NULL*/)
{
    ATLASSERT(IsEmpty());
    ATLASSERT(pfnWaitCallback);

    return (m_pWait = ::CreateThreadpoolWait(pfnWaitCallback, pContext, pEnvironment));
}

inline void CThreadPoolWait::Close()
{
    if (m_pWait != NULL)
    {
        ::CloseThreadpoolWait(m_pWait);
        m_pWait = NULL;
    }
}

inline CThreadPoolWait& CThreadPoolWait::operator=(PTP_WAIT pWait)
{
    m_pWait = pWait;
    return *this;
}

inline CThreadPoolWait::operator PTP_WAIT() const
{
    return m_pWait;
}

inline void CThreadPoolWait::Wait(BOOL bCancelPendingCallbacks/* = FALSE*/)
{
    ATLASSERT(!IsEmpty());
    ::WaitForThreadpoolWaitCallbacks(m_pWait, bCancelPendingCallbacks);
}

inline void CThreadPoolWait::SetWait(HANDLE hObject, LPFILETIME pftTimeout/* = NULL*/)
{
    ATLASSERT(!IsEmpty());
    ::SetThreadpoolWait(m_pWait, hObject, pftTimeout);
}

inline BOOL CThreadPoolWait::IsEmpty() const
{
    return (m_pWait == NULL);
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CThreadPoolTimer class
//

inline CThreadPoolTimer::CThreadPoolTimer(PTP_TIMER pTimer/* = NULL*/)
    : m_pTimer(pTimer)
{
}

inline PTP_TIMER CThreadPoolTimer::Create(PTP_TIMER_CALLBACK pfnTimerCallback, LPVOID pContext/* = NULL*/, PTP_CALLBACK_ENVIRON pEnvironment/* = NULL*/)
{
    ATLASSERT(IsEmpty());
    ATLASSERT(pfnTimerCallback);

    return (m_pTimer = ::CreateThreadpoolTimer(pfnTimerCallback, pContext, pEnvironment));
}

inline void CThreadPoolTimer::Close()
{
    if (m_pTimer != NULL)
    {
        ::CloseThreadpoolTimer(m_pTimer);
        m_pTimer = NULL;
    }
}

inline CThreadPoolTimer& CThreadPoolTimer::operator=(PTP_TIMER pTimer)
{
    m_pTimer = pTimer;
    return *this;
}

inline CThreadPoolTimer::operator PTP_TIMER() const
{
    return m_pTimer;
}

inline void CThreadPoolTimer::Wait(BOOL bCancelPendingCallbacks/* = FALSE*/)
{
    ATLASSERT(!IsEmpty());
    ::WaitForThreadpoolTimerCallbacks(m_pTimer, bCancelPendingCallbacks);
}

inline void CThreadPoolTimer::SetTimer(DWORD dwPeriod, LPFILETIME pftDueTime/* = NULL*/, DWORD dwWindowLength/* = 0*/)
{
    ATLASSERT(!IsEmpty());
    ::SetThreadpoolTimer(m_pTimer, pftDueTime, dwPeriod, dwWindowLength);
}

inline BOOL CThreadPoolTimer::IsEmpty() const
{
    return (m_pTimer == NULL);
}

inline BOOL CThreadPoolTimer::IsTimerSet() const
{
    ATLASSERT(!IsEmpty());
    return ::IsThreadpoolTimerSet(m_pTimer);
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CThreadPoolCleanupGroupTraits class
//

inline BOOL WINAPI CThreadPoolCleanupGroupTraits::Destroy(HANDLET_PTR hHandle)
{
    ::CloseThreadpoolCleanupGroup(hHandle);
    return TRUE;
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CThreadPoolCleanupGroup class
//

inline CThreadPoolCleanupGroup::CThreadPoolCleanupGroup()
{
}

inline PTP_CLEANUP_GROUP CThreadPoolCleanupGroup::Create()
{
    ATLASSERT(IsEmpty());
    return (m_hHandle = ::CreateThreadpoolCleanupGroup());
}

inline void CThreadPoolCleanupGroup::CloseMembers(LPVOID pCleanupContext/* = NULL*/, BOOL bCancelPendingCallbacks/* = FALSE*/)
{
    ATLASSERT(!IsEmpty());
    ::CloseThreadpoolCleanupGroupMembers(m_hHandle, bCancelPendingCallbacks, pCleanupContext);
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CTPCallbackInstance class
//

inline CTPCallbackInstance::CTPCallbackInstance(PTP_CALLBACK_INSTANCE pInstance/* = NULL*/)
    : m_pInstance(pInstance)
{
}

inline CTPCallbackInstance& CTPCallbackInstance::operator=(PTP_CALLBACK_INSTANCE pInstance)
{
    m_pInstance = pInstance;
    return *this;
}

inline CTPCallbackInstance::operator PTP_CALLBACK_INSTANCE()
{
    return m_pInstance;
}

inline BOOL CTPCallbackInstance::CallbackMayRunLong()
{
    ATLASSERT(!IsEmpty());
    return ::CallbackMayRunLong(m_pInstance);
}

inline void CTPCallbackInstance::DisassociateCurrentThreadFromCallback()
{
    ATLASSERT(!IsEmpty());
    ::DisassociateCurrentThreadFromCallback(m_pInstance);
}

inline void CTPCallbackInstance::FreeLibraryWhenCallbackReturns(HMODULE hModule)
{
    ATLASSERT(hModule);
    ATLASSERT(!IsEmpty());

    ::FreeLibraryWhenCallbackReturns(m_pInstance, hModule);
}

inline void CTPCallbackInstance::LeaveCriticalSectionWhenCallbackReturns(LPCRITICAL_SECTION pcs)
{
    ATLASSERT(pcs);
    ATLASSERT(!IsEmpty());

    ::LeaveCriticalSectionWhenCallbackReturns(m_pInstance, pcs);
}

inline void CTPCallbackInstance::SetEventWhenCallbackReturns(HANDLE hEvent)
{
    ATLASSERT(hEvent);
    ATLASSERT(!IsEmpty());

    ::SetEventWhenCallbackReturns(m_pInstance, hEvent);
}

inline void CTPCallbackInstance::ReleaseMutexWhenCallbackReturns(HANDLE hMutex)
{
    ATLASSERT(hMutex);
    ATLASSERT(!IsEmpty());

    ::ReleaseMutexWhenCallbackReturns(m_pInstance, hMutex);
}

inline void CTPCallbackInstance::ReleaseSemaphoreWhenCallbackReturns(HANDLE hSemaphore, DWORD dwIncSemaphoreCount)
{
    ATLASSERT(!IsEmpty());
    ATLASSERT(hSemaphore);

    ::ReleaseSemaphoreWhenCallbackReturns(m_pInstance, hSemaphore, dwIncSemaphoreCount);
}

inline BOOL CTPCallbackInstance::IsEmpty() const
{
    return (m_pInstance == NULL);
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CTPCallbackEnvironment class
//

inline CTPCallbackEnvironment::CTPCallbackEnvironment()
{
    ::InitializeThreadpoolEnvironment(this);
}

inline void CTPCallbackEnvironment::Destroy()
{
    ::DestroyThreadpoolEnvironment(this);
}

inline CTPCallbackEnvironment::operator PTP_CALLBACK_ENVIRON()
{
    return this;
}

inline void CTPCallbackEnvironment::SetCallbackRunsLong()
{
    ::SetThreadpoolCallbackRunsLong(this);
}

inline void CTPCallbackEnvironment::SetCallbackLibrary(LPVOID pModule)
{
    ::SetThreadpoolCallbackLibrary(this, pModule);
}

#if (_WIN32_WINNT >= 0x0601)
inline void CTPCallbackEnvironment::SetCallbackPersistent()
{
    ::SetThreadpoolCallbackPersistent(this);
}

inline void CTPCallbackEnvironment::SetCallbackPriority(TP_CALLBACK_PRIORITY nPriority)
{
    ::SetThreadpoolCallbackPriority(this, nPriority);
}
#endif  // (_WIN32_WINNT >= 0x0601)

inline void CTPCallbackEnvironment::SetCallbackPool(PTP_POOL pPool/* = NULL*/)
{
    ::SetThreadpoolCallbackPool(this, pPool);
}

inline void CTPCallbackEnvironment::SetCallbackCleanupGroup(PTP_CLEANUP_GROUP pCleanupGroup, PTP_CLEANUP_GROUP_CANCEL_CALLBACK pfnCancelCallback/* = NULL*/)
{
    ::SetThreadpoolCallbackCleanupGroup(this, pCleanupGroup, pfnCancelCallback);
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CTPIoCallbackImpl class
//

template <typename T>
inline void CTPIoCallbackImpl<T>::TPIo_OnCallback(CTPCallbackInstance /*instance*/, PTP_IO /*pIo*/, ULONG_PTR /*dwNumberOfBytesTransferred*/, LPVOID /*pOverlapped*/, ULONG /*ulIoResult*/)
{
}

template <typename T>
inline PTP_IO CTPIoCallbackImpl<T>::TPIo_CreateIo(CThreadPoolIo& TPIo, HANDLE hHandle, PTP_CALLBACK_ENVIRON pEnvironment/* = NULL*/)
{
    ATLASSERT(hHandle);
    ATLASSERT(TPIo.IsEmpty());

    return TPIo.Create(hHandle, T::TPIo_Callback, static_cast<T*>(this), pEnvironment);
}

template <typename T>
inline void CALLBACK CTPIoCallbackImpl<T>::TPIo_Callback(PTP_CALLBACK_INSTANCE pInstance, LPVOID pContext, LPVOID pOverlapped, ULONG ulIoResult, ULONG_PTR dwNumberOfBytesTransferred, PTP_IO pIo)
{
    ATLASSERT(pContext);
    reinterpret_cast<T*>(pContext)->TPIo_OnCallback(pInstance, pIo, dwNumberOfBytesTransferred, pOverlapped, ulIoResult);
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CTPWorkCallbackImpl class
//

template <typename T>
inline void CTPWorkCallbackImpl<T>::TPWork_OnCallback(CTPCallbackInstance /*instance*/, PTP_WORK /*pWork*/)
{
}

template <typename T>
inline PTP_WORK CTPWorkCallbackImpl<T>::TPWork_CreateWork(PTP_CALLBACK_ENVIRON pEnvironment/* = NULL*/)
{
    return ::CreateThreadpoolWork(T::TPWork_Callback, static_cast<T*>(this), pEnvironment);
}

template <typename T>
inline void CALLBACK CTPWorkCallbackImpl<T>::TPWork_Callback(PTP_CALLBACK_INSTANCE pInstance, LPVOID pContext, PTP_WORK pWork)
{
    ATLASSERT(pContext);
    reinterpret_cast<T*>(pContext)->TPWork_OnCallback(pInstance, pWork);
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CTPWaitCallbackImpl class
//

template <typename T>
inline void CTPWaitCallbackImpl<T>::TPWait_OnCallback(CTPCallbackInstance /*instance*/, PTP_WAIT /*pWait*/, TP_WAIT_RESULT /*dwWaitResult*/)
{
}

template <typename T>
inline PTP_WAIT CTPWaitCallbackImpl<T>::TPWait_CreateWait(PTP_CALLBACK_ENVIRON pEnvironment/* = NULL*/)
{
    return ::CreateThreadpoolWait(T::TPWait_Callback, static_cast<T*>(this), pEnvironment);
}

template <typename T>
inline void CALLBACK CTPWaitCallbackImpl<T>::TPWait_Callback(PTP_CALLBACK_INSTANCE pInstance, LPVOID pContext, PTP_WAIT pWait, TP_WAIT_RESULT dwWaitResult)
{
    ATLASSERT(pContext);
    reinterpret_cast<T*>(pContext)->TPWait_OnCallback(pInstance, pWait, dwWaitResult);
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CTPTimerCallbackImpl class
//

template <typename T>
inline void CTPTimerCallbackImpl<T>::TPTimer_OnCallback(CTPCallbackInstance /*instance*/, PTP_TIMER /*pTimer*/)
{
}

template <typename T>
inline PTP_TIMER CTPTimerCallbackImpl<T>::TPTimer_CreateTimer(PTP_CALLBACK_ENVIRON pEnvironment/* = NULL*/)
{
    return ::CreateThreadpoolTimer(T::TPTimer_Callback, static_cast<T*>(this), pEnvironment);
}

template <typename T>
inline void CALLBACK CTPTimerCallbackImpl<T>::TPTimer_Callback(PTP_CALLBACK_INSTANCE pInstance, LPVOID pContext, PTP_TIMER pTimer)
{
    ATLASSERT(pContext);
    reinterpret_cast<T*>(pContext)->TPTimer_OnCallback(pInstance, pTimer);
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CTPSimpleCallbackImpl class
//

template <typename T>
inline void CTPSimpleCallbackImpl<T>::TPSimple_OnCallback(CTPCallbackInstance /*instance*/)
{
}

template <typename T>
inline BOOL CTPSimpleCallbackImpl<T>::TPSimple_Submit(PTP_CALLBACK_ENVIRON pEnvironment/* = NULL*/)
{
    return ::TrySubmitThreadpoolCallback(T::TPSimple_Callback, static_cast<T*>(this), pEnvironment);
}

template <typename T>
inline void CALLBACK CTPSimpleCallbackImpl<T>::TPSimple_Callback(PTP_CALLBACK_INSTANCE pInstance, LPVOID pContext)
{
    ATLASSERT(pContext);
    reinterpret_cast<T*>(pContext)->TPSimple_OnCallback(pInstance);
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CThreadPoolCleanupGroupImpl class
//

template <typename T>
inline void CThreadPoolCleanupGroupImpl<T>::TPCleanupGroup_OnCallback(LPVOID /*pObjectContext*/)
{
}

template <typename T>
inline PTP_CLEANUP_GROUP CThreadPoolCleanupGroupImpl<T>::TPCleanupGroup_Create()
{
    ATLASSERT(TPCleanupGroup_IsEmpty());
    return m_cleanupGroup.Create();
}

template <typename T>
inline void CThreadPoolCleanupGroupImpl<T>::TPCleanupGroup_Close()
{
    m_cleanupGroup.Destroy();
}

template <typename T>
inline void CThreadPoolCleanupGroupImpl<T>::TPCleanupGroup_Register(PTP_CALLBACK_ENVIRON pEnvironment/* = NULL*/)
{
    ATLASSERT(!TPCleanupGroup_IsEmpty());
    ::SetThreadpoolCallbackCleanupGroup(pEnvironment, m_cleanupGroup.m_hHandle, T::TPCleanupGroup_Callback);
}

template <typename T>
inline void CThreadPoolCleanupGroupImpl<T>::TPCleanupGroup_CloseMembers(BOOL bCancelPendingCallbacks/* = FALSE*/)
{
    ATLASSERT(!TPCleanupGroup_IsEmpty());
    m_cleanupGroup.CloseMembers(static_cast<T*>(this), bCancelPendingCallbacks);
}

template <typename T>
inline BOOL CThreadPoolCleanupGroupImpl<T>::TPCleanupGroup_IsEmpty() const
{
    return m_cleanupGroup.IsEmpty();
}

template <typename T>
inline void CALLBACK CThreadPoolCleanupGroupImpl<T>::TPCleanupGroup_Callback(LPVOID pObjectContext, LPVOID pCleanupContext)
{
    ATLASSERT(pCleanupContext);
    reinterpret_cast<T*>(pCleanupContext)->TPCleanupGroup_OnCallback(pObjectContext);
}
#endif  // (_WIN32_WINNT >= 0x0600)

}  // namespace atlutil

#endif  // __ATLSYNCUTIL_INL__