/////////////////////////////////////////////////////////////////////
// splash.h : 头文件
//
// Copyright(c) 2007, Garfield. All rights reserved.
// Author  : Garfield
// Version : 1.0
// Finish Time : 2007/3/4

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __SPLASH_H__
#define __SPLASH_H__

namespace afxctrls {

///////////////////////////////////////////////////////////////////////////////
// CSplashWnd 类的定义
//

class CSplashWnd : public CWnd
{
// 构造/销毁
protected:
    CSplashWnd();
    virtual ~CSplashWnd();

// 重写
protected:
    virtual void PostNcDestroy();

// 操作
public:
    // 处理任意键消息
    static BOOL TranslateMassage(MSG* pMsg);

    // 显示窗口
    static void ShowSplashWnd(CWnd* pParentWnd, UINT nIDBitmap, UINT nDelayTime = 1000);
    static void ShowSplashWnd(CWnd* pParentWnd, HBITMAP hBitmap, UINT nDelayTime = 1000);
    static void ShowSplashWnd(CWnd* pParentWnd, LPCTSTR lpszBitmap, UINT nDelayTime = 1000);

// 实现
protected:
    virtual BOOL Create(CWnd* pParentWnd);

// 成员
private:
    UINT m_nDelayTime;      // 延时时间
    HBITMAP m_hBitmap;      // 位图句柄

    static CSplashWnd* m_pSplashWnd;

// 生成的消息映射函数
public:
    afx_msg void OnPaint();
    afx_msg void OnTimer(UINT /*nIDEvent*/);
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    DECLARE_MESSAGE_MAP()
};

inline CSplashWnd::CSplashWnd()
    : m_nDelayTime(0)
    , m_hBitmap(NULL)
{
}

inline CSplashWnd::~CSplashWnd()
{
    ::DeleteObject(m_hBitmap);
    _ASSERTE(m_pSplashWnd == this);
    m_pSplashWnd = NULL;
}

inline void CSplashWnd::PostNcDestroy()
{
    delete this;
}

inline BOOL CSplashWnd::TranslateMassage(MSG* pMsg)
{
    if (m_pSplashWnd != NULL)
    {
        // 处理任意键消息
        if (pMsg->message == WM_KEYDOWN || pMsg->message == WM_SYSKEYDOWN || pMsg->message == WM_LBUTTONDOWN
            || pMsg->message == WM_RBUTTONDOWN || pMsg->message == WM_MBUTTONDOWN || pMsg->message == WM_NCLBUTTONDOWN
            || pMsg->message == WM_NCRBUTTONDOWN || pMsg->message == WM_NCMBUTTONDOWN)
        {
            VERIFY(m_pSplashWnd->DestroyWindow());
            return TRUE;
        }
    }

    return FALSE;   // 返回 FALSE 表示消息没有处理
}

inline void CSplashWnd::ShowSplashWnd(CWnd* pParentWnd, LPCTSTR lpszBitmap, UINT nDelayTime)
{
    ShowSplashWnd(pParentWnd, ::LoadBitmap(AfxGetInstanceHandle(), lpszBitmap), nDelayTime);
}

inline void CSplashWnd::ShowSplashWnd(CWnd* pParentWnd, UINT nIDBitmap, UINT nDelayTime)
{
    ShowSplashWnd(pParentWnd, ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(nIDBitmap)), nDelayTime);
}

inline BOOL CSplashWnd::Create(CWnd* pParentWnd)
{
    BITMAP bmp;
    VERIFY(::GetObject(m_hBitmap, sizeof(BITMAP), &bmp));

    return CreateEx(0, AfxRegisterWndClass(0, ::LoadCursor(NULL, IDC_ARROW)), NULL, WS_POPUP | WS_VISIBLE, 0, 0,
                    bmp.bmWidth, bmp.bmHeight, pParentWnd->GetSafeHwnd(), NULL);
}

inline void CSplashWnd::OnTimer(UINT /*nIDEvent*/)
{
    VERIFY(KillTimer(ID_TIMER));
    VERIFY(DestroyWindow());
}

inline int CSplashWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CWnd::OnCreate(lpCreateStruct) == -1)
        return -1;

    CenterWindow();
    SetTimer(ID_TIMER, m_nDelayTime, NULL);

    return 0;
}


///////////////////////////////////////////////////////////////////////////////
// CSplashDlg 类的定义
//

class CSplashDlg : public CDialog
{
// 构造/销毁
protected:
    CSplashDlg();
    virtual ~CSplashDlg();

// 重写
protected:
    virtual BOOL OnInitDialog();
    virtual void PostNcDestroy();

// 操作
public:
    // 处理任意键消息
    static BOOL TranslateMassage(MSG* pMsg);

    // 显示窗口
    static void ShowSplashWnd(CWnd* pParentWnd, UINT nIDTemplate, UINT nIDBitmap, UINT nDelayTime = 1000);
    static void ShowSplashWnd(CWnd* pParentWnd, UINT nIDTemplate, LPCTSTR lpszBitmap, UINT nDelayTime = 1000);
    static void ShowSplashWnd(CWnd* pParentWnd, UINT nIDTemplate, HBITMAP hBitmap = NULL, UINT nDelayTime = 1000);

// 成员
private:
    UINT m_nDelayTime;      // 延时时间
    HBITMAP m_hBitmap;      // 显示的位图句柄

    static CSplashDlg* m_pSplashDlg;

// 生成的消息映射函数
public:
    afx_msg void OnTimer(UINT /*nIDEvent*/);
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    DECLARE_MESSAGE_MAP()
};

inline CSplashDlg::CSplashDlg()
    : m_nDelayTime(0)
    , m_hBitmap(NULL)
{
}

inline CSplashDlg::~CSplashDlg()
{
    ::DeleteObject(m_hBitmap);
    _ASSERTE(m_pSplashDlg == this);
    m_pSplashDlg = NULL;
}

inline void CSplashDlg::PostNcDestroy()
{
    delete this;
}

inline BOOL CSplashDlg::TranslateMassage(MSG* pMsg)
{
    if (m_pSplashDlg != NULL)
    {
        // 处理任意键消息
        if (pMsg->message == WM_KEYDOWN || pMsg->message == WM_SYSKEYDOWN || pMsg->message == WM_LBUTTONDOWN || 
            pMsg->message == WM_RBUTTONDOWN || pMsg->message == WM_MBUTTONDOWN || pMsg->message == WM_NCLBUTTONDOWN || 
            pMsg->message == WM_NCRBUTTONDOWN || pMsg->message == WM_NCMBUTTONDOWN)
        {
            VERIFY(m_pSplashDlg->DestroyWindow());
            return TRUE;
        }
    }

    return FALSE;   // 返回 FALSE 表示消息没有处理
}

inline void CSplashDlg::ShowSplashWnd(CWnd* pParentWnd, UINT nIDTemplate, LPCTSTR lpszBitmap, UINT nDelayTime)
{
    ShowSplashWnd(pParentWnd, nIDTemplate, ::LoadBitmap(AfxGetInstanceHandle(), lpszBitmap), nDelayTime);
}

inline void CSplashDlg::ShowSplashWnd(CWnd* pParentWnd, UINT nIDTemplate, UINT nIDBitmap, UINT nDelayTime)
{
    ShowSplashWnd(pParentWnd, nIDTemplate, ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(nIDBitmap)), nDelayTime);
}

inline void CSplashDlg::OnTimer(UINT /*nIDEvent*/)
{
    VERIFY(KillTimer(ID_TIMER));
    VERIFY(DestroyWindow());
}

} // namespace afxctrls

#endif  // __SPLASH_H__