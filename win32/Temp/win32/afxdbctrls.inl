///////////////////////////////////////////////////////////////////////////////
// afxdbctrls.inl : 内联文件
//
// Copyright(c) 2012, Garfield. All rights reserved.
// Author  : Garfield
// Version : 2.0
// Finish Time : 2007/6/15

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __AFXDBCTRLS_INL__
#define __AFXDBCTRLS_INL__

namespace afxctrls {

///////////////////////////////////////////////////////////////////////////////
// CDBComboBox 类的实现
//

inline CDBComboBox::CDBComboBox()
{
}

inline CDBComboBox::~CDBComboBox()
{
}

inline HRESULT CDBComboBox::DataBind(_Connection* pConnection, BSTR bstrCommandText, const VARIANT& vtValueIndex, const VARIANT* pTextIndex/* = NULL*/, long lOptions/* = adCmdText*/)
{
    return _DataBind(m_hWnd, pConnection, bstrCommandText, vtValueIndex, pTextIndex, lOptions);
}

inline const CComVariant* CDBComboBox::GetSelectedValue() const
{
    ASSERT_VALID(this);

    int nSelect = GetCurSel();
    return (nSelect != -1) ? (CComVariant*)GetItemData(nSelect) : NULL;
}

inline int CDBComboBox::SetSelectedValue(const VARIANT& vtValue)
{
    ASSERT_VALID(this);

    int nCount = GetCount();
    while (--nCount >= 0)
    {
        if (*(CComVariant*)GetItemData(nCount) == vtValue)
            break;
    }

    SetCurSel(nCount);
    return nCount;
}

inline void CDBComboBox::OnDestroy()
{
    _DestroyAllItems(m_hWnd);
    CComboBox::OnDestroy();
}

inline LRESULT CDBComboBox::OnResetContent(WPARAM wParam, LPARAM lParam)
{
    _DestroyAllItems(m_hWnd);
    return DefWindowProc(CB_RESETCONTENT, wParam, lParam);
}

inline LRESULT CDBComboBox::OnDeleteString(WPARAM wParam, LPARAM lParam)
{
    CComVariant* pValue = (CComVariant*)GetItemData((int)wParam);
    LRESULT lResult = DefWindowProc(CB_DELETESTRING, wParam, lParam);

    if (lResult != CB_ERR)
        delete pValue;

    return lResult;
}


///////////////////////////////////////////////////////////////////////////////
// CDBListBox 类的实现
//

inline CDBListBox::CDBListBox()
{
}

inline CDBListBox::~CDBListBox()
{
}

inline HRESULT CDBListBox::DataBind(_Connection* pConnection, BSTR bstrCommandText, const VARIANT& vtValueIndex, const VARIANT* pTextIndex/* = NULL*/, long lOptions/* = adCmdText*/)
{
    return _DataBind(m_hWnd, pConnection, bstrCommandText, vtValueIndex, pTextIndex, lOptions);
}

inline const CComVariant* CDBListBox::GetSelectedValue() const
{
    ASSERT_VALID(this);

    int nSelect = GetCurSel();
    return (nSelect != -1) ? (CComVariant*)GetItemData(nSelect) : NULL;
}

inline int CDBListBox::SetSelectedValue(const VARIANT& vtValue)
{
    ASSERT_VALID(this);

    int nCount = GetCount();
    while (--nCount >= 0)
    {
        if (*(CComVariant*)GetItemData(nCount) == vtValue)
            break;
    }

    SetCurSel(nCount);
    return nCount;
}

inline void CDBListBox::OnDestroy()
{
    _DestroyAllItems(m_hWnd);
    CListBox::OnDestroy();
}

inline LRESULT CDBListBox::OnResetContent(WPARAM wParam, LPARAM lParam)
{
    _DestroyAllItems(m_hWnd);
    return DefWindowProc(LB_RESETCONTENT, wParam, lParam);
}

inline LRESULT CDBListBox::OnDeleteString(WPARAM wParam, LPARAM lParam)
{
    CComVariant* pValue = (CComVariant*)GetItemData((int)wParam);
    LRESULT lResult = DefWindowProc(LB_DELETESTRING, wParam, lParam);

    if (lResult != LB_ERR)
        delete pValue;

    return lResult;
}


///////////////////////////////////////////////////////////////////////////////
// CDBListCtrl 类的实现
//

inline CDBListCtrl::CDBListCtrl()
{
}

inline CDBListCtrl::~CDBListCtrl()
{
}

inline void CDBListCtrl::PreSubclassWindow()
{
    ModifyStyle(0, LVS_REPORT);
    SetExtendedStyle(GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_INFOTIP);

    CListCtrl::PreSubclassWindow();
}

inline int CDBListCtrl::UpdateItem(int nItem)
{
    return _UpdateItem(m_hWnd, nItem);
}

inline HRESULT CDBListCtrl::DataBind(_Connection* pConnection, const VARIANT& vtSource, int nColumnWidth, int nFormat/* = LVCFMT_CENTER*/, int nBeginColumn/* = 0*/,
                                     int nEndColumn/* = -1*/, long lOptions/* = adCmdText*/, BOOL bDeleteAllColumns/* = FALSE*/)
{
    return _DataBind(m_hWnd, pConnection, vtSource, nColumnWidth, nFormat, nBeginColumn, nEndColumn, lOptions, bDeleteAllColumns);
}

inline void CDBListCtrl::OnDestroy()
{
    _COM_VERIFY(Close());
    CListCtrl::OnDestroy();
}

inline int CDBListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CListCtrl::OnCreate(lpCreateStruct) == -1)
        return -1;

    ModifyStyle(0, LVS_REPORT);
    SetExtendedStyle(GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_INFOTIP);

    return 0;
}

