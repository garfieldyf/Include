///////////////////////////////////////////////////////////////////////////////
// menuutil.inl : 实现文件
//
// Copyright(c) 2012, Garfield. All rights reserved.
// Author  : Garfield
// Version : 1.0
// Finish Time : 2009/4/25

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __MENUUTIL_INL__
#define __MENUUTIL_INL__

namespace atlctrls {

///////////////////////////////////////////////////////////////////////////////
// 常量定义
//

static const TCHAR _szMenuClassName[] = _T("#32768");
static const TCHAR _szMenuProcName[]  = _T("C11F8588-05EE-4C84-AE1E-1117C1010DF9");

///////////////////////////////////////////////////////////////////////////////
// CMenuHookModule 类的实现
//

#ifndef _ATL_HOOK_ALLMENUS
__declspec(selectany) BOOL CMenuHookModule::m_bDrawMenu = FALSE;
#endif

inline CMenuHookModule::CMenuHookModule()
    : m_hHook(NULL)
{
}

inline HHOOK CMenuHookModule::SetMenuHook()
{
#ifndef _ATL_HOOK_ALLMENUS
    return (m_hHook = ::SetWindowsHookEx(WH_CALLWNDPROC, _AtlMenuHookProc, NULL, ::GetCurrentThreadId()));
#else
    return (m_hHook = ::SetWindowsHookEx(WH_CBT, _AtlMenuHookProc, NULL, ::GetCurrentThreadId()));
#endif
}

inline BOOL CMenuHookModule::UnhookMenuHook()
{
    return ::UnhookWindowsHookEx(m_hHook);
}

inline void WINAPI CMenuHookModule::OnNcDraw(HWND hWnd, HDC hDC)
{
    ATLASSERT(hDC);
    ATLASSERT(::IsWindow(hWnd));

#ifndef _ATL_HOOK_ALLMENUS
    m_bDrawMenu = FALSE;
#endif

    RECT rcWindow;
    ATLVERIFY(::GetWindowRect(hWnd, &rcWindow));
    ATLVERIFY(::OffsetRect(&rcWindow, -rcWindow.left, -rcWindow.top));

    int dx = ::GetSystemMetrics(SM_CXBORDER) + ::GetSystemMetrics(SM_CXEDGE);
    int dy = ::GetSystemMetrics(SM_CYBORDER) + ::GetSystemMetrics(SM_CYEDGE);
    ::ExcludeClipRect(hDC, rcWindow.left + dx, rcWindow.top + dy, rcWindow.right - dx, rcWindow.bottom - dy);

    // Add code to draw menu nonclient area
    CBrush brush;
    brush.CreateSolidBrush(RGB(255, 0, 0));
    ::FillRect(hDC, &rcWindow, brush);

    CBrush brFrame;
    brFrame.CreateSolidBrush(RGB(0, 0, 255));
    ::FrameRect(hDC, &rcWindow, brFrame);
}

inline LRESULT CALLBACK CMenuHookModule::_AtlMenuHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
#ifndef _ATL_HOOK_ALLMENUS
    if (nCode == HC_ACTION)
    {
        LPCWPSTRUCT pcwp = reinterpret_cast<LPCWPSTRUCT>(lParam);
        ATLASSERT(pcwp);

        if (pcwp->message == WM_CREATE)
        {
            TCHAR szClassName[10];
            ::GetClassName(pcwp->hwnd, szClassName, _countof(szClassName));
            if (::lstrcmp(szClassName, _szMenuClassName) == 0)
                ATLVERIFY(::SetProp(pcwp->hwnd, _szMenuProcName, (HANDLE)::SetWindowLong(pcwp->hwnd, GWL_WNDPROC, (LONG)_AtlMenuProc)));
        }
        else if (pcwp->message == WM_INITMENUPOPUP)
        {
            // Add code
            m_bDrawMenu = !HIWORD(pcwp->lParam);
        }
    }
#else   // !_ATL_HOOK_ALLMENUS
    if (nCode == HCBT_CREATEWND)
    {
        TCHAR szClassName[10];
        ::GetClassName((HWND)wParam, szClassName, _countof(szClassName));
        if (::lstrcmp(szClassName, _szMenuClassName) == 0)
            ATLVERIFY(::SetProp((HWND)wParam, _szMenuProcName, (HANDLE)::SetWindowLong((HWND)wParam, GWL_WNDPROC, (LONG)_AtlMenuProc)));
    }
#endif  // _ATL_HOOK_ALLMENUS

    return ::CallNextHookEx(NULL, nCode, wParam, lParam);
}

inline LRESULT CALLBACK CMenuHookModule::_AtlMenuProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    WNDPROC pfnDefWndProc = (WNDPROC)::GetProp(hWnd, _szMenuProcName);
    ATLASSERT(pfnDefWndProc);
    ATLASSERT(::IsWindow(hWnd));

    LRESULT lResult = 0L;
    switch (message)
    {
    case WM_PRINT:
        lResult = ::CallWindowProc(pfnDefWndProc, hWnd, message, wParam, lParam);
    #ifndef _ATL_HOOK_ALLMENUS
        if (m_bDrawMenu)
    #endif
            OnNcDraw(hWnd, (HDC)wParam);
        break;

    case WM_NCPAINT:
    #ifndef _ATL_HOOK_ALLMENUS
        if (m_bDrawMenu)
    #endif
        {
            HDC hDC = ::GetWindowDC(hWnd);
            ATLASSERT(hDC);

            OnNcDraw(hWnd, hDC);
            ATLVERIFY(::ReleaseDC(hWnd, hDC));
            break;
        }

    default:
        lResult = ::CallWindowProc(pfnDefWndProc, hWnd, message, wParam, lParam);
    }

    return lResult;
}

}  // namespace atlctrls

#endif  // __MENUUTIL_INL__