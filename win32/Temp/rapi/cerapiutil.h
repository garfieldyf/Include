///////////////////////////////////////////////////////////////////////////////
// cerapiutil.h : ͷ�ļ�
//
// Copyright(c) 2012, Garfield. All rights reserved.
// Author  : Garfield
// Version : 1.0
// Finish Time : 2009/10/22

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __CERAPIUTIL_H__
#define __CERAPIUTIL_H__

#ifndef RAPI_H
    #error cerapiutil.h requires rapi2.h to be included first
#endif

#include <shlwapi.h>
#include <atlcomcli.h>

///////////////////////////////////////////////////////////////////////////////
// Classes in this file:
//
// CCePropVal
// CCeDeviceInfo
// CCeSystemInfo
// CCeSystemVersion
// CCeMemoryStatus
// CCeFindFile
// CCeFindAllFiles
// CCeFindDatabase
// CCeFindAllDatabases
// IRAPISinkImpl<T>
// IDccManSink2Impl<T>
//
// Global functions in this file:
//
// CeFileSeek
// CeGetFileSize
// CeSetFileSize
// CeRegEnumKey
// CeRegEnumValue
// CeRegGetStringValue
// CeRegSetStringValue
// CeRegGetBinaryValue
// CeRegGetIntegerValue<_Ty>
// CeRegSetIntegerValue<_Ty>

namespace cerapi {

/////////////////////////////////////////////////////////////////////////////
// CCePropVal ��Ķ���
//

class CCePropVal
{
// ����/����
public:
    CCePropVal(CEPROPVAL* pPropVal = NULL);
    ~CCePropVal();

// ����
public:
    void Empty();
    CEPROPVAL* Detach();

    void operator=(CEPROPVAL* pPropVal);

// ����
public:
    BOOL IsNull() const;

    DWORD GetSize() const;
    LPBYTE GetBlob() const;

    BOOL GetBool() const;
    long GetLong() const;
    short GetShort() const;
    double GetDouble() const;
    LPCWSTR GetString() const;

    const FILETIME& GetDateTime() const;
    BOOL GetDateTime(SYSTEMTIME& datetime) const;

// ʵ��
private:
    CCePropVal(const CCePropVal& that);
    CCePropVal& operator=(const CCePropVal& that);

// ��Ա
public:
    CEPROPVAL* m_pPropVal;
};


/////////////////////////////////////////////////////////////////////////////
// CCeDeviceInfo ��Ķ���
//

class CCeDeviceInfo : public RAPI_DEVICEINFO
{
// ����/����
public:
    CCeDeviceInfo();
    ~CCeDeviceInfo();

// ����
public:
    HRESULT GetDeviceInfo(IRAPIDevice* pDevice);

// ʵ��
private:
    CCeDeviceInfo(const CCeDeviceInfo& that);
    CCeDeviceInfo& operator=(const CCeDeviceInfo& that);
};


/////////////////////////////////////////////////////////////////////////////
// CCeSystemInfo ��Ķ���
//

struct CCeSystemInfo : public SYSTEM_INFO
{
    CCeSystemInfo()
    {
        ::CeGetSystemInfo(this);
    }

    explicit CCeSystemInfo(IRAPISession* pSession)
    {
        ATLASSERT(pSession);
        pSession->CeGetSystemInfo(this);
    }
};


/////////////////////////////////////////////////////////////////////////////
// CCeSystemVersion ��Ķ���
//

struct CCeSystemVersion : public CEOSVERSIONINFO
{
    CCeSystemVersion()
    {
        dwOSVersionInfoSize = sizeof(CEOSVERSIONINFO);
    }

    BOOL GetVersion()
    {
        return ::CeGetVersionEx(this);
    }

