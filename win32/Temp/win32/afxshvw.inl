///////////////////////////////////////////////////////////////////////////////
// afxshvw.inl
//
// Copyright(c) 2012, Garfield. All rights reserved.
// Author  : Garfield
// Version : 1.0
// Finish Time : 2007/4/14

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __AFXSHVW_INL__
#define __AFXSHVW_INL__

namespace afxctrls {

///////////////////////////////////////////////////////////////////////////////
// Implementation of the CWebBrowserCtrl class
//

inline CWebBrowserCtrl::CWebBrowserCtrl()
{
}

inline CWebBrowserCtrl::~CWebBrowserCtrl()
{
}

inline BOOL CWebBrowserCtrl::get_Busy()
{
    BOOL _result = FALSE;
    InvokeHelper(0xd4, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&_result, NULL);
    return _result;
}

inline long CWebBrowserCtrl::get_HWND()
{
    long _result = 0;
    InvokeHelper(DISPID_HWND, DISPATCH_PROPERTYGET, VT_I4, (void*)&_result, NULL);
    return _result;
}

inline const CLSID& CWebBrowserCtrl::GetCLSID()
{
    static const CLSID clsid = { 0x8856F961, 0x340A, 0x11D0, { 0xA9, 0x6B, 0x0, 0xC0, 0x4F, 0xD7, 0x5, 0xA2 } };
    return clsid;
}

inline READYSTATE CWebBrowserCtrl::get_ReadyState()
{
    READYSTATE _result = READYSTATE_UNINITIALIZED;
    InvokeHelper(DISPID_READYSTATE, DISPATCH_PROPERTYGET, VT_I4, (void*)&_result, NULL);
    return _result;
}

inline BOOL CWebBrowserCtrl::get_TopLevelContainer()
{
    BOOL _result = FALSE;
    InvokeHelper(0xcc, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&_result, NULL);
    return _result;
}

inline long CWebBrowserCtrl::get_Left()
{
    long _result = 0;
    InvokeHelper(0xce, DISPATCH_PROPERTYGET, VT_I4, (void*)&_result, NULL);
    return _result;
}

inline void CWebBrowserCtrl::put_Left(long lLeft)
{
    static BYTE params[] = VTS_I4;
    InvokeHelper(0xce, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, params, lLeft);
}

inline long CWebBrowserCtrl::get_Top()
{
    long _result = 0;
    InvokeHelper(0xcf, DISPATCH_PROPERTYGET, VT_I4, (void*)&_result, NULL);
    return _result;
}

inline void CWebBrowserCtrl::put_Top(long lTop)
{
    static BYTE params[] = VTS_I4;
    InvokeHelper(0xcf, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, params, lTop);
}

inline long CWebBrowserCtrl::get_Width()
{
    long _result = 0;
    InvokeHelper(0xd0, DISPATCH_PROPERTYGET, VT_I4, (void*)&_result, NULL);
    return _result;
}

inline void CWebBrowserCtrl::put_Width(long lWidth)
{
    static BYTE params[] = VTS_I4;
    InvokeHelper(0xd0, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, params, lWidth);
}

inline long CWebBrowserCtrl::get_Height()
{
    long _result = 0;
    InvokeHelper(0xd1, DISPATCH_PROPERTYGET, VT_I4, (void*)&_result, NULL);
    return _result;
}

inline void CWebBrowserCtrl::put_Height(long lHeight)
{
    static BYTE params[] = VTS_I4;
    InvokeHelper(0xd1, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, params, lHeight);
}

inline LPDISPATCH CWebBrowserCtrl::get_Application()
{
    LPDISPATCH _result = NULL;
    InvokeHelper(0xc8, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&_result, NULL);
    return _result;
}

inline LPDISPATCH CWebBrowserCtrl::get_Parent()
{
    LPDISPATCH _result = NULL;
    InvokeHelper(0xc9, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&_result, NULL);
    return _result;
}

inline LPDISPATCH CWebBrowserCtrl::get_Container()
{
    LPDISPATCH _result = NULL;
    InvokeHelper(0xca, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&_result, NULL);
    return _result;
}

inline LPDISPATCH CWebBrowserCtrl::get_Document()
{
    LPDISPATCH _result = NULL;
    InvokeHelper(0xcb, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&_result, NULL);
    return _result;
}

inline CString CWebBrowserCtrl::get_Type()
{
    CString _result;
    InvokeHelper(0xcd, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&_result, NULL);
    return _result;
}

inline CString CWebBrowserCtrl::get_Path()
{
    CString _result;
    InvokeHelper(0x191, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&_result, NULL);
    return _result;
}

inline CString CWebBrowserCtrl::get_Name()
{
    CString _result;
    InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&_result, NULL);
    return _result;
}

inline CString CWebBrowserCtrl::get_FullName()
{
    CString _result;
    InvokeHelper(0x190, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&_result, NULL);
    return _result;
}

inline CString CWebBrowserCtrl::get_LocationURL()
{
    CString _result;
    InvokeHelper(0xd3, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&_result, NULL);
    return _result;
}

