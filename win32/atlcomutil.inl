///////////////////////////////////////////////////////////////////////////////
// atlcomutil.inl
//
// Copyright(c) 2019, Garfield. All rights reserved.
// Author  : Garfield
// Version : 3.5
// Creation Date : 2008/3/22

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __ATLCOMUTIL_INL__
#define __ATLCOMUTIL_INL__

#ifndef __ATLCOMUTIL_H__
    #error atlcomutil.inl requires atlcomutil.h to be included first
#endif

namespace atlutil {

///////////////////////////////////////////////////////////////////////////////
// Implementation of the IStreamImpl class
//

template <typename T>
inline STDMETHODIMP IStreamImpl<T>::Read(LPVOID pBuffer, ULONG cbSize, ULONG* pcbRead)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);

    return pThis->IStream_Read(pBuffer, cbSize, pcbRead);
}

template <typename T>
inline STDMETHODIMP IStreamImpl<T>::Write(LPCVOID pBuffer, ULONG cbSize, ULONG* pcbWritten)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);

    return pThis->IStream_Write(pBuffer, cbSize, pcbWritten);
}

template <typename T>
inline STDMETHODIMP IStreamImpl<T>::Revert()
{
    return E_NOTIMPL;
}

template <typename T>
inline STDMETHODIMP IStreamImpl<T>::Clone(IStream** /*ppStream*/)
{
    return E_NOTIMPL;
}

template <typename T>
inline STDMETHODIMP IStreamImpl<T>::Stat(STATSTG* pStat, DWORD dwStatFlag)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);

    return pThis->IStream_Stat(pStat, dwStatFlag);
}

template <typename T>
inline STDMETHODIMP IStreamImpl<T>::LockRegion(ULARGE_INTEGER liOffset, ULARGE_INTEGER cbSize, DWORD dwLockType)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);

    return pThis->IStream_LockRegion(liOffset, cbSize, dwLockType);
}

template <typename T>
inline STDMETHODIMP IStreamImpl<T>::UnlockRegion(ULARGE_INTEGER liOffset, ULARGE_INTEGER cbSize, DWORD dwLockType)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);

    return pThis->IStream_UnlockRegion(liOffset, cbSize, dwLockType);
}

template <typename T>
inline STDMETHODIMP IStreamImpl<T>::Commit(DWORD dwCommitFlags)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);

    return pThis->IStream_Commit(dwCommitFlags);
}

template <typename T>
inline STDMETHODIMP IStreamImpl<T>::SetSize(ULARGE_INTEGER liNewSize)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);

    return pThis->IStream_SetSize(liNewSize);
}

template <typename T>
inline STDMETHODIMP IStreamImpl<T>::Seek(LARGE_INTEGER liMove, DWORD dwOrigin, ULARGE_INTEGER* pNewPosition)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);

    return pThis->IStream_Seek(liMove, dwOrigin, pNewPosition);
}

template <typename T>
inline STDMETHODIMP IStreamImpl<T>::CopyTo(IStream* pStream, ULARGE_INTEGER cbSize, ULARGE_INTEGER* pcbRead, ULARGE_INTEGER* pcbWritten)
{
    ATLASSERT(pStream);

    HRESULT hr = E_OUTOFMEMORY;
    ATL::CTempBuffer<BYTE, 1024> buffer;
    if (LPBYTE pBuffer = buffer.AllocateBytes(cbSize.LowPart))
    {
        hr = Read(pBuffer, cbSize.LowPart, &cbSize.LowPart);
        if (SUCCEEDED(hr))
            hr = pStream->Write(pBuffer, cbSize.LowPart, &cbSize.HighPart);
    }

    if (pcbRead != NULL)
        pcbRead->QuadPart = cbSize.LowPart;

    if (pcbWritten != NULL)
        pcbWritten->QuadPart = cbSize.HighPart;

    return hr;
}

template <typename T>
inline HRESULT IStreamImpl<T>::IStream_Read(LPVOID /*pBuffer*/, ULONG /*cbSize*/, ULONG* /*pcbRead*/)
{
    return E_NOTIMPL;
}

