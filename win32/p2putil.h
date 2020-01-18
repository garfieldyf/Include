///////////////////////////////////////////////////////////////////////////////
// p2putil.h
//
// Author : Garfield
// Creation Date : 2011/6/23

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __P2PUTIL_H__
#define __P2PUTIL_H__

#include <p2p.h>
#include "atlmiscutil.h"

///////////////////////////////////////////////////////////////////////////////
// Classes in this file:
//
// CPeerGraph
// CPeerGroup
// CPeerIdentity
// CPeerGroupEvent
// CPeerGraphEvent
// CPeerEnumeration
// CPeerGraphEnumeration
// CPeerString
// CPeerRecord
// CPeerNodeInfo
// CPeerGraphRecord
// CPeerInvitationInfo
// CPeerGroupEventData
// CPeerGraphEventData
// CPeerGroupProperties
// CPeerGraphProperties
// CPeerRecordArray
// CPeerMemberArray
// CPeerNamePairArray
// CPeerNodeInfoArray
// CPeerConnInfoArray
// CPeerGraphRecordArray
// CPeerGraphConnInfoArray

namespace atlutil {

///////////////////////////////////////////////////////////////////////////////
// Interface of the CPeerDataBase class
//

template <typename _Ty>
class CPeerDataBase
{
// Constructors
protected:
    CPeerDataBase();

// Operations
public:
    _Ty** operator&();
    operator _Ty*();
    operator const _Ty*() const;

// Attributes
public:
    BOOL IsEmpty() const;

// Implementation
private:
    CPeerDataBase(const CPeerDataBase& that);
    CPeerDataBase& operator=(const CPeerDataBase& that);

// Data members
public:
    _Ty* m_pData;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CPeerData class
//

template <typename _Ty>
class CPeerData : public CPeerDataBase<_Ty>
{
// Constructors/Destructor
public:
    CPeerData();
    ~CPeerData();
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CPeerGraphData class
//

template <typename _Ty>
class CPeerGraphData : public CPeerDataBase<_Ty>
{
// Constructors/Destructor
public:
    CPeerGraphData();
    ~CPeerGraphData();
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CPeerArray class
//

template <typename TBase>
class CPeerArray : public TBase
{
// Constructors
public:
    CPeerArray();

// Operations
public:
    LPVOID** GetAddress();

// Data members
public:
    ULONG m_nCount;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CPeerGraphTraits class
//

struct CPeerGraphTraits : public CHandleTraits
{
    typedef HGRAPH HANDLET_PTR;
    static BOOL WINAPI Destroy(HANDLET_PTR hHandle);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CPeerGraph class
//

class CPeerGraph : public CHandle<CPeerGraphTraits>
{
// Constructors
public:
    CPeerGraph();

// Operations
public:
    HRESULT Create(PPEER_GRAPH_PROPERTIES pProperties, LPCWSTR pszDatabaseName, PPEER_SECURITY_INTERFACE pSecurityInterface = NULL);
    HRESULT Open(LPCWSTR pszGraphId, LPCWSTR pszPeerId, LPCWSTR pszDatabaseName, PPEER_SECURITY_INTERFACE pSecurityInterface = NULL, LPGUID pRecordTypeSyncPrecedence = NULL, ULONG nNumberOfRecordTypeSyncPrecedence = 0);

    HRESULT Listen(DWORD dwScope = PEER_GRAPH_SCOPE_GLOBAL, DWORD dwScopeId = 0, WORD wPort = 0);
    HRESULT Connect(PPEER_ADDRESS pAddress, ULONGLONG& ullConnectionId, LPCWSTR pszPeerId = NULL);
    HRESULT OpenConnection(LPCWSTR pszPeerId, PPEER_ADDRESS pAddress, ULONGLONG& ullConnectionId);
    HRESULT CloseConnection(ULONGLONG ullConnectionId);
    HRESULT SendData(ULONGLONG ullConnectionId, LPGUID pType, LPVOID pvData, ULONG cbData) const;

    HRESULT AddRecord(PPEER_RECORD pRecord, GUID& recordId);
    HRESULT DeleteRecord(LPGUID pRecordId, BOOL bLocal = FALSE);
    HRESULT UpdateRecord(PPEER_RECORD pRecord);
    HRESULT GetRecord(LPGUID pRecordId, PPEER_RECORD* ppRecord) const;

    HRESULT ImportDatabase(LPCWSTR pszFilePath);
    HRESULT ExportDatabase(LPCWSTR pszFilePath) const;

    HRESULT SetPresence(BOOL bPresent);
    HRESULT SetNodeAttributes(LPCWSTR pszAttributes);

