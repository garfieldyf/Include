///////////////////////////////////////////////////////////////////////////////
// iocputil.h : ͷ�ļ�
//
// Copyright(c) 2012, Garfield. All rights reserved.
// Author  : Garfield
// Version : 2.0
// Finish Time : 2007/12/21

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __IOCPUTIL_H__
#define __IOCPUTIL_H__

#ifndef _INC_PROCESS
#include <process.h>
#endif

#ifndef __ATLCOLL_H__
#include <atlcoll.h>
#endif

///////////////////////////////////////////////////////////////////////////////
// Classes in this file:
//
// CIoCompletionEvent
// CIoCompletionPort

namespace stdutil {

///////////////////////////////////////////////////////////////////////////////
// CIoCompletionEvent ��Ķ���
//

class ATL_NO_VTABLE CIoCompletionEvent : public OVERLAPPED
{
// ����/����
public:
    explicit CIoCompletionEvent(HANDLE hEvent = NULL, DWORD dwOffset = 0, DWORD dwOffsetHigh = 0);
    virtual ~CIoCompletionEvent();

// ����
public:
    LPOVERLAPPED GetOverlapped();
    const OVERLAPPED* GetOverlapped() const;
    void SetOverlapped(HANDLE hEvent = NULL, DWORD dwOffset = 0, DWORD dwOffsetHigh = 0);

// ��д
public:
    virtual void OnIoCompleted(DWORD dwNumberOfBytesTransferred, ULONG_PTR dwCompletionKey, DWORD dwErrorCode) = 0;
};


///////////////////////////////////////////////////////////////////////////////
// CIoCompletionPort ��Ķ���
//

class CIoCompletionPort
{
// ����/����
public:
    CIoCompletionPort();
    ~CIoCompletionPort();

// ����
public:
    BOOL Close();
    BOOL Create(DWORD dwNumberOfConcurrentThreads = 0);
    BOOL Associate(HANDLE hHandle, ULONG_PTR dwCompletionKey = 0);

    BOOL CreateThreadPool(DWORD dwThreadCount = 0);
    void CloseThreadPool();

    BOOL PostQueuedCompletionStatus(DWORD dwNumberOfBytesTransferred, ULONG_PTR dwCompletionKey = 0, LPOVERLAPPED pOverlapped = NULL);
    BOOL GetQueuedCompletionStatus(LPDWORD pNumberOfBytesTransferred, PULONG_PTR pCompletionKey, LPOVERLAPPED* pOverlapped, DWORD dwMilliseconds = INFINITE);

// ʵ��
protected:
    enum { IO_COMPLETION_KEY_EXIT = 0xEFEFEFE };
    typedef ATL::CAtlArray<HANDLE> CHandleArray;

    void ProcessSurplusIoPacket();
    static UINT WINAPI ThreadProc(LPVOID pvParameter);

// ��Ա
protected:
    HANDLE m_hCompletionPort;
    CHandleArray m_arrThreads;
};

}  // namespace stdutil

#include "iocputil.inl"

#endif  // __IOCPUTIL_H__