///////////////////////////////////////////////////////////////////////////////
// atlmiscutil.inl
//
// Author : Garfield
// Creation Date : 2007/12/21

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __ATLMISCUTIL_INL__
#define __ATLMISCUTIL_INL__

#ifndef __ATLMISCUTIL_H__
    #error atlmiscutil.inl requires atlmiscutil.h to be included first
#endif

namespace atlutil {

///////////////////////////////////////////////////////////////////////////////
// Implementation of the CHandleTraits class
//

inline BOOL WINAPI CHandleTraits::Destroy(HANDLET_PTR hHandle)
{
    return ::CloseHandle(hHandle);
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CHandle class
//

template <typename _Traits>
inline CHandle<_Traits>::CHandle(HANDLET_PTR hHandle/* = (HANDLET_PTR)_Traits::INVALID_HANDLE*/)
    : m_hHandle(hHandle)
{
}

template <typename _Traits>
inline CHandle<_Traits>::~CHandle()
{
    Destroy();
}

template <typename _Traits>
inline void CHandle<_Traits>::Destroy()
{
    if (m_hHandle != (HANDLET_PTR)_Traits::INVALID_HANDLE && _Traits::Destroy(m_hHandle))
        m_hHandle = (HANDLET_PTR)_Traits::INVALID_HANDLE;
}

template <typename _Traits>
inline CHandle<_Traits>::operator HANDLET_PTR() const
{
    return m_hHandle;
}

template <typename _Traits>
inline typename CHandle<_Traits>::HANDLET_PTR CHandle<_Traits>::Detach()
{
    HANDLET_PTR hHandle = m_hHandle;
    m_hHandle = (HANDLET_PTR)_Traits::INVALID_HANDLE;

    return hHandle;
}

template <typename _Traits>
inline void CHandle<_Traits>::Attach(HANDLET_PTR hHandle)
{
    ATLASSERT((m_hHandle == (HANDLET_PTR)_Traits::INVALID_HANDLE && hHandle == (HANDLET_PTR)_Traits::INVALID_HANDLE) || m_hHandle != hHandle);

    Destroy();
    m_hHandle = hHandle;
}

template <typename _Traits>
inline BOOL CHandle<_Traits>::IsEmpty() const
{
    return (m_hHandle == (HANDLET_PTR)_Traits::INVALID_HANDLE);
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CBitmapInfo class
//

inline CBitmapInfo::CBitmapInfo()
{
    bmBits = NULL;
}

inline CBitmapInfo::~CBitmapInfo()
{
    ::free(bmBits);
}

inline LPVOID CBitmapInfo::GetBits(size_t cbSize)
{
    ATLASSERT(bmBits == NULL);

    bmBits = ::malloc(cbSize);
    ATLASSERT(bmBits);
    ATLTRACE(_T("bmBits = 0x%p, allocate size = %u\n"), bmBits, cbSize);

    return bmBits;
}

inline void CBitmapInfo::FreeBits()
{
    ::free(bmBits);
    bmBits = NULL;
}

inline size_t CBitmapInfo::GetBitsSize() const
{
    return (bmBits != NULL ? ::_msize(bmBits) : 0);
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CBitmapDC class
//

inline CBitmapDC::CBitmapDC(HBITMAP hBitmap)
#ifndef __ATLGDI_H__
    : m_hDC(CMemoryDCPool::GetDC())
#else
    : CDCHandle(CMemoryDCPool::GetDC())
#endif  // __ATLGDI_H__
{
    ATLASSERT(m_hDC);
    ATLASSERT(hBitmap);

    m_hBitmap = ::SelectObject(m_hDC, hBitmap);
}

inline CBitmapDC::~CBitmapDC()
{
    ::SelectObject(m_hDC, m_hBitmap);
    CMemoryDCPool::ReleaseDC(m_hDC);
}

#ifndef __ATLGDI_H__
inline CBitmapDC::operator HDC() const
{
    return m_hDC;
}
#endif  // __ATLGDI_H__


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CMemoryDC class
//

inline CMemoryDC::CMemoryDC(HDC hDC, const RECT& rcPaint)
#ifndef __ATLGDI_H__
    : m_hDC(CMemoryDCPool::GetDC())
#else
    : CDCHandle(CMemoryDCPool::GetDC())
#endif  // __ATLGDI_H__
    , m_rcPaint(rcPaint), m_hOriginalDC(hDC)
    , m_hBitmap(::CreateCompatibleBitmap(hDC, rcPaint.right - rcPaint.left, rcPaint.bottom - rcPaint.top))
{
    ATLASSERT(hDC);
    ATLASSERT(m_hDC);
    ATLASSERT(m_hBitmap);

    ::SelectObject(m_hDC, m_hBitmap);
    ATLVERIFY(::SetViewportOrgEx(m_hDC, -rcPaint.left, -rcPaint.top, NULL));
}

inline CMemoryDC::~CMemoryDC()
{
    ATLVERIFY(::BitBlt(m_hOriginalDC, m_rcPaint.left, m_rcPaint.top, m_rcPaint.right - m_rcPaint.left, m_rcPaint.bottom - m_rcPaint.top, m_hDC, m_rcPaint.left, m_rcPaint.top, SRCCOPY));
    ATLVERIFY(::DeleteObject(m_hBitmap));

    CMemoryDCPool::ReleaseDC(m_hDC);
}

#ifndef __ATLGDI_H__
inline CMemoryDC::operator HDC() const
{
    return m_hDC;
}
#endif  // __ATLGDI_H__


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CMemoryDCPool class
//

__declspec(selectany) CMemoryDCPool CMemoryDCPool::m_dcPool;

inline CMemoryDCPool::CMemoryDCPool()
{
    ::memset(m_hMemDCs, 0, sizeof(m_hMemDCs));
}

inline CMemoryDCPool::~CMemoryDCPool()
{
    for (size_t i = 0; i < _countof(m_hMemDCs); ++i)
    {
        if (m_hMemDCs[i] != NULL)
        {
            ATLTRACE(_T("CMemoryDCPool destructor - delete memory dc = 0x%p\n"), m_hMemDCs[i]);
            ATLVERIFY(::DeleteDC(m_hMemDCs[i]));
        }
    }
}

inline HDC WINAPI CMemoryDCPool::GetDC()
{
    return m_dcPool.InternalGetDC();
}

inline void WINAPI CMemoryDCPool::ReleaseDC(HDC hDC)
{
    return m_dcPool.InternalReleaseDC(hDC);
}

inline HDC CMemoryDCPool::InternalGetDC()
{
    for (size_t i = 0; i < _countof(m_hMemDCs); ++i)
    {
        if (HDC hDC = (HDC)InterlockedExchangePointer((LPVOID*)&m_hMemDCs[i], NULL))
            return hDC;
    }

    return ::CreateCompatibleDC(NULL);
}

inline void CMemoryDCPool::InternalReleaseDC(HDC hDC)
{
    for (size_t i = 0; i < _countof(m_hMemDCs); ++i)
    {
        HDC hOldDC = (HDC)InterlockedExchangePointer((LPVOID*)&m_hMemDCs[i], hDC);
        if (hOldDC == NULL)
            return;

        hDC = hOldDC;
    }

    ATLTRACE(_T("CMemoryDCPool ReleaseDC - delete memory dc = 0x%p\n"), hDC);
    ATLVERIFY(::DeleteDC(hDC));
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CHandle class
//

template <typename _Ty>
inline CHeapBufferImpl<_Ty>::CHeapBufferImpl()
{
}

template <typename _Ty>
inline void CHeapBufferImpl<_Ty>::Destroy()
{
    m_buffer.clear();
}

template <typename _Ty>
inline BOOL CHeapBufferImpl<_Ty>::IsEmpty() const
{
    return m_buffer.empty();
}

template <typename _Ty>
inline size_t CHeapBufferImpl<_Ty>::GetCapacity() const
{
    return m_buffer.capacity();
}

}  // namespace atlutil

#endif  // __ATLMISCUTIL_INL__