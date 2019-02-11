///////////////////////////////////////////////////////////////////////////////
// atlcectrls.inl
//
// Copyright(c) 2019, Garfield. All rights reserved.
// Author  : Garfield
// Version : 1.0
// Creation Date : 2009/12/14

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __ATLCECTRLS_INL__
#define __ATLCECTRLS_INL__

#ifndef __ATLCECTRLS_H__
    #error atlcectrls.inl requires atlcectrls.h to be included first
#endif

namespace WCE {

#ifdef __ATLCTRLS_H__
///////////////////////////////////////////////////////////////////////////////
// Type definition
//

typedef CSHToolBarCtrlT<WTL::CToolBarCtrl> CSHToolBarCtrl;

#endif  // __ATLCTRLS_H__


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CWaitCursor class
//

inline CWaitCursor::CWaitCursor(HCURSOR hCursor/* = ::LoadCursor(NULL, IDC_WAIT)*/)
    : m_hCursor(::SetCursor(hCursor))
{
}

inline CWaitCursor::~CWaitCursor()
{
    ::SetCursor(m_hCursor);
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CFullScreenBase class
//

inline CFullScreenBase::CFullScreenBase()
    : m_bFullScreen(FALSE)
{
}

inline void CFullScreenBase::DoFullScreen(HWND hWnd, HWND hWndCommandBar)
{
    ATLASSERT(::IsWindow(hWnd));
    ATLASSERT(::IsWindow(hWndCommandBar));

    ShowTaskBar(hWnd, !m_bFullScreen, FALSE);
    ShowMenuBar(hWnd, hWndCommandBar, !m_bFullScreen);
}

inline void CFullScreenBase::ShowTaskBar(HWND hWnd, BOOL bShow, BOOL bRepaint)
{
    ATLASSERT(::IsWindow(hWnd));

    CRect rcWindow;
    ATLVERIFY(::SystemParametersInfo(SPI_GETWORKAREA, 0, rcWindow, FALSE));
    if (!bShow) rcWindow.top = 0;

    ::SHFullScreen(hWnd, (bShow ? SHFS_SHOWTASKBAR : SHFS_HIDETASKBAR));
    ATLVERIFY(::MoveWindow(hWnd, rcWindow.left, rcWindow.top, rcWindow.Width(), rcWindow.Height(), bRepaint));
}

inline void CFullScreenBase::ShowMenuBar(HWND hWnd, HWND hWndCommandBar, BOOL bShow)
{
    ATLASSERT(::IsWindow(hWnd));
    ATLASSERT(::IsWindow(hWndCommandBar));

    sysutil::CWindowRect rcWindow(hWnd);
    sysutil::CWindowRect rcMenuBar(hWndCommandBar);

    int nCmdShow = SW_HIDE, nState = SHFS_HIDESIPBUTTON, nHeight = rcMenuBar.bottom - rcWindow.top;
    if (bShow)
        nCmdShow = SW_SHOWNORMAL, nState = SHFS_SHOWSIPBUTTON, nHeight = rcMenuBar.top - rcWindow.top;

    ::SHFullScreen(hWnd, nState);
    ::ShowWindow(hWndCommandBar, nCmdShow);
    ATLVERIFY(::SetWindowPos(hWnd, NULL, 0, 0, rcWindow.Width(), nHeight, SWP_NOMOVE | SWP_NOZORDER));
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CFullScreenImpl class
//

template <typename T>
inline CFullScreenImpl<T>::CFullScreenImpl()
{
}

template <typename T>
inline void CFullScreenImpl<T>::SetFullScreen(BOOL bFullScreen)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);
    ATLASSERT(::IsWindow(pThis->m_hWnd));

    m_bFullScreen = bFullScreen;
    pThis->DoFullScreen(pThis->m_hWnd, pThis->m_hWndCECommandBar);
}

template <typename T>
inline LRESULT CFullScreenImpl<T>::OnActivate(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
{
    if (m_bFullScreen && wParam != WA_INACTIVE)
    {
        T* pThis = static_cast<T*>(this);
        ATLASSERT(pThis);
        ATLASSERT(::IsWindow(pThis->m_hWnd));

        pThis->DoFullScreen(pThis->m_hWnd, pThis->m_hWndCECommandBar);
    }

    return bHandled = FALSE;
}

template <typename T>
inline LRESULT CFullScreenImpl<T>::OnSettingChange(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
{
    if (m_bFullScreen && wParam != SETTINGCHANGE_RESET)
    {
        T* pThis = static_cast<T*>(this);
        ATLASSERT(pThis);
        ATLASSERT(::IsWindow(pThis->m_hWnd));

        pThis->DoFullScreen(pThis->m_hWnd, pThis->m_hWndCECommandBar);
    }

    return bHandled = FALSE;
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CScrollWindowBase class
//

inline CScrollWindowBase::CScrollWindowBase()
    : m_nScrollX(0), m_nScrollY(0), m_sizeLine(0, 0), m_sizeTotal(0, 0)
{
}

inline void WINAPI CScrollWindowBase::OnScroll(HWND hWnd, int nBar, UINT nSBCode, int nLine)
{
    ATLASSERT(::IsWindow(hWnd));
    ATLASSERT(nBar == SB_VERT || nBar == SB_HORZ);

    SCROLLINFO si = { sizeof(SCROLLINFO), SIF_ALL };
    ATLVERIFY(::GetScrollInfo(hWnd, nBar, &si));

    int& nScrollPos = (nBar == SB_HORZ ? m_nScrollX : m_nScrollY);
    switch (nSBCode)
    {
    case SB_LINEUP:
        nScrollPos -= nLine;
        break;

    case SB_LINEDOWN:
        nScrollPos += nLine;
        break;

    case SB_PAGEUP:
        nScrollPos -= si.nPage;
        break;

    case SB_PAGEDOWN:
        nScrollPos += si.nPage;
        break;

    case SB_THUMBTRACK:
    case SB_THUMBPOSITION:
        nScrollPos = si.nTrackPos;
        break;
    }

    // Adjust scroll position in the range.
    nScrollPos = stdutil::AdjustRange(nScrollPos, 0, si.nMax - (int)si.nPage + 1);

    // Compare two position for in-equality (nScrollPos is new position, si.nPos is old position)
    if (nScrollPos != si.nPos)
    {
        int nDelta = si.nPos - nScrollPos;
        ::SetScrollPos(hWnd, nBar, nScrollPos, TRUE);
        ::ScrollWindowEx(hWnd, (nBar == SB_HORZ ? nDelta : 0), (nBar == SB_HORZ ? 0 : nDelta), NULL, NULL, NULL, NULL, SW_INVALIDATE | SW_SCROLLCHILDREN | SW_ERASE);
    }
}

inline void CScrollWindowBase::UpdateScrollInfo(HWND hWnd, int cx, int cy)
{
    ATLASSERT(::IsWindow(hWnd));

    SCROLLINFO si = { sizeof(SCROLLINFO), SIF_ALL | SIF_DISABLENOSCROLL, 0, m_sizeTotal.cx - 1, cx, m_nScrollX };
    ::SetScrollInfo(hWnd, SB_HORZ, &si, TRUE);

    m_nScrollY = stdutil::AdjustRange(m_nScrollY, 0, m_sizeTotal.cy - cy);
    si.nMax = m_sizeTotal.cy - 1, si.nPage = cy, si.nPos = m_nScrollY;
    ::SetScrollInfo(hWnd, SB_VERT, &si, TRUE);

    ATLVERIFY(::InvalidateRect(hWnd, NULL, TRUE));
    ATLVERIFY(::UpdateWindow(hWnd));
}

inline void CScrollWindowBase::SetScrollSizes(HWND hWnd, int cxTotal, int cyTotal, int cxLine, int cyLine)
{
    ATLASSERT(::IsWindow(hWnd));
    ATLASSERT(cxTotal >= 0 && cyTotal >= 0);

#ifdef _DEBUG
    if (cxTotal == 0)
        ATLASSERT(cyTotal > 0 && cyLine > 0);
    else if (cyTotal == 0)
        ATLASSERT(cxTotal > 0 && cxLine > 0);
    else
        ATLASSERT(cxLine > 0 && cyLine > 0);
#endif  // _DEBUG

    sysutil::CClientRect rcClient(hWnd);
    if (cxTotal == 0)
        cxTotal = rcClient.right - GetSystemMetrics(SM_CXVSCROLL);
    else if (cyTotal == 0)
        cyTotal = rcClient.bottom - GetSystemMetrics(SM_CYHSCROLL);

    m_sizeLine.SetSize(cxLine, cyLine);
    m_sizeTotal.SetSize(cxTotal, cyTotal);
    UpdateScrollInfo(hWnd, rcClient.right, rcClient.bottom);
}

inline LRESULT CScrollWindowBase::OnSize(HWND hWnd, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    ATLASSERT(::IsWindow(hWnd));

    if (wParam != SIZE_MINIMIZED)
        UpdateScrollInfo(hWnd, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));

    return bHandled = FALSE;
}

inline LRESULT CScrollWindowBase::OnHScroll(HWND hWnd, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    ATLASSERT(::IsWindow(hWnd));

    OnScroll(hWnd, SB_HORZ, LOWORD(wParam), m_sizeLine.cx);
    return 0L;
}

inline LRESULT CScrollWindowBase::OnVScroll(HWND hWnd, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    ATLASSERT(::IsWindow(hWnd));

    OnScroll(hWnd, SB_VERT, LOWORD(wParam), m_sizeLine.cy);
    return 0L;
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CScrollWindowImpl class
//

template <typename T>
inline CScrollWindowImpl<T>::CScrollWindowImpl()
{
}

template <typename T>
inline void CScrollWindowImpl<T>::ScrollWindow_SetScrollSizes(int cxTotal, int cyTotal, int cxLine, int cyLine)
{
    CScrollWindowBase::SetScrollSizes(static_cast<T*>(this)->m_hWnd, cxTotal, cyTotal, cxLine, cyLine);
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CImageButtonBase class
//

inline CImageButtonBase::CImageButtonBase()
{
    ::memset(m_nImages, 0, sizeof(m_nImages));
}

inline void CImageButtonBase::OnDrawBkgnd(LPDRAWITEMSTRUCT pDrawItem)
{
    LPDRAWITEMSTRUCT pDrawItem = reinterpret_cast<LPDRAWITEMSTRUCT>(lParam);
    ATLASSERT(pDrawItem);
    ATLASSERT(pDrawItem->hDC);
    ATLASSERT(::IsWindow(pDrawItem->hwndItem));
    ATLASSERT(pDrawItem->CtlType == ODT_BUTTON);

    if (!stdutil::DrawBkgnd(pDrawItem->hwndItem, pDrawItem->hDC, pDrawItem->rcItem))
    {
        if (HBRUSH hBrush = (HBRUSH)::SendMessage(::GetParent(pDrawItem->hwndItem), WM_CTLCOLORBTN, (WPARAM)pDrawItem->hDC, (LPARAM)pDrawItem->hwndItem))
            ATLVERIFY(::FillRect(pDrawItem->hDC, &pDrawItem->rcItem, hBrush));
    }
}

inline void CImageButtonBase::SetImages(int nNormal, int nSelected/* = 0*/, int nFocused/* = 0*/, int nDisabled/* = 0*/)
{
    m_nImages[IMAGE_NORMAL]   = nNormal;
    m_nImages[IMAGE_FOCUSED]  = nFocused;
    m_nImages[IMAGE_SELECTED] = nSelected;
    m_nImages[IMAGE_DISABLED] = nDisabled;
}

inline int CImageButtonBase::GetImageIndex(UINT nState) const
{
    int nImage = m_nImages[IMAGE_NORMAL];
    if (nState & ODS_SELECTED)
        nImage = m_nImages[IMAGE_SELECTED];
    else if (nState & ODS_FOCUS)
        nImage = m_nImages[IMAGE_FOCUSED];
    else if (nState & ODS_DISABLED)
        nImage = m_nImages[IMAGE_DISABLED];

    return nImage;
}

inline LRESULT CImageButtonBase::OnEraseBkgnd(HWND /*hWnd*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    return 1L;
}


#ifdef __CEIMAGEUTIL_H__
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
    return SetWindowPos(NULL, 0, 0, m_pImageList->GetImageWidth(), m_pImageList->GetImageHeight(), SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
}

template <typename T, typename TBase, typename TWinTraits>
inline LRESULT CImageButtonImpl<T, TBase, TWinTraits>::OnDrawItem(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
    LPDRAWITEMSTRUCT pDrawItem = reinterpret_cast<LPDRAWITEMSTRUCT>(lParam);
    ATLASSERT(pDrawItem);
    ATLASSERT(IsWindow());
    ATLASSERT(m_pImageList);
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
#endif  // __CEIMAGEUTIL_H__


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

    int cx = 0, cy = 0;
    ATLVERIFY(::ImageList_GetIconSize(m_hImageList, &cx, &cy));

    return SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
}

template <typename T, typename TBase, typename TWinTraits>
inline LRESULT CBitmapButtonImpl<T, TBase, TWinTraits>::OnDrawItem(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
    LPDRAWITEMSTRUCT pDrawItem = reinterpret_cast<LPDRAWITEMSTRUCT>(lParam);
    ATLASSERT(pDrawItem);
    ATLASSERT(IsWindow());
    ATLASSERT(m_hImageList);
    ATLASSERT(pDrawItem->CtlType == ODT_BUTTON);

    static_cast<T*>(this)->OnDrawBkgnd(pDrawItem);
    return ::ImageList_Draw(m_hImageList, GetImageIndex(pDrawItem->itemState), pDrawItem->hDC, pDrawItem->rcItem.left, pDrawItem->rcItem.top, ILD_NORMAL);
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CSHToolBarCtrlT class
//

template <typename TBase>
inline CSHToolBarCtrlT<TBase>::CSHToolBarCtrlT(HWND hWnd/* = NULL*/)
    : TBase(hWnd)
{
}

template <typename TBase>
inline void CSHToolBarCtrlT<TBase>::SHCalcButtonWidth(int cxClient)
{
    ATLASSERT(::IsWindow(m_hWnd));

    CRect rcLeft, rcRight;
    if (::SendMessage(m_hWnd, TB_GETITEMRECT, 0, (LPARAM)&rcLeft) && ::SendMessage(m_hWnd, TB_GETITEMRECT, 2, (LPARAM)&rcRight))
    {
        TBBUTTONINFO tbbi;
        tbbi.cbSize = sizeof(TBBUTTONINFO), tbbi.dwMask = TBIF_SIZE;
        tbbi.cx = (WORD)(cxClient - rcLeft.Width() - rcRight.Width());

        const WORD cxMin = 3, cxMax = (WORD)(cxClient - cxMin - rcRight.Width());
        if (rcLeft.Width() > cxMax)
        {
            TBBUTTON tbb;
            ATLVERIFY(::SendMessage(m_hWnd, TB_GETBUTTON, 0, (LPARAM)&tbb));

            tbbi.cx = cxMax;
            ATLVERIFY(::SendMessage(m_hWnd, TB_SETBUTTONINFO, tbb.idCommand, (LPARAM)&tbbi));
            tbbi.cx = cxMin;
        }

        ATLVERIFY(::SendMessage(m_hWnd, TB_SETBUTTONINFO, 0, (LPARAM)&tbbi));
    }
}

template <typename TBase>
inline BOOL CSHToolBarCtrlT<TBase>::SHAddButtons(LPCTBBUTTON pButtons)
{
    ATLASSERT(pButtons);
    ATLASSERT(::IsWindow(m_hWnd));

    TBBUTTON dummy = { -1, 0, TBSTATE_INDETERMINATE, TBSTYLE_BUTTON, 0, 0, 0, -1 };
    ::SendMessage(m_hWnd, TB_BUTTONSTRUCTSIZE, sizeof(TBBUTTON), 0L);

    return (::SendMessage(m_hWnd, TB_ADDBUTTONS, 1, (LPARAM)pButtons) && ::SendMessage(m_hWnd, TB_ADDBUTTONS, 1, (LPARAM)&dummy) && ::SendMessage(m_hWnd, TB_ADDBUTTONS, 1, (LPARAM)(pButtons + 1)));
}

template <typename TBase>
inline CSHToolBarCtrlT<TBase>& CSHToolBarCtrlT<TBase>::operator=(HWND hWnd)
{
    m_hWnd = hWnd;
    return *this;
}

template <typename TBase>
inline LRESULT CSHToolBarCtrlT<TBase>::OnCustomDraw(int /*nIDCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/)
{
    LPNMCUSTOMDRAW pNMCustomDraw = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
    ATLASSERT(pNMCustomDraw);
    ATLASSERT(::IsWindow(m_hWnd));

    // return CDRF_NOVERTBAR prevent default draw a vertical separator bar for TBSTYLE_DROPDOWN buttons
    return (pNMCustomDraw->dwDrawStage == CDDS_PREPAINT ? CDRF_NOTIFYITEMDRAW : (pNMCustomDraw->dwDrawStage == CDDS_ITEMPREPAINT ? CDRF_NOVERTBAR : CDRF_DODEFAULT));
}


#ifdef __CEIMAGEUTIL_H__
///////////////////////////////////////////////////////////////////////////////
// Implementation of the CAnimImageCtrlBase class
//

inline CAnimImageCtrlBase::CAnimImageCtrlBase()
    : m_bPlaying(FALSE), m_nFrameIndex(0)
{
}

inline HRESULT CAnimImageCtrlBase::LoadImage(IImagingFactory* pFactory, IStream* pStream, DecoderInitFlag flag/* = DecoderInitFlagBuiltIn1st*/)
{
    return m_AnimImage.Load(pFactory, pStream, flag);
}

inline HRESULT CAnimImageCtrlBase::LoadImage(IImagingFactory* pFactory, LPCWSTR pszFileName, DecoderInitFlag flag/* = DecoderInitFlagBuiltIn1st*/)
{
    return m_AnimImage.Load(pFactory, pszFileName, flag);
}

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

inline UINT CAnimImageCtrlBase::GetImageWidth() const
{
    return m_AnimImage.GetWidth();
}

inline UINT CAnimImageCtrlBase::GetImageHeight() const
{
    return m_AnimImage.GetHeight();
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

    return SetWindowPos(NULL, 0, 0, m_AnimImage.GetWidth(), m_AnimImage.GetHeight(), SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
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

    return bHandled = FALSE;
}
#endif  // __CEIMAGEUTIL_H__


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


#ifdef _TODAYCMN_H_
///////////////////////////////////////////////////////////////////////////////
// Implementation of the CTodayItemWindowImpl class
//

template <typename T>
inline CTodayItemWindowImpl<T>::CTodayItemWindowImpl()
    : m_bSelected(FALSE)
{
}

template <typename T>
inline void CTodayItemWindowImpl<T>::OnAction(UINT /*nChar*/)
{
}

template <typename T>
inline BOOL CTodayItemWindowImpl<T>::QueryRefreshCache(TODAYLISTITEM* ptlItem)
{
    ATLASSERT(ptlItem);
    return (ptlItem->cyp == 0 ? ptlItem->cyp = DRA::SCALEY(20) : FALSE);
}

template <typename T>
inline void CTodayItemWindowImpl<T>::TookSelection()
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);
    ATLASSERT(::IsWindow(pThis->m_hWnd));
    ATLASSERT(::IsWindow(::GetParent(pThis->m_hWnd)));

    ::PostMessage(::GetParent(pThis->m_hWnd), TODAYM_TOOKSELECTION, (WPARAM)pThis->m_hWnd, 0L);
}

template <typename T>
inline COLORREF CTodayItemWindowImpl<T>::GetTextColor() const
{
    const T* pThis = static_cast<const T*>(this);
    ATLASSERT(pThis);
    ATLASSERT(::IsWindow(pThis->m_hWnd));
    ATLASSERT(::IsWindow(::GetParent(pThis->m_hWnd)));

    return ::SendMessage(::GetParent(pThis->m_hWnd), TODAYM_GETCOLOR, (m_bSelected ? TODAYCOLOR_HIGHLIGHTEDTEXT : TODAYCOLOR_TEXT), 0L);
}

template <typename T>
inline COLORREF CTodayItemWindowImpl<T>::GetColor(UINT nFlags/* = TODAYCOLOR_TEXT*/) const
{
    const T* pThis = static_cast<const T*>(this);
    ATLASSERT(pThis);
    ATLASSERT(::IsWindow(pThis->m_hWnd));
    ATLASSERT(::IsWindow(::GetParent(pThis->m_hWnd)));

    return ::SendMessage(::GetParent(pThis->m_hWnd), TODAYM_GETCOLOR, nFlags, 0L);
}

template <typename T>
inline HWND CTodayItemWindowImpl<T>::InitCustomItem(TODAYLISTITEM* ptlItem, HWND hWndParent)
{
    HWND hWnd = NULL;
    if (ptlItem->fEnabled)
    {
        T* pThis = static_cast<T*>(this);
        ATLASSERT(pThis);
        ATLASSERT(!::IsWindow(pThis->m_hWnd));

        hWnd = pThis->Create(hWndParent);
        ::ShowWindow(pThis->m_hWnd, SW_SHOW);
    }

    return hWnd;
}

template <typename T>
inline LRESULT CTodayItemWindowImpl<T>::OnItemAction(UINT uMsg, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);
    ATLASSERT(::IsWindow(pThis->m_hWnd));

    pThis->OnAction(uMsg == WM_LBUTTONUP ? VK_LBUTTON : wParam);
    return 0L;
}

template <typename T>
inline LRESULT CTodayItemWindowImpl<T>::OnEraseBkgnd(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);
    ATLASSERT(wParam);
    ATLASSERT(::IsWindow(pThis->m_hWnd));
    ATLASSERT(::IsWindow(::GetParent(pThis->m_hWnd)));

    RECT rect;
    ATLVERIFY(::GetClientRect(pThis->m_hWnd, &rect));

    if (m_bSelected)
    {
        COLORREF color = ::SetBkColor((HDC)wParam, ::SendMessage(::GetParent(pThis->m_hWnd), TODAYM_GETCOLOR, TODAYCOLOR_HIGHLIGHT, 0L));
        ::ExtTextOut((HDC)wParam, 0, 0, ETO_OPAQUE, &rect, NULL, 0, NULL);
        ::SetBkColor((HDC)wParam, color);
    }
    else
    {
        TODAYDRAWWATERMARKINFO dwi = { (HDC)wParam, rect, pThis->m_hWnd };
        ::SendMessage(::GetParent(pThis->m_hWnd), TODAYM_DRAWWATERMARK, 0, (LPARAM)&dwi);
    }

    return 1L;
}

template <typename T>
inline LRESULT CTodayItemWindowImpl<T>::OnLButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);
    ATLASSERT(::IsWindow(pThis->m_hWnd));
    ATLASSERT(::IsWindow(::GetParent(pThis->m_hWnd)));

