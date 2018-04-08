///////////////////////////////////////////////////////////////////////////////
// image.h
//
// Copyright(c) 2012, Garfield. All rights reserved.
// Author  : Garfield
// Version : 2.5
// Finish Time : 2007/9/20

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __IMAGE_H__
#define __IMAGE_H__

#ifdef _WIN32_WCE
    #error image.h compiles under Windows only
#endif

#include "stdutil.h"
#include "gdiputil.h"
#include "atlmiscutil.h"

#pragma push_macro("new")
#undef new
#include <gdiplus.h>
#pragma pop_macro("new")

///////////////////////////////////////////////////////////////////////////////
// Classes in this file:
//
// CImage
// CImageList
// CAnimateImage

#define DECLARE_GRAY_MATRIX(name) \
    static const Gdiplus::ColorMatrix name = \
    { \
        0.30f, 0.30f, 0.30f, 0.00f, 0.00f, \
        0.59f, 0.59f, 0.59f, 0.00f, 0.00f, \
        0.11f, 0.11f, 0.11f, 0.00f, 0.00f, \
        0.00f, 0.00f, 0.00f, 1.00f, 0.00f, \
        0.00f, 0.00f, 0.00f, 0.00f, 1.00f  \
    }

#define DECLARE_DARK_MATRIX(name) \
    static const Gdiplus::ColorMatrix name = \
    { \
        0.75f, 0.00f, 0.00f, 0.00f, 0.00f, \
        0.00f, 0.75f, 0.00f, 0.00f, 0.00f, \
        0.00f, 0.00f, 0.75f, 0.00f, 0.00f, \
        0.00f, 0.00f, 0.00f, 1.00f, 0.00f, \
        0.10f, 0.10f, 0.10f, 0.00f, 1.00f  \
    }

#define DECLARE_BRIGHT_MATRIX(name) \
    static const Gdiplus::ColorMatrix name = \
    { \
        1.05f, 0.00f, 0.00f, 0.00f, 0.00f, \
        0.00f, 1.05f, 0.00f, 0.00f, 0.00f, \
        0.00f, 0.00f, 1.05f, 0.00f, 0.00f, \
        0.00f, 0.00f, 0.00f, 1.00f, 0.00f, \
        0.10f, 0.10f, 0.10f, 0.00f, 1.00f  \
    }

namespace stdutil {

///////////////////////////////////////////////////////////////////////////////
// Interface of the CImageTraits class
//

struct CImageTraits : public atlutil::CHandleTraits
{
    typedef Gdiplus::Bitmap* HANDLET_PTR;
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
    BOOL Load(HINSTANCE hInstance, UINT nIDResource);
    BOOL Load(const BITMAPINFO* pBitmapInfo, LPVOID pvBitmapData);
    BOOL Load(IStream* pStream, BOOL bUseEmbeddedColorManagement = FALSE);
    BOOL Load(LPCTSTR pszFileName, BOOL bUseEmbeddedColorManagement = FALSE);

    BOOL Save(IStream* pStream, const CLSID* pclsIdEncoder, const Gdiplus::EncoderParameters* pParameters = NULL) const;
    BOOL Save(LPCTSTR pszFileName, const CLSID* pclsIdEncoder, const Gdiplus::EncoderParameters* pParameters = NULL) const;

#if defined(__ATLSAFE_H__) && defined(__ATLCOMUTIL_H__)
    HRESULT Load(const ATL::CComSafeArray<BYTE>& saImage, BOOL bUseEmbeddedColorManagement = FALSE);
    HRESULT Save(ATL::CComSafeArray<BYTE>& saImage, const CLSID* pclsIdEncoder, const Gdiplus::EncoderParameters* pParameters = NULL) const;
#endif

    BOOL Draw(HDC hDC, int x, int y, int nWidth, int nHeight, int srcX, int srcY, int srcWidth, int srcHeight, COLORREF clrMask = CLR_INVALID) const;
    BOOL Draw(HDC hDC, const Gdiplus::Rect& destRect, int srcX, int srcY, int srcWidth, int srcHeight, const Gdiplus::ImageAttributes* pAttributes = NULL, Gdiplus::Unit srcUnit = Gdiplus::UnitPixel, Gdiplus::DrawImageAbort pfnDrawImageAbort = NULL, LPVOID pParameter = NULL) const;

    BOOL GetThumbnail(CImage& image, UINT nThumbWidth = 128, UINT nThumbHeight = 128) const;
    BOOL SelectActiveFrame(UINT nIndex = 0, const GUID& dimensionID = Gdiplus::FrameDimensionTime) const;

    BOOL Clone(CImage& image, const Gdiplus::Rect& rect, Gdiplus::PixelFormat format = PixelFormatDontCare) const;
    BOOL Create(Gdiplus::Image* pImage, int x, int y, int nWidth, int nHeight, Gdiplus::PixelFormat format = PixelFormat32bppARGB);

    BOOL UnlockBits(Gdiplus::BitmapData* pBitmapData) const;
    BOOL LockBits(Gdiplus::BitmapData* pBitmapData, const Gdiplus::Rect* pRect = NULL, Gdiplus::PixelFormat format = PixelFormat24bppRGB, UINT nFlags = Gdiplus::ImageLockModeRead) const;

// Attributes
public:
    UINT GetWidth() const;
    UINT GetHeight() const;