    BOOL GetVersion(IRAPISession* pSession)
    {
        ATLASSERT(pSession);
        return pSession->CeGetVersionEx(this);
    }
};


/////////////////////////////////////////////////////////////////////////////
// CCeMemoryStatus ��Ķ���
//

struct CCeMemoryStatus : public MEMORYSTATUS
{
    CCeMemoryStatus()
    {
        dwLength = sizeof(MEMORYSTATUS);
        ::CeGlobalMemoryStatus(this);
    }

    explicit CCeMemoryStatus(IRAPISession* pSession)
    {
        ATLASSERT(pSession);

        dwLength = sizeof(MEMORYSTATUS);
        pSession->CeGlobalMemoryStatus(this);
    }
};


/////////////////////////////////////////////////////////////////////////////
// CCeFindFile ��Ķ���
//

class CCeFindFile : public CE_FIND_DATA
{
// ����/����
public:
    explicit CCeFindFile(IRAPISession* pSession);
    ~CCeFindFile();

// ����
public:
    void Close();
    operator HANDLE() const;

    BOOL FindNext();
    BOOL FindFirst(LPCWSTR pszPathName);

// ����
public:
    BOOL IsInRom() const;
    BOOL IsNormal() const;
    BOOL IsSystem() const;
    BOOL IsHidden() const;
    BOOL IsShortcut() const;
    BOOL IsReadOnly() const;
    BOOL IsArchived() const;
    BOOL IsTemporary() const;
    BOOL IsRomModule() const;
    BOOL IsDirectory() const;
    BOOL HasChildren() const;
    BOOL IsCompressed() const;
    BOOL IsRomStaticRef() const;

    LPCTSTR GetPath() const;
    LPCTSTR GetFileExt() const;
    ULONGLONG GetFileSize() const;

#if defined(__ATLSTR_H__) || defined(__AFXSTR_H__)
    CString GetFilePath() const;
    CString GetFileTitle() const;
#endif  // defined(__ATLSTR_H__) || defined(__AFXSTR_H__)

// ʵ��
private:
    CCeFindFile(const CCeFindFile& that);
    CCeFindFile& operator=(const CCeFindFile& that);

// ��Ա
public:
    HANDLE m_hFind;
    TCHAR m_szPath[MAX_PATH];
    IRAPISession* m_pSession;
};


/////////////////////////////////////////////////////////////////////////////
// CCeFindAllFiles ��Ķ���
//

class CCeFindAllFiles
{
// ����/����
public:
    explicit CCeFindAllFiles(IRAPISession* pSession);
    ~CCeFindAllFiles();

// ����
public:
    BOOL Find(LPCWSTR pszPathName, DWORD dwFlags = FAF_ATTRIBUTES | FAF_CREATION_TIME | FAF_LASTACCESS_TIME | FAF_LASTWRITE_TIME | FAF_SIZE_HIGH | FAF_SIZE_LOW | FAF_OID | FAF_NAME | FAF_ATTRIB_CHILDREN | FAF_ATTRIB_NO_HIDDEN);
    void Empty();

// ����
public:
    BOOL IsInRom(DWORD dwIndex) const;
    BOOL IsNormal(DWORD dwIndex) const;
    BOOL IsSystem(DWORD dwIndex) const;
    BOOL IsHidden(DWORD dwIndex) const;
    BOOL IsShortcut(DWORD dwIndex) const;
    BOOL IsReadOnly(DWORD dwIndex) const;
    BOOL IsArchived(DWORD dwIndex) const;
    BOOL IsTemporary(DWORD dwIndex) const;
    BOOL IsRomModule(DWORD dwIndex) const;
    BOOL IsDirectory(DWORD dwIndex) const;
    BOOL HasChildren(DWORD dwIndex) const;
    BOOL IsCompressed(DWORD dwIndex) const;
    BOOL IsRomStaticRef(DWORD dwIndex) const;

