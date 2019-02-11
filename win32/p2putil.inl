///////////////////////////////////////////////////////////////////////////////
// p2putil.inl
//
// Copyright(c) 2019, Garfield. All rights reserved.
// Author  : Garfield
// Version : 1.0
// Creation Date : 2011/6/23

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __P2PUTIL_INL__
#define __P2PUTIL_INL__

#ifndef __P2PUTIL_H__
    #error p2putil.inl requires p2putil.h to be included first
#endif

namespace atlutil {

///////////////////////////////////////////////////////////////////////////////
// Type Definition
//

// Peer Common Type Definition
typedef CPeerData<WCHAR> CPeerString;
typedef CPeerData<PEER_RECORD> CPeerRecord;
typedef CPeerData<PEER_NODE_INFO> CPeerNodeInfo;
typedef CPeerData<PEER_INVITATION_INFO> CPeerInvitationInfo;
typedef CPeerData<PEER_GROUP_EVENT_DATA> CPeerGroupEventData;
typedef CPeerData<PEER_GROUP_PROPERTIES> CPeerGroupProperties;

// Peer Common Array Type Definition
typedef CPeerArray<CPeerData<PPEER_MEMBER> > CPeerMemberArray;
typedef CPeerArray<CPeerData<PPEER_RECORD> > CPeerRecordArray;
typedef CPeerArray<CPeerData<PPEER_NAME_PAIR> > CPeerNamePairArray;
typedef CPeerArray<CPeerData<PEER_CONNECTION_INFO*> > CPeerConnInfoArray;

// Peer Graph Common Type Definition
typedef CPeerGraphData<PEER_RECORD> CPeerGraphRecord;
typedef CPeerGraphData<PEER_GRAPH_EVENT_DATA> CPeerGraphEventData;
typedef CPeerGraphData<PEER_GRAPH_PROPERTIES> CPeerGraphProperties;

// Peer Graph Type Definition
typedef CPeerArray<CPeerGraphData<PPEER_NODE_INFO> > CPeerNodeInfoArray;
typedef CPeerArray<CPeerGraphData<PPEER_RECORD> > CPeerGraphRecordArray;
typedef CPeerArray<CPeerGraphData<PEER_CONNECTION_INFO*> > CPeerGraphConnInfoArray;


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CPeerDataBase class
//

template <typename _Ty>
inline CPeerDataBase<_Ty>::CPeerDataBase()
    : m_pData(NULL)
{
}

template <typename _Ty>
inline _Ty** CPeerDataBase<_Ty>::operator&()
{
    ATLASSERT(IsEmpty());
    return &m_pData;
}

template <typename _Ty>
inline CPeerDataBase<_Ty>::operator _Ty*()
{
    return m_pData;
}

template <typename _Ty>
inline CPeerDataBase<_Ty>::operator const _Ty*() const
{
    return m_pData;
}

template <typename _Ty>
inline BOOL CPeerDataBase<_Ty>::IsEmpty() const
{
    return (m_pData == NULL);
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CPeerData class
//

template <typename _Ty>
inline CPeerData<_Ty>::CPeerData()
{
}

template <typename _Ty>
inline CPeerData<_Ty>::~CPeerData()
{
    ::PeerFreeData(m_pData);
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CPeerGraphData class
//

template <typename _Ty>
inline CPeerGraphData<_Ty>::CPeerGraphData()
{
}

template <typename _Ty>
inline CPeerGraphData<_Ty>::~CPeerGraphData()
{
    ::PeerGraphFreeData(m_pData);
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CPeerArray class
//

template <typename TBase>
inline CPeerArray<TBase>::CPeerArray()
    : m_nCount(0)
{
}

template <typename TBase>
inline LPVOID** CPeerArray<TBase>::GetAddress()
{
    ATLASSERT(IsEmpty());
    return (LPVOID**)&m_pData;
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CPeerGraphTraits class
//

inline BOOL WINAPI CPeerGraphTraits::Destroy(HANDLET_PTR hHandle)
{
    return SUCCEEDED(::PeerGraphClose(hHandle));
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CPeerGraph class
//

inline CPeerGraph::CPeerGraph()
{
}

inline HRESULT CPeerGraph::Create(PPEER_GRAPH_PROPERTIES pProperties, LPCWSTR pszDatabaseName, PPEER_SECURITY_INTERFACE pSecurityInterface/* = NULL*/)
{
    ATLASSERT(IsEmpty());
    ATLASSERT(pProperties);
    ATLASSERT(pszDatabaseName);

    return ::PeerGraphCreate(pProperties, pszDatabaseName, pSecurityInterface, &m_hHandle);
}

inline HRESULT CPeerGraph::Open(LPCWSTR pszGraphId, LPCWSTR pszPeerId, LPCWSTR pszDatabaseName, PPEER_SECURITY_INTERFACE pSecurityInterface/* = NULL*/, LPGUID pRecordTypeSyncPrecedence/* = NULL*/, ULONG nNumberOfRecordTypeSyncPrecedence/* = 0*/)
{
    ATLASSERT(IsEmpty());
    ATLASSERT(pszPeerId);
    ATLASSERT(pszGraphId);
    ATLASSERT(pszDatabaseName);

    return ::PeerGraphOpen(pszGraphId, pszPeerId, pszDatabaseName, pSecurityInterface, nNumberOfRecordTypeSyncPrecedence, pRecordTypeSyncPrecedence, &m_hHandle);
}

inline HRESULT CPeerGraph::Listen(DWORD dwScope/* = PEER_GRAPH_SCOPE_GLOBAL*/, DWORD dwScopeId/* = 0*/, WORD wPort/* = 0*/)
{
    ATLASSERT(!IsEmpty());
    return ::PeerGraphListen(m_hHandle, dwScope, dwScopeId, wPort);
}

inline HRESULT CPeerGraph::Connect(PPEER_ADDRESS pAddress, ULONGLONG& ullConnectionId, LPCWSTR pszPeerId/* = NULL*/)
{
    ATLASSERT(pAddress);
    ATLASSERT(!IsEmpty());

    return ::PeerGraphConnect(m_hHandle, pszPeerId, pAddress, &ullConnectionId);
}

inline HRESULT CPeerGraph::OpenConnection(LPCWSTR pszPeerId, PPEER_ADDRESS pAddress, ULONGLONG& ullConnectionId)
{
    ATLASSERT(pAddress);
    ATLASSERT(!IsEmpty());

    return ::PeerGraphOpenDirectConnection(m_hHandle, pszPeerId, pAddress, &ullConnectionId);
}

inline HRESULT CPeerGraph::CloseConnection(ULONGLONG ullConnectionId)
{
    ATLASSERT(!IsEmpty());
    return ::PeerGraphCloseDirectConnection(m_hHandle, ullConnectionId);
}

inline HRESULT CPeerGraph::SendData(ULONGLONG ullConnectionId, LPGUID pType, LPVOID pvData, ULONG cbData) const
{
    ATLASSERT(pType);
    ATLASSERT(!IsEmpty());

    return ::PeerGraphSendData(m_hHandle, ullConnectionId, pType, cbData, pvData);
}

inline HRESULT CPeerGraph::AddRecord(PPEER_RECORD pRecord, GUID& recordId)
{
    ATLASSERT(pRecord);
    ATLASSERT(!IsEmpty());

    return ::PeerGraphAddRecord(m_hHandle, pRecord, &recordId);
}

inline HRESULT CPeerGraph::DeleteRecord(LPGUID pRecordId, BOOL bLocal/* = FALSE*/)
{
    ATLASSERT(pRecordId);
    ATLASSERT(!IsEmpty());

    return ::PeerGraphDeleteRecord(m_hHandle, pRecordId, bLocal);
}

inline HRESULT CPeerGraph::UpdateRecord(PPEER_RECORD pRecord)
{
    ATLASSERT(pRecord);
    ATLASSERT(!IsEmpty());

    return ::PeerGraphUpdateRecord(m_hHandle, pRecord);
}

inline HRESULT CPeerGraph::GetRecord(LPGUID pRecordId, PPEER_RECORD* ppRecord) const
{
    ATLASSERT(ppRecord);
    ATLASSERT(pRecordId);
    ATLASSERT(!IsEmpty());

    return ::PeerGraphGetRecord(m_hHandle, pRecordId, ppRecord);
}

inline HRESULT CPeerGraph::ImportDatabase(LPCWSTR pszFilePath)
{
    ATLASSERT(!IsEmpty());
    ATLASSERT(pszFilePath);

    return ::PeerGraphImportDatabase(m_hHandle, pszFilePath);
}

inline HRESULT CPeerGraph::ExportDatabase(LPCWSTR pszFilePath) const
{
    ATLASSERT(!IsEmpty());
    ATLASSERT(pszFilePath);

    return ::PeerGraphExportDatabase(m_hHandle, pszFilePath);
}

inline HRESULT CPeerGraph::SetPresence(BOOL bPresent)
{
    ATLASSERT(!IsEmpty());
    return ::PeerGraphSetPresence(m_hHandle, bPresent);
}

inline HRESULT CPeerGraph::SetNodeAttributes(LPCWSTR pszAttributes)
{
    ATLASSERT(!IsEmpty());
    ATLASSERT(pszAttributes);

    return ::PeerGraphSetNodeAttributes(m_hHandle, pszAttributes);
}

inline HRESULT CPeerGraph::Delete(LPCWSTR pszGraphId, LPCWSTR pszPeerId, LPCWSTR pszDatabaseName)
{
    ATLASSERT(!IsEmpty());
    ATLASSERT(pszPeerId);
    ATLASSERT(pszGraphId);
    ATLASSERT(pszDatabaseName);

    return ::PeerGraphDelete(pszGraphId, pszPeerId, pszDatabaseName);
}

inline HRESULT CPeerGraph::ValidateDeferredRecords(LPGUID pRecordIds/* = NULL*/, ULONG nNumberOfRecordIds/* = 0*/) const
{
    ATLASSERT(!IsEmpty());
    return ::PeerGraphValidateDeferredRecords(m_hHandle, nNumberOfRecordIds, pRecordIds);
}

inline HRESULT CPeerGraph::PeerTimeToUniversalTime(const FILETIME& ftPeerTime, FILETIME& ftUniversalTime) const
{
    ATLASSERT(!IsEmpty());
    return ::PeerGraphPeerTimeToUniversalTime(m_hHandle, (LPFILETIME)&ftPeerTime, &ftUniversalTime);
}

inline HRESULT CPeerGraph::UniversalTimeToPeerTime(const FILETIME& ftUniversalTime, FILETIME& ftPeerTime) const
{
    ATLASSERT(!IsEmpty());
    return ::PeerGraphUniversalTimeToPeerTime(m_hHandle, (LPFILETIME)&ftUniversalTime, &ftPeerTime);
}

inline HRESULT CPeerGraph::GetStatus(DWORD& dwStatus) const
{
    ATLASSERT(!IsEmpty());
    return ::PeerGraphGetStatus(m_hHandle, &dwStatus);
}

inline HRESULT CPeerGraph::GetNodeInfo(ULONGLONG ullNodeId, PPEER_NODE_INFO* ppNodeInfo) const
{
    ATLASSERT(!IsEmpty());
    ATLASSERT(ppNodeInfo);

    return ::PeerGraphGetNodeInfo(m_hHandle, ullNodeId, ppNodeInfo);
}

inline HRESULT CPeerGraph::SetProperties(PPEER_GRAPH_PROPERTIES pProperties)
{
    ATLASSERT(!IsEmpty());
    ATLASSERT(pProperties);

    return ::PeerGraphSetProperties(m_hHandle, pProperties);
}

inline HRESULT CPeerGraph::GetProperties(PPEER_GRAPH_PROPERTIES* ppProperties) const
{
    ATLASSERT(!IsEmpty());
    return ::PeerGraphGetProperties(m_hHandle, ppProperties);
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CPeerGraphTraits class
//

inline BOOL WINAPI CPeerGroupTraits::Destroy(HANDLET_PTR hHandle)
{
    return SUCCEEDED(::PeerGroupClose(hHandle));
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CPeerGroup class
//

inline CPeerGroup::CPeerGroup()
{
}

inline HRESULT CPeerGroup::Create(PPEER_GROUP_PROPERTIES pProperties)
{
    ATLASSERT(IsEmpty());
    ATLASSERT(pProperties);

    return ::PeerGroupCreate(pProperties, &m_hHandle);
}

inline HRESULT CPeerGroup::Open(LPCWSTR pszIdentity, LPCWSTR pszGroupPeerName, LPCWSTR pszCloud/* = NULL*/)
{
    ATLASSERT(IsEmpty());
    ATLASSERT(pszIdentity);
    ATLASSERT(pszGroupPeerName);

    return ::PeerGroupOpen(pszIdentity, pszGroupPeerName, pszCloud, &m_hHandle);
}

inline HRESULT CPeerGroup::Join(LPCWSTR pszIdentity, LPCWSTR pszInvitation, LPCWSTR pszCloud/* = NULL*/)
{
    ATLASSERT(IsEmpty());
    ATLASSERT(pszIdentity);
    ATLASSERT(pszInvitation);

    return ::PeerGroupJoin(pszIdentity, pszInvitation, pszCloud, &m_hHandle);
}

inline HRESULT CPeerGroup::Connect()
{
    ATLASSERT(!IsEmpty());
    return ::PeerGroupConnect(m_hHandle);
}

inline HRESULT CPeerGroup::OpenConnection(LPCWSTR pszIdentity, PPEER_ADDRESS pAddress, ULONGLONG& ullConnectionId)
{
    ATLASSERT(pAddress);
    ATLASSERT(!IsEmpty());
    ATLASSERT(pszIdentity);

    return ::PeerGroupOpenDirectConnection(m_hHandle, pszIdentity, pAddress, &ullConnectionId);
}

inline HRESULT CPeerGroup::CloseConnection(ULONGLONG ullConnectionId)
{
    ATLASSERT(!IsEmpty());
    return ::PeerGroupCloseDirectConnection(m_hHandle, ullConnectionId);
}

inline HRESULT CPeerGroup::SendData(ULONGLONG ullConnectionId, LPGUID pType, LPVOID pvData, ULONG cbData) const
{
    ATLASSERT(pType);
    ATLASSERT(pvData);
    ATLASSERT(!IsEmpty());

    return ::PeerGroupSendData(m_hHandle, ullConnectionId, pType, cbData, pvData);
}

inline HRESULT CPeerGroup::AddRecord(PPEER_RECORD pRecord, GUID& recordId)
{
    ATLASSERT(pRecord);
    ATLASSERT(!IsEmpty());

    return ::PeerGroupAddRecord(m_hHandle, pRecord, &recordId);
}

inline HRESULT CPeerGroup::DeleteRecord(LPGUID pRecordId)
{
    ATLASSERT(pRecordId);
    ATLASSERT(!IsEmpty());

    return ::PeerGroupDeleteRecord(m_hHandle, pRecordId);
}

inline HRESULT CPeerGroup::UpdateRecord(PPEER_RECORD pRecord)
{
    ATLASSERT(pRecord);
    ATLASSERT(!IsEmpty());

    return ::PeerGroupUpdateRecord(m_hHandle, pRecord);
}

inline HRESULT CPeerGroup::GetRecord(LPGUID pRecordId, PPEER_RECORD* ppRecord) const
{
    ATLASSERT(ppRecord);
    ATLASSERT(pRecordId);
    ATLASSERT(!IsEmpty());

    return ::PeerGroupGetRecord(m_hHandle, pRecordId, ppRecord);
}

inline HRESULT CPeerGroup::ImportDatabase(LPCWSTR pszFilePath)
{
    ATLASSERT(!IsEmpty());
    ATLASSERT(pszFilePath);

    return ::PeerGroupImportDatabase(m_hHandle, pszFilePath);
}

inline HRESULT CPeerGroup::ExportDatabase(LPCWSTR pszFilePath) const
{
    ATLASSERT(!IsEmpty());
    ATLASSERT(pszFilePath);

    return ::PeerGroupExportDatabase(m_hHandle, pszFilePath);
}

inline HRESULT CPeerGroup::ExportConfig(LPCWSTR pszPassword, LPWSTR* ppszXML) const
{
    ATLASSERT(ppszXML);
    ATLASSERT(!IsEmpty());
    ATLASSERT(pszPassword);

    return ::PeerGroupExportConfig(m_hHandle, pszPassword, ppszXML);
}

inline HRESULT CPeerGroup::PeerTimeToUniversalTime(const FILETIME& ftPeerTime, FILETIME& ftUniversalTime) const
{
    ATLASSERT(!IsEmpty());
    return ::PeerGroupPeerTimeToUniversalTime(m_hHandle, (LPFILETIME)&ftPeerTime, &ftUniversalTime);
}

inline HRESULT CPeerGroup::UniversalTimeToPeerTime(const FILETIME& ftUniversalTime, FILETIME& ftPeerTime) const
{
    ATLASSERT(!IsEmpty());
    return ::PeerGroupUniversalTimeToPeerTime(m_hHandle, (LPFILETIME)&ftUniversalTime, &ftPeerTime);
}

inline HRESULT CPeerGroup::IssueCredentials(LPCWSTR pszSubjectIdentity, PPEER_CREDENTIAL_INFO pCredentialInfo/* = NULL*/, DWORD dwFlags/* = 0*/, LPWSTR* ppszInvitation/* = NULL*/)
{
    ATLASSERT(!IsEmpty());
    ATLASSERT(pszSubjectIdentity);

    return ::PeerGroupIssueCredentials(m_hHandle, pszSubjectIdentity, pCredentialInfo, dwFlags, ppszInvitation);
}

inline HRESULT CPeerGroup::CreateInvitation(LPCWSTR pszIdentityInfo, LPWSTR* ppszInvitation, PEER_ROLE_ID* pRoles/* = NULL*/, ULONG nNumberOfRoles/* = 0*/, LPFILETIME pftExpiration/* = NULL*/)
{
    ATLASSERT(!IsEmpty());
    ATLASSERT(ppszInvitation);
    ATLASSERT(pszIdentityInfo);

    return ::PeerGroupCreateInvitation(m_hHandle, pszIdentityInfo, pftExpiration, nNumberOfRoles, pRoles, ppszInvitation);
}

#if (_WIN32_WINNT >= 0x0600)
inline HRESULT CPeerGroup::CreateInvitation(LPWSTR* ppszInvitation)
{
    ATLASSERT(!IsEmpty());
    ATLASSERT(ppszInvitation);

    return ::PeerGroupCreatePasswordInvitation(m_hHandle, ppszInvitation);
}

inline HRESULT CPeerGroup::Connect(PPEER_ADDRESS pAddresses, ULONG nNumberOfAddresses/* = 1*/) const
{
    ATLASSERT(!IsEmpty());
    ATLASSERT(pAddresses);

    return ::PeerGroupConnectByAddress(m_hHandle, nNumberOfAddresses, pAddresses);
}

inline HRESULT CPeerGroup::Join(LPCWSTR pszIdentity, LPCWSTR pszInvitation, LPCWSTR pszPassword, LPCWSTR pszCloud)
{
    ATLASSERT(IsEmpty());
    ATLASSERT(pszIdentity);
    ATLASSERT(pszPassword);
    ATLASSERT(pszInvitation);

    return ::PeerGroupPasswordJoin(pszIdentity, pszInvitation, pszPassword, pszCloud, &m_hHandle);
}
#endif  // (_WIN32_WINNT >= 0x0600)

inline HRESULT CPeerGroup::GetStatus(DWORD& dwStatus) const
{
    ATLASSERT(!IsEmpty());
    return ::PeerGroupGetStatus(m_hHandle, &dwStatus);
}

inline HRESULT CPeerGroup::SetProperties(PPEER_GROUP_PROPERTIES pProperties)
{
    ATLASSERT(!IsEmpty());
    ATLASSERT(pProperties);

    return ::PeerGroupSetProperties(m_hHandle, pProperties);
}

inline HRESULT CPeerGroup::GetProperties(PPEER_GROUP_PROPERTIES* ppProperties) const
{
    ATLASSERT(!IsEmpty());
    ATLASSERT(ppProperties);

    return ::PeerGroupGetProperties(m_hHandle, ppProperties);
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CPeerIdentityTraits class
//

inline BOOL WINAPI CPeerIdentityTraits::Destroy(HANDLET_PTR hHandle)
{
    ::PeerFreeData(hHandle);
    return TRUE;
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CPeerIdentity class
//

inline CPeerIdentity::CPeerIdentity()
{
}

inline HRESULT CPeerIdentity::Create(LPCWSTR pszClassifier/* = NULL*/, LPCWSTR pszFriendlyName/* = NULL*/, HCRYPTPROV hCryptProv/* = NULL*/)
{
    ATLASSERT(IsEmpty());
    return ::PeerIdentityCreate(pszClassifier, pszFriendlyName, hCryptProv, &m_hHandle);
}

inline HRESULT CPeerIdentity::CreatePeerName(LPWSTR* ppszPeerName, LPCWSTR pszClassifier/* = NULL*/) const
{
    return ::PeerCreatePeerName(m_hHandle, pszClassifier, ppszPeerName);
}

inline HRESULT CPeerIdentity::Delete() const
{
    ATLASSERT(!IsEmpty());
    return ::PeerIdentityDelete(m_hHandle);
}

inline HRESULT CPeerIdentity::SetFriendlyName(LPCWSTR pszFriendlyName)
{
    ATLASSERT(!IsEmpty());
    return ::PeerIdentitySetFriendlyName(m_hHandle, pszFriendlyName);
}

inline HRESULT CPeerIdentity::GetFriendlyName(LPWSTR* ppszFriendlyName) const
{
    ATLASSERT(!IsEmpty());
    ATLASSERT(ppszFriendlyName);

    return ::PeerIdentityGetFriendlyName(m_hHandle, ppszFriendlyName);
}

inline HRESULT CPeerIdentity::GetXML(LPWSTR* ppszIdentityXML) const
{
    ATLASSERT(!IsEmpty());
    ATLASSERT(ppszIdentityXML);

    return ::PeerIdentityGetXML(m_hHandle, ppszIdentityXML);
}

inline HRESULT CPeerIdentity::GetCryptKey(HCRYPTPROV* phCryptProv) const
{
    ATLASSERT(!IsEmpty());
    ATLASSERT(phCryptProv);

    return ::PeerIdentityGetCryptKey(m_hHandle, phCryptProv);
}

inline HRESULT CPeerIdentity::Import(LPCWSTR pszPassword, LPCWSTR pszExportXML)
{
    ATLASSERT(IsEmpty());
    ATLASSERT(pszPassword);
    ATLASSERT(pszExportXML);

    return ::PeerIdentityImport(pszExportXML, pszPassword, &m_hHandle);
}

inline HRESULT CPeerIdentity::Export(LPCWSTR pszPassword, LPWSTR* ppszExportXML) const
{
    ATLASSERT(!IsEmpty());
    ATLASSERT(pszPassword);
    ATLASSERT(ppszExportXML);

    return ::PeerIdentityExport(m_hHandle, pszPassword, ppszExportXML);
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CPeerGroupEventTraits class
//

inline BOOL WINAPI CPeerGroupEventTraits::Destroy(HANDLET_PTR hHandle)
{
    return SUCCEEDED(::PeerGroupUnregisterEvent(hHandle));
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CPeerGroupEvent class
//

inline CPeerGroupEvent::CPeerGroupEvent()
{
}

inline HRESULT CPeerGroupEvent::Register(HGROUP hGroup, HANDLE hEvent, PPEER_GROUP_EVENT_REGISTRATION pEventRegistrations, DWORD dwNumberOfEventRegistration)
{
    ATLASSERT(hGroup);
    ATLASSERT(hEvent);
    ATLASSERT(IsEmpty());
    ATLASSERT(pEventRegistrations);

    return ::PeerGroupRegisterEvent(hGroup, hEvent, dwNumberOfEventRegistration, pEventRegistrations, &m_hHandle);
}

inline HRESULT CPeerGroupEvent::GetData(PPEER_GROUP_EVENT_DATA* ppEventData)
{
    ATLASSERT(!IsEmpty());
    ATLASSERT(ppEventData);

    return ::PeerGroupGetEventData(m_hHandle, ppEventData);
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CPeerGraphEventTraits class
//

inline BOOL WINAPI CPeerGraphEventTraits::Destroy(HANDLET_PTR hHandle)
{
    return SUCCEEDED(::PeerGraphUnregisterEvent(hHandle));
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CPeerGraphEvent class
//

inline CPeerGraphEvent::CPeerGraphEvent()
{
}

inline HRESULT CPeerGraphEvent::Register(HGRAPH hGraph, HANDLE hEvent, PPEER_GRAPH_EVENT_REGISTRATION pEventRegistrations, DWORD dwNumberOfEventRegistration)
{
    ATLASSERT(hGraph);
    ATLASSERT(hEvent);
    ATLASSERT(IsEmpty());
    ATLASSERT(pEventRegistrations);

    return ::PeerGraphRegisterEvent(hGraph, hEvent, dwNumberOfEventRegistration, pEventRegistrations, &m_hHandle);
}

inline HRESULT CPeerGraphEvent::GetData(PPEER_GRAPH_EVENT_DATA* ppEventData)
{
    ATLASSERT(!IsEmpty());
    ATLASSERT(ppEventData);

    return ::PeerGraphGetEventData(m_hHandle, ppEventData);
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CPeerEnumerationTraits class
//

inline BOOL WINAPI CPeerEnumerationTraits::Destroy(HANDLET_PTR hHandle)
{
    return SUCCEEDED(::PeerEndEnumeration(hHandle));
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CPeerEnumeration class
//

inline CPeerEnumeration::CPeerEnumeration()
{
}

inline HRESULT CPeerEnumeration::EnumGroups(LPCWSTR pszIdentity)
{
    ATLASSERT(IsEmpty());
    ATLASSERT(pszIdentity);

    return ::PeerEnumGroups(pszIdentity, &m_hHandle);
}

inline HRESULT CPeerEnumeration::EnumIdentities()
{
    ATLASSERT(IsEmpty());
    return ::PeerEnumIdentities(&m_hHandle);
}

inline HRESULT CPeerEnumeration::EnumGroupConnections(HGROUP hGroup, DWORD dwFlags/* = 0*/)
{
    ATLASSERT(hGroup);
    ATLASSERT(IsEmpty());

    return ::PeerGroupEnumConnections(hGroup, dwFlags, &m_hHandle);
}

inline HRESULT CPeerEnumeration::EnumGroupRecords(HGROUP hGroup, LPGUID pRecordType/* = NULL*/)
{
    ATLASSERT(hGroup);
    ATLASSERT(IsEmpty());

    return ::PeerGroupEnumRecords(hGroup, pRecordType, &m_hHandle);
}

inline HRESULT CPeerEnumeration::EnumGroupMembers(HGROUP hGroup, DWORD dwFlags/* = PEER_MEMBER_PRESENT*/, LPCWSTR pszIdentity/* = NULL*/)
{
    ATLASSERT(hGroup);
    ATLASSERT(IsEmpty());

    return ::PeerGroupEnumMembers(hGroup, dwFlags, pszIdentity, &m_hHandle);
}

inline HRESULT CPeerEnumeration::GetCount(ULONG& nCount) const
{
    ATLASSERT(!IsEmpty());
    return ::PeerGetItemCount(m_hHandle, &nCount);
}

inline HRESULT CPeerEnumeration::GetNext(LPVOID** pppvItems, ULONG& nCount) const
{
    ATLASSERT(pppvItems);
    ATLASSERT(!IsEmpty());

#if (_WIN32_WINNT >= 0x0600)
    return ::PeerGetNextItem(m_hHandle, &nCount, pppvItems);
#else
    return ::PeerGetNextItem(m_hHandle, &nCount, (LPVOID*)pppvItems);
#endif  // (_WIN32_WINNT >= 0x0600)
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CPeerGraphEnumerationTraits class
//

inline BOOL WINAPI CPeerGraphEnumerationTraits::Destroy(HANDLET_PTR hHandle)
{
    return SUCCEEDED(::PeerGraphEndEnumeration(hHandle));
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CPeerGraphEnumeration class
//

inline CPeerGraphEnumeration::CPeerGraphEnumeration()
{
}

inline HRESULT CPeerGraphEnumeration::EnumConnections(HGRAPH hGraph, DWORD dwFlags/* = 0*/)
{
    ATLASSERT(hGraph);
    ATLASSERT(IsEmpty());

    return ::PeerGraphEnumConnections(hGraph, dwFlags, &m_hHandle);
}

inline HRESULT CPeerGraphEnumeration::EnumNodes(HGRAPH hGraph, LPCWSTR pszPeerId/* = NULL*/)
{
    ATLASSERT(hGraph);
    ATLASSERT(IsEmpty());

    return ::PeerGraphEnumNodes(hGraph, pszPeerId, &m_hHandle);
}

inline HRESULT CPeerGraphEnumeration::SearchRecords(HGRAPH hGraph, LPCWSTR pszCriteria)
{
    ATLASSERT(hGraph);
    ATLASSERT(IsEmpty());

    return ::PeerGraphSearchRecords(hGraph, pszCriteria, &m_hHandle);
}

inline HRESULT CPeerGraphEnumeration::EnumRecords(HGRAPH hGraph, LPGUID pRecordType/* = NULL*/, LPCWSTR pszPeerId/* = NULL*/)
{
    ATLASSERT(hGraph);
    ATLASSERT(IsEmpty());

    return ::PeerGraphEnumRecords(hGraph, pRecordType, pszPeerId, &m_hHandle);
}

inline HRESULT CPeerGraphEnumeration::GetCount(ULONG& nCount) const
{
    ATLASSERT(!IsEmpty());
    return ::PeerGraphGetItemCount(m_hHandle, &nCount);
}

inline HRESULT CPeerGraphEnumeration::GetNext(LPVOID** pppvItems, ULONG& nCount) const
{
    ATLASSERT(pppvItems);
    ATLASSERT(!IsEmpty());

#if (_WIN32_WINNT >= 0x0600)
    return ::PeerGraphGetNextItem(m_hHandle, &nCount, pppvItems);
#else
    return ::PeerGraphGetNextItem(m_hHandle, &nCount, (LPVOID*)pppvItems);
#endif  // (_WIN32_WINNT >= 0x0600)
}

}  // namespace atlutil

#endif  // __P2PUTIL_INL__