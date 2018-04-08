///////////////////////////////////////////////////////////////////////////////
// atldbctrls.h : 头文件
//
// Copyright(c) 2012, Garfield. All rights reserved.
// Author  : Garfield
// Version : 3.0
// Finish Time : 2007/7/15

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __ATLDBCTRLS_H__
#define __ATLDBCTRLS_H__

#ifndef __DBCTRLS_H__
#include "dbctrls.h"
#endif

#ifndef __STDCTRLS_H__
#include "stdctrls.h"
#endif

///////////////////////////////////////////////////////////////////////////////
// Classes in this file:
//
// CDBControlImpl<T, TCtrlTraits>
// CDBListCtrlImpl<T>
// CDBTreeCtrlImpl<T>

namespace atlctrls {

///////////////////////////////////////////////////////////////////////////////
// CDBControlImpl 类的定义
//

template <typename T, typename TCtrlTraits>
class ATL_NO_VTABLE CDBControlImpl : public stdutil::CDBControlBaseT<TCtrlTraits>
{
// 构造
public:
    CDBControlImpl();

// 实现
public:
    typedef CDBControlImpl<T, TCtrlTraits> _CDBControlImpl;

    BEGIN_MSG_MAP(_CDBControlImpl)
        MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
        MESSAGE_HANDLER(CB_RESETCONTENT, OnResetContent)
        MESSAGE_HANDLER(CB_RESETCONTENT, OnResetContent)
        MESSAGE_HANDLER(CB_DELETESTRING, OnDeleteString)
        MESSAGE_HANDLER(CB_DELETESTRING, OnDeleteString)
    END_MSG_MAP()

// 操作
public:
    HRESULT DataBind(_Connection* pConnection, BSTR bstrCommandText, const VARIANT& vtValueIndex, const VARIANT* pTextIndex = NULL, long lOptions = adCmdText);

// 属性
public:
    const CComVariant* GetSelectedValue() const;
    int SetSelectedValue(const VARIANT& vtValue);

// 生成的消息映射函数
public:
    LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnResetContent(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnDeleteString(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
};


///////////////////////////////////////////////////////////////////////////////
// CDBListCtrlImpl 类的定义
//

template <typename T>
class ATL_NO_VTABLE CDBListCtrlImpl : public stdctrls::CSortListCtrlBase, public stdutil::CDBListCtrlBase
{
// 构造
public:
    CDBListCtrlImpl();

// 实现
public:
    BEGIN_MSG_MAP(CDBListCtrlImpl<T>)
        MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
        MESSAGE_HANDLER(LVM_INSERTITEM, OnInsertItem)
        MESSAGE_HANDLER(LVM_DELETEITEM, OnDeleteItem)
        REFLECTED_NOTIFY_CODE_HANDLER(LVN_COLUMNCLICK, OnColumnClick)
        REFLECTED_NOTIFY_CODE_HANDLER(LVN_GETDISPINFO, OnGetDispInfo)
    END_MSG_MAP()

// 操作
public:
    int UpdateItem(int nItem);
    HRESULT DataBind(_Connection* pConnection, const VARIANT& vtSource, int nColumnWidth, int nFormat = LVCFMT_CENTER, int nBeginColumn = 0,
                     int nEndColumn = -1, long lOptions = adCmdText, BOOL bDeleteAllColumns = FALSE);

// 生成的消息映射函数
public:
    LRESULT OnColumnClick(int nIDCtrl, LPNMHDR pNMHDR, BOOL& bHandled);
    LRESULT OnGetDispInfo(int nIDCtrl, LPNMHDR pNMHDR, BOOL& bHandled);
    LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnInsertItem(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnDeleteItem(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
};


///////////////////////////////////////////////////////////////////////////////
// CDBTreeCtrlImpl 类的定义
//

template <typename T>
class ATL_NO_VTABLE CDBTreeCtrlImpl : public stdutil::CDBTreeCtrlBase
{
// 构造
public:
    CDBTreeCtrlImpl();

// 实现
public:
    BEGIN_MSG_MAP(CDBTreeCtrlImpl<T>)
        REFLECTED_NOTIFY_CODE_HANDLER(TVN_DELETEITEM, OnDeleteItem)
    END_MSG_MAP()

// 操作
public:
    HRESULT DataBind(_Connection* pConnection, BSTR bstrCommandText, const VARIANT& vtParentIndex, const VARIANT& vtValueIndex, const VARIANT* pTextIndex = NULL,
                     const VARIANT* pImageIndex = NULL, const VARIANT* pSelImageIndex = NULL, long lOptions = adCmdText, BOOL bExpanding = TRUE);

// 属性
public:
    const CComVariant* GetSelectedValue() const;
    void SetSelectedValue(const VARIANT& vtValue);

// 实现
protected:
    HTREEITEM FindItem(HTREEITEM hStart, const VARIANT& vtFind);

// 生成的消息映射函数
public:
    LRESULT OnDeleteItem(int nIDCtrl, LPNMHDR pNMHDR, BOOL& bHandled);
};

}  // namespace atlctrls

#include "atldbctrls.inl"

#endif  // __ATLDBCTRLS_H__