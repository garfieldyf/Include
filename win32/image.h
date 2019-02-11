///////////////////////////////////////////////////////////////////////////////
// image.h
//
// Copyright(c) 2019, Garfield. All rights reserved.
// Author  : Garfield
// Version : 3.0
// Creation Date : 2007/9/20

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __IMAGE_H__
#define __IMAGE_H__

#ifdef _WIN32_WCE
    #error image.h compiles under Windows only
#endif

#include <atlcoll.h>
#include "gdiputil.h"

///////////////////////////////////////////////////////////////////////////////
// Classes in this file:
//
// CImage
// CImageList
// CAnimateImage

namespace atlutil {

///////////////////////////////////////////////////////////////////////////////
// Forward declarations
//

class CImage;
class CImageList;
class CAnimateImage;


///////////////////////////////////////////////////////////////////////////////
// Interface of the CImageTraits class
//

struct CImageTraits : public CHandleTraits
{
    typedef HBITMAP HANDLET_PTR;
    static BOOL WINAPI Destroy(HANDLET_PTR hHandle);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CImage class
//

class CImage : public CHandle<CImageTraits>
{
// Constructors
public:
    CImage();

// Operations
public:
    BOOL Load(LPCWSTR pszFileName, BOOL bUseEmbeddedColorManagement = FALSE);
    BOOL Load(IStream* pStream, BOOL bUseEmbeddedColorManagement = FALSE);
    BOOL Load(const BITMAPINFO* pBitmapInfo, LPVOID pBitmapData);
    BOOL Load(HINSTANCE hInstance, UINT nIDResource, LPCTSTR pType = NULL, BOOL bUseEmbeddedColorManagement = FALSE);

    BOOL Save(IStream* pStream, const CLSID* pclsIdEncoder, const Gdiplus::EncoderParameters* pParameters = NULL) const;
    BOOL Save(LPCWSTR pszFileName, const CLSID* pclsIdEncoder, const Gdiplus::EncoderParameters* pParameters = NULL) const;

#if defined(__ATLSAFE_H__) && defined(__ATLCOMUTIL_H__)
    HRESULT Load(const ATL::CComSafeArray<BYTE>& saImage, BOOL bUseEmbeddedColorManagement = FALSE);
    HRESULT Save(ATL::CComSafeArray<BYTE>& saImage, const CLSID* pclsIdEncoder, const Gdiplus::EncoderParameters* pParameters = NULL) const;
#endif

    BOOL Draw(HDC hDC, const RECT& rect, int srcX, int srcY, DWORD dwRop = SRCCOPY) const;
    BOOL Draw(HDC hDC, int x, int y, int nWidth, int nHeight, int srcX, int srcY, DWORD dwRop = SRCCOPY) const;

    BOOL Draw(HDC hDC, const RECT& rect, int srcX, int srcY, int srcWidth, int srcHeight, BYTE nSourceAlpha = 255) const;
    BOOL Draw(HDC hDC, int x, int y, int nWidth, int nHeight, int srcX, int srcY, int srcWidth, int srcHeight, BYTE nSourceAlpha = 255) const;

// Attributes
public:
    BOOL IsAlphaChannel() const;
    BOOL GetBitmap(BITMAP& bmp) const;

// Implementation
protected:
    BOOL GetBitmapInfo(CBitmapInfo& bmpInfo) const;
    friend class CAnimateImage;

    BOOL Decode(Gdiplus::Image& image);
    Gdiplus::GpImage* Encode(CBitmapInfo& bmpInfo) const;

// Data members
public:
    Gdiplus::PixelFormat m_nPixelFormat;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CImageList class
//

class CImageList
{
// Constructors
public:
    CImageList();

// Operations
public:
    BOOL Load(LPCWSTR pszFileName, UINT nImageWidth, BOOL bUseEmbeddedColorManagement = FALSE);
    BOOL Load(IStream* pStream, UINT nImageWidth, BOOL bUseEmbeddedColorManagement = FALSE);
    BOOL Load(const BITMAPINFO* pBitmapInfo, LPVOID pBitmapData, UINT nImageWidth);
    BOOL Load(HINSTANCE hInstance, UINT nIDResource, UINT nImageWidth, LPCTSTR pType = NULL, BOOL bUseEmbeddedColorManagement = FALSE);

