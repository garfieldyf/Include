/******************************************************************************
 *
 *  atlctrls.h : 头文件
 *
 *  Copyright(c) 2006, Garfield. All rights reserved.
 *  Author  : Garfield
 *  Version : 1.0
 *  Finish Time : 2006/12/24
 *
 *****************************************************************************/

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __ATLCONTROLS_H__
#define __ATLCONTROLS_H__

#ifndef __ATLWIN_H__
    #error atlctrls.h requires atlwin.h to be included first
#endif

#ifndef _INC_COMMCTRL
    #error atlctrls.h requires commctrl.h to be included first
#endif

namespace ATL_CTRLS {

template <class TBase> class CStaticT;
template <class TBase> class CButtonT;
template <class TBase> class CListBoxT;
template <class TBase> class CComboBoxT;
template <class TBase> class CEditT;
template <class TBase> class CScrollBarT;
template <class TBase> class CListCtrlT;
template <class TBase> class CHeaderCtrlT;
template <class TBase> class CToolTipCtrlT;

///////////////////////////////////////////////////////////////////////////////
// CStatic 类的定义
//

template <class TBase>
class CStaticT : public TBase
{
// 构造
public:
    CStaticT(HWND hWnd = NULL)
        : TBase(hWnd)
    {
    }

    CStaticT<TBase>& operator = (HWND hWnd)
    {
        m_hWnd = hWnd;
        return *this;
    }

    HWND Create(HWND hWndParent, ATL::_U_RECT rect = NULL, LPCTSTR lpszWindowName = NULL, DWORD dwStyle = 0,
                DWORD dwExStyle = 0, ATL::_U_MENUorID MenuOrID = 0U, LPVOID lpCreateParam = NULL)
    {
        return TBase::Create(GetWndClassName(), hWndParent, rect, lpszWindowName, dwStyle, dwExStyle, MenuOrID, lpCreateParam);
    }

// 属性
public:
    static LPCTSTR GetWndClassName()
    {
        return WC_STATIC;
    }

    HICON GetIcon() const
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (HICON)::SendMessage(m_hWnd, STM_GETICON, 0. 0L);
    }

    HICON SetIcon(HICON hIcon)
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (HICON)::SendMessage(m_hWnd, STM_SETICON, (WPARAM)hIcon, 0L);
    }

    HCURSOR GetCursor() const
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (HCURSOR)::SendMessage(m_hWnd, STM_GETIMAGE, IMAGE_CURSOR, 0L);
    }

    HCURSOR SetCursor(HCURSOR hCursor)
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (HCURSOR)::SendMessage(m_hWnd, STM_SETIMAGE, IMAGE_CURSOR, (LPARAM)hCursor);
    }

    HBITMAP GetBitmap() const
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (HBITMAP)::SendMessage(m_hWnd, STM_GETIMAGE, IMAGE_BITMAP, 0L);
    }

    HBITMAP SetBitmap(HBITMAP hBitmap)
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (HBITMAP)::SendMessage(m_hWnd, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap);
    }

    HENHMETAFILE GetEnhMetaFile() const
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (HENHMETAFILE)::SendMessage(m_hWnd, STM_GETIMAGE, IMAGE_ENHMETAFILE, 0L);
    }

    HENHMETAFILE SetEnhMetaFile(HENHMETAFILE hMetaFile)
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (HENHMETAFILE)::SendMessage(m_hWnd, STM_SETIMAGE, IMAGE_ENHMETAFILE, (LPARAM)hMetaFile);
    }
};

typedef CStaticT<ATL::CWindow> CStatic;


///////////////////////////////////////////////////////////////////////////////
// CButton 类的定义
//

template <class TBase>
class CButtonT : public TBase
{
// 构造
public:
    CButtonT(HWND hWnd = NULL)
        : TBase(hWnd)
    {
    }

    CButtonT<TBase>& operator = (HWND hWnd)
    {
        m_hWnd = hWnd;
        return *this;
    }

    HWND Create(HWND hWndParent, ATL::_U_RECT rect = NULL, LPCTSTR lpszWindowName = NULL, DWORD dwStyle = 0,
                DWORD dwExStyle = 0, ATL::_U_MENUorID MenuOrID = 0U, LPVOID lpCreateParam = NULL)
    {
        return TBase::Create(GetWndClassName(), hWndParent, rect, lpszWindowName, dwStyle, dwExStyle, MenuOrID, lpCreateParam);
    }

// 属性
public:
    static LPCTSTR GetWndClassName()
    {
        return WC_BUTTON;
    }

    UINT GetState() const
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (UINT)::SendMessage(m_hWnd, BM_GETSTATE, 0, 0L);
    }

    void SetState(BOOL bHighlight = TRUE)
    {
        ATLASSERT(::IsWindow(m_hWnd));
        ::SendMessage(m_hWnd, BM_SETSTATE, bHighlight, 0L);
    }

    int GetCheck() const
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (int)::SendMessage(m_hWnd, BM_GETCHECK, 0, 0L);
    }

    void SetCheck(int nCheck)
    {
        ATLASSERT(::IsWindow(m_hWnd));
        ::SendMessage(m_hWnd, BM_SETCHECK, nCheck, 0L);
    }

    UINT GetButtonStyle() const
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (UINT)::GetWindowLong(m_hWnd, GWL_STYLE) & 0xFFFF;
    }

    void SetButtonStyle(UINT nStyle, BOOL bRedraw = TRUE)
    {
        ATLASSERT(::IsWindow(m_hWnd));
        ::SendMessage(m_hWnd, BM_SETSTYLE, nStyle, (LPARAM)bRedraw);
    }

    HICON GetIcon() const
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (HICON)::SendMessage(m_hWnd, BM_GETIMAGE, IMAGE_ICON, 0L);
    }

    HICON SetIcon(HICON hIcon)
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (HICON)::SendMessage(m_hWnd, BM_SETIMAGE, IMAGE_ICON, (LPARAM)hIcon);
    }

    HCURSOR GetCursor() const
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (HCURSOR)::SendMessage(m_hWnd, BM_GETIMAGE, IMAGE_CURSOR, 0L);
    }

    HCURSOR SetCursor(HCURSOR hCursor)
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (HCURSOR)::SendMessage(m_hWnd, BM_SETIMAGE, IMAGE_CURSOR, (LPARAM)hCursor);
    }

    HBITMAP GetBitmap() const
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (HBITMAP)::SendMessage(m_hWnd, BM_GETIMAGE, IMAGE_BITMAP, 0L);
    }

    HBITMAP SetBitmap(HBITMAP hBitmap)
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (HBITMAP)::SendMessage(m_hWnd, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap);
    }

#if (_WIN32_WINNT >= 0x0501)
    BOOL GetIdealSize(LPSIZE lpSize) const
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (BOOL)::SendMessage(m_hWnd, BCM_GETIDEALSIZE, 0, (LPARAM)lpSize);
    }

    BOOL GetImageList(PBUTTON_IMAGELIST pButtonImagelist) const
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (BOOL)::SendMessage(m_hWnd, BCM_GETIMAGELIST, 0, (LPARAM)pButtonImagelist);
    }

    BOOL SetImageList(PBUTTON_IMAGELIST pButtonImagelist)
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (BOOL)::SendMessage(m_hWnd, BCM_SETIMAGELIST, 0, (LPARAM)pButtonImagelist);
    }

    BOOL GetTextMargin(LPRECT lpRect) const
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (BOOL)::SendMessage(m_hWnd, BCM_GETTEXTMARGIN, 0, (LPARAM)lpRect);
    }

    BOOL SetTextMargin(LPRECT lpRect)
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (BOOL)::SendMessage(m_hWnd, BCM_SETTEXTMARGIN, 0, (LPARAM)lpRect);
    }
#endif  // _WIN32_WINNT >= 0x0501
};

