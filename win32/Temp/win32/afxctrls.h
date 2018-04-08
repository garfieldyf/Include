///////////////////////////////////////////////////////////////////////////////
// afxctrls.h
//
// Copyright(c) 2013, Garfield. All rights reserved.
// Author  : Garfield
// Version : 4.5
// Finish Time : 2007/1/21
// Summary:
//  _AFX_SUPPORT_CCE_TEXTCOLOR          Enable CCueEdit set/get text color
//  _AFX_SUPPORT_CSLV_SORTCHANGED       Enable CSortListCtrl LVN_SORTCHANGED notify message
//  _AFX_SUPPORT_CBB_MOUSENOTIFY        Enable CBitmapButton BN_MOUSELEAVE and BN_MOUSEHOVER notify message
//  _AFX_SUPPORT_CILB_TEXTCOLOR         Enable CImageListBox set/get text color and background

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __AFXCTRLS_H__
#define __AFXCTRLS_H__

#ifndef __STDCTRLS_H__
    #error afxctrls.h requires stdctrls.h to be included first
#endif

#include <afxcmn.h>

///////////////////////////////////////////////////////////////////////////////
// Classes in this file:
//
// CCueEdit
// CLinkCtrl
// CTabSheet
// CImageCtrl
// CSplashWnd
// CPictureCtrl
// CSortListCtrl
// CBitmapButton
// CImageListBox
// CImageLinkCtrl
// CColorComboBox
// CBalloonTipCtrl

