///////////////////////////////////////////////////////////////////////////////
// splash.cpp : 实现文件
//

#include "stdafx.h"
#include "splash.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace afxctrls {

///////////////////////////////////////////////////////////////////////////////
// CSplashWnd 类的实现
//

CSplashWnd* CSplashWnd::m_pSplashWnd;

BEGIN_MESSAGE_MAP(CSplashWnd, CWnd)
    ON_WM_PAINT()
    ON_WM_TIMER()
    ON_WM_CREATE()
END_MESSAGE_MAP()

// CSplashWnd 静态函数

void CSplashWnd::ShowSplashWnd(CWnd* pParentWnd, HBITMAP hBitmap, UINT nDelayTime)
{
    _ASSERTE(hBitmap);
    _ASSERTE(pParentWnd);

    // 判断窗口是否有效
    if (m_pSplashWnd != NULL)
        return;

    // 创建对象
    m_pSplashWnd = new CSplashWnd;
    m_pSplashWnd->m_hBitmap = hBitmap;
    m_pSplashWnd->m_nDelayTime = nDelayTime;

    // 创建窗口
    if (!m_pSplashWnd->Create(pParentWnd))
    {
        delete m_pSplashWnd;
        return;
    }

    // 显示窗口
    m_pSplashWnd->ShowWindow(SW_SHOW);
    m_pSplashWnd->UpdateWindow();
}

// CSplashWnd 消息处理程序

void CSplashWnd::OnPaint()
{
    CPaintDC dc(this);

    // 创建一个与显示兼容的内存 DC
    CDC dcMem;
    if (!dcMem.CreateCompatibleDC(&dc))
        return;

    // 获得客户区大小
    CRect rcClient;
    GetClientRect(&rcClient);

    // 将位图加载到内存 DC
    HGDIOBJ hOldBitmap = dcMem.SelectObject(m_hBitmap);
    _ASSERTE(hOldBitmap);

    // 将内存中的位图绘制到窗口上
    dc.BitBlt(rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height(), &dcMem, 0, 0, SRCCOPY);
    dcMem.SelectObject(hOldBitmap);
}


///////////////////////////////////////////////////////////////////////////////
// CSplashDlg 类的实现
//

CSplashDlg* CSplashDlg::m_pSplashDlg;

BEGIN_MESSAGE_MAP(CSplashDlg, CDialog)
    ON_WM_TIMER()
    ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

// CSplashDlg 静态函数

void CSplashDlg::ShowSplashWnd(CWnd* pParentWnd, UINT nIDTemplate, HBITMAP hBitmap, UINT nDelayTime)
{
    _ASSERTE(pParentWnd);

    // 判断窗口是否有效
    if (m_pSplashDlg != NULL)
        return;

    // 创建对象
    m_pSplashDlg = new CSplashDlg;
    m_pSplashDlg->m_hBitmap = hBitmap;
    m_pSplashDlg->m_nDelayTime = nDelayTime;

    // 创建窗口
    if (!m_pSplashDlg->Create(nIDTemplate, pParentWnd))
    {
        delete m_pSplashDlg;
        return;
    }

    // 显示窗口
    m_pSplashDlg->ShowWindow(SW_SHOW);
    m_pSplashDlg->UpdateWindow();
}

// CSplashDlg 消息处理程序

BOOL CSplashDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // 更改窗口大小
    if (m_hBitmap != NULL)
    {
        BITMAP bmp;
        VERIFY(::GetObject(m_hBitmap, sizeof(BITMAP), &bmp));
        MoveWindow(0, 0, bmp.bmWidth, bmp.bmHeight, FALSE);
    }

    CenterWindow();
    SetTimer(ID_TIMER, m_nDelayTime, NULL);

    return TRUE;
}

HBRUSH CSplashDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hBrush = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

    if (m_hBitmap != NULL)
    {
        // 更改窗口背景
        if (nCtlColor == CTLCOLOR_DLG)
            hBrush = ::CreatePatternBrush(m_hBitmap);

        // 更改静态文本框背景模式
        if (nCtlColor == CTLCOLOR_STATIC)
        {
            hBrush = static_cast<HBRUSH>(::GetStockObject(NULL_BRUSH));
            pDC->SetBkMode(TRANSPARENT);
        }
    }

    return hBrush;
}

}  // namespace afxctrls