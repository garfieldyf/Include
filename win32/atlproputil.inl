///////////////////////////////////////////////////////////////////////////////
// atlproputil.inl
//
// Author : Garfield
// Creation Date : 2010/4/26

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __ATLPROPUTIL_INL__
#define __ATLPROPUTIL_INL__

#ifndef __ATLPROPUTIL_H__
    #error atlproputil.inl requires atlproputil.h to be included first
#endif

namespace atlutil {

///////////////////////////////////////////////////////////////////////////////
// Implementation of the CPropDataExchangeBase class
//

inline CPropDataExchangeBase::CPropDataExchangeBase(stdutil::CProperties* pProperties)
    : m_pProperties(pProperties)
{
}

inline void CPropDataExchangeBase::DDX_PropDateTime(HWND hWnd, UINT nID, LPCTSTR pszName, BOOL bSave)
{
    HWND hWndCtrl = ::GetDlgItem(hWnd, nID);
    ATLASSERT(pszName);
    ATLASSERT(m_pProperties);
    ATLASSERT(::IsWindow(hWnd));
    ATLASSERT(::IsWindow(hWndCtrl));

    SYSTEMTIME sysTime;
    if (bSave)
    {
        DWORD dwRet = DateTime_GetSystemtime(hWndCtrl, &sysTime);
        ATLASSERT(dwRet != GDT_ERROR);

        if (dwRet == GDT_VALID)
            m_pProperties->SetDate(pszName, sysTime);
        else if (dwRet == GDT_NONE)
            m_pProperties->Remove(pszName);
    }
    else
    {
        BOOL bSuccessful = m_pProperties->GetDate(pszName, sysTime);
        DateTime_SetSystemtime(hWndCtrl, (bSuccessful ? GDT_VALID : GDT_NONE), &sysTime);
    }
}

inline void CPropDataExchangeBase::DDX_PropMonthCal(HWND hWnd, UINT nID, LPCTSTR pszName, BOOL bSave)
{
    HWND hWndCtrl = ::GetDlgItem(hWnd, nID);
    ATLASSERT(pszName);
    ATLASSERT(m_pProperties);
    ATLASSERT(::IsWindow(hWnd));
    ATLASSERT(::IsWindow(hWndCtrl));
    ATLASSERT(!(::GetWindowLong(hWndCtrl, GWL_STYLE) & MCS_MULTISELECT));

    SYSTEMTIME sysTime;
    if (bSave)
    {
        ATLVERIFY(MonthCal_GetCurSel(hWndCtrl, &sysTime));
        m_pProperties->SetDate(pszName, sysTime);
    }
    else
    {
        if (m_pProperties->GetDate(pszName, sysTime))
            ATLVERIFY(MonthCal_SetCurSel(hWndCtrl, &sysTime));
    }
}

inline void CPropDataExchangeBase::DDX_PropRadio(HWND hWnd, UINT nID, LPCTSTR pszName, BOOL bSave, int nDefault)
{
    HWND hWndCtrl = ::GetDlgItem(hWnd, nID);
    ATLASSERT(pszName);
    ATLASSERT(m_pProperties);
    ATLASSERT(::IsWindow(hWnd));
    ATLASSERT(::IsWindow(hWndCtrl));
    ATLASSERT(::GetWindowLong(hWndCtrl, GWL_STYLE) & WS_GROUP);
    ATLASSERT(::SendMessage(hWndCtrl, WM_GETDLGCODE, 0, 0L) & DLGC_RADIOBUTTON);

    if (!bSave)
        nDefault = (int)m_pProperties->GetInteger(pszName, (LONGLONG)nDefault);

    int nIndex = 0;
    do
    {
        if (::SendMessage(hWndCtrl, WM_GETDLGCODE, 0, 0L) & DLGC_RADIOBUTTON)
        {
            if (bSave)
            {
                if (::SendMessage(hWndCtrl, BM_GETCHECK, 0, 0L) != BST_UNCHECKED)
                    m_pProperties->SetInteger(pszName, (LONGLONG)nIndex);
            }
            else
            {
                ::SendMessage(hWndCtrl, BM_SETCHECK, (nIndex == nDefault), 0L);
            }

            ++nIndex;
        }

        hWndCtrl = ::GetWindow(hWndCtrl, GW_HWNDNEXT);
    }
    while (hWndCtrl != NULL && !(::GetWindowLong(hWndCtrl, GWL_STYLE) & WS_GROUP));
}

inline void CPropDataExchangeBase::DDX_PropCheck(HWND hWnd, UINT nID, LPCTSTR pszName, BOOL bSave, BOOL bDefault)
{
    HWND hWndCtrl = ::GetDlgItem(hWnd, nID);
    ATLASSERT(pszName);
    ATLASSERT(m_pProperties);
    ATLASSERT(::IsWindow(hWnd));
    ATLASSERT(::IsWindow(hWndCtrl));
    ATLASSERT(bDefault >= BST_UNCHECKED && bDefault <= BST_INDETERMINATE);

    if (bSave)
        m_pProperties->SetInteger(pszName, (LONGLONG)::SendMessage(hWndCtrl, BM_GETCHECK, 0, 0L));
    else
        ::SendMessage(hWndCtrl, BM_SETCHECK, (WPARAM)m_pProperties->GetInteger(pszName, (LONGLONG)bDefault), 0L);
}

inline void CPropDataExchangeBase::DDX_PropSlider(HWND hWnd, UINT nID, LPCTSTR pszName, BOOL bSave, int nDefault)
{
    HWND hWndCtrl = ::GetDlgItem(hWnd, nID);
    ATLASSERT(pszName);
    ATLASSERT(m_pProperties);
    ATLASSERT(::IsWindow(hWnd));
    ATLASSERT(::IsWindow(hWndCtrl));

    if (bSave)
        m_pProperties->SetInteger(pszName, (LONGLONG)::SendMessage(hWndCtrl, TBM_GETPOS, 0, 0L));
    else
        ::SendMessage(hWndCtrl, TBM_SETPOS, TRUE, (LPARAM)m_pProperties->GetInteger(pszName, (LONGLONG)nDefault));
}

inline void CPropDataExchangeBase::DDX_PropCBIndex(HWND hWnd, UINT nID, LPCTSTR pszName, BOOL bSave, int nDefault)
{
    HWND hWndCtrl = ::GetDlgItem(hWnd, nID);
    ATLASSERT(pszName);
    ATLASSERT(m_pProperties);
    ATLASSERT(::IsWindow(hWnd));
    ATLASSERT(::IsWindow(hWndCtrl));

    if (bSave)
        m_pProperties->SetInteger(pszName, (LONGLONG)::SendMessage(hWndCtrl, CB_GETCURSEL, 0, 0L));
    else
        ::SendMessage(hWndCtrl, CB_SETCURSEL, (WPARAM)m_pProperties->GetInteger(pszName, (LONGLONG)nDefault), 0L);
}

inline void CPropDataExchangeBase::DDX_PropLBIndex(HWND hWnd, UINT nID, LPCTSTR pszName, BOOL bSave, int nDefault)
{
    HWND hWndCtrl = ::GetDlgItem(hWnd, nID);
    ATLASSERT(pszName);
    ATLASSERT(m_pProperties);
    ATLASSERT(::IsWindow(hWnd));
    ATLASSERT(::IsWindow(hWndCtrl));
    ATLASSERT(!(::GetWindowLong(hWndCtrl, GWL_STYLE) & LBS_MULTIPLESEL));
    ATLASSERT(!(::GetWindowLong(hWndCtrl, GWL_STYLE) & LBS_EXTENDEDSEL));

    if (bSave)
        m_pProperties->SetInteger(pszName, (LONGLONG)::SendMessage(hWndCtrl, LB_GETCURSEL, 0, 0L));
    else
        ::SendMessage(hWndCtrl, LB_SETCURSEL, (WPARAM)m_pProperties->GetInteger(pszName, (LONGLONG)nDefault), 0L);
}

inline BOOL CPropDataExchangeBase::DDX_PropInt(HWND hWnd, UINT nID, LPCTSTR pszName, BOOL bSave, BOOL bSigned, UINT& nValue)
{
    ATLASSERT(pszName);
    ATLASSERT(m_pProperties);
    ATLASSERT(::IsWindow(hWnd));

    BOOL bSuccessful = TRUE;
    if (bSave)
        nValue = ::GetDlgItemInt(hWnd, nID, &bSuccessful, bSigned);
    else
        ATLVERIFY(::SetDlgItemInt(hWnd, nID, (UINT)m_pProperties->GetInteger(pszName, (LONGLONG)nValue), bSigned));

    return bSuccessful;
}

inline BOOL CPropDataExchangeBase::DDX_PropFloat(HWND hWnd, UINT nID, LPCTSTR pszName, BOOL bSave, double& dblValue, int nDigit)
{
    ATLASSERT(pszName);
    ATLASSERT(m_pProperties);
    ATLASSERT(::IsWindow(hWnd));

    TCHAR szBuffer[65];
    BOOL bSuccessful = TRUE;

    if (bSave)
    {
        bSuccessful = (::GetDlgItemText(hWnd, nID, szBuffer, _countof(szBuffer)) && _AtlFloatParse(szBuffer, dblValue));
    }
    else
    {
        ::_tssprintf(szBuffer, _countof(szBuffer), _T("%.*f"), nDigit, m_pProperties->GetDecimal(pszName, dblValue));
        ATLVERIFY(::SetDlgItemText(hWnd, nID, szBuffer));
    }

    return bSuccessful;
}

inline void CPropDataExchangeBase::DDX_PropString(HWND hWnd, UINT nID, LPCTSTR pszName, BOOL bSave, LPCTSTR pszDefault, BOOL (WINAPI *pfnGetPropString)(HWND, CString&), void (WINAPI *pfnSetPropString)(HWND, LPCTSTR))
{
    HWND hWndCtrl = ::GetDlgItem(hWnd, nID);
    ATLASSERT(pszName);
    ATLASSERT(m_pProperties);
    ATLASSERT(::IsWindow(hWnd));
    ATLASSERT(pfnGetPropString);
    ATLASSERT(pfnSetPropString);
    ATLASSERT(::IsWindow(hWndCtrl));

    if (bSave)
    {
        CString strText;
        ATLVERIFY(pfnGetPropString(hWndCtrl, strText));
        m_pProperties->SetString(pszName, strText);
    }
    else
    {
        pfnSetPropString(hWndCtrl, m_pProperties->GetString(pszName, pszDefault));
    }
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CPropDataExchange class
//

template <typename T>
inline CPropDataExchange<T>::CPropDataExchange(stdutil::CProperties* pProperties/* = NULL*/)
    : CPropDataExchangeBase(pProperties)
{
}

template <typename T>
inline void CPropDataExchange<T>::OnPropExchangeError(UINT /*nID*/, LPCTSTR /*pszName*/, int /*nDataType*/)
{
}

template <typename T>
inline BOOL CPropDataExchange<T>::OnPropValidateInt(UINT /*nID*/, LPCTSTR /*pszName*/, int /*nPropValue*/)
{
    return TRUE;
}

template <typename T>
inline BOOL CPropDataExchange<T>::OnPropValidateFloat(UINT /*nID*/, LPCTSTR /*pszName*/, double /*dblPropValue*/)
{
    return TRUE;
}

template <typename T>
inline BOOL CPropDataExchange<T>::OnPropValidateText(UINT /*nID*/, LPCTSTR /*pszName*/, const CString& /*strPropValue*/)
{
    return TRUE;
}

template <typename T>
inline BOOL CPropDataExchange<T>::DDX_PropInt(UINT nID, LPCTSTR pszName, BOOL bSave, BOOL bSigned, UINT nDefault)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);
    ATLASSERT(pszName);
    ATLASSERT(m_pProperties);
    ATLASSERT(::IsWindow(pThis->m_hWnd));

