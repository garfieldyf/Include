///////////////////////////////////////////////////////////////////////////////
// pimutil.inl
//
// Copyright(c) 2019, Garfield. All rights reserved.
// Author  : Garfield
// Version : 1.0
// Creation Date : 2010/4/22

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __PIMUTIL_INL__
#define __PIMUTIL_INL__

#ifndef __PIMUTIL_H__
    #error pimutil.inl requires pimutil.h to be included first
#endif

namespace WCE {

///////////////////////////////////////////////////////////////////////////////
// Implementation of the CCePropValTraits class
//

inline BOOL WINAPI CCePropValTraits::Destroy(HANDLET_PTR hHandle)
{
    return ::HeapFree(::GetProcessHeap(), 0, hHandle);
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CCePropVal class
//

inline CCePropVal::CCePropVal()
{
}

inline CEPROPVAL** CCePropVal::GetAddress()
{
    Destroy();
    return &m_hHandle;
}

inline WORD CCePropVal::GetFlags(DWORD dwIndex/* = 0*/) const
{
    ATLASSERT(m_hHandle);
    return m_hHandle[dwIndex].wFlags;
}

inline BOOL CCePropVal::IsValidProp(DWORD dwIndex/* = 0*/) const
{
    ATLASSERT(m_hHandle);
    return (m_hHandle[dwIndex].wFlags != CEDB_PROPNOTFOUND);
}

inline DWORD CCePropVal::GetSize(DWORD dwIndex/* = 0*/) const
{
    ATLASSERT(m_hHandle);
    ATLASSERT(m_hHandle[dwIndex].wFlags != CEDB_PROPNOTFOUND && LOWORD(m_hHandle[dwIndex].propid) == CEVT_BLOB);

    return m_hHandle[dwIndex].val.blob.dwCount;
}

inline LPBYTE CCePropVal::GetBlob(DWORD dwIndex/* = 0*/) const
{
    ATLASSERT(m_hHandle);
    ATLASSERT(m_hHandle[dwIndex].wFlags != CEDB_PROPNOTFOUND && LOWORD(m_hHandle[dwIndex].propid) == CEVT_BLOB);

    return m_hHandle[dwIndex].val.blob.lpb;
}

inline BOOL CCePropVal::GetBool(DWORD dwIndex/* = 0*/) const
{
    ATLASSERT(m_hHandle);
    ATLASSERT(m_hHandle[dwIndex].wFlags != CEDB_PROPNOTFOUND && LOWORD(m_hHandle[dwIndex].propid) == CEVT_BOOL);

    return m_hHandle[dwIndex].val.boolVal;
}

inline long CCePropVal::GetLong(DWORD dwIndex/* = 0*/) const
{
    ATLASSERT(m_hHandle);
    ATLASSERT(m_hHandle[dwIndex].wFlags != CEDB_PROPNOTFOUND);
    ATLASSERT(LOWORD(m_hHandle[dwIndex].propid) == CEVT_I4 || LOWORD(m_hHandle[dwIndex].propid) == CEVT_UI4 || LOWORD(m_hHandle[dwIndex].propid) == CEVT_PIM_AUTO_I4);

    return m_hHandle[dwIndex].val.lVal;
}

inline short CCePropVal::GetShort(DWORD dwIndex/* = 0*/) const
{
    ATLASSERT(m_hHandle);
    ATLASSERT(m_hHandle[dwIndex].wFlags != CEDB_PROPNOTFOUND);
    ATLASSERT(LOWORD(m_hHandle[dwIndex].propid) == CEVT_I2 || LOWORD(m_hHandle[dwIndex].propid) == CEVT_UI2);

    return m_hHandle[dwIndex].val.iVal;
}

inline double CCePropVal::GetDouble(DWORD dwIndex/* = 0*/) const
{
    ATLASSERT(m_hHandle);
    ATLASSERT(m_hHandle[dwIndex].wFlags != CEDB_PROPNOTFOUND && LOWORD(m_hHandle[dwIndex].propid) == CEVT_R8);

    return m_hHandle[dwIndex].val.dblVal;
}

inline LPCWSTR CCePropVal::GetString(DWORD dwIndex/* = 0*/) const
{
    ATLASSERT(m_hHandle);
    ATLASSERT(m_hHandle[dwIndex].wFlags != CEDB_PROPNOTFOUND && LOWORD(m_hHandle[dwIndex].propid) == CEVT_LPWSTR);

    return m_hHandle[dwIndex].val.lpwstr;
}

inline const FILETIME& CCePropVal::GetDateTime(DWORD dwIndex/* = 0*/) const
{
    ATLASSERT(m_hHandle);
    ATLASSERT(m_hHandle[dwIndex].wFlags != CEDB_PROPNOTFOUND && LOWORD(m_hHandle[dwIndex].propid) == CEVT_FILETIME);

    return m_hHandle[dwIndex].val.filetime;
}

inline BOOL CCePropVal::GetDateTime(SYSTEMTIME& date, DWORD dwIndex/* = 0*/) const
{
    ATLASSERT(m_hHandle);
    ATLASSERT(m_hHandle[dwIndex].wFlags != CEDB_PROPNOTFOUND && LOWORD(m_hHandle[dwIndex].propid) == CEVT_FILETIME);

    return ::FileTimeToSystemTime(&m_hHandle[dwIndex].val.filetime, &date);
}

}  // namespace WCE

#endif  // __PIMUTIL_INL__