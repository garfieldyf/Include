///////////////////////////////////////////////////////////////////////////////
// atldbctrls.inl : 实现文件
//
// Copyright(c) 2012, Garfield. All rights reserved.
// Author  : Garfield
// Version : 3.0
// Finish Time : 2007/7/15

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __ATLDBCTRLS_INL__
#define __ATLDBCTRLS_INL__

namespace atlctrls {

///////////////////////////////////////////////////////////////////////////////
// CDBControlImpl 类的实现
//

template <typename T, typename TCtrlTraits>
inline CDBControlImpl<T, TCtrlTraits>::CDBControlImpl()
{
}

template <typename T, typename TCtrlTraits>
inline HRESULT CDBControlImpl<T, TCtrlTraits>::DataBind(_Connection* pConnection, BSTR bstrCommandText, const VARIANT& vtValueIndex, const VARIANT* pTextIndex/* = NULL*/, long lOptions/* = adCmdText*/)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);
    ATLASSERT(::IsWindow(pThis->m_hWnd));

    return _DataBind(pThis->m_hWnd, pConnection, bstrCommandText, vtValueIndex, pTextIndex, lOptions);
}

template <typename T, typename TCtrlTraits>
inline const CComVariant* CDBControlImpl<T, TCtrlTraits>::GetSelectedValue() const
{
    const T* pThis = static_cast<const T*>(this);
    ATLASSERT(pThis);
    ATLASSERT(::IsWindow(pThis->m_hWnd));

    int nSelect = TCtrlTraits::GetCurSel(pThis->m_hWnd);
    return (nSelect != -1) ? TCtrlTraits::GetItemData(pThis->m_hWnd, nSelect) : NULL;
}

template <typename T, typename TCtrlTraits>
inline int CDBControlImpl<T, TCtrlTraits>::SetSelectedValue(const VARIANT& vtValue)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);
    ATLASSERT(::IsWindow(pThis->m_hWnd));

    int nCount = TCtrlTraits::GetCount(pThis->m_hWnd);
    while (--nCount >= 0)
    {
        if ((*TCtrlTraits::GetItemData(pThis->m_hWnd, nCount)) == vtValue)
            break;
    }

    TCtrlTraits::SetCurSel(pThis->m_hWnd, nCount);
    return nCount;
}

template <typename T, typename TCtrlTraits>
inline LRESULT CDBControlImpl<T, TCtrlTraits>::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);
    ATLASSERT(::IsWindow(pThis->m_hWnd));

    bHandled = FALSE;
    _DestroyAllItems(pThis->m_hWnd);

    return 0L;
}

template <typename T, typename TCtrlTraits>
inline LRESULT CDBControlImpl<T, TCtrlTraits>::OnResetContent(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);
    ATLASSERT(::IsWindow(pThis->m_hWnd));

    _DestroyAllItems(pThis->m_hWnd);
    return pThis->DefWindowProc(uMsg, wParam, lParam);
}

template <typename T, typename TCtrlTraits>
inline LRESULT CDBControlImpl<T, TCtrlTraits>::OnDeleteString(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);
    ATLASSERT(::IsWindow(pThis->m_hWnd));

    CComVariant* pValue = TCtrlTraits::GetItemData(pThis->m_hWnd, wParam);
    LRESULT lResult = pThis->DefWindowProc(uMsg, wParam, lParam);

    if (lResult != -1)
        delete pValue;

    return lResult;
}


///////////////////////////////////////////////////////////////////////////////
// CDBListCtrlImpl 类的实现
//

template <typename T>
inline CDBListCtrlImpl<T>::CDBListCtrlImpl()
{
}

template <typename T>
inline int CDBListCtrlImpl<T>::UpdateItem(int nItem)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);
    ATLASSERT(::IsWindow(pThis->m_hWnd));

    return _UpdateItem(pThis->m_hWnd, nItem);
}

template <typename T>
inline HRESULT CDBListCtrlImpl<T>::DataBind(_Connection* pConnection, const VARIANT& vtSource, int nColumnWidth, int nFormat/* = LVCFMT_CENTER*/,
                                            int nBeginColumn/* = 0*/, int nEndColumn/* = -1*/, long lOptions/* = adCmdText*/, BOOL bDeleteAllColumns/* = FALSE*/)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);
    ATLASSERT(::IsWindow(pThis->m_hWnd));

    return _DataBind(pThis->m_hWnd, pConnection, vtSource, nColumnWidth, nFormat, nBeginColumn, nEndColumn, lOptions, bDeleteAllColumns);
}

