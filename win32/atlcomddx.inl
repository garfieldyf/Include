///////////////////////////////////////////////////////////////////////////////
// atlcomddx.inl
//
// Copyright(c) 2018, Garfield. All rights reserved.
// Author  : Garfield
// Version : 4.0
// Creation Date : 2007/1/22

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __ATLCOMDDX_INL__
#define __ATLCOMDDX_INL__

#ifndef __ATLCOMDDX_H__
    #error atlcomddx.inl requires atlcomddx.h to be included first
#endif

namespace atlutil {

///////////////////////////////////////////////////////////////////////////////
// Implementation of the CDataExchangeBase class
//

inline CDataExchangeBase::CDataExchangeBase()
{
}

inline void WINAPI CDataExchangeBase::DDX_Radio(HWND hWnd, UINT nID, int& nValue, BOOL bSave)
{
    HWND hWndCtrl = ::GetDlgItem(hWnd, nID);
    ATLASSERT(::IsWindow(hWndCtrl));
    ATLASSERT(::GetWindowLong(hWndCtrl, GWL_STYLE) & WS_GROUP);
    ATLASSERT(::SendMessage(hWndCtrl, WM_GETDLGCODE, 0, 0L) & DLGC_RADIOBUTTON);

    if (bSave)
        nValue = -1;

    int nIndex = 0;
    do
    {
        if (::SendMessage(hWndCtrl, WM_GETDLGCODE, 0, 0L) & DLGC_RADIOBUTTON)
        {
            if (bSave)
            {
                if (::SendMessage(hWndCtrl, BM_GETCHECK, 0, 0L) != BST_UNCHECKED)
                    nValue = nIndex;
            }
            else
            {
                ::SendMessage(hWndCtrl, BM_SETCHECK, (nIndex == nValue), 0L);
            }

            ++nIndex;
        }

        hWndCtrl = ::GetWindow(hWndCtrl, GW_HWNDNEXT);
    }
    while (hWndCtrl != NULL && !(::GetWindowLong(hWndCtrl, GWL_STYLE) & WS_GROUP));
}

inline void WINAPI CDataExchangeBase::DDX_Check(HWND hWnd, UINT nID, BOOL& bValue, BOOL bSave)
{
    HWND hWndCtrl = ::GetDlgItem(hWnd, nID);
    ATLASSERT(::IsWindow(hWndCtrl));
    ATLASSERT(bValue >= BST_UNCHECKED && bValue <= BST_INDETERMINATE);

    if (bSave)
        bValue = (BOOL)::SendMessage(hWndCtrl, BM_GETCHECK, 0, 0L);
    else
        ::SendMessage(hWndCtrl, BM_SETCHECK, (WPARAM)bValue, 0L);
}

inline void WINAPI CDataExchangeBase::DDX_Slider(HWND hWnd, UINT nID, int& nValue, BOOL bSave)
{
    HWND hWndCtrl = ::GetDlgItem(hWnd, nID);
    ATLASSERT(::IsWindow(hWndCtrl));

    if (bSave)
        nValue = (int)::SendMessage(hWndCtrl, TBM_GETPOS, 0, 0L);
    else
        ::SendMessage(hWndCtrl, TBM_SETPOS, TRUE, (LPARAM)nValue);
}

inline void WINAPI CDataExchangeBase::DDX_CBIndex(HWND hWnd, UINT nID, int& nValue, BOOL bSave)
{
    HWND hWndCtrl = ::GetDlgItem(hWnd, nID);
    ATLASSERT(::IsWindow(hWndCtrl));

    if (bSave)
        nValue = (int)::SendMessage(hWndCtrl, CB_GETCURSEL, 0, 0L);
    else
        ::SendMessage(hWndCtrl, CB_SETCURSEL, (WPARAM)nValue, 0L);
}

inline void WINAPI CDataExchangeBase::DDX_LBIndex(HWND hWnd, UINT nID, int& nValue, BOOL bSave)
{
    HWND hWndCtrl = ::GetDlgItem(hWnd, nID);
    ATLASSERT(::IsWindow(hWndCtrl));
    ATLASSERT(!(::GetWindowLong(hWndCtrl, GWL_STYLE) & LBS_MULTIPLESEL));
    ATLASSERT(!(::GetWindowLong(hWndCtrl, GWL_STYLE) & LBS_EXTENDEDSEL));

    if (bSave)
        nValue = (int)::SendMessage(hWndCtrl, LB_GETCURSEL, 0, 0L);
    else
        ::SendMessage(hWndCtrl, LB_SETCURSEL, (WPARAM)nValue, 0L);
}

inline void WINAPI CDataExchangeBase::DDX_DateTime(HWND hWnd, UINT nID, SYSTEMTIME& dtValue, BOOL bSave)
{
    HWND hWndCtrl = ::GetDlgItem(hWnd, nID);
    ATLASSERT(::IsWindow(hWndCtrl));
    ATLASSERT(!(::GetWindowLong(hWndCtrl, GWL_STYLE) & DTS_SHOWNONE));

    if (bSave)
        _COND_VERIFY(DateTime_GetSystemtime(hWndCtrl, &dtValue), GDT_VALID);
    else
        ATLVERIFY(DateTime_SetSystemtime(hWndCtrl, GDT_VALID, &dtValue));
}

inline void WINAPI CDataExchangeBase::DDX_MonthCal(HWND hWnd, UINT nID, SYSTEMTIME& dtValue, BOOL bSave)
{
    HWND hWndCtrl = ::GetDlgItem(hWnd, nID);
    ATLASSERT(::IsWindow(hWndCtrl));
    ATLASSERT(!(::GetWindowLong(hWndCtrl, GWL_STYLE) & MCS_MULTISELECT));

    if (bSave)
        ATLVERIFY(MonthCal_GetCurSel(hWndCtrl, &dtValue));
    else
        ATLVERIFY(MonthCal_SetCurSel(hWndCtrl, &dtValue));
}

#ifdef __ATLCOMTIME_H__
inline void WINAPI CDataExchangeBase::DDX_DateTime(HWND hWnd, UINT nID, COleDateTime& dtValue, BOOL bSave)
{
    HWND hWndCtrl = ::GetDlgItem(hWnd, nID);
    ATLASSERT(::IsWindow(hWndCtrl));

    SYSTEMTIME sysTime;
    if (bSave)
    {
        DWORD dwRet = DateTime_GetSystemtime(hWndCtrl, &sysTime);
        ATLASSERT(dwRet != GDT_ERROR);

        if (dwRet == GDT_VALID)
            dtValue = sysTime;
        else if (dwRet == GDT_NONE)
            dtValue.SetStatus(COleDateTime::null);
    }
    else
    {
        dtValue.GetAsSystemTime(sysTime);
        DateTime_SetSystemtime(hWndCtrl, (dtValue.GetStatus() == COleDateTime::valid ? GDT_VALID : GDT_NONE), &sysTime);
    }
}

inline void WINAPI CDataExchangeBase::DDX_MonthCal(HWND hWnd, UINT nID, COleDateTime& dtValue, BOOL bSave)
{
    HWND hWndCtrl = ::GetDlgItem(hWnd, nID);
    ATLASSERT(::IsWindow(hWndCtrl));
    ATLASSERT(!(::GetWindowLong(hWndCtrl, GWL_STYLE) & MCS_MULTISELECT));

    SYSTEMTIME sysTime;
    if (bSave)
    {
        ATLVERIFY(MonthCal_GetCurSel(hWndCtrl, &sysTime));
        dtValue = sysTime;
    }
    else
    {
        ATLVERIFY(dtValue.GetAsSystemTime(sysTime));
        ATLVERIFY(MonthCal_SetCurSel(hWndCtrl, &sysTime));
    }
}
#endif  // __ATLCOMTIME_H__

inline BOOL WINAPI CDataExchangeBase::DDX_Int(HWND hWnd, UINT nID, UINT& nValue, BOOL bSave, BOOL bSigned)
{
    ATLASSERT(::IsWindow(hWnd));

    BOOL bSuccessful = TRUE;
    if (bSave)
        nValue = ::GetDlgItemInt(hWnd, nID, &bSuccessful, bSigned);
    else
        ATLVERIFY(::SetDlgItemInt(hWnd, nID, nValue, bSigned));

    return bSuccessful;
}

inline BOOL WINAPI CDataExchangeBase::DDX_Float(HWND hWnd, UINT nID, double& dblValue, BOOL bSave, int nDigit)
{
    ATLASSERT(::IsWindow(hWnd));

    TCHAR szBuffer[65];
    BOOL bSuccessful = TRUE;

    if (bSave)
    {
        bSuccessful = (::GetDlgItemText(hWnd, nID, szBuffer, _countof(szBuffer)) && _AtlFloatParse(szBuffer, dblValue));
    }
    else
    {
        ::_tssprintf(szBuffer, _countof(szBuffer), _T("%.*f"), nDigit, dblValue);
        ATLVERIFY(::SetDlgItemText(hWnd, nID, szBuffer));
    }

    return bSuccessful;
}

template <typename _Ty>
inline void CDataExchangeBase::DDX_Text(HWND hWnd, UINT nID, _Ty& _Value, BOOL bSave, UINT nMaxLength)
{
    HWND hWndCtrl = ::GetDlgItem(hWnd, nID);
    ATLASSERT(::IsWindow(hWnd));
    ATLASSERT(::IsWindow(hWndCtrl));

    if (bSave)
    {
        ATLVERIFY(_AtlGetWindowText(hWndCtrl, _Value));
    }
    else
    {
        ATLVERIFY(_AtlSetWindowText(hWndCtrl, _Value));
        if (nMaxLength != 0)
        {
            ATLASSERT(_AtlStringLength(_Value) < nMaxLength);
            ::SendMessage(hWndCtrl, EM_LIMITTEXT, (WPARAM)nMaxLength, 0L);
        }
    }
}

template <typename _Ty>
inline void CDataExchangeBase::DDX_String(HWND hWnd, UINT nID, _Ty& _Value, BOOL bSave, BOOL (WINAPI *pfnGetString)(HWND, _Ty&), void (WINAPI *pfnSetString)(HWND, LPCTSTR))
{
    HWND hWndCtrl = ::GetDlgItem(hWnd, nID);
    ATLASSERT(pfnGetString);
    ATLASSERT(pfnSetString);
    ATLASSERT(::IsWindow(hWndCtrl));

    if (bSave)
        ATLVERIFY(pfnGetString(hWndCtrl, _Value));
    else
        pfnSetString(hWndCtrl, _Value);
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CDataExchange class
//

template <typename T>
inline CDataExchange<T>::CDataExchange()
{
}

template <typename T>
inline void CDataExchange<T>::OnDataExchangeError(UINT /*nID*/, int /*nDataType*/)
{
}

template <typename T>
inline BOOL CDataExchange<T>::OnDataValidateInt(UINT /*nID*/, int /*nData*/)
{
    return TRUE;
}

template <typename T>
inline BOOL CDataExchange<T>::OnDataValidateFloat(UINT /*nID*/, double /*dblData*/)
{
    return TRUE;
}

template <typename T>
inline BOOL CDataExchange<T>::OnDataValidateText(UINT /*nID*/, LPCTSTR /*pszData*/)
{
    return TRUE;
}

template <typename T>
inline BOOL CDataExchange<T>::DDX_Float(UINT nID, float& fltValue, BOOL bSave, int nDigit/* = 6*/)
{
    double dblValue = (double)fltValue;
    BOOL bSuccessful = DDX_Float(nID, dblValue, bSave, nDigit);
    fltValue = (float)dblValue;

    return bSuccessful;
}

template <typename T>
inline BOOL CDataExchange<T>::DDX_Float(UINT nID, double& dblValue, BOOL bSave, int nDigit/* = 6*/)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);
    ATLASSERT(::IsWindow(pThis->m_hWnd));

