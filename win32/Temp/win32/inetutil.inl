///////////////////////////////////////////////////////////////////////////////
// inetutil.inl : 实现文件
//
// Copyright(c) 2012, Garfield. All rights reserved.
// Author  : Garfield
// Version : 1.0
// Finish Time : 2008/9/12

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __INETUTIL_INL__
#define __INETUTIL_INL__

namespace stdutil {

///////////////////////////////////////////////////////////////////////////////
// CRangeQueue 类的实现
//

inline CRangeQueue::CRangeQueue()
{
}

inline void CRangeQueue::RemoveAll()
{
    ATL::CCritSecLock lock(m_cs);
    m_deqRanges.clear();
}

inline BOOL CRangeQueue::Pop(ULARGE_INTEGER& liRange)
{
    ATL::CCritSecLock lock(m_cs);

    BOOL bSuccessful = (m_deqRanges.size() > 0);
    if (bSuccessful)
    {
        liRange = m_deqRanges.back();
        m_deqRanges.pop_front();
    }

    return bSuccessful;
}

inline void CRangeQueue::Push(const ULARGE_INTEGER& liRange)
{
    ATL::CCritSecLock lock(m_cs);
    m_deqRanges.push_back(liRange);
}

#ifdef _DEBUG
inline void CRangeQueue::Dump() const
{
    ATLTRACE(_T("[ Concurrent Thread Count = %u ]\nRange :\n"), m_deqRanges.size());
    for (std::deque<ULARGE_INTEGER>::const_iterator itor = m_deqRanges.begin(); itor != m_deqRanges.end(); ++itor)
    {
        const ULARGE_INTEGER& liRange = (*itor);
        ATLTRACE(_T("    [ %u - %u bytes = %u ]\n"), liRange.LowPart, liRange.HighPart, liRange.HighPart - liRange.LowPart);
    }
}
#endif  // _DEBUG

inline DWORD CRangeQueue::GetCount() const
{
    return m_deqRanges.size();
}


///////////////////////////////////////////////////////////////////////////////
// CHttpDownloader 类的实现
//

inline CHttpDownloader::CHttpDownloader()
    : m_hWnd(NULL), m_hEvent(NULL), m_hThread(NULL), m_hInternet(NULL), m_dwWaitTime(0), m_bAcceptRanges(FALSE), m_dwRepeatCount(0), m_dwContentLength(0)
{
}

inline CHttpDownloader::~CHttpDownloader()
{
    ::CloseHandle(m_hEvent);
}

inline void CHttpDownloader::Clear()
{
    if (m_hThread != NULL && ::CloseHandle(m_hThread))
        m_hThread = NULL;

    m_dwContentLength = 0;
    m_bAcceptRanges = FALSE;

    ::ResetEvent(m_hEvent);
    m_deqRanges.RemoveAll();
}

inline void CHttpDownloader::Cancel()
{
    ATLASSERT(m_hEvent);

    if (::SetEvent(m_hEvent))
    {
        ::WaitForSingleObject(m_hThread, INFINITE);
        ATLVERIFY(::CloseHandle(m_hThread));
        m_hThread = NULL;
    }
}

inline BOOL CHttpDownloader::OpenUrl(HINTERNET hInternet, LPCTSTR pszUrl, DWORD dwConnectionTimeout/* = 5000*/)
{
    ATLASSERT(pszUrl);
    ATLASSERT(hInternet);
    ATLASSERT(m_hThread == NULL);
    ATLASSERT(m_deqRanges.GetCount() == 0);

    TCHAR szUrl[INTERNET_MAX_URL_LENGTH];
    DWORD dwUrlLength = _countof(szUrl);

    return (::InternetCanonicalizeUrl(pszUrl, szUrl, &dwUrlLength, ICU_BROWSER_MODE) && ::InternetSetOption(m_hInternet = hInternet, INTERNET_OPTION_CONNECT_TIMEOUT, &dwConnectionTimeout, sizeof(DWORD)) && Open(szUrl) ? (m_strUrl = szUrl, TRUE) : FALSE);
}

inline BOOL CHttpDownloader::Download(LPCTSTR pszFileName, HWND hWnd/* = NULL*/, DWORD dwNumberOfConcurrentThreads/* = 5*/, DWORD dwRepeatCount/* = 10*/, DWORD dwWaitTime/* = 5000*/)
{
    ATLASSERT(pszFileName);
    ATLASSERT(m_hInternet);
    ATLASSERT(m_hThread == NULL);
    ATLASSERT(m_deqRanges.GetCount() == 0);
    ATLASSERT(dwNumberOfConcurrentThreads > 0);

    if (CreateFile(pszFileName))
    {
        m_hWnd = hWnd;
        m_dwWaitTime  = dwWaitTime;
        m_strFileName = pszFileName;
        m_dwRepeatCount = dwRepeatCount;

        ULARGE_INTEGER liRange = { 0 };
        DWORD dwThreadCount    = (m_bAcceptRanges ? dwNumberOfConcurrentThreads : 1);
        DWORD dwAverageLength  = m_dwContentLength / dwThreadCount;

        for (DWORD i = 1; i < dwThreadCount; ++i)
        {
            liRange.HighPart = liRange.LowPart + dwAverageLength;
            m_deqRanges.Push(liRange);
            liRange.LowPart  = liRange.HighPart;
        }

        liRange.HighPart = liRange.LowPart + (m_dwContentLength - liRange.LowPart);
        m_deqRanges.Push(liRange);

    #ifdef _DEBUG
        Dump();
    #endif  // _DEBUG

        UINT nThreadId = 0;
        m_hThread = (HANDLE)::_beginthreadex(NULL, 0, ThreadProc, this, 0, &nThreadId);
    }

    return (m_hThread != NULL && CreateEvent());
}

inline DWORD CHttpDownloader::GetContentLength() const
{
    return m_dwContentLength;
}

inline LPCTSTR CHttpDownloader::GetFileName() const
{
    LPCTSTR pszFileName = ::_tcsrchr(m_strUrl, _T('/'));
    if (pszFileName != NULL)
        ++pszFileName;

    return pszFileName;
}

inline BOOL CHttpDownloader::CreateEvent()
{
    if (m_hEvent == NULL)
        m_hEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);

