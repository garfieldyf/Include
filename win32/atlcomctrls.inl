///////////////////////////////////////////////////////////////////////////////
// atlcomctrls.inl
//
// Author : Garfield
// Version : 5.0
// Creation Date : 2008/2/4

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __ATLCOMCTRLS_INL__
#define __ATLCOMCTRLS_INL__

#ifndef __ATLCOMCTRLS_H__
    #error atlcomctrls.inl requires atlcomctrls.h to be included first
#endif

namespace atlctrls {

#ifdef __IMAGE_H__
///////////////////////////////////////////////////////////////////////////////
// Implementation of the CImageCtrlImpl class
//

template <typename T, typename TBase, typename TWinTraits>
inline CImageCtrlImpl<T, TBase, TWinTraits>::CImageCtrlImpl()
{
}

template <typename T, typename TBase, typename TWinTraits>
inline LRESULT CImageCtrlImpl<T, TBase, TWinTraits>::OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    PAINTSTRUCT paint;
    static_cast<T*>(this)->DoPaint(m_hWnd, BeginPaint(&paint));
    ATLVERIFY(::EndPaint(m_hWnd, &paint));

    return 0L;
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CAnimImageCtrlImpl class
//

template <typename T, typename TBase, typename TWinTraits>
inline CAnimImageCtrlImpl<T, TBase, TWinTraits>::CAnimImageCtrlImpl()
{
}

template <typename T, typename TBase, typename TWinTraits>
inline BOOL CAnimImageCtrlImpl<T, TBase, TWinTraits>::Play()
{
    ATLASSERT(IsWindow());
    ATLASSERT(!m_AnimImage.IsEmpty());

    if (!m_bPlaying && m_AnimImage.IsAnimation())
        m_bPlaying = (SetTimer(IDT_TIMER, 0) == IDT_TIMER);

    return m_bPlaying;
}

template <typename T, typename TBase, typename TWinTraits>
inline void CAnimImageCtrlImpl<T, TBase, TWinTraits>::Stop()
{
    ATLASSERT(IsWindow());

    Pause();
    m_nFrameIndex = 0;
}

template <typename T, typename TBase, typename TWinTraits>
inline void CAnimImageCtrlImpl<T, TBase, TWinTraits>::Pause()
{
    ATLASSERT(IsWindow());

    if (m_bPlaying)
        m_bPlaying = !KillTimer(IDT_TIMER);
}

template <typename T, typename TBase, typename TWinTraits>
inline BOOL CAnimImageCtrlImpl<T, TBase, TWinTraits>::SizeToContent()
{
    ATLASSERT(IsWindow());
    ATLASSERT(!m_AnimImage.IsEmpty());

    BITMAP bmp;
    ATLVERIFY(m_AnimImage.GetBitmap(bmp));

    return SetWindowPos(NULL, 0, 0, bmp.bmWidth, bmp.bmHeight, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
}

template <typename T, typename TBase, typename TWinTraits>
inline LRESULT CAnimImageCtrlImpl<T, TBase, TWinTraits>::OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);

    PAINTSTRUCT paint;
    HDC hDC = BeginPaint(&paint);
    ATLASSERT(hDC);

    pThis->OnDrawBkgnd(hDC, paint.rcPaint);
    pThis->DoPaint(hDC, paint.rcPaint);

    ATLVERIFY(::EndPaint(m_hWnd, &paint));
    return 0L;
}

template <typename T, typename TBase, typename TWinTraits>
inline LRESULT CAnimImageCtrlImpl<T, TBase, TWinTraits>::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
    Stop();
    m_AnimImage.Destroy();
    m_AnimImage.m_Property.Destroy();

    return bHandled = FALSE;
}
#endif  // __IMAGE_H__


#ifdef __PICTURE_H__
///////////////////////////////////////////////////////////////////////////////
// Implementation of the CPictureCtrlImpl class
//

template <typename T, typename TBase, typename TWinTraits>
inline CPictureCtrlImpl<T, TBase, TWinTraits>::CPictureCtrlImpl()
{
}

template <typename T, typename TBase, typename TWinTraits>
inline LRESULT CPictureCtrlImpl<T, TBase, TWinTraits>::OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    PAINTSTRUCT paint;
    static_cast<T*>(this)->DoPaint(m_hWnd, BeginPaint(&paint));
    ATLVERIFY(::EndPaint(m_hWnd, &paint));

    return 0L;
}
#endif  // __PICTURE_H__


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CEditImpl class
//

template <typename T>
inline CEditImpl<T>::CEditImpl()
{
}

template <typename T>
inline UINT CEditImpl<T>::GetState() const
{
    return m_nState;
}

template <typename T>
inline BOOL CEditImpl<T>::SetState(UINT nState)
{
    return CEditBase::SetState(static_cast<T*>(this)->m_hWnd, nState);
}

template <typename T>
inline LRESULT CEditImpl<T>::OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);
    ATLASSERT(::IsWindow(pThis->m_hWnd));

    HDC hDC = ::GetWindowDC(pThis->m_hWnd);
    ATLASSERT(hDC);

    RECT rcWindow;
    ATLVERIFY(stdutil::ClipClientRect(pThis->m_hWnd, hDC, rcWindow));

    if (!pThis->OnDrawFrame(hDC, m_nState, rcWindow))
        pThis->DefWindowProc(uMsg, wParam, lParam);

    ATLVERIFY(::ReleaseDC(pThis->m_hWnd, hDC));
    return 0L;
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CCueEditImpl class
//

template <typename T>
inline CCueEditImpl<T>::CCueEditImpl()
#ifdef _ATL_SUPPORT_CCE_TEXTCOLOR
    : m_clrText(::GetSysColor(COLOR_WINDOWTEXT))
    , m_clrCueText(::GetSysColor(COLOR_GRAYTEXT))
#endif  // _ATL_SUPPORT_CCE_TEXTCOLOR
{
}

template <typename T>
inline void CCueEditImpl<T>::SetCueText(ATL::_U_STRINGorID text)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);
    ATLASSERT(::IsWindow(pThis->m_hWnd));

    m_strCueText = CString(text.m_lpstr);
    if (!::SendMessage(pThis->m_hWnd, EM_GETMODIFY, 0, 0L))
        pThis->DefWindowProc(WM_SETTEXT, 0, (LPARAM)(LPCTSTR)m_strCueText);
}

template <typename T>
inline LRESULT CCueEditImpl<T>::OnSetText(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);
    ATLASSERT(::IsWindow(pThis->m_hWnd));

    BOOL bEmpty = stdutil::StringIsEmpty((LPCTSTR)lParam);
    LRESULT lResult = pThis->DefWindowProc(uMsg, wParam, (bEmpty ? (LPARAM)(LPCTSTR)m_strCueText : lParam));
    ::SendMessage(pThis->m_hWnd, EM_SETMODIFY, !bEmpty, 0L);

    return lResult;
}

template <typename T>
inline LRESULT CCueEditImpl<T>::OnGetText(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);
    ATLASSERT(::IsWindow(pThis->m_hWnd));

    return (::SendMessage(pThis->m_hWnd, EM_GETMODIFY, 0, 0L) ? pThis->DefWindowProc(uMsg, wParam, lParam) : (*((LPTSTR)lParam) = _T('\0')));
}

template <typename T>
inline LRESULT CCueEditImpl<T>::OnSetFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);
    ATLASSERT(::IsWindow(pThis->m_hWnd));

    if (!::SendMessage(pThis->m_hWnd, EM_GETMODIFY, 0, 0L))
        pThis->DefWindowProc(WM_SETTEXT, 0, 0L);

    return bHandled = FALSE;
}

template <typename T>
inline LRESULT CCueEditImpl<T>::OnCtlColor(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);
    ATLASSERT(wParam);
    ATLASSERT(::IsWindow(pThis->m_hWnd));

#ifdef _ATL_SUPPORT_CCE_TEXTCOLOR
    ::SetTextColor((HDC)wParam, ::SendMessage(pThis->m_hWnd, EM_GETMODIFY, 0, 0L) ? m_clrText : m_clrCueText);
#else
    ::SetTextColor((HDC)wParam, ::GetSysColor(::SendMessage(pThis->m_hWnd, EM_GETMODIFY, 0, 0L) ? COLOR_WINDOWTEXT : COLOR_GRAYTEXT));
#endif  // _ATL_SUPPORT_CCE_TEXTCOLOR

    return (COLOR_WINDOW + 1);
}

template <typename T>
inline LRESULT CCueEditImpl<T>::OnKillFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);
    ATLASSERT(::IsWindow(pThis->m_hWnd));

    if (pThis->DefWindowProc(WM_GETTEXTLENGTH, 0, 0L) == 0)
        pThis->DefWindowProc(WM_SETTEXT, 0, (LPARAM)(LPCTSTR)m_strCueText);

    return bHandled = FALSE;
}

template <typename T>
inline LRESULT CCueEditImpl<T>::OnGetTextLength(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);
    ATLASSERT(::IsWindow(pThis->m_hWnd));

    return (::SendMessage(pThis->m_hWnd, EM_GETMODIFY, 0, 0L) ? pThis->DefWindowProc(uMsg, wParam, lParam) : 0L);
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CLinkCtrlImpl class
//

template <typename T, typename TBase, typename TWinTraits>
inline CLinkCtrlImpl<T, TBase, TWinTraits>::CLinkCtrlImpl(DWORD dwExStyle/* = 0*/)
    : CLinkCtrlBase(dwExStyle)
{
}

template <typename T, typename TBase, typename TWinTraits>
inline BOOL CLinkCtrlImpl<T, TBase, TWinTraits>::SubclassWindow(HWND hWnd)
{
    ATLASSERT(::IsWindow(hWnd));

    BOOL bSuccessful = ATL::CWindowImpl<T, TBase, TWinTraits>::SubclassWindow(hWnd);
    if (bSuccessful)
    {
        ATLVERIFY(CLinkCtrlBase::CreateFont(GetFont()));
        SetWindowLong(GWL_STYLE, GetWindowLong(GWL_STYLE) | TWinTraits::GetWndStyle(0));
    }

    return bSuccessful;
}

