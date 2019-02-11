///////////////////////////////////////////////////////////////////////////////
// httputil.h
//
// Copyright(c) 2019, Garfield. All rights reserved.
// Author  : Garfield
// Version : 1.0
// Creation Date : 2009/10/21

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __HTTPUTIL_H__
#define __HTTPUTIL_H__

#ifdef _WIN32_WCE
    #error httputil.h compiles under Windows only
#endif

#include <winhttp.h>
#include "atlmiscutil.h"

///////////////////////////////////////////////////////////////////////////////
// Classes in this file:
//
// CHttpString
// CHttpBuffer
// CHttpSession
// CHttpConnection
// CHttpRequest
// CHttpProxyInfo
// CHttpProxyConfig
// CHttpStatusCallbackImpl<T>

namespace atlutil {

///////////////////////////////////////////////////////////////////////////////
// Interface of the CHttpString class
//

class CHttpString
{
// Constructors/Destructor
public:
    CHttpString();
    ~CHttpString();

// Operations
public:
    LPWSTR* operator&();
    operator LPWSTR();
    operator LPCWSTR() const;

// Attributes
public:
    BOOL IsEmpty() const;
    int GetLength() const;

// Implementation
private:
    CHttpString(const CHttpString& that);
    CHttpString& operator=(const CHttpString& that);

// Data members
private:
    LPWSTR m_pszData;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CHttpBuffer class
//

class CHttpBuffer : public CHeapBufferImpl<BYTE>
{
// Constructors
public:
    CHttpBuffer();

// Operations
public:
    BOOL Allocate(HINTERNET hRequest);
    BOOL Allocate(DWORD cbSize);

    BOOL Read(HINTERNET hRequest, DWORD dwNumberOfBytesToRead);
    void ReadComplete(HINTERNET hRequest, DWORD dwNumberOfBytesRead);
    BOOL Write(HINTERNET hRequest, DWORD dwNumberOfBytesToWrite) const;

// Data members
public:
    DWORD m_dwRead;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CHttpHandleTraits class
//

struct CHttpHandleTraits : public CHandleTraits
{
    typedef HINTERNET HANDLET_PTR;
    static BOOL WINAPI Destroy(HANDLET_PTR hHandle);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CHttpHandle class
//

class CHttpHandle : public CHandle<CHttpHandleTraits>
{
// Constructors
protected:
    CHttpHandle();

// Operations
public:
    BOOL SetOption(DWORD dwOption, LPVOID pBuffer, DWORD dwBufferLength);
    BOOL SetOption(DWORD dwOption, LPCWSTR pszValue);
    BOOL SetOption(DWORD dwOption, DWORD_PTR dwValue);

#if defined(__ATLSTR_H__) || defined(__AFXSTR_H__)
    BOOL QueryOption(DWORD dwOption, CStringW& strValue) const;
#endif

    BOOL QueryOption(DWORD dwOption, DWORD_PTR& dwValue) const;
    BOOL QueryOption(DWORD dwOption, LPVOID pBuffer, DWORD& dwBufferLength) const;

    BOOL SetTimeouts(int nResolveTimeout, int nConnectTimeout, int nSendTimeout, int nReceiveTimeout);
    BOOL SetStatusCallback(WINHTTP_STATUS_CALLBACK pfnStatusCallback, DWORD dwNotificationFlags = WINHTTP_CALLBACK_FLAG_ALL_COMPLETIONS, DWORD_PTR dwReserved = 0);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CHttpSession class
//

class CHttpSession : public CHttpHandle
{
// Constructors
public:
    CHttpSession();

// Operations
public:
    HINTERNET Open(LPCWSTR pszUserAgent = NULL, DWORD dwFlags = 0, DWORD dwAccessType = WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, LPCWSTR pszProxyName = WINHTTP_NO_PROXY_NAME, LPCWSTR pszProxyBypass = WINHTTP_NO_PROXY_BYPASS);
    BOOL GetProxyForUrl(LPCWSTR pszUrl, WINHTTP_AUTOPROXY_OPTIONS* pAutoProxyOptions, WINHTTP_PROXY_INFO* pProxyInfo) const;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CHttpConnection class
//

class CHttpConnection : public CHttpHandle
{
// Constructors
public:
    CHttpConnection();

// Operations
public:
    HINTERNET Connect(HINTERNET hSession, LPCWSTR pszServerName, INTERNET_PORT nServerPort = INTERNET_DEFAULT_PORT, DWORD dwReserved = 0);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CHttpRequest class
//

class CHttpRequest : public CHttpHandle
{
// Constructors
public:
    CHttpRequest();

// Operations
public:
    HINTERNET Open(HINTERNET hConnect, LPCWSTR pszObjectName = NULL, LPCWSTR pszVerb = NULL, DWORD dwFlags = WINHTTP_FLAG_ESCAPE_PERCENT | WINHTTP_FLAG_REFRESH, LPCWSTR* ppszAcceptTypes = WINHTTP_DEFAULT_ACCEPT_TYPES, LPCWSTR pszVersion = NULL, LPCWSTR pszReferer = WINHTTP_NO_REFERER);

    BOOL Send(LPCWSTR pszHeaders = WINHTTP_NO_ADDITIONAL_HEADERS, DWORD dwHeadersLength = 0, LPVOID pOptional = WINHTTP_NO_REQUEST_DATA, DWORD dwOptionalLength = 0, DWORD dwTotalLength = 0, DWORD_PTR dwContext = 0) const;
    BOOL AddHeaders(LPCWSTR pszHeaders, DWORD dwHeadersLength, DWORD dwModifiers = WINHTTP_ADDREQ_FLAG_ADD) const;

