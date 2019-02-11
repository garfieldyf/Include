///////////////////////////////////////////////////////////////////////////////
// stddefx.h
//
// Copyright(c) 2019, Garfield. All rights reserved.
// Author  : Garfield
// Version : 2.0
// Creation Date : 2007/3/22

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __STDDEFX_H__
#define __STDDEFX_H__

#ifndef _WIN32_WCE
#include <crtdbg.h>
#endif

#include <shlobj.h>
#include <windows.h>
#include <commctrl.h>
#include <shellapi.h>

///////////////////////////////////////////////////////////////////////////////
// Macros in this file:
//

#ifndef ATLASSERT
#define ATLASSERT(expr)                     _ASSERTE(expr)
#endif

#ifndef ATLVERIFY
#ifdef _DEBUG
#define ATLVERIFY(expr)                     ATLASSERT(expr)
#else
#define ATLVERIFY(expr)                     (expr)
#endif
#endif

#ifndef ATL_NOINLINE
#define ATL_NOINLINE                        __declspec(noinline)
#endif

#ifndef ATL_NO_VTABLE
#define ATL_NO_VTABLE                       __declspec(novtable)
#endif

#ifndef _COM_VERIFY
#ifdef _DEBUG
#define _COM_VERIFY(hr)                     ATLASSERT(SUCCEEDED(hr))
#else
#define _COM_VERIFY(hr)                     (hr)
#endif
#endif

#ifndef _COND_VERIFY
#ifdef _DEBUG
#define _COND_VERIFY(expr, result)          ATLASSERT((expr) == (result))
#else
#define _COND_VERIFY(expr, result)          (expr)
#endif
#endif

#ifdef _CRT_DEBUG_DUMP
#define _CRT_DUMP(_Obj, _Printer)           (_Obj).Dump(_Printer)
#define _DUMP_RECT(_Rect, _Printer)         stdutil::DumpRect(_Printer, (_Rect))
#else
#define _CRT_DUMP(_Obj, _Printer)           __noop
#define _DUMP_RECT(_Rect, _Printer)         __noop
#endif

#ifdef _WIN32_WCE
#ifdef  VARIANT_TRUE
#undef  VARIANT_TRUE
#define VARIANT_TRUE                        ((VARIANT_BOOL)-1)
#endif
#endif

#ifndef UNUSED_PARAMETER
#define UNUSED_PARAMETER(var)               (var)
#endif

#ifndef RT_TOOLBAR
#define RT_TOOLBAR                          MAKEINTRESOURCE(241)
#endif

#ifndef CP_ACP
#define CP_ACP                              0
#endif

#ifndef CP_UTF8
#define CP_UTF8                             65001
#endif

#ifndef CP_UTF16
#define CP_UTF16                            1200
#endif

#ifndef CP_UTF16BE
#define CP_UTF16BE                          1201
#endif

#ifndef WM_THEMECHANGED
#define WM_THEMECHANGED                     0x031A
#endif

#ifndef WM_NCXBUTTONDOWN
#define WM_NCXBUTTONDOWN                    0x00AB
#endif

#ifndef WM_NCXBUTTONDBLCLK
#define WM_NCXBUTTONDBLCLK                  0x00AD
#endif

#ifndef HIMETRIC_PER_INCH
#define HIMETRIC_PER_INCH                   2540
#endif

#ifndef ATL_IDW_BAND_FIRST
#define ATL_IDW_BAND_FIRST                  0xEB00
#endif

#ifndef SHCNE_SHELLEVENTS
#define SHCNE_SHELLEVENTS                   0x207FF
#endif

#ifndef SHCNRF_ShellLevel
#define SHCNRF_ShellLevel                   0x0002
#endif

#ifndef SHCNRF_NewDelivery
#define SHCNRF_NewDelivery                  0x8000
#endif

#ifndef SHCNRF_InterruptLevel
#define SHCNRF_InterruptLevel               0x0001
#endif

#ifndef SHCNRF_RecursiveInterrupt
#define SHCNRF_RecursiveInterrupt           0x1000
#endif

#ifndef CO_E_INSUFFICIENT_BUFFER
#define CO_E_INSUFFICIENT_BUFFER            _HRESULT_TYPEDEF_(0x8007007AL)
#endif

#ifndef WM_USERFIRST
#define WM_USERFIRST                        (WM_APP + 0x0100)
#endif

#ifndef CBN_SELCUSTOM
#define CBN_SELCUSTOM                       (WM_USERFIRST + 1)
#endif

#ifndef WM_NCDRAWITEM
#define WM_NCDRAWITEM                       (WM_USERFIRST + 2)
#endif

#ifndef NM_PAINTBKGND
#define NM_PAINTBKGND                       (WM_USERFIRST + 3)
#endif

#ifndef BN_MOUSELEAVE
#define BN_MOUSELEAVE                       (WM_USERFIRST + 4)
#endif