template <typename T, typename TBase, typename TWinTraits>
inline BOOL CLinkCtrlImpl<T, TBase, TWinTraits>::SizeToContent()
{
    ATLASSERT(IsWindow());
    return CLinkCtrlBase::SizeToContent(m_hWnd);
}

template <typename T, typename TBase, typename TWinTraits>
inline LRESULT CLinkCtrlImpl<T, TBase, TWinTraits>::OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);

    PAINTSTRUCT paint;
    HDC hDC = BeginPaint(&paint);
    ATLASSERT(hDC);

    pThis->OnDrawBkgnd(m_hWnd, hDC, paint.rcPaint);
    pThis->DoPaint(m_hWnd, hDC, paint.rcPaint);

    ATLVERIFY(::EndPaint(m_hWnd, &paint));
    return 0L;
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CLinkCtrl class
//

inline CLinkCtrl::CLinkCtrl(DWORD dwExStyle/* = 0*/)
    : CLinkCtrlImpl<CLinkCtrl>(dwExStyle)
{
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CSplashWindowImpl class
//

template <typename T, typename TBase, typename TWinTraits>
inline CSplashWindowImpl<T, TBase, TWinTraits>::CSplashWindowImpl()
{
}

template <typename T, typename TBase, typename TWinTraits>
inline HWND CSplashWindowImpl<T, TBase, TWinTraits>::Create(HBITMAP hBitmap, COLORREF clrMask/* = CLR_INVALID*/, HWND hWndParent/* = NULL*/)
{
    ATLASSERT(hBitmap);
    ATLASSERT(m_hWnd == NULL);
    ATLASSERT(m_hBitmap == NULL);

    BITMAP bmp;
    m_hBitmap = hBitmap;
    ATLVERIFY(::GetObject(hBitmap, sizeof(BITMAP), &bmp));

    RECT rect = { 0, 0, bmp.bmWidth, bmp.bmHeight };
    if (ATL::CWindowImpl<T, TBase, TWinTraits>::Create(hWndParent, rect))
    {
        ATLVERIFY(CenterWindow());
        if (clrMask != CLR_INVALID)
            ATLVERIFY(SetWindowRgn(stdutil::CreateRegion(hBitmap, clrMask), FALSE));
    }

    return m_hWnd;
}

template <typename T, typename TBase, typename TWinTraits>
inline void CSplashWindowImpl<T, TBase, TWinTraits>::SetDrawText(POSITION pos, UINT nIDText/* = 0*/, COLORREF clrText/* = CLR_INVALID*/, LPLOGFONT pLogFont/* = NULL*/, UINT nFormat/* = (UINT)-1*/, LPCRECT pRect/* = NULL*/)
{
    ATLASSERT(IsWindow());
    ATLASSERT(pos != NULL);

    CSplashWindowBase::SetDrawText(m_hWnd, pos, CString(MAKEINTRESOURCE(nIDText)), clrText, pLogFont, nFormat, pRect);
}

template <typename T, typename TBase, typename TWinTraits>
inline void CSplashWindowImpl<T, TBase, TWinTraits>::SetDrawText(POSITION pos, LPCTSTR pszText/* = NULL*/, COLORREF clrText/* = CLR_INVALID*/, LPLOGFONT pLogFont/* = NULL*/, UINT nFormat/* = (UINT)-1*/, LPCRECT pRect/* = NULL*/)
{
    ATLASSERT(IsWindow());
    ATLASSERT(pos != NULL);

    CSplashWindowBase::SetDrawText(m_hWnd, pos, pszText, clrText, pLogFont, nFormat, pRect);
}

template <typename T, typename TBase, typename TWinTraits>
inline LRESULT CSplashWindowImpl<T, TBase, TWinTraits>::OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    PAINTSTRUCT paint;
    static_cast<T*>(this)->DoPaint(m_hWnd, BeginPaint(&paint));
    ATLVERIFY(::EndPaint(m_hWnd, &paint));

    return 0L;
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CBalloonTipImpl class
//

template <typename T>
inline CBalloonTipImpl<T>::CBalloonTipImpl()
{
}

template <typename T>
inline HWND CBalloonTipImpl<T>::BalloonTip_Create(DWORD dwStyle/* = TTS_BALLOON*/, int nMaxWidth/* = 300*/)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);
    ATLASSERT(::IsWindow(pThis->m_hWnd));

    return m_wndBalloon.Create(pThis->m_hWnd, dwStyle, nMaxWidth);
}

template <typename T>
inline void CBalloonTipImpl<T>::BalloonTip_ShowWindow(HWND hWnd, ATL::_U_STRINGorID title/* = (LPCTSTR)NULL*/, UINT_PTR nIcon/* = TTI_WARNING*/, int x/* = -1*/, int y/* = -1*/, UINT nElapse/* = 5000*/)
{
    CBalloonTipBase::BalloonTip_ShowWindow(static_cast<T*>(this)->m_hWnd, hWnd, title, nIcon, x, y, nElapse);
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CDragDropImpl class
//

template <typename T>
inline CDragDropImpl<T>::CDragDropImpl()
{
}

template <typename T>
inline LRESULT CDragDropImpl<T>::OnBeginDrag(int /*nIDCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);
    ATLASSERT(m_hDragImage == NULL);
    ATLASSERT(::IsWindow(pThis->m_hWnd));

    POINT ptDrag, ptHotSpot;
    if ((m_hDragImage = pThis->CreateDragImageEx(pNMHDR, ptDrag, ptHotSpot)) != NULL)
        BeginDrag(pThis->m_hWnd, ptDrag, ptHotSpot);

    return 0L;
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CDropFilesImpl class
//

template <typename T>
inline CDropFilesImpl<T>::CDropFilesImpl()
{
}

template <typename T>
inline void CDropFilesImpl<T>::DropFiles_OnDropFile(LPCTSTR pszFileName, UINT nLength)
{
    UNUSED_PARAMETER(nLength);
    UNUSED_PARAMETER(pszFileName);
    ATLTRACE(_T("CDropFilesImpl<T> - filename length = %u, filename = %s\n"), nLength, pszFileName);
}

template <typename T>
inline void CDropFilesImpl<T>::DropFiles_OnDropFiles(HDROP hDropInfo, UINT nFileCount)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);
    ATLASSERT(hDropInfo);
    ATLASSERT(::IsWindow(pThis->m_hWnd));
    ATLTRACE(_T("CDropFilesImpl<T> - file count = %u\n"), nFileCount);

    TCHAR szFileName[MAX_PATH];
    for (UINT i = 0; i < nFileCount; ++i)
        pThis->DropFiles_OnDropFile(szFileName, ::DragQueryFile(hDropInfo, i, szFileName, _countof(szFileName)));
}

template <typename T>
inline void CDropFilesImpl<T>::DropFiles_DragAcceptFiles(BOOL bAccept/* = TRUE*/)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);
    ATLASSERT(::IsWindow(pThis->m_hWnd));

    ::DragAcceptFiles(pThis->m_hWnd, bAccept);
}

template <typename T>
inline LRESULT CDropFilesImpl<T>::OnDropFiles(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);
    ATLASSERT(::IsWindow(pThis->m_hWnd));

    pThis->DropFiles_OnDropFiles((HDROP)wParam, ::DragQueryFile((HDROP)wParam, 0xFFFFFFFF, NULL, 0));
    ::DragFinish((HDROP)wParam);

    return 0L;
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CTabSheetImpl class
//

template <typename T>
inline CTabSheetImpl<T>::CTabSheetImpl()
{
}

template <typename T>
inline BOOL CTabSheetImpl<T>::PreTranslateMessage(LPMSG pMsg)
{
    return ::IsDialogMessage(GetActivePage(), pMsg);
}

template <typename T>
inline void CTabSheetImpl<T>::SetPageRect(HWND hWndPage, int /*nPage*/, const RECT& rect)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);
    ATLASSERT(::IsWindow(hWndPage));
    ATLASSERT(::IsWindow(pThis->m_hWnd));

    RECT rcPage = rect;
    TabCtrl_AdjustRect(pThis->m_hWnd, FALSE, &rcPage);

    ATLVERIFY(::SetWindowPos(hWndPage, NULL, rcPage.left - 1, rcPage.top, rcPage.right - rcPage.left + 1, rcPage.bottom - rcPage.top, SWP_NOZORDER | SWP_NOREDRAW | SWP_NOACTIVATE));
}

template <typename T>
inline int CTabSheetImpl<T>::AddPage(HWND hWndPage, ATL::_U_STRINGorID title, int nImage/* = -1*/, LPARAM lParam/* = 0L*/)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);
    ATLASSERT(::IsWindow(hWndPage));

    return InsertPage(TabCtrl_GetItemCount(pThis->m_hWnd), hWndPage, title, nImage, lParam);
}

template <typename T>
inline int CTabSheetImpl<T>::InsertPage(int nPage, HWND hWndPage, ATL::_U_STRINGorID title, int nImage/* = -1*/, LPARAM lParam/* = 0L*/)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);
    ATLASSERT(::IsWindow(hWndPage));
    ATLASSERT(::IsWindow(pThis->m_hWnd));

    CString strTitle(title.m_lpstr);
    TSITEM item = { { TCIF_TEXT | TCIF_PARAM, 0, 0, (LPTSTR)(LPCTSTR)strTitle, 0, nImage }, hWndPage, lParam };
    if (nImage != -1)
        item.tciHeader.mask |= TCIF_IMAGE;

    if ((nPage = TabCtrl_InsertItem(pThis->m_hWnd, nPage, &item)) != -1)
        pThis->SetPageRect(hWndPage, nPage, sysutil::CClientRect(pThis->m_hWnd));

    return nPage;
}

template <typename T>
inline BOOL CTabSheetImpl<T>::SetPageExtra()
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);
    ATLASSERT(::IsWindow(pThis->m_hWnd));
    ATLASSERT(TabCtrl_GetItemCount(pThis->m_hWnd) == 0);

    return TabCtrl_SetItemExtra(pThis->m_hWnd, TCITEM_EXTRA_SIZE);
}

