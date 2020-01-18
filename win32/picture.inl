///////////////////////////////////////////////////////////////////////////////
// picture.inl
//
// Author : Garfield
// Creation Date : 2007/10/20

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __PICTURE_INL__
#define __PICTURE_INL__

#ifndef __PICTURE_H__
    #error picture.inl requires picture.h to be included first
#endif

namespace stdutil {

///////////////////////////////////////////////////////////////////////////////
// Implementation of the CPicture class
//

inline CPicture::CPicture()
{
}

inline CPicture::CPicture(const CPicture& that)
    : m_spPicture(that.m_spPicture)
{
}

inline CPicture::CPicture(IPicture* pPicture)
    : m_spPicture(pPicture)
{
}

inline HRESULT CPicture::Load(LPCTSTR pszURLorPath)
{
    ATLASSERT(IsEmpty());
    ATLASSERT(pszURLorPath);

    return ::OleLoadPicturePath((LPOLESTR)pszURLorPath, NULL, 0, 0, IID_IPicture, (LPVOID*)&m_spPicture);
}

inline HRESULT CPicture::Load(IStream* pStream, BOOL bRunmode/* = FALSE*/)
{
    ATLASSERT(pStream);
    ATLASSERT(IsEmpty());

    return ::OleLoadPicture(pStream, 0, bRunmode, IID_IPicture, (LPVOID*)&m_spPicture);
}

inline HRESULT CPicture::Load(HINSTANCE hInstance, UINT nIDResource, LPCTSTR pType, BOOL bRunmode/* = FALSE*/)
{
    ATLASSERT(pType);
    ATLASSERT(nIDResource != 0);

    ATL::CComPtr<IStream> spStream;
    HRESULT hr = atlutil::CMemoryStream::CreateInstance(hInstance, nIDResource, pType, &spStream);
    if (SUCCEEDED(hr))
        hr = Load(spStream, bRunmode);

    return hr;
}

inline HRESULT CPicture::Save(LPCTSTR pszFileName) const
{
    ATLASSERT(pszFileName);
    ATLASSERT(m_spPicture != NULL);

    ATL::CComPtr<IStream> spStream;
    HRESULT hr = atlutil::CFileStream::CreateInstance(pszFileName, &spStream, GENERIC_WRITE, 0, CREATE_ALWAYS);
    if (SUCCEEDED(hr))
    {
        LONG cbSize = 0;
        hr = m_spPicture->SaveAsFile(spStream, FALSE, &cbSize);
    }

    return hr;
}

inline HRESULT CPicture::Save(IStream* pStream, BOOL bSaveMemCopy/* = FALSE*/, LONG* pcbSize/* = NULL*/) const
{
    ATLASSERT(pStream);
    ATLASSERT(m_spPicture != NULL);

    return m_spPicture->SaveAsFile(pStream, bSaveMemCopy, pcbSize);
}

#if defined(__ATLSAFE_H__) && defined(__ATLCOMUTIL_H__)
inline HRESULT CPicture::Load(const ATL::CComSafeArray<BYTE>& saPicture, BOOL bRunmode/* = FALSE*/)
{
    ATLASSERT(IsEmpty());
    ATLASSERT(saPicture.m_psa);
    ATLASSERT(saPicture.GetCount() != 0);
    ATLASSERT(saPicture.GetDimensions() == 1);

    LPVOID pvData = NULL;
    HRESULT hr = ::SafeArrayAccessData(saPicture.m_psa, &pvData);
    if (SUCCEEDED(hr))
    {
        CComPtr<IStream> spStream;
        hr = atlutil::CMemoryStream::CreateInstance((LPBYTE)pvData, saPicture.GetCount(), &spStream);
        if (SUCCEEDED(hr))
            hr = ::OleLoadPicture(spStream, 0, bRunmode, IID_IPicture, (LPVOID*)&m_spPicture);

        _COM_VERIFY(::SafeArrayUnaccessData(saPicture.m_psa));
    }

    return hr;
}

inline HRESULT CPicture::Save(ATL::CComSafeArray<BYTE>& saPicture) const
{
    ATLASSERT(m_spPicture != NULL);
    ATLASSERT(saPicture.m_psa == NULL);

    CComPtr<IStream> spStream;
    HRESULT hr = ::CreateStreamOnHGlobal(NULL, TRUE, &spStream);
    if (SUCCEEDED(hr))
    {
        LONG cbSize = 0;
        hr = m_spPicture->SaveAsFile(spStream, FALSE, &cbSize);
        if (SUCCEEDED(hr))
            hr = stdutil::CreateSafeArray(spStream, cbSize, saPicture);
    }

    return hr;
}
#endif  // defined(__ATLSAFE_H__) && defined(__ATLCOMUTIL_H__)

inline HRESULT CPicture::PictureChanged()
{
    ATLASSERT(m_spPicture != NULL);
    return m_spPicture->PictureChanged();
}

inline HRESULT CPicture::SelectPicture(HDC hDCIn, HDC* phDCOut/* = NULL*/, OLE_HANDLE* phBmpOut/* = NULL*/)
{
    ATLASSERT(hDCIn);
    ATLASSERT(m_spPicture != NULL);

    return m_spPicture->SelectPicture(hDCIn, phDCOut, phBmpOut);
}

inline HRESULT CPicture::Draw(HDC hDC, const RECT& destRect, LONG srcX, LONG srcY, LONG srcWidth, LONG srcHeight, LPCRECT prcWBounds/* = NULL*/) const
{
    ATLASSERT(hDC);
    ATLASSERT(m_spPicture != NULL);

    RECT srcRect = { srcX, srcY, srcX + srcWidth, srcY + srcHeight };
    stdutil::PixelToHIMETRIC(hDC, &srcRect);

    return m_spPicture->Render(hDC, destRect.left, destRect.top, destRect.right - destRect.left, destRect.bottom - destRect.top, srcRect.left, GetHeight() - srcRect.top, srcRect.right - srcRect.left, srcRect.top - srcRect.bottom, prcWBounds);
}

inline HRESULT CPicture::Draw(HDC hDC, LONG x, LONG y, LONG lWidth, LONG lHeight, OLE_XPOS_HIMETRIC srcX, OLE_YPOS_HIMETRIC srcY, OLE_XSIZE_HIMETRIC srcWidth, OLE_YSIZE_HIMETRIC srcHeight, LPCRECT prcWBounds/* = NULL*/) const
{
    ATLASSERT(hDC);
    ATLASSERT(m_spPicture != NULL);

    return m_spPicture->Render(hDC, x, y, lWidth, lHeight, srcX, GetHeight() - srcY, srcWidth, -srcHeight, prcWBounds);
}

inline BOOL CPicture::IsEmpty() const
{
    return (m_spPicture == NULL);
}

inline SHORT CPicture::GetType() const
{
    ATLASSERT(m_spPicture != NULL);

    SHORT _result = PICTYPE_NONE;
    _COM_VERIFY(m_spPicture->get_Type(&_result));
    return _result;
}

inline DWORD CPicture::GetAttributes() const
{
    ATLASSERT(m_spPicture != NULL);

    DWORD _result = 0;
    _COM_VERIFY(m_spPicture->get_Attributes(&_result));
    return _result;
}

inline HDC CPicture::GetCurDC() const
{
    ATLASSERT(m_spPicture != NULL);

    HDC _result = NULL;
    _COM_VERIFY(m_spPicture->get_CurDC(&_result));
    return _result;
}

inline OLE_HANDLE CPicture::GetHandle() const
{
    ATLASSERT(m_spPicture != NULL);

    OLE_HANDLE _result = 0;
    _COM_VERIFY(m_spPicture->get_Handle(&_result));
    return _result;
}

inline OLE_HANDLE CPicture::GetPal() const
{
    ATLASSERT(m_spPicture != NULL);

    OLE_HANDLE _result = 0;
    _COM_VERIFY(m_spPicture->get_hPal(&_result));
    return _result;
}

inline HRESULT CPicture::SetPal(OLE_HANDLE hPal)
{
    ATLASSERT(m_spPicture != NULL);
    return m_spPicture->set_hPal(hPal);
}

inline OLE_XSIZE_HIMETRIC CPicture::GetWidth() const
{
    ATLASSERT(m_spPicture != NULL);

    OLE_XSIZE_HIMETRIC _result = 0;
    _COM_VERIFY(m_spPicture->get_Width(&_result));
    return _result;
}

inline OLE_YSIZE_HIMETRIC CPicture::GetHeight() const
{
    ATLASSERT(m_spPicture != NULL);

    OLE_YSIZE_HIMETRIC _result = 0;
    _COM_VERIFY(m_spPicture->get_Height(&_result));
    return _result;
}

inline BOOL CPicture::GetKeepOriginalFormat() const
{
    ATLASSERT(m_spPicture != NULL);

    BOOL _result = FALSE;
    _COM_VERIFY(m_spPicture->get_KeepOriginalFormat(&_result));
    return _result;
}

inline HRESULT CPicture::SetKeepOriginalFormat(BOOL bKeep)
{
    ATLASSERT(m_spPicture != NULL);
    return m_spPicture->put_KeepOriginalFormat(bKeep);
}

inline CPicture& CPicture::operator=(IPicture* pPicture)
{
    m_spPicture = pPicture;
    return *this;
}

inline CPicture& CPicture::operator=(const CPicture& that)
{
    m_spPicture = that.m_spPicture;
    return *this;
}

}  // namespace stdutil

#endif  // __PICTURE_INL__