typedef CButtonT<ATL::CWindow> CButton;


///////////////////////////////////////////////////////////////////////////////
// CListBox 类的定义
//

template <class TBase>
class CListBoxT : public TBase
{
// 构造
public:
    CListBoxT(HWND hWnd = NULL)
        : TBase(hWnd)
    {
    }

    CListBoxT<TBase>& operator = (HWND hWnd)
    {
        m_hWnd = hWnd;
        return *this;
    }

    HWND Create(HWND hWndParent, ATL::_U_RECT rect = NULL, LPCTSTR lpszWindowName = NULL, DWORD dwStyle = 0,
                DWORD dwExStyle = 0, ATL::_U_MENUorID MenuOrID = 0U, LPVOID lpCreateParam = NULL)
    {
        return TBase::Create(GetWndClassName(), hWndParent, rect, lpszWindowName, dwStyle, dwExStyle, MenuOrID, lpCreateParam);
    }

// 属性
public:
    static LPCTSTR GetWndClassName()
    {
        return WC_LISTBOX;
    }

    int GetCount() const
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (int)::SendMessage(m_hWnd, LB_GETCOUNT, 0, 0L);
    }

    int GetHorizontalExtent() const
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (int)::SendMessage(m_hWnd, LB_GETHORIZONTALEXTENT, 0, 0L);
    }

    void SetHorizontalExtent(int cxExtent)
    {
        ATLASSERT(::IsWindow(m_hWnd));
        ::SendMessage(m_hWnd, LB_SETHORIZONTALEXTENT, cxExtent, 0L);
    }

    int GetTopIndex() const
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (int)::SendMessage(m_hWnd, LB_GETTOPINDEX, 0, 0L);
    }

    int SetTopIndex(int nIndex)
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (int)::SendMessage(m_hWnd, LB_SETTOPINDEX, nIndex, 0L);
    }

    LCID GetLocale() const
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (LCID)::SendMessage(m_hWnd, LB_GETLOCALE, 0, 0L);
    }

    LCID SetLocale(LCID nNewLocale)
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (LCID)::SendMessage(m_hWnd, LB_SETLOCALE, (WPARAM)nNewLocale, 0L);
    }

#if (WINVER >= 0x0500)
    DWORD GetListBoxInfo() const
    {
        ATLASSERT(::IsWindow(m_hWnd));
    #if (_WIN32_WINNT >= 0x0501)
        return (DWORD)::SendMessage(m_hWnd, LB_GETLISTBOXINFO, 0, 0L);
    #else   // _WIN32_WINNT < 0x0501
        return ::GetListBoxInfo(m_hWnd);
    #endif  // _WIN32_WINNT >= 0x0501
    }
#endif  // WINVER >= 0x0500

    int GetCurSel() const
    {
        ATLASSERT(::IsWindow(m_hWnd));
        ATLASSERT((GetStyle() & (LBS_MULTIPLESEL | LBS_EXTENDEDSEL)) == 0);

        return (int)::SendMessage(m_hWnd, LB_GETCURSEL, 0, 0L);
    }

    int SetCurSel(int nSelect)
    {
        ATLASSERT(::IsWindow(m_hWnd));
        ATLASSERT((GetStyle() & (LBS_MULTIPLESEL | LBS_EXTENDEDSEL)) == 0);

        return (int)::SendMessage(m_hWnd, LB_SETCURSEL, nSelect, 0L);
    }

    int GetSel(int nIndex) const
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (int)::SendMessage(m_hWnd, LB_GETSEL, nIndex, 0L);
    }

    int SetSel(int nIndex, BOOL bSelect = TRUE)
    {
        ATLASSERT(::IsWindow(m_hWnd));
        ATLASSERT((GetStyle() & (LBS_MULTIPLESEL | LBS_EXTENDEDSEL)) != 0);

        return (int)::SendMessage(m_hWnd, LB_SETSEL, bSelect, nIndex);
    }

    int GetSelCount() const
    {
        ATLASSERT(::IsWindow(m_hWnd));
        ATLASSERT((GetStyle() & (LBS_MULTIPLESEL | LBS_EXTENDEDSEL)) != 0);

        return (int)::SendMessage(m_hWnd, LB_GETSELCOUNT, 0, 0L);
    }

    int GetSelItems(int nMaxItems, LPINT rgIndex) const
    {
        ATLASSERT(::IsWindow(m_hWnd));
        ATLASSERT((GetStyle() & (LBS_MULTIPLESEL | LBS_EXTENDEDSEL)) != 0);

        return (int)::SendMessage(m_hWnd, LB_GETSELITEMS, nMaxItems, (LPARAM)rgIndex);
    }

    int GetAnchorIndex() const
    {
        ATLASSERT(::IsWindow(m_hWnd));
        ATLASSERT((GetStyle() & (LBS_MULTIPLESEL | LBS_EXTENDEDSEL)) != 0);
        
        return (int)::SendMessage(m_hWnd, LB_GETANCHORINDEX, 0, 0L);
    }

    void SetAnchorIndex(int nIndex)
    {
        ATLASSERT(::IsWindow(m_hWnd));
        ATLASSERT((GetStyle() & (LBS_MULTIPLESEL | LBS_EXTENDEDSEL)) != 0);
        
        ::SendMessage(m_hWnd, LB_SETANCHORINDEX, nIndex, 0L);
    }

    int GetCaretIndex() const
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (int)::SendMessage(m_hWnd, LB_GETCARETINDEX, 0, 0L);
    }

    int SetCaretIndex(int nIndex, BOOL bScroll = TRUE)
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (int)::SendMessage(m_hWnd, LB_SETCARETINDEX, nIndex, MAKELONG(bScroll, 0));
    }

    void SetColumnWidth(int cxWidth)
    {
        ATLASSERT(::IsWindow(m_hWnd));
        ::SendMessage(m_hWnd, LB_SETCOLUMNWIDTH, cxWidth, 0L);
    }

    BOOL SetTabStops()
    {
        ATLASSERT(::IsWindow(m_hWnd));
        ATLASSERT((GetStyle() & LBS_USETABSTOPS) != 0);
        
        return (BOOL)::SendMessage(m_hWnd, LB_SETTABSTOPS, 0, 0L);
    }

    BOOL SetTabStops(int nTabStops, LPINT rgTabStops)
    {
        ATLASSERT(::IsWindow(m_hWnd));
        ATLASSERT((GetStyle() & LBS_USETABSTOPS) != 0);
        
        return (BOOL)::SendMessage(m_hWnd, LB_SETTABSTOPS, nTabStops, (LPARAM)rgTabStops);
    }

    BOOL SetTabStops(const int& cxEachStop)
    {
        ATLASSERT(::IsWindow(m_hWnd));
        ATLASSERT((GetStyle() & LBS_USETABSTOPS) != 0);
        
        return (BOOL)::SendMessage(m_hWnd, LB_SETTABSTOPS, 1, (LPARAM)(LPINT)&cxEachStop);
    }

    int GetItemHeight(int nIndex) const
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (int)::SendMessage(m_hWnd, LB_GETITEMHEIGHT, nIndex, 0L);
    }

    int SetItemHeight(int nIndex, UINT cyItemHeight)
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (int)::SendMessage(m_hWnd, LB_SETITEMHEIGHT, nIndex, MAKELONG(cyItemHeight, 0));
    }

    DWORD_PTR GetItemData(int nIndex) const
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (DWORD_PTR)::SendMessage(m_hWnd, LB_GETITEMDATA, nIndex, 0L);
    }

    int SetItemData(int nIndex, DWORD_PTR dwItemData)
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (int)::SendMessage(m_hWnd, LB_SETITEMDATA, nIndex, (LPARAM)dwItemData);
    }

    void* GetItemDataPtr(int nIndex) const
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (void*)::SendMessage(m_hWnd, LB_GETITEMDATA, nIndex, 0L);
    }

    int SetItemDataPtr(int nIndex, void* pData)
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return SetItemData(nIndex, (DWORD_PTR)pData);
    }

    int GetItemRect(int nIndex, LPRECT lpRect) const
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (int)::SendMessage(m_hWnd, LB_GETITEMRECT, nIndex, (LPARAM)lpRect);
    }

    int GetTextLen(int nIndex) const
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (int)::SendMessage(m_hWnd, LB_GETTEXTLEN, nIndex, 0L);
    }

    int GetText(int nIndex, LPTSTR lpszBuffer) const
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (int)::SendMessage(m_hWnd, LB_GETTEXT, nIndex, (LPARAM)lpszBuffer);
    }

