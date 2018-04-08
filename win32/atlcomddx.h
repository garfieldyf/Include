///////////////////////////////////////////////////////////////////////////////
// atlcomddx.h
//
// Copyright(c) 2018, Garfield. All rights reserved.
// Author  : Garfield
// Version : 4.0
// Creation Date : 2007/1/22

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __ATLCOMDDX_H__
#define __ATLCOMDDX_H__

#include "atlcomutil.h"

///////////////////////////////////////////////////////////////////////////////
// Classes in this file:
//
// CDataExchange<T>

namespace atlutil {

///////////////////////////////////////////////////////////////////////////////
// Interface of the CDataExchangeBase class
//

class ATL_NO_VTABLE CDataExchangeBase
{
// Constructors
protected:
    CDataExchangeBase();

// Implementation
protected:
    static void WINAPI DDX_Radio(HWND hWnd, UINT nID, int& nValue, BOOL bSave);
    static void WINAPI DDX_Check(HWND hWnd, UINT nID, BOOL& bValue, BOOL bSave);
    static void WINAPI DDX_Slider(HWND hWnd, UINT nID, int& nValue, BOOL bSave);

    static void WINAPI DDX_CBIndex(HWND hWnd, UINT nID, int& nValue, BOOL bSave);
    static void WINAPI DDX_LBIndex(HWND hWnd, UINT nID, int& nValue, BOOL bSave);

    static void WINAPI DDX_DateTime(HWND hWnd, UINT nID, SYSTEMTIME& dtValue, BOOL bSave);
    static void WINAPI DDX_MonthCal(HWND hWnd, UINT nID, SYSTEMTIME& dtValue, BOOL bSave);

#ifdef __ATLCOMTIME_H__
    static void WINAPI DDX_DateTime(HWND hWnd, UINT nID, COleDateTime& dtValue, BOOL bSave);
    static void WINAPI DDX_MonthCal(HWND hWnd, UINT nID, COleDateTime& dtValue, BOOL bSave);
#endif

    static BOOL WINAPI DDX_Int(HWND hWnd, UINT nID, UINT& nValue, BOOL bSave, BOOL bSigned);
    static BOOL WINAPI DDX_Float(HWND hWnd, UINT nID, double& dblValue, BOOL bSave, int nDigit);

    template <typename _Ty> static void DDX_Text(HWND hWnd, UINT nID, _Ty& _Value, BOOL bSave, UINT nMaxLength);
    template <typename _Ty> static void DDX_String(HWND hWnd, UINT nID, _Ty& _Value, BOOL bSave, BOOL (WINAPI *pfnGetString)(HWND, _Ty&), void (WINAPI *pfnSetString)(HWND, LPCTSTR));
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CDataExchange class
//

template <typename T>
class ATL_NO_VTABLE CDataExchange : public CDataExchangeBase
{
// Constructors
protected:
    CDataExchange();

// Overridables
public:
    void OnDataExchangeError(UINT nID, int nDataType);
    BOOL OnDataValidateInt(UINT nID, int nData);
    BOOL OnDataValidateFloat(UINT nID, double dblData);
    BOOL OnDataValidateText(UINT nID, LPCTSTR pszData);

// Implementation
protected:
    BOOL DDX_Float(UINT nID, float& fltValue, BOOL bSave, int nDigit = 6);
    BOOL DDX_Float(UINT nID, double& dblValue, BOOL bSave, int nDigit = 6);

    BOOL DDX_Int(UINT nID, UINT& nValue, BOOL bSave, BOOL bSigned);
    template <typename _Ty> BOOL DDX_Int(UINT nID, _Ty& _Value, BOOL bSave, BOOL bSigned);
    template <typename _Ty> BOOL DDX_Text(UINT nID, _Ty& _Value, BOOL bSave, UINT nMaxLength = 0);
};

}  // namespace atlutil

#include "atlcomddx.inl"

#endif  // __ATLCOMDDX_H__