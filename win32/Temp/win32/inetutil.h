///////////////////////////////////////////////////////////////////////////////
// inetutil.h : ͷ�ļ�
//
// Copyright(c) 2012, Garfield. All rights reserved.
// Author  : Garfield
// Version : 1.0
// Finish Time : 2008/9/12

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __INETUTIL_H__
#define __INETUTIL_H__

#include <deque>
#include <atlstr.h>
#include <atlcoll.h>
#include <atlsync.h>
#include <atlfile.h>
#include <wininet.h>
#include <process.h>

///////////////////////////////////////////////////////////////////////////////
// Classes in this file:
//
// CHttpDownloader

namespace stdutil {

///////////////////////////////////////////////////////////////////////////////
// CRangeQueue ��Ķ���
//

class CRangeQueue
{
// ����
public:
    CRangeQueue();

// ����
public:
    void RemoveAll();

    BOOL Pop(ULARGE_INTEGER& liRange);
    void Push(const ULARGE_INTEGER& liRange);

#ifdef _DEBUG
    void Dump() const;
#endif

// ����
public:
    DWORD GetCount() const;

// ��Ա
private:
    ATL::CCriticalSection m_cs;
    std::deque<ULARGE_INTEGER> m_deqRanges;
};


///////////////////////////////////////////////////////////////////////////////
// CHttpDownloader ��Ķ���
//

class CHttpDownloader
{
// ����/����
public:
    CHttpDownloader();
    ~CHttpDownloader();

// ����
public:
    void Clear();
    void Cancel();

    BOOL OpenUrl(HINTERNET hInternet, LPCTSTR pszUrl, DWORD dwConnectionTimeout = 5000);
    BOOL Download(LPCTSTR pszFileName, HWND hWnd = NULL, DWORD dwNumberOfConcurrentThreads = 5, DWORD dwRepeatCount = 10, DWORD dwWaitTime = 5000);

// ����
public:
    LPCTSTR GetFileName() const;
    DWORD GetContentLength() const;

// ʵ��
protected:
    BOOL CreateEvent();
    BOOL Open(LPTSTR pszUrl);

    BOOL CreateFile(LPCTSTR pszFileName);
    BOOL DoDownload(ATL::CAtlFile& file, ULARGE_INTEGER& liRange);

#ifdef _DEBUG
    void Dump() const;
#endif

    static UINT WINAPI ThreadProc(LPVOID pvParameter);
    static UINT WINAPI DownloadProc(LPVOID pvParameter);

// ��Ա
protected:
    HWND   m_hWnd;
    HANDLE m_hEvent;
    HANDLE m_hThread;

    CString m_strUrl;
    CString m_strFileName;
    HINTERNET m_hInternet;
    CRangeQueue m_deqRanges;

    DWORD m_dwWaitTime;
    BOOL  m_bAcceptRanges;
    DWORD m_dwRepeatCount;
    DWORD m_dwContentLength;
};

}  // namespace stdutil

#include "inetutil.inl"

#endif  // __INETUTIL_H__