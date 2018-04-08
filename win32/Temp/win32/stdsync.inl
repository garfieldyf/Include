///////////////////////////////////////////////////////////////////////////////
// stdsync.inl
//
// Copyright(c) 2010, Garfield. All rights reserved.
// Author  : Garfield
// Version : 2.0
// Finish Time : 2010/5/22

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __STDSYNC_INL__
#define __STDSYNC_INL__

namespace stdutil {

///////////////////////////////////////////////////////////////////////////////
// Implementation of the CAsyncTaskBase class
//

inline CAsyncTaskBase::CAsyncTaskBase()
    : m_lCancel(FALSE), m_hThread(NULL)
{
}

inline CAsyncTaskBase::~CAsyncTaskBase()
{
    ATLASSERT(!m_lCancel);
    ATLASSERT(m_hThread == NULL);
}

inline void CAsyncTaskBase::CancelTask()
{
    ::InterlockedExchange(&m_lCancel, TRUE);
}

inline DWORD CAsyncTaskBase::PauseTask() const
{
    ATLASSERT(m_hThread);
    return ::SuspendThread(m_hThread);
}

inline DWORD CAsyncTaskBase::ResumeTask() const
{
    ATLASSERT(m_hThread);
    return ::ResumeThread(m_hThread);
}

inline BOOL CAsyncTaskBase::IsTaskCancelled() const
{
    return ::InterlockedExchange((LONG*)&m_lCancel, m_lCancel);
}

inline void CAsyncTaskBase::FinishTask(DWORD dwTimeout/* = 5000*/)
{
    ATLASSERT(m_hThread);

    _COND_VERIFY(::WaitForSingleObject(m_hThread, dwTimeout), WAIT_OBJECT_0);
    ATLVERIFY(::CloseHandle(m_hThread));

    m_hThread = NULL;
    m_lCancel = FALSE;
}


#ifdef __ATLWIN_H__
///////////////////////////////////////////////////////////////////////////////
// Implementation of the CAsyncTaskImpl class
//

template <typename T>
inline CAsyncTaskImpl<T>::CAsyncTaskImpl()
{
}

template <typename T>
inline HRESULT CAsyncTaskImpl<T>::OnPrepareTask()
{
    // Return S_OK, if successful
    // Return COM-defined error code, if failed
    return S_OK;
}

template <typename T>
inline HRESULT CAsyncTaskImpl<T>::OnExecuteTask()
{
    // Return S_OK, if successful
    // Return E_ABORT, if cancelled
    // Return COM-defined error code, if failed
    return S_OK;
}

template <typename T>
inline void CAsyncTaskImpl<T>::OnFinishTask(HRESULT /*hrResult*/)
{
}

template <typename T>
inline BOOL CAsyncTaskImpl<T>::ExecuteTask(DWORD dwCreationFlags/* = 0*/, SIZE_T dwStackSize/* = 0*/, LPSECURITY_ATTRIBUTES psa/* = NULL*/)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);
    ATLASSERT(!m_lCancel);
    ATLASSERT(m_hThread == NULL);

    HRESULT hr = pThis->OnPrepareTask();
    if (SUCCEEDED(hr))
        m_hThread = _CreateThread(psa, dwStackSize, T::AsyncTaskProc, pThis, dwCreationFlags, NULL);
    else
        pThis->OnFinishTask(hr);

    return (m_hThread != NULL);
}

template <typename T>
inline DWORD WINAPI CAsyncTaskImpl<T>::AsyncTaskProc(LPVOID pParameter)
{
    T* pThis = reinterpret_cast<T*>(pParameter);
    ATLASSERT(pThis);
    ATLASSERT(::IsWindow(pThis->m_hWnd));

    ATLVERIFY(::PostMessage(pThis->m_hWnd, WM_ASYNCTAST_FINISH, 0, pThis->OnExecuteTask()));
    ATLTRACE(_T("Thread exit - CAsyncTaskImpl<T>::AsyncTaskProc()\n"));

    return 0;
}

