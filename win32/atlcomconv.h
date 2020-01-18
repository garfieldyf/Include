///////////////////////////////////////////////////////////////////////////////
// atlcomconv.h
//
// Author : Garfield
// Creation Date : 2010/3/15

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __ATLCOMCONV_H__
#define __ATLCOMCONV_H__

#include "stddefx.h"

///////////////////////////////////////////////////////////////////////////////
// Classes in this file:
//
// CA2AEX2
// CA2WEX2
// CA2TEX2
// CW2AEX2
// CW2WEX2
// CW2TEX2
// CT2AEX2
// CT2WEX2
// CT2TEX2

namespace atlutil {

///////////////////////////////////////////////////////////////////////////////
// Interface of the CA2W_EX class
//

template <int t_nFixedLength = 128>
class CA2W_EX
{
// Constructors/Destructor
public:
    CA2W_EX(LPCSTR psz, int nLength, UINT nCodePage = CP_ACP);
    ~CA2W_EX();

// Operations
public:
    operator LPWSTR() const;

// Attributes
public:
    int Length() const;

// Implementation
private:
    CA2W_EX(const CA2W_EX& that);
    CA2W_EX& operator=(const CA2W_EX& that);

// Data members
private:
    LPWSTR m_pszData;
    WCHAR  m_szBuffer[t_nFixedLength];
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CW2A_EX class
//

template <int t_nFixedLength = 128>
class CW2A_EX
{
// Constructors/Destructor
public:
    CW2A_EX(LPCWSTR psz, int nLength, UINT nCodePage = CP_ACP);
    ~CW2A_EX();

// Operations
public:
    operator LPSTR() const;

// Attributes
public:
    int Length() const;

// Implementation
private:
    CW2A_EX(const CW2A_EX& that);
    CW2A_EX& operator=(const CW2A_EX& that);

// Data members
private:
    LPSTR m_pszData;
    CHAR  m_szBuffer[t_nFixedLength];
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CW2W_EX class
//

template <int t_nFixedLength = 128>
class CW2W_EX
{
// Constructors/Destructor
public:
    CW2W_EX(LPCWSTR psz, int nLength, UINT nCodePage = CP_ACP);
    ~CW2W_EX();

// Operations
public:
    operator LPWSTR() const;

// Attributes
public:
    int Length() const;

// Implementation
private:
    CW2W_EX(const CW2W_EX& that);
    CW2W_EX& operator=(const CW2W_EX& that);

// Data members
private:
    LPWSTR m_pszData;
    WCHAR  m_szBuffer[t_nFixedLength];
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CA2A_EX class
//

template <int t_nFixedLength = 128>
class CA2A_EX
{
// Constructors/Destructor
public:
    CA2A_EX(LPCSTR psz, int nLength, UINT nCodePage = CP_ACP);
    ~CA2A_EX();

// Operations
public:
    operator LPSTR() const;

// Attributes
public:
    int Length() const;

// Implementation
private:
    CA2A_EX(const CA2A_EX& that);
    CA2A_EX& operator=(const CA2A_EX& that);

// Data members
private:
    LPSTR m_pszData;
    CHAR  m_szBuffer[t_nFixedLength];
};

}  // namespace atlutil

#include "atlcomconv.inl"

#endif  // __ATLCOMCONV_H__