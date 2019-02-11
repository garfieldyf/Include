///////////////////////////////////////////////////////////////////////////////
// stdctrls.inl
//
// Copyright(c) 2019, Garfield. All rights reserved.
// Author  : Garfield
// Version : 5.0
// Creation Date : 2007/1/21

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __STDCTRLS_INL__
#define __STDCTRLS_INL__

#ifndef __STDCTRLS_H__
    #error stdctrls.inl requires stdctrls.h to be included first
#endif

namespace atlctrls {

///////////////////////////////////////////////////////////////////////////////
// Implementation of the CEditBase class
//

inline CEditBase::CEditBase()
    : m_nState(0)
{
}

inline BOOL CEditBase::OnDrawFrame(HDC /*hDC*/, UINT /*nState*/, const RECT& /*rcWindow*/)
{
    return FALSE;
}

inline BOOL CEditBase::SetState(HWND hWnd, UINT nState)
{
    ATLASSERT(::IsWindow(hWnd));

    m_nState = nState;
    return ::SetWindowPos(hWnd, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_NOZORDER | SWP_DRAWFRAME);
}

inline LRESULT CEditBase::OnSetFocus(HWND hWnd, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
    ATLVERIFY(SetState(hWnd, CDIS_HOT));
    return bHandled = FALSE;
}

inline LRESULT CEditBase::OnKillFocus(HWND hWnd, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
    ATLVERIFY(SetState(hWnd, 0));
    return bHandled = FALSE;
}

inline LRESULT CEditBase::OnMouseMove(HWND hWnd, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
    ATLASSERT(::IsWindow(hWnd));

    if (m_nState != CDIS_HOT)
    {
        ATLVERIFY(SetState(hWnd, CDIS_HOT));
        TRACKMOUSEEVENT tme = { sizeof(TRACKMOUSEEVENT), TME_LEAVE, hWnd, 0 };
        ATLVERIFY(::TrackMouseEvent(&tme));
    }

    return bHandled = FALSE;
}

inline LRESULT CEditBase::OnMouseLeave(HWND hWnd, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
    ATLASSERT(::IsWindow(hWnd));

    if (::GetFocus() != hWnd)
        ATLVERIFY(SetState(hWnd, 0));

    return bHandled = FALSE;
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CDragDropBase class
//

inline CDragDropBase::CDragDropBase()
    : m_hWndDrop(NULL), m_hDragImage(NULL)
#ifdef _ATL_SUPPORT_CDD_GETDRAGDROPFLAG
    , m_dwFlags(DRAGDROP_FLAG_ALLOW)
#endif  // _ATL_SUPPORT_CDD_GETDRAGDROPFLAG
{
}

inline CDragDropBase::~CDragDropBase()
{
    ATLASSERT(m_hDragImage == NULL);
}

inline void CDragDropBase::RegisterDragDrop(HWND hWnd)
{
    m_hWndDrop = hWnd;
}

inline HIMAGELIST CDragDropBase::CreateDragImageEx(LPNMHDR pNMHDR, POINT& ptDrag, POINT& ptHotSpot)
{
    LPNMLISTVIEW pNMListView = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
    ATLASSERT(pNMListView);
    ATLASSERT(::IsWindow(pNMHDR->hwndFrom));

    HIMAGELIST hDragImage = ListView_CreateDragImage(pNMHDR->hwndFrom, pNMListView->iItem, &ptHotSpot);
    if (hDragImage != NULL)
    {
        ptHotSpot.x = pNMListView->ptAction.x - ptHotSpot.x;
        ptHotSpot.y = pNMListView->ptAction.y - ptHotSpot.y;

        ptDrag = pNMListView->ptAction;
        ATLVERIFY(::ScreenToClient(pNMHDR->hwndFrom, &ptDrag));
    }

    return hDragImage;
}

inline void CDragDropBase::BeginDrag(HWND hWnd, POINT point, POINT ptHotSpot)
{
    ATLASSERT(m_hDragImage);
    ATLASSERT(::IsWindow(hWnd));

    if (::ImageList_BeginDrag(m_hDragImage, 0, ptHotSpot.x, ptHotSpot.y))
    {
        ATLVERIFY(::ImageList_DragEnter(::GetDesktopWindow(), point.x, point.y));
        ::SetCapture(hWnd);
    }
}

inline LRESULT CDragDropBase::OnButtonUp(HWND hWnd, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
    ATLASSERT(::IsWindow(hWnd));

    if (m_hDragImage != NULL)
    {
        ATLVERIFY(::ReleaseCapture());
        ATLVERIFY(::ImageList_DragLeave(::GetDesktopWindow()));
        ::ImageList_EndDrag();
        ATLVERIFY(::ImageList_Destroy(m_hDragImage));
        m_hDragImage = NULL;

        CPoint point(lParam);
        ATLVERIFY(::ClientToScreen(hWnd, &point));

    #ifdef _ATL_SUPPORT_CDD_GETDRAGDROPFLAG
        if (::WindowFromPoint(point) == m_hWndDrop && m_dwFlags == DRAGDROP_FLAG_ALLOW)
    #else
        if (::WindowFromPoint(point) == m_hWndDrop)
    #endif  // _ATL_SUPPORT_CDD_GETDRAGDROPFLAG
        {
            int nCtrlID = ::GetDlgCtrlID(hWnd);
            NMDRAGDROP nmdd = { { hWnd, nCtrlID, NM_ENDDRAGDROP }, point, wParam, m_hWndDrop };
            ::SendMessage(::GetParent(hWnd), WM_NOTIFY, nCtrlID, (LPARAM)&nmdd);
        }
    }

    return 0L;
}

inline LRESULT CDragDropBase::OnMouseMove(HWND hWnd, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
    ATLASSERT(::IsWindow(hWnd));

    if ((wParam & (MK_LBUTTON | MK_RBUTTON)) && m_hDragImage != NULL)
    {
        CPoint point(lParam);
        ATLVERIFY(::ClientToScreen(hWnd, &point));

        ATLVERIFY(::ImageList_DragMove(point.x, point.y));
        HWND hWndDrop = ::WindowFromPoint(point);

    #ifdef _ATL_SUPPORT_CDD_GETDRAGDROPFLAG
        LPCTSTR pszCursorName = (hWndDrop == hWnd ? IDC_ARROW : IDC_NO);
        if (hWndDrop == m_hWndDrop)
        {
            int nCtrlID = ::GetDlgCtrlID(hWnd);
            NMDRAGDROP nmdd = { { hWnd, nCtrlID, NM_GETDRAGDROPFLAG }, point, wParam, m_hWndDrop };
            if ((m_dwFlags = ::SendMessage(::GetParent(hWnd), WM_NOTIFY, nCtrlID, (LPARAM)&nmdd)) == DRAGDROP_FLAG_ALLOW)
                pszCursorName = IDC_ARROW;
        }

        ::SetCursor(::LoadCursor(NULL, pszCursorName));
    #else
        ::SetCursor(::LoadCursor(NULL, (hWndDrop == m_hWndDrop || hWndDrop == hWnd ? IDC_ARROW : IDC_NO)));
    #endif  // _ATL_SUPPORT_CDD_GETDRAGDROPFLAG
    }

    return 0L;
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CLinkCtrlBase class
//

inline CLinkCtrlBase::CLinkCtrlBase(DWORD dwExStyle)
    : m_hFont(NULL), m_dwExStyle(dwExStyle), m_bMouseHover(FALSE)
    , m_clrText(::GetSysColor(COLOR_HOTLIGHT)), m_clrHover(::GetSysColor(COLOR_HIGHLIGHT))
{
}

inline CLinkCtrlBase::~CLinkCtrlBase()
{
    ATLVERIFY(::DeleteObject(m_hFont));
}

inline void CLinkCtrlBase::DoPaint(HWND hWnd, HDC hDC, const RECT& rect)
{
    ATLASSERT(hDC);
    ATLASSERT(::IsWindow(hWnd));

    // Draw text
    CString strText;
    ATLVERIFY(atlutil::_AtlGetWindowText(hWnd, strText));
    HGDIOBJ hOldFont = ::SelectObject(hDC, ((m_dwExStyle & LCS_EX_NOTUNDERLINED) == 0 && ((m_dwExStyle & LCS_EX_UNDERLINEHOVER) ? m_bMouseHover : TRUE) ? m_hFont : (HFONT)::SendMessage(hWnd, WM_GETFONT, 0, 0L)));

    ::SetBkMode(hDC, TRANSPARENT);
    ::SetTextColor(hDC, (::IsWindowEnabled(hWnd) ? (m_bMouseHover ? m_clrHover : m_clrText) : ::GetSysColor(COLOR_GRAYTEXT)));
    ::DrawText(hDC, strText, strText.GetLength(), (LPRECT)&rect, DT_WORDBREAK | DT_EXPANDTABS);

    ::SelectObject(hDC, hOldFont);
}

inline void CLinkCtrlBase::OnDrawBkgnd(HWND hWnd, HDC hDC, const RECT& rect)
{
    ATLASSERT(hDC);
    ATLASSERT(::IsWindow(hWnd));

    if (!stdutil::DrawBkgnd(hWnd, hDC, rect))
        ATLVERIFY(::FillRect(hDC, (LPRECT)&rect, (HBRUSH)::SendMessage(::GetParent(hWnd), WM_CTLCOLORSTATIC, (WPARAM)hDC, (LPARAM)hWnd)));
}

inline BOOL CLinkCtrlBase::SizeToContent(HWND hWnd)
{
    ATLASSERT(m_hFont);
    ATLASSERT(::IsWindow(hWnd));

    CString strText;
    ATLVERIFY(atlutil::_AtlGetWindowText(hWnd, strText));

    CRect rect(0, 0, 0, 0);
    stdutil::CalcTextRect(strText, m_hFont, rect, DT_EXPANDTABS);

    return ::SetWindowPos(hWnd, NULL, 0, 0, rect.Width(), rect.Height(), SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
}

inline HFONT CLinkCtrlBase::CreateFont(HFONT hFont)
{
    ATLASSERT(hFont);

    if (m_hFont != NULL)
        ATLVERIFY(::DeleteObject(m_hFont));

    LOGFONT lf;
    ATLVERIFY(::GetObject(hFont, sizeof(LOGFONT), &lf));
    lf.lfUnderline = TRUE;

    return (m_hFont = ::CreateFontIndirect(&lf));
}

inline LRESULT CLinkCtrlBase::OnEnable(HWND hWnd, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    ATLASSERT(::IsWindow(hWnd));

    ATLVERIFY(::InvalidateRect(hWnd, NULL, FALSE));
    ATLVERIFY(::UpdateWindow(hWnd));

    return 0L;
}

inline LRESULT CLinkCtrlBase::OnSetFont(HWND /*hWnd*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
{
    ATLVERIFY(CreateFont(wParam != 0 ? (HFONT)wParam : (HFONT)::GetStockObject(SYSTEM_FONT)));
    return bHandled = FALSE;
}

inline LRESULT CLinkCtrlBase::OnSetCursor(HWND /*hWnd*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    ::SetCursor(::LoadCursor(NULL, IDC_HAND));
    return 1L;
}

inline LRESULT CLinkCtrlBase::OnMouseMove(HWND hWnd, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    ATLASSERT(::IsWindow(hWnd));

    if (!m_bMouseHover && (m_dwExStyle & (LCS_EX_HOVER | LCS_EX_UNDERLINEHOVER)))
    {
        TRACKMOUSEEVENT tme = { sizeof(TRACKMOUSEEVENT), TME_LEAVE, hWnd, 0 };
        m_bMouseHover = ::TrackMouseEvent(&tme);

        ATLVERIFY(::InvalidateRect(hWnd, NULL, FALSE));
        ATLVERIFY(::UpdateWindow(hWnd));
    }

    return 0L;
}

inline LRESULT CLinkCtrlBase::OnMouseLeave(HWND hWnd, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    ATLASSERT(::IsWindow(hWnd));

    m_bMouseHover = FALSE;
    ATLVERIFY(::InvalidateRect(hWnd, NULL, FALSE));
    ATLVERIFY(::UpdateWindow(hWnd));

    return 0L;
}

inline LRESULT CLinkCtrlBase::OnEraseBkgnd(HWND /*hWnd*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    return 1L;
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CBalloonTipCtrlT class
//

template <typename TBase>
inline CBalloonTipCtrlT<TBase>::CBalloonTipCtrlT(HWND hWnd/* = NULL*/)
    : TBase(hWnd)
{
}

template <typename TBase>
inline HWND CBalloonTipCtrlT<TBase>::Create(HWND hWndParent, DWORD dwStyle/* = TTS_BALLOON*/, int nMaxWidth/* = 300*/)
{
    ATLASSERT(::IsWindow(hWndParent));

    if ((m_hWnd = ::CreateWindowEx(WS_EX_TOOLWINDOW, GetWndClassName(), NULL, dwStyle | WS_POPUP, 0, 0, 0, 0, hWndParent, NULL, ATL::_AtlBaseModule.GetModuleInstance(), NULL)) != NULL)
        ::SendMessage(m_hWnd, TTM_SETMAXTIPWIDTH, 0, nMaxWidth);

    return m_hWnd;
}

template <typename TBase>
inline void CBalloonTipCtrlT<TBase>::RelayEvent(LPMSG pMsg)
{
    ATLASSERT(pMsg);
    ATLASSERT(::IsWindow(m_hWnd));

    if (IsInputMessage(pMsg))
        HideBalloonTip();
}

template <typename TBase>
inline BOOL CBalloonTipCtrlT<TBase>::IsInputMessage(LPMSG pMsg) const
{
    return (pMsg->message == WM_KEYDOWN || pMsg->message == WM_SYSKEYDOWN || (pMsg->message > WM_MOUSEFIRST && pMsg->message <= WM_MOUSELAST) || (pMsg->message > WM_NCMOUSEMOVE && pMsg->message <= WM_NCMBUTTONDBLCLK)
            || (pMsg->message == WM_TIMER && pMsg->wParam == IDT_TIMER && pMsg->hwnd == m_hWnd) || pMsg->message == WM_NCXBUTTONDOWN || pMsg->message == WM_NCXBUTTONDBLCLK);
}

template <typename TBase>
inline void CBalloonTipCtrlT<TBase>::DeleteItem(HWND hWnd)
{
    ATLASSERT(::IsWindow(m_hWnd));

    TOOLINFO ti;
    ti.cbSize = sizeof(TOOLINFO);
    ti.uFlags = TTF_IDISHWND;
    ti.uId    = (UINT_PTR)hWnd;
    ti.hwnd   = ::GetParent(hWnd);

    ::SendMessage(m_hWnd, TTM_DELTOOL, 0, (LPARAM)&ti);
}

template <typename TBase>
inline BOOL CBalloonTipCtrlT<TBase>::AddItem(HWND hWnd, ATL::_U_STRINGorID text/* = LPSTR_TEXTCALLBACK*/, UINT nFlags/* = 0*/)
{
    ATLASSERT(::IsWindow(hWnd));
    ATLASSERT(::IsWindow(m_hWnd));
    ATLASSERT(!(nFlags & TTF_SUBCLASS));

    if ((::GetWindowLong(m_hWnd, GWL_STYLE) & TTS_BALLOON) == 0)
        nFlags |= TTF_ABSOLUTE;

    TOOLINFO ti;
    ti.cbSize = sizeof(TOOLINFO);
    ti.uId    = (UINT_PTR)hWnd;
    ti.hwnd   = ::GetParent(hWnd);
    ti.uFlags = nFlags | TTF_IDISHWND | TTF_TRACK;
    ti.lpszText = (LPTSTR)text.m_lpstr;
    ti.hinst  = ATL::_AtlBaseModule.GetResourceInstance();

    return ::SendMessage(m_hWnd, TTM_ADDTOOL, 0, (LPARAM)&ti);
}

template <typename TBase>
inline void CBalloonTipCtrlT<TBase>::HideBalloonTip()
{
    if (::IsWindowVisible(m_hWnd))
    {
        ::KillTimer(m_hWnd, IDT_TIMER);
        ::SendMessage(m_hWnd, TTM_ACTIVATE, FALSE, 0L);
    }
}

template <typename TBase>
inline void CBalloonTipCtrlT<TBase>::ShowBalloonTip(HWND hWnd, ATL::_U_STRINGorID title/* = (LPCTSTR)NULL*/, UINT_PTR nIcon/* = TTI_WARNING*/, int x/* = -1*/, int y/* = -1*/, UINT nElapse/* = 5000*/)
{
    ATLASSERT(::IsWindow(hWnd));
    ATLASSERT(::IsWindow(m_hWnd));

    if (x == -1 || y == -1)
    {
        sysutil::CWindowRect rcWindow(hWnd);
        x = rcWindow.left, y = rcWindow.bottom;

        if (GetStyle() & TTS_BALLOON)
            x += 8, y -= 6;
    }

    TOOLINFO ti;
    ti.cbSize = sizeof(TOOLINFO);
    ti.uFlags = TTF_IDISHWND;
    ti.uId    = (UINT_PTR)hWnd;
    ti.hwnd   = ::GetParent(hWnd);

    ::SendMessage(m_hWnd, TTM_ACTIVATE, TRUE, 0L);
    ::SendMessage(m_hWnd, TTM_SETTITLE, nIcon, (LPARAM)(LPCTSTR)CString(title.m_lpstr));
    ::SendMessage(m_hWnd, TTM_TRACKPOSITION, 0, MAKELPARAM(x, y));
    ::SendMessage(m_hWnd, TTM_TRACKACTIVATE, TRUE, (LPARAM)&ti);

    ::SetFocus(hWnd);
    ::SetTimer(m_hWnd, IDT_TIMER, nElapse, NULL);
}

template <typename TBase>
inline int CBalloonTipCtrlT<TBase>::GetItemCount() const
{
    ATLASSERT(::IsWindow(m_hWnd));
    return (int)::SendMessage(m_hWnd, TTM_GETTOOLCOUNT, 0, 0L);
}

template <typename TBase>
inline CString CBalloonTipCtrlT<TBase>::GetItemText(HWND hWnd) const
{
    ATLASSERT(::IsWindow(hWnd));
    ATLASSERT(::IsWindow(m_hWnd));

    TOOLINFO ti;
    CString strText;

    ti.cbSize = sizeof(TOOLINFO);
    ti.uFlags = TTF_IDISHWND;
    ti.uId    = (UINT_PTR)hWnd;
    ti.hwnd   = ::GetParent(hWnd);
    ti.lpszText = strText.GetBuffer(MAX_PATH);

    ::SendMessage(m_hWnd, TTM_GETTEXT, 0, (LPARAM)&ti);
    strText.ReleaseBuffer();

    return strText;
}

template <typename TBase>
inline HRESULT CBalloonTipCtrlT<TBase>::GetItemText(HWND hWnd, BSTR& bstrText) const
{
    ATLASSERT(::IsWindow(hWnd));
    ATLASSERT(::IsWindow(m_hWnd));

    ::SysFreeString(bstrText);
    bstrText = GetItemText(hWnd).AllocSysString();

    return (bstrText != NULL ? S_OK : E_OUTOFMEMORY);
}

template <typename TBase>
inline void CBalloonTipCtrlT<TBase>::SetItemText(HWND hWnd, ATL::_U_STRINGorID text)
{
    ATLASSERT(::IsWindow(hWnd));
    ATLASSERT(::IsWindow(m_hWnd));

    TOOLINFO ti;
    ti.cbSize = sizeof(TOOLINFO);
    ti.uFlags = TTF_IDISHWND;
    ti.uId    = (UINT_PTR)hWnd;
    ti.hwnd   = ::GetParent(hWnd);
    ti.lpszText = (LPTSTR)text.m_lpstr;
    ti.hinst  = ATL::_AtlBaseModule.GetResourceInstance();

    ::SendMessage(m_hWnd, TTM_UPDATETIPTEXT, 0, (LPARAM)&ti);
}

template <typename TBase>
inline LPCTSTR WINAPI CBalloonTipCtrlT<TBase>::GetWndClassName()
{
    return TOOLTIPS_CLASS;
}

template <typename TBase>
inline CBalloonTipCtrlT<TBase>& CBalloonTipCtrlT<TBase>::operator=(HWND hWnd)
{
    m_hWnd = hWnd;
    return *this;
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CBalloonTipBase class
//

inline CBalloonTipBase::CBalloonTipBase()
{
}

inline void CBalloonTipBase::BalloonTip_SetItemText(HWND hWnd, ATL::_U_STRINGorID text)
{
    m_wndBalloon.SetItemText(hWnd, text);
}

inline void CBalloonTipBase::BalloonTip_DeleteItem(HWND hWnd)
{
    m_wndBalloon.DeleteItem(hWnd);
}

inline BOOL CBalloonTipBase::BalloonTip_AddItem(HWND hWnd, ATL::_U_STRINGorID text/* = LPSTR_TEXTCALLBACK*/, UINT nFlags/* = 0*/)
{
    return m_wndBalloon.AddItem(hWnd, text, nFlags);
}

inline int CBalloonTipBase::BalloonTip_GetItemCount() const
{
    return m_wndBalloon.GetItemCount();
}

inline void CBalloonTipBase::BalloonTip_ShowWindow(HWND hWnd, HWND hWndCtrl, ATL::_U_STRINGorID title, UINT_PTR nIcon, int x, int y, UINT nElapse)
{
    ATLASSERT(::IsWindow(hWnd));
    ATLASSERT(::IsWindow(hWndCtrl));

    MSG msg;
    m_wndBalloon.ShowBalloonTip(hWndCtrl, title, nIcon, x, y, nElapse);

    while (::GetMessage(&msg, NULL, 0, 0) > 0)
    {
        ::TranslateMessage(&msg);
        ::DispatchMessage(&msg);

        if (m_wndBalloon.IsInputMessage(&msg) || (msg.message == WM_USERFIRST && msg.hwnd == hWnd))
            break;
    }

    m_wndBalloon.HideBalloonTip();
    ATLTRACE(_T("CBalloonTipBase::BalloonTip_ShowWindow\n"));
}

inline LRESULT CBalloonTipBase::OnActivate(HWND hWnd, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
{
    if ((LOWORD(wParam) == WA_INACTIVE || HIWORD(wParam)) && (::IsWindowVisible(m_wndBalloon.m_hWnd)))
    {
        ATLASSERT(::IsWindow(hWnd));
        ATLVERIFY(::PostMessage(hWnd, WM_USERFIRST, 0, 0L));
    }

    return bHandled = FALSE;
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CHeaderCtrlBase class
//

__declspec(selectany) BOOL CHeaderCtrlBase::m_bThemeActive = sysutil::IsThemeActive();

inline CHeaderCtrlBase::CHeaderCtrlBase(int nHeight)
    : m_nItem(-1), m_nState(0)
#ifdef _ATL_SUPPORT_CHD_SETHEIGHT
    , m_nHeight(nHeight)
#endif  // _ATL_SUPPORT_CHD_SETHEIGHT
{
    UNUSED_PARAMETER(nHeight);
}

inline void CHeaderCtrlBase::OnDrawItem(const HDDRAWITEM& /*hddi*/)
{
    ATLASSERT(FALSE && _T("MUST BE IMPLEMENTED IN A DERIVED CLASS."));
}

inline void CHeaderCtrlBase::OnDrawBkgnd(const HDDRAWITEM& /*hddi*/)
{
    ATLASSERT(FALSE && _T("MUST BE IMPLEMENTED IN A DERIVED CLASS."));
}

inline void CHeaderCtrlBase::OnMouseMove(int nItem, UINT nFlags, UINT /*nHitFlags*/)
{
    if (m_bThemeActive)
        m_nState = (nFlags == MK_LBUTTON && m_nItem == nItem ? CDIS_SELECTED : CDIS_HOT);

    m_nItem = nItem;
}

inline LRESULT CHeaderCtrlBase::OnMouseEvent(HWND /*hWnd*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
    m_nItem = -1;
    return bHandled = FALSE;
}

inline LRESULT CHeaderCtrlBase::OnEraseBkgnd(HWND /*hWnd*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    return 1L;
}

inline LRESULT CHeaderCtrlBase::OnLButtonDown(HWND hWnd, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled)
{
    ATLASSERT(::IsWindow(hWnd));

    HDHITTESTINFO hitInfo = { { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) } };
    m_nItem  = ::SendMessage(hWnd, HDM_HITTEST, 0, (LPARAM)&hitInfo);
    m_nState = CDIS_SELECTED;

    return bHandled = FALSE;
}

inline LRESULT CHeaderCtrlBase::OnThemeChanged(HWND /*hWnd*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    m_bThemeActive = sysutil::IsThemeActive();
    return 0L;
}


#ifdef __IMAGE_H__
///////////////////////////////////////////////////////////////////////////////
// Implementation of the CImageCtrlBase class
//

inline CImageCtrlBase::CImageCtrlBase()
    : m_nSizeMode(SIZE_MODE_NORMAL)
{
}

inline BOOL CImageCtrlBase::LoadImage(LPCTSTR pszFileName, BOOL bUseEmbeddedColorManagement/* = FALSE*/)
{
    return m_image.Load(pszFileName, bUseEmbeddedColorManagement);
}

inline BOOL CImageCtrlBase::LoadImage(IStream* pStream, BOOL bUseEmbeddedColorManagement/* = FALSE*/)
{
    return m_image.Load(pStream, bUseEmbeddedColorManagement);
}

inline BOOL CImageCtrlBase::LoadImage(const BITMAPINFO* pBitmapInfo, LPVOID pvBitmapData)
{
    return m_image.Load(pBitmapInfo, pvBitmapData);
}

inline BOOL CImageCtrlBase::LoadImage(HINSTANCE hInstance, UINT nIDResource, LPCTSTR pType/* = NULL*/, BOOL bUseEmbeddedColorManagement/* = FALSE*/)
{
    return m_image.Load(hInstance, nIDResource, pType, bUseEmbeddedColorManagement);
}

inline BOOL CImageCtrlBase::SaveImage(IStream* pStream, const CLSID* pclsIdEncoder, const Gdiplus::EncoderParameters* pParameters/* = NULL*/) const
{
    return m_image.Save(pStream, pclsIdEncoder, pParameters);
}

inline BOOL CImageCtrlBase::SaveImage(LPCWSTR pszFileName, const CLSID* pclsIdEncoder, const Gdiplus::EncoderParameters* pParameters/* = NULL*/) const
{
    return m_image.Save(pszFileName, pclsIdEncoder, pParameters);
}

#if defined(__ATLSAFE_H__) && defined(__ATLCOMUTIL_H__)
inline HRESULT CImageCtrlBase::LoadImage(const ATL::CComSafeArray<BYTE>& saImage, BOOL bUseEmbeddedColorManagement/* = FALSE*/)
{
    return m_image.Load(saImage, bUseEmbeddedColorManagement);
}

inline HRESULT CImageCtrlBase::SaveImage(ATL::CComSafeArray<BYTE>& saImage, const CLSID* pclsIdEncoder, const Gdiplus::EncoderParameters* pParameters/* = NULL*/) const
{
    return m_image.Save(saImage, pclsIdEncoder, pParameters);
}
#endif  // defined(__ATLSAFE_H__) && defined(__ATLCOMUTIL_H__)

inline void CImageCtrlBase::DoPaint(HWND hWnd, HDC hDC)
{
    ATLASSERT(hDC);
    ATLASSERT(::IsWindow(hWnd));

    if (m_image.m_hHandle)
    {
        // Calculate srcRect and destRect size
        BITMAP bmp;
        ATLVERIFY(m_image.GetBitmap(bmp));

        sysutil::CClientRect destRect(hWnd);
        CRect srcRect(0, 0, bmp.bmWidth, bmp.bmHeight);
        stdutil::CalcDrawRect(m_nSizeMode, srcRect, destRect);

        // Draw image
        ATLVERIFY(m_image.Draw(hDC, destRect, srcRect.left, srcRect.top, srcRect.Width(), srcRect.Height()));
    }
}

inline BOOL CImageCtrlBase::IsImageEmpty() const
{
    return m_image.IsEmpty();
}

inline BOOL CImageCtrlBase::GetBitmap(BITMAP& bmp) const
{
    return m_image.GetBitmap(bmp);
}

inline LRESULT CImageCtrlBase::OnEraseBkgnd(HWND /*hWnd*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    return 1L;
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CAnimImageCtrlBase class
//

inline CAnimImageCtrlBase::CAnimImageCtrlBase()
    : m_bPlaying(FALSE), m_nFrameIndex(0)
{
}

inline BOOL CAnimImageCtrlBase::LoadImage(LPCTSTR pszFileName, BOOL bUseEmbeddedColorManagement/* = FALSE*/)
{
    return m_AnimImage.Load(pszFileName, bUseEmbeddedColorManagement);
}

inline BOOL CAnimImageCtrlBase::LoadImage(IStream* pStream, BOOL bUseEmbeddedColorManagement/* = FALSE*/)
{
    return m_AnimImage.Load(pStream, bUseEmbeddedColorManagement);
}

#if defined(__ATLSAFE_H__) && defined(__ATLCOMUTIL_H__)
inline HRESULT CAnimImageCtrlBase::LoadImage(const ATL::CComSafeArray<BYTE>& saImage, BOOL bUseEmbeddedColorManagement/* = FALSE*/)
{
    return m_AnimImage.Load(saImage, bUseEmbeddedColorManagement);
}
#endif  // defined(__ATLSAFE_H__) && defined(__ATLCOMUTIL_H__)

#ifdef _CRT_DEBUG_DUMP
template <typename _Pr>
inline void CAnimImageCtrlBase::Dump(const _Pr& _Printer) const
{
    m_AnimImage.Dump(_Printer);
}
#endif  // _CRT_DEBUG_DUMP

inline void CAnimImageCtrlBase::DoPaint(HDC hDC, const RECT& rect)
{
    ATLASSERT(hDC);

    if (!m_AnimImage.IsEmpty())
    {
        if (m_AnimImage.IsAlphaChannel())
            m_AnimImage.Draw(hDC, m_nFrameIndex, rect);
        else
            m_AnimImage.Draw(hDC, m_nFrameIndex, rect.left, rect.top);
    }
}

inline void CAnimImageCtrlBase::OnDrawBkgnd(HDC /*hDC*/, const RECT& /*rect*/)
{
}

inline BOOL CAnimImageCtrlBase::IsPlaying() const
{
    return m_bPlaying;
}

inline BOOL CAnimImageCtrlBase::IsImageEmpty() const
{
    return m_AnimImage.IsEmpty();
}

inline BOOL CAnimImageCtrlBase::GetBitmap(BITMAP& bmp) const
{
    return m_AnimImage.GetBitmap(bmp);
}

inline LRESULT CAnimImageCtrlBase::OnTimer(HWND hWnd, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    ATLASSERT(::IsWindow(hWnd));
    ATLASSERT(!m_AnimImage.IsEmpty());

    ATLVERIFY(::InvalidateRect(hWnd, NULL, FALSE));
    ATLVERIFY(::UpdateWindow(hWnd));

    UINT nDelay = m_AnimImage.GetFrameDelay(m_nFrameIndex);
    m_nFrameIndex = (m_nFrameIndex + 1) % m_AnimImage.GetFrameCount();
    _COND_VERIFY(::SetTimer(hWnd, IDT_TIMER, nDelay, NULL), IDT_TIMER);

    return 0L;
}

inline LRESULT CAnimImageCtrlBase::OnEraseBkgnd(HWND /*hWnd*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    return 1L;
}
#endif  // __IMAGE_H__


#ifdef __PICTURE_H__
///////////////////////////////////////////////////////////////////////////////
// Implementation of the CPictureCtrlBase class
//

inline CPictureCtrlBase::CPictureCtrlBase()
    : m_nSizeMode(SIZE_MODE_NORMAL)
{
}

inline HRESULT CPictureCtrlBase::LoadPicture(LPCTSTR pszURLorPath)
{
    return m_Picture.Load(pszURLorPath);
}

inline HRESULT CPictureCtrlBase::LoadPicture(IStream* pStream, BOOL bRunmode/* = FALSE*/)
{
    return m_Picture.Load(pStream, bRunmode);
}

inline HRESULT CPictureCtrlBase::LoadPicture(HINSTANCE hInstance, UINT nIDResource, LPCTSTR pType, BOOL bRunmode/* = FALSE*/)
{
    return m_Picture.Load(hInstance, nIDResource, pType, bRunmode);
}

inline HRESULT CPictureCtrlBase::SavePicture(LPCTSTR pszFileName) const
{
    return m_Picture.Save(pszFileName);
}

inline HRESULT CPictureCtrlBase::SavePicture(IStream* pStream, BOOL bSaveMemCopy/* = FALSE*/, LONG* pcbSize/* = NULL*/) const
{
    return m_Picture.Save(pStream, bSaveMemCopy, pcbSize);
}

#if defined(__ATLSAFE_H__) && defined(__ATLCOMUTIL_H__)
inline HRESULT CPictureCtrlBase::SavePicture(ATL::CComSafeArray<BYTE>& saPicture) const
{
    return m_Picture.Save(saPicture);
}

inline HRESULT CPictureCtrlBase::LoadPicture(const ATL::CComSafeArray<BYTE>& saPicture, BOOL bRunmode/* = FALSE*/)
{
    return m_Picture.Load(saPicture, bRunmode);
}
#endif  // defined(__ATLSAFE_H__) && defined(__ATLCOMUTIL_H__)

inline void CPictureCtrlBase::DoPaint(HWND hWnd, HDC hDC)
{
    ATLASSERT(hDC);
    ATLASSERT(::IsWindow(hWnd));

    if (m_Picture.m_spPicture != NULL)
    {
        sysutil::CClientRect destRect(hWnd);
        CRect srcRect(0, 0, m_Picture.GetWidth(), m_Picture.GetHeight());

        // Calculate srcRect and destRect size
        stdutil::HIMETRICToPixel(hDC, (LPSIZE)&srcRect + 1);
        stdutil::CalcDrawRect(m_nSizeMode, srcRect, destRect);

        // Draw image
        _COM_VERIFY(m_Picture.Draw(hDC, destRect, srcRect.left, srcRect.top, srcRect.Width(), srcRect.Height()));
    }
}

inline BOOL CPictureCtrlBase::IsPictureEmpty() const
{
    return m_Picture.IsEmpty();
}

inline OLE_XSIZE_HIMETRIC CPictureCtrlBase::GetPictureWidth() const
{
    return m_Picture.GetWidth();
}

inline OLE_YSIZE_HIMETRIC CPictureCtrlBase::GetPictureHeight() const
{
    return m_Picture.GetHeight();
}

inline LRESULT CPictureCtrlBase::OnEraseBkgnd(HWND /*hWnd*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    return 1L;
}
#endif  // __PICTURE_H__


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CShellTrayNotify class
//

inline BOOL WINAPI CShellTrayNotify::Create(HWND hWnd, UINT nID, HICON hIcon, LPCTSTR pszTip, UINT nCallbackMessage/* = 0*/)
{
    ATLASSERT(hIcon);
    ATLASSERT(pszTip);
    ATLASSERT(::IsWindow(hWnd));

    NOTIFYICONDATA nid;
    nid.cbSize = sizeof(NOTIFYICONDATA);
    nid.uID    = nID;
    nid.hWnd   = hWnd;
    nid.hIcon  = hIcon;
    nid.uCallbackMessage = nCallbackMessage;
    nid.uFlags = (nCallbackMessage != 0 ? NIF_TIP | NIF_ICON | NIF_MESSAGE : NIF_TIP | NIF_ICON);
    ::_tsstrcpy(nid.szTip, pszTip, _countof(nid.szTip));

    return ::Shell_NotifyIcon(NIM_ADD, &nid);
}

inline BOOL WINAPI CShellTrayNotify::Create(HWND hWnd, UINT nID, HICON hIcon, LPCTSTR pszTip, LPCTSTR pszInfo, LPCTSTR pszInfoTitle, UINT nCallbackMessage/* = 0*/, DWORD dwInfoFlags/* = NIIF_INFO*/, HICON hBalloonIcon/* = NULL*/, UINT nTimeout/* = 5000*/)
{
    ATLASSERT(hIcon);
    ATLASSERT(pszTip);
    ATLASSERT(pszInfo);
    ATLASSERT(pszInfoTitle);
    ATLASSERT(::IsWindow(hWnd));
    UNUSED_PARAMETER(hBalloonIcon);

    NOTIFYICONDATA nid;
    nid.cbSize = sizeof(NOTIFYICONDATA);
    nid.uID    = nID;
    nid.hWnd   = hWnd;
    nid.hIcon  = hIcon;
    nid.uTimeout    = nTimeout;
    nid.dwInfoFlags = dwInfoFlags;
    nid.uCallbackMessage = nCallbackMessage;
    nid.uFlags = (nCallbackMessage != 0 ? NIF_ICON | NIF_INFO | NIF_TIP | NIF_MESSAGE : NIF_ICON | NIF_INFO | NIF_TIP);
    ::_tsstrcpy(nid.szTip, CString(pszTip), _countof(nid.szTip));
    ::_tsstrcpy(nid.szInfo, CString(pszInfo), _countof(nid.szInfo));
    ::_tsstrcpy(nid.szInfoTitle, CString(pszInfoTitle), _countof(nid.szInfoTitle));

#if (_WIN32_WINNT >= 0x0600)
    nid.hBalloonIcon = hBalloonIcon;
#endif  // (_WIN32_WINNT >= 0x0600)

    return ::Shell_NotifyIcon(NIM_ADD, &nid);
}

inline BOOL WINAPI CShellTrayNotify::SetIcon(HWND hWnd, UINT nID, HICON hIcon)
{
    ATLASSERT(hIcon);
    ATLASSERT(::IsWindow(hWnd));

    NOTIFYICONDATA nid;
    nid.cbSize = sizeof(NOTIFYICONDATA);
    nid.uID    = nID;
    nid.hWnd   = hWnd;
    nid.hIcon  = hIcon;
    nid.uFlags = NIF_ICON;

    return ::Shell_NotifyIcon(NIM_MODIFY, &nid);
}

inline BOOL WINAPI CShellTrayNotify::SetTipText(HWND hWnd, UINT nID, LPCTSTR pszTip)
{
    ATLASSERT(pszTip);
    ATLASSERT(::IsWindow(hWnd));

    NOTIFYICONDATA nid;
    nid.cbSize = sizeof(NOTIFYICONDATA);
    nid.uID    = nID;
    nid.hWnd   = hWnd;
    nid.uFlags = NIF_TIP;
    ::_tsstrcpy(nid.szTip, CString(pszTip), _countof(nid.szTip));

    return ::Shell_NotifyIcon(NIM_MODIFY, &nid);
}

inline BOOL WINAPI CShellTrayNotify::SetCallbackMessage(HWND hWnd, UINT nID, UINT nCallbackMessage)
{
    ATLASSERT(::IsWindow(hWnd));

    NOTIFYICONDATA nid;
    nid.cbSize = sizeof(NOTIFYICONDATA);
    nid.uID    = nID;
    nid.hWnd   = hWnd;
    nid.uFlags = NIF_MESSAGE;
    nid.uCallbackMessage = nCallbackMessage;

    return ::Shell_NotifyIcon(NIM_MODIFY, &nid);
}

inline BOOL WINAPI CShellTrayNotify::SetState(HWND hWnd, UINT nID, DWORD dwState, DWORD dwStateMask)
{
    ATLASSERT(::IsWindow(hWnd));

    NOTIFYICONDATA nid;
    nid.cbSize  = sizeof(NOTIFYICONDATA);
    nid.uID     = nID;
    nid.hWnd    = hWnd;
    nid.dwState = dwState;
    nid.uFlags  = NIF_STATE;
    nid.dwStateMask = dwStateMask;

    return ::Shell_NotifyIcon(NIM_MODIFY, &nid);
}

inline BOOL WINAPI CShellTrayNotify::SetVersion(HWND hWnd, UINT nID, UINT nVersion/* = NOTIFYICON_VERSION*/)
{
    ATLASSERT(::IsWindow(hWnd));

    NOTIFYICONDATA nid;
    nid.cbSize = sizeof(NOTIFYICONDATA);
    nid.uID    = nID;
    nid.hWnd   = hWnd;
    nid.uVersion = nVersion;

    return ::Shell_NotifyIcon(NIM_SETVERSION, &nid);
}

inline BOOL WINAPI CShellTrayNotify::Destroy(HWND hWnd, UINT nID)
{
    ATLASSERT(::IsWindow(hWnd));

    NOTIFYICONDATA nid;
    nid.cbSize = sizeof(NOTIFYICONDATA);
    nid.uID    = nID;
    nid.hWnd   = hWnd;

    return ::Shell_NotifyIcon(NIM_DELETE, &nid);
}

inline BOOL WINAPI CShellTrayNotify::NotifyIcon(const NOTIFYICONDATA& nid, DWORD dwMessage/* = NIM_MODIFY*/)
{
    return ::Shell_NotifyIcon(dwMessage, (PNOTIFYICONDATA)&nid);
}

inline BOOL WINAPI CShellTrayNotify::ShowBalloonTip(HWND hWnd, UINT nID, LPCTSTR pszInfo, LPCTSTR pszInfoTitle, DWORD dwInfoFlags/* = NIIF_INFO*/, HICON hBalloonIcon/* = NULL*/, UINT nTimeout/* = 5000*/)
{
    ATLASSERT(pszInfo);
    ATLASSERT(pszInfoTitle);
    ATLASSERT(::IsWindow(hWnd));
    UNUSED_PARAMETER(hBalloonIcon);

    NOTIFYICONDATA nid;
    nid.cbSize = sizeof(NOTIFYICONDATA);
    nid.uID    = nID;
    nid.hWnd   = hWnd;
    nid.uFlags = NIF_INFO;
    nid.uTimeout    = nTimeout;
    nid.dwInfoFlags = dwInfoFlags;
    ::_tsstrcpy(nid.szInfo, CString(pszInfo), _countof(nid.szInfo));
    ::_tsstrcpy(nid.szInfoTitle, CString(pszInfoTitle), _countof(nid.szInfoTitle));

#if (_WIN32_WINNT >= 0x0600)
    nid.hBalloonIcon = hBalloonIcon;
#endif  // (_WIN32_WINNT >= 0x0600)

    return ::Shell_NotifyIcon(NIM_MODIFY, &nid);
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CImageButtonBase class
//

inline CImageButtonBase::CImageButtonBase()
    : m_bMouseHover(FALSE)
{
    ::memset(m_nImages, 0, sizeof(m_nImages));
}

inline void CImageButtonBase::OnDrawBkgnd(LPDRAWITEMSTRUCT pDrawItem)
{
    ATLASSERT(pDrawItem);
    ATLASSERT(pDrawItem->hDC);
    ATLASSERT(::IsWindow(pDrawItem->hwndItem));
    ATLASSERT(pDrawItem->CtlType == ODT_BUTTON);

    if (!stdutil::DrawBkgnd(pDrawItem->hwndItem, pDrawItem->hDC, pDrawItem->rcItem))
        ATLVERIFY(::FillRect(pDrawItem->hDC, &pDrawItem->rcItem, (HBRUSH)::SendMessage(::GetParent(pDrawItem->hwndItem), WM_CTLCOLORBTN, (WPARAM)pDrawItem->hDC, (LPARAM)pDrawItem->hwndItem)));
}

inline void CImageButtonBase::SetImages(int nNormal, int nHover/* = 0*/, int nSelected/* = 0*/, int nDisabled/* = 0*/)
{
    m_nImages[IMAGE_NORMAL]   = nNormal;
    m_nImages[IMAGE_HOVER]    = nHover;
    m_nImages[IMAGE_SELECTED] = nSelected;
    m_nImages[IMAGE_DISABLED] = nDisabled;
}

inline int CImageButtonBase::GetImageIndex(UINT nState) const
{
    int nImage = m_nImages[IMAGE_NORMAL];
    if (nState & ODS_SELECTED)
        nImage = m_nImages[IMAGE_SELECTED];
    else if (nState & ODS_DISABLED)
        nImage = m_nImages[IMAGE_DISABLED];
    else if (m_bMouseHover)
        nImage = m_nImages[IMAGE_HOVER];

    return nImage;
}

inline LRESULT CImageButtonBase::OnMouseMove(HWND hWnd, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
    ATLASSERT(::IsWindow(hWnd));

    if (!m_bMouseHover)
    {
        TRACKMOUSEEVENT tme = { sizeof(TRACKMOUSEEVENT), TME_LEAVE, hWnd, 0 };
        m_bMouseHover = ::TrackMouseEvent(&tme);

        ATLVERIFY(::InvalidateRect(hWnd, NULL, FALSE));
        ATLVERIFY(::UpdateWindow(hWnd));

    #ifdef _ATL_SUPPORT_CBN_MOUSENOTIFY
        ATLVERIFY(::PostMessage(::GetParent(hWnd), WM_COMMAND, MAKEWPARAM(::GetDlgCtrlID(hWnd), BN_MOUSEHOVER), (LPARAM)hWnd));
    #endif  // _ATL_SUPPORT_CBN_MOUSENOTIFY
    }

    return bHandled = FALSE;
}

inline LRESULT CImageButtonBase::OnMouseLeave(HWND hWnd, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    ATLASSERT(::IsWindow(hWnd));

    m_bMouseHover = FALSE;
    ATLVERIFY(::InvalidateRect(hWnd, NULL, FALSE));
    ATLVERIFY(::UpdateWindow(hWnd));

#ifdef _ATL_SUPPORT_CBN_MOUSENOTIFY
    ATLVERIFY(::PostMessage(::GetParent(hWnd), WM_COMMAND, MAKEWPARAM(::GetDlgCtrlID(hWnd), BN_MOUSELEAVE), (LPARAM)hWnd));
#endif  // _ATL_SUPPORT_CBN_MOUSENOTIFY

    return 0L;
}

inline LRESULT CImageButtonBase::OnEraseBkgnd(HWND /*hWnd*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    return 1L;
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CSplashWindowBase class
//

inline CSplashWindowBase::CSplashWindowBase()
    : m_hBitmap(NULL)
{
}

inline POSITION CSplashWindowBase::AddDrawText(const DRAWTEXT& drawText)
{
    return m_drawTexts.AddTail(drawText);
}

inline POSITION CSplashWindowBase::AddDrawText(UINT nIDText, const RECT& rect, COLORREF clrText/* = RGB(0, 0, 0)*/, UINT nFormat/* = DT_SINGLELINE | DT_VCENTER*/, LPLOGFONT pLogFont/* = NULL*/)
{
    return AddDrawText(CString(MAKEINTRESOURCE(nIDText)), rect, clrText, nFormat, pLogFont);
}

inline POSITION CSplashWindowBase::AddDrawText(LPCTSTR pszText, const RECT& rect, COLORREF clrText/* = RGB(0, 0, 0)*/, UINT nFormat/* = DT_SINGLELINE | DT_VCENTER*/, LPLOGFONT pLogFont/* = NULL*/)
{
    ATLASSERT(pszText);

    DRAWTEXT drawText;
    drawText.rcText  = rect;
    drawText.clrText = clrText;
    drawText.nFormat = nFormat;
    ::lstrcpyn(drawText.szText, pszText, _countof(drawText.szText));

    if (pLogFont != NULL)
        ::CopyMemory(&drawText.lfLogFont, pLogFont, sizeof(LOGFONT));
    else
        ATLVERIFY(::SystemParametersInfo(SPI_GETICONTITLELOGFONT, sizeof(LOGFONT), &drawText.lfLogFont, 0));

    return m_drawTexts.AddTail(drawText);
}

inline void CSplashWindowBase::DoPaint(HWND hWnd, HDC hDC)
{
    ATLASSERT(hDC);
    ATLASSERT(m_hBitmap);
    ATLASSERT(::IsWindow(hWnd));

    // Draw background
    sysutil::CClientRect rcClient(hWnd);
    ::SetBkMode(hDC, TRANSPARENT);
    ATLVERIFY(::DrawState(hDC, NULL, NULL, (LPARAM)m_hBitmap, 0, rcClient.left, rcClient.top, rcClient.right, rcClient.bottom, DSS_NORMAL | DST_BITMAP));

    // Draw text
    for (POSITION pos = m_drawTexts.GetHeadPosition(); pos != NULL; )
    {
        DRAWTEXT& drawText = m_drawTexts.GetNext(pos);
        HFONT hFont = ::CreateFontIndirect(&drawText.lfLogFont);
        ATLASSERT(hFont);

        HGDIOBJ hOldFont = ::SelectObject(hDC, hFont);
        ::SetTextColor(hDC, drawText.clrText);
        ::DrawText(hDC, drawText.szText, -1, &drawText.rcText, drawText.nFormat);

        ::SelectObject(hDC, hOldFont);
        ATLVERIFY(::DeleteObject(hFont));
    }
}

inline void CSplashWindowBase::SetDrawText(HWND hWnd, POSITION pos, LPCTSTR pszText, COLORREF clrText, LPLOGFONT pLogFont, UINT nFormat, LPCRECT pRect)
{
    ATLASSERT(pos != NULL);
    ATLASSERT(::IsWindow(hWnd));

    DRAWTEXT& drawText = m_drawTexts.GetAt(pos);
    if (nFormat != (UINT)-1)
        drawText.nFormat = nFormat;
    if (clrText != CLR_INVALID)
        drawText.clrText = clrText;
    if (pRect != NULL)
        drawText.rcText = *pRect;
    if (pLogFont != NULL)
        ::CopyMemory(&drawText.lfLogFont, pLogFont, sizeof(LOGFONT));
    if (pszText != NULL)
        ::lstrcpyn(drawText.szText, pszText, _countof(drawText.szText));

    ATLVERIFY(::InvalidateRect(hWnd, NULL, FALSE));
    ATLVERIFY(::UpdateWindow(hWnd));
}

inline LRESULT CSplashWindowBase::OnEraseBkgnd(HWND /*hWnd*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    return 1L;
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CSortListCtrlBase class
//

inline CSortListCtrlBase::CSortListCtrlBase()
    : m_nSortCol(-1), m_bAscending(TRUE)
{
}

inline int WINAPI CSortListCtrlBase::CompareInteger(LPCTSTR pszSource, LPCTSTR pszDest)
{
    ATLASSERT(pszDest);
    ATLASSERT(pszSource);

    INT64 nSource = ::_ttoi64(pszSource), nDest = ::_ttoi64(pszDest);
    return (nSource > nDest ? 1 : (nSource < nDest ? -1 : 0));
}

inline int WINAPI CSortListCtrlBase::CompareDecimal(LPCTSTR pszSource, LPCTSTR pszDest)
{
    ATLASSERT(pszDest);
    ATLASSERT(pszSource);

    double dblSource = ::_tstof(pszSource), dblDest = ::_tstof(pszDest);
    return (dblSource > dblDest ? 1 : (dblSource < dblDest ? -1 : 0));
}

inline int WINAPI CSortListCtrlBase::CompareDateTime(LPCTSTR pszSource, LPCTSTR pszDest)
{
    ATLASSERT(pszDest);
    ATLASSERT(pszSource);

    COleDateTime dtSource, dtDest;
    ATLVERIFY(dtDest.ParseDateTime(pszDest));
    ATLVERIFY(dtSource.ParseDateTime(pszSource));

    return (dtSource > dtDest ? 1 : (dtSource < dtDest ? -1 : 0));
}

inline int CALLBACK CSortListCtrlBase::CompareItem(LPARAM lSourceItem, LPARAM lDestItem, LPARAM lSortParam)
{
    LPLVSORTSTRUCT pSortStruct = reinterpret_cast<LPLVSORTSTRUCT>(lSortParam);
    ATLASSERT(pSortStruct);
    ATLASSERT(::IsWindow(pSortStruct->hWnd));

    LVITEM lvItem;
    TCHAR szSource[MAX_PATH], szDest[MAX_PATH];

    lvItem.iSubItem = pSortStruct->nSortCol, lvItem.pszText = szSource, lvItem.cchTextMax = _countof(szSource);
    ::SendMessage(pSortStruct->hWnd, LVM_GETITEMTEXT, lSourceItem, (LPARAM)&lvItem);

    lvItem.pszText = szDest;
    ::SendMessage(pSortStruct->hWnd, LVM_GETITEMTEXT, lDestItem, (LPARAM)&lvItem);

    int nResult = 0;
    switch (pSortStruct->nSortMode)
    {
    case LVSM_TEXT:
        nResult = ::lstrcmp(szSource, szDest);
        break;

    case LVSM_TEXTNOCASE:
        nResult = ::lstrcmpi(szSource, szDest);
        break;

    case LVSM_INTEGER:
        nResult = CompareInteger(szSource, szDest);
        break;

    case LVSM_DECIMAL:
        nResult = CompareDecimal(szSource, szDest);
        break;

    case LVSM_DATETIME:
        nResult = CompareDateTime(szSource, szDest);
        break;

    default:
        ATLASSERT(FALSE);
    };

    return (pSortStruct->bAscending) ? nResult : -nResult;
}

inline void CSortListCtrlBase::SetSortCol(HWND hWnd, int nCol, BOOL bAscending)
{
    HWND hWndHD = ListView_GetHeader(hWnd);
    ATLASSERT(::IsWindow(hWnd));
    ATLASSERT(::IsWindow(hWndHD));
    ATLASSERT(nCol >= 0 && nCol < Header_GetItemCount(hWndHD));

    HDITEM hdItem;
    hdItem.mask = HDI_FORMAT;
    Header_GetItem(hWndHD, m_nSortCol, &hdItem);
    hdItem.fmt &= ~(HDF_SORTUP | HDF_SORTDOWN);
    Header_SetItem(hWndHD, m_nSortCol, &hdItem);

    m_nSortCol   = nCol;
    m_bAscending = bAscending;

    ATLVERIFY(Header_GetItem(hWndHD, nCol, &hdItem));
    hdItem.fmt |= (bAscending ? HDF_SORTUP : HDF_SORTDOWN);
    ATLVERIFY(Header_SetItem(hWndHD, nCol, &hdItem));
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CImageListBoxBase class
//

inline CImageListBoxBase::CImageListBoxBase()
    : m_hImageList(NULL)
#ifdef _ATL_SUPPORT_CILB_TEXTCOLOR
    , m_clrBack(::GetSysColor(COLOR_WINDOW))
    , m_clrText(::GetSysColor(COLOR_WINDOWTEXT))
#endif  // _ATL_SUPPORT_CILB_TEXTCOLOR
{
}

inline int CImageListBoxBase::GetItemImage(int nItem) const
{
    ATLASSERT(nItem >= 0 && nItem < (int)m_arrImages.GetCount());
    return m_arrImages[nItem];
}

inline void CImageListBoxBase::SetItemImage(int nItem, int nImage)
{
    ATLASSERT(nItem >= 0 && nItem < (int)m_arrImages.GetCount());
    m_arrImages[nItem] = nImage;
}

inline BOOL CImageListBoxBase::SetImageList(HWND hWnd, HIMAGELIST hImageList)
{
    ATLASSERT(::IsWindow(hWnd));
    ATLASSERT(::GetWindowLong(hWnd, GWL_STYLE) & LBS_HASSTRINGS);
    ATLASSERT(::GetWindowLong(hWnd, GWL_STYLE) & LBS_OWNERDRAWFIXED);

    int nImageWidth = 0, nImageHeight = 0;
    BOOL bSuccessful = ::ImageList_GetIconSize(hImageList, &nImageWidth, &nImageHeight);
    if (bSuccessful)
    {
        m_hImageList = hImageList;
        ::SendMessage(hWnd, LB_SETITEMHEIGHT, 0, nImageHeight);
    }

    return bSuccessful;
}

inline int CImageListBoxBase::AddItem(HWND hWnd, LPCTSTR pszText, int nImage)
{
    ATLASSERT(::IsWindow(hWnd));

    int nItem = (int)::SendMessage(hWnd, LB_ADDSTRING, 0, (LPARAM)pszText);
    if (nItem != LB_ERR)
        m_arrImages.InsertAt(nItem, nImage);

    return nItem;
}

inline int CImageListBoxBase::InsertItem(HWND hWnd, int nItem, LPCTSTR pszText, int nImage)
{
    ATLASSERT(::IsWindow(hWnd));

    nItem = (int)::SendMessage(hWnd, LB_INSERTSTRING, nItem, (LPARAM)pszText);
    if (nItem != LB_ERR)
        m_arrImages.InsertAt(nItem, nImage);

    return nItem;
}

#ifdef _ATL_SUPPORT_CILB_TEXTCOLOR
inline LRESULT CImageListBoxBase::OnEnable(HWND hWnd, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    ATLASSERT(::IsWindow(hWnd));

    ATLVERIFY(::InvalidateRect(hWnd, NULL, TRUE));
    ATLVERIFY(::UpdateWindow(hWnd));

    return 0L;
}

inline LRESULT CImageListBoxBase::OnEraseBkgnd(HWND hWnd, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    ATLASSERT(wParam);
    ATLASSERT(::IsWindow(hWnd));

    HBRUSH hBrush = ::CreateSolidBrush(::IsWindowEnabled(hWnd) ? m_clrBack : ::GetSysColor(COLOR_WINDOW));
    ATLVERIFY(::FillRect((HDC)wParam, sysutil::CClientRect(hWnd), hBrush));
    ATLVERIFY(::DeleteObject(hBrush));

    return 1L;
}
#endif  // _ATL_SUPPORT_CILB_TEXTCOLOR

inline LRESULT CImageListBoxBase::OnDrawItem(HWND hWnd, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
    LPDRAWITEMSTRUCT pDrawItem = reinterpret_cast<LPDRAWITEMSTRUCT>(lParam);
    ATLASSERT(pDrawItem);
    ATLASSERT(pDrawItem->hDC);
    ATLASSERT(::IsWindow(hWnd));
    ATLASSERT(pDrawItem->CtlType == ODT_LISTBOX);

    if (pDrawItem->itemID == (UINT)-1)
        return 0L;

#ifdef _ATL_SUPPORT_CILB_TEXTCOLOR
    // Normal state
    UINT nStyle = ILD_NORMAL;
    COLORREF clrBack = m_clrBack;
    COLORREF clrText = m_clrText;
    BOOL bSelected = ((pDrawItem->itemAction | ODA_SELECT) && (pDrawItem->itemState & ODS_SELECTED));

    // Selected state
    if (bSelected)
    {
        nStyle  = ILD_SELECTED;
        clrBack = ::GetSysColor(COLOR_HIGHLIGHT);
        clrText = ::GetSysColor(COLOR_HIGHLIGHTTEXT);
    }

    // Disabled state
    if (pDrawItem->itemState & ODS_DISABLED)
    {
        clrText = ::GetSysColor(COLOR_GRAYTEXT);
        if (!bSelected)
            clrBack = ::GetSysColor(COLOR_WINDOW);
    }

    // Get image size
    int nImageWidth = 0, nImageHeight = 0;
    ::ImageList_GetIconSize(m_hImageList, &nImageWidth, &nImageHeight);

    // Draw background and image
    ::ImageList_Draw(m_hImageList, m_arrImages[pDrawItem->itemID], pDrawItem->hDC, pDrawItem->rcItem.left, pDrawItem->rcItem.top, nStyle);
    pDrawItem->rcItem.left += nImageWidth;

    HBRUSH hBrush = ::CreateSolidBrush(clrBack);
    ATLASSERT(hBrush);
    ATLVERIFY(::FillRect(pDrawItem->hDC, &pDrawItem->rcItem, hBrush));
    ATLVERIFY(::DeleteObject(hBrush));

    // Draw focus
    if (pDrawItem->itemState & ODS_FOCUS)
        ATLVERIFY(::DrawFocusRect(pDrawItem->hDC, &pDrawItem->rcItem));

    // Draw text
    CString strText;
    int nLength = (int)::SendMessage(hWnd, LB_GETTEXTLEN, pDrawItem->itemID, 0L);
    ::SendMessage(hWnd, LB_GETTEXT, pDrawItem->itemID, (LPARAM)strText.GetBuffer(nLength));
    strText.ReleaseBuffer(nLength);

    pDrawItem->rcItem.left += 3;
    ::SetTextColor(pDrawItem->hDC, clrText);
    ::SetBkMode(pDrawItem->hDC, TRANSPARENT);
    ::DrawText(pDrawItem->hDC, strText, nLength, &pDrawItem->rcItem, DT_VCENTER | DT_SINGLELINE | DT_WORDBREAK);
#else
    // Normal state
    UINT nStyle    = ILD_NORMAL;
    int nBackColor = COLOR_WINDOW;
    int nTextColor = COLOR_WINDOWTEXT;

    // Selected state
    if ((pDrawItem->itemAction | ODA_SELECT) && (pDrawItem->itemState & ODS_SELECTED))
    {
        nStyle     = ILD_SELECTED;
        nBackColor = COLOR_HIGHLIGHT;
        nTextColor = COLOR_HIGHLIGHTTEXT;
    }

    // Disabled state
    if (pDrawItem->itemState & ODS_DISABLED)
        nTextColor = COLOR_GRAYTEXT;

    // Get image size
    int nImageWidth = 0, nImageHeight = 0;
    ::ImageList_GetIconSize(m_hImageList, &nImageWidth, &nImageHeight);

    // Draw background and image
    ::ImageList_Draw(m_hImageList, m_arrImages[pDrawItem->itemID], pDrawItem->hDC, pDrawItem->rcItem.left, pDrawItem->rcItem.top, nStyle);
    pDrawItem->rcItem.left += nImageWidth;
    ATLVERIFY(::FillRect(pDrawItem->hDC, &pDrawItem->rcItem, ::GetSysColorBrush(nBackColor)));

    // Draw focus
    if (pDrawItem->itemState & ODS_FOCUS)
        ATLVERIFY(::DrawFocusRect(pDrawItem->hDC, &pDrawItem->rcItem));

    // Draw text
    CString strText;
    int nLength = (int)::SendMessage(hWnd, LB_GETTEXTLEN, pDrawItem->itemID, 0L);
    ::SendMessage(hWnd, LB_GETTEXT, pDrawItem->itemID, (LPARAM)strText.GetBuffer(nLength));
    strText.ReleaseBuffer(nLength);

    pDrawItem->rcItem.left += 3;
    ::SetBkMode(pDrawItem->hDC, TRANSPARENT);
    ::SetTextColor(pDrawItem->hDC, ::GetSysColor(nTextColor));
    ::DrawText(pDrawItem->hDC, strText, nLength, &pDrawItem->rcItem, DT_VCENTER | DT_SINGLELINE | DT_WORDBREAK);
#endif  // _ATL_SUPPORT_CILB_TEXTCOLOR

    return 1L;
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CColorComboBoxBase class
//

inline CColorComboBoxBase::CColorComboBoxBase()
    : m_nCurSel((UINT)-1)
{
}

inline const COLORMAPENTRY* WINAPI CColorComboBoxBase::GetColorMap()
{
    BEGIN_DEFINE_COLORMAP(_colorEntries)
        COLOR_ENTRY(_T("��ɫ"),   RGB(0x00, 0x00, 0x00))
        COLOR_ENTRY(_T("���ɫ"), RGB(0x80, 0x00, 0x00))
        COLOR_ENTRY(_T("��ɫ"),   RGB(0x00, 0x80, 0x00))
        COLOR_ENTRY(_T("���ɫ"), RGB(0x80, 0x80, 0x00))
        COLOR_ENTRY(_T("����ɫ"), RGB(0x00, 0x00, 0x80))
        COLOR_ENTRY(_T("��ɫ"),   RGB(0x80, 0x00, 0x80))
        COLOR_ENTRY(_T("����"),   RGB(0x00, 0x80, 0x80))
        COLOR_ENTRY(_T("��ɫ"),   RGB(0x80, 0x80, 0x80))
        COLOR_ENTRY(_T("��ɫ"),   RGB(0xC0, 0xC0, 0xC0))
        COLOR_ENTRY(_T("��ɫ"),   RGB(0xFF, 0x00, 0x00))
        COLOR_ENTRY(_T("ǳ��ɫ"), RGB(0x00, 0xFF, 0x00))
        COLOR_ENTRY(_T("��ɫ"),   RGB(0xFF, 0xFF, 0x00))
        COLOR_ENTRY(_T("��ɫ"),   RGB(0x00, 0x00, 0xFF))
        COLOR_ENTRY(_T("�Ϻ�ɫ"), RGB(0xFF, 0x00, 0xFF))
        COLOR_ENTRY(_T("ǳ��ɫ"), RGB(0x00, 0xFF, 0xFF))
        COLOR_ENTRY(_T("��ɫ"),   RGB(0xFF, 0xFF, 0xFF))
    END_DEFINE_COLORMAP(_T("�Զ���"))

    return _colorEntries;
}

inline BOOL WINAPI CColorComboBoxBase::GetVkScanUp()
{
    return ((::GetKeyState(VK_UP) & 0xFF00) || (::GetKeyState(VK_LEFT) & 0xFF00) || (::GetKeyState(VK_PRIOR) & 0xFF00) || (::GetKeyState(VK_HOME) & 0xFF00));
}

inline BOOL WINAPI CColorComboBoxBase::GetVkScanDown()
{
    return ((::GetKeyState(VK_DOWN) & 0xFF00) || (::GetKeyState(VK_RIGHT) & 0xFF00) || (::GetKeyState(VK_NEXT) & 0xFF00) || (::GetKeyState(VK_END) & 0xFF00));
}

inline int WINAPI CColorComboBoxBase::GetCustomItem(HWND hWnd)
{
    ATLASSERT(::IsWindow(hWnd));
    return (int)::SendMessage(hWnd, CB_GETCOUNT, 0, 0L) - 1;
}

inline int WINAPI CColorComboBoxBase::GetSeparatorItem(HWND hWnd)
{
    ATLASSERT(::IsWindow(hWnd));
    return (int)::SendMessage(hWnd, CB_GETCOUNT, 0, 0L) - 2;
}

inline void WINAPI CColorComboBoxBase::SetSelectedColor(HWND hWnd, COLORREF color)
{
    ATLASSERT(::IsWindow(hWnd));

    // Find the color
    int nIndex = 0, nCount = (int)::SendMessage(hWnd, CB_GETCOUNT, 0, 0L);
    for (; nIndex < nCount; ++nIndex)
    {
        if ((COLORREF)::SendMessage(hWnd, CB_GETITEMDATA, nIndex, 0L) == color)
            break;
    }

    // Set custom color, if not found
    if (nIndex == nCount)
        ::SendMessage(hWnd, CB_SETITEMDATA, --nIndex, (LPARAM)color);

    ::SendMessage(hWnd, CB_SETCURSEL, nIndex, 0L);
}

inline void WINAPI CColorComboBoxBase::SetColorMap(HWND hWnd, const COLORMAPENTRY* pColorMap, int nItemHeight)
{
    ATLASSERT(pColorMap);
    ATLASSERT(::IsWindow(hWnd));
    ATLASSERT(::SendMessage(hWnd, CB_GETCOUNT, 0, 0L) == 0);
    ATLASSERT(!(::GetWindowLong(hWnd, GWL_STYLE) & CBS_SORT));
    ATLASSERT(::GetWindowLong(hWnd, GWL_STYLE) & CBS_HASSTRINGS);
    ATLASSERT(::GetWindowLong(hWnd, GWL_STYLE) & CBS_OWNERDRAWFIXED);

    for (LRESULT i = 0; pColorMap->name != NULL; ++pColorMap)
    {
        i = ::SendMessage(hWnd, CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)CString(pColorMap->name));
        ::SendMessage(hWnd, CB_SETITEMDATA, i, (LPARAM)pColorMap->color);
    }

    ::SendMessage(hWnd, CB_SETCURSEL, 0, 0L);
    ::SendMessage(hWnd, CB_SETITEMHEIGHT, 0, nItemHeight);
    ::SendMessage(hWnd, CB_SETITEMHEIGHT, (WPARAM)-1, nItemHeight);
}

inline LRESULT CColorComboBoxBase::OnSelCustom(int /*nIDCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/)
{
    LPNMCBCUSTOMITEM pNMCustomItem = reinterpret_cast<LPNMCBCUSTOMITEM>(pNMHDR);
    ATLASSERT(pNMCustomItem);
    ATLASSERT(::IsWindow(pNMHDR->hwndFrom));

    static COLORREF clrCustom[] =
    {
        0x00FFFFFF, 0x00FFFFFF, 0x00FFFFFF, 0x00FFFFFF, 0x00FFFFFF, 0x00FFFFFF, 0x00FFFFFF, 0x00FFFFFF,
        0x00FFFFFF, 0x00FFFFFF, 0x00FFFFFF, 0x00FFFFFF, 0x00FFFFFF, 0x00FFFFFF, 0x00FFFFFF, 0x00FFFFFF
    };

    CHOOSECOLOR chColor = { sizeof(CHOOSECOLOR), pNMHDR->hwndFrom, NULL, pNMCustomItem->color, clrCustom, CC_RGBINIT | CC_FULLOPEN };
    if (::ChooseColor(&chColor))
    {
        ::SendMessage(pNMHDR->hwndFrom, CB_SETITEMDATA, pNMCustomItem->nItem, (LPARAM)chColor.rgbResult);
        ATLVERIFY(::InvalidateRect(pNMHDR->hwndFrom, NULL, FALSE));
        ATLVERIFY(::UpdateWindow(pNMHDR->hwndFrom));
    }

    return 0L;
}

inline LRESULT CColorComboBoxBase::OnDrawItem(HWND hWnd, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
    LPDRAWITEMSTRUCT pDrawItem = reinterpret_cast<LPDRAWITEMSTRUCT>(lParam);
    ATLASSERT(pDrawItem);
    ATLASSERT(pDrawItem->hDC);
    ATLASSERT(::IsWindow(hWnd));
    ATLASSERT(pDrawItem->CtlType == ODT_COMBOBOX);

    if (pDrawItem->itemID == (UINT)-1)
        return 0L;

    CRect rcPaint = pDrawItem->rcItem;
    if (pDrawItem->itemID != (UINT)GetSeparatorItem(hWnd))
    {
        // Normal state
        int nBackColor = COLOR_WINDOW;
        int nTextColor = COLOR_WINDOWTEXT;
        int nBorderColor = COLOR_WINDOWTEXT;

        // Disabled state
        if (pDrawItem->itemState & ODS_DISABLED)
        {
            nBackColor = COLOR_3DFACE;
            nTextColor = COLOR_GRAYTEXT;
            nBorderColor = COLOR_GRAYTEXT;
        }
        // Selected state
        else if ((pDrawItem->itemAction | ODA_SELECT) && (pDrawItem->itemState & ODS_SELECTED))
        {
            nBackColor = COLOR_HIGHLIGHT;
            nTextColor = COLOR_HIGHLIGHTTEXT;
        }

        // Draw background and color rectangle
        HBRUSH hBrush = ::CreateSolidBrush(pDrawItem->itemData);
        ATLASSERT(hBrush);
        rcPaint.DeflateRect(1, 1, rcPaint.Width() - 21, 1);

        ATLVERIFY(::FillRect(pDrawItem->hDC, &pDrawItem->rcItem, ::GetSysColorBrush(nBackColor)));
        ATLVERIFY(::FillRect(pDrawItem->hDC, rcPaint, hBrush));
        ATLVERIFY(::FrameRect(pDrawItem->hDC, rcPaint, ::GetSysColorBrush(nBorderColor)));
        ATLVERIFY(::DeleteObject(hBrush));

        // Draw text
        CString strText;
        int nLength = (int)::SendMessage(hWnd, CB_GETLBTEXTLEN, pDrawItem->itemID, 0L);
        ::SendMessage(hWnd, CB_GETLBTEXT, pDrawItem->itemID, (LPARAM)strText.GetBuffer(nLength));
        strText.ReleaseBuffer(nLength);

        ::SetBkMode(pDrawItem->hDC, TRANSPARENT);
        ::SetTextColor(pDrawItem->hDC, ::GetSysColor(nTextColor));
        stdutil::SetRect(rcPaint, pDrawItem->rcItem.left + rcPaint.Width() + 3, pDrawItem->rcItem.top, pDrawItem->rcItem.right, pDrawItem->rcItem.bottom);
        ::DrawText(pDrawItem->hDC, strText, nLength, rcPaint, DT_VCENTER | DT_SINGLELINE | DT_WORDBREAK);
    }
    else
    {
        // Draw separator
        rcPaint.DeflateRect(1, 1, 1, 1);
        ATLVERIFY(::MoveToEx(pDrawItem->hDC, rcPaint.left, rcPaint.top + rcPaint.Height() / 2, NULL));
        ATLVERIFY(::LineTo(pDrawItem->hDC, rcPaint.right, rcPaint.top + rcPaint.Height() / 2));
    }

    return 1L;
}

inline LRESULT CColorComboBoxBase::OnCloseUp(WORD /*wNotifyCode*/, WORD /*wID*/, HWND hWndCtrl, BOOL& /*bHandled*/)
{
    return ::SendMessage(hWndCtrl, CB_SETCURSEL, ::SendMessage(hWndCtrl, CB_GETCURSEL, 0, 0L), 0L);
}

inline LRESULT CColorComboBoxBase::OnSelEndOK(WORD /*wNotifyCode*/, WORD wID, HWND hWndCtrl, BOOL& /*bHandled*/)
{
    ATLASSERT(::IsWindow(hWndCtrl));

    if (!GetVkScanDown() && !GetVkScanUp())
    {
        int nCurSel = (int)::SendMessage(hWndCtrl, CB_GETCURSEL, 0, 0L);
        if (nCurSel == GetCustomItem(hWndCtrl))
        {
            NMCBCUSTOMITEM customItem = { { hWndCtrl, wID, CBN_SELCUSTOM }, nCurSel, ::SendMessage(hWndCtrl, CB_GETITEMDATA, nCurSel, 0L) };
            ::SendMessage(::GetParent(hWndCtrl), WM_NOTIFY, wID, (LPARAM)&customItem);
        }

        ::SendMessage(hWndCtrl, CB_SETCURSEL, nCurSel, 0L);
    }

    return 0L;
}

inline LRESULT CColorComboBoxBase::OnSelChange(WORD /*wNotifyCode*/, WORD /*wID*/, HWND hWndCtrl, BOOL& /*bHandled*/)
{
    ATLASSERT(::IsWindow(hWndCtrl));

    BOOL bVkKeyDown = GetVkScanDown();
    if (bVkKeyDown || GetVkScanUp())
    {
        int nCurSel = (int)::SendMessage(hWndCtrl, CB_GETCURSEL, 0, 0L);
        if (nCurSel == GetSeparatorItem(hWndCtrl))
            ::SendMessage(hWndCtrl, CB_SETCURSEL, (bVkKeyDown ? nCurSel + 1 : nCurSel - 1), 0L);
    }

    return 0L;
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CStatusBarCtrlBase class
//

inline CStatusBarCtrlBase::CStatusBarCtrlBase(DWORD dwExStyle)
    : m_dwExStyle(dwExStyle), m_nPaneCount(0), m_pPaneWidth(NULL)
{
}

inline CStatusBarCtrlBase::~CStatusBarCtrlBase()
{
    ::free(m_pPaneWidth);
}

inline void CStatusBarCtrlBase::CalcPaneWidth(HWND hWnd)
{
    ATLASSERT(::IsWindow(hWnd));
    ATLASSERT(m_nPaneCount > 0);
    ATLASSERT(m_pPaneWidth != NULL);

    ATL::CTempBuffer<int> arrPaneWidth;
    if (int* pPaneWidth = arrPaneWidth.Allocate(m_nPaneCount))
    {
        int i = 0, nWidth = 0;
        sysutil::CClientRect rcClient(hWnd);

        for (; i < m_nPaneCount; ++i)
        {
            pPaneWidth[i] = (m_pPaneWidth[i] == -1 ? INT_MAX / 2 : nWidth + m_pPaneWidth[i]);
            nWidth = pPaneWidth[i];
        }

        nWidth = rcClient.right - pPaneWidth[m_nPaneCount - 1];
        for (i = 0; i < m_nPaneCount; ++i)
        {
            if (m_pPaneWidth[i] == -1)
                break;
        }

        for (; i < m_nPaneCount; ++i)
            pPaneWidth[i] += nWidth;

        ATLVERIFY(::SendMessage(hWnd, SB_SETPARTS, m_nPaneCount, (LPARAM)pPaneWidth));
    }
}

inline void CStatusBarCtrlBase::OnDrawPane(const SBDRAWPANE& /*drawPane*/)
{
    ATLASSERT(FALSE && _T("MUST BE IMPLEMENTED IN A DERIVED CLASS."));
}

inline void CStatusBarCtrlBase::GetPaneRect(HWND hWnd, int nIndex, BOOL bSimple, RECT& rcPane) const
{
    ATLASSERT(::IsWindow(hWnd));

    if (bSimple)
        ATLVERIFY(::GetClientRect(hWnd, &rcPane));
    else
        ATLVERIFY(::SendMessage(hWnd, SB_GETRECT, nIndex, (LPARAM)&rcPane));
}

inline BOOL CStatusBarCtrlBase::SetPanes(HWND hWnd, const SBPANE* pPanes, int nCount)
{
    ATLASSERT(pPanes);
    ATLASSERT(nCount > 0);
    ATLASSERT(::IsWindow(hWnd));
    ATLASSERT(m_nPaneCount == 0);
    ATLASSERT(m_pPaneWidth == NULL);

    BOOL bSuccessful = FALSE;
    if ((m_pPaneWidth = (int*)::malloc(nCount * sizeof(int))) != NULL)
    {
        m_nPaneCount = nCount;
        for (int i = 0; i < nCount; ++i)
            m_pPaneWidth[i] = pPanes[i].nWidth;

        bSuccessful = ::SendMessage(hWnd, SB_SETPARTS, nCount, (LPARAM)m_pPaneWidth);
        if (bSuccessful)
        {
            for (int i = 0; i < nCount; ++i)
                ATLVERIFY(::SendMessage(hWnd, SB_SETTEXT, i, (LPARAM)(LPCTSTR)CString(pPanes[i].pszText)));
        }
    }

    return bSuccessful;
}

inline int CStatusBarCtrlBase::GetPaneWidth(int nIndex) const
{
    ATLASSERT(m_pPaneWidth);
    ATLASSERT(nIndex >= 0 && nIndex < m_nPaneCount);

    return m_pPaneWidth[nIndex];
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CPaintChildBkgndBase class
//

inline CPaintChildBkgndBase::CPaintChildBkgndBase()
    : m_hbmBkgnd(NULL)
{
}

inline CPaintChildBkgndBase::~CPaintChildBkgndBase()
{
    ATLVERIFY(::DeleteObject(m_hbmBkgnd));
}

inline void CPaintChildBkgndBase::PaintBkgnd_CreateBitmap(HWND hWnd, int cx, int cy)
{
    HDC hDC = ::GetDC(hWnd);
    ATLASSERT(hDC);
    ATLASSERT(::IsWindow(hWnd));

    if (m_hbmBkgnd != NULL)
        ATLVERIFY(::DeleteObject(m_hbmBkgnd));

    if ((m_hbmBkgnd = ::CreateCompatibleBitmap(hDC, cx, cy)) != NULL)
    {
        atlutil::CBitmapDC dc(m_hbmBkgnd);
        ::SendMessage(hWnd, WM_ERASEBKGND, (WPARAM)dc.m_hDC, 0L);
    }

    ATLVERIFY(::ReleaseDC(hWnd, hDC));
}

inline LRESULT CPaintChildBkgndBase::OnNMPaintBkgnd(int /*nIDCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/)
{
    LPNMPAINTSTRUCT pNMPaint = reinterpret_cast<LPNMPAINTSTRUCT>(pNMHDR);
    ATLASSERT(pNMPaint);
    ATLASSERT(m_hbmBkgnd);
    ATLASSERT(pNMPaint->hDC);
    ATLASSERT(::IsWindow(pNMPaint->hdr.hwndFrom));

    return stdutil::DrawBitmap(pNMPaint->hDC, pNMPaint->rect, m_hbmBkgnd, pNMPaint->point.x, pNMPaint->point.y);
}

inline LRESULT CPaintChildBkgndBase::OnSize(HWND hWnd, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    ATLASSERT(::IsWindow(hWnd));

    if (wParam != SIZE_MINIMIZED)
        PaintBkgnd_CreateBitmap(hWnd, LOWORD(lParam), HIWORD(lParam));

    return bHandled = FALSE;
}

}  // namespace atlctrls

#endif  // __STDCTRLS_INL__