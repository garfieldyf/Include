///////////////////////////////////////////////////////////////////////////////
// atlcomutil.h
//
// Copyright(c) 2019, Garfield. All rights reserved.
// Author  : Garfield
// Version : 3.5
// Creation Date : 2008/3/22

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __ATLCOMUTIL_H__
#define __ATLCOMUTIL_H__

#include "stdutil.h"
#include <atlcom.h>
#include <atlcomcli.h>

///////////////////////////////////////////////////////////////////////////////
// Global functions in this file:
//
// _AtlStringLength()
// _AtlSetWindowText()
// _AtlGetWindowText()
// _AtlGetLBText()
// _AtlFloatParse()
// _AtlCBSelectString()
// _AtlLBSelectString()
// _AtlCBSelectStringExact()
// _AtlLBSelectStringExact()
//
// Classes in this file:
//
// IStreamImpl<T>
// CFileStream
// CMemoryStream
// CopyVariant<T>
// CopyInterface<T>
// IContextMenuImpl<T>
// IObjectWithSiteImpl<T>
// IShellPropSheetImpl<T>
// ICategorizePropertiesImpl<T>
// CComContainerObject<_Container, ThreadModel>
// IUnknownEventImpl<T, UINT, const IID*, const IID*>

namespace atlutil {

///////////////////////////////////////////////////////////////////////////////
// Global functions
//

inline UINT WINAPI _AtlStringLength(BSTR bstrSrc)
{
    return ::SysStringLen(bstrSrc);
}

#if defined(__ATLSTR_H__) || defined(__AFXSTR_H__)
inline UINT WINAPI _AtlStringLength(const CString& strSrc)
{
    return strSrc.GetLength();
}
#endif  // defined(__ATLSTR_H__) || defined(__AFXSTR_H__)

inline BOOL WINAPI _AtlSetWindowText(HWND hWnd, LPCWSTR pszText)
{
    ATLASSERT(::IsWindow(hWnd));
    return ::SetWindowTextW(hWnd, pszText);
}

inline BOOL WINAPI _AtlSetWindowText(HWND hWnd, LPCSTR pszText)
{
    ATLASSERT(::IsWindow(hWnd));
    return ::SetWindowTextA(hWnd, pszText);
}

inline BOOL WINAPI _AtlGetWindowText(HWND hWnd, BSTR& bstrText)
{
    ATLASSERT(::IsWindow(hWnd));
    ::SysFreeString(bstrText);
    bstrText = NULL;

    int nLength = ::GetWindowTextLengthW(hWnd);
    if (nLength > 0)
    {
        if ((bstrText = ::SysAllocStringLen(NULL, nLength + 1)) != NULL)
            ::GetWindowTextW(hWnd, bstrText, nLength + 1);
    }

    return (nLength == 0 ? TRUE : (bstrText != NULL));
}

inline BOOL WINAPI _AtlGetWindowText(HWND hWnd, CComBSTR& bstrText)
{
    return _AtlGetWindowText(hWnd, bstrText.m_str);
}

#if defined(__ATLSTR_H__) || defined(__AFXSTR_H__)
inline BOOL WINAPI _AtlGetWindowText(HWND hWnd, CString& strText)
{
    ATLASSERT(::IsWindow(hWnd));

    int nLength = ::GetWindowTextLength(hWnd);
    int nResult = ::GetWindowText(hWnd, strText.GetBuffer(nLength + 1), nLength + 1);
    strText.ReleaseBuffer(nLength);

    return (nResult >= nLength);
}
#endif  // defined(__ATLSTR_H__) || defined(__AFXSTR_H__)

inline BOOL WINAPI _AtlGetLBText(HWND hWnd, BSTR& bstrText)
{
    ATLASSERT(::IsWindow(hWnd));
    ::SysFreeString(bstrText);
    bstrText = NULL;

    LRESULT nIndex = ::SendMessageW(hWnd, LB_GETCURSEL, 0, 0L);
    if (nIndex == LB_ERR)
        return FALSE;

    LRESULT nLength = ::SendMessageW(hWnd, LB_GETTEXTLEN, nIndex, 0L);
    if (nLength > 0)
    {
        if ((bstrText = ::SysAllocStringLen(NULL, nLength + 1)) != NULL)
            ::SendMessageW(hWnd, LB_GETTEXT, (WPARAM)nIndex, (LPARAM)bstrText);
    }

    return (nLength == 0 ? TRUE : (bstrText != NULL));
}

inline BOOL WINAPI _AtlGetLBText(HWND hWnd, CComBSTR& bstrText)
{
    return _AtlGetLBText(hWnd, bstrText.m_str);
}

#if defined(__ATLSTR_H__) || defined(__AFXSTR_H__)
inline BOOL WINAPI _AtlGetLBText(HWND hWnd, CString& strText)
{
    ATLASSERT(::IsWindow(hWnd));

    BOOL bSuccessful = TRUE;
    int nIndex = (int)::SendMessage(hWnd, LB_GETCURSEL, 0, 0L);
    if (nIndex != LB_ERR)
    {
        int nLength = (int)::SendMessage(hWnd, LB_GETTEXTLEN, nIndex, 0L);
        ::SendMessage(hWnd, LB_GETTEXT, nIndex, (LPARAM)strText.GetBuffer(nLength));
        strText.ReleaseBuffer(nLength);
    }
    else
    {
        strText.Empty();
        bSuccessful = FALSE;
    }

    return bSuccessful;
}
#endif  // defined(__ATLSTR_H__) || defined(__AFXSTR_H__)

inline BOOL WINAPI _AtlFloatParse(LPCTSTR pszText, double& dblResult)
{
    ATLASSERT(pszText != NULL);
    while (*pszText == _T(' ') || *pszText == _T('\t'))
        ++pszText;

    BOOL bSuccessful = FALSE;
    if (*pszText != _T('\0'))
    {
        dblResult = ::_tcstod(pszText, (LPTSTR*)&pszText);
        while (*pszText == _T(' ') || *pszText == _T('\t'))
            ++pszText;

        bSuccessful = (*pszText == _T('\0'));
    }

    return bSuccessful;
}

inline void WINAPI _AtlCBSelectString(HWND hWnd, LPCWSTR pszSelect)
{
    ATLASSERT(::IsWindow(hWnd));
    if (::SendMessageW(hWnd, CB_SELECTSTRING, (WPARAM)-1, (LPARAM)pszSelect) == CB_ERR)
        ::SetWindowTextW(hWnd, pszSelect);
}

inline void WINAPI _AtlCBSelectString(HWND hWnd, LPCSTR pszSelect)
{
    ATLASSERT(::IsWindow(hWnd));
    if (::SendMessageA(hWnd, CB_SELECTSTRING, (WPARAM)-1, (LPARAM)pszSelect) == CB_ERR)
        ::SetWindowTextA(hWnd, pszSelect);
}

inline void WINAPI _AtlLBSelectString(HWND hWnd, LPCWSTR pszSelect)
{
    ATLASSERT(::IsWindow(hWnd));
#ifdef _DEBUG
    if (::SendMessageW(hWnd, LB_SELECTSTRING, (WPARAM)-1, (LPARAM)pszSelect) == LB_ERR)
        ATLTRACE(_T("Warning: no listbox item selected.\n"));
#else
    ::SendMessageW(hWnd, LB_SELECTSTRING, (WPARAM)-1, (LPARAM)pszSelect);
#endif  // _DEBUG
}

inline void WINAPI _AtlLBSelectString(HWND hWnd, LPCSTR pszSelect)
{
    ATLASSERT(::IsWindow(hWnd));
#ifdef _DEBUG
    if (::SendMessageA(hWnd, LB_SELECTSTRING, (WPARAM)-1, (LPARAM)pszSelect) == LB_ERR)
        ATLTRACE(_T("Warning: no listbox item selected.\n"));
#else
    ::SendMessageA(hWnd, LB_SELECTSTRING, (WPARAM)-1, (LPARAM)pszSelect);
#endif  // _DEBUG
}

inline void WINAPI _AtlCBSelectStringExact(HWND hWnd, LPCWSTR pszSelect)
{
    ATLASSERT(::IsWindow(hWnd));

    int nIndex = (int)::SendMessageW(hWnd, CB_FINDSTRINGEXACT, (WPARAM)-1, (LPARAM)pszSelect);
    if (nIndex != CB_ERR)
        ::SendMessageW(hWnd, CB_SETCURSEL, nIndex, 0L);
    else
        ::SetWindowTextW(hWnd, pszSelect);
}

inline void WINAPI _AtlCBSelectStringExact(HWND hWnd, LPCSTR pszSelect)
{
    ATLASSERT(::IsWindow(hWnd));

    int nIndex = (int)::SendMessageA(hWnd, CB_FINDSTRINGEXACT, (WPARAM)-1, (LPARAM)pszSelect);
    if (nIndex != CB_ERR)
        ::SendMessageA(hWnd, CB_SETCURSEL, nIndex, 0L);
    else
        ::SetWindowTextA(hWnd, pszSelect);
}

inline void WINAPI _AtlLBSelectStringExact(HWND hWnd, LPCWSTR pszSelect)
{
    ATLASSERT(::IsWindow(hWnd));

    int nIndex = (int)::SendMessageW(hWnd, LB_FINDSTRINGEXACT, (WPARAM)-1, (LPARAM)pszSelect);
    if (nIndex != LB_ERR)
        ::SendMessageW(hWnd, LB_SETCURSEL, nIndex, 0L);
#ifdef _DEBUG
    else
        ATLTRACE(_T("Warning: no listbox item selected.\n"));
#endif  // _DEBUG
}

inline void WINAPI _AtlLBSelectStringExact(HWND hWnd, LPCSTR pszSelect)
{
    ATLASSERT(::IsWindow(hWnd));

    int nIndex = (int)::SendMessageA(hWnd, LB_FINDSTRINGEXACT, (WPARAM)-1, (LPARAM)pszSelect);
    if (nIndex != LB_ERR)
        ::SendMessageA(hWnd, LB_SETCURSEL, nIndex, 0L);
#ifdef _DEBUG
    else
        ATLTRACE(_T("Warning: no listbox item selected.\n"));
#endif  // _DEBUG
}


///////////////////////////////////////////////////////////////////////////////
// Interface of the IStreamImpl class
//

template <typename T>
class ATL_NO_VTABLE IStreamImpl : public IStream
{
// Implementation
public:
    // ISequentialStream interface
    STDMETHOD(Read)(LPVOID pBuffer, ULONG cbSize, ULONG* pcbRead);
    STDMETHOD(Write)(LPCVOID pBuffer, ULONG cbSize, ULONG* pcbWritten);

