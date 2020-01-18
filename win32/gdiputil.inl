///////////////////////////////////////////////////////////////////////////////
// gdiputil.inl
//
// Author : Garfield
// Creation Date : 2007/9/20

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __GDIPUTIL_INL__
#define __GDIPUTIL_INL__

#ifndef __GDIPUTIL_H__
    #error gdiputil.inl requires gdiputil.h to be included first
#endif

namespace stdutil {

///////////////////////////////////////////////////////////////////////////////
// Implementation of the CGdiplusModule class
//

inline CGdiplusModule::CGdiplusModule()
    : m_ulToken(0)
{
}

inline CGdiplusModule::~CGdiplusModule()
{
    Shutdown();
}

inline BOOL CGdiplusModule::Startup(const Gdiplus::GdiplusStartupInput& input/* = Gdiplus::GdiplusStartupInput()*/, Gdiplus::GdiplusStartupOutput* output/* = NULL*/)
{
    ATLASSERT(m_ulToken == 0);
    return (Gdiplus::GdiplusStartup(&m_ulToken, &input, output) == Gdiplus::Ok);
}

inline void CGdiplusModule::Shutdown()
{
    if (m_ulToken != 0)
    {
        Gdiplus::GdiplusShutdown(m_ulToken);
        m_ulToken = 0;
    }
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CColorPalette class
//

inline CColorPalette::CColorPalette()
{
}

inline BOOL CColorPalette::Load(Gdiplus::Image* pImage)
{
    ATLASSERT(pImage);

    INT cbSize = pImage->GetPaletteSize();
    BOOL bSuccessful = (cbSize > sizeof(Gdiplus::ColorPalette) && m_buffer.allocateBytes(cbSize) != NULL && pImage->GetPalette(m_buffer.data, cbSize) == Gdiplus::Ok);
    if (!bSuccessful)
        m_buffer.clear();

    return bSuccessful;
}

#ifdef _CRT_DEBUG_DUMP
template <typename _Pr>
inline void CColorPalette::Dump(const _Pr& _Printer) const
{
    _Printer(_T("\n[ COLOR PALETTE FLAGS = 0x%08lX, COUNT = %u ]\n[\n"), GetFlags(), GetCount());
    for (UINT i = 0; i < GetCount(); ++i)
        _Printer(_T("\tINDEX = %03u, COLOR(ARGB) = (%3u, %3u, %3u, %3u)\n"), i + 1, (BYTE)(m_buffer.data->Entries[i] >> 24), (BYTE)(m_buffer.data->Entries[i] >> 16),
                 (BYTE)(m_buffer.data->Entries[i] >> 8), (BYTE)m_buffer.data->Entries[i]);

    _Printer(_T("]\n"));
}
#endif  // _CRT_DEBUG_DUMP

inline UINT CColorPalette::GetFlags() const
{
    ATLASSERT(!IsEmpty());
    return m_buffer.data->Flags;
}

inline UINT CColorPalette::GetCount() const
{
    ATLASSERT(!IsEmpty());
    return m_buffer.data->Count;
}

inline Gdiplus::ARGB CColorPalette::GetItem(UINT nIndex) const
{
    ATLASSERT(!IsEmpty());
    ATLASSERT(nIndex < m_buffer.data->Count);

    return m_buffer.data->Entries[nIndex];
}

inline void CColorPalette::SetItem(UINT nIndex, Gdiplus::ARGB color)
{
    ATLASSERT(!IsEmpty());
    ATLASSERT(nIndex < m_buffer.data->Count);

    m_buffer.data->Entries[nIndex] = color;
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CPropertyItem class
//

inline CPropertyItem::CPropertyItem()
{
}

inline BOOL CPropertyItem::Load(Gdiplus::Image* pImage, PROPID propid)
{
    ATLASSERT(pImage);

    UINT cbSize = pImage->GetPropertyItemSize(propid);
    BOOL bSuccessful = (cbSize != 0 && m_buffer.allocateBytes(cbSize) != NULL && pImage->GetPropertyItem(propid, cbSize, m_buffer.data) == Gdiplus::Ok);
    if (!bSuccessful)
        m_buffer.clear();

    return bSuccessful;
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CPropertyItems class
//

inline CPropertyItems::CPropertyItems()
    : m_nCount(0)
{
}

inline BOOL CPropertyItems::Load(Gdiplus::Image* pImage)
{
    ATLASSERT(pImage);

    UINT cbSize = 0;
    _COND_VERIFY(pImage->GetPropertySize(&cbSize, &m_nCount), Gdiplus::Ok);

    BOOL bSuccessful = (cbSize != 0 && m_buffer.allocateBytes(cbSize) != NULL && pImage->GetAllPropertyItems(cbSize, m_nCount, m_buffer.data) == Gdiplus::Ok);
    if (!bSuccessful)
        m_buffer.clear();

    return bSuccessful;
}

inline LPCVOID CPropertyItems::Find(PROPID propid, DWORD dwFlags/* = PROPERTY_ITEM_INFO*/) const
{
    ATLASSERT(!IsEmpty());

    for (UINT i = 0; i < m_nCount; ++i)
    {
        if (m_buffer.data[i].id == propid)
            return ((LPBYTE)(m_buffer.data + i) + dwFlags);
    }

    return NULL;
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CImageCodecInfo class
//

inline CImageCodecInfo::CImageCodecInfo()
    : m_nCount(0)
{
}

inline BOOL CImageCodecInfo::LoadDecoders()
{
    UINT cbSize = 0;
    BOOL bSuccessful = (Gdiplus::GetImageDecodersSize(&m_nCount, &cbSize) == Gdiplus::Ok && m_buffer.allocateBytes(cbSize) != NULL && Gdiplus::GetImageDecoders(m_nCount, cbSize, m_buffer.data) == Gdiplus::Ok);
    if (!bSuccessful)
        m_buffer.clear();

    return bSuccessful;
}

inline BOOL CImageCodecInfo::LoadEncoders()
{
    UINT cbSize = 0;
    BOOL bSuccessful = (Gdiplus::GetImageEncodersSize(&m_nCount, &cbSize) == Gdiplus::Ok && m_buffer.allocateBytes(cbSize) != NULL && Gdiplus::GetImageEncoders(m_nCount, cbSize, m_buffer.data) == Gdiplus::Ok);
    if (!bSuccessful)
        m_buffer.clear();

    return bSuccessful;
}

inline LPCVOID CImageCodecInfo::Find(const GUID& formatID, DWORD dwFlags/* = IMAGE_CODEC_CLSID*/) const
{
    ATLASSERT(!IsEmpty());
    ATLASSERT(m_nCount > 0);

    for (UINT i = 0; i < m_nCount; ++i)
    {
        if (m_buffer.data[i].FormatID == formatID)
            return ((LPBYTE)(m_buffer.data + i) + dwFlags);
    }

    return NULL;
}

inline LPCVOID CImageCodecInfo::Find(LPCTSTR pszExtOrMimeType, DWORD dwFlags/* = IMAGE_CODEC_CLSID*/) const
{
    ATLASSERT(!IsEmpty());
    ATLASSERT(m_nCount > 0);

    if (!stdutil::StringIsEmpty(pszExtOrMimeType))
    {
        for (UINT i = 0; i < m_nCount; ++i)
        {
            if (::StrStrI(m_buffer.data[i].FilenameExtension, pszExtOrMimeType) != NULL || ::StrCmpI(m_buffer.data[i].MimeType, pszExtOrMimeType) == 0)
                return ((LPBYTE)(m_buffer.data + i) + dwFlags);
        }
    }

    return NULL;
}

#if defined(__ATLSTR_H__) || defined(__AFXSTR_H__)
inline CString CImageCodecInfo::GetFilterString(LPCTSTR pszDescription/* = NULL*/, TCHAR chSeparator/* = _T('|')*/) const
{
    ATLASSERT(!IsEmpty());
    ATLASSERT(m_nCount > 0);

    CString strFilter, strDescription;
    if (pszDescription != NULL)
    {
        CString strExtension;
        for (UINT i = 0; i < m_nCount; ++i)
        {
            AppendFilter(strDescription, m_buffer.data[i].FormatDescription, m_buffer.data[i].FilenameExtension);
            strExtension += m_buffer.data[i].FilenameExtension;
            strExtension += _T(';');
        }

        strExtension.Truncate(strExtension.GetLength() - 1); // Delete the last ";"
        AppendFilter(strFilter, CString(pszDescription), strExtension);
    }
    else
    {
        for (UINT i = 0; i < m_nCount; ++i)
            AppendFilter(strDescription, m_buffer.data[i].FormatDescription, m_buffer.data[i].FilenameExtension);
    }

    strFilter += strDescription;
    strFilter += chSeparator;
    strFilter.Replace(_T('|'), chSeparator);

    return strFilter;
}
#endif  // defined(__ATLSTR_H__) || defined(__AFXSTR_H__)

#ifdef _CRT_DEBUG_DUMP
template <typename _Pr>
inline void CImageCodecInfo::Dump(const _Pr& _Printer) const
{
    OLECHAR szCodeClsID[64], szFormatID[64];
    _Printer(_T("\n[ IMAGE CODEC COUNT = %u ]\n"), m_nCount);

    for (UINT i = 0; i < m_nCount; ++i)
    {
        ATLVERIFY(::StringFromGUID2(m_buffer.data[i].Clsid, szCodeClsID, _countof(szCodeClsID)));
        ATLVERIFY(::StringFromGUID2(m_buffer.data[i].FormatID, szFormatID, _countof(szFormatID)));

        _Printer(_T("[\n\tCodec Name\t\t\t= %s\n\tDll Name\t\t\t= %s\n\tCodec Classs ID\t\t= %s\n\tFormat ID\t\t\t= %s\n\tFilename Extension\t= %s\n\t\
                    Format Description\t= %s\n\tMimeType\t\t\t= %s\n\tFlags\t\t\t\t= %08lx\n\tVersion\t\t\t\t= %u\n\tSignature Count\t\t= %u\n\tSignature Size\t\t= %u\n"),
                    m_buffer.data[i].CodecName, m_buffer.data[i].DllName, szCodeClsID, szFormatID, m_buffer.data[i].FilenameExtension, m_buffer.data[i].FormatDescription,
                    m_buffer.data[i].MimeType, m_buffer.data[i].Flags, m_buffer.data[i].Version, m_buffer.data[i].SigCount, m_buffer.data[i].SigSize);

        DumpSig(_Printer, _T("\tSignature Patterns\t= { "), m_buffer.data[i]);
        DumpSig(_Printer, _T("\tSignature Masks\t\t= { "), m_buffer.data[i]);
        _Printer(_T("]\n"));
    }

    _Printer(_T("\n"));
}
#endif  // _CRT_DEBUG_DUMP

#if defined(__ATLSTR_H__) || defined(__AFXSTR_H__)
inline void WINAPI CImageCodecInfo::AppendFilter(CString& strFilter, LPCTSTR pszDescription, LPCTSTR pszExtension)
{
    ATLASSERT(pszExtension);
    ATLASSERT(pszDescription);

    strFilter += pszDescription;
    strFilter += _T(" (");
    strFilter += pszExtension;
    strFilter += _T(")|");
    strFilter += pszExtension;
    strFilter += _T('|');
}
#endif  // defined(__ATLSTR_H__) || defined(__AFXSTR_H__)

#ifdef _CRT_DEBUG_DUMP
template <typename _Pr>
inline void CImageCodecInfo::DumpSig(const _Pr& _Printer, LPCTSTR pszPrefix, const Gdiplus::ImageCodecInfo& codecInfo) const
{
    _Printer(_T("%s"), pszPrefix);
    BOOL bPatterns = (::_tcsstr(pszPrefix, _T("Patterns")) != NULL);

    for (DWORD i = 0; i < codecInfo.SigSize * codecInfo.SigCount; ++i)
        _Printer(_T("%02x "), (bPatterns ? codecInfo.SigPattern[i] : codecInfo.SigMask[i]));
    _Printer(_T("}\n"));
}
#endif  // _CRT_DEBUG_DUMP


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CAnimateProperty class
//

inline CAnimateProperty::CAnimateProperty()
{
}

inline BOOL CAnimateProperty::Load(Gdiplus::Image* pImage)
{
    ATLASSERT(pImage);

    UINT cbSize = pImage->GetPropertyItemSize(PropertyTagFrameDelay);
    BOOL bSuccessful = (cbSize != 0 && m_buffer.allocateBytes(cbSize) != NULL && pImage->GetPropertyItem(PropertyTagFrameDelay, cbSize, m_buffer.data) == Gdiplus::Ok);
    if (bSuccessful)
    {
        ATLASSERT(m_buffer.data->length % sizeof(UINT) == 0);
        ATLASSERT(m_buffer.data->type == PropertyTagTypeLong);

        m_buffer.data->length /= sizeof(UINT);
        for (UINT i = 0; i < m_buffer.data->length; ++i)
            ((LPUINT)m_buffer.data->value)[i] *= 10;

        ATLASSERT(pImage->GetFrameCount(&Gdiplus::FrameDimensionTime) == GetFrameCount());
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
            _Printer(_T("\tFRAME = %02u, DELAY TIME = %u ms\n"), i + 1, GetFrameDelay(i));
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
// Implementation of the CFrameDimensions class
//

inline CFrameDimensions::CFrameDimensions()
    : m_nCount(0)
{
}

inline BOOL CFrameDimensions::Load(Gdiplus::Image* pImage)
{
    ATLASSERT(pImage);

    BOOL bSuccessful = ((m_nCount = pImage->GetFrameDimensionsCount()) != 0 && m_buffer.allocateBytes(m_nCount * sizeof(GUID)) != NULL && pImage->GetFrameDimensionsList(m_buffer.data, m_nCount) == Gdiplus::Ok);
    if (!bSuccessful)
        m_buffer.clear();

    return bSuccessful;
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CEncoderParameters class
//

inline CEncoderParameters::CEncoderParameters()
{
}

inline BOOL CEncoderParameters::Load(Gdiplus::Image* pImage, const CLSID* pclsIdEncoder)
{
    ATLASSERT(pImage);
    ATLASSERT(pclsIdEncoder);

    UINT cbSize = pImage->GetEncoderParameterListSize(pclsIdEncoder);
    BOOL bSuccessful = (cbSize != 0 && m_buffer.allocateBytes(cbSize) != NULL && pImage->GetEncoderParameterList(pclsIdEncoder, cbSize, m_buffer.data) == Gdiplus::Ok);
    if (!bSuccessful)
        m_buffer.clear();

    return bSuccessful;
}

inline LPCVOID CEncoderParameters::Find(const GUID& ParamterID, DWORD dwFlags/* = ENCODER_PARAMETER_INFO*/) const
{
    ATLASSERT(!IsEmpty());

    for (UINT i = 0; i < m_buffer.data->Count; ++i)
    {
        if (m_buffer.data->Parameter[i].Guid == ParamterID)
            return ((LPBYTE)(m_buffer.data->Parameter + i) + dwFlags);
    }

    return NULL;
}

inline CEncoderParameters::operator Gdiplus::EncoderParameters*()
{
    return m_buffer.data;
}

inline CEncoderParameters::operator const Gdiplus::EncoderParameters*() const
{
    return m_buffer.data;
}

inline UINT CEncoderParameters::GetCount() const
{
    ATLASSERT(!IsEmpty());
    return m_buffer.data->Count;
}

inline const Gdiplus::EncoderParameter& CEncoderParameters::GetItem(UINT nIndex) const
{
    ATLASSERT(!IsEmpty());
    ATLASSERT(nIndex < m_buffer.data->Count);

    return m_buffer.data->Parameter[nIndex];
}

}  // namespace stdutil

#endif  // __GDIPUTIL_INL__