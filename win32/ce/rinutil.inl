///////////////////////////////////////////////////////////////////////////////
// rinutil.inl
//
// Copyright(c) 2019, Garfield. All rights reserved.
// Author  : Garfield
// Version : 1.0
// Creation Date : 2010/7/22

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __RINUTIL_INL__
#define __RINUTIL_INL__

#ifndef __RINUTIL_H__
    #error rinutil.inl requires rinutil.h to be included first
#endif

///////////////////////////////////////////////////////////////////////////////
// Global functions forward declaration
//

EXTERN_C DWORD  SetKMode(DWORD dwMode);
EXTERN_C DWORD  SetProcPermissions(DWORD dwPermissions);
EXTERN_C DWORD  PerformCallBack4(LPCALLBACKINFO pcbi, ...);
EXTERN_C LPVOID MapPtrToProcess(LPVOID pAddress, HANDLE hProcess);

namespace WCE {

///////////////////////////////////////////////////////////////////////////////
// Implementation of the CRemoteInvoker class
//

inline CRemoteInvoker::CRemoteInvoker()
    : m_hModule(NULL), m_hProcess(NULL)
{
}

inline BOOL CRemoteInvoker::LoadLibrary(HANDLE hProcess, LPCTSTR pszLibFileName)
{
    ATLASSERT(m_hModule == NULL);
    ATLASSERT(m_hProcess == NULL);

    if (FARPROC pfn = (FARPROC)::MapPtrToProcess(::GetProcAddress(::GetModuleHandle(_T("coredll")), _T("LoadLibraryW")), ::GetCurrentProcess()))
    {
        m_hProcess = hProcess;
        m_hModule  = (HMODULE)Invoke(pfn, ::MapPtrToProcess((LPVOID)pszLibFileName, ::GetCurrentProcess()));
    }

    return (m_hModule != NULL);
}

inline BOOL CRemoteInvoker::FreeLibrary()
{
    ATLASSERT(m_hModule);
    ATLASSERT(m_hProcess);

    BOOL bSuccessful = FALSE;
    if (FARPROC pfn = (FARPROC)::MapPtrToProcess(::GetProcAddress(::GetModuleHandle(_T("coredll")), _T("FreeLibrary")), ::GetCurrentProcess()))
        bSuccessful = (BOOL)Invoke(pfn, m_hModule);

    return bSuccessful;
}

inline DWORD CRemoteInvoker::Invoke(FARPROC pfn, LPVOID pvArg0/* = NULL*/, LPVOID pvArg1/* = NULL*/, LPVOID pvArg2/* = NULL*/, LPVOID pvArg3/* = NULL*/)
{
    ATLASSERT(pfn);
    ATLASSERT(m_hProcess);

    DWORD dwResult   = 0;
    DWORD dwOldMode  = ::SetKMode(TRUE);
    DWORD dwOldPerms = ::SetProcPermissions(0xFFFFFFFF);

    __try
    {
        CALLBACKINFO cbi = { m_hProcess, pfn, pvArg0 };
        dwResult = ::PerformCallBack4(&cbi, pvArg1, pvArg2, pvArg3);
    }
    __finally
    {
        ::SetKMode(dwOldMode);
        ::SetProcPermissions(dwOldPerms);
    }

    return dwResult;
}

inline DWORD CRemoteInvoker::Invoke(LPCTSTR pszProcName, LPVOID pvArg0/* = NULL*/, LPVOID pvArg1/* = NULL*/, LPVOID pvArg2/* = NULL*/, LPVOID pvArg3/* = NULL*/)
{
    ATLASSERT(m_hModule);
    ATLASSERT(m_hProcess);
    ATLASSERT(pszProcName);

    DWORD dwResult = 0;
    if (FARPROC pfn = ::GetProcAddress(m_hModule, pszProcName))
        dwResult = Invoke(pfn, pvArg0, pvArg1, pvArg2, pvArg3);

    return dwResult;
}

inline LPVOID WINAPI CRemoteInvoker::MapPtrToProcess(LPVOID pAddress)
{
    return ::MapPtrToProcess(pAddress, ::GetCurrentProcess());
}

}  // namespace WCE

#endif  // __RINUTIL_INL__