    // IStream interface
    STDMETHOD(Revert)();
    STDMETHOD(Clone)(IStream** ppStream);
    STDMETHOD(Stat)(STATSTG* pStat, DWORD dwStatFlag);

    STDMETHOD(LockRegion)(ULARGE_INTEGER liOffset, ULARGE_INTEGER cbSize, DWORD dwLockType);
    STDMETHOD(UnlockRegion)(ULARGE_INTEGER liOffset, ULARGE_INTEGER cbSize, DWORD dwLockType);

    STDMETHOD(Commit)(DWORD dwCommitFlags);
    STDMETHOD(SetSize)(ULARGE_INTEGER liNewSize);
    STDMETHOD(Seek)(LARGE_INTEGER liMove, DWORD dwOrigin, ULARGE_INTEGER* pNewPosition);
    STDMETHOD(CopyTo)(IStream* pStream, ULARGE_INTEGER cbSize, ULARGE_INTEGER* pcbRead, ULARGE_INTEGER* pcbWritten);

// Overridables
public:
    HRESULT IStream_Read(LPVOID pBuffer, ULONG cbSize, ULONG* pcbRead);
    HRESULT IStream_Write(LPCVOID pBuffer, ULONG cbSize, ULONG* pcbWritten);

    HRESULT IStream_Commit(DWORD dwCommitFlags);
    HRESULT IStream_SetSize(ULARGE_INTEGER liNewSize);
    HRESULT IStream_Stat(STATSTG* pStat, DWORD dwStatFlag);
    HRESULT IStream_Seek(LARGE_INTEGER liMove, DWORD dwOrigin, ULARGE_INTEGER* pNewPosition);