    LPCTSTR GetPath() const;
    LPCTSTR GetFileExt(DWORD dwIndex) const;
    ULONGLONG GetFileSize(DWORD dwIndex) const;

#if defined(__ATLSTR_H__) || defined(__AFXSTR_H__)
    CString GetFilePath(DWORD dwIndex) const;
    CString GetFileTitle(DWORD dwIndex) const;
#endif  // defined(__ATLSTR_H__) || defined(__AFXSTR_H__)

// ʵ��
private:
    CCeFindAllFiles(const CCeFindAllFiles& that);
    CCeFindAllFiles& operator=(const CCeFindAllFiles& that);

// ��Ա
public:
    DWORD m_dwCount;
    LPCE_FIND_DATA m_pFind;
    TCHAR m_szPath[MAX_PATH];
    IRAPISession* m_pSession;
};


/////////////////////////////////////////////////////////////////////////////
// CCeFindDatabase ��Ķ���
//

class CCeFindDatabase
{
// ����/����
public:
    explicit CCeFindDatabase(IRAPISession* pSession);
    ~CCeFindDatabase();

// ����
public:
    void Close();
    operator HANDLE() const;

    BOOL FindNext(PCEGUID pceguid = NULL);
    BOOL FindFirst(PCEGUID pceguid = NULL, DWORD dwDbaseType = 0);

// ʵ��
private:
    CCeFindDatabase(const CCeFindDatabase& that);
    CCeFindDatabase& operator=(const CCeFindDatabase& that);

// ��Ա
public:
    CEOID m_nOidDB;
    HANDLE m_hFind;
    IRAPISession* m_pSession;
};


/////////////////////////////////////////////////////////////////////////////
// CCeFindAllDatabases ��Ķ���
//

class CCeFindAllDatabases
{
// ����/����
public:
    explicit CCeFindAllDatabases(IRAPISession* pSession);
    ~CCeFindAllDatabases();

// ����
public:
    BOOL Find(DWORD dwDbaseType = 0, WORD wFlags = FAD_OID | FAD_FLAGS | FAD_NAME | FAD_TYPE | FAD_NUM_RECORDS | FAD_NUM_SORT_ORDER | FAD_SIZE | FAD_LAST_MODIFIED | FAD_SORT_SPECS);
    void Empty();

// ʵ��
private:
    CCeFindAllDatabases(const CCeFindAllDatabases& that);
    CCeFindAllDatabases& operator=(const CCeFindAllDatabases& that);

// ��Ա
public:
    WORD m_nCount;
    LPCEDB_FIND_DATA m_pFind;
    IRAPISession* m_pSession;
};


#ifdef __ATLCOMUTIL_H__
/////////////////////////////////////////////////////////////////////////////
// IRAPISinkImpl ��Ķ���
//

template <typename T>
class ATL_NO_VTABLE IRAPISinkImpl : public atlutil::IUnknownEventImpl<T, IRAPISINKID, &IID_IRAPISink>
{
// ����
public:
    HRESULT IRAPISink_Advise(IRAPIDesktop* pDesktop);
    HRESULT IRAPISink_Unadvise(IRAPIDesktop* pDesktop);

// ʵ��
public:
    STDMETHOD(OnDeviceConnected)(IRAPIDevice* pDevice);
    STDMETHOD(OnDeviceDisconnected)(IRAPIDevice* pDevice);

// ��д
public:
    HRESULT IRAPISink_OnDeviceConnected(IRAPIDevice* pDevice);
    HRESULT IRAPISink_OnDeviceDisconnected(IRAPIDevice* pDevice);
};


#ifdef _DCCOLEH_
/////////////////////////////////////////////////////////////////////////////
// IDccManSink2Impl ��Ķ���
//

template <typename T>
class ATL_NO_VTABLE IDccManSink2Impl : public atlutil::IUnknownEventImpl<T, IDCCMANSINK2ID, &IID_IDccManSink, &IID_IDccManSink2>
{
// ����
public:
    HRESULT IDccManSink2_Advise(IDccMan* pDccMan);
    HRESULT IDccManSink2_Unadvise(IDccMan* pDccMan);

// ʵ��
public:
    // IDccManSink �ӿ�
    STDMETHOD(OnLogIpAddr)(DWORD dwIPAddr);
    STDMETHOD(OnLogTerminated)();
    STDMETHOD(OnLogActive)();
    STDMETHOD(OnLogInactive)();
    STDMETHOD(OnLogAnswered)();
    STDMETHOD(OnLogListen)();
    STDMETHOD(OnLogDisconnection)();
    STDMETHOD(OnLogError)();