#ifndef BN_MOUSEHOVER
#define BN_MOUSEHOVER                       (WM_USERFIRST + 5)
#endif

#ifndef NM_ENDDRAGDROP
#define NM_ENDDRAGDROP                      (WM_USERFIRST + 6)
#endif

#ifndef TSM_SETPAGERECT
#define TSM_SETPAGERECT                     (WM_USERFIRST + 7)
#endif

#ifndef LVM_GETSORTMODE
#define LVM_GETSORTMODE                     (WM_USERFIRST + 8)
#endif

#ifndef LVM_SETSORTMODE
#define LVM_SETSORTMODE                     (WM_USERFIRST + 9)
#endif

#ifndef LVN_SORTCHANGED
#define LVN_SORTCHANGED                     (WM_USERFIRST + 10)
#endif

#ifndef WM_SHCHANGENOTIFY
#define WM_SHCHANGENOTIFY                   (WM_USERFIRST + 11)
#endif

#ifndef NM_GETDRAGDROPFLAG
#define NM_GETDRAGDROPFLAG                  (WM_USERFIRST + 12)
#endif

#ifndef WM_ASYNCTAST_FINISH
#define WM_ASYNCTAST_FINISH                 (WM_USERFIRST + 13)
#endif

#ifndef WM_GESTUREANIMATION
#define WM_GESTUREANIMATION                 (WM_USERFIRST + 14)
#endif

#define _R16(rgb)                           ((COLOR16)(LOBYTE(rgb) << 8))
#define _G16(rgb)                           ((COLOR16)((WORD)(rgb) & 0xff00))
#define _B16(rgb)                           ((COLOR16)((((DWORD)(rgb)) >> 8) & 0xff00))

#define PEER_GET_MAJOR_VERSION(wVersion)    LOBYTE((WORD)(wVersion))
#define PEER_GET_MINOR_VERSION(wVersion)    HIBYTE((WORD)(wVersion))

#ifndef _countof
template <typename _Ty, size_t _CountOfArray>
char (&__sizeof_helper(_Ty (&_Array)[_CountOfArray]))[_CountOfArray];
#define _countof(_Array)                    sizeof(__sizeof_helper(_Array))
#endif

#ifndef LODWORD
#define LODWORD(_Val)                       ((DWORD)(_Val))
#endif

#ifndef HIDWORD
#define HIDWORD(_Val)                       ((DWORD)(((ULONGLONG)(_Val) >> 32) & 0xffffffff))
#endif

#ifndef MAKEULONGLONG
#define MAKEULONGLONG(low, high)            ((ULONGLONG)(((DWORD)(low)) | (((ULONGLONG)((DWORD)(high))) << 32)))
#endif

#ifndef GetAValue
#define GetAValue(argb)                     (LOBYTE((DWORD)argb >> 24))
#endif

#ifndef ARGB
#define ARGB(a, r, g, b)                    ((COLORREF)((DWORD)(BYTE)(a) << 24 | RGB(r, g, b)))
#endif

#ifndef MAKEARGB
#define MAKEARGB(a, rgb)                    ((Gdiplus::ARGB)(((DWORD)(a) << 24) | (((rgb) & 0xff) << 16) | (((WORD)(rgb)) & 0xff00) | (((rgb) >> 16) & 0xff)))
#endif

#if (_WIN32_WINNT >= 0x0600)
#define HDI_ALLMASK                         (HDI_WIDTH | HDI_HEIGHT | HDI_TEXT | HDI_FORMAT | HDI_LPARAM | HDI_BITMAP | HDI_IMAGE | HDI_ORDER | HDI_FILTER | HDI_STATE)
#elif (_WIN32_IE >= 0x0500)
#define HDI_ALLMASK                         (HDI_WIDTH | HDI_HEIGHT | HDI_TEXT | HDI_FORMAT | HDI_LPARAM | HDI_BITMAP | HDI_IMAGE | HDI_ORDER | HDI_FILTER)
#else
#define HDI_ALLMASK                         (HDI_WIDTH | HDI_HEIGHT | HDI_TEXT | HDI_FORMAT | HDI_LPARAM | HDI_BITMAP | HDI_IMAGE | HDI_ORDER)
#endif

#ifdef __CRT_TRACE__
#define CRTTRACE                            stdutil::CCrtTrace::_CrtTrace
#define CRTLOG(TAG, ...)                    stdutil::CCrtTrace::_CrtTrace.LogPrint((TAG),  __VA_ARGS__)
#define _CrtOutputErrorMessage              stdutil::CCrtTrace::_CrtTrace
#else
#define CRTTRACE                            __noop
#define CRTLOG(TAG, ...)                    __noop
#define _CrtOutputErrorMessage              __noop
#endif

