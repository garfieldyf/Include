///////////////////////////////////////////////////////////////////////////////
// symconutil.h : 头文件
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
// CDevice 类的定义
//

class CDevice : public CONAPI_DEVICE
{
// 构造/销毁
public:
    CDevice();
    ~CDevice();

// 操作
public:
    void Empty();
    DWORD GetDevice(DMHANDLE hDMHandle, const WCHAR* pszSerialNumber);

// 实现
private:
    CDevice(const CDevice& that);
    CDevice& operator=(const CDevice& that);
};


/////////////////////////////////////////////////////////////////////////////
// CFileData 类的定义
//

class CFileData
{
// 构造/销毁
public:
    CFileData();
    ~CFileData();

// 操作
public:
    void Empty();
    unsigned char** GetAddress();

// 成员
public:
    unsigned char* m_pFileData;
};


/////////////////////////////////////////////////////////////////////////////
// CCAIDList 类的定义
//

class CCAIDList : public CA_ID_LIST
{
// 构造/销毁
public:
    CCAIDList();
    ~CCAIDList();

// 操作
public:
    void Empty();

    DWORD CACommitOperations(CAOPERATIONHANDLE hOperHandle);
    DWORD GetIDList(CAHANDLE hCAHandle, DWORD dwFolderID, DWORD dwOptions = CA_OPTION_USE_CACHE);

// 实现
private:
    CCAIDList(const CCAIDList& that);
    CCAIDList& operator=(const CCAIDList& that);
};


/////////////////////////////////////////////////////////////////////////////
// CFileInfo 类的定义
//

class CFileInfo : public CONAPI_FILE_INFO
{
// 构造/销毁
public:
    CFileInfo();
    ~CFileInfo();

// 操作
public:
    operator LPCONAPI_FILE_INFO();
    operator const CONAPI_FILE_INFO*() const;

    void Empty();
    DWORD GetFileInfo(FSHANDLE hFSHandle, const WCHAR* pszFileName, const WCHAR* pszPath = NULL);

// 属性
public:
    const WCHAR* SetFileName(const WCHAR* pszFileName);
    const WCHAR* SetMimeType(const WCHAR* pszMimeType);

// 实现
private:
    CFileInfo(const CFileInfo& that);
    CFileInfo& operator=(const CFileInfo& that);
};


/////////////////////////////////////////////////////////////////////////////
// CFolderInfo 类的定义
//

class CFolderInfo : public CONAPI_FOLDER_CONTENT
{
// 构造/销毁
public:
    CFolderInfo();
    ~CFolderInfo();

// 操作
public:
    void Empty();
    DWORD GetFolderInfo(FSHANDLE hFSHandle, const WCHAR* pszFolderName, const WCHAR* pszPath = NULL, DWORD dwOptions = CONA_GET_FOLDER_INFO, PFN_CONA_FS_FOLDERINFO_CALLBACK pfnFolderInfoNotify = NULL);

// 实现
private:
    CFolderInfo(const CFolderInfo& that);
    CFolderInfo& operator=(const CFolderInfo& that);
};


/////////////////////////////////////////////////////////////////////////////
// CFindFile 类的定义
//

class CFindFile : public CONAPI_FILE_INFO
{
// 构造/销毁
public:
    CFindFile();
    ~CFindFile();

// 操作
public:
    void Close();
    operator FINDHANDLE() const;

    BOOL FindNext();
    DWORD FindFirst(FSHANDLE hFSHandle, const WCHAR* pszPath = NULL, DWORD dwFindOptions = 0);

// 实现
private:
    CFindFile(const CFindFile& that);
    CFindFile& operator=(const CFindFile& that);

// 成员
public:
    FINDHANDLE m_hFind;
};


/////////////////////////////////////////////////////////////////////////////
// CFindFolder 类的定义
//

class CFindFolder : public CONAPI_FOLDER_INFO
{
// 构造/销毁
public:
    CFindFolder();
    ~CFindFolder();

// 操作
public:
    void Close();
    operator FINDHANDLE() const;

