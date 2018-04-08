///////////////////////////////////////////////////////////////////////////////
// sysutil.h
//
// Copyright(c) 2018, Garfield. All rights reserved.
// Author  : Garfield
// Version : 2.0
// Creation Date : 2007/9/10

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __SYSUTIL_H__
#define __SYSUTIL_H__

#include "stddefx.h"
#include <shlwapi.h>
#include <atltypes.h>

///////////////////////////////////////////////////////////////////////////////
// Classes in this file:
//
// CSystemInfo
// CSystemTime
// CSystemVersion
// CDllVersionInfo
// CClientRect
// CWindowRect
// CMemoryStatus
// CDiskBasicInfo
// CMemoryBasicInfo
//
// Global functions in this file:
//
// GetModulePath()
// GetWindowPath()
// GetSystemPath()
// GetTempPath()
// GetDocumentPath()
// GetSpecialPath()
// IsPocketPC()
// IsSmartPhone()
// SetWindowTheme()
// IsWindowsXP()
// IsWindowsVista()
// IsThemeActive()
// IsCommCtrlVersion6()
// IsCompositionEnabled()
// CoRegisterServer()
// CoUnregisterServer()
// RegistryTodayItem()
// UnregistryTodayItem()

namespace sysutil {

///////////////////////////////////////////////////////////////////////////////
// CSystemInfo
//

struct CSystemInfo : public SYSTEM_INFO
{
    CSystemInfo()
    {
        ::GetSystemInfo(this);
    }

#ifdef _CRT_DEBUG_DUMP
    template <typename _Pr>
    void Dump(const _Pr& _Printer) const
    {
        _Printer(_T("[ SYSTEM INFO ]\n[\n\tNumber Of Processors        = %u\n\tProcessor Architecture      = %d\n\tProcessor Type              = %u\n\t\
                    Processor Level             = %u\n\tProcessor Revision          = 0x%08lx\n\tActive Processor Mask       = 0x%08lx\n\t\
                    Page Size                   = %u\n\tAllocation Granularity      = %u\n\tMinimum Application Address = 0x%p\n\tMaximum Application Address \
                    = 0x%p\n]\n"), dwNumberOfProcessors, wProcessorArchitecture, dwProcessorType, wProcessorLevel, wProcessorRevision, dwActiveProcessorMask,
                    dwPageSize, dwAllocationGranularity, lpMinimumApplicationAddress, lpMaximumApplicationAddress);
    }
#endif  // _CRT_DEBUG_DUMP
};


///////////////////////////////////////////////////////////////////////////////
// CSystemTime
//

struct CSystemTime : public SYSTEMTIME
{
    CSystemTime()
    {
        ::GetLocalTime(this);
    }

#ifdef _CRT_DEBUG_DUMP
    template <typename _Pr>
    void Dump(const _Pr& _Printer) const
    {
        _Printer(_T("[ %04d/%02d/%02d %02d:%02d:%02d.%03d Week = %d ]\n"), wYear, wMonth, wDay, wHour, wMinute, wSecond, wMilliseconds, wDayOfWeek);
    }
#endif  // _CRT_DEBUG_DUMP
};


///////////////////////////////////////////////////////////////////////////////
// CSystemVersion
//

#ifndef _WIN32_WCE
struct CSystemVersion : public OSVERSIONINFOEX
{
    CSystemVersion()
    {
        dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
        ATLVERIFY(::GetVersionEx((LPOSVERSIONINFO)this));
    }

#ifdef _CRT_DEBUG_DUMP
    template <typename _Pr>
    void Dump(const _Pr& _Printer) const
    {
        _Printer(_T("[ OS VERSION INFO ]\n[\n\tOS Version           = %u.%u\n\tService Pack         = %s\n\tService Pack Version = %u.%u\n\t\
                    Platform Id          = %u\n\tBuild Number         = %u\n\tProduct Type         = %u\n\tSuite Mask           = 0x%04x\n]\n"), dwMajorVersion,
                    dwMinorVersion, szCSDVersion, wServicePackMajor, wServicePackMinor, dwPlatformId, dwBuildNumber, wProductType, wSuiteMask);
    }
#endif  // _CRT_DEBUG_DUMP
};
#else
struct CSystemVersion : public OSVERSIONINFO
{
    CSystemVersion()
    {
        dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
        ATLVERIFY(::GetVersionEx(this));
    }
};
#endif  // _WIN32_WCE


///////////////////////////////////////////////////////////////////////////////
// CDllVersionInfo
//

struct CDllVersionInfo : public DLLVERSIONINFO
{
    CDllVersionInfo()
    {
        cbSize = sizeof(DLLVERSIONINFO);
    }

