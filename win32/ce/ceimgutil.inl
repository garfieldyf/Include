///////////////////////////////////////////////////////////////////////////////
// ceimgutil.inl
//
// Copyright(c) 2019, Garfield. All rights reserved.
// Author : Garfield
// Version : 1.0
// Creation Date : 2010/10/17

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __CEIMAGEUTIL_INL__
#define __CEIMAGEUTIL_INL__

#ifndef __CEIMAGEUTIL_H__
    #error ceimgutil.inl requires ceimgutil.h to be included first
#endif

namespace WCE {

#ifdef _ATL_INIT_IMAGING_GUID
///////////////////////////////////////////////////////////////////////////////
// Global variables
//

// FrameDimensionTime
static const GUID FrameDimensionTime = { 0x6aedbd6d, 0x3fb5, 0x418a, { 0x83, 0xa6, 0x7f, 0x45, 0x22, 0x9d, 0xc8, 0x72 } };
#endif  // _ATL_INIT_IMAGING_GUID


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
    : m_dwImageSize(0), m_nPixelFormat(PixelFormatUndefined)
{
}

inline HRESULT CImage::Load(IImagingFactory* pFactory, IStream* pStream, DecoderInitFlag flag/* = DecoderInitFlagBuiltIn1st*/)
{
    ATLASSERT(pStream);
    ATLASSERT(pFactory);
    ATLASSERT(IsEmpty());

    ATL::CComPtr<IImageDecoder> spDecoder;
    HRESULT hr = pFactory->CreateImageDecoder(pStream, flag, &spDecoder);
    if (SUCCEEDED(hr))
    {
        ImageInfo imageInfo;
        _COM_VERIFY(spDecoder->GetImageInfo(&imageInfo));

        hr = Decode(pFactory, spDecoder, imageInfo);
    }

    return hr;
}

inline HRESULT CImage::Load(IImagingFactory* pFactory, LPCWSTR pszFileName, DecoderInitFlag flag/* = DecoderInitFlagBuiltIn1st*/)
{
    ATLASSERT(pFactory);
    ATLASSERT(IsEmpty());
    ATLASSERT(pszFileName);

    ATL::CComPtr<IStream> spStream;
    HRESULT hr = atlutil::CFileStream::CreateInstance(pszFileName, &spStream);
    if (SUCCEEDED(hr))
        hr = Load(pFactory, spStream, flag);

    return hr;
}

inline HRESULT CImage::Load(IImagingFactory* pFactory, HINSTANCE hInstance, UINT nIDResource, LPCTSTR pType, DecoderInitFlag flag/* = DecoderInitFlagBuiltIn1st*/)
{
    ATLASSERT(pType);
    ATLASSERT(pFactory);
    ATLASSERT(IsEmpty());
    ATLASSERT(hInstance);
    ATLASSERT(nIDResource != 0);

    ATL::CComPtr<IStream> spStream;
    HRESULT hr = atlutil::CMemoryStream::CreateInstance(hInstance, nIDResource, pType, &spStream);
    if (SUCCEEDED(hr))
        hr = Load(pFactory, spStream, flag);

    return hr;
}

inline BOOL CImage::Draw(HDC hDC, const RECT& rect, int srcX, int srcY, DWORD dwRop/* = SRCCOPY*/) const
{
    ATLASSERT(hDC);
    ATLASSERT(!IsEmpty());

    atlutil::CBitmapDC dc(m_hHandle);
    return ::BitBlt(hDC, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, dc, srcX, srcY, dwRop);
}

inline BOOL CImage::Draw(HDC hDC, int x, int y, int nWidth, int nHeight, int srcX, int srcY, DWORD dwRop/* = SRCCOPY*/) const
{
    ATLASSERT(hDC);
    ATLASSERT(!IsEmpty());

    atlutil::CBitmapDC dc(m_hHandle);
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
    atlutil::CBitmapDC dc(m_hHandle);

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

inline UINT CImage::GetWidth() const
{
    return LOWORD(m_dwImageSize);
}

inline UINT CImage::GetHeight() const
{
    return HIWORD(m_dwImageSize);
}

inline BOOL CImage::IsAlphaChannel() const
{
    return (m_nPixelFormat & PixelFormatAlpha);
}

inline HRESULT CImage::DecodeImage(IBitmapImage* pBitmap, const ImageInfo& imageInfo)
{
    ATLASSERT(pBitmap);
    ATLASSERT(IsEmpty());

    HDC hDC = ::GetDC(NULL);
    ATLASSERT(hDC);
    HRESULT hr = E_OUTOFMEMORY;

    if ((m_hHandle = ::CreateCompatibleBitmap(hDC, imageInfo.Width, imageInfo.Height)) != NULL)
    {
        atlutil::CBitmapDC dc(m_hHandle);
        RECT rect = { 0, 0, imageInfo.Width, imageInfo.Height };

        CComQIPtr<IImage> spImage = pBitmap;
        hr = spImage->Draw(dc, &rect, NULL);
        if (FAILED(hr)) Destroy();
    }

    ATLVERIFY(::ReleaseDC(NULL, hDC));
    return hr;
}

inline HRESULT CImage::DecodeAlphaImage(IBitmapImage* pBitmap, const ImageInfo& imageInfo)
{
    ATLASSERT(pBitmap);
    ATLASSERT(IsEmpty());

    BitmapData bitmapData;
    if (SUCCEEDED(pBitmap->LockBits(NULL, ImageLockModeRead, imageInfo.PixelFormat, &bitmapData)))
    {
        LPVOID pBits = NULL;
        BITMAPINFO bitmapInfo = { { sizeof(BITMAPINFOHEADER), imageInfo.Width, (bitmapData.Stride > 0 ? -(int)imageInfo.Height : (int)imageInfo.Height), 1, 32, BI_RGB } };

        if ((m_hHandle = ::CreateDIBSection(NULL, &bitmapInfo, DIB_RGB_COLORS, &pBits, NULL, 0)) != NULL)
            ::CopyMemory(pBits, bitmapData.Scan0, ::abs(bitmapData.Stride) * imageInfo.Height);

        _COM_VERIFY(pBitmap->UnlockBits(&bitmapData));
    }

    return (m_hHandle != NULL ? S_OK : E_OUTOFMEMORY);
}

inline HRESULT CImage::Decode(IImagingFactory* pFactory, IImageDecoder* pDecoder, const ImageInfo& imageInfo)
{
    ATLASSERT(pFactory);
    ATLASSERT(pDecoder);
    ATLASSERT(IsEmpty());

    CComPtr<IBitmapImage> spBitmap;
    HRESULT hr = pFactory->CreateNewBitmap(imageInfo.Width, imageInfo.Height, imageInfo.PixelFormat, &spBitmap);
    if (SUCCEEDED(hr))
    {
        ATL::CComQIPtr<IImageSink> spImageSink = spBitmap;

        _COM_VERIFY(pDecoder->BeginDecode(spImageSink, NULL));
        hr = pDecoder->Decode();
        _COM_VERIFY(pDecoder->EndDecode(hr));

        if (SUCCEEDED(hr))
        {
            m_nPixelFormat = imageInfo.PixelFormat;
            m_dwImageSize  = MAKELONG(imageInfo.Width, imageInfo.Height);
            hr = (IsAlphaChannel() ? DecodeAlphaImage(spBitmap.p, imageInfo) : DecodeImage(spBitmap.p, imageInfo));
        }
    }

    return hr;
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CImageList class
//

inline CImageList::CImageList()
    : m_nImageWidth(0)
{
}

inline HRESULT CImageList::Load(IImagingFactory* pFactory, LPCWSTR pszFileName, UINT nImageWidth, DecoderInitFlag flag/* = DecoderInitFlagBuiltIn1st*/)
{
    ATLASSERT(pFactory);
    ATLASSERT(IsEmpty());
    ATLASSERT(pszFileName);
    ATLASSERT(nImageWidth > 0);

    m_nImageWidth = nImageWidth;
    return m_image.Load(pFactory, pszFileName, flag);
}

inline HRESULT CImageList::Load(IImagingFactory* pFactory, IStream* pStream, UINT nImageWidth, DecoderInitFlag flag/* = DecoderInitFlagBuiltIn1st*/)
{
    ATLASSERT(pStream);
    ATLASSERT(pFactory);
    ATLASSERT(IsEmpty());
    ATLASSERT(nImageWidth > 0);

    m_nImageWidth = nImageWidth;
    return m_image.Load(pFactory, pStream, flag);
}

inline HRESULT CImageList::Load(IImagingFactory* pFactory, HINSTANCE hInstance, UINT nIDResource, LPCTSTR pType, UINT nImageWidth, DecoderInitFlag flag/* = DecoderInitFlagBuiltIn1st*/)
{
    ATLASSERT(pType);
    ATLASSERT(pFactory);
    ATLASSERT(IsEmpty());
    ATLASSERT(hInstance);
    ATLASSERT(nImageWidth > 0);
    ATLASSERT(nIDResource != 0);

    m_nImageWidth = nImageWidth;
    return m_image.Load(pFactory, hInstance, nIDResource, pType, flag);
}

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

    return m_image.GetWidth() / m_nImageWidth;
}

inline UINT CImageList::GetImageWidth() const
{
    return m_nImageWidth;
}

inline UINT CImageList::GetImageHeight() const
{
    ATLASSERT(!IsEmpty());
    return m_image.GetHeight();
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CAnimateProperty class
//

inline CAnimateProperty::CAnimateProperty()
{
}

inline BOOL CAnimateProperty::Load(IImageDecoder* pDecoder)
{
    ATLASSERT(pDecoder);

    UINT cbSize = 0;
    BOOL bSuccessful = (SUCCEEDED(pDecoder->GetPropertyItemSize(PropertyTagFrameDelay, &cbSize)) && m_buffer.allocateBytes(cbSize) != NULL
        && SUCCEEDED(pDecoder->GetPropertyItem(PropertyTagFrameDelay, cbSize, m_buffer.data)));

    if (bSuccessful)
    {
        ATLASSERT(m_buffer.data->length % sizeof(UINT) == 0);
        ATLASSERT(m_buffer.data->type == PropertyTagTypeLong);

        m_buffer.data->length /= sizeof(UINT);
        for (UINT i = 0; i < m_buffer.data->length; ++i)
            ((LPUINT)m_buffer.data->value)[i] *= 10;

    #ifdef _DEBUG
        UINT nCount = 0;
        _COM_VERIFY(pDecoder->GetFrameCount(&FrameDimensionTime, &nCount));
        ATLASSERT(nCount == GetFrameCount());
    #endif  // _DEBUG
    }
    else
    {
        m_buffer.clear();
    }

    return bSuccessful;
}

#ifdef _CRT_DEBUG_DUMP
template <typename _Pr>
inline void CAnimateProperty::Dump(const _Pr& _Printer) const
{
    if (!IsEmpty())
    {
        _Printer(_T("\n[ FRAME COUNT = %u ]\n[\n"), GetFrameCount());
        for (UINT i = 0; i < GetFrameCount(); ++i)
            _Printer(_T("\tINDEX = %02u, DELAY TIME = %u ms\n"), i + 1, GetFrameDelay(i));
        _Printer(_T("]\n"));
    }
    else
    {
        _Printer(_T("\n[ THE ANIMATE IMAGE FRAME INFO IS EMPTY! ]\n"));
    }
}
#endif  // _CRT_DEBUG_DUMP

inline UINT CAnimateProperty::GetFrameCount() const
{
    ATLASSERT(!IsEmpty());
    return m_buffer.data->length;
}

inline UINT CAnimateProperty::GetFrameDelay(UINT nFrameIndex) const
{
    ATLASSERT(!IsEmpty());
    ATLASSERT(nFrameIndex < GetFrameCount());

    return ((LPUINT)m_buffer.data->value)[nFrameIndex];
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CAnimateImage class
//

inline CAnimateImage::CAnimateImage()
{
}

inline HRESULT CAnimateImage::Load(IImagingFactory* pFactory, IStream* pStream, DecoderInitFlag flag/* = DecoderInitFlagBuiltIn1st*/)
{
    ATLASSERT(pStream);
    ATLASSERT(pFactory);
    ATLASSERT(IsEmpty());

    ATL::CComPtr<IImageDecoder> spDecoder;
    HRESULT hr = pFactory->CreateImageDecoder(pStream, flag, &spDecoder);
    if (SUCCEEDED(hr))
    {
        hr = DecodeImage(pFactory, spDecoder);
        if (FAILED(hr)) Destroy();
    }

    return hr;
}

inline HRESULT CAnimateImage::Load(IImagingFactory* pFactory, LPCWSTR pszFileName, DecoderInitFlag flag/* = DecoderInitFlagBuiltIn1st*/)
{
    ATLASSERT(pFactory);
    ATLASSERT(IsEmpty());
    ATLASSERT(pszFileName);

    ATL::CComPtr<IStream> spStream;
    HRESULT hr = atlutil::CFileStream::CreateInstance(pszFileName, &spStream);
    if (SUCCEEDED(hr))
        hr = Load(pFactory, spStream, flag);

    return hr;
}

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
    return image.Draw(hDC, x, y, image.GetWidth(), image.GetHeight(), 0, 0, dwRop);
}

inline BOOL CAnimateImage::Draw(HDC hDC, UINT nFrameIndex, const RECT& rect, BYTE nSourceAlpha/* = 255*/) const
{
    ATLASSERT(hDC);
    ATLASSERT(!IsEmpty());
    ATLASSERT(nFrameIndex < m_arrImages.GetCount());

    const CImage& image = m_arrImages[nFrameIndex];
    return image.Draw(hDC, rect, 0, 0, image.GetWidth(), image.GetHeight(), nSourceAlpha);
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

inline UINT CAnimateImage::GetWidth() const
{
    ATLASSERT(!IsEmpty());
    return m_arrImages[0].GetWidth();
}

inline UINT CAnimateImage::GetHeight() const
{
    ATLASSERT(!IsEmpty());
    return m_arrImages[0].GetHeight();
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

inline HRESULT CAnimateImage::DecodeImage(IImagingFactory* pFactory, IImageDecoder* pDecoder)
{
    ATLASSERT(pFactory);
    ATLASSERT(pDecoder);
    ATLASSERT(IsEmpty());

    ImageInfo imageInfo;
    _COM_VERIFY(pDecoder->GetImageInfo(&imageInfo));

    UINT nCount = (m_Property.Load(pDecoder) ? m_Property.GetFrameCount() : 1);
    ATLVERIFY(m_arrImages.SetCount(nCount));

    HRESULT hr = S_OK;
    for (UINT i = 0; i < nCount && SUCCEEDED(hr); ++i)
    {
        pDecoder->SelectActiveFrame(&FrameDimensionTime, i);
        hr = m_arrImages[i].Decode(pFactory, pDecoder, imageInfo);
    }

    return hr;
}

} // namespace WCE

#endif // __CEIMAGEUTIL_INL__