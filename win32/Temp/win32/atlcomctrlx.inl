///////////////////////////////////////////////////////////////////////////////
// atlcomctrlx.inl : 实现文件
//
// Copyright(c) 2012, Garfield. All rights reserved.
// Author  : Garfield
// Version : 1.0
// Finish Time : 2007/2/4

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __ATLCOMCTRLX_INL__
#define __ATLCOMCTRLX_INL__

namespace atlctrls {

///////////////////////////////////////////////////////////////////////////////
// CCustomListCtrlImpl 类的实现
//

inline CCustomListCtrlImpl::CCustomListCtrlImpl()
    : m_clrAlterBk(RGB(255, 255, 220)), m_clrNormalBk(RGB(225, 240, 255))
{
    m_clrNormalText = m_clrAlterText = ::GetSysColor(COLOR_WINDOWTEXT);
}

inline COLORREF CCustomListCtrlImpl::GetAlterBkColor() const
{
    return m_clrAlterBk;
}

inline void CCustomListCtrlImpl::SetAlterBkColor(COLORREF color)
{
    m_clrAlterBk = color;
}

inline COLORREF CCustomListCtrlImpl::GetNormalBkColor() const
{
    return m_clrNormalBk;
}

inline void CCustomListCtrlImpl::SetNormalBkColor(COLORREF color)
{
    m_clrNormalBk = color;
}

inline COLORREF CCustomListCtrlImpl::GetAlterTextColor() const
{
    return m_clrAlterText;
}

inline void CCustomListCtrlImpl::SetAlterTextColor(COLORREF color)
{
    m_clrAlterText = color;
}

inline COLORREF CCustomListCtrlImpl::GetNormalTextColor() const
{
    return m_clrNormalText;
}

inline void CCustomListCtrlImpl::SetNormalTextColor(COLORREF color)
{
    m_clrNormalText = color;
}

inline LRESULT CCustomListCtrlImpl::OnCustomDraw(int /*nIDCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/)
{
    LPNMLVCUSTOMDRAW pNMCustomDraw = reinterpret_cast<LPNMLVCUSTOMDRAW>(pNMHDR);
    ATLASSERT(pNMCustomDraw);
    ATLASSERT(::IsWindow(pNMCustomDraw->nmcd.hdr.hwndFrom));

    LRESULT lResult = CDRF_DODEFAULT;
    switch (pNMCustomDraw->nmcd.dwDrawStage)
    {
    // 绘制整个列表控件
    case CDDS_PREPAINT:
        if (::IsWindowEnabled(pNMCustomDraw->nmcd.hdr.hwndFrom))
            lResult = CDRF_NOTIFYITEMDRAW;
        break;

    // 绘制列表项
    case CDDS_ITEMPREPAINT:
        pNMCustomDraw->clrText   = (pNMCustomDraw->nmcd.dwItemSpec % 2) ? m_clrAlterText : m_clrNormalText;
        pNMCustomDraw->clrTextBk = (pNMCustomDraw->nmcd.dwItemSpec % 2) ? m_clrAlterBk   : m_clrNormalBk;
        break;
    }

    return lResult;
}


///////////////////////////////////////////////////////////////////////////////
// CListCtrl 类的实现
//

inline CListCtrl::CListCtrl()
{
}

inline BOOL CListCtrl::SubclassWindow(HWND hWnd)
{
    BOOL bSuccessful = ATL::CWindowImpl<CListCtrl, WTL::CListViewCtrl, CListCtrlTraits>::SubclassWindow(hWnd);
    if (bSuccessful)
    {
        ModifyStyle(0, CListCtrlTraits::GetWndStyle(0));
        SetExtendedListViewStyle(CListCtrlTraits::GetWndExStyle(GetExtendedListViewStyle()));
    }

    return bSuccessful;
}


///////////////////////////////////////////////////////////////////////////////
// CDBListCtrl 类的实现
//

inline CDBListCtrl::CDBListCtrl()
{
}

inline BOOL CDBListCtrl::SubclassWindow(HWND hWnd)
{
    BOOL bSuccessful = ATL::CWindowImpl<CDBListCtrl, WTL::CListViewCtrl, CListCtrlTraits>::SubclassWindow(hWnd);
    if (bSuccessful)
    {
        ModifyStyle(0, CListCtrlTraits::GetWndStyle(0));
        SetExtendedListViewStyle(CListCtrlTraits::GetWndExStyle(GetExtendedListViewStyle()));
    }

    return bSuccessful;
}


///////////////////////////////////////////////////////////////////////////////
// CDBTreeCtrl 类的实现
//

inline CDBTreeCtrl::CDBTreeCtrl()
{
}

inline BOOL CDBTreeCtrl::SubclassWindow(HWND hWnd)
{
    BOOL bSuccessful = ATL::CWindowImpl<CDBTreeCtrl, WTL::CTreeViewCtrl, CTreeCtrlTraits>::SubclassWindow(hWnd);
    if (bSuccessful)
        ModifyStyle(0, CTreeCtrlTraits::GetWndStyle(0));

    return bSuccessful;
}

}  // namespace atlctrls

#endif  // __ATLCOMCTRLX_INL__