///////////////////////////////////////////////////////////////////////////////
// atlcomconv.inl
//
// Author : Garfield
// Creation Date : 2010/3/15

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __ATLCOMCONV_INL__
#define __ATLCOMCONV_INL__

#ifndef __ATLCOMCONV_H__
    #error atlcomconv.inl requires atlcomconv.h to be included first
#endif

namespace atlutil {

///////////////////////////////////////////////////////////////////////////////
// Type definition
//

#if defined(_UNICODE) || defined(UNICODE)
typedef CA2A_EX<> CA2AEX2;
typedef CA2W_EX<> CA2WEX2;
typedef CA2W_EX<> CA2TEX2;
typedef CW2A_EX<> CW2AEX2;
typedef CW2W_EX<> CW2WEX2;
typedef CW2W_EX<> CW2TEX2;
typedef CW2A_EX<> CT2AEX2;
typedef CW2W_EX<> CT2WEX2;
typedef CW2W_EX<> CT2TEX2;
#else
typedef CA2A_EX<> CA2AEX2;
typedef CA2W_EX<> CA2WEX2;
typedef CA2A_EX<> CA2TEX2;
typedef CW2A_EX<> CW2AEX2;
typedef CW2W_EX<> CW2WEX2;
typedef CW2A_EX<> CW2TEX2;
typedef CA2A_EX<> CT2AEX2;
typedef CA2W_EX<> CT2WEX2;
typedef CA2A_EX<> CT2TEX2;
#endif  // defined(_UNICODE) || defined(UNICODE)


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CA2W_EX class
//

template <int t_nFixedLength>
inline CA2W_EX<t_nFixedLength>::CA2W_EX(LPCSTR psz, int nLength, UINT nCodePage/* = CP_ACP*/)
    : m_pszData(m_szBuffer)
{
    int nLengthW = 0;
    if (psz != NULL)
    {
        nLengthW = ::MultiByteToWideChar(nCodePage, 0, psz, nLength, m_pszData, t_nFixedLength - 1);
        if (nLengthW == 0 && ::GetLastError() == ERROR_INSUFFICIENT_BUFFER)
        {
            nLengthW = ::MultiByteToWideChar(nCodePage, 0, psz, nLength, NULL, 0) + 1;
            if ((m_pszData = (LPWSTR)::malloc(nLengthW * sizeof(WCHAR))) != NULL)
                nLengthW = ::MultiByteToWideChar(nCodePage, 0, psz, nLength, m_pszData, nLengthW);
        }
    }

    m_pszData[nLengthW] = L'\0';
}

template <int t_nFixedLength>
inline CA2W_EX<t_nFixedLength>::~CA2W_EX()
{
    if (m_pszData != m_szBuffer)
        ::free(m_pszData);
#ifdef _DEBUG
    else
        ::memset(m_szBuffer, 0xCCCCCCCC, sizeof(m_szBuffer));
#endif  // _DEBUG
}

template <int t_nFixedLength>
inline CA2W_EX<t_nFixedLength>::operator LPWSTR() const
{
    return m_pszData;
}

template <int t_nFixedLength>
inline int CA2W_EX<t_nFixedLength>::Length() const
{
    return ::lstrlenW(m_pszData);
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CW2A_EX class
//

template <int t_nFixedLength>
inline CW2A_EX<t_nFixedLength>::CW2A_EX(LPCWSTR psz, int nLength, UINT nCodePage/* = CP_ACP*/)
    : m_pszData(m_szBuffer)
{
    int nLengthA = 0;
    if (psz != NULL)
    {
        nLengthA = ::WideCharToMultiByte(nCodePage, 0, psz, nLength, m_pszData, t_nFixedLength - 1, NULL, NULL);
        if (nLengthA == 0 && ::GetLastError() == ERROR_INSUFFICIENT_BUFFER)
        {
            nLengthA = ::WideCharToMultiByte(nCodePage, 0, psz, nLength, NULL, 0, NULL, NULL) + 1;
            if ((m_pszData = (LPSTR)::malloc(nLengthA)) != NULL)
                nLengthA = ::WideCharToMultiByte(nCodePage, 0, psz, nLength, m_pszData, nLengthA, NULL, NULL);
        }
    }

    m_pszData[nLengthA] = '\0';
}

template <int t_nFixedLength>
inline CW2A_EX<t_nFixedLength>::~CW2A_EX()
{
    if (m_pszData != m_szBuffer)
        ::free(m_pszData);
#ifdef _DEBUG
    else
        ::memset(m_szBuffer, 0xCCCCCCCC, sizeof(m_szBuffer));
#endif  // _DEBUG
}

template <int t_nFixedLength>
inline CW2A_EX<t_nFixedLength>::operator LPSTR() const
{
    return m_pszData;
}

template <int t_nFixedLength>
inline int CW2A_EX<t_nFixedLength>::Length() const
{
    return ::lstrlenA(m_pszData);
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CW2W_EX class
//

template <int t_nFixedLength>
inline CW2W_EX<t_nFixedLength>::CW2W_EX(LPCWSTR psz, int nLength, UINT /*nCodePage = CP_ACP*/)
    : m_pszData(m_szBuffer)
{
    m_szBuffer[0] = L'\0';
    if (psz != NULL)
    {
        if (t_nFixedLength < ++nLength)
            m_pszData = (LPWSTR)::malloc(nLength * sizeof(WCHAR));

        ATLASSERT(m_pszData);
        ::lstrcpynW(m_pszData, psz, nLength);
    }
}

template <int t_nFixedLength>
inline CW2W_EX<t_nFixedLength>::~CW2W_EX()
{
    if (m_pszData != m_szBuffer)
        ::free(m_pszData);
#ifdef _DEBUG
    else
        ::memset(m_szBuffer, 0xCCCCCCCC, sizeof(m_szBuffer));
#endif  // _DEBUG
}

template <int t_nFixedLength>
inline CW2W_EX<t_nFixedLength>::operator LPWSTR() const
{
    return m_pszData;
}

template <int t_nFixedLength>
inline int CW2W_EX<t_nFixedLength>::Length() const
{
    return ::lstrlenW(m_pszData);
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CA2A_EX class
//

template <int t_nFixedLength>
inline CA2A_EX<t_nFixedLength>::CA2A_EX(LPCSTR psz, int nLength, UINT /*nCodePage = CP_ACP*/)
    : m_pszData(m_szBuffer)
{
    m_szBuffer[0] = '\0';
    if (psz != NULL)
    {
        if (t_nFixedLength < ++nLength)
            m_pszData = (LPSTR)::malloc(nLength);

        ATLASSERT(m_pszData);
        ::lstrcpynA(m_pszData, psz, nLength);
    }
}

template <int t_nFixedLength>
inline CA2A_EX<t_nFixedLength>::~CA2A_EX()
{
    if (m_pszData != m_szBuffer)
        ::free(m_pszData);
#ifdef _DEBUG
    else
        ::memset(m_szBuffer, 0xCCCCCCCC, sizeof(m_szBuffer));
#endif  // _DEBUG
}

template <int t_nFixedLength>
inline CA2A_EX<t_nFixedLength>::operator LPSTR() const
{
    return m_pszData;
}

template <int t_nFixedLength>
inline int CA2A_EX<t_nFixedLength>::Length() const
{
    return ::lstrlenA(m_pszData);
}

}  // namespace atlutil

#endif  // __ATLCOMCONV_INL__