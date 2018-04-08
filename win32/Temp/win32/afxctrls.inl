///////////////////////////////////////////////////////////////////////////////
// afxctrls.inl
//
// Copyright(c) 2013, Garfield. All rights reserved.
// Author  : Garfield
// Version : 4.5
// Finish Time : 2007/1/21

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __AFXCTRLS_INL__
#define __AFXCTRLS_INL__

namespace afxctrls {

///////////////////////////////////////////////////////////////////////////////
// Implementation of the CCueEdit class
//

inline CCueEdit::CCueEdit()
#ifdef _AFX_SUPPORT_CCE_TEXTCOLOR
    : m_clrText(::GetSysColor(COLOR_WINDOWTEXT))
    , m_clrCueText(::GetSysColor(COLOR_GRAYTEXT))
#endif  // _AFX_SUPPORT_CCE_TEXTCOLOR
{
}

inline CCueEdit::~CCueEdit()
{
}

inline const CString& CCueEdit::GetCueText() const
{
    ASSERT_VALID(this);
    return m_strCueText;
}

inline void CCueEdit::SetCueText(LPCTSTR pszText)
{
    ASSERT_VALID(this);

    m_strCueText = CString(pszText);
    if (!GetModify())
        DefWindowProc(WM_SETTEXT, 0, (LPARAM)(LPCTSTR)m_strCueText);
}

inline void CCueEdit::OnSetFocus(CWnd* pOldWnd)
{
    ASSERT_VALID(this);

    if (!GetModify())
        DefWindowProc(WM_SETTEXT, 0, 0L);

    CEdit::OnSetFocus(pOldWnd);
}

inline void CCueEdit::OnKillFocus(CWnd* pNewWnd)
{
    ASSERT_VALID(this);

    if (DefWindowProc(WM_GETTEXTLENGTH, 0, 0L) == 0)
        DefWindowProc(WM_SETTEXT, 0, (LPARAM)(LPCTSTR)m_strCueText);

    CEdit::OnKillFocus(pNewWnd);
}

inline HBRUSH CCueEdit::CtlColor(CDC* pDC, UINT /*nCtlColor*/)
{
    ASSERT_VALID(this);

#ifdef _AFX_SUPPORT_CCE_TEXTCOLOR
    pDC->SetTextColor(GetModify() ? m_clrText : m_clrCueText);
#else
    pDC->SetTextColor(::GetSysColor(GetModify() ? COLOR_WINDOWTEXT : COLOR_GRAYTEXT));
#endif  // _AFX_SUPPORT_CCE_TEXTCOLOR

    return (HBRUSH)(COLOR_WINDOW + 1);
}

inline LRESULT CCueEdit::OnSetText(WPARAM wParam, LPARAM lParam)
{
    ASSERT_VALID(this);

    BOOL bEmpty = stdutil::StringIsEmpty((LPCTSTR)lParam);
    LRESULT lResult = DefWindowProc(WM_SETTEXT, wParam, bEmpty ? (LPARAM)(LPCTSTR)m_strCueText : lParam);
    SetModify(!bEmpty);

    return lResult;
}

inline LRESULT CCueEdit::OnGetText(WPARAM wParam, LPARAM lParam)
{
    ASSERT_VALID(this);
    ASSERT(wParam != 0);

    return (GetModify() ? DefWindowProc(WM_GETTEXT, wParam, lParam) : (*((LPTSTR)lParam) = _T('\0'), 0L));
}

inline LRESULT CCueEdit::OnGetTextLength(WPARAM wParam, LPARAM lParam)
{
    ASSERT_VALID(this);
    return (GetModify() ? DefWindowProc(WM_GETTEXTLENGTH, wParam, lParam) : 0L);
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CImageLinkCtrl class
//

inline CImageLinkCtrl::CImageLinkCtrl()
{
}

inline CImageLinkCtrl::~CImageLinkCtrl()
{
}

inline void CImageLinkCtrl::PreSubclassWindow()
{
    ASSERT_VALID(this);

    ::SetWindowLong(m_hWnd, GWL_STYLE, ::GetWindowLong(m_hWnd, GWL_STYLE) | SS_NOTIFY);
    CStatic::PreSubclassWindow();
}

inline BOOL CImageLinkCtrl::IsValidContent() const
{
    DWORD dwStyle = GetStyle();
    return ((dwStyle & SS_BITMAP) == SS_BITMAP || (dwStyle & SS_ICON) == SS_ICON || GetWindowTextLength());
}

inline void CImageLinkCtrl::OnLButtonUp(UINT /*nFlags*/, CPoint point)
{
    ASSERT_VALID(this);

    if (::GetCapture() == m_hWnd)
    {
        VERIFY(::ReleaseCapture());
        sysutil::CClientRect rcClient(m_hWnd);

        if (rcClient.PtInRect(point))
            ::SendMessage(::GetParent(m_hWnd), WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(), STN_CLICKED), (LPARAM)m_hWnd);
    }
}

inline void CImageLinkCtrl::OnLButtonDown(UINT /*nFlags*/, CPoint /*point*/)
{
    ASSERT_VALID(this);

    if (IsValidContent())
        SetCapture();
}

inline BOOL CImageLinkCtrl::OnSetCursor(CWnd* /*pWnd*/, UINT /*nHitTest*/, UINT /*message*/)
{
    ::SetCursor(::LoadCursor(NULL, IsValidContent() ? IDC_HAND : IDC_ARROW));
    return TRUE;
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CLinkCtrl class
//

inline CLinkCtrl::CLinkCtrl(DWORD dwExStyle/* = 0*/)
    : CLinkCtrlBase(dwExStyle)
{
}

inline CLinkCtrl::~CLinkCtrl()
{
}

inline void CLinkCtrl::OnPaint()
{
    ASSERT_VALID(this);
    ASSERT(::IsWindow(m_hWnd));

    CPaintDC dc(this);
    DoPaint(m_hWnd, dc.m_hDC);
}

inline void CLinkCtrl::OnEnable(BOOL /*bEnable*/)
{
    Invalidate(TRUE);
    UpdateWindow();
}

inline BOOL CLinkCtrl::OnEraseBkgnd(CDC* pDC)
{
    ASSERT_VALID(this);
    ASSERT_VALID(pDC);
    ASSERT(pDC->m_hDC);

    if (HBRUSH hBrush = (HBRUSH)::SendMessage(::GetParent(m_hWnd), WM_CTLCOLORSTATIC, (WPARAM)pDC->m_hDC, (LPARAM)m_hWnd))
        VERIFY(::FillRect(pDC->m_hDC, sysutil::CClientRect(m_hWnd), hBrush));

    return TRUE;
}

inline void CLinkCtrl::OnMouseMove(UINT /*nFlags*/, CPoint /*point*/)
{
    ASSERT_VALID(this);

    if (!m_bMouseHover && (m_dwExStyle & (LCS_EX_HOVER | LCS_EX_UNDERLINEHOVER)) && GetWindowTextLength())
    {
        TRACKMOUSEEVENT eventTrack = { sizeof(TRACKMOUSEEVENT), TME_LEAVE, m_hWnd, 0 };
        m_bMouseHover = ::TrackMouseEvent(&eventTrack);

        Invalidate(TRUE);
        UpdateWindow();
    }
}

inline LRESULT CLinkCtrl::OnMouseLeave(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
    ASSERT_VALID(this);

    m_bMouseHover = FALSE;
    Invalidate(TRUE);
    UpdateWindow();

    return 0L;
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CTabSheet class
//

inline CTabSheet::CTabSheet()
{
}

inline CTabSheet::~CTabSheet()
{
}

inline void CTabSheet::PreSubclassWindow()
{
    ASSERT_VALID(this);

    VERIFY(SetItemExtra(TCITEM_EXTRA_SIZE));
    ::SetWindowLong(m_hWnd, GWL_STYLE, ::GetWindowLong(m_hWnd, GWL_STYLE) | WS_CLIPCHILDREN);
    ::SetWindowLong(m_hWnd, GWL_EXSTYLE, ::GetWindowLong(m_hWnd, GWL_EXSTYLE) | WS_EX_CONTROLPARENT);

    CTabCtrl::PreSubclassWindow();
}

inline BOOL CTabSheet::PreTranslateMessage(LPMSG pMsg)
{
    ASSERT_VALID(this);
    return (::IsDialogMessage(GetActivePage(), pMsg) ? TRUE : CTabCtrl::PreTranslateMessage(pMsg));
}

inline int CTabSheet::AddPage(HWND hWndPage, LPCTSTR pszTitle, int nImage/* = -1*/, LPARAM lParam/* = 0L*/)
{
    return InsertPage(GetItemCount(), hWndPage, pszTitle, nImage, lParam);
}

inline int CTabSheet::InsertPage(int nPage, HWND hWndPage, LPCTSTR pszTitle, int nImage/* = -1*/, LPARAM lParam/* = 0L*/)
{
    ASSERT_VALID(this);
    ASSERT(::IsWindow(m_hWnd));
    ASSERT(::IsWindow(hWndPage));

    CString strTitle(pszTitle);
    TSITEM item = { { TCIF_TEXT | TCIF_PARAM, 0, 0, (LPTSTR)(LPCTSTR)strTitle, 0, nImage }, hWndPage, lParam };
    if (nImage != -1)
        item.tciHeader.mask |= TCIF_IMAGE;

    if ((nPage = TabCtrl_InsertItem(m_hWnd, nPage, &item)) != -1)
        ::SendMessage(m_hWnd, TSM_SETPAGERECT, nPage, (LPARAM)hWndPage);

    return nPage;
}

inline HWND CTabSheet::GetActivePage() const
{
    ASSERT_VALID(this);
    ASSERT(::IsWindow(m_hWnd));

    return GetPage(GetCurSel());
}

inline HWND CTabSheet::GetPage(int nPage) const
{
    ASSERT_VALID(this);
    ASSERT(::IsWindow(m_hWnd));

    TSITEM item;
    item.tciHeader.mask = TCIF_PARAM, item.hWnd = NULL;
    TabCtrl_GetItem(m_hWnd, nPage, &item);

    return item.hWnd;
}

inline BOOL CTabSheet::SetPage(int nPage, HWND hWnd)
{
    ASSERT_VALID(this);
    ASSERT(::IsWindow(m_hWnd));
    ASSERT(nPage >= 0 && nPage < GetItemCount());

    TSITEM item;
    item.tciHeader.mask = TCIF_PARAM;
    VERIFY(TabCtrl_GetItem(m_hWnd, nPage, &item));
    item.hWnd = hWnd;

    return TabCtrl_SetItem(m_hWnd, nPage, &item);
}

inline int CTabSheet::GetPageImage(int nPage) const
{
    ASSERT_VALID(this);
    ASSERT(::IsWindow(m_hWnd));
    ASSERT(nPage >= 0 && nPage < GetItemCount());

    TSITEM item;
    item.tciHeader.iImage = -1;
    item.tciHeader.mask = TCIF_IMAGE;
    VERIFY(TabCtrl_GetItem(m_hWnd, nPage, &item));

    return item.tciHeader.iImage;
}

inline BOOL CTabSheet::SetPageImage(int nPage, int nImage)
{
    ASSERT_VALID(this);
    ASSERT(::IsWindow(m_hWnd));
    ASSERT(nPage >= 0 && nPage < GetItemCount());

    TSITEM item;
    item.tciHeader.mask = TCIF_IMAGE;
    item.tciHeader.iImage = nImage;

    return TabCtrl_SetItem(m_hWnd, nPage, &item);
}

inline DWORD_PTR CTabSheet::GetPageData(int nPage) const
{
    ASSERT_VALID(this);
    ASSERT(::IsWindow(m_hWnd));
    ASSERT(nPage >= 0 && nPage < GetItemCount());

    TSITEM item;
    item.tciHeader.mask = TCIF_PARAM, item.lParam = 0L;
    VERIFY(TabCtrl_GetItem(m_hWnd, nPage, &item));

    return item.lParam;
}

inline BOOL CTabSheet::SetPageData(int nPage, DWORD_PTR dwData)
{
    ASSERT_VALID(this);
    ASSERT(::IsWindow(m_hWnd));
    ASSERT(nPage >= 0 && nPage < GetItemCount());

    TSITEM item;
    item.tciHeader.mask = TCIF_PARAM;
    VERIFY(TabCtrl_GetItem(m_hWnd, nPage, &item));
    item.lParam = (LPARAM)dwData;

    return TabCtrl_SetItem(m_hWnd, nPage, &item);
}

inline CString CTabSheet::GetPageTitle(int nPage) const
{
    ASSERT_VALID(this);
    ASSERT(::IsWindow(m_hWnd));
    ASSERT(nPage >= 0 && nPage < GetItemCount());

    TSITEM item;
    CString strTitle;
    item.tciHeader.pszText = strTitle.GetBuffer(MAX_PATH);
    item.tciHeader.mask = TCIF_TEXT, item.tciHeader.cchTextMax = MAX_PATH;

    VERIFY(TabCtrl_GetItem(m_hWnd, nPage, &item));
    strTitle.ReleaseBuffer();

    return strTitle;
}

inline BOOL CTabSheet::SetPageTitle(int nPage, LPCTSTR pszTitle)
{
    ASSERT_VALID(this);
    ASSERT(::IsWindow(m_hWnd));
    ASSERT(nPage >= 0 && nPage < GetItemCount());

    TSITEM item;
    CString strTitle(pszTitle);
    item.tciHeader.mask    = TCIF_TEXT;
    item.tciHeader.pszText = (LPTSTR)(LPCTSTR)strTitle;

    return TabCtrl_SetItem(m_hWnd, nPage, &item);
}

inline void CTabSheet::OnSize(UINT nType, int /*cx*/, int /*cy*/)
{
    ASSERT_VALID(this);
    if (nType != SIZE_MINIMIZED)
    {
        for (int i = GetItemCount() - 1; i >= 0; --i)
            ::SendMessage(m_hWnd, TSM_SETPAGERECT, i, (LPARAM)GetPage(i));
    }
}

inline int CTabSheet::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    ASSERT_VALID(this);

    int nRet = CTabCtrl::OnCreate(lpCreateStruct);
    if (nRet != -1)
        VERIFY(SetItemExtra(TCITEM_EXTRA_SIZE));

    return nRet;
}

inline void CTabSheet::OnSelChange(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
    ASSERT_VALID(this);

    *pResult = 0L;
    SetCurSel(GetCurSel());
}

inline LRESULT CTabSheet::OnSetCurSel(WPARAM wParam, LPARAM lParam)
{
    ASSERT_VALID(this);

    ::ShowWindow(::GetTopWindow(m_hWnd), SW_HIDE);
    if (HWND hWnd = GetPage((int)wParam))
    {
        VERIFY(::SetWindowPos(hWnd, HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_SHOWWINDOW));
        VERIFY(::UpdateWindow(hWnd));
        ::SetFocus(hWnd);
    }

    return DefWindowProc(TCM_SETCURSEL, wParam, lParam);
}

inline LRESULT CTabSheet::OnSetPageRect(WPARAM /*wParam*/, LPARAM lParam)
{
    ASSERT_VALID(this);
    ASSERT(::IsWindow(m_hWnd));
    ASSERT(::IsWindow((HWND)lParam));

    sysutil::CClientRect rcPage(m_hWnd);
    AdjustRect(FALSE, &rcPage);

    return ::SetWindowPos((HWND)lParam, NULL, rcPage.left - 1, rcPage.top, rcPage.right - rcPage.left + 1, rcPage.bottom - rcPage.top, SWP_NOZORDER | SWP_NOREDRAW | SWP_NOACTIVATE);
}

inline LRESULT CTabSheet::OnDeleteItem(WPARAM wParam, LPARAM lParam)
{
    ASSERT_VALID(this);

    int nCurSel = GetCurSel();
    LRESULT lResult = DefWindowProc(TCM_DELETEITEM, wParam, lParam);
    if (lResult && nCurSel == (int)wParam)
        SetCurSel(nCurSel == 0 ? 0 : nCurSel - 1);

    return lResult;
}

inline LRESULT CTabSheet::OnDeleteAllItems(WPARAM wParam, LPARAM lParam)
{
    ASSERT_VALID(this);

    LRESULT lResult = DefWindowProc(TCM_DELETEALLITEMS, wParam, lParam);
    if (lResult)
        ::ShowWindow(::GetTopWindow(m_hWnd), SW_HIDE);

    return lResult;
}


#ifdef __IMAGE_H__
///////////////////////////////////////////////////////////////////////////////
// Implementation of the CImageCtrl class
//

inline CImageCtrl::CImageCtrl()
{
}

inline CImageCtrl::~CImageCtrl()
{
}

inline void CImageCtrl::OnPaint()
{
    ASSERT_VALID(this);

    CPaintDC dc(this);
    sysutil::CClientRect rcClient(m_hWnd);

    DoDrawBkgnd(dc.m_hDC, rcClient);
    rcClient.DeflateRect(1, 1, 1, 1);
    DoPaint(dc.m_hDC, rcClient);
}

inline void CImageCtrl::OnDestroy()
{
    m_image.Destroy();
    CWnd::OnDestroy();
}

inline BOOL CImageCtrl::OnEraseBkgnd(CDC* /*pDC*/)
{
    return TRUE;
}
#endif  // __IMAGE_H__


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CSplashWnd class
//

inline CSplashWnd::CSplashWnd()
{
}

inline CSplashWnd::~CSplashWnd()
{
    DestroyWindow();
}

inline BOOL CSplashWnd::Create(HBITMAP hBitmap, COLORREF clrMask/* = CLR_INVALID*/, CWnd* pParentWnd/* = NULL*/)
{
    ASSERT_VALID(this);
    ASSERT(hBitmap);
    ASSERT(m_hWnd == NULL);
    ASSERT(m_hBitmap == NULL);

    BITMAP bmp;
    m_hBitmap = hBitmap;
    VERIFY(::GetObject(hBitmap, sizeof(BITMAP), &bmp));

#if (_WIN32_WINNT >= 0x0501)
    BOOL bSuccessful = CreateEx(WS_EX_TOOLWINDOW, AfxRegisterWndClass((sysutil::IsWindowsXP() ? CS_DROPSHADOW : 0), ::LoadCursor(NULL, IDC_ARROW)), NULL, WS_POPUP, 0, 0, bmp.bmWidth, bmp.bmHeight, pParentWnd->GetSafeHwnd(), NULL);
#else
    BOOL bSuccessful = CreateEx(WS_EX_TOOLWINDOW, AfxRegisterWndClass(0, ::LoadCursor(NULL, IDC_ARROW)), NULL, WS_POPUP, 0, 0, bmp.bmWidth, bmp.bmHeight, pParentWnd->GetSafeHwnd(), NULL);
#endif  // _WIN32_WINNT >= 0x0501

    if (bSuccessful)
    {
        CenterWindow();
        if (clrMask != CLR_INVALID)
            VERIFY(SetWindowRgn(stdutil::CreateRegion(hBitmap, clrMask), FALSE));
    }

    return bSuccessful;
}

inline void CSplashWnd::SetDrawText(POSITION pos, UINT nIDText/* = 0*/, COLORREF clrText/* = CLR_INVALID*/, LPLOGFONT pLogFont/* = NULL*/, UINT nFormat/* = (UINT)-1*/, LPCRECT pRect/* = NULL*/)
{
    ASSERT_VALID(this);
    ASSERT(pos != NULL);
    ASSERT(::IsWindow(m_hWnd));

    CString strText;
    if (nIDText != 0)
        VERIFY(strText.LoadString(nIDText));

    CSplashWindowBase::SetDrawText(m_hWnd, pos, strText, clrText, pLogFont, nFormat, pRect);
}

inline void CSplashWnd::SetDrawText(POSITION pos, LPCTSTR pszText/* = NULL*/, COLORREF clrText/* = CLR_INVALID*/, LPLOGFONT pLogFont/* = NULL*/, UINT nFormat/* = (UINT)-1*/, LPCRECT pRect/* = NULL*/)
{
    ASSERT_VALID(this);
    ASSERT(pos != NULL);
    ASSERT(::IsWindow(m_hWnd));

    CSplashWindowBase::SetDrawText(m_hWnd, pos, pszText, clrText, pLogFont, nFormat, pRect);
}

inline void CSplashWnd::OnPaint()
{
    ASSERT_VALID(this);
    ASSERT(::IsWindow(m_hWnd));

    CPaintDC dc(this);
    DoPaint(m_hWnd, dc.m_hDC);
}

inline BOOL CSplashWnd::OnEraseBkgnd(CDC* /*pDC*/)
{
    return TRUE;
}


#ifdef __PICTURE_H__
///////////////////////////////////////////////////////////////////////////////
// Implementation of the CPictureCtrl class
//

inline CPictureCtrl::CPictureCtrl()
{
}

inline CPictureCtrl::~CPictureCtrl()
{
}

inline void CPictureCtrl::OnPaint()
{
    ASSERT_VALID(this);

    CPaintDC dc(this);
    sysutil::CClientRect rcClient(m_hWnd);

    DoDrawBkgnd(dc.m_hDC, rcClient);
    rcClient.DeflateRect(1, 1, 1, 1);
    DoPaint(dc.m_hDC, rcClient);
}

inline BOOL CPictureCtrl::OnEraseBkgnd(CDC* /*pDC*/)
{
    return TRUE;
}
#endif  // __PICTURE_H__


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CSortListCtrl class
//

inline CSortListCtrl::CSortListCtrl()
    : m_pfnCompare(CompareItem)
{
}

inline CSortListCtrl::~CSortListCtrl()
{
}

inline BOOL CSortListCtrl::SortItem(int nCol, BOOL bAscending/* = TRUE*/)
{
    ASSERT_VALID(this);

    BOOL bSuccessful = FALSE;
    int nSortMode = GetSortMode(nCol);
    if (nSortMode > 0)
    {
        LVSORTSTRUCT lvSort = { m_hWnd, nCol, nSortMode, bAscending };
        bSuccessful = ListView_SortItemsEx(m_hWnd, m_pfnCompare, &lvSort);
        if (bSuccessful)
            SetSortCol(m_hWnd, nCol, bAscending);
    }

    return bSuccessful;
}

inline int CSortListCtrl::InsertColumnEx(int nCol, const LVCOLUMN* pColumn, int nSortMode/* = LVSM_TEXT*/)
{
    ASSERT_VALID(this);
    ASSERT(::IsWindow(m_hWnd));

    nCol = InsertColumn(nCol, pColumn);
    if (nCol != -1)
        SetSortMode(nCol, nSortMode);

    return nCol;
}

inline int CSortListCtrl::InsertColumnEx(int nCol, LPCTSTR pszColumnHeading, int nFormat/* = LVCFMT_LEFT*/, int nWidth/* = -1*/, int nSortMode/* = LVSM_TEXT*/, int nSubItem/* = -1*/, int nImage/* = -1*/, int nOrder/* = -1*/)
{
    ASSERT_VALID(this);
    ASSERT(::IsWindow(m_hWnd));

    CString strColumnHeading(pszColumnHeading);
    LVCOLUMN column = { LVCF_TEXT | LVCF_FMT, nFormat, nWidth, (LPTSTR)(LPCTSTR)strColumnHeading, 0, nSubItem, nImage, nOrder };
    if (nWidth != -1)
        column.mask |= LVCF_WIDTH;
    if (nSubItem != -1)
        column.mask |= LVCF_SUBITEM;
    if (nImage != -1)
        column.mask |= LVCF_IMAGE;
    if (nOrder != -1)
        column.mask |= LVCF_ORDER;

    return InsertColumnEx(nCol, &column, nSortMode);
}

inline int CSortListCtrl::GetSortMode(int nCol) const
{
    ASSERT_VALID(this);
    ASSERT(::IsWindow(m_hWnd));

    return (int)::SendMessage(m_hWnd, LVM_GETSORTMODE, nCol, 0L);
}

inline BOOL CSortListCtrl::SetSortMode(int nCol, int nSortMode)
{
    ASSERT_VALID(this);
    ASSERT(::IsWindow(m_hWnd));

    return (BOOL)::SendMessage(m_hWnd, LVM_SETSORTMODE, nCol, nSortMode);
}

inline void CSortListCtrl::OnColumnClick(NMHDR* pNMHDR, LRESULT* pResult)
{
    LPNMLISTVIEW pNMListView = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
    ASSERT_VALID(this);
    ASSERT(pNMListView);

#ifdef _AFX_SUPPORT_CSLV_SORTCHANGED
    int nSortCol = m_nSortCol;
    if (SortItem(pNMListView->iSubItem, (pNMListView->iSubItem == m_nSortCol ? !m_bAscending : TRUE)))
    {
        NMLVSORTCHANGED nmsc = { { m_hWnd, (int)pNMHDR->idFrom, LVN_SORTCHANGED }, m_bAscending, pNMListView->iSubItem, nSortCol };
        ::SendMessage(::GetParent(m_hWnd), WM_NOTIFY, pNMHDR->idFrom, (LPARAM)&nmsc);
    }
#else
    SortItem(pNMListView->iSubItem, (pNMListView->iSubItem == m_nSortCol ? !m_bAscending : TRUE));
#endif  // _AFX_SUPPORT_CSLV_SORTCHANGED

    *pResult = 0L;
}

inline LRESULT CSortListCtrl::OnGetSortMode(WPARAM wParam, LPARAM /*lParam*/)
{
    ASSERT_VALID(this);
    ASSERT(::IsWindow(ListView_GetHeader(m_hWnd)));

    HDITEM hdItem;
    hdItem.mask = HDI_LPARAM, hdItem.lParam = LVSM_ERROR;
    Header_GetItem(ListView_GetHeader(m_hWnd), wParam, &hdItem);

    return hdItem.lParam;
}

inline LRESULT CSortListCtrl::OnSetSortMode(WPARAM wParam, LPARAM lParam)
{
    ASSERT_VALID(this);
    ASSERT(lParam != LVSM_ERROR);
    ASSERT(::IsWindow(ListView_GetHeader(m_hWnd)));

    HDITEM hdItem;
    hdItem.lParam = lParam;
    hdItem.mask = HDI_LPARAM;

    return Header_SetItem(ListView_GetHeader(m_hWnd), wParam, &hdItem);
}

inline LRESULT CSortListCtrl::OnDeleteColumn(WPARAM wParam, LPARAM lParam)
{
    ASSERT_VALID(this);

    LRESULT lResult = DefWindowProc(LVM_DELETECOLUMN, wParam, lParam);
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
// Implementation of the CBitmapButton class
//

inline CBitmapButton::CBitmapButton()
{
}

inline CBitmapButton::~CBitmapButton()
{
}

inline void CBitmapButton::DrawItem(LPDRAWITEMSTRUCT pDrawItem)
{
    ASSERT_VALID(this);
    DoDrawItem(pDrawItem);
}

inline void CBitmapButton::PreSubclassWindow()
{
    ASSERT_VALID(this);

    ::SetWindowLong(m_hWnd, GWL_EXSTYLE, ::GetWindowLong(m_hWnd, GWL_EXSTYLE) | WS_EX_TRANSPARENT);
    ::SetWindowLong(m_hWnd, GWL_STYLE, ::GetWindowLong(m_hWnd, GWL_STYLE) | BS_OWNERDRAW | WS_CLIPCHILDREN);

    CButton::PreSubclassWindow();
}

inline BOOL CBitmapButton::SizeToContent()
{
    ASSERT_VALID(this);
    ASSERT(m_hbmNormal);
    ASSERT(::IsWindow(m_hWnd));

    BITMAP bmp;
    VERIFY(::GetObject(m_hbmNormal, sizeof(BITMAP), &bmp));

    return SetWindowPos(NULL, 0, 0, bmp.bmWidth, bmp.bmHeight, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
}

inline BOOL CBitmapButton::SetMaskColor(COLORREF color)
{
    ASSERT_VALID(this);
    ASSERT(m_hbmNormal);
    ASSERT(::IsWindow(m_hWnd));

    return SetWindowRgn(stdutil::CreateRegion(m_hbmNormal, color), TRUE);
}

inline BOOL CBitmapButton::PtInWindow(POINT point) const
{
    return CBitmapButtonBase::PtInWindow(m_hWnd, point);
}

inline BOOL CBitmapButton::OnEraseBkgnd(CDC* /*pDC*/)
{
    return TRUE;
}

inline void CBitmapButton::OnMouseMove(UINT nFlags, CPoint point)
{
    ASSERT_VALID(this);

    CBitmapButtonBase::OnMouseMove(m_hWnd);
    CButton::OnMouseMove(nFlags, point);
}

inline LRESULT CBitmapButton::OnMouseLeave(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
    ASSERT_VALID(this);

    CBitmapButtonBase::OnMouseLeave(m_hWnd);
    return 0L;
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CImageListBox class
//

inline CImageListBox::CImageListBox()
{
}

inline CImageListBox::~CImageListBox()
{
}

inline void CImageListBox::DrawItem(LPDRAWITEMSTRUCT pDrawItem)
{
    ASSERT_VALID(this);
    DoDrawItem(pDrawItem);
}

inline int CImageListBox::AddItem(LPCTSTR pszItem, int nImage/* = -1*/)
{
    ASSERT_VALID(this);
    ASSERT(::IsWindow(m_hWnd));

    int nItem = AddString(pszItem);
    if (nItem != LB_ERR)
        m_arrImages.InsertAt(nItem, nImage);

    return nItem;
}

inline int CImageListBox::InsertItem(int nItem, LPCTSTR pszItem, int nImage/* = -1*/)
{
    ASSERT_VALID(this);
    ASSERT(::IsWindow(m_hWnd));

    nItem = InsertString(nItem, pszItem);
    if (nItem != LB_ERR)
        m_arrImages.InsertAt(nItem, nImage);

    return nItem;
}

inline HIMAGELIST CImageListBox::GetImageList() const
{
    ASSERT_VALID(this);
    return m_hImageList;
}

inline BOOL CImageListBox::SetImageList(HIMAGELIST hImageList)
{
    ASSERT_VALID(this);
    return CImageListBoxBase::SetImageList(m_hWnd, hImageList);
}

#ifdef _AFX_SUPPORT_CILB_TEXTCOLOR
inline void CImageListBox::OnEnable(BOOL /*bEnable*/)
{
    ASSERT_VALID(this);

    Invalidate(TRUE);
    UpdateWindow();
}

inline BOOL CImageListBox::OnEraseBkgnd(CDC* pDC)
{
    ASSERT_VALID(this);

    CBrush brush;
    VERIFY(brush.CreateSolidBrush(IsWindowEnabled() ? m_clrBack : ::GetSysColor(COLOR_WINDOW)));
    pDC->FillRect(sysutil::CClientRect(m_hWnd), &brush);

    return TRUE;
}
#endif  // _AFX_SUPPORT_CILB_TEXTCOLOR

inline LRESULT CImageListBox::OnInitStorage(WPARAM wParam, LPARAM lParam)
{
    ASSERT_VALID(this);

    LRESULT lResult = DefWindowProc(LB_INITSTORAGE, wParam, lParam);
    if (lResult != LB_ERRSPACE)
        m_arrImages.SetCount(wParam);

    return lResult;
}

inline LRESULT CImageListBox::OnDeleteString(WPARAM wParam, LPARAM lParam)
{
    ASSERT_VALID(this);

    LRESULT lResult = DefWindowProc(LB_DELETESTRING, wParam, lParam);
    if (lResult != LB_ERR)
        m_arrImages.RemoveAt(wParam);

    return lResult;
}

inline LRESULT CImageListBox::OnResetContent(WPARAM wParam, LPARAM lParam)
{
    ASSERT_VALID(this);

    m_arrImages.RemoveAll();
    return DefWindowProc(LB_RESETCONTENT, wParam, lParam);
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CColorComboBox class
//

inline CColorComboBox::CColorComboBox()
{
}

inline CColorComboBox::~CColorComboBox()
{
}

inline void CColorComboBox::DrawItem(LPDRAWITEMSTRUCT pDrawItem)
{
    ASSERT_VALID(this);
    DoDrawItem(pDrawItem);
}

inline void CColorComboBox::SetColorMap(const COLORMAPENTRY* pColorMap/* = GetColorMap()*/, int nItemHeight/* = 14*/)
{
    ASSERT_VALID(this);
    CColorComboBoxBase::SetColorMap(m_hWnd, pColorMap, nItemHeight);
}

inline COLORREF CColorComboBox::GetSelectedColor() const
{
    ASSERT_VALID(this);
    ASSERT(GetCurSel() != CB_ERR);

    return GetItemData(GetCurSel());
}

inline void CColorComboBox::SetSelectedColor(COLORREF color)
{
    ASSERT_VALID(this);
    CColorComboBoxBase::SetSelectedColor(m_hWnd, color);
}

inline void CColorComboBox::OnCloseUp()
{
    ASSERT_VALID(this);
    SetCurSel(GetCurSel());
}

inline void CColorComboBox::OnSelEndOK()
{
    ASSERT_VALID(this);
    DoSelEndOK(GetDlgCtrlID(), m_hWnd);
}

inline void CColorComboBox::OnSelChange()
{
    ASSERT_VALID(this);
    DoSelChange(GetDlgCtrlID(), m_hWnd);
}

inline void CColorComboBox::OnSelCustom(NMHDR* pNMHDR, LRESULT* pResult)
{
    ASSERT_VALID(this);
    *pResult = DoSelCustom(pNMHDR);
}

inline LRESULT CColorComboBox::OnSetCurSel(WPARAM wParam, LPARAM lParam)
{
    ASSERT_VALID(this);

    if (wParam == (WPARAM)GetSeparatorItem(m_hWnd))
        wParam = m_nCurSel;
    else
        m_nCurSel = wParam;

    return DefWindowProc(CB_SETCURSEL, wParam, lParam);
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CBalloonTipCtrl class
//

inline CBalloonTipCtrl::CBalloonTipCtrl()
{
}

inline CBalloonTipCtrl::~CBalloonTipCtrl()
{
}

inline BOOL CBalloonTipCtrl::Create(CWnd* pParentWnd, DWORD dwStyle/* = TTS_BALLOON*/, int nMaxWidth/* = 300*/, LPVOID pvParam/* = NULL*/)
{
    ASSERT_VALID(this);
    ASSERT_VALID(pParentWnd);
    ASSERT(::IsWindow(pParentWnd->m_hWnd));

    BOOL bSuccessful = CWnd::CreateEx(WS_EX_TOOLWINDOW, TOOLTIPS_CLASS, NULL, dwStyle | WS_POPUP, 0, 0, CW_USEDEFAULT, CW_USEDEFAULT, pParentWnd->m_hWnd, NULL, pvParam);
    if (bSuccessful)
        ::SendMessage(m_hWnd, TTM_SETMAXTIPWIDTH, 0, nMaxWidth);

    return bSuccessful;
}

inline void CBalloonTipCtrl::RelayEventEx(LPMSG pMsg)
{
    ASSERT_VALID(this);
    ASSERT(pMsg);
    ASSERT(::IsWindow(m_hWnd));

    if (IsInputMessage(pMsg))
        HideBalloonTip();
}

inline void CBalloonTipCtrl::HideBalloonTip()
{
    ASSERT_VALID(this);

    if (::IsWindowVisible(m_hWnd))
    {
        ::KillTimer(m_hWnd, IDT_TIMER);
        ::SendMessage(m_hWnd, TTM_ACTIVATE, FALSE, 0L);
    }
}

inline BOOL CBalloonTipCtrl::IsInputMessage(LPMSG pMsg) const
{
    return (pMsg->message == WM_KEYDOWN || pMsg->message == WM_SYSKEYDOWN || (pMsg->message > WM_MOUSEFIRST && pMsg->message <= WM_MOUSELAST) || (pMsg->message > WM_NCMOUSEMOVE && pMsg->message <= WM_NCMBUTTONDBLCLK)
            || (pMsg->message == WM_TIMER && pMsg->wParam == IDT_TIMER && pMsg->hwnd == m_hWnd) || pMsg->message == WM_NCXBUTTONDOWN || pMsg->message == WM_NCXBUTTONDBLCLK)
}

inline void CBalloonTipCtrl::DeleteItem(CWnd* pWnd)
{
    ASSERT_VALID(this);
    ASSERT(::IsWindow(m_hWnd));

    TOOLINFO ti;
    ti.cbSize = sizeof(TOOLINFO);
    ti.uFlags = TTF_IDISHWND;
    ti.uId    = (UINT_PTR)pWnd->GetSafeHwnd();
    ti.hwnd   = ::GetParent(pWnd->GetSafeHwnd());

    ::SendMessage(m_hWnd, TTM_DELTOOL, 0, (LPARAM)&ti);
}

inline BOOL CBalloonTipCtrl::AddItem(CWnd* pWnd, LPCTSTR pszText/* = LPSTR_TEXTCALLBACK*/, UINT nFlags/* = 0*/)
{
    ASSERT_VALID(this);
    ASSERT_VALID(pWnd);
    ASSERT(::IsWindow(m_hWnd));
    ASSERT(::IsWindow(pWnd->m_hWnd));
    ASSERT(!(nFlags & TTF_SUBCLASS));

    if ((GetStyle() & TTS_BALLOON) == 0)
        nFlags |= TTF_ABSOLUTE;

    TOOLINFO ti;
    ti.cbSize = sizeof(TOOLINFO);
    ti.lpszText = (LPTSTR)pszText;
    ti.hinst  = AfxGetResourceHandle();
    ti.uId    = (UINT_PTR)pWnd->m_hWnd;
    ti.hwnd   = ::GetParent(pWnd->m_hWnd);
    ti.uFlags = nFlags | TTF_IDISHWND | TTF_TRACK;

    return ::SendMessage(m_hWnd, TTM_ADDTOOL, 0, (LPARAM)&ti);
}

inline void CBalloonTipCtrl::SetItemText(CWnd* pWnd, LPCTSTR pszText)
{
    ASSERT_VALID(this);
    ASSERT_VALID(pWnd);
    ASSERT(::IsWindow(m_hWnd));
    ASSERT(::IsWindow(pWnd->m_hWnd));

    TOOLINFO ti;
    ti.cbSize = sizeof(TOOLINFO);
    ti.uFlags = TTF_IDISHWND;
    ti.lpszText = (LPTSTR)pszText;
    ti.hinst  = AfxGetResourceHandle();
    ti.uId    = (UINT_PTR)pWnd->m_hWnd;
    ti.hwnd   = ::GetParent(pWnd->m_hWnd);

    ::SendMessage(m_hWnd, TTM_UPDATETIPTEXT, 0, (LPARAM)&ti);
}

inline void CBalloonTipCtrl::ShowBalloonTip(CWnd* pWnd, LPCTSTR pszTitle/* = (LPCTSTR)NULL*/, UINT_PTR nIcon/* = TTI_WARNING*/, int x/* = -1*/, int y/* = -1*/, UINT nElapse/* = 5000*/)
{
    ASSERT_VALID(this);
    ASSERT_VALID(pWnd);
    ASSERT(::IsWindow(m_hWnd));
    ASSERT(::IsWindow(pWnd->m_hWnd));

    if (x == -1 || y == -1)
    {
        sysutil::CWindowRect rcWindow(pWnd->m_hWnd);
        x = rcWindow.left, y = rcWindow.bottom;

        if (GetStyle() & TTS_BALLOON)
            x += 7, y -= 4;
    }

    TOOLINFO ti;
    ti.cbSize = sizeof(TOOLINFO);
    ti.uFlags = TTF_IDISHWND;
    ti.uId    = (UINT_PTR)pWnd->m_hWnd;
    ti.hwnd   = ::GetParent(pWnd->m_hWnd);

    ::SendMessage(m_hWnd, TTM_ACTIVATE, TRUE, 0L);
    ::SendMessage(m_hWnd, TTM_SETTITLE, nIcon, (LPARAM)(LPCTSTR)CString(pszTitle));
    ::SendMessage(m_hWnd, TTM_TRACKPOSITION, 0, MAKELPARAM(x, y));
    ::SendMessage(m_hWnd, TTM_TRACKACTIVATE, TRUE, (LPARAM)&ti);

    ::SetFocus(pWnd->m_hWnd);
    ::SetTimer(m_hWnd, IDT_TIMER, nElapse, NULL);
}

}  // namespace afxctrls

#endif  // __AFXCTRLS_INL__