#ifdef __ATLSTR_H__
    int GetText(int nIndex, CString& strText) const
    {
        ATLASSERT(::IsWindow(m_hWnd));
        int nLength = GetTextLen(nIndex);
        if (nLength == LB_ERR)
            return LB_ERR;

        int nRet = LB_ERR;
        LPTSTR lpstr = strText.GetBufferSetLength(nLength);
        if (lpstr != NULL)
        {
            nRet = GetText(nIndex, lpstr);
            strText.ReleaseBuffer();
        }

        return nRet;
    }
#endif  // __ATLSTR_H__

#ifdef _OLEAUTO_H_
    BOOL GetText(int nIndex, BSTR& bstrText) const
    {
        ATLASSERT(::IsWindow(m_hWnd));
        USES_CONVERSION;
        ::SysFreeString(bstrText);

        int nLength = GetTextLen(nIndex);
        if (nLength == LB_ERR)
            return FALSE;

        LPTSTR lpszText = (LPTSTR)::_alloca((nLength + 1) * sizeof(TCHAR));
        if (GetText(nIndex, lpszText) == LB_ERR)
            return FALSE;

        bstrText = ::SysAllocString(T2OLE(lpszText));
        return (bstrText != NULL) ? TRUE : FALSE;
    }
#endif  // _OLEAUTO_H_

// 操作
public:
    int InitStorage(int nItems, UINT nBytes)
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (int)::SendMessage(m_hWnd, LB_INITSTORAGE, (WPARAM)nItems, nBytes);
    }

    int AddString(LPCTSTR lpszItem)
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (int)::SendMessage(m_hWnd, LB_ADDSTRING, 0, (LPARAM)lpszItem);
    }

    int DeleteString(UINT nIndex)
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (int)::SendMessage(m_hWnd, LB_DELETESTRING, nIndex, 0L);
    }

    int InsertString(int nIndex, LPCTSTR lpszItem)
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (int)::SendMessage(m_hWnd, LB_INSERTSTRING, nIndex, (LPARAM)lpszItem);
    }

    int SelectString(int nStartAfter, LPCTSTR lpszItem)
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (int)::SendMessage(m_hWnd, LB_SELECTSTRING, nStartAfter, (LPARAM)lpszItem);
    }

    int FindString(int nStartAfter, LPCTSTR lpszItem) const
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (int)::SendMessage(m_hWnd, LB_FINDSTRING, nStartAfter, (LPARAM)lpszItem);
    }

    int FindStringExact(int nIndexStart, LPCTSTR lpszFind) const
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (int)::SendMessage(m_hWnd, LB_FINDSTRINGEXACT, nIndexStart, (LPARAM)lpszFind);
    }

    int SelItemRange(BOOL bSelect, int nFirstItem, int nLastItem)
    {
        ATLASSERT(::IsWindow(m_hWnd));
        ATLASSERT((GetStyle() & (LBS_MULTIPLESEL | LBS_EXTENDEDSEL)) != 0);

        return bSelect ? (int)::SendMessage(m_hWnd, LB_SELITEMRANGEEX, nFirstItem, nLastItem)
                       : (int)::SendMessage(m_hWnd, LB_SELITEMRANGEEX, nLastItem, nFirstItem);
    }

    UINT ItemFromPoint(POINT pt, BOOL& bOutside) const
    {
        ATLASSERT(::IsWindow(m_hWnd));
        DWORD dw = (DWORD)::SendMessage(m_hWnd, LB_ITEMFROMPOINT, 0, MAKELPARAM(pt.x, pt.y));
        bOutside = (BOOL)HIWORD(dw);
        return (UINT)LOWORD(dw);
    }

    void ResetContent()
    {
        ATLASSERT(::IsWindow(m_hWnd));
        ::SendMessage(m_hWnd, LB_RESETCONTENT, 0, 0L);
    }

    int Dir(UINT attr, LPCTSTR lpszWildCard)
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (int)::SendMessage(m_hWnd, LB_DIR, attr, (LPARAM)lpszWildCard);
    }

    int AddFile(LPCTSTR lpszFileName)
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (int)::SendMessage(m_hWnd, LB_ADDFILE, 0, (LPARAM)lpszFileName);
    }
};

typedef CListBoxT<ATL::CWindow> CListBox;


///////////////////////////////////////////////////////////////////////////////
// CComboBox 类的定义
//

template <class TBase>
class CComboBoxT : public TBase
{
// 构造
public:
    CComboBoxT(HWND hWnd = NULL)
        : TBase(hWnd)
    {
    }

    CComboBoxT<TBase>& operator = (HWND hWnd)
    {
        m_hWnd = hWnd;
        return *this;
    }

    HWND Create(HWND hWndParent, ATL::_U_RECT rect = NULL, LPCTSTR lpszWindowName = NULL, DWORD dwStyle = 0,
                DWORD dwExStyle = 0, ATL::_U_MENUorID MenuOrID = 0U, LPVOID lpCreateParam = NULL)
    {
        return TBase::Create(GetWndClassName(), hWndParent, rect, lpszWindowName, dwStyle, dwExStyle, MenuOrID, lpCreateParam);
    }

// 属性
public:
    static LPCTSTR GetWndClassName()
    {
        return WC_COMBOBOX;
    }

    int GetCount() const
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (int)::SendMessage(m_hWnd, CB_GETCOUNT, 0, 0L);
    }

    int GetCurSel() const
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (int)::SendMessage(m_hWnd, CB_GETCURSEL, 0, 0L);
    }

    int SetCurSel(int nSelect)
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (int)::SendMessage(m_hWnd, CB_SETCURSEL, nSelect, 0L);
    }

    LCID GetLocale() const
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (LCID)::SendMessage(m_hWnd, CB_GETLOCALE, 0, 0L);
    }

    LCID SetLocale(LCID nNewLocale)
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (LCID)::SendMessage(m_hWnd, CB_SETLOCALE, (WPARAM)nNewLocale, 0L);
    }

    int GetTopIndex() const
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (int)::SendMessage(m_hWnd, CB_GETTOPINDEX, 0, 0L);
    }

    int SetTopIndex(int nIndex)
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (int)::SendMessage(m_hWnd, CB_SETTOPINDEX, nIndex, 0L);
    }

    UINT GetHorizontalExtent() const
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (UINT)::SendMessage(m_hWnd, CB_GETHORIZONTALEXTENT, 0, 0L);
    }

    void SetHorizontalExtent(UINT nExtent)
    {
        ATLASSERT(::IsWindow(m_hWnd));
        ::SendMessage(m_hWnd, CB_SETHORIZONTALEXTENT, nExtent, 0L);
    }

    int GetDroppedWidth() const
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (int)::SendMessage(m_hWnd, CB_GETDROPPEDWIDTH, 0, 0L);
    }

    int SetDroppedWidth(UINT nWidth)
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (int)::SendMessage(m_hWnd, CB_SETDROPPEDWIDTH, nWidth, 0L);
    }