    BOOL FindNext();
    DWORD FindFirst(FSHANDLE hFSHandle, const WCHAR* pszPath = NULL, DWORD dwFindOptions = 0);

// 实现
private:
    CFindFolder(const CFindFolder& that);
    CFindFolder& operator=(const CFindFolder& that);

// 成员
public:
    FINDHANDLE m_hFind;
};


/////////////////////////////////////////////////////////////////////////////
// CDeviceInfo 类的定义
//

template <typename _Ty, DWORD dwType>
class CDeviceInfo
{
// 构造/销毁
public:
    CDeviceInfo();
    ~CDeviceInfo();

// 操作
public:
    void Empty();
    DWORD GetInfo(DMHANDLE hDMHandle, const WCHAR* pszSerialNumber);

// 实现
private:
    CDeviceInfo(const CDeviceInfo& that);
    CDeviceInfo& operator=(const CDeviceInfo& that);

// 成员
public:
    _Ty* m_pInfo;
};


/////////////////////////////////////////////////////////////////////////////
// CDeviceList 类的定义
//

class CDeviceList
{
// 构造/销毁
public:
    explicit CDeviceList(DWORD dwDeviceCount);
    ~CDeviceList();

// 操作
public:
    void Empty();
    DWORD GetDevices(DMHANDLE hDMHandle);

// 实现
private:
    CDeviceList(const CDeviceList& that);
    CDeviceList& operator=(const CDeviceList& that);

// 成员
public:
    DWORD m_dwDeviceCount;
    CONAPI_DEVICE* m_pDevices;
};


/////////////////////////////////////////////////////////////////////////////
// CAppInfoList 类的定义
//

class CAppInfoList
{
// 构造/销毁
public:
    CAppInfoList();
    ~CAppInfoList();

// 操作
public:
    void Empty();
    DWORD GetAppInfo(FSHANDLE hFSHandle, DWORD dwOptions = CONA_LIST_ALL_APPICATIONS);

// 实现
private:
    CAppInfoList(const CAppInfoList& that);
    CAppInfoList& operator=(const CAppInfoList& that);

// 成员
public:
    DWORD m_dwCount;
    CONAPI_APPLICATION_INFO* m_pAppInfo;
};


/////////////////////////////////////////////////////////////////////////////
// CMemoryStatus 类的定义
//

class CMemoryStatus
{
// 操作
public:
    DWORD GetMemoryStatus(FSHANDLE hFSHandle, const WCHAR* pszMemoryType);

// 成员
public:
    __int64 nFreeBytes;
    __int64 nUsedBytes;
    __int64 nTotalBytes;
};


/////////////////////////////////////////////////////////////////////////////
// CCAItemData 类的定义
//

template <typename TBase, DWORD dwDataFormat>
class CCAItemData : public TBase
{
// 构造/销毁
public:
    explicit CCAItemData(CAHANDLE hCAHandle);
    ~CCAItemData();

// 操作
public:
    void Empty();
    DWORD ReadItem(CAOPERATIONHANDLE hOperHandle, CA_ITEM_ID* pID, DWORD dwOptions = CA_OPTION_USE_CACHE);

// 实现
private:
    CCAItemData(const CCAItemData& that);
    CCAItemData& operator=(const CCAItemData& that);

// 成员
public:
    CAHANDLE m_hCAHandle;
};


/////////////////////////////////////////////////////////////////////////////
// CCAFolderInfo 类的定义
//

class CCAFolderInfo : public CA_FOLDER_INFO
{
// 构造/销毁
public:
    CCAFolderInfo();
    ~CCAFolderInfo();

// 操作
public:
    void Empty();
    DWORD GetFolderInfo(CAHANDLE hCAHandle);

// 实现
private:
    CCAFolderInfo(const CCAFolderInfo& that);
    CCAFolderInfo& operator=(const CCAFolderInfo& that);
};


/////////////////////////////////////////////////////////////////////////////
// ICANotifyImpl 类的定义
//

