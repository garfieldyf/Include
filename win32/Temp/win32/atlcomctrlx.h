///////////////////////////////////////////////////////////////////////////////
// atlcomctrlx.h : ͷ�ļ�
//
// Copyright(c) 2012, Garfield. All rights reserved.
// Author  : Garfield
// Version : 1.0
// Finish Time : 2007/2/4

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __ATLCOMCTRLX_H__
#define __ATLCOMCTRLX_H__

#ifndef __ATLAPP_H__
#include <atlapp.h>
#endif

#ifndef __ATLCTRLS_H__
#include <atlctrls.h>
#endif

#ifndef __ATLCOMCTRLS_H__
#include "../atlcomctrls.h"
#endif

#ifndef __ATLDBCTRLS_H__
#include "atldbctrls.h"
#endif

///////////////////////////////////////////////////////////////////////////////
// Classes in this file:
//
// CListCtrl
// CDBComboBox
// CDBListBox
// CDBListCtrl
// CDBTreeCtrl

namespace atlctrls {

///////////////////////////////////////////////////////////////////////////////
// CWinTraits Typedef
//

typedef ATL::CWinTraitsOR<TVS_INFOTIP> CTreeCtrlTraits;
typedef ATL::CWinTraitsOR<LVS_REPORT, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_INFOTIP> CListCtrlTraits;

///////////////////////////////////////////////////////////////////////////////
// CCustomListCtrlImpl ��Ķ���
//

class ATL_NO_VTABLE CCustomListCtrlImpl
{
// ����
public:
    CCustomListCtrlImpl();

// ʵ��
public:
    BEGIN_MSG_MAP(CCustomListCtrlImpl)
        REFLECTED_NOTIFY_CODE_HANDLER(NM_CUSTOMDRAW, OnCustomDraw)
    END_MSG_MAP()

// ����
public:
    COLORREF GetAlterBkColor() const;
    void SetAlterBkColor(COLORREF color);

    COLORREF GetNormalBkColor() const;
    void SetNormalBkColor(COLORREF color);

    COLORREF GetAlterTextColor() const;
    void SetAlterTextColor(COLORREF color);

    COLORREF GetNormalTextColor() const;
    void SetNormalTextColor(COLORREF color);

// ���ɵ���Ϣӳ�亯��
public:
    LRESULT OnCustomDraw(int nIDCtrl, LPNMHDR pNMHDR, BOOL& bHandled);

// ��Ա
protected:
    COLORREF m_clrAlterBk;
    COLORREF m_clrNormalBk;
    COLORREF m_clrAlterText;
    COLORREF m_clrNormalText;
};


///////////////////////////////////////////////////////////////////////////////
// CListCtrl ��Ķ���
//

class CListCtrl :
    public ATL::CWindowImpl<CListCtrl, WTL::CListViewCtrl, CListCtrlTraits>,
    public CSortListCtrlImpl<CListCtrl>,
    public CDragDropImpl<CListCtrl>,
    public CCustomListCtrlImpl
{
// ����
public:
    CListCtrl();

// ʵ��
public:
    BEGIN_MSG_MAP(CListCtrl)
        REFLECTED_NOTIFY_CODE_HANDLER(NM_CUSTOMDRAW, OnCustomDraw)
        CHAIN_MSG_MAP(CSortListCtrlImpl<CListCtrl>)
        CHAIN_MSG_MAP(CDragDropImpl<CListCtrl>)
    END_MSG_MAP()

// ��д
public:
    BOOL SubclassWindow(HWND hWnd);
};


///////////////////////////////////////////////////////////////////////////////
// CDBComboBox ��Ķ���
//

class CDBComboBox :
    public ATL::CWindowImpl<CDBComboBox, WTL::CComboBox>,
    public CDBControlImpl<CDBComboBox, stdutil::CComboBoxTraits>
{
public:
    typedef CDBControlImpl<CDBComboBox, stdutil::CComboBoxTraits> _BaseClass;

    BEGIN_MSG_MAP(CDBComboBox)
        CHAIN_MSG_MAP(_BaseClass)
    END_MSG_MAP()
};


///////////////////////////////////////////////////////////////////////////////
// CDBListBox ��Ķ���
//

class CDBListBox :
    public ATL::CWindowImpl<CDBListBox, WTL::CListBox>,
    public CDBControlImpl<CDBListBox, stdutil::CListBoxTraits>
{
public:
    typedef CDBControlImpl<CDBListBox, stdutil::CListBoxTraits> _BaseClass;

    BEGIN_MSG_MAP(CDBListBox)
        CHAIN_MSG_MAP(_BaseClass)
    END_MSG_MAP()
};


///////////////////////////////////////////////////////////////////////////////
// CDBListCtrl ��Ķ���
//

class CDBListCtrl :
    public ATL::CWindowImpl<CDBListCtrl, WTL::CListViewCtrl, CListCtrlTraits>,
    public CDBListCtrlImpl<CDBListCtrl>,
    public CCustomListCtrlImpl
{
// ����
public:
    CDBListCtrl();

// ʵ��
public:
    BEGIN_MSG_MAP(CDBListCtrl)
        CHAIN_MSG_MAP(CDBListCtrlImpl<CDBListCtrl>)
        CHAIN_MSG_MAP(CCustomListCtrlImpl)
    END_MSG_MAP()

// ��д
public:
    BOOL SubclassWindow(HWND hWnd);
};


///////////////////////////////////////////////////////////////////////////////
// CDBTreeCtrl ��Ķ���
//

class CDBTreeCtrl :
    public ATL::CWindowImpl<CDBTreeCtrl, WTL::CTreeViewCtrl, CTreeCtrlTraits>,
    public CDBTreeCtrlImpl<CDBTreeCtrl>
{
// ����
public:
    CDBTreeCtrl();

// ʵ��
public:
    BEGIN_MSG_MAP(CDBTreeCtrl)
        CHAIN_MSG_MAP(CDBTreeCtrlImpl<CDBTreeCtrl>)
    END_MSG_MAP()

// ��д
public:
    BOOL SubclassWindow(HWND hWnd);
};

}  // namespace atlctrls

#include "atlcomctrlx.inl"

#endif  // __ATLCOMCTRLX_H__