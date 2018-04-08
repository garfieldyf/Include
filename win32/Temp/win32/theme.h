///////////////////////////////////////////////////////////////////////////////
// theme.h : 头文件
//
// Copyright(c) 2008, Garfield. All rights reserved.
// Author  : Garfield
// Version : 1.0
// Finish Time : 2007/10/10

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __THEME_H__
#define __THEME_H__

#ifndef _INC_CRTDBG
#include <crtdbg.h>
#endif

#ifndef _UXTHEME_H_
#include <uxtheme.h>
#endif

#pragma comment(lib, "uxtheme.lib")

///////////////////////////////////////////////////////////////////////////////
// Classes in this file:
//
// CTheme

namespace stdutil {

///////////////////////////////////////////////////////////////////////////////
// CTheme 类的定义
//

class CTheme
{
// 构造/销毁
public:
    CTheme();
    ~CTheme();

// 操作
public:
    operator HTHEME() const;
    static BOOL WINAPI IsThemeSupported();

    BOOL OpenThemeData(HWND hWnd, LPCWSTR pszClassList);
    HRESULT CloseThemeData();

    HRESULT DrawThemeBackground(HDC hDC, int nPartID, int nStateID, LPCRECT pRect, LPCRECT pClipRect = NULL) const;
    HRESULT DrawThemeIcon(HDC hDC, int nPartID, int nStateID, LPCRECT pRect, HIMAGELIST hImageList, int nImageIndex) const;
    HRESULT DrawThemeEdge(HDC hDC, int nPartID, int nStateID, LPCRECT pDestRect, UINT nEdge, UINT nFlags, LPRECT pContentRect = NULL) const;
    HRESULT DrawThemeText(HDC hDC, int nPartID, int nStateID, LPCRECT pRect, LPCWSTR pszText, int nCharCount = -1, DWORD dwTextFlags = DT_LEFT) const;

    BOOL IsThemePartDefined(int nPartID) const;
    BOOL IsThemeBackgroundPartiallyTransparent(int nPartID, int nStateID) const;
    HRESULT HitTestThemeBackground(HDC hDC, int nPartID, int nStateID, DWORD dwOptions, LPCRECT pRect, HRGN hRgn, POINT ptTest, WORD* pHitTestCode) const;

// 属性
public:
    int GetThemeSysSize(int nSizeID) const;
    BOOL GetThemeSysBool(int nBoolID) const;
    COLORREF GetThemeSysColor(int nColorID) const;
    HBRUSH  GetThemeSysColorBrush(int nColorID) const;
    HRESULT GetThemeSysInt(int nIntID, int* pValue) const;
    HRESULT GetThemeSysFont(int nFontID, LPLOGFONTW pLogFont) const;

    HRESULT GetThemeSysString(int nStringID, BSTR& bstrBuffer) const;
    HRESULT GetThemeSysString(int nStringID, LPWSTR pszBuffer, int nMaxCharCount) const;

    HRESULT GetThemeInt(int nPartID, int nStateID, int nPropID, int* pValue) const;
    HRESULT GetThemeBool(int nPartID, int nStateID, int nPropID, BOOL* pValue) const;
    HRESULT GetThemeRect(int nPartID, int nStateID, int nPropID, LPRECT pRect) const;
    HRESULT GetThemeEnumValue(int nPartID, int nStateID, int nPropID, int* pValue) const;
    HRESULT GetThemeColor(int nPartID, int nStateID, int nPropID, COLORREF* pColor) const;
    HRESULT GetThemePosition(int nPartID, int nStateID, int nPropID, LPPOINT pPoint) const;
    HRESULT GetThemeIntList(int nPartID, int nStateID, int nPropID, PINTLIST pIntList) const;
    HRESULT GetThemeMetric(HDC hDC, int nPartID, int nStateID, int nPropID, int* pValue) const;

    HRESULT GetThemeString(int nPartID, int nStateID, int nPropID, BSTR& bstrBuffer) const;
    HRESULT GetThemeString(int nPartID, int nStateID, int nPropID, LPWSTR pszBuffer, int nMaxCharCount) const;

    HRESULT GetThemeFilename(int nPartID, int nStateID, int nPropID, BSTR& bstrFileName) const;
    HRESULT GetThemeFilename(int nPartID, int nStateID, int nPropID, LPWSTR pszFileName, int nMaxCharCount) const;

    HRESULT GetThemeTextMetrics(HDC hDC, int nPartID, int nStateID, LPTEXTMETRIC pTextMetric) const;
    HRESULT GetThemeFont(HDC hDC, int nPartID, int nStateID, int nPropID, LPLOGFONTW pLogFont) const;
    HRESULT GetThemePropertyOrigin(int nPartID, int nStateID, int nPropID, PROPERTYORIGIN* pOrigin) const;
    HRESULT GetThemeBackgroundRegion(HDC hDC, int nPartID, int nStateID, LPCRECT pRect, HRGN* pRegion) const;
    HRESULT GetThemeMargins(HDC hDC, int nPartID, int nStateID, int nPropID, LPRECT pRect, PMARGINS pMargins) const;
    HRESULT GetThemePartSize(HDC hDC, int nPartID, int nStateID, LPRECT pRect, THEMESIZE ThemeSize, LPSIZE pSize) const;
    HRESULT GetThemeBackgroundExtent(HDC hDC, int nPartID, int nStateID, LPCRECT pContentRect, LPRECT pExtentRect) const;
    HRESULT GetThemeBackgroundContentRect(HDC hDC, int nPartID, int nStateID, LPCRECT pBoundingRect, LPRECT pContentRect) const;
    HRESULT GetThemeTextExtent(HDC hDC, int nPartID, int nStateID, LPRECT pExtentRect, LPCWSTR pszText, int nCharCount = -1, DWORD dwTextFlags = DT_LEFT, LPCRECT pBoundingRect = NULL) const;

#ifdef __ATLSTR_H__
    HRESULT GetThemeSysString(int nStringID, CString& strBuffer) const;
    HRESULT GetThemeString(int nPartID, int nStateID, int nPropID, CString& strBuffer) const;
    HRESULT GetThemeFilename(int nPartID, int nStateID, int nPropID, CString& strFileName) const;
#endif

// 实现
private:
    CTheme(const CTheme& that);
    CTheme& operator=(const CTheme& that);

// 成员
protected:
    HTHEME m_hTheme;
};

}  // namespace stdutil

#include "theme.inl"

#endif  // __THEME_H__