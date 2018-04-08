///////////////////////////////////////////////////////////////////////////////
// image.inl
//
// Copyright(c) 2012, Garfield. All rights reserved.
// Author  : Garfield
// Version : 2.5
// Finish Time : 2007/9/20

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __IMAGE_INL__
#define __IMAGE_INL__

namespace stdutil {

///////////////////////////////////////////////////////////////////////////////
// Implementation of the CImageTraits class
//

inline BOOL WINAPI CImageTraits::Destroy(HANDLET_PTR hHandle)
{
    delete hHandle;
    return TRUE;
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CImage class
//

inline CImage::CImage()
{
}

inline BOOL CImage::Load(HINSTANCE hInstance, UINT nIDResource)
{
    ATLASSERT(IsEmpty());
    return ((m_hHandle = Gdiplus::Bitmap::FromResource(hInstance, MAKEINTRESOURCE(nIDResource))) != NULL);
}

inline BOOL CImage::Load(const BITMAPINFO* pBitmapInfo, LPVOID pvBitmapData)
{
    ATLASSERT(IsEmpty());
    ATLASSERT(pBitmapInfo);
    ATLASSERT(pvBitmapData);

    return ((m_hHandle = Gdiplus::Bitmap::FromBITMAPINFO(pBitmapInfo, pvBitmapData)) != NULL);
}

inline BOOL CImage::Load(IStream* pStream, BOOL bUseEmbeddedColorManagement/* = FALSE*/)
{
    ATLASSERT(pStream);
    ATLASSERT(IsEmpty());

    return ((m_hHandle = Gdiplus::Bitmap::FromStream(pStream, bUseEmbeddedColorManagement)) != NULL);
}

inline BOOL CImage::Load(LPCTSTR pszFileName, BOOL bUseEmbeddedColorManagement/* = FALSE*/)
{
    ATLASSERT(IsEmpty());
    ATLASSERT(pszFileName);

    return ((m_hHandle = Gdiplus::Bitmap::FromFile(pszFileName, bUseEmbeddedColorManagement)) != NULL);
}

inline BOOL CImage::Save(IStream* pStream, const CLSID* pclsIdEncoder, const Gdiplus::EncoderParameters* pParameters/* = NULL*/) const
{
    ATLASSERT(pStream);
    ATLASSERT(m_hHandle);
    ATLASSERT(pclsIdEncoder);

    return (m_hHandle->Save(pStream, pclsIdEncoder, pParameters) == Gdiplus::Ok);
}

inline BOOL CImage::Save(LPCTSTR pszFileName, const CLSID* pclsIdEncoder, const Gdiplus::EncoderParameters* pParameters/* = NULL*/) const
{
    ATLASSERT(m_hHandle);
    ATLASSERT(pszFileName);
    ATLASSERT(pclsIdEncoder);

    return (m_hHandle->Save(pszFileName, pclsIdEncoder, pParameters) == Gdiplus::Ok);
}

#if defined(__ATLSAFE_H__) && defined(__ATLCOMUTIL_H__)
inline HRESULT CImage::Load(const ATL::CComSafeArray<BYTE>& saImage, BOOL bUseEmbeddedColorManagement/* = FALSE*/)
{
    ATLASSERT(IsEmpty());
    ATLASSERT(saImage.m_psa);
    ATLASSERT(saImage.GetCount() != 0);
    ATLASSERT(saImage.GetDimensions() == 1);

    LPVOID pvData = NULL;
    HRESULT hr = ::SafeArrayAccessData(saImage.m_psa, &pvData);
    if (SUCCEEDED(hr))
    {
        CComPtr<IStream> spStream;
        hr = atlutil::CMemoryStream::CreateInstance((LPBYTE)pvData, saImage.GetCount(), &spStream);
        if (SUCCEEDED(hr))
            hr = ((m_hHandle = Gdiplus::Bitmap::FromStream(spStream, bUseEmbeddedColorManagement)) != NULL ? S_OK : E_FAIL);

        _COM_VERIFY(::SafeArrayUnaccessData(saImage.m_psa));
    }

    return hr;
}

inline HRESULT CImage::Save(ATL::CComSafeArray<BYTE>& saImage, const CLSID* pclsIdEncoder, const Gdiplus::EncoderParameters* pParameters/* = NULL*/) const
{
    ATLASSERT(m_hHandle);
    ATLASSERT(pclsIdEncoder);
    ATLASSERT(saImage.m_psa == NULL);

    HRESULT hr = E_FAIL;
    IStream* pStream = NULL;

    __try
    {
        if (FAILED(::CreateStreamOnHGlobal(NULL, TRUE, &pStream)))
            __leave;

        if (m_hHandle->Save(pStream, pclsIdEncoder, pParameters) != Gdiplus::Ok)
            __leave;

        HGLOBAL hGlobal = NULL;
        if (FAILED(::GetHGlobalFromStream(pStream, &hGlobal)))
            __leave;

        hr = stdutil::CreateSafeArray(pStream, ::GlobalSize(hGlobal), saImage);
    }
    __finally
    {
        if (pStream != NULL)
            pStream->Release();
    }

    return hr;
}
#endif  // defined(__ATLSAFE_H__) && defined(__ATLCOMUTIL_H__)

inline BOOL CImage::Draw(HDC hDC, int x, int y, int nWidth, int nHeight, int srcX, int srcY, int srcWidth, int srcHeight, COLORREF clrMask/* = CLR_INVALID*/) const
{
    ATLASSERT(hDC);
    ATLASSERT(m_hHandle);

    Gdiplus::Graphics graphics(hDC);
    Gdiplus::ImageAttributes imageAttribs;

    if (clrMask != CLR_INVALID)
    {
        Gdiplus::ColorMap colorMap = { stdutil::MakeARGB(255, clrMask), stdutil::MakeARGB(0, clrMask) };
        imageAttribs.SetRemapTable(1, &colorMap);
    }

    return (graphics.DrawImage(m_hHandle, Gdiplus::Rect(x, y, nWidth, nHeight), srcX, srcY, srcWidth, srcHeight, Gdiplus::UnitPixel, &imageAttribs) == Gdiplus::Ok);
}

inline BOOL CImage::Draw(HDC hDC, const Gdiplus::Rect& destRect, int srcX, int srcY, int srcWidth, int srcHeight, const Gdiplus::ImageAttributes* pAttributes/* = NULL*/, Gdiplus::Unit srcUnit/* = Gdiplus::UnitPixel*/, Gdiplus::DrawImageAbort pfnDrawImageAbort/* = NULL*/, LPVOID pParameter/* = NULL*/) const
{
    ATLASSERT(hDC);
    ATLASSERT(m_hHandle);

    Gdiplus::Graphics graphics(hDC);
    return (graphics.DrawImage(m_hHandle, destRect, srcX, srcY, srcWidth, srcHeight, srcUnit, pAttributes, pfnDrawImageAbort, pParameter) == Gdiplus::Ok);
}

inline BOOL CImage::GetThumbnail(CImage& image, UINT nThumbWidth/* = 128*/, UINT nThumbHeight/* = 128*/) const
{
    ATLASSERT(m_hHandle);
    ATLASSERT(image.IsEmpty());

    return ((image.m_hHandle = static_cast<Gdiplus::Bitmap*>(m_hHandle->GetThumbnailImage(nThumbWidth, nThumbHeight))) != NULL);
}

inline BOOL CImage::SelectActiveFrame(UINT nIndex/* = 0*/, const GUID& dimensionID/* = Gdiplus::FrameDimensionTime*/) const
{
    ATLASSERT(m_hHandle);
    return (m_hHandle->SelectActiveFrame(&dimensionID, nIndex) == Gdiplus::Ok);
}

inline BOOL CImage::Clone(CImage& image, const Gdiplus::Rect& rect, Gdiplus::PixelFormat format/* = PixelFormatDontCare*/) const
{
    ATLASSERT(m_hHandle);
    ATLASSERT(image.IsEmpty());

    return ((image.m_hHandle = m_hHandle->Clone(rect, format)) != NULL);
}

inline BOOL CImage::Create(Gdiplus::Image* pImage, int x, int y, int nWidth, int nHeight, Gdiplus::PixelFormat format/* = PixelFormat32bppARGB*/)
{
    ATLASSERT(pImage);
    ATLASSERT(IsEmpty());
    ATLASSERT(x >= 0 && y >= 0);
    ATLASSERT(nWidth > 0 && nHeight > 0);

    BOOL bSuccessful = FALSE;
    if ((m_hHandle = new Gdiplus::Bitmap(nWidth, nHeight, format)) != NULL && m_hHandle->SetResolution(pImage->GetHorizontalResolution(), pImage->GetVerticalResolution()) == Gdiplus::Ok)
    {
        Gdiplus::Graphics graphics(m_hHandle);
        bSuccessful = (graphics.DrawImage(pImage, 0, 0, x, y, nWidth, nHeight, Gdiplus::UnitPixel) == Gdiplus::Ok);
    }

    return bSuccessful;
}

inline BOOL CImage::UnlockBits(Gdiplus::BitmapData* pBitmapData) const
{
    ATLASSERT(m_hHandle);
    ATLASSERT(pBitmapData);

    return (m_hHandle->UnlockBits(pBitmapData) == Gdiplus::Ok);
}

inline BOOL CImage::LockBits(Gdiplus::BitmapData* pBitmapData, const Gdiplus::Rect* pRect/* = NULL*/, Gdiplus::PixelFormat format/* = PixelFormat24bppRGB*/, UINT nFlags/* = Gdiplus::ImageLockModeRead*/) const
{
    ATLASSERT(m_hHandle);
    ATLASSERT(pBitmapData);

    return (m_hHandle->LockBits(pRect, nFlags, format, pBitmapData) == Gdiplus::Ok);
}

inline UINT CImage::GetWidth() const
{
    ATLASSERT(m_hHandle);
    return m_hHandle->GetWidth();
}

inline UINT CImage::GetHeight() const
{
    ATLASSERT(m_hHandle);
    return m_hHandle->GetHeight();
}

inline BOOL CImage::GetFormat(GUID& formatID) const
{
    ATLASSERT(m_hHandle);
    return (m_hHandle->GetRawFormat(&formatID) == Gdiplus::Ok);
}

inline Gdiplus::PixelFormat CImage::GetPixelFormat() const
{
    ATLASSERT(m_hHandle);
    return m_hHandle->GetPixelFormat();
}

inline HBITMAP CImage::GetBitmap(const Gdiplus::Color& clrBackground/* = Gdiplus::Color::Transparent*/) const
{
    ATLASSERT(m_hHandle);

    HBITMAP hBitmap = NULL;
    _COND_VERIFY(m_hHandle->GetHBITMAP(clrBackground, &hBitmap), Gdiplus::Ok);

    return hBitmap;
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CImageList class
//

inline CImageList::CImageList()
    : m_nImageWidth(0)
{
}

inline BOOL CImageList::Load(Gdiplus::Image* pImage, UINT nImageWidth)
{
    ATLASSERT(pImage);
    ATLASSERT(IsEmpty());
    ATLASSERT(nImageWidth > 0);

    m_nImageWidth = nImageWidth;
    return m_image.Create(pImage, 0, 0, pImage->GetWidth(), pImage->GetHeight());
}

inline BOOL CImageList::Load(IStream* pStream, UINT nImageWidth, BOOL bUseEmbeddedColorManagement/* = FALSE*/)
{
    ATLASSERT(pStream);
    ATLASSERT(IsEmpty());
    ATLASSERT(nImageWidth > 0);

    m_nImageWidth = nImageWidth;
    return m_image.Load(pStream, bUseEmbeddedColorManagement);
}

inline BOOL CImageList::Load(LPCTSTR pszFileName, UINT nImageWidth, BOOL bUseEmbeddedColorManagement/* = FALSE*/)
{
    ATLASSERT(IsEmpty());
    ATLASSERT(pszFileName);
    ATLASSERT(nImageWidth > 0);

    m_nImageWidth = nImageWidth;
    return m_image.Load(pszFileName, bUseEmbeddedColorManagement);
}

#ifdef __ATLCOMUTIL_H__
inline BOOL CImageList::Load(HINSTANCE hInstance, UINT nIDResource, LPCTSTR pType, UINT nImageWidth, BOOL bUseEmbeddedColorManagement/* = FALSE*/)
{
    ATLASSERT(pType);
    ATLASSERT(IsEmpty());
    ATLASSERT(hInstance);
    ATLASSERT(nImageWidth > 0);
    ATLASSERT(nIDResource != 0);

    BOOL bSuccessful = FALSE;
    CComPtr<IStream> spStream;

    m_nImageWidth = nImageWidth;
    if (SUCCEEDED(atlutil::CMemoryStream::CreateInstance(hInstance, nIDResource, pType, &spStream)))
        bSuccessful = m_image.Load(spStream, bUseEmbeddedColorManagement);

    return bSuccessful;
}
#endif  // __ATLCOMUTIL_H__

inline BOOL CImageList::Draw(HDC hDC, UINT nIndex, int x, int y, int nWidth, int nHeight, UINT nState) const
{
    ATLASSERT(hDC);
    ATLASSERT(!IsEmpty());
    ATLASSERT(m_nImageWidth > 0);
    ATLASSERT(nIndex < GetImageCount());

    DECLARE_GRAY_MATRIX(grayMatrix);
    DECLARE_DARK_MATRIX(darkMatrix);
    DECLARE_BRIGHT_MATRIX(brightMatrix);

    Gdiplus::ImageAttributes imageAttribs;
    if (nState & CDIS_SELECTED)
        imageAttribs.SetColorMatrix(&darkMatrix);
    else if (nState & CDIS_HOT)
        imageAttribs.SetColorMatrix(&brightMatrix);
    else if (nState & CDIS_DISABLED)
        imageAttribs.SetColorMatrix(&grayMatrix);

    return m_image.Draw(hDC, Gdiplus::Rect(x, y, nWidth, nHeight), nIndex * m_nImageWidth, 0, m_nImageWidth, m_image.GetHeight(), &imageAttribs);
}

inline BOOL CImageList::Draw(HDC hDC, UINT nIndex, const Gdiplus::Rect& rect, const Gdiplus::ImageAttributes* pAttributes/* = NULL*/) const
{
    ATLASSERT(hDC);
    ATLASSERT(!IsEmpty());
    ATLASSERT(m_nImageWidth > 0);
    ATLASSERT(nIndex < GetImageCount());

    return m_image.Draw(hDC, rect, nIndex * m_nImageWidth, 0, m_nImageWidth, m_image.GetHeight(), pAttributes);
}

inline void CImageList::Destroy()
{
    m_image.Destroy();
}

inline HBITMAP CImageList::GetBitmap(const Gdiplus::Color& clrBackground/* = Gdiplus::Color::Transparent*/) const
{
    ATLASSERT(!IsEmpty());
    return m_image.GetBitmap(clrBackground);
}

inline BOOL CImageList::IsEmpty() const
{
    return m_image.IsEmpty();
}

inline UINT CImageList::GetImageCount() const
{
    ATLASSERT(!IsEmpty());
    ATLASSERT(m_nImageWidth > 0);

    return m_image.GetWidth() / m_nImageWidth;
}

inline UINT CImageList::GetImageWidth() const
{
    return m_nImageWidth;
}

inline UINT CImageList::GetImageHeight() const
{
    return m_image.GetHeight();
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CAnimateImage class
//

inline CAnimateImage::CAnimateImage()
{
}

inline BOOL CAnimateImage::Load(IStream* pStream, BOOL bUseEmbeddedColorManagement/* = FALSE*/)
{
    ATLASSERT(pStream);
    ATLASSERT(IsEmpty());

    BOOL bSuccessful = m_image.Load(pStream, bUseEmbeddedColorManagement);
    if (bSuccessful)
        m_Property.Load(m_image);

    return bSuccessful;
}

inline BOOL CAnimateImage::Load(LPCTSTR pszFileName, BOOL bUseEmbeddedColorManagement/* = FALSE*/)
{
    ATLASSERT(IsEmpty());
    ATLASSERT(pszFileName);

    BOOL bSuccessful = m_image.Load(pszFileName, bUseEmbeddedColorManagement);
    if (bSuccessful)
        m_Property.Load(m_image);

    return bSuccessful;
}

#if defined(__ATLSAFE_H__) && defined(__ATLCOMUTIL_H__)
inline HRESULT CAnimateImage::Load(const ATL::CComSafeArray<BYTE>& saImage, BOOL bUseEmbeddedColorManagement/* = FALSE*/)
{
    ATLASSERT(IsEmpty());

    HRESULT hr = m_image.Load(saImage, bUseEmbeddedColorManagement);
    if (SUCCEEDED(hr))
        m_Property.Load(m_image);

    return hr;
}
#endif  // defined(__ATLSAFE_H__) && defined(__ATLCOMUTIL_H__)

inline void CAnimateImage::Empty()
{
    m_image.Destroy();
    m_Property.Empty();
}

inline BOOL CAnimateImage::SelectActiveFrame(UINT nIndex) const
{
    return m_image.SelectActiveFrame(nIndex);
}

inline BOOL CAnimateImage::Draw(HDC hDC, const RECT& destRect, int srcX/* = 0*/, int srcY/* = 0*/, int srcWidth/* = 0*/, int srcHeight/* = 0*/) const
{
    ATLASSERT(hDC);
    ATLASSERT(!IsEmpty());
    ATLASSERT((srcWidth == 0 && srcHeight == 0) || (srcWidth != 0 && srcHeight != 0));

    if (srcWidth == 0)
    {
        srcWidth  = m_image.GetWidth();
        srcHeight = m_image.GetHeight();
    }

    return m_image.Draw(hDC, Gdiplus::Rect(destRect.left, destRect.top, destRect.right - destRect.left, destRect.bottom - destRect.top), srcX, srcY, srcWidth, srcHeight);
}

#ifdef _CRT_DEBUG_DUMP
template <typename _Pr>
inline void CAnimateImage::Dump(const _Pr& _Printer) const
{
    m_Property.Dump(_Printer);
}
#endif  // _CRT_DEBUG_DUMP

inline BOOL CAnimateImage::IsEmpty() const
{
    return m_image.IsEmpty();
}

inline BOOL CAnimateImage::IsAnimation() const
{
    return !m_Property.IsEmpty();
}

inline UINT CAnimateImage::GetWidth() const
{
    return m_image.GetWidth();
}

inline UINT CAnimateImage::GetHeight() const
{
    return m_image.GetHeight();
}

inline UINT CAnimateImage::GetFrameCount() const
{
    return m_Property.GetFrameCount();
}

inline LONG CAnimateImage::GetFrameDelay(UINT nIndex) const
{
    return m_Property.GetFrameDelay(nIndex);
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CAnimatePaint class
//

inline CAnimatePaint::CAnimatePaint(HWND hWnd, HANDLE hEvent, Gdiplus::Image* pImage, IAnimatePaint* pPaint, const CAnimateProperty& Property, const Gdiplus::Rect& destRect, const Gdiplus::Rect& srcRect)
    : m_hWnd(hWnd), m_hEvent(hEvent), m_pImage(pImage), m_pPaint(pPaint), m_Property(Property), m_destRect(destRect), m_srcRect(srcRect)
{
    ATLASSERT(hEvent);
    ATLASSERT(pImage);
    ATLASSERT(::IsWindow(hWnd));
}

inline CAnimatePaint::~CAnimatePaint()
{
    ATLTRACE(_T("CAnimatePaint::~CAnimatePaint() - destroy the animation paint.\n"));
}

inline void CAnimatePaint::OnDrawImage(Gdiplus::Graphics& graphics, Gdiplus::Image* pImage, UINT nFrameIndex, UINT /*nFrameCount*/, const Gdiplus::Rect& destRect, const Gdiplus::Rect& srcRect)
{
    ATLASSERT(pImage);

    _COND_VERIFY(pImage->SelectActiveFrame(&Gdiplus::FrameDimensionTime, nFrameIndex), Gdiplus::Ok);
    _COND_VERIFY(graphics.DrawImage(pImage, destRect, srcRect.X, srcRect.Y, srcRect.Width, srcRect.Height, Gdiplus::UnitPixel), Gdiplus::Ok);
}

inline void CAnimatePaint::Paint()
{
    ATLASSERT(m_hEvent);
    ATLASSERT(m_pImage);
    ATLASSERT(::IsWindow(m_hWnd));

    if (m_pPaint == NULL)
        m_pPaint = this;

    Gdiplus::Graphics graphics(m_hWnd);
    for (UINT i = 0, nDelay = 0; ::WaitForSingleObject(m_hEvent, nDelay) == WAIT_TIMEOUT; i = (i + 1) % m_Property.GetFrameCount())
    {
        m_pPaint->OnDrawImage(graphics, m_pImage, i, m_Property.GetFrameCount(), m_destRect, m_srcRect);
        nDelay = m_Property.GetFrameDelay(i);
    }
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CAnimateImage class
//

inline CAnimateImage::CAnimateImage()
    : m_bPlaying(FALSE), m_hThread(NULL), m_hEvent(::CreateEvent(NULL, FALSE, FALSE, NULL))
{
    ATLASSERT(m_hEvent);
}

inline CAnimateImage::~CAnimateImage()
{
    ATLASSERT(m_hThread == NULL);
    ATLVERIFY(::CloseHandle(m_hEvent));
}

inline BOOL CAnimateImage::Play(HWND hWnd, int x, int y, int nWidth, int nHeight, int srcX, int srcY, int srcWidth, int srcHeight, IAnimatePaint* pPaint/* = NULL*/)
{
    ATLASSERT(m_hEvent);
    ATLASSERT(m_hHandle);
    ATLASSERT(::IsWindow(hWnd));
    ATLASSERT(m_hThread == NULL);
    ATLASSERT(::WaitForSingleObject(m_hEvent, 0) == WAIT_TIMEOUT);

    if (m_Property.Load(m_hHandle))
        m_hThread = _CreateThread(NULL, 0, ThreadProc, new CAnimatePaint(hWnd, m_hEvent, m_hHandle, pPaint, m_Property, Gdiplus::Rect(x, y, nWidth, nHeight), Gdiplus::Rect(srcX, srcY, srcWidth, srcHeight)), 0, NULL);

    return m_bPlaying = (m_hThread != NULL);
}

inline BOOL CAnimateImage::Stop(DWORD dwTimeout/* = INFINITE*/)
{
    BOOL bSucceessful = TRUE;
    if (m_hThread != NULL)
    {
        Resume();
        ATLVERIFY(::SetEvent(m_hEvent));
        bSucceessful = (::WaitForSingleObject(m_hThread, dwTimeout) == WAIT_OBJECT_0);

        ATLVERIFY(::CloseHandle(m_hThread));
        m_hThread = NULL, m_bPlaying = FALSE;
        ATLTRACE(_T("CAnimateImage::Stop() - stop the draw animation.\n"));
    }

    return bSucceessful;
}

inline void CAnimateImage::Pause()
{
    if (m_hThread != NULL && m_bPlaying)
    {
        m_bPlaying = FALSE;
        ::SuspendThread(m_hThread);
    }
}

inline void CAnimateImage::Resume()
{
    if (m_hThread != NULL && !m_bPlaying)
    {
        m_bPlaying = TRUE;
        ::ResumeThread(m_hThread);
    }
}

#ifdef _CRT_DEBUG_DUMP
template <typename _Pr>
inline void CAnimateImage::Dump(const _Pr& _Printer) const
{
    m_Property.Dump(_Printer);
}
#endif  // _CRT_DEBUG_DUMP

inline BOOL CAnimateImage::IsPlaying() const
{
    return m_bPlaying;
}

inline DWORD WINAPI CAnimateImage::ThreadProc(LPVOID pParameter)
{
    CAnimatePaint* pPaint = reinterpret_cast<CAnimatePaint*>(pParameter);
    ATLASSERT(pPaint);

    pPaint->Paint();
    delete pPaint;

    ATLTRACE(_T("CAnimateImage::ThreadProc() - exiting the draw animation thread.\n"));
    return 0;
}

}  // namespace stdutil

#endif  // __IMAGE_INL__