    return (m_hEvent != NULL);
}

inline BOOL CHttpDownloader::Open(LPTSTR pszUrl)
{
    ATLASSERT(pszUrl);
    ATLASSERT(m_hInternet);

    BOOL bSuccessful = FALSE;
    HINTERNET hConnect = NULL;

    __try
    {
        if ((hConnect = ::InternetOpenUrl(m_hInternet, pszUrl, NULL, 0, INTERNET_FLAG_DONT_CACHE | INTERNET_FLAG_KEEP_CONNECTION | INTERNET_FLAG_RELOAD | INTERNET_FLAG_PRAGMA_NOCACHE | INTERNET_FLAG_NO_AUTO_REDIRECT, 0)) == NULL)
            __leave;

        DWORD dwStatus = 0, dwLength = sizeof(DWORD);
        if (!::HttpQueryInfo(hConnect, HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER, &dwStatus, &dwLength, NULL))
            __leave;

    #ifdef _DEBUG
        TCHAR szHeaders[2048];
        DWORD dwHeaderLength = sizeof(szHeaders);
        if (::HttpQueryInfo(hConnect, HTTP_QUERY_RAW_HEADERS_CRLF, szHeaders, &dwHeaderLength, NULL))
            CRTTRACE(_T("%s\n"), szHeaders);
    #endif  // _DEBUG

        if (dwStatus == HTTP_STATUS_OK)
        {
            TCHAR szAcceptRanges[32];
            dwLength = sizeof(szAcceptRanges);
            if (::HttpQueryInfo(hConnect, HTTP_QUERY_ACCEPT_RANGES, szAcceptRanges, &dwLength, NULL))
                m_bAcceptRanges = (szAcceptRanges[0] != _T('\0'));

            dwLength = sizeof(DWORD);
            bSuccessful = ::HttpQueryInfo(hConnect, HTTP_QUERY_CONTENT_LENGTH | HTTP_QUERY_FLAG_NUMBER, &m_dwContentLength, &dwLength, NULL);
        }
        else if (dwStatus == HTTP_STATUS_REDIRECT)
        {
            dwLength = INTERNET_MAX_URL_LENGTH * sizeof(TCHAR);
            if (::HttpQueryInfo(hConnect, HTTP_QUERY_LOCATION, pszUrl, &dwLength, NULL))
                bSuccessful = Open(pszUrl);
        }
    }
    __finally
    {
        ::InternetCloseHandle(hConnect);
    }

    return bSuccessful;
}

