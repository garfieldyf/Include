///////////////////////////////////////////////////////////////////////////////
// xtpvw.cpp : ʵ���ļ�
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
// CXTPreviewView ���ʵ��
//

IMPLEMENT_DYNCREATE(CXTPreviewView, CPreviewView)

BEGIN_MESSAGE_MAP(CXTPreviewView, CPreviewView)
    ON_WM_CREATE()
    ON_WM_WINDOWPOSCHANGED()
    ON_UPDATE_COMMAND_UI(AFX_ID_PREVIEW_NUMPAGE, &CXTPreviewView::OnUpdateNumPageChange)
END_MESSAGE_MAP()

// CXTPreviewView ���

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

// CXTPreviewView ��Ϣ�������

int CXTPreviewView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CPreviewView::OnCreate(lpCreateStruct) == -1)
        return -1;

    // ����������
    ASSERT_VALID(m_pToolBar);
    if (!m_wndToolBar.CreateToolBar(WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS, m_pToolBar)
        || !m_wndToolBar.LoadToolBar(XTP_IDR_PREVIEW_TOOLBAR))
    {
        TRACE0("����������ʧ��\n");
        return -1;
    }

    FindToolbarButton(AFX_IDS_ONEPAGE)->SetVisible(FALSE);                          // ���ء�һҳ����ť
    FindToolbarButton(AFX_IDS_TWOPAGE)->SetVisible(FALSE);                          // ���ء���ҳ����ť
    FindToolbarButton(AFX_ID_PREVIEW_PRINT)->SetStyle(xtpButtonIconAndCaption);     // ���á���ӡ����ť���
    FindToolbarButton(AFX_ID_PREVIEW_CLOSE)->SetStyle(xtpButtonCaption);            // ���á��رա���ť���

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
        // ��ø����ڵĿͻ�����С
        CFrameWnd* pFrame = m_pToolBar->GetDockingFrame();
        ASSERT_VALID(pFrame);

        CRect rcClient;
        pFrame->GetClientRect(&rcClient);

        // ���㹤����ʵ�ʴ�С
        CSize size = m_wndToolBar.CalcDockingLayout(rcClient.Width(), LM_HORZDOCK | LM_HORZ | LM_COMMIT);
        size.cx = rcClient.Width();

        // �ƶ�������
        m_pToolBar->m_sizeDefault.cy = size.cy;
        m_wndToolBar.MoveWindow(0, 0, size.cx, size.cy);
        m_wndToolBar.Invalidate(FALSE);
    }
}

}  // namespace XTP