    BOOL GetVersion(LPCTSTR pszDllName)
    {
        ATLASSERT(pszDllName);
        BOOL bSuccessful = FALSE;

        if (HINSTANCE hInstance = ::LoadLibrary(pszDllName))
        {
        #ifndef _WIN32_WCE
            if (DLLGETVERSIONPROC pfnDllGetVersion = (DLLGETVERSIONPROC)::GetProcAddress(hInstance, "DllGetVersion"))
                bSuccessful = SUCCEEDED(pfnDllGetVersion(this));
        #else
            if (DLLGETVERSIONPROC pfnDllGetVersion = (DLLGETVERSIONPROC)::GetProcAddress(hInstance, L"DllGetVersion"))
                bSuccessful = SUCCEEDED(pfnDllGetVersion(this));
        #endif  // _WIN32_WCE

            ATLVERIFY(::FreeLibrary(hInstance));
        }

        return bSuccessful;
    }
};


///////////////////////////////////////////////////////////////////////////////
// CClientRect
//

struct CClientRect : public CRect
{
    explicit CClientRect(HWND hWnd)
    {
        ATLASSERT(::IsWindow(hWnd));
        ATLVERIFY(::GetClientRect(hWnd, this));
    }

    BOOL Get(HWND hWnd)
    {
        ATLASSERT(::IsWindow(hWnd));
        return ::GetClientRect(hWnd, this);
    }
};


///////////////////////////////////////////////////////////////////////////////
// CWindowRect
//

struct CWindowRect : public CRect
{
    explicit CWindowRect(HWND hWnd)
    {
        ATLASSERT(::IsWindow(hWnd));
        ATLVERIFY(::GetWindowRect(hWnd, this));
    }

    BOOL Get(HWND hWnd)
    {
        ATLASSERT(::IsWindow(hWnd));
        return ::GetWindowRect(hWnd, this);
    }
};


///////////////////////////////////////////////////////////////////////////////
// CMemoryStatus
//

#ifndef _WIN32_WCE
struct CMemoryStatus : public MEMORYSTATUSEX
{
    CMemoryStatus()
    {
        dwLength = sizeof(MEMORYSTATUSEX);
        ATLVERIFY(::GlobalMemoryStatusEx(this));
    }

    LPTSTR FormatTotalPhysSize(LPTSTR pszBuffer, UINT cchBuffer) const
    {
        ATLASSERT(pszBuffer);
        return ::StrFormatByteSize64((LONGLONG)ullTotalPhys, pszBuffer, cchBuffer);
    }

    LPTSTR FormatTotalPhysKBSize(LPTSTR pszBuffer, UINT cchBuffer) const
    {
        ATLASSERT(pszBuffer);
        return ::StrFormatKBSize((LONGLONG)ullTotalPhys, pszBuffer, cchBuffer);
    }

    LPTSTR FormatAvailPhysSize(LPTSTR pszBuffer, UINT cchBuffer) const
    {
        ATLASSERT(pszBuffer);
        return ::StrFormatByteSize64((LONGLONG)ullAvailPhys, pszBuffer, cchBuffer);
    }

    LPTSTR FormatAvailPhysKBSize(LPTSTR pszBuffer, UINT cchBuffer) const
    {
        ATLASSERT(pszBuffer);
        return ::StrFormatKBSize((LONGLONG)ullAvailPhys, pszBuffer, cchBuffer);
    }

    LPTSTR FormatTotalVirtualSize(LPTSTR pszBuffer, UINT cchBuffer) const
    {
        ATLASSERT(pszBuffer);
        return ::StrFormatByteSize64((LONGLONG)ullTotalVirtual, pszBuffer, cchBuffer);
    }

    LPTSTR FormatTotalVirtualKBSize(LPTSTR pszBuffer, UINT cchBuffer) const
    {
        ATLASSERT(pszBuffer);
        return ::StrFormatKBSize((LONGLONG)ullTotalVirtual, pszBuffer, cchBuffer);
    }

    LPTSTR FormatAvailVirtualSize(LPTSTR pszBuffer, UINT cchBuffer) const
    {
        ATLASSERT(pszBuffer);
        return ::StrFormatByteSize64((LONGLONG)ullAvailVirtual, pszBuffer, cchBuffer);
    }

