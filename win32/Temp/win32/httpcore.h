///////////////////////////////////////////////////////////////////////////////
// httpcore.h
//
// Copyright(c) 2012, Garfield. All rights reserved.
// Author  : Garfield
// Version : 1.0
// Finish Time : 2010/11/19

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __HTTPCORE_H__
#define __HTTPCORE_H__

#include <string>
#include <map>
#include <crtdbg.h>
#include <winsock2.h>

///////////////////////////////////////////////////////////////////////////////
// HTTP Version
//

#define HTTP_VERSION_1_0                    "HTTP/1.0"
#define HTTP_VERSION_1_1                    "HTTP/1.1"

///////////////////////////////////////////////////////////////////////////////
// HTTP Verb
//

#define HTTP_VERB_GET                       "GET"
#define HTTP_VERB_PUT                       "PUT"
#define HTTP_VERB_POST                      "POST"
#define HTTP_VERB_HEAD                      "HEAD"

///////////////////////////////////////////////////////////////////////////////
// HTTP Accept Types
//

#define HTTP_DEFAULT_ACCEPT_TYPES           "*/*"

///////////////////////////////////////////////////////////////////////////////
// Classes in this file:
//
// CHttpRequest

namespace stdutil {

///////////////////////////////////////////////////////////////////////////////
// Interface of the CHttpRequest class
//

class CHttpRequest
{
protected:
    typedef std::map<std::string, std::string> CMapHeaders;
    typedef std::map<std::string, std::string>::iterator iterator;
    typedef std::map<std::string, std::string>::value_type CHttpHeader;

// Constructors/Destructor
public:
    CHttpRequest();
    ~CHttpRequest();

// Operations
public:
    bool Open(const char* pszUrl, const char* pszVerb = HTTP_VERB_GET, unsigned nFlags = 0, const char* pszVersion = HTTP_VERSION_1_1, const char* pszAcceptTypes = HTTP_DEFAULT_ACCEPT_TYPES);
    void Close();

    bool Send(const void* pOptional = NULL, unsigned nOptionalLength = 0);
    bool Receive();

    void SetHeader(const char* pszHeader, const char* pszValue);
    bool QueryHeaders(const char* pszHeader, void* pBuffer, unsigned& nBufferLength);

#ifdef _CRT_DEBUG_DUMP
    template <typename _Pr> void Dump(const _Pr& _Printer) const;
#endif

// Implementation
protected:

// Data members
protected:
    SOCKET m_hSocket;
    CMapHeaders m_mapHeaders;
};

}  // namespace stdutil

#include "httpcore.inl"

#endif  // __HTTPCORE_H__