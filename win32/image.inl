///////////////////////////////////////////////////////////////////////////////
// image.inl
//
// Author : Garfield
// Creation Date : 2007/9/20

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __IMAGE_INL__
#define __IMAGE_INL__

#ifndef __IMAGE_H__
    #error image.inl requires image.h to be included first
#endif

namespace atlutil {

///////////////////////////////////////////////////////////////////////////////
// Implementation of the CImageTraits class
//

inline BOOL WINAPI CImageTraits::Destroy(HANDLET_PTR hHandle)
{
    return ::DeleteObject(hHandle);
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CImage class
//

inline CImage::CImage()
    : m_nPixelFormat(PixelFormatUndefined)
{
}

inline BOOL CImage::Load(LPCWSTR pszFileName, BOOL bUseEmbeddedColorManagement/* = FALSE*/)
{
    ATLASSERT(IsEmpty());
    ATLASSERT(pszFileName);

    Gdiplus::Image image(pszFileName, bUseEmbeddedColorManagement);
    return (image.GetLastStatus() == Gdiplus::Ok && Decode(image));
}

inline BOOL CImage::Load(IStream* pStream, BOOL bUseEmbeddedColorManagement/* = FALSE*/)
{
    ATLASSERT(pStream);
    ATLASSERT(IsEmpty());

    Gdiplus::Image image(pStream, bUseEmbeddedColorManagement);
    return (image.GetLastStatus() == Gdiplus::Ok && Decode(image));
}

inline BOOL CImage::Load(const BITMAPINFO* pBitmapInfo, LPVOID pBitmapData)
{
    ATLASSERT(IsEmpty());
    ATLASSERT(pBitmapInfo);
    ATLASSERT(pBitmapData);

    Gdiplus::Bitmap bitmap(pBitmapInfo, pBitmapData);
    return (bitmap.GetLastStatus() == Gdiplus::Ok && Decode(bitmap));
}

inline BOOL CImage::Load(HINSTANCE hInstance, UINT nIDResource, LPCTSTR pType/* = NULL*/, BOOL bUseEmbeddedColorManagement/* = FALSE*/)
{
    ATLASSERT(IsEmpty());
    UNUSED_PARAMETER(bUseEmbeddedColorManagement);

    BOOL bSuccessful = FALSE;
    if (pType == NULL)
    {
        Gdiplus::Bitmap bitmap(hInstance, MAKEINTRESOURCEW(nIDResource));
        bSuccessful = (bitmap.GetLastStatus() == Gdiplus::Ok && Decode(bitmap));
    }
#ifdef __ATLCOMUTIL_H__
    else
    {
        CComPtr<IStream> spStream;
        if (SUCCEEDED(CMemoryStream::CreateInstance(hInstance, nIDResource, pType, &spStream)))
            bSuccessful = Load(spStream, bUseEmbeddedColorManagement);
    }
#endif  // __ATLCOMUTIL_H__

    return bSuccessful;
}

inline BOOL CImage::Save(IStream* pStream, const CLSID* pclsIdEncoder, const Gdiplus::EncoderParameters* pParameters/* = NULL*/) const
{
    ATLASSERT(pStream);
    ATLASSERT(!IsEmpty());
    ATLASSERT(pclsIdEncoder);

    CBitmapInfo bmpInfo;
    BOOL bSuccessful = FALSE;

    if (Gdiplus::GpImage* pImage = Encode(bmpInfo))
    {
        bSuccessful = (Gdiplus::DllExports::GdipSaveImageToStream(pImage, pStream, pclsIdEncoder, pParameters) == Gdiplus::Ok);
        _COND_VERIFY(Gdiplus::DllExports::GdipDisposeImage(pImage), Gdiplus::Ok);
    }

    return bSuccessful;
}

inline BOOL CImage::Save(LPCWSTR pszFileName, const CLSID* pclsIdEncoder, const Gdiplus::EncoderParameters* pParameters/* = NULL*/) const
{
    ATLASSERT(!IsEmpty());
    ATLASSERT(pszFileName);
    ATLASSERT(pclsIdEncoder);

    CBitmapInfo bmpInfo;
    BOOL bSuccessful = FALSE;

    if (Gdiplus::GpImage* pImage = Encode(bmpInfo))
    {
        bSuccessful = (Gdiplus::DllExports::GdipSaveImageToFile(pImage, pszFileName, pclsIdEncoder, pParameters) == Gdiplus::Ok);
        _COND_VERIFY(Gdiplus::DllExports::GdipDisposeImage(pImage), Gdiplus::Ok);
    }

    return bSuccessful;
}

#if defined(__ATLSAFE_H__) && defined(__ATLCOMUTIL_H__)
inline HRESULT CImage::Load(const ATL::CComSafeArray<BYTE>& saImage, BOOL bUseEmbeddedColorManagement/* = FALSE*/)
{
    ATLASSERT(IsEmpty());
    ATLASSERT(saImage.m_psa);
    ATLASSERT(saImage.GetCount() != 0);
    ATLASSERT(saImage.GetDimensions() == 1);

    LPVOID pData = NULL;
    HRESULT hr = ::SafeArrayAccessData(saImage.m_psa, &pData);
    if (SUCCEEDED(hr))
    {
        CComPtr<IStream> spStream;
        hr = CMemoryStream::CreateInstance((LPBYTE)pData, saImage.GetCount(), &spStream);
        if (SUCCEEDED(hr))
            hr = (Load(spStream, bUseEmbeddedColorManagement) ? S_OK : E_FAIL);

        _COM_VERIFY(::SafeArrayUnaccessData(saImage.m_psa));
    }

    return hr;
}

inline HRESULT CImage::Save(ATL::CComSafeArray<BYTE>& saImage, const CLSID* pclsIdEncoder, const Gdiplus::EncoderParameters* pParameters/* = NULL*/) const
{
    ATLASSERT(!IsEmpty());
    ATLASSERT(pclsIdEncoder);
    ATLASSERT(saImage.m_psa == NULL);

    HRESULT hr = E_FAIL;
    IStream* pStream = NULL;

    __try
    {
        if (FAILED(::CreateStreamOnHGlobal(NULL, TRUE, &pStream)))
            __leave;

        if (!Save(pStream, pclsIdEncoder, pParameters))
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

inline BOOL CImage::Draw(HDC hDC, const RECT& rect, int srcX, int srcY, DWORD dwRop/* = SRCCOPY*/) const
{
    ATLASSERT(hDC);
    ATLASSERT(!IsEmpty());

    CBitmapDC dc(m_hHandle);
    return ::BitBlt(hDC, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, dc, srcX, srcY, dwRop);
}

inline BOOL CImage::Draw(HDC hDC, int x, int y, int nWidth, int nHeight, int srcX, int srcY, DWORD dwRop/* = SRCCOPY*/) const
{
    ATLASSERT(hDC);
    ATLASSERT(!IsEmpty());

    CBitmapDC dc(m_hHandle);
    return ::BitBlt(hDC, x, y, nWidth, nHeight, dc, srcX, srcY, dwRop);
}

inline BOOL CImage::Draw(HDC hDC, const RECT& rect, int srcX, int srcY, int srcWidth, int srcHeight, BYTE nSourceAlpha/* = 255*/) const
{
    ATLASSERT(hDC);
    ATLASSERT(!IsEmpty());

    return Draw(hDC, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, srcX, srcY, srcWidth, srcHeight, nSourceAlpha);
}

inline BOOL CImage::Draw(HDC hDC, int x, int y, int nWidth, int nHeight, int srcX, int srcY, int srcWidth, int srcHeight, BYTE nSourceAlpha/* = 255*/) const
{
    ATLASSERT(hDC);
    ATLASSERT(!IsEmpty());

    BOOL bSuccessful = FALSE;
    CBitmapDC dc(m_hHandle);

    if (IsAlphaChannel())
    {
        BLENDFUNCTION blend = { AC_SRC_OVER, 0, nSourceAlpha, AC_SRC_ALPHA };
        bSuccessful = ::AlphaBlend(hDC, x, y, nWidth, nHeight, dc, srcX, srcY, srcWidth, srcHeight, blend);
    }
    else
    {
        bSuccessful = ::StretchBlt(hDC, x, y, nWidth, nHeight, dc, srcX, srcY, srcWidth, srcHeight, SRCCOPY);
    }

    return bSuccessful;
}

inline BOOL CImage::IsAlphaChannel() const
{
    return Gdiplus::IsAlphaPixelFormat(m_nPixelFormat);
}

inline BOOL CImage::GetBitmap(BITMAP& bmp) const
{
    ATLASSERT(!IsEmpty());
    return ::GetObject(m_hHandle, sizeof(BITMAP), &bmp);
}

inline BOOL CImage::Decode(Gdiplus::Image& image)
{
    ATLASSERT(IsEmpty());

    BOOL bSuccessful = FALSE;
    HDC hDC = ::GetDC(NULL);
    ATLASSERT(hDC);

    Gdiplus::Rect rect(0, 0, image.GetWidth(), image.GetHeight());
    if ((m_hHandle = ::CreateCompatibleBitmap(hDC, rect.Width, rect.Height)) != NULL)
    {
        CBitmapDC dc(m_hHandle);
        m_nPixelFormat = image.GetPixelFormat();

        Gdiplus::Graphics graphics(dc);
        bSuccessful = (graphics.DrawImage(&image, rect, 0, 0, rect.Width, rect.Height, Gdiplus::UnitPixel) == Gdiplus::Ok);
        if (!bSuccessful) Destroy();
    }

    ATLVERIFY(::ReleaseDC(NULL, hDC));
    return bSuccessful;
}

inline Gdiplus::GpImage* CImage::Encode(CBitmapInfo& bmpInfo) const
{
    ATLASSERT(!IsEmpty());

    Gdiplus::GpBitmap* pBitmap = NULL;
    if (IsAlphaChannel())
    {
        if (GetBitmapInfo(bmpInfo))
            _COND_VERIFY(Gdiplus::DllExports::GdipCreateBitmapFromScan0(bmpInfo.bmWidth, bmpInfo.bmHeight, bmpInfo.bmWidthBytes, m_nPixelFormat, (LPBYTE)bmpInfo.bmBits, &pBitmap), Gdiplus::Ok);
    }
    else
    {
        _COND_VERIFY(Gdiplus::DllExports::GdipCreateBitmapFromHBITMAP(m_hHandle, NULL, &pBitmap), Gdiplus::Ok);
    }

    return pBitmap;
}

inline BOOL CImage::GetBitmapInfo(CBitmapInfo& bmpInfo) const
{
    ATLASSERT(!IsEmpty());

    BOOL bSuccessful = FALSE;
    ATLVERIFY(::GetObject(m_hHandle, sizeof(BITMAP), &bmpInfo));
    ATLASSERT(bmpInfo.bmBits == NULL);

    if (bmpInfo.GetBits(bmpInfo.bmWidth * bmpInfo.bmHeight * sizeof(DWORD)))
    {
        HDC hMemDC = CMemoryDCPool::GetDC();
        BITMAPINFO bitmapInfo = { { sizeof(BITMAPINFOHEADER), bmpInfo.bmWidth, -bmpInfo.bmHeight, 1, 32, BI_RGB } };

        bSuccessful = ::GetDIBits(hMemDC, m_hHandle, 0, bmpInfo.bmHeight, bmpInfo.bmBits, &bitmapInfo, DIB_RGB_COLORS);
        CMemoryDCPool::ReleaseDC(hMemDC);
    }

    return bSuccessful;
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CImageList class
//

inline CImageList::CImageList()
    : m_nImageWidth(0)
{
}

inline BOOL CImageList::Load(LPCWSTR pszFileName, UINT nImageWidth, BOOL bUseEmbeddedColorManagement/* = FALSE*/)
{
    ATLASSERT(IsEmpty());
    ATLASSERT(pszFileName);
    ATLASSERT(nImageWidth > 0);

    m_nImageWidth = nImageWidth;
    return m_image.Load(pszFileName, bUseEmbeddedColorManagement);
}

inline BOOL CImageList::Load(IStream* pStream, UINT nImageWidth, BOOL bUseEmbeddedColorManagement/* = FALSE*/)
{
    ATLASSERT(pStream);
    ATLASSERT(IsEmpty());
    ATLASSERT(nImageWidth > 0);

    m_nImageWidth = nImageWidth;
    return m_image.Load(pStream, bUseEmbeddedColorManagement);
}

inline BOOL CImageList::Load(const BITMAPINFO* pBitmapInfo, LPVOID pBitmapData, UINT nImageWidth)
{
    ATLASSERT(IsEmpty());
    ATLASSERT(pBitmapInfo);
    ATLASSERT(pBitmapData);
    ATLASSERT(nImageWidth > 0);

    m_nImageWidth = nImageWidth;
    return m_image.Load(pBitmapInfo, pBitmapData);
}

inline BOOL CImageList::Load(HINSTANCE hInstance, UINT nIDResource, UINT nImageWidth, LPCTSTR pType/* = NULL*/, BOOL bUseEmbeddedColorManagement/* = FALSE*/)
{
    ATLASSERT(IsEmpty());
    ATLASSERT(nImageWidth > 0);

    m_nImageWidth = nImageWidth;
    return m_image.Load(hInstance, nIDResource, pType, bUseEmbeddedColorManagement);
}

inline BOOL CImageList::Save(IStream* pStream, const CLSID* pclsIdEncoder, const Gdiplus::EncoderParameters* pParameters/* = NULL*/) const
{
    ATLASSERT(pStream);
    ATLASSERT(!IsEmpty());
    ATLASSERT(pclsIdEncoder);

    return m_image.Save(pStream, pclsIdEncoder, pParameters);
}

inline BOOL CImageList::Save(LPCWSTR pszFileName, const CLSID* pclsIdEncoder, const Gdiplus::EncoderParameters* pParameters/* = NULL*/) const
{
    ATLASSERT(!IsEmpty());
    ATLASSERT(pszFileName);
    ATLASSERT(pclsIdEncoder);

    return m_image.Save(pszFileName, pclsIdEncoder, pParameters);
}

#if defined(__ATLSAFE_H__) && defined(__ATLCOMUTIL_H__)
inline HRESULT CImageList::Load(const ATL::CComSafeArray<BYTE>& saImage, UINT nImageWidth, BOOL bUseEmbeddedColorManagement/* = FALSE*/)
{
    ATLASSERT(IsEmpty());
    ATLASSERT(saImage.m_psa);
    ATLASSERT(nImageWidth > 0);
    ATLASSERT(saImage.GetCount() != 0);
    ATLASSERT(saImage.GetDimensions() == 1);

    m_nImageWidth = nImageWidth;
    return m_image.Load(saImage, bUseEmbeddedColorManagement);
}

inline HRESULT CImageList::Save(ATL::CComSafeArray<BYTE>& saImage, const CLSID* pclsIdEncoder, const Gdiplus::EncoderParameters* pParameters/* = NULL*/) const
{
    ATLASSERT(!IsEmpty());
    ATLASSERT(pclsIdEncoder);
    ATLASSERT(saImage.m_psa == NULL);

    return m_image.Save(saImage, pclsIdEncoder, pParameters);
}
#endif  // defined(__ATLSAFE_H__) && defined(__ATLCOMUTIL_H__)

inline void CImageList::Destroy()
{
    m_image.Destroy();
}

inline BOOL CImageList::Draw(HDC hDC, UINT nIndex, int x, int y, DWORD dwRop/* = SRCCOPY*/) const
{
    ATLASSERT(hDC);
    ATLASSERT(!IsEmpty());
    ATLASSERT(m_nImageWidth > 0);
    ATLASSERT(nIndex < GetImageCount());

    return m_image.Draw(hDC, x, y, m_nImageWidth, GetImageHeight(), nIndex * m_nImageWidth, 0, dwRop);
}

inline BOOL CImageList::Draw(HDC hDC, UINT nIndex, const RECT& rect, BYTE nSourceAlpha/* = 255*/) const
{
    ATLASSERT(hDC);
    ATLASSERT(!IsEmpty());
    ATLASSERT(m_nImageWidth > 0);
    ATLASSERT(nIndex < GetImageCount());

    return m_image.Draw(hDC, rect, nIndex * m_nImageWidth, 0, m_nImageWidth, GetImageHeight(), nSourceAlpha);
}

inline BOOL CImageList::Draw(HDC hDC, UINT nIndex, int x, int y, int nWidth, int nHeight, BYTE nSourceAlpha/* = 255*/) const
{
    ATLASSERT(hDC);
    ATLASSERT(!IsEmpty());
    ATLASSERT(m_nImageWidth > 0);
    ATLASSERT(nIndex < GetImageCount());

    return m_image.Draw(hDC, x, y, nWidth, nHeight, nIndex * m_nImageWidth, 0, m_nImageWidth, GetImageHeight(), nSourceAlpha);
}

inline BOOL CImageList::IsEmpty() const
{
    return m_image.IsEmpty();
}

inline BOOL CImageList::IsAlphaChannel() const
{
    return m_image.IsAlphaChannel();
}

inline UINT CImageList::GetImageCount() const
{
    ATLASSERT(!IsEmpty());
    ATLASSERT(m_nImageWidth > 0);

    BITMAP bmp;
    ATLVERIFY(m_image.GetBitmap(bmp));
    ATLASSERT(bmp.bmBits == NULL);

    return bmp.bmWidth / m_nImageWidth;
}

inline UINT CImageList::GetImageWidth() const
{
    return m_nImageWidth;
}

inline UINT CImageList::GetImageHeight() const
{
    BITMAP bmp;
    ATLVERIFY(m_image.GetBitmap(bmp));
    ATLASSERT(bmp.bmBits == NULL);

    return bmp.bmHeight;
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CAnimateImage class
//

inline CAnimateImage::CAnimateImage()
{
}

inline BOOL CAnimateImage::Load(LPCWSTR pszFileName, BOOL bUseEmbeddedColorManagement/* = FALSE*/)
{
    ATLASSERT(IsEmpty());
    ATLASSERT(pszFileName);

    Gdiplus::Image image(pszFileName, bUseEmbeddedColorManagement);
    return (image.GetLastStatus() == Gdiplus::Ok && Decode(image));
}

inline BOOL CAnimateImage::Load(IStream* pStream, BOOL bUseEmbeddedColorManagement/* = FALSE*/)
{
    ATLASSERT(pStream);
    ATLASSERT(IsEmpty());

    Gdiplus::Image image(pStream, bUseEmbeddedColorManagement);
    return (image.GetLastStatus() == Gdiplus::Ok && Decode(image));
}

#if defined(__ATLSAFE_H__) && defined(__ATLCOMUTIL_H__)
inline HRESULT CAnimateImage::Load(const ATL::CComSafeArray<BYTE>& saImage, BOOL bUseEmbeddedColorManagement/* = FALSE*/)
{
    ATLASSERT(IsEmpty());
    ATLASSERT(saImage.m_psa);
    ATLASSERT(saImage.GetCount() != 0);
    ATLASSERT(saImage.GetDimensions() == 1);

    LPVOID pData = NULL;
    HRESULT hr = ::SafeArrayAccessData(saImage.m_psa, &pData);
    if (SUCCEEDED(hr))
    {
        CComPtr<IStream> spStream;
        hr = CMemoryStream::CreateInstance((LPBYTE)pData, saImage.GetCount(), &spStream);
        if (SUCCEEDED(hr))
            hr = (Load(spStream, bUseEmbeddedColorManagement) ? S_OK : E_FAIL);

        _COM_VERIFY(::SafeArrayUnaccessData(saImage.m_psa));
    }

    return hr;
}
#endif  // defined(__ATLSAFE_H__) && defined(__ATLCOMUTIL_H__)

inline void CAnimateImage::Destroy()
{
    m_arrImages.RemoveAll();
}

inline BOOL CAnimateImage::Draw(HDC hDC, UINT nFrameIndex, int x, int y, DWORD dwRop/* = SRCCOPY*/) const
{
    ATLASSERT(hDC);
    ATLASSERT(!IsEmpty());
    ATLASSERT(nFrameIndex < m_arrImages.GetCount());

    const CImage& image = m_arrImages[nFrameIndex];
    ATLASSERT(!image.IsEmpty());

    BITMAP bmp;
    ATLVERIFY(image.GetBitmap(bmp));
    ATLASSERT(bmp.bmBits == NULL);

    return image.Draw(hDC, x, y, bmp.bmWidth, bmp.bmHeight, 0, 0, dwRop);
}

inline BOOL CAnimateImage::Draw(HDC hDC, UINT nFrameIndex, const RECT& rect, BYTE nSourceAlpha/* = 255*/) const
{
    ATLASSERT(hDC);
    ATLASSERT(!IsEmpty());
    ATLASSERT(nFrameIndex < m_arrImages.GetCount());

    const CImage& image = m_arrImages[nFrameIndex];
    ATLASSERT(!image.IsEmpty());

    BITMAP bmp;
    ATLVERIFY(image.GetBitmap(bmp));
    ATLASSERT(bmp.bmBits == NULL);

    return image.Draw(hDC, rect, 0, 0, bmp.bmWidth, bmp.bmHeight, nSourceAlpha);
}

inline BOOL CAnimateImage::Draw(HDC hDC, UINT nFrameIndex, const RECT& rect, int srcX, int srcY, int srcWidth, int srcHeight, BYTE nSourceAlpha/* = 255*/) const
{
    ATLASSERT(hDC);
    ATLASSERT(!IsEmpty());
    ATLASSERT(nFrameIndex < m_arrImages.GetCount());

    return m_arrImages[nFrameIndex].Draw(hDC, rect, srcX, srcY, srcWidth, srcHeight, nSourceAlpha);
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
    return m_arrImages.IsEmpty();
}

inline BOOL CAnimateImage::IsAnimation() const
{
    return !m_Property.IsEmpty();
}

inline BOOL CAnimateImage::IsAlphaChannel() const
{
    ATLASSERT(!IsEmpty());
    return m_arrImages[0].IsAlphaChannel();
}

inline BOOL CAnimateImage::GetBitmap(BITMAP& bmp) const
{
    ATLASSERT(!IsEmpty());
    return m_arrImages[0].GetBitmap(bmp);
}

inline UINT CAnimateImage::GetFrameCount() const
{
    return m_arrImages.GetCount();
}

inline UINT CAnimateImage::GetFrameDelay(UINT nFrameIndex) const
{
    ATLASSERT(IsAnimation());
    return m_Property.GetFrameDelay(nFrameIndex);
}

inline CImage& CAnimateImage::GetFrame(UINT nFrameIndex)
{
    ATLASSERT(!IsEmpty());
    ATLASSERT(nFrameIndex < m_arrImages.GetCount());

    return m_arrImages[nFrameIndex];
}

inline const CImage& CAnimateImage::GetFrame(UINT nFrameIndex) const
{
    ATLASSERT(!IsEmpty());
    ATLASSERT(nFrameIndex < m_arrImages.GetCount());

    return m_arrImages[nFrameIndex];
}

inline BOOL CAnimateImage::Decode(Gdiplus::Image& image)
{
    ATLASSERT(IsEmpty());

    UINT nCount = (m_Property.Load(&image) ? m_Property.GetFrameCount() : 1);
    BOOL bSuccessful = m_arrImages.SetCount(nCount);

    for (UINT i = 0; i < nCount && bSuccessful; ++i)
    {
        image.SelectActiveFrame(&Gdiplus::FrameDimensionTime, i);
        bSuccessful = m_arrImages[i].Decode(image);
    }

    if (!bSuccessful)
        Destroy();

    return bSuccessful;
}

}  // namespace atlutil

#endif  // __IMAGE_INL__