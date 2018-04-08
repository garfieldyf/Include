///////////////////////////////////////////////////////////////////////////////
// dbctrls.h : ͷ�ļ�
//
// Copyright(c) 2012, Garfield. All rights reserved.
// Author  : Garfield
// Version : 2.0
// Finish Time : 2007/7/15

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __DBCTRLS_H__
#define __DBCTRLS_H__

#ifndef __STDDEFX_H__
#include "stddefx.h"
#endif

#ifndef __STDCOLLX_H__
#include "stdcollx.h"
#endif

#ifndef _INC_COMMCTRL
#include <commctrl.h>
#endif

#ifndef _INC_WINDOWSX
#pragma push_macro("SubclassWindow")
#undef SubclassWindow
#include <windowsx.h>
#pragma pop_macro("SubclassWindow")
#endif

///////////////////////////////////////////////////////////////////////////////
// Classes in this file:
//
// CComboBoxTraits
// CListBoxTraits
// CDBControlBaseT<TCtrlTraits>
// CDBListCtrlBase
// CDBTreeCtrlBase

namespace stdutil {

///////////////////////////////////////////////////////////////////////////////
// CComboBoxTraits ��Ķ���
//

struct CComboBoxTraits
{
    static int GetCurSel(HWND hWnd);
    static int SetCurSel(HWND hWnd, int nSelect);
    static int GetCount(HWND hWnd);
    static int InsertString(HWND hWnd, int nIndex, LPCWSTR pszString);
    static CComVariant* GetItemData(HWND hWnd, int nIndex);
    static int SetItemData(HWND hWnd, int nIndex, CComVariant* pVariant);
};


///////////////////////////////////////////////////////////////////////////////
// CListBoxTraits ��Ķ���
//

struct CListBoxTraits
{
    static int GetCurSel(HWND hWnd);
    static int SetCurSel(HWND hWnd, int nSelect);
    static int GetCount(HWND hWnd);
    static int InsertString(HWND hWnd, int nIndex, LPCWSTR pszString);
    static CComVariant* GetItemData(HWND hWnd, int nIndex);
    static int SetItemData(HWND hWnd, int nIndex, CComVariant* pVariant);
};


///////////////////////////////////////////////////////////////////////////////
// CDBControlBaseT ��Ķ���
//

template <typename TCtrlTraits>
class ATL_NO_VTABLE CDBControlBaseT
{
// ����
public:
    CDBControlBaseT();

// ʵ��
protected:
    void _DestroyAllItems(HWND hWnd);
    HRESULT _DataBind(HWND hWnd, _Connection* pConnection, BSTR bstrCommandText, const VARIANT& vtValueIndex, const VARIANT* pTextIndex, long lOptions);
};


///////////////////////////////////////////////////////////////////////////////
// CDBListCtrlBase ��Ķ���
//

class ATL_NO_VTABLE CDBListCtrlBase
{
// ����
public:
    CDBListCtrlBase();

// ����
public:
    HRESULT Close();
    HRESULT Commit();
    HRESULT Modify(int nItem);

    HRESULT SetFieldValue(const VARIANT& vtIndex, const BSTR bstrValue);
    HRESULT SetFieldValue(const VARIANT& vtIndex, const VARIANT& vtValue);

// ʵ��
protected:
    int _UpdateItem(HWND hWnd, int nItem);
    LRESULT _GetDispInfo(NMLVDISPINFO* pDispInfo);
    HRESULT _DataBind(HWND hWnd, _Connection* pConnection, const VARIANT& vtSource, int nColumnWidth, int nFormat, int nBeginColumn, int nEndColumn, long lOptions, BOOL bDeleteAllColumns);

// ��Ա
protected:
    int m_nPreItem;
    CComPtr<_Recordset> m_spRecordset;
};


///////////////////////////////////////////////////////////////////////////////
// CDBTreeCtrlBase ��Ķ���
//

class ATL_NO_VTABLE CDBTreeCtrlBase
{
// ����
public:
    CDBTreeCtrlBase();

// ʵ��
protected:
    HTREEITEM InsertNode(HWND hWnd, HTREEITEM hParent, stdutil::CTreeNode* pNode);
    void InsertRoot(HWND hWnd, stdutil::CTreeNodeList& nodeList, BOOL bExpanding);
    void InsertChild(HWND hWnd, stdutil::CTreeNodeList& nodeList, stdutil::CTreeNode* pStart, HTREEITEM hParent, const CComVariant* pParent, BOOL bExpanding);
};

}  // namespace stdutil

#include "dbctrls.inl"

#endif  // __DBCTRLS_H__