namespace afxctrls {

///////////////////////////////////////////////////////////////////////////////
// Interface of the CCueEdit class
//

class CCueEdit : public CEdit
{
    DECLARE_DYNCREATE(CCueEdit)
// Constructors/Destructor
public:
    CCueEdit();
    virtual ~CCueEdit();

// Attributes
public:
    const CString& GetCueText() const;
    void SetCueText(LPCTSTR pszText);   // The pszText parameter can pass pointer to a null-terminated string, or resource ID of the string.

// Data members
public:
#ifdef _AFX_SUPPORT_CCE_TEXTCOLOR
    COLORREF m_clrText;
    COLORREF m_clrCueText;
#endif
    CString  m_strCueText;

// Message map functions
public:
    afx_msg void OnSetFocus(CWnd* pOldWnd);
    afx_msg void OnKillFocus(CWnd* pNewWnd);
    afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
    afx_msg LRESULT OnSetText(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnGetText(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnGetTextLength(WPARAM wParam, LPARAM lParam);
    DECLARE_MESSAGE_MAP()
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CImageLinkCtrl class
//

class CImageLinkCtrl : public CStatic
{
    DECLARE_DYNCREATE(CImageLinkCtrl)
// Constructors/Destructor
public:
    CImageLinkCtrl();
    virtual ~CImageLinkCtrl();

// Overrides
public:
    virtual void PreSubclassWindow();

// Implementation
public:
    BOOL IsValidContent() const;

// Message map functions
public:
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
    DECLARE_MESSAGE_MAP()
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CLinkCtrl class
//

class CLinkCtrl : public CImageLinkCtrl, public atlctrls::CLinkCtrlBase
{
    DECLARE_DYNCREATE(CLinkCtrl)
// Constructors/Destructor
public:
    explicit CLinkCtrl(DWORD dwExStyle = 0);
    virtual ~CLinkCtrl();

// Message map functions
public:
    afx_msg void OnPaint();
    afx_msg void OnEnable(BOOL bEnable);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
    DECLARE_MESSAGE_MAP()
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CTabSheet class
//

class CTabSheet : public CTabCtrl
{
    DECLARE_DYNCREATE(CTabSheet)
// Constructors/Destructor
public:
    CTabSheet();
    virtual ~CTabSheet();

// Overrides
public:
    virtual void PreSubclassWindow();
    virtual BOOL PreTranslateMessage(LPMSG pMsg);

// Operations
public:
    // The pszTitle parameter can pass pointer to a null-terminated string, or resource ID of the string.
    int AddPage(HWND hWndPage, LPCTSTR pszTitle, int nImage = -1, LPARAM lParam = 0L);
    int InsertPage(int nPage, HWND hWndPage, LPCTSTR pszTitle, int nImage = -1, LPARAM lParam = 0L);

// Attributes
public:
    HWND GetActivePage() const;

    HWND GetPage(int nPage) const;
    BOOL SetPage(int nPage, HWND hWnd);

    int GetPageImage(int nPage) const;
    BOOL SetPageImage(int nPage, int nImage);

    DWORD_PTR GetPageData(int nPage) const;
    BOOL SetPageData(int nPage, DWORD_PTR dwData);

    CString GetPageTitle(int nPage) const;
    BOOL SetPageTitle(int nPage, LPCTSTR pszTitle);

// Message map functions
public:
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnSelChange(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg LRESULT OnSetCurSel(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnSetPageRect(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnDeleteItem(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnDeleteAllItems(WPARAM wParam, LPARAM lParam);
    DECLARE_MESSAGE_MAP()
};


#ifdef __IMAGE_H__
///////////////////////////////////////////////////////////////////////////////
// Interface of the CImageCtrl class
//

class CImageCtrl : public CWnd, public atlctrls::CImageCtrlBase
{
    DECLARE_DYNCREATE(CImageCtrl)
// Constructors/Destructor
public:
    CImageCtrl();
    virtual ~CImageCtrl();

// Message map functions
public:
    afx_msg void OnPaint();
    afx_msg void OnDestroy();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    DECLARE_MESSAGE_MAP()
};
#endif  // __IMAGE_H__


///////////////////////////////////////////////////////////////////////////////
// Interface of the CSplashWnd class
//

class CSplashWnd : public CWnd, public atlctrls::CSplashWindowBase
{
    DECLARE_DYNCREATE(CSplashWnd)
// Constructors/Destructor
public:
    CSplashWnd();
    virtual ~CSplashWnd();

// Overrides
public:
    virtual BOOL Create(HBITMAP hBitmap, COLORREF clrMask = CLR_INVALID, CWnd* pParentWnd = NULL);

// Operations
public:
    void SetDrawText(POSITION pos, UINT nIDText = 0, COLORREF clrText = CLR_INVALID, LPLOGFONT pLogFont = NULL, UINT nFormat = (UINT)-1, LPCRECT pRect = NULL);
    void SetDrawText(POSITION pos, LPCTSTR pszText = NULL, COLORREF clrText = CLR_INVALID, LPLOGFONT pLogFont = NULL, UINT nFormat = (UINT)-1, LPCRECT pRect = NULL);

// Message map functions
public:
    afx_msg void OnPaint();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    DECLARE_MESSAGE_MAP()
};


#ifdef __PICTURE_H__
///////////////////////////////////////////////////////////////////////////////
// Interface of the CPictureCtrl class
//

class CPictureCtrl : public CWnd, public atlctrls::CPictureCtrlBase
{
    DECLARE_DYNCREATE(CPictureCtrl)
// Constructors/Destructor
public:
    CPictureCtrl();
    virtual ~CPictureCtrl();

// Message map functions
public:
    afx_msg void OnPaint();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    DECLARE_MESSAGE_MAP()
};
#endif  // __PICTURE_H__


///////////////////////////////////////////////////////////////////////////////
// Interface of the CSortListCtrl class
//

class CSortListCtrl : public CListCtrl, public atlctrls::CSortListCtrlBase
{
    DECLARE_DYNCREATE(CSortListCtrl)
// Constructors/Destructor
public:
    CSortListCtrl();
    virtual ~CSortListCtrl();

// Operations
public:
    BOOL SortItem(int nCol, BOOL bAscending = TRUE);

    // The pszColumnHeading parameter can pass pointer to a null-terminated string, or resource ID of the string.
    int InsertColumnEx(int nCol, const LVCOLUMN* pColumn, int nSortMode = LVSM_TEXT);
    int InsertColumnEx(int nCol, LPCTSTR pszColumnHeading, int nFormat = LVCFMT_LEFT, int nWidth = -1, int nSortMode = LVSM_TEXT, int nSubItem = -1, int nImage = -1, int nOrder = -1);

// Attributes
public:
    int GetSortMode(int nCol) const;
    BOOL SetSortMode(int nCol, int nSortMode);

// Data members
public:
    PFNLVCOMPARE m_pfnCompare;

// Message map functions
public:
    afx_msg void OnColumnClick(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg LRESULT OnGetSortMode(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnSetSortMode(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnDeleteColumn(WPARAM wParam, LPARAM lParam);
    DECLARE_MESSAGE_MAP()
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CBitmapButton class
//

class CBitmapButton : public CButton, public atlctrls::CBitmapButtonBase
{
    DECLARE_DYNCREATE(CBitmapButton)
// Constructors/Destructor
public:
    CBitmapButton();
    virtual ~CBitmapButton();

// Overrides
public:
    virtual void PreSubclassWindow();
    virtual void DrawItem(LPDRAWITEMSTRUCT pDrawItem);

// Operations
public:
    BOOL SizeToContent();
    BOOL SetMaskColor(COLORREF color);
    BOOL PtInWindow(POINT point) const;

// Message map functions
public:
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
    DECLARE_MESSAGE_MAP()
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CImageListBox class
//

class CImageListBox : public CListBox, public atlctrls::CImageListBoxBase
{
    DECLARE_DYNCREATE(CImageListBox)
// Constructors/Destructor
public:
    CImageListBox();
    virtual ~CImageListBox();

// Overrides
public:
    virtual void DrawItem(LPDRAWITEMSTRUCT pDrawItem);

// Operations
public:
    int AddItem(LPCTSTR pszItem, int nImage = -1);
    int InsertItem(int nItem, LPCTSTR pszItem, int nImage = -1);

// Attributes
public:
    HIMAGELIST GetImageList() const;
    BOOL SetImageList(HIMAGELIST hImageList);

// Message map functions
public:
#ifdef _AFX_SUPPORT_CILB_TEXTCOLOR
    afx_msg void OnEnable(BOOL bEnable);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
#endif
    afx_msg LRESULT OnInitStorage(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnDeleteString(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnResetContent(WPARAM wParam, LPARAM lParam);
    DECLARE_MESSAGE_MAP()
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CColorComboBox class
//

class CColorComboBox : public CComboBox, public atlctrls::CColorComboBoxBase
{
    DECLARE_DYNCREATE(CColorComboBox)
// Constructors/Destructor
public:
    CColorComboBox();
    virtual ~CColorComboBox();

// Overrides
public:
    virtual void DrawItem(LPDRAWITEMSTRUCT pDrawItem);

// Operations
public:
    void SetColorMap(const COLORMAPENTRY* pColorMap = GetColorMap(), int nItemHeight = 14);

// Attributes
public:
    COLORREF GetSelectedColor() const;
    void SetSelectedColor(COLORREF color);

// Message map functions
public:
    afx_msg void OnCloseUp();
    afx_msg void OnSelEndOK();
    afx_msg void OnSelChange();
    afx_msg void OnSelCustom(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg LRESULT OnSetCurSel(WPARAM wParam, LPARAM lParam);
    DECLARE_MESSAGE_MAP()
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CBalloonTipCtrl class
//

class CBalloonTipCtrl : public CToolTipCtrl
{
    DECLARE_DYNCREATE(CBalloonTipCtrl)
// Constructors/Destructor
public:
    CBalloonTipCtrl();
    virtual ~CBalloonTipCtrl();

// Overrides
public:
    virtual BOOL Create(CWnd* pParentWnd, DWORD dwStyle = TTS_BALLOON, int nMaxWidth = 300, LPVOID pvParam = NULL);

// Operations
public:
    void RelayEventEx(LPMSG pMsg);
    void HideBalloonTip();
    BOOL IsInputMessage(LPMSG pMsg) const;

    void DeleteItem(CWnd* pWnd);
    BOOL AddItem(CWnd* pWnd, LPCTSTR pszText = LPSTR_TEXTCALLBACK, UINT nFlags = 0);

    // The pszText and pszTitle parameters can pass pointer to a null-terminated string, or resource ID of the string.
    void SetItemText(CWnd* pWnd, LPCTSTR pszText);
    void ShowBalloonTip(CWnd* pWnd, LPCTSTR pszTitle = NULL, UINT_PTR nIcon = TTI_WARNING, int x = -1, int y = -1, UINT nElapse = 5000);
};

}  // namespace afxctrls

#include "afxctrls.inl"

#endif  // __AFXCTRLS_H__