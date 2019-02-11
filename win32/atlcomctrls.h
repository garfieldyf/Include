///////////////////////////////////////////////////////////////////////////////
// atlcomctrls.h
//
// Copyright(c) 2019, Garfield. All rights reserved.
// Author  : Garfield
// Version : 5.0
// Creation Date : 2008/2/4
// Summary:
//  _ATL_SUPPORT_CCE_TEXTCOLOR          Enable CCueEditImpl set/get text color
//  _ATL_SUPPORT_LVN_BEGINDRAG          Enable CDragDropImpl LVN_BEGINDRAG notify message
//  _ATL_SUPPORT_TVN_BEGINDRAG          Enable CDragDropImpl TVN_BEGINDRAG notify message
//  _ATL_SUPPORT_CDD_GETDRAGDROPFLAG    Enable CDragDropImpl NM_GETDRAGDROPFLAG notify message
//  _ATL_SUPPORT_CSLV_SORTCHANGED       Enable CSortListCtrlImpl LVN_SORTCHANGED notify message
//  _ATL_SUPPORT_CILB_TEXTCOLOR         Enable CImageListBoxImpl set/get text color and background color
//  _ATL_SUPPORT_CHD_SETHEIGHT          Enable CHeaderCtrlImpl set height
//  _ATL_SUPPORT_CBN_MOUSENOTIFY        Enable CImageButtonImpl and CBitmapButtonImpl BN_MOUSELEAVE and BN_MOUSEHOVER notify message

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __ATLCOMCTRLS_H__
#define __ATLCOMCTRLS_H__

#ifdef _WIN32_WCE
    #error atlcomctrls.h compiles under Windows only
#endif

#include "stdctrls.h"

///////////////////////////////////////////////////////////////////////////////
// Classes in this file:
//
// CImageCtrlImpl<T, TBase, TWinTraits>
// CImageCtrl
// CAnimImageCtrlImpl<T, TBase, TWinTraits>
// CAnimImageCtrl
// CPictureCtrlImpl<T, TBase, TWinTraits>
// CPictureCtrl
// CEditImpl<T>
// CCueEditImpl<T>
// CCueEdit
// CLinkCtrlImpl<T, TBase, TWinTraits>
// CLinkCtrl
// CSplashWindowImpl<T, TBase, TWinTraits>
// CSplashWindow
// CBalloonTipImpl<T>
// CDragDropImpl<T>
// CDropFilesImpl<T>
// CTabSheetImpl<T>
// CTabSheet
// CDialogBarImpl<T>
// CReBarCtrlImpl<T>
// CMarginEditImpl<T>
// CToolBarCtrlImpl<T>
// CHeaderCtrlImpl<T>
// CTrackBarCtrlImpl<T>
// CStatusBarCtrlImpl<T>
// CImageButtonImpl<T, TBase, TWinTraits>
// CImageButton
// CBitmapButtonImpl<T, TBase, TWinTraits>
// CBitmapButton
// CSortListCtrlImpl<T>
// CImageListBoxImpl<T, TBase, TWinTraits>
// CImageListBox
// CColorComboBoxImpl<T, TBase, TWinTraits>
// CColorComboBox
// CProgressBarCtrlImpl<T, TBase, TWinTraits>
// CPaintChildBkgndImpl<T>
// CShellChangeNotifyImpl<T>
// CShellTrayNotifyImpl<T, UINT>