    // IDccManSink2 �ӿ�
    STDMETHOD(OnLogIpAddrEx)(const SOCKADDR_STORAGE* pIPAddr);

// ��д
public:
    HRESULT IDccManSink2_OnLogError();
    HRESULT IDccManSink2_OnLogActive();
    HRESULT IDccManSink2_OnLogListen();
    HRESULT IDccManSink2_OnLogInactive();
    HRESULT IDccManSink2_OnLogAnswered();
    HRESULT IDccManSink2_OnLogTerminated();
    HRESULT IDccManSink2_OnLogDisconnection();
    HRESULT IDccManSink2_OnLogIpAddr(DWORD dwIPAddr);
    HRESULT IDccManSink2_OnLogIpAddrEx(const SOCKADDR_STORAGE* pIPAddr);
};
#endif  // _DCCOLEH_
#endif  // __ATLCOMUTIL_H__


///////////////////////////////////////////////////////////////////////////////
// Global functions
//

inline LONGLONG WINAPI CeFileSeek(IRAPISession* pSession, HANDLE hFile, LONGLONG nOffset, DWORD dwFrom = FILE_CURRENT)
{
    ATLASSERT(pSession);
    ATLASSERT(hFile != INVALID_HANDLE_VALUE);
    ATLASSERT(dwFrom == FILE_BEGIN || dwFrom == FILE_END || dwFrom == FILE_CURRENT);

    LARGE_INTEGER liOffset;
    liOffset.QuadPart = nOffset;
    liOffset.LowPart  = pSession->CeSetFilePointer(hFile, liOffset.LowPart, &liOffset.HighPart, dwFrom);

    return (liOffset.LowPart == 0xFFFFFFFF && pSession->CeGetLastError() != NO_ERROR ? -1 : liOffset.QuadPart);
}

inline ULONGLONG WINAPI CeGetFileSize(IRAPISession* pSession, HANDLE hFile)
{
    ATLASSERT(pSession);
    ATLASSERT(hFile != INVALID_HANDLE_VALUE);

    ULARGE_INTEGER liFileSize;
    liFileSize.LowPart = pSession->CeGetFileSize(hFile, &liFileSize.HighPart);

    return (liFileSize.LowPart == 0xFFFFFFFF && pSession->CeGetLastError() != NO_ERROR ? (ULONGLONG)-1 : liFileSize.QuadPart);
}

inline BOOL WINAPI CeSetFileSize(IRAPISession* pSession, HANDLE hFile, ULONGLONG nFileSize)
{
    ATLASSERT(pSession);
    ATLASSERT(hFile != INVALID_HANDLE_VALUE);

    return (CeFileSeek(pSession, hFile, nFileSize, FILE_BEGIN) != -1 ? pSession->CeSetEndOfFile(hFile) : FALSE);
}

#if defined(__ATLSTR_H__) || defined(__AFXSTR_H__)
inline LONG WINAPI CeRegEnumKey(IRAPISession* pSession, HKEY hKey, DWORD dwIndex, CStringW& strName, LPWSTR pszClass = NULL, LPDWORD pClassLength = NULL, LPFILETIME pLastWriteTime = NULL)
{
    ATLASSERT(hKey);
    ATLASSERT(pSession);

    DWORD dwLength = MAX_PATH;
    LONG lRetVal = pSession->CeRegEnumKeyEx(hKey, dwIndex, strName.GetBuffer(MAX_PATH), &dwLength, NULL, pszClass, pClassLength, pLastWriteTime);
    strName.ReleaseBufferSetLength(dwLength);

    return lRetVal;
}

inline LONG WINAPI CeRegEnumValue(IRAPISession* pSession, HKEY hKey, DWORD dwIndex, CStringW& strName, LPVOID pValue = NULL, LPDWORD pcbSize = NULL, LPDWORD pType = NULL)
{
    ATLASSERT(hKey);
    ATLASSERT(pSession);

    DWORD dwLength = MAX_PATH;
    LONG lRetVal = pSession->CeRegEnumValue(hKey, dwIndex, strName.GetBuffer(MAX_PATH), &dwLength, NULL, pType, (LPBYTE)pValue, pcbSize);
    strName.ReleaseBufferSetLength(dwLength);

    return lRetVal;
}

inline LONG WINAPI CeRegGetStringValue(IRAPISession* pSession, HKEY hKey, LPCWSTR pszName, CStringW& strValue)
{
    ATLASSERT(hKey);
    ATLASSERT(pSession);

    DWORD cbSize = 0;
    LONG lRetVal = pSession->CeRegQueryValueEx(hKey, pszName, NULL, NULL, NULL, &cbSize);
    if (lRetVal == ERROR_SUCCESS)
    {
        lRetVal = pSession->CeRegQueryValueEx(hKey, pszName, NULL, NULL, (LPBYTE)strValue.GetBuffer(cbSize / sizeof(WCHAR)), &cbSize);
        strValue.ReleaseBufferSetLength(cbSize / sizeof(WCHAR));
    }

    return lRetVal;
}
#endif  // defined(__ATLSTR_H__) || defined(__AFXSTR_H__)

inline LONG WINAPI CeRegSetStringValue(IRAPISession* pSession, HKEY hKey, LPCWSTR pszName, LPCWSTR pszValue, DWORD dwType = REG_SZ)
{
    ATLASSERT(hKey);
    ATLASSERT(pSession);

    return pSession->CeRegSetValueEx(hKey, pszName, 0, dwType, (LPBYTE)pszValue, (::lstrlenW(pszValue) + 1) * sizeof(WCHAR));
}

inline LONG WINAPI GetBinaryValue(IRAPISession* pSession, HKEY hKey, LPCWSTR pszName, LPVOID pValue, DWORD cbSize)
{
    ATLASSERT(hKey);
    ATLASSERT(pValue);
    ATLASSERT(pSession);

    return pSession->CeRegQueryValueEx(hKey, pszName, NULL, NULL, (LPBYTE)pValue, &cbSize);
}

template <typename _Ty>
inline LONG CeRegGetIntegerValue(IRAPISession* pSession, HKEY hKey, LPCWSTR pszName, _Ty& _Value)
{
    ATLASSERT(hKey);
    ATLASSERT(pSession);
    ATLASSERT(sizeof(_Ty) == sizeof(DWORD));

    DWORD cbSize = sizeof(_Ty);
    return pSession->CeRegQueryValueEx(hKey, pszName, NULL, NULL, (LPBYTE)&_Value, &cbSize);
}

template <typename _Ty>
inline LONG CeRegSetIntegerValue(IRAPISession* pSession, HKEY hKey, LPCWSTR pszName, _Ty _Value, DWORD dwType = REG_DWORD)
{
    ATLASSERT(hKey);
    ATLASSERT(pSession);
    ATLASSERT(sizeof(_Ty) == sizeof(DWORD));

    return pSession->CeRegSetValueEx(hKey, pszName, 0, dwType, (LPBYTE)&_Value, sizeof(_Ty));
}

}  // namespace cerapi

#include "cerapiutil.inl"

#endif  // __CERAPIUTIL_H__