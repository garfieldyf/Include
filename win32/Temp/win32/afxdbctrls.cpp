///////////////////////////////////////////////////////////////////////////////
// afxdbctrls.cpp : 实现文件
//
// Copyright(c) 2012, Garfield. All rights reserved.
// Author  : Garfield
// Version : 2.0
// Finish Time : 2007/6/15

#include "stdafx.h"
#include "afxdbctrls.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace afxctrls {

///////////////////////////////////////////////////////////////////////////////
// CDBComboBox 类的实现
//

IMPLEMENT_DYNAMIC(CDBComboBox, CComboBox)

BEGIN_MESSAGE_MAP(CDBComboBox, CComboBox)
    ON_WM_DESTROY()
    ON_MESSAGE(CB_RESETCONTENT, OnResetContent)
    ON_MESSAGE(CB_DELETESTRING, OnDeleteString)
END_MESSAGE_MAP()


///////////////////////////////////////////////////////////////////////////////
// CDBListBox 类的实现
//

IMPLEMENT_DYNAMIC(CDBListBox, CListBox)

BEGIN_MESSAGE_MAP(CDBListBox, CListBox)
    ON_WM_DESTROY()
    ON_MESSAGE(LB_RESETCONTENT, OnResetContent)
    ON_MESSAGE(LB_DELETESTRING, OnDeleteString)
END_MESSAGE_MAP()


///////////////////////////////////////////////////////////////////////////////
// CDBListCtrl 类的实现
//

IMPLEMENT_DYNAMIC(CDBListCtrl, CListCtrl)

BEGIN_MESSAGE_MAP(CDBListCtrl, CListCtrl)
    ON_WM_CREATE()
    ON_WM_DESTROY()
    ON_MESSAGE(LVM_INSERTITEM, OnInsertItem)
    ON_MESSAGE(LVM_DELETEITEM, OnDeleteItem)
    ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomDraw)
    ON_NOTIFY_REFLECT(LVN_GETDISPINFO, OnGetDispInfo)
    ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnColumnClick)
END_MESSAGE_MAP()

// CDBListCtrl 消息处理程序

LRESULT CDBListCtrl::OnInsertItem(WPARAM wParam, LPARAM lParam)
{
    ASSERT(m_spRecordset != NULL);

    LPLVITEM pItem = reinterpret_cast<LPLVITEM>(lParam);
    ASSERT(pItem);

    pItem->iItem = GetItemCount();
    LRESULT lResult = DefWindowProc(LVM_INSERTITEM, wParam, lParam);
    if (lResult != -1)
    {
        VARIANT_BOOL bEOF = VARIANT_TRUE;
        if (SUCCEEDED(m_spRecordset->get_EOF(&bEOF)) && !bEOF)
            _COM_VERIFY(m_spRecordset->MoveLast());

        CComVariant vtEmpty;
        _COM_VERIFY(m_spRecordset->AddNew(vtEmpty, vtEmpty));
        _UpdateItem(m_hWnd, pItem->iItem);
    }

    return lResult;
}

void CDBListCtrl::OnColumnClick(NMHDR* pNMHDR, LRESULT* pResult)
{
    LPNMLISTVIEW pNMListView = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
    ASSERT(pNMListView);
    ASSERT(m_spRecordset != NULL);
    ASSERT((pNMListView->iSubItem >= 0 && pNMListView->iSubItem < GetHeaderCtrl()->GetItemCount()));

    if (GetItemCount() > 0)
    {
        TCHAR szBuffer[MAX_PATH];
        HDITEM hdItem  = { HDI_TEXT };
        hdItem.pszText = szBuffer;
        hdItem.cchTextMax = _countof(szBuffer);
        VERIFY(GetHeaderCtrl()->GetItem(pNMListView->iSubItem, &hdItem));

        // 排序
        BOOL bAscending = (pNMListView->iSubItem == m_nSortColumn ? !m_bAscending : TRUE);
        ::lstrcat(szBuffer, (bAscending ? _T(" ASC") : _T(" DESC")));

        if (SUCCEEDED(m_spRecordset->put_Sort(CComBSTR(szBuffer))))
        {
            SetSortFlags(m_hWnd, pNMListView->iSubItem, bAscending);
            Invalidate(TRUE);
        }
    }

    *pResult = 0L;
}


///////////////////////////////////////////////////////////////////////////////
// CDBTreeCtrl 类的实现
//

IMPLEMENT_DYNAMIC(CDBTreeCtrl, CTreeCtrl)

BEGIN_MESSAGE_MAP(CDBTreeCtrl, CTreeCtrl)
    ON_NOTIFY_REFLECT(TVN_DELETEITEM, OnDeleteItem)
END_MESSAGE_MAP()

}  // namespace afxctrls