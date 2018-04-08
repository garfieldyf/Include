///////////////////////////////////////////////////////////////////////////////
// iocputil.inl : ʵ���ļ�
//
// Copyright(c) 2012, Garfield. All rights reserved.
// Author  : Garfield
// Version : 2.0
// Finish Time : 2007/12/21

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __IOCPUTIL_INL__
#define __IOCPUTIL_INL__

namespace stdutil {

///////////////////////////////////////////////////////////////////////////////
// CIoCompletionEvent ���ʵ��
//

inline CIoCompletionEvent::CIoCompletionEvent(HANDLE hEvent/* = NULL*/, DWORD dwOffset/* = 0*/, DWORD dwOffsetHigh/* = 0*/)
{
    SetOverlapped(hEvent, dwOffset, dwOffsetHigh);
}

inline CIoCompletionEvent::~CIoCompletionEvent()
{
}

inline LPOVERLAPPED CIoCompletionEvent::GetOverlapped()
{
    return this;
}

inline const OVERLAPPED* CIoCompletionEvent::GetOverlapped() const
{
    return this;
}

inline void CIoCompletionEvent::SetOverlapped(HANDLE hEvent/* = NULL*/, DWORD dwOffset/* = 0*/, DWORD dwOffsetHigh/* = 0*/)
{
    this->hEvent = hEvent;
    this->Offset = dwOffset;
    this->OffsetHigh = dwOffsetHigh;
    this->Internal   = this->InternalHigh = 0;
}


///////////////////////////////////////////////////////////////////////////////
// CIoCompletionPort ���ʵ��
//

inline CIoCompletionPort::CIoCompletionPort()
    : m_hCompletionPort(NULL)
{
}

inline CIoCompletionPort::~CIoCompletionPort()
{
    ATLVERIFY(Close());
}

inline BOOL CIoCompletionPort::Close()
{
    BOOL bSuccessful = TRUE;
    if (m_hCompletionPort != NULL)
    {
        bSuccessful = ::CloseHandle(m_hCompletionPort);
        if (bSuccessful)
            m_hCompletionPort = NULL;
    }

    return bSuccessful;
}

inline BOOL CIoCompletionPort::Create(DWORD dwNumberOfConcurrentThreads/* = 0*/)
{
    ATLASSERT(m_hCompletionPort == NULL);

    m_hCompletionPort = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, dwNumberOfConcurrentThreads);
    return (m_hCompletionPort != NULL);
}

inline BOOL CIoCompletionPort::Associate(HANDLE hHandle, ULONG_PTR dwCompletionKey/* = 0*/)
{
    ATLASSERT(hHandle);
    return (::CreateIoCompletionPort(hHandle, m_hCompletionPort, dwCompletionKey, 0) == m_hCompletionPort);
}

inline BOOL CIoCompletionPort::CreateThreadPool(DWORD dwThreadCount/* = 0*/)
{
    // ����Ĭ�ϵĹ����߳���Ŀ��dwNumberOfProcessors * 2��
    if (dwThreadCount == 0)
    {
        SYSTEM_INFO sysInfo;
        ::GetSystemInfo(&sysInfo);
        dwThreadCount = sysInfo.dwNumberOfProcessors * 2;
    }

    // ���������߳�
    UINT nThreadID = 0;
    for (DWORD i = 0; i < dwThreadCount; ++i)
    {
        if (HANDLE hThread = reinterpret_cast<HANDLE>(::_beginthreadex(NULL, 0, ThreadProc, m_hCompletionPort, 0, &nThreadID)))
            m_arrThreads.Add(hThread);
    }

    return (m_arrThreads.GetCount() == dwThreadCount);
}

inline void CIoCompletionPort::CloseThreadPool()
{
    // ���� IO_COMPLETION_KEY_EXIT ��Ϣ
    for (size_t i = 0; i < m_arrThreads.GetCount(); ++i)
        ATLVERIFY(PostQueuedCompletionStatus(0, IO_COMPLETION_KEY_EXIT));

    // �ȴ����е��߳̽���
    ::WaitForMultipleObjects(m_arrThreads.GetCount(), m_arrThreads.GetData(), TRUE, INFINITE);

    // �ر����е��߳̾��
    for (size_t i = 0; i < m_arrThreads.GetCount(); ++i)
        ATLVERIFY(::CloseHandle(m_arrThreads[i]));

    // ����߳̾��, ����ʣ�����ɰ�
    m_arrThreads.RemoveAll();
    ProcessSurplusIoPacket();
}

inline BOOL CIoCompletionPort::PostQueuedCompletionStatus(DWORD dwNumberOfBytesTransferred, ULONG_PTR dwCompletionKey/* = 0*/, LPOVERLAPPED pOverlapped/* = NULL*/)
{
    return ::PostQueuedCompletionStatus(m_hCompletionPort, dwNumberOfBytesTransferred, dwCompletionKey, pOverlapped);
}

inline BOOL CIoCompletionPort::GetQueuedCompletionStatus(LPDWORD pNumberOfBytesTransferred, PULONG_PTR pCompletionKey, LPOVERLAPPED* pOverlapped, DWORD dwMilliseconds/* = INFINITE*/)
{
    ATLASSERT(pOverlapped);
    ATLASSERT(pCompletionKey);
    ATLASSERT(pNumberOfBytesTransferred);

    return ::GetQueuedCompletionStatus(m_hCompletionPort, pNumberOfBytesTransferred, pCompletionKey, pOverlapped, dwMilliseconds);
}

inline void CIoCompletionPort::ProcessSurplusIoPacket()
{
    BOOL bSuccessful = FALSE;
    ULONG_PTR dwCompletionKey = 0;
    LPOVERLAPPED pOverlapped = NULL;
    DWORD dwNumberOfBytesTransferred = 0;

    for (; ;)
    {
        bSuccessful = GetQueuedCompletionStatus(&dwNumberOfBytesTransferred, &dwCompletionKey, &pOverlapped, 0);
        if (pOverlapped == NULL)
            break;

        static_cast<CIoCompletionEvent*>(pOverlapped)->OnIoCompleted(dwNumberOfBytesTransferred, dwCompletionKey, (bSuccessful ? 0 : ::GetLastError()));
    }
}

inline UINT WINAPI CIoCompletionPort::ThreadProc(LPVOID pvParameter)
{
    ATLASSERT(pvParameter);

    BOOL bSuccessful = FALSE;
    ULONG_PTR dwCompletionKey = 0;
    LPOVERLAPPED pOverlapped = NULL;
    DWORD dwNumberOfBytesTransferred = 0;

    for (; ;)
    {
        bSuccessful = ::GetQueuedCompletionStatus(pvParameter, &dwNumberOfBytesTransferred, &dwCompletionKey, &pOverlapped, INFINITE);

        // �յ��߳��˳�����ɰ�
        if (pOverlapped == NULL && bSuccessful && dwCompletionKey == IO_COMPLETION_KEY_EXIT)
            break;

        if (pOverlapped != NULL)
            static_cast<CIoCompletionEvent*>(pOverlapped)->OnIoCompleted(dwNumberOfBytesTransferred, dwCompletionKey, (bSuccessful ? 0 : ::GetLastError()));
    }

    return 0;
}

}  // namespace stdutil

#endif  // __IOCPUTIL_INL__