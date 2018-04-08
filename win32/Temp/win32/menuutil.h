///////////////////////////////////////////////////////////////////////////////
// menuutil.h : 头文件
//
// Copyright(c) 2012, Garfield. All rights reserved.
// Author  : Garfield
// Version : 1.0
// Finish Time : 2009/4/25

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __MENUUTIL_H__
#define __MENUUTIL_H__

#ifndef __STDDEFX_H__
    #error menuutil.h requires stddefx.h to be included first
#endif

///////////////////////////////////////////////////////////////////////////////
// Classes in this file:
//
// CMenuHookModule

namespace atlctrls {

///////////////////////////////////////////////////////////////////////////////
// CMenuHookModule 类的定义
//

class CMenuHookModule
{
// 构造
public:
    CMenuHookModule();

// 操作
public:
    HHOOK SetMenuHook();
    BOOL UnhookMenuHook();

// 实现
protected:
    static void WINAPI OnNcDraw(HWND hWnd, HDC hDC);
    static LRESULT CALLBACK _AtlMenuHookProc(int nCode, WPARAM wParam, LPARAM lParam);
    static LRESULT CALLBACK _AtlMenuProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

// 成员
protected:
    HHOOK m_hHook;
#ifndef _ATL_HOOK_ALLMENUS
    static BOOL m_bDrawMenu;
#endif
};

}  // namespace atlctrls

#include "menuutil.inl"

#endif  // __MENUUTIL_H__