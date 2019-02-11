///////////////////////////////////////////////////////////////////////////////
// tlhelp.inl
//
// Copyright(c) 2019, Garfield. All rights reserved.
// Author  : Garfield
// Version : 1.0
// Creation Date : 2008/3/19

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __TOOLHELP_INL__
#define __TOOLHELP_INL__

#ifndef __TOOLHELP_H__
    #error tlhelp.inl requires tlhelp.h to be included first
#endif

namespace sysutil {

///////////////////////////////////////////////////////////////////////////////
// Implementation of the CSystemSnapshotTraits class
//

inline BOOL WINAPI CSystemSnapshotTraits::Destroy(HANDLET_PTR hHandle)
{
#ifndef _WIN32_WCE
    return ::CloseHandle(hHandle);
#else
    return ::CloseToolhelp32Snapshot(hHandle);
#endif  // _WIN32_WCE
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CSystemSnapshot class
//

inline CSystemSnapshot::CSystemSnapshot()
{
}

inline BOOL CSystemSnapshot::Create(DWORD dwFlags, DWORD dwProcessID/* = 0*/)
{
    ATLASSERT(IsEmpty());

    m_hHandle = ::CreateToolhelp32Snapshot(dwFlags, dwProcessID);
    return (m_hHandle != INVALID_HANDLE_VALUE);
}

inline BOOL CSystemSnapshot::HeapListFirst(HEAPLIST32& heapList) const
{
    ATLASSERT(!IsEmpty());
    return ::Heap32ListFirst(m_hHandle, &heapList);
}

inline BOOL CSystemSnapshot::HeapListNext(HEAPLIST32& heapList) const
{
    ATLASSERT(!IsEmpty());
    return ::Heap32ListNext(m_hHandle, &heapList);
}

inline BOOL CSystemSnapshot::ThreadFirst(THREADENTRY32& threadEntry) const
{
    ATLASSERT(!IsEmpty());
    return ::Thread32First(m_hHandle, &threadEntry);
}

inline BOOL CSystemSnapshot::ThreadNext(THREADENTRY32& threadEntry) const
{
    ATLASSERT(!IsEmpty());
    return ::Thread32Next(m_hHandle, &threadEntry);
}

inline BOOL CSystemSnapshot::ThreadFind(DWORD dwThreadID, THREADENTRY32& threadEntry) const
{
    BOOL bFound = ThreadFirst(threadEntry);
    for (; bFound; bFound = ThreadNext(threadEntry))
    {
        if (threadEntry.th32ThreadID == dwThreadID)
            break;
    }

    return bFound;
}

inline BOOL CSystemSnapshot::ProcessFirst(PROCESSENTRY32& processEntry) const
{
    ATLASSERT(!IsEmpty());
    return ::Process32First(m_hHandle, &processEntry);
}

inline BOOL CSystemSnapshot::ProcessNext(PROCESSENTRY32& processEntry) const
{
    ATLASSERT(!IsEmpty());
    return ::Process32Next(m_hHandle, &processEntry);
}

inline BOOL CSystemSnapshot::ProcessFind(DWORD dwProcessID, PROCESSENTRY32& processEntry) const
{
    BOOL bFound = ProcessFirst(processEntry);
    for (; bFound; bFound = ProcessNext(processEntry))
    {
        if (processEntry.th32ProcessID == dwProcessID)
            break;
    }

    return bFound;
}

inline BOOL CSystemSnapshot::ModuleFirst(MODULEENTRY32& modileEntry) const
{
    ATLASSERT(!IsEmpty());
    return ::Module32First(m_hHandle, &modileEntry);
}

inline BOOL CSystemSnapshot::ModuleNext(MODULEENTRY32& modileEntry) const
{
    ATLASSERT(!IsEmpty());
    return ::Module32Next(m_hHandle, &modileEntry);
}

inline BOOL CSystemSnapshot::ModuleFind(LPCTSTR pszModuleName, MODULEENTRY32& modileEntry) const
{
    ATLASSERT(pszModuleName);

    BOOL bFound = ModuleFirst(modileEntry);
    for (; bFound; bFound = ModuleNext(modileEntry))
    {
        if (::lstrcmpi(modileEntry.szExePath, pszModuleName) == 0 || ::lstrcmpi(modileEntry.szModule, pszModuleName) == 0)
            break;
    }

    return bFound;
}

inline BOOL CSystemSnapshot::ModuleFind(LPCVOID pvBaseAddress, MODULEENTRY32& modileEntry) const
{
    ATLASSERT(pvBaseAddress);

    BOOL bFound = ModuleFirst(modileEntry);
    for (; bFound; bFound = ModuleNext(modileEntry))
    {
        if (modileEntry.modBaseAddr == pvBaseAddress)
            break;
    }

    return bFound;
}

#ifdef _WIN32_WCE
inline BOOL CSystemSnapshot::HeapFirst(HEAPENTRY32& heapEntry, DWORD dwProcessID, ULONG_PTR ulHeapID)
{
    ATLASSERT(!IsEmpty());
    return ::Heap32First(m_hHandle, &heapEntry, dwProcessID, ulHeapID);
}

inline BOOL CSystemSnapshot::HeapNext(HEAPENTRY32& heapEntry)
{
    ATLASSERT(!IsEmpty());
    return ::Heap32Next(m_hHandle, &heapEntry);
}
#else
inline BOOL WINAPI CSystemSnapshot::HeapFirst(HEAPENTRY32& heapEntry, DWORD dwProcessID, ULONG_PTR ulHeapID)
{
    return ::Heap32First(&heapEntry, dwProcessID, ulHeapID);
}

inline BOOL WINAPI CSystemSnapshot::HeapNext(HEAPENTRY32& heapEntry)
{
    return ::Heap32Next(&heapEntry);
}
#endif  // _WIN32_WCE

inline BOOL WINAPI CSystemSnapshot::ReadProcessMemory(DWORD dwProcessID, LPCVOID pvBaseAddress, LPVOID pvBuffer, SIZE_T cbRead, SIZE_T* pNumberOfBytesRead/* = NULL*/)
{
    return ::Toolhelp32ReadProcessMemory(dwProcessID, pvBaseAddress, pvBuffer, cbRead, pNumberOfBytesRead);
}

}  // namespace sysutil

#endif  // __TOOLHELP_INL__