template <typename T>
inline HRESULT IStreamImpl<T>::IStream_Write(LPCVOID /*pBuffer*/, ULONG /*cbSize*/, ULONG* /*pcbWritten*/)
{
    return E_NOTIMPL;
}

template <typename T>
inline HRESULT IStreamImpl<T>::IStream_Commit(DWORD /*dwCommitFlags*/)
{
    return E_NOTIMPL;
}

template <typename T>
inline HRESULT IStreamImpl<T>::IStream_SetSize(ULARGE_INTEGER /*liNewSize*/)
{
    return E_NOTIMPL;
}

template <typename T>
inline HRESULT IStreamImpl<T>::IStream_Stat(STATSTG* /*pStat*/, DWORD /*dwStatFlag*/)
{
    return E_NOTIMPL;
}

template <typename T>
inline HRESULT IStreamImpl<T>::IStream_Seek(LARGE_INTEGER /*liMove*/, DWORD /*dwOrigin*/, ULARGE_INTEGER* /*pNewPosition*/)
{
    return E_NOTIMPL;
}

template <typename T>
inline HRESULT IStreamImpl<T>::IStream_LockRegion(ULARGE_INTEGER /*liOffset*/, ULARGE_INTEGER /*cbSize*/, DWORD /*dwLockType*/)
{
    return E_NOTIMPL;
}