inline BOOL CHttpDownloader::CreateFile(LPCTSTR pszFileName)
{
    ATLASSERT(pszFileName);

    BOOL bSuccessful = FALSE;
    HANDLE hFile = INVALID_HANDLE_VALUE;

    __try
    {
        if ((hFile = ::CreateFile(pszFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL)) == INVALID_HANDLE_VALUE)
            __leave;

        LARGE_INTEGER liOffset = { m_dwContentLength };
        liOffset.LowPart = ::SetFilePointer(hFile, (LONG)liOffset.LowPart, &liOffset.HighPart, FILE_BEGIN);
        if (liOffset.LowPart == INVALID_SET_FILE_POINTER && ::GetLastError() != NO_ERROR)
            __leave;

        bSuccessful = ::SetEndOfFile(hFile) && ::FlushFileBuffers(hFile);
    }
    __finally
    {
        ::CloseHandle(hFile);
        if (!bSuccessful)
            ::DeleteFile(pszFileName);
    }

    return bSuccessful;
}

#ifdef _DEBUG
inline void CHttpDownloader::Dump() const
{
    CString strDump;
    strDump.Format(_T("\n[ Filename = %s ]\n"), m_strFileName);
    ::OutputDebugString(strDump);

    ATLTRACE(_T("[ Wait Time = %u ms, Repeat Count = %u, Content Length = %u bytes ]\n"), m_dwWaitTime, m_dwRepeatCount, m_dwContentLength);
    m_deqRanges.Dump();
    ATLTRACE(_T("\n"));
}
#endif  // _DEBUG

inline UINT WINAPI CHttpDownloader::ThreadProc(LPVOID pvParameter)
{
    CHttpDownloader* pThis = reinterpret_cast<CHttpDownloader*>(pvParameter);
    ATLASSERT(pThis);

    if (DWORD dwThreadCount = pThis->m_deqRanges.GetCount() - 1)
    {
        ATL::CAtlArray<HANDLE> arrThreads;
        if (arrThreads.SetCount(dwThreadCount))
        {
            UINT nThreadId = 0;
            for (DWORD i = 0; i < dwThreadCount; ++i)
                arrThreads[i] = (HANDLE)::_beginthreadex(NULL, 0, DownloadProc, pvParameter, 0, &nThreadId);

            DownloadProc(pvParameter);
            ::WaitForMultipleObjects(dwThreadCount, arrThreads.GetData(), TRUE, INFINITE);

            for (DWORD i = 0; i < dwThreadCount; ++i)
                ATLVERIFY(::CloseHandle(arrThreads[i]));
        }
    }
    else
    {
        DownloadProc(pvParameter);
    }

    return 0;
}

inline UINT WINAPI CHttpDownloader::DownloadProc(LPVOID pvParameter)
{
    CHttpDownloader* pThis = reinterpret_cast<CHttpDownloader*>(pvParameter);
    ATLASSERT(pThis);

    ATL::CAtlFile file;
    if (SUCCEEDED(file.Create(pThis->m_strFileName, GENERIC_WRITE, FILE_SHARE_WRITE, OPEN_EXISTING)))
    {
        for (DWORD i = 0; i < pThis->m_dwRepeatCount; ++i)
        {
            ULARGE_INTEGER liRange;
            if (pThis->m_deqRanges.Pop(liRange))
            {
                if (pThis->DoDownload(file, liRange))
                {
                    continue;
                }
                else
                {
                    pThis->m_deqRanges.Push(liRange);
                    DWORD dwRet = ::WaitForSingleObject(pThis->m_hEvent, pThis->m_dwWaitTime);

                    if (dwRet == WAIT_OBJECT_0)
                    {
                        //cancal download;
                    }
                    else if (dwRet == WAIT_TIMEOUT)
                    {
                        //continue download
                        continue;
                    }
                    else if (dwRet == WAIT_FAILED)
                    {
                        //error download
                        continue;
                    }
                }
            }
            else
            {
                break;
            }
        }
    }

    return 0;
}

inline BOOL CHttpDownloader::DoDownload(ATL::CAtlFile& file, ULARGE_INTEGER& liRange)
{
    BOOL bSuccessful = FALSE;

    __try
    {
        if (FAILED(file.Seek(liRange.LowPart)))
            __leave;

        //file.Write(NULL, 0);
    }
    __finally
    {
    }

    return bSuccessful;
}

}  // namespace stdutil

#endif  // __INETUTIL_INL__