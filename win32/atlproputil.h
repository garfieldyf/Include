///////////////////////////////////////////////////////////////////////////////
// atlproputil.h
//
// Author : Garfield
// Creation Date : 2010/4/26

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __ATLPROPUTIL_H__
#define __ATLPROPUTIL_H__

#include "prefutil.h"
#include "atlcomutil.h"

///////////////////////////////////////////////////////////////////////////////
// Classes in this file:
//
// CPropDataExchange<T>

namespace atlutil {

///////////////////////////////////////////////////////////////////////////////
// Interface of the CPropDataExchangeBase class
//

class ATL_NO_VTABLE CPropDataExchangeBase
{
// Constructors
protected:
    CPropDataExchangeBase(stdutil::CProperties* pProperties);

// Implementation
protected:
    void DDX_PropDateTime(HWND hWnd, UINT nID, LPCTSTR pszName, BOOL bSave);
    void DDX_PropMonthCal(HWND hWnd, UINT nID, LPCTSTR pszName, BOOL bSave);

    void DDX_PropRadio(HWND hWnd, UINT nID, LPCTSTR pszName, BOOL bSave, int nDefault);
    void DDX_PropCheck(HWND hWnd, UINT nID, LPCTSTR pszName, BOOL bSave, BOOL bDefault);
    void DDX_PropSlider(HWND hWnd, UINT nID, LPCTSTR pszName, BOOL bSave, int nDefault);

    void DDX_PropCBIndex(HWND hWnd, UINT nID, LPCTSTR pszName, BOOL bSave, int nDefault);
    void DDX_PropLBIndex(HWND hWnd, UINT nID, LPCTSTR pszName, BOOL bSave, int nDefault);

    BOOL DDX_PropInt(HWND hWnd, UINT nID, LPCTSTR pszName, BOOL bSave, BOOL bSigned, UINT& nValue);
    BOOL DDX_PropFloat(HWND hWnd, UINT nID, LPCTSTR pszName, BOOL bSave, double& dblValue, int nDigit);
    void DDX_PropString(HWND hWnd, UINT nID, LPCTSTR pszName, BOOL bSave, LPCTSTR pszDefault, BOOL (WINAPI *pfnGetPropString)(HWND, CString&), void (WINAPI *pfnSetPropString)(HWND, LPCTSTR));

// Data members
protected:
    stdutil::CProperties* m_pProperties;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CPropDataExchange class
//

template <typename T>
class ATL_NO_VTABLE CPropDataExchange : public CPropDataExchangeBase
{
// Constructors
protected:
    CPropDataExchange(stdutil::CProperties* pProperties = NULL);

// Overridables
protected:
    void OnPropExchangeError(UINT nID, LPCTSTR pszName, int nDataType);
    BOOL OnPropValidateInt(UINT nID, LPCTSTR pszName, int nPropValue);
    BOOL OnPropValidateFloat(UINT nID, LPCTSTR pszName, double dblPropValue);
    BOOL OnPropValidateText(UINT nID, LPCTSTR pszName, const CString& strPropValue);

// Implementation
protected:
    BOOL DDX_PropInt(UINT nID, LPCTSTR pszName, BOOL bSave, BOOL bSigned, UINT nDefault);
    BOOL DDX_PropFloat(UINT nID, LPCTSTR pszName, BOOL bSave, double dblDefault, int nDigit = 6);
    BOOL DDX_PropText(UINT nID, LPCTSTR pszName, BOOL bSave, LPCTSTR pszDefault, UINT nMaxLength = 0);
};

}  // namespace atlutil

#include "atlproputil.inl"

#endif  // __ATLPROPUTIL_H__