namespace atlctrls {

///////////////////////////////////////////////////////////////////////////////
// CWinTraits Typedef
//

typedef ATL::CWinTraitsOR<WS_TABSTOP | SS_NOTIFY, WS_EX_NOPARENTNOTIFY> CLinkCtrlTraits;
typedef ATL::CWinTraitsOR<WS_TABSTOP | CCS_BOTTOM | SBARS_SIZEGRIP, 0> CStatusBarCtrlTraits;
typedef ATL::CWinTraitsOR<WS_TABSTOP | BS_OWNERDRAW, WS_EX_NOPARENTNOTIFY> CImageButtonTraits;
typedef ATL::CWinTraitsOR<WS_TABSTOP, WS_EX_CONTROLPARENT | WS_EX_NOPARENTNOTIFY> CTabSheetTraits;
typedef ATL::CWinTraitsOR<WS_TABSTOP | RBS_VARHEIGHT | RBS_AUTOSIZE | CCS_NODIVIDER, 0> CReBarTraits;
typedef ATL::CWinTraits<WS_POPUP | WS_VISIBLE, WS_EX_TOOLWINDOW | WS_EX_NOPARENTNOTIFY> CSplashWindowTraits;
typedef ATL::CWinTraitsOR<WS_TABSTOP | LBS_OWNERDRAWFIXED | LBS_HASSTRINGS, WS_EX_NOPARENTNOTIFY> CImageListBoxTraits;
typedef ATL::CWinTraitsOR<WS_TABSTOP | CBS_OWNERDRAWFIXED | CBS_HASSTRINGS | CBS_DROPDOWNLIST, WS_EX_NOPARENTNOTIFY> CColorComboBoxTraits;
typedef ATL::CWinTraitsOR<WS_TABSTOP | CCS_NODIVIDER | CCS_NORESIZE | CCS_NOPARENTALIGN | TBSTYLE_TOOLTIPS | TBSTYLE_FLAT, WS_EX_TOOLWINDOW> CToolBarTraits;


#ifdef __IMAGE_H__
///////////////////////////////////////////////////////////////////////////////
// Interface of the CImageCtrlImpl class
//

template <typename T, typename TBase = ATL::CWindow, typename TWinTraits = ATL::CControlWinTraits>
class ATL_NO_VTABLE CImageCtrlImpl : public ATL::CWindowImpl<T, TBase, TWinTraits>, public CImageCtrlBase
{
// Constructors
protected:
    CImageCtrlImpl();

// Implementation
public:
    BEGIN_MSG_MAP(CImageCtrlImpl)
        MESSAGE_HANDLER(WM_PAINT, OnPaint)
        MESSAGE_HANDLER_EX(WM_ERASEBKGND, OnEraseBkgnd)
    END_MSG_MAP()

// Message map functions
public:
    LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CImageCtrl class
//

class CImageCtrl : public CImageCtrlImpl<CImageCtrl>
{
public:
    DECLARE_WND_CLASS(_T("ATL_ImageCtrl"))
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CAnimImageCtrlImpl class
//

template <typename T, typename TBase = ATL::CWindow, typename TWinTraits = ATL::CControlWinTraits>
class ATL_NO_VTABLE CAnimImageCtrlImpl : public ATL::CWindowImpl<T, TBase, TWinTraits>, public CAnimImageCtrlBase
{
// Constructors
protected:
    CAnimImageCtrlImpl();

// Implementation
public:
    BEGIN_MSG_MAP(CAnimImageCtrlImpl)
        MESSAGE_HANDLER(WM_PAINT, OnPaint)
        MESSAGE_HANDLER_EX(WM_TIMER, OnTimer)
        MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
        MESSAGE_HANDLER_EX(WM_ERASEBKGND, OnEraseBkgnd)
    END_MSG_MAP()

// Operations
public:
    BOOL Play();
    void Stop();

