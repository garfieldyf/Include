///////////////////////////////////////////////////////////////////////////////
// xtpext.inl : 内联文件
//
// Copyright(c) 2012, Garfield. All rights reserved.
// Author  : Garfield
// Version : 1.0
// Finish Time : 2007/10/24

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __XTPEXT_INL__
#define __XTPEXT_INL__

namespace XTP {

///////////////////////////////////////////////////////////////////////////////
// CXTPPropertyGridItemIcon 内联函数
//

inline CXTPPropertyGridItemIcon::CXTPPropertyGridItemIcon(UINT nID, HICON hIcon/* = NULL*/, UINT nDlgFilter/* = 0*/)
    : CXTPPropertyGridItem(nID), m_hIcon(::CopyIcon(hIcon))
{
    m_nFlags = pgiHasExpandButton;
    m_strFilter.LoadString(nDlgFilter);
}

inline CXTPPropertyGridItemIcon::CXTPPropertyGridItemIcon(LPCTSTR pszCaption, HICON hIcon/* = NULL*/, LPCTSTR pszDlgFilter/* = NULL*/)
    : CXTPPropertyGridItem(pszCaption), m_hIcon(::CopyIcon(hIcon)), m_strFilter(pszDlgFilter)
{
    m_nFlags = pgiHasExpandButton;
}

inline CXTPPropertyGridItemIcon::~CXTPPropertyGridItemIcon()
{
    ::DestroyIcon(m_hIcon);
}

inline HICON CXTPPropertyGridItemIcon::GetIcon() const
{
    ASSERT_VALID(this);
    return ::CopyIcon(m_hIcon);
}

inline void CXTPPropertyGridItemIcon::SetIcon(HICON hIcon)
{
    _ASSERTE(hIcon);
    ASSERT_VALID(this);

    m_hIcon = ::CopyIcon(hIcon);
}

inline void CXTPPropertyGridItemIcon::SetDialogFilter(UINT nID)
{
    ASSERT_VALID(this);
    m_strFilter.LoadString(nID);
}

inline void CXTPPropertyGridItemIcon::SetDialogFilter(LPCTSTR pszFilter)
{
    ASSERT_VALID(this);
    _ASSERTE(pszFilter);

    m_strFilter = pszFilter;
}

inline void CXTPPropertyGridItemIcon::OnInplaceButtonDown()
{
    ASSERT_VALID(this);

    CFileDialog dlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, m_strFilter);
    if (dlg.DoModal() == IDOK)
    {
        ::DestroyIcon(m_hIcon);
        m_hIcon = reinterpret_cast<HICON>(::LoadImage(NULL, dlg.GetPathName(), IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE));

        OnValueChanged(_T(""));
        _ASSERTE(m_pGrid);
        m_pGrid->Invalidate(FALSE);
    }
}

inline BOOL CXTPPropertyGridItemIcon::OnDrawItemValue(CDC& dc, CRect rcValue)
{
    ASSERT(dc.m_hDC);
    ASSERT_VALID(this);

    CRect rcIcon(rcValue.left - 2, rcValue.top + 1, rcValue.left + 18, rcValue.bottom - 1);
    if (m_hIcon != NULL)
        ::DrawIconEx(dc.m_hDC, rcIcon.left, rcIcon.top, m_hIcon, rcIcon.Width(), rcIcon.Height(), 0, 0, DI_NORMAL);

    dc.Draw3dRect(&rcIcon, RGB(0, 0, 0), RGB(0, 0, 0));
    rcValue.DeflateRect(22, 0, 0, 0);
    dc.DrawText(_T("(Icon)"), &rcValue, DT_SINGLELINE | DT_VCENTER);

    return TRUE;
}


///////////////////////////////////////////////////////////////////////////////
// CXTPPropertyGridItemFolder 内联函数
//

inline CXTPPropertyGridItemFolder::CXTPPropertyGridItemFolder(UINT nID, LPCTSTR pszValue/* = NULL*/, CString* pBindString/* = NULL*/, UINT nDlgTitle/* = 0*/)
    : CXTPPropertyGridItem(nID, pszValue, pBindString)
{
    m_nFlags = pgiHasEdit | pgiHasExpandButton;
    m_strTitle.LoadString(nDlgTitle);
}

inline CXTPPropertyGridItemFolder::CXTPPropertyGridItemFolder(LPCTSTR pszCaption, LPCTSTR pszValue/* = NULL*/, CString* pBindString/* = NULL*/, LPCTSTR pszDlgTitle/* = NULL*/)
    : CXTPPropertyGridItem(pszCaption, pszValue, pBindString), m_strTitle(pszDlgTitle)
{
    m_nFlags = pgiHasEdit | pgiHasExpandButton;
}

