///////////////////////////////////////////////////////////////////////////////
// phoneutil.h
//
// Copyright(c) 2018, Garfield. All rights reserved.
// Author  : Garfield
// Version : 1.0
// Creation Date : 2010/4/25

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __PHONEUTIL_H__
#define __PHONEUTIL_H__

#ifndef _WIN32_WCE
    #error phoneutil.h compiles under Windows CE only
#endif

#include <phone.h>
#include <simmgr.h>
#include "atlmiscutil.h"

///////////////////////////////////////////////////////////////////////////////
// Classes in this file:
//
// CSimStruct<T>
// CSimCaps
// CSimMessage
// CSimRecordInfo
// CSimFileRefresh
// CSimPhonebookCaps
// CSimPhonebookEntry
// CSimManager
// CCallLog
// CCallLogEntry
// ISimCallback
// ISimCallbackImpl<T>
//
// Global functions in this file:
//
// EnumSimMessages<_Predicate>()
// EnumSimPhonebookEntries<_Predicate>()

namespace WCE {

///////////////////////////////////////////////////////////////////////////////
// Interface of the ISimCallback class
//

interface ATL_NO_VTABLE ISimCallback
{
    virtual void OnSimNotify(DWORD dwNotifyCode, LPCVOID pData, DWORD dwDataSize) = 0;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CSimStruct class
//

template <typename T>
struct CSimStruct : public T
{
// Constructors
public:
    CSimStruct(DWORD dwParams);

// Operations
public:
    operator T*();
    operator const T*() const;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CSimManagerTraits class
//

struct CSimManagerTraits : public atlutil::CHandleTraits
{
    typedef HSIM HANDLET_PTR;
    static BOOL WINAPI Destroy(HANDLET_PTR hHandle);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CSimManager class
//

class CSimManager : public atlutil::CHandle<CSimManagerTraits>
{
// Constructors
public:
    CSimManager();

// Operations
public:
    HRESULT Create(DWORD dwFlags = 0, ISimCallback* pCallback = NULL);
    HRESULT GetPhonebookCaps(SIMPHONEBOOKCAPS& simcaps);
    HRESULT GetDevCaps(SIMCAPS& simcaps, DWORD dwCapsType = SIM_CAPSTYPE_ALL);

    HRESULT GetPhonebookStatus(DWORD& dwUsed, DWORD& dwTotal, DWORD dwLocation = SIM_PBSTORAGE_SIM);
    HRESULT GetSmsStorageStatus(DWORD& dwUsed, DWORD& dwTotal, DWORD dwStorage = SIM_SMSSTORAGE_SIM);

    HRESULT DeleteMessage(DWORD dwIndex, DWORD dwStorage = SIM_SMSSTORAGE_SIM);
    HRESULT ReadMessage(SIMMESSAGE& message, DWORD dwIndex, DWORD dwStorage = SIM_SMSSTORAGE_SIM);
    HRESULT WriteMessage(const SIMMESSAGE& message, DWORD& dwIndex, DWORD dwStorage = SIM_SMSSTORAGE_SIM);

    HRESULT DeletePhonebookEntry(DWORD dwIndex, DWORD dwLocation = SIM_PBSTORAGE_SIM);
    HRESULT ReadPhonebookEntry(SIMPHONEBOOKENTRY& pbEntry, DWORD dwIndex, DWORD dwLocation = SIM_PBSTORAGE_SIM);
    HRESULT WritePhonebookEntry(const SIMPHONEBOOKENTRY& pbEntry, DWORD dwLocation = SIM_PBSTORAGE_SIM, DWORD dwIndex = SIM_PBINDEX_FIRSTAVAILABLE);

    template <typename _Predicate> DWORD EnumMessages(_Predicate _EnumProc, DWORD_PTR dwData, DWORD dwStorage = SIM_SMSSTORAGE_SIM);
    template <typename _Predicate> DWORD EnumPhonebookEntries(_Predicate _EnumProc, DWORD_PTR dwData, DWORD dwLocation = SIM_PBSTORAGE_SIM);

// Implementation
protected:
    static void SimCallback(DWORD dwNotifyCode, LPCVOID pData, DWORD dwDataSize, DWORD dwParam);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CCallLogTraits class
//

struct CCallLogTraits : public atlutil::CHandleTraits
{
    static BOOL WINAPI Destroy(HANDLET_PTR hHandle);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CCallLog class
//

class CCallLog : public atlutil::CHandle<CCallLogTraits>
{
// Constructors
public:
    CCallLog();

// Operations
public:
    HRESULT Open();
    HRESULT GetEntry(CALLLOGENTRY& logEntry) const;
    HRESULT Seek(DWORD dwRecord, CALLLOGSEEK seek = CALLLOGSEEK_BEGINNING, LPDWORD pdwRecord = NULL);

// Attributes
public:
    DWORD GetCount() const;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CCallLogEntry class
//

class CCallLogEntry : public CALLLOGENTRY
{
// Constructors/Destructor
public:
    CCallLogEntry();
    ~CCallLogEntry();

// Operations
public:
    PCALLLOGENTRY GetAddress();
    CALLLOGENTRY& GetReference();
    void Empty();

