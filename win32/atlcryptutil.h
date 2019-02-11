///////////////////////////////////////////////////////////////////////////////
// atlcryptutil.h
//
// Copyright(c) 2019, Garfield. All rights reserved.
// Author  : Garfield
// Version : 1.0
// Creation Date : 2009/10/8

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __ATLCRYPTUTIL_H__
#define __ATLCRYPTUTIL_H__

#include <wincrypt.h>
#include "atlmiscutil.h"

///////////////////////////////////////////////////////////////////////////////
// Classes in this file:
//
// CCryptProv
// CCryptKey
// CCryptHash

namespace atlutil {

///////////////////////////////////////////////////////////////////////////////
// Interface of the CCryptProvTraits class
//

struct CCryptProvTraits : public CHandleTraits
{
    typedef HCRYPTPROV HANDLET_PTR;
    static BOOL WINAPI Destroy(HANDLET_PTR hHandle);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CCryptProv class
//

class CCryptProv : public CHandle<CCryptProvTraits>
{
// Constructors
public:
    CCryptProv();

// Operations
public:
    BOOL Acquire(DWORD dwProvType = PROV_RSA_FULL, LPCTSTR pszContainer = NULL, LPCTSTR pszProvider = NULL, DWORD dwFlags = CRYPT_VERIFYCONTEXT | CRYPT_MACHINE_KEYSET);
    BOOL AddRef(DWORD dwFlags = 0, LPDWORD pdwReserved = NULL);
    BOOL Release(DWORD dwFlags = 0);

    BOOL SetParam(const BYTE* pData, DWORD dwParam, DWORD dwFlags = 0);
    BOOL GetParam(LPBYTE pData, LPDWORD pcbSize, DWORD dwParam, DWORD dwFlags = 0);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CCryptKeyTraits class
//

struct CCryptKeyTraits : public CHandleTraits
{
    typedef HCRYPTKEY HANDLET_PTR;
    static BOOL WINAPI Destroy(HANDLET_PTR hHandle);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CCryptKey class
//

class CCryptKey : public CHandle<CCryptKeyTraits>
{
// Constructors
public:
    CCryptKey();

// Operations
public:
    BOOL CreateUserKey(HCRYPTPROV hCryptProv, DWORD dwKeySpec = AT_KEYEXCHANGE);
    BOOL CreateRandomKey(HCRYPTPROV hCryptProv, ALG_ID Algid, DWORD dwFlags);
    BOOL CreateDerivedKey(HCRYPTPROV hCryptProv, HCRYPTHASH hCryptHash, ALG_ID Algid, DWORD dwFlags = CRYPT_EXPORTABLE);

    BOOL Duplicate(CCryptKey& that, DWORD dwFlags = 0, LPDWORD pdwReserved = NULL);
    BOOL Duplicate(HCRYPTKEY* phCryptKey, DWORD dwFlags = 0, LPDWORD pdwReserved = NULL);

    BOOL SetParam(LPBYTE pData, DWORD dwParam, DWORD dwFlags = 0);
    BOOL GetParam(LPBYTE pData, LPDWORD pcbSize, DWORD dwParam = KP_BLOCKLEN, DWORD dwFlags = 0);

    BOOL Export(HCRYPTKEY hExpKey, LPBYTE pData, LPDWORD pcbSize, DWORD dwFlags, DWORD dwBlobType = SIMPLEBLOB);
    BOOL Import(HCRYPTPROV hCryptProv, HCRYPTKEY hPubKey, const BYTE* pData, DWORD cbSize, DWORD dwFlags = CRYPT_EXPORTABLE);

    BOOL Decrypt(HCRYPTHASH hCryptHash, LPBYTE pData, LPDWORD pcbSize, BOOL bFinal = FALSE, DWORD dwFlags = 0);
    BOOL Encrypt(HCRYPTHASH hCryptHash, LPBYTE pData, LPDWORD pcbSize, DWORD cbBufSize, BOOL bFinal = FALSE, DWORD dwFlags = 0);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CCryptHashTraits class
//

struct CCryptHashTraits : public CHandleTraits
{
    typedef HCRYPTHASH HANDLET_PTR;
    static BOOL WINAPI Destroy(HANDLET_PTR hHandle);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CCryptHash class
//

class CCryptHash : public CHandle<CCryptHashTraits>
{
// Constructors
public:
    CCryptHash();

// Operations
public:
    BOOL Create(HCRYPTPROV hCryptProv, ALG_ID Algid = CALG_MD5, HCRYPTKEY hCryptKey = NULL, DWORD dwFlags = 0);

    BOOL HashData(const BYTE* pData, DWORD cbSize, DWORD dwFlags = 0);
    BOOL SignHash(LPBYTE pSignature, LPDWORD pcbSize, DWORD dwKeySpec = AT_SIGNATURE, DWORD dwFlags = 0, LPCTSTR pszDescription = NULL);

    BOOL Duplicate(CCryptHash& that, DWORD dwFlags = 0, LPDWORD pdwReserved = NULL);
    BOOL Duplicate(HCRYPTHASH* phCryptHash, DWORD dwFlags = 0, LPDWORD pdwReserved = NULL);

    BOOL SetParam(const BYTE* pData, DWORD dwParam, DWORD dwFlags = 0);
    BOOL GetParam(LPBYTE pData, LPDWORD pcbSize, DWORD dwParam = HP_HASHVAL, DWORD dwFlags = 0);
};

}  // namespace atlutil

#include "atlcryptutil.inl"

#endif  // __ATLCRYPTUTIL_H__