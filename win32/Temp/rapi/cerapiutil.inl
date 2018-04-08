///////////////////////////////////////////////////////////////////////////////
// cerapiutil.inl : 实现文件
//
// Copyright(c) 2012, Garfield. All rights reserved.
// Author  : Garfield
// Version : 1.0
// Finish Time : 2009/10/22

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __CERAPIUTIL_INL__
#define __CERAPIUTIL_INL__

namespace cerapi {

/////////////////////////////////////////////////////////////////////////////
// CCePropVal 类的实现
//

inline CCePropVal::CCePropVal(CEPROPVAL* pPropVal/* = NULL*/)
    : m_pPropVal(pPropVal)
{
}

inline CCePropVal::~CCePropVal()
{
    Empty();
}

inline void CCePropVal::Empty()
{
    if (m_pPropVal != NULL && ::LocalFree(m_pPropVal) == NULL)
        m_pPropVal = NULL;
}

inline CEPROPVAL* CCePropVal::Detach()
{
    CEPROPVAL* pPropVal = m_pPropVal;
    m_pPropVal = NULL;

    return pPropVal;
}

inline void CCePropVal::operator=(CEPROPVAL* pPropVal)
{
    Empty();
    m_pPropVal = pPropVal;
}

inline BOOL CCePropVal::IsNull() const
{
    return (m_pPropVal == NULL || m_pPropVal->wFlags == CEDB_PROPNOTFOUND);
}

inline DWORD CCePropVal::GetSize() const
{
    ATLASSERT(m_pPropVal);
    ATLASSERT(m_pPropVal->wFlags != CEDB_PROPNOTFOUND && LOWORD(m_pPropVal->propid) == CEVT_BLOB);

    return m_pPropVal->val.blob.dwCount;
}

inline LPBYTE CCePropVal::GetBlob() const
{
    ATLASSERT(m_pPropVal);
    ATLASSERT(m_pPropVal->wFlags != CEDB_PROPNOTFOUND && LOWORD(m_pPropVal->propid) == CEVT_BLOB);

    return m_pPropVal->val.blob.lpb;
}

inline BOOL CCePropVal::GetBool() const
{
    ATLASSERT(m_pPropVal);
    ATLASSERT(m_pPropVal->wFlags != CEDB_PROPNOTFOUND && LOWORD(m_pPropVal->propid) == CEVT_BOOL);

    return m_pPropVal->val.boolVal;
}

inline long CCePropVal::GetLong() const
{
    ATLASSERT(m_pPropVal);
    ATLASSERT(m_pPropVal->wFlags != CEDB_PROPNOTFOUND);
    ATLASSERT(LOWORD(m_pPropVal->propid) == CEVT_I4 || LOWORD(m_pPropVal->propid) == CEVT_UI4 || LOWORD(m_pPropVal->propid) == CEVT_PIM_AUTO_I4);

    return m_pPropVal->val.lVal;
}

inline short CCePropVal::GetShort() const
{
    ATLASSERT(m_pPropVal);
    ATLASSERT(m_pPropVal->wFlags != CEDB_PROPNOTFOUND);
    ATLASSERT(LOWORD(m_pPropVal->propid) == CEVT_I2 || LOWORD(m_pPropVal->propid) == CEVT_UI2);

    return m_pPropVal->val.iVal;
}

inline double CCePropVal::GetDouble() const
{
    ATLASSERT(m_pPropVal);
    ATLASSERT(m_pPropVal->wFlags != CEDB_PROPNOTFOUND && LOWORD(m_pPropVal->propid) == CEVT_R8);

    return m_pPropVal->val.dblVal;
}

inline LPCWSTR CCePropVal::GetString() const
{
    ATLASSERT(m_pPropVal);
    ATLASSERT(m_pPropVal->wFlags != CEDB_PROPNOTFOUND && LOWORD(m_pPropVal->propid) == CEVT_LPWSTR);

    return m_pPropVal->val.lpwstr;
}

inline const FILETIME& CCePropVal::GetDateTime() const
{
    ATLASSERT(m_pPropVal);
    ATLASSERT(m_pPropVal->wFlags != CEDB_PROPNOTFOUND && LOWORD(m_pPropVal->propid) == CEVT_FILETIME);

    return m_pPropVal->val.filetime;
}

inline BOOL CCePropVal::GetDateTime(SYSTEMTIME& datetime) const
{
    ATLASSERT(m_pPropVal);
    ATLASSERT(m_pPropVal->wFlags != CEDB_PROPNOTFOUND && LOWORD(m_pPropVal->propid) == CEVT_FILETIME);

    return ::FileTimeToSystemTime(&m_pPropVal->val.filetime, &datetime);
}


/////////////////////////////////////////////////////////////////////////////
// CCeDeviceInfo 类的实现
//

inline CCeDeviceInfo::CCeDeviceInfo()
{
    bstrName = bstrPlatform = NULL;
}

inline CCeDeviceInfo::~CCeDeviceInfo()
{
    ::SysFreeString(bstrName);
    ::SysFreeString(bstrPlatform);
}

inline HRESULT CCeDeviceInfo::GetDeviceInfo(IRAPIDevice* pDevice)
{
    ATLASSERT(pDevice);
    ATLASSERT(bstrName == NULL);
    ATLASSERT(bstrPlatform == NULL);

    return pDevice->GetDeviceInfo(this);
}


/////////////////////////////////////////////////////////////////////////////
// CCeFindFile 类的实现
//

inline CCeFindFile::CCeFindFile(IRAPISession* pSession)
    : m_hFind(INVALID_HANDLE_VALUE), m_pSession(pSession)
{
    ATLASSERT(pSession);
    m_szPath[0] = _T('\0');
}

inline CCeFindFile::~CCeFindFile()
{
    Close();
}

inline void CCeFindFile::Close()
{
    ATLASSERT(m_pSession);

    if (m_hFind != INVALID_HANDLE_VALUE && m_pSession->CeFindClose(m_hFind))
        m_hFind = INVALID_HANDLE_VALUE;
}

inline CCeFindFile::operator HANDLE() const
{
    return m_hFind;
}

inline BOOL CCeFindFile::FindNext()
{
    ATLASSERT(m_pSession);
    ATLASSERT(m_hFind != INVALID_HANDLE_VALUE);

    return m_pSession->CeFindNextFile(m_hFind, this);
}

inline BOOL CCeFindFile::FindFirst(LPCWSTR pszPathName)
{
    ATLASSERT(m_pSession);
    ATLASSERT(pszPathName);
    ATLASSERT(m_hFind == INVALID_HANDLE_VALUE);
    ATLASSERT(::StrRChr(pszPathName, NULL, _T('\\')) != NULL);

    BOOL bSuccessful = ((m_hFind = m_pSession->CeFindFirstFile(pszPathName, this)) != INVALID_HANDLE_VALUE);
    if (bSuccessful)
        ::lstrcpyn(m_szPath, pszPathName, ::StrRChr(pszPathName, NULL, _T('\\')) - pszPathName);

    return bSuccessful;
}

inline BOOL CCeFindFile::IsInRom() const
{
    return (dwFileAttributes & FILE_ATTRIBUTE_INROM);
}

inline BOOL CCeFindFile::IsNormal() const
{
    return (dwFileAttributes & FILE_ATTRIBUTE_NORMAL);
}

inline BOOL CCeFindFile::IsSystem() const
{
    return (dwFileAttributes & FILE_ATTRIBUTE_SYSTEM);
}

inline BOOL CCeFindFile::IsHidden() const
{
    return (dwFileAttributes & FILE_ATTRIBUTE_HIDDEN);
}

inline BOOL CCeFindFile::IsShortcut() const
{
    return (dwFileAttributes & FILE_ATTRIBUTE_SHORTCUT);
}

inline BOOL CCeFindFile::IsReadOnly() const
{
    return (dwFileAttributes & FILE_ATTRIBUTE_READONLY);
}

inline BOOL CCeFindFile::IsArchived() const
{
    return (dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE);
}

inline BOOL CCeFindFile::IsTemporary() const
{
    return (dwFileAttributes & FILE_ATTRIBUTE_TEMPORARY);
}

inline BOOL CCeFindFile::IsRomModule() const
{
    return (dwFileAttributes & FILE_ATTRIBUTE_ROMMODULE);
}

inline BOOL CCeFindFile::IsDirectory() const
{
    return (dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY);
}

inline BOOL CCeFindFile::HasChildren() const
{
    return (dwFileAttributes & FILE_ATTRIBUTE_HAS_CHILDREN);
}

inline BOOL CCeFindFile::IsCompressed() const
{
    return (dwFileAttributes & FILE_ATTRIBUTE_COMPRESSED);
}

inline BOOL CCeFindFile::IsRomStaticRef() const
{
    return (dwFileAttributes & FILE_ATTRIBUTE_ROMSTATICREF);
}

inline LPCTSTR CCeFindFile::GetPath() const
{
    return (m_szPath[0] == _T('\0') ? _T("\\") : m_szPath);
}

inline LPCTSTR CCeFindFile::GetFileExt() const
{
    return ::PathFindExtension(cFileName);
}

inline ULONGLONG CCeFindFile::GetFileSize() const
{
    return ((ULONGLONG)(nFileSizeLow | (((ULONGLONG)nFileSizeHigh) << 32)));
}

#if defined(__ATLSTR_H__) || defined(__AFXSTR_H__)
inline CString CCeFindFile::GetFilePath() const
{
    CString strFilePath;
    strFilePath.Format(_T("%s\\%s"), m_szPath, cFileName);

    return strFilePath;
}

inline CString CCeFindFile::GetFileTitle() const
{
    return CString(cFileName, ::PathFindExtension(cFileName) - cFileName);
}
#endif  // defined(__ATLSTR_H__) || defined(__AFXSTR_H__)


/////////////////////////////////////////////////////////////////////////////
// CCeFindAllFiles 类的实现
//

inline CCeFindAllFiles::CCeFindAllFiles(IRAPISession* pSession)
    : m_pFind(NULL), m_dwCount(0), m_pSession(pSession)
{
    ATLASSERT(pSession);
    m_szPath[0] = _T('\0');
}

inline CCeFindAllFiles::~CCeFindAllFiles()
{
    Empty();
}

inline BOOL CCeFindAllFiles::Find(LPCWSTR pszPathName, DWORD dwFlags/* = FAF_ATTRIBUTES | FAF_CREATION_TIME | FAF_LASTACCESS_TIME | FAF_LASTWRITE_TIME | FAF_SIZE_HIGH | FAF_SIZE_LOW | FAF_OID | FAF_NAME | FAF_ATTRIB_CHILDREN | FAF_ATTRIB_NO_HIDDEN*/)
{
    ATLASSERT(m_pSession);
    ATLASSERT(pszPathName);
    ATLASSERT(::StrRChr(pszPathName, NULL, _T('\\')) != NULL);
    Empty();

    BOOL bSuccessful = m_pSession->CeFindAllFiles(pszPathName, dwFlags, &m_dwCount, &m_pFind);
    if (bSuccessful)
        ::lstrcpyn(m_szPath, pszPathName, ::StrRChr(pszPathName, NULL, _T('\\')) - pszPathName);

    return bSuccessful;
}

inline void CCeFindAllFiles::Empty()
{
    ATLASSERT(m_pSession);

    if (m_pFind != NULL && SUCCEEDED(m_pSession->CeRapiFreeBuffer(m_pFind)))
        m_pFind = NULL, m_dwCount = 0;
}

inline BOOL CCeFindAllFiles::IsInRom(DWORD dwIndex) const
{
    ATLASSERT(m_pFind);
    ATLASSERT(dwIndex < m_dwCount);

    return ((m_pFind[dwIndex].dwFileAttributes & FILE_ATTRIBUTE_INROM) != 0);
}

inline BOOL CCeFindAllFiles::IsNormal(DWORD dwIndex) const
{
    ATLASSERT(m_pFind);
    ATLASSERT(dwIndex < m_dwCount);

    return ((m_pFind[dwIndex].dwFileAttributes & FILE_ATTRIBUTE_NORMAL) != 0);
}

inline BOOL CCeFindAllFiles::IsSystem(DWORD dwIndex) const
{
    ATLASSERT(m_pFind);
    ATLASSERT(dwIndex < m_dwCount);

    return ((m_pFind[dwIndex].dwFileAttributes & FILE_ATTRIBUTE_SYSTEM) != 0);
}

inline BOOL CCeFindAllFiles::IsHidden(DWORD dwIndex) const
{
    ATLASSERT(m_pFind);
    ATLASSERT(dwIndex < m_dwCount);

    return ((m_pFind[dwIndex].dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) != 0);
}

inline BOOL CCeFindAllFiles::IsShortcut(DWORD dwIndex) const
{
    ATLASSERT(m_pFind);
    ATLASSERT(dwIndex < m_dwCount);

    return ((m_pFind[dwIndex].dwFileAttributes & FILE_ATTRIBUTE_SHORTCUT) != 0);
}

inline BOOL CCeFindAllFiles::IsReadOnly(DWORD dwIndex) const
{
    ATLASSERT(m_pFind);
    ATLASSERT(dwIndex < m_dwCount);

    return ((m_pFind[dwIndex].dwFileAttributes & FILE_ATTRIBUTE_READONLY) != 0);
}

inline BOOL CCeFindAllFiles::IsArchived(DWORD dwIndex) const
{
    ATLASSERT(m_pFind);
    ATLASSERT(dwIndex < m_dwCount);

    return ((m_pFind[dwIndex].dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE) != 0);
}

inline BOOL CCeFindAllFiles::IsTemporary(DWORD dwIndex) const
{
    ATLASSERT(m_pFind);
    ATLASSERT(dwIndex < m_dwCount);

    return ((m_pFind[dwIndex].dwFileAttributes & FILE_ATTRIBUTE_TEMPORARY) != 0);
}

inline BOOL CCeFindAllFiles::IsRomModule(DWORD dwIndex) const
{
    ATLASSERT(m_pFind);
    ATLASSERT(dwIndex < m_dwCount);

    return ((m_pFind[dwIndex].dwFileAttributes & FILE_ATTRIBUTE_ROMMODULE) != 0);
}

inline BOOL CCeFindAllFiles::IsDirectory(DWORD dwIndex) const
{
    ATLASSERT(m_pFind);
    ATLASSERT(dwIndex < m_dwCount);

    return ((m_pFind[dwIndex].dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0);
}

inline BOOL CCeFindAllFiles::HasChildren(DWORD dwIndex) const
{
    ATLASSERT(m_pFind);
    ATLASSERT(dwIndex < m_dwCount);

    return ((m_pFind[dwIndex].dwFileAttributes & FILE_ATTRIBUTE_HAS_CHILDREN) != 0);
}

inline BOOL CCeFindAllFiles::IsCompressed(DWORD dwIndex) const
{
    ATLASSERT(m_pFind);
    ATLASSERT(dwIndex < m_dwCount);

    return ((m_pFind[dwIndex].dwFileAttributes & FILE_ATTRIBUTE_COMPRESSED) != 0);
}

inline BOOL CCeFindAllFiles::IsRomStaticRef(DWORD dwIndex) const
{
    ATLASSERT(m_pFind);
    ATLASSERT(dwIndex < m_dwCount);

    return ((m_pFind[dwIndex].dwFileAttributes & FILE_ATTRIBUTE_ROMSTATICREF) != 0);
}

inline LPCTSTR CCeFindAllFiles::GetPath() const
{
    return (m_szPath[0] == _T('\0') ? _T("\\") : m_szPath);
}

inline LPCTSTR CCeFindAllFiles::GetFileExt(DWORD dwIndex) const
{
    ATLASSERT(m_pFind);
    ATLASSERT(dwIndex < m_dwCount);

    return ::PathFindExtension(m_pFind[dwIndex].cFileName);
}

inline ULONGLONG CCeFindAllFiles::GetFileSize(DWORD dwIndex) const
{
    ATLASSERT(m_pFind);
    ATLASSERT(dwIndex < m_dwCount);

    return ((ULONGLONG)(m_pFind[dwIndex].nFileSizeLow | (((ULONGLONG)m_pFind[dwIndex].nFileSizeHigh) << 32)));
}

#if defined(__ATLSTR_H__) || defined(__AFXSTR_H__)
inline CString CCeFindAllFiles::GetFilePath(DWORD dwIndex) const
{
    ATLASSERT(m_pFind);
    ATLASSERT(dwIndex < m_dwCount);

    CString strFilePath;
    strFilePath.Format(_T("%s\\%s"), m_szPath, m_pFind[dwIndex].cFileName);

    return strFilePath;
}

inline CString CCeFindAllFiles::GetFileTitle(DWORD dwIndex) const
{
    ATLASSERT(m_pFind);
    ATLASSERT(dwIndex < m_dwCount);

    return CString(m_pFind[dwIndex].cFileName, ::PathFindExtension(m_pFind[dwIndex].cFileName) - m_pFind[dwIndex].cFileName);
}
#endif  // defined(__ATLSTR_H__) || defined(__AFXSTR_H__)


/////////////////////////////////////////////////////////////////////////////
// CCeFindDatabase 类的实现
//

inline CCeFindDatabase::CCeFindDatabase(IRAPISession* pSession)
    : m_hFind(INVALID_HANDLE_VALUE), m_nOidDB(0), m_pSession(pSession)
{
    ATLASSERT(pSession);
}

inline CCeFindDatabase::~CCeFindDatabase()
{
    Close();
}

inline void CCeFindDatabase::Close()
{
    ATLASSERT(m_pSession);

    if (m_hFind != INVALID_HANDLE_VALUE && m_pSession->CeCloseHandle(m_hFind))
        m_hFind = INVALID_HANDLE_VALUE;
}

inline CCeFindDatabase::operator HANDLE() const
{
    return m_hFind;
}

inline BOOL CCeFindDatabase::FindNext(PCEGUID pceguid/* = NULL*/)
{
    ATLASSERT(m_pSession);
    ATLASSERT(m_hFind != INVALID_HANDLE_VALUE);

    return m_nOidDB = m_pSession->CeFindNextDatabaseEx(m_hFind, pceguid);
}

inline BOOL CCeFindDatabase::FindFirst(PCEGUID pceguid/* = NULL*/, DWORD dwDbaseType/* = 0*/)
{
    ATLASSERT(m_pSession);
    ATLASSERT(m_hFind == INVALID_HANDLE_VALUE);

    return ((m_hFind = m_pSession->CeFindFirstDatabaseEx(pceguid, dwDbaseType)) != INVALID_HANDLE_VALUE);
}


/////////////////////////////////////////////////////////////////////////////
// CCeFindAllDatabases 类的实现
//

inline CCeFindAllDatabases::CCeFindAllDatabases(IRAPISession* pSession)
    : m_pFind(NULL), m_nCount(0), m_pSession(pSession)
{
    ATLASSERT(pSession);
}

inline CCeFindAllDatabases::~CCeFindAllDatabases()
{
    Empty();
}

inline BOOL CCeFindAllDatabases::Find(DWORD dwDbaseType/* = 0*/, WORD wFlags/* = FAD_OID | FAD_FLAGS | FAD_NAME | FAD_TYPE | FAD_NUM_RECORDS | FAD_NUM_SORT_ORDER | FAD_SIZE | FAD_LAST_MODIFIED | FAD_SORT_SPECS*/)
{
    ATLASSERT(m_pSession);
    ATLASSERT(m_pFind == NULL);

    return m_pSession->CeFindAllDatabases(dwDbaseType, wFlags, &m_nCount, &m_pFind);
}

inline void CCeFindAllDatabases::Empty()
{
    ATLASSERT(m_pSession);

    if (m_pFind != NULL && SUCCEEDED(m_pSession->CeRapiFreeBuffer(m_pFind)))
        m_pFind = NULL, m_nCount = 0;
}


#ifdef __CERAPIEXT_H__
/////////////////////////////////////////////////////////////////////////////
// CSimPhoneBookEntry 类的实现
//

inline CSimPhoneBookEntry::CSimPhoneBookEntry(LPCTSTR pszText, LPCTSTR pszAddress, DWORD dwParams/* = SIM_PARAM_PBE_ALL*/, DWORD dwAddressType/* = SIM_ADDRTYPE_UNKNOWN*/, DWORD dwNumPlan/* = SIM_NUMPLAN_TELEPHONE*/)
{
    ATLASSERT(pszText);
    ATLASSERT(pszAddress);

    this->cbSize = sizeof(SIMPHONEBOOKENTRY);
    this->dwParams      = dwParams;
    this->dwNumPlan     = dwNumPlan;
    this->dwAddressType = dwAddressType;
    ::lstrcpyn(lpszText, pszText, _countof(lpszText));
    ::lstrcpyn(lpszAddress, pszAddress, _countof(lpszAddress));
}

inline CSimPhoneBookEntry::operator LPSIMPHONEBOOKENTRY()
{
    return this;
}

inline CSimPhoneBookEntry::operator const SIMPHONEBOOKENTRY*() const
{
    return this;
}

inline void CSimPhoneBookEntry::Set(LPCTSTR pszText, LPCTSTR pszAddress)
{
    ATLASSERT(pszText);
    ATLASSERT(pszAddress);

    ::lstrcpyn(lpszText, pszText, _countof(lpszText));
    ::lstrcpyn(lpszAddress, pszAddress, _countof(lpszAddress));
}


/////////////////////////////////////////////////////////////////////////////
// CCeSimPhoneBookEntry 类的实现
//

inline CCeSimPhoneBookEntry::CCeSimPhoneBookEntry(LPCESIMPBENTRY pPhoneEntry/* = NULL*/)
    : m_pPhoneEntry(pPhoneEntry)
{
}

inline CCeSimPhoneBookEntry::~CCeSimPhoneBookEntry()
{
    Empty();
}

inline void CCeSimPhoneBookEntry::Empty()
{
    if (m_pPhoneEntry != NULL && ::LocalFree(m_pPhoneEntry) == NULL)
        m_pPhoneEntry = NULL;
}

inline LPCESIMPBENTRY CCeSimPhoneBookEntry::Detach()
{
    LPCESIMPBENTRY pPhoneEntry = m_pPhoneEntry;
    m_pPhoneEntry = NULL;

    return pPhoneEntry;
}

inline void CCeSimPhoneBookEntry::operator=(LPCESIMPBENTRY pPhoneEntry)
{
    Empty();
    m_pPhoneEntry = pPhoneEntry;
}

inline BOOL CCeSimPhoneBookEntry::IsEmpty() const
{
    return (m_pPhoneEntry == NULL);
}

inline LPCTSTR CCeSimPhoneBookEntry::GetText() const
{
    ATLASSERT(m_pPhoneEntry);
    return m_pPhoneEntry->pszText;
}

inline LPCTSTR CCeSimPhoneBookEntry::GetAddress() const
{
    ATLASSERT(m_pPhoneEntry);
    return m_pPhoneEntry->pszAddress;
}

inline DWORD CCeSimPhoneBookEntry::GetNumPlan() const
{
    ATLASSERT(m_pPhoneEntry);
    return m_pPhoneEntry->dwNumPlan;
}

inline DWORD CCeSimPhoneBookEntry::GetAddressType() const
{
    ATLASSERT(m_pPhoneEntry);
    return m_pPhoneEntry->dwAddressType;
}
#endif  // __CERAPIEXT_H__


#ifdef __ATLCOMUTIL_H__
/////////////////////////////////////////////////////////////////////////////
// IRAPISinkImpl 类的实现
//

template <typename T>
inline HRESULT IRAPISinkImpl<T>::IRAPISink_Advise(IRAPIDesktop* pDesktop)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);
    ATLASSERT(pDesktop);
    ATLASSERT(m_dwCookie == INVALID_COOKIE_VALUE);

