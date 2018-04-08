///////////////////////////////////////////////////////////////////////////////
// inetutil.h : 头文件
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
// CRangeQueue 类的定义
//

class CRangeQueue
{
// 构造
public:
    CRangeQueue();

// 操作
public:
    void RemoveAll();

    BOOL Pop(ULARGE_INTEGER& liRange);
    void Push(const ULARGE_INTEGER& liRange);

#ifdef _DEBUG
    void Dump() const;
#endif

// 属性
public:
    DWORD GetCount() const;

// 成员
private:
    ATL::CCriticalSection m_cs;
    std::deque<ULARGE_INTEGER> m_deqRanges;
};


///////////////////////////////////////////////////////////////////////////////
// CHttpDownloader 类的定义
//

class CHttpDownloader
{
// 构造/销毁
public:
    CHttpDownloader();
    ~CHttpDownloader();

// 操作
public:
    void Clear();
    void Cancel();

    BOOL OpenUrl(HINTERNET hInternet, LPCTSTR pszUrl, DWORD dwConnectionTimeout = 5000);
    BOOL Download(LPCTSTR pszFileName, HWND hWnd = NULL, DWORD dwNumberOfConcurrentThreads = 5, DWORD dwRepeatCount = 10, DWORD dwWaitTime = 5000);

// 属性
public:
    LPCTSTR GetFileName() const;
    DWORD GetContentLength() const;

// 实现
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

// 成员
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