#if (WINVER >= 0x0500)
    BOOL GetComboBoxInfo(PCOMBOBOXINFO pComboBoxInfo) const
    {
        ATLASSERT(::IsWindow(m_hWnd));
    #if (_WIN32_WINNT >= 0x0501)
        return (BOOL)::SendMessage(m_hWnd, CB_GETCOMBOBOXINFO, 0, (LPARAM)pComboBoxInfo);
    #else   // _WIN32_WINNT < 0x0501
        return ::GetComboBoxInfo(m_hWnd, pComboBoxInfo);
    #endif  // _WIN32_WINNT >= 0x0501
    }
#endif  // WINVER >= 0x0500

    DWORD GetEditSel() const
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (DWORD)::SendMessage(m_hWnd, CB_GETEDITSEL, 0, 0L);
    }

    BOOL SetEditSel(int nStartChar, int nEndChar)
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (BOOL)::SendMessage(m_hWnd, CB_SETEDITSEL, 0, MAKELONG(nStartChar, nEndChar));
    }

    int GetLBTextLen(int nIndex) const
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (int)::SendMessage(m_hWnd, CB_GETLBTEXTLEN, nIndex, 0L);
    }

    int GetLBText(int nIndex, LPTSTR lpszText) const
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (int)::SendMessage(m_hWnd, CB_GETLBTEXT, nIndex, (LPARAM)lpszText);
    }

#ifdef __ATLSTR_H__
    int GetLBText(int nIndex, CString& strText) const
    {
        ATLASSERT(::IsWindow(m_hWnd));
        int nLength = GetLBTextLen(nIndex);
        if (nLength == CB_ERR)
            return CB_ERR;

        int nRet = CB_ERR;
        LPTSTR lpstr = strText.GetBufferSetLength(nLength);
        if (lpstr != NULL)
        {
            nRet = GetLBText(nIndex, lpstr);
            strText.ReleaseBuffer();
        }
        
        return nRet;
    }
#endif  // __ATLSTR_H__

#ifdef _OLEAUTO_H_
    BOOL GetLBText(int nIndex, BSTR& bstrText) const
    {
        ATLASSERT(::IsWindow(m_hWnd));
        USES_CONVERSION;
        ::SysFreeString(bstrText);

        int nLength = GetLBTextLen(nIndex);
        if (nLength == CB_ERR)
            return FALSE;

        LPTSTR lpszText = (LPTSTR)_alloca((nLength + 1) * sizeof(TCHAR));
        if (GetLBText(nIndex, lpszText) == CB_ERR)
            return FALSE;

        bstrText = ::SysAllocString(T2OLE(lpszText));
        return (bstrText != NULL) ? TRUE : FALSE;
    }
#endif  // _OLEAUTO_H_

    int GetItemHeight(int nIndex) const
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (int)::SendMessage(m_hWnd, CB_GETITEMHEIGHT, nIndex, 0L);
    }

    int SetItemHeight(int nIndex, UINT cyItemHeight)
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (int)::SendMessage(m_hWnd, CB_SETITEMHEIGHT, nIndex, MAKELONG(cyItemHeight, 0));
    }

    DWORD_PTR GetItemData(int nIndex) const
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (DWORD_PTR)::SendMessage(m_hWnd, CB_GETITEMDATA, nIndex, 0L);
    }

    int SetItemData(int nIndex, DWORD_PTR dwItemData)
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (int)::SendMessage(m_hWnd, CB_SETITEMDATA, nIndex, (LPARAM)dwItemData);
    }

    void* GetItemDataPtr(int nIndex) const
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (void*)GetItemData(nIndex);
    }

    int SetItemDataPtr(int nIndex, void* pData)
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return SetItemData(nIndex, (DWORD_PTR)pData);
    }

    BOOL GetExtendedUI() const
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (BOOL)::SendMessage(m_hWnd, CB_GETEXTENDEDUI, 0, 0L);
    }

    int SetExtendedUI(BOOL bExtended = TRUE)
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (int)::SendMessage(m_hWnd, CB_SETEXTENDEDUI, bExtended, 0L);
    }

    void GetDroppedControlRect(LPRECT lprect) const
    {
        ATLASSERT(::IsWindow(m_hWnd));
        ::SendMessage(m_hWnd, CB_GETDROPPEDCONTROLRECT, 0, (LPARAM)lprect);
    }

    BOOL GetDroppedState() const
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (BOOL)::SendMessage(m_hWnd, CB_GETDROPPEDSTATE, 0, 0L);
    }

#if (_WIN32_WINNT >= 0x0501)
    int GetMinVisible() const
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (int)::SendMessage(m_hWnd, CB_GETMINVISIBLE, 0, 0L);
    }

    BOOL SetMinVisible(int nMinVisible)
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (BOOL)::SendMessage(m_hWnd, CB_SETMINVISIBLE, nMinVisible, 0L);
    }
#endif  // _WIN32_WINNT >= 0x0501

// 操作
public:
    int InitStorage(int nItems, UINT nBytes)
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (int)::SendMessage(m_hWnd, CB_INITSTORAGE, (WPARAM)nItems, nBytes);
    }

    BOOL LimitText(int nMaxChars)
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (BOOL)::SendMessage(m_hWnd, CB_LIMITTEXT, nMaxChars, 0L);
    }

    void ShowDropDown(BOOL bShowIt = TRUE)
    {
        ATLASSERT(::IsWindow(m_hWnd));
        ::SendMessage(m_hWnd, CB_SHOWDROPDOWN, bShowIt, 0L);
    }

    int AddString(LPCTSTR lpszString)
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (int)::SendMessage(m_hWnd, CB_ADDSTRING, 0, (LPARAM)lpszString);
    }

    int DeleteString(UINT nIndex)
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (int)::SendMessage(m_hWnd, CB_DELETESTRING, nIndex, 0L);
    }

    int InsertString(int nIndex, LPCTSTR lpszString)
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (int)::SendMessage(m_hWnd, CB_INSERTSTRING, nIndex, (LPARAM)lpszString);
    }

    int SelectString(int nStartAfter, LPCTSTR lpszString)
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (int)::SendMessage(m_hWnd, CB_SELECTSTRING, nStartAfter, (LPARAM)lpszString);
    }

    int FindString(int nStartAfter, LPCTSTR lpszString) const
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (int)::SendMessage(m_hWnd, CB_FINDSTRING, nStartAfter, (LPARAM)lpszString);
    }

    int FindStringExact(int nIndexStart, LPCTSTR lpszFind) const
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (int)::SendMessage(m_hWnd, CB_FINDSTRINGEXACT, nIndexStart, (LPARAM)lpszFind);
    }

    void Clear()
    {
        ATLASSERT(::IsWindow(m_hWnd));
        ::SendMessage(m_hWnd, WM_CLEAR, 0, 0L);
    }

    void Copy()
    {
        ATLASSERT(::IsWindow(m_hWnd));
        ::SendMessage(m_hWnd, WM_COPY, 0, 0L);
    }

    void Cut()
    {
        ATLASSERT(::IsWindow(m_hWnd));
        ::SendMessage(m_hWnd, WM_CUT, 0, 0L);
    }

    void Paste()
    {
        ATLASSERT(::IsWindow(m_hWnd));
        ::SendMessage(m_hWnd, WM_PASTE, 0, 0L);
    }

    void ResetContent()
    {
        ATLASSERT(::IsWindow(m_hWnd));
        ::SendMessage(m_hWnd, CB_RESETCONTENT, 0, 0L);
    }

    int Dir(UINT attr, LPCTSTR lpszWildCard)
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (int)::SendMessage(m_hWnd, CB_DIR, attr, (LPARAM)lpszWildCard);
    }
};

typedef CComboBoxT<ATL::CWindow> CComboBox;


///////////////////////////////////////////////////////////////////////////////
// CEdit 类的定义
//

template <class TBase>
class CEditT : public TBase
{
// 构造
public:
    CEditT(HWND hWnd = NULL)
        : TBase(hWnd)
    {
    }

