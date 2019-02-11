///////////////////////////////////////////////////////////////////////////////
// httputil.inl
//
// Copyright(c) 2019, Garfield. All rights reserved.
// Author  : Garfield
// Version : 1.0
// Creation Date : 2009/10/21

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __HTTPUTIL_INL__
#define __HTTPUTIL_INL__

#ifndef __HTTPUTIL_H__
    #error httputil.inl requires httputil.h to be included first
#endif

namespace atlutil {

///////////////////////////////////////////////////////////////////////////////
// Implementation of the CHttpString class
//

inline CHttpString::CHttpString()
    : m_pszData(NULL)
{
}

inline CHttpString::~CHttpString()
{
    _COND_VERIFY(::GlobalFree(m_pszData), NULL);
}

inline LPWSTR* CHttpString::operator&()
{
    ATLASSERT(IsEmpty());
    return &m_pszData;
}

inline CHttpString::operator LPWSTR()
{
    return m_pszData;
}

inline CHttpString::operator LPCWSTR() const
{
    return m_pszData;
}

inline BOOL CHttpString::IsEmpty() const
{
    return (m_pszData == NULL);
}

inline int CHttpString::GetLength() const
{
    ATLASSERT(!IsEmpty());
    return ::lstrlenW(m_pszData);
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CHttpBuffer class
//

inline CHttpBuffer::CHttpBuffer()
    : m_dwRead(0)
{
}

inline BOOL CHttpBuffer::Allocate(HINTERNET hRequest)
{
    ATLASSERT(hRequest);

    DWORD dwLength = 0, cbSize = sizeof(DWORD);
    ::WinHttpQueryHeaders(hRequest, WINHTTP_QUERY_CONTENT_LENGTH | WINHTTP_QUERY_FLAG_NUMBER, WINHTTP_HEADER_NAME_BY_INDEX, &dwLength, &cbSize, WINHTTP_NO_HEADER_INDEX);
    ATLASSERT(cbSize == sizeof(DWORD));

    m_dwRead = 0;
    return (dwLength != 0 && m_buffer.allocateBytes(dwLength) != NULL);
}

inline BOOL CHttpBuffer::Allocate(DWORD cbSize)
{
    ATLASSERT(cbSize != 0);

    m_dwRead = 0;
    return (m_buffer.allocateBytes(cbSize) != NULL);
}

inline BOOL CHttpBuffer::Read(HINTERNET hRequest, DWORD dwNumberOfBytesToRead)
{
    ATLASSERT(hRequest);
    ATLASSERT(!IsEmpty());
    ATLASSERT(dwNumberOfBytesToRead != 0);

    return ::WinHttpReadData(hRequest, m_buffer.data + m_dwRead, dwNumberOfBytesToRead, NULL);
}

inline void CHttpBuffer::ReadComplete(HINTERNET hRequest, DWORD dwNumberOfBytesRead)
{
    ATLASSERT(hRequest);
    ATLASSERT(m_dwRead + dwNumberOfBytesRead <= m_buffer.capacity());

    m_dwRead += dwNumberOfBytesRead;
    ATLVERIFY(::WinHttpQueryDataAvailable(hRequest, NULL));
}

inline BOOL CHttpBuffer::Write(HINTERNET hRequest, DWORD dwNumberOfBytesToWrite) const
{
    ATLASSERT(hRequest);
    ATLASSERT(!IsEmpty());
    ATLASSERT(dwNumberOfBytesToWrite != 0);

    return ::WinHttpWriteData(hRequest, m_buffer.data, dwNumberOfBytesToWrite, NULL);
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CHttpHandleTraits class
//

inline BOOL WINAPI CHttpHandleTraits::Destroy(HANDLET_PTR hHandle)
{
    return ::WinHttpCloseHandle(hHandle);
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CHttpHandle class
//

inline CHttpHandle::CHttpHandle()
{
}

inline BOOL CHttpHandle::SetOption(DWORD dwOption, LPVOID pBuffer, DWORD dwBufferLength)
{
    ATLASSERT(pBuffer);
    ATLASSERT(!IsEmpty());

    return ::WinHttpSetOption(m_hHandle, dwOption, pBuffer, dwBufferLength);
}

inline BOOL CHttpHandle::SetOption(DWORD dwOption, LPCWSTR pszValue)
{
    ATLASSERT(pszValue);
    ATLASSERT(!IsEmpty());

    return ::WinHttpSetOption(m_hHandle, dwOption, (LPVOID)pszValue, ::lstrlenW(pszValue));
}

inline BOOL CHttpHandle::SetOption(DWORD dwOption, DWORD_PTR dwValue)
{
    ATLASSERT(!IsEmpty());
    return ::WinHttpSetOption(m_hHandle, dwOption, &dwValue, sizeof(DWORD_PTR));
}

#if defined(__ATLSTR_H__) || defined(__AFXSTR_H__)
inline BOOL CHttpHandle::QueryOption(DWORD dwOption, CStringW& strValue) const
{
    ATLASSERT(!IsEmpty());

    BOOL bSuccessful = FALSE;
    DWORD dwBufferLength = 0;

    if (!::WinHttpQueryOption(m_hHandle, dwOption, NULL, &dwBufferLength) && ::GetLastError() == ERROR_INSUFFICIENT_BUFFER)
    {
        ATLASSERT(dwBufferLength % sizeof(WCHAR) == 0);
        bSuccessful = ::WinHttpQueryOption(m_hHandle, dwOption, strValue.GetBuffer(dwBufferLength / sizeof(WCHAR)), &dwBufferLength);
        strValue.ReleaseBuffer(dwBufferLength / sizeof(WCHAR));
    }

    return bSuccessful;
}
#endif  // defined(__ATLSTR_H__) || defined(__AFXSTR_H__)

inline BOOL CHttpHandle::QueryOption(DWORD dwOption, DWORD_PTR& dwValue) const
{
    ATLASSERT(!IsEmpty());

    DWORD dwBufferLength = sizeof(DWORD_PTR);
    return ::WinHttpQueryOption(m_hHandle, dwOption, &dwValue, &dwBufferLength);
}

inline BOOL CHttpHandle::QueryOption(DWORD dwOption, LPVOID pBuffer, DWORD& dwBufferLength) const
{
    ATLASSERT(!IsEmpty());
    return ::WinHttpQueryOption(m_hHandle, dwOption, pBuffer, &dwBufferLength);
}

inline BOOL CHttpHandle::SetTimeouts(int nResolveTimeout, int nConnectTimeout, int nSendTimeout, int nReceiveTimeout)
{
    ATLASSERT(!IsEmpty());
    return ::WinHttpSetTimeouts(m_hHandle, nResolveTimeout, nConnectTimeout, nSendTimeout, nReceiveTimeout);
}

inline BOOL CHttpHandle::SetStatusCallback(WINHTTP_STATUS_CALLBACK pfnStatusCallback, DWORD dwNotificationFlags/* = WINHTTP_CALLBACK_FLAG_ALL_COMPLETIONS*/, DWORD_PTR dwReserved/* = 0*/)
{
    ATLASSERT(!IsEmpty());
    return (::WinHttpSetStatusCallback(m_hHandle, pfnStatusCallback, dwNotificationFlags, dwReserved) != WINHTTP_INVALID_STATUS_CALLBACK);
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CHttpSession class
//

inline CHttpSession::CHttpSession()
{
}

inline HINTERNET CHttpSession::Open(LPCWSTR pszUserAgent/* = NULL*/, DWORD dwFlags/* = 0*/, DWORD dwAccessType/* = WINHTTP_ACCESS_TYPE_DEFAULT_PROXY*/, LPCWSTR pszProxyName/* = WINHTTP_NO_PROXY_NAME*/, LPCWSTR pszProxyBypass/* = WINHTTP_NO_PROXY_BYPASS*/)
{
    ATLASSERT(IsEmpty());
    return (m_hHandle = ::WinHttpOpen(pszUserAgent, dwAccessType, pszProxyName, pszProxyBypass, dwFlags));
}

inline BOOL CHttpSession::GetProxyForUrl(LPCWSTR pszUrl, WINHTTP_AUTOPROXY_OPTIONS* pAutoProxyOptions, WINHTTP_PROXY_INFO* pProxyInfo) const
{
    ATLASSERT(IsEmpty());
    ATLASSERT(pProxyInfo);
    ATLASSERT(pAutoProxyOptions);

    return ::WinHttpGetProxyForUrl(m_hHandle, pszUrl, pAutoProxyOptions, pProxyInfo);
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CHttpConnection class
//

inline CHttpConnection::CHttpConnection()
{
}

inline HINTERNET CHttpConnection::Connect(HINTERNET hSession, LPCWSTR pszServerName, INTERNET_PORT nServerPort/* = INTERNET_DEFAULT_PORT*/, DWORD dwReserved/* = 0*/)
{
    ATLASSERT(hSession);
    ATLASSERT(IsEmpty());
    ATLASSERT(pszServerName);

    return (m_hHandle = ::WinHttpConnect(hSession, pszServerName, nServerPort, dwReserved));
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CHttpRequest class
//

inline CHttpRequest::CHttpRequest()
{
}

inline HINTERNET CHttpRequest::Open(HINTERNET hConnect, LPCWSTR pszObjectName/* = NULL*/, LPCWSTR pszVerb/* = NULL*/, DWORD dwFlags/* = WINHTTP_FLAG_ESCAPE_PERCENT | WINHTTP_FLAG_REFRESH*/, LPCWSTR* ppszAcceptTypes/* = WINHTTP_DEFAULT_ACCEPT_TYPES*/, LPCWSTR pszVersion/* = NULL*/, LPCWSTR pszReferer/* = WINHTTP_NO_REFERER*/)
{
    ATLASSERT(hConnect);
    ATLASSERT(IsEmpty());

    return (m_hHandle = ::WinHttpOpenRequest(hConnect, pszVerb, pszObjectName, pszVersion, pszReferer, ppszAcceptTypes, dwFlags));
}

inline BOOL CHttpRequest::Send(LPCWSTR pszHeaders/* = WINHTTP_NO_ADDITIONAL_HEADERS*/, DWORD dwHeadersLength/* = 0*/, LPVOID pOptional/* = WINHTTP_NO_REQUEST_DATA*/, DWORD dwOptionalLength/* = 0*/, DWORD dwTotalLength/* = 0*/, DWORD_PTR dwContext/* = 0*/) const
{
    ATLASSERT(!IsEmpty());
    return ::WinHttpSendRequest(m_hHandle, pszHeaders, dwHeadersLength, pOptional, dwOptionalLength, dwTotalLength, dwContext);
}

inline BOOL CHttpRequest::AddHeaders(LPCWSTR pszHeaders, DWORD dwHeadersLength, DWORD dwModifiers/* = WINHTTP_ADDREQ_FLAG_ADD*/) const
{
    ATLASSERT(!IsEmpty());
    ATLASSERT(pszHeaders);

    return ::WinHttpAddRequestHeaders(m_hHandle, pszHeaders, dwHeadersLength, dwModifiers);
}

inline BOOL CHttpRequest::ReceiveResponse(LPVOID pReserved/* = NULL*/) const
{
    ATLASSERT(!IsEmpty());
    return ::WinHttpReceiveResponse(m_hHandle, pReserved);
}

inline BOOL CHttpRequest::QueryDataAvailable(LPDWORD pdwNumberOfBytesAvailable/* = NULL*/) const
{
    ATLASSERT(!IsEmpty());
    return ::WinHttpQueryDataAvailable(m_hHandle, pdwNumberOfBytesAvailable);
}

inline BOOL CHttpRequest::ReadData(LPVOID pBuffer, DWORD dwNumberOfBytesToRead, LPDWORD pdwNumberOfBytesRead) const
{
    ATLASSERT(pBuffer);
    ATLASSERT(!IsEmpty());

    return ::WinHttpReadData(m_hHandle, pBuffer, dwNumberOfBytesToRead, pdwNumberOfBytesRead);
}

inline BOOL CHttpRequest::WriteData(LPCVOID pBuffer, DWORD dwNumberOfBytesToWrite, LPDWORD pdwNumberOfBytesWritten) const
{
    ATLASSERT(pBuffer);
    ATLASSERT(!IsEmpty());

    return ::WinHttpWriteData(m_hHandle, pBuffer, dwNumberOfBytesToWrite, pdwNumberOfBytesWritten);
}

#if defined(__ATLSTR_H__) || defined(__AFXSTR_H__)
inline BOOL CHttpRequest::QueryHeaders(DWORD dwInfoLevel, CStringW& strValue, LPCWSTR pszName/* = WINHTTP_HEADER_NAME_BY_INDEX*/, LPDWORD pdwIndex/* = WINHTTP_NO_HEADER_INDEX*/) const
{
    ATLASSERT(!IsEmpty());

    BOOL bSuccessful = FALSE;
    DWORD dwBufferLength = 0;

    if (!::WinHttpQueryHeaders(m_hHandle, dwInfoLevel, pszName, WINHTTP_NO_OUTPUT_BUFFER, &dwBufferLength, pdwIndex) && ::GetLastError() == ERROR_INSUFFICIENT_BUFFER)
    {
        ATLASSERT(dwBufferLength % sizeof(WCHAR) == 0);
        bSuccessful = ::WinHttpQueryHeaders(m_hHandle, dwInfoLevel, pszName, strValue.GetBuffer(dwBufferLength / sizeof(WCHAR)), &dwBufferLength, pdwIndex);
        strValue.ReleaseBuffer(dwBufferLength / sizeof(WCHAR));
    }

    return bSuccessful;
}
#endif  // defined(__ATLSTR_H__) || defined(__AFXSTR_H__)

inline BOOL CHttpRequest::QueryHeaders(DWORD dwInfoLevel, DWORD& dwValue, LPCWSTR pszName/* = WINHTTP_HEADER_NAME_BY_INDEX*/, LPDWORD pdwIndex/* = WINHTTP_NO_HEADER_INDEX*/) const
{
    ATLASSERT(!IsEmpty());

    DWORD dwBufferLength = sizeof(DWORD);
    return ::WinHttpQueryHeaders(m_hHandle, dwInfoLevel | WINHTTP_QUERY_FLAG_NUMBER, pszName, &dwValue, &dwBufferLength, pdwIndex);
}

inline BOOL CHttpRequest::QueryHeaders(DWORD dwInfoLevel, LPVOID pBuffer, DWORD& dwBufferLength, LPCWSTR pszName/* = WINHTTP_HEADER_NAME_BY_INDEX*/, LPDWORD pdwIndex/* = WINHTTP_NO_HEADER_INDEX*/) const
{
    ATLASSERT(!IsEmpty());
    return ::WinHttpQueryHeaders(m_hHandle, dwInfoLevel, pszName, pBuffer, &dwBufferLength, pdwIndex);
}

inline BOOL CHttpRequest::QueryAuthSchemes(DWORD& dwSupportedSchemes, DWORD& dwFirstScheme, DWORD& dwAuthTarget) const
{
    ATLASSERT(!IsEmpty());
    return ::WinHttpQueryAuthSchemes(m_hHandle, &dwSupportedSchemes, &dwFirstScheme, &dwAuthTarget);
}

inline BOOL CHttpRequest::SetCredentials(LPCWSTR pszUserName, LPCWSTR pszPassword, DWORD dwAuthTargets/* = WINHTTP_AUTH_TARGET_SERVER*/, DWORD dwAuthScheme/* = WINHTTP_AUTH_SCHEME_BASIC*/, LPVOID pAuthParams/* = NULL*/)
{
    ATLASSERT(!IsEmpty());
    return ::WinHttpSetCredentials(m_hHandle, dwAuthTargets, dwAuthScheme, pszUserName, pszPassword, pAuthParams);
}

#if (defined(__ATLSTR_H__) || defined(__AFXSTR_H__)) && defined(_CRT_DEBUG_DUMP)
template <typename _Pr>
inline void CHttpRequest::Dump(const _Pr& _Printer) const
{
    ATLASSERT(!IsEmpty());

    CStringW strHeaders;
    if (QueryHeaders(WINHTTP_QUERY_RAW_HEADERS_CRLF | WINHTTP_QUERY_FLAG_REQUEST_HEADERS, strHeaders))
        _Printer(L"[ ========== REQUEST  HEADERS ========== ]\n%s", strHeaders);

    if (QueryHeaders(WINHTTP_QUERY_RAW_HEADERS_CRLF, strHeaders))
        _Printer(L"[ ========== RESPONSE HEADERS ========== ]\n%s", strHeaders);
}
#endif  // (defined(__ATLSTR_H__) || defined(__AFXSTR_H__)) && defined(_CRT_DEBUG_DUMP)

inline BOOL CHttpRequest::GetStatusCode(DWORD& dwStatusCode) const
{
    ATLASSERT(!IsEmpty());
    return QueryHeaders(WINHTTP_QUERY_STATUS_CODE, dwStatusCode);
}

inline BOOL CHttpRequest::GetContentLength(DWORD& dwContentLength) const
{
    ATLASSERT(!IsEmpty());
    return QueryHeaders(WINHTTP_QUERY_CONTENT_LENGTH, dwContentLength);
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CHttpProxyInfo class
//

inline CHttpProxyInfo::CHttpProxyInfo()
{
    lpszProxy = lpszProxyBypass = NULL;
}

inline CHttpProxyInfo::~CHttpProxyInfo()
{
    _COND_VERIFY(::GlobalFree(lpszProxy), NULL);
    _COND_VERIFY(::GlobalFree(lpszProxyBypass), NULL);
}

inline CHttpProxyInfo::operator WINHTTP_PROXY_INFO*()
{
    ATLASSERT(lpszProxy == NULL);
    ATLASSERT(lpszProxyBypass == NULL);

    return this;
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CHttpProxyConfig class
//

inline CHttpProxyConfig::CHttpProxyConfig()
{
    lpszProxy = lpszProxyBypass = lpszAutoConfigUrl = NULL;
}

inline CHttpProxyConfig::~CHttpProxyConfig()
{
    _COND_VERIFY(::GlobalFree(lpszProxy), NULL);
    _COND_VERIFY(::GlobalFree(lpszProxyBypass), NULL);
    _COND_VERIFY(::GlobalFree(lpszAutoConfigUrl), NULL);
}

inline BOOL CHttpProxyConfig::LoadIEProxyConfig()
{
    ATLASSERT(lpszProxy == NULL);
    ATLASSERT(lpszProxyBypass == NULL);
    ATLASSERT(lpszAutoConfigUrl == NULL);

    return ::WinHttpGetIEProxyConfigForCurrentUser(this);
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CHttpStatusCallbackImpl class
//

template <typename T>
inline BOOL CHttpStatusCallbackImpl<T>::HttpStatusCallback_Register(HINTERNET hInternet, DWORD dwNotificationFlags/* = WINHTTP_CALLBACK_FLAG_ALL_COMPLETIONS*/, DWORD_PTR dwReserved/* = 0*/)
{
    ATLASSERT(hInternet);

#ifdef _DEBUG
    DWORD_PTR dwValue = 0, dwBufferLength = sizeof(DWORD_PTR);
    ATLVERIFY(::WinHttpQueryOption(hInternet, WINHTTP_OPTION_CALLBACK, &dwValue, &dwBufferLength));
    ATLASSERT(dwValue == 0 && dwBufferLength == sizeof(DWORD_PTR));

    ATLVERIFY(::WinHttpQueryOption(hInternet, WINHTTP_OPTION_CONTEXT_VALUE, &dwValue, &dwBufferLength));
    ATLASSERT(dwValue == 0 && dwBufferLength == sizeof(DWORD_PTR));
#endif  // _DEBUG

    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);

    return (::WinHttpSetOption(hInternet, WINHTTP_OPTION_CONTEXT_VALUE, &pThis, sizeof(DWORD_PTR)) && ::WinHttpSetStatusCallback(hInternet, T::OnHttpStatusCallback, dwNotificationFlags, dwReserved) != WINHTTP_INVALID_STATUS_CALLBACK);
}

template <typename T>
inline BOOL CHttpStatusCallbackImpl<T>::HttpStatusCallback_Unregister(HINTERNET hInternet, DWORD_PTR dwReserved/* = 0*/)
{
    ATLASSERT(hInternet);
    return (::WinHttpSetStatusCallback(hInternet, NULL, 0, dwReserved) != WINHTTP_INVALID_STATUS_CALLBACK);
}

template <typename T>
inline void CHttpStatusCallbackImpl<T>::HttpStatusCallback_OnSendRequestComplete(HINTERNET /*hInternet*/)
{
    // To GET, the derived class should call WinHttpReceiveResponse to begin receiving the response.
    // To POST, the derived class should call WinHttpWriteData to uploading data to server.

    // Note : If the write operation is asynchronous, do not delete the buffer immediately after calling
    // WinHttpWriteData. This causes WinHTTP to read invalid data. The correct time to delete the buffer
    // is in the status callback function after the WINHTTP_CALLBACK_STATUS_WRITE_COMPLETE completion is
    // received.
}

template <typename T>
inline void CHttpStatusCallbackImpl<T>::HttpStatusCallback_OnHeadersAvailable(HINTERNET /*hInternet*/)
{
    // The derived class should call WinHttpQueryHeaders to retrieves the HTTP headers, such as
    // http status code, http status text, content length, etc.

    // The derived class should prepare the buffer to start downloading the contents and then call
    // WinHttpQueryDataAvailable to retrieves the available data.

    // Note : The call to WinHttpQueryDataAvailable generates a status callback with a
    // WINHTTP_CALLBACK_STATUS_DATA_AVAILABLE completion in the dwInternetStatus parameter.
}

template <typename T>
inline void CHttpStatusCallbackImpl<T>::HttpStatusCallback_OnDataAvailable(HINTERNET /*hInternet*/, DWORD /*dwNumberOfBytesAvailable*/)
{
    // If dwNumberOfBytesAvailable is not zero, the derived class should call WinHttpReadData to read
    // the data. Otherwise, there is no data to read.

    // Note : If the read operation is asynchronous, do not delete the buffer immediately after calling
    // WinHttpReadData. This causes WinHTTP to try to write data to unallocated memory, and an exception
    // is raised. The correct time to delete the buffer is in the status callback function after the
    // WINHTTP_CALLBACK_STATUS_READ_COMPLETE completion is received.
}

template <typename T>
inline void CHttpStatusCallbackImpl<T>::HttpStatusCallback_OnHandleClosing(HINTERNET /*hInternet*/, HINTERNET /*hInternetClosing*/)
{
}

template <typename T>
inline void CHttpStatusCallbackImpl<T>::HttpStatusCallback_OnRedirect(HINTERNET /*hInternet*/, LPCWSTR /*pszNewUrl*/, DWORD /*dwNewUrlLength*/)
{
}

template <typename T>
inline void CHttpStatusCallbackImpl<T>::HttpStatusCallback_OnSecureFailure(HINTERNET /*hInternet*/, DWORD /*dwSecureFlags*/)
{
}

template <typename T>
inline void CHttpStatusCallbackImpl<T>::HttpStatusCallback_OnIntermediateResponse(HINTERNET /*hInternet*/, DWORD /*dwStatusCode*/)
{
}

template <typename T>
inline void CHttpStatusCallbackImpl<T>::HttpStatusCallback_OnRequestError(HINTERNET /*hInternet*/, LPWINHTTP_ASYNC_RESULT /*pAsyncResult*/)
{
}

template <typename T>
inline void CHttpStatusCallbackImpl<T>::HttpStatusCallback_OnWriteComplete(HINTERNET /*hInternet*/, DWORD /*dwNumberOfBytesWritten*/)
{
    // To POST, the derived class should call WinHttpReceiveResponse to begin receiving the response.
}

template <typename T>
inline void CHttpStatusCallbackImpl<T>::HttpStatusCallback_OnReadComplete(HINTERNET /*hInternet*/, LPVOID /*pBuffer*/, DWORD /*dwNumberOfBytesRead*/)
{
    // The derived class should call WinHttpQueryDataAvailable to retrieves the available data.
}

template <typename T>
inline void CALLBACK CHttpStatusCallbackImpl<T>::OnHttpStatusCallback(HINTERNET hInternet, DWORD_PTR dwContext, DWORD dwInternetStatus, LPVOID pStatusInformation, DWORD dwStatusInformationLength)
{
    T* pThis = reinterpret_cast<T*>(dwContext);
    ATLASSERT(pThis);
    ATLASSERT(hInternet);

    switch (dwInternetStatus)
    {
    case WINHTTP_CALLBACK_STATUS_DATA_AVAILABLE:
        pThis->HttpStatusCallback_OnDataAvailable(hInternet, *(LPDWORD)pStatusInformation);
        break;

    case WINHTTP_CALLBACK_STATUS_REDIRECT:
        pThis->HttpStatusCallback_OnRedirect(hInternet, (LPCWSTR)pStatusInformation, dwStatusInformationLength - 1);
        break;

    case WINHTTP_CALLBACK_STATUS_SENDREQUEST_COMPLETE:
        pThis->HttpStatusCallback_OnSendRequestComplete(hInternet);
        break;

    case WINHTTP_CALLBACK_STATUS_HEADERS_AVAILABLE:
        pThis->HttpStatusCallback_OnHeadersAvailable(hInternet);
        break;

    case WINHTTP_CALLBACK_STATUS_REQUEST_ERROR:
        pThis->HttpStatusCallback_OnRequestError(hInternet, (LPWINHTTP_ASYNC_RESULT)pStatusInformation);
        break;

    case WINHTTP_CALLBACK_STATUS_WRITE_COMPLETE:
        pThis->HttpStatusCallback_OnWriteComplete(hInternet, *(LPDWORD)pStatusInformation);
        break;

    case WINHTTP_CALLBACK_STATUS_READ_COMPLETE:
        pThis->HttpStatusCallback_OnReadComplete(hInternet, pStatusInformation, dwStatusInformationLength);
        break;

    case WINHTTP_CALLBACK_STATUS_HANDLE_CLOSING:
        pThis->HttpStatusCallback_OnHandleClosing(hInternet, *(LPHINTERNET)pStatusInformation);
        break;

    case WINHTTP_CALLBACK_STATUS_SECURE_FAILURE:
        pThis->HttpStatusCallback_OnSecureFailure(hInternet, *(LPDWORD)pStatusInformation);
        break;

    case WINHTTP_CALLBACK_STATUS_INTERMEDIATE_RESPONSE:
        pThis->HttpStatusCallback_OnIntermediateResponse(hInternet, *(LPDWORD)pStatusInformation);
        break;
    }
}

}  // namespace atlutil

#endif  // __HTTPUTIL_INL__