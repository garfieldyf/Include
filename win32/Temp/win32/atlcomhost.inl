///////////////////////////////////////////////////////////////////////////////
// atlcomhost.inl : 实现文件
//
// Copyright(c) 2012, Garfield. All rights reserved.
// Author  : Garfield
// Version : 1.0
// Finish Time : 2007/2/4

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __ATLCOMHOST_INL__
#define __ATLCOMHOST_INL__

namespace atlctrls {

///////////////////////////////////////////////////////////////////////////////
// IDocHostUIHandler2Impl 类的实现
//

inline IDocHostUIHandler2Impl::IDocHostUIHandler2Impl()
{
}

inline HRESULT IDocHostUIHandler2Impl::SetUIHandler(IDispatch* pDispatch)
{
    ATLASSERT(pDispatch);
    HRESULT hr = E_NOINTERFACE;

    if (CComQIPtr<IOleObject> spOleObject = pDispatch)
    {
        CComPtr<IOleClientSite> spClientSite;
        hr = spOleObject->GetClientSite(&spClientSite);

        if (SUCCEEDED(hr) && spClientSite != NULL)
        {
            m_spUIHandler = spClientSite;
            ATLASSERT(m_spUIHandler != NULL);

            hr = E_NOINTERFACE;
            if (CComQIPtr<ICustomDoc> spCustomDoc = pDispatch)
                hr = spCustomDoc->SetUIHandler(this);
        }
    }

    return hr;
}

inline HRESULT IDocHostUIHandler2Impl::ReleaseUIHandler(IDispatch* pDispatch)
{
    ATLASSERT(pDispatch);
    HRESULT hr = E_NOINTERFACE;

    if (CComQIPtr<ICustomDoc> spCustomDoc = pDispatch)
    {
        hr = spCustomDoc->SetUIHandler(m_spUIHandler);
        m_spUIHandler = NULL;
    }

    return hr;
}

inline STDMETHODIMP IDocHostUIHandler2Impl::QueryInterface(REFIID riid, LPVOID* ppvObject)
{
    ATLASSERT(ppvObject);
    if (::InlineIsEqualGUID(riid, IID_IDocHostUIHandler2) || ::InlineIsEqualGUID(riid, IID_IDocHostUIHandler) || ::InlineIsEqualGUID(riid, IID_IUnknown))
    {
        *ppvObject = this;
        AddRef();
        return S_OK;
    }
    else
    {
        *ppvObject = NULL;
        return E_NOINTERFACE;
    }
}

inline STDMETHODIMP_(ULONG) IDocHostUIHandler2Impl::AddRef()
{
    return 1L;
}

inline STDMETHODIMP_(ULONG) IDocHostUIHandler2Impl::Release()
{
    return 1L;
}

inline STDMETHODIMP IDocHostUIHandler2Impl::HideUI()
{
    ATLASSERT(m_spUIHandler != NULL);
    ATLTRACE(_T("IDocHostUIHandler2Impl::HideUI\n"));

    return m_spUIHandler->HideUI();
}

inline STDMETHODIMP IDocHostUIHandler2Impl::UpdateUI()
{
    ATLASSERT(m_spUIHandler != NULL);
    ATLTRACE(_T("IDocHostUIHandler2Impl::UpdateUI\n"));

    return m_spUIHandler->UpdateUI();
}

inline STDMETHODIMP IDocHostUIHandler2Impl::EnableModeless(BOOL bEnable)
{
    ATLASSERT(m_spUIHandler != NULL);
    ATLTRACE(_T("IDocHostUIHandler2Impl::EnableModeless\n"));

    return m_spUIHandler->EnableModeless(bEnable);
}

inline STDMETHODIMP IDocHostUIHandler2Impl::GetHostInfo(DOCHOSTUIINFO* pInfo)
{
    ATLASSERT(m_spUIHandler != NULL);
    ATLTRACE(_T("IDocHostUIHandler2Impl::GetHostInfo\n"));

    return m_spUIHandler->GetHostInfo(pInfo);
}

inline STDMETHODIMP IDocHostUIHandler2Impl::OnDocWindowActivate(BOOL bActivate)
{
    ATLASSERT(m_spUIHandler != NULL);
    ATLTRACE(_T("IDocHostUIHandler2Impl::OnDocWindowActivate\n"));

    return m_spUIHandler->OnDocWindowActivate(bActivate);
}

inline STDMETHODIMP IDocHostUIHandler2Impl::GetExternal(IDispatch** ppDispatch)
{
    ATLASSERT(m_spUIHandler != NULL);
    ATLTRACE(_T("IDocHostUIHandler2Impl::GetExternal\n"));

    return m_spUIHandler->GetExternal(ppDispatch);
}

inline STDMETHODIMP IDocHostUIHandler2Impl::OnFrameWindowActivate(BOOL bActivate)
{
    ATLASSERT(m_spUIHandler != NULL);
    ATLTRACE(_T("IDocHostUIHandler2Impl::OnFrameWindowActivate\n"));

    return m_spUIHandler->OnFrameWindowActivate(bActivate);
}

inline STDMETHODIMP IDocHostUIHandler2Impl::GetOptionKeyPath(LPOLESTR* ppszKey, DWORD dwReserved)
{
    ATLASSERT(m_spUIHandler != NULL);
    ATLTRACE(_T("IDocHostUIHandler2Impl::GetOptionKeyPath\n"));

    return m_spUIHandler->GetOptionKeyPath(ppszKey, dwReserved);
}

inline STDMETHODIMP IDocHostUIHandler2Impl::GetDropTarget(IDropTarget* pDropTarget, IDropTarget** ppDropTarget)
{
    ATLASSERT(m_spUIHandler != NULL);
    ATLTRACE(_T("IDocHostUIHandler2Impl::GetDropTarget\n"));

    return m_spUIHandler->GetDropTarget(pDropTarget, ppDropTarget);
}

inline STDMETHODIMP IDocHostUIHandler2Impl::TranslateAccelerator(LPMSG pMsg, const GUID* pCmdGroup, DWORD nCmdID)
{
    ATLASSERT(m_spUIHandler != NULL);
    ATLTRACE(_T("IDocHostUIHandler2Impl::TranslateAccelerator\n"));

    return m_spUIHandler->TranslateAccelerator(pMsg, pCmdGroup, nCmdID);
}

inline STDMETHODIMP IDocHostUIHandler2Impl::FilterDataObject(IDataObject* pDataObject, IDataObject** ppDataObject)
{
    ATLASSERT(m_spUIHandler != NULL);
    ATLTRACE(_T("IDocHostUIHandler2Impl::FilterDataObject\n"));

    return m_spUIHandler->FilterDataObject(pDataObject, ppDataObject);
}

inline STDMETHODIMP IDocHostUIHandler2Impl::TranslateUrl(DWORD dwTranslate, LPOLESTR pszURLIn, LPOLESTR* ppszURLOut)
{
    ATLASSERT(m_spUIHandler != NULL);
    ATLTRACE(_T("IDocHostUIHandler2Impl::TranslateUrl\n"));

    return m_spUIHandler->TranslateUrl(dwTranslate, pszURLIn, ppszURLOut);
}

inline STDMETHODIMP IDocHostUIHandler2Impl::ResizeBorder(LPCRECT prcBorder, IOleInPlaceUIWindow* pUIWindow, BOOL bRameWindow)
{
    ATLASSERT(m_spUIHandler != NULL);
    ATLTRACE(_T("IDocHostUIHandler2Impl::ResizeBorder\n"));

    return m_spUIHandler->ResizeBorder(prcBorder, pUIWindow, bRameWindow);
}

inline STDMETHODIMP IDocHostUIHandler2Impl::ShowContextMenu(DWORD /*dwID*/, LPPOINT /*pPoint*/, IUnknown* /*pCmdTargetReserved*/, IDispatch* /*pDispReserved*/)
{
    ATLTRACE(_T("IDocHostUIHandler2Impl::ShowContextMenu\n"));
    return S_OK;
}

inline STDMETHODIMP IDocHostUIHandler2Impl::ShowUI(DWORD dwID, IOleInPlaceActiveObject* pActiveObject, IOleCommandTarget* pCmdTarget, IOleInPlaceFrame* pFrame, IOleInPlaceUIWindow* pDoc)
{
    ATLASSERT(m_spUIHandler != NULL);
    ATLTRACE(_T("IDocHostUIHandler2Impl::ShowUI\n"));

    return m_spUIHandler->ShowUI(dwID, pActiveObject, pCmdTarget, pFrame, pDoc);
}

inline STDMETHODIMP IDocHostUIHandler2Impl::GetOverrideKeyPath(LPOLESTR* ppszKey, DWORD dwReserved)
{
    ATLASSERT(m_spUIHandler != NULL);
    ATLTRACE(_T("IDocHostUIHandler2Impl::GetOverrideKeyPath\n"));

    return m_spUIHandler->GetOverrideKeyPath(ppszKey, dwReserved);
}

}  // namespace atlctrls

#endif  // __ATLCOMHOST_INL__