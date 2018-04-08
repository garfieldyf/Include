///////////////////////////////////////////////////////////////////////////////
// httpcore.inl
//
// Copyright(c) 2012, Garfield. All rights reserved.
// Author  : Garfield
// Version : 1.0
// Finish Time : 2010/11/19

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __HTTPCORE_INL__
#define __HTTPCORE_INL__

#ifndef __HTTPCORE_H__
    #error httpcore.inl requires httpcore.h to be included first
#endif

namespace stdutil {

///////////////////////////////////////////////////////////////////////////////
// Implementation of the CHttpRequest class
//

inline CHttpRequest::CHttpRequest()
    : m_hSocket(INVALID_SOCKET)
{
}

inline CHttpRequest::~CHttpRequest()
{
    Close();
}

inline bool CHttpRequest::Open(const char* pszUrl, const char* pszVerb/* = HTTP_VERB_GET*/, unsigned nFlags/* = 0*/, const char* pszVersion/* = HTTP_VERSION_1_1*/, const char* pszAcceptTypes/* = HTTP_DEFAULT_ACCEPT_TYPES*/)
{
    _ASSERTE(pszUrl);
    _ASSERTE(pszVerb);
    _ASSERTE(pszVersion);

    m_hSocket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
}

inline void CHttpRequest::Close()
{
    if (m_hSocket != INVALID_SOCKET && ::closesocket(m_hSocket) == 0)
        m_hSocket = INVALID_SOCKET;
}

inline void CHttpRequest::SetHeader(const char* pszHeader, const char* pszValue)
{
    _ASSERTE(pszHeader);

    if (pszValue == NULL)
        m_mapHeaders.erase(pszHeader);
    else
        m_mapHeaders.insert(CHttpHeader(pszHeader, pszValue));
}

}  // namespace stdutil

#endif  // __HTTPCORE_INL__