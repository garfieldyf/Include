///////////////////////////////////////////////////////////////////////////////
// atlcomhost.h : ͷ�ļ�
//
// Copyright(c) 2012, Garfield. All rights reserved.
// Author  : Garfield
// Version : 1.0
// Finish Time : 2007/2/4

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __ATLCOMHOST_H__
#define __ATLCOMHOST_H__

#ifndef __mshtml_h__
#include <mshtml.h>
#endif

#ifndef __mshtmhst_h__
#include <mshtmhst.h>
#endif

///////////////////////////////////////////////////////////////////////////////
// Classes in this file:
//
// IDocHostUIHandler2Impl

namespace atlctrls {

///////////////////////////////////////////////////////////////////////////////
// IDocHostUIHandler2Impl ��Ķ���
//

class ATL_NO_VTABLE IDocHostUIHandler2Impl : public IDocHostUIHandler2
{
// ����
public:
    IDocHostUIHandler2Impl();

// ����
public:
    HRESULT SetUIHandler(IDispatch* pDispatch);
    HRESULT ReleaseUIHandler(IDispatch* pDispatch);

    // IUnknown �ӿ�
    STDMETHOD(QueryInterface)(REFIID riid, LPVOID* ppvObject);
    STDMETHOD_(ULONG, AddRef)();
    STDMETHOD_(ULONG, Release)();

    // IDocHostUIHandler �ӿ�
    STDMETHOD(HideUI)();
    STDMETHOD(UpdateUI)();
    STDMETHOD(EnableModeless)(BOOL bEnable);
    STDMETHOD(GetHostInfo)(DOCHOSTUIINFO* pInfo);
    STDMETHOD(OnDocWindowActivate)(BOOL bActivate);
    STDMETHOD(GetExternal)(IDispatch** ppDispatch);
    STDMETHOD(OnFrameWindowActivate)(BOOL bActivate);
    STDMETHOD(GetOptionKeyPath)(LPOLESTR* ppszKey, DWORD dwReserved);
    STDMETHOD(GetDropTarget)(IDropTarget* pDropTarget, IDropTarget** ppDropTarget);
    STDMETHOD(TranslateAccelerator)(LPMSG pMsg, const GUID* pCmdGroup, DWORD nCmdID);
    STDMETHOD(FilterDataObject)(IDataObject* pDataObject, IDataObject** ppDataObject);
    STDMETHOD(TranslateUrl)(DWORD dwTranslate, LPOLESTR pszURLIn, LPOLESTR* ppszURLOut);
    STDMETHOD(ResizeBorder)(LPCRECT prcBorder, IOleInPlaceUIWindow* pUIWindow, BOOL bRameWindow);
    STDMETHOD(ShowContextMenu)(DWORD dwID, LPPOINT pPoint, IUnknown* pCmdTargetReserved, IDispatch* pDispReserved);
    STDMETHOD(ShowUI)(DWORD dwID, IOleInPlaceActiveObject* pActiveObject, IOleCommandTarget* pCmdTarget, IOleInPlaceFrame* pFrame, IOleInPlaceUIWindow* pDoc);

    // IDocHostUIHandler2 �ӿ�
    STDMETHOD(GetOverrideKeyPath)(LPOLESTR* ppszKey, DWORD dwReserved);

// ��Ա
protected:
    CComPtr<IDocHostUIHandler2> m_spUIHandler;
};

}  // namespace atlctrls

#include "atlcomhost.inl"

#endif  // __ATLCOMHOST_H__