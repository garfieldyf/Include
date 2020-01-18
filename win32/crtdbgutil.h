///////////////////////////////////////////////////////////////////////////////
// crtdbgutil.h
//
// Author : Garfield
// Creation Date : 2008/11/23

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __CRTDBGUTIL_H__
#define __CRTDBGUTIL_H__

#include "stddefx.h"
#include <atlstr.h>
#include <atlconv.h>

///////////////////////////////////////////////////////////////////////////////
// Classes in this file:
//
// CCrtConsole
// CCrtTrace
// CCrtReportLog

namespace stdutil {

#if (defined(__CRT_CONSOLE_TRACE__) && !defined(_WIN32_WCE))
///////////////////////////////////////////////////////////////////////////////
// Interface of the CCrtConsole class
//

class CCrtConsole
{
// Constructors/Destructor
private:
    ATL_NOINLINE CCrtConsole();
    ATL_NOINLINE ~CCrtConsole();

// Operations
public:
    ATL_NOINLINE void __cdecl operator()(LPCSTR pszFormat, ...) const;
    ATL_NOINLINE void __cdecl operator()(LPCWSTR pszFormat, ...) const;
    ATL_NOINLINE void operator()(DWORD dwErrorCode = ::GetLastError()) const;

    ATL_NOINLINE void __cdecl LogPrint(LPCSTR pszTag, LPCSTR pszFormat, ...) const;
    ATL_NOINLINE void __cdecl LogPrint(LPCWSTR pszTag, LPCWSTR pszFormat, ...) const;

// Attributes
public:
    static CCrtConsole _CrtConsole;

// Data members
private:
    HANDLE m_hConsole;
};
#endif  // (defined(__CRT_CONSOLE_TRACE__) && !defined(_WIN32_WCE))


#if (defined(__CRT_TRACE__) || defined(_DEBUG))
///////////////////////////////////////////////////////////////////////////////
// Interface of the CCrtTrace class
//

class CCrtTrace
{
// Constructors/Destructor
public:
    ATL_NOINLINE CCrtTrace();
    ATL_NOINLINE ~CCrtTrace();

// Operations
public:
    ATL_NOINLINE void __cdecl operator()(LPCSTR pszFormat, ...) const;
    ATL_NOINLINE void __cdecl operator()(LPCWSTR pszFormat, ...) const;
    ATL_NOINLINE void operator()(DWORD dwErrorCode = ::GetLastError()) const;

    ATL_NOINLINE void __cdecl LogPrint(LPCSTR pszTag, LPCSTR pszFormat, ...) const;
    ATL_NOINLINE void __cdecl LogPrint(LPCWSTR pszTag, LPCWSTR pszFormat, ...) const;

// Data members
public:
    static CCrtTrace _CrtTrace;
};
#endif  // (defined(__CRT_TRACE__) || defined(_DEBUG))


#ifdef __CRT_REPORT_LOG__
///////////////////////////////////////////////////////////////////////////////
// Interface of the CCrtReportLog class
//

class CCrtReportLog
{
// Constructors/Destructor
private:
    ATL_NOINLINE CCrtReportLog();
    ATL_NOINLINE ~CCrtReportLog();

// Operations
public:
    ATL_NOINLINE void SetReportLogFile(LPCTSTR pszFileName);
    ATL_NOINLINE void __cdecl operator()(LPCSTR pszTag, LPCSTR pszFileName, int nLine, LPCSTR pszFormat, ...) const;
    ATL_NOINLINE void __cdecl operator()(LPCWSTR pszTag, LPCSTR pszFileName, int nLine, LPCWSTR pszFormat, ...) const;

// Attributes
public:
    static CCrtReportLog _CrtReportLog;

// Implementation
private:
    ATL_NOINLINE void WriteMessage(LPCWSTR pszTag, LPCSTR pszFileName, int nLine, LPCWSTR pszMessage) const;

// Data members
private:
    HANDLE m_hFile;
};
#endif  // __CRT_REPORT_LOG__

}  // namespace stdutil

#include "crtdbgutil.inl"

#endif  // __CRTDBGUTIL_H__