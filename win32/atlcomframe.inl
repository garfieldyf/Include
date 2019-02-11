///////////////////////////////////////////////////////////////////////////////
// atlcomframe.inl
//
// Copyright(c) 2019, Garfield. All rights reserved.
// Author  : Garfield
// Version : 1.0
// Creation Date : 2008/10/2

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __ATLCOMFRAME_INL__
#define __ATLCOMFRAME_INL__

#ifndef __ATLCOMFRAME_H__
    #error atlcomframe.inl requires atlcomframe.h to be included first
#endif

namespace atlctrls {

///////////////////////////////////////////////////////////////////////////////
// Implementation of the CWindowPaintFrameBase class
//

inline CWindowPaintFrameBase::CWindowPaintFrameBase(UINT nFrameWidth, UINT nFrameHeight, UINT nCaptionHeight)
    : m_nFrameWidth(nFrameWidth), m_nFrameHeight(nFrameHeight), m_nHitCode(HTNOWHERE)
    , m_nCaptionHeight(nCaptionHeight), m_bMouseHover(FALSE), m_bThemeInactive(FALSE)
{
}

inline void CWindowPaintFrameBase::PaintFrame_Init(HWND hWnd)
{
    ATLASSERT(::IsWindow(hWnd));

    sysutil::CClientRect rcClient(hWnd);
    m_bThemeInactive = PaintFrame_IsThemeInactive(hWnd);
    ATLVERIFY(::SetWindowPos(hWnd, NULL, 0, 0, rcClient.right + m_nFrameWidth * 2, rcClient.bottom + m_nFrameHeight * 2 + m_nCaptionHeight, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER | SWP_DRAWFRAME));
}

inline void CWindowPaintFrameBase::PaintFrame_CalcCaptionRect(HWND hWnd, const RECT& rcWindow, RECT& rcCaption) const
{
    ATLASSERT(::IsWindow(hWnd));

    UINT cxFrame = m_nFrameWidth, cyFrame = m_nFrameHeight;
    if (::IsZoomed(hWnd))
    {
        cxFrame = ::GetSystemMetrics(SM_CXSIZEFRAME);
        cyFrame = ::GetSystemMetrics(SM_CYSIZEFRAME);
    }

    stdutil::SetRect(rcCaption, rcWindow.left + cxFrame, rcWindow.top + cyFrame, rcWindow.right - cxFrame, rcWindow.top + cyFrame + m_nCaptionHeight);
}

inline BOOL WINAPI CWindowPaintFrameBase::PaintFrame_IsThemeInactive(HWND hWnd)
{
    ATLASSERT(::IsWindow(hWnd));

    TCHAR szClassName[10];
    ::GetClassName(hWnd, szClassName, _countof(szClassName));

    return ((::lstrcmp(szClassName, _T("#32770")) == 0) && (::GetWindowLong(hWnd, GWL_STYLE) & WS_THICKFRAME) && !(sysutil::IsWindowsVista() && sysutil::IsThemeActive()));
}

inline BOOL WINAPI CWindowPaintFrameBase::PaintFrame_IsButtonHitCode(UINT nHitCode)
{
    return (nHitCode == HTCLOSE || nHitCode == HTHELP || nHitCode == HTMINBUTTON || nHitCode == HTMAXBUTTON || nHitCode >= HTCUSTOM);
}

inline UINT WINAPI CWindowPaintFrameBase::PaintFrame_MapWindowCommand(HWND hWnd, UINT nHitCode)
{
    ATLASSERT(::IsWindow(hWnd));

    UINT nCommand = 0;
    switch (nHitCode)
    {
    case HTCLOSE:
        nCommand = SC_CLOSE;
        break;

    case HTHELP:
        nCommand = SC_CONTEXTHELP;
        break;

    case HTMINBUTTON:
        nCommand = SC_MINIMIZE;
        break;

    case HTMAXBUTTON:
        nCommand = (::IsZoomed(hWnd) ? SC_RESTORE : SC_MAXIMIZE);
        break;

    default:
        if (nHitCode >= HTCUSTOM)
            nCommand = nHitCode;
    }

    return nCommand;
}

inline UINT WINAPI CWindowPaintFrameBase::PaintFrame_CalcAllItemsWidth(const _ATL_FRAME_ENTRY* pFirst)
{
    ATLASSERT(pFirst);

    UINT cx = 0;
    for (; pFirst->nHitCode != HTNOWHERE; ++pFirst)
        cx += pFirst->nWidth + pFirst->nPadding;

    return cx;
}

inline UINT WINAPI CWindowPaintFrameBase::PaintFrame_FindItem(UINT nHitCode, const _ATL_FRAME_ENTRY*& pEntry)
{
    ATLASSERT(pEntry);
    ATLASSERT(nHitCode < 0xF000);
    ATLASSERT(PaintFrame_IsButtonHitCode(nHitCode));

    UINT nIndex = 0;
    for (; pEntry->nHitCode != HTNOWHERE; ++pEntry, ++nIndex)
    {
        if (pEntry->nHitCode == nHitCode)
            break;
    }

    ATLASSERT(pEntry != NULL && pEntry->nHitCode != HTNOWHERE);
    return nIndex;
}

inline void WINAPI CWindowPaintFrameBase::PaintFrame_CalcItemRect(const _ATL_FRAME_ENTRY* pFirst, const _ATL_FRAME_ENTRY* pEntry, const RECT& rcCaption, UINT nIndex, RECT& rcItem)
{
    ATLASSERT(pEntry);
    ATLASSERT(pFirst);

    rcItem.top    = rcCaption.top + (rcCaption.bottom - rcCaption.top - pEntry->nHeight) / 2;
    rcItem.right  = rcCaption.right - pEntry->nPadding;
    rcItem.bottom = rcItem.top + pEntry->nHeight;

    for (UINT i = 0; i < nIndex; ++i)
        rcItem.right -= pFirst[i].nWidth + pFirst[i].nPadding;

    rcItem.left = rcItem.right - pEntry->nWidth;
}

inline LRESULT CWindowPaintFrameBase::OnNcCalcSize(HWND hWnd, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
    if (wParam)
    {
        LPNCCALCSIZE_PARAMS pParams = reinterpret_cast<LPNCCALCSIZE_PARAMS>(lParam);
        ATLASSERT(pParams);
        ATLASSERT(::IsWindow(hWnd));

        if (::IsZoomed(hWnd))
        {
            ATLVERIFY(::SystemParametersInfo(SPI_GETWORKAREA, 0, &pParams->rgrc[0], 0));
            pParams->rgrc[0].top += m_nCaptionHeight;
        }
        else
        {
            pParams->rgrc[0].left   += m_nFrameWidth;
            pParams->rgrc[0].top    += m_nFrameHeight + m_nCaptionHeight;
            pParams->rgrc[0].right  -= m_nFrameWidth;
            pParams->rgrc[0].bottom -= m_nFrameHeight;
        }
    }

    return 0L;
}

inline LRESULT CWindowPaintFrameBase::OnNcLButtonUp(HWND hWnd, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    ATLASSERT(::IsWindow(hWnd));

    if (UINT nCommand = PaintFrame_MapWindowCommand(hWnd, wParam))
    {
        ::SendMessage(hWnd, WM_NCDRAWITEM, wParam, 0L);
        ::SendMessage(hWnd, WM_SYSCOMMAND, nCommand, lParam);
    }

    return bHandled = FALSE;
}

inline LRESULT CWindowPaintFrameBase::OnNcMouseMove(HWND hWnd, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
{
    ATLASSERT(::IsWindow(hWnd));

    if (!m_bMouseHover)
    {
        TRACKMOUSEEVENT tme = { sizeof(TRACKMOUSEEVENT), TME_NONCLIENT | TME_LEAVE, hWnd, 0 };
        m_bMouseHover = ::TrackMouseEvent(&tme);
    }

    if (wParam != m_nHitCode)
    {
        ::SendMessage(hWnd, WM_NCDRAWITEM, m_nHitCode, 0L);
        ::SendMessage(hWnd, WM_NCDRAWITEM, wParam, CDIS_HOT);
        m_nHitCode = wParam;
    }

    return bHandled = FALSE;
}

inline LRESULT CWindowPaintFrameBase::OnNcMouseLeave(HWND hWnd, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    ATLASSERT(::IsWindow(hWnd));

    ::SendMessage(hWnd, WM_NCDRAWITEM, m_nHitCode, 0L);
    return m_bMouseHover = m_nHitCode = FALSE;
}

inline LRESULT CWindowPaintFrameBase::OnThemeChanged(HWND hWnd, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    ATLASSERT(::IsWindow(hWnd));

    m_bThemeInactive = PaintFrame_IsThemeInactive(hWnd);
    return 0L;
}

inline LRESULT CWindowPaintFrameBase::OnNcLButtonDown(HWND hWnd, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
{
    ATLASSERT(::IsWindow(hWnd));

    if (m_bThemeInactive && (wParam == HTCAPTION || (wParam >= HTLEFT && wParam <= HTBORDER)))
        ::SetWindowLong(hWnd, GWL_STYLE, ::GetWindowLong(hWnd, GWL_STYLE) & ~WS_SYSMENU);

    bHandled = ::SendMessage(hWnd, WM_NCDRAWITEM, wParam, CDIS_SELECTED);
    return 0L;
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CWindowPaintFrameImpl class
//

template <typename T>
inline CWindowPaintFrameImpl<T>::CWindowPaintFrameImpl(UINT nFrameWidth/* = ::GetSystemMetrics(SM_CXSIZEFRAME)*/, UINT nFrameHeight/* = ::GetSystemMetrics(SM_CYSIZEFRAME)*/, UINT nCaptionHeight/* = ::GetSystemMetrics(SM_CYCAPTION)*/)
    : CWindowPaintFrameBase(nFrameWidth, nFrameHeight, nCaptionHeight)
{
}

template <typename T>
inline void CWindowPaintFrameImpl<T>::PaintFrame_OnNcDraw(BOOL bActivate)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);
    ATLASSERT(::IsWindow(pThis->m_hWnd));

    HDC hDC = ::GetWindowDC(pThis->m_hWnd);
    ATLASSERT(hDC);

    PAINTITEM paint = { hDC, { 0 }, 0, 0, bActivate };
    ATLVERIFY(::GetWindowRect(pThis->m_hWnd, &paint.rcItem));
    ATLVERIFY(::OffsetRect(&paint.rcItem, -paint.rcItem.left, -paint.rcItem.top));

    if (!::IsZoomed(pThis->m_hWnd))
    {
        ::ExcludeClipRect(hDC, paint.rcItem.left + m_nFrameWidth, paint.rcItem.top + m_nFrameHeight + m_nCaptionHeight, paint.rcItem.right - m_nFrameWidth, paint.rcItem.bottom - m_nFrameHeight);
        pThis->PaintFrame_OnDrawFrame(paint);
    }

    RECT rcCaption;
    pThis->PaintFrame_CalcCaptionRect(paint.rcItem, rcCaption);
    paint.rcItem = rcCaption;
    pThis->PaintFrame_OnDrawCaption(paint);

    UINT nIndex = 0;
    for (const _ATL_FRAME_ENTRY* pEntries = T::_GetFrameEntries(); pEntries->nHitCode != HTNOWHERE; ++pEntries, ++nIndex)
    {
        paint.nHitCode = pEntries->nHitCode;
        pThis->PaintFrame_CalcItemRect(rcCaption, nIndex, pEntries, paint.rcItem);
        pThis->PaintFrame_OnDrawItem(paint);
    }

    ATLVERIFY(::ReleaseDC(pThis->m_hWnd, hDC));
}

template <typename T>
inline void CWindowPaintFrameImpl<T>::PaintFrame_OnDrawItem(const PAINTITEM& /*paint*/)
{
    ATLASSERT(FALSE && _T("draw minimized, maximized and close button, MUST BE IMPLEMENTED IN A DERIVED CLASS."));
}

template <typename T>
inline void CWindowPaintFrameImpl<T>::PaintFrame_OnDrawFrame(const PAINTITEM& /*paint*/)
{
    ATLASSERT(FALSE && _T("draw window frame, MUST BE IMPLEMENTED IN A DERIVED CLASS."));
}

template <typename T>
inline void CWindowPaintFrameImpl<T>::PaintFrame_OnDrawCaption(const PAINTITEM& /*paint*/)
{
    ATLASSERT(FALSE && _T("draw window caption, caption text and icon, MUST BE IMPLEMENTED IN A DERIVED CLASS."));
}

template <typename T>
inline void CWindowPaintFrameImpl<T>::PaintFrame_CalcCaptionRect(const RECT& rcWindow, RECT& rcCaption) const
{
    CWindowPaintFrameBase::PaintFrame_CalcCaptionRect(static_cast<const T*>(this)->m_hWnd, rcWindow, rcCaption);
}

template <typename T>
inline void CWindowPaintFrameImpl<T>::PaintFrame_CalcItemRect(const RECT& rcCaption, UINT nIndex, const _ATL_FRAME_ENTRY* pEntry, RECT& rcItem) const
{
    CWindowPaintFrameBase::PaintFrame_CalcItemRect(T::_GetFrameEntries(), pEntry, rcCaption, nIndex, rcItem);
}

template <typename T>
inline void CWindowPaintFrameImpl<T>::PaintFrame_Init()
{
    CWindowPaintFrameBase::PaintFrame_Init(static_cast<T*>(this)->m_hWnd);
}

template <typename T>
inline UINT CWindowPaintFrameImpl<T>::PaintFrame_GetAllItemsWidth() const
{
    return T::PaintFrame_CalcAllItemsWidth(T::_GetFrameEntries());
}

template <typename T>
inline void CWindowPaintFrameImpl<T>::PaintFrame_GetItemRect(const RECT& rcWindow, UINT nHitCode, RECT& rcItem) const
{
    const T* pThis = static_cast<const T*>(this);
    ATLASSERT(pThis);
    ATLASSERT(nHitCode < 0xF000);
    ATLASSERT(::IsWindow(pThis->m_hWnd));
    ATLASSERT(PaintFrame_IsButtonHitCode(nHitCode));

    const _ATL_FRAME_ENTRY* pEntries = T::_GetFrameEntries();
    UINT nIndex = PaintFrame_FindItem(nHitCode, pEntries);

    RECT rcCaption;
    pThis->PaintFrame_CalcCaptionRect(rcWindow, rcCaption);
    pThis->PaintFrame_CalcItemRect(rcCaption, nIndex, pEntries, rcItem);
}

template <typename T>
inline BOOL CWindowPaintFrameImpl<T>::PaintFrame_DrawItem(UINT nHitCode, UINT nState/* = 0*/)
{
    return ::SendMessage(static_cast<T*>(this)->m_hWnd, WM_NCDRAWITEM, nHitCode, (LPARAM)nState);
}

template <typename T>
inline UINT CWindowPaintFrameImpl<T>::PaintFrame_HitTest(const RECT& rcCaption, POINT point) const
{
    const T* pThis = static_cast<const T*>(this);
    ATLASSERT(pThis);
    ATLASSERT(::IsWindow(pThis->m_hWnd));

    UINT nIndex = 0;
    RECT rcItem;

    for (const _ATL_FRAME_ENTRY* pEntries = T::_GetFrameEntries(); pEntries->nHitCode != HTNOWHERE; ++pEntries, ++nIndex)
    {
        pThis->PaintFrame_CalcItemRect(rcCaption, nIndex, pEntries, rcItem);
        if (::PtInRect(&rcItem, point))
            return pEntries->nHitCode;
    }

    return HTCAPTION;
}

template <typename T>
inline BOOL CWindowPaintFrameImpl<T>::PaintFrame_WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult, WNDPROC pfnDefWindowProc)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);
    ATLASSERT(pfnDefWindowProc);
    ATLASSERT(::IsWindow(pThis->m_hWnd));

    BOOL bHandled = CWindowPaintFrameImpl<T>::ProcessWindowMessage(pThis->m_hWnd, uMsg, wParam, lParam, lResult);
    if (m_bThemeInactive && !bHandled)
    {
        if (uMsg == WM_INITMENU)
        {
            bHandled = TRUE;
            pThis->PaintFrame_OnNcDraw(::GetActiveWindow() == pThis->m_hWnd);
        }
        else if (uMsg == WM_SETCURSOR)
        {
            DWORD dwStyle = ::GetWindowLong(pThis->m_hWnd, GWL_STYLE);
            ::SetWindowLong(pThis->m_hWnd, GWL_STYLE, dwStyle & ~WS_SYSMENU);

            lResult  = ::CallWindowProc(pfnDefWindowProc, pThis->m_hWnd, uMsg, wParam, lParam);
            bHandled = (::SetWindowLong(pThis->m_hWnd, GWL_STYLE, dwStyle | WS_SYSMENU), TRUE);
        }
        else if (uMsg == WM_NCLBUTTONDBLCLK)
        {
            ::SetWindowLong(pThis->m_hWnd, GWL_STYLE, ::GetWindowLong(pThis->m_hWnd, GWL_STYLE) & ~WS_SYSMENU);
        }
    }

    return bHandled;
}