    CEditT<TBase>& operator = (HWND hWnd)
    {
        m_hWnd = hWnd;
        return *this;
    }

    HWND Create(HWND hWndParent, ATL::_U_RECT rect = NULL, LPCTSTR lpszWindowName = NULL, DWORD dwStyle = 0,
                DWORD dwExStyle = 0, ATL::_U_MENUorID MenuOrID = 0U, LPVOID lpCreateParam = NULL)
    {
        return TBase::Create(GetWndClassName(), hWndParent, rect, lpszWindowName, dwStyle, dwExStyle, MenuOrID, lpCreateParam);
    }

// 属性
public:
    static LPCTSTR GetWndClassName()
    {
        return WC_EDIT;
    }

    int GetLineCount() const
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (int)::SendMessage(m_hWnd, EM_GETLINECOUNT, 0, 0L);
    }

    void GetRect(LPRECT lpRect) const
    {
        ATLASSERT(::IsWindow(m_hWnd));
        ::SendMessage(m_hWnd, EM_GETRECT, 0, (LPARAM)lpRect);
    }

    void SetRect(LPCRECT lpRect)
    {
        ATLASSERT(::IsWindow(m_hWnd));
        ::SendMessage(m_hWnd, EM_SETRECT, 0, (LPARAM)lpRect);
    }

    BOOL GetModify() const
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (BOOL)::SendMessage(m_hWnd, EM_GETMODIFY, 0, 0L);
    }

    void SetModify(BOOL bModified = TRUE)
    {
        ATLASSERT(::IsWindow(m_hWnd));
        ::SendMessage(m_hWnd, EM_SETMODIFY, bModified, 0L);
    }

    DWORD GetSel() const
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (DWORD)::SendMessage(m_hWnd, EM_GETSEL, 0, 0L);
    }

    void GetSel(int& nStartChar, int& nEndChar) const
    {
        ATLASSERT(::IsWindow(m_hWnd));
        ::SendMessage(m_hWnd, EM_GETSEL, (WPARAM)&nStartChar, (LPARAM)&nEndChar);
    }

    void SetSel(DWORD dwSelection, BOOL bNoScroll = FALSE)
    {
        ATLASSERT(::IsWindow(m_hWnd));
        ::SendMessage(m_hWnd, EM_SETSEL, LOWORD(dwSelection), HIWORD(dwSelection));
        if (!bNoScroll) ::SendMessage(m_hWnd, EM_SCROLLCARET, 0, 0L);
    }

    void SetSel(int nStartChar, int nEndChar, BOOL bNoScroll = FALSE)
    {
        ATLASSERT(::IsWindow(m_hWnd));
        ::SendMessage(m_hWnd, EM_SETSEL, nStartChar, nEndChar);
        if (!bNoScroll) ::SendMessage(m_hWnd, EM_SCROLLCARET, 0, 0L);
    }

    HLOCAL GetHandle() const
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (HLOCAL)::SendMessage(m_hWnd, EM_GETHANDLE, 0, 0L);
    }

    void SetHandle(HLOCAL hBuffer)
    {
        ATLASSERT(::IsWindow(m_hWnd));
        ::SendMessage(m_hWnd, EM_SETHANDLE, (WPARAM)hBuffer, 0L);
    }

    DWORD GetMargins() const
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (DWORD)::SendMessage(m_hWnd, EM_GETMARGINS, 0, 0L);
    }

    void SetMargins(UINT nLeft, UINT nRight)
    {
        ATLASSERT(::IsWindow(m_hWnd));
        ::SendMessage(m_hWnd, EM_SETMARGINS, EC_LEFTMARGIN|EC_RIGHTMARGIN, MAKELONG(nLeft, nRight));
    }

    UINT GetLimitText() const
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (UINT)::SendMessage(m_hWnd, EM_GETLIMITTEXT, 0, 0L);
    }

    void SetLimitText(UINT nMax)
    {
        ATLASSERT(::IsWindow(m_hWnd));
        ::SendMessage(m_hWnd, EM_SETLIMITTEXT, nMax, 0L);
    }

    POINT PosFromChar(UINT nChar) const
    {
        ATLASSERT(::IsWindow(m_hWnd));
        POINT point;
        ::SendMessage(m_hWnd, EM_POSFROMCHAR, (WPARAM)&point, (LPARAM)nChar);
        return point;
    }

    int CharFromPos(POINT point) const
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (int)::SendMessage(m_hWnd, EM_CHARFROMPOS, 0, MAKELPARAM(point.x, point.y));
    }

    int GetLine(int nIndex, LPTSTR lpszBuffer) const
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (int)::SendMessage(m_hWnd, EM_GETLINE, nIndex, (LPARAM)lpszBuffer);
    }

    int GetLine(int nIndex, LPTSTR lpszBuffer, int nMaxLength) const
    {
        ATLASSERT(::IsWindow(m_hWnd));
        *(LPWORD)lpszBuffer = (WORD)nMaxLength;
        return (int)::SendMessage(m_hWnd, EM_GETLINE, nIndex, (LPARAM)lpszBuffer);
    }

    TCHAR GetPasswordChar() const
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (TCHAR)::SendMessage(m_hWnd, EM_GETPASSWORDCHAR, 0, 0L);
    }

    void SetPasswordChar(TCHAR ch)
    {
        ATLASSERT(::IsWindow(m_hWnd));
        ::SendMessage(m_hWnd, EM_SETPASSWORDCHAR, ch, 0L);
    }

#if (_WIN32_WINNT >= 0x0501)
    BOOL GetCueBanner(LPCWSTR lpszText, int cchText) const
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (BOOL)::SendMessage(m_hWnd, EM_GETCUEBANNER, (WPARAM)lpszText, cchText);
    }

    BOOL SetCueBanner(LPCWSTR lpszText)
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (BOOL)::SendMessage(m_hWnd, EM_SETCUEBANNER, 0, (LPARAM)lpszText);
    }

    UINT GetImeStatus(UINT uStatus) const
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (UINT)::SendMessage(m_hWnd, EM_GETIMESTATUS, uStatus, 0L);
    }

    UINT SetImeStatus(UINT uStatus, UINT uData)
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (UINT)::SendMessage(m_hWnd, EM_SETIMESTATUS, uStatus, uData);
    }
#endif  // _WIN32_WINNT >= 0x0501

