///////////////////////////////////////////////////////////////////////////////
// atlcectrls.h
//
// Author : Garfield
// Creation Date : 2009/12/14

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __ATLCECTRLS_H__
#define __ATLCECTRLS_H__

#ifndef _WIN32_WCE
    #error atlcectrls.h compiles under Windows CE only
#endif

#include <atlbase.h>
#include <atlwin.h>
#include "stdutil.h"
#include "sysutil.h"

///////////////////////////////////////////////////////////////////////////////
// Classes in this file:
//
// CWaitCursor
// CFullScreenImpl<T>
// CScrollWindowImpl<T>
// CImageButtonImpl<T, TBase, TWinTraits>
// CImageButton
// CBitmapButtonImpl<T, TBase, TWinTraits>
// CBitmapButton
// CSHToolBarCtrlT<T>
// CSHToolBarCtrl
// CAnimImageCtrlImpl<T, TBase, TWinTraits>
// CAnimImageCtrl
// CPaintChildBkgndImpl<T>
// CTodayItemWindowImpl<T>
// CGestureWindowImpl<T>
// CAutoGestureWindowImpl<T>
// CShellChangeNotifyImpl<T>

namespace WCE {

///////////////////////////////////////////////////////////////////////////////
// CWinTraits Typedef
//

typedef ATL::CWinTraitsOR<BS_OWNERDRAW, 0> CBitmapButtonTraits;
typedef ATL::CWinTraitsOR<WS_BORDER | TBSTYLE_FLAT | TBSTYLE_LIST | CCS_NODIVIDER, WS_EX_TOOLWINDOW> CSHToolBarCtrlTraits;


///////////////////////////////////////////////////////////////////////////////
// Interface of the CWaitCursor class
//

class CWaitCursor
{
// Constructors/Destructor
public:
    explicit CWaitCursor(HCURSOR hCursor = ::LoadCursor(NULL, IDC_WAIT));
    ~CWaitCursor();

// Data members
public:
    HCURSOR m_hCursor;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CFullScreenBase class
//

class ATL_NO_VTABLE CFullScreenBase
{
// Constructors
protected:
    CFullScreenBase();

// Overridables
protected:
    void DoFullScreen(HWND hWnd, HWND hWndCommandBar);

// Implementation
protected:
    void ShowTaskBar(HWND hWnd, BOOL bShow, BOOL bRepaint);
    void ShowMenuBar(HWND hWnd, HWND hWndCommandBar, BOOL bShow);

// Data members
public:
    BOOL m_bFullScreen;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CFullScreenImpl class
//

template <typename T>
class ATL_NO_VTABLE CFullScreenImpl : public CFullScreenBase
{
// Constructors
protected:
    CFullScreenImpl();

// Implementation
public:
    BEGIN_MSG_MAP(CFullScreenImpl<T>)
        MESSAGE_HANDLER(WM_ACTIVATE, OnActivate)
        MESSAGE_HANDLER(WM_SETTINGCHANGE, OnSettingChange)
    END_MSG_MAP()

// Operations
public:
    void SetFullScreen(BOOL bFullScreen);

// Message map functions
public:
    LRESULT OnActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnSettingChange(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CScrollWindowBase class
//

class ATL_NO_VTABLE CScrollWindowBase
{
// Constructors
protected:
    CScrollWindowBase();

// Implementation
protected:
    void OnScroll(HWND hWnd, int nBar, UINT nSBCode, int nLine);
    void UpdateScrollInfo(HWND hWnd, int cx, int cy);
    void SetScrollSizes(HWND hWnd, int cxTotal, int cyTotal, int cxLine, int cyLine);

// Data members
public:
    int m_nScrollX;
    int m_nScrollY;
    CSize m_sizeLine;
    CSize m_sizeTotal;

// Message map functions
public:
    LRESULT OnSize(HWND hWnd, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnHScroll(HWND hWnd, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnVScroll(HWND hWnd, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CScrollWindowImpl class
//

template <typename T>
class ATL_NO_VTABLE CScrollWindowImpl : public CScrollWindowBase
{
// Constructors
protected:
    CScrollWindowImpl();

// Implementation
public:
    BEGIN_MSG_MAP(CScrollWindowImpl<T>)
        MESSAGE_HANDLER_EX(WM_SIZE, OnSize)
        MESSAGE_HANDLER_EX(WM_HSCROLL, OnHScroll)
        MESSAGE_HANDLER_EX(WM_VSCROLL, OnVScroll)
    END_MSG_MAP()

// Operations
public:
    void ScrollWindow_SetScrollSizes(int cxTotal, int cyTotal, int cxLine, int cyLine);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CImageButtonBase class
//

class ATL_NO_VTABLE CImageButtonBase
{
// Constructors
protected:
    CImageButtonBase();

// Overridables
protected:
    void OnDrawBkgnd(LPDRAWITEMSTRUCT pDrawItem);

// Operations
public:
    void SetImages(int nNormal, int nSelected = 0, int nFocused = 0, int nDisabled = 0);

// Implementation
protected:
    int GetImageIndex(UINT nState) const;

// Data members
public:
    int m_nImages[4];

// Message map functions
public:
    LRESULT OnEraseBkgnd(HWND hWnd, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
};


#ifdef __CEIMAGEUTIL_H__
///////////////////////////////////////////////////////////////////////////////
// Interface of the CImageButtonImpl class
//

template <typename T, typename TBase = ATL::CWindow, typename TWinTraits = CBitmapButtonTraits>
class ATL_NO_VTABLE CImageButtonImpl : public ATL::CWindowImpl<T, TBase, TWinTraits>, public CImageButtonBase
{
// Constructors
protected:
    CImageButtonImpl();

// Implementation
public:
    BEGIN_MSG_MAP(CImageButtonImpl)
        MESSAGE_HANDLER(OCM_DRAWITEM, OnDrawItem)
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
    WCE::CImageList* m_pImageList;

// Message map functions
public:
    LRESULT OnDrawItem(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
};
#endif  // __CEIMAGEUTIL_H__


///////////////////////////////////////////////////////////////////////////////
// Interface of the CBitmapButtonImpl class
//

template <typename T, typename TBase = ATL::CWindow, typename TWinTraits = CBitmapButtonTraits>
class ATL_NO_VTABLE CBitmapButtonImpl : public ATL::CWindowImpl<T, TBase, TWinTraits>, public CImageButtonBase
{
// Constructors
protected:
    CBitmapButtonImpl();

// Implementation
public:
    BEGIN_MSG_MAP(CBitmapButtonImpl)
        MESSAGE_HANDLER(OCM_DRAWITEM, OnDrawItem)
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
    LRESULT OnDrawItem(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CSHToolBarCtrlT class
//

template <typename TBase>
class CSHToolBarCtrlT : public TBase
{
// Constructors
public:
    CSHToolBarCtrlT(HWND hWnd = NULL);

// Operations
public:
    void SHCalcButtonWidth(int cxClient);
    BOOL SHAddButtons(LPCTBBUTTON pButtons);

// Implementation
public:
    CSHToolBarCtrlT<TBase>& operator=(HWND hWnd);
    LRESULT OnCustomDraw(int nIDCtrl, LPNMHDR pNMHDR, BOOL& bHandled);
};


#ifdef __CEIMAGEUTIL_H__
///////////////////////////////////////////////////////////////////////////////
// Interface of the CAnimImageCtrlBase class
//

class ATL_NO_VTABLE CAnimImageCtrlBase
{
// Constructors
protected:
    CAnimImageCtrlBase();

// Operations
public:
    HRESULT LoadImage(IImagingFactory* pFactory, IStream* pStream, DecoderInitFlag flag = DecoderInitFlagBuiltIn1st);
    HRESULT LoadImage(IImagingFactory* pFactory, LPCWSTR pszFileName, DecoderInitFlag flag = DecoderInitFlagBuiltIn1st);

#ifdef _CRT_DEBUG_DUMP
    template <typename _Pr> void Dump(const _Pr& _Printer) const;
#endif

// Overridables
protected:
    void DoPaint(HDC hDC, const RECT& rect);
    void OnDrawBkgnd(HDC hDC, const RECT& rect);

// Attributes
public:
    BOOL IsPlaying() const;
    BOOL IsImageEmpty() const;

    UINT GetImageWidth() const;
    UINT GetImageHeight() const;

// Data members
public:
    BOOL m_bPlaying;
    UINT m_nFrameIndex;
    WCE::CAnimateImage m_AnimImage;

// Message map functions
public:
    LRESULT OnTimer(HWND hWnd, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnEraseBkgnd(HWND hWnd, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
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
    DECLARE_WND_SUPERCLASS(_T("ATL_AnimImageCtrl"), GetWndClassName())
};
#endif  // __CEIMAGEUTIL_H__


///////////////////////////////////////////////////////////////////////////////
// Interface of the CPaintChildBkgndBase class
//

class ATL_NO_VTABLE CPaintChildBkgndBase
{
// Constructors/Destructor
protected:
    CPaintChildBkgndBase();
    ~CPaintChildBkgndBase();

// Implementation
protected:
    void PaintBkgnd_CreateBitmap(HWND hWnd, int cx, int cy);

// Data members
protected:
    HBITMAP m_hbmBkgnd;

// Message map functions
public:
    LRESULT OnNMPaintBkgnd(int nIDCtrl, LPNMHDR pNMHDR, BOOL& bHandled);
    LRESULT OnSize(HWND hWnd, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
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


#ifdef _TODAYCMN_H_
///////////////////////////////////////////////////////////////////////////////
// Interface of the CTodayItemWindowImpl class
//

template <typename T>
class ATL_NO_VTABLE CTodayItemWindowImpl
{
// Constructors
protected:
    CTodayItemWindowImpl();

// Implementation
public:
    BEGIN_MSG_MAP(CTodayItemWindowImpl<T>)
        MESSAGE_HANDLER(WM_LBUTTONUP, OnItemAction)
        MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
        MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
        MESSAGE_HANDLER(WM_TODAYCUSTOM_ACTION, OnItemAction)
        MESSAGE_HANDLER(WM_TODAYCUSTOM_LOSTSELECTION, OnLostSelection)
        MESSAGE_HANDLER(WM_TODAYCUSTOM_RECEIVEDSELECTION, OnReceivedSelection)
        MESSAGE_HANDLER(WM_TODAYCUSTOM_QUERYREFRESHCACHE, OnQueryRefreshCache)
    END_MSG_MAP()

// Overridables
public:
    void OnAction(UINT nChar);
    BOOL QueryRefreshCache(TODAYLISTITEM* ptlItem);

// Operations
public:
    void TookSelection();
    COLORREF GetTextColor() const;

    COLORREF GetColor(UINT nFlags = TODAYCOLOR_TEXT) const;
    HWND InitCustomItem(TODAYLISTITEM* ptlItem, HWND hWndParent);

// Data members
public:
    BOOL m_bSelected;

// Message map functions
public:
    LRESULT OnItemAction(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnEraseBkgnd(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnLostSelection(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnReceivedSelection(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnQueryRefreshCache(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
};
#endif  // _TODAYCMN_H_


#ifdef __GESTUREUTIL_H__
///////////////////////////////////////////////////////////////////////////////
// Interface of the CGesturePaintImpl class
//

template <typename T>
class ATL_NO_VTABLE CGesturePaintImpl
{
// Constructors
protected:
    CGesturePaintImpl();

// Overridables
protected:
    void DoPaint(HDC hDC, const RECT& rcPaint);
    void DoPaintExtent(HDC hDC, const RECT& rcPaint);

// Implementation
protected:
    void OnPaint(const SIZE& sizeBoundary, int nScrollX, int nScrollY);
    void OnPaintExtent(HDC hDC, const RECT& rcPaint, int left, int top, int right, int bottom);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CGestureWindowBase class
//

class ATL_NO_VTABLE CGestureWindowBase
{
// Constructors/Destructor
protected:
    CGestureWindowBase();
    ~CGestureWindowBase();

// Operations
public:
    void GestureWindow_Init(int cxBoundary, int cyBoundary, UINT nFlags = GWIF_VSCROLLABLE | GWIF_HSCROLLABLE, BYTE nHorizontalExtent = 50, BYTE nVerticalExtent = 50);

// Overridables
protected:
    BOOL OnGestureBegin(const GESTUREINFO& info);
    BOOL OnGestureEnd(const GESTUREINFO& info);
    BOOL OnGestureHold(const GESTUREINFO& info);
    BOOL OnGestureScroll(const GESTUREINFO& info);
    BOOL OnGestureSelect(const GESTUREINFO& info);
    BOOL OnGestureDoubleSelect(const GESTUREINFO& info);
    void OnGestureAnimation(HWND hWnd, int nScrollX, int nScrollY, long lVelocityX, long lVelocityY, DWORD dwTime);

// Attributes
public:
    BOOL IsAnimationStopped() const;

// Implementation
protected:
    void StartScroll(const GESTUREINFO& info);
    void StopScroll(HWND hWnd);
    POINT AdjustScrollPos(HWND hWnd, int nScrollX, int nScrollY);

// Data members
protected:
    UINT m_nFlags;
    int  m_nScrollX;
    int  m_nScrollY;
    BYTE m_nHoriExtent;
    BYTE m_nVertExtent;
    POINTS m_ptsLocation;
    CSize  m_sizeBoundary;
    WCE::CPhysicsEngine m_PhysicsEngine;

// Message map functions
public:
    LRESULT OnSize(HWND hWnd, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnDestroy(HWND hWnd, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CGestureWindowImpl class
//

template <typename T>
class ATL_NO_VTABLE CGestureWindowImpl : public CGestureWindowBase, public CGesturePaintImpl<T>
{
// Constructors
protected:
    CGestureWindowImpl();

// Implementation
public:
    BEGIN_MSG_MAP(CGestureWindowImpl<T>)
        MESSAGE_HANDLER(WM_PAINT, OnPaint)
        MESSAGE_HANDLER(WM_TIMER, OnTimer)
        MESSAGE_HANDLER_EX(WM_SIZE, OnSize)
        MESSAGE_HANDLER(WM_GESTURE, OnGesture)
        MESSAGE_HANDLER_EX(WM_DESTROY, OnDestroy)
    END_MSG_MAP()

// Overridables
protected:
    BOOL OnGesturePan(const GESTUREINFO& info);

// Message map functions
public:
    LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnGesture(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CAutoGestureWindowImpl class
//

template <typename T>
class ATL_NO_VTABLE CAutoGestureWindowImpl : public CScrollWindowImpl<T>, public CGesturePaintImpl<T>
{
// Constructors
protected:
    CAutoGestureWindowImpl();

// Implementation
public:
    BEGIN_MSG_MAP(CAutoGestureWindowImpl<T>)
        MESSAGE_HANDLER(WM_PAINT, OnPaint)
        MESSAGE_HANDLER(WM_GESTUREANIMATION, OnGestureAnimation)
        CHAIN_MSG_MAP(CScrollWindowImpl<T>)
    END_MSG_MAP()

// Operations
public:
    BOOL AutoGestureWindow_Init(DWORD dwFlags = WAGIF_OWNERANIMATE | WAGIF_VSCROLLABLE | WAGIF_HSCROLLABLE, UINT nAnimateStatusMessage = 0, BYTE nHorizontalExtent = 50, BYTE nVerticalExtent = 50, HBRUSH hExtentBrush = NULL);

// Message map functions
public:
    LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnGestureAnimation(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
};
#endif  // __GESTUREUTIL_H__


#ifdef __AYGSHELL_H__
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
        MESSAGE_HANDLER(WM_FILECHANGEINFO, OnFileChange)
    END_MSG_MAP()

// Overridables
public:
    void SHChangeNotify_OnFileChange(const FILECHANGEINFO& fci);

// Operations
public:
    BOOL SHChangeNotify_Register(DWORD dwEvent = SHCNE_ALLEVENTS, LPCTSTR pszWatchDir = NULL, BOOL bRecursive = TRUE);
    BOOL SHChangeNotify_Unregister();

// Message map functions
public:
    LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnFileChange(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
};
#endif  // __AYGSHELL_H__


#ifdef __ATLCTRLS_H__
///////////////////////////////////////////////////////////////////////////////
// Interface of the CBitmapButton class
//

class CBitmapButton : public CBitmapButtonImpl<CBitmapButton, WTL::CButton>
{
public:
    DECLARE_WND_SUPERCLASS(_T("ATL_BitmapButton"), GetWndClassName());
};


#ifdef __CEIMAGEUTIL_H__
///////////////////////////////////////////////////////////////////////////////
// Interface of the CImageButton class
//

class CImageButton : public CImageButtonImpl<CImageButton, WTL::CButton>
{
public:
    DECLARE_WND_SUPERCLASS(_T("ATL_ImageButton"), GetWndClassName());
};
#endif  // __CEIMAGEUTIL_H__
#endif  // __ATLCTRLS_H__

}  // namespace WCE

#include "atlcectrls.inl"

#endif  // __ATLCECTRLS_H__