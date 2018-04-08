///////////////////////////////////////////////////////////////////////////////
// atlcomframe.inl : 实现文件
//
// Copyright(c) 2012, Garfield. All rights reserved.
// Author  : Garfield
// Version : 1.0
// Finish Time : 2008/8/17

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __ATLCOMFRAME_INL__
#define __ATLCOMFRAME_INL__

namespace atlctrls {

///////////////////////////////////////////////////////////////////////////////
// CResizeDialogImpl 类的实现
//

template <typename T>
inline CResizeDialogImpl<T>::CResizeDialogImpl()
{
    m_ptMinSize.x = m_ptMinSize.y = -1;
    m_sizeClient.cx = m_sizeClient.cy = 0;
}

template <typename T>
inline void CResizeDialogImpl<T>::ResizeDialog_Initialize(BOOL bAddGripper/* = TRUE*/, BOOL bUseMinSize/* = TRUE*/)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);
    ATLASSERT(::IsWindow(pThis->m_hWnd));

    RECT rect;
    ATLVERIFY(pThis->GetClientRect(&rect));
    m_sizeClient.cx = rect.right;
    m_sizeClient.cy = rect.bottom;

    int nCount = 0;
    _ATL_DLGITEM_ENTRY* pEntry = T::_GetDlgItemEntry(nCount);
    ATLASSERT(pEntry != NULL && nCount > 0);

    if (bAddGripper)
    {
        ATL::CWindow wndScrollBar = pThis->GetDlgItem(_ATL_ID_SCROLL_BAR);
        ATLASSERT(!wndScrollBar.IsWindow());

        wndScrollBar.Create(WC_SCROLLBAR, pThis->m_hWnd, &rect, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | SBS_SIZEBOX | SBS_SIZEGRIP | SBS_SIZEBOXBOTTOMRIGHTALIGN, 0, _ATL_ID_SCROLL_BAR);
        ATLASSERT(wndScrollBar.IsWindow());

        ATLVERIFY(wndScrollBar.GetWindowRect(&pEntry[0].rect));
        ATLVERIFY(pThis->ScreenToClient(&pEntry[0].rect));
    }

    if (bUseMinSize)
    {
        ATLVERIFY(pThis->GetWindowRect(&rect));
        m_ptMinSize.x = rect.right - rect.left;
        m_ptMinSize.y = rect.bottom - rect.top;
    }

    for (int i = 1, nIndex = -1; i < nCount; ++i)
    {
        if (pEntry[i].dwFlags == _DLGITEM_GROUP)
        {
            nIndex = i;
            continue;
        }

        ATLASSERT(pEntry[i].nID != 0);
        ATLVERIFY(pThis->GetDlgItem(pEntry[i].nID).GetWindowRect(&pEntry[i].rect));
        ATLVERIFY(pThis->ScreenToClient(&pEntry[i].rect));

        if (nIndex != -1)
        {
            ++pEntry[nIndex].nID;
            pEntry[nIndex].rect.left   = min(pEntry[nIndex].rect.left, pEntry[i].rect.left);
            pEntry[nIndex].rect.top    = min(pEntry[nIndex].rect.top, pEntry[i].rect.top);
            pEntry[nIndex].rect.right  = max(pEntry[nIndex].rect.right, pEntry[i].rect.right);
            pEntry[nIndex].rect.bottom = max(pEntry[nIndex].rect.bottom, pEntry[i].rect.bottom);
        }
    }
}

