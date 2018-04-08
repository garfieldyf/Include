///////////////////////////////////////////////////////////////////////////////
// atlcomdlgs.inl : 实现文件
//
// Copyright(c) 2012, Garfield. All rights reserved.
// Author  : Garfield
// Version : 1.0
// Finish Time : 2008/10/24

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __ATLCOMDLGS_INL__
#define __ATLCOMDLGS_INL__

namespace atlctrls {

///////////////////////////////////////////////////////////////////////////////
// CFileDialogImpl 类的实现
//

template <typename T>
inline CFileDialogImpl<T>::CFileDialogImpl(BOOL bOpenFileDialog, LPCTSTR pszDefExt/* = NULL*/, LPCTSTR pszFileName/* = NULL*/, LPCTSTR pszFilter/* = NULL*/,
                                           DWORD dwFlags/* = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT*/, HWND hWndParent/* = NULL*/)
{
    ::ZeroMemory(&m_ofn, sizeof(OPENFILENAME));
    m_bOpenFileDialog    = bOpenFileDialog;
    m_szFileTitle[0]     = _T('\0');
    m_ofn.lStructSize    = sizeof(OPENFILENAME);
    m_ofn.lpstrFile      = m_szFileName;
    m_ofn.nMaxFile       = _countof(m_szFileName);
    m_ofn.lpstrFileTitle = m_szFileTitle;
    m_ofn.nMaxFileTitle  = _countof(m_szFileTitle);
    m_ofn.lpstrDefExt    = pszDefExt;
    m_ofn.lpstrFilter    = pszFilter;
    m_ofn.hwndOwner      = hWndParent;
    m_ofn.lpfnHook       = (LPOFNHOOKPROC)T::StartDialogProc;
    m_ofn.hInstance      = ATL::_AtlBaseModule.GetModuleInstance();
    m_ofn.Flags          = dwFlags | OFN_EXPLORER | OFN_ENABLEHOOK | OFN_ENABLESIZING;
    ::lstrcpyn(m_szFileName, (pszFileName != NULL) ? pszFileName : _T(""), _countof(m_szFileName));
}

template <typename T>
inline INT_PTR CFileDialogImpl<T>::DoModal(HWND hWndParent = ::GetActiveWindow())
{
    ATLASSERT(m_ofn.lpfnHook);
    ATLASSERT(m_ofn.Flags & OFN_EXPLORER);
    ATLASSERT(m_ofn.Flags & OFN_ENABLEHOOK);

    if (m_ofn.hwndOwner == NULL)
        m_ofn.hwndOwner = hWndParent;

    ATLASSERT(m_hWnd == NULL);
    ATL::_AtlWinModule.AddCreateWndData(&m_thunk.cd, this);
    INT_PTR nResult = (m_bOpenFileDialog) ? ::GetOpenFileName(&m_ofn) : ::GetSaveFileName(&m_ofn);
    m_hWnd = NULL;

    return nResult ? nResult : IDCANCEL;
}

template <typename T>
inline LPCTSTR CFileDialogImpl<T>::GetFileExt() const
{
    ATLASSERT(m_ofn.Flags & OFN_EXPLORER);
    return (m_ofn.nFileExtension != 0) ? m_ofn.lpstrFile + m_ofn.nFileExtension : NULL;
}

template <typename T>
inline LPCTSTR CFileDialogImpl<T>::GetPathName() const
{
    ATLASSERT(m_ofn.Flags & OFN_EXPLORER);
    return m_ofn.lpstrFile;
}

template <typename T>
inline LPCTSTR CFileDialogImpl<T>::GetFileName() const
{
    ATLASSERT(m_ofn.Flags & OFN_EXPLORER);
    return m_ofn.lpstrFileTitle;
}

template <typename T>
inline CString CFileDialogImpl<T>::GetFileTitle() const
{
    CString strTitle = m_ofn.lpstrFileTitle;
    ::PathRemoveExtension(strTitle.GetBuffer());
    strTitle.ReleaseBuffer();

    return strTitle;
}

template <typename T>
inline BOOL CFileDialogImpl<T>::GetReadOnlyPref() const
{
    return (m_ofn.Flags & OFN_READONLY) != 0;
}

template <typename T>
inline POSITION CFileDialogImpl<T>::GetStartPosition() const
{
    return reinterpret_cast<POSITION>(m_ofn.lpstrFile);
}

template <typename T>
inline CString CFileDialogImpl<T>::GetNextPathName(POSITION& pos) const
{
    ATLASSERT(m_ofn.Flags & OFN_EXPLORER);
    ATLASSERT(m_ofn.Flags & OFN_ALLOWMULTISELECT);

    LPTSTR pszPos = reinterpret_cast<LPTSTR>(pos);
    if (pszPos == m_ofn.lpstrFile)      // 第一次执行
    {
        while (*++pszPos != _T('\0'));
        if (*++pszPos == _T('\0'))      // 只选择了一个文件
        {
            pos = NULL;
            return m_ofn.lpstrFile;
        }
    }

    // 构造全路径
    CString strPath;
    LPCTSTR pszLast = m_ofn.lpstrFile + ::lstrlen(m_ofn.lpstrFile) - 1;
    strPath.Format((*pszLast != _T('\\')) ? _T("%s\\%s") : _T("%s%s"), m_ofn.lpstrFile, pszPos);

    // 让 POSITION 指向下一个文件名
    while (*++pszPos != _T('\0'));
    pos = (*++pszPos != _T('\0')) ? reinterpret_cast<POSITION>(pszPos) : NULL;

    return strPath;
}

template <typename T>
inline OPENFILENAME& CFileDialogImpl<T>::GetOFN()
{
    return m_ofn;
}

template <typename T>
inline const OPENFILENAME& CFileDialogImpl<T>::GetOFN() const
{
    return m_ofn;
}

template <typename T>
inline CString CFileDialogImpl<T>::GetFolderPath() const
{
    ATLASSERT(IsWindow());
    ATLASSERT(m_ofn.Flags & OFN_EXPLORER);

    CString strPath;
    GetParent().SendMessage(CDM_GETFOLDERPATH, MAX_PATH, reinterpret_cast<LPARAM>(strPath.GetBuffer(MAX_PATH)));
    strPath.ReleaseBuffer();

    return strPath;
}

template <typename T>
inline void CFileDialogImpl<T>::HideControl(UINT nCtrlID)
{
    ATLASSERT(IsWindow());
    ATLASSERT(m_ofn.Flags & OFN_EXPLORER);

    GetParent().SendMessage(CDM_HIDECONTROL, nCtrlID, 0L);
}

template <typename T>
inline void CFileDialogImpl<T>::SetDefExt(LPCSTR pszDefExt)
{
    ATLASSERT(IsWindow());
    ATLASSERT(m_ofn.Flags & OFN_EXPLORER);

    GetParent().SendMessage(CDM_SETDEFEXT, 0, reinterpret_cast<LPARAM>(pszDefExt));
}

template <typename T>
inline void CFileDialogImpl<T>::SetControlText(UINT nCtrlID, LPCTSTR pszText)
{
    ATLASSERT(IsWindow());
    ATLASSERT(m_ofn.Flags & OFN_EXPLORER);

    GetParent().SendMessage(CDM_SETCONTROLTEXT, nCtrlID, reinterpret_cast<LPARAM>(pszText));
}

template <typename T>
inline void CFileDialogImpl<T>::OnHelp(LPOFNOTIFY /*lpOFN*/)
{
}

template <typename T>
inline void CFileDialogImpl<T>::OnInitDone(LPOFNOTIFY /*lpOFN*/)
{
}

template <typename T>
inline void CFileDialogImpl<T>::OnSelChange(LPOFNOTIFY /*lpOFN*/)
{
}

template <typename T>
inline BOOL CFileDialogImpl<T>::OnFileNameOK(LPOFNOTIFY /*lpOFN*/)
{
    return TRUE;
}

template <typename T>
inline void CFileDialogImpl<T>::OnTypeChange(LPOFNOTIFY /*lpOFN*/)
{
}

template <typename T>
inline void CFileDialogImpl<T>::OnFolderChange(LPOFNOTIFY /*lpOFN*/)
{
}

template <typename T>
inline int CFileDialogImpl<T>::OnShareViolation(LPOFNOTIFY /*lpOFN*/)
{
    return 0;
}

template <typename T>
inline LRESULT CFileDialogImpl<T>::_OnHelp(int /*nCtrlID*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/)
{
    ATLASSERT(IsWindow());
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);

    pThis->OnHelp(reinterpret_cast<LPOFNOTIFY>(pNMHDR));
    return 0L;
}

template <typename T>
inline LRESULT CFileDialogImpl<T>::_OnInitDone(int /*nCtrlID*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/)
{
    ATLASSERT(IsWindow());
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);

    pThis->OnInitDone(reinterpret_cast<LPOFNOTIFY>(pNMHDR));
    return 0L;
}

template <typename T>
inline LRESULT CFileDialogImpl<T>::_OnSelChange(int /*nCtrlID*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/)
{
    ATLASSERT(IsWindow());
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);

    pThis->OnSelChange(reinterpret_cast<LPOFNOTIFY>(pNMHDR));
    return 0L;
}

template <typename T>
inline LRESULT CFileDialogImpl<T>::_OnFileNameOK(int /*nCtrlID*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/)
{
    ATLASSERT(IsWindow());
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);

    return !pThis->OnFileNameOK(reinterpret_cast<LPOFNOTIFY>(pNMHDR));
}

template <typename T>
inline LRESULT CFileDialogImpl<T>::_OnTypeChange(int /*nCtrlID*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/)
{
    ATLASSERT(IsWindow());
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);

    pThis->OnTypeChange(reinterpret_cast<LPOFNOTIFY>(pNMHDR));
    return 0L;
}

template <typename T>
inline LRESULT CFileDialogImpl<T>::_OnFolderChange(int /*nCtrlID*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/)
{
    ATLASSERT(IsWindow());
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);

    pThis->OnFolderChange(reinterpret_cast<LPOFNOTIFY>(pNMHDR));
    return 0L;
}

template <typename T>
inline LRESULT CFileDialogImpl<T>::_OnShareViolation(int /*nCtrlID*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/)
{
    ATLASSERT(IsWindow());
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);

    return pThis->OnShareViolation(reinterpret_cast<LPOFNOTIFY>(pNMHDR));
}


///////////////////////////////////////////////////////////////////////////////
// CFolderDialogImpl 类的实现
//

template <typename T>
inline CFolderDialogImpl<T>::CFolderDialogImpl(LPCTSTR pszTitle/* = NULL*/, LPCTSTR pszInitFolder/* = NULL*/,
                                               UINT uFlags/* = BIF_NEWDIALOGSTYLE | BIF_RETURNONLYFSDIRS*/, HWND hWndParent/* = NULL*/)
{
    ::ZeroMemory(&m_browseInfo, sizeof(BROWSEINFO));
    m_szDisplayName[0]     = _T('\0');
    m_browseInfo.hwndOwner = hWndParent;
    m_browseInfo.lpszTitle = pszTitle;
    m_browseInfo.ulFlags   = uFlags;
    m_browseInfo.lParam    = reinterpret_cast<LPARAM>(static_cast<T*>(this));
    m_browseInfo.lpfn      = FolderDialogProc;
    m_browseInfo.pszDisplayName = m_szDisplayName;
    ::lstrcpyn(m_szPath, (pszInitFolder != NULL) ? pszInitFolder : _T(""), _countof(m_szPath));
}

template <typename T>
inline INT_PTR CFolderDialogImpl<T>::DoModal(HWND hWndParent = ::GetActiveWindow())
{
    if (m_browseInfo.hwndOwner == NULL)
        m_browseInfo.hwndOwner = hWndParent;

    INT_PTR nResult = IDCANCEL;
    if (LPITEMIDLIST pItemIDList = ::SHBrowseForFolder(&m_browseInfo))
    {
        if (::SHGetPathFromIDList(pItemIDList, m_szPath))
        {
            nResult = IDOK;
            LPMALLOC pMalloc = NULL;

            if (::SHGetMalloc(&pMalloc) == NOERROR)
            {
                pMalloc->Free(pItemIDList);
                pMalloc->Release();
            }
        }
    }

    return nResult;
}

template <typename T>
inline int CFolderDialogImpl<T>::GetImage() const
{
    return m_browseInfo.iImage;
}

template <typename T>
inline LPCTSTR CFolderDialogImpl<T>::GetDisplayName() const
{
    return m_szDisplayName;
}

template <typename T>
inline LPCTSTR CFolderDialogImpl<T>::GetSelectedPath() const
{
    return m_szPath;
}

template <typename T>
inline void CFolderDialogImpl<T>::SetInitFolder(LPCTSTR pszInitFolder)
{
    ATLASSERT(pszInitFolder);
    ::lstrcpyn(m_szPath, pszInitFolder, _countof(m_szPath));
}

template <typename T>
inline BROWSEINFO& CFolderDialogImpl<T>::GetBrowseInfo()
{
    return m_browseInfo;
}

template <typename T>
inline const BROWSEINFO& CFolderDialogImpl<T>::GetBrowseInfo() const
{
    return m_browseInfo;
}

template <typename T>
inline void CFolderDialogImpl<T>::OnInitialized(HWND /*hWnd*/)
{
}

template <typename T>
inline void CFolderDialogImpl<T>::OnIUnknown(HWND /*hWnd*/, IUnknown* /*pUnk*/)
{
}

template <typename T>
inline void CFolderDialogImpl<T>::OnSelChanged(HWND /*hWnd*/, LPITEMIDLIST /*pItemIDList*/)
{
}

template <typename T>
inline int CFolderDialogImpl<T>::OnValidateFailed(HWND /*hWnd*/, LPCTSTR /*pszFolderPath*/)
{
    return 1;   // 1 = continue, 0 = EndDialog
}

template <typename T>
inline int CALLBACK CFolderDialogImpl<T>::FolderDialogProc(HWND hWnd, UINT uMsg, LPARAM lParam, LPARAM lData)
{
    T* pThis = reinterpret_cast<T*>(lData);
    ATLASSERT(pThis);
    int nResult = 0;

    switch (uMsg)
    {
    case BFFM_INITIALIZED:
        if (pThis->m_szPath[0] != _T('\0'))
            ::SendMessage(hWnd, BFFM_SETSELECTION, TRUE, reinterpret_cast<LPARAM>(pThis->m_szPath));
        pThis->OnInitialized(hWnd);
        break;

    case BFFM_IUNKNOWN:
        pThis->OnIUnknown(hWnd, reinterpret_cast<IUnknown*>(lParam));
        break;

    case BFFM_SELCHANGED:
        pThis->OnSelChanged(hWnd, reinterpret_cast<LPITEMIDLIST>(lParam));
        break;

    case BFFM_VALIDATEFAILED:
        nResult = pThis->OnValidateFailed(hWnd, reinterpret_cast<LPTSTR>(lParam));
        break;

    default:
        ATLTRACE2(atlTraceUI, 0, _T("Unknown message received in CFolderDialogImpl::FolderDialogProc\n"));
    }

    return nResult;
}

}  // namespace atlctrls

#endif  // __ATLCOMDLGS_INL__