template <typename T>
inline LRESULT CAsyncTaskImpl<T>::OnAsyncTaskFinish(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);

    pThis->OnFinishTask(lParam);
    pThis->FinishTask();

    return 0L;
}
#endif  // __ATLWIN_H__


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CThreadBase class
//

inline CThreadBase::CThreadBase()
    : m_hThread(NULL)
{
}

inline CThreadBase::~CThreadBase()
{
    ATLASSERT(m_hThread == NULL);
}

inline CThreadBase::operator HANDLE() const
{
    return m_hThread;
}

inline DWORD CThreadBase::Resume() const
{
    ATLASSERT(m_hThread);
    return ::ResumeThread(m_hThread);
}

inline DWORD CThreadBase::Suspend() const
{
    ATLASSERT(m_hThread);
    return ::SuspendThread(m_hThread);
}

inline int CThreadBase::GetPriority() const
{
    ATLASSERT(m_hThread);
    return ::GetThreadPriority(m_hThread);
}

inline BOOL CThreadBase::SetPriority(int nPriority)
{
    ATLASSERT(m_hThread);
    return ::SetThreadPriority(m_hThread, nPriority);
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CTimerTask class
//

inline CTimerTask::CTimerTask()
    : m_dwDelay(0), m_dwEventId(0), m_pEvent(NULL), m_hEvent(::CreateEvent(NULL, FALSE, FALSE, NULL))
{
    ATLASSERT(m_hEvent);
}

inline CTimerTask::~CTimerTask()
{
    ATLVERIFY(::CloseHandle(m_hEvent));
}

inline BOOL CTimerTask::Start(ITimerEvent* pEvent, DWORD dwEventId, DWORD dwDelay, BOOL bFixedDelay/* = TRUE*/, DWORD dwCreationFlags/* = 0*/, SIZE_T dwStackSize/* = 0*/, LPSECURITY_ATTRIBUTES psa/* = NULL*/)
{
    ATLASSERT(pEvent);
    ATLASSERT(m_hEvent);
    ATLASSERT(m_hThread == NULL);

    m_pEvent  = pEvent;
    m_dwDelay = dwDelay;
    m_dwEventId = dwEventId;

    return ((m_hThread = _CreateThread(psa, dwStackSize, (bFixedDelay ? FixedTimerProc : DelayTimerProc), this, dwCreationFlags, NULL)) != NULL);
}

inline BOOL CTimerTask::Cancel() const
{
    ATLASSERT(m_hEvent);
    return ::SetEvent(m_hEvent);
}

inline BOOL CTimerTask::Stop(DWORD dwTimeout/* = INFINITE*/)
{
    BOOL bSucceessful = TRUE;
    if (m_hThread != NULL)
    {
        ATLVERIFY(::SetEvent(m_hEvent));
        bSucceessful = (::WaitForSingleObject(m_hThread, dwTimeout) == WAIT_OBJECT_0);

        ATLVERIFY(::CloseHandle(m_hThread));
        m_hThread = NULL;
        ATLTRACE(_T("CTimerTask::Stop() - stop the timer task [ Event ID = %u ]\n"), m_dwEventId);
    }

    return bSucceessful;
}

inline BOOL CTimerTask::IsCancelled() const
{
    ATLASSERT(m_hEvent);
    return (::WaitForSingleObject(m_hEvent, 0) == WAIT_TIMEOUT);
}

inline DWORD CTimerTask::GetEventId() const
{
    return m_dwEventId;
}

inline DWORD WINAPI CTimerTask::FixedTimerProc(LPVOID pParameter)
{
    CTimerTask* pThis = reinterpret_cast<CTimerTask*>(pParameter);
    ATLASSERT(pThis);
    ATLASSERT(pThis->m_pEvent);
    ATLASSERT(pThis->m_hEvent);

    while (::WaitForSingleObject(pThis->m_hEvent, pThis->m_dwDelay) == WAIT_TIMEOUT)
        pThis->m_pEvent->OnTimerEvent(pThis->m_dwEventId);

    ATLTRACE(_T("CTimerTask::FixedTimerProc() - exiting the timer thread [ Event ID = %u ]\n"), pThis->m_dwEventId);
    return 0;
}

inline DWORD WINAPI CTimerTask::DelayTimerProc(LPVOID pParameter)
{
    CTimerTask* pThis = reinterpret_cast<CTimerTask*>(pParameter);
    ATLASSERT(pThis);
    ATLASSERT(pThis->m_pEvent);
    ATLASSERT(pThis->m_hEvent);

    DWORD dwDelay = pThis->m_dwDelay;
    while (::WaitForSingleObject(pThis->m_hEvent, dwDelay) == WAIT_TIMEOUT)
    {
        dwDelay = ::GetTickCount();
        pThis->m_pEvent->OnTimerEvent(pThis->m_dwEventId);

        dwDelay = ::GetTickCount() - dwDelay;
        dwDelay = (pThis->m_dwDelay > dwDelay ? pThis->m_dwDelay - dwDelay : 0);
    }

    ATLTRACE(_T("CTimerTask::DelayTimerProc() - exiting the timer thread [ Event ID = %u ]\n"), pThis->m_dwEventId);
    return 0;
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CMessageThread class
//

inline CMessageThread::CMessageThread()
    : m_lCancel(TRUE), m_pEvent(NULL), m_dwThreadId(0)
{
}

inline BOOL CMessageThread::Create(IMessageEvent* pEvent, DWORD dwCreationFlags/* = 0*/, SIZE_T dwStackSize/* = 0*/, LPSECURITY_ATTRIBUTES psa/* = NULL*/)
{
    ATLASSERT(pEvent);
    ATLASSERT(m_hThread == NULL);

    m_lCancel = FALSE;
    m_pEvent  = pEvent;

    return ((m_hThread = _CreateThread(psa, dwStackSize, ThreadProc, this, dwCreationFlags, &m_dwThreadId)) != NULL);
}

inline BOOL CMessageThread::Quit(DWORD dwTimeout/* = INFINITE*/)
{
    BOOL bSucceessful = TRUE;
    if (m_hThread != NULL)
    {
        ATLASSERT(::GetCurrentThreadId() != m_dwThreadId);
        ::InterlockedExchange(&m_lCancel, TRUE);
        ATLVERIFY(::PostThreadMessage(m_dwThreadId, WM_QUIT, 0, 0L));

        bSucceessful = (::WaitForSingleObject(m_hThread, dwTimeout) == WAIT_OBJECT_0);
        ATLVERIFY(::CloseHandle(m_hThread));

        m_hThread = NULL, m_dwThreadId = 0;
        ATLTRACE(_T("CMessageThread::Quit() - quitting the message loop.\n"));
    }

    return bSucceessful;
}

inline void CMessageThread::Cancel()
{
    ::InterlockedExchange(&m_lCancel, TRUE);
}

inline BOOL CMessageThread::PostMessage(UINT uMsg, WPARAM wParam/* = 0*/, LPARAM lParam/* = 0L*/) const
{
    ATLASSERT(m_hThread);
    ATLASSERT(uMsg != WM_QUIT);

    return ::PostThreadMessage(m_dwThreadId, uMsg, wParam, lParam);
}

inline BOOL CMessageThread::IsCancelled() const
{
    return ::InterlockedExchange((LONG*)&m_lCancel, m_lCancel);
}

inline DWORD CMessageThread::GetThreadId() const
{
    return m_dwThreadId;
}

inline DWORD WINAPI CMessageThread::ThreadProc(LPVOID pParameter)
{
    CMessageThread* pThis = reinterpret_cast<CMessageThread*>(pParameter);
    ATLASSERT(pThis);
    ATLASSERT(pThis->m_pEvent);

    MSG msg;
    while (::GetMessage(&msg, NULL, 0, 0) > 0)
    {
        if (!pThis->IsCancelled())
            pThis->m_pEvent->OnProcessMessage(msg);
    }

    ATLTRACE(_T("CMessageThread::ThreadProc() - exiting the message thread.\n"));
    return 0;
}

}  // namespace stdutil

#endif  // __STDSYNC_INL__