inline CXTPPropertyGridItemFolder::~CXTPPropertyGridItemFolder()
{
}

inline void CXTPPropertyGridItemFolder::SetDialogTitle(UINT nID)
{
    ASSERT_VALID(this);
    m_strTitle.LoadString(nID);
}

inline void CXTPPropertyGridItemFolder::SetDialogTitle(LPCTSTR pszTitle)
{
    ASSERT_VALID(this);
    m_strTitle = pszTitle;
}

inline void CXTPPropertyGridItemFolder::OnInplaceButtonDown()
{
    _ASSERTE(m_pGrid);
    ASSERT_VALID(this);

    CXTBrowseDialog dlg(m_pGrid);
    dlg.SetTitle(m_strTitle.GetBuffer());
    dlg.SetOptions(BIF_NEWDIALOGSTYLE | BIF_RETURNONLYFSDIRS);

    if (dlg.DoModal() == IDOK)
    {
        OnValueChanged(dlg.GetSelPath());
        m_pGrid->Invalidate(TRUE);
    }
}


///////////////////////////////////////////////////////////////////////////////
// CXTPPropertyGridItemFile 内联函数
//

inline CXTPPropertyGridItemFile::CXTPPropertyGridItemFile(UINT nID, LPCTSTR pszValue/* = NULL*/, CString* pBindString/* = NULL*/, UINT nDlgFilter/* = 0*/)
    : CXTPPropertyGridItem(nID, pszValue, pBindString)
{
    m_nFlags = pgiHasEdit | pgiHasExpandButton;
    m_strFilter.LoadString(nDlgFilter);
}

inline CXTPPropertyGridItemFile::CXTPPropertyGridItemFile(LPCTSTR pszCaption, LPCTSTR pszValue/* = NULL*/, CString* pBindString/* = NULL*/, LPCTSTR pszDlgFilter/* = NULL*/)
    : CXTPPropertyGridItem(pszCaption, pszValue, pBindString), m_strFilter(pszDlgFilter)
{
    m_nFlags = pgiHasEdit | pgiHasExpandButton;
}

inline CXTPPropertyGridItemFile::~CXTPPropertyGridItemFile()
{
}

inline void CXTPPropertyGridItemFile::SetDialogFilter(UINT nID)
{
    ASSERT_VALID(this);
    m_strFilter.LoadString(nID);
}

inline void CXTPPropertyGridItemFile::SetDialogFilter(LPCTSTR pszFilter)
{
    ASSERT_VALID(this);
    m_strFilter = pszFilter;
}

inline void CXTPPropertyGridItemFile::OnInplaceButtonDown()
{
    _ASSERTE(m_pGrid);
    ASSERT_VALID(this);

    CFileDialog dlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, m_strFilter);
    if (dlg.DoModal() == IDOK)
    {
        OnValueChanged(dlg.GetPathName());
        m_pGrid->Invalidate(FALSE);
    }
}


///////////////////////////////////////////////////////////////////////////////
// CXTPPropertyGridItemRect 内联函数
//

inline CXTPPropertyGridItemRect::CXTPPropertyGridItemRect(UINT nID, LPRECT lpRect/* = NULL*/, LPRECT lpBindRect/* = NULL*/)
    : CXTPPropertyGridItem(nID), m_rcValue(0, 0, 0, 0), m_pBindRect(lpBindRect), m_pItemAll(NULL)
    , m_pItemLeft(NULL), m_pItemTop(NULL), m_pItemRight(NULL), m_pItemBottom(NULL)
{
    if (lpRect != NULL)
        m_rcValue = *lpRect;

    if (m_pBindRect != NULL)
        *m_pBindRect = m_rcValue;

    m_strValue = RectToString(m_rcValue);
}

inline CXTPPropertyGridItemRect::CXTPPropertyGridItemRect(LPCTSTR pszCaption, LPRECT lpRect/* = NULL*/, LPRECT lpBindRect/* = NULL*/)
    : CXTPPropertyGridItem(pszCaption), m_rcValue(0, 0, 0, 0), m_pBindRect(lpBindRect), m_pItemAll(NULL)
    , m_pItemLeft(NULL), m_pItemTop(NULL), m_pItemRight(NULL), m_pItemBottom(NULL)
{
    if (lpRect != NULL)
        m_rcValue = *lpRect;

    if (m_pBindRect != NULL)
        *m_pBindRect = m_rcValue;

    m_strValue = RectToString(m_rcValue);
}

inline CXTPPropertyGridItemRect::~CXTPPropertyGridItemRect()
{
}

inline const RECT& CXTPPropertyGridItemRect::GetRect() const
{
    ASSERT_VALID(this);
    return m_rcValue;
}

