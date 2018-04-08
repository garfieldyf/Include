///////////////////////////////////////////////////////////////////////////////
// xtpvw.cpp : 实现文件
//
// Copyright(c) 2012, Garfield. All rights reserved.
// Author  : Garfield
// Version : 1.0
// Finish Time : 2007/11/15

#include "stdafx.h"
#include "xtpvw.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace XTP {

///////////////////////////////////////////////////////////////////////////////
// CXTPreviewView 类的实现
//

IMPLEMENT_DYNCREATE(CXTPreviewView, CPreviewView)

BEGIN_MESSAGE_MAP(CXTPreviewView, CPreviewView)
    ON_WM_CREATE()
    ON_WM_WINDOWPOSCHANGED()
    ON_UPDATE_COMMAND_UI(AFX_ID_PREVIEW_NUMPAGE, &CXTPreviewView::OnUpdateNumPageChange)
END_MESSAGE_MAP()

// CXTPreviewView 诊断

#ifdef _DEBUG
void CXTPreviewView::AssertValid() const
{
    CPreviewView::AssertValid();
}

void CXTPreviewView::Dump(CDumpContext& dc) const
{
    CPreviewView::Dump(dc);
}
#endif //_DEBUG

// CXTPreviewView 消息处理程序

int CXTPreviewView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CPreviewView::OnCreate(lpCreateStruct) == -1)
        return -1;

    // 创建工具栏
    ASSERT_VALID(m_pToolBar);
    if (!m_wndToolBar.CreateToolBar(WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS, m_pToolBar)
        || !m_wndToolBar.LoadToolBar(XTP_IDR_PREVIEW_TOOLBAR))
    {
        TRACE0("创建工具栏失败\n");
        return -1;
    }

    FindToolbarButton(AFX_IDS_ONEPAGE)->SetVisible(FALSE);                          // 隐藏“一页”按钮
    FindToolbarButton(AFX_IDS_TWOPAGE)->SetVisible(FALSE);                          // 隐藏“两页”按钮
    FindToolbarButton(AFX_ID_PREVIEW_PRINT)->SetStyle(xtpButtonIconAndCaption);     // 设置“打印”按钮风格
    FindToolbarButton(AFX_ID_PREVIEW_CLOSE)->SetStyle(xtpButtonCaption);            // 设置“关闭”按钮风格

    m_wndToolBar.SetOwner(this);
    UpdateNumPageIcon();

    return 0;
}

void CXTPreviewView::OnWindowPosChanged(LPWINDOWPOS pWndPos)
{
    CPreviewView::OnWindowPosChanged(pWndPos);
    ASSERT(pWndPos);

    if (::IsWindow(m_wndToolBar.m_hWnd))
    {
        // 获得父窗口的客户区大小
        CFrameWnd* pFrame = m_pToolBar->GetDockingFrame();
        ASSERT_VALID(pFrame);

        CRect rcClient;
        pFrame->GetClientRect(&rcClient);

        // 计算工具栏实际大小
        CSize size = m_wndToolBar.CalcDockingLayout(rcClient.Width(), LM_HORZDOCK | LM_HORZ | LM_COMMIT);
        size.cx = rcClient.Width();

        // 移动工具栏
        m_pToolBar->m_sizeDefault.cy = size.cy;
        m_wndToolBar.MoveWindow(0, 0, size.cx, size.cy);
        m_wndToolBar.Invalidate(FALSE);
    }
}

}  // namespace XTP