    HRESULT Delete(LPCWSTR pszGraphId, LPCWSTR pszPeerId, LPCWSTR pszDatabaseName);
    HRESULT ValidateDeferredRecords(LPGUID pRecordIds = NULL, ULONG nNumberOfRecordIds = 0) const;

    HRESULT PeerTimeToUniversalTime(const FILETIME& ftPeerTime, FILETIME& ftUniversalTime) const;
    HRESULT UniversalTimeToPeerTime(const FILETIME& ftUniversalTime, FILETIME& ftPeerTime) const;

// Attributes
public:
    HRESULT GetStatus(DWORD& dwStatus) const;
    HRESULT GetNodeInfo(ULONGLONG ullNodeId, PPEER_NODE_INFO* ppNodeInfo) const;

    HRESULT SetProperties(PPEER_GRAPH_PROPERTIES pProperties);
    HRESULT GetProperties(PPEER_GRAPH_PROPERTIES* ppProperties) const;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CPeerGroupTraits class
//

struct CPeerGroupTraits : public CHandleTraits
{
    typedef HGROUP HANDLET_PTR;
    static BOOL WINAPI Destroy(HANDLET_PTR hHandle);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CPeerGroup class
//

class CPeerGroup : public CHandle<CPeerGroupTraits>
{
// Constructors
public:
    CPeerGroup();

// Operations
public:
    HRESULT Create(PPEER_GROUP_PROPERTIES pProperties);
    HRESULT Open(LPCWSTR pszIdentity, LPCWSTR pszGroupPeerName, LPCWSTR pszCloud = NULL);
    HRESULT Join(LPCWSTR pszIdentity, LPCWSTR pszInvitation, LPCWSTR pszCloud = NULL);

    HRESULT Connect();
    HRESULT OpenConnection(LPCWSTR pszIdentity, PPEER_ADDRESS pAddress, ULONGLONG& ullConnectionId);
    HRESULT CloseConnection(ULONGLONG ullConnectionId);
    HRESULT SendData(ULONGLONG ullConnectionId, LPGUID pType, LPVOID pvData, ULONG cbData) const;

    HRESULT AddRecord(PPEER_RECORD pRecord, GUID& recordId);
    HRESULT DeleteRecord(LPGUID pRecordId);
    HRESULT UpdateRecord(PPEER_RECORD pRecord);
    HRESULT GetRecord(LPGUID pRecordId, PPEER_RECORD* ppRecord) const;

    HRESULT ImportDatabase(LPCWSTR pszFilePath);
    HRESULT ExportDatabase(LPCWSTR pszFilePath) const;
    HRESULT ExportConfig(LPCWSTR pszPassword, LPWSTR* ppszXML) const;

    HRESULT PeerTimeToUniversalTime(const FILETIME& ftPeerTime, FILETIME& ftUniversalTime) const;
    HRESULT UniversalTimeToPeerTime(const FILETIME& ftUniversalTime, FILETIME& ftPeerTime) const;

    HRESULT IssueCredentials(LPCWSTR pszSubjectIdentity, PPEER_CREDENTIAL_INFO pCredentialInfo = NULL, DWORD dwFlags = 0, LPWSTR* ppszInvitation = NULL);
    HRESULT CreateInvitation(LPCWSTR pszIdentityInfo, LPWSTR* ppszInvitation, PEER_ROLE_ID* pRoles = NULL, ULONG nNumberOfRoles = 0, LPFILETIME pftExpiration = NULL);

#if (_WIN32_WINNT >= 0x0600)
    HRESULT CreateInvitation(LPWSTR* ppszInvitation);
    HRESULT Connect(PPEER_ADDRESS pAddresses, ULONG nNumberOfAddresses = 1) const;
    HRESULT Join(LPCWSTR pszIdentity, LPCWSTR pszInvitation, LPCWSTR pszPassword, LPCWSTR pszCloud);
#endif

// Attributes
public:
    HRESULT GetStatus(DWORD& dwStatus) const;

    HRESULT SetProperties(PPEER_GROUP_PROPERTIES pProperties);
    HRESULT GetProperties(PPEER_GROUP_PROPERTIES* ppProperties) const;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CPeerIdentityTraits class
//

struct CPeerIdentityTraits : public CHandleTraits
{
    typedef LPWSTR HANDLET_PTR;
    static BOOL WINAPI Destroy(HANDLET_PTR hHandle);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CPeerIdentity class
//

class CPeerIdentity : public CHandle<CPeerIdentityTraits>
{
// Constructors
public:
    CPeerIdentity();

// Operations
public:
    HRESULT Create(LPCWSTR pszClassifier = NULL, LPCWSTR pszFriendlyName = NULL, HCRYPTPROV hCryptProv = NULL);
    HRESULT CreatePeerName(LPWSTR* ppszPeerName, LPCWSTR pszClassifier = NULL) const;
    HRESULT Delete() const;

