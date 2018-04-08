///////////////////////////////////////////////////////////////////////////////
// shvwutil.inl : 实现文件
//
// Copyright(c) 2008, Garfield. All rights reserved.
// Author  : Garfield
// Version : 1.0
// Finish Time : 2008/5/17

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __SHVWUTIL_INL__
#define __SHVWUTIL_INL__

namespace atlutil {

///////////////////////////////////////////////////////////////////////////////
// CDocHostUIHandler 类的实现
//

inline CDocHostUIHandler::CDocHostUIHandler()
{
}

inline HRESULT CDocHostUIHandler::SetUIHandler(IDispatch* pDocument)
{
    ATLASSERT(pDocument);

    CComQIPtr<IOleObject> spOleObject = pDocument;
    ATLASSERT(spOleObject != NULL);

    CComPtr<IOleClientSite> spClientSite;
    HRESULT hr = spOleObject->GetClientSite(&spClientSite);
    if (SUCCEEDED(hr) && spClientSite != NULL)
    {
        m_spDefUIHandler = spClientSite;
        ATLASSERT(m_spDefUIHandler != NULL);

        CComQIPtr<ICustomDoc> spCustomDoc = pDocument;
        ATLASSERT(spCustomDoc != NULL);

        hr = spCustomDoc->SetUIHandler(this);
    }

    return hr;
}

inline STDMETHODIMP CDocHostUIHandler::QueryInterface(REFIID riid, void** ppvObject)
{
    if (ppvObject == NULL)
        return E_POINTER;

    HRESULT hr = E_NOINTERFACE;
    if (riid == IID_IUnknown || riid == IID_IDocHostUIHandler)
    {
        *ppvObject = this;
        reinterpret_cast<IUnknown*>(*ppvObject)->AddRef();
        hr = S_OK;
    }

    return hr;
}

inline STDMETHODIMP_(ULONG) CDocHostUIHandler::AddRef()
{
    return 1L;
}

inline STDMETHODIMP_(ULONG) CDocHostUIHandler::Release()
{
    return 1L;
}

inline STDMETHODIMP CDocHostUIHandler::HideUI()
{
    ATLASSERT(m_spDefUIHandler != NULL);
    return m_spDefUIHandler->HideUI();
}

inline STDMETHODIMP CDocHostUIHandler::UpdateUI()
{
    ATLASSERT(m_spDefUIHandler != NULL);
    return m_spDefUIHandler->UpdateUI();
}

inline STDMETHODIMP CDocHostUIHandler::EnableModeless(BOOL bEnable)
{
    ATLASSERT(m_spDefUIHandler != NULL);
    return m_spDefUIHandler->EnableModeless(bEnable);
}

inline STDMETHODIMP CDocHostUIHandler::GetHostInfo(DOCHOSTUIINFO* pInfo)
{
    ATLASSERT(m_spDefUIHandler != NULL);
    return m_spDefUIHandler->GetHostInfo(pInfo);
}

inline STDMETHODIMP CDocHostUIHandler::GetExternal(IDispatch** ppDispatch)
{
    ATLASSERT(m_spDefUIHandler != NULL);
    return m_spDefUIHandler->GetExternal(ppDispatch);
}

inline STDMETHODIMP CDocHostUIHandler::OnDocWindowActivate(BOOL bActivate)
{
    ATLASSERT(m_spDefUIHandler != NULL);
    return m_spDefUIHandler->OnDocWindowActivate(bActivate);
}

inline STDMETHODIMP CDocHostUIHandler::OnFrameWindowActivate(BOOL bActivate)
{
    ATLASSERT(m_spDefUIHandler != NULL);
    return m_spDefUIHandler->OnFrameWindowActivate(bActivate);
}

inline STDMETHODIMP CDocHostUIHandler::GetOptionKeyPath(LPOLESTR* ppszKey, DWORD dwReserved)
{
    ATLASSERT(m_spDefUIHandler != NULL);
    return m_spDefUIHandler->GetOptionKeyPath(ppszKey, dwReserved);
}

inline STDMETHODIMP CDocHostUIHandler::GetDropTarget(IDropTarget* pDropTarget, IDropTarget** ppDropTarget)
{
    ATLASSERT(m_spDefUIHandler != NULL);
    return m_spDefUIHandler->GetDropTarget(pDropTarget, ppDropTarget);
}

inline STDMETHODIMP CDocHostUIHandler::FilterDataObject(IDataObject* pDataObject, IDataObject** ppDataObject)
{
    ATLASSERT(m_spDefUIHandler != NULL);
    return m_spDefUIHandler->FilterDataObject(pDataObject, ppDataObject);
}

inline STDMETHODIMP CDocHostUIHandler::TranslateAccelerator(LPMSG lpMsg, const GUID* pCmdGroup, DWORD nCmdID)
{
    ATLASSERT(m_spDefUIHandler != NULL);
    return m_spDefUIHandler->TranslateAccelerator(lpMsg, pCmdGroup, nCmdID);
}

inline STDMETHODIMP CDocHostUIHandler::TranslateUrl(DWORD dwTranslate, OLECHAR* pszURLIn, OLECHAR** ppszURLOut)
{
    ATLASSERT(m_spDefUIHandler != NULL);
    return m_spDefUIHandler->TranslateUrl(dwTranslate, pszURLIn, ppszURLOut);
}

inline STDMETHODIMP CDocHostUIHandler::ResizeBorder(LPCRECT pBorder, IOleInPlaceUIWindow* pUIWindow, BOOL bRameWindow)
{
    ATLASSERT(m_spDefUIHandler != NULL);
    return m_spDefUIHandler->ResizeBorder(pBorder, pUIWindow, bRameWindow);
}

inline STDMETHODIMP CDocHostUIHandler::ShowContextMenu(DWORD /*dwID*/, POINT* /*pPoint*/, IUnknown* /*pCmdTargetReserved*/, IDispatch* /*pDispReserved*/)
{
    return S_OK;
}

inline STDMETHODIMP CDocHostUIHandler::ShowUI(DWORD dwID, IOleInPlaceActiveObject* pActiveObject, IOleCommandTarget* pCommandTarget, IOleInPlaceFrame* pFrame, IOleInPlaceUIWindow* pDoc)
{
    ATLASSERT(m_spDefUIHandler != NULL);
    return m_spDefUIHandler->ShowUI(dwID, pActiveObject, pCommandTarget, pFrame, pDoc);
}

}  // namespace atlutil

#endif  // __SHVWUTIL_INL__