#if (defined(__CRT_CONSOLE_TRACE__) && !defined(_WIN32_WCE))
#define CONTRACE                            stdutil::CCrtConsole::_CrtConsole
#define CONLOG(TAG, ...)                    stdutil::CCrtConsole::_CrtConsole.LogPrint((TAG),  __VA_ARGS__)
#define _ConOutputErrorMessage              stdutil::CCrtConsole::_CrtConsole
#else
#define CONTRACE                            __noop
#define CONLOG(TAG, ...)                    __noop
#define _ConOutputErrorMessage              __noop
#endif

#ifdef __CRT_REPORT_LOG__
#define _CRT_REPORT_LOGFILE(_Filename)      stdutil::CCrtReportLog::_CrtReportLog.SetReportLogFile(_Filename)
#define _CRT_REPORT_LOG(TAG, ...)           stdutil::CCrtReportLog::_CrtReportLog((TAG), __FILE__, __LINE__, __VA_ARGS__)
#else
#define _CRT_REPORT_LOGFILE(_Filename)      __noop
#define _CRT_REPORT_LOG(TAG, ...)           __noop
#endif

#define IMAGE_CODEC_INFO                    ((DWORD)offsetof(Gdiplus::ImageCodecInfo, Clsid))
#define IMAGE_CODEC_CLSID                   IMAGE_CODEC_INFO
#define IMAGE_CODEC_FLAGS                   ((DWORD)offsetof(Gdiplus::ImageCodecInfo, Flags))
#define IMAGE_CODEC_SIGSIZE                 ((DWORD)offsetof(Gdiplus::ImageCodecInfo, SigSize))
#define IMAGE_CODEC_SIGMASK                 ((DWORD)offsetof(Gdiplus::ImageCodecInfo, SigMask))
#define IMAGE_CODEC_DLLNAME                 ((DWORD)offsetof(Gdiplus::ImageCodecInfo, DllName))
#define IMAGE_CODEC_VERSION                 ((DWORD)offsetof(Gdiplus::ImageCodecInfo, Version))
#define IMAGE_CODEC_SIGCOUNT                ((DWORD)offsetof(Gdiplus::ImageCodecInfo, SigCount))
#define IMAGE_CODEC_MIMETYPE                ((DWORD)offsetof(Gdiplus::ImageCodecInfo, MimeType))
#define IMAGE_CODEC_FORMATID                ((DWORD)offsetof(Gdiplus::ImageCodecInfo, FormatID))
#define IMAGE_CODEC_CODECNAME               ((DWORD)offsetof(Gdiplus::ImageCodecInfo, CodecName))
#define IMAGE_CODEC_SIGPATTERN              ((DWORD)offsetof(Gdiplus::ImageCodecInfo, SigPattern))
#define IMAGE_CODEC_FORMAT_DESCRIPTION      ((DWORD)offsetof(Gdiplus::ImageCodecInfo, FormatDescription))
#define IMAGE_CODEC_FILENAME_EXTENSION      ((DWORD)offsetof(Gdiplus::ImageCodecInfo, FilenameExtension))

#define PROPERTY_ITEM_INFO                  ((DWORD)offsetof(Gdiplus::PropertyItem, id))
#define PROPERTY_ITEM_ID                    PROPERTY_ITEM_INFO
#define PROPERTY_ITEM_TYPE                  ((DWORD)offsetof(Gdiplus::PropertyItem, type))
#define PROPERTY_ITEM_VALUE                 ((DWORD)offsetof(Gdiplus::PropertyItem, value))
#define PROPERTY_ITEM_LENGTH                ((DWORD)offsetof(Gdiplus::PropertyItem, length))

#define ENCODER_PARAMETER_INFO              ((DWORD)offsetof(Gdiplus::EncoderParameter, Guid))
#define ENCODER_PARAMETER_GUID              ENCODER_PARAMETER_INFO
#define ENCODER_PARAMETER_TYPE              ((DWORD)offsetof(Gdiplus::EncoderParameter, Type))
#define ENCODER_PARAMETER_VALUE             ((DWORD)offsetof(Gdiplus::EncoderParameter, Value))
#define ENCODER_PARAMETER_NUMBEROFVALUES    ((DWORD)offsetof(Gdiplus::EncoderParameter, NumberOfValues))

