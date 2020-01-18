///////////////////////////////////////////////////////////////////////////////
// gdiputil.h
//
// Author : Garfield
// Creation Date : 2007/9/20
//
// Summary:
// Flags                                Return Value
// ------------------------------------------------------------
// IMAGE_CODEC_INFO                     const ImageCodecInfo*
// IMAGE_CODEC_CLSID                    const CLSID*
// IMAGE_CODEC_FORMATID                 const GUID*
// IMAGE_CODEC_CODECNAME                LPCWSTR*
// IMAGE_CODEC_DLLNAME                  LPCWSTR*
// IMAGE_CODEC_FORMAT_DESCRIPTION       LPCWSTR*
// IMAGE_CODEC_FILENAME_EXTENSION       LPCWSTR*
// IMAGE_CODEC_MIMETYPE                 LPCWSTR*
// IMAGE_CODEC_FLAGS                    const DWORD*
// IMAGE_CODEC_VERSION                  const DWORD*
// IMAGE_CODEC_SIGCOUNT                 const DWORD*
// IMAGE_CODEC_SIGSIZE                  const DWORD*
// IMAGE_CODEC_SIGPATTERN               const BYTE**
// IMAGE_CODEC_SIGMASK                  const BYTE**
//
// Flags                                Return Value
// ------------------------------------------------------------
// PROPERTY_ITEM_INFO                   const PropertyItem*
// PROPERTY_ITEM_ID                     const PROPID*
// PROPERTY_ITEM_LENGTH                 const ULONG*
// PROPERTY_ITEM_TYPE                   const WORD*
// PROPERTY_ITEM_VALUE                  const VOID**
//
// Flags                                Return Value
// ------------------------------------------------------------
// ENCODER_PARAMETER_INFO               const EncoderParameter*
// ENCODER_PARAMETER_GUID               const GUID*
// ENCODER_PARAMETER_NUMBEROFVALUES     const ULONG*
// ENCODER_PARAMETER_TYPE               const ULONG*
// ENCODER_PARAMETER_VALUE              const VOID**

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __GDIPUTIL_H__
#define __GDIPUTIL_H__

#ifdef _WIN32_WCE
    #error gdiputil.h compiles under Windows only
#endif

#include "stdutil.h"
#include <shlwapi.h>

#pragma push_macro("new")
#undef new
#include <gdiplus.h>
#pragma pop_macro("new")

///////////////////////////////////////////////////////////////////////////////
// Classes in this file:
//
// CGdiplusModule
// CColorPalette
// CPropertyItem
// CPropertyItems
// CImageCodecInfo
// CAnimateProperty
// CFrameDimensions
// CEncoderParameters

namespace stdutil {

///////////////////////////////////////////////////////////////////////////////
// Interface of the CGdiplusModule class
//

class CGdiplusModule
{
// Constructors/Destructor
public:
    CGdiplusModule();
    ~CGdiplusModule();

// Operations
public:
    BOOL Startup(const Gdiplus::GdiplusStartupInput& input = Gdiplus::GdiplusStartupInput(), Gdiplus::GdiplusStartupOutput* output = NULL);
    void Shutdown();

// Implementation
private:
    CGdiplusModule(const CGdiplusModule& that);
    CGdiplusModule& operator=(const CGdiplusModule& that);

// Data members
private:
    ULONG_PTR m_ulToken;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CColorPalette class
//

class CColorPalette : public atlutil::CHeapBufferImpl<Gdiplus::ColorPalette>
{
// Constructors
public:
    CColorPalette();

// Operations
public:
    BOOL Load(Gdiplus::Image* pImage);

#ifdef _CRT_DEBUG_DUMP
    template <typename _Pr> void Dump(const _Pr& _Printer) const;
#endif

// Attributes
public:
    UINT GetFlags() const;
    UINT GetCount() const;