    ::PostMessage(::GetParent(pThis->m_hWnd), TODAYM_TOOKSELECTION, (WPARAM)pThis->m_hWnd, 0L);
    return 0L;
}

template <typename T>
inline LRESULT CTodayItemWindowImpl<T>::OnLostSelection(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    return m_bSelected = FALSE;
}

template <typename T>
inline LRESULT CTodayItemWindowImpl<T>::OnReceivedSelection(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    return (m_bSelected = (wParam == 0 || wParam == VK_UP || wParam == VK_DOWN));
}

template <typename T>
inline LRESULT CTodayItemWindowImpl<T>::OnQueryRefreshCache(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);
    ATLASSERT(::IsWindow(pThis->m_hWnd));

    return pThis->QueryRefreshCache((TODAYLISTITEM*)wParam);
}
#endif  // _TODAYCMN_H_


#ifdef __GESTUREUTIL_H__
///////////////////////////////////////////////////////////////////////////////
// Implementation of the CGesturePaintImpl class
//

template <typename T>
inline CGesturePaintImpl<T>::CGesturePaintImpl()
{
}

template <typename T>
inline void CGesturePaintImpl<T>::DoPaint(HDC /*hDC*/, const RECT& /*rcPaint*/)
{
}

template <typename T>
inline void CGesturePaintImpl<T>::DoPaintExtent(HDC hDC, const RECT& rcPaint)
{
    ATLASSERT(hDC);
    ::FillRect(hDC, &rcPaint, (HBRUSH)::GetStockObject(WHITE_BRUSH));
}