#ifndef __ATLDDX_H__
#define BEGIN_DDX_MAP(thisClass) \
public: \
    BOOL UpdateData(BOOL bSave = TRUE, UINT nCtrlID = (UINT)-1) \
    { \
        (bSave); \
        (nCtrlID);

#define DDX_TEXT(nID, var) \
        if (nCtrlID == (UINT)-1 || nCtrlID == nID) \
        { \
            if (!DDX_Text(nID, var, bSave)) \
                return FALSE; \
        }

#define DDX_TEXT_LEN(nID, var, length) \
        if (nCtrlID == (UINT)-1 || nCtrlID == nID) \
        { \
            if (!DDX_Text(nID, var, bSave, length)) \
                return FALSE; \
        }

#define DDX_INT(nID, var) \
        if (nCtrlID == (UINT)-1 || nCtrlID == nID) \
        { \
            if (!DDX_Int(nID, var, bSave, TRUE)) \
                return FALSE; \
        }

#define DDX_UINT(nID, var) \
        if (nCtrlID == (UINT)-1 || nCtrlID == nID) \
        { \
            if (!DDX_Int(nID, var, bSave, FALSE)) \
                return FALSE; \
        }

#define DDX_FLOAT(nID, var) \
        if (nCtrlID == (UINT)-1 || nCtrlID == nID) \
        { \
            if (!DDX_Float(nID, var, bSave)) \
                return FALSE; \
        }

#define DDX_FLOAT_P(nID, var, digits) \
        if (nCtrlID == (UINT)-1 || nCtrlID == nID) \
        { \
            if (!DDX_Float(nID, var, bSave, digits)) \
                return FALSE; \
        }

#define DDX_CBINDEX(nID, var) \
        if (nCtrlID == (UINT)-1 || nCtrlID == nID) \
            DDX_CBIndex(m_hWnd, nID, var, bSave);

#define DDX_CBSTRING(nID, var) \
        if (nCtrlID == (UINT)-1 || nCtrlID == nID) \
            DDX_String(m_hWnd, nID, var, bSave, atlutil::_AtlGetWindowText, atlutil::_AtlCBSelectString);

#define DDX_CBSTRINGEXACT(nID, var) \
        if (nCtrlID == (UINT)-1 || nCtrlID == nID) \
            DDX_String(m_hWnd, nID, var, bSave, atlutil::_AtlGetWindowText, atlutil::_AtlCBSelectStringExact);

#define DDX_LBINDEX(nID, var) \
        if (nCtrlID == (UINT)-1 || nCtrlID == nID) \
            DDX_LBIndex(m_hWnd, nID, var, bSave);

#define DDX_LBSTRING(nID, var) \
        if (nCtrlID == (UINT)-1 || nCtrlID == nID) \
            DDX_String(m_hWnd, nID, var, bSave, atlutil::_AtlGetLBText, atlutil::_AtlLBSelectString);

#define DDX_LBSTRINGEXACT(nID, var) \
        if (nCtrlID == (UINT)-1 || nCtrlID == nID) \
            DDX_String(m_hWnd, nID, var, bSave, atlutil::_AtlGetLBText, atlutil::_AtlLBSelectStringExact);

#define DDX_CHECK(nID, var) \
        if (nCtrlID == (UINT)-1 || nCtrlID == nID) \
            DDX_Check(m_hWnd, nID, var, bSave);

#define DDX_RADIO(nID, var) \
        if (nCtrlID == (UINT)-1 || nCtrlID == nID) \
            DDX_Radio(m_hWnd, nID, var, bSave);

#define DDX_SLIDER(nID, var) \
        if (nCtrlID == (UINT)-1 || nCtrlID == nID) \
            DDX_Slider(m_hWnd, nID, var, bSave);

#define DDX_DATETIME(nID, var) \
        if (nCtrlID == (UINT)-1 || nCtrlID == nID) \
            DDX_DateTime(m_hWnd, nID, var, bSave);

#define DDX_MONTHCAL(nID, var) \
        if (nCtrlID == (UINT)-1 || nCtrlID == nID) \
            DDX_MonthCal(m_hWnd, nID, var, bSave);

#ifdef __ATLWIN_H__
#define DDX_OLE_CONTROL(nID, var) \
        if ((var == NULL) && (nCtrlID == (UINT)-1 || nCtrlID == nID)) \
        { \
            ATLASSERT(!bSave); \
            ATL::CAxWindow wndCtrl = ::GetDlgItem(m_hWnd, nID); \
            _COM_VERIFY(wndCtrl.QueryControl(&var)); \
        }
#endif  // __ATLWIN_H__

#define DDX_CONTROL(nID, var) \
        if ((var.m_hWnd == NULL) && (nCtrlID == (UINT)-1 || nCtrlID == nID)) \
        { \
            ATLASSERT(!bSave); \
            ATLVERIFY(var.SubclassWindow(::GetDlgItem(m_hWnd, nID))); \
        }

#define DDX_CONTROL_HANDLE(nID, var) \
        if ((var.m_hWnd == NULL) && (nCtrlID == (UINT)-1 || nCtrlID == nID)) \
        { \
            ATLASSERT(!bSave); \
            var = ::GetDlgItem(m_hWnd, nID); \
        }

#define CHAIN_DDX_MAP(theChainMember) \
        if (!theChainMember.UpdateData(bSave, nCtrlID)) \
            return FALSE;

#define END_DDX_MAP() \
        return TRUE; \
    }
#endif  // __ATLDDX_H__

