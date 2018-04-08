// mainfrm.cpp : CMainFrame ���ʵ��
//

#include "stdafx.h"
#include "mainfrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ȫ�ֱ���
static LPCTSTR lpszWindowPos    = _T("Position");
static LPCTSTR lpszWindowState  = _T("Windows State");
static LPCTSTR lpszMaximized    = _T("Maximized");
static LPCTSTR lpszMinimized    = _T("Minimized");
static LPCTSTR lpszControlState = _T("Control State");

static UINT indicators[] =
{
    ID_SEPARATOR,           // ״̬��ָʾ��
    ID_INDICATOR_CAPS,
    ID_INDICATOR_NUM,
    ID_INDICATOR_SCRL,
};

// CMainFrame ���ʵ��

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
    ON_WM_CREATE()
    ON_WM_DESTROY()
END_MESSAGE_MAP()

// CMainFrame ����/����

CMainFrame::CMainFrame()
    : m_bFirstShow(TRUE)
{
}

CMainFrame::~CMainFrame()
{
}

// CMainFrame ��Ա����

BOOL CMainFrame::CreateReBar()
{
    if (!m_wndReBar.Create(this) || !m_wndReBar.AddBar(&m_wndToolBar, NULL, NULL, RBBS_GRIPPERALWAYS | RBBS_FIXEDBMP | RBBS_USECHEVRON))
    {
        TRACE0("δ�ܴ��� ReBar\n");
        return FALSE;
    }

    return TRUE;
}

BOOL CMainFrame::CreateToolBar()
{
    if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_ALIGN_TOP | CBRS_TOOLTIPS | CBRS_FLYBY)
        || !m_wndToolBar.LoadToolBar(IDR_MAINFRAME)) 
    {
        TRACE0("δ�ܴ���������\n");
        return FALSE;
    }

    // ���ù���������
    m_wndToolBar.SetWindowText(_T("������"));
    return TRUE;
}

BOOL CMainFrame::CreateStatusBar()
{
    if (!m_wndStatusBar.Create(this) || !m_wndStatusBar.SetIndicators(indicators, sizeof(indicators) / sizeof(UINT)))
    {
        TRACE0("δ�ܴ���״̬��\n");
        return FALSE;
    }

    // ��������״̬���ķ��
    UINT nID = 0, nStyle = 0;
    int cxWidth = 0;
    m_wndStatusBar.GetPaneInfo(0, nID, nStyle, cxWidth);
    m_wndStatusBar.SetPaneInfo(0, nID, SBPS_STRETCH | SBPS_NORMAL, cxWidth);

    return TRUE;
}

// CMainFrame ��д

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
    if( !CFrameWnd::PreCreateWindow(cs) )
        return FALSE;
    
    // �ڴ˴�ͨ���޸� CREATESTRUCT cs ���޸Ĵ��������ʽ
    
    return TRUE;
}

void CMainFrame::ActivateFrame(int /*nCmdShow*/)
{
    if (m_bFirstShow)
    {
        m_bFirstShow = FALSE;
        WINDOWPLACEMENT wndPlace = { sizeof(WINDOWPLACEMENT), 0, SW_SHOWNORMAL, { 0, 0 }, { 0, 0 }, { 50, 50, 690, 530 } };

        // ��ô��ڵ�λ��
        CWinApp* pApp = AfxGetApp();
        ASSERT_VALID(pApp);

        CString strWndPos = pApp->GetProfileString(lpszWindowState, lpszWindowPos);
        if (!strWndPos.IsEmpty())
        {
            ::SetRect(&wndPlace.rcNormalPosition, ::_tstoi(strWndPos), ::_tstoi(static_cast<LPCTSTR>(strWndPos) + 5),
                      ::_tstoi(static_cast<LPCTSTR>(strWndPos) + 10), ::_tstoi(static_cast<LPCTSTR>(strWndPos) + 15));
        }

        // ��ô��ڵ����/��С��״̬
        UINT nMaximized = pApp->GetProfileInt(lpszWindowState, lpszMaximized, 0);
        UINT nMinimized = pApp->GetProfileInt(lpszWindowState, lpszMinimized, 0);

        // ���ô��ڵĲ���
        wndPlace.showCmd = (nMinimized ? SW_SHOWMINNOACTIVE : (nMaximized ? SW_SHOWMAXIMIZED : SW_SHOWNORMAL));
        wndPlace.flags   = (nMaximized ? WPF_RESTORETOMAXIMIZED : WPF_SETMINPOSITION);
        wndPlace.ptMaxPosition.x = -::GetSystemMetrics(SM_CXBORDER);
        wndPlace.ptMaxPosition.y = -::GetSystemMetrics(SM_CYBORDER);

        SetWindowPlacement(&wndPlace);
        LoadBarState(lpszControlState);
    }
}

// CMainFrame ���

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
    CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
    CFrameWnd::Dump(dc);
}
#endif //_DEBUG

// CMainFrame ��Ϣ�������

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
        return -1;

    // ������������IE ���Ŀ�������״̬��
    if (!CreateToolBar() || !CreateReBar() || !CreateStatusBar())
        return -1;

    return 0;
}

void CMainFrame::OnDestroy()
{
    // ��ô��ڵĲ���
    WINDOWPLACEMENT wndPlace = { sizeof(WINDOWPLACEMENT) };
    GetWindowPlacement(&wndPlace);

    // Ĭ��Ϊ����������ʾ
    BOOL bMaximized = FALSE;
    BOOL bMinimized = FALSE;

    if (wndPlace.showCmd == SW_SHOWMAXIMIZED)       // ���������ʾ
    {
        bMinimized = FALSE;
        bMaximized = TRUE;
    }
    else if (wndPlace.showCmd == SW_SHOWMINIMIZED)  // ������С����ʾ
    {
        bMinimized = TRUE;
        bMaximized = (wndPlace.flags ? TRUE : FALSE);
    }

    // ��ʽ�����ڵ�λ��
    CString strWndPos;
    strWndPos.Format(_T("%04d %04d %04d %04d"), wndPlace.rcNormalPosition.left, wndPlace.rcNormalPosition.top,
                     wndPlace.rcNormalPosition.right, wndPlace.rcNormalPosition.bottom);

    // ��¼���ڵĲ���
    CWinApp* pApp = AfxGetApp();
    ASSERT_VALID(pApp);

    pApp->WriteProfileString(lpszWindowState, lpszWindowPos, strWndPos);
    pApp->WriteProfileInt(lpszWindowState, lpszMaximized, bMaximized);
    pApp->WriteProfileInt(lpszWindowState, lpszMinimized, bMinimized);
    SaveBarState(lpszControlState);

    CFrameWnd::OnDestroy();
}