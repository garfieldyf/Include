///////////////////////////////////////////////////////////////////////////////
// dbctrls.inl : 实现文件
//
// Copyright(c) 2012, Garfield. All rights reserved.
// Author  : Garfield
// Version : 2.0
// Finish Time : 2007/7/15

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __DBCTRLS_INL__
#define __DBCTRLS_INL__

namespace stdutil {

///////////////////////////////////////////////////////////////////////////////
// CComboBoxTraits 类的实现
//

inline int CComboBoxTraits::GetCurSel(HWND hWnd)
{
    return ComboBox_GetCurSel(hWnd);
}

inline int CComboBoxTraits::SetCurSel(HWND hWnd, int nSelect)
{
    return ComboBox_SetCurSel(hWnd, nSelect);
}

inline int CComboBoxTraits::GetCount(HWND hWnd)
{
    return ComboBox_GetCount(hWnd);
}

inline int CComboBoxTraits::InsertString(HWND hWnd, int nIndex, LPCWSTR pszString)
{
    return (int)::SendMessageW(hWnd, CB_INSERTSTRING, nIndex, (LPARAM)pszString);
}

inline CComVariant* CComboBoxTraits::GetItemData(HWND hWnd, int nIndex)
{
    return (CComVariant*)ComboBox_GetItemData(hWnd, nIndex);
}

inline int CComboBoxTraits::SetItemData(HWND hWnd, int nIndex, CComVariant* pVariant)
{
    return ComboBox_SetItemData(hWnd, nIndex, pVariant);
}


///////////////////////////////////////////////////////////////////////////////
// CListBoxTraits 类的实现
//

inline int CListBoxTraits::GetCurSel(HWND hWnd)
{
    return ListBox_GetCurSel(hWnd);
}

inline int CListBoxTraits::SetCurSel(HWND hWnd, int nSelect)
{
    return ListBox_SetCurSel(hWnd, nSelect);
}

inline int CListBoxTraits::GetCount(HWND hWnd)
{
    return ListBox_GetCount(hWnd);
}

inline int CListBoxTraits::InsertString(HWND hWnd, int nIndex, LPCWSTR pszString)
{
    return (int)::SendMessageW(hWnd, LB_INSERTSTRING, nIndex, (LPARAM)pszString);
}

inline CComVariant* CListBoxTraits::GetItemData(HWND hWnd, int nIndex)
{
    return (CComVariant*)ListBox_GetItemData(hWnd, nIndex);
}

inline int CListBoxTraits::SetItemData(HWND hWnd, int nIndex, CComVariant* pVariant)
{
    return ListBox_SetItemData(hWnd, nIndex, pVariant);
}


///////////////////////////////////////////////////////////////////////////////
// CDBControlBaseT 类的实现
//

template <typename TCtrlTraits>
inline CDBControlBaseT<TCtrlTraits>::CDBControlBaseT()
{
}

template <typename TCtrlTraits>
inline void CDBControlBaseT<TCtrlTraits>::_DestroyAllItems(HWND hWnd)
{
    ATLASSERT(::IsWindow(hWnd));

    int nCount = TCtrlTraits::GetCount(hWnd);
    while (--nCount >= 0)
        delete TCtrlTraits::GetItemData(hWnd, nCount);
}

template <typename TCtrlTraits>
inline HRESULT CDBControlBaseT<TCtrlTraits>::_DataBind(HWND hWnd, _Connection* pConnection, BSTR bstrCommandText, const VARIANT& vtValueIndex, const VARIANT* pTextIndex, long lOptions)
{
    ATLASSERT(bstrCommandText);
    ATLASSERT(::IsWindow(hWnd));
    ATLASSERT(pConnection != NULL);
    ATLASSERT(vtValueIndex.vt != VT_EMPTY);
    ATLASSERT(!(GetWindowStyle(hWnd) & CBS_OWNERDRAWFIXED));
    ATLASSERT(!(GetWindowStyle(hWnd) & LBS_OWNERDRAWFIXED));
    ATLASSERT(!(GetWindowStyle(hWnd) & CBS_OWNERDRAWVARIABLE));
    ATLASSERT(!(GetWindowStyle(hWnd) & LBS_OWNERDRAWVARIABLE));

    CComPtr<_Recordset> spRecordset;
    HRESULT hr = pConnection->Execute(bstrCommandText, NULL, lOptions, &spRecordset);
    if (FAILED(hr))
        return hr;

    VARIANT_BOOL bEOF = VARIANT_TRUE;
    hr = spRecordset->get_EOF(&bEOF);
    const VARIANT& vtIndex = (pTextIndex != NULL) ? *pTextIndex : vtValueIndex;

    for (int i = 0, nIndex = 0; SUCCEEDED(hr) && !bEOF; ++i)
    {
        CComVariant vtText;
        _COM_VERIFY(spRecordset->get_Collect(vtIndex, &vtText));

        BSTR bstrText = NULL;
        if (SUCCEEDED(vtText.ChangeType(VT_BSTR)))
            bstrText = vtText.bstrVal;
        nIndex = TCtrlTraits::InsertString(hWnd, i, bstrText);

        if (pTextIndex != NULL)
        {
            CComVariant* pValue = new CComVariant();
            _COM_VERIFY(spRecordset->get_Collect(vtValueIndex, pValue));
            TCtrlTraits::SetItemData(hWnd, nIndex, pValue);
        }

        _COM_VERIFY(spRecordset->MoveNext());
        hr = spRecordset->get_EOF(&bEOF);
    }

    TCtrlTraits::SetCurSel(hWnd, 0);
    _COM_VERIFY(spRecordset->Close());

    return hr;
}


///////////////////////////////////////////////////////////////////////////////
// CDBListCtrlBase 类的实现
//

inline CDBListCtrlBase::CDBListCtrlBase()
    : m_nPreItem(-1)
{
    _COM_VERIFY(m_spRecordset.CoCreateInstance(L"ADODB.Recordset"));
}

inline HRESULT CDBListCtrlBase::Close()
{
    ATLASSERT(m_spRecordset != NULL);

    long lState = adStateOpen;
    _COM_VERIFY(m_spRecordset->get_State(&lState));
    return (lState != adStateClosed ? m_spRecordset->Close() : S_OK);
}

inline HRESULT CDBListCtrlBase::Commit()
{
    ATLASSERT(m_spRecordset != NULL);
    return m_spRecordset->UpdateBatch(adAffectAll);
}

inline HRESULT CDBListCtrlBase::Modify(int nItem)
{
    ATLASSERT(m_spRecordset != NULL);
    return m_spRecordset->Move(nItem, CComVariant(adBookmarkFirst));
}

inline HRESULT CDBListCtrlBase::SetFieldValue(const VARIANT& vtIndex, const BSTR bstrValue)
{
    ATLASSERT(m_spRecordset != NULL);

    VARIANT vtValue = { VT_BSTR };
    vtValue.bstrVal = bstrValue;

    return m_spRecordset->put_Collect(vtIndex, vtValue);
}

inline HRESULT CDBListCtrlBase::SetFieldValue(const VARIANT& vtIndex, const VARIANT& vtValue)
{
    ATLASSERT(m_spRecordset != NULL);
    return m_spRecordset->put_Collect(vtIndex, vtValue);
}

inline int CDBListCtrlBase::_UpdateItem(HWND hWnd, int nItem)
{
    if (UINT nCount = ListView_GetSelectedCount(hWnd))
    {
        int nIndex = -1;
        for (UINT i = 0; i < nCount; ++i)
        {
            nIndex = ListView_GetNextItem(hWnd, nIndex, LVNI_SELECTED);
            ListView_SetItemState(hWnd, nIndex, 0, LVIS_SELECTED);
        }
    }

    ::SetFocus(hWnd);
    ListView_EnsureVisible(hWnd, nItem, FALSE);
    ListView_SetItemState(hWnd, nItem, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);

    return nItem;
}

inline LRESULT CDBListCtrlBase::_GetDispInfo(NMLVDISPINFO* pDispInfo)
{
    ATLASSERT(pDispInfo);
    ATLASSERT(m_spRecordset != NULL);

    if (pDispInfo->item.mask & LVIF_TEXT)
    {
        // 移动到要显示的记录上
        if (m_nPreItem != pDispInfo->item.iItem)
        {
            _COM_VERIFY(m_spRecordset->MoveFirst());
            _COM_VERIFY(m_spRecordset->Move(pDispInfo->item.iItem, CComVariant(adBookmarkFirst)));
            m_nPreItem = pDispInfo->item.iItem;
        }

        // 获得记录值，填充 ListCtrl
        CComVariant vtValue;
        _COM_VERIFY(m_spRecordset->get_Collect(CComVariant(pDispInfo->item.iSubItem), &vtValue));
        if (SUCCEEDED(vtValue.ChangeType(VT_BSTR)))
            ::lstrcpyn(pDispInfo->item.pszText, COLE2CT(vtValue.bstrVal), pDispInfo->item.cchTextMax);
    }

    return 0L;
}

inline HRESULT CDBListCtrlBase::_DataBind(HWND hWnd, _Connection* pConnection, const VARIANT& vtSource, int nColumnWidth, int nFormat, int nBeginColumn,
                                          int nEndColumn, long lOptions, BOOL bDeleteAllColumns)
{
    ATLASSERT(::IsWindow(hWnd));
    ATLASSERT(nBeginColumn >= 0);
    ATLASSERT(pConnection != NULL);
    ATLASSERT(GetWindowStyle(hWnd) & LVS_OWNERDATA);

    _COM_VERIFY(Close());
    _COM_VERIFY(m_spRecordset->put_CursorLocation(adUseClient));

    HRESULT hr = m_spRecordset->Open(vtSource, CComVariant(pConnection), adOpenStatic, adLockBatchOptimistic, lOptions);
    if (FAILED(hr))
        return hr;

    HWND hWndHD = ListView_GetHeader(hWnd);
    ATLASSERT(::IsWindow(hWndHD));

    // 清除 ListCtrl 中原来的项
    ListView_DeleteAllItems(hWnd);
    if (bDeleteAllColumns)
    {
        for (int i = Header_GetItemCount(hWndHD) - 1; i >= 0; --i)
            ListView_DeleteColumn(hWnd, i);
    }

    // 添加表头
    long lCount = Header_GetItemCount(hWndHD);
    if (lCount <= 0)
    {
        CComPtr<Fields> spFields;
        _COM_VERIFY(m_spRecordset->get_Fields(&spFields));
        _COM_VERIFY(spFields->get_Count(&lCount));

        if (nEndColumn == -1)
            nEndColumn = lCount;
        ATLASSERT(nEndColumn >= 0);

        for (long i = 0; i < lCount; ++i)
        {
            // 获得字段名称
            CComPtr<Field> spField;
            if (SUCCEEDED(spFields->get_Item(CComVariant(i), &spField)))
            {
                CComBSTR bstrName;
                _COM_VERIFY(spField->get_Name(&bstrName));

                USES_CONVERSION;
                LVCOLUMN column = { LVCF_TEXT | LVCF_FMT | LVCF_WIDTH, nFormat, (i >= nBeginColumn && i < nEndColumn ? nColumnWidth : 0), OLE2T(bstrName) };
                ListView_InsertColumn(hWnd, i, &column);
            }
        }
    }

    _COM_VERIFY(m_spRecordset->get_RecordCount(&lCount));
    ListView_SetItemCountEx(hWnd, lCount, LVSICF_NOINVALIDATEALL);
    ::InvalidateRect(hWnd, NULL, TRUE);

    return hr;
}


///////////////////////////////////////////////////////////////////////////////
// CDBTreeCtrlBase 类的实现
//

inline CDBTreeCtrlBase::CDBTreeCtrlBase()
{
}

inline HTREEITEM CDBTreeCtrlBase::InsertNode(HWND hWnd, HTREEITEM hParent, stdutil::CTreeNode* pNode)
{
    ATLASSERT(pNode);
    ATLASSERT(hParent);
    ATLASSERT(::IsWindow(hWnd));
    USES_CONVERSION;

    TVINSERTSTRUCT insert =
    {
        hParent, TVI_LAST, { TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM, NULL, 0, 0, NULL, 0, pNode->GetImage(),
        pNode->GetSelectedImage(), 0, (pNode->GetText()) ? reinterpret_cast<LPARAM>(pNode->DetachValue()) : 0 }
    };

    CComVariant* pText = (pNode->GetText() ? pNode->GetText() : pNode->GetValue());
    if (SUCCEEDED(pText->ChangeType(VT_BSTR)))
        insert.item.pszText = OLE2T(pText->bstrVal);

    return TreeView_InsertItem(hWnd, &insert);
}

inline void CDBTreeCtrlBase::InsertRoot(HWND hWnd, stdutil::CTreeNodeList& nodeList, BOOL bExpanding)
{
    ATLASSERT(::IsWindow(hWnd));

    for (stdutil::CTreeNode* pNode = nodeList.GetHead(); pNode != NULL; pNode = pNode->GetNext())
    {
        if (nodeList.FindNode(nodeList.GetHead(), stdutil::CTreeNodeList::CFindParent(pNode->GetParent())) == NULL)
        {
            // 增加根节点
            const CComVariant* pValue = pNode->GetValue();
            HTREEITEM hItem = InsertNode(hWnd, TVI_ROOT, pNode);

            InsertChild(hWnd, nodeList, nodeList.GetHead(), hItem, pValue, bExpanding);     // 添加该节点的所有孩子
            pNode = nodeList.DeleteNode(pNode);                                             // 从链表中删除该节点

            if (bExpanding)
                TreeView_Expand(hWnd, hItem, TVE_EXPAND);
        }
    }
}

inline void CDBTreeCtrlBase::InsertChild(HWND hWnd, stdutil::CTreeNodeList& nodeList, stdutil::CTreeNode* pStart, HTREEITEM hParent, const CComVariant* pParent, BOOL bExpanding)
{
    ATLASSERT(::IsWindow(hWnd));

    if (stdutil::CTreeNode* pNode = nodeList.FindNode(pStart, stdutil::CTreeNodeList::CFindChild(pParent)))
    {
        // 增加节点
        const CComVariant* pValue = pNode->GetValue();
        HTREEITEM hItem = InsertNode(hWnd, hParent, pNode);

        InsertChild(hWnd, nodeList, nodeList.GetHead(), hItem, pValue, bExpanding);             // 添加孩子节点
        InsertChild(hWnd, nodeList, nodeList.DeleteNode(pNode), hParent, pParent, bExpanding);  // 添加兄弟节点

        if (bExpanding)
            TreeView_Expand(hWnd, hItem, TVE_EXPAND);
    }
}

}  // namespace stdutil

#endif  // __DBCTRLS_INL__