    HRESULT IStream_LockRegion(ULARGE_INTEGER liOffset, ULARGE_INTEGER cbSize, DWORD dwLockType);
    HRESULT IStream_UnlockRegion(ULARGE_INTEGER liOffset, ULARGE_INTEGER cbSize, DWORD dwLockType);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CFileStream class
//

class ATL_NO_VTABLE CFileStream : public ATL::CComObjectRoot, public IStreamImpl<CFileStream>
{
// Constructors/Destructor
protected:
    CFileStream();
    ~CFileStream();

// Implementation
public:
    BEGIN_COM_MAP(CFileStream)
        COM_INTERFACE_ENTRY(IStream)
        COM_INTERFACE_ENTRY(ISequentialStream)
    END_COM_MAP()

// Operations
public:
    static HRESULT WINAPI CreateInstance(LPCTSTR pszFileName, IStream** ppStream, DWORD dwDesiredAccess = GENERIC_READ, DWORD dwShareMode = FILE_SHARE_READ, DWORD dwCreationDisposition = OPEN_EXISTING, LPSECURITY_ATTRIBUTES psa = NULL);

// Overrides
public:
    HRESULT IStream_Read(LPVOID pBuffer, ULONG cbSize, ULONG* pcbRead);
    HRESULT IStream_Write(LPCVOID pBuffer, ULONG cbSize, ULONG* pcbWritten);