    LPTSTR FormatAvailVirtualKBSize(LPTSTR pszBuffer, UINT cchBuffer) const
    {
        ATLASSERT(pszBuffer);
        return ::StrFormatKBSize((LONGLONG)ullAvailVirtual, pszBuffer, cchBuffer);
    }

#ifdef _CRT_DEBUG_DUMP
    template <typename _Pr>
    void Dump(const _Pr& _Printer) const
    {
        TCHAR szTotalPhys[64], szAvailPhys[64], szTotalVirtual[64], szAvailVirtual[64];
        FormatTotalPhysSize(szTotalPhys, _countof(szTotalPhys));
        FormatAvailPhysSize(szAvailPhys, _countof(szAvailPhys));
        FormatTotalVirtualSize(szTotalVirtual, _countof(szTotalVirtual));
        FormatAvailVirtualSize(szAvailVirtual, _countof(szAvailVirtual));

        _Printer(_T("[ MEMORY STATUS INFO ]\n[\n\tTotal Physical Size     = %s\n\tAvailable Physical Size = %s\n\tTotal Virtual Size      = %s\n\tAvailable Virtual Size  = %s\n]\n"),
                 szTotalPhys, szAvailPhys, szTotalVirtual, szAvailVirtual);
    }
#endif  // _CRT_DEBUG_DUMP
};
#else
struct CMemoryStatus : public MEMORYSTATUS
{
    CMemoryStatus()
    {
        dwLength = sizeof(MEMORYSTATUS);
        ::GlobalMemoryStatus(this);
    }
};
#endif  // _WIN32_WCE


///////////////////////////////////////////////////////////////////////////////
// CDiskBasicInfo
//

struct CDiskBasicInfo
{
    explicit CDiskBasicInfo(LPCTSTR pszDirectoryName = NULL)
    {
        FreeBytesAvailable.QuadPart = TotalNumberOfBytes.QuadPart = TotalNumberOfFreeBytes.QuadPart = 0;
        ATLVERIFY(::GetDiskFreeSpaceEx(pszDirectoryName, &FreeBytesAvailable, &TotalNumberOfBytes, &TotalNumberOfFreeBytes));
    }

    BOOL Load(LPCTSTR pszDirectoryName)
    {
        return ::GetDiskFreeSpaceEx(pszDirectoryName, &FreeBytesAvailable, &TotalNumberOfBytes, &TotalNumberOfFreeBytes);
    }

#ifndef _WIN32_WCE
    LPTSTR FormatTotalSize(LPTSTR pszBuffer, UINT cchBuffer) const
    {
        ATLASSERT(pszBuffer);
        return ::StrFormatByteSize64((LONGLONG)TotalNumberOfBytes.QuadPart, pszBuffer, cchBuffer);
    }

    LPTSTR FormatTotalKBSize(LPTSTR pszBuffer, UINT cchBuffer) const
    {
        ATLASSERT(pszBuffer);
        return ::StrFormatKBSize((LONGLONG)TotalNumberOfBytes.QuadPart, pszBuffer, cchBuffer);
    }

    LPTSTR FormatTotalFreeSize(LPTSTR pszBuffer, UINT cchBuffer) const
    {
        ATLASSERT(pszBuffer);
        return ::StrFormatByteSize64((LONGLONG)TotalNumberOfFreeBytes.QuadPart, pszBuffer, cchBuffer);
    }

    LPTSTR FormatTotalFreeKBSize(LPTSTR pszBuffer, UINT cchBuffer) const
    {
        ATLASSERT(pszBuffer);
        return ::StrFormatKBSize((LONGLONG)TotalNumberOfFreeBytes.QuadPart, pszBuffer, cchBuffer);
    }

    LPTSTR FormatFreeAvailableSize(LPTSTR pszBuffer, UINT cchBuffer) const
    {
        ATLASSERT(pszBuffer);
        return ::StrFormatByteSize64((LONGLONG)FreeBytesAvailable.QuadPart, pszBuffer, cchBuffer);
    }

    LPTSTR FormatFreeAvailableKBSize(LPTSTR pszBuffer, UINT cchBuffer) const
    {
        ATLASSERT(pszBuffer);
        return ::StrFormatKBSize((LONGLONG)FreeBytesAvailable.QuadPart, pszBuffer, cchBuffer);
    }
#endif  // _WIN32_WCE