template <typename T>
inline HWND CTabSheetImpl<T>::GetActivePage() const
{
    const T* pThis = static_cast<const T*>(this);
    ATLASSERT(pThis);
    ATLASSERT(::IsWindow(pThis->m_hWnd));

    return GetPage(TabCtrl_GetCurSel(pThis->m_hWnd));
}

template <typename T>
inline HWND CTabSheetImpl<T>::GetPage(int nPage) const
{
    const T* pThis = static_cast<const T*>(this);
    ATLASSERT(pThis);
    ATLASSERT(::IsWindow(pThis->m_hWnd));

    TSITEM item;
    item.tciHeader.mask = TCIF_PARAM, item.hWnd = NULL;
    TabCtrl_GetItem(pThis->m_hWnd, nPage, &item);

    return item.hWnd;
}

template <typename T>
inline BOOL CTabSheetImpl<T>::SetPage(int nPage, HWND hWnd)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);
    ATLASSERT(::IsWindow(pThis->m_hWnd));
    ATLASSERT(nPage >= 0 && nPage < TabCtrl_GetItemCount(pThis->m_hWnd));

    TSITEM item;
    item.tciHeader.mask = TCIF_PARAM;
    ATLVERIFY(TabCtrl_GetItem(pThis->m_hWnd, nPage, &item));
    item.hWnd = hWnd;

    return TabCtrl_SetItem(pThis->m_hWnd, nPage, &item);
}

template <typename T>
inline int CTabSheetImpl<T>::GetPageImage(int nPage) const
{
    const T* pThis = static_cast<const T*>(this);
    ATLASSERT(pThis);
    ATLASSERT(::IsWindow(pThis->m_hWnd));
    ATLASSERT(nPage >= 0 && nPage < TabCtrl_GetItemCount(pThis->m_hWnd));

    TSITEM item;
    item.tciHeader.mask = TCIF_IMAGE, item.tciHeader.iImage = -1;
    ATLVERIFY(TabCtrl_GetItem(pThis->m_hWnd, nPage, &item));

    return item.tciHeader.iImage;
}

template <typename T>
inline BOOL CTabSheetImpl<T>::SetPageImage(int nPage, int nImage)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);
    ATLASSERT(::IsWindow(pThis->m_hWnd));
    ATLASSERT(nPage >= 0 && nPage < TabCtrl_GetItemCount(pThis->m_hWnd));

    TSITEM item;
    item.tciHeader.iImage = nImage;
    item.tciHeader.mask = TCIF_IMAGE;

    return TabCtrl_SetItem(pThis->m_hWnd, nPage, &item);
}

template <typename T>
inline DWORD_PTR CTabSheetImpl<T>::GetPageData(int nPage) const
{
    const T* pThis = static_cast<const T*>(this);
    ATLASSERT(pThis);
    ATLASSERT(::IsWindow(pThis->m_hWnd));
    ATLASSERT(nPage >= 0 && nPage < TabCtrl_GetItemCount(pThis->m_hWnd));

    TSITEM item;
    item.tciHeader.mask = TCIF_PARAM, item.lParam = 0L;
    ATLVERIFY(TabCtrl_GetItem(pThis->m_hWnd, nPage, &item));

    return item.lParam;
}

template <typename T>
inline BOOL CTabSheetImpl<T>::SetPageData(int nPage, DWORD_PTR dwData)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);
    ATLASSERT(::IsWindow(pThis->m_hWnd));
    ATLASSERT(nPage >= 0 && nPage < TabCtrl_GetItemCount(pThis->m_hWnd));

    TSITEM item;
    item.tciHeader.mask = TCIF_PARAM;
    ATLVERIFY(TabCtrl_GetItem(pThis->m_hWnd, nPage, &item));
    item.lParam = (LPARAM)dwData;

    return TabCtrl_SetItem(pThis->m_hWnd, nPage, &item);
}

template <typename T>
inline CString CTabSheetImpl<T>::GetPageTitle(int nPage) const
{
    const T* pThis = static_cast<const T*>(this);
    ATLASSERT(pThis);
    ATLASSERT(::IsWindow(pThis->m_hWnd));
    ATLASSERT(nPage >= 0 && nPage < TabCtrl_GetItemCount(pThis->m_hWnd));

    TSITEM item;
    CString strTitle;
    item.tciHeader.pszText = strTitle.GetBuffer(MAX_PATH);
    item.tciHeader.mask = TCIF_TEXT, item.tciHeader.cchTextMax = MAX_PATH;

    ATLVERIFY(TabCtrl_GetItem(pThis->m_hWnd, nPage, &item));
    strTitle.ReleaseBuffer();

    return strTitle;
}

template <typename T>
inline HRESULT CTabSheetImpl<T>::GetPageTitle(int nPage, BSTR& bstrTitle) const
{
    ::SysFreeString(bstrTitle);
    bstrTitle = GetPageTitle(nPage).AllocSysString();

    return (bstrTitle != NULL ? S_OK : E_OUTOFMEMORY);
}

template <typename T>
inline BOOL CTabSheetImpl<T>::SetPageTitle(int nPage, ATL::_U_STRINGorID title)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);
    ATLASSERT(::IsWindow(pThis->m_hWnd));
    ATLASSERT(nPage >= 0 && nPage < TabCtrl_GetItemCount(pThis->m_hWnd));

    TSITEM item;
    CString strTitle(title.m_lpstr);
    item.tciHeader.mask    = TCIF_TEXT;
    item.tciHeader.pszText = (LPTSTR)(LPCTSTR)strTitle;

    return TabCtrl_SetItem(pThis->m_hWnd, nPage, &item);
}

template <typename T>
inline LRESULT CTabSheetImpl<T>::OnSelChange(int /*nIDCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/)
{
    ATLASSERT(::IsWindow(pNMHDR->hwndFrom));

    TabCtrl_SetCurSel(pNMHDR->hwndFrom, TabCtrl_GetCurSel(pNMHDR->hwndFrom));
    return 0L;
}

template <typename T>
inline LRESULT CTabSheetImpl<T>::OnSize(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
    if (wParam != SIZE_MINIMIZED)
    {
        T* pThis = static_cast<T*>(this);
        ATLASSERT(pThis);
        ATLASSERT(::IsWindow(pThis->m_hWnd));

        RECT rcClient = { 0, 0, LOWORD(lParam), HIWORD(lParam) };
        for (int i = TabCtrl_GetItemCount(pThis->m_hWnd) - 1; i >= 0; --i)
            pThis->SetPageRect(GetPage(i), i, rcClient);
    }

    return 0L;
}

template <typename T>
inline LRESULT CTabSheetImpl<T>::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
    ATLVERIFY(SetPageExtra());
    return bHandled = FALSE;
}

template <typename T>
inline LRESULT CTabSheetImpl<T>::OnSetCurSel(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);
    ATLASSERT(::IsWindow(pThis->m_hWnd));

    ::ShowWindow(::GetTopWindow(pThis->m_hWnd), SW_HIDE);
    if (HWND hWndPage = GetPage((int)wParam))
    {
        ATLVERIFY(::SetWindowPos(hWndPage, HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_SHOWWINDOW));
        ATLVERIFY(::UpdateWindow(hWndPage));
        ::SetFocus(hWndPage);
    }

    return pThis->DefWindowProc(uMsg, wParam, lParam);
}

template <typename T>
inline LRESULT CTabSheetImpl<T>::OnDeleteItem(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);
    ATLASSERT(::IsWindow(pThis->m_hWnd));

    int nCurSel = TabCtrl_GetCurSel(pThis->m_hWnd);
    LRESULT lResult = pThis->DefWindowProc(uMsg, wParam, lParam);
    if (lResult && nCurSel == (int)wParam)
        TabCtrl_SetCurSel(pThis->m_hWnd, (nCurSel == 0 ? 0 : nCurSel - 1));

    return lResult;
}

template <typename T>
inline LRESULT CTabSheetImpl<T>::OnDeleteAllItems(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);
    ATLASSERT(::IsWindow(pThis->m_hWnd));

    LRESULT lResult = pThis->DefWindowProc(uMsg, wParam, lParam);
    if (lResult)
        ::ShowWindow(::GetTopWindow(pThis->m_hWnd), SW_HIDE);

    return lResult;
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CDialogBarImpl class
//

template <typename T>
inline CDialogBarImpl<T>::CDialogBarImpl()
{
}

template <typename T>
inline LRESULT CDialogBarImpl<T>::OnMove(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);
    ATLASSERT(::IsWindow(pThis->m_hWnd));

    ATLVERIFY(::InvalidateRect(pThis->m_hWnd, NULL, TRUE));
    return 0L;
}

template <typename T>
inline LRESULT CDialogBarImpl<T>::OnEraseBkgnd(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);
    ATLASSERT(::IsWindow(pThis->m_hWnd));

    HWND hWndParent = ::GetParent(pThis->m_hWnd);
    ATLASSERT(::IsWindow(hWndParent));

    POINT ptOriginal, point = { 0 };
    ::MapWindowPoints(pThis->m_hWnd, hWndParent, &point, 1);

    ATLVERIFY(::OffsetWindowOrgEx((HDC)wParam, point.x, point.y, &ptOriginal));
    LRESULT lResult = ::SendMessage(hWndParent, WM_ERASEBKGND, wParam, 0L);
    ATLVERIFY(::SetWindowOrgEx((HDC)wParam, ptOriginal.x, ptOriginal.y, NULL));

    return lResult;
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CReBarCtrlImpl class
//

template <typename T>
inline CReBarCtrlImpl<T>::CReBarCtrlImpl()
{
}

template <typename T>
inline void CReBarCtrlImpl<T>::OnDrawBand(const RBDRAWBAND& rbdb)
{
    ATLASSERT(rbdb.hDC);

    if (rbdb.hbmBack == NULL)
    {
        stdutil::GradientFillRect(rbdb.hDC, rbdb.rcBand, rbdb.clrFrom, rbdb.clrTo, GRADIENT_FILL_RECT_V);
    }
    else
    {
        BITMAP bmp;
        ATLVERIFY(::GetObject(rbdb.hbmBack, sizeof(BITMAP), &bmp));

        ::SetStretchBltMode(rbdb.hDC, COLORONCOLOR);
        ATLVERIFY(stdutil::StretchBitmap(rbdb.hDC, rbdb.rcBand, rbdb.hbmBack, 0, 0, bmp.bmWidth, bmp.bmHeight));
    }
}

template <typename T>
inline BOOL CReBarCtrlImpl<T>::AddBar(HWND hWndBand, HBITMAP hbmBack, UINT nStyle/* = RBBS_BREAK | RBBS_CHILDEDGE | RBBS_NOGRIPPER | RBBS_USECHEVRON*/, LPCTSTR pszTitle/* = NULL*/, int nImage/* = -1*/, LPARAM lParam/* = 0L*/)
{
    ATLASSERT(hbmBack);
    ATLASSERT(::IsWindow(hWndBand));

    REBARBANDINFO rbBand = { sizeof(REBARBANDINFO) };
    rbBand.fStyle  = nStyle;
    rbBand.iImage  = nImage;
    rbBand.lParam  = lParam;
    rbBand.hbmBack = hbmBack;
    rbBand.hwndChild = hWndBand;
    rbBand.lpText  = (LPTSTR)pszTitle;
    rbBand.fMask   = RBBIM_ID | RBBIM_CHILD | RBBIM_CHILDSIZE | RBBIM_STYLE | RBBIM_SIZE | RBBIM_IDEALSIZE | RBBIM_BACKGROUND | RBBIM_LPARAM;

    BOOL bSuccessful = _AddBar(hWndBand, rbBand, pszTitle, nImage);
    if (bSuccessful)
        ::SendMessage(hWndBand, TB_SETEXTENDEDSTYLE, 0, ::SendMessage(hWndBand, TB_GETEXTENDEDSTYLE, 0, 0L) | TBSTYLE_EX_HIDECLIPPEDBUTTONS);

    return bSuccessful;
}

