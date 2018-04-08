///////////////////////////////////////////////////////////////////////////////
// symconutil.inl : 实现文件
//
// Copyright(c) 2012, Garfield. All rights reserved.
// Author  : Garfield
// Version : 1.0
// Finish Time : 2009/11/5

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __SYMCONUTIL_INL__
#define __SYMCONUTIL_INL__

namespace symutil {

///////////////////////////////////////////////////////////////////////////////
// Type Definition
//

typedef CCAItemData<CA_DATA_MSG, CA_DATA_FORMAT_STRUCT> CCADataMsg;
typedef CCAItemData<CA_MMS_DATA, CA_DATA_FORMAT_STRUCT> CCADataMms;
typedef CCAItemData<CA_DATA_NOTE, CA_DATA_FORMAT_STRUCT> CCADataNote;
typedef CCAItemData<CA_DATA_VERSIT, CA_DATA_FORMAT_VERSIT> CCADataVersit;
typedef CCAItemData<CA_DATA_CONTACT, CA_DATA_FORMAT_STRUCT> CCADataContact;
typedef CCAItemData<CA_DATA_CALENDAR, CA_DATA_FORMAT_STRUCT> CCADataCalendar;
typedef CCAItemData<CA_DATA_BOOKMARK, CA_DATA_FORMAT_STRUCT> CCADataBookmark;

typedef CDeviceInfo<CONAPI_DEVICE_INFO_ICON, CONAPI_DEVICE_ICON_INFO> CDeviceIconInfo;
typedef CDeviceInfo<CONAPI_DEVICE_GEN_INFO, CONAPI_DEVICE_GENERAL_INFO> CDeviceGeneralInfo;
typedef CDeviceInfo<CONAPI_DEVICE_INFO_PRODUCT, CONAPI_DEVICE_PRODUCT_INFO> CDeviceProductInfo;
typedef CDeviceInfo<CONAPI_DEVICE_INFO_PROPERTIES, CONAPI_DEVICE_PROPERTIES_INFO> CDevicePropertiesInfo;


/////////////////////////////////////////////////////////////////////////////
// CDevice 类的实现
//

inline CDevice::CDevice()
{
    ::ZeroMemory(this, sizeof(CONAPI_DEVICE));
}

inline CDevice::~CDevice()
{
    Empty();
}

inline void CDevice::Empty()
{
    if (pstrSerialNumber != NULL && ::CONAFreeDeviceStructure(1, this) == CONA_OK)
        pstrSerialNumber = NULL;
}

inline DWORD CDevice::GetDevice(DMHANDLE hDMHandle, const WCHAR* pszSerialNumber)
{
    ATLASSERT(hDMHandle);
    ATLASSERT(pszSerialNumber);
    ATLASSERT(pstrSerialNumber == NULL);

    return ::CONAGetDevice(hDMHandle, pszSerialNumber, this);
}


/////////////////////////////////////////////////////////////////////////////
// CFileData 类的实现
//

inline CFileData::CFileData()
    : m_pFileData(NULL)
{
}

inline CFileData::~CFileData()
{
    Empty();
}

inline void CFileData::Empty()
{
    if (m_pFileData != NULL && ::CONAFreeFileDataMemory(m_pFileData) == CONA_OK)
        m_pFileData = NULL;
}

inline unsigned char** CFileData::GetAddress()
{
    ATLASSERT(m_pFileData == NULL);
    return &m_pFileData;
}


/////////////////////////////////////////////////////////////////////////////
// CCAIDList 类的实现
//

inline CCAIDList::CCAIDList()
{
    ::ZeroMemory(this, sizeof(CA_ID_LIST));
    dwSize = sizeof(CA_ID_LIST);
}

inline CCAIDList::~CCAIDList()
{
    Empty();
}

inline void CCAIDList::Empty()
{
    if (pUIDs != NULL && ::CAFreeIdListStructure(this) == CONA_OK)
        pUIDs = NULL;
}

inline DWORD CCAIDList::CACommitOperations(CAOPERATIONHANDLE hOperHandle)
{
    ATLASSERT(hOperHandle);
    ATLASSERT(pUIDs == NULL);

    return ::CACommitOperations(hOperHandle, this);
}

inline DWORD CCAIDList::GetIDList(CAHANDLE hCAHandle, DWORD dwFolderID, DWORD dwOptions/* = CA_OPTION_USE_CACHE*/)
{
    ATLASSERT(hCAHandle);

    Empty();
    return ::CAGetIDList(hCAHandle, dwFolderID, dwOptions, this);
}


/////////////////////////////////////////////////////////////////////////////
// CFileInfo 类的实现
//

inline CFileInfo::CFileInfo()
{
    ::ZeroMemory(this, sizeof(CONAPI_FILE_INFO));
}

inline CFileInfo::~CFileInfo()
{
    Empty();
}

inline CFileInfo::operator LPCONAPI_FILE_INFO()
{
    return this;
}

inline CFileInfo::operator const CONAPI_FILE_INFO*() const
{
    return this;
}

inline void CFileInfo::Empty()
{
    if (pstrName != NULL && ::CONAFreeFileInfoStructure(this) == CONA_OK)
        pstrName = NULL;
}

inline DWORD CFileInfo::GetFileInfo(FSHANDLE hFSHandle, const WCHAR* pszFileName, const WCHAR* pszPath/* = NULL*/)
{
    ATLASSERT(hFSHandle);

    Empty();
    return ::CONAGetFileInfo(hFSHandle, pszFileName, this, pszPath);
}

inline const WCHAR* CFileInfo::SetFileName(const WCHAR* pszFileName)
{
    ATLASSERT(pszFileName);

    ::CONAFreeString(pstrName);
    return pstrName = ::CONAAllocString((LPAPIWCHAR)pszFileName);
}

inline const WCHAR* CFileInfo::SetMimeType(const WCHAR* pszMimeType)
{
    ATLASSERT(pszMimeType);

    ::CONAFreeString(pstrMIMEType);
    return pstrMIMEType = ::CONAAllocString((LPAPIWCHAR)pszMimeType);
}


/////////////////////////////////////////////////////////////////////////////
// CFolderInfo 类的实现
//

inline CFolderInfo::CFolderInfo()
{
    ::ZeroMemory(this, sizeof(CONAPI_FOLDER_CONTENT));
    dwSize = sizeof(CONAPI_FOLDER_CONTENT);
}

inline CFolderInfo::~CFolderInfo()
{
    Empty();
}

inline void CFolderInfo::Empty()
{
    if (pstrPath != NULL && ::CONAFreeFolderContentStructure(this) == CONA_OK)
        pstrPath = NULL;
}

inline DWORD CFolderInfo::GetFolderInfo(FSHANDLE hFSHandle, const WCHAR* pszFolderName, const WCHAR* pszPath/* = NULL*/, DWORD dwOptions/* = CONA_GET_FOLDER_INFO*/, PFN_CONA_FS_FOLDERINFO_CALLBACK pfnFolderInfoNotify/* = NULL*/)
{
    ATLASSERT(hFSHandle);

    Empty();
    return ::CONAGetFolderInfo(hFSHandle, dwOptions, pszFolderName, pszPath, this, pfnFolderInfoNotify);
}


/////////////////////////////////////////////////////////////////////////////
// CFindFile 类的实现
//

inline CFindFile::CFindFile()
{
    ::ZeroMemory(this, sizeof(CFindFile));
}

inline CFindFile::~CFindFile()
{
    Close();
}

inline void CFindFile::Close()
{
    if (m_hFind != NULL && ::CONAFindEnd(m_hFind) == CONA_OK)
        m_hFind = NULL;

    if (pstrName != NULL && ::CONAFreeFileInfoStructure(this) == CONA_OK)
        pstrName = NULL;
}

inline CFindFile::operator FINDHANDLE() const
{
    return m_hFind;
}

inline BOOL CFindFile::FindNext()
{
    ATLASSERT(m_hFind);

    DWORD dwResult = ::CONAFreeFileInfoStructure(this);
    if (dwResult == CONA_OK)
        dwResult = ::CONAFindNextFile(m_hFind, this);

    return (dwResult == CONA_OK);
}

inline DWORD CFindFile::FindFirst(FSHANDLE hFSHandle, const WCHAR* pszPath/* = NULL*/, DWORD dwFindOptions/* = 0*/)
{
    ATLASSERT(hFSHandle);
    ATLASSERT(m_hFind == NULL);

    return ::CONAFindBegin(hFSHandle, dwFindOptions, &m_hFind, pszPath);
}


/////////////////////////////////////////////////////////////////////////////
// CFindFolder 类的实现
//

inline CFindFolder::CFindFolder()
{
    ::ZeroMemory(this, sizeof(CFindFolder));
}

inline CFindFolder::~CFindFolder()
{
    Close();
}

inline void CFindFolder::Close()
{
    if (m_hFind != NULL && ::CONAFindEnd(m_hFind) == CONA_OK)
        m_hFind = NULL;

    if (pstrName != NULL && ::CONAFreeFolderInfoStructure(this) == CONA_OK)
        pstrName = NULL;
}

inline CFindFolder::operator FINDHANDLE() const
{
    return m_hFind;
}

inline BOOL CFindFolder::FindNext()
{
    ATLASSERT(m_hFind);

    DWORD dwResult = ::CONAFreeFolderInfoStructure(this);
    if (dwResult == CONA_OK)
        dwResult = ::CONAFindNextFolder(m_hFind, this);

    return (dwResult == CONA_OK);
}

inline DWORD CFindFolder::FindFirst(FSHANDLE hFSHandle, const WCHAR* pszPath/* = NULL*/, DWORD dwFindOptions/* = 0*/)
{
    ATLASSERT(hFSHandle);
    ATLASSERT(m_hFind == NULL);

    return ::CONAFindBegin(hFSHandle, dwFindOptions, &m_hFind, pszPath);
}


/////////////////////////////////////////////////////////////////////////////
// CDeviceInfo 类的实现
//

template <typename _Ty, DWORD dwType>
inline CDeviceInfo<_Ty, dwType>::CDeviceInfo()
    : m_pInfo(NULL)
{
}

template <typename _Ty, DWORD dwType>
inline CDeviceInfo<_Ty, dwType>::~CDeviceInfo()
{
    Empty();
}

template <typename _Ty, DWORD dwType>
inline void CDeviceInfo<_Ty, dwType>::Empty()
{
    if (m_pInfo != NULL && :: CONAFreeDeviceInfoStructure(dwType, m_pInfo) == CONA_OK)
        m_pInfo = NULL;
}

template <typename _Ty, DWORD dwType>
inline DWORD CDeviceInfo<_Ty, dwType>::GetInfo(DMHANDLE hDMHandle, const WCHAR* pszSerialNumber)
{
    ATLASSERT(hDMHandle);
    ATLASSERT(pszSerialNumber);
    ATLASSERT(m_pInfo == NULL);

    return ::CONAGetDeviceInfo(hDMHandle, pszSerialNumber, dwType, (LPVOID*)&m_pInfo);
}


/////////////////////////////////////////////////////////////////////////////
// CDeviceList 类的实现
//

inline CDeviceList::CDeviceList(DWORD dwDeviceCount)
    : m_pDevices(NULL), m_dwDeviceCount(dwDeviceCount)
{
    ATLASSERT(dwDeviceCount > 0);
}

inline CDeviceList::~CDeviceList()
{
    Empty();
}

inline void CDeviceList::Empty()
{
    if (m_pDevices != NULL && ::CONAFreeDeviceStructure(m_dwDeviceCount, m_pDevices) == CONA_OK)
    {
        ::free(m_pDevices);
        m_pDevices = NULL;
        m_dwDeviceCount = 0;
    }
}

inline DWORD CDeviceList::GetDevices(DMHANDLE hDMHandle)
{
    ATLASSERT(hDMHandle);
    ATLASSERT(m_pDevices == NULL);
    ATLASSERT(m_dwDeviceCount > 0);

    m_pDevices = (CONAPI_DEVICE*)::malloc(sizeof(CONAPI_DEVICE) * m_dwDeviceCount);
    ATLASSERT(m_pDevices);

    return ::CONAGetDevices(hDMHandle, &m_dwDeviceCount, m_pDevices);
}


/////////////////////////////////////////////////////////////////////////////
// CAppInfoList 类的实现
//

inline CAppInfoList::CAppInfoList()
    : m_pAppInfo(NULL), m_dwCount(0)
{
}

inline CAppInfoList::~CAppInfoList()
{
    Empty();
}

inline void CAppInfoList::Empty()
{
    if (m_pAppInfo != NULL && ::CONAFreeApplicationInfoStructures(m_dwCount, &m_pAppInfo))
        m_pAppInfo = NULL;
}

inline DWORD CAppInfoList::GetAppInfo(FSHANDLE hFSHandle, DWORD dwOptions/* = CONA_LIST_ALL_APPICATIONS*/)
{
    ATLASSERT(hFSHandle);
    ATLASSERT(m_pAppInfo == NULL);

    return ::CONAListApplications(hFSHandle, dwOptions, &m_dwCount, &m_pAppInfo);
}


/////////////////////////////////////////////////////////////////////////////
// CMemoryStatus 类的实现
//

inline DWORD CMemoryStatus::GetMemoryStatus(FSHANDLE hFSHandle, const WCHAR* pszMemoryType)
{
    ATLASSERT(hFSHandle);
    ATLASSERT(pszMemoryType);

    return ::CONAGetMemoryValues(hFSHandle, pszMemoryType, &nFreeBytes, &nTotalBytes, &nUsedBytes);
}


/////////////////////////////////////////////////////////////////////////////
// CCAItemData 类的实现
//

template <typename TBase, DWORD dwDataFormat>
inline CCAItemData<TBase, dwDataFormat>::CCAItemData(CAHANDLE hCAHandle)
    : m_hCAHandle(hCAHandle)
{
    ATLASSERT(hCAHandle);
    ::ZeroMemory(static_cast<TBase*>(this), sizeof(TBase));
}

template <typename TBase, DWORD dwDataFormat>
inline CCAItemData<TBase, dwDataFormat>::~CCAItemData()
{
    Empty();
}

template <typename TBase, DWORD dwDataFormat>
inline void CCAItemData<TBase, dwDataFormat>::Empty()
{
    if (dwSize != 0 && ::CAFreeItemData(m_hCAHandle, dwDataFormat, static_cast<TBase*>(this)) == CONA_OK)
        dwSize = 0;
}

template <typename TBase, DWORD dwDataFormat>
inline DWORD CCAItemData<TBase, dwDataFormat>::ReadItem(CAOPERATIONHANDLE hOperHandle, CA_ITEM_ID* pID, DWORD dwOptions/* = CA_OPTION_USE_CACHE*/)
{
    ATLASSERT(hOperHandle);

    Empty();
    dwSize = sizeof(TBase);

    return ::CAReadItem(hOperHandle, pID, dwOptions, dwDataFormat, static_cast<TBase*>(this));
}


/////////////////////////////////////////////////////////////////////////////
// CCAFolderInfo 类的实现
//

inline CCAFolderInfo::CCAFolderInfo()
{
    ::ZeroMemory(this, sizeof(CA_FOLDER_INFO));
    dwSize = sizeof(CA_FOLDER_INFO);
}

inline CCAFolderInfo::~CCAFolderInfo()
{
    Empty();
}

inline void CCAFolderInfo::Empty()
{
    if (pstrName != NULL && ::CAFreeFolderInfoStructure(this) == CONA_OK)
        pstrName = NULL;
}

inline DWORD CCAFolderInfo::GetFolderInfo(CAHANDLE hCAHandle)
{
    ATLASSERT(hCAHandle);
    ATLASSERT(pstrName == NULL);

    return ::CAGetFolderInfo(hCAHandle, this);
}


/////////////////////////////////////////////////////////////////////////////
// ICANotifyImpl 类的实现
//

template <typename T>
inline ICANotifyImpl<T>::ICANotifyImpl()
{
}

template <typename T>
inline DWORD ICANotifyImpl<T>::ICANotify_OnNotify(DWORD /*dwReason*/, DWORD /*dwStatus*/, CA_ITEM_ID* /*pItemID*/)
{
    return CONA_OK;
}

template <typename T>
inline DWORD ICANotifyImpl<T>::ICANotify_Register(CAHANDLE hCAHandle)
{
    ATLASSERT(hCAHandle);
    return ::CARegisterNotifyIF(hCAHandle, API_REGISTER, this);
}

template <typename T>
inline DWORD ICANotifyImpl<T>::ICANotify_Unregister(CAHANDLE hCAHandle)
{
    ATLASSERT(hCAHandle);
    return ::CARegisterNotifyIF(hCAHandle, API_UNREGISTER, this);
}

template <typename T>
inline DWORD ICANotifyImpl<T>::OnCANotify(DWORD dwReason, DWORD dwStatus, CA_ITEM_ID* pItemID)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);

    return pThis->ICANotify_OnNotify(dwReason, dwStatus, pItemID);
}