#define BEGIN_DDX_PROP_MAP(thisClass) \
public: \
    BOOL UpdatePropData(BOOL bSave = TRUE, UINT nCtrlID = (UINT)-1) \
    { \
        (bSave); \
        (nCtrlID);

#define DDX_PROP_TEXT(nID, pszName, pszDefault) \
        if (nCtrlID == (UINT)-1 || nCtrlID == nID) \
        { \
            if (!DDX_PropText(nID, pszName, bSave, pszDefault)) \
                return FALSE; \
        }

#define DDX_PROP_TEXT_LEN(nID, pszName, pszDefault, length) \
        if (nCtrlID == (UINT)-1 || nCtrlID == nID) \
        { \
            if (!DDX_PropText(nID, pszName, bSave, pszDefault, length)) \
                return FALSE; \
        }

#define DDX_PROP_INT(nID, pszName, nDefault) \
        if (nCtrlID == (UINT)-1 || nCtrlID == nID) \
        { \
            if (!DDX_PropInt(nID, pszName, bSave, TRUE, nDefault)) \
                return FALSE; \
        }

#define DDX_PROP_UINT(nID, pszName, nDefault) \
        if (nCtrlID == (UINT)-1 || nCtrlID == nID) \
        { \
            if (!DDX_PropInt(nID, pszName, bSave, FALSE, nDefault)) \
                return FALSE; \
        }

#define DDX_PROP_FLOAT(nID, pszName, dblDefault) \
        if (nCtrlID == (UINT)-1 || nCtrlID == nID) \
        { \
            if (!DDX_PropFloat(nID, pszName, bSave, dblDefault)) \
                return FALSE; \
        }

#define DDX_PROP_FLOAT_P(nID, pszName, dblDefault, digits) \
        if (nCtrlID == (UINT)-1 || nCtrlID == nID) \
        { \
            if (!DDX_PropFloat(nID, pszName, bSave, dblDefault, digits)) \
                return FALSE; \
        }

#define DDX_PROP_CBINDEX(nID, pszName, nDefault) \
        if (nCtrlID == (UINT)-1 || nCtrlID == nID) \
            DDX_PropCBIndex(m_hWnd, nID, pszName, bSave, nDefault);

#define DDX_PROP_CBSTRING(nID, pszName, pszDefault) \
        if (nCtrlID == (UINT)-1 || nCtrlID == nID) \
            DDX_PropString(m_hWnd, nID, pszName, bSave, pszDefault, atlutil::_AtlGetWindowText, atlutil::_AtlCBSelectString);

#define DDX_PROP_CBSTRINGEXACT(nID, pszName, pszDefault) \
        if (nCtrlID == (UINT)-1 || nCtrlID == nID) \
            DDX_PropString(m_hWnd, nID, pszName, bSave, pszDefault, atlutil::_AtlGetWindowText, atlutil::_AtlCBSelectStringExact);

#define DDX_PROP_LBINDEX(nID, pszName, nDefault) \
        if (nCtrlID == (UINT)-1 || nCtrlID == nID) \
            DDX_PropLBIndex(m_hWnd, nID, pszName, bSave, nDefault);

#define DDX_PROP_LBSTRING(nID, pszName, pszDefault) \
        if (nCtrlID == (UINT)-1 || nCtrlID == nID) \
            DDX_PropString(m_hWnd, nID, pszName, bSave, pszDefault, atlutil::_AtlGetLBText, atlutil::_AtlLBSelectString);

#define DDX_PROP_LBSTRINGEXACT(nID, pszName, pszDefault) \
        if (nCtrlID == (UINT)-1 || nCtrlID == nID) \
            DDX_PropString(m_hWnd, nID, pszName, bSave, pszDefault, atlutil::_AtlGetLBText, atlutil::_AtlLBSelectStringExact);

#define DDX_PROP_CHECK(nID, pszName, bDefault) \
        if (nCtrlID == (UINT)-1 || nCtrlID == nID) \
            DDX_PropCheck(m_hWnd, nID, pszName, bSave, bDefault);

#define DDX_PROP_RADIO(nID, pszName, nDefault) \
        if (nCtrlID == (UINT)-1 || nCtrlID == nID) \
            DDX_PropRadio(m_hWnd, nID, pszName, bSave, nDefault);

#define DDX_PROP_SLIDER(nID, pszName, nDefault) \
        if (nCtrlID == (UINT)-1 || nCtrlID == nID) \
            DDX_PropSlider(m_hWnd, nID, pszName, bSave, nDefault);

#define DDX_PROP_DATETIME(nID, pszName) \
        if (nCtrlID == (UINT)-1 || nCtrlID == nID) \
            DDX_PropDateTime(m_hWnd, nID, pszName, bSave);

#define DDX_PROP_MONTHCAL(nID, pszName) \
        if (nCtrlID == (UINT)-1 || nCtrlID == nID) \
            DDX_PropMonthCal(m_hWnd, nID, pszName, bSave);

#define CHAIN_DDX_PROP_MAP(theChainMember) \
        if (!theChainMember.UpdatePropData(bSave, nCtrlID)) \
            return FALSE;

#define END_DDX_PROP_MAP() \
        return TRUE; \
    }

