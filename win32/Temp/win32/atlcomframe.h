///////////////////////////////////////////////////////////////////////////////
// atlcomframe.h : 头文件
//
// Copyright(c) 2012, Garfield. All rights reserved.
// Author  : Garfield
// Version : 1.0
// Finish Time : 2008/5/17

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __ATLCOMFRAME_H__
#define __ATLCOMFRAME_H__

///////////////////////////////////////////////////////////////////////////////
//
// Macros in this file:
//
// BEGIN_DLGITEM_MAP(thisClass)
//   DLGITEM_ENTRY(nID, flags)
//   DLGITEM_GROUP()
// END_DLGITEM_MAP()
//
// Structures in this file:
//
// _ATL_DLGITEM_ENTRY
//
// Classes in this file:
//
// CResizeDialogImpl

///////////////////////////////////////////////////////////////////////////////
// Macros
//

#define BEGIN_DLGITEM_MAP(thisClass) \
    static atlctrls::_ATL_DLGITEM_ENTRY* WINAPI _GetDlgItemEntry(int& nCount) throw() { \
    static atlctrls::_ATL_DLGITEM_ENTRY _dlgItem[] = { { _ATL_ID_SCROLL_BAR, DLGITEM_MOVE_X | DLGITEM_MOVE_Y, { 0, 0, 0, 0 } },

#define DLGITEM_ENTRY(nID, flags) \
    { (nID), (flags), { 0, 0, 0, 0 } },

#define DLGITEM_GROUP() \
    { 0, _DLGITEM_GROUP, { MAXLONG, MAXLONG, MINLONG, MINLONG } },

#define END_DLGITEM_MAP() \
    }; nCount = _countof(_dlgItem); return _dlgItem; }

namespace atlctrls {

///////////////////////////////////////////////////////////////////////////////
// _ATL_DLGITEM_ENTRY 类的定义
//

struct _ATL_DLGITEM_ENTRY
{
    int   nID;
    DWORD dwFlags;
    RECT  rect;
};


///////////////////////////////////////////////////////////////////////////////
// CResizeDialogImpl 类的定义
//

template <typename T>
class ATL_NO_VTABLE CResizeDialogImpl
{
public:
    enum
    {
        DLGITEM_SIZE_X  = 0x0001,
        DLGITEM_SIZE_Y  = 0x0002,
        DLGITEM_MOVE_X  = 0x0004,
        DLGITEM_MOVE_Y  = 0x0008,

        // internal use only
        _DLGITEM_GROUP = 0x1000,
        _ATL_ID_SCROLL_BAR = 0xEFEF
    };

// 构造
public:
    CResizeDialogImpl();

// 实现
public:
    BEGIN_MSG_MAP(CResizeDialogImpl<T>)
        MESSAGE_HANDLER(WM_SIZE, OnSize)
        MESSAGE_HANDLER(WM_GETMINMAXINFO, OnGetMinMaxInfo)
    END_MSG_MAP()

// 操作
protected:
    void ResizeDialog_UpdateLayout(int cx, int cy);
    void ResizeDialog_Initialize(BOOL bAddGripper = TRUE, BOOL bUseMinSize = TRUE);
    BOOL ResizeDialog_SetDlgItemPos(const _ATL_DLGITEM_ENTRY& item, int dx, int dy, LPCRECT lpRect = NULL, int xOffset = -1, int yOffset = -1);

// 成员
protected:
    POINT m_ptMinSize;
    SIZE  m_sizeClient;

// 生成的消息映射函数
public:
    LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
};

}  // namespace atlctrls

#include "atlcomframe.inl"

#endif  // __ATLCOMFRAME_H__