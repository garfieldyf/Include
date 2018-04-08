///////////////////////////////////////////////////////////////////////////////
// afxshvw.h
//
// Copyright(c) 2012, Garfield. All rights reserved.
// Author  : Garfield
// Version : 1.0
// Finish Time : 2007/4/14

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __AFXSHVW_H__
#define __AFXSHVW_H__

#ifdef _WIN32_WCE
    #error afxshvw.h compiles under Windows only
#endif

#include <afxwin.h>

///////////////////////////////////////////////////////////////////////////////
// Classes in this file:
//
// CWebBrowserCtrl

namespace afxctrls {

///////////////////////////////////////////////////////////////////////////////
// Interface of the CWebBrowserCtrl class
//

class CWebBrowserCtrl : public CWnd
{
// Constructors/Destructor
public:
    CWebBrowserCtrl();
    virtual ~CWebBrowserCtrl();

// Attributes
public:
    BOOL get_Busy();
    long get_HWND();
    const CLSID& GetCLSID();
    READYSTATE get_ReadyState();
    BOOL get_TopLevelContainer();

    long get_Left();
    void put_Left(long lLeft);
    long get_Top();
    void put_Top(long lTop);
    long get_Width();
    void put_Width(long lWidth);
    long get_Height();
    void put_Height(long lHeight);

    LPDISPATCH get_Application();
    LPDISPATCH get_Parent();
    LPDISPATCH get_Container();
    LPDISPATCH get_Document();

    CString get_Type();
    CString get_Path();
    CString get_Name();
    CString get_FullName();
    CString get_LocationURL();
    CString get_LocationName();

    long get_ToolBar();
    void put_ToolBar(long lToolBar);
    BOOL get_MenuBar();
    void put_MenuBar(BOOL bMenuBar);
    BOOL get_StatusBar();
    void put_StatusBar(BOOL bStatusBar);
    CString get_StatusText();
    void put_StatusText(LPCTSTR pszText);
    BOOL get_AddressBar();
    void put_AddressBar(BOOL bAddressBar);

    BOOL get_Silent();
    void put_Silent(BOOL bSilent);
    BOOL get_Offline();
    void put_Offline(BOOL bOffline);
    BOOL get_Visible();
    void put_Visible(BOOL bVisible);
    BOOL get_Resizable();
    void put_Resizable(BOOL bResizable);
    BOOL get_FullScreen();
    void put_FullScreen(BOOL bFullScreen);
    BOOL get_TheaterMode();
    void put_TheaterMode(BOOL bTheaterMode);
    BOOL get_RegisterAsBrowser();
    void put_RegisterAsBrowser(BOOL bRegister);
    BOOL get_RegisterAsDropTarget();
    void put_RegisterAsDropTarget(BOOL bRegister);

// Operations
public:
    virtual BOOL Create(LPCTSTR pszClassName, LPCTSTR pszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
    BOOL Create(LPCTSTR pszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CFile* pPersist = NULL, BOOL bStorage = FALSE, BSTR bstrLicKey = NULL);

    void Navigate(LPCTSTR pszURL, VARIANT* pvtFlags = NULL, VARIANT* pvtTargetFrameName = NULL, VARIANT* pvtPostData = NULL, VARIANT* pvtHeaders = NULL);
    void Navigate2(VARIANT* pvtURL, VARIANT* pvtFlags = NULL, VARIANT* pvtTargetFrameName = NULL, VARIANT* pvtPostData = NULL, VARIANT* pvtHeaders = NULL);

    void Refresh();
    void Refresh2(VARIANT* pvtLevel = NULL);

    VARIANT GetProperty(LPCTSTR pszProperty);
    void PutProperty(LPCTSTR pszProperty, VARIANT vtValue);

    void Stop();
    void Quit();
    void GoBack();
    void GoHome();
    void GoSearch();
    void GoForward();
    OLECMDF QueryStatusWB(OLECMDID CmdID);
    void ClientToWindow(long* pWidth, long* pHeight);
    void ExecWB(OLECMDID CmdID, OLECMDEXECOPT CmdExecopt, VARIANT* pvtIn, VARIANT* pvtOut);
    void ShowBrowserBar(VARIANT* pvtClsid, VARIANT* pvtShow = NULL, VARIANT* pvtSize = NULL);
};

}  // namespace afxctrls

#include "afxshvw.inl"

#endif  // __AFXSHVW_H__