#define BEGIN_DEFINE_COLORMAP(name) \
    static const COLORMAPENTRY name[] = {

#define COLOR_ENTRY(name, color) \
        { (name), (color) },

#define END_DEFINE_COLORMAP(customName) \
        { _T(""), CLR_INVALID }, \
        { (customName), 0x00FFFFFF }, \
        { NULL, NULL } \
    };

#define BEGIN_PAINTFRAME_MAP(thisClass) \
public: \
    static const _ATL_FRAME_ENTRY* WINAPI _GetFrameEntries() throw() { \
        static const int cxSize = ::GetSystemMetrics(SM_CXSIZE); \
        static const int cySize = ::GetSystemMetrics(SM_CYSIZE); \
        static const _ATL_FRAME_ENTRY _entries[] = {

#define PAINTFRAME_ENTRY(cx, cy, rightPadding, hitcode) \
            { (cx == DEFAULT_SIZE ? cxSize : cx), \
              (cy == DEFAULT_SIZE ? cySize : cy), (rightPadding), (hitcode) },

#define END_PAINTFRAME_MAP() \
            { 0, 0, 0, HTNOWHERE } \
        }; \
        return _entries; \
    }

#define BEGIN_PROPCAT_MAP(thisClass) \
public: \
    static const _ATL_PROPCAT_ENTRY* WINAPI _GetPropcatMap(UINT& nCount) throw() { \
        static const _ATL_PROPCAT_ENTRY _propcat[] = {

#define PROPCAT_ENTRY(dispid, propcat) \
            { (dispid), (propcat), NULL },

#define PROPCAT_NAME_ENTRY(propcat, name) \
            { 0, (propcat), OLESTR(name) },

#define PROPCAT_NAME_ENTRY_EX(propcat, lcid, name) \
            { (DISPID)(lcid), (propcat), OLESTR(name) },

#define END_PROPCAT_MAP() \
        }; nCount = _countof(_propcat); return _propcat; \
    }

#ifndef __ATLCRACK_H__
// Message handler prototype : LRESULT MessageHandler(HWND hWnd, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
#define MESSAGE_HANDLER_EX(msg, func) \
    if (uMsg == msg) \
    { \
        bHandled = TRUE; \
        lResult = func(hWnd, wParam, lParam, bHandled); \
        if (bHandled) \
            return TRUE; \
    }
#endif  // __ATLCRACK_H__

#define CHAIN_WINDOW_MSG_MAP(theChainClass) \
    { \
        if (theChainClass::PaintFrame_WindowProc(uMsg, wParam, lParam, lResult, m_pfnSuperWindowProc)) \
            return TRUE; \
    }

#define CHAIN_DIALOG_MSG_MAP(theChainClass) \
    { \
        if (theChainClass::PaintFrame_WindowProc(uMsg, wParam, lParam, lResult, ::DefWindowProc)) \
            return TRUE; \
    }

#if ((_MSC_VER >= 1400) || defined(_SECURE_ATL)) && !defined(_WIN32_WCE)
#define _tsscanf    _stscanf_s
#define _tssprintf  _stprintf_s
#define _tsmakepath(_PathResult, _Size, _Drive, _Dir, _Fname, _Ext) _tmakepath_s((_PathResult), (_Size), (_Drive), (_Dir), (_Fname), (_Ext))
#define _tssplitpath(_Path, _Drive, _DriveSize, _Dir, _DirSize, _Fname, _FnameSize, _Ext, _ExtSize) _tsplitpath_s((_Path), (_Drive), (_DriveSize), (_Dir), (_DirSize), (_Fname), (_FnameSize), (_Ext), (_ExtSize))
#else
#define _tsscanf    _stscanf
#define _tssprintf  _sntprintf
#define _tsmakepath(_PathResult, _Size, _Drive, _Dir, _Fname, _Ext) _tmakepath((_PathResult), (_Drive), (_Dir), (_Fname), (_Ext))
#define _tssplitpath(_Path, _Drive, _DriveSize, _Dir, _DirSize, _Fname, _FnameSize, _Ext, _ExtSize) _tsplitpath((_Path), (_Drive), (_Dir), (_Fname), (_Ext))
#endif

#ifndef _WIN32_WCE
#define _tsstrrchr(_Start, _End, _Match)        StrRChr((_Start), (_End), (_Match))
#define _tsstrcpy(_DstBuf, _SrcBuf, _DstSize)   lstrcpyn((_DstBuf), (_SrcBuf), (_DstSize))
#define _CreateThread(_Security, _StackSize, _StartAddress, _ArgList, _InitFlag, _ThreadId)     (HANDLE)::_beginthreadex((void*)(_Security), (unsigned)(_StackSize), (unsigned (__stdcall*)(void*))(_StartAddress), (void*)(_ArgList), (unsigned)(_InitFlag), (unsigned*)(_ThreadId))
#else
#define _tsstrrchr(_Start, _End, _Match)        _tcsrchr((_Start), (_Match))
#define _tsstrcpy(_DstBuf, _SrcBuf, _DstSize)   StringCchCopy((_DstBuf), (_DstSize), (_SrcBuf))
#define _CreateThread(_Security, _StackSize, _StartAddress, _ArgList, _InitFlag, _ThreadId)     ::CreateThread((LPSECURITY_ATTRIBUTES)(_Security), (SIZE_T)(_StackSize), (LPTHREAD_START_ROUTINE)(_StartAddress), (LPVOID)(_ArgList), (DWORD)(_InitFlag), (LPDWORD)(_ThreadId))
#endif