template <typename T>
inline BOOL CReBarCtrlImpl<T>::AddBar(HWND hWndBand, COLORREF clrFrom, COLORREF clrTo, UINT nStyle/* = RBBS_BREAK | RBBS_CHILDEDGE | RBBS_NOGRIPPER | RBBS_USECHEVRON*/, LPCTSTR pszTitle/* = NULL*/, int nImage/* = -1*/, LPARAM lParam/* = 0L*/)
{
    ATLASSERT(::IsWindow(hWndBand));

    REBARBANDINFO rbBand = { sizeof(REBARBANDINFO) };
    rbBand.fStyle  = nStyle;
    rbBand.clrFore = clrFrom;
    rbBand.clrBack = clrTo;
    rbBand.iImage  = nImage;
    rbBand.lParam  = lParam;
    rbBand.hwndChild = hWndBand;
    rbBand.lpText  = (LPTSTR)pszTitle;
    rbBand.fMask   = RBBIM_ID | RBBIM_CHILD | RBBIM_CHILDSIZE | RBBIM_STYLE | RBBIM_SIZE | RBBIM_IDEALSIZE | RBBIM_COLORS | RBBIM_LPARAM;

    BOOL bSuccessful = _AddBar(hWndBand, rbBand, pszTitle, nImage);
    if (bSuccessful)
        ::SendMessage(hWndBand, TB_SETEXTENDEDSTYLE, 0, ::SendMessage(hWndBand, TB_GETEXTENDEDSTYLE, 0, 0L) | TBSTYLE_EX_HIDECLIPPEDBUTTONS);

    return bSuccessful;
}

template <typename T>
inline BOOL CReBarCtrlImpl<T>::_AddBar(HWND hWndBand, REBARBANDINFO& rbBand, LPCTSTR pszTitle, int nImage)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);
    ATLASSERT(::IsWindow(hWndBand));
    ATLASSERT(::IsWindow(pThis->m_hWnd));

    if (pszTitle != NULL)
        rbBand.fMask |= RBBIM_TEXT;
    if (nImage != -1)
        rbBand.fMask |= RBBIM_IMAGE;

    RECT rect;
    int nCount = (int)::SendMessage(hWndBand, TB_BUTTONCOUNT, 0, 0L);

    if (nCount > 0)
    {
        ATLVERIFY(::SendMessage(hWndBand, TB_GETITEMRECT, nCount - 1, (LPARAM)&rect));
        rbBand.cx = rect.right;
        rbBand.cyMinChild = rect.bottom - rect.top;

        if (pszTitle == NULL)
        {
            ATLVERIFY(::SendMessage(hWndBand, TB_GETITEMRECT, 0, (LPARAM)&rect));
            rbBand.cxMinChild = rect.right;
        }
    }
    else
    {
        ATLVERIFY(::GetWindowRect(hWndBand, &rect));
        rbBand.cx = rect.right - rect.left;
        rbBand.cyMinChild = rect.bottom - rect.top;
    }

    rbBand.cxIdeal = rbBand.cx;
    rbBand.wID = ATL_IDW_BAND_FIRST + ::SendMessage(pThis->m_hWnd, RB_GETBANDCOUNT, 0, 0L);

    return (BOOL)::SendMessage(pThis->m_hWnd, RB_INSERTBAND, (WPARAM)-1, (LPARAM)&rbBand);
}

template <typename T>
inline LRESULT CReBarCtrlImpl<T>::OnEraseBkgnd(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);
    ATLASSERT(::IsWindow(pThis->m_hWnd));

    REBARBANDINFO rbBand;
    rbBand.cbSize = sizeof(REBARBANDINFO);
    rbBand.fMask  = RBBIM_BACKGROUND | RBBIM_COLORS | RBBIM_CHILD | RBBIM_LPARAM;

    RBDRAWBAND rbDrawBand;
    rbDrawBand.hDC   = (HDC)wParam;
    rbDrawBand.nBand = (int)::SendMessage(pThis->m_hWnd, RB_GETBANDCOUNT, 0, 0L) - 1;

    for (; rbDrawBand.nBand >= 0; --rbDrawBand.nBand)
    {
        ATLVERIFY(::SendMessage(pThis->m_hWnd, RB_GETBANDINFO, rbDrawBand.nBand, (LPARAM)&rbBand));
        ATLASSERT(rbBand.hbmBack != NULL || (rbBand.clrFore != CLR_INVALID && rbBand.clrBack != CLR_INVALID));

        rbDrawBand.hbmBack   = rbBand.hbmBack;
        rbDrawBand.clrFrom   = rbBand.clrFore;
        rbDrawBand.clrTo     = rbBand.clrBack;
        rbDrawBand.lParam    = rbBand.lParam;
        rbDrawBand.hWndChild = rbBand.hwndChild;

        ATLVERIFY(::SendMessage(pThis->m_hWnd, RB_GETRECT, rbDrawBand.nBand, (LPARAM)&rbDrawBand.rcBand));
        if (rbDrawBand.rcBand.left <= 2)
            rbDrawBand.rcBand.left = 0;

        pThis->OnDrawBand(rbDrawBand);
    }

    return 1L;
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CMarginEditImpl class
//

template <typename T>
inline CMarginEditImpl<T>::CMarginEditImpl()
    : m_nMarginWidth(-1), m_bMouseHover(FALSE)
{
}

template <typename T>
inline BOOL CMarginEditImpl<T>::PreTranslateMessage(LPMSG pMsg)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);
    ATLASSERT(::IsWindow(pThis->m_hWnd));

    BOOL bSuccessful = (pThis->m_hWnd == pMsg->hwnd && pMsg->message == WM_CHAR && pMsg->wParam == VK_RETURN);
    if (bSuccessful)
    {
        int nCtrlID = ::GetDlgCtrlID(pThis->m_hWnd);
        NMMARGINEDIT nmme = { { pThis->m_hWnd, nCtrlID, NM_CLICK }, { 0 }, HTMARGIN };
        ::SendMessage(::GetParent(pThis->m_hWnd), WM_NOTIFY, nCtrlID, (LPARAM)&nmme);
    }

    return bSuccessful;
}

template <typename T>
inline void CMarginEditImpl<T>::CalcMarginRect(const RECT& rcWindow, RECT& rcMargin) const
{
    const T* pThis = static_cast<const T*>(this);
    ATLASSERT(pThis);
    ATLASSERT(m_nMarginWidth > 0);
    ATLASSERT(::IsWindow(pThis->m_hWnd));

    sysutil::CClientRect rcClient(pThis->m_hWnd);
    int dx = (rcWindow.right - rcWindow.left - rcClient.right - m_nMarginWidth) / 2;
    int dy = (rcWindow.bottom - rcWindow.top - rcClient.bottom) / 2;

    stdutil::SetRect(rcMargin, rcWindow.right - dx - m_nMarginWidth, rcWindow.top + dy, rcWindow.right - dx, rcWindow.bottom - dy);
}

template <typename T>
inline void CMarginEditImpl<T>::OnDrawMargin(HDC /*hDC*/, const RECT& /*rcWindow*/, const RECT& /*rcMargin*/)
{
    ATLASSERT(FALSE && _T("MUST BE IMPLEMENTED IN A DERIVED CLASS."));
}

template <typename T>
inline int CMarginEditImpl<T>::GetMarginWidth() const
{
    return m_nMarginWidth;
}

template <typename T>
inline BOOL CMarginEditImpl<T>::SetMarginWidth(int nWidth)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);
    ATLASSERT(::IsWindow(pThis->m_hWnd));
    ATLASSERT((::GetWindowLong(pThis->m_hWnd, GWL_STYLE) & ES_MULTILINE) == 0);

#ifdef _DEBUG
    sysutil::CClientRect rcClient(pThis->m_hWnd);
    ATLASSERT(nWidth > 0 && nWidth < rcClient.Width());
#endif  // _DEBUG

    m_nMarginWidth = nWidth;
    return ::SetWindowPos(pThis->m_hWnd, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_NOZORDER | SWP_DRAWFRAME);
}

template <typename T>
inline void CMarginEditImpl<T>::GetMarginRect(RECT& rect) const
{
    const T* pThis = static_cast<const T*>(this);
    ATLASSERT(pThis);
    ATLASSERT(::IsWindow(pThis->m_hWnd));

    sysutil::CWindowRect rcWindow(pThis->m_hWnd);
    pThis->CalcMarginRect(rcWindow, rect);
}