template <typename T>
inline void CGesturePaintImpl<T>::OnPaint(const SIZE& sizeBoundary, int nScrollX, int nScrollY)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);
    ATLASSERT(::IsWindow(pThis->m_hWnd));

    PAINTSTRUCT paint;
    HDC hDC = ::BeginPaint(pThis->m_hWnd, &paint);
    ATLASSERT(hDC);

    pThis->DoPaint(hDC, paint.rcPaint);
    sysutil::CClientRect rcClient(pThis->m_hWnd);

    int nExtent = sizeBoundary.cx - rcClient.right;
    if (nScrollX < 0)
        pThis->OnPaintExtent(hDC, paint.rcPaint, rcClient.left, rcClient.top, -nScrollX, rcClient.bottom);
    else if (nScrollX > nExtent)
        pThis->OnPaintExtent(hDC, paint.rcPaint, rcClient.right - (nScrollX - nExtent), rcClient.top, rcClient.right, rcClient.bottom);

    nExtent = sizeBoundary.cy - rcClient.bottom;
    if (nScrollY < 0)
        pThis->OnPaintExtent(hDC, paint.rcPaint, rcClient.left, rcClient.top, rcClient.right, -nScrollY);
    else if (nScrollY > nExtent)
        pThis->OnPaintExtent(hDC, paint.rcPaint, rcClient.left, rcClient.bottom - (nScrollY - nExtent), rcClient.right, rcClient.bottom);

    ATLVERIFY(::EndPaint(pThis->m_hWnd, &paint));
}