template <typename T>
inline HRESULT IStreamImpl<T>::IStream_UnlockRegion(ULARGE_INTEGER /*liOffset*/, ULARGE_INTEGER /*cbSize*/, DWORD /*dwLockType*/)
{
    return E_NOTIMPL;
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CFileStream class
//

inline CFileStream::CFileStream()
    : m_hFile(INVALID_HANDLE_VALUE)
{
}

inline CFileStream::~CFileStream()
{
    ::CloseHandle(m_hFile);
}

inline HRESULT WINAPI CFileStream::CreateInstance(LPCTSTR pszFileName, IStream** ppStream, DWORD dwDesiredAccess/* = GENERIC_READ*/, DWORD dwShareMode/* = FILE_SHARE_READ*/, DWORD dwCreationDisposition/* = OPEN_EXISTING*/, LPSECURITY_ATTRIBUTES psa/* = NULL*/)
{
    ATLASSERT(ppStream);
    ATLASSERT(pszFileName);

    *ppStream = NULL;
    ATL::CComObject<CFileStream>* pStream = NULL;

    HRESULT hr = ATL::CComObject<CFileStream>::CreateInstance(&pStream);
    if (SUCCEEDED(hr))
    {
        pStream->m_hFile = ::CreateFile(pszFileName, dwDesiredAccess, dwShareMode, psa, dwCreationDisposition, FILE_ATTRIBUTE_NORMAL, NULL);
        if (pStream->m_hFile != INVALID_HANDLE_VALUE)
        {
            pStream->AddRef();
            *ppStream = pStream;
        }
        else
        {
            hr = ATL::AtlHresultFromLastError();
            delete pStream;
        }
    }

    return hr;
}

inline HRESULT CFileStream::IStream_Read(LPVOID pBuffer, ULONG cbSize, ULONG* pcbRead)
{
    ATLASSERT(pBuffer);

    HRESULT hr = (::ReadFile(m_hFile, pBuffer, cbSize, &cbSize, NULL) ? S_OK : ATL::AtlHresultFromLastError());
    if (pcbRead != NULL)
        *pcbRead = cbSize;

    return hr;
}

inline HRESULT CFileStream::IStream_Write(LPCVOID pBuffer, ULONG cbSize, ULONG* pcbWritten)
{
    ATLASSERT(pBuffer);

    HRESULT hr = (::WriteFile(m_hFile, pBuffer, cbSize, &cbSize, NULL) ? S_OK : ATL::AtlHresultFromLastError());
    if (pcbWritten != NULL)
        *pcbWritten = cbSize;

    return hr;
}

inline HRESULT CFileStream::IStream_Commit(DWORD /*dwCommitFlags*/)
{
    return (::FlushFileBuffers(m_hFile) ? S_OK : ATL::AtlHresultFromLastError());
}

inline HRESULT CFileStream::IStream_Stat(STATSTG* pStat, DWORD /*dwStatFlag*/)
{
    ATLASSERT(pStat);

    HRESULT hr = S_OK;
    ::memset(pStat, 0, sizeof(STATSTG));

    pStat->type = STGTY_STREAM;
    pStat->grfLocksSupported = LOCK_WRITE | LOCK_EXCLUSIVE;
    ATLVERIFY(::GetFileTime(m_hFile, &pStat->ctime, &pStat->atime, &pStat->mtime));

    pStat->cbSize.LowPart = ::GetFileSize(m_hFile, &pStat->cbSize.HighPart);
    if (pStat->cbSize.LowPart == INVALID_FILE_SIZE)
        hr = ATL::AtlHresultFromLastError();

    return hr;
}

inline HRESULT CFileStream::IStream_Seek(LARGE_INTEGER liMove, DWORD dwOrigin, ULARGE_INTEGER* pNewPosition)
{
    ATLASSERT(dwOrigin >= STREAM_SEEK_SET && dwOrigin <= STREAM_SEEK_END);

    HRESULT hr = S_OK;
    liMove.LowPart = ::SetFilePointer(m_hFile, liMove.LowPart, &liMove.HighPart, dwOrigin);
    if (liMove.LowPart == INVALID_SET_FILE_POINTER)
        hr = ATL::AtlHresultFromLastError();

    if (pNewPosition != NULL)
        pNewPosition->QuadPart = liMove.QuadPart;

    return hr;
}

inline HRESULT CFileStream::IStream_LockRegion(ULARGE_INTEGER liOffset, ULARGE_INTEGER cbSize, DWORD dwLockType)
{
    OVERLAPPED overlapped = { 0, 0, liOffset.LowPart, liOffset.HighPart };
    return (::LockFileEx(m_hFile, dwLockType & LOCK_EXCLUSIVE, 0, cbSize.LowPart, cbSize.HighPart, &overlapped) ? S_OK : ATL::AtlHresultFromLastError());
}

inline HRESULT CFileStream::IStream_UnlockRegion(ULARGE_INTEGER liOffset, ULARGE_INTEGER cbSize, DWORD /*dwLockType*/)
{
    OVERLAPPED overlapped = { 0, 0, liOffset.LowPart, liOffset.HighPart };
    return (::UnlockFileEx(m_hFile, 0, cbSize.LowPart, cbSize.HighPart, &overlapped) ? S_OK : ATL::AtlHresultFromLastError());
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CMemoryStream class
//

inline CMemoryStream::CMemoryStream()
    : m_pMemory(NULL), m_dwSize(0), m_dwPosition(0)
{
}

inline HRESULT WINAPI CMemoryStream::CreateInstance(LPBYTE pMemory, DWORD cbSize, IStream** ppStream)
{
    ATLASSERT(pMemory);
    ATLASSERT(ppStream);
    ATLASSERT(cbSize > 0);

    *ppStream = NULL;
    ATL::CComObject<CMemoryStream>* pStream = NULL;

    HRESULT hr = ATL::CComObject<CMemoryStream>::CreateInstance(&pStream);
    if (SUCCEEDED(hr))
    {
        pStream->AddRef();
        *ppStream = pStream;

        pStream->m_dwSize  = cbSize;
        pStream->m_pMemory = pMemory;
    }

    return hr;
}

inline HRESULT WINAPI CMemoryStream::CreateInstance(HINSTANCE hInstance, UINT nIDResource, LPCTSTR pType, IStream** ppStream)
{
    ATLASSERT(pType);
    ATLASSERT(ppStream);
    ATLASSERT(hInstance);
    ATLASSERT(nIDResource != 0);

    *ppStream = NULL;
    ATL::CComObject<CMemoryStream>* pStream = NULL;

    HRESULT hr = ATL::CComObject<CMemoryStream>::CreateInstance(&pStream);
    if (SUCCEEDED(hr))
    {
        if (pStream->LoadResource(hInstance, nIDResource, pType))
        {
            pStream->AddRef();
            *ppStream = pStream;
        }
        else
        {
            hr = ATL::AtlHresultFromLastError();
            delete pStream;
        }
    }

    return hr;
}

inline HRESULT CMemoryStream::IStream_Read(LPVOID pBuffer, ULONG cbSize, ULONG* pcbRead)
{
    ATLASSERT(pBuffer);
    ATLASSERT(m_pMemory);
    ATLASSERT(m_dwSize > 0);

    return Copy(pBuffer, m_pMemory + m_dwPosition, cbSize, pcbRead);
}

inline HRESULT CMemoryStream::IStream_Write(LPCVOID pBuffer, ULONG cbSize, ULONG* pcbWritten)
{
    ATLASSERT(pBuffer);
    ATLASSERT(m_pMemory);
    ATLASSERT(m_dwSize > 0);

    return Copy(m_pMemory + m_dwPosition, pBuffer, cbSize, pcbWritten);
}

inline HRESULT CMemoryStream::IStream_Stat(STATSTG* pStat, DWORD /*dwStatFlag*/)
{
    ATLASSERT(pStat);
    ATLASSERT(m_pMemory);

    ::memset(pStat, 0, sizeof(STATSTG));
    pStat->type = STGTY_STREAM;
    pStat->cbSize.QuadPart = m_dwSize;

    return S_OK;
}

inline HRESULT CMemoryStream::IStream_Seek(LARGE_INTEGER liMove, DWORD dwOrigin, ULARGE_INTEGER* pNewPosition)
{
    ATLASSERT(m_pMemory);

    switch (dwOrigin)
    {
    case STREAM_SEEK_SET:
        m_dwPosition = liMove.LowPart;
        break;

    case STREAM_SEEK_CUR:
        m_dwPosition += (LONG)liMove.LowPart;
        break;

    case STREAM_SEEK_END:
        m_dwPosition = m_dwSize + (LONG)liMove.LowPart;
        break;
    }

    m_dwPosition = stdutil::AdjustRange(m_dwPosition, 0, m_dwSize);
    if (pNewPosition != NULL)
        pNewPosition->QuadPart = m_dwPosition;

    return S_OK;
}

inline BOOL CMemoryStream::LoadResource(HINSTANCE hInstance, UINT nIDResource, LPCTSTR pType)
{
    ATLASSERT(pType);
    ATLASSERT(hInstance);
    ATLASSERT(nIDResource != 0);

    if (HRSRC hResource = ::FindResource(hInstance, MAKEINTRESOURCE(nIDResource), pType))
    {
        if ((m_dwSize = ::SizeofResource(hInstance, hResource)) != 0)
            m_pMemory = (LPBYTE)::LockResource(::LoadResource(hInstance, hResource));
    }

    return (m_pMemory != NULL);
}

inline HRESULT CMemoryStream::Copy(LPVOID pDest, LPCVOID pSource, DWORD cbSize, ULONG* pcbSize)
{
    ATLASSERT(pDest);
    ATLASSERT(pSource);
    ATLASSERT(m_dwSize > 0);

    DWORD cbRemain = m_dwSize - m_dwPosition;
    if (cbSize > cbRemain)
        cbSize = cbRemain;

    if (cbSize != 0)
    {
        ::CopyMemory(pDest, pSource, cbSize);
        m_dwPosition += cbSize;
    }

    if (pcbSize != NULL)
        *pcbSize = cbSize;

    return S_OK;
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CopyVariant class
//

template <typename T>
inline HRESULT CopyVariant<T>::copy(VARIANT* pVariant, const CAdapt<CComPtr<T> >* pInterface)
{
    ATLASSERT(pVariant);
    ATLASSERT(pInterface);
    ATLASSERT(pInterface.m_T.p);

    HRESULT hr = pInterface->m_T.QueryInterface(&pVariant->pdispVal);
    if (SUCCEEDED(hr))
    {
        pVariant->vt = VT_DISPATCH;
    }
    else
    {
        hr = pInterface->m_T.QueryInterface(&pVariant->punkVal);
        ATLASSERT(SUCCEEDED(hr));
        pVariant->vt = VT_UNKNOWN;
    }

    return hr;
}

template <typename T>
inline void CopyVariant<T>::init(VARIANT* pVariant)
{
    ATLASSERT(pVariant);
    ::VariantInit(pVariant);
}

template <typename T>
inline void CopyVariant<T>::destroy(VARIANT* pVariant)
{
    if (pVariant != NULL)
        _COM_VERIFY(::VariantClear(pVariant));
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CopyInterface class
//

template <typename T>
inline HRESULT CopyInterface<T>::copy(T** ppInterface, const CAdapt<CComPtr<T> >* pInterface)
{
    ATLASSERT(pInterface);
    ATLASSERT(ppInterface);

    *ppInterface = pInterface->m_T.p;
    if (*ppInterface != NULL)
        (*ppInterface)->AddRef();

    return S_OK;
}

template <typename T>
inline void CopyInterface<T>::init(T** ppInterface)
{
    ATLASSERT(ppInterface);
    *ppInterface = NULL;
}

template <typename T>
inline void CopyInterface<T>::destroy(T** ppInterface)
{
    if (*ppInterface != NULL)
        (*ppInterface)->Release();
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the IUnknownEvent class
//

inline IUnknownEvent::IUnknownEvent()
    : m_dwCookie(INVALID_COOKIE_VALUE)
{
}

inline ULONG STDMETHODCALLTYPE IUnknownEvent::AddRef()
{
    return 1L;
}

inline ULONG STDMETHODCALLTYPE IUnknownEvent::Release()
{
    return 1L;
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the IUnknownEventImpl class
//

template <typename T, UINT nEventID, const IID* piid, const IID* piid2/* = &IID_NULL*/>
inline STDMETHODIMP IUnknownEventImpl<T, nEventID, piid, piid2>::_IUEQueryInterface(REFIID riid, LPVOID* ppvObject)
{
    ATLASSERT(piid);
    ATLASSERT(piid2);
    ATLASSERT(ppvObject);

    HRESULT hr = S_OK;
    if (::InlineIsEqualGUID(riid, *piid) || ::InlineIsEqualGUID(riid, *piid2) || ATL::InlineIsEqualUnknown(riid))
    {
        *ppvObject = this;
        AddRef();
    }
    else
    {
        *ppvObject = NULL;
        hr = E_NOINTERFACE;
    }

    return hr;
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the IContextMenuImpl class
//

template <typename T>
inline STDMETHODIMP IContextMenuImpl<T>::InvokeCommand(LPCMINVOKECOMMANDINFO pCmdInfo)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);

    return pThis->IContextMenu_InvokeCommand(pCmdInfo);
}

template <typename T>
inline STDMETHODIMP IContextMenuImpl<T>::GetCommandString(UINT idCmd, UINT nType, UINT* pReserved, LPSTR pszName, UINT cchMax)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);

    return pThis->IContextMenu_GetCommandString(idCmd, nType, pReserved, pszName, cchMax);
}

template <typename T>
inline STDMETHODIMP IContextMenuImpl<T>::QueryContextMenu(HMENU hMenu, UINT indexMenu, UINT idCmdFirst, UINT idCmdLast, UINT nFlags)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);

    return pThis->IContextMenu_QueryContextMenu(hMenu, indexMenu, idCmdFirst, idCmdLast, nFlags);
}

template <typename T>
inline HRESULT IContextMenuImpl<T>::IContextMenu_InvokeCommand(LPCMINVOKECOMMANDINFO /*pCmdInfo*/)
{
    return E_NOTIMPL;
}

template <typename T>
inline HRESULT IContextMenuImpl<T>::IContextMenu_GetCommandString(UINT /*idCmd*/, UINT /*nType*/, UINT* /*pReserved*/, LPSTR /*pszName*/, UINT /*cchMax*/)
{
    return E_NOTIMPL;
}

template <typename T>
inline HRESULT IContextMenuImpl<T>::IContextMenu_QueryContextMenu(HMENU /*hMenu*/, UINT /*indexMenu*/, UINT /*idCmdFirst*/, UINT /*idCmdLast*/, UINT /*nFlags*/)
{
    return E_NOTIMPL;
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the IObjectWithSiteImpl class
//

template <typename T>
inline STDMETHODIMP IObjectWithSiteImpl<T>::SetSite(IUnknown* pUnkSite)
{
    m_spUnkSite = pUnkSite;
    return S_OK;
}

template <typename T>
inline STDMETHODIMP IObjectWithSiteImpl<T>::GetSite(REFIID riid, LPVOID* ppvSite)
{
    ATLASSERT(ppvSite);
    return (m_spUnkSite != NULL ? m_spUnkSite->QueryInterface(riid, ppvSite) : (*ppvSite = NULL, E_NOINTERFACE));
}


#if defined(__shobjidl_h__) && defined(__SYSUTIL_H__)
///////////////////////////////////////////////////////////////////////////////
// Implementation of the IShellPropSheetImpl class
//

template <typename T>
inline HRESULT WINAPI IShellPropSheetImpl<T>::IShellPropSheet_RegisterServer(LPCTSTR pszFileType)
{
    ATLASSERT(pszFileType);

    OLECHAR szClsID[64];
    ATLVERIFY(::StringFromGUID2(T::GetObjectCLSID(), szClsID, _countof(szClsID)));

    TCHAR szBuffer[MAX_PATH];
    ATLVERIFY(::GetModuleFileName(ATL::_AtlBaseModule.GetModuleInstance(), szBuffer, _countof(szBuffer)));

    HRESULT hr = sysutil::CoRegisterServer(szBuffer, szClsID, NULL, _T("Apartment"));
    if (SUCCEEDED(hr))
    {
        HKEY hKey = NULL;
        ::_tssprintf(szBuffer, _countof(szBuffer), _T("%s\\shellex\\PropertySheetHandlers\\%s"), pszFileType, szClsID);

        LONG lResult = ::RegCreateKeyEx(HKEY_CLASSES_ROOT, szBuffer, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, NULL);
        if (lResult == ERROR_SUCCESS)
            lResult = ::RegCloseKey(hKey);

        hr = HRESULT_FROM_WIN32(lResult);
    }

    return hr;
}

template <typename T>
inline HRESULT WINAPI IShellPropSheetImpl<T>::IShellPropSheet_UnregisterServer(LPCTSTR pszFileType)
{
    ATLASSERT(pszFileType);

    OLECHAR szClsID[64];
    ATLVERIFY(::StringFromGUID2(T::GetObjectCLSID(), szClsID, _countof(szClsID)));

    HRESULT hr = sysutil::CoUnregisterServer(szClsID);
    if (SUCCEEDED(hr))
    {
        TCHAR szSubKey[MAX_PATH];
        ::_tssprintf(szSubKey, _countof(szSubKey), _T("%s\\shellex\\PropertySheetHandlers\\%s"), pszFileType, szClsID);

        hr = HRESULT_FROM_WIN32(::RegDeleteKey(HKEY_CLASSES_ROOT, szSubKey));
    }

    return hr;
}

template <typename T>
inline STDMETHODIMP IShellPropSheetImpl<T>::Initialize(LPCITEMIDLIST /*pidlFolder*/, IDataObject* pDataObject, HKEY hKeyProgID)
{
    ATLASSERT(pDataObject);

    STGMEDIUM stg;
    FORMATETC etc = { CF_HDROP, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };

    HRESULT hr = pDataObject->GetData(&etc, &stg);
    if (SUCCEEDED(hr))
    {
        T* pThis = static_cast<T*>(this);
        ATLASSERT(pThis);

        hr = pThis->IShellPropSheet_Initialize((HDROP)stg.hGlobal, hKeyProgID);
        ::ReleaseStgMedium(&stg);
    }

    return hr;
}

template <typename T>
inline STDMETHODIMP IShellPropSheetImpl<T>::AddPages(LPFNSVADDPROPSHEETPAGE pfnAddPage, LPARAM lParam)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);

    return pThis->IShellPropSheet_AddPages(pfnAddPage, lParam);
}