    CComPtr<IRAPIEnumDevices> spEnumDevices;
    HRESULT hr = pDesktop->EnumDevices(&spEnumDevices);
    if (SUCCEEDED(hr))
    {
        ULONG nDeviceCount = 0;
        spEnumDevices->GetCount(&nDeviceCount);

        for (ULONG i = 0; i < nDeviceCount; ++i)
        {
            CComPtr<IRAPIDevice> spDevice;
            if (SUCCEEDED(spEnumDevices->Next(&spDevice)))
                pThis->IRAPISink_OnDeviceConnected(spDevice);
        }

        hr = pDesktop->Advise((IRAPISink*)this, &m_dwCookie);   // must be C style cast
    }

    return hr;
}

template <typename T>
inline HRESULT IRAPISinkImpl<T>::IRAPISink_Unadvise(IRAPIDesktop* pDesktop)
{
    ATLASSERT(pDesktop);
    ATLASSERT(m_dwCookie != INVALID_COOKIE_VALUE);

    return pDesktop->UnAdvise(m_dwCookie);
}

template <typename T>
inline STDMETHODIMP IRAPISinkImpl<T>::OnDeviceConnected(IRAPIDevice* pDevice)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);
    ATLASSERT(pDevice);

    return pThis->IRAPISink_OnDeviceConnected(pDevice);
}