template <typename T>
inline LRESULT CMarginEditImpl<T>::OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);
    ATLASSERT(m_nMarginWidth > 0);
    ATLASSERT(::IsWindow(pThis->m_hWnd));

    HDC hDC = ::GetWindowDC(pThis->m_hWnd);
    ATLASSERT(hDC);

    RECT rcWindow;
    ATLVERIFY(stdutil::ClipClientRect(pThis->m_hWnd, hDC, rcWindow));

    if (!pThis->OnDrawFrame(hDC, m_nState, rcWindow))
        pThis->DefWindowProc(uMsg, wParam, lParam);

    RECT rcMargin;
    pThis->CalcMarginRect(rcWindow, rcMargin);
    pThis->OnDrawMargin(hDC, rcWindow, rcMargin);

    ATLVERIFY(::ReleaseDC(pThis->m_hWnd, hDC));
    return 0L;
}

template <typename T>
inline LRESULT CMarginEditImpl<T>::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);
    ATLASSERT(m_nMarginWidth > 0);
    ATLASSERT(::IsWindow(pThis->m_hWnd));

    LRESULT lResult = pThis->DefWindowProc(uMsg, wParam, lParam);
    return (lResult == HTNOWHERE ? HTMARGIN : lResult);
}

template <typename T>
inline LRESULT CMarginEditImpl<T>::OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);
    ATLASSERT(m_nMarginWidth > 0);
    ATLASSERT(::IsWindow(pThis->m_hWnd));

    LRESULT lResult = pThis->DefWindowProc(uMsg, wParam, lParam);
    if (wParam)
        reinterpret_cast<LPNCCALCSIZE_PARAMS>(lParam)->rgrc[0].right -= m_nMarginWidth;

    return lResult;
}

template <typename T>
inline LRESULT CMarginEditImpl<T>::OnNcMouseMove(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
{
    if (wParam == HTMARGIN && !m_bMouseHover)
    {
        T* pThis = static_cast<T*>(this);
        ATLASSERT(pThis);
        ATLASSERT(::IsWindow(pThis->m_hWnd));

        ATLVERIFY(SetState(CDIS_HOT));
        TRACKMOUSEEVENT tme = { sizeof(TRACKMOUSEEVENT), TME_NONCLIENT | TME_LEAVE, pThis->m_hWnd, 0 };
        m_bMouseHover = ::TrackMouseEvent(&tme);
    }

    return bHandled = FALSE;
}

template <typename T>
inline LRESULT CMarginEditImpl<T>::OnNcLButtonUp(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    if (wParam == HTMARGIN)
    {
        T* pThis = static_cast<T*>(this);
        ATLASSERT(pThis);
        ATLASSERT(::IsWindow(pThis->m_hWnd));

        int nCtrlID = ::GetDlgCtrlID(pThis->m_hWnd);
        ATLVERIFY(SetState(::GetFocus() == pThis->m_hWnd ? CDIS_HOT : 0));

        NMMARGINEDIT nmme = { { pThis->m_hWnd, nCtrlID, NM_CLICK }, { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) }, wParam };
        ::SendMessage(::GetParent(pThis->m_hWnd), WM_NOTIFY, nCtrlID, (LPARAM)&nmme);
    }

    return bHandled = FALSE;
}

template <typename T>
inline LRESULT CMarginEditImpl<T>::OnNcMouseLeave(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);
    ATLASSERT(::IsWindow(pThis->m_hWnd));

    ATLVERIFY(SetState(::GetFocus() == pThis->m_hWnd ? CDIS_HOT : 0));
    return bHandled = m_bMouseHover = FALSE;
}

template <typename T>
inline LRESULT CMarginEditImpl<T>::OnNcLButtonDown(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
{
    if (wParam == HTMARGIN)
        ATLVERIFY(SetState(CDIS_SELECTED));

    return bHandled = FALSE;
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CToolBarCtrlImpl class
//

template <typename T>
inline CToolBarCtrlImpl<T>::CToolBarCtrlImpl()
{
}

template <typename T>
inline LRESULT CToolBarCtrlImpl<T>::OnDrawButton(LPNMTBCUSTOMDRAW /*pTBCustomDraw*/)
{
    return CDRF_DODEFAULT;  // return TBCDRF_NOEDGES | TBCDRF_NOBACKGROUND | TBCDRF_NOOFFSET prevent default draw selected state
}

template <typename T>
inline BOOL CToolBarCtrlImpl<T>::LoadToolBar(UINT nIDResource, COLORREF clrMask/* = CLR_DEFAULT*/, int cxButtonMargin/* = 7*/, int cyButtonMargin/* = 7*/, int cxSeparator/* = 8*/)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);
    ATLASSERT(::IsWindow(pThis->m_hWnd));

    return (stdutil::LoadToolBar(pThis->m_hWnd, ATL::_AtlBaseModule.GetResourceInstance(), nIDResource, clrMask, cxButtonMargin, cyButtonMargin, cxSeparator) != NULL);
}

template <typename T>
inline void CToolBarCtrlImpl<T>::SetSizes(UINT cxImage, UINT cyImage, UINT cxButton, UINT cyButton)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);
    ATLASSERT(cxButton > cxImage);
    ATLASSERT(cyButton > cyImage);
    ATLASSERT(::IsWindow(pThis->m_hWnd));
    ATLASSERT(cxImage != 0 && cyImage != 0);
    ATLASSERT(cxButton != 0 && cyButton != 0);

    ATLVERIFY(::SendMessage(pThis->m_hWnd, TB_SETBITMAPSIZE, 0, MAKELPARAM(cxImage, cyImage)));
    ATLVERIFY(::SendMessage(pThis->m_hWnd, TB_SETBUTTONSIZE, 0, MAKELPARAM(cxButton, cyButton)));
}

template <typename T>
inline BOOL CToolBarCtrlImpl<T>::LoadButtons(const WORD* pIDArray, int nIDCount, int cxSeparator/* = 8*/)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);
    ATLASSERT(pIDArray);
    ATLASSERT(::IsWindow(pThis->m_hWnd));

    return stdutil::LoadButtons(pThis->m_hWnd, pIDArray, nIDCount, cxSeparator);
}

template <typename T>
inline LRESULT CToolBarCtrlImpl<T>::OnCustomDraw(int /*nIDCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/)
{
    LPNMTBCUSTOMDRAW pTBCustomDraw = reinterpret_cast<LPNMTBCUSTOMDRAW>(pNMHDR);
    ATLASSERT(pTBCustomDraw);
    ATLASSERT(pTBCustomDraw->nmcd.hdc);
    ATLASSERT(::IsWindow(pNMHDR->hwndFrom));

    LRESULT lResult = CDRF_DODEFAULT;
    switch (pTBCustomDraw->nmcd.dwDrawStage)
    {
    case CDDS_PREPAINT:
        lResult = CDRF_NOTIFYITEMDRAW;
        break;

    case CDDS_ITEMPREPAINT:
        lResult = static_cast<T*>(this)->OnDrawButton(pTBCustomDraw);
        break;
    }

    return lResult;
}

template <typename T>
inline LRESULT CToolBarCtrlImpl<T>::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);
    ATLASSERT(::IsWindow(pThis->m_hWnd));

    ::ImageList_Destroy((HIMAGELIST)::SendMessage(pThis->m_hWnd, TB_GETIMAGELIST, 0, 0L));
    ::ImageList_Destroy((HIMAGELIST)::SendMessage(pThis->m_hWnd, TB_GETHOTIMAGELIST, 0, 0L));
    ::ImageList_Destroy((HIMAGELIST)::SendMessage(pThis->m_hWnd, TB_GETDISABLEDIMAGELIST, 0, 0L));

#if (_WIN32_WINNT >= 0x0600)
    ::ImageList_Destroy((HIMAGELIST)::SendMessage(pThis->m_hWnd, TB_GETPRESSEDIMAGELIST, 0, 0L));
#endif  // (_WIN32_WINNT >= 0x0600)

    return bHandled = FALSE;
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CHeaderCtrlImpl class
//

template <typename T>
inline CHeaderCtrlImpl<T>::CHeaderCtrlImpl(int nHeight/* = -1*/)
    : CHeaderCtrlBase(nHeight)
{
}

#ifdef _ATL_SUPPORT_CHD_SETHEIGHT
template <typename T>
inline LRESULT CHeaderCtrlImpl<T>::OnLayout(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
    LRESULT lResult = static_cast<T*>(this)->DefWindowProc(uMsg, wParam, lParam);
    if (lResult && m_nHeight != -1)
    {
        LPHDLAYOUT pLayout = reinterpret_cast<LPHDLAYOUT>(lParam);
        ATLASSERT(pLayout);

        pLayout->prc->top = pLayout->pwpos->cy = m_nHeight;
    }

    return lResult;
}
#endif  // _ATL_SUPPORT_CHD_SETHEIGHT

template <typename T>
inline LRESULT CHeaderCtrlImpl<T>::OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);
    ATLASSERT(::IsWindow(pThis->m_hWnd));

    PAINTSTRUCT paint;
    HDC hDC = ::BeginPaint(pThis->m_hWnd, &paint);
    ATLASSERT(hDC);

    HDDRAWITEM hddi;
    TCHAR szText[MAX_PATH];
    hddi.hDC = hDC, hddi.item.mask = HDI_ALLMASK, hddi.item.pvFilter = NULL;
    hddi.item.pszText = szText, hddi.item.cchTextMax = _countof(szText);

    // Draw background
    ATLVERIFY(::GetClientRect(pThis->m_hWnd, &hddi.rcItem));
    pThis->OnDrawBkgnd(hddi);

    // Draw items
    for (hddi.nItem = Header_GetItemCount(pThis->m_hWnd) - 1; hddi.nItem >= 0; --hddi.nItem)
    {
        ATLVERIFY(Header_GetItem(pThis->m_hWnd, hddi.nItem, &hddi.item));
        ATLVERIFY(Header_GetItemRect(pThis->m_hWnd, hddi.nItem, &hddi.rcItem));

        hddi.nState = (hddi.nItem == m_nItem ? m_nState : 0);
        pThis->OnDrawItem(hddi);
    }

    ATLVERIFY(::EndPaint(pThis->m_hWnd, &paint));
    return 0L;
}

