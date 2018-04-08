///////////////////////////////////////////////////////////////////////////////
// atlcomdlgs.h : ͷ�ļ�
//
// Copyright(c) 2012, Garfield. All rights reserved.
// Author  : Garfield
// Version : 1.0
// Finish Time : 2008/10/24

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __ATLCOMDLGS_H__
#define __ATLCOMDLGS_H__

#ifndef __ATLWIN_H__
#include <atlwin.h>
#endif

#ifndef _INC_COMMCTRL
#include <commctrl.h>
#endif

///////////////////////////////////////////////////////////////////////////////
// Classes in this file:
//
// CFileDialogImpl<T>
// CFileDialog
// CFolderDialogImpl<T>
// CFolderDialog

namespace atlctrls {

///////////////////////////////////////////////////////////////////////////////
// CFileDialogImpl ��Ķ���
//

template <typename T>
class ATL_NO_VTABLE CFileDialogImpl : public ATL::CDialogImplBase
{
// ����
public:
    explicit CFileDialogImpl(BOOL bOpenFileDialog, LPCTSTR pszDefExt = NULL, LPCTSTR pszFileName = NULL, LPCTSTR pszFilter = NULL,
                             DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, HWND hWndParent = NULL);

// ʵ��
public:
    BEGIN_MSG_MAP(CFileDialogImpl<T>)
        NOTIFY_CODE_HANDLER(CDN_HELP, _OnHelp)
        NOTIFY_CODE_HANDLER(CDN_INITDONE, _OnInitDone)
        NOTIFY_CODE_HANDLER(CDN_FILEOK, _OnFileNameOK)
        NOTIFY_CODE_HANDLER(CDN_SELCHANGE, _OnSelChange)
        NOTIFY_CODE_HANDLER(CDN_TYPECHANGE, _OnTypeChange)
        NOTIFY_CODE_HANDLER(CDN_FOLDERCHANGE, _OnFolderChange)
        NOTIFY_CODE_HANDLER(CDN_SHAREVIOLATION, _OnShareViolation)
    END_MSG_MAP()

// ����
public:
    INT_PTR DoModal(HWND hWndParent = ::GetActiveWindow());

    LPCTSTR GetFileExt() const;
    LPCTSTR GetPathName() const;
    LPCTSTR GetFileName() const;
    CString GetFileTitle() const;
    BOOL GetReadOnlyPref() const;

    POSITION GetStartPosition() const;
    CString GetNextPathName(POSITION& pos) const;

    OPENFILENAME& GetOFN();
    const OPENFILENAME& GetOFN() const;

    CString GetFolderPath() const;
    void HideControl(UINT nCtrlID);
    void SetDefExt(LPCSTR pszDefExt);
    void SetControlText(UINT nCtrlID, LPCTSTR pszText);

// ��д
public:
    void OnHelp(LPOFNOTIFY lpOFN);
    void OnInitDone(LPOFNOTIFY lpOFN);
    void OnSelChange(LPOFNOTIFY lpOFN);
    BOOL OnFileNameOK(LPOFNOTIFY lpOFN);
    void OnTypeChange(LPOFNOTIFY lpOFN);
    void OnFolderChange(LPOFNOTIFY lpOFN);
    int  OnShareViolation(LPOFNOTIFY lpOFN);

// ���ɵ���Ϣӳ�亯��
public:
    LRESULT _OnHelp(int nCtrlID, LPNMHDR pNMHDR, BOOL& bHandled);
    LRESULT _OnInitDone(int nCtrlID, LPNMHDR pNMHDR, BOOL& bHandled);
    LRESULT _OnSelChange(int nCtrlID, LPNMHDR pNMHDR, BOOL& bHandled);
    LRESULT _OnFileNameOK(int nCtrlID, LPNMHDR pNMHDR, BOOL& bHandled);
    LRESULT _OnTypeChange(int nCtrlID, LPNMHDR pNMHDR, BOOL& bHandled);
    LRESULT _OnFolderChange(int nCtrlID, LPNMHDR pNMHDR, BOOL& bHandled);
    LRESULT _OnShareViolation(int nCtrlID, LPNMHDR pNMHDR, BOOL& bHandled);

// ��Ա
protected:
    OPENFILENAME m_ofn;
    BOOL  m_bOpenFileDialog;
    TCHAR m_szFileName[MAX_PATH];
    TCHAR m_szFileTitle[MAX_PATH];
};


///////////////////////////////////////////////////////////////////////////////
// CFileDialog ��Ķ���
//

class CFileDialog : public CFileDialogImpl<CFileDialog>
{
// ����
public:
    explicit CFileDialog(BOOL bOpenFileDialog, LPCTSTR pszDefExt = NULL, LPCTSTR pszFileName = NULL, LPCTSTR pszFilter = NULL, DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, HWND hWndParent = NULL)
        : CFileDialogImpl<CFileDialog>(bOpenFileDialog, pszDefExt, pszFileName, pszFilter, dwFlags, hWndParent)
    {
    }

    DECLARE_EMPTY_MSG_MAP()
};


///////////////////////////////////////////////////////////////////////////////
// CFolderDialogImpl ��Ķ���
//

template <typename T>
class ATL_NO_VTABLE CFolderDialogImpl
{
// ����
public:
    explicit CFolderDialogImpl(LPCTSTR pszTitle = NULL, LPCTSTR pszInitFolder = NULL, UINT uFlags = BIF_NEWDIALOGSTYLE | BIF_RETURNONLYFSDIRS, HWND hWndParent = NULL);

// ����
public:
    INT_PTR DoModal(HWND hWndParent = ::GetActiveWindow());

    int GetImage() const;
    LPCTSTR GetDisplayName() const;
    LPCTSTR GetSelectedPath() const;
    void SetInitFolder(LPCTSTR pszInitFolder);

// ����
public:
    BROWSEINFO& GetBrowseInfo();
    const BROWSEINFO& GetBrowseInfo() const;

// ��д
public:
    void OnInitialized(HWND hWnd);
    void OnIUnknown(HWND hWnd, IUnknown* pUnk);
    void OnSelChanged(HWND hWnd, LPITEMIDLIST pItemIDList);
    int  OnValidateFailed(HWND hWnd, LPCTSTR pszFolderPath);

// ʵ��
protected:
    static int CALLBACK FolderDialogProc(HWND hWnd, UINT uMsg, LPARAM lParam, LPARAM lData);

// ��Ա
protected:
    BROWSEINFO m_browseInfo;
    TCHAR m_szPath[MAX_PATH];
    TCHAR m_szDisplayName[MAX_PATH];
};


///////////////////////////////////////////////////////////////////////////////
// CFolderDialog ��Ķ���
//

class CFolderDialog : public CFolderDialogImpl<CFolderDialog>
{
// ����
public:
    explicit CFolderDialog(LPCTSTR pszTitle = NULL, LPCTSTR pszInitFolder = NULL, UINT uFlags = BIF_NEWDIALOGSTYLE | BIF_RETURNONLYFSDIRS, HWND hWndParent = NULL)
        : CFolderDialogImpl<CFolderDialog>(pszTitle, pszInitFolder, uFlags, hWndParent)
    {
    }
};

}  // namespace atlctrls

#include "atlcomdlgs.inl"

#endif  // __ATLCOMDLGS_H__