// 操作
public:
    BOOL SetTabStops()
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (BOOL)::SendMessage(m_hWnd, EM_SETTABSTOPS, 0, 0L);
    }

    BOOL SetTabStops(const int& cxEachStop)
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (BOOL)::SendMessage(m_hWnd, EM_SETTABSTOPS, 1, (LPARAM)(LPINT)&cxEachStop);
    }

    BOOL SetTabStops(int nTabStops, LPINT rgTabStops)
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (BOOL)::SendMessage(m_hWnd, EM_SETTABSTOPS, nTabStops, (LPARAM)rgTabStops);
    }

    void SetRectNP(LPCRECT lpRect)
    {
        ATLASSERT(::IsWindow(m_hWnd));
        ::SendMessage(m_hWnd, EM_SETRECTNP, 0, (LPARAM)lpRect);
    }

    BOOL CanUndo() const
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (BOOL)::SendMessage(m_hWnd, EM_CANUNDO, 0, 0L);
    }

    void EmptyUndoBuffer()
    {
        ATLASSERT(::IsWindow(m_hWnd));
        ::SendMessage(m_hWnd, EM_EMPTYUNDOBUFFER, 0, 0L);
    }

    BOOL FmtLines(BOOL bAddEOL)
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (BOOL)::SendMessage(m_hWnd, EM_FMTLINES, bAddEOL, 0L);
    }

    void LimitText(int nChars = 0)
    {
        ATLASSERT(::IsWindow(m_hWnd));
        ::SendMessage(m_hWnd, EM_LIMITTEXT, nChars, 0L);
    }

    int LineFromChar(int nIndex = -1) const
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (int)::SendMessage(m_hWnd, EM_LINEFROMCHAR, nIndex, 0L);
    }

    int LineIndex(int nLine = -1) const
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (int)::SendMessage(m_hWnd, EM_LINEINDEX, nLine, 0L);
    }

    int LineLength(int nLine = -1) const
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (int)::SendMessage(m_hWnd, EM_LINELENGTH, nLine, 0L);
    }

    void LineScroll(int nLines, int nChars = 0)
    {
        ATLASSERT(::IsWindow(m_hWnd));
        ::SendMessage(m_hWnd, EM_LINESCROLL, nChars, nLines);
    }

    void ReplaceSel(LPCTSTR lpszNewText, BOOL bCanUndo = FALSE)
    {
        ATLASSERT(::IsWindow(m_hWnd));
        ::SendMessage(m_hWnd, EM_REPLACESEL, (WPARAM) bCanUndo, (LPARAM)lpszNewText);
    }

    void ScrollCaret()
    {
        ATLASSERT(::IsWindow(m_hWnd));
        ::SendMessage(m_hWnd, EM_SCROLLCARET, 0, 0L);
    }

    BOOL Scroll(int nScrollAction)
    {
        ATLASSERT(::IsWindow(m_hWnd));
        ATLASSERT((GetStyle() & ES_MULTILINE) != 0);

        return (BOOL)HIWORD(::SendMessage(m_hWnd, EM_SCROLL, nScrollAction, 0L));
    }

#if (_WIN32_WINNT >= 0x0501)
    BOOL ShowBalloonTip(PEDITBALLOONTIP pEditBaloonTip)
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (BOOL)::SendMessage(m_hWnd, EM_SHOWBALLOONTIP, 0, (LPARAM)pEditBaloonTip);
    }

    BOOL HideBalloonTip()
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (BOOL)::SendMessage(m_hWnd, EM_HIDEBALLOONTIP, 0, 0L);
    }
#endif  // _WIN32_WINNT >= 0x0501

    BOOL Undo()
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (BOOL)::SendMessage(m_hWnd, EM_UNDO, 0, 0L);
    }

    void Clear()
    {
        ATLASSERT(::IsWindow(m_hWnd));
        ::SendMessage(m_hWnd, WM_CLEAR, 0, 0L);
    }

    void Copy()
    {
        ATLASSERT(::IsWindow(m_hWnd));
        ::SendMessage(m_hWnd, WM_COPY, 0, 0L);
    }

    void Cut()
    {
        ATLASSERT(::IsWindow(m_hWnd));
        ::SendMessage(m_hWnd, WM_CUT, 0, 0L);
    }

    void Paste()
    {
        ATLASSERT(::IsWindow(m_hWnd));
        ::SendMessage(m_hWnd, WM_PASTE, 0, 0L);
    }

    BOOL SetReadOnly(BOOL bReadOnly = TRUE)
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (BOOL)::SendMessage(m_hWnd, EM_SETREADONLY, bReadOnly, 0L);
    }

    int GetFirstVisibleLine() const
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (int)::SendMessage(m_hWnd, EM_GETFIRSTVISIBLELINE, 0, 0L);
    }
};

typedef CEditT<ATL::CWindow> CEdit;


///////////////////////////////////////////////////////////////////////////////
// CScrollBar 类的定义
//

template <class TBase>
class CScrollBarT : public TBase
{
// 构造
public:
    CScrollBarT(HWND hWnd = NULL)
        : TBase(hWnd)
    {
    }

    CScrollBarT<TBase>& operator = (HWND hWnd)
    {
        m_hWnd = hWnd;
        return *this;
    }

    HWND Create(HWND hWndParent, ATL::_U_RECT rect = NULL, LPCTSTR lpszWindowName = NULL, DWORD dwStyle = 0,
                DWORD dwExStyle = 0, ATL::_U_MENUorID MenuOrID = 0U, LPVOID lpCreateParam = NULL)
    {
        return TBase::Create(GetWndClassName(), hWndParent, rect, lpszWindowName, dwStyle, dwExStyle, MenuOrID, lpCreateParam);
    }

// 属性
public:
    static LPCTSTR GetWndClassName()
    {
        return WC_SCROLLBAR;
    }

    int GetScrollPos() const
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return ::GetScrollPos(m_hWnd, SB_CTL);
    }
    int SetScrollPos(int nPos, BOOL bRedraw = TRUE)
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return ::SetScrollPos(m_hWnd, SB_CTL, nPos, bRedraw);
    }

    BOOL GetScrollRange(LPINT lpMinPos, LPINT lpMaxPos) const
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return ::GetScrollRange(m_hWnd, SB_CTL, lpMinPos, lpMaxPos);
    }

    BOOL SetScrollRange(int nMinPos, int nMaxPos, BOOL bRedraw = TRUE)
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return ::SetScrollRange(m_hWnd, SB_CTL, nMinPos, nMaxPos, bRedraw);
    }

    BOOL GetScrollInfo(LPSCROLLINFO lpScrollInfo) const
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return ::GetScrollInfo(m_hWnd, SB_CTL, lpScrollInfo);
    }

    int SetScrollInfo(LPSCROLLINFO lpScrollInfo, BOOL bRedraw = TRUE)
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return ::SetScrollInfo(m_hWnd, SB_CTL, lpScrollInfo, bRedraw);
    }

    int GetScrollLimit() const
    {
        ATLASSERT(::IsWindow(m_hWnd));

        int nMin = 0, nMax = 0;
        ::GetScrollRange(m_hWnd, SB_CTL, &nMin, &nMax);
        
        SCROLLINFO info;
        info.cbSize = sizeof(SCROLLINFO);
        info.fMask = SIF_PAGE;
        
        if(::GetScrollInfo(m_hWnd, SB_CTL, &info))
            nMax -= ((info.nPage - 1) > 0) ? (info.nPage - 1) : 0;

        return nMax;
    }

#if (WINVER >= 0x0500)
    BOOL GetScrollBarInfo(PSCROLLBARINFO pScrollBarInfo) const
    {
        ATLASSERT(::IsWindow(m_hWnd));
    #if (_WIN32_WINNT >= 0x0501)
        return (BOOL)::SendMessage(m_hWnd, SBM_GETSCROLLBARINFO, 0, (LPARAM)pScrollBarInfo);
    #else   // _WIN32_WINNT < 0x0501
        return ::GetScrollBarInfo(m_hWnd, OBJID_CLIENT, pScrollBarInfo);
    #endif  // _WIN32_WINNT >= 0x0501
    }
#endif  // WINVER >= 0x0500

// 操作
public:
    BOOL ShowScrollBar(BOOL bShow = TRUE)
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return ::ShowScrollBar(m_hWnd, SB_CTL, bShow);
    }

    BOOL EnableScrollBar(UINT nArrowFlags = ESB_ENABLE_BOTH)
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return ::EnableScrollBar(m_hWnd, SB_CTL, nArrowFlags);
    }
};

typedef CScrollBarT<ATL::CWindow> CScrollBar;


///////////////////////////////////////////////////////////////////////////////
// CListCtrl 类的定义
//

template <class TBase>
class CListCtrlT : public TBase
{
// 构造
public:
    CListCtrlT(HWND hWnd = NULL)
        : TBase(hWnd)
    {
    }

    CListCtrlT<TBase>& operator = (HWND hWnd)
    {
        m_hWnd = hWnd;
        return *this;
    }