    BOOL Save(IStream* pStream, const CLSID* pclsIdEncoder, const Gdiplus::EncoderParameters* pParameters = NULL) const;
    BOOL Save(LPCWSTR pszFileName, const CLSID* pclsIdEncoder, const Gdiplus::EncoderParameters* pParameters = NULL) const;

#if defined(__ATLSAFE_H__) && defined(__ATLCOMUTIL_H__)
    HRESULT Load(const ATL::CComSafeArray<BYTE>& saImage, UINT nImageWidth, BOOL bUseEmbeddedColorManagement = FALSE);
    HRESULT Save(ATL::CComSafeArray<BYTE>& saImage, const CLSID* pclsIdEncoder, const Gdiplus::EncoderParameters* pParameters = NULL) const;
#endif

    void Destroy();
    BOOL Draw(HDC hDC, UINT nIndex, int x, int y, DWORD dwRop = SRCCOPY) const;
    BOOL Draw(HDC hDC, UINT nIndex, const RECT& rect, BYTE nSourceAlpha = 255) const;
    BOOL Draw(HDC hDC, UINT nIndex, int x, int y, int nWidth, int nHeight, BYTE nSourceAlpha = 255) const;

// Attributes
public:
    BOOL IsEmpty() const;
    BOOL IsAlphaChannel() const;

    UINT GetImageCount() const;
    UINT GetImageWidth() const;
    UINT GetImageHeight() const;

// Data members
public:
    CImage m_image;
    UINT m_nImageWidth;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CAnimateImage class
//

class CAnimateImage
{
// Constructors
public:
    CAnimateImage();

// Operations
public:
    BOOL Load(LPCWSTR pszFileName, BOOL bUseEmbeddedColorManagement = FALSE);
    BOOL Load(IStream* pStream, BOOL bUseEmbeddedColorManagement = FALSE);

#if defined(__ATLSAFE_H__) && defined(__ATLCOMUTIL_H__)
    HRESULT Load(const ATL::CComSafeArray<BYTE>& saImage, BOOL bUseEmbeddedColorManagement = FALSE);
#endif

    void Destroy();
    BOOL Draw(HDC hDC, UINT nFrameIndex, int x, int y, DWORD dwRop = SRCCOPY) const;
    BOOL Draw(HDC hDC, UINT nFrameIndex, const RECT& rect, BYTE nSourceAlpha = 255) const;
    BOOL Draw(HDC hDC, UINT nFrameIndex, const RECT& rect, int srcX, int srcY, int srcWidth, int srcHeight, BYTE nSourceAlpha = 255) const;

#ifdef _CRT_DEBUG_DUMP
    template <typename _Pr> void Dump(const _Pr& _Printer) const;
#endif

// Attributes
public:
    BOOL IsEmpty() const;
    BOOL IsAnimation() const;
    BOOL IsAlphaChannel() const;

    BOOL GetBitmap(BITMAP& bmp) const;
    UINT GetFrameCount() const;
    UINT GetFrameDelay(UINT nFrameIndex) const;

    CImage& GetFrame(UINT nFrameIndex);
    const CImage& GetFrame(UINT nFrameIndex) const;

// Implementation
protected:
    BOOL Decode(Gdiplus::Image& image);

// Data members
public:
    ATL::CAtlArray<CImage> m_arrImages;
    stdutil::CAnimateProperty m_Property;
};

}  // namespace atlutil

#include "image.inl"

#endif  // __IMAGE_H__