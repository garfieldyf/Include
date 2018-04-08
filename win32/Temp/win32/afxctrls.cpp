///////////////////////////////////////////////////////////////////////////////
// afxctrls.cpp
//
// Copyright(c) 2013, Garfield. All rights reserved.
// Author  : Garfield
// Version : 4.5
// Finish Time : 2007/1/21

#include "stdafx.h"
#include "afxctrls.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace afxctrls {

///////////////////////////////////////////////////////////////////////////////
// Implementation of the CCueEdit class
//

IMPLEMENT_DYNCREATE(CCueEdit, CEdit)

BEGIN_MESSAGE_MAP(CCueEdit, CEdit)
    ON_WM_SETFOCUS()
    ON_WM_KILLFOCUS()
    ON_WM_CTLCOLOR_REFLECT()
    ON_MESSAGE(WM_SETTEXT, OnSetText)
    ON_MESSAGE(WM_GETTEXT, OnGetText)
    ON_MESSAGE(WM_GETTEXTLENGTH, OnGetTextLength)
END_MESSAGE_MAP()


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CImageLinkCtrl class
//

IMPLEMENT_DYNCREATE(CImageLinkCtrl, CStatic)

BEGIN_MESSAGE_MAP(CImageLinkCtrl, CStatic)
    ON_WM_SETCURSOR()
    ON_WM_LBUTTONUP()
    ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CLinkCtrl class
//

IMPLEMENT_DYNCREATE(CLinkCtrl, CImageLinkCtrl)

BEGIN_MESSAGE_MAP(CLinkCtrl, CImageLinkCtrl)
    ON_WM_PAINT()
    ON_WM_ENABLE()
    ON_WM_MOUSEMOVE()
    ON_WM_ERASEBKGND()
    ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
END_MESSAGE_MAP()


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CTabSheet class
//

IMPLEMENT_DYNCREATE(CTabSheet, CTabCtrl)

BEGIN_MESSAGE_MAP(CTabSheet, CTabCtrl)
    ON_WM_SIZE()
    ON_WM_CREATE()
    ON_MESSAGE(TCM_SETCURSEL, OnSetCurSel)
    ON_MESSAGE(TSM_SETPAGERECT, OnSetPageRect)
    ON_NOTIFY_REFLECT(TCN_SELCHANGE, OnSelChange)
    ON_MESSAGE(TCM_DELETEITEM, OnDeleteItem)
    ON_MESSAGE(TCM_DELETEALLITEMS, OnDeleteAllItems)
END_MESSAGE_MAP()


#ifdef __IMAGE_H__
///////////////////////////////////////////////////////////////////////////////
// Implementation of the CImageCtrl class
//

IMPLEMENT_DYNCREATE(CImageCtrl, CWnd)

BEGIN_MESSAGE_MAP(CImageCtrl, CWnd)
    ON_WM_PAINT()
    ON_WM_DESTROY()
    ON_WM_ERASEBKGND()
END_MESSAGE_MAP()
#endif  // __IMAGE_H__


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CSplashWnd class
//

IMPLEMENT_DYNCREATE(CSplashWnd, CWnd)

BEGIN_MESSAGE_MAP(CSplashWnd, CWnd)
    ON_WM_PAINT()
    ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


#ifdef __PICTURE_H__
///////////////////////////////////////////////////////////////////////////////
// Implementation of the CPictureCtrl class
//

IMPLEMENT_DYNCREATE(CPictureCtrl, CWnd)

BEGIN_MESSAGE_MAP(CPictureCtrl, CWnd)
    ON_WM_PAINT()
    ON_WM_ERASEBKGND()
END_MESSAGE_MAP()
#endif  // __PICTURE_H__


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CSortListCtrl class
//

IMPLEMENT_DYNCREATE(CSortListCtrl, CListCtrl)

BEGIN_MESSAGE_MAP(CSortListCtrl, CListCtrl)
    ON_MESSAGE(LVM_GETSORTMODE, OnGetSortMode)
    ON_MESSAGE(LVM_SETSORTMODE, OnSetSortMode)
    ON_MESSAGE(LVM_DELETECOLUMN, OnDeleteColumn)
    ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnColumnClick)
END_MESSAGE_MAP()


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CBitmapButton class
//

IMPLEMENT_DYNCREATE(CBitmapButton, CButton)

BEGIN_MESSAGE_MAP(CBitmapButton, CButton)
    ON_WM_MOUSEMOVE()
    ON_WM_ERASEBKGND()
    ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
END_MESSAGE_MAP()


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CImageListBox class
//

IMPLEMENT_DYNCREATE(CImageListBox, CListBox)

BEGIN_MESSAGE_MAP(CImageListBox, CListBox)
#ifdef _AFX_SUPPORT_CILB_TEXTCOLOR
    ON_WM_ENABLE()
    ON_WM_ERASEBKGND()
#endif
    ON_MESSAGE(LB_INITSTORAGE, OnInitStorage)
    ON_MESSAGE(LB_DELETESTRING, OnDeleteString)
    ON_MESSAGE(LB_RESETCONTENT, OnResetContent)
END_MESSAGE_MAP()


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CColorComboBox class
//

IMPLEMENT_DYNCREATE(CColorComboBox, CComboBox)

BEGIN_MESSAGE_MAP(CColorComboBox, CComboBox)
    ON_MESSAGE(CB_SETCURSEL, OnSetCurSel)
    ON_CONTROL_REFLECT(CBN_CLOSEUP, OnCloseUp)
    ON_CONTROL_REFLECT(CBN_SELENDOK, OnSelEndOK)
    ON_NOTIFY_REFLECT(CBN_SELCUSTOM, OnSelCustom)
    ON_CONTROL_REFLECT(CBN_SELCHANGE, OnSelChange)
END_MESSAGE_MAP()


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CBalloonTipCtrl class
//

IMPLEMENT_DYNCREATE(CBalloonTipCtrl, CToolTipCtrl)

}  // namespace afxctrls