template <typename T>
inline void CGesturePaintImpl<T>::OnPaintExtent(HDC hDC, const RECT& rcPaint, int left, int top, int right, int bottom)
{
    ATLASSERT(hDC);

    RECT rect = { left, top, right, bottom };
    if (::IntersectRect(&rect, &rect, &rcPaint))
        static_cast<T*>(this)->DoPaintExtent(hDC, rect);
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CGestureWindowBase class
//

inline CGestureWindowBase::CGestureWindowBase()
    : m_nFlags(0), m_nScrollX(0), m_nScrollY(0), m_nHoriExtent(0), m_nVertExtent(0), m_sizeBoundary(0, 0)
{
    m_ptsLocation.x = m_ptsLocation.y = 0;
}

inline CGestureWindowBase::~CGestureWindowBase()
{
    ATLASSERT(IsAnimationStopped());
}

inline void CGestureWindowBase::GestureWindow_Init(int cxBoundary, int cyBoundary, UINT nFlags/* = GWIF_VSCROLLABLE | GWIF_HSCROLLABLE*/, BYTE nHorizontalExtent/* = 50*/, BYTE nVerticalExtent/* = 50*/)
{
    ATLASSERT(cxBoundary > 0 && cyBoundary > 0);
    ATLASSERT(nHorizontalExtent <= 100 && nVerticalExtent <= 100);

    m_nFlags = nFlags;
    m_nVertExtent = nVerticalExtent;
    m_nHoriExtent = nHorizontalExtent;
    m_sizeBoundary.SetSize(cxBoundary, cyBoundary);
}

inline BOOL CGestureWindowBase::OnGestureBegin(const GESTUREINFO& info)
{
    StopScroll(info.hwndTarget);
    m_ptsLocation = info.ptsLocation;

    return TRUE;
}

inline BOOL CGestureWindowBase::OnGestureEnd(const GESTUREINFO& info)
{
    StartScroll(info);
    return TRUE;
}

inline BOOL CGestureWindowBase::OnGestureScroll(const GESTUREINFO& info)
{
    StartScroll(info);
    return TRUE;
}

inline BOOL CGestureWindowBase::OnGestureHold(const GESTUREINFO& /*info*/)
{
    return FALSE;
}

inline BOOL CGestureWindowBase::OnGestureSelect(const GESTUREINFO& /*info*/)
{
    return FALSE;
}

inline BOOL CGestureWindowBase::OnGestureDoubleSelect(const GESTUREINFO& /*info*/)
{
    return FALSE;
}

inline void CGestureWindowBase::OnGestureAnimation(HWND hWnd, int nScrollX, int nScrollY, long /*lVelocityX*/, long /*lVelocityY*/, DWORD /*dwTime*/)
{
    ATLASSERT(::IsWindow(hWnd));

    POINT ptScrollPos = AdjustScrollPos(hWnd, nScrollX, nScrollY);
    if (m_nScrollX != ptScrollPos.x || m_nScrollY != ptScrollPos.y)
    {
        ::ScrollWindowEx(hWnd, m_nScrollX - ptScrollPos.x, m_nScrollY - ptScrollPos.y, NULL, NULL, NULL, NULL, SW_INVALIDATE | SW_SCROLLCHILDREN | SW_ERASE);
        m_nScrollX = ptScrollPos.x;
        m_nScrollY = ptScrollPos.y;

        ATLVERIFY(::UpdateWindow(hWnd));
    }
}

inline BOOL CGestureWindowBase::IsAnimationStopped() const
{
    return m_PhysicsEngine.IsEmpty();
}

inline void CGestureWindowBase::StartScroll(const GESTUREINFO& info)
{
    ATLASSERT(::IsWindow(info.hwndTarget));

    if (m_PhysicsEngine.IsEmpty() && (m_nFlags & GWIF_IGNORESCROLL) == 0)
    {
        sysutil::CClientRect rcClient(info.hwndTarget);
        WCE::CPhysicsEngineInit init(0, 0, m_sizeBoundary.cx, m_sizeBoundary.cy, m_nScrollX, m_nScrollY, rcClient.right, rcClient.bottom, -GID_SCROLL_VELOCITY(info.ullArguments), GID_SCROLL_ANGLE(info.ullArguments));
        if (SUCCEEDED(m_PhysicsEngine.Create(init)))
            _COND_VERIFY(::SetTimer(info.hwndTarget, IDT_GESTURE_ANIMATION, GESTURE_ANIMATION_FRAME_DELAY_MS, NULL), IDT_GESTURE_ANIMATION);
    }
}

inline void CGestureWindowBase::StopScroll(HWND hWnd)
{
    ATLASSERT(::IsWindow(hWnd));
    if (m_PhysicsEngine != NULL)
    {
        ATLVERIFY(::KillTimer(hWnd, IDT_GESTURE_ANIMATION));
        m_PhysicsEngine.Destroy();
    }
}

inline POINT CGestureWindowBase::AdjustScrollPos(HWND hWnd, int nScrollX, int nScrollY)
{
    ATLASSERT(::IsWindow(hWnd));

    POINT ptScrollPos = { 0 };
    sysutil::CClientRect rcClient(hWnd);

    if (m_nFlags & GWIF_HSCROLLABLE)
    {
        int nHoriExtent = ::MulDiv(rcClient.right, m_nHoriExtent, 100);
        ptScrollPos.x = stdutil::AdjustRange(nScrollX, -nHoriExtent, m_sizeBoundary.cx - rcClient.right + nHoriExtent);
    }

    if (m_nFlags & GWIF_VSCROLLABLE)
    {
        int nVertExtent = ::MulDiv(rcClient.bottom, m_nVertExtent, 100);
        ptScrollPos.y = stdutil::AdjustRange(nScrollY, -nVertExtent, m_sizeBoundary.cy - rcClient.bottom + nVertExtent);
    }

    return ptScrollPos;
}

inline LRESULT CGestureWindowBase::OnSize(HWND hWnd, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    ATLASSERT(::IsWindow(hWnd));

    if (wParam != SIZE_MINIMIZED)
    {
        m_nScrollY = stdutil::AdjustRange(m_nScrollY, 0, m_sizeBoundary.cy - HIWORD(lParam));
        ATLVERIFY(::InvalidateRect(hWnd, NULL, TRUE));
        ATLVERIFY(::UpdateWindow(hWnd));
    }

    return bHandled = FALSE;
}

inline LRESULT CGestureWindowBase::OnDestroy(HWND hWnd, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
    ATLASSERT(::IsWindow(hWnd));

    StopScroll(hWnd);
    return bHandled = FALSE;
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CGestureWindowImpl class
//

template <typename T>
inline CGestureWindowImpl<T>::CGestureWindowImpl()
{
}

template <typename T>
inline BOOL CGestureWindowImpl<T>::OnGesturePan(const GESTUREINFO& info)
{
    if ((m_nFlags & GWIF_IGNOREPAN) == 0)
    {
        T* pThis = static_cast<T*>(this);
        ATLASSERT(pThis);
        ATLASSERT(::IsWindow(pThis->m_hWnd));

        pThis->OnGestureAnimation(pThis->m_hWnd, m_nScrollX - (info.ptsLocation.x - m_ptsLocation.x), m_nScrollY - (info.ptsLocation.y - m_ptsLocation.y), 0, 0, 0);
        m_ptsLocation = info.ptsLocation;
    }

    return TRUE;
}

template <typename T>
inline LRESULT CGestureWindowImpl<T>::OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    CGesturePaintImpl<T>::OnPaint(m_sizeBoundary, m_nScrollX, m_nScrollY);
    return 0L;
}

template <typename T>
inline LRESULT CGestureWindowImpl<T>::OnTimer(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
{
    bHandled = (wParam == IDT_GESTURE_ANIMATION);
    if (bHandled)
    {
        T* pThis = static_cast<T*>(this);
        ATLASSERT(pThis);
        ATLASSERT(!IsAnimationStopped());
        ATLASSERT(::IsWindow(pThis->m_hWnd));

        WCE::CPhysicsEngineState state;
        if (SUCCEEDED(m_PhysicsEngine.Query(state)))
        {
            pThis->OnGestureAnimation(pThis->m_hWnd, state.ptPosition.x, state.ptPosition.y, state.lVelocityX, state.lVelocityY, state.dwTime);
            if (state.fComplete)
                StopScroll(pThis->m_hWnd);
        }
    }

    return 0L;
}

template <typename T>
inline LRESULT CGestureWindowImpl<T>::OnGesture(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    WCE::CGestureInfo info;
    if (info.GetGestureInfo(reinterpret_cast<HGESTUREINFO>(lParam)))
    {
        T* pThis = static_cast<T*>(this);
        ATLASSERT(pThis);
        ATLASSERT(::IsWindow(pThis->m_hWnd));
        info.hwndTarget = pThis->m_hWnd;

        switch (wParam)
        {
        case GID_BEGIN:
            bHandled = pThis->OnGestureBegin(info);
            break;

        case GID_END:
            bHandled = pThis->OnGestureEnd(info);
            break;

        case GID_PAN:
            bHandled = pThis->OnGesturePan(info);
            break;

        case GID_SCROLL:
            bHandled = pThis->OnGestureScroll(info);
            break;

        case GID_HOLD:
            bHandled = pThis->OnGestureHold(info);
            break;

        case GID_SELECT:
            bHandled = pThis->OnGestureSelect(info);
            break;

        case GID_DOUBLESELECT:
            bHandled = pThis->OnGestureDoubleSelect(info);
            break;

        default:
            ATLASSERT(FALSE);
        }
    }

    return bHandled;
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CAutoGestureWindowImpl class
//

template <typename T>
inline CAutoGestureWindowImpl<T>::CAutoGestureWindowImpl()
{
}

template <typename T>
inline BOOL CAutoGestureWindowImpl<T>::AutoGestureWindow_Init(DWORD dwFlags/* = WAGIF_OWNERANIMATE | WAGIF_VSCROLLABLE | WAGIF_HSCROLLABLE*/, UINT nAnimateStatusMessage/* = 0*/, BYTE nHorizontalExtent/* = 50*/, BYTE nVerticalExtent/* = 50*/, HBRUSH hExtentBrush/* = NULL*/)
{
    WCE::CAutoGestureInfo wagInfo(dwFlags, WM_GESTUREANIMATION, nAnimateStatusMessage, 1, 1, nHorizontalExtent, nVerticalExtent, hExtentBrush);
    return wagInfo.SetWindowAutoGesture(static_cast<T*>(this)->m_hWnd);
}

template <typename T>
inline LRESULT CAutoGestureWindowImpl<T>::OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    CGesturePaintImpl<T>::OnPaint(m_sizeTotal, m_nScrollX, m_nScrollY);
    return 0L;
}

template <typename T>
inline LRESULT CAutoGestureWindowImpl<T>::OnGestureAnimation(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);
    ATLASSERT(::IsWindow(pThis->m_hWnd));

    WCE::CAnimateMessageInfo info;
    if (info.GetAnimateMessageInfo(pThis->m_hWnd, wParam, lParam))
    {
        ::ScrollWindowEx(pThis->m_hWnd, m_nScrollX - info.nHPixelPosition, m_nScrollY - info.nVPixelPosition, NULL, NULL, NULL, NULL, SW_INVALIDATE | SW_SCROLLCHILDREN | SW_ERASE);
        m_nScrollX = info.nHPixelPosition;
        m_nScrollY = info.nVPixelPosition;

        ::SetScrollPos(pThis->m_hWnd, SB_HORZ, m_nScrollX, TRUE);
        ::SetScrollPos(pThis->m_hWnd, SB_VERT, m_nScrollY, TRUE);
        ATLVERIFY(::UpdateWindow(pThis->m_hWnd));
    }

    return 0L;
}
#endif  // __GESTUREUTIL_H__


#ifdef __AYGSHELL_H__
///////////////////////////////////////////////////////////////////////////////
// Implementation of the CShellChangeNotifyImpl class
//

template <typename T>
inline CShellChangeNotifyImpl<T>::CShellChangeNotifyImpl()
{
}

template <typename T>
inline void CShellChangeNotifyImpl<T>::SHChangeNotify_OnFileChange(const FILECHANGEINFO& /*fci*/)
{
}

template <typename T>
inline BOOL CShellChangeNotifyImpl<T>::SHChangeNotify_Register(DWORD dwEvent/* = SHCNE_ALLEVENTS*/, LPCTSTR pszWatchDir/* = NULL*/, BOOL bRecursive/* = TRUE*/)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);
    ATLASSERT(::IsWindow(pThis->m_hWnd));

    SHCHANGENOTIFYENTRY shcne = { dwEvent, (LPTSTR)pszWatchDir, bRecursive };
    return ::SHChangeNotifyRegister(pThis->m_hWnd, &shcne);
}

template <typename T>
inline BOOL CShellChangeNotifyImpl<T>::SHChangeNotify_Unregister()
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);
    ATLASSERT(::IsWindow(pThis->m_hWnd));

    return ::SHChangeNotifyDeregister(pThis->m_hWnd);
}

template <typename T>
inline LRESULT CShellChangeNotifyImpl<T>::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
    SHChangeNotify_Unregister();
    return bHandled = FALSE;
}

template <typename T>
inline LRESULT CShellChangeNotifyImpl<T>::OnFileChange(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);
    ATLASSERT(lParam);
    ATLASSERT(::IsWindow(pThis->m_hWnd));

    pThis->SHChangeNotify_OnFileChange(((FILECHANGENOTIFY*)lParam)->fci);
    return 0L;
}
#endif  // __AYGSHELL_H__

}  // namespace WCE

#endif  // __ATLCECTRLS_INL__