template <typename T>
inline LRESULT CHeaderCtrlImpl<T>::OnMouseMove(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);
    ATLASSERT(::IsWindow(pThis->m_hWnd));

    HDHITTESTINFO hitInfo = { { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) } };
    ::SendMessage(pThis->m_hWnd, HDM_HITTEST, 0, (LPARAM)&hitInfo);
    pThis->OnMouseMove(hitInfo.iItem, wParam, hitInfo.flags);

    return bHandled = FALSE;
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CTrackBarCtrlImpl class
//

template <typename T>
inline CTrackBarCtrlImpl<T>::CTrackBarCtrlImpl()
{
}

template <typename T>
inline void CTrackBarCtrlImpl<T>::OnSetPos(int x, int y)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);
    ATLASSERT(::IsWindow(pThis->m_hWnd));

    CRect rcThumb;
    ::SendMessage(pThis->m_hWnd, TBM_GETTHUMBRECT, 0, (LPARAM)&rcThumb);

    if (!rcThumb.PtInRect(CPoint(x, y)))
    {
        CRect rcChannel;
        ::SendMessage(pThis->m_hWnd, TBM_GETCHANNELRECT, 0, (LPARAM)&rcChannel);

        int nMin = ::SendMessage(pThis->m_hWnd, TBM_GETRANGEMIN, 0, 0L);
        int nMax = ::SendMessage(pThis->m_hWnd, TBM_GETRANGEMAX, 0, 0L);
        BOOL bVertical = (::GetWindowLong(pThis->m_hWnd, GWL_STYLE) & TBS_VERT);

        ::SendMessage(pThis->m_hWnd, TBM_SETPOS, TRUE, stdutil::AdjustRange(::MulDiv((bVertical ? y : x) - rcChannel.left,
                      nMax - nMin, rcChannel.Width() - (bVertical ? rcThumb.Height() : rcThumb.Width()) / 2) + nMin, nMin, nMax));
    }
}

template <typename T>
inline LRESULT CTrackBarCtrlImpl<T>::OnLButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);
    ATLASSERT(::IsWindow(pThis->m_hWnd));

    pThis->OnSetPos(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
    return bHandled = FALSE;
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CStatusBarCtrlImpl class
//

template <typename T>
inline CStatusBarCtrlImpl<T>::CStatusBarCtrlImpl(DWORD dwExStyle/* = 0*/)
    : CStatusBarCtrlBase(dwExStyle)
{
}

template <typename T>
inline BOOL CStatusBarCtrlImpl<T>::SetPanes(const SBPANE* pPanes, int nCount)
{
    return CStatusBarCtrlBase::SetPanes(static_cast<T*>(this)->m_hWnd, pPanes, nCount);
}

template <typename T>
inline void CStatusBarCtrlImpl<T>::SetPaneWidth(int nIndex, int nWidth)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);
    ATLASSERT(m_pPaneWidth);
    ATLASSERT(::IsWindow(pThis->m_hWnd));
    ATLASSERT(nIndex >= 0 && nIndex < m_nPaneCount);

    m_pPaneWidth[nIndex] = nWidth;
    pThis->CalcPaneWidth(pThis->m_hWnd);
}

template <typename T>
inline LRESULT CStatusBarCtrlImpl<T>::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);
    ATLASSERT(::IsWindow(pThis->m_hWnd));

    LRESULT lResult = pThis->DefWindowProc(uMsg, wParam, lParam);
    if (wParam != SIZE_MINIMIZED && m_pPaneWidth != NULL)
        pThis->CalcPaneWidth(pThis->m_hWnd);

    return lResult;
}

template <typename T>
inline LRESULT CStatusBarCtrlImpl<T>::OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
    bHandled = (m_dwExStyle & SBS_EX_CUSTOMDRAW);
    if (bHandled)
    {
        T* pThis = static_cast<T*>(this);
        ATLASSERT(pThis);
        ATLASSERT(::IsWindow(pThis->m_hWnd));

        PAINTSTRUCT paint;
        HDC hDC = ::BeginPaint(pThis->m_hWnd, &paint);
        ATLASSERT(hDC);

        SBDRAWPANE drawPane;
        drawPane.hDC = hDC;
        TCHAR szText[MAX_PATH];

        BOOL bSimple = ::SendMessage(pThis->m_hWnd, SB_ISSIMPLE, 0, 0L);
        for (drawPane.nPane = (bSimple ? 0 : ::SendMessage(pThis->m_hWnd, SB_GETPARTS, 0, 0L) - 1); drawPane.nPane >= 0; --drawPane.nPane)
        {
            pThis->GetPaneRect(pThis->m_hWnd, drawPane.nPane, bSimple, drawPane.rcPane);

            LRESULT lResult = ::SendMessage(pThis->m_hWnd, SB_GETTEXT, drawPane.nPane, (LPARAM)szText);
            drawPane.nType  = HIWORD(::SendMessage(pThis->m_hWnd, SB_GETTEXTLENGTH, drawPane.nPane, 0L));
            drawPane.dwData = (drawPane.nType & SBT_OWNERDRAW) ? lResult : (DWORD)szText;

            pThis->OnDrawPane(drawPane);
        }

        ATLVERIFY(::EndPaint(pThis->m_hWnd, &paint));
    }

    return 0L;
}


#ifdef __IMAGE_H__
///////////////////////////////////////////////////////////////////////////////
// Implementation of the CImageButtonImpl class
//

template <typename T, typename TBase, typename TWinTraits>
inline CImageButtonImpl<T, TBase, TWinTraits>::CImageButtonImpl()
    : m_pImageList(NULL)
{
}

template <typename T, typename TBase, typename TWinTraits>
inline BOOL CImageButtonImpl<T, TBase, TWinTraits>::SubclassWindow(HWND hWnd)
{
    ATLASSERT(::IsWindow(hWnd));

    BOOL bSuccessful = ATL::CWindowImpl<T, TBase, TWinTraits>::SubclassWindow(hWnd);
    if (bSuccessful)
        SetWindowLong(GWL_STYLE, GetWindowLong(GWL_STYLE) | TWinTraits::GetWndStyle(0));

    return bSuccessful;
}

template <typename T, typename TBase, typename TWinTraits>
inline BOOL CImageButtonImpl<T, TBase, TWinTraits>::SizeToContent()
{
    ATLASSERT(IsWindow());
    ATLASSERT(m_pImageList);
    ATLASSERT(!m_pImageList->IsEmpty());

    return SetWindowPos(NULL, 0, 0, m_pImageList->GetImageWidth(), m_pImageList->GetImageHeight(), SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
}

template <typename T, typename TBase, typename TWinTraits>
inline LRESULT CImageButtonImpl<T, TBase, TWinTraits>::OnDrawItem(HWND /*hWnd*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
    LPDRAWITEMSTRUCT pDrawItem = reinterpret_cast<LPDRAWITEMSTRUCT>(lParam);
    ATLASSERT(pDrawItem);
    ATLASSERT(m_pImageList);
    ATLASSERT(pDrawItem->hDC);
    ATLASSERT(!m_pImageList->IsEmpty());
    ATLASSERT(pDrawItem->CtlType == ODT_BUTTON);

    if (m_pImageList->IsAlphaChannel())
    {
        static_cast<T*>(this)->OnDrawBkgnd(pDrawItem);
        ATLVERIFY(m_pImageList->Draw(pDrawItem->hDC, GetImageIndex(pDrawItem->itemState), pDrawItem->rcItem));
    }
    else
    {
        ATLVERIFY(m_pImageList->Draw(pDrawItem->hDC, GetImageIndex(pDrawItem->itemState), pDrawItem->rcItem.left, pDrawItem->rcItem.top));
    }

    return 1L;
}
#endif  // __IMAGE_H__


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CBitmapButtonImpl class
//

template <typename T, typename TBase, typename TWinTraits>
inline CBitmapButtonImpl<T, TBase, TWinTraits>::CBitmapButtonImpl()
    : m_hImageList(NULL)
{
}

template <typename T, typename TBase, typename TWinTraits>
inline BOOL CBitmapButtonImpl<T, TBase, TWinTraits>::SubclassWindow(HWND hWnd)
{
    ATLASSERT(::IsWindow(hWnd));

    BOOL bSuccessful = ATL::CWindowImpl<T, TBase, TWinTraits>::SubclassWindow(hWnd);
    if (bSuccessful)
        SetWindowLong(GWL_STYLE, GetWindowLong(GWL_STYLE) | TWinTraits::GetWndStyle(0));

    return bSuccessful;
}

template <typename T, typename TBase, typename TWinTraits>
inline BOOL CBitmapButtonImpl<T, TBase, TWinTraits>::SizeToContent()
{
    ATLASSERT(IsWindow());
    ATLASSERT(m_hImageList);

    int cx = 0, cy = 0;
    ATLVERIFY(::ImageList_GetIconSize(m_hImageList, &cx, &cy));

    return SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
}

template <typename T, typename TBase, typename TWinTraits>
inline LRESULT CBitmapButtonImpl<T, TBase, TWinTraits>::OnDrawItem(HWND /*hWnd*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
    LPDRAWITEMSTRUCT pDrawItem = reinterpret_cast<LPDRAWITEMSTRUCT>(lParam);
    ATLASSERT(pDrawItem);
    ATLASSERT(m_hImageList);
    ATLASSERT(pDrawItem->hDC);
    ATLASSERT(pDrawItem->CtlType == ODT_BUTTON);

    static_cast<T*>(this)->OnDrawBkgnd(pDrawItem);
    return ::ImageList_Draw(m_hImageList, GetImageIndex(pDrawItem->itemState), pDrawItem->hDC, pDrawItem->rcItem.left, pDrawItem->rcItem.top, ILD_NORMAL);
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CSortListCtrlImpl class
//

template <typename T>
inline CSortListCtrlImpl<T>::CSortListCtrlImpl()
{
}

template <typename T>
inline int CSortListCtrlImpl<T>::GetSortMode(int nCol) const
{
    const T* pThis = static_cast<const T*>(this);
    ATLASSERT(pThis);
    ATLASSERT(::IsWindow(pThis->m_hWnd));
    ATLASSERT(::IsWindow(ListView_GetHeader(pThis->m_hWnd)));

    HDITEM hdItem;
    hdItem.mask = HDI_LPARAM, hdItem.lParam = LVSM_ERROR;
    Header_GetItem(ListView_GetHeader(pThis->m_hWnd), nCol, &hdItem);

    return hdItem.lParam;
}

template <typename T>
inline void CSortListCtrlImpl<T>::SetSortMode(int nCol, int nSortMode)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);
    ATLASSERT(nSortMode != LVSM_ERROR);
    ATLASSERT(::IsWindow(pThis->m_hWnd));
    ATLASSERT(::IsWindow(ListView_GetHeader(pThis->m_hWnd)));

    HDITEM hdItem;
    hdItem.mask = HDI_LPARAM;
    hdItem.lParam = nSortMode;

    ATLVERIFY(Header_SetItem(ListView_GetHeader(pThis->m_hWnd), nCol, &hdItem));
}

template <typename T>
inline BOOL CSortListCtrlImpl<T>::SortItem(int nCol, BOOL bAscending/* = TRUE*/)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);
    ATLASSERT(::IsWindow(pThis->m_hWnd));

    BOOL bSuccessful = FALSE;
    int nSortMode = pThis->GetSortMode(pThis->m_hWnd, nCol);
    if (nSortMode > 0)
    {
        LVSORTSTRUCT lvSort = { pThis->m_hWnd, nCol, nSortMode, bAscending };
        bSuccessful = ListView_SortItemsEx(pThis->m_hWnd, T::CompareItem, &lvSort);
        if (bSuccessful)
            pThis->SetSortCol(pThis->m_hWnd, nCol, bAscending);
    }

    return bSuccessful;
}