    HWND Create(HWND hWndParent, ATL::_U_RECT rect = NULL, LPCTSTR lpszWindowName = NULL, DWORD dwStyle = 0,
                DWORD dwExStyle = 0, ATL::_U_MENUorID MenuOrID = 0U, LPVOID lpCreateParam = NULL)
    {
        return TBase::Create(GetWndClassName(), hWndParent, rect, lpszWindowName, dwStyle, dwExStyle, MenuOrID, lpCreateParam);
    }

// 属性
public:
    static LPCTSTR GetWndClassName()
    {
        return WC_LISTVIEW;
    }

    COLORREF GetBkColor() const
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (COLORREF)::SendMessage(m_hWnd, LVM_GETBKCOLOR, 0, 0L);
    }

    BOOL SetBkColor(COLORREF color)
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (BOOL)::SendMessage(m_hWnd, LVM_SETBKCOLOR, 0, color);
    }

    HIMAGELIST GetImageList(int nImageList) const
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (HIMAGELIST)::SendMessage(m_hWnd, LVM_GETIMAGELIST, nImageList, 0L);
    }

    HIMAGELIST SetImageList(HIMAGELIST hImageList, int nImageList)
    {
        ATLASSERT(::IsWindow(m_hWnd)); 
        return (HIMAGELIST)::SendMessage(m_hWnd, LVM_SETIMAGELIST, nImageList, (LPARAM)hImageList);
    }

    int GetItemCount() const
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (int)::SendMessage(m_hWnd, LVM_GETITEMCOUNT, 0, 0L);
    }

    BOOL SetItemCount(int nItems)
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (BOOL)::SendMessage(m_hWnd, LVM_SETITEMCOUNT, nItems, 0L);
    }

    BOOL GetItem(LVITEM* pItem) const
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (BOOL)::SendMessage(m_hWnd, LVM_GETITEM, 0, (LPARAM)pItem);
    }

    BOOL SetItem(const LVITEM* pItem)
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (BOOL)::SendMessage(m_hWnd, LVM_SETITEM, 0, (LPARAM)pItem);
    }

    BOOL SetItem(int nItem, int nSubItem, UINT nMask, LPCTSTR lpszItem, int nImage, UINT nState,
                 UINT nStateMask, LPARAM lParam)
    {
        ATLASSERT(::IsWindow(m_hWnd));
        
        LVITEM lvi = { 0 };
        lvi.mask      = nMask;
        lvi.iItem     = nItem;
        lvi.iSubItem  = nSubItem;
        lvi.stateMask = nStateMask;
        lvi.state     = nState;
        lvi.pszText   = (LPTSTR)lpszItem;
        lvi.iImage    = nImage;
        lvi.lParam    = lParam;
        
        return (BOOL)::SendMessage(m_hWnd, LVM_SETITEM, 0, (LPARAM)&lvi);
    }

    UINT GetItemState(int nItem, UINT nMask) const
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (UINT)::SendMessage(m_hWnd, LVM_GETITEMSTATE, nItem, nMask);
    }

    BOOL SetItemState(int nItem, UINT nState, UINT nStateMask)
    {
        ATLASSERT(::IsWindow(m_hWnd));
        
        LVITEM lvi = { 0 };
        lvi.state     = nState;
        lvi.stateMask = nStateMask;
        
        return (BOOL)::SendMessage(m_hWnd, LVM_SETITEMSTATE, nItem, (LPARAM)&lvi);
    }

    BOOL SetItemState(int nItem, LVITEM* pItem)
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (BOOL)::SendMessage(m_hWnd, LVM_SETITEMSTATE, nItem, (LPARAM)pItem);
    }

    int GetItemText(int nItem, int nSubItem, LPTSTR lpszText, int nLength) const
    {
        ATLASSERT(::IsWindow(m_hWnd));

        LVITEM lvi = { 0 };
        lvi.iSubItem   = nSubItem;
        lvi.cchTextMax = nLength;
        lvi.pszText    = lpszText;

        return (int)::SendMessage(m_hWnd, LVM_GETITEMTEXT, (WPARAM)nItem, (LPARAM)&lvi);
    }

#ifdef __ATLSTR_H__
    CString GetItemText(int nItem, int nSubItem) const
    {
        ATLSSERT(::IsWindow(m_hWnd));

        CString str;
        LVITEM lvi = { 0 };
        lvi.iSubItem = nSubItem;
        int nResult = 0, nLength = 128;
        
        do
        {
            nLength *= 2;
            lvi.cchTextMax = nLength;
            lvi.pszText    = str.GetBufferSetLength(nLength);
            nResult  = (int)::SendMessage(m_hWnd, LVM_GETITEMTEXT, (WPARAM)nItem, (LPARAM)&lvi);
        }
        while (nResult >= nLength - 1);
        
        str.ReleaseBuffer();
        return str;
    }
#endif  // __ATLSTR_H__

#ifdef _OLEAUTO_H_
    BOOL GetItemText(int nItem, int nSubItem, BSTR& bstrText) const
    {
        ATLASSERT(::IsWindow(m_hWnd));
        USES_CONVERSION;
        ::SysFreeString(bstrText);

        LVITEM lvi = { 0 };
        lvi.iSubItem = nSubItem;
        LPTSTR lpszText = NULL;
        int nResult = 0, nLength = 128;

        do
        {
            nLength *= 2;
            if (lpszText != NULL)
            {
                delete [] lpszText;
                lpszText = NULL;
            }
            ATLTRY(lpszText = new TCHAR[nLength]);
            if (lpszText == NULL)
                break;

            lvi.pszText    = lpszText;
            lvi.cchTextMax = nLength;
            nResult = (int)::SendMessage(m_hWnd, LVM_GETITEMTEXT, (WPARAM)nItem, (LPARAM)&lvi);
        }
        while (nResult >= nLength - 1);

        bstrText = ::SysAllocString(T2OLE(lpszText));
        delete [] lpszText;

        return (bstrText != NULL) ? TRUE : FALSE;
    }
#endif  // _OLEAUTO_H_

    BOOL SetItemText(int nItem, int nSubItem, LPCTSTR lpszText)
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return SetItem(nItem, nSubItem, LVIF_TEXT, lpszText, 0, 0, 0, 0);
    }

    DWORD_PTR GetItemData(int nItem) const
    {
        ATLASSERT(::IsWindow(m_hWnd));

        LVITEM lvi = { 0 };
        lvi.iItem = nItem;
        lvi.mask  = LVIF_PARAM;
        BOOL nResult = (BOOL)::SendMessage(m_hWnd, LVM_GETITEM, 0, (LPARAM)&lvi);
        
        return (DWORD_PTR)(nResult ? lvi.lParam : NULL);
    }

    BOOL SetItemData(int nItem, DWORD_PTR dwData)
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return SetItem(nItem, 0, LVIF_PARAM, NULL, 0, 0, 0, (LPARAM)dwData);
    }

    UINT GetCallbackMask() const
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (UINT)::SendMessage(m_hWnd, LVM_GETCALLBACKMASK, 0, 0L);
    }

    BOOL SetCallbackMask(UINT nMask)
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (BOOL)::SendMessage(m_hWnd, LVM_SETCALLBACKMASK, nMask, 0L);
    }

    BOOL GetItemPosition(int nItem, LPPOINT lpPoint) const
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (BOOL)::SendMessage(m_hWnd, LVM_GETITEMPOSITION, nItem, (LPARAM)lpPoint);
    }

    BOOL SetItemPosition(int nItem, POINT pt)
    {
        ATLASSERT(::IsWindow(m_hWnd));
        ATLASSERT(((GetStyle() & LVS_TYPEMASK) == LVS_ICON) || ((GetStyle() & LVS_TYPEMASK) == LVS_SMALLICON));
        
        return (BOOL)::SendMessage(m_hWnd, LVM_SETITEMPOSITION32, nItem, (LPARAM)&pt);
    }

    BOOL SetItemPosition(int nItem, int x, int y)
    {
        ATLASSERT(::IsWindow(m_hWnd));
        ATLASSERT(((GetStyle() & LVS_TYPEMASK) == LVS_ICON) || ((GetStyle() & LVS_TYPEMASK) == LVS_SMALLICON));
        
        POINT pt = { x, y };
        return (BOOL)::SendMessage(m_hWnd, LVM_SETITEMPOSITION32, nItem, (LPARAM)&pt);
    }

    int GetStringWidth(LPCTSTR lpszString) const
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (int)::SendMessage(m_hWnd, LVM_GETSTRINGWIDTH, 0, (LPARAM)lpszString);
    }

    CEdit GetEditControl() const
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return CEdit((HWND)::SendMessage(m_hWnd, LVM_GETEDITCONTROL, 0, 0L));
    }

    BOOL GetColumn(int nCol, LVCOLUMN* pColumn) const
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (BOOL)::SendMessage(m_hWnd, LVM_GETCOLUMN, nCol, (LPARAM)pColumn);
    }

    BOOL SetColumn(int nCol, const LVCOLUMN* pColumn)
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (BOOL)::SendMessage(m_hWnd, LVM_SETCOLUMN, nCol, (LPARAM)pColumn);
    }

    int GetColumnWidth(int nCol) const
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (int)::SendMessage(m_hWnd, LVM_GETCOLUMNWIDTH, nCol, 0L);
    }

    BOOL SetColumnWidth(int nCol, int cx)
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (BOOL)::SendMessage(m_hWnd, LVM_SETCOLUMNWIDTH, nCol, MAKELPARAM(cx, 0));
    }