    operator PCALLLOGENTRY();
    operator const CALLLOGENTRY*() const;

    BOOL GetStartTime(SYSTEMTIME& st) const;
    BOOL GetEndTime(SYSTEMTIME& st) const;

#ifdef _CRT_DEBUG_DUMP
    template <typename _Pr> void Dump(const _Pr& _Printer) const;
#endif

// Implementation
private:
    CCallLogEntry(const CCallLogEntry& that);
    CCallLogEntry& operator=(const CCallLogEntry& that);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the ISimCallbackImpl class
//

template <typename T>
class ATL_NO_VTABLE ISimCallbackImpl : public ISimCallback
{
// Implementation
public:
    virtual void OnSimNotify(DWORD dwNotifyCode, LPCVOID pData, DWORD dwDataSize);

// Overridables
public:
    void ISimCallback_OnRadioOn();
    void ISimCallback_OnRadioOff();
    void ISimCallback_OnRadioRemoved();
    void ISimCallback_OnRadioPresent(DWORD dwRadioPresent);

    void ISimCallback_OnCardRemoved();
    void ISimCallback_OnFileRefresh(LPSIMFILEREFRESH pFileRefresh);

    void ISimCallback_OnMessageStored(LPSIMMESSAGECHANGE pMessageChange);
    void ISimCallback_OnMessageDeleted(LPSIMMESSAGECHANGE pMessageChange);
    void ISimCallback_OnMessageReceived(LPSIMMESSAGECHANGE pMessageChange);

    void ISimCallback_OnPhonebookEntryStored(LPSIMPBECHANGE pPBEntryChange);
    void ISimCallback_OnPhonebookEntryDeleted(LPSIMPBECHANGE pPBEntryChange);
};


///////////////////////////////////////////////////////////////////////////////
// Global functions
//

// _EnumProc prototype : void EnumSimMessagesProc(DWORD dwIndex, const SIMMESSAGE& message, DWORD_PTR dwData);
template <typename _Predicate>
inline DWORD EnumSimMessages(HSIM hSim, _Predicate _EnumProc, DWORD_PTR dwData, DWORD dwStorage = SIM_SMSSTORAGE_SIM)
{
    ATLASSERT(hSim);

    DWORD dwUsed = 0, dwTotal = 0, dwEnumCount = 0;
    if (SUCCEEDED(::SimGetSmsStorageStatus(hSim, dwStorage, &dwUsed, &dwTotal)))
    {
        CSimMessage message(SIM_PARAM_MSG_ALL);
        for (DWORD i = 1; dwEnumCount < dwUsed && i <= dwTotal; ++i)
        {
            if (SUCCEEDED(::SimReadMessage(hSim, dwStorage, i, message)))
                _EnumProc(i, message, dwData), ++dwEnumCount;
        }
    }

    return dwEnumCount;
}

// _EnumProc prototype : void EnumPhonebookEntriesProc(DWORD dwIndex, const SIMPHONEBOOKENTRY& pbEntry, DWORD_PTR dwData);
template <typename _Predicate>
inline DWORD EnumSimPhonebookEntries(HSIM hSim, _Predicate _EnumProc, DWORD_PTR dwData, DWORD dwLocation = SIM_PBSTORAGE_SIM)
{
    ATLASSERT(hSim);

    DWORD dwUsed = 0, dwTotal = 0, dwEnumCount = 0;
    if (SUCCEEDED(::SimGetPhonebookStatus(hSim, dwLocation, &dwUsed, &dwTotal)))
    {
        CSimPhonebookEntry pbEntry(SIM_PARAM_PBE_ALL);
        for (DWORD i = 1; dwEnumCount < dwUsed && i <= dwTotal; ++i)
        {
            if (SUCCEEDED(::SimReadPhonebookEntry(hSim, dwLocation, i, pbEntry)))
                _EnumProc(i, pbEntry, dwData), ++dwEnumCount;
        }
    }

    return dwEnumCount;
}

}  // namespace WCE

#include "phoneutil.inl"

#endif  // __PHONEUTIL_H__