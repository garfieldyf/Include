///////////////////////////////////////////////////////////////////////////////
// stdutil.h
//
// Copyright(c) 2018, Garfield. All rights reserved.
// Author  : Garfield
// Version : 4.0
// Creation Date : 2007/2/3

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __STDUTIL_H__
#define __STDUTIL_H__

#include <atlalloc.h>
#include "atlmiscutil.h"

///////////////////////////////////////////////////////////////////////////////
// Global functions in this file:
//
// StrToInt()
// StrToDouble()
// StringIsEmpty()
// AdjustRange()
// IsEqualBool<_Tx, _Ty>()
// IsEqualDate()
// IsEqualDouble()
// VariantTo<_Ty>()
// IsVariantMissing()
// FindItem<_Predicate>()
// SetRect()
// ExtSetRect()
// LoadString()
// PathFileExists()
// DeleteFiles()
// CreateDirectory()
// CreateDirectoryEx()
// DumpRect<_Pr>()
// GetTextCodePage()
// DrawLine()
// DrawBkgnd()
// DrawBitmap()
// AlphaBitmap()
// StretchBitmap()
// ComputeStride()
// CreateDIBitmap()
// CreateRegion()
// CreateAngleRectRgn()
// StreamSeekBegin()
// StreamSeekEnd()
// CreateSafeArray()
// ClipClientRect()
// CalcTextRect()
// CalcDrawRect()
// LoadButtons()
// LoadToolBar()
// PixelToHIMETRIC()
// HIMETRICToPixel()
// FormatPhysicalAddress()
// GradientFillRect()
// GradientFillRgn()
// CalcTrianglePoints()
// DrawTriangle()
// FrameTriangle()
// GradientFillTriangle()