    BOOL bSuccessful = CDataExchangeBase::DDX_Float(pThis->m_hWnd, nID, dblValue, bSave, nDigit);
    if (bSave)
    {
        if (bSuccessful)
        {
            bSuccessful = pThis->OnDataValidateFloat(nID, dblValue);
        }
        else
        {
            ATLTRACE(_T("ATL: Warning - DDX_FLOAT or DDX_FLOAT_P is failure! (ID = %u)\n"), nID);
            pThis->OnDataExchangeError(nID, DDX_DATA_FLOAT);
        }
    }

    return bSuccessful;
}

template <typename T>
inline BOOL CDataExchange<T>::DDX_Int(UINT nID, UINT& nValue, BOOL bSave, BOOL bSigned)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);
    ATLASSERT(::IsWindow(pThis->m_hWnd));

    BOOL bSuccessful = CDataExchangeBase::DDX_Int(pThis->m_hWnd, nID, nValue, bSave, bSigned);
    if (bSave)
    {
        if (bSuccessful)
        {
            bSuccessful = pThis->OnDataValidateInt(nID, (int)nValue);
        }
        else
        {
            ATLTRACE(_T("ATL: Warning - DDX_INT or DDX_UINT is failure! (ID = %u)\n"), nID);
            pThis->OnDataExchangeError(nID, DDX_DATA_INT);
        }
    }

    return bSuccessful;
}

template <typename T> template <typename _Ty>
inline BOOL CDataExchange<T>::DDX_Int(UINT nID, _Ty& _Value, BOOL bSave, BOOL bSigned)
{
    UINT nValue = (UINT)_Value;
    BOOL bSuccessful = DDX_Int(nID, nValue, bSave, bSigned);
    _Value = (_Ty)nValue;

    return bSuccessful;
}

template <typename T> template <typename _Ty>
inline BOOL CDataExchange<T>::DDX_Text(UINT nID, _Ty& _Value, BOOL bSave, UINT nMaxLength/* = 0*/)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);
    ATLASSERT(::IsWindow(pThis->m_hWnd));

    BOOL bSuccessful = TRUE;
    CDataExchangeBase::DDX_Text(pThis->m_hWnd, nID, _Value, bSave, nMaxLength);
    if (bSave)
        bSuccessful = pThis->OnDataValidateText(nID, _Value);

    return bSuccessful;
}

}  // namespace atlutil

#endif  // __ATLCOMDDX_INL__