template <typename T>
class ATL_NO_VTABLE ICANotifyImpl : public ICANotify
{
// 构造
public:
    ICANotifyImpl();

// 重写
public:
    DWORD ICANotify_OnNotify(DWORD dwReason, DWORD dwStatus, CA_ITEM_ID* pItemID);

// 操作
public:
    DWORD ICANotify_Register(CAHANDLE hCAHandle);
    DWORD ICANotify_Unregister(CAHANDLE hCAHandle);

// 实现
public:
    virtual DWORD OnCANotify(DWORD dwReason, DWORD dwStatus, CA_ITEM_ID* pItemID);
};


/////////////////////////////////////////////////////////////////////////////
// IFSAPINotifyImpl 类的定义
//

template <typename T>
class ATL_NO_VTABLE IFSAPINotifyImpl : public IFSAPINotify
{
// 构造
public:
    IFSAPINotifyImpl();

// 重写
public:
    DWORD IFSAPINotify_OnFSConnectionLost(DWORD dwError);
    DWORD IFSAPINotify_OnFSOperationState(DWORD dwFSFunction, DWORD dwState, DWORD dwTransferredBytes, DWORD dwAllBytes);

// 操作
public:
    DWORD IFSAPINotify_Register(FSHANDLE hFSHandle);
    DWORD IFSAPINotify_Unregister(FSHANDLE hFSHandle);

// 实现
public:
    virtual DWORD OnFSConnectionLost(DWORD dwError);
    virtual DWORD OnFSOperationState(DWORD dwFSFunction, DWORD dwState, DWORD dwTransferredBytes, DWORD dwAllBytes);
};


/////////////////////////////////////////////////////////////////////////////
// ICAOperationNotifyImpl 类的定义
//

template <typename T>
class ATL_NO_VTABLE ICAOperationNotifyImpl : public ICAOperationNotify
{
// 构造
public:
    ICAOperationNotifyImpl();

// 重写
public:
    DWORD ICAOperationNotify_OnNotify(DWORD dwOperation, DWORD dwCurrent, DWORD dwTotalAmount, DWORD dwStatus, CA_ITEM_ID* pItemID);

// 操作
public:
    DWORD ICAOperationNotify_Register(CAOPERATIONHANDLE hOperHandle);
    DWORD ICAOperationNotify_Unregister(CAOPERATIONHANDLE hOperHandle);

// 实现
public:
    virtual DWORD OnCAOperationNotify(DWORD dwOperation, DWORD dwCurrent, DWORD dwTotalAmount, DWORD dwStatus, CA_ITEM_ID* pItemID);
};


/////////////////////////////////////////////////////////////////////////////
// IDMAPIDeviceNotifyImpl 类的定义
//

template <typename T>
class ATL_NO_VTABLE IDMAPIDeviceNotifyImpl : public IDMAPIDeviceNotify
{
// 构造
public:
    IDMAPIDeviceNotifyImpl();

// 重写
public:
    void IDMAPIDeviceNotify_OnDeviceListUpdated();
    void IDMAPIDeviceNotify_OnDeviceAdded(const WCHAR* pszSerialNumber, DWORD dwStatus);
    void IDMAPIDeviceNotify_OnDeviceRemoved(const WCHAR* pszSerialNumber, DWORD dwStatus);
    void IDMAPIDeviceNotify_OnDeviceUpdated(const WCHAR* pszSerialNumber, DWORD dwStatus);

// 操作
public:
    DWORD IDMAPIDeviceNotify_Register(DMHANDLE hDMHandle);
    DWORD IDMAPIDeviceNotify_Unregister(DMHANDLE hDMHandle);

// 实现
public:
    virtual void OnDeviceListUpdated();
    virtual void OnDeviceAdded(const WCHAR* pszSerialNumber, DWORD dwStatus);
    virtual void OnDeviceRemoved(const WCHAR* pszSerialNumber, DWORD dwStatus);
    virtual void OnDeviceUpdated(const WCHAR* pszSerialNumber, DWORD dwStatus);
};

}  // namespace symutil

#include "symconutil.inl"

#endif  // __SYMCONUTIL_H__