// 操作
public:
    int InsertColumn(int nCol, const LVCOLUMN* pColumn)
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (int)::SendMessage(m_hWnd, LVM_INSERTCOLUMN, nCol, (LPARAM)pColumn);
    }

    int InsertColumn(int nCol, LPCTSTR lpszColumnHeading, int nFormat = LVCFMT_LEFT, 
            int nWidth = -1, int nSubItem = -1, int iImage = -1, int iOrder = -1)
    {
        LVCOLUMN column = { 0 };
        column.mask = LVCF_TEXT|LVCF_FMT;
        column.pszText = (LPTSTR)lpszColumnHeading;
        column.fmt = nFormat;
        if (nWidth != -1)
        {
            column.mask |= LVCF_WIDTH;
            column.cx = nWidth;
        }
        if (nSubItem != -1)
        {
            column.mask |= LVCF_SUBITEM;
            column.iSubItem = nSubItem;
        }
        if (iImage != -1)
        {
            column.mask |= LVCF_IMAGE;
            column.iImage = iImage;
        }
        if (iOrder != -1)
        {
            column.mask |= LVCF_ORDER;
            column.iOrder = iOrder;
        }
        return InsertColumn(nCol, &column);
    }

    int InsertItem(UINT nMask, int nItem, LPCTSTR lpszItem, UINT nState, UINT nStateMask, int nImage, LPARAM lParam)
    {
        ATLASSERT(::IsWindow(m_hWnd));
        LVITEM item = { 0 };
        item.mask = nMask;
        item.iItem = nItem;
        item.iSubItem = 0;
        item.pszText = (LPTSTR)lpszItem;
        item.state = nState;
        item.stateMask = nStateMask;
        item.iImage = nImage;
        item.lParam = lParam;
        return InsertItem(&item);
    }

    int InsertItem(const LVITEM* pItem)
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (int)::SendMessage(m_hWnd, LVM_INSERTITEM, 0, (LPARAM)pItem);
    }

    int InsertItem(int nItem, LPCTSTR lpszItem)
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return InsertItem(LVIF_TEXT, nItem, lpszItem, 0, 0, 0, 0);
    }

    int InsertItem(int nItem, LPCTSTR lpszItem, int nImage)
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return InsertItem(LVIF_TEXT|LVIF_IMAGE, nItem, lpszItem, 0, 0, nImage, 0);
    }
};

typedef CListCtrlT<ATL::CWindow> CListCtrl;


///////////////////////////////////////////////////////////////////////////////
// CHeaderCtrl 类的定义
//

template <class TBase>
class CHeaderCtrlT : public TBase
{
// 构造
public:
    CHeaderCtrlT(HWND hWnd = NULL)
        : TBase(hWnd)
    {
    }

    CHeaderCtrlT<TBase>& operator = (HWND hWnd)
    {
        m_hWnd = hWnd;
        return *this;
    }

    HWND Create(HWND hWndParent, ATL::_U_RECT rect = NULL, LPCTSTR lpszWindowName = NULL, DWORD dwStyle = 0,
                DWORD dwExStyle = 0, ATL::_U_MENUorID MenuOrID = 0U, LPVOID lpCreateParam = NULL)
    {
        return TBase::Create(GetWndClassName(), hWndParent, rect, lpszWindowName, dwStyle, dwExStyle, MenuOrID, lpCreateParam);
    }

// 属性
public:
    static LPCTSTR GetWndClassName()
    {
        return WC_HEADER;
    }

    int GetItemCount() const
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (int)::SendMessage(m_hWnd, HDM_GETITEMCOUNT, 0, 0L);
    }

    BOOL GetItem(int nIndex, LPHDITEM pHeaderItem) const
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (BOOL)::SendMessage(m_hWnd, HDM_GETITEM, nIndex, (LPARAM)pHeaderItem);
    }

    BOOL SetItem(int nIndex, LPHDITEM pHeaderItem)
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (BOOL)::SendMessage(m_hWnd, HDM_SETITEM, nIndex, (LPARAM)pHeaderItem);
    }

    HIMAGELIST GetImageList() const
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (HIMAGELIST)::SendMessage(m_hWnd, HDM_GETIMAGELIST, 0, 0L);
    }

    HIMAGELIST SetImageList(HIMAGELIST hImageList)
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (HIMAGELIST)::SendMessage(m_hWnd, HDM_SETIMAGELIST, 0, (LPARAM)hImageList);
    }

    BOOL GetOrderArray(int nSize, int* lpnArray) const
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (BOOL)::SendMessage(m_hWnd, HDM_GETORDERARRAY, nSize, (LPARAM)lpnArray);
    }

    BOOL SetOrderArray(int nSize, int* lpnArray)
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (BOOL)::SendMessage(m_hWnd, HDM_SETORDERARRAY, nSize, (LPARAM)lpnArray);
    }

    BOOL GetItemRect(int nIndex, LPRECT lpRect) const
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return (BOOL)::SendMessage(m_hWnd, HDM_GETITEMRECT, nIndex, (LPARAM)lpRect);
    }

// 操作
public:

};

typedef CHeaderCtrlT<ATL::CWindow> CHeaderCtrl;


///////////////////////////////////////////////////////////////////////////////
// CToolTipCtrl 类的定义
//

template <class TBase>
class CToolTipCtrlT : public TBase
{
// 构造
public:
    CToolTipCtrlT(HWND hWnd = NULL)
        : TBase(hWnd)
    {
    }

    CToolTipCtrlT<TBase>& operator = (HWND hWnd)
    {
        m_hWnd = hWnd;
        return *this;
    }

    HWND Create(HWND hWndParent, ATL::_U_RECT rect = NULL, LPCTSTR lpszWindowName = NULL, DWORD dwStyle = 0,
                DWORD dwExStyle = 0, ATL::_U_MENUorID MenuOrID = 0U, LPVOID lpCreateParam = NULL)
    {
        return TBase::Create(GetWndClassName(), hWndParent, rect, lpszWindowName, dwStyle, dwExStyle, MenuOrID, lpCreateParam);
    }

// 属性
public:
    static LPCTSTR GetWndClassName()
    {
        return TOOLTIPS_CLASS;
    }

// 操作
public:
};

typedef CToolTipCtrlT<ATL::CWindow> CToolTipCtrl;

}  // namespace ATL_CTRLS

#endif  // __ATLCONTROLS_H__