template <typename T>
inline STDMETHODIMP IShellPropSheetImpl<T>::ReplacePage(EXPPS /*nPageID*/, LPFNSVADDPROPSHEETPAGE /*pfnReplacePage*/, LPARAM /*lParam*/)
{
    return E_NOTIMPL;
}

template <typename T>
inline HRESULT IShellPropSheetImpl<T>::IShellPropSheet_Initialize(HDROP /*hDrop*/, HKEY /*hKeyProgID*/)
{
    return E_NOTIMPL;
}

template <typename T>
inline HRESULT IShellPropSheetImpl<T>::IShellPropSheet_AddPages(LPFNSVADDPROPSHEETPAGE /*pfnAddPage*/, LPARAM /*lParam*/)
{
    return E_NOTIMPL;
}
#endif  // defined(__shobjidl_h__) && defined(__SYSUTIL_H__)


#ifdef __propcat_h__
///////////////////////////////////////////////////////////////////////////////
// Implementation of the ICategorizePropertiesImpl class
//

template <typename T>
inline STDMETHODIMP ICategorizePropertiesImpl<T>::MapPropertyToCategory(DISPID dispid, PROPCAT* pPropcat)
{
    if (pPropcat == NULL)
        return E_INVALIDARG;

    UINT nCount = 0;
    const _ATL_PROPCAT_ENTRY* pEntries = T::_GetPropcatMap(nCount);
    ATLASSERT(pEntries != NULL && nCount > 0);

    for (UINT i = 0; i < nCount; ++i)
    {
        if (pEntries[i].pszName == NULL && pEntries[i].dispid == dispid)
        {
            *pPropcat = pEntries[i].propcat;
            return S_OK;
        }
    }

    return E_FAIL;
}

