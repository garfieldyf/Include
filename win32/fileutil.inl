///////////////////////////////////////////////////////////////////////////////
// fileutil.inl
//
// Author : Garfield
// Creation Date : 2007/12/21

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __FILEUTIL_INL__
#define __FILEUTIL_INL__

#ifndef __FILEUTIL_H__
    #error fileutil.inl requires fileutil.h to be included first
#endif

namespace stdutil {

///////////////////////////////////////////////////////////////////////////////
// Implementation of the CFindFile class
//

inline CFindFile::CFindFile()
    : m_hFind(INVALID_HANDLE_VALUE)
{
    m_szPath[0] = _T('\0');
}

inline CFindFile::~CFindFile()
{
    Close();
}

inline BOOL CFindFile::FindFirst(LPCTSTR pszPathName, BOOL bSkipDots/* = TRUE*/, FINDEX_SEARCH_OPS SearchOp/* = FindExSearchNameMatch*/, FINDEX_INFO_LEVELS InfoLevelId/* = FindExInfoStandard*/, LPVOID pSearchFilter/* = NULL*/, DWORD dwAdditionalFlags/* = 0*/)
{
    ATLASSERT(pszPathName);
    ATLASSERT(m_hFind == INVALID_HANDLE_VALUE);
    ATLASSERT(::_tcsrchr(pszPathName, _T('\\')) != NULL);
    UNUSED_PARAMETER(bSkipDots);

    BOOL bSuccessful = ((m_hFind = ::FindFirstFileEx(pszPathName, InfoLevelId, this, SearchOp, pSearchFilter, dwAdditionalFlags)) != INVALID_HANDLE_VALUE);
    if (bSuccessful)
    {
        ::_tssprintf(m_szPath, _countof(m_szPath), _T("%.*s"), ::_tcsrchr(pszPathName, _T('\\')) - pszPathName, pszPathName);

    #ifndef _WIN32_WCE
        if (bSkipDots)
            bSuccessful = ::FindNextFile(m_hFind, this) && ::FindNextFile(m_hFind, this);
    #endif  // _WIN32_WCE
    }

    return bSuccessful;
}

inline BOOL CFindFile::FindNext()
{
    ATLASSERT(m_hFind != INVALID_HANDLE_VALUE);
    return ::FindNextFile(m_hFind, this);
}

inline void CFindFile::Close()
{
    if (m_hFind != INVALID_HANDLE_VALUE && ::FindClose(m_hFind))
        m_hFind = INVALID_HANDLE_VALUE;
}

inline CFindFile::operator HANDLE() const
{
    return m_hFind;
}

inline BOOL CFindFile::IsNormal() const
{
    return (dwFileAttributes & FILE_ATTRIBUTE_NORMAL);
}

inline BOOL CFindFile::IsSystem() const
{
    return (dwFileAttributes & FILE_ATTRIBUTE_SYSTEM);
}

inline BOOL CFindFile::IsHidden() const
{
    return (dwFileAttributes & FILE_ATTRIBUTE_HIDDEN);
}

inline BOOL CFindFile::IsReadOnly() const
{
    return (dwFileAttributes & FILE_ATTRIBUTE_READONLY);
}

inline BOOL CFindFile::IsArchived() const
{
    return (dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE);
}

inline BOOL CFindFile::IsEncrypted() const
{
    return (dwFileAttributes & FILE_ATTRIBUTE_ENCRYPTED);
}

inline BOOL CFindFile::IsTemporary() const
{
    return (dwFileAttributes & FILE_ATTRIBUTE_TEMPORARY);
}

inline BOOL CFindFile::IsDirectory() const
{
    return (dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY);
}

inline BOOL CFindFile::IsCompressed() const
{
    return (dwFileAttributes & FILE_ATTRIBUTE_COMPRESSED);
}

#ifdef _WIN32_WCE
inline BOOL CFindFile::IsInRom() const
{
    return (dwFileAttributes & FILE_ATTRIBUTE_INROM);
}

inline BOOL CFindFile::IsRomModule() const
{
    return (dwFileAttributes & FILE_ATTRIBUTE_ROMMODULE);
}

inline BOOL CFindFile::IsRomStaticRef() const
{
    return (dwFileAttributes & FILE_ATTRIBUTE_ROMSTATICREF);
}
#endif  // _WIN32_WCE

inline LPCTSTR CFindFile::GetPath() const
{
#ifndef _WIN32_WCE
    return m_szPath;
#else
    return (m_szPath[0] == _T('\0') ? _T("\\") : m_szPath);
#endif  // _WIN32_WCE
}

inline LPCTSTR CFindFile::GetFileExt() const
{
    return ::PathFindExtension(cFileName);
}

inline ULONGLONG CFindFile::GetFileSize() const
{
    return MAKEULONGLONG(nFileSizeLow, nFileSizeHigh);
}

#if defined(__ATLSTR_H__) || defined(__AFXSTR_H__)
inline CString CFindFile::GetFileURL() const
{
    CString strFileURL;
    strFileURL.Format(_T("file://%s\\%s"), m_szPath, cFileName);

    return strFileURL;
}

inline CString CFindFile::GetFilePath() const
{
    CString strFilePath;
    strFilePath.Format(_T("%s\\%s"), m_szPath, cFileName);

    return strFilePath;
}

inline CString CFindFile::GetFileTitle() const
{
    return CString(cFileName, ::PathFindExtension(cFileName) - cFileName);
}
#endif  // defined(__ATLSTR_H__) || defined(__AFXSTR_H__)


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CMappingViewBase class
//

inline CMappingViewBase::CMappingViewBase(ULONGLONG nOffset/* = 0*/, SIZE_T nMappingSize/* = 0*/)
    : m_pViewData(NULL), m_nMappingSize(nMappingSize)
{
    m_liOffset.QuadPart = nOffset;
}

inline CMappingViewBase::~CMappingViewBase()
{
    UnmapView();
}

inline BOOL CMappingViewBase::FlushView(SIZE_T nNumberOfBytesToFlush/* = 0*/)
{
    ATLASSERT(!IsEmpty());
    return ::FlushViewOfFile(m_pViewData, nNumberOfBytesToFlush);
}

inline void CMappingViewBase::UnmapView()
{
    if (m_pViewData != NULL && ::UnmapViewOfFile(m_pViewData))
        m_pViewData = NULL;
}

inline void CMappingViewBase::SetMappingSize(ULONGLONG nOffset, SIZE_T nMappingSize)
{
    m_liOffset.QuadPart = nOffset;
    m_nMappingSize = nMappingSize;
}

inline BOOL CMappingViewBase::IsEmpty() const
{
    return (m_pViewData == NULL);
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CMappingView class
//

template <typename _Ty>
inline CMappingView<_Ty>::CMappingView(ULONGLONG nOffset/* = 0*/, SIZE_T nMappingSize/* = 0*/)
    : CMappingViewBase(nOffset, nMappingSize)
{
}

template <typename _Ty>
inline _Ty* CMappingView<_Ty>::GetData() const
{
    return reinterpret_cast<_Ty*>(m_pViewData);
}

template <typename _Ty>
inline SIZE_T CMappingView<_Ty>::GetCount() const
{
    return (m_nMappingSize / sizeof(_Ty));
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CFileMapping class
//

inline CFileMapping::CFileMapping()
{
}

inline HANDLE CFileMapping::Create(HANDLE hFile, ULONGLONG nMaximumSize/* = 0*/, DWORD dwProtect/* = PAGE_READWRITE*/, LPCTSTR pszName/* = NULL*/, LPSECURITY_ATTRIBUTES psa/* = NULL*/)
{
    ATLASSERT(IsEmpty());
    return (m_hHandle = ::CreateFileMapping(hFile, psa, dwProtect, HIDWORD(nMaximumSize), LODWORD(nMaximumSize), pszName));
}

template <typename _Ty>
inline _Ty* CFileMapping::MapView(CMappingView<_Ty>& view, DWORD dwDesiredAccess/* = FILE_MAP_ALL_ACCESS*/)
{
    ATLASSERT(!IsEmpty());
    ATLASSERT(view.IsEmpty());

    view.m_pViewData = ::MapViewOfFile(m_hHandle, dwDesiredAccess, view.m_liOffset.HighPart, view.m_liOffset.LowPart, view.m_nMappingSize);
    return reinterpret_cast<_Ty*>(view.m_pViewData);
}

#ifndef _WIN32_WCE
inline HANDLE CFileMapping::Open(LPCTSTR pszName, DWORD dwDesiredAccess/* = FILE_MAP_ALL_ACCESS*/, BOOL bInheritHandle/* = FALSE*/)
{
    ATLASSERT(pszName);
    ATLASSERT(IsEmpty());

    return (m_hHandle = ::OpenFileMapping(dwDesiredAccess, bInheritHandle, pszName));
}

template <typename _Ty>
inline _Ty* CFileMapping::MapView(CMappingView<_Ty>& view, LPVOID pBaseAddress, DWORD dwDesiredAccess/* = FILE_MAP_ALL_ACCESS*/)
{
    ATLASSERT(!IsEmpty());

    view.m_pViewData = ::MapViewOfFileEx(m_hHandle, dwDesiredAccess, view.m_liOffset.HighPart, view.m_liOffset.LowPart, view.m_nMappingSize, pBaseAddress);
    return reinterpret_cast<_Ty*>(view.m_pViewData);
}
#endif  // _WIN32_WCE


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CFileVersion class
//

inline CFileVersion::CFileVersion()
{
}

inline BOOL CFileVersion::GetFileVersionInfo(LPCTSTR pszFileName)
{
    ATLASSERT(pszFileName);

    DWORD cbSize = ::GetFileVersionInfoSize((LPTSTR)pszFileName, NULL);
    BOOL bSuccessful = (cbSize != 0 && m_buffer.allocateBytes(cbSize) != NULL && ::GetFileVersionInfo((LPTSTR)pszFileName, NULL, cbSize, m_buffer.data));
    if (!bSuccessful)
        m_buffer.clear();

    return bSuccessful;
}

inline const VS_FIXEDFILEINFO* CFileVersion::GetFixedFileInfo() const
{
    ATLASSERT(!IsEmpty());

    VS_FIXEDFILEINFO* pFixedFileInfo = NULL;
    ::VerQueryValue(m_buffer.data, _T("\\"), (LPVOID*)&pFixedFileInfo, NULL);

    return pFixedFileInfo;
}

inline BOOL CFileVersion::GetCodePage(LPVS_CODEPAGE& pCodePage, LPUINT pcbSize/* = NULL*/) const
{
    ATLASSERT(!IsEmpty());
    return ::VerQueryValue(m_buffer.data, _T("\\VarFileInfo\\Translation"), (LPVOID*)&pCodePage, pcbSize);
}

inline LPCTSTR CFileVersion::QueryValue(LPCTSTR pszKeyName) const
{
    ATLASSERT(!IsEmpty());
    ATLASSERT(pszKeyName);

    LPCTSTR pszValue = NULL;
    LPVS_CODEPAGE pCodePage = NULL;

    if (GetCodePage(pCodePage, NULL))
        pszValue = QueryValue(pszKeyName, pCodePage);

    return pszValue;
}

inline LPCTSTR CFileVersion::QueryValue(LPCTSTR pszKeyName, const VS_CODEPAGE* pCodePage) const
{
    ATLASSERT(pCodePage);
    ATLASSERT(!IsEmpty());
    ATLASSERT(pszKeyName);

    TCHAR szSubBlock[64];
    LPTSTR pszValue = NULL;

    ::_tssprintf(szSubBlock, _countof(szSubBlock), _T("\\StringFileInfo\\%04x%04x\\%s"), pCodePage->wLanguage, pCodePage->wCodePage, pszKeyName);
    ::VerQueryValue(m_buffer.data, szSubBlock, (LPVOID*)&pszValue, NULL);

    return pszValue;
}


#ifndef _WIN32_WCE
///////////////////////////////////////////////////////////////////////////////
// Implementation of the CSplitterPath class
//

inline CSplitterPath::CSplitterPath()
{
    m_szDrive[0] = m_szDir[0] = m_szFname[0] = m_szExt[0] = _T('\0');
}

inline CSplitterPath::CSplitterPath(LPCTSTR pszPath)
{
    ATLASSERT(pszPath);
    SplitPath(pszPath);
}

#if defined(__ATLSTR_H__) || defined(__AFXSTR_H__)
inline CString CSplitterPath::GetFileName() const
{
    CString strFileName;
    strFileName.Format(_T("%s%s"), m_szFname, m_szExt);

    return strFileName;
}

inline CString CSplitterPath::GetFilePath() const
{
    CString strFilePath;
    strFilePath.Format(_T("%s%s"), m_szDrive, m_szDir);

    return strFilePath;
}
#endif  // defined(__ATLSTR_H__) || defined(__AFXSTR_H__)

inline void CSplitterPath::SplitPath(LPCTSTR pszPath)
{
    ATLASSERT(pszPath);
    _tssplitpath(pszPath, m_szDrive, _countof(m_szDrive), m_szDir, _countof(m_szDir), m_szFname, _countof(m_szFname), m_szExt, _countof(m_szExt));
}

#if defined(__ATLSTR_H__) || defined(__AFXSTR_H__)
inline CString WINAPI CSplitterPath::MakePath(LPCTSTR pszDrive, LPCTSTR pszDir, LPCTSTR pszFileName, LPCTSTR pszFileExt)
{
    CString strPath;
    _tsmakepath(strPath.GetBuffer(MAX_PATH), MAX_PATH, pszDrive, pszDir, pszFileName, pszFileExt);
    strPath.ReleaseBuffer();

    return strPath;
}
#endif  // defined(__ATLSTR_H__) || defined(__AFXSTR_H__)
#endif  // _WIN32_WCE

}  // namespace stdutil

#endif  // __FILEUTIL_INL__