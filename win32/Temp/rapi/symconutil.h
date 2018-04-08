///////////////////////////////////////////////////////////////////////////////
// symconutil.h : ͷ�ļ�
//
// Copyright(c) 2012, Garfield. All rights reserved.
// Author  : Garfield
// Version : 1.0
// Finish Time : 2009/11/5

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __SYMCONUTIL_H__
#define __SYMCONUTIL_H__

#ifndef _CONAFILESYSTEM_H_
    #error symconutil.h requires CONAFileSystem.h to be included first
#endif

#ifndef __DACONTENTACCESSAPI_H__
    #error symconutil.h requires DAContentAccess.h to be included first
#endif

#ifndef _COMMONFUNCTIONSAPI_H_
    #error symconutil.h requires CommonFunctionsAPI.h to be included first
#endif

#ifndef _CONADEVICEMANAGEMENT_H_
    #error symconutil.h requires CONADeviceManagement.h to be included first
#endif

#ifndef _CONAAPPLICATIONINSTALLATION_H_
    #error symconutil.h requires CONAApplicationInstallation.h to be included first
#endif

///////////////////////////////////////////////////////////////////////////////
// Classes in this file:
//
// CDevice
// CFileData
// CCAIDList
// CFileInfo
// CFolderInfo
// CFindFile
// CFindFolder
// CDeviceList
// CAppInfoList
// CMemoryStatus
// CCADataMsg
// CCADataMms
// CCADataNote
// CCADataVersit
// CCADataContact
// CCADataCalendar
// CCADataBookmark
// CCAFolderInfo
// CDeviceIconInfo
// CDeviceGeneralInfo
// CDeviceProductInfo
// CDevicePropertiesInfo
// ICANotifyImpl<T>
// IFSAPINotifyImpl<T>
// ICAOperationNotifyImpl<T>
// IDMAPIDeviceNotifyImpl<T>

namespace symutil {

/////////////////////////////////////////////////////////////////////////////
// CDevice ��Ķ���
//

class CDevice : public CONAPI_DEVICE
{
// ����/����
public:
    CDevice();
    ~CDevice();

// ����
public:
    void Empty();
    DWORD GetDevice(DMHANDLE hDMHandle, const WCHAR* pszSerialNumber);

// ʵ��
private:
    CDevice(const CDevice& that);
    CDevice& operator=(const CDevice& that);
};


/////////////////////////////////////////////////////////////////////////////
// CFileData ��Ķ���
//

class CFileData
{
// ����/����
public:
    CFileData();
    ~CFileData();

// ����
public:
    void Empty();
    unsigned char** GetAddress();

// ��Ա
public:
    unsigned char* m_pFileData;
};


/////////////////////////////////////////////////////////////////////////////
// CCAIDList ��Ķ���
//

class CCAIDList : public CA_ID_LIST
{
// ����/����
public:
    CCAIDList();
    ~CCAIDList();

// ����
public:
    void Empty();

    DWORD CACommitOperations(CAOPERATIONHANDLE hOperHandle);
    DWORD GetIDList(CAHANDLE hCAHandle, DWORD dwFolderID, DWORD dwOptions = CA_OPTION_USE_CACHE);

// ʵ��
private:
    CCAIDList(const CCAIDList& that);
    CCAIDList& operator=(const CCAIDList& that);
};


/////////////////////////////////////////////////////////////////////////////
// CFileInfo ��Ķ���
//

class CFileInfo : public CONAPI_FILE_INFO
{
// ����/����
public:
    CFileInfo();
    ~CFileInfo();

// ����
public:
    operator LPCONAPI_FILE_INFO();
    operator const CONAPI_FILE_INFO*() const;

    void Empty();
    DWORD GetFileInfo(FSHANDLE hFSHandle, const WCHAR* pszFileName, const WCHAR* pszPath = NULL);

// ����
public:
    const WCHAR* SetFileName(const WCHAR* pszFileName);
    const WCHAR* SetMimeType(const WCHAR* pszMimeType);

// ʵ��
private:
    CFileInfo(const CFileInfo& that);
    CFileInfo& operator=(const CFileInfo& that);
};


/////////////////////////////////////////////////////////////////////////////
// CFolderInfo ��Ķ���
//

class CFolderInfo : public CONAPI_FOLDER_CONTENT
{
// ����/����
public:
    CFolderInfo();
    ~CFolderInfo();

// ����
public:
    void Empty();
    DWORD GetFolderInfo(FSHANDLE hFSHandle, const WCHAR* pszFolderName, const WCHAR* pszPath = NULL, DWORD dwOptions = CONA_GET_FOLDER_INFO, PFN_CONA_FS_FOLDERINFO_CALLBACK pfnFolderInfoNotify = NULL);

// ʵ��
private:
    CFolderInfo(const CFolderInfo& that);
    CFolderInfo& operator=(const CFolderInfo& that);
};


/////////////////////////////////////////////////////////////////////////////
// CFindFile ��Ķ���
//

class CFindFile : public CONAPI_FILE_INFO
{
// ����/����
public:
    CFindFile();
    ~CFindFile();

// ����
public:
    void Close();
    operator FINDHANDLE() const;