    HRESULT SetFriendlyName(LPCWSTR pszFriendlyName);
    HRESULT GetFriendlyName(LPWSTR* ppszFriendlyName) const;

    HRESULT GetXML(LPWSTR* ppszIdentityXML) const;
    HRESULT GetCryptKey(HCRYPTPROV* phCryptProv) const;

    HRESULT Import(LPCWSTR pszPassword, LPCWSTR pszExportXML);
    HRESULT Export(LPCWSTR pszPassword, LPWSTR* ppszExportXML) const;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CPeerGroupEventTraits class
//

struct CPeerGroupEventTraits : public CHandleTraits
{
    typedef HPEEREVENT HANDLET_PTR;
    static BOOL WINAPI Destroy(HANDLET_PTR hHandle);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CPeerGroupEvent class
//

class CPeerGroupEvent : public CHandle<CPeerGroupEventTraits>
{
// Constructors
public:
    CPeerGroupEvent();

// Operations
public:
    HRESULT Register(HGROUP hGroup, HANDLE hEvent, PPEER_GROUP_EVENT_REGISTRATION pEventRegistrations, DWORD dwNumberOfEventRegistration);
    HRESULT GetData(PPEER_GROUP_EVENT_DATA* ppEventData);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CPeerGraphEventTraits class
//

struct CPeerGraphEventTraits : public CHandleTraits
{
    typedef HPEEREVENT HANDLET_PTR;
    static BOOL WINAPI Destroy(HANDLET_PTR hHandle);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CPeerGraphEvent class
//

class CPeerGraphEvent : public CHandle<CPeerGraphEventTraits>
{
// Constructors
public:
    CPeerGraphEvent();

// Operations
public:
    HRESULT Register(HGRAPH hGraph, HANDLE hEvent, PPEER_GRAPH_EVENT_REGISTRATION pEventRegistrations, DWORD dwNumberOfEventRegistration);
    HRESULT GetData(PPEER_GRAPH_EVENT_DATA* ppEventData);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CPeerEnumerationTraits class
//

struct CPeerEnumerationTraits : public CHandleTraits
{
    typedef HPEERENUM HANDLET_PTR;
    static BOOL WINAPI Destroy(HANDLET_PTR hHandle);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CPeerEnumeration class
//

class CPeerEnumeration : public CHandle<CPeerEnumerationTraits>
{
// Constructors
public:
    CPeerEnumeration();

// Operations
public:
    HRESULT EnumGroups(LPCWSTR pszIdentity);
    HRESULT EnumIdentities();

    HRESULT EnumGroupConnections(HGROUP hGroup, DWORD dwFlags = 0);
    HRESULT EnumGroupRecords(HGROUP hGroup, LPGUID pRecordType = NULL);
    HRESULT EnumGroupMembers(HGROUP hGroup, DWORD dwFlags = PEER_MEMBER_PRESENT, LPCWSTR pszIdentity = NULL);

    HRESULT GetCount(ULONG& nCount) const;
    HRESULT GetNext(LPVOID** pppvItems, ULONG& nCount) const;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CPeerGraphEnumerationTraits class
//

struct CPeerGraphEnumerationTraits : public CHandleTraits
{
    typedef HPEERENUM HANDLET_PTR;
    static BOOL WINAPI Destroy(HANDLET_PTR hHandle);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CPeerGraphEnumeration class
//

class CPeerGraphEnumeration : public CHandle<CPeerGraphEnumerationTraits>
{
// Constructors
public:
    CPeerGraphEnumeration();

// Operations
public:
    HRESULT EnumConnections(HGRAPH hGraph, DWORD dwFlags = 0);
    HRESULT EnumNodes(HGRAPH hGraph, LPCWSTR pszPeerId = NULL);

    HRESULT SearchRecords(HGRAPH hGraph, LPCWSTR pszCriteria);
    HRESULT EnumRecords(HGRAPH hGraph, LPGUID pRecordType = NULL, LPCWSTR pszPeerId = NULL);

    HRESULT GetCount(ULONG& nCount) const;
    HRESULT GetNext(LPVOID** pppvItems, ULONG& nCount) const;
};

}  // namespace atlutil

#include "p2putil.inl"

#endif  // __P2PUTIL_H__