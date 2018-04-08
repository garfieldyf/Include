///////////////////////////////////////////////////////////////////////////////
// splash.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "splash.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace afxctrls {

///////////////////////////////////////////////////////////////////////////////
// CSplashWnd ���ʵ��
//

CSplashWnd* CSplashWnd::m_pSplashWnd;

BEGIN_MESSAGE_MAP(CSplashWnd, CWnd)
    ON_WM_PAINT()
    ON_WM_TIMER()
    ON_WM_CREATE()
END_MESSAGE_MAP()

// CSplashWnd ��̬����

void CSplashWnd::ShowSplashWnd(CWnd* pParentWnd, HBITMAP hBitmap, UINT nDelayTime)
{
    _ASSERTE(hBitmap);
    _ASSERTE(pParentWnd);

    // �жϴ����Ƿ���Ч
    if (m_pSplashWnd != NULL)
        return;

    // ��������
    m_pSplashWnd = new CSplashWnd;
    m_pSplashWnd->m_hBitmap = hBitmap;
    m_pSplashWnd->m_nDelayTime = nDelayTime;

    // ��������
    if (!m_pSplashWnd->Create(pParentWnd))
    {
        delete m_pSplashWnd;
        return;
    }

    // ��ʾ����
    m_pSplashWnd->ShowWindow(SW_SHOW);
    m_pSplashWnd->UpdateWindow();
}

// CSplashWnd ��Ϣ�������

void CSplashWnd::OnPaint()
{
    CPaintDC dc(this);

    // ����һ������ʾ���ݵ��ڴ� DC
    CDC dcMem;
    if (!dcMem.CreateCompatibleDC(&dc))
        return;

    // ��ÿͻ�����С
    CRect rcClient;
    GetClientRect(&rcClient);

    // ��λͼ���ص��ڴ� DC
    HGDIOBJ hOldBitmap = dcMem.SelectObject(m_hBitmap);
    _ASSERTE(hOldBitmap);

    // ���ڴ��е�λͼ���Ƶ�������
    dc.BitBlt(rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height(), &dcMem, 0, 0, SRCCOPY);
    dcMem.SelectObject(hOldBitmap);
}


///////////////////////////////////////////////////////////////////////////////
// CSplashDlg ���ʵ��
//

CSplashDlg* CSplashDlg::m_pSplashDlg;

BEGIN_MESSAGE_MAP(CSplashDlg, CDialog)
    ON_WM_TIMER()
    ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

// CSplashDlg ��̬����

void CSplashDlg::ShowSplashWnd(CWnd* pParentWnd, UINT nIDTemplate, HBITMAP hBitmap, UINT nDelayTime)
{
    _ASSERTE(pParentWnd);

    // �жϴ����Ƿ���Ч
    if (m_pSplashDlg != NULL)
        return;

    // ��������
    m_pSplashDlg = new CSplashDlg;
    m_pSplashDlg->m_hBitmap = hBitmap;
    m_pSplashDlg->m_nDelayTime = nDelayTime;

    // ��������
    if (!m_pSplashDlg->Create(nIDTemplate, pParentWnd))
    {
        delete m_pSplashDlg;
        return;
    }

    // ��ʾ����
    m_pSplashDlg->ShowWindow(SW_SHOW);
    m_pSplashDlg->UpdateWindow();
}

// CSplashDlg ��Ϣ�������

BOOL CSplashDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // ���Ĵ��ڴ�С
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
        // ���Ĵ��ڱ���
        if (nCtlColor == CTLCOLOR_DLG)
            hBrush = ::CreatePatternBrush(m_hBitmap);

        // ���ľ�̬�ı��򱳾�ģʽ
        if (nCtlColor == CTLCOLOR_STATIC)
        {
            hBrush = static_cast<HBRUSH>(::GetStockObject(NULL_BRUSH));
            pDC->SetBkMode(TRANSPARENT);
        }
    }

    return hBrush;
}

}  // namespace afxctrls