    BOOL GetFormat(GUID& formatID) const;
    Gdiplus::PixelFormat GetPixelFormat() const;
    HBITMAP GetBitmap(const Gdiplus::Color& clrBackground = Gdiplus::Color::Transparent) const;
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
    BOOL Load(Gdiplus::Image* pImage, UINT nImageWidth);
    BOOL Load(IStream* pStream, UINT nImageWidth, BOOL bUseEmbeddedColorManagement = FALSE);
    BOOL Load(LPCTSTR pszFileName, UINT nImageWidth, BOOL bUseEmbeddedColorManagement = FALSE);

#ifdef __ATLCOMUTIL_H__
    BOOL Load(HINSTANCE hInstance, UINT nIDResource, LPCTSTR pType, UINT nImageWidth, BOOL bUseEmbeddedColorManagement = FALSE);
#endif

    BOOL Draw(HDC hDC, UINT nIndex, int x, int y, int nWidth, int nHeight, UINT nState) const;
    BOOL Draw(HDC hDC, UINT nIndex, const Gdiplus::Rect& rect, const Gdiplus::ImageAttributes* pAttributes = NULL) const;

    void Destroy();
    HBITMAP GetBitmap(const Gdiplus::Color& clrBackground = Gdiplus::Color::Transparent) const;

// Attributes
public:
    BOOL IsEmpty() const;
    UINT GetImageCount() const;

    UINT GetImageWidth() const;
    UINT GetImageHeight() const;

// Implementation
private:
    CImageList(const CImageList& that);
    CImageList& operator=(const CImageList& that);

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
    BOOL Load(IStream* pStream, BOOL bUseEmbeddedColorManagement = FALSE);
    BOOL Load(LPCTSTR pszFileName, BOOL bUseEmbeddedColorManagement = FALSE);

#if defined(__ATLSAFE_H__) && defined(__ATLCOMUTIL_H__)
    HRESULT Load(const ATL::CComSafeArray<BYTE>& saImage, BOOL bUseEmbeddedColorManagement = FALSE);
#endif

    void Empty();
    BOOL SelectActiveFrame(UINT nIndex) const;
    BOOL Draw(HDC hDC, const RECT& destRect, int srcX = 0, int srcY = 0, int srcWidth = 0, int srcHeight = 0) const;

#ifdef _CRT_DEBUG_DUMP
    template <typename _Pr> void Dump(const _Pr& _Printer) const;
#endif

// Attributes
public:
    BOOL IsEmpty() const;
    BOOL IsAnimation() const;

    UINT GetWidth() const;
    UINT GetHeight() const;

    UINT GetFrameCount() const;
    LONG GetFrameDelay(UINT nIndex) const;

// Implementation
private:
    CAnimateImage(const CAnimateImage& that);
    CAnimateImage& operator=(const CAnimateImage& that);

// Data members
public:
    CImage m_image;
    CAnimateProperty m_Property;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the IAnimatePaint class
//

interface ATL_NO_VTABLE IAnimatePaint
{
    virtual void OnDrawImage(Gdiplus::Graphics& graphics, Gdiplus::Image* pImage, UINT nFrameIndex, UINT nFrameCount, const Gdiplus::Rect& destRect, const Gdiplus::Rect& srcRect) = 0;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CAnimatePaint class
//

class CAnimatePaint : public IAnimatePaint
{
// Constructors/Destructor
public:
    CAnimatePaint(HWND hWnd, HANDLE hEvent, Gdiplus::Image* pImage, IAnimatePaint* pPaint, const CAnimateProperty& Property, const Gdiplus::Rect& destRect, const Gdiplus::Rect& srcRect);
    ~CAnimatePaint();

// Overrides
public:
    virtual void OnDrawImage(Gdiplus::Graphics& graphics, Gdiplus::Image* pImage, UINT nFrameIndex, UINT nFrameCount, const Gdiplus::Rect& destRect, const Gdiplus::Rect& srcRect);

// Operations
public:
    void Paint();

// Data members
private:
    const HWND m_hWnd;
    const HANDLE m_hEvent;
    IAnimatePaint* m_pPaint;
    Gdiplus::Image* m_pImage;
    const Gdiplus::Rect m_srcRect;
    const Gdiplus::Rect m_destRect;
    const CAnimateProperty& m_Property;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CAnimateImage class
//

class CAnimateImage : public CImage
{
// Constructors/Destructor
public:
    CAnimateImage();
    ~CAnimateImage();

// Operations
public:
    BOOL Play(HWND hWnd, int x, int y, int nWidth, int nHeight, int srcX, int srcY, int srcWidth, int srcHeight, IAnimatePaint* pPaint = NULL);
    BOOL Stop(DWORD dwTimeout = INFINITE);

    void Pause();
    void Resume();

#ifdef _CRT_DEBUG_DUMP
    template <typename _Pr> void Dump(const _Pr& _Printer) const;
#endif

// Attributes
public:
    BOOL IsPlaying() const;

// Implementation
protected:
    static DWORD WINAPI ThreadProc(LPVOID pParameter);

// Data members
protected:
    BOOL m_bPlaying;
    HANDLE m_hEvent;
    HANDLE m_hThread;
    CAnimateProperty m_Property;
};

}  // namespace stdutil

#include "image.inl"

#endif  // __IMAGE_H__