namespace stdutil {

inline BOOL WINAPI StrToInt(LPCSTR pszString, long& lResult, int nRadix = 10)
{
    ATLASSERT(pszString);

    lResult = ::strtol(pszString, (LPSTR*)&pszString, nRadix);
    return (*pszString == '\0');
}

inline BOOL WINAPI StrToInt(LPCWSTR pszString, long& lResult, int nRadix = 10)
{
    ATLASSERT(pszString);

    lResult = ::wcstol(pszString, (LPWSTR*)&pszString, nRadix);
    return (*pszString == L'\0');
}

#ifndef _WIN32_WCE
inline BOOL WINAPI StrToInt(LPCSTR pszString, LONGLONG& nResult, int nRadix = 10)
{
    ATLASSERT(pszString);

    nResult = ::_strtoi64(pszString, (LPSTR*)&pszString, nRadix);
    return (*pszString == '\0');
}

inline BOOL WINAPI StrToInt(LPCWSTR pszString, LONGLONG& nResult, int nRadix = 10)
{
    ATLASSERT(pszString);

    nResult = ::_wcstoi64(pszString, (LPWSTR*)&pszString, nRadix);
    return (*pszString == L'\0');
}
#endif  // _WIN32_WCE

inline BOOL WINAPI StrToDouble(LPCSTR pszString, double& dblResult)
{
    ATLASSERT(pszString);

    dblResult = ::strtod(pszString, (LPSTR*)&pszString);
    return (*pszString == '\0');
}

inline BOOL WINAPI StrToDouble(LPCWSTR pszString, double& dblResult)
{
    ATLASSERT(pszString);

    dblResult = ::wcstod(pszString, (LPWSTR*)&pszString);
    return (*pszString == L'\0');
}

inline BOOL WINAPI StringIsEmpty(LPCSTR pszString)
{
    return (pszString == NULL || (*pszString == '\0'));
}

inline BOOL WINAPI StringIsEmpty(LPCWSTR pszString)
{
    return (pszString == NULL || (*pszString == L'\0'));
}

inline int WINAPI AdjustRange(int nValue, int nMin, int nMax)
{
    return (nValue < nMin ? nMin : (nValue > nMax ? nMax : nValue));
}

template <typename _Tx, typename _Ty>
inline BOOL IsEqualBool(_Tx _Source, _Ty _Dest)
{
    return ((bool)(!_Source) == (bool)(!_Dest));
}

#ifdef __ATLCOMTIME_H__
inline BOOL WINAPI IsEqualDate(const COleDateTime& dtSource, const COleDateTime& dtDest)
{
    SYSTEMTIME source = { 0 }, dest = { 0 };
    dtDest.GetAsSystemTime(dest);
    dtSource.GetAsSystemTime(source);

    return (source.wYear == dest.wYear && source.wMonth == dest.wMonth && source.wDay == dest.wDay);
}
#endif  // __ATLCOMTIME_H__

inline BOOL WINAPI IsEqualDouble(double dblSource, double dblDest)
{
    return (::memcmp(&dblSource, &dblDest, sizeof(double)) == 0);
}

#ifdef _INC_COMDEF
template <typename _Ty>
inline _Ty VariantTo(const _variant_t& _Source)
{
    if (_Source.vt == VT_NULL || _Source.vt == VT_EMPTY)
        return _Ty();

    return _Ty(_Source);
}

template <typename _Ty>
inline _Ty VariantTo(const _variant_t* _Source)
{
    if (_Source == NULL || _Source->vt == VT_NULL || _Source->vt == VT_EMPTY)
        return _Ty();

    return _Ty(*_Source);
}
#endif  // _INC_COMDEF

inline BOOL WINAPI IsVariantMissing(const VARIANT& Value)
{
    return (Value.vt == VT_ERROR && Value.scode == DISP_E_PARAMNOTFOUND);
}

inline BOOL WINAPI IsVariantMissing(const VARIANT* Value)
{
    ATLASSERT(Value);
    return (Value->vt == VT_ERROR && Value->scode == DISP_E_PARAMNOTFOUND);
}

// _CompareItem prototype : BOOL CompareItem(LPTVITEM pItem);
template <typename _Predicate>
inline HTREEITEM FindItem(HWND hWnd, HTREEITEM hStart, _Predicate _CompareItem, UINT nMask = TVIF_TEXT | TVIF_PARAM)
{
    ATLASSERT(::IsWindow(hWnd));

    if (hStart != NULL)
    {
    #ifndef _WIN32_WCE
        TVITEMEX item;
    #else
        TVITEM item;
    #endif

        TCHAR szText[MAX_PATH];
        item.mask  = nMask, item.hItem = hStart, item.pszText = szText, item.cchTextMax = _countof(szText);
        ATLVERIFY(TreeView_GetItem(hWnd, &item));

        if (!_CompareItem(&item))
        {
            HTREEITEM hFound = FindItem(hWnd, TreeView_GetChild(hWnd, hStart), _CompareItem, nMask);
            hStart = (hFound != NULL ? hFound : FindItem(hWnd, TreeView_GetNextSibling(hWnd, hStart), _CompareItem, nMask));
        }
    }

    return hStart;
}

inline void WINAPI SetRect(RECT& rect, int left, int top, int right, int bottom)
{
    rect.left   = left;
    rect.top    = top;
    rect.right  = right;
    rect.bottom = bottom;
}

inline void WINAPI ExtSetRect(RECT& rect, int x, int y, int nWidth, int nHeight)
{
    rect.left   = x;
    rect.top    = y;
    rect.right  = x + nWidth;
    rect.bottom = y + nHeight;
}

#ifdef _WIN32_WCE
#ifdef __ATLCORE_H__
// To use this function, the �Cn flag must be set with the resource compiler, RC
inline LPCTSTR WINAPI LoadString(UINT nID)
{
    return (LPCTSTR)::LoadString(ATL::_AtlBaseModule.GetResourceInstance(), nID, NULL, 0);
}
#endif  // __ATLCORE_H__

// To use this function, the �Cn flag must be set with the resource compiler, RC
inline LPCTSTR WINAPI LoadString(HINSTANCE hInstance, UINT nID)
{
    return (LPCTSTR)::LoadString(hInstance, nID, NULL, 0);
}

inline BOOL WINAPI PathFileExists(LPCTSTR pszPath)
{
    return (::GetFileAttributes(pszPath) != INVALID_FILE_ATTRIBUTES);
}
#else
// The pszPathName parameter is terminated with a double null character ("\0\0") to indicate the end.
inline BOOL WINAPI DeleteFiles(LPCTSTR pszPathName, FILEOP_FLAGS wFlags = 0)
{
#ifndef _WIN32_WCE
    SHFILEOPSTRUCT shfo = { NULL, FO_DELETE, pszPathName, NULL, FOF_SILENT | FOF_NOCONFIRMATION | FOF_NOERRORUI | wFlags };
#else
    SHFILEOPSTRUCT shfo = { NULL, FO_DELETE, pszPathName, NULL, FOF_SILENT | FOF_NOCONFIRMATION | wFlags };
#endif  // _WIN32_WCE

    return (::SHFileOperation(&shfo) == 0);
}

inline BOOL WINAPI CreateDirectory(LPCSTR pszPath, HWND hWnd = NULL, LPSECURITY_ATTRIBUTES psa = NULL)
{
    ATLASSERT(pszPath);

    int nErrorCode = ::SHCreateDirectoryExA(hWnd, pszPath, psa);
    return (nErrorCode == ERROR_SUCCESS || nErrorCode == ERROR_ALREADY_EXISTS || nErrorCode == ERROR_FILE_EXISTS);
}

inline BOOL WINAPI CreateDirectory(LPCWSTR pszPath, HWND hWnd = NULL, LPSECURITY_ATTRIBUTES psa = NULL)
{
    ATLASSERT(pszPath);

    int nErrorCode = ::SHCreateDirectoryExW(hWnd, pszPath, psa);
    return (nErrorCode == ERROR_SUCCESS || nErrorCode == ERROR_ALREADY_EXISTS || nErrorCode == ERROR_FILE_EXISTS);
}

inline HRESULT WINAPI CreateDirectoryEx(LPCSTR pszPath, DWORD dwFlags = SHPPFW_DIRCREATE | SHPPFW_IGNOREFILENAME, HWND hWnd = NULL, IUnknown* pUnkEnableModless = NULL)
{
    ATLASSERT(pszPath);
    return ::SHPathPrepareForWriteA(hWnd, pUnkEnableModless, pszPath, dwFlags);
}

inline HRESULT WINAPI CreateDirectoryEx(LPCWSTR pszPath, DWORD dwFlags = SHPPFW_DIRCREATE | SHPPFW_IGNOREFILENAME, HWND hWnd = NULL, IUnknown* pUnkEnableModless = NULL)
{
    ATLASSERT(pszPath);
    return ::SHPathPrepareForWriteW(hWnd, pUnkEnableModless, pszPath, dwFlags);
}
#endif  // _WIN32_WCE

#ifdef _CRT_DEBUG_DUMP
template <typename _Pr>
inline void DumpRect(const _Pr& _Printer, const RECT& rect)
{
    _Printer(_T("[ left = %d, right = %d, top = %d, bottom = %d, width = %d, height = %d ]\n"), rect.left, rect.right, rect.top, rect.bottom, rect.right - rect.left, rect.bottom - rect.top);
}
#endif  // _CRT_DEBUG_DUMP

inline int WINAPI GetTextCodePage(DWORD dwTextBytes)
{
    return (LOWORD(dwTextBytes) == 0xFEFF ? CP_UTF16 : (LOWORD(dwTextBytes) == 0xFFFE ? CP_UTF16BE : ((dwTextBytes & 0xFFFFFF) == 0xBFBBEF ? CP_UTF8 : CP_ACP)));
}

inline void WINAPI DrawLine(HDC hDC, int x1, int y1, int x2, int y2, COLORREF color)
{
    ATLASSERT(hDC);

#ifndef _WIN32_WCE
    HGDIOBJ hOldPen = ::SelectObject(hDC, ::GetStockObject(DC_PEN));
    ::SetDCPenColor(hDC, color);

    ATLVERIFY(::MoveToEx(hDC, x1, y1, NULL));
    ATLVERIFY(::LineTo(hDC, x2, y2));

    ::SelectObject(hDC, hOldPen);
#else
    HPEN hPen = ::CreatePen(PS_SOLID, 1, color);
    ATLASSERT(hPen);

    HGDIOBJ hOldPen = ::SelectObject(hDC, hPen);
    ATLVERIFY(::MoveToEx(hDC, x1, y1, NULL));
    ATLVERIFY(::LineTo(hDC, x2, y2));

    ::SelectObject(hDC, hOldPen);
    ATLVERIFY(::DeleteObject(hPen));
#endif  // _WIN32_WCE
}

inline BOOL WINAPI DrawBkgnd(HWND hWnd, HDC hDC, const RECT& rect, int srcX = 0, int srcY = 0)
{
    ATLASSERT(hDC);
    ATLASSERT(::IsWindow(hWnd));

    HWND hWndParent = ::GetParent(hWnd);
    ATLASSERT(::IsWindow(hWndParent));

    NMPAINTSTRUCT nmps = { { hWnd, 0, NM_PAINTBKGND }, hDC, rect, { srcX, srcY } };
    ::MapWindowPoints(hWnd, hWndParent, &nmps.point, 1);

    return (BOOL)::SendMessage(hWndParent, WM_NOTIFY, 0, (LPARAM)&nmps);
}

inline BOOL WINAPI DrawBitmap(HDC hDC, const RECT& rect, HBITMAP hBitmap, int srcX, int srcY, DWORD dwRop = SRCCOPY)
{
    ATLASSERT(hDC);
    ATLASSERT(hBitmap);

    atlutil::CBitmapDC dc(hBitmap);
    return ::BitBlt(hDC, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, dc, srcX, srcY, dwRop);
}

inline BOOL WINAPI DrawBitmap(HDC hDC, int x, int y, int cx, int cy, HBITMAP hBitmap, int srcX, int srcY, DWORD dwRop = SRCCOPY)
{
    ATLASSERT(hDC);
    ATLASSERT(hBitmap);

    atlutil::CBitmapDC dc(hBitmap);
    return ::BitBlt(hDC, x, y, cx, cy, dc, srcX, srcY, dwRop);
}

inline BOOL WINAPI AlphaBitmap(HDC hDC, const RECT& rect, HBITMAP hBitmap, int srcX, int srcY, int srcWidth, int srcHeight, BLENDFUNCTION blend)
{
    ATLASSERT(hDC);
    ATLASSERT(hBitmap);

    atlutil::CBitmapDC dc(hBitmap);
    return ::AlphaBlend(hDC, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, dc, srcX, srcY, srcWidth, srcHeight, blend);
}

inline BOOL WINAPI AlphaBitmap(HDC hDC, int x, int y, int nWidth, int nHeight, HBITMAP hBitmap, int srcX, int srcY, int srcWidth, int srcHeight, BLENDFUNCTION blend)
{
    ATLASSERT(hDC);
    ATLASSERT(hBitmap);

    atlutil::CBitmapDC dc(hBitmap);
    return ::AlphaBlend(hDC, x, y, nWidth, nHeight, dc, srcX, srcY, srcWidth, srcHeight, blend);
}

inline BOOL WINAPI StretchBitmap(HDC hDC, const RECT& rect, HBITMAP hBitmap, int srcX, int srcY, int srcWidth, int srcHeight, DWORD dwRop = SRCCOPY)
{
    atlutil::CBitmapDC dc(hBitmap);
    return ::StretchBlt(hDC, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, dc, srcX, srcY, srcWidth, srcHeight, dwRop);
}

inline BOOL WINAPI StretchBitmap(HDC hDC, int x, int y, int nWidth, int nHeight, HBITMAP hBitmap, int srcX, int srcY, int srcWidth, int srcHeight, DWORD dwRop = SRCCOPY)
{
    ATLASSERT(hDC);
    ATLASSERT(hBitmap);

    atlutil::CBitmapDC dc(hBitmap);
    return ::StretchBlt(hDC, x, y, nWidth, nHeight, dc, srcX, srcY, srcWidth, srcHeight, dwRop);
}

inline UINT WINAPI ComputeStride(UINT nWidth, UINT nBitCount)
{
    return (((nWidth * nBitCount + 31) >> 5) << 2);
}

inline HBITMAP WINAPI CreateDIBitmap(HBITMAP hBitmap, int x, int y, int nWidth, int nHeight, LPVOID* ppBits = NULL)
{
    ATLASSERT(hBitmap);

    // Create 32 bpp DIB section.
    HBITMAP hbmDIB = NULL;
    BITMAPINFO bitmapInfo = { { sizeof(BITMAPINFOHEADER), nWidth, nHeight, 1, 32, BI_RGB } };

    if ((hbmDIB = ::CreateDIBSection(NULL, &bitmapInfo, DIB_RGB_COLORS, ppBits, NULL, 0)) != NULL)
    {
        // Draw hBitmap. When the function returns, the ppBits parameter contains DIB bit values.
        atlutil::CBitmapDC dcDest(hbmDIB), dcSrc(hBitmap);
        ATLVERIFY(::BitBlt(dcDest, 0, 0, nWidth, ::abs(nHeight), dcSrc, x, y, SRCCOPY));
    }

    return hbmDIB;
}

inline HRGN WINAPI CreateRegion(const COLORREF* pBits, int x, int y, int nWidth, int nHeight, COLORREF clrMask)
{
    ATLASSERT(pBits);
    ATLASSERT(nWidth > 0 && nHeight > 0);

    HRGN hRgn = ::CreateRectRgn(x, y, x + nWidth, y + nHeight), hRgnMask = ::CreateRectRgn(0, 0, 0, 0);
    ATLASSERT(hRgn);
    ATLASSERT(hRgnMask);

    // Convert COLORREF to RGBQUAD
    clrMask = RGB(GetBValue(clrMask), GetGValue(clrMask), GetRValue(clrMask));
    for (int i = 0, nStart = 0; i < nHeight; ++i, pBits += nWidth)
    {
        for (int j = 0; j < nWidth; ++j)
        {
            for (nStart = j; j < nWidth; ++j)
            {
                if (pBits[j] != clrMask)
                    break;
            }

            if (j > nStart)
            {
                ATLVERIFY(::SetRectRgn(hRgnMask, nStart + x, i + y, j + x, i + y + 1));
                ATLVERIFY(::CombineRgn(hRgn, hRgn, hRgnMask, RGN_DIFF));
            }
        }
    }

    ATLVERIFY(::DeleteObject(hRgnMask));
    return hRgn;
}

inline HRGN WINAPI CreateRegion(HBITMAP hBitmap, COLORREF clrMask, int x = 0, int y = 0)
{
    ATLASSERT(hBitmap);

    HRGN hRgn    = NULL;
    LPVOID pBits = NULL;

    BITMAP bmp;
    ATLVERIFY(::GetObject(hBitmap, sizeof(BITMAP), &bmp));

    if (HBITMAP hbmDIB = CreateDIBitmap(hBitmap, 0, 0, bmp.bmWidth, -bmp.bmHeight, &pBits))
    {
        hRgn = CreateRegion((LPCOLORREF)pBits, x, y, bmp.bmWidth, bmp.bmHeight, clrMask);
        ATLVERIFY(::DeleteObject(hbmDIB));
    }

    return hRgn;
}

#ifndef _WIN32_WCE
inline HRGN WINAPI CreateAngleRectRgn(int left, int top, int right, int bottom, int nWidth, int nHeight)
{
    POINT points[] =
    {
        { left, top + nHeight }, { left + nWidth, top }, { right - nWidth, top }, { right, top + nHeight },
        { right, bottom - nHeight }, { right - nWidth, bottom }, { left + nWidth, bottom }, { left, bottom - nHeight }
    };

    return ::CreatePolygonRgn(points, _countof(points), WINDING);
}

inline HRGN WINAPI CreateAngleRectRgn(const RECT& rect, int nWidth, int nHeight)
{
    return CreateAngleRectRgn(rect.left, rect.top, rect.right, rect.bottom, nWidth, nHeight);
}
#endif  // _WIN32_WCE

inline HRESULT WINAPI StreamSeekBegin(IStream* pStream)
{
    ATLASSERT(pStream);

    LARGE_INTEGER liOffset = { 0 };
    return pStream->Seek(liOffset, STREAM_SEEK_SET, NULL);
}

inline HRESULT WINAPI StreamSeekEnd(IStream* pStream, PULARGE_INTEGER pNewPosition = NULL)
{
    ATLASSERT(pStream);

    LARGE_INTEGER liOffset = { 0 };
    return pStream->Seek(liOffset, STREAM_SEEK_END, pNewPosition);
}

#ifdef __ATLSAFE_H__
inline HRESULT WINAPI CreateSafeArray(IStream* pStream, ULONG cbSize, ATL::CComSafeArray<BYTE>& saArray)
{
    ATLASSERT(pStream);
    ATLASSERT(cbSize != 0);
    ATLASSERT(saArray.m_psa == NULL);

    HRESULT hr = saArray.Create(cbSize);
    if (SUCCEEDED(hr))
    {
        LPVOID pData = NULL;
        hr = ::SafeArrayAccessData(saArray.m_psa, &pData);
        if (SUCCEEDED(hr))
        {
            _COM_VERIFY(StreamSeekBegin(pStream));
            hr = pStream->Read(pData, cbSize, NULL);
            _COM_VERIFY(::SafeArrayUnaccessData(saArray.m_psa));
        }
    }

    return hr;
}
#endif  // __ATLSAFE_H__

inline BOOL WINAPI ClipClientRect(HWND hWnd, HDC hDC, RECT& rcWindow)
{
    ATLASSERT(hDC);
    ATLASSERT(::IsWindow(hWnd));

    RECT rcClient;
    ATLVERIFY(::GetClientRect(hWnd, &rcClient));
    ATLVERIFY(::GetWindowRect(hWnd, &rcWindow));

    ::MapWindowPoints(hWnd, HWND_DESKTOP, (LPPOINT)&rcClient, 2);
    ::ExcludeClipRect(hDC, rcClient.left - rcWindow.left, rcClient.top - rcWindow.top, rcClient.right - rcWindow.left, rcClient.bottom - rcWindow.top);

    return ::OffsetRect(&rcWindow, -rcWindow.left, -rcWindow.top);
}

inline int WINAPI CalcTextRect(LPCTSTR pszText, HFONT hFont, LPRECT pRect = NULL, UINT nFormat = DT_LEFT | DT_VCENTER | DT_SINGLELINE)
{
    ATLASSERT(hFont);
    ATLASSERT(pszText);

    HDC hDC = ::GetDC(NULL);
    ATLASSERT(hDC);

    RECT rcText = { 0 };
    if (pRect == NULL)
        pRect = &rcText;

    HGDIOBJ hOldFont = ::SelectObject(hDC, hFont);
    ::DrawText(hDC, pszText, -1, pRect, nFormat | DT_CALCRECT);

    ::SelectObject(hDC, hOldFont);
    ATLVERIFY(::ReleaseDC(NULL, hDC));

    return (pRect->right - pRect->left);
}

inline void WINAPI CalcDrawRect(int nSizeMode, RECT& srcRect, RECT& destRect)
{
    ATLASSERT(nSizeMode >= SIZE_MODE_NORMAL && nSizeMode <= SIZE_MODE_STRETCH_CENTER);

    if (nSizeMode == SIZE_MODE_STRETCH)
        return;

    int srcWidth   = srcRect.right   - srcRect.left;
    int srcHeight  = srcRect.bottom  - srcRect.top;
    int destWidth  = destRect.right  - destRect.left;
    int destHeight = destRect.bottom - destRect.top;

    switch (nSizeMode)
    {
    case SIZE_MODE_NORMAL:
        {
            int nNewWidth   = min(srcWidth, destWidth);
            int nNewHeight  = min(srcHeight, destHeight);

            srcRect.right   = srcRect.left  + nNewWidth;
            srcRect.bottom  = srcRect.top   + nNewHeight;
            destRect.right  = destRect.left + nNewWidth;
            destRect.bottom = destRect.top  + nNewHeight;
        }
        break;

    case SIZE_MODE_CENTER:
        {
            int dx = ::abs((srcWidth - destWidth) / 2);
            int dy = ::abs((srcHeight - destHeight) / 2);

            if (srcWidth > destWidth)
                srcRect.left += dx, srcRect.right -= dx;
            else
                destRect.left += dx, destRect.right -= dx;

            if (srcHeight > destHeight)
                srcRect.top += dy, srcRect.bottom -= dy;
            else
                destRect.top += dy, destRect.bottom -= dy;
        }
        break;

    case SIZE_MODE_CONTENT:
        {
            destRect.right  = destRect.left + srcWidth;
            destRect.bottom = destRect.top + srcHeight;
        }
        break;

    case SIZE_MODE_STRETCH_LEFT:
        {
            int nOffset = destHeight - ::MulDiv(srcHeight, destWidth, srcWidth);
            if (nOffset > 0)
                destRect.bottom -= nOffset;
            else
                destRect.right  -= (destWidth  - ::MulDiv(srcWidth, destHeight, srcHeight));
        }
        break;

    case SIZE_MODE_STRETCH_RIGHT:
        {
            int nOffset = destHeight - ::MulDiv(srcHeight, destWidth, srcWidth);
            if (nOffset > 0)
                destRect.top  += nOffset;
            else
                destRect.left += (destWidth - ::MulDiv(srcWidth, destHeight, srcHeight));
        }
        break;

    case SIZE_MODE_STRETCH_CENTER:
        {
            int nOffset = (destHeight - ::MulDiv(srcHeight, destWidth, srcWidth)) / 2;
            if (nOffset > 0)
            {
                destRect.top    += nOffset;
                destRect.bottom -= nOffset;
            }
            else
            {
                nOffset = (destWidth - ::MulDiv(srcWidth, destHeight, srcHeight)) / 2;
                destRect.left += nOffset, destRect.right -= nOffset;
            }
        }
        break;
    }
}

inline BOOL WINAPI LoadButtons(HWND hWnd, const WORD* pIDArray, int nIDCount, int cxSeparator = 8)
{
    ATLASSERT(pIDArray);
    ATLASSERT(::IsWindow(hWnd));

    BOOL bSuccessful = FALSE;
    ATL::CTempBuffer<TBBUTTON, 512> buttons;

    if (LPTBBUTTON pButtons = buttons.Allocate(nIDCount))
    {
        for (int i = 0, nImage = 0; i < nIDCount; ++i)
        {
            pButtons[i].fsState = TBSTATE_ENABLED;
            pButtons[i].dwData  = 0;
            pButtons[i].iString = 0;

            if ((pButtons[i].idCommand = pIDArray[i]) != 0)
            {
                pButtons[i].iBitmap = nImage++;
                pButtons[i].fsStyle = TBSTYLE_BUTTON;
            }
            else
            {
                pButtons[i].iBitmap = cxSeparator;
                pButtons[i].fsStyle = TBSTYLE_SEP;
            }
        }

        ::SendMessage(hWnd, TB_BUTTONSTRUCTSIZE, sizeof(TBBUTTON), 0L);
        bSuccessful = ::SendMessage(hWnd, TB_ADDBUTTONS, nIDCount, (LPARAM)pButtons);
    }

    return bSuccessful;
}

inline HIMAGELIST WINAPI LoadToolBar(HWND hWnd, HINSTANCE hInstance, UINT nIDResource, COLORREF clrMask = CLR_DEFAULT, int cxButtonMargin = 7, int cyButtonMargin = 7, int cxSeparator = 8)
{
    ATLASSERT(hInstance);
    ATLASSERT(::IsWindow(hWnd));

    HIMAGELIST hImageList = NULL;
    if (LPWORD pIDArray = (LPWORD)::LockResource(::LoadResource(hInstance, ::FindResource(hInstance, MAKEINTRESOURCE(nIDResource), RT_TOOLBAR))))
    {
        ATLASSERT(pIDArray[0] == 1);
        if (LoadButtons(hWnd, pIDArray + 4, pIDArray[3], cxSeparator))
        {
        #ifndef _WIN32_WCE
            hImageList = ::ImageList_LoadImage(hInstance, MAKEINTRESOURCE(nIDResource), pIDArray[1], 1, clrMask, IMAGE_BITMAP, LR_CREATEDIBSECTION | LR_DEFAULTSIZE);
            ATLASSERT(hImageList);
        #else
            hImageList = ::ImageList_LoadImage(hInstance, MAKEINTRESOURCE(nIDResource), pIDArray[1], 1, clrMask, IMAGE_BITMAP, 0);
            ATLASSERT(hImageList);
        #endif  // _WIN32_WCE

            ::SendMessage(hWnd, TB_SETIMAGELIST, 0, (LPARAM)hImageList);
            ::SendMessage(hWnd, TB_SETBITMAPSIZE, 0, MAKELPARAM(pIDArray[1], pIDArray[2]));
            ::SendMessage(hWnd, TB_SETBUTTONSIZE, 0, MAKELPARAM(pIDArray[1] + cxButtonMargin, pIDArray[2] + cyButtonMargin));
        }
    }

    return hImageList;
}

inline void WINAPI PixelToHIMETRIC(HDC hDC, LPSIZE pSize)
{
    ATLASSERT(hDC);
    ATLASSERT(pSize);

    pSize->cx = ::MulDiv(pSize->cx, HIMETRIC_PER_INCH, ::GetDeviceCaps(hDC, LOGPIXELSX));
    pSize->cy = ::MulDiv(pSize->cy, HIMETRIC_PER_INCH, ::GetDeviceCaps(hDC, LOGPIXELSY));
}

inline void WINAPI PixelToHIMETRIC(HDC hDC, LPRECT pRect)
{
    ATLASSERT(hDC);
    ATLASSERT(pRect);

    const int xPixels = ::GetDeviceCaps(hDC, LOGPIXELSX);
    const int yPixels = ::GetDeviceCaps(hDC, LOGPIXELSY);

    pRect->left   = ::MulDiv(pRect->left,   HIMETRIC_PER_INCH, xPixels);
    pRect->top    = ::MulDiv(pRect->top,    HIMETRIC_PER_INCH, yPixels);
    pRect->right  = ::MulDiv(pRect->right,  HIMETRIC_PER_INCH, xPixels);
    pRect->bottom = ::MulDiv(pRect->bottom, HIMETRIC_PER_INCH, yPixels);
}

inline void WINAPI PixelToHIMETRIC(LPSIZE pSize, double Xdpi, double Ydpi)
{
    ATLASSERT(pSize);
    ATLASSERT(Xdpi != 0.0 && Ydpi != 0.0);

    pSize->cx = (LONG)(pSize->cx * HIMETRIC_PER_INCH / Xdpi);
    pSize->cy = (LONG)(pSize->cy * HIMETRIC_PER_INCH / Ydpi);
}

inline void WINAPI PixelToHIMETRIC(LPRECT pRect, double Xdpi, double Ydpi)
{
    ATLASSERT(pRect);
    ATLASSERT(Xdpi != 0.0 && Ydpi != 0.0);

    PixelToHIMETRIC((LPSIZE)pRect, Xdpi, Ydpi);
    PixelToHIMETRIC((LPSIZE)pRect + 1, Xdpi, Ydpi);
}

inline void WINAPI HIMETRICToPixel(HDC hDC, LPSIZE pSize)
{
    ATLASSERT(hDC);
    ATLASSERT(pSize);

    pSize->cx = ::MulDiv(pSize->cx, ::GetDeviceCaps(hDC, LOGPIXELSX), HIMETRIC_PER_INCH);
    pSize->cy = ::MulDiv(pSize->cy, ::GetDeviceCaps(hDC, LOGPIXELSY), HIMETRIC_PER_INCH);
}

inline void WINAPI HIMETRICToPixel(HDC hDC, LPRECT pRect)
{
    ATLASSERT(hDC);
    ATLASSERT(pRect);

    const int xPixels = ::GetDeviceCaps(hDC, LOGPIXELSX);
    const int yPixels = ::GetDeviceCaps(hDC, LOGPIXELSY);

    pRect->left   = ::MulDiv(pRect->left,   xPixels, HIMETRIC_PER_INCH);
    pRect->top    = ::MulDiv(pRect->top,    yPixels, HIMETRIC_PER_INCH);
    pRect->right  = ::MulDiv(pRect->right,  xPixels, HIMETRIC_PER_INCH);
    pRect->bottom = ::MulDiv(pRect->bottom, yPixels, HIMETRIC_PER_INCH);
}

inline void WINAPI HIMETRICToPixel(LPSIZE pSize, double Xdpi, double Ydpi)
{
    ATLASSERT(pSize);

    pSize->cx = (LONG)(pSize->cx * Xdpi / HIMETRIC_PER_INCH);
    pSize->cy = (LONG)(pSize->cy * Ydpi / HIMETRIC_PER_INCH);
}

inline void WINAPI HIMETRICToPixel(LPRECT pRect, double Xdpi, double Ydpi)
{
    ATLASSERT(pRect);

    HIMETRICToPixel((LPSIZE)pRect, Xdpi, Ydpi);
    HIMETRICToPixel((LPSIZE)pRect + 1, Xdpi, Ydpi);
}

inline int WINAPI FormatPhysicalAddress(const BYTE* pAddress, LPTSTR pszBuffer, TCHAR chSeparator = _T('-'))
{
    ATLASSERT(pAddress);
    ATLASSERT(pszBuffer);

    return _tssprintf(pszBuffer, PHYSICAL_ADDRESS_LENGTH, _T("%02X%c%02X%c%02X%c%02X%c%02X%c%02X"), pAddress[0], chSeparator, pAddress[1], chSeparator, pAddress[2], chSeparator, pAddress[3], chSeparator, pAddress[4], chSeparator, pAddress[5]);
}

inline BOOL WINAPI GradientFillRect(HDC hDC, int left, int top, int right, int bottom, COLORREF clrFrom, COLORREF clrTo, ULONG ulMode = GRADIENT_FILL_RECT_V)
{
    ATLASSERT(hDC);

    GRADIENT_RECT grc  = { 0, 1 };
    TRIVERTEX vertex[] = { { left, top, _R16(clrFrom), _G16(clrFrom), _B16(clrFrom) }, { right, bottom, _R16(clrTo), _G16(clrTo), _B16(clrTo) } };

    return ::GradientFill(hDC, vertex, _countof(vertex), &grc, 1, ulMode);
}

inline BOOL WINAPI GradientFillRect(HDC hDC, const RECT& rect, COLORREF clrFrom, COLORREF clrTo, ULONG ulMode = GRADIENT_FILL_RECT_V)
{
    return GradientFillRect(hDC, rect.left, rect.top, rect.right, rect.bottom, clrFrom, clrTo, ulMode);
}

inline BOOL WINAPI GradientFillRect(HDC hDC, int left, int top, int right, int bottom, COLORREF clrFrom1, COLORREF clrTo1, COLORREF clrFrom2, COLORREF clrTo2, ULONG ulMode = GRADIENT_FILL_RECT_V, BYTE nPercent = 50)
{
    ATLASSERT(hDC);
    ATLASSERT(nPercent > 0 && nPercent <= 100);
    ATLASSERT(ulMode == GRADIENT_FILL_RECT_H || ulMode == GRADIENT_FILL_RECT_V);

    GRADIENT_RECT grcs[] = { 0, 1, 2, 3 };
    TRIVERTEX vertex[] =
    {
        { left,  top,    _R16(clrFrom1), _G16(clrFrom1), _B16(clrFrom1) },
        { right, bottom, _R16(clrTo1),   _G16(clrTo1),   _B16(clrTo1)   },
        { left,  top,    _R16(clrFrom2), _G16(clrFrom2), _B16(clrFrom2) },
        { right, bottom, _R16(clrTo2),   _G16(clrTo2),   _B16(clrTo2)   }
    };

    if (ulMode == GRADIENT_FILL_RECT_H)
        vertex[1].x = vertex[2].x = left + ::MulDiv(right - left, nPercent, 100);
    else
        vertex[1].y = vertex[2].y = top  + ::MulDiv(bottom - top, nPercent, 100);

    return ::GradientFill(hDC, vertex, _countof(vertex), grcs, _countof(grcs), ulMode);
}

inline BOOL WINAPI GradientFillRect(HDC hDC, const RECT& rect, COLORREF clrFrom1, COLORREF clrTo1, COLORREF clrFrom2, COLORREF clrTo2, ULONG ulMode = GRADIENT_FILL_RECT_V, BYTE nPercent = 50)
{
    return GradientFillRect(hDC, rect.left, rect.top, rect.right, rect.bottom, clrFrom1, clrTo1, clrFrom2, clrTo2, ulMode, nPercent);
}

inline BOOL WINAPI GradientFillRgn(HDC hDC, HRGN hRgn, COLORREF clrFrom, COLORREF clrTo, ULONG ulMode = GRADIENT_FILL_REGION_V)
{
    ATLASSERT(hDC);
    ATLASSERT(hRgn);
    ATLASSERT(ulMode == GRADIENT_FILL_REGION_H || ulMode == GRADIENT_FILL_REGION_V);

    RECT rcBound;
    BOOL bSuccessful = FALSE;

    if (::GetRgnBox(hRgn, &rcBound) > NULLREGION)
    {
        GRADIENT_RECT grc  = { 0, 1 };
        TRIVERTEX vertex[] = { { rcBound.left, rcBound.top, _R16(clrFrom), _G16(clrFrom), _B16(clrFrom) }, { rcBound.right, rcBound.bottom, _R16(clrTo), _G16(clrTo), _B16(clrTo) } };

        ATLVERIFY(::ExtSelectClipRgn(hDC, hRgn, RGN_AND));
        bSuccessful = ::GradientFill(hDC, vertex, _countof(vertex), &grc, 1, ulMode);
        ::SelectClipRgn(hDC, NULL);
    }

    return bSuccessful;
}

inline BOOL WINAPI GradientFillRgn(HDC hDC, HRGN hRgn, COLORREF clrFrom1, COLORREF clrTo1, COLORREF clrFrom2, COLORREF clrTo2, ULONG ulMode = GRADIENT_FILL_REGION_V, BYTE nPercent = 50)
{
    ATLASSERT(hDC);
    ATLASSERT(hRgn);
    ATLASSERT(ulMode == GRADIENT_FILL_REGION_H || ulMode == GRADIENT_FILL_REGION_V);

    RECT rcBound;
    BOOL bSuccessful = FALSE;

    if (::GetRgnBox(hRgn, &rcBound) > NULLREGION)
    {
        ATLVERIFY(::ExtSelectClipRgn(hDC, hRgn, RGN_AND));
        bSuccessful = GradientFillRect(hDC, rcBound.left, rcBound.top, rcBound.right, rcBound.bottom, clrFrom1, clrTo1, clrFrom2, clrTo2, ulMode, nPercent);
        ::SelectClipRgn(hDC, NULL);
    }

    return bSuccessful;
}

inline void WINAPI CalcTrianglePoints(int left, int top, int right, int bottom, DWORD dwDirection, LPPOINT pPoints)
{
    ATLASSERT(pPoints);
    ATLASSERT(dwDirection >= TRI_DIRECTION_UP && dwDirection <= TRI_DIRECTION_RIGTH);

    pPoints[0].y = top,  pPoints[2].x = right;
    pPoints[1].x = left, pPoints[1].y = bottom;

    switch (dwDirection)
    {
    case TRI_DIRECTION_DOWN:
        pPoints[0].x = left, pPoints[2].y = top;
        pPoints[1].x = left + (right - left) / 2;
        break;

    case TRI_DIRECTION_LEFT:
        pPoints[0].x = right, pPoints[2].y = bottom;
        pPoints[1].y = top + (bottom - top) / 2;
        break;

    case TRI_DIRECTION_RIGTH:
        pPoints[0].x = left;
        pPoints[2].y = top + (bottom - top) / 2;
        break;

    default:
        pPoints[2].y = bottom;
        pPoints[0].x = left + (right - left) / 2;
    }
}

inline BOOL WINAPI DrawTriangle(HDC hDC, const POINT* pPoints, COLORREF clrBorder, COLORREF clrFill)
{
    ATLASSERT(hDC);
    ATLASSERT(pPoints);

#ifndef _WIN32_WCE
    HGDIOBJ hOldPen   = ::SelectObject(hDC, ::GetStockObject(DC_PEN));
    HGDIOBJ hOldBrush = ::SelectObject(hDC, ::GetStockObject(DC_BRUSH));

    ::SetDCPenColor(hDC, clrBorder);
    ::SetDCBrushColor(hDC, clrFill);
    BOOL bSuccessful = ::Polygon(hDC, pPoints, 3);

    ::SelectObject(hDC, hOldPen);
    ::SelectObject(hDC, hOldBrush);

    return bSuccessful;
#else
    HBRUSH hBrush = ::CreateSolidBrush(clrFill);
    HPEN hPen = ::CreatePen(PS_SOLID, 1, clrBorder);
    ATLASSERT(hBrush && hPen);

    HGDIOBJ hOldPen   = ::SelectObject(hDC, hPen);
    HGDIOBJ hOldBrush = ::SelectObject(hDC, hBrush);
    BOOL bSuccessful  = ::Polygon(hDC, pPoints, 3);

    ::SelectObject(hDC, hOldPen);
    ::SelectObject(hDC, hOldBrush);

    ATLVERIFY(::DeleteObject(hPen));
    ATLVERIFY(::DeleteObject(hBrush));

    return bSuccessful;
#endif  // _WIN32_WCE
}

inline BOOL WINAPI DrawTriangle(HDC hDC, int left, int top, int right, int bottom, COLORREF clrBorder, COLORREF clrFill, DWORD dwDirection = TRI_DIRECTION_UP)
{
    ATLASSERT(hDC);

    POINT points[3];
    CalcTrianglePoints(left, top, right, bottom, dwDirection, points);

    return DrawTriangle(hDC, points, clrBorder, clrFill);
}

inline BOOL WINAPI DrawTriangle(HDC hDC, const RECT& rcBorder, COLORREF clrBorder, COLORREF clrFill, DWORD dwDirection = TRI_DIRECTION_UP)
{
    ATLASSERT(hDC);

    POINT points[3];
    CalcTrianglePoints(rcBorder.left, rcBorder.top, rcBorder.right, rcBorder.bottom, dwDirection, points);

    return DrawTriangle(hDC, points, clrBorder, clrFill);
}

inline void WINAPI FrameTriangle(HDC hDC, const POINT* pPoints, COLORREF color)
{
    ATLASSERT(hDC);
    ATLASSERT(pPoints);

#ifndef _WIN32_WCE
    HGDIOBJ hOldPen = ::SelectObject(hDC, ::GetStockObject(DC_PEN));
    ::SetDCPenColor(hDC, color);

    ATLVERIFY(::MoveToEx(hDC, pPoints[0].x, pPoints[0].y, NULL));
    ATLVERIFY(::LineTo(hDC, pPoints[1].x, pPoints[1].y));
    ATLVERIFY(::LineTo(hDC, pPoints[2].x, pPoints[2].y));
    ATLVERIFY(::LineTo(hDC, pPoints[0].x, pPoints[0].y));

    ::SelectObject(hDC, hOldPen);
#else
    HPEN hPen = ::CreatePen(PS_SOLID, 1, color);
    ATLASSERT(hPen);

    HGDIOBJ hOldPen = ::SelectObject(hDC, hPen);
    ATLVERIFY(::MoveToEx(hDC, pPoints[0].x, pPoints[0].y, NULL));
    ATLVERIFY(::LineTo(hDC, pPoints[1].x, pPoints[1].y));
    ATLVERIFY(::LineTo(hDC, pPoints[2].x, pPoints[2].y));
    ATLVERIFY(::LineTo(hDC, pPoints[0].x, pPoints[0].y));

    ::SelectObject(hDC, hOldPen);
    ATLVERIFY(::DeleteObject(hPen));
#endif  // _WIN32_WCE
}

inline void WINAPI FrameTriangle(HDC hDC, int left, int top, int right, int bottom, COLORREF color, DWORD dwDirection = TRI_DIRECTION_UP)
{
    ATLASSERT(hDC);

    POINT points[3];
    CalcTrianglePoints(left, top, right, bottom, dwDirection, points);

    FrameTriangle(hDC, points, color);
}

inline void WINAPI FrameTriangle(HDC hDC, const RECT& rcBorder, COLORREF color, DWORD dwDirection = TRI_DIRECTION_UP)
{
    ATLASSERT(hDC);

    POINT points[3];
    CalcTrianglePoints(rcBorder.left, rcBorder.top, rcBorder.right, rcBorder.bottom, dwDirection, points);

    FrameTriangle(hDC, points, color);
}

inline BOOL WINAPI GradientFillTriangle(HDC hDC, const POINT* pPoints, COLORREF clrVertex1, COLORREF clrVertex2, COLORREF clrVertex3)
{
    ATLASSERT(hDC);
    ATLASSERT(pPoints);

    GRADIENT_TRIANGLE tri = { 0, 1, 2 };
    TRIVERTEX vertex[] =
    {
        { pPoints[0].x, pPoints[0].y, _R16(clrVertex1), _G16(clrVertex1), _B16(clrVertex1) },
        { pPoints[1].x, pPoints[1].y, _R16(clrVertex2), _G16(clrVertex2), _B16(clrVertex2) },
        { pPoints[2].x, pPoints[2].y, _R16(clrVertex3), _G16(clrVertex3), _B16(clrVertex3) }
    };

    return ::GradientFill(hDC, vertex, _countof(vertex), &tri, 1, GRADIENT_FILL_TRIANGLE);
}

inline BOOL WINAPI GradientFillTriangle(HDC hDC, int left, int top, int right, int bottom, COLORREF clrVertex1, COLORREF clrVertex2, COLORREF clrVertex3, DWORD dwDirection = TRI_DIRECTION_UP)
{
    ATLASSERT(hDC);

    POINT points[3];
    CalcTrianglePoints(left, top, right, bottom, dwDirection, points);

    return GradientFillTriangle(hDC, points, clrVertex1, clrVertex2, clrVertex3);
}

inline BOOL WINAPI GradientFillTriangle(HDC hDC, const RECT& rcBorder, COLORREF clrVertex1, COLORREF clrVertex2, COLORREF clrVertex3, DWORD dwDirection = TRI_DIRECTION_UP)
{
    ATLASSERT(hDC);

    POINT points[3];
    CalcTrianglePoints(rcBorder.left, rcBorder.top, rcBorder.right, rcBorder.bottom, dwDirection, points);

    return GradientFillTriangle(hDC, points, clrVertex1, clrVertex2, clrVertex3);
}

}  // namespace stdutil

#endif  // __STDUTIL_H__