inline LRESULT CDBListCtrl::OnDeleteItem(WPARAM wParam, LPARAM lParam)
{
    ASSERT(m_spRecordset != NULL);

    LRESULT lResult = DefWindowProc(LVM_DELETEITEM, wParam, lParam);
    if (lResult)
    {
        _COM_VERIFY(m_spRecordset->Move((ADO_LONGPTR)wParam, CComVariant(adBookmarkFirst)));
        _COM_VERIFY(m_spRecordset->Delete(adAffectCurrent));
    }

    return lResult;
}

inline void CDBListCtrl::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult)
{
    *pResult = CustomDraw(reinterpret_cast<LPNMLVCUSTOMDRAW>(pNMHDR));
}

inline void CDBListCtrl::OnGetDispInfo(NMHDR* pNMHDR, LRESULT* pResult)
{
    *pResult = _GetDispInfo(reinterpret_cast<NMLVDISPINFO*>(pNMHDR));
}


///////////////////////////////////////////////////////////////////////////////
// CDBListCtrl 类的实现
//

inline CDBTreeCtrl::CDBTreeCtrl()
{
}

inline CDBTreeCtrl::~CDBTreeCtrl()
{
}

inline void CDBTreeCtrl::PreSubclassWindow()
{
    ModifyStyle(0, TVS_INFOTIP);
    CTreeCtrl::PreSubclassWindow();
}

inline HRESULT CDBTreeCtrl::DataBind(_Connection* pConnection, BSTR bstrCommandText, const VARIANT& vtParentIndex, const VARIANT& vtValueIndex,
    const VARIANT* pTextIndex/* = NULL*/, const VARIANT* pImageIndex/* = NULL*/, const VARIANT* pSelImageIndex/* = NULL*/, long lOptions/* = adCmdText*/, BOOL bExpanding/* = TRUE*/)
{
    stdutil::CTreeNodeList nodeList;
    HRESULT hr = nodeList.DataBind(pConnection, bstrCommandText, vtParentIndex, vtValueIndex, pTextIndex, pImageIndex, pSelImageIndex, lOptions);
    if (SUCCEEDED(hr))
        InsertRoot(m_hWnd, nodeList, bExpanding);

    return hr;
}

inline const CComVariant* CDBTreeCtrl::GetSelectedValue() const
{
    HTREEITEM hItem = GetSelectedItem();
    return (hItem != NULL) ? (CComVariant*)GetItemData(hItem) : NULL;
}

inline void CDBTreeCtrl::SetSelectedValue(const VARIANT& vtValue)
{
    if (HTREEITEM hItem = FindItem(GetRootItem(), vtValue))
        SelectItem(hItem);
}

inline HTREEITEM CDBTreeCtrl::FindItem(HTREEITEM hStart, const VARIANT& vtFind)
{
    if (hStart != NULL)
    {
        if (*(CComVariant*)GetItemData(hStart) != vtFind)
        {
            if (HTREEITEM hFind = FindItem(GetChildItem(hStart), vtFind))
                return hFind;

            return FindItem(GetNextSiblingItem(hStart), vtFind);
        }
    }

    return hStart;
}

inline void CDBTreeCtrl::OnDeleteItem(NMHDR* pNMHDR, LRESULT* pResult)
{
    LPNMTREEVIEW pNMTV = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
    ASSERT(pNMTV);

    delete (CComVariant*)pNMTV->itemOld.lParam;
    *pResult = 0;
}

}  // namespace afxctrls

#endif  // __AFXDBCTRLS_INL__