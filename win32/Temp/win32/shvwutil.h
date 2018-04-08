///////////////////////////////////////////////////////////////////////////////
// shvwutil.h : 头文件
//
// Copyright(c) 2008, Garfield. All rights reserved.
// Author  : Garfield
// Version : 1.0
// Finish Time : 2008/5/17

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __SHVWUTIL_H__
#define __SHVWUTIL_H__

#ifndef __mshtmhst_h__
#include <mshtmhst.h>
#endif

#ifndef __ATLCOMCLI_H__
#include <atlcomcli.h>
#endif

///////////////////////////////////////////////////////////////////////////////
// Classes in this file:
//
// CDocHostUIHandler

namespace atlutil {

///////////////////////////////////////////////////////////////////////////////
// CDocHostUIHandler 类的定义
//

class CDocHostUIHandler : public IDocHostUIHandler
{
// 构造
public:
    CDocHostUIHandler();

// 操作
public:
    HRESULT SetUIHandler(IDispatch* pDocument);

    // IUnknown 接口
    STDMETHOD(QueryInterface)(REFIID riid, void** ppvObject);
    STDMETHOD_(ULONG, AddRef)();
    STDMETHOD_(ULONG, Release)();

    // IDocHostUIHandler 接口
    STDMETHOD(HideUI)();
    STDMETHOD(UpdateUI)();
    STDMETHOD(EnableModeless)(BOOL bEnable);
    STDMETHOD(GetHostInfo)(DOCHOSTUIINFO* pInfo);
    STDMETHOD(GetExternal)(IDispatch** ppDispatch);
    STDMETHOD(OnDocWindowActivate)(BOOL bActivate);
    STDMETHOD(OnFrameWindowActivate)(BOOL bActivate);
    STDMETHOD(GetOptionKeyPath)(LPOLESTR* ppszKey, DWORD dwReserved);
    STDMETHOD(GetDropTarget)(IDropTarget* pDropTarget, IDropTarget** ppDropTarget);
    STDMETHOD(FilterDataObject)(IDataObject* pDataObject, IDataObject** ppDataObject);
    STDMETHOD(TranslateAccelerator)(LPMSG lpMsg, const GUID* pCmdGroup, DWORD nCmdID);
    STDMETHOD(TranslateUrl)(DWORD dwTranslate, OLECHAR* pszURLIn, OLECHAR** ppszURLOut);
    STDMETHOD(ResizeBorder)(LPCRECT pBorder, IOleInPlaceUIWindow* pUIWindow, BOOL bRameWindow);
    STDMETHOD(ShowContextMenu)(DWORD dwID, POINT* pPoint, IUnknown* pCmdTargetReserved, IDispatch* pDispReserved);
    STDMETHOD(ShowUI)(DWORD dwID, IOleInPlaceActiveObject* pActiveObject, IOleCommandTarget* pCommandTarget, IOleInPlaceFrame* pFrame, IOleInPlaceUIWindow* pDoc);

// 成员
protected:
    CComQIPtr<IDocHostUIHandler> m_spDefUIHandler;
};

}  // namespace atlutil

#include "shvwutil.inl"

#endif  // __SHVWUTIL_H__