/////////////////////////////////////////////////////////////////////
// splash.h : ͷ�ļ�
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
// CSplashWnd ��Ķ���
//

class CSplashWnd : public CWnd
{
// ����/����
protected:
    CSplashWnd();
    virtual ~CSplashWnd();

// ��д
protected:
    virtual void PostNcDestroy();

// ����
public:
    // �����������Ϣ
    static BOOL TranslateMassage(MSG* pMsg);

    // ��ʾ����
    static void ShowSplashWnd(CWnd* pParentWnd, UINT nIDBitmap, UINT nDelayTime = 1000);
    static void ShowSplashWnd(CWnd* pParentWnd, HBITMAP hBitmap, UINT nDelayTime = 1000);
    static void ShowSplashWnd(CWnd* pParentWnd, LPCTSTR lpszBitmap, UINT nDelayTime = 1000);

// ʵ��
protected:
    virtual BOOL Create(CWnd* pParentWnd);

// ��Ա
private:
    UINT m_nDelayTime;      // ��ʱʱ��
    HBITMAP m_hBitmap;      // λͼ���

    static CSplashWnd* m_pSplashWnd;

// ���ɵ���Ϣӳ�亯��
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
        // �����������Ϣ
        if (pMsg->message == WM_KEYDOWN || pMsg->message == WM_SYSKEYDOWN || pMsg->message == WM_LBUTTONDOWN
            || pMsg->message == WM_RBUTTONDOWN || pMsg->message == WM_MBUTTONDOWN || pMsg->message == WM_NCLBUTTONDOWN
            || pMsg->message == WM_NCRBUTTONDOWN || pMsg->message == WM_NCMBUTTONDOWN)
        {
            VERIFY(m_pSplashWnd->DestroyWindow());
            return TRUE;
        }
    }

    return FALSE;   // ���� FALSE ��ʾ��Ϣû�д���
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
// CSplashDlg ��Ķ���
//

class CSplashDlg : public CDialog
{
// ����/����
protected:
    CSplashDlg();
    virtual ~CSplashDlg();

// ��д
protected:
    virtual BOOL OnInitDialog();
    virtual void PostNcDestroy();

// ����
public:
    // �����������Ϣ
    static BOOL TranslateMassage(MSG* pMsg);

    // ��ʾ����
    static void ShowSplashWnd(CWnd* pParentWnd, UINT nIDTemplate, UINT nIDBitmap, UINT nDelayTime = 1000);
    static void ShowSplashWnd(CWnd* pParentWnd, UINT nIDTemplate, LPCTSTR lpszBitmap, UINT nDelayTime = 1000);
    static void ShowSplashWnd(CWnd* pParentWnd, UINT nIDTemplate, HBITMAP hBitmap = NULL, UINT nDelayTime = 1000);

// ��Ա
private:
    UINT m_nDelayTime;      // ��ʱʱ��
    HBITMAP m_hBitmap;      // ��ʾ��λͼ���

    static CSplashDlg* m_pSplashDlg;

// ���ɵ���Ϣӳ�亯��
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
        // �����������Ϣ
        if (pMsg->message == WM_KEYDOWN || pMsg->message == WM_SYSKEYDOWN || pMsg->message == WM_LBUTTONDOWN || 
            pMsg->message == WM_RBUTTONDOWN || pMsg->message == WM_MBUTTONDOWN || pMsg->message == WM_NCLBUTTONDOWN || 
            pMsg->message == WM_NCRBUTTONDOWN || pMsg->message == WM_NCMBUTTONDOWN)
        {
            VERIFY(m_pSplashDlg->DestroyWindow());
            return TRUE;
        }
    }

    return FALSE;   // ���� FALSE ��ʾ��Ϣû�д���
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