    void Pause();
    BOOL SizeToContent();

// Message map functions
public:
    LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CAnimImageCtrl class
//

class CAnimImageCtrl : public CAnimImageCtrlImpl<CAnimImageCtrl>
{
public:
    DECLARE_WND_CLASS(_T("ATL_AnimImageCtrl"))
};
#endif  // __IMAGE_H__


#ifdef __PICTURE_H__
///////////////////////////////////////////////////////////////////////////////
// Interface of the CPictureCtrlImpl class
//

template <typename T, typename TBase = ATL::CWindow, typename TWinTraits = ATL::CControlWinTraits>
class ATL_NO_VTABLE CPictureCtrlImpl : public ATL::CWindowImpl<T, TBase, TWinTraits>, public CPictureCtrlBase
{
// Constructors
protected:
    CPictureCtrlImpl();

// Implementation
public:
    BEGIN_MSG_MAP(CPictureCtrlImpl)
        MESSAGE_HANDLER(WM_PAINT, OnPaint)
        MESSAGE_HANDLER_EX(WM_ERASEBKGND, OnEraseBkgnd)
    END_MSG_MAP()

// Message map functions
public:
    LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CPictureCtrl class
//

class CPictureCtrl : public CPictureCtrlImpl<CPictureCtrl>
{
public:
    DECLARE_WND_CLASS(_T("ATL_PictureCtrl"))
};
#endif  // __PICTURE_H__


///////////////////////////////////////////////////////////////////////////////
// Interface of the CEditImpl class
//

template <typename T>
class ATL_NO_VTABLE CEditImpl : public CEditBase
{
// Constructors
protected:
    CEditImpl();

// Implementation
public:
    BEGIN_MSG_MAP(CEditImpl<T>)
        MESSAGE_HANDLER(WM_NCPAINT, OnNcPaint)
        MESSAGE_HANDLER_EX(WM_SETFOCUS, OnSetFocus)
        MESSAGE_HANDLER_EX(WM_KILLFOCUS, OnKillFocus)
        MESSAGE_HANDLER_EX(WM_MOUSEMOVE, OnMouseMove)
        MESSAGE_HANDLER_EX(WM_MOUSELEAVE, OnMouseLeave)
    END_MSG_MAP()

// Attributes
public:
    UINT GetState() const;
    BOOL SetState(UINT nState);

// Message map functions
public:
    LRESULT OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CCueEditImpl class
//

template <typename T>
class ATL_NO_VTABLE CCueEditImpl
{
// Constructors
protected:
    CCueEditImpl();

// Implementation
public:
    BEGIN_MSG_MAP(CCueEditImpl<T>)
        MESSAGE_HANDLER(WM_SETTEXT, OnSetText)
        MESSAGE_HANDLER(WM_GETTEXT, OnGetText)
        MESSAGE_HANDLER(WM_SETFOCUS, OnSetFocus)
        MESSAGE_HANDLER(WM_KILLFOCUS, OnKillFocus)
        MESSAGE_HANDLER(OCM_CTLCOLOREDIT, OnCtlColor)
        MESSAGE_HANDLER(WM_GETTEXTLENGTH, OnGetTextLength)
    END_MSG_MAP()

// Operations
public:
    void SetCueText(ATL::_U_STRINGorID text);

// Data members
public:
#ifdef _ATL_SUPPORT_CCE_TEXTCOLOR
    COLORREF m_clrText;
    COLORREF m_clrCueText;
#endif
    CString  m_strCueText;

// Message map functions
public:
    LRESULT OnSetText(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnGetText(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnSetFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnCtlColor(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnGetTextLength(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CLinkCtrlImpl class
//

template <typename T, typename TBase = ATL::CWindow, typename TWinTraits = CLinkCtrlTraits>
class ATL_NO_VTABLE CLinkCtrlImpl : public ATL::CWindowImpl<T, TBase, TWinTraits>, public CLinkCtrlBase
{
// Constructors
protected:
    CLinkCtrlImpl(DWORD dwExStyle = 0);

// Implementation
public:
    BEGIN_MSG_MAP(CLinkCtrlImpl)
        MESSAGE_HANDLER(WM_PAINT, OnPaint)
        MESSAGE_HANDLER_EX(WM_ENABLE, OnEnable)
        MESSAGE_HANDLER_EX(WM_SETFONT, OnSetFont)
        MESSAGE_HANDLER_EX(WM_SETCURSOR, OnSetCursor)
        MESSAGE_HANDLER_EX(WM_MOUSEMOVE, OnMouseMove)
        MESSAGE_HANDLER_EX(WM_MOUSELEAVE, OnMouseLeave)
        MESSAGE_HANDLER_EX(WM_ERASEBKGND, OnEraseBkgnd)
    END_MSG_MAP()

// Overrides
public:
    BOOL SubclassWindow(HWND hWnd);

// Operations
public:
    BOOL SizeToContent();

// Message map functions
public:
    LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CLinkCtrl class
//

class CLinkCtrl : public CLinkCtrlImpl<CLinkCtrl>
{
// Constructors
public:
    explicit CLinkCtrl(DWORD dwExStyle = 0);

// Implementation
public:
    DECLARE_WND_CLASS(_T("ATL_LinkCtrl"));
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CSplashWindowImpl class
//

template <typename T, typename TBase = ATL::CWindow, typename TWinTraits = CSplashWindowTraits>
class ATL_NO_VTABLE CSplashWindowImpl : public ATL::CWindowImpl<T, TBase, TWinTraits>, public CSplashWindowBase
{
// Constructors
protected:
    CSplashWindowImpl();

// Implementation
public:
    BEGIN_MSG_MAP(CSplashWindowImpl)
        MESSAGE_HANDLER(WM_PAINT, OnPaint)
        MESSAGE_HANDLER_EX(WM_ERASEBKGND, OnEraseBkgnd)
    END_MSG_MAP()

// Operations
public:
    HWND Create(HBITMAP hBitmap, COLORREF clrMask = CLR_INVALID, HWND hWndParent = NULL);

    void SetDrawText(POSITION pos, UINT nIDText = 0, COLORREF clrText = CLR_INVALID, LPLOGFONT pLogFont = NULL, UINT nFormat = (UINT)-1, LPCRECT pRect = NULL);
    void SetDrawText(POSITION pos, LPCTSTR pszText = NULL, COLORREF clrText = CLR_INVALID, LPLOGFONT pLogFont = NULL, UINT nFormat = (UINT)-1, LPCRECT pRect = NULL);

// Message map functions
public:
    LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CSplashWindow class
//

class CSplashWindow : public CSplashWindowImpl<CSplashWindow>
{
public:
    DECLARE_WND_CLASS_EX(_T("ATL_SplashWindow"), (sysutil::IsWindowsXP() ? CS_DROPSHADOW : 0), COLOR_WINDOW);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CBalloonTipImpl class
//

template <typename T>
class ATL_NO_VTABLE CBalloonTipImpl : public CBalloonTipBase
{
// Constructors
protected:
    CBalloonTipImpl();

// Implementation
public:
    BEGIN_MSG_MAP(CBalloonTipImpl<T>)
        MESSAGE_HANDLER_EX(WM_ACTIVATE, OnActivate)
    END_MSG_MAP()

// Operations
public:
    HWND BalloonTip_Create(DWORD dwStyle = TTS_BALLOON, int nMaxWidth = 300);
    void BalloonTip_ShowWindow(HWND hWnd, ATL::_U_STRINGorID title = (LPCTSTR)NULL, UINT_PTR nIcon = TTI_WARNING, int x = -1, int y = -1, UINT nElapse = 5000);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CDragDropImpl class
//

template <typename T>
class ATL_NO_VTABLE CDragDropImpl : public CDragDropBase
{
// Constructors
protected:
    CDragDropImpl();

// Implementation
public:
    BEGIN_MSG_MAP(CDragDropImpl<T>)
        MESSAGE_HANDLER_EX(WM_LBUTTONUP, OnButtonUp)
        MESSAGE_HANDLER_EX(WM_RBUTTONUP, OnButtonUp)
        MESSAGE_HANDLER_EX(WM_MOUSEMOVE, OnMouseMove)
    #ifdef _ATL_SUPPORT_LVN_BEGINDRAG
        REFLECTED_NOTIFY_CODE_HANDLER(LVN_BEGINDRAG, OnBeginDrag)
    #endif
    #ifdef _ATL_SUPPORT_TVN_BEGINDRAG
        REFLECTED_NOTIFY_CODE_HANDLER(TVN_BEGINDRAG, OnBeginDrag)
    #endif
    END_MSG_MAP()

// Message map functions
public:
    LRESULT OnBeginDrag(int nIDCtrl, LPNMHDR pNMHDR, BOOL& bHandled);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CDropFilesImpl class
//

template <typename T>
class ATL_NO_VTABLE CDropFilesImpl
{
// Constructors
protected:
    CDropFilesImpl();

// Implementation
public:
    BEGIN_MSG_MAP(CDropFilesImpl<T>)
        MESSAGE_HANDLER(WM_DROPFILES, OnDropFiles)
    END_MSG_MAP()

// Overridables
public:
    void DropFiles_OnDropFile(LPCTSTR pszFileName, UINT nLength);
    void DropFiles_OnDropFiles(HDROP hDropInfo, UINT nFileCount);

// Operations
public:
    void DropFiles_DragAcceptFiles(BOOL bAccept = TRUE);

// Message map functions
public:
    LRESULT OnDropFiles(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CTabSheetImpl class
//

template <typename T>
class ATL_NO_VTABLE CTabSheetImpl
{
// Constructors
protected:
    CTabSheetImpl();

// Implementation
public:
    BEGIN_MSG_MAP(CTabSheetImpl<T>)
        MESSAGE_HANDLER(WM_SIZE, OnSize)
        MESSAGE_HANDLER(WM_CREATE, OnCreate)
        MESSAGE_HANDLER(TCM_SETCURSEL, OnSetCurSel)
        MESSAGE_HANDLER(TCM_DELETEITEM, OnDeleteItem)
        MESSAGE_HANDLER(TCM_DELETEALLITEMS, OnDeleteAllItems)
        REFLECTED_NOTIFY_CODE_HANDLER(TCN_SELCHANGE, OnSelChange)
    END_MSG_MAP()

// Overridables
public:
    BOOL PreTranslateMessage(LPMSG pMsg);
    void SetPageRect(HWND hWndPage, int nPage, const RECT& rect);

// Operations
public:
    int AddPage(HWND hWndPage, ATL::_U_STRINGorID title, int nImage = -1, LPARAM lParam = 0L);
    int InsertPage(int nPage, HWND hWndPage, ATL::_U_STRINGorID title, int nImage = -1, LPARAM lParam = 0L);

// Attributes
public:
    BOOL SetPageExtra();
    HWND GetActivePage() const;

    HWND GetPage(int nPage) const;
    BOOL SetPage(int nPage, HWND hWnd);

    int GetPageImage(int nPage) const;
    BOOL SetPageImage(int nPage, int nImage);

    DWORD_PTR GetPageData(int nPage) const;
    BOOL SetPageData(int nPage, DWORD_PTR dwData);

    CString GetPageTitle(int nPage) const;
    HRESULT GetPageTitle(int nPage, BSTR& bstrTitle) const;
    BOOL SetPageTitle(int nPage, ATL::_U_STRINGorID title);

// Message map functions
public:
    LRESULT OnSelChange(int nIDCtrl, LPNMHDR pNMHDR, BOOL& bHandled);
    LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnSetCurSel(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnDeleteItem(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnDeleteAllItems(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CDialogBarImpl class
//

template <typename T>
class ATL_NO_VTABLE CDialogBarImpl
{
// Constructors
protected:
    CDialogBarImpl();

// Implementation
public:
    BEGIN_MSG_MAP(CDialogBarImpl<T>)
        MESSAGE_HANDLER(WM_MOVE, OnMove)
        MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
    END_MSG_MAP()

// Message map functions
public:
    LRESULT OnMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnEraseBkgnd(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CReBarCtrlImpl class
//

template <typename T>
class ATL_NO_VTABLE CReBarCtrlImpl
{
// Constructors
protected:
    CReBarCtrlImpl();

// Implementation
public:
    BEGIN_MSG_MAP(CReBarCtrlImpl<T>)
        MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
    END_MSG_MAP()

// Overridables
public:
    void OnDrawBand(const RBDRAWBAND& rbdb);

// Operations
public:
    BOOL AddBar(HWND hWndBand, HBITMAP hbmBack, UINT nStyle = RBBS_BREAK | RBBS_CHILDEDGE | RBBS_NOGRIPPER | RBBS_USECHEVRON, LPCTSTR pszTitle = NULL, int nImage = -1, LPARAM lParam = 0L);
    BOOL AddBar(HWND hWndBand, COLORREF clrFrom, COLORREF clrTo, UINT nStyle = RBBS_BREAK | RBBS_CHILDEDGE | RBBS_NOGRIPPER | RBBS_USECHEVRON, LPCTSTR pszTitle = NULL, int nImage = -1, LPARAM lParam = 0L);

// Implementation
protected:
    BOOL _AddBar(HWND hWndBand, REBARBANDINFO& rbBand, LPCTSTR pszTitle, int nImage);

// Message map functions
public:
    LRESULT OnEraseBkgnd(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CMarginEditImpl class
//

template <typename T>
class ATL_NO_VTABLE CMarginEditImpl : public CEditImpl<T>
{
// Constructors
protected:
    CMarginEditImpl();

// Implementation
public:
    BEGIN_MSG_MAP(CMarginEditImpl<T>)
        MESSAGE_HANDLER(WM_NCPAINT, OnNcPaint)
        MESSAGE_HANDLER(WM_NCHITTEST, OnNcHitTest)
        MESSAGE_HANDLER(WM_NCCALCSIZE, OnNcCalcSize)
        MESSAGE_HANDLER(WM_NCMOUSEMOVE, OnNcMouseMove)
        MESSAGE_HANDLER(WM_NCLBUTTONUP, OnNcLButtonUp)
        MESSAGE_HANDLER(WM_NCMOUSELEAVE, OnNcMouseLeave)
        MESSAGE_HANDLER(WM_NCLBUTTONDOWN, OnNcLButtonDown)
        CHAIN_MSG_MAP(CEditImpl<T>)
    END_MSG_MAP()

// Overridables
public:
    BOOL PreTranslateMessage(LPMSG pMsg);

    void CalcMarginRect(const RECT& rcWindow, RECT& rcMargin) const;
    void OnDrawMargin(HDC hDC, const RECT& rcWindow, const RECT& rcMargin);

// Attributes
public:
    int GetMarginWidth() const;
    BOOL SetMarginWidth(int nMargin);

    void GetMarginRect(RECT& rect) const;

// Data members
public:
    int m_nMarginWidth;
    BOOL m_bMouseHover;

// Message map functions
public:
    LRESULT OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnNcMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnNcLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnNcMouseLeave(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnNcLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CToolBarCtrlImpl class
//

template <typename T>
class ATL_NO_VTABLE CToolBarCtrlImpl
{
// Constructors
protected:
    CToolBarCtrlImpl();

// Implementation
public:
    BEGIN_MSG_MAP(CToolBarCtrlImpl<T>)
        MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
        REFLECTED_NOTIFY_CODE_HANDLER(NM_CUSTOMDRAW, OnCustomDraw)
    END_MSG_MAP()

// Overridables
public:
    LRESULT OnDrawButton(LPNMTBCUSTOMDRAW pTBCustomDraw);

// Operations
public:
    BOOL LoadToolBar(UINT nIDResource, COLORREF clrMask = CLR_DEFAULT, int cxButtonMargin = 7, int cyButtonMargin = 7, int cxSeparator = 8);

    void SetSizes(UINT cxImage, UINT cyImage, UINT cxButton, UINT cyButton);
    BOOL LoadButtons(const WORD* pIDArray, int nIDCount, int cxSeparator = 8);

// Message map functions
public:
    LRESULT OnCustomDraw(int nIDCtrl, LPNMHDR pNMHDR, BOOL& bHandled);
    LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CHeaderCtrlImpl class
//

template <typename T>
class ATL_NO_VTABLE CHeaderCtrlImpl : public CHeaderCtrlBase
{
// Constructors
protected:
    CHeaderCtrlImpl(int nHeight = -1);

// Implementation
public:
    BEGIN_MSG_MAP(CHeaderCtrlImpl<T>)
        MESSAGE_HANDLER(WM_PAINT, OnPaint)
    #ifdef _ATL_SUPPORT_CHD_SETHEIGHT
        MESSAGE_HANDLER(HDM_LAYOUT, OnLayout)
    #endif
        MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
        MESSAGE_HANDLER_EX(WM_LBUTTONUP, OnMouseEvent)
        MESSAGE_HANDLER_EX(WM_MOUSELEAVE, OnMouseEvent)
        MESSAGE_HANDLER_EX(WM_ERASEBKGND, OnEraseBkgnd)
        MESSAGE_HANDLER_EX(WM_LBUTTONDOWN, OnLButtonDown)
        MESSAGE_HANDLER_EX(WM_THEMECHANGED, OnThemeChanged)
    END_MSG_MAP()

// Message map functions
public:
#ifdef _ATL_SUPPORT_CHD_SETHEIGHT
    LRESULT OnLayout(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
#endif
    LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CTrackBarCtrlImpl class
//

template <typename T>
class ATL_NO_VTABLE CTrackBarCtrlImpl
{
// Constructors
protected:
    CTrackBarCtrlImpl();

// Implementation
public:
    BEGIN_MSG_MAP(CTrackBarCtrlImpl<T>)
        MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
    END_MSG_MAP()

// Overridables
public:
    void OnSetPos(int x, int y);

// Message map functions
public:
    LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CStatusBarCtrlImpl class
//

template <typename T>
class ATL_NO_VTABLE CStatusBarCtrlImpl : public CStatusBarCtrlBase
{
// Constructors
protected:
    CStatusBarCtrlImpl(DWORD dwExStyle = 0);

// Implementation
public:
    BEGIN_MSG_MAP(CStatusBarCtrlImpl<T>)
        MESSAGE_HANDLER(WM_SIZE, OnSize)
        MESSAGE_HANDLER(WM_PAINT, OnPaint)
    END_MSG_MAP()

// Operations
public:
    BOOL SetPanes(const SBPANE* pPanes, int nCount);

// Attributes
public:
    void SetPaneWidth(int nIndex, int nWidth);

// Message map functions
public:
    LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
};


#ifdef __IMAGE_H__
///////////////////////////////////////////////////////////////////////////////
// Interface of the CImageButtonImpl class
//

template <typename T, typename TBase = ATL::CWindow, typename TWinTraits = CImageButtonTraits>
class ATL_NO_VTABLE CImageButtonImpl : public ATL::CWindowImpl<T, TBase, TWinTraits>, public CImageButtonBase
{
// Constructors
protected:
    CImageButtonImpl();

// Implementation
public:
    BEGIN_MSG_MAP(CImageButtonImpl)
        MESSAGE_HANDLER_EX(OCM_DRAWITEM, OnDrawItem)
        MESSAGE_HANDLER_EX(WM_MOUSEMOVE, OnMouseMove)
        MESSAGE_HANDLER_EX(WM_MOUSELEAVE, OnMouseLeave)
        MESSAGE_HANDLER_EX(WM_ERASEBKGND, OnEraseBkgnd)
    END_MSG_MAP()

// Overrides
public:
    BOOL SubclassWindow(HWND hWnd);

// Operations
public:
    BOOL SizeToContent();

// Data members
public:
    atlutil::CImageList* m_pImageList;

// Message map functions
public:
    LRESULT OnDrawItem(HWND hWnd, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
};
#endif  // __IMAGE_H__


///////////////////////////////////////////////////////////////////////////////
// Interface of the CBitmapButtonImpl class
//

template <typename T, typename TBase = ATL::CWindow, typename TWinTraits = CImageButtonTraits>
class ATL_NO_VTABLE CBitmapButtonImpl : public ATL::CWindowImpl<T, TBase, TWinTraits>, public CImageButtonBase
{
// Constructors
protected:
    CBitmapButtonImpl();

// Implementation
public:
    BEGIN_MSG_MAP(CBitmapButtonImpl)
        MESSAGE_HANDLER_EX(OCM_DRAWITEM, OnDrawItem)
        MESSAGE_HANDLER_EX(WM_MOUSEMOVE, OnMouseMove)
        MESSAGE_HANDLER_EX(WM_MOUSELEAVE, OnMouseLeave)
        MESSAGE_HANDLER_EX(WM_ERASEBKGND, OnEraseBkgnd)
    END_MSG_MAP()

// Overrides
public:
    BOOL SubclassWindow(HWND hWnd);

// Operations
public:
    BOOL SizeToContent();

// Data members
public:
    HIMAGELIST m_hImageList;

// Message map functions
public:
    LRESULT OnDrawItem(HWND hWnd, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CSortListCtrlImpl class
//

template <typename T>
class ATL_NO_VTABLE CSortListCtrlImpl : public CSortListCtrlBase
{
// Constructors
protected:
    CSortListCtrlImpl();

// Implementation
public:
    BEGIN_MSG_MAP(CSortListCtrlImpl<T>)
        NOTIFY_CODE_HANDLER(HDN_ITEMCLICK, OnColumnClick)
        MESSAGE_HANDLER(LVM_DELETECOLUMN, OnDeleteColumn)
    END_MSG_MAP()

// Overridables
public:
    int GetSortMode(int nCol) const;
    void SetSortMode(int nCol, int nSortMode);

// Operations
public:
    BOOL SortItem(int nCol, BOOL bAscending = TRUE);

    int InsertColumnEx(int nCol, const LVCOLUMN* pColumn, int nSortMode = LVSM_TEXT);
    int InsertColumnEx(int nCol, ATL::_U_STRINGorID columnHeading, int nFormat = LVCFMT_LEFT, int nWidth = -1, int nSortMode = LVSM_TEXT, int nSubItem = -1, int nImage = -1, int nOrder = -1);

// Message map functions
public:
    LRESULT OnColumnClick(int nIDCtrl, LPNMHDR pNMHDR, BOOL& bHandled);
    LRESULT OnDeleteColumn(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CImageListBoxImpl class
//

template <typename T, typename TBase = ATL::CWindow, typename TWinTraits = CImageListBoxTraits>
class ATL_NO_VTABLE CImageListBoxImpl : public ATL::CWindowImpl<T, TBase, TWinTraits>, public CImageListBoxBase
{
// Constructors
protected:
    CImageListBoxImpl();

// Implementation
public:
    BEGIN_MSG_MAP(CImageListBoxImpl)
    #ifdef _ATL_SUPPORT_CILB_TEXTCOLOR
        MESSAGE_HANDLER_EX(WM_ENABLE, OnEnable)
        MESSAGE_HANDLER_EX(WM_ERASEBKGND, OnEraseBkgnd)
    #endif
        MESSAGE_HANDLER_EX(OCM_DRAWITEM, OnDrawItem)
        MESSAGE_HANDLER(LB_INITSTORAGE, OnInitStorage)
        MESSAGE_HANDLER(LB_DELETESTRING, OnDeleteString)
        MESSAGE_HANDLER(LB_RESETCONTENT, OnResetContent)
    END_MSG_MAP()

// Operations
public:
    int AddItem(LPCTSTR pszText, int nImage = -1);
    int InsertItem(int nItem, LPCTSTR pszText, int nImage = -1);

// Attributes
public:
    HIMAGELIST GetImageList() const;
    BOOL SetImageList(HIMAGELIST hImageList);

// Message map functions
public:
    LRESULT OnInitStorage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnDeleteString(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnResetContent(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CColorComboBoxImpl class
//

template <typename T, typename TBase = ATL::CWindow, typename TWinTraits = CColorComboBoxTraits>
class ATL_NO_VTABLE CColorComboBoxImpl : public ATL::CWindowImpl<T, TBase, TWinTraits>, public CColorComboBoxBase
{
// Constructors
protected:
    CColorComboBoxImpl();

// Implementation
public:
    BEGIN_MSG_MAP(CColorComboBoxImpl)
        MESSAGE_HANDLER(CB_SETCURSEL, OnSetCurSel)
        MESSAGE_HANDLER_EX(OCM_DRAWITEM, OnDrawItem)
        REFLECTED_COMMAND_CODE_HANDLER(CBN_CLOSEUP, OnCloseUp)
        REFLECTED_COMMAND_CODE_HANDLER(CBN_SELENDOK, OnSelEndOK)
        REFLECTED_NOTIFY_CODE_HANDLER(CBN_SELCUSTOM, OnSelCustom)
        REFLECTED_COMMAND_CODE_HANDLER(CBN_SELCHANGE, OnSelChange)
    END_MSG_MAP()

// Operations
public:
    void SetColorMap(const COLORMAPENTRY* pColorMap = GetColorMap(), int nItemHeight = 15);

// Attributes
public:
    COLORREF GetCustomColor() const;
    void SetCustomColor(COLORREF color);

    COLORREF GetSelectedColor() const;
    void SetSelectedColor(COLORREF color);

// Message map functions
public:
    LRESULT OnSetCurSel(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CProgressBarCtrlImpl class
//

template <typename T, typename TBase = ATL::CWindow, typename TWinTraits = ATL::CControlWinTraits>
class ATL_NO_VTABLE CProgressBarCtrlImpl : public ATL::CWindowImpl<T, TBase, TWinTraits>
{
// Constructors
protected:
    CProgressBarCtrlImpl();

// Implementation
public:
    BEGIN_MSG_MAP(CProgressBarCtrlImpl)
        MESSAGE_HANDLER(WM_PAINT, OnPaint)
        MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
    END_MSG_MAP()

// Overridables
public:
    void OnDraw(HDC hDC, const RECT& rect, int nPos);
    void OnDrawBkgnd(HDC hDC, const RECT& rect);

// Message map functions
public:
    LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnEraseBkgnd(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CPaintChildBkgndImpl class
//

template <typename T>
class ATL_NO_VTABLE CPaintChildBkgndImpl : public CPaintChildBkgndBase
{
// Constructors
protected:
    CPaintChildBkgndImpl();

// Implementation
public:
    BEGIN_MSG_MAP(CPaintChildBkgndImpl<T>)
        MESSAGE_HANDLER_EX(WM_SIZE, OnSize)
        NOTIFY_CODE_HANDLER(NM_PAINTBKGND, OnNMPaintBkgnd)
    END_MSG_MAP()

// Operations
public:
    void PaintBkgnd_Init();
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CShellChangeNotifyImpl class
//

template <typename T>
class ATL_NO_VTABLE CShellChangeNotifyImpl
{
// Constructors
protected:
    CShellChangeNotifyImpl();

// Implementation
public:
    BEGIN_MSG_MAP(CShellChangeNotifyImpl<T>)
        MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
        MESSAGE_HANDLER(WM_SHCHANGENOTIFY, OnSHChangeNotify)
    END_MSG_MAP()

// Overridables
public:
    void SHChangeNotify_OnChange(LONG lEvent, LPCITEMIDLIST pidl1, LPCITEMIDLIST pidl2);

// Operations
public:
    BOOL SHChangeNotify_Register(LPCITEMIDLIST pidl, LONG lEvents = SHCNE_SHELLEVENTS, int nSources = SHCNRF_ShellLevel, BOOL bRecursive = TRUE);
    void SHChangeNotify_Unregister();

// Data members
public:
    ULONG m_ulID;

// Message map functions
public:
    LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnSHChangeNotify(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CShellTrayNotifyImpl class
//

template <typename T, UINT nID = 0>
class ATL_NO_VTABLE CShellTrayNotifyImpl
{
// Constructors
protected:
    CShellTrayNotifyImpl();

// Implementation
public:
    BEGIN_MSG_MAP(CShellTrayNotifyImpl)
        MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
    END_MSG_MAP()

// Operations
public:
    BOOL SHTrayNotify_Create(HICON hIcon, ATL::_U_STRINGorID tip, UINT nCallbackMessage = 0);
    BOOL SHTrayNotify_Create(HICON hIcon, ATL::_U_STRINGorID tip, ATL::_U_STRINGorID info, ATL::_U_STRINGorID infoTitle, UINT nCallbackMessage = 0, DWORD dwInfoFlags = NIIF_INFO, HICON hBalloonIcon = NULL, UINT nTimeout = 5000);

    BOOL SHTrayNotify_SetIcon(UINT nIcon);
    BOOL SHTrayNotify_SetIcon(HICON hIcon);

    BOOL SHTrayNotify_SetTipText(ATL::_U_STRINGorID tip);
    BOOL SHTrayNotify_SetCallbackMessage(UINT nCallbackMessage);
    BOOL SHTrayNotify_SetState(DWORD dwState, DWORD dwStateMask);

    BOOL SHTrayNotify_SetVersion(UINT nVersion = NOTIFYICON_VERSION);
    BOOL SHTrayNotify_ShowBalloonTip(ATL::_U_STRINGorID info, ATL::_U_STRINGorID infoTitle, DWORD dwInfoFlags = NIIF_INFO, HICON hBalloonIcon = NULL, UINT nTimeout = 5000);

// Message map functions
public:
    LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
};


#ifdef __ATLCTRLS_H__
///////////////////////////////////////////////////////////////////////////////
// Interface of the CCueEdit class
//

class CCueEdit : public ATL::CWindowImpl<CCueEdit, WTL::CEdit>, public CCueEditImpl<CCueEdit>
{
public:
    DECLARE_WND_SUPERCLASS(_T("ATL_CueEdit"), GetWndClassName());

    BEGIN_MSG_MAP(CCueEdit)
        CHAIN_MSG_MAP(CCueEditImpl<CCueEdit>)
    END_MSG_MAP()
};


#ifdef __IMAGE_H__
///////////////////////////////////////////////////////////////////////////////
// Interface of the CImageButton class
//

class CImageButton : public CImageButtonImpl<CImageButton, WTL::CButton>
{
public:
    DECLARE_WND_SUPERCLASS(_T("ATL_ImageButton"), GetWndClassName());
};
#endif  // __IMAGE_H__


///////////////////////////////////////////////////////////////////////////////
// Interface of the CBitmapButton class
//

class CBitmapButton : public CBitmapButtonImpl<CBitmapButton, WTL::CButton>
{
public:
    DECLARE_WND_SUPERCLASS(_T("ATL_BitmapButton"), GetWndClassName());
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CImageListBox class
//

class CImageListBox : public CImageListBoxImpl<CImageListBox, WTL::CListBox>
{
public:
    DECLARE_WND_SUPERCLASS(_T("ATL_ImageListBox"), GetWndClassName());
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CColorComboBox class
//

class CColorComboBox : public CColorComboBoxImpl<CColorComboBox, WTL::CComboBox>
{
public:
    DECLARE_WND_SUPERCLASS(_T("ATL_ColorComboBox"), GetWndClassName());
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CTabSheet class
//

class CTabSheet : public ATL::CWindowImpl<CTabSheet, WTL::CTabCtrl, CTabSheetTraits>, public CTabSheetImpl<CTabSheet>
{
// Constructors
public:
    CTabSheet();

// Implementation
public:
    DECLARE_WND_SUPERCLASS(_T("ATL_TabSheet"), GetWndClassName());

    BEGIN_MSG_MAP(CTabSheet)
        CHAIN_MSG_MAP(CTabSheetImpl<CTabSheet>)
    END_MSG_MAP()

// Overrides
public:
    BOOL SubclassWindow(HWND hWnd);
};
#endif  // __ATLCTRLS_H__

}  // namespace atlctrls

#include "atlcomctrls.inl"

#endif  // __ATLCOMCTRLS_H__