///////////////////////////////////////////////////////////////////////////////
// Enumerations in this file:
//

enum
{
    // PAINTFRAME_ENTRY used
    DEFAULT_SIZE = -1,

    // MFC - CBalloonTipCtrl, ATL - CBalloonTipCtrl and CAnimImageCtrlImpl used
    IDT_TIMER = 0xFEFE,

    // ATL - CImageButtonBase and WCE - CImageButtonBase used
    IMAGE_NORMAL   = 0,
    IMAGE_HOVER    = 1,
    IMAGE_FOCUSED  = 1,
    IMAGE_SELECTED = 2,
    IMAGE_DISABLED = 3,

    // MFC - CTabSheet and ATL - CTabSheetImpl used
    TCITEM_EXTRA_SIZE = 8,

    // ATL - CGestureWindowImpl flags
    GWIF_IGNOREPAN    = 0x01,
    GWIF_HSCROLLABLE  = 0x02,
    GWIF_VSCROLLABLE  = 0x04,
    GWIF_IGNORESCROLL = 0x08,

    // WCE - CMessageThread used
    MSGQUEUE_MSGQUIT = 0x7FFFFFFF,

    // CDataExchange and CPropDataExchange used
    DDX_DATA_INT   = 1,
    DDX_DATA_FLOAT = 2,
    DDX_DATA_LAST  = DDX_DATA_FLOAT,

    // MFC - CSortListCtrl and ATL - CSortListCtrlImpl used
    LVSM_ERROR      = -1,
    LVSM_NONE       = 0,
    LVSM_TEXT       = 1,
    LVSM_TEXTNOCASE = 2,
    LVSM_INTEGER    = 3,
    LVSM_DECIMAL    = 4,
    LVSM_DATETIME   = 5,
    LVSM_LAST       = LVSM_DATETIME,

    HTMARGIN = HTHELP + 50,     // ATL - CMarginEditImpl used
    HTCUSTOM = HTHELP + 51,     // ATL - CWindowPaintFrameImpl used

    // CalcTriangle used
    TRI_DIRECTION_UP    = 0,
    TRI_DIRECTION_DOWN  = 1,
    TRI_DIRECTION_LEFT  = 2,
    TRI_DIRECTION_RIGTH = 3,

    // ATL - CDragDropImpl used
    DRAGDROP_FLAG_ALLOW    = 0,
    DRAGDROP_FLAG_DISALLOW = 1,

    // FormatPhysicalAddress used
    PHYSICAL_ADDRESS_LENGTH = 18,

    // CalcDrawRect used
    SIZE_MODE_NORMAL         = 0,
    SIZE_MODE_CENTER         = 1,
    SIZE_MODE_CONTENT        = 2,
    SIZE_MODE_STRETCH        = 3,
    SIZE_MODE_STRETCH_LEFT   = 4,
    SIZE_MODE_STRETCH_RIGHT  = 5,
    SIZE_MODE_STRETCH_CENTER = 6,

    // ATL - CGestureWindowImpl used
    IDT_GESTURE_ANIMATION = 0xFEFD,

    // MFC - CLinkCtrl and ATL - CLinkCtrl Extended styles
    LCS_EX_HOVER          = 0x0001,
    LCS_EX_NOTUNDERLINED  = 0x0002,
    LCS_EX_UNDERLINEHOVER = 0x0004,

    // CStatusBarCtrlImpl Extended styles
    SBS_EX_CUSTOMDRAW = 0x04000000,

    // ATL - IUnknownEvent used
    INVALID_COOKIE_VALUE = 0x7FFFFFFF,

    // CryptCreateHash, FormatSecureHash and FormatMessageDigest used
    MD5_SIZE      = 16,
    SHA_SIZE      = 20,
    SHA1_SIZE     = 20,
    SHA256_SIZE   = 32,
    SHA384_SIZE   = 48,
    SHA512_SIZE   = 64,
    MD5_LENGTH    = (MD5_SIZE << 1) + 1,
    SHA_LENGTH    = (SHA_SIZE << 1) + 1,
    SHA1_LENGTH   = (SHA1_SIZE << 1) + 1,
    SHA256_LENGTH = (SHA256_SIZE << 1) + 1,
    SHA384_LENGTH = (SHA384_SIZE << 1) + 1,
    SHA512_LENGTH = (SHA512_SIZE << 1) + 1,

