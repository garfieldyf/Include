///////////////////////////////////////////////////////////////////////////////
// timerutil.inl
//
// Copyright(c) 2012, Garfield. All rights reserved.
// Author  : Garfield
// Version : 1.0
// Finish Time : 2010/6/20

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __TIMERUTIL_INL__
#define __TIMERUTIL_INL__

namespace atlutil {

///////////////////////////////////////////////////////////////////////////////
// Implementation of the CTimerTask class
//

inline CTimerTask::CTimerTask()
    : m_nDelay(0), m_nIDEvent(0), m_pHandler(NULL), m_hEvent(::CreateEvent(NULL, TRUE, TRUE, NULL))
{
    ATLASSERT(m_hEvent);
}

inline CTimerTask::~CTimerTask()
{
    ATLVERIFY(::CloseHandle(m_hEvent));
}

inline BOOL CTimerTask::Start(ITimerHandler* pHandler, UINT_PTR nIDEvent, UINT nDelay, BOOL bFixedDelay/* = TRUE*/, DWORD dwCreationFlags/* = 0*/)
{
    ATLASSERT(pHandler);
    ATLASSERT(m_hEvent);
    ATLASSERT(IsCancelled());
    ATLASSERT(m_hThread == NULL);

    m_nDelay   = nDelay;
    m_pHandler = pHandler;
    m_nIDEvent = nIDEvent;
    ATLVERIFY(::ResetEvent(m_hEvent));

    return ((m_hThread = _CreateThread(NULL, 0, (bFixedDelay ? FixedTimerProc : DelayTimerProc), this, dwCreationFlags, NULL)) != NULL);
}

inline BOOL CTimerTask::Stop(DWORD dwTimeout/* = INFINITE*/)
{
    BOOL bSuccessful = TRUE;
    if (m_hThread != NULL)
    {
        ATLVERIFY(Cancel());
        bSuccessful = (::WaitForSingleObject(m_hThread, dwTimeout) == WAIT_OBJECT_0);

        ATLVERIFY(::CloseHandle(m_hThread));
        m_hThread = NULL;
        ATLTRACE(_T("CTimerTask::Stop() - stop the timer task [ Event ID = %u ]\n"), m_nIDEvent);
    }

    return bSuccessful;
}

inline BOOL CTimerTask::Cancel() const
{
    ATLASSERT(m_hEvent);
    return ::SetEvent(m_hEvent);
}

inline void CTimerTask::Set(UINT nDelay)
{
    m_nDelay = nDelay;
}

inline BOOL CTimerTask::IsCancelled() const
{
    ATLASSERT(m_hEvent);
    return (::WaitForSingleObject(m_hEvent, 0) == WAIT_OBJECT_0);
}

inline UINT_PTR CTimerTask::GetEventId() const
{
    return m_nIDEvent;
}

inline DWORD WINAPI CTimerTask::FixedTimerProc(LPVOID pParameter)
{
    CTimerTask* pThis = reinterpret_cast<CTimerTask*>(pParameter);
    ATLASSERT(pThis);
    ATLASSERT(pThis->m_hEvent);
    ATLASSERT(pThis->m_pHandler);

    while (::WaitForSingleObject(pThis->m_hEvent, pThis->m_nDelay) == WAIT_TIMEOUT)
        pThis->m_pHandler->OnTimer(pThis->m_nIDEvent);

    ATLTRACE(_T("CTimerTask::FixedTimerProc() - exiting the timer thread [ Event ID = %u ]\n"), pThis->m_nIDEvent);
    return 0;
}

inline DWORD WINAPI CTimerTask::DelayTimerProc(LPVOID pParameter)
{
    CTimerTask* pThis = reinterpret_cast<CTimerTask*>(pParameter);
    ATLASSERT(pThis);
    ATLASSERT(pThis->m_hEvent);
    ATLASSERT(pThis->m_pHandler);

    volatile UINT nDelay = pThis->m_nDelay;
    while (::WaitForSingleObject(pThis->m_hEvent, nDelay) == WAIT_TIMEOUT)
    {
        nDelay = ::GetTickCount();
        pThis->m_pHandler->OnTimer(pThis->m_nIDEvent);

        nDelay = ::GetTickCount() - nDelay;
        nDelay = (pThis->m_nDelay > nDelay ? pThis->m_nDelay - nDelay : 0);
    }

    ATLTRACE(_T("CTimerTask::DelayTimerProc() - exiting the timer thread [ Event ID = %u ]\n"), pThis->m_nIDEvent);
    return 0;
}

}  // namespace atlutil

#endif  // __TIMERUTIL_INL__