/////////////////////////////////////////////////////////////////////////////
// IFSAPINotifyImpl 类的实现
//

template <typename T>
inline IFSAPINotifyImpl<T>::IFSAPINotifyImpl()
{
}

template <typename T>
inline DWORD IFSAPINotifyImpl<T>::IFSAPINotify_OnFSConnectionLost(DWORD /*dwError*/)
{
    return CONA_OK;
}

template <typename T>
inline DWORD IFSAPINotifyImpl<T>::IFSAPINotify_OnFSOperationState(DWORD dwFSFunction, DWORD dwState, DWORD dwTransferredBytes, DWORD dwAllBytes)
{
    return CONA_OK;
}

template <typename T>
inline DWORD IFSAPINotifyImpl<T>::IFSAPINotify_Register(FSHANDLE hFSHandle)
{
    ATLASSERT(hFSHandle);
    return ::CONARegisterFSNotifyIF(hFSHandle, API_REGISTER, this);
}

template <typename T>
inline DWORD IFSAPINotifyImpl<T>::IFSAPINotify_Unregister(FSHANDLE hFSHandle)
{
    ATLASSERT(hFSHandle);
    return ::CONARegisterFSNotifyIF(hFSHandle, API_UNREGISTER, this);
}

template <typename T>
inline DWORD IFSAPINotifyImpl<T>::OnFSConnectionLost(DWORD dwError)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);

    return pThis->IFSAPINotify_OnFSConnectionLost(dwError);
}

