///////////////////////////////////////////////////////////////////////////////
// tlhelp.h
//
// Copyright(c) 2018, Garfield. All rights reserved.
// Author  : Garfield
// Version : 1.0
// Creation Date : 2008/3/19

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __TOOLHELP_H__
#define __TOOLHELP_H__

#include <tlhelp32.h>
#include "atlmiscutil.h"

///////////////////////////////////////////////////////////////////////////////
// Classes in this file:
//
// CSystemSnapshot

namespace sysutil {

///////////////////////////////////////////////////////////////////////////////
// Interface of the CSystemSnapshotTraits class
//

struct CSystemSnapshotTraits : public atlutil::CHandleTraits
{
    enum { INVALID_HANDLE = (int)INVALID_HANDLE_VALUE };
    static BOOL WINAPI Destroy(HANDLET_PTR hHandle);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CSystemSnapshot class
//

class CSystemSnapshot : public atlutil::CHandle<CSystemSnapshotTraits>
{
// Constructors
public:
    CSystemSnapshot();

// Operations
public:
    BOOL Create(DWORD dwFlags, DWORD dwProcessID = 0);

    BOOL HeapListFirst(HEAPLIST32& heapList) const;
    BOOL HeapListNext(HEAPLIST32& heapList) const;

    BOOL ThreadFirst(THREADENTRY32& threadEntry) const;
    BOOL ThreadNext(THREADENTRY32& threadEntry) const;
    BOOL ThreadFind(DWORD dwThreadID, THREADENTRY32& threadEntry) const;

    BOOL ProcessFirst(PROCESSENTRY32& processEntry) const;
    BOOL ProcessNext(PROCESSENTRY32& processEntry) const;
    BOOL ProcessFind(DWORD dwProcessID, PROCESSENTRY32& processEntry) const;

    BOOL ModuleFirst(MODULEENTRY32& modileEntry) const;
    BOOL ModuleNext(MODULEENTRY32& modileEntry) const;
    BOOL ModuleFind(LPCTSTR pszModuleName, MODULEENTRY32& modileEntry) const;
    BOOL ModuleFind(LPCVOID pvBaseAddress, MODULEENTRY32& modileEntry) const;

#ifdef _WIN32_WCE
    BOOL HeapFirst(HEAPENTRY32& heapEntry, DWORD dwProcessID, ULONG_PTR ulHeapID);
    BOOL HeapNext(HEAPENTRY32& heapEntry);
#else
    static BOOL WINAPI HeapFirst(HEAPENTRY32& heapEntry, DWORD dwProcessID, ULONG_PTR ulHeapID);
    static BOOL WINAPI HeapNext(HEAPENTRY32& heapEntry);
#endif

    static BOOL WINAPI ReadProcessMemory(DWORD dwProcessID, LPCVOID pvBaseAddress, LPVOID pvBuffer, SIZE_T cbRead, SIZE_T* pNumberOfBytesRead = NULL);
};

}  // namespace sysutil

#include "tlhelp.inl"

#endif  // __TOOLHELP_H__