template <typename T>
inline int CSortListCtrlImpl<T>::InsertColumnEx(int nCol, const LVCOLUMN* pColumn, int nSortMode/* = LVSM_TEXT*/)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);
    ATLASSERT(::IsWindow(pThis->m_hWnd));

    nCol = ListView_InsertColumn(pThis->m_hWnd, nCol, pColumn);
    if (nCol != -1)
        pThis->SetSortMode(pThis->m_hWnd, nCol, nSortMode);

    return nCol;
}

template <typename T>
inline int CSortListCtrlImpl<T>::InsertColumnEx(int nCol, ATL::_U_STRINGorID columnHeading, int nFormat/* = LVCFMT_LEFT*/, int nWidth/* = -1*/, int nSortMode/* = LVSM_TEXT*/, int nSubItem/* = -1*/, int nImage/* = -1*/, int nOrder/* = -1*/)
{
    CString strColumnHeading(columnHeading.m_lpstr);
    LVCOLUMN column = { LVCF_TEXT | LVCF_FMT, nFormat, nWidth, (LPTSTR)(LPCTSTR)strColumnHeading, 0, nSubItem, nImage, nOrder };
    if (nWidth != -1)
        column.mask |= LVCF_WIDTH;
    if (nImage != -1)
        column.mask |= LVCF_IMAGE;
    if (nOrder != -1)
        column.mask |= LVCF_ORDER;
    if (nSubItem != -1)
        column.mask |= LVCF_SUBITEM;

    return InsertColumnEx(nCol, &column, nSortMode);
}

template <typename T>
inline LRESULT CSortListCtrlImpl<T>::OnColumnClick(int nIDCtrl, LPNMHDR pNMHDR, BOOL& /*bHandled*/)
{
    LPNMHEADER pNMHeader = reinterpret_cast<LPNMHEADER>(pNMHDR);
    ATLASSERT(pNMHeader);
    UNUSED_PARAMETER(nIDCtrl);

    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);
    ATLASSERT(::IsWindow(pThis->m_hWnd));

#ifdef _ATL_SUPPORT_CSLV_SORTCHANGED
    int nSortCol = m_nSortCol;
    if (pThis->SortItem(pNMHeader->iItem, (pNMHeader->iItem == m_nSortCol ? !m_bAscending : TRUE)))
    {
        NMLVSORTCHANGED nmsc = { { pThis->m_hWnd, nIDCtrl, LVN_SORTCHANGED }, m_bAscending, pNMHeader->iItem, nSortCol };
        ::SendMessage(::GetParent(pThis->m_hWnd), WM_NOTIFY, nIDCtrl, (LPARAM)&nmsc);
    }
#else
    pThis->SortItem(pNMHeader->iItem, (pNMHeader->iItem == m_nSortCol ? !m_bAscending : TRUE));
#endif  // _ATL_SUPPORT_CSLV_SORTCHANGED

    return 0L;
}

template <typename T>
inline LRESULT CSortListCtrlImpl<T>::OnDeleteColumn(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
    LRESULT lResult = static_cast<T*>(this)->DefWindowProc(uMsg, wParam, lParam);
    if (lResult)
    {
        if (m_nSortCol == (int)wParam)
            m_nSortCol = -1;
        else if (m_nSortCol > (int)wParam)
            --m_nSortCol;
    }

    return lResult;
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CImageListBoxImpl class
//

template <typename T, typename TBase, typename TWinTraits>
inline CImageListBoxImpl<T, TBase, TWinTraits>::CImageListBoxImpl()
{
}

template <typename T, typename TBase, typename TWinTraits>
inline int CImageListBoxImpl<T, TBase, TWinTraits>::AddItem(LPCTSTR pszText, int nImage/* = -1*/)
{
    ATLASSERT(IsWindow());
    return CImageListBoxBase::AddItem(m_hWnd, pszText, nImage);
}

template <typename T, typename TBase, typename TWinTraits>
inline int CImageListBoxImpl<T, TBase, TWinTraits>::InsertItem(int nItem, LPCTSTR pszText, int nImage/* = -1*/)
{
    ATLASSERT(IsWindow());
    return CImageListBoxBase::InsertItem(m_hWnd, nItem, pszText, nImage);
}

template <typename T, typename TBase, typename TWinTraits>
inline HIMAGELIST CImageListBoxImpl<T, TBase, TWinTraits>::GetImageList() const
{
    return m_hImageList;
}

template <typename T, typename TBase, typename TWinTraits>
inline BOOL CImageListBoxImpl<T, TBase, TWinTraits>::SetImageList(HIMAGELIST hImageList)
{
    ATLASSERT(IsWindow());
    return CImageListBoxBase::SetImageList(m_hWnd, hImageList);
}

template <typename T, typename TBase, typename TWinTraits>
inline LRESULT CImageListBoxImpl<T, TBase, TWinTraits>::OnInitStorage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
    ATLASSERT(IsWindow());

    LRESULT lResult = DefWindowProc(uMsg, wParam, lParam);
    if (lResult != LB_ERRSPACE)
        ATLVERIFY(m_arrImages.SetCount(wParam));

    return lResult;
}

template <typename T, typename TBase, typename TWinTraits>
inline LRESULT CImageListBoxImpl<T, TBase, TWinTraits>::OnDeleteString(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
    ATLASSERT(IsWindow());

    LRESULT lResult = DefWindowProc(uMsg, wParam, lParam);
    if (lResult != LB_ERR)
        m_arrImages.RemoveAt(wParam);

    return lResult;
}

template <typename T, typename TBase, typename TWinTraits>
inline LRESULT CImageListBoxImpl<T, TBase, TWinTraits>::OnResetContent(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
    ATLASSERT(IsWindow());

    m_arrImages.RemoveAll();
    return DefWindowProc(uMsg, wParam, lParam);
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CColorComboBoxImpl class
//

template <typename T, typename TBase, typename TWinTraits>
inline CColorComboBoxImpl<T, TBase, TWinTraits>::CColorComboBoxImpl()
{
}

template <typename T, typename TBase, typename TWinTraits>
inline void CColorComboBoxImpl<T, TBase, TWinTraits>::SetColorMap(const COLORMAPENTRY* pColorMap/* = GetColorMap()*/, int nItemHeight/* = 15*/)
{
    ATLASSERT(IsWindow());
    CColorComboBoxBase::SetColorMap(m_hWnd, pColorMap, nItemHeight);
}

template <typename T, typename TBase, typename TWinTraits>
inline COLORREF CColorComboBoxImpl<T, TBase, TWinTraits>::GetCustomColor() const
{
    ATLASSERT(IsWindow());
    return ::SendMessage(m_hWnd, CB_GETITEMDATA, GetCustomItem(m_hWnd), 0L);
}

template <typename T, typename TBase, typename TWinTraits>
inline void CColorComboBoxImpl<T, TBase, TWinTraits>::SetCustomColor(COLORREF color)
{
    ATLASSERT(IsWindow());
    return ::SendMessage(m_hWnd, CB_SETITEMDATA, GetCustomItem(m_hWnd), color);
}

template <typename T, typename TBase, typename TWinTraits>
inline COLORREF CColorComboBoxImpl<T, TBase, TWinTraits>::GetSelectedColor() const
{
    ATLASSERT(IsWindow());
    ATLASSERT(::SendMessage(m_hWnd, CB_GETCURSEL, 0, 0L) != CB_ERR);

    return ::SendMessage(m_hWnd, CB_GETITEMDATA, ::SendMessage(m_hWnd, CB_GETCURSEL, 0, 0L), 0L);
}

template <typename T, typename TBase, typename TWinTraits>
inline void CColorComboBoxImpl<T, TBase, TWinTraits>::SetSelectedColor(COLORREF color)
{
    ATLASSERT(IsWindow());
    CColorComboBoxBase::SetSelectedColor(m_hWnd, color);
}

template <typename T, typename TBase, typename TWinTraits>
inline LRESULT CColorComboBoxImpl<T, TBase, TWinTraits>::OnSetCurSel(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
    ATLASSERT(IsWindow());

    if (wParam == (WPARAM)GetSeparatorItem(m_hWnd))
        wParam = m_nCurSel;
    else
        m_nCurSel = wParam;

    return DefWindowProc(uMsg, wParam, lParam);
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CProgressBarCtrlImpl class
//

template <typename T, typename TBase, typename TWinTraits>
inline CProgressBarCtrlImpl<T, TBase, TWinTraits>::CProgressBarCtrlImpl()
{
}

template <typename T, typename TBase, typename TWinTraits>
inline void CProgressBarCtrlImpl<T, TBase, TWinTraits>::OnDraw(HDC /*hDC*/, const RECT& /*rect*/, int /*nPos*/)
{
    ATLASSERT(FALSE && _T("MUST BE IMPLEMENTED IN A DERIVED CLASS."));
}

template <typename T, typename TBase, typename TWinTraits>
inline void CProgressBarCtrlImpl<T, TBase, TWinTraits>::OnDrawBkgnd(HDC /*hDC*/, const RECT& /*rect*/)
{
}

template <typename T, typename TBase, typename TWinTraits>
inline LRESULT CProgressBarCtrlImpl<T, TBase, TWinTraits>::OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);
    ATLASSERT(IsWindow());

    PAINTSTRUCT paint;
    HDC hDC = BeginPaint(&paint);
    ATLASSERT(hDC);

    sysutil::CClientRect rcClient(m_hWnd);
    atlutil::CMemoryDC dcMem(hDC, rcClient);

    PBRANGE range = { 0 };
    SendMessage(PBM_GETRANGE, TRUE, (LPARAM)&range);
    ATLASSERT(range.iHigh - range.iLow != 0);

    pThis->OnDrawBkgnd(dcMem, rcClient);
    if (int nPos = SendMessage(PBM_GETPOS, 0, 0L))
        pThis->OnDraw(dcMem, rcClient, ::MulDiv(rcClient.right, nPos - range.iLow, range.iHigh - range.iLow));

    ATLVERIFY(::EndPaint(m_hWnd, &paint));
    return 0L;
}