    BOOL bSuccessful = CPropDataExchangeBase::DDX_PropInt(pThis->m_hWnd, nID, pszName, bSave, bSigned, nDefault);
    if (bSave)
    {
        if (bSuccessful)
        {
            bSuccessful = pThis->OnPropValidateInt(nID, pszName, (int)nDefault);
            if (bSuccessful)
                m_pProperties->SetInteger(pszName, (LONGLONG)nDefault);
        }
        else
        {
            ATLTRACE(_T("ATL: Warning - PROP_DDX_INT or PROP_DDX_UINT is failure! (ID = %u)\n"), nID);
            pThis->OnPropExchangeError(nID, pszName, DDX_DATA_INT);
        }
    }

    return bSuccessful;
}

template <typename T>
inline BOOL CPropDataExchange<T>::DDX_PropFloat(UINT nID, LPCTSTR pszName, BOOL bSave, double dblDefault, int nDigit/* = 6*/)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);
    ATLASSERT(pszName);
    ATLASSERT(m_pProperties);
    ATLASSERT(::IsWindow(pThis->m_hWnd));

    BOOL bSuccessful = CPropDataExchangeBase::DDX_PropFloat(pThis->m_hWnd, nID, pszName, bSave, dblDefault, nDigit);
    if (bSave)
    {
        if (bSuccessful)
        {
            bSuccessful = pThis->OnPropValidateFloat(nID, pszName, dblDefault);
            if (bSuccessful)
                m_pProperties->SetDecimal(pszName, dblDefault, nDigit);
        }
        else
        {
            ATLTRACE(_T("ATL: Warning - PROP_DDX_FLOAT or PROP_DDX_FLOAT_P is failure! (ID = %u)\n"), nID);
            pThis->OnPropExchangeError(nID, pszName, DDX_DATA_FLOAT);
        }
    }

    return bSuccessful;
}