template <typename T>
inline STDMETHODIMP IRAPISinkImpl<T>::OnDeviceDisconnected(IRAPIDevice* pDevice)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);
    ATLASSERT(pDevice);

    return pThis->IRAPISink_OnDeviceDisconnected(pDevice);
}

template <typename T>
inline HRESULT IRAPISinkImpl<T>::IRAPISink_OnDeviceConnected(IRAPIDevice* /*pDevice*/)
{
    return S_OK;
}

template <typename T>
inline HRESULT IRAPISinkImpl<T>::IRAPISink_OnDeviceDisconnected(IRAPIDevice* /*pDevice*/)
{
    return S_OK;
}


#ifdef _DCCOLEH_
/////////////////////////////////////////////////////////////////////////////
// IDccManSink2Impl 类的实现
//

template <typename T>
inline HRESULT IDccManSink2Impl<T>::IDccManSink2_Advise(IDccMan* pDccMan)
{
    ATLASSERT(pDccMan);
    ATLASSERT(m_dwCookie == INVALID_COOKIE_VALUE);

    return pDccMan->Advise((IDccManSink*)this, &m_dwCookie);    // must be C style cast
}

template <typename T>
inline HRESULT IDccManSink2Impl<T>::IDccManSink2_Unadvise(IDccMan* pDccMan)
{
    ATLASSERT(pDccMan);
    ATLASSERT(m_dwCookie != INVALID_COOKIE_VALUE);

    return pDccMan->Unadvise(m_dwCookie);
}

