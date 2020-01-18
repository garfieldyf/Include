///////////////////////////////////////////////////////////////////////////////
// cemapiutil.h
//
// Author : Garfield
// Creation Date : 2009/12/14

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __CEMAPIUTIL_H__
#define __CEMAPIUTIL_H__

#ifndef _WIN32_WCE
    #error cemapiutil.h compiles under Windows CE only
#endif

#include <regext.h>
#include <cemapi.h>
#include <mapiutil.h>
#include "atlmiscutil.h"

///////////////////////////////////////////////////////////////////////////////
// Classes in this file:
//
// CSRowSet
// CSPropValue
// CSPropArrayTraits<_Prop, _PropArray>
// CAdrListTraits
// CSPropProblemArrayTraits
// CSPropArray<_PropArrayTraits, cValues>
// IMAPIAdviseSinkImpl<T>
// IMailRuleClientImpl<T>

namespace WCE {

///////////////////////////////////////////////////////////////////////////////
// Interface of the CSRowSetTraits class
//

struct CSRowSetTraits : public atlutil::CHandleTraits
{
    typedef SRowSet* HANDLET_PTR;
    static BOOL WINAPI Destroy(HANDLET_PTR hHandle);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CSRowSet class
//

class CSRowSet : public atlutil::CHandle<CSRowSetTraits>
{
// Constructors
public:
    CSRowSet();

// Operations
public:
    LPSRowSet* GetAddress();

// Attributes
public:
    ULONG GetRows() const;
    ULONG GetCols() const;

    ULONG GetPropTag(ULONG ulRow, ULONG ulCol) const;
    ULONG GetPropType(ULONG ulRow, ULONG ulCol) const;

    LONG GetLong(ULONG ulRow, ULONG ulCol) const;
    BOOL GetBool(ULONG ulRow, ULONG ulCol) const;
    short GetShort(ULONG ulRow, ULONG ulCol) const;
    float GetFloat(ULONG ulRow, ULONG ulCol) const;
    double GetDouble(ULONG ulRow, ULONG ulCol) const;
    LONGLONG GetInt64(ULONG ulRow, ULONG ulCol) const;

    ULONG GetSize(ULONG ulRow, ULONG ulCol) const;
    LPBYTE GetBinary(ULONG ulRow, ULONG ulCol) const;

    LPCSTR GetStringA(ULONG ulRow, ULONG ulCol) const;
    LPCWSTR GetStringW(ULONG ulRow, ULONG ulCol) const;

    const GUID* GetGUID(ULONG ulRow, ULONG ulCol) const;
    const CURRENCY& GetCurrency(ULONG ulRow, ULONG ulCol) const;

    const FILETIME& GetDateTime(ULONG ulRow, ULONG ulCol) const;
    BOOL GetDateTime(ULONG ulRow, ULONG ulCol, SYSTEMTIME& st) const;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CSPropValue class
//

class CSPropValue
{
// Constructors/Destructor
public:
    CSPropValue();
    ~CSPropValue();

// Operations
public:
    LPSPropValue* GetAddress();
    void Empty();

// Attributes
public:
    ULONG GetPropTag(ULONG ulIndex) const;
    ULONG GetPropType(ULONG ulIndex) const;

    LONG GetLong(ULONG ulIndex) const;
    BOOL GetBool(ULONG ulIndex) const;
    short GetShort(ULONG ulIndex) const;
    float GetFloat(ULONG ulIndex) const;
    double GetDouble(ULONG ulIndex) const;
    LONGLONG GetInt64(ULONG ulIndex) const;

    ULONG GetSize(ULONG ulIndex) const;
    LPBYTE GetBinary(ULONG ulIndex) const;

    LPCSTR GetStringA(ULONG ulIndex) const;
    LPCWSTR GetStringW(ULONG ulIndex) const;

    const GUID* GetGUID(ULONG ulIndex) const;
    const CURRENCY& GetCurrency(ULONG ulIndex) const;

