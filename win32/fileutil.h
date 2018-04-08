///////////////////////////////////////////////////////////////////////////////
// fileutil.h
//
// Copyright(c) 2018, Garfield. All rights reserved.
// Author  : Garfield
// Version : 1.5
// Creation Date : 2007/12/21

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __FILEUTIL_H__
#define __FILEUTIL_H__

#include <shlwapi.h>
#include "atlmiscutil.h"

///////////////////////////////////////////////////////////////////////////////
// Classes in this file:
//
// CFindFile
// CMappingView<_Ty>
// CFileMapping
// CFileVersion
// CSplitterPath

namespace stdutil {

///////////////////////////////////////////////////////////////////////////////
// Interface of the CFindFile class
//

class CFindFile : public WIN32_FIND_DATA
{
// Constructors/Destructor
public:
    CFindFile();
    ~CFindFile();

// Operations
public:
    BOOL FindFirst(LPCTSTR pszPathName, BOOL bSkipDots = TRUE, FINDEX_SEARCH_OPS SearchOp = FindExSearchNameMatch, FINDEX_INFO_LEVELS InfoLevelId = FindExInfoStandard, LPVOID pSearchFilter = NULL, DWORD dwAdditionalFlags = 0);
    BOOL FindNext();

    void Close();
    operator HANDLE() const;

// Attributes
public:
    BOOL IsNormal() const;
    BOOL IsSystem() const;
    BOOL IsHidden() const;
    BOOL IsReadOnly() const;
    BOOL IsArchived() const;
    BOOL IsEncrypted() const;
    BOOL IsTemporary() const;
    BOOL IsDirectory() const;
    BOOL IsCompressed() const;

#ifdef _WIN32_WCE
    BOOL IsInRom() const;
    BOOL IsRomModule() const;
    BOOL IsRomStaticRef() const;
#endif

    LPCTSTR GetPath() const;
    LPCTSTR GetFileExt() const;
    ULONGLONG GetFileSize() const;

#if defined(__ATLSTR_H__) || defined(__AFXSTR_H__)
    CString GetFileURL() const;
    CString GetFilePath() const;
    CString GetFileTitle() const;
#endif

// Implementation
private:
    CFindFile(const CFindFile& that);
    CFindFile& operator=(const CFindFile& that);

// Data members
public:
    HANDLE m_hFind;
    TCHAR m_szPath[MAX_PATH];
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CMappingViewBase class
//

class ATL_NO_VTABLE CMappingViewBase
{
// Constructors/Destructor
protected:
    CMappingViewBase(ULONGLONG nOffset = 0, SIZE_T nMappingSize = 0);
    ~CMappingViewBase();

// Operations
public:
    BOOL FlushView(SIZE_T nNumberOfBytesToFlush = 0);
    void UnmapView();
    void SetMappingSize(ULONGLONG nOffset, SIZE_T nMappingSize);

// Attributes
public:
    BOOL IsEmpty() const;

// Implementation
private:
    CMappingViewBase(const CMappingViewBase& that);
    CMappingViewBase& operator=(const CMappingViewBase& that);

// Data members
public:
    LPVOID m_pViewData;
    SIZE_T m_nMappingSize;
    ULARGE_INTEGER m_liOffset;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CMappingView class
//

template <typename _Ty = BYTE>
class CMappingView : public CMappingViewBase
{
// Constructors
public:
    explicit CMappingView(ULONGLONG nOffset = 0, SIZE_T nMappingSize = 0);

// Attributes
public:
    _Ty* GetData() const;
    SIZE_T GetCount() const;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CFileMapping class
//

class CFileMapping : public atlutil::CHandle<>
{
// Constructors
public:
    CFileMapping();

// Operations
public:
    HANDLE Create(HANDLE hFile, ULONGLONG nMaximumSize = 0, DWORD dwProtect = PAGE_READWRITE, LPCTSTR pszName = NULL, LPSECURITY_ATTRIBUTES psa = NULL);
    template <typename _Ty> _Ty* MapView(CMappingView<_Ty>& view, DWORD dwDesiredAccess = FILE_MAP_ALL_ACCESS);

#ifndef _WIN32_WCE
    HANDLE Open(LPCTSTR pszName, DWORD dwDesiredAccess = FILE_MAP_ALL_ACCESS, BOOL bInheritHandle = FALSE);
    template <typename _Ty> _Ty* MapView(CMappingView<_Ty>& view, LPVOID pBaseAddress, DWORD dwDesiredAccess = FILE_MAP_ALL_ACCESS);
#endif
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CFileVersion class
//

class CFileVersion : public atlutil::CHeapBufferImpl<VOID>
{
// Constructors
public:
    CFileVersion();

// Operations
public:
    BOOL GetFileVersionInfo(LPCTSTR pszFileName);

    const VS_FIXEDFILEINFO* GetFixedFileInfo() const;
    BOOL GetCodePage(LPVS_CODEPAGE& pCodePage, LPUINT pcbSize = NULL) const;

    LPCTSTR QueryValue(LPCTSTR pszKeyName) const;
    LPCTSTR QueryValue(LPCTSTR pszKeyName, const VS_CODEPAGE* pCodePage) const;
};


#ifndef _WIN32_WCE
///////////////////////////////////////////////////////////////////////////////
// Interface of the CSplitterPath class
//

class CSplitterPath
{
// Constructors
public:
    CSplitterPath();
    explicit CSplitterPath(LPCTSTR pszPath);

// Attributes
public:
#if defined(__ATLSTR_H__) || defined(__AFXSTR_H__)
    CString GetFileName() const;
    CString GetFilePath() const;
#endif

// Operations
public:
    void SplitPath(LPCTSTR pszPath);

#if defined(__ATLSTR_H__) || defined(__AFXSTR_H__)
    static CString WINAPI MakePath(LPCTSTR pszDrive, LPCTSTR pszDir, LPCTSTR pszFileName, LPCTSTR pszFileExt);
#endif

// Data members
public:
    TCHAR m_szDir[_MAX_DIR];
    TCHAR m_szExt[_MAX_EXT];
    TCHAR m_szDrive[_MAX_DRIVE];
    TCHAR m_szFname[_MAX_FNAME];
};
#endif  // _WIN32_WCE

}  // namespace stdutil

#include "fileutil.inl"

#endif  // __FILEUTIL_H__