template <typename T>
inline STDMETHODIMP IDccManSink2Impl<T>::OnLogIpAddr(DWORD dwIPAddr)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);

    return pThis->IDccManSink2_OnLogIpAddr(dwIPAddr);
}

template <typename T>
inline STDMETHODIMP IDccManSink2Impl<T>::OnLogTerminated()
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);

    return pThis->IDccManSink2_OnLogTerminated();
}

template <typename T>
inline STDMETHODIMP IDccManSink2Impl<T>::OnLogActive()
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);

    return pThis->IDccManSink2_OnLogActive();
}

template <typename T>
inline STDMETHODIMP IDccManSink2Impl<T>::OnLogInactive()
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);

    return pThis->IDccManSink2_OnLogInactive();
}

template <typename T>
inline STDMETHODIMP IDccManSink2Impl<T>::OnLogAnswered()
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);

    return pThis->IDccManSink2_OnLogAnswered();
}

template <typename T>
inline STDMETHODIMP IDccManSink2Impl<T>::OnLogListen()
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);

    return pThis->IDccManSink2_OnLogListen();
}

template <typename T>
inline STDMETHODIMP IDccManSink2Impl<T>::OnLogDisconnection()
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);

    return pThis->IDccManSink2_OnLogDisconnection();
}

template <typename T>
inline STDMETHODIMP IDccManSink2Impl<T>::OnLogError()
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);

    return pThis->IDccManSink2_OnLogError();
}

