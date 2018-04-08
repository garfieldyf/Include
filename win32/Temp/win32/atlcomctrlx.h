///////////////////////////////////////////////////////////////////////////////
// atlcomctrlx.h : 头文件
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
// CCustomListCtrlImpl 类的定义
//

class ATL_NO_VTABLE CCustomListCtrlImpl
{
// 构造
public:
    CCustomListCtrlImpl();

// 实现
public:
    BEGIN_MSG_MAP(CCustomListCtrlImpl)
        REFLECTED_NOTIFY_CODE_HANDLER(NM_CUSTOMDRAW, OnCustomDraw)
    END_MSG_MAP()

// 属性
public:
    COLORREF GetAlterBkColor() const;
    void SetAlterBkColor(COLORREF color);

    COLORREF GetNormalBkColor() const;
    void SetNormalBkColor(COLORREF color);

    COLORREF GetAlterTextColor() const;
    void SetAlterTextColor(COLORREF color);

    COLORREF GetNormalTextColor() const;
    void SetNormalTextColor(COLORREF color);

// 生成的消息映射函数
public:
    LRESULT OnCustomDraw(int nIDCtrl, LPNMHDR pNMHDR, BOOL& bHandled);

// 成员
protected:
    COLORREF m_clrAlterBk;
    COLORREF m_clrNormalBk;
    COLORREF m_clrAlterText;
    COLORREF m_clrNormalText;
};


///////////////////////////////////////////////////////////////////////////////
// CListCtrl 类的定义
//

class CListCtrl :
    public ATL::CWindowImpl<CListCtrl, WTL::CListViewCtrl, CListCtrlTraits>,
    public CSortListCtrlImpl<CListCtrl>,
    public CDragDropImpl<CListCtrl>,
    public CCustomListCtrlImpl
{
// 构造
public:
    CListCtrl();

// 实现
public:
    BEGIN_MSG_MAP(CListCtrl)
        REFLECTED_NOTIFY_CODE_HANDLER(NM_CUSTOMDRAW, OnCustomDraw)
        CHAIN_MSG_MAP(CSortListCtrlImpl<CListCtrl>)
        CHAIN_MSG_MAP(CDragDropImpl<CListCtrl>)
    END_MSG_MAP()

// 重写
public:
    BOOL SubclassWindow(HWND hWnd);
};


///////////////////////////////////////////////////////////////////////////////
// CDBComboBox 类的定义
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
// CDBListBox 类的定义
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
// CDBListCtrl 类的定义
//

class CDBListCtrl :
    public ATL::CWindowImpl<CDBListCtrl, WTL::CListViewCtrl, CListCtrlTraits>,
    public CDBListCtrlImpl<CDBListCtrl>,
    public CCustomListCtrlImpl
{
// 构造
public:
    CDBListCtrl();

// 实现
public:
    BEGIN_MSG_MAP(CDBListCtrl)
        CHAIN_MSG_MAP(CDBListCtrlImpl<CDBListCtrl>)
        CHAIN_MSG_MAP(CCustomListCtrlImpl)
    END_MSG_MAP()

// 重写
public:
    BOOL SubclassWindow(HWND hWnd);
};


///////////////////////////////////////////////////////////////////////////////
// CDBTreeCtrl 类的定义
//

class CDBTreeCtrl :
    public ATL::CWindowImpl<CDBTreeCtrl, WTL::CTreeViewCtrl, CTreeCtrlTraits>,
    public CDBTreeCtrlImpl<CDBTreeCtrl>
{
// 构造
public:
    CDBTreeCtrl();

// 实现
public:
    BEGIN_MSG_MAP(CDBTreeCtrl)
        CHAIN_MSG_MAP(CDBTreeCtrlImpl<CDBTreeCtrl>)
    END_MSG_MAP()

// 重写
public:
    BOOL SubclassWindow(HWND hWnd);
};

}  // namespace atlctrls

#include "atlcomctrlx.inl"

#endif  // __ATLCOMCTRLX_H__