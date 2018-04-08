///////////////////////////////////////////////////////////////////////////////
// atlcryptutil.inl
//
// Copyright(c) 2018, Garfield. All rights reserved.
// Author  : Garfield
// Version : 1.0
// Creation Date : 2009/10/8

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __ATLCRYPTUTIL_INL__
#define __ATLCRYPTUTIL_INL__

#ifndef __ATLCRYPTUTIL_H__
    #error atlcryptutil.inl requires atlcryptutil.h to be included first
#endif

namespace atlutil {

///////////////////////////////////////////////////////////////////////////////
// Implementation of the CCryptProvTraits class
//

inline BOOL WINAPI CCryptProvTraits::Destroy(HANDLET_PTR hHandle)
{
    return ::CryptReleaseContext(hHandle, 0);
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CCryptProv class
//

inline CCryptProv::CCryptProv()
{
}

inline BOOL CCryptProv::Acquire(DWORD dwProvType/* = PROV_RSA_FULL*/, LPCTSTR pszContainer/* = NULL*/, LPCTSTR pszProvider/* = NULL*/, DWORD dwFlags/* = CRYPT_VERIFYCONTEXT | CRYPT_MACHINE_KEYSET*/)
{
    ATLASSERT(IsEmpty());
    return ::CryptAcquireContext(&m_hHandle, pszContainer, pszProvider, dwProvType, dwFlags);
}

inline BOOL CCryptProv::AddRef(DWORD dwFlags/* = 0*/, LPDWORD pdwReserved/* = NULL*/)
{
    ATLASSERT(!IsEmpty());
    return ::CryptContextAddRef(m_hHandle, pdwReserved, dwFlags);
}

inline BOOL CCryptProv::Release(DWORD dwFlags/* = 0*/)
{
    ATLASSERT(!IsEmpty());
    return ::CryptReleaseContext(m_hHandle, dwFlags);
}

inline BOOL CCryptProv::SetParam(const BYTE* pData, DWORD dwParam, DWORD dwFlags/* = 0*/)
{
    ATLASSERT(!IsEmpty());
    return ::CryptSetProvParam(m_hHandle, dwParam, (LPBYTE)pData, dwFlags);
}

inline BOOL CCryptProv::GetParam(LPBYTE pData, LPDWORD pcbSize, DWORD dwParam, DWORD dwFlags/* = 0*/)
{
    ATLASSERT(!IsEmpty());
    return ::CryptGetProvParam(m_hHandle, dwParam, pData, pcbSize, dwFlags);
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CCryptKeyTraits class
//

inline BOOL WINAPI CCryptKeyTraits::Destroy(HANDLET_PTR hHandle)
{
    return ::CryptDestroyKey(hHandle);
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CCryptKey class
//

inline CCryptKey::CCryptKey()
{
}

inline BOOL CCryptKey::CreateUserKey(HCRYPTPROV hCryptProv, DWORD dwKeySpec/* = AT_KEYEXCHANGE*/)
{
    ATLASSERT(IsEmpty());
    return ::CryptGetUserKey(hCryptProv, dwKeySpec, &m_hHandle);
}

inline BOOL CCryptKey::CreateRandomKey(HCRYPTPROV hCryptProv, ALG_ID Algid, DWORD dwFlags)
{
    ATLASSERT(IsEmpty());
    return ::CryptGenKey(hCryptProv, Algid, dwFlags, &m_hHandle);
}

inline BOOL CCryptKey::CreateDerivedKey(HCRYPTPROV hCryptProv, HCRYPTHASH hCryptHash, ALG_ID Algid, DWORD dwFlags/* = CRYPT_EXPORTABLE*/)
{
    ATLASSERT(IsEmpty());
    return ::CryptDeriveKey(hCryptProv, Algid, hCryptHash, dwFlags, &m_hHandle);
}

inline BOOL CCryptKey::Duplicate(CCryptKey& that, DWORD dwFlags/* = 0*/, LPDWORD pdwReserved/* = NULL*/)
{
    ATLASSERT(!IsEmpty());
    ATLASSERT(that.IsEmpty());

    return ::CryptDuplicateKey(m_hHandle, pdwReserved, dwFlags, &that.m_hHandle);
}

inline BOOL CCryptKey::Duplicate(HCRYPTKEY* phCryptKey, DWORD dwFlags/* = 0*/, LPDWORD pdwReserved/* = NULL*/)
{
    ATLASSERT(!IsEmpty());
    return ::CryptDuplicateKey(m_hHandle, pdwReserved, dwFlags, phCryptKey);
}

inline BOOL CCryptKey::SetParam(LPBYTE pData, DWORD dwParam, DWORD dwFlags/* = 0*/)
{
    ATLASSERT(!IsEmpty());
    return ::CryptSetKeyParam(m_hHandle, dwParam, pData, dwFlags);
}

inline BOOL CCryptKey::GetParam(LPBYTE pData, LPDWORD pcbSize, DWORD dwParam/* = KP_BLOCKLEN*/, DWORD dwFlags/* = 0*/)
{
    ATLASSERT(!IsEmpty());
    return ::CryptGetKeyParam(m_hHandle, dwParam, pData, pcbSize, dwFlags);
}

inline BOOL CCryptKey::Export(HCRYPTKEY hExpKey, LPBYTE pData, LPDWORD pcbSize, DWORD dwFlags, DWORD dwBlobType/* = SIMPLEBLOB*/)
{
    ATLASSERT(!IsEmpty());
    return ::CryptExportKey(m_hHandle, hExpKey, dwBlobType, dwFlags, pData, pcbSize);
}

inline BOOL CCryptKey::Import(HCRYPTPROV hCryptProv, HCRYPTKEY hPubKey, const BYTE* pData, DWORD cbSize, DWORD dwFlags/* = CRYPT_EXPORTABLE*/)
{
    ATLASSERT(IsEmpty());
    return ::CryptImportKey(hCryptProv, pData, cbSize, hPubKey, dwFlags, &m_hHandle);
}

inline BOOL CCryptKey::Decrypt(HCRYPTHASH hCryptHash, LPBYTE pData, LPDWORD pcbSize, BOOL bFinal/* = FALSE*/, DWORD dwFlags/* = 0*/)
{
    ATLASSERT(!IsEmpty());
    return ::CryptDecrypt(m_hHandle, hCryptHash, bFinal, dwFlags, pData, pcbSize);
}

inline BOOL CCryptKey::Encrypt(HCRYPTHASH hCryptHash, LPBYTE pData, LPDWORD pcbSize, DWORD cbBufSize, BOOL bFinal/* = FALSE*/, DWORD dwFlags/* = 0*/)
{
    ATLASSERT(!IsEmpty());
    return ::CryptEncrypt(m_hHandle, hCryptHash, bFinal, dwFlags, pData, pcbSize, cbBufSize);
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CCryptHashTraits class
//

inline BOOL WINAPI CCryptHashTraits::Destroy(HANDLET_PTR hHandle)
{
    return ::CryptDestroyHash(hHandle);
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CCryptHash class
//

inline CCryptHash::CCryptHash()
{
}

inline BOOL CCryptHash::Create(HCRYPTPROV hCryptProv, ALG_ID Algid/* = CALG_MD5*/, HCRYPTKEY hCryptKey/* = NULL*/, DWORD dwFlags/* = 0*/)
{
    ATLASSERT(IsEmpty());
    return ::CryptCreateHash(hCryptProv, Algid, hCryptKey, dwFlags, &m_hHandle);
}

inline BOOL CCryptHash::HashData(const BYTE* pData, DWORD cbSize, DWORD dwFlags/* = 0*/)
{
    ATLASSERT(!IsEmpty());
    return ::CryptHashData(m_hHandle, pData, cbSize, dwFlags);
}

inline BOOL CCryptHash::SignHash(LPBYTE pSignature, LPDWORD pcbSize, DWORD dwKeySpec/* = AT_SIGNATURE*/, DWORD dwFlags/* = 0*/, LPCTSTR pszDescription/* = NULL*/)
{
    ATLASSERT(!IsEmpty());
    return ::CryptSignHash(m_hHandle, dwKeySpec, pszDescription, dwFlags, pSignature, pcbSize);
}

inline BOOL CCryptHash::Duplicate(CCryptHash& that, DWORD dwFlags/* = 0*/, LPDWORD pdwReserved/* = NULL*/)
{
    ATLASSERT(!IsEmpty());
    ATLASSERT(that.IsEmpty());

    return ::CryptDuplicateHash(m_hHandle, pdwReserved, dwFlags, &that.m_hHandle);
}

inline BOOL CCryptHash::Duplicate(HCRYPTHASH* phCryptHash, DWORD dwFlags/* = 0*/, LPDWORD pdwReserved/* = NULL*/)
{
    ATLASSERT(!IsEmpty());
    return ::CryptDuplicateHash(m_hHandle, pdwReserved, dwFlags, phCryptHash);
}

inline BOOL CCryptHash::SetParam(const BYTE* pData, DWORD dwParam, DWORD dwFlags/* = 0*/)
{
    ATLASSERT(!IsEmpty());
    return ::CryptSetHashParam(m_hHandle, dwParam, (LPBYTE)pData, dwFlags);
}

inline BOOL CCryptHash::GetParam(LPBYTE pData, LPDWORD pcbSize, DWORD dwParam/* = HP_HASHVAL*/, DWORD dwFlags/* = 0*/)
{
    ATLASSERT(!IsEmpty());
    return ::CryptGetHashParam(m_hHandle, dwParam, pData, pcbSize, dwFlags);
}

}  // namespace atlutil

#endif  // __ATLCRYPTUTIL_INL__