    BOOL FindNext();
    DWORD FindFirst(FSHANDLE hFSHandle, const WCHAR* pszPath = NULL, DWORD dwFindOptions = 0);

// ʵ��
private:
    CFindFile(const CFindFile& that);
    CFindFile& operator=(const CFindFile& that);

// ��Ա
public:
    FINDHANDLE m_hFind;
};


/////////////////////////////////////////////////////////////////////////////
// CFindFolder ��Ķ���
//

class CFindFolder : public CONAPI_FOLDER_INFO
{
// ����/����
public:
    CFindFolder();
    ~CFindFolder();

// ����
public:
    void Close();
    operator FINDHANDLE() const;

    BOOL FindNext();
    DWORD FindFirst(FSHANDLE hFSHandle, const WCHAR* pszPath = NULL, DWORD dwFindOptions = 0);

// ʵ��
private:
    CFindFolder(const CFindFolder& that);
    CFindFolder& operator=(const CFindFolder& that);

// ��Ա
public:
    FINDHANDLE m_hFind;
};


/////////////////////////////////////////////////////////////////////////////
// CDeviceInfo ��Ķ���
//

template <typename _Ty, DWORD dwType>
class CDeviceInfo
{
// ����/����
public:
    CDeviceInfo();
    ~CDeviceInfo();

// ����
public:
    void Empty();
    DWORD GetInfo(DMHANDLE hDMHandle, const WCHAR* pszSerialNumber);

// ʵ��
private:
    CDeviceInfo(const CDeviceInfo& that);
    CDeviceInfo& operator=(const CDeviceInfo& that);

// ��Ա
public:
    _Ty* m_pInfo;
};


/////////////////////////////////////////////////////////////////////////////
// CDeviceList ��Ķ���
//

class CDeviceList
{
// ����/����
public:
    explicit CDeviceList(DWORD dwDeviceCount);
    ~CDeviceList();

// ����
public:
    void Empty();
    DWORD GetDevices(DMHANDLE hDMHandle);

// ʵ��
private:
    CDeviceList(const CDeviceList& that);
    CDeviceList& operator=(const CDeviceList& that);

// ��Ա
public:
    DWORD m_dwDeviceCount;
    CONAPI_DEVICE* m_pDevices;
};


/////////////////////////////////////////////////////////////////////////////
// CAppInfoList ��Ķ���
//

class CAppInfoList
{
// ����/����
public:
    CAppInfoList();
    ~CAppInfoList();

// ����
public:
    void Empty();
    DWORD GetAppInfo(FSHANDLE hFSHandle, DWORD dwOptions = CONA_LIST_ALL_APPICATIONS);

// ʵ��
private:
    CAppInfoList(const CAppInfoList& that);
    CAppInfoList& operator=(const CAppInfoList& that);

// ��Ա
public:
    DWORD m_dwCount;
    CONAPI_APPLICATION_INFO* m_pAppInfo;
};


/////////////////////////////////////////////////////////////////////////////
// CMemoryStatus ��Ķ���
//

class CMemoryStatus
{
// ����
public:
    DWORD GetMemoryStatus(FSHANDLE hFSHandle, const WCHAR* pszMemoryType);

// ��Ա
public:
    __int64 nFreeBytes;
    __int64 nUsedBytes;
    __int64 nTotalBytes;
};


/////////////////////////////////////////////////////////////////////////////
// CCAItemData ��Ķ���
//

template <typename TBase, DWORD dwDataFormat>
class CCAItemData : public TBase
{
// ����/����
public:
    explicit CCAItemData(CAHANDLE hCAHandle);
    ~CCAItemData();

// ����
public:
    void Empty();
    DWORD ReadItem(CAOPERATIONHANDLE hOperHandle, CA_ITEM_ID* pID, DWORD dwOptions = CA_OPTION_USE_CACHE);

// ʵ��
private:
    CCAItemData(const CCAItemData& that);
    CCAItemData& operator=(const CCAItemData& that);

// ��Ա
public:
    CAHANDLE m_hCAHandle;
};


/////////////////////////////////////////////////////////////////////////////
// CCAFolderInfo ��Ķ���
//

class CCAFolderInfo : public CA_FOLDER_INFO
{
// ����/����
public:
    CCAFolderInfo();
    ~CCAFolderInfo();

// ����
public:
    void Empty();
    DWORD GetFolderInfo(CAHANDLE hCAHandle);

// ʵ��
private:
    CCAFolderInfo(const CCAFolderInfo& that);
    CCAFolderInfo& operator=(const CCAFolderInfo& that);
};


/////////////////////////////////////////////////////////////////////////////
// ICANotifyImpl ��Ķ���
//

template <typename T>
class ATL_NO_VTABLE ICANotifyImpl : public ICANotify
{
// ����
public:
    ICANotifyImpl();

// ��д
public:
    DWORD ICANotify_OnNotify(DWORD dwReason, DWORD dwStatus, CA_ITEM_ID* pItemID);

// ����
public:
    DWORD ICANotify_Register(CAHANDLE hCAHandle);
    DWORD ICANotify_Unregister(CAHANDLE hCAHandle);

// ʵ��
public:
    virtual DWORD OnCANotify(DWORD dwReason, DWORD dwStatus, CA_ITEM_ID* pItemID);
};


/////////////////////////////////////////////////////////////////////////////
// IFSAPINotifyImpl ��Ķ���
//

template <typename T>
class ATL_NO_VTABLE IFSAPINotifyImpl : public IFSAPINotify
{
// ����
public:
    IFSAPINotifyImpl();

// ��д
public:
    DWORD IFSAPINotify_OnFSConnectionLost(DWORD dwError);
    DWORD IFSAPINotify_OnFSOperationState(DWORD dwFSFunction, DWORD dwState, DWORD dwTransferredBytes, DWORD dwAllBytes);

// ����
public:
    DWORD IFSAPINotify_Register(FSHANDLE hFSHandle);
    DWORD IFSAPINotify_Unregister(FSHANDLE hFSHandle);

// ʵ��
public:
    virtual DWORD OnFSConnectionLost(DWORD dwError);
    virtual DWORD OnFSOperationState(DWORD dwFSFunction, DWORD dwState, DWORD dwTransferredBytes, DWORD dwAllBytes);
};


/////////////////////////////////////////////////////////////////////////////
// ICAOperationNotifyImpl ��Ķ���
//

template <typename T>
class ATL_NO_VTABLE ICAOperationNotifyImpl : public ICAOperationNotify
{
// ����
public:
    ICAOperationNotifyImpl();

// ��д
public:
    DWORD ICAOperationNotify_OnNotify(DWORD dwOperation, DWORD dwCurrent, DWORD dwTotalAmount, DWORD dwStatus, CA_ITEM_ID* pItemID);

// ����
public:
    DWORD ICAOperationNotify_Register(CAOPERATIONHANDLE hOperHandle);
    DWORD ICAOperationNotify_Unregister(CAOPERATIONHANDLE hOperHandle);

// ʵ��
public:
    virtual DWORD OnCAOperationNotify(DWORD dwOperation, DWORD dwCurrent, DWORD dwTotalAmount, DWORD dwStatus, CA_ITEM_ID* pItemID);
};


/////////////////////////////////////////////////////////////////////////////
// IDMAPIDeviceNotifyImpl ��Ķ���
//

template <typename T>
class ATL_NO_VTABLE IDMAPIDeviceNotifyImpl : public IDMAPIDeviceNotify
{
// ����
public:
    IDMAPIDeviceNotifyImpl();

// ��д
public:
    void IDMAPIDeviceNotify_OnDeviceListUpdated();
    void IDMAPIDeviceNotify_OnDeviceAdded(const WCHAR* pszSerialNumber, DWORD dwStatus);
    void IDMAPIDeviceNotify_OnDeviceRemoved(const WCHAR* pszSerialNumber, DWORD dwStatus);
    void IDMAPIDeviceNotify_OnDeviceUpdated(const WCHAR* pszSerialNumber, DWORD dwStatus);

// ����
public:
    DWORD IDMAPIDeviceNotify_Register(DMHANDLE hDMHandle);
    DWORD IDMAPIDeviceNotify_Unregister(DMHANDLE hDMHandle);

// ʵ��
public:
    virtual void OnDeviceListUpdated();
    virtual void OnDeviceAdded(const WCHAR* pszSerialNumber, DWORD dwStatus);
    virtual void OnDeviceRemoved(const WCHAR* pszSerialNumber, DWORD dwStatus);
    virtual void OnDeviceUpdated(const WCHAR* pszSerialNumber, DWORD dwStatus);
};

}  // namespace symutil

#include "symconutil.inl"

#endif  // __SYMCONUTIL_H__