    const FILETIME& GetDateTime(ULONG ulIndex) const;
    BOOL GetDateTime(ULONG ulIndex, SYSTEMTIME& st) const;

// Implementation
private:
    CSPropValue(const CSPropValue& that);
    CSPropValue& operator=(const CSPropValue& that);

// Data members
public:
    ULONG m_ulValues;
    LPSPropValue m_pPropVal;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CSPropArrayTraits class
//

template <typename _Prop, typename _PropArray>
struct CSPropArrayTraits
{
    typedef _Prop _PropType;
    typedef const _Prop& _RefProp;
    typedef _PropArray* LPPropArray;
    typedef const _PropArray* LPCPropArray;
};

template <>
struct CSPropArrayTraits<ULONG, SPropTagArray>
{
    typedef ULONG _PropType;
    typedef ULONG _RefProp;
    typedef SPropTagArray* LPPropArray;
    typedef const SPropTagArray* LPCPropArray;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CSPropArray class
//

template <typename _PropArrayTraits, ULONG cValues>
class CSPropArray
{
public:
    typedef typename _PropArrayTraits::_PropType _Prop;
    typedef typename _PropArrayTraits::_RefProp  _RefProp;
    typedef typename _PropArrayTraits::LPPropArray  LPPropArray;
    typedef typename _PropArrayTraits::LPCPropArray LPCPropArray;

// Constructors
public:
    CSPropArray();
    CSPropArray(_RefProp _Value);
    CSPropArray(_RefProp _Value, _RefProp _Value2);
    CSPropArray(_RefProp _Value, _RefProp _Value2, _RefProp _Value3);

// Operations
public:
    operator LPPropArray();
    operator LPCPropArray() const;

// Data members
public:
    ULONG m_ulValues;
    _Prop m_Props[cValues];
};


#ifndef __ATLCOMUTIL_H__
///////////////////////////////////////////////////////////////////////////////
// Interface of the IMAPIAdviseSinkImpl class
//

template <typename T>
class ATL_NO_VTABLE IMAPIAdviseSinkImpl : public atlutil::IUnknownEventImpl<T, IMAPIADVISESINKID, &IID_IMAPIAdviseSink>
{
// Operations
public:
    HRESULT IMAPIAdviseSink_Advise(IMsgStore* pStore, ULONG ulEventMask);
    HRESULT IMAPIAdviseSink_Advise(IMAPISession* pSession, ULONG ulEventMask);

    HRESULT IMAPIAdviseSink_Unadvise(IMsgStore* pStore);
    HRESULT IMAPIAdviseSink_Unadvise(IMAPISession* pSession);

// Implementation
public:
    STDMETHOD_(ULONG, OnNotify)(ULONG cNotif, LPNOTIFICATION pNotifications);

// Overridables
public:
    ULONG IMAPIAdviseSink_NewMail(NEWMAIL_NOTIFICATION* pNotifications);
    ULONG IMAPIAdviseSink_Extended(EXTENDED_NOTIFICATION* pNotifications);
    ULONG IMAPIAdviseSink_ObjectMoved(OBJECT_NOTIFICATION* pNotifications);
    ULONG IMAPIAdviseSink_CriticalError(ERROR_NOTIFICATION* pNotifications);
    ULONG IMAPIAdviseSink_TableModified(TABLE_NOTIFICATION* pNotifications);
    ULONG IMAPIAdviseSink_ObjectCopied(OBJECT_NOTIFICATION* pNotifications);
    ULONG IMAPIAdviseSink_ObjectCreated(OBJECT_NOTIFICATION* pNotifications);
    ULONG IMAPIAdviseSink_ObjectDeleted(OBJECT_NOTIFICATION* pNotifications);
    ULONG IMAPIAdviseSink_ObjectModified(OBJECT_NOTIFICATION* pNotifications);
    ULONG IMAPIAdviseSink_SearchComplete(OBJECT_NOTIFICATION* pNotifications);
};
#endif  // __ATLCOMUTIL_H__


///////////////////////////////////////////////////////////////////////////////
// Interface of the IMailRuleClientImpl class
//

template <typename T>
class ATL_NO_VTABLE IMailRuleClientImpl : public IMailRuleClient
{
// Operations
public:
    static HRESULT WINAPI IMailRuleClient_RegisterServer();
    static HRESULT WINAPI IMailRuleClient_UnregisterServer();

// Implementation
public:
    STDMETHOD(Initialize)(IMsgStore* pStore, MRCACCESS* pmaDesired);
    STDMETHOD(ProcessMessage)(IMsgStore* pStore, ULONG cbMsg, LPENTRYID pMsg, ULONG cbDestFolder, LPENTRYID pDestFolder, ULONG* pulEventType, MRCHANDLED* pHandled);

// Overridables
public:
    HRESULT IMailRuleClient_ProcessMessage(IMsgStore* pStore, ULONG cbMsg, LPENTRYID pMsg, ULONG cbDestFolder, LPENTRYID pDestFolder, ULONG* pulEventType, MRCHANDLED* pHandled);
};

}  // namespace WCE

#include "cemapiutil.inl"

#endif  // __CEMAPIUTIL_H__