    BOOL ReceiveResponse(LPVOID pReserved = NULL) const;
    BOOL QueryDataAvailable(LPDWORD pdwNumberOfBytesAvailable = NULL) const;

    BOOL ReadData(LPVOID pBuffer, DWORD dwNumberOfBytesToRead, LPDWORD pdwNumberOfBytesRead) const;
    BOOL WriteData(LPCVOID pBuffer, DWORD dwNumberOfBytesToWrite, LPDWORD pdwNumberOfBytesWritten) const;

#if defined(__ATLSTR_H__) || defined(__AFXSTR_H__)
    BOOL QueryHeaders(DWORD dwInfoLevel, CStringW& strValue, LPCWSTR pszName = WINHTTP_HEADER_NAME_BY_INDEX, LPDWORD pdwIndex = WINHTTP_NO_HEADER_INDEX) const;
#endif
    BOOL QueryHeaders(DWORD dwInfoLevel, DWORD& dwValue, LPCWSTR pszName = WINHTTP_HEADER_NAME_BY_INDEX, LPDWORD pdwIndex = WINHTTP_NO_HEADER_INDEX) const;
    BOOL QueryHeaders(DWORD dwInfoLevel, LPVOID pBuffer, DWORD& dwBufferLength, LPCWSTR pszName = WINHTTP_HEADER_NAME_BY_INDEX, LPDWORD pdwIndex = WINHTTP_NO_HEADER_INDEX) const;

    BOOL QueryAuthSchemes(DWORD& dwSupportedSchemes, DWORD& dwFirstScheme, DWORD& dwAuthTarget) const;
    BOOL SetCredentials(LPCWSTR pszUserName, LPCWSTR pszPassword, DWORD dwAuthTargets = WINHTTP_AUTH_TARGET_SERVER, DWORD dwAuthScheme = WINHTTP_AUTH_SCHEME_BASIC, LPVOID pAuthParams = NULL);

#if (defined(__ATLSTR_H__) || defined(__AFXSTR_H__)) && defined(_CRT_DEBUG_DUMP)
    template <typename _Pr> void Dump(const _Pr& _Printer) const;
#endif

// Attributes
public:
    BOOL GetStatusCode(DWORD& dwStatusCode) const;
    BOOL GetContentLength(DWORD& dwContentLength) const;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CHttpProxyInfo class
//

class CHttpProxyInfo : public WINHTTP_PROXY_INFO
{
// Constructors/Destructor
public:
    CHttpProxyInfo();
    ~CHttpProxyInfo();

// Operations
public:
    operator WINHTTP_PROXY_INFO*();

// Implementation
private:
    CHttpProxyInfo(const CHttpProxyInfo& that);
    CHttpProxyInfo& operator=(const CHttpProxyInfo& that);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CHttpProxyConfig class
//

class CHttpProxyConfig : public WINHTTP_CURRENT_USER_IE_PROXY_CONFIG
{
// Constructors/Destructor
public:
    CHttpProxyConfig();
    ~CHttpProxyConfig();

// Operations
public:
    BOOL LoadIEProxyConfig();

// Implementation
private:
    CHttpProxyConfig(const CHttpProxyConfig& that);
    CHttpProxyConfig& operator=(const CHttpProxyConfig& that);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CHttpStatusCallbackImpl class
//

template <typename T>
class ATL_NO_VTABLE CHttpStatusCallbackImpl
{
// Operations
public:
    BOOL HttpStatusCallback_Register(HINTERNET hInternet, DWORD dwNotificationFlags = WINHTTP_CALLBACK_FLAG_ALL_COMPLETIONS, DWORD_PTR dwReserved = 0);
    BOOL HttpStatusCallback_Unregister(HINTERNET hInternet, DWORD_PTR dwReserved = 0);

// Overridables
public:
    void HttpStatusCallback_OnSendRequestComplete(HINTERNET hInternet);
    void HttpStatusCallback_OnHeadersAvailable(HINTERNET hInternet);
    void HttpStatusCallback_OnDataAvailable(HINTERNET hInternet, DWORD dwNumberOfBytesAvailable);

    void HttpStatusCallback_OnHandleClosing(HINTERNET hInternet, HINTERNET hInternetClosing);
    void HttpStatusCallback_OnRedirect(HINTERNET hInternet, LPCWSTR pszNewUrl, DWORD dwNewUrlLength);

    void HttpStatusCallback_OnSecureFailure(HINTERNET hInternet, DWORD dwSecureFlags);
    void HttpStatusCallback_OnIntermediateResponse(HINTERNET hInternet, DWORD dwStatusCode);
    void HttpStatusCallback_OnRequestError(HINTERNET hInternet, LPWINHTTP_ASYNC_RESULT pAsyncResult);

    void HttpStatusCallback_OnWriteComplete(HINTERNET hInternet, DWORD dwNumberOfBytesWritten);
    void HttpStatusCallback_OnReadComplete(HINTERNET hInternet, LPVOID pBuffer, DWORD dwNumberOfBytesRead);

// Implementation
protected:
    static void CALLBACK OnHttpStatusCallback(HINTERNET hInternet, DWORD_PTR dwContext, DWORD dwInternetStatus, LPVOID pStatusInformation, DWORD dwStatusInformationLength);
};

}  // namespace atlutil

#include "httputil.inl"

#endif  // __HTTPUTIL_H__