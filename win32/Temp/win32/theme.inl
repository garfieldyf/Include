///////////////////////////////////////////////////////////////////////////////
// theme.inl : 实现文件
//
// Copyright(c) 2008, Garfield. All rights reserved.
// Author  : Garfield
// Version : 1.0
// Finish Time : 2007/10/10

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __THEME_INL__
#define __THEME_INL__

namespace stdutil {

///////////////////////////////////////////////////////////////////////////////
// CTheme 类的实现
//

inline CTheme::CTheme()
    : m_hTheme(NULL)
{
}

inline CTheme::~CTheme()
{
    CloseThemeData();
}

inline CTheme::operator HTHEME() const
{
    return m_hTheme;
}

inline BOOL WINAPI CTheme::IsThemeSupported()
{
    static BOOL bSupported = -1;
    if (bSupported == -1)
    {
        HINSTANCE hInstance = ::LoadLibrary(_T("uxtheme.dll"));
        bSupported = (hInstance != NULL);
        ::FreeLibrary(hInstance);
    }

    return bSupported;
}

inline BOOL CTheme::OpenThemeData(HWND hWnd, LPCWSTR pszClassList)
{
    _ASSERTE(m_hTheme == NULL);
    m_hTheme = ::OpenThemeData(hWnd, pszClassList);

    return (m_hTheme != NULL);
}

inline HRESULT CTheme::CloseThemeData()
{
    HRESULT hr = S_FALSE;
    if (m_hTheme != NULL)
    {
        if (SUCCEEDED(hr = ::CloseThemeData(m_hTheme)))
            m_hTheme = NULL;
    }

    return hr;
}

inline HRESULT CTheme::DrawThemeBackground(HDC hDC, int nPartID, int nStateID, LPCRECT lpRect, LPCRECT lpClipRect/* = NULL*/) const
{
    _ASSERTE(m_hTheme != NULL);
    return ::DrawThemeBackground(m_hTheme, hDC, nPartID, nStateID, lpRect, lpClipRect);
}

inline HRESULT CTheme::DrawThemeIcon(HDC hDC, int nPartID, int nStateID, LPCRECT pRect, HIMAGELIST hImageList, int nImageIndex) const
{
    _ASSERTE(m_hTheme != NULL);
    return ::DrawThemeIcon(m_hTheme, hDC, nPartID, nStateID, pRect, hImageList, nImageIndex);
}

inline HRESULT CTheme::DrawThemeEdge(HDC hDC, int nPartID, int nStateID, LPCRECT pDestRect, UINT nEdge, UINT nFlags, LPRECT pContentRect/* = NULL*/) const
{
    _ASSERTE(m_hTheme != NULL);
    return ::DrawThemeEdge(m_hTheme, hDC, nPartID, nStateID, pDestRect, nEdge, nFlags, pContentRect);
}

inline HRESULT CTheme::DrawThemeText(HDC hDC, int nPartID, int nStateID, LPCRECT lpRect, LPCWSTR pszText, int nCharCount/* = -1*/, DWORD dwTextFlags/* = DT_LEFT*/) const
{
    _ASSERTE(m_hTheme != NULL);
    return ::DrawThemeText(m_hTheme, hDC, nPartID, nStateID, pszText, nCharCount, dwTextFlags, 0, lpRect);
}

inline BOOL CTheme::IsThemePartDefined(int nPartID) const
{
    _ASSERTE(m_hTheme != NULL);
    return ::IsThemePartDefined(m_hTheme, nPartID, 0);
}

inline BOOL CTheme::IsThemeBackgroundPartiallyTransparent(int nPartID, int nStateID) const
{
    _ASSERTE(m_hTheme != NULL);
    return ::IsThemeBackgroundPartiallyTransparent(m_hTheme, nPartID, nStateID);
}

inline HRESULT CTheme::HitTestThemeBackground(HDC hDC, int nPartID, int nStateID, DWORD dwOptions, LPCRECT pRect, HRGN hRgn, POINT ptTest, WORD *pHitTestCode) const
{
    _ASSERTE(m_hTheme != NULL);
    return ::HitTestThemeBackground(m_hTheme, hDC, nPartID, nStateID, dwOptions, pRect, hRgn, ptTest, pHitTestCode);
}

}  // namespace stdutil

#endif  // __THEME_INL__