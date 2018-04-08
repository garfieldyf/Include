///////////////////////////////////////////////////////////////////////////////
// xtpvw.inl : 内联文件
//
// Copyright(c) 2012, Garfield. All rights reserved.
// Author  : Garfield
// Version : 1.0
// Finish Time : 2007/11/15

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __XTPVW_INL__
#define __XTPVW_INL__

namespace XTP {

///////////////////////////////////////////////////////////////////////////////
// CXTPreviewView 内联函数
//

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

#endif  // __XTPVW_INL__