template <typename T>
inline BOOL CResizeDialogImpl<T>::ResizeDialog_SetDlgItemPos(const _ATL_DLGITEM_ENTRY& item, int dx, int dy, LPCRECT lpRect/* = NULL*/, int xOffset/* = -1*/, int yOffset/* = -1*/)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);
    ATLASSERT(::IsWindow(pThis->m_hWnd));

    ATL::CWindow wndCtrl = pThis->GetDlgItem(item.nID);
    if (!wndCtrl.IsWindow())
        return FALSE;

    RECT rect = item.rect;
    if (lpRect != NULL)
    {
        ATLASSERT((item.dwFlags & DLGITEM_MOVE_X) == 0);
        ATLASSERT((item.dwFlags & DLGITEM_MOVE_Y) == 0);

        if (item.dwFlags & DLGITEM_SIZE_X)
        {
            int nWidth = lpRect->right - lpRect->left;
            rect.left  = lpRect->left + ::MulDiv(item.rect.left - lpRect->left, nWidth + dx, nWidth);
            rect.right = lpRect->left + ::MulDiv((xOffset != -1 ? xOffset : item.rect.right) - lpRect->left, nWidth + dx, nWidth - (xOffset != -1 ? xOffset - item.rect.right : 0));
        }

        if (item.dwFlags & DLGITEM_SIZE_Y)
        {
            int nHeight = lpRect->bottom - lpRect->top;
            rect.top    = lpRect->top + ::MulDiv(item.rect.top - lpRect->top, nHeight + dy, nHeight);
            rect.bottom = lpRect->top + ::MulDiv((yOffset != -1 ? yOffset : item.rect.bottom) - lpRect->top, nHeight + dy, nHeight - (yOffset != -1 ? yOffset - item.rect.bottom : 0));
        }
    }
    else
    {
        if (item.dwFlags & (DLGITEM_SIZE_X | DLGITEM_MOVE_X))
        {
            rect.right = item.rect.right + dx;
            if (item.dwFlags & DLGITEM_MOVE_X)
                rect.left = rect.right - (item.rect.right - item.rect.left);
        }

        if (item.dwFlags & (DLGITEM_SIZE_Y | DLGITEM_MOVE_Y))
        {
            rect.bottom = item.rect.bottom + dy;
            if (item.dwFlags & DLGITEM_MOVE_Y)
                rect.top = rect.bottom - (item.rect.bottom - item.rect.top);
        }
    }

    return wndCtrl.SetWindowPos(NULL, &rect, SWP_NOZORDER | SWP_NOACTIVATE);
}