inline void CXTPPropertyGridItemRect::SetRect(const RECT& rect)
{
    SetRect(rect.left, rect.top, rect.right, rect.bottom);
}

inline void CXTPPropertyGridItemRect::SetRect(int left, int top, int right, int bottom)
{
    ASSERT_VALID(this);

    m_rcValue.SetRect(left, top, right, bottom);
    if (m_pBindRect != NULL)
        *m_pBindRect = m_rcValue;

    CXTPPropertyGridItem::SetValue(RectToString(m_rcValue));
    UpdateChilds();
}

inline void CXTPPropertyGridItemRect::BindToRect(LPRECT lpRect)
{
    ASSERT_VALID(this);

    m_pBindRect = lpRect;
    if (m_pBindRect != NULL)
        *m_pBindRect = m_rcValue;
}

inline void CXTPPropertyGridItemRect::OnAddChildItem()
{
    ASSERT_VALID(this);

    m_pItemAll    = static_cast<CXTPPropertyGridItemAll*>(AddChildItem(new CXTPPropertyGridItemAll(_T("All"))));
    m_pItemLeft   = static_cast<CXTPPropertyGridItemPad*>(AddChildItem(new CXTPPropertyGridItemPad(_T("Left"), &m_rcValue.left)));
    m_pItemTop    = static_cast<CXTPPropertyGridItemPad*>(AddChildItem(new CXTPPropertyGridItemPad(_T("Top"), &m_rcValue.top)));
    m_pItemRight  = static_cast<CXTPPropertyGridItemPad*>(AddChildItem(new CXTPPropertyGridItemPad(_T("Right"), &m_rcValue.right)));
    m_pItemBottom = static_cast<CXTPPropertyGridItemPad*>(AddChildItem(new CXTPPropertyGridItemPad(_T("Bottom"), &m_rcValue.bottom)));

    UpdateChilds();
}

inline void CXTPPropertyGridItemRect::SetValue(CString strValue)
{
    SetRect(StringToRect(strValue));
}

inline void CXTPPropertyGridItemRect::UpdateChilds()
{
    ASSERT_VALID(this);

    m_pItemLeft->SetNumber(m_rcValue.left);
    m_pItemTop->SetNumber(m_rcValue.top);
    m_pItemRight->SetNumber(m_rcValue.right);
    m_pItemBottom->SetNumber(m_rcValue.bottom);
}

inline RECT CXTPPropertyGridItemRect::StringToRect(LPCTSTR pszValue)
{
    ASSERT_VALID(this);

    CString strLeft, strTop, strRight, strBottom;
    AfxExtractSubString(strLeft, pszValue, 0, ';');
    AfxExtractSubString(strTop, pszValue, 1, ';');
    AfxExtractSubString(strRight, pszValue, 2, ';');
    AfxExtractSubString(strBottom, pszValue, 3, ';');

    RECT rect = { ::_ttol(strLeft), ::_ttol(strTop), ::_ttol(strRight), ::_ttol(strBottom) };
    return rect;
}

inline CString CXTPPropertyGridItemRect::RectToString(const RECT& rect)
{
    ASSERT_VALID(this);

    CString strValue;
    strValue.Format(_T("%d; %d; %d; %d"), rect.left, rect.top, rect.right, rect.bottom);

    return strValue;
}


///////////////////////////////////////////////////////////////////////////////
// CXTPPropertyGridItemPad 内联函数
//

inline CXTPPropertyGridItemRect::CXTPPropertyGridItemPad::CXTPPropertyGridItemPad(LPCTSTR pszCaption, long* pBindNumber)
    : CXTPPropertyGridItemNumber(pszCaption, *pBindNumber, pBindNumber)
{
}

inline void CXTPPropertyGridItemRect::CXTPPropertyGridItemPad::OnValueChanged(CString strValue)
{
    SetValue(strValue);
    *m_pBindNumber = m_nValue;

    CXTPPropertyGridItemRect* pParent = static_cast<CXTPPropertyGridItemRect*>(m_pParent);
    _ASSERTE(pParent);

    pParent->m_pItemAll->SetNumber(0);
    pParent->OnValueChanged(pParent->RectToString(pParent->m_rcValue));
}


///////////////////////////////////////////////////////////////////////////////
// CXTPPropertyGridItemAll 内联函数
//

inline CXTPPropertyGridItemRect::CXTPPropertyGridItemAll::CXTPPropertyGridItemAll(LPCTSTR pszCaption)
    : CXTPPropertyGridItemNumber(pszCaption)
{
}