template <typename T>
inline DWORD IFSAPINotifyImpl<T>::OnFSOperationState(DWORD dwFSFunction, DWORD dwState, DWORD dwTransferredBytes, DWORD dwAllBytes)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);

    return pThis->IFSAPINotify_OnFSOperationState(dwFSFunction, dwState, dwTransferredBytes, dwAllBytes);
}


/////////////////////////////////////////////////////////////////////////////
// ICAOperationNotifyImpl 类的实现
//

template <typename T>
inline ICAOperationNotifyImpl<T>::ICAOperationNotifyImpl()
{
}

template <typename T>
inline DWORD ICAOperationNotifyImpl<T>::ICAOperationNotify_OnNotify(DWORD /*dwOperation*/, DWORD /*dwCurrent*/, DWORD /*dwTotalAmount*/, DWORD /*dwStatus*/, CA_ITEM_ID* /*pItemID*/)
{
    return CONA_OK;
}

template <typename T>
inline DWORD ICAOperationNotifyImpl<T>::ICAOperationNotify_Register(CAOPERATIONHANDLE hOperHandle)
{
    ATLASSERT(hOperHandle);
    return ::CARegisterOperationNotifyIF(hOperHandle, API_REGISTER, this);
}

template <typename T>
inline DWORD ICAOperationNotifyImpl<T>::ICAOperationNotify_Unregister(CAOPERATIONHANDLE hOperHandle)
{
    ATLASSERT(hOperHandle);
    return ::CARegisterOperationNotifyIF(hOperHandle, API_UNREGISTER, this);
}

