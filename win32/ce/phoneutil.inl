///////////////////////////////////////////////////////////////////////////////
// phoneutil.inl
//
// Author : Garfield
// Creation Date : 2010/4/25

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __PHONEUTIL_INL__
#define __PHONEUTIL_INL__

#ifndef __PHONEUTIL_H__
    #error phoneutil.inl requires phoneutil.h to be included first
#endif

namespace WCE {

///////////////////////////////////////////////////////////////////////////////
// Type definition
//

typedef CSimStruct<SIMCAPS> CSimCaps;
typedef CSimStruct<SIMMESSAGE> CSimMessage;
typedef CSimStruct<SIMRECORDINFO> CSimRecordInfo;
typedef CSimStruct<SIMFILEREFRESH> CSimFileRefresh;
typedef CSimStruct<SIMPHONEBOOKCAPS> CSimPhonebookCaps;
typedef CSimStruct<SIMPHONEBOOKENTRY> CSimPhonebookEntry;


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CSimStruct class
//

template <typename T>
inline CSimStruct<T>::CSimStruct(DWORD dwParams)
{
    this->cbSize   = sizeof(T);
    this->dwParams = dwParams;
}

template <typename T>
inline CSimStruct<T>::operator T*()
{
    return this;
}

template <typename T>
inline CSimStruct<T>::operator const T*() const
{
    return this;
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CSimManagerTraits class
//

inline BOOL WINAPI CSimManagerTraits::Destroy(HANDLET_PTR hHandle)
{
    return SUCCEEDED(::SimDeinitialize(hHandle));
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CSimManager class
//

inline CSimManager::CSimManager()
{
}

inline HRESULT CSimManager::Create(DWORD dwFlags/* = 0*/, ISimCallback* pCallback/* = NULL*/)
{
    ATLASSERT(m_hHandle == NULL);
    return ::SimInitialize(dwFlags, SimCallback, (DWORD)pCallback, &m_hHandle);
}

inline HRESULT CSimManager::GetPhonebookCaps(SIMPHONEBOOKCAPS& simcaps)
{
    ATLASSERT(m_hHandle);
    return ::SimGetPhonebookCapabilities(m_hHandle, &simcaps);
}

inline HRESULT CSimManager::GetDevCaps(SIMCAPS& simcaps, DWORD dwCapsType/* = SIM_CAPSTYPE_ALL*/)
{
    ATLASSERT(m_hHandle);
    return ::SimGetDevCaps(m_hHandle, dwCapsType, &simcaps);
}

inline HRESULT CSimManager::GetPhonebookStatus(DWORD& dwUsed, DWORD& dwTotal, DWORD dwLocation/* = SIM_PBSTORAGE_SIM*/)
{
    ATLASSERT(m_hHandle);
    return ::SimGetPhonebookStatus(m_hHandle, dwLocation, &dwUsed, &dwTotal);
}

inline HRESULT CSimManager::GetSmsStorageStatus(DWORD& dwUsed, DWORD& dwTotal, DWORD dwStorage/* = SIM_SMSSTORAGE_SIM*/)
{
    ATLASSERT(m_hHandle);
    return ::SimGetSmsStorageStatus(m_hHandle, dwStorage, &dwUsed, &dwTotal);
}

inline HRESULT CSimManager::DeleteMessage(DWORD dwIndex, DWORD dwStorage/* = SIM_SMSSTORAGE_SIM*/)
{
    ATLASSERT(m_hHandle);
    return ::SimDeleteMessage(m_hHandle, dwStorage, dwIndex);
}

inline HRESULT CSimManager::ReadMessage(SIMMESSAGE& message, DWORD dwIndex, DWORD dwStorage/* = SIM_SMSSTORAGE_SIM*/)
{
    ATLASSERT(m_hHandle);
    return ::SimReadMessage(m_hHandle, dwStorage, dwIndex, &message);
}

inline HRESULT CSimManager::WriteMessage(const SIMMESSAGE& message, DWORD& dwIndex, DWORD dwStorage/* = SIM_SMSSTORAGE_SIM*/)
{
    ATLASSERT(m_hHandle);
    return ::SimWriteMessage(m_hHandle, dwStorage, &dwIndex, (LPSIMMESSAGE)&message);
}

inline HRESULT CSimManager::DeletePhonebookEntry(DWORD dwIndex, DWORD dwLocation/* = SIM_PBSTORAGE_SIM*/)
{
    ATLASSERT(m_hHandle);
    return ::SimDeletePhonebookEntry(m_hHandle, dwLocation, dwIndex);
}

inline HRESULT CSimManager::ReadPhonebookEntry(SIMPHONEBOOKENTRY& pbEntry, DWORD dwIndex, DWORD dwLocation/* = SIM_PBSTORAGE_SIM*/)
{
    ATLASSERT(m_hHandle);
    return ::SimReadPhonebookEntry(m_hHandle, dwLocation, dwIndex, &pbEntry);
}

inline HRESULT CSimManager::WritePhonebookEntry(const SIMPHONEBOOKENTRY& pbEntry, DWORD dwLocation/* = SIM_PBSTORAGE_SIM*/, DWORD dwIndex/* = SIM_PBINDEX_FIRSTAVAILABLE*/)
{
    ATLASSERT(m_hHandle);
    return ::SimWritePhonebookEntry(m_hHandle, dwLocation, dwIndex, (LPSIMPHONEBOOKENTRY)&pbEntry);
}

template <typename _Predicate>
inline DWORD CSimManager::EnumMessages(_Predicate _EnumProc, DWORD_PTR dwData, DWORD dwStorage/* = SIM_SMSSTORAGE_SIM*/)
{
    ATLASSERT(m_hHandle);
    return EnumSimMessages(m_hHandle, _EnumProc, dwData, dwStorage);
}

template <typename _Predicate>
inline DWORD CSimManager::EnumPhonebookEntries(_Predicate _EnumProc, DWORD_PTR dwData, DWORD dwLocation/* = SIM_PBSTORAGE_SIM*/)
{
    ATLASSERT(m_hHandle);
    return EnumSimPhonebookEntries(m_hHandle, _EnumProc, dwData, dwLocation);
}

inline void CSimManager::SimCallback(DWORD dwNotifyCode, LPCVOID pData, DWORD dwDataSize, DWORD dwParam)
{
    ATLASSERT(dwParam);
    ((ISimCallback*)dwParam)->OnSimNotify(dwNotifyCode, pData, dwDataSize);
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CCallLogTraits class
//

inline BOOL WINAPI CCallLogTraits::Destroy(HANDLET_PTR hHandle)
{
    return SUCCEEDED(::PhoneCloseCallLog(hHandle));
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CCallLog class
//

inline CCallLog::CCallLog()
{
}

inline HRESULT CCallLog::Open()
{
    ATLASSERT(m_hHandle == NULL);
    return ::PhoneOpenCallLog(&m_hHandle);
}

inline HRESULT CCallLog::GetEntry(CALLLOGENTRY& logEntry) const
{
    ATLASSERT(m_hHandle);
    return ::PhoneGetCallLogEntry(m_hHandle, &logEntry);
}

inline HRESULT CCallLog::Seek(DWORD dwRecord, CALLLOGSEEK seek/* = CALLLOGSEEK_BEGINNING*/, LPDWORD pdwRecord/* = NULL*/)
{
    ATLASSERT(m_hHandle);
    return ::PhoneSeekCallLog(m_hHandle, seek, dwRecord, pdwRecord);
}

inline DWORD CCallLog::GetCount() const
{
    ATLASSERT(m_hHandle);

    DWORD dwCount = 0;
    if (SUCCEEDED(::PhoneSeekCallLog(m_hHandle, CALLLOGSEEK_END, 0, &dwCount)))
        ++dwCount;

    return dwCount;
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CCallLogEntry class
//

inline CCallLogEntry::CCallLogEntry()
{
    ::ZeroMemory(this, sizeof(CALLLOGENTRY));
    cbSize = sizeof(CALLLOGENTRY);
}

inline CCallLogEntry::~CCallLogEntry()
{
    Empty();
}

inline PCALLLOGENTRY CCallLogEntry::GetAddress()
{
    Empty();
    return this;
}

inline CALLLOGENTRY& CCallLogEntry::GetReference()
{
    Empty();
    return *this;
}

inline void CCallLogEntry::Empty()
{
    if (pszName != NULL && ::LocalFree(pszName) == NULL)
        pszName = NULL;

    if (pszNote != NULL && ::LocalFree(pszNote) == NULL)
        pszNote = NULL;

    if (pszNumber != NULL && ::LocalFree(pszNumber) == NULL)
        pszNumber = NULL;

    if (pszNameType != NULL && ::LocalFree(pszNameType) == NULL)
        pszNameType = NULL;
}

inline CCallLogEntry::operator PCALLLOGENTRY()
{
    return this;
}

inline CCallLogEntry::operator const CALLLOGENTRY*() const
{
    return this;
}

inline BOOL CCallLogEntry::GetStartTime(SYSTEMTIME& st) const
{
    FILETIME ft;
    return (::FileTimeToLocalFileTime(&ftStartTime, &ft) && ::FileTimeToSystemTime(&ft, &st));
}

inline BOOL CCallLogEntry::GetEndTime(SYSTEMTIME& st) const
{
    FILETIME ft;
    return (::FileTimeToLocalFileTime(&ftEndTime, &ft) && ::FileTimeToSystemTime(&ft, &st));
}

#ifdef _CRT_DEBUG_DUMP
template <typename _Pr>
inline void CCallLogEntry::Dump(const _Pr& _Printer) const
{
    SYSTEMTIME startTime, endTime;
    ATLVERIFY(GetStartTime(startTime));
    ATLVERIFY(GetEndTime(endTime));

    _Printer(_T("CALL LOG INFO:\n\t[ Name       = %s ]\n\t[ Type       = %s ]\n\t[ Number     = %s ]\n\t[ Note       = %s ]\n"), pszName, pszNameType, pszNumber, pszNote);
    _Printer(_T("\t[ Call Type  = %s ]\n"), (iom == IOM_MISSED ? _T("MISS INCOMING CALL") : (iom == IOM_INCOMING ? _T("INCOMING CALL") : _T("OUTGOING CALL"))));
    _Printer(_T("\t[ Roam       = %s ]\n\t[ Ended      = %s ]\n\t[ Connected  = %s ]\n"), (fRoam ? _T("TRUE") : _T("FALSE")), (fEnded ? _T("TRUE") : _T("FALSE")), (fConnected ? _T("TRUE") : _T("FALSE")));
    _Printer(_T("\t[ Start Time = %04d/%02d/%02d %02d:%02d:%02d ]\n"), startTime.wYear, startTime.wMonth, startTime.wDay, startTime.wHour, startTime.wMinute, startTime.wSecond);
    _Printer(_T("\t[ End Time   = %04d/%02d/%02d %02d:%02d:%02d ]\n"), endTime.wYear, endTime.wMonth, endTime.wDay, endTime.wHour, endTime.wMinute, endTime.wSecond);
    _Printer(_T("\t[ Caller ID Type = %s ]\n\n"), (cidt == CALLERIDTYPE_UNAVAILABLE ? _T("UNAVAILABLE") : (cidt == CALLERIDTYPE_BLOCKED ? _T("BLOCKED") : _T("AVAILABLE"))));
}
#endif  // _CRT_DEBUG_DUMP


///////////////////////////////////////////////////////////////////////////////
// Implementation of the ISimCallbackImpl class
//

template <typename T>
inline void ISimCallbackImpl<T>::OnSimNotify(DWORD dwNotifyCode, LPCVOID pData, DWORD dwDataSize)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);
    UNUSED_PARAMETER(dwDataSize);

    switch (dwNotifyCode)
    {
    case SIM_NOTIFY_RADIOON:
        pThis->ISimCallback_OnRadioOn();
        break;

    case SIM_NOTIFY_RADIOOFF:
        pThis->ISimCallback_OnRadioOff();
        break;

    case SIM_NOTIFY_RADIOREMOVED:
        pThis->ISimCallback_OnRadioRemoved();
        break;

    case SIM_NOTIFY_RADIOPRESENT:
        ATLASSERT(dwDataSize == sizeof(DWORD));
        pThis->ISimCallback_OnRadioPresent(*(LPDWORD)pData);
        break;

    case SIM_NOTIFY_CARD_REMOVED:
        pThis->ISimCallback_OnCardRemoved();
        break;

    case SIM_NOTIFY_FILE_REFRESH:
        ATLASSERT(dwDataSize == sizeof(SIMFILEREFRESH));
        pThis->ISimCallback_OnFileRefresh((LPSIMFILEREFRESH)pData);
        break;

    case SIM_NOTIFY_MSG_STORED:
        ATLASSERT(dwDataSize == sizeof(SIMMESSAGECHANGE));
        pThis->ISimCallback_OnMessageStored((LPSIMMESSAGECHANGE)pData);
        break;

    case SIM_NOTIFY_MSG_DELETED:
        ATLASSERT(dwDataSize == sizeof(SIMMESSAGECHANGE));
        pThis->ISimCallback_OnMessageDeleted((LPSIMMESSAGECHANGE)pData);
        break;

    case SIM_NOTIFY_MSG_RECEIVED:
        ATLASSERT(dwDataSize == sizeof(SIMMESSAGECHANGE));
        pThis->ISimCallback_OnMessageReceived((LPSIMMESSAGECHANGE)pData);
        break;

    case SIM_NOTIFY_PBE_STORED:
        ATLASSERT(dwDataSize == sizeof(SIMPBECHANGE));
        pThis->ISimCallback_OnPhonebookEntryStored((LPSIMPBECHANGE)pData);
        break;

    case SIM_NOTIFY_PBE_DELETED:
        ATLASSERT(dwDataSize == sizeof(SIMPBECHANGE));
        pThis->ISimCallback_OnPhonebookEntryDeleted((LPSIMPBECHANGE)pData);
        break;
    }
}

template <typename T>
inline void ISimCallbackImpl<T>::ISimCallback_OnRadioOn()
{
}

template <typename T>
inline void ISimCallbackImpl<T>::ISimCallback_OnRadioOff()
{
}

template <typename T>
inline void ISimCallbackImpl<T>::ISimCallback_OnRadioRemoved()
{
}

template <typename T>
inline void ISimCallbackImpl<T>::ISimCallback_OnRadioPresent(DWORD /*dwRadioPresent*/)
{
}

template <typename T>
inline void ISimCallbackImpl<T>::ISimCallback_OnCardRemoved()
{
}

template <typename T>
inline void ISimCallbackImpl<T>::ISimCallback_OnFileRefresh(LPSIMFILEREFRESH /*pFileRefresh*/)
{
}

template <typename T>
inline void ISimCallbackImpl<T>::ISimCallback_OnMessageStored(LPSIMMESSAGECHANGE /*pMessageChange*/)
{
}

template <typename T>
inline void ISimCallbackImpl<T>::ISimCallback_OnMessageDeleted(LPSIMMESSAGECHANGE /*pMessageChange*/)
{
}

template <typename T>
inline void ISimCallbackImpl<T>::ISimCallback_OnMessageReceived(LPSIMMESSAGECHANGE /*pMessageChange*/)
{
}

template <typename T>
inline void ISimCallbackImpl<T>::ISimCallback_OnPhonebookEntryStored(LPSIMPBECHANGE /*pPBEntryChange*/)
{
}

template <typename T>
inline void ISimCallbackImpl<T>::ISimCallback_OnPhonebookEntryDeleted(LPSIMPBECHANGE /*pPBEntryChange*/)
{
}

}  // namespace WCE

#endif  // __PHONEUTIL_INL__