    HRESULT IStream_Commit(DWORD dwCommitFlags);
    HRESULT IStream_Stat(STATSTG* pStat, DWORD dwStatFlag);
    HRESULT IStream_Seek(LARGE_INTEGER liMove, DWORD dwOrigin, ULARGE_INTEGER* pNewPosition);

    HRESULT IStream_LockRegion(ULARGE_INTEGER liOffset, ULARGE_INTEGER cbSize, DWORD dwLockType);
    HRESULT IStream_UnlockRegion(ULARGE_INTEGER liOffset, ULARGE_INTEGER cbSize, DWORD dwLockType);

// Data members
protected:
    HANDLE m_hFile;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CMemoryStream class
//

class ATL_NO_VTABLE CMemoryStream : public ATL::CComObjectRoot, public IStreamImpl<CMemoryStream>
{
// Constructors
protected:
    CMemoryStream();

// Implementation
public:
    BEGIN_COM_MAP(CMemoryStream)
        COM_INTERFACE_ENTRY(IStream)
        COM_INTERFACE_ENTRY(ISequentialStream)
    END_COM_MAP()

// Operations
public:
    static HRESULT WINAPI CreateInstance(LPBYTE pMemory, DWORD cbSize, IStream** ppStream);
    static HRESULT WINAPI CreateInstance(HINSTANCE hInstance, UINT nIDResource, LPCTSTR pType, IStream** ppStream);

// Overrides
public:
    HRESULT IStream_Read(LPVOID pBuffer, ULONG cbSize, ULONG* pcbRead);
    HRESULT IStream_Write(LPCVOID pBuffer, ULONG cbSize, ULONG* pcbWritten);

    HRESULT IStream_Stat(STATSTG* pStat, DWORD dwStatFlag);
    HRESULT IStream_Seek(LARGE_INTEGER liMove, DWORD dwOrigin, ULARGE_INTEGER* pNewPosition);

// Implementation
protected:
    BOOL LoadResource(HINSTANCE hInstance, UINT nIDResource, LPCTSTR pType);
    HRESULT Copy(LPVOID pDest, LPCVOID pSource, DWORD cbSize, ULONG* pcbSize);

// Data members
protected:
    BYTE* m_pMemory;
    DWORD m_dwSize;
    DWORD m_dwPosition;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CopyVariant class
//

template <typename T>
struct CopyVariant
{
    static HRESULT copy(VARIANT* pVariant, const CAdapt<CComPtr<T> >* pInterface);
    static void init(VARIANT* pVariant);
    static void destroy(VARIANT* pVariant);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CopyInterface class
//

template <typename T>
struct CopyInterface
{
    static HRESULT copy(T** ppInterface, const CAdapt<CComPtr<T> >* pInterface);
    static void init(T** ppInterface);
    static void destroy(T** ppInterface);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the IUnknownEvent class
//

class ATL_NO_VTABLE IUnknownEvent
{
// Constructors
protected:
    IUnknownEvent();

// Implementation
public:
    STDMETHOD(_IUEQueryInterface)(REFIID riid, LPVOID* ppvObject) = 0;
    virtual ULONG STDMETHODCALLTYPE AddRef();
    virtual ULONG STDMETHODCALLTYPE Release();

// Data members
public:
    DWORD m_dwCookie;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the IUnknownEventImpl class
//

template <typename T, UINT nEventID, const IID* piid, const IID* piid2 = &IID_NULL>
class ATL_NO_VTABLE IUnknownEventImpl : public IUnknownEvent
{
// Overrides
public:
    STDMETHOD(_IUEQueryInterface)(REFIID riid, LPVOID* ppvObject);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the IContextMenuImpl class
//

template <typename T>
class ATL_NO_VTABLE IContextMenuImpl : public IContextMenu
{
// Implementation
public:
    STDMETHOD(InvokeCommand)(LPCMINVOKECOMMANDINFO pCmdInfo);
    STDMETHOD(GetCommandString)(UINT idCmd, UINT nType, UINT* pReserved, LPSTR pszName, UINT cchMax);
    STDMETHOD(QueryContextMenu)(HMENU hMenu, UINT indexMenu, UINT idCmdFirst, UINT idCmdLast, UINT nFlags);

// Overridables
public:
    HRESULT IContextMenu_InvokeCommand(LPCMINVOKECOMMANDINFO pCmdInfo);
    HRESULT IContextMenu_GetCommandString(UINT idCmd, UINT nType, UINT* pReserved, LPSTR pszName, UINT cchMax);
    HRESULT IContextMenu_QueryContextMenu(HMENU hMenu, UINT indexMenu, UINT idCmdFirst, UINT idCmdLast, UINT nFlags);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the IObjectWithSiteImpl class
//

template <typename T>
class ATL_NO_VTABLE IObjectWithSiteImpl : public IObjectWithSite
{
// Implementation
public:
    STDMETHOD(SetSite)(IUnknown* pUnkSite);
    STDMETHOD(GetSite)(REFIID riid, LPVOID* ppvSite);

// Data members
public:
    CComPtr<IUnknown> m_spUnkSite;
};


#if defined(__shobjidl_h__) && defined(__SYSUTIL_H__)
///////////////////////////////////////////////////////////////////////////////
// Interface of the IShellPropSheetImpl class
//

template <typename T>
class ATL_NO_VTABLE IShellPropSheetImpl : public IShellExtInit, public IShellPropSheetExt
{
// Operations
public:
    static HRESULT WINAPI IShellPropSheet_RegisterServer(LPCTSTR pszFileType);
    static HRESULT WINAPI IShellPropSheet_UnregisterServer(LPCTSTR pszFileType);

// Implementation
public:
    // IShellExtInit interface
    STDMETHOD(Initialize)(LPCITEMIDLIST pidlFolder, IDataObject* pDataObject, HKEY hKeyProgID);