template <typename T>
inline DWORD ICAOperationNotifyImpl<T>::OnCAOperationNotify(DWORD dwOperation, DWORD dwCurrent, DWORD dwTotalAmount, DWORD dwStatus, CA_ITEM_ID* pItemID)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);

    return pThis->ICAOperationNotify_OnNotify(dwOperation, dwCurrent, dwTotalAmount, dwStatus, pItemID);
}


/////////////////////////////////////////////////////////////////////////////
// IDMAPIDeviceNotifyImpl 类的实现
//

template <typename T>
inline IDMAPIDeviceNotifyImpl<T>::IDMAPIDeviceNotifyImpl()
{
}

template <typename T>
inline void IDMAPIDeviceNotifyImpl<T>::IDMAPIDeviceNotify_OnDeviceListUpdated()
{
}

template <typename T>
inline void IDMAPIDeviceNotifyImpl<T>::IDMAPIDeviceNotify_OnDeviceAdded(const WCHAR* /*pszSerialNumber*/, DWORD /*dwStatus*/)
{
}

template <typename T>
inline void IDMAPIDeviceNotifyImpl<T>::IDMAPIDeviceNotify_OnDeviceRemoved(const WCHAR* /*pszSerialNumber*/, DWORD /*dwStatus*/)
{
}

