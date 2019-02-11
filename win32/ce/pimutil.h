///////////////////////////////////////////////////////////////////////////////
// pimutil.h
//
// Copyright(c) 2019, Garfield. All rights reserved.
// Author  : Garfield
// Version : 1.0
// Creation Date : 2010/4/22

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __PIMUTIL_H__
#define __PIMUTIL_H__

#ifndef _WIN32_WCE
    #error pimutil.h compiles under Windows CE only
#endif

#include "atlmiscutil.h"

///////////////////////////////////////////////////////////////////////////////
// Classes in this file:
//
// CCePropVal

namespace WCE {

///////////////////////////////////////////////////////////////////////////////
// Interface of the CCePropValTraits class
//

struct CCePropValTraits : public atlutil::CHandleTraits
{
    typedef CEPROPVAL* HANDLET_PTR;
    static BOOL WINAPI Destroy(HANDLET_PTR hHandle);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CCePropVal class
//

class CCePropVal : public atlutil::CHandle<CCePropValTraits>
{
// Constructors
public:
    CCePropVal();

// Operations
public:
    CEPROPVAL** GetAddress();

// Attributes
public:
    WORD GetFlags(DWORD dwIndex = 0) const;
    BOOL IsValidProp(DWORD dwIndex = 0) const;

    DWORD GetSize(DWORD dwIndex = 0) const;
    LPBYTE GetBlob(DWORD dwIndex = 0) const;

    BOOL GetBool(DWORD dwIndex = 0) const;
    long GetLong(DWORD dwIndex = 0) const;
    short GetShort(DWORD dwIndex = 0) const;
    double GetDouble(DWORD dwIndex = 0) const;
    LPCWSTR GetString(DWORD dwIndex = 0) const;

    const FILETIME& GetDateTime(DWORD dwIndex = 0) const;
    BOOL GetDateTime(SYSTEMTIME& date, DWORD dwIndex = 0) const;
};

}  // namespace WCE

#include "pimutil.inl"

#endif  // __PIMUTIL_H__