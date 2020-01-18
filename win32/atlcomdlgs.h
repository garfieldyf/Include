///////////////////////////////////////////////////////////////////////////////
// atlcomdlgs.h
//
// Author : Garfield
// Creation Date : 2009/10/20
//
// Summary :
//  All of CProgressDialog class member function that contains the LPCTSTR
//  parameters, can pass pointer to a null-terminated string, or resource ID
//  of the string.
//
//  For example :
//      SetTitle(_T("title")) or SetTitle(MAKEINTRESOURCE(IDS_TITLE))

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __ATLCOMDLGS_H__
#define __ATLCOMDLGS_H__

#ifdef _WIN32_WCE
    #error atlcomdlgs.h compiles under Windows only
#endif

#include <shlobj.h>
#include <atlstr.h>
#include <atlcomcli.h>

///////////////////////////////////////////////////////////////////////////////
// Classes in this file:
//
// CProgressDialog

namespace atlctrls {

///////////////////////////////////////////////////////////////////////////////
// Interface of the CProgressDialog class
//

class CProgressDialog
{
// Constructors
public:
    CProgressDialog();
    CProgressDialog(const CProgressDialog& that);
    explicit CProgressDialog(IProgressDialog* pProgressDlg);

// Operations
public:
    HRESULT Create(IUnknown* pUnkOuter = NULL, DWORD dwClsContext = CLSCTX_ALL);
    HRESULT StartDialog(HWND hWndParent, DWORD dwFlags = PROGDLG_MODAL | PROGDLG_NOMINIMIZE | PROGDLG_AUTOTIME, IUnknown* pUnkEnableModless = NULL, LPCVOID pvReserved = NULL);
    HRESULT StopDialog();

    HRESULT Timer(DWORD dwTimerAction = PDTIMER_RESET, LPCVOID pvReserved = NULL);
    BOOL HasUserCancelled();

    HRESULT SetProgress(ULONGLONG nCompleted, ULONGLONG nTotal);
    HRESULT SetAnimation(HINSTANCE hInstance, UINT nIDAnimation);

    HRESULT SetTitle(LPCTSTR pszTitle);
    HRESULT SetCancelMsg(LPCTSTR pszCancelMsg, LPCVOID pvReserved = NULL);
    HRESULT SetLine(DWORD dwLineNum, LPCTSTR pszString, BOOL bCompactPath = TRUE, LPCVOID pvReserved = NULL);

// Attributes
public:
    BOOL IsEmpty() const;

// Implementation
public:
    CProgressDialog& operator=(const CProgressDialog& that);
    CProgressDialog& operator=(IProgressDialog* pProgressDlg);

// Data members
public:
    ATL::CComPtr<IProgressDialog> m_spProgressDlg;
};

}  // namespace atlctrls

#include "atlcomdlgs.inl"

#endif  // __ATLCOMDLGS_H__