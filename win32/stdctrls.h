///////////////////////////////////////////////////////////////////////////////
// stdctrls.h
//
// Author : Garfield
// Creation Date : 2007/1/21

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __STDCTRLS_H__
#define __STDCTRLS_H__

#ifdef _WIN32_WCE
    #error stdctrls.h compiles under Windows only
#endif

#include <atlbase.h>
#include <atlwin.h>
#include <atlstr.h>
#include <atlcoll.h>
#include "sysutil.h"
#include "atlcomutil.h"
#include <atlcomtime.h>

///////////////////////////////////////////////////////////////////////////////
// Classes in this file:
//
// CEditBase
// CDragDropBase
// CLinkCtrlBase
// CBalloonTipCtrlT<TBase>
// CBalloonTipCtrl
// CBalloonTipBase
// CHeaderCtrlBase
// CImageCtrlBase
// CAnimImageCtrlBase
// CPictureCtrlBase
// CShellTrayNotify
// CImageButtonBase
// CSplashWindowBase
// CSortListCtrlBase
// CImageListBoxBase
// CColorComboBoxBase
// CStatusBarCtrlBase
// CPaintChildBkgndBase

namespace atlctrls {

///////////////////////////////////////////////////////////////////////////////
// Interface of the CEditBase class
//

class ATL_NO_VTABLE CEditBase
{
// Constructors
protected:
    CEditBase();

// Overridables
protected:
    BOOL OnDrawFrame(HDC hDC, UINT nState, const RECT& rcWindow);

// Implementation
protected:
    BOOL SetState(HWND hWnd, UINT nState);

// Data members
public:
    UINT m_nState;      // 0, CDIS_HOT for CEditImpl / 0, CDIS_HOT, CDIS_SELECTED for CMarginEditImpl

// Message map functions
public:
    LRESULT OnSetFocus(HWND hWnd, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnKillFocus(HWND hWnd, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnMouseMove(HWND hWnd, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnMouseLeave(HWND hWnd, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CDragDropBase class
//

class ATL_NO_VTABLE CDragDropBase
{
// Constructors/Destructor
protected:
    CDragDropBase();
    ~CDragDropBase();

// Operations
public:
    void RegisterDragDrop(HWND hWnd);

// Overridables
protected:
    HIMAGELIST CreateDragImageEx(LPNMHDR pNMHDR, POINT& ptDrag, POINT& ptHotSpot);

// Implementation
protected:
    void BeginDrag(HWND hWnd, POINT point, POINT ptHotSpot);

// Data members
public:
#ifdef _ATL_SUPPORT_CDD_GETDRAGDROPFLAG
    DWORD m_dwFlags;
#endif
    HWND m_hWndDrop;
    HIMAGELIST m_hDragImage;

// Message map functions
public:
    LRESULT OnButtonUp(HWND hWnd, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnMouseMove(HWND hWnd, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CLinkCtrlBase class
//

class ATL_NO_VTABLE CLinkCtrlBase
{
// Constructors/Destructor
protected:
    CLinkCtrlBase(DWORD dwExStyle);
    ~CLinkCtrlBase();

// Overridables
protected:
    void DoPaint(HWND hWnd, HDC hDC, const RECT& rect);
    void OnDrawBkgnd(HWND hWnd, HDC hDC, const RECT& rect);

// Implementation
protected:
    BOOL SizeToContent(HWND hWnd);
    HFONT CreateFont(HFONT hFont);

// Data members
public:
    HFONT m_hFont;
    DWORD m_dwExStyle;
    BOOL m_bMouseHover;
    COLORREF m_clrText;
    COLORREF m_clrHover;

// Message map functions
public:
    LRESULT OnEnable(HWND hWnd, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnSetFont(HWND hWnd, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnSetCursor(HWND hWnd, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnMouseMove(HWND hWnd, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnMouseLeave(HWND hWnd, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnEraseBkgnd(HWND hWnd, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CBalloonTipCtrlT class
//

template <typename TBase>
class CBalloonTipCtrlT : public TBase
{
// Constructors
public:
    CBalloonTipCtrlT(HWND hWnd = NULL);

// Operations
public:
    HWND Create(HWND hWndParent, DWORD dwStyle = TTS_BALLOON, int nMaxWidth = 300);
    void RelayEvent(LPMSG pMsg);
    BOOL IsInputMessage(LPMSG pMsg) const;

    void DeleteItem(HWND hWnd);
    BOOL AddItem(HWND hWnd, ATL::_U_STRINGorID text = LPSTR_TEXTCALLBACK, UINT nFlags = 0);

    void HideBalloonTip();
    void ShowBalloonTip(HWND hWnd, ATL::_U_STRINGorID title = (LPCTSTR)NULL, UINT_PTR nIcon = TTI_WARNING, int x = -1, int y = -1, UINT nElapse = 5000);

// Attributes
public:
    int GetItemCount() const;

    CString GetItemText(HWND hWnd) const;
    HRESULT GetItemText(HWND hWnd, BSTR& bstrText) const;
    void SetItemText(HWND hWnd, ATL::_U_STRINGorID text);

// Implementation
public:
    static LPCTSTR WINAPI GetWndClassName();
    CBalloonTipCtrlT<TBase>& operator=(HWND hWnd);
};

typedef CBalloonTipCtrlT<ATL::CWindow> CBalloonTipCtrl;


///////////////////////////////////////////////////////////////////////////////
// Interface of the CBalloonTipBase class
//

class ATL_NO_VTABLE CBalloonTipBase
{
// Constructors
protected:
    CBalloonTipBase();

// Operations
public:
    void BalloonTip_SetItemText(HWND hWnd, ATL::_U_STRINGorID text);
    void BalloonTip_DeleteItem(HWND hWnd);
    BOOL BalloonTip_AddItem(HWND hWnd, ATL::_U_STRINGorID text = LPSTR_TEXTCALLBACK, UINT nFlags = 0);

// Attributes
public:
    int BalloonTip_GetItemCount() const;

// Implementation
protected:
    void BalloonTip_ShowWindow(HWND hWnd, HWND hWndCtrl, ATL::_U_STRINGorID title, UINT_PTR nIcon, int x, int y, UINT nElapse);

// Data members
protected:
    CBalloonTipCtrl m_wndBalloon;

// Message map functions
public:
    LRESULT OnActivate(HWND hWnd, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CHeaderCtrlBase class
//

class ATL_NO_VTABLE CHeaderCtrlBase
{
// Constructors
protected:
    CHeaderCtrlBase(int nHeight);

// Overridables
protected:
    void OnDrawItem(const HDDRAWITEM& hddi);
    void OnDrawBkgnd(const HDDRAWITEM& hddi);
    void OnMouseMove(int nItem, UINT nFlags, UINT nHitFlags);

// Data members
public:
#ifdef _ATL_SUPPORT_CHD_SETHEIGHT
    int m_nHeight;
#endif

    int  m_nItem;
    UINT m_nState;
    static BOOL m_bThemeActive;

// Message map functions
public:
    LRESULT OnMouseEvent(HWND hWnd, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnEraseBkgnd(HWND hWnd, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnLButtonDown(HWND hWnd, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnThemeChanged(HWND hWnd, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
};


#ifdef __IMAGE_H__
///////////////////////////////////////////////////////////////////////////////
// Interface of the CImageCtrlBase class
//

class ATL_NO_VTABLE CImageCtrlBase
{
// Constructors
protected:
    CImageCtrlBase();

// Operations
public:
    BOOL LoadImage(LPCWSTR pszFileName, BOOL bUseEmbeddedColorManagement = FALSE);
    BOOL LoadImage(IStream* pStream, BOOL bUseEmbeddedColorManagement = FALSE);
    BOOL LoadImage(const BITMAPINFO* pBitmapInfo, LPVOID pvBitmapData);
    BOOL LoadImage(HINSTANCE hInstance, UINT nIDResource, LPCTSTR pType = NULL, BOOL bUseEmbeddedColorManagement = FALSE);

    BOOL SaveImage(IStream* pStream, const CLSID* pclsIdEncoder, const Gdiplus::EncoderParameters* pParameters = NULL) const;
    BOOL SaveImage(LPCWSTR pszFileName, const CLSID* pclsIdEncoder, const Gdiplus::EncoderParameters* pParameters = NULL) const;

#if defined(__ATLSAFE_H__) && defined(__ATLCOMUTIL_H__)
    HRESULT LoadImage(const ATL::CComSafeArray<BYTE>& saImage, BOOL bUseEmbeddedColorManagement = FALSE);
    HRESULT SaveImage(ATL::CComSafeArray<BYTE>& saImage, const CLSID* pclsIdEncoder, const Gdiplus::EncoderParameters* pParameters = NULL) const;
#endif

// Overridables
protected:
    void DoPaint(HWND hWnd, HDC hDC);

// Attributes
public:
    BOOL IsImageEmpty() const;
    BOOL GetBitmap(BITMAP& bmp) const;

// Data members
public:
    int m_nSizeMode;
    atlutil::CImage m_image;

// Message map functions
public:
    LRESULT OnEraseBkgnd(HWND hWnd, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
};


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
    BOOL LoadImage(LPCTSTR pszFileName, BOOL bUseEmbeddedColorManagement = FALSE);
    BOOL LoadImage(IStream* pStream, BOOL bUseEmbeddedColorManagement = FALSE);

#if defined(__ATLSAFE_H__) && defined(__ATLCOMUTIL_H__)
    HRESULT LoadImage(const ATL::CComSafeArray<BYTE>& saImage, BOOL bUseEmbeddedColorManagement = FALSE);
#endif

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
    BOOL GetBitmap(BITMAP& bmp) const;

// Data members
public:
    BOOL m_bPlaying;
    UINT m_nFrameIndex;
    atlutil::CAnimateImage m_AnimImage;

// Message map functions
public:
    LRESULT OnTimer(HWND hWnd, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnEraseBkgnd(HWND hWnd, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
};
#endif  // __IMAGE_H__


#ifdef __PICTURE_H__
///////////////////////////////////////////////////////////////////////////////
// Interface of the CPictureCtrlBase class
//

class ATL_NO_VTABLE CPictureCtrlBase
{
// Constructors
protected:
    CPictureCtrlBase();

// Operations
public:
    HRESULT LoadPicture(LPCTSTR pszURLorPath);
    HRESULT LoadPicture(IStream* pStream, BOOL bRunmode = FALSE);
    HRESULT LoadPicture(HINSTANCE hInstance, UINT nIDResource, LPCTSTR pType, BOOL bRunmode = FALSE);

    HRESULT SavePicture(LPCTSTR pszFileName) const;
    HRESULT SavePicture(IStream* pStream, BOOL bSaveMemCopy = FALSE, LONG* pcbSize = NULL) const;

#if defined(__ATLSAFE_H__) && defined(__ATLCOMUTIL_H__)
    HRESULT SavePicture(ATL::CComSafeArray<BYTE>& saPicture) const;
    HRESULT LoadPicture(const ATL::CComSafeArray<BYTE>& saPicture, BOOL bRunmode = FALSE);
#endif

// Overridables
protected:
    void DoPaint(HWND hWnd, HDC hDC);

// Attributes
public:
    BOOL IsPictureEmpty() const;

    OLE_XSIZE_HIMETRIC GetPictureWidth() const;
    OLE_YSIZE_HIMETRIC GetPictureHeight() const;

// Data members
public:
    int m_nSizeMode;
    stdutil::CPicture m_Picture;

// Message map functions
public:
    LRESULT OnEraseBkgnd(HWND hWnd, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
};
#endif  // __PICTURE_H__


///////////////////////////////////////////////////////////////////////////////
// Interface of the CShellTrayNotify class
//

class ATL_NO_VTABLE CShellTrayNotify
{
// Constructors
private:
    CShellTrayNotify();

// Operations
public:
    static BOOL WINAPI Create(HWND hWnd, UINT nID, HICON hIcon, LPCTSTR pszTip, UINT nCallbackMessage = 0);
    static BOOL WINAPI Create(HWND hWnd, UINT nID, HICON hIcon, LPCTSTR pszTip, LPCTSTR pszInfo, LPCTSTR pszInfoTitle, UINT nCallbackMessage = 0, DWORD dwInfoFlags = NIIF_INFO, HICON hBalloonIcon = NULL, UINT nTimeout = 5000);

    static BOOL WINAPI SetIcon(HWND hWnd, UINT nID, HICON hIcon);
    static BOOL WINAPI SetTipText(HWND hWnd, UINT nID, LPCTSTR pszTip);
    static BOOL WINAPI SetCallbackMessage(HWND hWnd, UINT nID, UINT nCallbackMessage);

    static BOOL WINAPI SetState(HWND hWnd, UINT nID, DWORD dwState, DWORD dwStateMask);
    static BOOL WINAPI SetVersion(HWND hWnd, UINT nID, UINT nVersion = NOTIFYICON_VERSION);

    static BOOL WINAPI Destroy(HWND hWnd, UINT nID);
    static BOOL WINAPI NotifyIcon(const NOTIFYICONDATA& nid, DWORD dwMessage = NIM_MODIFY);
    static BOOL WINAPI ShowBalloonTip(HWND hWnd, UINT nID, LPCTSTR pszInfo, LPCTSTR pszInfoTitle, DWORD dwInfoFlags = NIIF_INFO, HICON hBalloonIcon = NULL, UINT nTimeout = 5000);
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
    void SetImages(int nNormal, int nHover = 0, int nSelected = 0, int nDisabled = 0);

// Implementation
protected:
    int GetImageIndex(UINT nState) const;

// Data members
public:
    int m_nImages[4];
    BOOL m_bMouseHover;

// Message map functions
public:
    LRESULT OnMouseMove(HWND hWnd, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnMouseLeave(HWND hWnd, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnEraseBkgnd(HWND hWnd, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CSplashWindowBase class
//

class ATL_NO_VTABLE CSplashWindowBase
{
protected:
    typedef ATL::CAtlList<DRAWTEXT> CDrawTextList;

// Constructors
protected:
    CSplashWindowBase();

// Operations
public:
    POSITION AddDrawText(const DRAWTEXT& drawText);
    POSITION AddDrawText(UINT nIDText, const RECT& rect, COLORREF clrText = RGB(0, 0, 0), UINT nFormat = DT_SINGLELINE | DT_VCENTER, LPLOGFONT pLogFont = NULL);
    POSITION AddDrawText(LPCTSTR pszText, const RECT& rect, COLORREF clrText = RGB(0, 0, 0), UINT nFormat = DT_SINGLELINE | DT_VCENTER, LPLOGFONT pLogFont = NULL);

// Overridables
protected:
    void DoPaint(HWND hWnd, HDC hDC);

// Implementation
protected:
    void SetDrawText(HWND hWnd, POSITION pos, LPCTSTR pszText, COLORREF clrText, LPLOGFONT pLogFont, UINT nFormat, LPCRECT pRect);

// Data members
public:
    HBITMAP m_hBitmap;
    CDrawTextList m_drawTexts;

// Message map functions
public:
    LRESULT OnEraseBkgnd(HWND hWnd, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CSortListCtrlBase class
//

class ATL_NO_VTABLE CSortListCtrlBase
{
// Constructors
protected:
    CSortListCtrlBase();

// Operations
public:
    static int WINAPI CompareInteger(LPCTSTR pszSource, LPCTSTR pszDest);
    static int WINAPI CompareDecimal(LPCTSTR pszSource, LPCTSTR pszDest);
    static int WINAPI CompareDateTime(LPCTSTR pszSource, LPCTSTR pszDest);
    static int CALLBACK CompareItem(LPARAM lSourceItem, LPARAM lDestItem, LPARAM lSortParam);

// Overridables
protected:
    void SetSortCol(HWND hWnd, int nCol, BOOL bAscending);

// Data members
public:
    int  m_nSortCol;
    BOOL m_bAscending;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CImageListBoxBase class
//

class ATL_NO_VTABLE CImageListBoxBase
{
protected:
    typedef ATL::CAtlArray<int> CIntArray;

// Constructors
protected:
    CImageListBoxBase();

// Attributes
public:
    int GetItemImage(int nItem) const;
    void SetItemImage(int nItem, int nImage);

// Implementation
protected:
    BOOL SetImageList(HWND hWnd, HIMAGELIST hImageList);

    int AddItem(HWND hWnd, LPCTSTR pszText, int nImage);
    int InsertItem(HWND hWnd, int nItem, LPCTSTR pszText, int nImage);

// Data members
public:
#ifdef _ATL_SUPPORT_CILB_TEXTCOLOR
    COLORREF m_clrText;
    COLORREF m_clrBack;
#endif
    CIntArray  m_arrImages;
    HIMAGELIST m_hImageList;

// Message map functions
public:
#ifdef _ATL_SUPPORT_CILB_TEXTCOLOR
    LRESULT OnEnable(HWND hWnd, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnEraseBkgnd(HWND hWnd, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
#endif
    LRESULT OnDrawItem(HWND hWnd, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CColorComboBoxBase class
//

class ATL_NO_VTABLE CColorComboBoxBase
{
// Constructors
protected:
    CColorComboBoxBase();

// Operations
public:
    static const COLORMAPENTRY* WINAPI GetColorMap();

// Implementation
protected:
    static BOOL WINAPI GetVkScanUp();
    static BOOL WINAPI GetVkScanDown();

    static int WINAPI GetCustomItem(HWND hWnd);
    static int WINAPI GetSeparatorItem(HWND hWnd);

    static void WINAPI SetSelectedColor(HWND hWnd, COLORREF color);
    static void WINAPI SetColorMap(HWND hWnd, const COLORMAPENTRY* pColorMap, int nItemHeight);

// Data members
public:
    UINT m_nCurSel;

// Message map functions
public:
    LRESULT OnSelCustom(int nIDCtrl, LPNMHDR pNMHDR, BOOL& bHandled);
    LRESULT OnDrawItem(HWND hWnd, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnCloseUp(WORD wNotifyCode, WORD wID, HWND hWndCtrl, BOOL& bHandled);
    LRESULT OnSelEndOK(WORD wNotifyCode, WORD wID, HWND hWndCtrl, BOOL& bHandled);
    LRESULT OnSelChange(WORD wNotifyCode, WORD wID, HWND hWndCtrl, BOOL& bHandled);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CStatusBarCtrlBase class
//

class ATL_NO_VTABLE CStatusBarCtrlBase
{
// Constructors/Destructor
protected:
    CStatusBarCtrlBase(DWORD dwExStyle);
    ~CStatusBarCtrlBase();

// Overridables
protected:
    void CalcPaneWidth(HWND hWnd);
    void OnDrawPane(const SBDRAWPANE& drawPane);
    void GetPaneRect(HWND hWnd, int nIndex, BOOL bSimple, RECT& rcPane) const;

// Attributes
public:
    int GetPaneWidth(int nIndex) const;

// Implementation
protected:
    BOOL SetPanes(HWND hWnd, const SBPANE* pPanes, int nCount);

// Data members
public:
    DWORD m_dwExStyle;
    int   m_nPaneCount;
    int*  m_pPaneWidth;
};


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

}  // namespace atlctrls

#include "stdctrls.inl"

#endif  // __STDCTRLS_H__