    // IShellPropSheetExt interface
    STDMETHOD(AddPages)(LPFNSVADDPROPSHEETPAGE pfnAddPage, LPARAM lParam);
    STDMETHOD(ReplacePage)(EXPPS nPageID, LPFNSVADDPROPSHEETPAGE pfnReplacePage, LPARAM lParam);

// Overridables
public:
    HRESULT IShellPropSheet_Initialize(HDROP hDrop, HKEY hKeyProgID);
    HRESULT IShellPropSheet_AddPages(LPFNSVADDPROPSHEETPAGE pfnAddPage, LPARAM lParam);
};
#endif  // defined(__shobjidl_h__) && defined(__SYSUTIL_H__)


#ifdef __propcat_h__
///////////////////////////////////////////////////////////////////////////////
// Interface of the ICategorizePropertiesImpl class
//

template <typename T>
class ATL_NO_VTABLE ICategorizePropertiesImpl : public ICategorizeProperties
{
// Implementation
public:
    STDMETHOD(MapPropertyToCategory)(DISPID dispid, PROPCAT* pPropcat);
    STDMETHOD(GetCategoryName)(PROPCAT propcat, LCID lcid, BSTR* pbstrName);

// Overridables
public:
    HRESULT ICategorizeProperties_GetCategoryName(PROPCAT propcat, LCID lcid, BSTR* pbstrName);
};
#endif  // __propcat_h__


///////////////////////////////////////////////////////////////////////////////
// Interface of the CComContainerObject class
//

template <typename _Container, typename ThreadModel = ATL::CComObjectThreadModel>
class ATL_NO_VTABLE CComContainerObject : public ATL::CComObjectRootEx<ThreadModel>, public IUnknown
{
public:
    typedef CComContainerObject<_Container, ThreadModel> _CComContainerObject;

// Implementation
public:
    BEGIN_COM_MAP(_CComContainerObject)
        COM_INTERFACE_ENTRY(IUnknown)
    END_COM_MAP()

// Operations
public:
    HRESULT Copy(const _Container& container);
    static HRESULT WINAPI CreateInstance(ATL::CComObject<CComContainerObject<_Container, ThreadModel> >** ppObject, const _Container& container);

// Data members
public:
    _Container m_container;
};

}  // namespace atlutil

#include "atlcomutil.inl"

#endif  // __ATLCOMUTIL_H__