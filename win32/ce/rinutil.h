///////////////////////////////////////////////////////////////////////////////
// rinutil.h
//
// Author : Garfield
// Creation Date : 2010/7/22

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __RINUTIL_H__
#define __RINUTIL_H__

#ifndef _WIN32_WCE
    #error rinutil.h compiles under Windows CE only
#endif

#include "stddefx.h"

///////////////////////////////////////////////////////////////////////////////
// Classes in this file:
//
// CRemoteInvoker

namespace WCE {

///////////////////////////////////////////////////////////////////////////////
// Interface of the CRemoteInvoker class
//

class CRemoteInvoker
{
// Constructors
public:
    CRemoteInvoker();

// Operations
public:
    BOOL LoadLibrary(HANDLE hProcess, LPCTSTR pszLibFileName);
    BOOL FreeLibrary();

    DWORD Invoke(FARPROC pfn, LPVOID pvArg0 = NULL, LPVOID pvArg1 = NULL, LPVOID pvArg2 = NULL, LPVOID pvArg3 = NULL);
    DWORD Invoke(LPCTSTR pszProcName, LPVOID pvArg0 = NULL, LPVOID pvArg1 = NULL, LPVOID pvArg2 = NULL, LPVOID pvArg3 = NULL);

// Implementation
public:
    static LPVOID WINAPI MapPtrToProcess(LPVOID pAddress);

// Data members
public:
    HMODULE m_hModule;
    HANDLE  m_hProcess;
};

}  // namespace WCE

#include "rinutil.inl"

#endif  // __RINUTIL_H__