inline CString CWebBrowserCtrl::get_LocationName()
{
    CString _result;
    InvokeHelper(0xd2, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&_result, NULL);
    return _result;
}

inline long CWebBrowserCtrl::get_ToolBar()
{
    long _result = 0;
    InvokeHelper(0x195, DISPATCH_PROPERTYGET, VT_I4, (void*)&_result, NULL);
    return _result;
}

inline void CWebBrowserCtrl::put_ToolBar(long lToolBar)
{
    static BYTE params[] = VTS_I4;
    InvokeHelper(0x195, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, params, lToolBar);
}

inline BOOL CWebBrowserCtrl::get_MenuBar()
{
    BOOL _result = FALSE;
    InvokeHelper(0x196, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&_result, NULL);
    return _result;
}

inline void CWebBrowserCtrl::put_MenuBar(BOOL bMenuBar)
{
    static BYTE params[] = VTS_BOOL;
    InvokeHelper(0x196, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, params, bMenuBar);
}

inline BOOL CWebBrowserCtrl::get_StatusBar()
{
    BOOL _result = FALSE;
    InvokeHelper(0x193, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&_result, NULL);
    return _result;
}

inline void CWebBrowserCtrl::put_StatusBar(BOOL bStatusBar)
{
    static BYTE params[] = VTS_BOOL;
    InvokeHelper(0x193, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, params, bStatusBar);
}

inline CString CWebBrowserCtrl::get_StatusText()
{
    CString _result;
    InvokeHelper(0x194, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&_result, NULL);
    return _result;
}

inline void CWebBrowserCtrl::put_StatusText(LPCTSTR pszText)
{
    static BYTE params[] = VTS_BSTR;
    InvokeHelper(0x194, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, params, pszText);
}

inline BOOL CWebBrowserCtrl::get_AddressBar()
{
    BOOL _result = FALSE;
    InvokeHelper(0x22b, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&_result, NULL);
    return _result;
}

inline void CWebBrowserCtrl::put_AddressBar(BOOL bAddressBar)
{
    static BYTE params[] = VTS_BOOL;
    InvokeHelper(0x22b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, params, bAddressBar);
}

inline BOOL CWebBrowserCtrl::get_Silent()
{
    BOOL _result = FALSE;
    InvokeHelper(0x227, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&_result, NULL);
    return _result;
}

inline void CWebBrowserCtrl::put_Silent(BOOL bSilent)
{
    static BYTE params[] = VTS_BOOL;
    InvokeHelper(0x227, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, params, bSilent);
}

inline BOOL CWebBrowserCtrl::get_Offline()
{
    BOOL _result = FALSE;
    InvokeHelper(0x226, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&_result, NULL);
    return _result;
}

inline void CWebBrowserCtrl::put_Offline(BOOL bOffline)
{
    static BYTE params[] = VTS_BOOL;
    InvokeHelper(0x226, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, params, bOffline);
}

inline BOOL CWebBrowserCtrl::get_Visible()
{
    BOOL _result = FALSE;
    InvokeHelper(0x192, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&_result, NULL);
    return _result;
}

inline void CWebBrowserCtrl::put_Visible(BOOL bVisible)
{
    static BYTE params[] = VTS_BOOL;
    InvokeHelper(0x192, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, params, bVisible);
}

inline BOOL CWebBrowserCtrl::get_Resizable()
{
    BOOL _result = FALSE;
    InvokeHelper(0x22c, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&_result, NULL);
    return _result;
}

inline void CWebBrowserCtrl::put_Resizable(BOOL bResizable)
{
    static BYTE params[] = VTS_BOOL;
    InvokeHelper(0x22c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, params, bResizable);
}

inline BOOL CWebBrowserCtrl::get_FullScreen()
{
    BOOL _result = FALSE;
    InvokeHelper(0x197, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&_result, NULL);
    return _result;
}

inline void CWebBrowserCtrl::put_FullScreen(BOOL bFullScreen)
{
    static BYTE params[] = VTS_BOOL;
    InvokeHelper(0x197, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, params, bFullScreen);
}

inline BOOL CWebBrowserCtrl::get_TheaterMode()
{
    BOOL _result = FALSE;
    InvokeHelper(0x22a, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&_result, NULL);
    return _result;
}

inline void CWebBrowserCtrl::put_TheaterMode(BOOL bTheaterMode)
{
    static BYTE params[] = VTS_BOOL;
    InvokeHelper(0x22a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, params, bTheaterMode);
}

inline BOOL CWebBrowserCtrl::get_RegisterAsBrowser()
{
    BOOL _result = FALSE;
    InvokeHelper(0x228, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&_result, NULL);
    return _result;
}

inline void CWebBrowserCtrl::put_RegisterAsBrowser(BOOL bRegister)
{
    static BYTE params[] = VTS_BOOL;
    InvokeHelper(0x228, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, params, bRegister);
}

inline BOOL CWebBrowserCtrl::get_RegisterAsDropTarget()
{
    BOOL _result = FALSE;
    InvokeHelper(0x229, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&_result, NULL);
    return _result;
}

inline void CWebBrowserCtrl::put_RegisterAsDropTarget(BOOL bRegister)
{
    static BYTE params[] = VTS_BOOL;
    InvokeHelper(0x229, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, params, bRegister);
}

