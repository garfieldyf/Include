///////////////////////////////////////////////////////////////////////////////
// atlcomdlgs.inl
//
// Copyright(c) 2018, Garfield. All rights reserved.
// Author  : Garfield
// Version : 1.0
// Creation Date : 2009/10/20

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __ATLCOMDLGS_INL__
#define __ATLCOMDLGS_INL__

#ifndef __ATLCOMDLGS_H__
    #error atlcomdlgs.inl requires atlcomdlgs.h to be included first
#endif

namespace atlctrls {

///////////////////////////////////////////////////////////////////////////////
// Implementation of the CProgressDialog class
//

inline CProgressDialog::CProgressDialog()
{
}

inline CProgressDialog::CProgressDialog(const CProgressDialog& that)
    : m_spProgressDlg(that.m_spProgressDlg)
{
}

inline CProgressDialog::CProgressDialog(IProgressDialog* pProgressDlg)
    : m_spProgressDlg(pProgressDlg)
{
}

inline HRESULT CProgressDialog::Create(IUnknown* pUnkOuter/* = NULL*/, DWORD dwClsContext/* = CLSCTX_ALL*/)
{
    ATLASSERT(m_spProgressDlg == NULL);
    return ::CoCreateInstance(CLSID_ProgressDialog, pUnkOuter, dwClsContext, IID_IProgressDialog, (LPVOID*)&m_spProgressDlg);
}

inline HRESULT CProgressDialog::StartDialog(HWND hWndParent, DWORD dwFlags/* = PROGDLG_MODAL | PROGDLG_NOMINIMIZE | PROGDLG_AUTOTIME*/, IUnknown* pUnkEnableModless/* = NULL*/, LPCVOID pvReserved/* = NULL*/)
{
    ATLASSERT(m_spProgressDlg != NULL);
    return m_spProgressDlg->StartProgressDialog(hWndParent, pUnkEnableModless, dwFlags, pvReserved);
}

inline HRESULT CProgressDialog::StopDialog()
{
    ATLASSERT(m_spProgressDlg != NULL);
    return m_spProgressDlg->StopProgressDialog();
}

inline HRESULT CProgressDialog::Timer(DWORD dwTimerAction/* = PDTIMER_RESET*/, LPCVOID pvReserved/* = NULL*/)
{
    ATLASSERT(m_spProgressDlg != NULL);
    return m_spProgressDlg->Timer(dwTimerAction, pvReserved);
}

inline BOOL CProgressDialog::HasUserCancelled()
{
    ATLASSERT(m_spProgressDlg != NULL);
    return m_spProgressDlg->HasUserCancelled();
}

inline HRESULT CProgressDialog::SetProgress(ULONGLONG nCompleted, ULONGLONG nTotal)
{
    ATLASSERT(m_spProgressDlg != NULL);
    return m_spProgressDlg->SetProgress64(nCompleted, nTotal);
}

inline HRESULT CProgressDialog::SetAnimation(HINSTANCE hInstance, UINT nIDAnimation)
{
    ATLASSERT(m_spProgressDlg != NULL);
    return m_spProgressDlg->SetAnimation(hInstance, nIDAnimation);
}

inline HRESULT CProgressDialog::SetTitle(LPCTSTR pszTitle)
{
    ATLASSERT(m_spProgressDlg != NULL);
    return m_spProgressDlg->SetTitle(CStringW(pszTitle));
}

inline HRESULT CProgressDialog::SetCancelMsg(LPCTSTR pszCancelMsg, LPCVOID pvReserved/* = NULL*/)
{
    ATLASSERT(m_spProgressDlg != NULL);
    return m_spProgressDlg->SetCancelMsg(CStringW(pszCancelMsg), pvReserved);
}

inline HRESULT CProgressDialog::SetLine(DWORD dwLineNum, LPCTSTR pszString, BOOL bCompactPath/* = TRUE*/, LPCVOID pvReserved/* = NULL*/)
{
    ATLASSERT(m_spProgressDlg != NULL);
    return m_spProgressDlg->SetLine(dwLineNum, CStringW(pszString), bCompactPath, pvReserved);
}

inline BOOL CProgressDialog::IsEmpty() const
{
    return (m_spProgressDlg == NULL);
}

inline CProgressDialog& CProgressDialog::operator=(const CProgressDialog& that)
{
    m_spProgressDlg = that.m_spProgressDlg;
    return *this;
}

inline CProgressDialog& CProgressDialog::operator=(IProgressDialog* pProgressDlg)
{
    m_spProgressDlg = pProgressDlg;
    return *this;
}

}  // namespace atlctrls

#endif  // __ATLCOMDLGS_INL__