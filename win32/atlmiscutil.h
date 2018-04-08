///////////////////////////////////////////////////////////////////////////////
// atlmiscutil.h
//
// Copyright(c) 2018, Garfield. All rights reserved.
// Author  : Garfield
// Version : 1.0
// Creation Date : 2007/12/21

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __ATLMISCUTIL_H__
#define __ATLMISCUTIL_H__

#include "stddefx.h"
#include "memutil.h"

///////////////////////////////////////////////////////////////////////////////
// Classes in this file:
//
// CHandleTraits
// CFileHandleTraits
// CHandle<_Traits>
// CBitmapInfo
// CBitmapDC
// CMemoryDC
// CMemoryDCPool
// CHeapBufferImpl<_Ty>

namespace atlutil {

///////////////////////////////////////////////////////////////////////////////
// Interface of the CHandleTraits class
//

struct CHandleTraits
{
    typedef HANDLE HANDLET_PTR;
    enum { INVALID_HANDLE = NULL };
    static BOOL WINAPI Destroy(HANDLET_PTR hHandle);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CFileHandleTraits class
//

struct CFileHandleTraits : public CHandleTraits
{
    enum { INVALID_HANDLE = (int)INVALID_HANDLE_VALUE };
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CHandle class
//

template <typename _Traits = CHandleTraits>
class ATL_NO_VTABLE CHandle
{
public:
    typedef typename _Traits::HANDLET_PTR HANDLET_PTR;

// Constructors/Destructor
protected:
    CHandle(HANDLET_PTR hHandle = (HANDLET_PTR)_Traits::INVALID_HANDLE);
    ~CHandle();

// Operations
public:
    void Destroy();
    operator HANDLET_PTR() const;

    HANDLET_PTR Detach();
    void Attach(HANDLET_PTR hHandle);

// Attributes
public:
    BOOL IsEmpty() const;

// Implementation
private:
    CHandle(const CHandle& that);
    CHandle& operator=(const CHandle& that);

// Data members
public:
    HANDLET_PTR m_hHandle;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CBitmapInfo class
//

class CBitmapInfo : public tagBITMAP
{
// Constructors/Destructor
public:
    CBitmapInfo();
    ~CBitmapInfo();

// Operations
public:
    LPVOID GetBits(size_t cbSize);
    void FreeBits();

// Attributes
public:
    size_t GetBitsSize() const;

// Implementation
private:
    CBitmapInfo(const CBitmapInfo& that);
    CBitmapInfo& operator=(const CBitmapInfo& that);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CBitmapDC class
//

#ifndef __ATLGDI_H__
class CBitmapDC
#else
class CBitmapDC : public WTL::CDCHandle
#endif
{
// Constructors/Destructor
public:
    explicit CBitmapDC(HBITMAP hBitmap);
    ~CBitmapDC();

// Operations
public:
#ifndef __ATLGDI_H__
    operator HDC() const;
#endif

// Implementation
private:
    CBitmapDC(const CBitmapDC& that);
    CBitmapDC& operator=(const CBitmapDC& that);

// Data members
public:
#ifndef __ATLGDI_H__
    HDC m_hDC;
#endif

    HGDIOBJ m_hBitmap;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CMemoryDC class
//

#ifndef __ATLGDI_H__
class CMemoryDC
#else
class CMemoryDC : public WTL::CDCHandle
#endif
{
// Constructors/Destructor
public:
    CMemoryDC(HDC hDC, const RECT& rcPaint);
    ~CMemoryDC();

// Operations
public:
#ifndef __ATLGDI_H__
    operator HDC() const;
#endif

// Implementation
private:
    CMemoryDC(const CMemoryDC& that);
    CMemoryDC& operator=(const CMemoryDC& that);

// Data members
public:
#ifndef __ATLGDI_H__
    HDC m_hDC;
#endif

    HDC m_hOriginalDC;
    HBITMAP m_hBitmap;
    const RECT& m_rcPaint;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CMemoryDCPool class
//

class CMemoryDCPool
{
// Constructors/Destructor
private:
    CMemoryDCPool();
    ~CMemoryDCPool();

// Operations
public:
    static HDC WINAPI GetDC();
    static void WINAPI ReleaseDC(HDC hDC);

// Data members
private:
    HDC m_hMemDCs[4];
    static CMemoryDCPool m_dcPool;

// Implementation
private:
    HDC InternalGetDC();
    void InternalReleaseDC(HDC hDC);

    CMemoryDCPool(const CMemoryDCPool& that);
    CMemoryDCPool& operator=(const CMemoryDCPool& that);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CHeapBufferImpl class
//

template <typename _Ty>
class ATL_NO_VTABLE CHeapBufferImpl
{
// Constructors
protected:
    CHeapBufferImpl();

// Operations
public:
    void Destroy();

// Attributes
public:
    BOOL IsEmpty() const;
    size_t GetCapacity() const;

// Implementation
private:
    CHeapBufferImpl(const CHeapBufferImpl& that);
    CHeapBufferImpl& operator=(const CHeapBufferImpl& that);

// Data members
protected:
    stdutil::heap_buffer<_Ty> m_buffer;
};

}  // namespace atlutil

#include "atlmiscutil.inl"

#endif  // __ATLMISCUTIL_H__