template <typename T>
inline void IDMAPIDeviceNotifyImpl<T>::IDMAPIDeviceNotify_OnDeviceUpdated(const WCHAR* /*pszSerialNumber*/, DWORD /*dwStatus*/)
{
}

template <typename T>
inline DWORD IDMAPIDeviceNotifyImpl<T>::IDMAPIDeviceNotify_Register(DMHANDLE hDMHandle)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);
    ATLASSERT(hDMHandle);

    DWORD dwDeviceCount = 0;
    if (::CONAGetDeviceCount(hDMHandle, &dwDeviceCount) == CONA_OK && dwDeviceCount > 0)
    {
        CDeviceList devices(dwDeviceCount);
        if (devices.GetDevices(hDMHandle) == CONA_OK)
        {
            for (DWORD i = 0; i < dwDeviceCount; ++i)
                pThis->IDMAPIDeviceNotify_OnDeviceAdded(devices.m_pDevices[i].pstrSerialNumber, 0x10010001);
        }
    }

    return ::CONARegisterDMNotifyIF(hDMHandle, API_REGISTER, this);
}

template <typename T>
inline DWORD IDMAPIDeviceNotifyImpl<T>::IDMAPIDeviceNotify_Unregister(DMHANDLE hDMHandle)
{
    ATLASSERT(hDMHandle);
    return ::CONARegisterDMNotifyIF(hDMHandle, API_UNREGISTER, this);
}

template <typename T>
inline void IDMAPIDeviceNotifyImpl<T>::OnDeviceListUpdated()
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);

    pThis->IDMAPIDeviceNotify_OnDeviceListUpdated();
}

template <typename T>
inline void IDMAPIDeviceNotifyImpl<T>::OnDeviceAdded(const WCHAR* pszSerialNumber, DWORD dwStatus)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);

    pThis->IDMAPIDeviceNotify_OnDeviceAdded(pszSerialNumber, dwStatus);
}

template <typename T>
inline void IDMAPIDeviceNotifyImpl<T>::OnDeviceRemoved(const WCHAR* pszSerialNumber, DWORD dwStatus)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);

    pThis->IDMAPIDeviceNotify_OnDeviceRemoved(pszSerialNumber, dwStatus);
}

template <typename T>
inline void IDMAPIDeviceNotifyImpl<T>::OnDeviceUpdated(const WCHAR* pszSerialNumber, DWORD dwStatus)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);

    pThis->IDMAPIDeviceNotify_OnDeviceUpdated(pszSerialNumber, dwStatus);
}

}  // namespace symutil

#endif  // __SYMCONUTIL_INL__