template <typename T>
inline STDMETHODIMP ICategorizePropertiesImpl<T>::GetCategoryName(PROPCAT propcat, LCID lcid, BSTR* pbstrName)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);

    return pThis->ICategorizeProperties_GetCategoryName(propcat, lcid, pbstrName);
}

template <typename T>
inline HRESULT ICategorizePropertiesImpl<T>::ICategorizeProperties_GetCategoryName(PROPCAT propcat, LCID /*lcid*/, BSTR* pbstrName)
{
    if (pbstrName == NULL)
        return E_INVALIDARG;

    UINT nCount = 0;
    const _ATL_PROPCAT_ENTRY* pEntries = T::_GetPropcatMap(nCount);
    ATLASSERT(pEntries != NULL && nCount > 0);

    for (UINT i = 0; i < nCount; ++i)
    {
        if (pEntries[i].pszName != NULL && pEntries[i].propcat == propcat)
        {
            *pbstrName = ::SysAllocString(pEntries[i].pszName);
            return (*pbstrName == NULL ? E_OUTOFMEMORY : S_OK);
        }
    }

    return E_FAIL;
}
#endif  // __propcat_h__


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CComContainerObject class
//

template <typename _Container, typename ThreadModel>
inline HRESULT CComContainerObject<_Container, ThreadModel>::Copy(const _Container& container)
{
    m_container = container;
    return S_OK;
}

template <typename _Container, typename ThreadModel>
inline HRESULT WINAPI CComContainerObject<_Container, ThreadModel>::CreateInstance(ATL::CComObject<CComContainerObject<_Container, ThreadModel> >** ppObject, const _Container& container)
{
    HRESULT hr = ATL::CComObject<CComContainerObject<_Container, ThreadModel> >::CreateInstance(ppObject);
    if (SUCCEEDED(hr))
        (*ppObject)->m_container = container;

    return hr;
}

}  // namespace atlutil

#endif  // __ATLCOMUTIL_INL__