template <typename T>
inline STDMETHODIMP IDccManSink2Impl<T>::OnLogIpAddrEx(const SOCKADDR_STORAGE* pIPAddr)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);

    return pThis->IDccManSink2_OnLogIpAddrEx(pIPAddr);
}

template <typename T>
inline HRESULT IDccManSink2Impl<T>::IDccManSink2_OnLogError()
{
    return S_OK;
}

template <typename T>
inline HRESULT IDccManSink2Impl<T>::IDccManSink2_OnLogActive()
{
    return S_OK;
}

template <typename T>
inline HRESULT IDccManSink2Impl<T>::IDccManSink2_OnLogListen()
{
    return S_OK;
}

template <typename T>
inline HRESULT IDccManSink2Impl<T>::IDccManSink2_OnLogInactive()
{
    return S_OK;
}

template <typename T>
inline HRESULT IDccManSink2Impl<T>::IDccManSink2_OnLogAnswered()
{
    return S_OK;
}

template <typename T>
inline HRESULT IDccManSink2Impl<T>::IDccManSink2_OnLogTerminated()
{
    return S_OK;
}

template <typename T>
inline HRESULT IDccManSink2Impl<T>::IDccManSink2_OnLogDisconnection()
{
    return S_OK;
}

template <typename T>
inline HRESULT IDccManSink2Impl<T>::IDccManSink2_OnLogIpAddr(DWORD /*dwIPAddr*/)
{
    return S_OK;
}

template <typename T>
inline HRESULT IDccManSink2Impl<T>::IDccManSink2_OnLogIpAddrEx(const SOCKADDR_STORAGE* /*pIPAddr*/)
{
    return S_OK;
}
#endif  // _DCCOLEH_
#endif  // __ATLCOMUTIL_H__

}  // namespace cerapi

#endif  // __CERAPIUTIL_INL__