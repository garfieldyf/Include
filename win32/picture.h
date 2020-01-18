///////////////////////////////////////////////////////////////////////////////
// picture.h
//
// Author : Garfield
// Creation Date : 2007/10/20

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __PICTURE_H__
#define __PICTURE_H__

#ifdef _WIN32_WCE
    #error picture.h compiles under Windows only
#endif

#include <ocidl.h>
#include "atlcomutil.h"

///////////////////////////////////////////////////////////////////////////////
// Classes in this file:
//
// CPicture

namespace stdutil {

///////////////////////////////////////////////////////////////////////////////
// Interface of the CPicture class
//

class CPicture
{
// Constructors
public:
    CPicture();
    CPicture(const CPicture& that);
    explicit CPicture(IPicture* pPicture);

// Operations
public:
    HRESULT Load(LPCTSTR pszURLorPath);
    HRESULT Load(IStream* pStream, BOOL bRunmode = FALSE);
    HRESULT Load(HINSTANCE hInstance, UINT nIDResource, LPCTSTR pType, BOOL bRunmode = FALSE);

    HRESULT Save(LPCTSTR pszFileName) const;
    HRESULT Save(IStream* pStream, BOOL bSaveMemCopy = FALSE, LONG* pcbSize = NULL) const;

#if defined(__ATLSAFE_H__) && defined(__ATLCOMUTIL_H__)
    HRESULT Save(ATL::CComSafeArray<BYTE>& saPicture) const;
    HRESULT Load(const ATL::CComSafeArray<BYTE>& saPicture, BOOL bRunmode = FALSE);
#endif

    HRESULT PictureChanged();
    HRESULT SelectPicture(HDC hDCIn, HDC* phDCOut = NULL, OLE_HANDLE* phBmpOut = NULL);

    HRESULT Draw(HDC hDC, const RECT& destRect, LONG srcX, LONG srcY, LONG srcWidth, LONG srcHeight, LPCRECT prcWBounds = NULL) const;
    HRESULT Draw(HDC hDC, LONG x, LONG y, LONG lWidth, LONG lHeight, OLE_XPOS_HIMETRIC srcX, OLE_YPOS_HIMETRIC srcY, OLE_XSIZE_HIMETRIC srcWidth, OLE_YSIZE_HIMETRIC srcHeight, LPCRECT prcWBounds = NULL) const;

// Attributes
public:
    BOOL IsEmpty() const;
    SHORT GetType() const;
    DWORD GetAttributes() const;

    HDC GetCurDC() const;
    OLE_HANDLE GetHandle() const;

    OLE_HANDLE GetPal() const;
    HRESULT SetPal(OLE_HANDLE hPal);

    OLE_XSIZE_HIMETRIC GetWidth() const;
    OLE_YSIZE_HIMETRIC GetHeight() const;

    BOOL GetKeepOriginalFormat() const;
    HRESULT SetKeepOriginalFormat(BOOL bKeep);

// Implementation
public:
    CPicture& operator=(IPicture* pPicture);
    CPicture& operator=(const CPicture& that);

// Data members
public:
    ATL::CComPtr<IPicture> m_spPicture;
};

}  // namespace stdutil

#include "picture.inl"

#endif  // __PICTURE_H__