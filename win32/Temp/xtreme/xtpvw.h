///////////////////////////////////////////////////////////////////////////////
// xtpvw.h : 头文件
//
// Copyright(c) 2012, Garfield. All rights reserved.
// Author  : Garfield
// Version : 1.0
// Finish Time : 2007/11/15

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __XTPVW_H__
#define __XTPVW_H__

///////////////////////////////////////////////////////////////////////////////
// Classes in this file:
//
// CXTPreviewView

namespace XTP {

///////////////////////////////////////////////////////////////////////////////
// CXTPreviewView 类的定义
//

class CXTPreviewView : public CPreviewView
{
    DECLARE_DYNCREATE(CXTPreviewView)
// 构造
protected:
    CXTPreviewView();

// 操作
public:
    virtual ~CXTPreviewView();
#ifdef _DEBUG
    void AssertValid() const;
    void Dump(CDumpContext& dc) const;
#endif

// 实现
protected:
    void UpdateNumPageIcon();
    CXTPControlButton* FindToolbarButton(UINT nCmdID);

// 成员
protected:
    CXTPToolBar m_wndToolBar;

// 生成的消息映射函数
protected:
    afx_msg void OnUpdateNumPageChange(CCmdUI* pCmdUI);
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnWindowPosChanged(LPWINDOWPOS pWndPos);
    DECLARE_MESSAGE_MAP()
};

inline CXTPreviewView::CXTPreviewView()
{
}

inline CXTPreviewView::~CXTPreviewView()
{
}

inline void CXTPreviewView::UpdateNumPageIcon()
{
    ASSERT_VALID(this);
    CXTPControl* pNumPage = FindToolbarButton(AFX_ID_PREVIEW_NUMPAGE);
    ASSERT_VALID(pNumPage);

    UINT nPages = (m_nZoomState == ZOOM_OUT ? m_nPages : m_nZoomOutPages);
    pNumPage->SetIconId(nPages != 1 ? AFX_IDS_ONEPAGE : AFX_IDS_TWOPAGE);
}

inline CXTPControlButton* CXTPreviewView::FindToolbarButton(UINT nCmdID)
{
    return static_cast<CXTPControlButton*>(m_wndToolBar.GetControls()->FindControl(xtpControlButton, nCmdID, TRUE, FALSE));
}

inline void CXTPreviewView::OnUpdateNumPageChange(CCmdUI* pCmdUI)
{
    CPreviewView::OnUpdateNumPageChange(pCmdUI);
    UpdateNumPageIcon();
}

}  // namespace XTP

#endif  // __XTPVW_H__