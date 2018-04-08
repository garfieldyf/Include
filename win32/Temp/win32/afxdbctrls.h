///////////////////////////////////////////////////////////////////////////////
// afxdbctrls.h : ͷ�ļ�
//
// Copyright(c) 2012, Garfield. All rights reserved.
// Author  : Garfield
// Version : 2.0
// Finish Time : 2007/6/15

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __AFXDBCTRLS_H__
#define __AFXDBCTRLS_H__

#ifndef __DBCTRLS_H__
#include "dbctrls.h"
#endif

#ifndef __AFXCTRLS_H__
#include "afxctrls.h"
#endif

///////////////////////////////////////////////////////////////////////////////
// Classes in this file:
//
// CDBComboBox
// CDBListBox
// CDBListCtrl
// CDBTreeCtrl

namespace afxctrls {

///////////////////////////////////////////////////////////////////////////////
// CDBComboBox ��Ķ���
//

class CDBComboBox : public CComboBox, public stdutil::CDBControlBaseT<stdutil::CComboBoxTraits>
{
    DECLARE_DYNAMIC(CDBComboBox)
// ����/����
public:
    CDBComboBox();
    virtual ~CDBComboBox();

// ����
public:
    HRESULT DataBind(_Connection* pConnection, BSTR bstrCommandText, const VARIANT& vtValueIndex, const VARIANT* pTextIndex = NULL, long lOptions = adCmdText);

// ����
public:
    const CComVariant* GetSelectedValue() const;
    int SetSelectedValue(const VARIANT& vtValue);

// ���ɵ���Ϣӳ�亯��
public:
    afx_msg void OnDestroy();
    afx_msg LRESULT OnResetContent(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnDeleteString(WPARAM wParam, LPARAM lParam);
    DECLARE_MESSAGE_MAP()
};


///////////////////////////////////////////////////////////////////////////////
// CDBListBox ��Ķ���
//

class CDBListBox : public CListBox, public stdutil::CDBControlBaseT<stdutil::CListBoxTraits>
{
    DECLARE_DYNAMIC(CDBListBox)
// ����/����
public:
    CDBListBox();
    virtual ~CDBListBox();

// ����
public:
    HRESULT DataBind(_Connection* pConnection, BSTR bstrCommandText, const VARIANT& vtValueIndex, const VARIANT* pTextIndex = NULL, long lOptions = adCmdText);

// ����
public:
    const CComVariant* GetSelectedValue() const;
    int SetSelectedValue(const VARIANT& vtValue);

// ���ɵ���Ϣӳ�亯��
public:
    afx_msg void OnDestroy();
    afx_msg LRESULT OnResetContent(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnDeleteString(WPARAM wParam, LPARAM lParam);
    DECLARE_MESSAGE_MAP()
};


///////////////////////////////////////////////////////////////////////////////
// CDBListCtrl ��Ķ���
//

class CDBListCtrl : public CListCtrl, public CCustomListCtrlImpl, public CSortListCtrlImpl, public stdutil::CDBListCtrlBase
{
    DECLARE_DYNAMIC(CDBListCtrl)
// ����/����
public:
    CDBListCtrl();
    virtual ~CDBListCtrl();

// ��д
protected:
    virtual void PreSubclassWindow();

// ����
public:
    int UpdateItem(int nItem);
    HRESULT DataBind(_Connection* pConnection, const VARIANT& vtSource, int nColumnWidth, int nFormat = LVCFMT_CENTER, int nBeginColumn = 0,
                     int nEndColumn = -1, long lOptions = adCmdText, BOOL bDeleteAllColumns = FALSE);

// ���ɵ���Ϣӳ�亯��
public:
    afx_msg void OnDestroy();
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg LRESULT OnInsertItem(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnDeleteItem(WPARAM wParam, LPARAM lParam);
    afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnGetDispInfo(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnColumnClick(NMHDR* pNMHDR, LRESULT* pResult);
    DECLARE_MESSAGE_MAP()
};


///////////////////////////////////////////////////////////////////////////////
// CDBTreeCtrl ��Ķ���
//

class CDBTreeCtrl : public CTreeCtrl, public stdutil::CDBTreeCtrlBase
{
    DECLARE_DYNAMIC(CDBTreeCtrl)
// ����/����
public:
    CDBTreeCtrl();
    virtual ~CDBTreeCtrl();

// ��д
protected:
    virtual void PreSubclassWindow();

// ����
public:
    HRESULT DataBind(_Connection* pConnection, BSTR bstrCommandText, const VARIANT& vtParentIndex, const VARIANT& vtValueIndex, const VARIANT* pTextIndex = NULL,
                     const VARIANT* pImageIndex = NULL, const VARIANT* pSelImageIndex = NULL, long lOptions = adCmdText, BOOL bExpanding = TRUE);

// ����
public:
    const CComVariant* GetSelectedValue() const;
    void SetSelectedValue(const VARIANT& vtValue);

// ʵ��
protected:
    HTREEITEM FindItem(HTREEITEM hStart, const VARIANT& vtFind);

// ���ɵ���Ϣӳ�亯��
public:
    afx_msg void OnDeleteItem(NMHDR* pNMHDR, LRESULT* pResult);
    DECLARE_MESSAGE_MAP()
};

}  // namespace afxctrls

#include "afxdbctrls.inl"

#endif  // __AFXDBCTRLS_H__