inline BOOL CWebBrowserCtrl::Create(LPCTSTR /*pszClassName*/, LPCTSTR pszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* /*pContext = NULL*/)
{
    return CreateControl(GetCLSID(), pszWindowName, dwStyle, rect, pParentWnd, nID); 
}

inline BOOL CWebBrowserCtrl::Create(LPCTSTR pszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CFile* pPersist/* = NULL*/, BOOL bStorage/* = FALSE*/, BSTR bstrLicKey/* = NULL*/)
{
    return CreateControl(GetCLSID(), pszWindowName, dwStyle, rect, pParentWnd, nID, pPersist, bStorage, bstrLicKey); 
}

inline void CWebBrowserCtrl::Navigate(LPCTSTR pszURL, VARIANT* pvtFlags/* = NULL*/, VARIANT* pvtTargetFrameName/* = NULL*/, VARIANT* pvtPostData/* = NULL*/, VARIANT* pvtHeaders/* = NULL*/)
{
    static BYTE params[] = VTS_BSTR VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT;
    InvokeHelper(0x68, DISPATCH_METHOD, VT_EMPTY, NULL, params, pszURL, pvtFlags, pvtTargetFrameName, pvtPostData, pvtHeaders);
}

inline void CWebBrowserCtrl::Navigate2(VARIANT* pvtURL, VARIANT* pvtFlags/* = NULL*/, VARIANT* pvtTargetFrameName/* = NULL*/, VARIANT* pvtPostData/* = NULL*/, VARIANT* pvtHeaders/* = NULL*/)
{
    static BYTE params[] = VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT;
    InvokeHelper(0x1f4, DISPATCH_METHOD, VT_EMPTY, NULL, params, pvtURL, pvtFlags, pvtTargetFrameName, pvtPostData, pvtHeaders);
}

inline void CWebBrowserCtrl::Refresh()
{
    InvokeHelper(DISPID_REFRESH, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

inline void CWebBrowserCtrl::Refresh2(VARIANT* pvtLevel/* = NULL*/)
{
    static BYTE params[] = VTS_PVARIANT;
    InvokeHelper(0x69, DISPATCH_METHOD, VT_EMPTY, NULL, params, pvtLevel);
}

inline VARIANT CWebBrowserCtrl::GetProperty(LPCTSTR pszProperty)
{
    VARIANT _result;
    ::VariantInit(&_result);

    static BYTE params[] = VTS_BSTR;
    InvokeHelper(0x12f, DISPATCH_METHOD, VT_VARIANT, (void*)&_result, params, pszProperty);
    return _result;
}

inline void CWebBrowserCtrl::PutProperty(LPCTSTR pszProperty, VARIANT vtValue)
{
    static BYTE params[] = VTS_BSTR VTS_VARIANT;
    InvokeHelper(0x12e, DISPATCH_METHOD, VT_EMPTY, NULL, params, pszProperty, &vtValue);
}

inline void CWebBrowserCtrl::Stop()
{
    InvokeHelper(0x6a, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

inline void CWebBrowserCtrl::Quit()
{
    InvokeHelper(0x12c, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

inline void CWebBrowserCtrl::GoBack()
{
    InvokeHelper(0x64, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

inline void CWebBrowserCtrl::GoHome()
{
    InvokeHelper(0x66, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

inline void CWebBrowserCtrl::GoSearch()
{
    InvokeHelper(0x67, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

inline void CWebBrowserCtrl::GoForward()
{
    InvokeHelper(0x65, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

inline OLECMDF CWebBrowserCtrl::QueryStatusWB(OLECMDID CmdID)
{
    OLECMDF _result;
    static BYTE params[] = VTS_I4;
    InvokeHelper(0x1f5, DISPATCH_METHOD, VT_I4, (void*)&_result, params, CmdID);
    return _result;
}

inline void CWebBrowserCtrl::ClientToWindow(long* pWidth, long* pHeight)
{
    static BYTE params[] = VTS_PI4 VTS_PI4;
    InvokeHelper(0x12d, DISPATCH_METHOD, VT_EMPTY, NULL, params, pWidth, pHeight);
}

inline void CWebBrowserCtrl::ExecWB(OLECMDID CmdID, OLECMDEXECOPT CmdExecopt, VARIANT* pvtIn, VARIANT* pvtOut)
{
    static BYTE params[] = VTS_I4 VTS_I4 VTS_PVARIANT VTS_PVARIANT;
    InvokeHelper(0x1f6, DISPATCH_METHOD, VT_EMPTY, NULL, params, CmdID, CmdExecopt, pvtIn, pvtOut);
}

inline void CWebBrowserCtrl::ShowBrowserBar(VARIANT* pvtClsid, VARIANT* pvtShow/* = NULL*/, VARIANT* pvtSize/* = NULL*/)
{
    static BYTE params[] = VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT;
    InvokeHelper(0x1f7, DISPATCH_METHOD, VT_EMPTY, NULL, params, pvtClsid, pvtShow, pvtSize);
}

}  // namespace afxctrls

#endif  // __AFXSHVW_INL__