    // ATL - IUnknownEventImpl Event ID
    IMAPIADVISESINKID   = 1,
    IUNKNOWNEVENTIDLAST = IMAPIADVISESINKID,

    // GradientFillRgn used
    GRADIENT_FILL_REGION_H = GRADIENT_FILL_RECT_H,
    GRADIENT_FILL_REGION_V = GRADIENT_FILL_RECT_V,
};


///////////////////////////////////////////////////////////////////////////////
// Structures in this file:
//

struct _ATL_FRAME_ENTRY
{
    UINT nWidth;
    UINT nHeight;
    UINT nPadding;
    UINT nHitCode;
};

#ifdef __propcat_h__
struct _ATL_PROPCAT_ENTRY
{
    DISPID    dispid;
    PROPCAT   propcat;
    LPCOLESTR pszName;
};
#endif  // __propcat_h__

typedef struct _SBPANE
{
    LPCTSTR pszText;
    int     nWidth;
} SBPANE, *LPSBPANE;

typedef struct _TSITEM
{
    TCITEMHEADER tciHeader;
    HWND         hWnd;
    LPARAM       lParam;
} TSITEM, *LPTSITEM;

typedef struct _DRAWTEXT
{
    TCHAR    szText[128];
    COLORREF clrText;
    LOGFONT  lfLogFont;
    RECT     rcText;
    UINT     nFormat;
} DRAWTEXT, *LPDRAWTEXT;

typedef struct _PAINTITEM
{
    HDC    hDC;
    RECT   rcItem;
    UINT   nState;                        // 0, CDIS_SELECTED, CDIS_HOT
    int    nHitCode;
    BOOL   bActivate;
} PAINTITEM, *LPPAINTITEM;

typedef struct _NMDRAGDROP
{
    NMHDR hdr;
    POINT point;
    UINT  nFlags;
    HWND  hWndDrop;
} NMDRAGDROP, *LPNMDRAGDROP;

typedef struct _SBDRAWPANE
{
    HDC   hDC;
    int   nPane;
    UINT  nType;
    RECT  rcPane;
    DWORD dwData;
} SBDRAWPANE, *LPSBDRAWPANE;

typedef struct _RBDRAWBAND
{
    HDC      hDC;
    int      nBand;
    HWND     hWndChild;
    RECT     rcBand;
    HBITMAP  hbmBack;
    COLORREF clrFrom;
    COLORREF clrTo;
    LPARAM   lParam;
} RBDRAWBAND, *LPRBDRAWBAND;

typedef struct _HDDRAWITEM
{
    HDC    hDC;
    HDITEM item;
    int    nItem;
    UINT   nState;                      // 0, CDIS_SELECTED, CDIS_HOT
    RECT   rcItem;
} HDDRAWITEM, *LPHDDRAWITEM;

typedef struct _VS_CODEPAGE
{
    WORD wLanguage;
    WORD wCodePage;
} VS_CODEPAGE, *LPVS_CODEPAGE;

#ifdef _WIN32_WCE
typedef struct _CALLBACKINFO
{
    HANDLE  hProc;
    FARPROC pfn;
    LPVOID  pvArg0;
} CALLBACKINFO, *LPCALLBACKINFO;
#endif  // _WIN32_WCE

typedef struct _NMMARGINEDIT
{
    NMHDR hdr;
    POINT point;
    UINT  nHitTest;
} NMMARGINEDIT, *LPNMMARGINEDIT;

typedef struct _LVSORTSTRUCT
{
    HWND hWnd;
    int  nSortCol;
    int  nSortMode;
    BOOL bAscending;
} LVSORTSTRUCT, *LPLVSORTSTRUCT;

typedef struct _NMPAINTSTRUCT
{
    NMHDR hdr;
    HDC   hDC;
    RECT  rect;
    POINT point;
} NMPAINTSTRUCT, *LPNMPAINTSTRUCT;

typedef struct _COLORMAPENTRY
{
    LPCTSTR  name;
    COLORREF color;
} COLORMAPENTRY, *LPCOLORMAPENTRY;

typedef struct _NMCBCUSTOMITEM
{
    NMHDR    hdr;
    int      nItem;
    COLORREF color;
} NMCBCUSTOMITEM, *LPNMCBCUSTOMITEM;

typedef struct _NMLVSORTCHANGED
{
    NMHDR hdr;
    BOOL  bAscending;
    int   nNewSortCol;
    int   nOldSortCol;
} NMLVSORTCHANGED, *LPNMLVSORTCHANGED;

#ifdef _WIN32_WCE
typedef struct _MSGQUEUE_MESSAGE
{
    UINT message;
} MSGQUEUE_MESSAGE, *LPMSGQUEUE_MESSAGE;
#endif  // _WIN32_WCE

#endif  // __STDDEFX_H__