template <typename T>
inline void CResizeDialogImpl<T>::ResizeDialog_UpdateLayout(int cx, int cy)
{
    //T* pT = static_cast<T*>(this);
    //ATLASSERT(::IsWindow(pT->m_hWnd));

    //// Restrict minimum size if requested
    //if(((pT->GetStyle() & WS_CHILD) != 0) && m_ptMinTrackSize.x != -1 && m_ptMinTrackSize.y != -1)
    //{
    //  if(cxWidth < m_ptMinTrackSize.x)
    //      cxWidth = m_ptMinTrackSize.x;
    //  if(cyHeight < m_ptMinTrackSize.y)
    //      cyHeight = m_ptMinTrackSize.y;
    //}

    //BOOL bVisible = pT->IsWindowVisible();
    //if(bVisible)
    //  pT->SetRedraw(FALSE);

    //RECT rectGroup = { 0, 0, 0, 0 };
    //for(int i = 0; i < m_arrData.GetSize(); i++)
    //{
    //  if((m_arrData[i].m_dwResizeFlags & _DLSZ_BEGIN_GROUP) != 0)   // start of a group
    //  {
    //      int nGroupCount = m_arrData[i].GetGroupCount();
    //      ATLASSERT(nGroupCount > 0 && i + nGroupCount - 1 < m_arrData.GetSize());
    //        rectGroup = m_arrData[i].m_rect;
    //        int j;
    //        for(j = 1; j < nGroupCount; j++)
    //        {
    //            rectGroup.left = min(rectGroup.left, m_arrData[i + j].m_rect.left);
    //            rectGroup.top = min(rectGroup.top, m_arrData[i + j].m_rect.top);
    //            rectGroup.right = max(rectGroup.right, m_arrData[i + j].m_rect.right);
    //            rectGroup.bottom = max(rectGroup.bottom, m_arrData[i + j].m_rect.bottom);
    //        }
    //        RECT rcThis = { 0 };
    //        RECT rcNext = { 0 };
    //        for(j = 0; j < nGroupCount; j++)
    //        {
    //            int xyStartNext = -1;
    //            if((j < (nGroupCount - 1)) && ((m_arrData[i + j].m_dwResizeFlags & (DLSZ_SIZE_X | DLSZ_SIZE_Y)) != 0) && ((m_arrData[i + j + 1].m_dwResizeFlags & (DLSZ_SIZE_X | DLSZ_SIZE_Y)) != 0))
    //            {
    //                ATL::CWindow ctlThis = pT->GetDlgItem(m_arrData[i + j].m_nCtlID);
    //                ctlThis.GetWindowRect(&rcThis);
    //                ::MapWindowPoints(NULL, pT->m_hWnd, (LPPOINT)&rcThis, 2);
    //                ATL::CWindow ctlNext = pT->GetDlgItem(m_arrData[i + j + 1].m_nCtlID);
    //                ctlNext.GetWindowRect(&rcNext);
    //                ::MapWindowPoints(NULL, pT->m_hWnd, (LPPOINT)&rcNext, 2);
    //                if((m_arrData[i + j].m_dwResizeFlags & DLSZ_SIZE_X) == DLSZ_SIZE_X && (m_arrData[i + j + 1].m_dwResizeFlags & DLSZ_SIZE_X) == DLSZ_SIZE_X)
    //                {
    //                    if(rcNext.left >= rcThis.right)
    //                        xyStartNext = m_arrData[i + j + 1].m_rect.left;
    //                }
    //                else if((m_arrData[i + j].m_dwResizeFlags & DLSZ_SIZE_Y) == DLSZ_SIZE_Y && (m_arrData[i + j + 1].m_dwResizeFlags & DLSZ_SIZE_Y) == DLSZ_SIZE_Y)
    //                {
    //                    if(rcNext.top >= rcThis.bottom)
    //                        xyStartNext = m_arrData[i + j + 1].m_rect.top;
    //                }
    //            }
    //            pT->DlgResize_PositionControl(cxWidth, cyHeight, rectGroup, m_arrData[i + j], true, xyStartNext);
    //        }
    //        // increment to skip all group controls
    //        i += nGroupCount - 1;
    //    }
    //    else      // one control entry
    //    {
    //        pT->DlgResize_PositionControl(cxWidth, cyHeight, rectGroup, m_arrData[i], false);
    //    }
    //}

    //if(bVisible)
    //  pT->SetRedraw(TRUE);
    //pT->RedrawWindow(NULL, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_UPDATENOW | RDW_ALLCHILDREN);

    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);
    ATLASSERT(::IsWindow(pThis->m_hWnd));
    pThis->SetRedraw(FALSE);

    int nCount = 0;
    _ATL_DLGITEM_ENTRY* pEntry = T::_GetDlgItemEntry(nCount);
    ATLASSERT(pEntry != NULL && nCount > 0);

    for (int i = 0; i < nCount; ++i)
    {
        if (pEntry[i].dwFlags != _DLGITEM_GROUP)
        {
            pThis->ResizeDialog_SetDlgItemPos(pEntry[i], cx - m_sizeClient.cx, cy - m_sizeClient.cy);
            continue;
        }

        int nCount = pEntry[i].nID;
        for (int j = 0; j < nCount; ++j, ++i)
        {
            pThis->ResizeDialog_SetDlgItemPos(pEntry[i], cx - m_sizeClient.cx, cy - m_sizeClient.cy);
        }
    }

    pThis->SetRedraw(TRUE);
    pThis->RedrawWindow(NULL, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_UPDATENOW | RDW_ALLCHILDREN);
}

template <typename T>
inline LRESULT CResizeDialogImpl<T>::OnSize(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
    if (wParam != SIZE_MINIMIZED)
    {
        T* pThis = static_cast<T*>(this);
        ATLASSERT(pThis);
        ATLASSERT(::IsWindow(pThis->m_hWnd));

        ATL::CWindow wndScrollBar = pThis->GetDlgItem(_ATL_ID_SCROLL_BAR);
        if (wndScrollBar.IsWindow())
            wndScrollBar.ShowWindow(wParam == SIZE_RESTORED ? SW_SHOW : SW_HIDE);

        pThis->ResizeDialog_UpdateLayout(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
    }

    return 0L;
}

template <typename T>
inline LRESULT CResizeDialogImpl<T>::OnGetMinMaxInfo(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
    if (m_ptMinSize.x != -1 && m_ptMinSize.y != -1)
        reinterpret_cast<LPMINMAXINFO>(lParam)->ptMinTrackSize = m_ptMinSize;

    return 0L;
}

}  // namespace atlctrls

#endif  // __ATLCOMFRAME_INL__