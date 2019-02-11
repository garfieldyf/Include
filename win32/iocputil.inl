///////////////////////////////////////////////////////////////////////////////
// iocputil.inl
//
// Copyright(c) 2019, Garfield. All rights reserved.
// Author  : Garfield
// Version : 2.0
// Creation Date : 2007/12/21

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __IOCPUTIL_INL__
#define __IOCPUTIL_INL__

#ifndef __IOCPUTIL_H__
    #error iocputil.inl requires iocputil.h to be included first
#endif

namespace stdutil {

///////////////////////////////////////////////////////////////////////////////
// Implementation of the CThreadPool class
//

inline CThreadPool::CThreadPool()
{
}

inline CThreadPool::~CThreadPool()
{
    ATLASSERT(m_arrThreads.IsEmpty());
}

inline BOOL CThreadPool::Create(HANDLE hCompletionPort, DWORD dwThreadCount/* = 0*/)
{
    ATLASSERT(hCompletionPort);
    ATLASSERT(m_arrThreads.IsEmpty());

    if (dwThreadCount == 0)
    {
        SYSTEM_INFO sysInfo;
        ::GetSystemInfo(&sysInfo);
        dwThreadCount = sysInfo.dwNumberOfProcessors * 2;
    }

    BOOL bSuccessful = m_arrThreads.SetCount(dwThreadCount);
    if (bSuccessful)
    {
        for (DWORD i = 0; i < dwThreadCount; ++i)
        {
            m_arrThreads[i] = _CreateThread(NULL, 0, ThreadProc, hCompletionPort, 0, NULL);
            ATLASSERT(m_arrThreads[i]);
        }
    }

    return bSuccessful;
}

inline void CThreadPool::Close(HANDLE hCompletionPort, DWORD dwTimeout/* = INFINITE*/)
{
    ATLASSERT(m_arrThreads.GetCount() != 0);

    // Post quit message
    for (size_t i = 0; i < m_arrThreads.GetCount(); ++i)
        ATLVERIFY(::PostQueuedCompletionStatus(hCompletionPort, 0, NULL, NULL));

    // Waiting for all threads are quit
    ::WaitForMultipleObjects(m_arrThreads.GetCount(), m_arrThreads.GetData(), TRUE, dwTimeout);

    // Close all threads handle
    for (size_t i = 0; i < m_arrThreads.GetCount(); ++i)
        ATLVERIFY(::CloseHandle(m_arrThreads[i]));

    // Clear all threads
    m_arrThreads.RemoveAll();
}

inline DWORD CThreadPool::GetThreadCount() const
{
    return m_arrThreads.GetCount();
}

inline HANDLE CThreadPool::GetThread(DWORD dwIndex) const
{
    ATLASSERT(dwIndex < m_arrThreads.GetCount());
    return m_arrThreads[dwIndex];
}

#if (_WIN32_WINNT >= 0x0600)
inline DWORD CThreadPool::GetThreadId(DWORD dwIndex) const
{
    ATLASSERT(dwIndex < m_arrThreads.GetCount());
    return ::GetThreadId(m_arrThreads[dwIndex]);
}
#endif  // (_WIN32_WINNT >= 0x0600)

inline DWORD WINAPI CThreadPool::ThreadProc(LPVOID pParameter)
{
    ATLASSERT(pParameter);

    BOOL bSuccessful = FALSE;
    LPOVERLAPPED pOverlapped   = NULL;
    IIOCompletionEvent* pEvent = NULL;
    DWORD dwNumberOfBytesTransferred = 0;

    for (; ;)
    {
        bSuccessful = ::GetQueuedCompletionStatus(pParameter, &dwNumberOfBytesTransferred, (PULONG_PTR)&pEvent, &pOverlapped, INFINITE);

        // Exit the loop, if received quit message
        if (bSuccessful && pOverlapped == NULL && pEvent == NULL)
            break;

        pEvent->OnIOComplete(dwNumberOfBytesTransferred, pOverlapped, ::GetLastError());
    }

    return 0;
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CIOCompletionPort class
//

inline CIOCompletionPort::CIOCompletionPort()
{
}

inline HANDLE CIOCompletionPort::Create(DWORD dwNumberOfConcurrentThreads/* = 0*/)
{
    ATLASSERT(IsEmpty());
    return (m_hHandle = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, dwNumberOfConcurrentThreads));
}

inline BOOL CIOCompletionPort::Associate(HANDLE hHandle, IIOCompletionEvent* pEvent)
{
    ATLASSERT(pEvent);
    ATLASSERT(hHandle);
    ATLASSERT(!IsEmpty());

    return (::CreateIoCompletionPort(hHandle, m_hHandle, (ULONG_PTR)pEvent, 0) == m_hHandle);
}

inline BOOL CIOCompletionPort::CreateThreadPool(DWORD dwThreadCount/* = 0*/)
{
    ATLASSERT(!IsEmpty());
    return m_threads.Create(m_hHandle, dwThreadCount);
}

inline void CIOCompletionPort::CloseThreadPool(DWORD dwTimeout/* = INFINITE*/)
{
    if (m_threads.GetThreadCount() != 0)
        m_threads.Close(m_hHandle, dwTimeout);
}

inline void CIOCompletionPort::ProcessSurplusPacket()
{
    ATLASSERT(!IsEmpty());

    LPOVERLAPPED pOverlapped   = NULL;
    IIOCompletionEvent* pEvent = NULL;
    DWORD dwNumberOfBytesTransferred = 0;

    for (; ;)
    {
        ::GetQueuedCompletionStatus(m_hHandle, &dwNumberOfBytesTransferred, (PULONG)&pEvent, &pOverlapped, 0);
        if (pOverlapped == NULL)
            break;

        ATLASSERT(pEvent);
        pEvent->OnIOComplete(dwNumberOfBytesTransferred, pOverlapped, ::GetLastError());
    }
}

inline BOOL CIOCompletionPort::PostQueuedStatus(DWORD dwNumberOfBytesTransferred, IIOCompletionEvent* pEvent, LPOVERLAPPED pOverlapped)
{
    ATLASSERT(!IsEmpty());
   return ::PostQueuedCompletionStatus(m_hHandle, dwNumberOfBytesTransferred, (ULONG_PTR)pEvent, pOverlapped);
}

inline BOOL CIOCompletionPort::GetQueuedStatus(LPDWORD pNumberOfBytesTransferred, IIOCompletionEvent** ppEvent, LPOVERLAPPED* ppOverlapped, DWORD dwMilliseconds/* = INFINITE*/)
{
    ATLASSERT(ppEvent);
    ATLASSERT(!IsEmpty());
    ATLASSERT(ppOverlapped);
    ATLASSERT(pNumberOfBytesTransferred);

    return ::GetQueuedCompletionStatus(m_hHandle, pNumberOfBytesTransferred, (PULONG)ppEvent, ppOverlapped, dwMilliseconds);
}

inline DWORD CIOCompletionPort::GetThreadCount() const
{
    return m_threads.GetThreadCount();
}

inline HANDLE CIOCompletionPort::GetThread(DWORD dwIndex) const
{
    return m_threads.GetThread(dwIndex);
}

#if (_WIN32_WINNT >= 0x0600)
inline DWORD CIOCompletionPort::GetThreadId(DWORD dwIndex) const
{
    return m_threads.GetThreadId(dwIndex);
}
#endif  // (_WIN32_WINNT >= 0x0600)

}  // namespace stdutil

#endif  // __IOCPUTIL_INL__