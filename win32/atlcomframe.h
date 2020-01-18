///////////////////////////////////////////////////////////////////////////////
// atlcomframe.h
//
// Author : Garfield
// Creation Date : 2008/10/2

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __ATLCOMFRAME_H__
#define __ATLCOMFRAME_H__

#ifdef _WIN32_WCE
    #error atlcomframe.h compiles under Windows only
#endif

#include "sysutil.h"
#include <atlbase.h>
#include <atlwin.h>

///////////////////////////////////////////////////////////////////////////////
// Classes in this file:
//
// CWindowPaintFrameImpl<T>

namespace atlctrls {

///////////////////////////////////////////////////////////////////////////////
// Interface of the CWindowPaintFrameBase class
//

class ATL_NO_VTABLE CWindowPaintFrameBase
{
// Constructors
protected:
    CWindowPaintFrameBase(UINT nFrameWidth, UINT nFrameHeight, UINT nCaptionHeight);

// Implementation
protected:
    void PaintFrame_Init(HWND hWnd);
    void PaintFrame_CalcCaptionRect(HWND hWnd, const RECT& rcWindow, RECT& rcCaption) const;

    static BOOL WINAPI PaintFrame_IsThemeInactive(HWND hWnd);
    static BOOL WINAPI PaintFrame_IsButtonHitCode(UINT nHitCode);
    static UINT WINAPI PaintFrame_MapWindowCommand(HWND hWnd, UINT nHitCode);

    static UINT WINAPI PaintFrame_CalcAllItemsWidth(const _ATL_FRAME_ENTRY* pFirst);
    static UINT WINAPI PaintFrame_FindItem(UINT nHitCode, const _ATL_FRAME_ENTRY*& pEntry);
    static void WINAPI PaintFrame_CalcItemRect(const _ATL_FRAME_ENTRY* pFirst, const _ATL_FRAME_ENTRY* pEntry, const RECT& rcCaption, UINT nIndex, RECT& rcItem);

// Data members
public:
    UINT m_nFrameWidth;
    UINT m_nFrameHeight;
    UINT m_nCaptionHeight;

protected:
    UINT m_nHitCode;
    BOOL m_bMouseHover;
    BOOL m_bThemeInactive;

// Message map functions
public:
    LRESULT OnNcCalcSize(HWND hWnd, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnNcLButtonUp(HWND hWnd, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnNcMouseMove(HWND hWnd, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnNcMouseLeave(HWND hWnd, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnThemeChanged(HWND hWnd, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnNcLButtonDown(HWND hWnd, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CWindowPaintFrameImpl class
//

template <typename T>
class ATL_NO_VTABLE CWindowPaintFrameImpl : public CWindowPaintFrameBase
{
// Constructors
protected:
    CWindowPaintFrameImpl(UINT nFrameWidth = ::GetSystemMetrics(SM_CXSIZEFRAME), UINT nFrameHeight = ::GetSystemMetrics(SM_CYSIZEFRAME), UINT nCaptionHeight = ::GetSystemMetrics(SM_CYCAPTION));

// Implementation
public:
    BEGIN_MSG_MAP(CWindowPaintFrameImpl<T>)
        MESSAGE_HANDLER(WM_NCPAINT, OnNcPaint)
        MESSAGE_HANDLER(WM_NCHITTEST, OnNcHitTest)
        MESSAGE_HANDLER(WM_NCACTIVATE, OnNcActivate)
        MESSAGE_HANDLER(WM_NCDRAWITEM, OnNcDrawItem)
        MESSAGE_HANDLER_EX(WM_NCCALCSIZE, OnNcCalcSize)
        MESSAGE_HANDLER_EX(WM_NCLBUTTONUP, OnNcLButtonUp)
        MESSAGE_HANDLER_EX(WM_NCMOUSEMOVE, OnNcMouseMove)
        MESSAGE_HANDLER_EX(WM_NCMOUSELEAVE, OnNcMouseLeave)
        MESSAGE_HANDLER_EX(WM_THEMECHANGED, OnThemeChanged)
        MESSAGE_HANDLER_EX(WM_NCLBUTTONDOWN, OnNcLButtonDown)
    END_MSG_MAP()

// Overridables
protected:
    void PaintFrame_OnNcDraw(BOOL bActivate);
    void PaintFrame_OnDrawItem(const PAINTITEM& paint);
    void PaintFrame_OnDrawFrame(const PAINTITEM& paint);
    void PaintFrame_OnDrawCaption(const PAINTITEM& paint);

    void PaintFrame_CalcCaptionRect(const RECT& rcWindow, RECT& rcCaption) const;
    void PaintFrame_CalcItemRect(const RECT& rcCaption, UINT nIndex, const _ATL_FRAME_ENTRY* pEntry, RECT& rcItem) const;

// Operations
protected:
    void PaintFrame_Init();
    UINT PaintFrame_GetAllItemsWidth() const;
    void PaintFrame_GetItemRect(const RECT& rcWindow, UINT nHitCode, RECT& rcItem) const;

    BOOL PaintFrame_DrawItem(UINT nHitCode, UINT nState = 0);
    UINT PaintFrame_HitTest(const RECT& rcCaption, POINT point) const;
    BOOL PaintFrame_WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult, WNDPROC pfnDefWindowProc);

// Message map functions
public:
    LRESULT OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnNcDrawItem(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
};

}  // namespace atlctrls

#include "atlcomframe.inl"

#endif  // __ATLCOMFRAME_H__