inline void CXTPPropertyGridItemRect::CXTPPropertyGridItemAll::OnValueChanged(CString strValue)
{
    SetValue(strValue);

    CXTPPropertyGridItemRect* pParent = static_cast<CXTPPropertyGridItemRect*>(m_pParent);
    _ASSERTE(pParent);

    pParent->m_rcValue.left = pParent->m_rcValue.top = pParent->m_rcValue.right = pParent->m_rcValue.bottom = m_nValue;
    pParent->OnValueChanged(pParent->RectToString(pParent->m_rcValue));
}


///////////////////////////////////////////////////////////////////////////////
// CXTPPropertyGridItemSpin 内联函数
//

inline CXTPPropertyGridItemSpin::CXTPPropertyGridItemSpin(UINT nID, long lValue/* = 0*/, long* pBindValue/* = NULL*/, int nLower/* = 0*/, int nUpper/* = 100*/)
    : CXTPPropertyGridItemNumber(nID, lValue, pBindValue), m_nLower(nLower), m_nUpper(nUpper)
{
    m_wndSpinButton.m_pItem = this;
}

inline CXTPPropertyGridItemSpin::CXTPPropertyGridItemSpin(LPCTSTR pszCaption, long lValue/* = 0*/, long* pBindValue/* = NULL*/, int nLower/* = 0*/, int nUpper/* = 100*/)
    : CXTPPropertyGridItemNumber(pszCaption, lValue, pBindValue), m_nLower(nLower), m_nUpper(nUpper)
{
    m_wndSpinButton.m_pItem = this;
}

inline CXTPPropertyGridItemSpin::~CXTPPropertyGridItemSpin()
{
}

inline int CXTPPropertyGridItemSpin::GetRange(int& nLower) const
{
    ASSERT_VALID(this);

    nLower = m_nLower;
    return m_nUpper;
}

inline void CXTPPropertyGridItemSpin::SetRange(int nLower, int nUpper)
{
    ASSERT_VALID(this);

    m_nLower = nLower;
    m_nUpper = nUpper;
}

inline void CXTPPropertyGridItemSpin::OnSelect()
{
    ASSERT_VALID(this);
    CXTPPropertyGridItemNumber::OnSelect();

    if (!m_bReadOnly)
    {
        CRect rcItem = GetItemRect();
        rcItem.left  = rcItem.right - 15;

        if (!::IsWindow(m_wndSpinButton.m_hWnd))
        {
            _ASSERTE(m_pGrid);
            m_wndSpinButton.Create(UDS_ARROWKEYS | WS_CHILD, rcItem, m_pGrid, 0);
            m_wndSpinButton.SetRange32(m_nLower, m_nUpper);
        }

        m_wndSpinButton.MoveWindow(&rcItem);
        m_wndSpinButton.ShowWindow(SW_SHOW);
    }
}

inline void CXTPPropertyGridItemSpin::OnDeselect()
{
    ASSERT_VALID(this);
    CXTPPropertyGridItemNumber::OnDeselect();

    if (::IsWindow(m_wndSpinButton.m_hWnd))
        m_wndSpinButton.ShowWindow(SW_HIDE);
}

inline CRect CXTPPropertyGridItemSpin::GetValueRect()
{
    ASSERT_VALID(this);

    CRect rcValue(CXTPPropertyGridItemNumber::GetValueRect());
    rcValue.right -= 17;

    return rcValue;
}

inline void CXTPPropertyGridItemSpin::SetValue(CString strValue)
{
    ASSERT_VALID(this);

    long lValue = ::_ttol(strValue);
    SetNumber((lValue < m_nLower ? m_nLower : (lValue > m_nUpper ? m_nUpper : lValue)));
}


///////////////////////////////////////////////////////////////////////////////
// CInplaceSpinButton 内联函数
//

inline CXTPPropertyGridItemSpin::CInplaceSpinButton::CInplaceSpinButton()
    : m_pItem(NULL)
{
}

inline void CXTPPropertyGridItemSpin::CInplaceSpinButton::OnDelTapos(NMHDR* pNMHDR, LRESULT* pResult)
{
    LPNMUPDOWN pNMUD = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
    _ASSERTE(pNMUD);
    _ASSERTE(m_pItem);
    ASSERT_VALID(this);
    *pResult = 1;

    int nValue = m_pItem->m_nValue + pNMUD->iDelta;
    m_pItem->m_nValue = (nValue < m_pItem->m_nLower ? m_pItem->m_nLower : (nValue > m_pItem->m_nUpper ? m_pItem->m_nUpper : nValue));

    CString strNumber;
    strNumber.Format(_T("%d"), m_pItem->m_nValue);

    m_pItem->OnValidateEdit();
    m_pItem->OnValueChanged(strNumber);
}

}  // namespace XTP

#endif  // __XTPEXT_INL__