template <typename T>
inline LRESULT CDBListCtrlImpl<T>::OnColumnClick(int /*nIDCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/)
{
    LPNMLISTVIEW pNMListView = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
    ATLASSERT(pNMListView);
    ATLASSERT(m_spRecordset != NULL);
    ATLASSERT(pNMListView->iSubItem >= 0 && pNMListView->iSubItem < Header_GetItemCount(ListView_GetHeader(pNMListView->hdr.hwndFrom)));

    if (ListView_GetItemCount(pNMListView->hdr.hwndFrom) > 0)
    {
        TCHAR szBuffer[MAX_PATH];
        HDITEM hdItem  = { HDI_TEXT };
        hdItem.pszText = szBuffer;
        hdItem.cchTextMax = _countof(szBuffer);
        ATLVERIFY(Header_GetItem(ListView_GetHeader(pNMListView->hdr.hwndFrom), pNMListView->iSubItem, &hdItem));

        // 排序
        BOOL bAscending = (pNMListView->iSubItem == m_nSortCol ? !m_bAscending : TRUE);
        ::lstrcat(szBuffer, (bAscending ? _T(" ASC") : _T(" DESC")));

        if (SUCCEEDED(m_spRecordset->put_Sort(CComBSTR(szBuffer))))
        {
            SetSortCol(pNMListView->hdr.hwndFrom, pNMListView->iSubItem, bAscending);
            ::InvalidateRect(pNMListView->hdr.hwndFrom, NULL, TRUE);
        }
    }

    return 0L;
}

template <typename T>
inline LRESULT CDBListCtrlImpl<T>::OnGetDispInfo(int /*nIDCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/)
{
    return _GetDispInfo(reinterpret_cast<NMLVDISPINFO*>(pNMHDR));
}

template <typename T>
inline LRESULT CDBListCtrlImpl<T>::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
    bHandled = FALSE;
    _COM_VERIFY(Close());

    return 0L;
}

template <typename T>
inline LRESULT CDBListCtrlImpl<T>::OnInsertItem(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);
    ATLASSERT(m_spRecordset != NULL);
    ATLASSERT(::IsWindow(pThis->m_hWnd));

    LPLVITEM pItem = reinterpret_cast<LPLVITEM>(lParam);
    ATLASSERT(pItem);

    pItem->iItem = ListView_GetItemCount(pThis->m_hWnd);
    LRESULT lResult = pThis->DefWindowProc(uMsg, wParam, lParam);
    if (lResult != -1)
    {
        VARIANT_BOOL bEOF = VARIANT_TRUE;
        if (SUCCEEDED(m_spRecordset->get_EOF(&bEOF)) && !bEOF)
            _COM_VERIFY(m_spRecordset->MoveLast());

        CComVariant vtEmpty;
        _COM_VERIFY(m_spRecordset->AddNew(vtEmpty, vtEmpty));
        _UpdateItem(pThis->m_hWnd, pItem->iItem);
    }

    return lResult;
}

template <typename T>
inline LRESULT CDBListCtrlImpl<T>::OnDeleteItem(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);
    ATLASSERT(m_spRecordset != NULL);
    ATLASSERT(::IsWindow(pThis->m_hWnd));

    LRESULT lResult = pThis->DefWindowProc(uMsg, wParam, lParam);
    if (lResult)
    {
        _COM_VERIFY(m_spRecordset->Move((ADO_LONGPTR)wParam, CComVariant(adBookmarkFirst)));
        _COM_VERIFY(m_spRecordset->Delete(adAffectCurrent));
    }

    return lResult;
}


///////////////////////////////////////////////////////////////////////////////
// CDBTreeCtrlImpl 类的实现
//

template <typename T>
inline CDBTreeCtrlImpl<T>::CDBTreeCtrlImpl()
{
}

template <typename T>
inline HRESULT CDBTreeCtrlImpl<T>::DataBind(_Connection* pConnection, BSTR bstrCommandText, const VARIANT& vtParentIndex, const VARIANT& vtValueIndex,
    const VARIANT* pTextIndex/* = NULL*/, const VARIANT* pImageIndex/* = NULL*/, const VARIANT* pSelImageIndex/* = NULL*/, long lOptions/* = adCmdText*/, BOOL bExpanding/* = TRUE*/)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);
    ATLASSERT(::IsWindow(pThis->m_hWnd));

    stdutil::CTreeNodeList nodeList;
    HRESULT hr = nodeList.DataBind(pConnection, bstrCommandText, vtParentIndex, vtValueIndex, pTextIndex, pImageIndex, pSelImageIndex, lOptions);
    if (SUCCEEDED(hr))
        InsertRoot(pThis->m_hWnd, nodeList, bExpanding);

    return hr;
}

template <typename T>
inline const CComVariant* CDBTreeCtrlImpl<T>::GetSelectedValue() const
{
    const T* pThis = static_cast<const T*>(this);
    ATLASSERT(pThis);
    ATLASSERT(::IsWindow(pThis->m_hWnd));

    CComVariant* pValue = NULL;
    if (HTREEITEM hItem = TreeView_GetSelection(pThis->m_hWnd))
    {
        TVITEM item = { TVIF_PARAM, hItem };
        ATLVERIFY(TreeView_GetItem(pThis->m_hWnd, &item));

        pValue = (CComVariant*)item.lParam;
    }

    return pValue;
}

template <typename T>
inline void CDBTreeCtrlImpl<T>::SetSelectedValue(const VARIANT& vtValue)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);
    ATLASSERT(::IsWindow(pThis->m_hWnd));

    if (HTREEITEM hItem = FindItem(TreeView_GetRoot(pThis->m_hWnd), vtValue))
        TreeView_SelectItem(pThis->m_hWnd, hItem);
}

template <typename T>
inline HTREEITEM CDBTreeCtrlImpl<T>::FindItem(HTREEITEM hStart, const VARIANT& vtFind)
{
    if (hStart != NULL)
    {
        T* pThis = static_cast<T*>(this);
        ATLASSERT(pThis);
        ATLASSERT(::IsWindow(pThis->m_hWnd));

        TVITEM item = { TVIF_PARAM, hStart };
        ATLVERIFY(TreeView_GetItem(pThis->m_hWnd, &item));

        if (*((CComVariant*)item.lParam) != vtFind)
        {
            if (HTREEITEM hFind = FindItem(TreeView_GetChild(pThis->m_hWnd, hStart), vtFind))
                return hFind;

            return FindItem(TreeView_GetNextSibling(pThis->m_hWnd, hStart), vtFind);
        }
    }

    return hStart;
}

template <typename T>
inline LRESULT CDBTreeCtrlImpl<T>::OnDeleteItem(int /*nIDCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/)
{
    LPNMTREEVIEW pNMTV = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
    ATLASSERT(pNMTV);

    delete (CComVariant*)pNMTV->itemOld.lParam;
    return 0L;
}

}  // namespace atlctrls

#endif  // __ATLDBCTRLS_INL__