    Gdiplus::ARGB GetItem(UINT nIndex) const;
    void SetItem(UINT nIndex, Gdiplus::ARGB color);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CPropertyItem class
//

class CPropertyItem : public atlutil::CHeapBufferImpl<Gdiplus::PropertyItem>
{
// Constructors
public:
    CPropertyItem();

// Operations
public:
    BOOL Load(Gdiplus::Image* pImage, PROPID propid);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CPropertyItems class
//

class CPropertyItems : public atlutil::CHeapBufferImpl<Gdiplus::PropertyItem>
{
// Constructors
public:
    CPropertyItems();

// Operations
public:
    BOOL Load(Gdiplus::Image* pImage);
    LPCVOID Find(PROPID propid, DWORD dwFlags = PROPERTY_ITEM_INFO) const;

// Data members
public:
    UINT m_nCount;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CImageCodecInfo class
//

class CImageCodecInfo : public atlutil::CHeapBufferImpl<Gdiplus::ImageCodecInfo>
{
// Constructors
public:
    CImageCodecInfo();

// Operations
public:
    BOOL LoadDecoders();
    BOOL LoadEncoders();

    LPCVOID Find(const GUID& formatID, DWORD dwFlags = IMAGE_CODEC_CLSID) const;
    LPCVOID Find(LPCTSTR pszExtOrMimeType, DWORD dwFlags = IMAGE_CODEC_CLSID) const;

#if defined(__ATLSTR_H__) || defined(__AFXSTR_H__)
    // The pszDescription parameter can pass pointer to a null-terminated string, or resource ID of the string.
    CString GetFilterString(LPCTSTR pszDescription = NULL, TCHAR chSeparator = _T('|')) const;
#endif

#ifdef _CRT_DEBUG_DUMP
    template <typename _Pr> void Dump(const _Pr& _Printer) const;
#endif

// Implementation
protected:
#if defined(__ATLSTR_H__) || defined(__AFXSTR_H__)
    static void WINAPI AppendFilter(CString& strFilter, LPCTSTR pszDescription, LPCTSTR pszExtension);
#endif

#ifdef _CRT_DEBUG_DUMP
    template <typename _Pr> void DumpSig(const _Pr& _Printer, LPCTSTR pszPrefix, const Gdiplus::ImageCodecInfo& codecInfo) const;
#endif

// Data members
public:
    UINT m_nCount;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CAnimateProperty class
//

class CAnimateProperty : public atlutil::CHeapBufferImpl<Gdiplus::PropertyItem>
{
// Constructors
public:
    CAnimateProperty();

// Operations
public:
    BOOL Load(Gdiplus::Image* pImage);

#ifdef _CRT_DEBUG_DUMP
    template <typename _Pr> void Dump(const _Pr& _Printer) const;
#endif

// Attributes
public:
    UINT GetFrameCount() const;
    UINT GetFrameDelay(UINT nFrameIndex) const;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CFrameDimensions class
//

class CFrameDimensions : public atlutil::CHeapBufferImpl<GUID>
{
// Constructors
public:
    CFrameDimensions();

// Operations
public:
    BOOL Load(Gdiplus::Image* pImage);

// Data members
public:
    UINT m_nCount;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CEncoderParameters class
//

class CEncoderParameters : public atlutil::CHeapBufferImpl<Gdiplus::EncoderParameters>
{
// Constructors
public:
    CEncoderParameters();

// Operations
public:
    BOOL Load(Gdiplus::Image* pImage, const CLSID* pclsIdEncoder);
    LPCVOID Find(const GUID& ParamterID, DWORD dwFlags = ENCODER_PARAMETER_INFO) const;

    operator Gdiplus::EncoderParameters*();
    operator const Gdiplus::EncoderParameters*() const;

// Attributes
public:
    UINT GetCount() const;
    const Gdiplus::EncoderParameter& GetItem(UINT nIndex) const;
};

}  // namespace stdutil

#include "gdiputil.inl"

#endif  // __GDIPUTIL_H__