    ULARGE_INTEGER FreeBytesAvailable;
    ULARGE_INTEGER TotalNumberOfBytes;
    ULARGE_INTEGER TotalNumberOfFreeBytes;
};


///////////////////////////////////////////////////////////////////////////////
// CMemoryBasicInfo
//

struct CMemoryBasicInfo : public MEMORY_BASIC_INFORMATION
{
    explicit CMemoryBasicInfo(LPCVOID pAddress = NULL)
    {
        ATLVERIFY(::VirtualQuery(pAddress, this, sizeof(MEMORY_BASIC_INFORMATION)));
    }

    SIZE_T Query(LPCVOID pAddress)
    {
        return ::VirtualQuery(pAddress, this, sizeof(MEMORY_BASIC_INFORMATION));
    }
};


///////////////////////////////////////////////////////////////////////////////
// Global functions
//

inline LPCTSTR WINAPI GetModulePath()
{
    static TCHAR szPath[MAX_PATH];
    if (szPath[0] == _T('\0'))
    {
        if (DWORD dwLength = ::GetModuleFileName(NULL, szPath, _countof(szPath)))
            *(::_tsstrrchr(szPath, szPath + dwLength, _T('\\'))) = _T('\0');
    }

    return szPath;
}

#if defined(__ATLSTR_H__) || defined(__AFXSTR_H__)
inline CString WINAPI GetModulePath(HINSTANCE hInstance)
{
    CString strPath;
    LPTSTR pszBuffer = strPath.GetBuffer(MAX_PATH);
    ATLASSERT(pszBuffer);

    DWORD dwLength = ::GetModuleFileName(hInstance, pszBuffer, MAX_PATH);
    strPath.ReleaseBuffer((dwLength ? ::_tsstrrchr(pszBuffer, pszBuffer + dwLength, _T('\\')) : pszBuffer) - pszBuffer);

    return strPath;
}
#endif  // defined(__ATLSTR_H__) || defined(__AFXSTR_H__)

inline LPCTSTR WINAPI GetWindowPath()
{
    static TCHAR szPath[MAX_PATH];
    if (szPath[0] == _T('\0'))
        ATLVERIFY(::SHGetSpecialFolderPath(NULL, szPath, CSIDL_WINDOWS, FALSE));

    return szPath;
}

#ifndef _WIN32_WCE
inline LPCTSTR WINAPI GetSystemPath()
{
    static TCHAR szPath[MAX_PATH];
    if (szPath[0] == _T('\0'))
        ATLVERIFY(::SHGetSpecialFolderPath(NULL, szPath, CSIDL_SYSTEM, FALSE));

    return szPath;
}
#endif  // _WIN32_WCE

inline LPCTSTR WINAPI GetTempPath()
{
    static TCHAR szPath[MAX_PATH];
    if (szPath[0] == _T('\0'))
        ::GetTempPath(MAX_PATH, szPath);

    return szPath;
}

inline LPCTSTR WINAPI GetDocumentPath()
{
    static TCHAR szPath[MAX_PATH];
    if (szPath[0] == _T('\0'))
        ATLVERIFY(::SHGetSpecialFolderPath(NULL, szPath, CSIDL_PERSONAL, FALSE));

    return szPath;
}

#if defined(__ATLSTR_H__) || defined(__AFXSTR_H__)
inline CString WINAPI GetSpecialPath(int nFolder, BOOL bCreate = FALSE, HWND hWnd = NULL)
{
    CString strPath;
    ATLVERIFY(::SHGetSpecialFolderPath(hWnd, strPath.GetBuffer(MAX_PATH), nFolder, bCreate));
    strPath.ReleaseBuffer();

    return strPath;
}
#endif  // defined(__ATLSTR_H__) || defined(__AFXSTR_H__)

#ifdef _WIN32_WCE
inline BOOL WINAPI IsPocketPC()
{
    static BOOL bPocketPC = -1;
    if (bPocketPC == -1)
    {
        TCHAR szPlatform[64];
        ATLVERIFY(::SystemParametersInfo(SPI_GETPLATFORMTYPE, _countof(szPlatform), szPlatform, 0));
        bPocketPC = (::lstrcmpi(szPlatform, _T("PocketPC")) == 0);
    }

    return bPocketPC;
}

inline BOOL WINAPI IsSmartPhone()
{
    static BOOL bSmartPhone = -1;
    if (bSmartPhone == -1)
    {
        TCHAR szPlatform[64];
        ATLVERIFY(::SystemParametersInfo(SPI_GETPLATFORMTYPE, _countof(szPlatform), szPlatform, 0));
        bSmartPhone = (::lstrcmpi(szPlatform, _T("SmartPhone")) == 0);
    }

    return bSmartPhone;
}
#else
inline HRESULT WINAPI SetWindowTheme(HWND hWnd, LPCWSTR pszSubAppName, LPCWSTR pszSubIdList = NULL)
{
    HRESULT hr = E_NOTIMPL;
    if (HINSTANCE hInstance = ::LoadLibrary(_T("uxtheme.dll")))
    {
        if (HRESULT (STDAPICALLTYPE* pfnSetWindowTheme)(HWND, LPCWSTR, LPCWSTR) = (HRESULT (STDAPICALLTYPE*)(HWND, LPCWSTR, LPCWSTR))::GetProcAddress(hInstance, "SetWindowTheme"))
            hr = pfnSetWindowTheme(hWnd, pszSubAppName, pszSubIdList);

        ATLVERIFY(::FreeLibrary(hInstance));
    }

    return hr;
}

inline BOOL WINAPI IsWindowsXP()
{
    static BOOL bResult = -1;
    if (bResult == -1)
    {
        CSystemVersion version;
        bResult = (version.dwMajorVersion > 5 || (version.dwMajorVersion == 5 && version.dwMinorVersion >= 1));
    }

    return bResult;
}

inline BOOL WINAPI IsWindowsVista()
{
    static BOOL bResult = -1;
    if (bResult == -1)
    {
        CSystemVersion version;
        bResult = (version.dwMajorVersion >= 6);
    }

    return bResult;
}

inline BOOL WINAPI IsThemeActive()
{
    BOOL bThemeActive = FALSE;
    if (HINSTANCE hInstance = ::LoadLibrary(_T("uxtheme.dll")))
    {
        if (BOOL (STDAPICALLTYPE* pfnIsThemeActive)() = (BOOL (STDAPICALLTYPE*)())::GetProcAddress(hInstance, "IsThemeActive"))
            bThemeActive = pfnIsThemeActive();

        ATLVERIFY(::FreeLibrary(hInstance));
    }

    return bThemeActive;
}

inline BOOL WINAPI IsCommCtrlVersion6()
{
    static BOOL bResult = -1;
    if (bResult == -1)
    {
        CDllVersionInfo dvi;
        ATLVERIFY(dvi.GetVersion(_T("comctl32.dll")));
        bResult = (dvi.dwMajorVersion >= 6);
    }

    return bResult;
}

inline BOOL WINAPI IsCompositionEnabled()
{
    BOOL bEnabled = FALSE;
    if (HINSTANCE hInstance = ::LoadLibrary(_T("dwmapi.dll")))
    {
        if (HRESULT (STDAPICALLTYPE* pfnDwmIsCompositionEnabled)(BOOL*) = (HRESULT (STDAPICALLTYPE*)(BOOL*))::GetProcAddress(hInstance, "DwmIsCompositionEnabled"))
            pfnDwmIsCompositionEnabled(&bEnabled);

        ATLVERIFY(::FreeLibrary(hInstance));
    }

    return bEnabled;
}
#endif  // _WIN32_WCE

inline HRESULT WINAPI CoRegisterServer(LPCTSTR pszModulePath, LPCTSTR pszClsID, LPCTSTR pszProgID = NULL, LPCTSTR pszThreadingModel = NULL)
{
    ATLASSERT(pszClsID);
    ATLASSERT(pszModulePath);

    LONG lResult = 0L;
    HKEY hKey = NULL, hSubKey = NULL;

    __try
    {
        TCHAR szSubKey[MAX_PATH];
        ::_tssprintf(szSubKey, _countof(szSubKey), _T("CLSID\\%s"), pszClsID);

        if ((lResult = ::RegCreateKeyEx(HKEY_CLASSES_ROOT, szSubKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, NULL)) != ERROR_SUCCESS)
            __leave;

        if (pszProgID != NULL && (lResult = ::RegSetValueEx(hKey, NULL, 0, REG_SZ, (LPBYTE)pszProgID, (lstrlen(pszProgID) + 1) * sizeof(TCHAR))) != ERROR_SUCCESS)
            __leave;

        if ((lResult = ::RegCreateKeyEx(hKey, _T("InprocServer32"), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hSubKey, NULL)) != ERROR_SUCCESS)
            __leave;

        if ((lResult = ::RegSetValueEx(hSubKey, NULL, 0, REG_SZ, (LPBYTE)pszModulePath, (lstrlen(pszModulePath) + 1) * sizeof(TCHAR))) != ERROR_SUCCESS)
            __leave;

        if (pszThreadingModel != NULL)
            lResult = ::RegSetValueEx(hSubKey, _T("ThreadingModel"), 0, REG_SZ, (LPBYTE)pszThreadingModel, (lstrlen(pszThreadingModel) + 1) * sizeof(TCHAR));
    }
    __finally
    {
        ::RegCloseKey(hKey);
        ::RegCloseKey(hSubKey);
    }

    return HRESULT_FROM_WIN32(lResult);
}

inline HRESULT WINAPI CoUnregisterServer(LPCTSTR pszClsID)
{
    ATLASSERT(pszClsID);

    TCHAR szSubKey[MAX_PATH];
    ::_tssprintf(szSubKey, _countof(szSubKey), _T("CLSID\\%s\\InprocServer32"), pszClsID);

    LONG lResult = ::RegDeleteKey(HKEY_CLASSES_ROOT, szSubKey);
    if (lResult == ERROR_SUCCESS)
    {
        (*_tcsrchr(szSubKey, _T('\\'))) = _T('\0');
        lResult = ::RegDeleteKey(HKEY_CLASSES_ROOT, szSubKey);
    }

    return HRESULT_FROM_WIN32(lResult);
}

#ifdef _TODAYCMN_H_
inline BOOL WINAPI RegistryTodayItem(LPCTSTR pszName, LPCTSTR pszModulePath, DWORD dwSelectability = TODAY_SELECTABILITY_CUSTOMSELECTION, BOOL bEnabled = TRUE, BOOL bOptions = FALSE)
{
    ATLASSERT(pszName);
    ATLASSERT(pszModulePath);

    TCHAR szSubKey[MAX_PATH];
    ::_tssprintf(szSubKey, _countof(szSubKey), _T("Software\\Microsoft\\Today\\Items\\%s"), pszName);

    HKEY hKey = NULL;
    BOOL bSuccessful = FALSE;

    if (::RegCreateKeyEx(HKEY_LOCAL_MACHINE, szSubKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, NULL) == ERROR_SUCCESS)
    {
        const DWORD dwType = 4;
        _COND_VERIFY(::RegSetValueEx(hKey, _T("Type"), 0, REG_DWORD, (LPBYTE)&dwType, sizeof(DWORD)), ERROR_SUCCESS);
        _COND_VERIFY(::RegSetValueEx(hKey, _T("Enabled"), 0, REG_DWORD, (LPBYTE)&bEnabled, sizeof(DWORD)), ERROR_SUCCESS);
        _COND_VERIFY(::RegSetValueEx(hKey, _T("Options"), 0, REG_DWORD, (LPBYTE)&bOptions, sizeof(DWORD)), ERROR_SUCCESS);
        _COND_VERIFY(::RegSetValueEx(hKey, _T("Selectability"), 0, REG_DWORD, (LPBYTE)&dwSelectability, sizeof(DWORD)), ERROR_SUCCESS);
        _COND_VERIFY(::RegSetValueEx(hKey, _T("DLL"), 0, REG_SZ, (LPBYTE)pszModulePath, (lstrlen(pszModulePath) + 1) * sizeof(TCHAR)), ERROR_SUCCESS);

        _COND_VERIFY(::RegCloseKey(hKey), ERROR_SUCCESS);
        bSuccessful = ::PostMessage(HWND_BROADCAST, WM_WININICHANGE, 0xF2, 0L);
    }

    return bSuccessful;
}

inline BOOL WINAPI UnregistryTodayItem(LPCTSTR pszName)
{
    ATLASSERT(pszName);

    TCHAR szSubKey[MAX_PATH];
    ::_tssprintf(szSubKey, _countof(szSubKey), _T("Software\\Microsoft\\Today\\Items\\%s"), pszName);

    BOOL bSuccessful = FALSE;
    if (::RegDeleteKey(HKEY_LOCAL_MACHINE, szSubKey) == ERROR_SUCCESS)
        bSuccessful = ::PostMessage(HWND_BROADCAST, WM_WININICHANGE, 0xF2, 0L);

    return bSuccessful;
}
#endif  // _TODAYCMN_H_

}  // namespace sysutil

#endif  // __SYSUTIL_H__