///////////////////////////////////////////////////////////////////////////////
// ceimgutil.h
//
// Copyright(c) 2019, Garfield. All rights reserved.
// Author : Garfield
// Version : 1.0
// Creation Date : 2010/10/17

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __CEIMAGEUTIL_H__
#define __CEIMAGEUTIL_H__

#ifndef _WIN32_WCE
    #error ceimgutil.h compiles under Windows CE only
#endif

#include <imaging.h>
#include <atlcoll.h>
#include "atlcomutil.h"
#include "atlmiscutil.h"

///////////////////////////////////////////////////////////////////////////////
// Classes in this file:
//
// CImage
// CImageList
// CAnimateImage
// CAnimateProperty

namespace WCE {

///////////////////////////////////////////////////////////////////////////////
// Forward declarations
//

class CImage;
class CImageList;
class CAnimateImage;


///////////////////////////////////////////////////////////////////////////////
// Interface of the CImageTraits class
//

struct CImageTraits : public atlutil::CHandleTraits
{
    typedef HBITMAP HANDLET_PTR;
    static BOOL WINAPI Destroy(HANDLET_PTR hHandle);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CImage class
//

class CImage : public atlutil::CHandle<CImageTraits>
{
// Constructors
public:
    CImage();

// Operations
public:
    HRESULT Load(IImagingFactory* pFactory, IStream* pStream, DecoderInitFlag flag = DecoderInitFlagBuiltIn1st);
    HRESULT Load(IImagingFactory* pFactory, LPCWSTR pszFileName, DecoderInitFlag flag = DecoderInitFlagBuiltIn1st);
    HRESULT Load(IImagingFactory* pFactory, HINSTANCE hInstance, UINT nIDResource, LPCTSTR pType, DecoderInitFlag flag = DecoderInitFlagBuiltIn1st);

    BOOL Draw(HDC hDC, const RECT& rect, int srcX, int srcY, DWORD dwRop = SRCCOPY) const;
    BOOL Draw(HDC hDC, int x, int y, int nWidth, int nHeight, int srcX, int srcY, DWORD dwRop = SRCCOPY) const;

    BOOL Draw(HDC hDC, const RECT& rect, int srcX, int srcY, int srcWidth, int srcHeight, BYTE nSourceAlpha = 255) const;
    BOOL Draw(HDC hDC, int x, int y, int nWidth, int nHeight, int srcX, int srcY, int srcWidth, int srcHeight, BYTE nSourceAlpha = 255) const;

// Attributes
public:
    UINT GetWidth() const;
    UINT GetHeight() const;

    BOOL IsAlphaChannel() const;

// Implementation
protected:
    HRESULT DecodeImage(IBitmapImage* pBitmap, const ImageInfo& imageInfo);
    HRESULT DecodeAlphaImage(IBitmapImage* pBitmap, const ImageInfo& imageInfo);

    friend class CAnimateImage;
    HRESULT Decode(IImagingFactory* pFactory, IImageDecoder* pDecoder, const ImageInfo& imageInfo);

// Data members
public:
    DWORD m_dwImageSize;
    PixelFormat m_nPixelFormat;
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
    HRESULT Load(IImagingFactory* pFactory, IStream* pStream, UINT nImageWidth, DecoderInitFlag flag = DecoderInitFlagBuiltIn1st);
    HRESULT Load(IImagingFactory* pFactory, LPCWSTR pszFileName, UINT nImageWidth, DecoderInitFlag flag = DecoderInitFlagBuiltIn1st);
    HRESULT Load(IImagingFactory* pFactory, HINSTANCE hInstance, UINT nIDResource, LPCTSTR pType, UINT nImageWidth, DecoderInitFlag flag = DecoderInitFlagBuiltIn1st);

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
// Interface of the CAnimateProperty class
//

class CAnimateProperty : public atlutil::CHeapBufferImpl<PropertyItem>
{
// Constructors
public:
    CAnimateProperty();

// Operations
public:
    BOOL Load(IImageDecoder* pDecoder);

#ifdef _CRT_DEBUG_DUMP
    template <typename _Pr> void Dump(const _Pr& _Printer) const;
#endif

// Attributes
public:
    UINT GetFrameCount() const;
    UINT GetFrameDelay(UINT nFrameIndex) const;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CAnimateImage class
//

class CAnimateImage
{
public:
    typedef ATL::CAtlArray<CImage> CImageArray;

// Constructors
public:
    CAnimateImage();

// Operations
public:
    HRESULT Load(IImagingFactory* pFactory, IStream* pStream, DecoderInitFlag flag = DecoderInitFlagBuiltIn1st);
    HRESULT Load(IImagingFactory* pFactory, LPCWSTR pszFileName, DecoderInitFlag flag = DecoderInitFlagBuiltIn1st);

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

    UINT GetWidth() const;
    UINT GetHeight() const;

    UINT GetFrameCount() const;
    UINT GetFrameDelay(UINT nFrameIndex) const;

    CImage& GetFrame(UINT nFrameIndex);
    const CImage& GetFrame(UINT nFrameIndex) const;

// Implementation
protected:
    HRESULT DecodeImage(IImagingFactory* pFactory, IImageDecoder* pDecoder);

// Data members
public:
    CImageArray m_arrImages;
    CAnimateProperty m_Property;
};

} // namespace WCE

#include "ceimgutil.inl"

#endif // __CEIMAGEUTIL_H__