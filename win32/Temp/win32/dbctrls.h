///////////////////////////////////////////////////////////////////////////////
// dbctrls.h : 头文件
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
// CComboBoxTraits 类的定义
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
// CListBoxTraits 类的定义
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
// CDBControlBaseT 类的定义
//

template <typename TCtrlTraits>
class ATL_NO_VTABLE CDBControlBaseT
{
// 构造
public:
    CDBControlBaseT();

// 实现
protected:
    void _DestroyAllItems(HWND hWnd);
    HRESULT _DataBind(HWND hWnd, _Connection* pConnection, BSTR bstrCommandText, const VARIANT& vtValueIndex, const VARIANT* pTextIndex, long lOptions);
};


///////////////////////////////////////////////////////////////////////////////
// CDBListCtrlBase 类的定义
//

class ATL_NO_VTABLE CDBListCtrlBase
{
// 构造
public:
    CDBListCtrlBase();

// 操作
public:
    HRESULT Close();
    HRESULT Commit();
    HRESULT Modify(int nItem);

    HRESULT SetFieldValue(const VARIANT& vtIndex, const BSTR bstrValue);
    HRESULT SetFieldValue(const VARIANT& vtIndex, const VARIANT& vtValue);

// 实现
protected:
    int _UpdateItem(HWND hWnd, int nItem);
    LRESULT _GetDispInfo(NMLVDISPINFO* pDispInfo);
    HRESULT _DataBind(HWND hWnd, _Connection* pConnection, const VARIANT& vtSource, int nColumnWidth, int nFormat, int nBeginColumn, int nEndColumn, long lOptions, BOOL bDeleteAllColumns);

// 成员
protected:
    int m_nPreItem;
    CComPtr<_Recordset> m_spRecordset;
};


///////////////////////////////////////////////////////////////////////////////
// CDBTreeCtrlBase 类的定义
//

class ATL_NO_VTABLE CDBTreeCtrlBase
{
// 构造
public:
    CDBTreeCtrlBase();

// 实现
protected:
    HTREEITEM InsertNode(HWND hWnd, HTREEITEM hParent, stdutil::CTreeNode* pNode);
    void InsertRoot(HWND hWnd, stdutil::CTreeNodeList& nodeList, BOOL bExpanding);
    void InsertChild(HWND hWnd, stdutil::CTreeNodeList& nodeList, stdutil::CTreeNode* pStart, HTREEITEM hParent, const CComVariant* pParent, BOOL bExpanding);
};

}  // namespace stdutil

#include "dbctrls.inl"

#endif  // __DBCTRLS_H__