template <typename T>
inline BOOL CPropDataExchange<T>::DDX_PropText(UINT nID, LPCTSTR pszName, BOOL bSave, LPCTSTR pszDefault, UINT nMaxLength/* = 0*/)
{
    T* pThis = static_cast<T*>(this);
    ATLASSERT(pThis);
    ATLASSERT(pszName);
    ATLASSERT(pszDefault);
    ATLASSERT(m_pProperties);
    ATLASSERT(::IsWindow(pThis->m_hWnd));

    HWND hWndCtrl = ::GetDlgItem(pThis->m_hWnd, nID);
    ATLASSERT(::IsWindow(hWndCtrl));

    BOOL bSuccessful = TRUE;
    if (bSave)
    {
        CString strText;
        ATLVERIFY(_AtlGetWindowText(hWndCtrl, strText));

        bSuccessful = pThis->OnPropValidateText(nID, pszName, strText);
        if (bSuccessful)
            m_pProperties->SetString(pszName, strText);
    }
    else
    {
        ATLVERIFY(_AtlSetWindowText(hWndCtrl, m_pProperties->GetString(pszName, pszDefault)));
        if (nMaxLength != 0)
            ::SendMessage(hWndCtrl, EM_LIMITTEXT, (WPARAM)nMaxLength, 0L);
    }

    return bSuccessful;
}

}  // namespace atlutil

#endif  // __ATLPROPUTIL_INL__