template <typename T>
inline LRESULT CWindowPaintFrameImpl<T>::OnNcPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);
    ATLASSERT(::IsWindow(pThis->m_hWnd));

    pThis->PaintFrame_OnNcDraw(::GetActiveWindow() == pThis->m_hWnd);
    return 0L;
}

template <typename T>
inline LRESULT CWindowPaintFrameImpl<T>::OnNcHitTest(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);
    ATLASSERT(::IsWindow(pThis->m_hWnd));

    RECT rcCaption;
    sysutil::CWindowRect rect(pThis->m_hWnd);
    pThis->PaintFrame_CalcCaptionRect(rect, rcCaption);

    bHandled = FALSE;
    POINT point = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };

    if (::PtInRect(&rcCaption, point))
    {
        stdutil::SetRect(rect, rcCaption.left, rcCaption.top, rcCaption.left + ::GetSystemMetrics(SM_CXSMICON), rcCaption.bottom);
        bHandled = (rect.PtInRect(point) ? HTMENU : PaintFrame_HitTest(rcCaption, point));
    }

    return bHandled;
}

template <typename T>
inline LRESULT CWindowPaintFrameImpl<T>::OnNcActivate(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);
    ATLASSERT(::IsWindow(pThis->m_hWnd));

    pThis->PaintFrame_OnNcDraw((BOOL)wParam);
    return 1L;
}

template <typename T>
inline LRESULT CWindowPaintFrameImpl<T>::OnNcDrawItem(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
    BOOL bSuccessful = PaintFrame_IsButtonHitCode(wParam);
    if (bSuccessful)
    {
        T* pThis = static_cast<T*>(this);
        ATLASSERT(pThis);
        ATLASSERT(::IsWindow(pThis->m_hWnd));

        HDC hDC = ::GetWindowDC(pThis->m_hWnd);
        ATLASSERT(hDC);

        sysutil::CWindowRect rcWindow(pThis->m_hWnd);
        rcWindow.OffsetRect(-rcWindow.left, -rcWindow.top);

        PAINTITEM paint = { hDC, { 0 }, lParam, wParam, (::GetActiveWindow() == pThis->m_hWnd) }; 
        PaintFrame_GetItemRect(rcWindow, wParam, paint.rcItem);
        pThis->PaintFrame_OnDrawItem(paint);

        ATLVERIFY(::ReleaseDC(pThis->m_hWnd, hDC));
    }

    return bSuccessful;
}

}  // namespace atlctrls

#endif  // __ATLCOMFRAME_INL__