// mainfrm.cpp : CMainFrame 类的实现
//

#include "stdafx.h"
#include "mainfrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 全局变量
static LPCTSTR lpszWindowPos    = _T("Position");
static LPCTSTR lpszWindowState  = _T("Windows State");
static LPCTSTR lpszMaximized    = _T("Maximized");
static LPCTSTR lpszMinimized    = _T("Minimized");
static LPCTSTR lpszControlState = _T("Control State");

static UINT indicators[] =
{
    ID_SEPARATOR,           // 状态行指示器
    ID_INDICATOR_CAPS,
    ID_INDICATOR_NUM,
    ID_INDICATOR_SCRL,
};

// CMainFrame 类的实现

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
    ON_WM_CREATE()
    ON_WM_DESTROY()
END_MESSAGE_MAP()

// CMainFrame 构造/析构

CMainFrame::CMainFrame()
    : m_bFirstShow(TRUE)
{
}

CMainFrame::~CMainFrame()
{
}

// CMainFrame 成员函数

BOOL CMainFrame::CreateReBar()
{
    if (!m_wndReBar.Create(this) || !m_wndReBar.AddBar(&m_wndToolBar, NULL, NULL, RBBS_GRIPPERALWAYS | RBBS_FIXEDBMP | RBBS_USECHEVRON))
    {
        TRACE0("未能创建 ReBar\n");
        return FALSE;
    }

    return TRUE;
}

BOOL CMainFrame::CreateToolBar()
{
    if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_ALIGN_TOP | CBRS_TOOLTIPS | CBRS_FLYBY)
        || !m_wndToolBar.LoadToolBar(IDR_MAINFRAME)) 
    {
        TRACE0("未能创建工具栏\n");
        return FALSE;
    }

    // 设置工具栏标题
    m_wndToolBar.SetWindowText(_T("工具栏"));
    return TRUE;
}

BOOL CMainFrame::CreateStatusBar()
{
    if (!m_wndStatusBar.Create(this) || !m_wndStatusBar.SetIndicators(indicators, sizeof(indicators) / sizeof(UINT)))
    {
        TRACE0("未能创建状态栏\n");
        return FALSE;
    }

    // 重新设置状态条的风格
    UINT nID = 0, nStyle = 0;
    int cxWidth = 0;
    m_wndStatusBar.GetPaneInfo(0, nID, nStyle, cxWidth);
    m_wndStatusBar.SetPaneInfo(0, nID, SBPS_STRETCH | SBPS_NORMAL, cxWidth);

    return TRUE;
}

// CMainFrame 重写

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
    if( !CFrameWnd::PreCreateWindow(cs) )
        return FALSE;
    
    // 在此处通过修改 CREATESTRUCT cs 来修改窗口类或样式
    
    return TRUE;
}

void CMainFrame::ActivateFrame(int /*nCmdShow*/)
{
    if (m_bFirstShow)
    {
        m_bFirstShow = FALSE;
        WINDOWPLACEMENT wndPlace = { sizeof(WINDOWPLACEMENT), 0, SW_SHOWNORMAL, { 0, 0 }, { 0, 0 }, { 50, 50, 690, 530 } };

        // 获得窗口的位置
        CWinApp* pApp = AfxGetApp();
        ASSERT_VALID(pApp);

        CString strWndPos = pApp->GetProfileString(lpszWindowState, lpszWindowPos);
        if (!strWndPos.IsEmpty())
        {
            ::SetRect(&wndPlace.rcNormalPosition, ::_tstoi(strWndPos), ::_tstoi(static_cast<LPCTSTR>(strWndPos) + 5),
                      ::_tstoi(static_cast<LPCTSTR>(strWndPos) + 10), ::_tstoi(static_cast<LPCTSTR>(strWndPos) + 15));
        }

        // 获得窗口的最大化/最小化状态
        UINT nMaximized = pApp->GetProfileInt(lpszWindowState, lpszMaximized, 0);
        UINT nMinimized = pApp->GetProfileInt(lpszWindowState, lpszMinimized, 0);

        // 设置窗口的布局
        wndPlace.showCmd = (nMinimized ? SW_SHOWMINNOACTIVE : (nMaximized ? SW_SHOWMAXIMIZED : SW_SHOWNORMAL));
        wndPlace.flags   = (nMaximized ? WPF_RESTORETOMAXIMIZED : WPF_SETMINPOSITION);
        wndPlace.ptMaxPosition.x = -::GetSystemMetrics(SM_CXBORDER);
        wndPlace.ptMaxPosition.y = -::GetSystemMetrics(SM_CYBORDER);

        SetWindowPlacement(&wndPlace);
        LoadBarState(lpszControlState);
    }
}

// CMainFrame 诊断

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

// CMainFrame 消息处理程序

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
        return -1;

    // 创建工具栏、IE 风格的控制条和状态栏
    if (!CreateToolBar() || !CreateReBar() || !CreateStatusBar())
        return -1;

    return 0;
}

void CMainFrame::OnDestroy()
{
    // 获得窗口的布局
    WINDOWPLACEMENT wndPlace = { sizeof(WINDOWPLACEMENT) };
    GetWindowPlacement(&wndPlace);

    // 默认为窗口正常显示
    BOOL bMaximized = FALSE;
    BOOL bMinimized = FALSE;

    if (wndPlace.showCmd == SW_SHOWMAXIMIZED)       // 窗口最大化显示
    {
        bMinimized = FALSE;
        bMaximized = TRUE;
    }
    else if (wndPlace.showCmd == SW_SHOWMINIMIZED)  // 窗口最小化显示
    {
        bMinimized = TRUE;
        bMaximized = (wndPlace.flags ? TRUE : FALSE);
    }

    // 格式化窗口的位置
    CString strWndPos;
    strWndPos.Format(_T("%04d %04d %04d %04d"), wndPlace.rcNormalPosition.left, wndPlace.rcNormalPosition.top,
                     wndPlace.rcNormalPosition.right, wndPlace.rcNormalPosition.bottom);

    // 记录窗口的布局
    CWinApp* pApp = AfxGetApp();
    ASSERT_VALID(pApp);

    pApp->WriteProfileString(lpszWindowState, lpszWindowPos, strWndPos);
    pApp->WriteProfileInt(lpszWindowState, lpszMaximized, bMaximized);
    pApp->WriteProfileInt(lpszWindowState, lpszMinimized, bMinimized);
    SaveBarState(lpszControlState);

    CFrameWnd::OnDestroy();
}