template <typename T, typename TBase, typename TWinTraits>
inline LRESULT CProgressBarCtrlImpl<T, TBase, TWinTraits>::OnEraseBkgnd(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    return 1L;
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CPaintChildBkgndImpl class
//

template <typename T>
inline CPaintChildBkgndImpl<T>::CPaintChildBkgndImpl()
{
}

template <typename T>
inline void CPaintChildBkgndImpl<T>::PaintBkgnd_Init()
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);
    ATLASSERT(::IsWindow(pThis->m_hWnd));

    sysutil::CClientRect rcClient(pThis->m_hWnd);
    PaintBkgnd_CreateBitmap(pThis->m_hWnd, rcClient.right, rcClient.bottom);
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CShellChangeNotifyImpl class
//

template <typename T>
inline CShellChangeNotifyImpl<T>::CShellChangeNotifyImpl()
    : m_ulID(0)
{
}

template <typename T>
inline void CShellChangeNotifyImpl<T>::SHChangeNotify_OnChange(LONG /*lEvent*/, LPCITEMIDLIST /*pidl1*/, LPCITEMIDLIST /*pidl2*/)
{
}

template <typename T>
inline BOOL CShellChangeNotifyImpl<T>::SHChangeNotify_Register(LPCITEMIDLIST pidl, LONG lEvents/* = SHCNE_SHELLEVENTS*/, int nSources/* = SHCNRF_ShellLevel*/, BOOL bRecursive/* = TRUE*/)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);
    ATLASSERT(m_ulID == 0);
    ATLASSERT(::IsWindow(pThis->m_hWnd));

    SHChangeNotifyEntry shcne = { pidl, bRecursive };
    return (m_ulID = ::SHChangeNotifyRegister(pThis->m_hWnd, nSources | SHCNRF_NewDelivery, lEvents, WM_SHCHANGENOTIFY, 1, &shcne));
}

template <typename T>
inline void CShellChangeNotifyImpl<T>::SHChangeNotify_Unregister()
{
    if (m_ulID != 0 && ::SHChangeNotifyDeregister(m_ulID))
        m_ulID = 0;
}

template <typename T>
inline LRESULT CShellChangeNotifyImpl<T>::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
    SHChangeNotify_Unregister();
    return bHandled = FALSE;
}

template <typename T>
inline LRESULT CShellChangeNotifyImpl<T>::OnSHChangeNotify(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);
    ATLASSERT(::IsWindow(pThis->m_hWnd));

    LONG lEvent = 0;
    LPITEMIDLIST* ppidl = NULL;

    if (HANDLE hLock = ::SHChangeNotification_Lock((HANDLE)wParam, lParam, &ppidl, &lEvent))
    {
        pThis->SHChangeNotify_OnChange(lEvent, ppidl[0], ppidl[1]);
        ATLVERIFY(::SHChangeNotification_Unlock(hLock));
    }

    return 0L;
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CShellTrayNotifyImpl class
//

template <typename T, UINT nID>
inline CShellTrayNotifyImpl<T, nID>::CShellTrayNotifyImpl()
{
}

template <typename T, UINT nID>
inline BOOL CShellTrayNotifyImpl<T, nID>::SHTrayNotify_Create(HICON hIcon, ATL::_U_STRINGorID tip, UINT nCallbackMessage/* = 0*/)
{
    return CShellTrayNotify::Create(static_cast<T*>(this)->m_hWnd, nID, hIcon, CString(tip.m_lpstr), nCallbackMessage);
}

template <typename T, UINT nID>
inline BOOL CShellTrayNotifyImpl<T, nID>::SHTrayNotify_Create(HICON hIcon, ATL::_U_STRINGorID tip, ATL::_U_STRINGorID info, ATL::_U_STRINGorID infoTitle, UINT nCallbackMessage/* = 0*/, DWORD dwInfoFlags/* = NIIF_INFO*/, HICON hBalloonIcon/* = NULL*/, UINT nTimeout/* = 5000*/)
{
    return CShellTrayNotify::Create(static_cast<T*>(this)->m_hWnd, nID, hIcon, CString(tip.m_lpstr), CString(info.m_lpstr), CString(infoTitle.m_lpstr), nCallbackMessage, dwInfoFlags, hBalloonIcon, nTimeout);
}

template <typename T, UINT nID>
inline BOOL CShellTrayNotifyImpl<T, nID>::SHTrayNotify_SetIcon(UINT nIcon)
{
    return CShellTrayNotify::SetIcon(static_cast<T*>(this)->m_hWnd, nID, ::LoadIcon(_AtlBaseModule.GetResourceInstance(), MAKEINTRESOURCE(nIcon)));
}

template <typename T, UINT nID>
inline BOOL CShellTrayNotifyImpl<T, nID>::SHTrayNotify_SetIcon(HICON hIcon)
{
    return CShellTrayNotify::SetIcon(static_cast<T*>(this)->m_hWnd, nID, hIcon);
}

template <typename T, UINT nID>
inline BOOL CShellTrayNotifyImpl<T, nID>::SHTrayNotify_SetTipText(ATL::_U_STRINGorID tip)
{
    return CShellTrayNotify::SetTipText(static_cast<T*>(this)->m_hWnd, nID, CString(tip.m_lpstr));
}

template <typename T, UINT nID>
inline BOOL CShellTrayNotifyImpl<T, nID>::SHTrayNotify_SetCallbackMessage(UINT nCallbackMessage)
{
    return CShellTrayNotify::SetCallbackMessage(static_cast<T*>(this)->m_hWnd, nID, nCallbackMessage);
}

template <typename T, UINT nID>
inline BOOL CShellTrayNotifyImpl<T, nID>::SHTrayNotify_SetState(DWORD dwState, DWORD dwStateMask)
{
    return CShellTrayNotify::SetState(static_cast<T*>(this)->m_hWnd, nID, dwState, dwStateMask);
}

template <typename T, UINT nID>
inline BOOL CShellTrayNotifyImpl<T, nID>::SHTrayNotify_SetVersion(UINT nVersion/* = NOTIFYICON_VERSION*/)
{
    return CShellTrayNotify::SetVersion(static_cast<T*>(this)->m_hWnd, nID, nVersion);
}

template <typename T, UINT nID>
inline BOOL CShellTrayNotifyImpl<T, nID>::SHTrayNotify_ShowBalloonTip(ATL::_U_STRINGorID info, ATL::_U_STRINGorID infoTitle, DWORD dwInfoFlags/* = NIIF_INFO*/, HICON hBalloonIcon/* = NULL*/, UINT nTimeout/* = 5000*/)
{
    return CShellTrayNotify::ShowBalloonTip(static_cast<T*>(this)->m_hWnd, nID, CString(info.m_lpstr), CString(infoTitle.m_lpstr), dwInfoFlags, hBalloonIcon, nTimeout);
}

template <typename T, UINT nID>
inline LRESULT CShellTrayNotifyImpl<T, nID>::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
    ATLVERIFY(CShellTrayNotify::Destroy(static_cast<T*>(this)->m_hWnd, nID));
    return bHandled = FALSE;
}


#ifdef __ATLCTRLS_H__
///////////////////////////////////////////////////////////////////////////////
// Implementation of the CTabSheet class
//

inline CTabSheet::CTabSheet()
{
}

inline BOOL CTabSheet::SubclassWindow(HWND hWnd)
{
    ATLASSERT(::IsWindow(hWnd));

    BOOL bSuccessful = ATL::CWindowImpl<CTabSheet, WTL::CTabCtrl, CTabSheetTraits>::SubclassWindow(hWnd);
    if (bSuccessful)
    {
        ATLVERIFY(SetPageExtra());
        SetWindowLong(GWL_STYLE, GetWindowLong(GWL_STYLE) | CTabSheetTraits::GetWndStyle(0));
        SetWindowLong(GWL_EXSTYLE, GetWindowLong(GWL_EXSTYLE) | CTabSheetTraits::GetWndExStyle(0));
    }

    return bSuccessful;
}
#endif  // __ATLCTRLS_H__

}  // namespace atlctrls

#endif  // __ATLCOMCTRLS_INL__