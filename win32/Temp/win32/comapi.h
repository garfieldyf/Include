///////////////////////////////////////////////////////////////////////////////
// comapi.h : ͷ�ļ�
//
// Copyright(c) 2006, Garfield. All rights reserved.
// Author  : Garfield
// Version : 1.0
// Finish Time : 2007/9/10
// Summary :
// 1. ���� Excel �е� COM �ӿ�
//    #import "C:\Program Files\Common Files\Microsoft Shared\Office11\MSO.DLL"
//          rename_namespace("Office") auto_rename raw_interfaces_only no_implementation
//          raw_method_prefix("raw_")
//
//    #import "C:\Program Files\Common Files\Microsoft Shared\VBA\VBA6\VBE6EXT.OLB"
//          rename_namespace("VBE6") raw_interfaces_only no_implementation raw_method_prefix("raw_")
//
//    #import "D:\Microsoft Office\OFFICE11\EXCEL.EXE" rename_namespace("Excel") auto_rename
//          exclude("IFont", "IPicture") no_dual_interfaces
//
// 2. ���� Word �е� COM �ӿ�
//    #import "D:\Microsoft Office\OFFICE11\MSWORD.OLB" rename_namespace("Word") auto_rename
//          no_dual_interfaces
//
// 3. ���� XML �е� COM �ӿ�
//    #import "msxml4.dll" rename_namespace("XML")

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __COMAPI_H__
#define __COMAPI_H__

#ifndef __CONVERT_H__
    #error comapi.h requires convert.h to be included first
#endif

#ifndef _INC_COMDEF
#include <comdef.h>
#endif

#ifndef __ATLSAFE_H__
#include <atlsafe.h>
#endif

#ifndef _INC_COMMCTRL
#include <commctrl.h>
#endif

#ifndef CHECK_HRESULT
  #ifdef _DEBUG
    #ifndef VERIFY
    #define VERIFY              ATLVERIFY
    #endif
    #define CHECK_HRESULT(hr)   VERIFY(SUCCEEDED(hr))
  #else
    #define CHECK_HRESULT(hr)   hr
  #endif
#endif

///////////////////////////////////////////////////////////////////////////////
// Global functions in this file:
//
// ExecuteScalar()
// ExecuteScalar<_Ty>()
// GetAttribute<_Ty>()
// SetAttribute()
// CreateNode()
// DataBind()
// ExportToExcel()

namespace utility {

///////////////////////////////////////////////////////////////////////////////
// COM Error Methods
//
#ifdef _DEBUG
inline void dump_com_error(LPCTSTR lpszSource, _com_error& e)
{
    CString strError;
    strError.Format(_T("%s Error\nCode = %08lx\nCode meaning = %s\nSource = %s\nDescription = %s"), lpszSource, e.Error(),
                    e.ErrorMessage(), static_cast<LPCTSTR>(e.Source()), static_cast<LPCTSTR>(e.Description()));
    ::MessageBox(::GetActiveWindow(), strError, _T("����"), MB_OK | MB_ICONERROR);
}
#endif

///////////////////////////////////////////////////////////////////////////////
// ADO Methods
//
inline _variant_t ExecuteScalar(const ADODB::_ConnectionPtr& spConnection, BSTR bstrCommandText,
                                long lOptions = ADODB::adCmdText, VARIANT* RecordsAffected = NULL)
{
    _COM_ASSERT(bstrCommandText);
    _COM_ASSERT(spConnection != NULL);

    VARIANT _result;
    ::VariantInit(&_result);
    ADODB::_RecordsetPtr spRecordset;

    if (SUCCEEDED(spConnection->raw_Execute(bstrCommandText, RecordsAffected, lOptions, &spRecordset)))
    {
        if (!spRecordset->GetADOEOF())
        {
            VARIANT _Index = { VT_I4 };
            _Index.lVal = 0L;
            CHECK_HRESULT(spRecordset->get_Collect(_Index, &_result));
        }

        CHECK_HRESULT(spRecordset->raw_Close());
    }

    return _variant_t(_result, false);
}

template <typename _Ty>
inline _Ty ExecuteScalar(const ADODB::_ConnectionPtr& spConnection, BSTR bstrCommandText,
                         long lOptions = ADODB::adCmdText, VARIANT* RecordsAffected = NULL)
{
    return VariantTo<_Ty>(ExecuteScalar(spConnection, bstrCommandText, lOptions, RecordsAffected));
}

inline _variant_t ExecuteScalar(const ADODB::_CommandPtr& spCommand, long lOptions = ADODB::adCmdText,
                                VARIANT* RecordsAffected = NULL)
{
    _COM_ASSERT(spCommand != NULL);

    VARIANT _result;
    ::VariantInit(&_result);
    ADODB::_RecordsetPtr spRecordset;

    if (SUCCEEDED(spCommand->raw_Execute(RecordsAffected, NULL, lOptions, &spRecordset)))
    {
        if (!spRecordset->GetADOEOF())
        {
            VARIANT _Index = { VT_I4 };
            _Index.lVal = 0L;
            CHECK_HRESULT(spRecordset->get_Collect(_Index, &_result));
        }

        CHECK_HRESULT(spRecordset->raw_Close());
    }

    return _variant_t(_result, false);
}

template <typename _Ty>
inline _Ty ExecuteScalar(const ADODB::_CommandPtr& spCommand, long lOptions = ADODB::adCmdText,
                         VARIANT* RecordsAffected = NULL)
{
    return VariantTo<_Ty>(ExecuteScalar(spCommand, lOptions, RecordsAffected));
}

///////////////////////////////////////////////////////////////////////////////
// XML Methods
//
template <typename _Ty>
inline _Ty GetAttribute(const XML::IXMLDOMNodePtr& spNode, _bstr_t bstrName)
{
    _COM_ASSERT(spNode != NULL);

    XML::IXMLDOMElementPtr spElement = spNode;
    return VariantTo<_Ty>(spElement->getAttribute(bstrName));
}

inline HRESULT SetAttribute(const XML::IXMLDOMNodePtr& spNode, _bstr_t bstrName, const _variant_t& vtValue)
{
    _COM_ASSERT(spNode != NULL);

    XML::IXMLDOMElementPtr spElement = spNode;
    return spElement->setAttribute(bstrName, vtValue);
}

inline XML::IXMLDOMNodePtr CreateNode(const XML::IXMLDOMDocumentPtr& spDoc, BSTR bstrName, const VARIANT& vtValue = vtMissing,
                                      XML::DOMNodeType nodeType = XML::NODE_ELEMENT)
{
    _COM_ASSERT(bstrName);
    _COM_ASSERT(spDoc != NULL);

    XML::IXMLDOMNode* _result = NULL;
    if (SUCCEEDED(spDoc->raw_createNode(_variant_t(nodeType), bstrName, NULL, &_result)))
    {
        if (vtMissing != vtValue)
            CHECK_HRESULT(_result->put_nodeTypedValue(vtValue));
    }

    return XML::IXMLDOMNodePtr(_result, false);
}

///////////////////////////////////////////////////////////////////////////////
// ListCtrl Methods
//
inline void DataBind(HWND hWnd, const ADODB::_RecordsetPtr& spRecordset, int nColWidth, int nFormat = LVCFMT_CENTER,
                     int nBeginCol = 0, int nEndCol = -1, bool bDeleteAllCols = false)
{
    _COM_ASSERT(::IsWindow(hWnd));
    _COM_ASSERT(nBeginCol >= 0);
    _COM_ASSERT(spRecordset != NULL);
    _COM_ASSERT(spRecordset->GetState() == ADODB::adStateOpen);
    _COM_ASSERT(::GetWindowLong(hWnd, GWL_STYLE) & LVS_REPORT);

    // ��ñ�ͷ�ؼ��ľ��
    HWND hWndHD = ListView_GetHeader(hWnd);
    _COM_ASSERT(::IsWindow(hWndHD));

    // ��� ListCtrl ��ԭ������
    ListView_DeleteAllItems(hWnd);
    if (bDeleteAllCols)
    {
        for (int i = Header_GetItemCount(hWndHD) - 1; i >= 0; --i)
            ListView_DeleteColumn(hWnd, i);
    }

    // ��� ListCtrl ������
    long lCount = Header_GetItemCount(hWndHD);
    VARIANT vtIndex = { VT_I4 };

    // ��� ListCtrl ���б�ͷ�Ͳ������ͷ
    if (lCount <= 0)
    {
        _bstr_t bstrName;
        ADODB::FieldPtr spField;
        ADODB::FieldsPtr spFields = spRecordset->GetFields();

        CHECK_HRESULT(spFields->get_Count(&lCount));
        nEndCol = (nEndCol == -1) ? lCount : nEndCol;
        _COM_ASSERT(nEndCol >= 0);

        LVCOLUMN lvColumn = { LVCF_TEXT | LVCF_FMT | LVCF_WIDTH };
        lvColumn.fmt = nFormat;

        for (long i = 0; i < lCount; ++i)
        {
            // ����ֶ�����
            vtIndex.lVal = i;
            if (SUCCEEDED(spFields->get_Item(vtIndex, &spField)))
                CHECK_HRESULT(spField->get_Name(bstrName.GetAddress()));

            // ���ֶ����Ʋ��뵽�б�ؼ���
            lvColumn.cx = (i >= nBeginCol && i < nEndCol) ? nColWidth : 0;
            lvColumn.pszText = static_cast<LPTSTR>(bstrName);
            ListView_InsertColumn(hWnd, i, &lvColumn);
        }
    }

    int nIndex = 0;
    _bstr_t bstrValue;
    _variant_t vtValue;
    LVITEM lvItem = { LVIF_TEXT };

    // ��ü�¼�����ֶε�ֵ
    while (!spRecordset->GetADOEOF())
    {
        for (long i = 0; i < lCount; ++i)
        {
            // ����ֶ�ֵ
            vtIndex.lVal = i;
            CHECK_HRESULT(spRecordset->get_Collect(vtIndex, vtValue.GetAddress()));
            if (vtValue.vt != VT_NULL && vtValue.vt != VT_EMPTY)
            {
                bstrValue       = vtValue;
                lvItem.iItem    = nIndex;
                lvItem.iSubItem = i;
                lvItem.pszText  = static_cast<LPTSTR>(bstrValue);

                // ���ֶ�ֵ���뵽�б�ؼ���
                (i == 0) ? ListView_InsertItem(hWnd, &lvItem) : ::SendMessage(hWnd, LVM_SETITEMTEXT, nIndex, reinterpret_cast<LPARAM>(&lvItem));
            }
        }

        // ���µ�ǰ��¼
        ListView_Update(hWnd, nIndex);

        // �ƶ�����һ����¼
        CHECK_HRESULT(spRecordset->raw_MoveNext());
        ++nIndex;
    }
}

///////////////////////////////////////////////////////////////////////////////
// Excel Methods
//
inline void SetRangeValue(const Excel::RangePtr& spRange, const _variant_t& vtValue, long lFontSize = 9L,
                          Excel::XlHAlign Align = Excel::xlHAlignCenter, VARIANT_BOOL bFontBold = VARIANT_FALSE)
{
    spRange->PutValue(Excel::xlRangeValueDefault, vtValue);     // ֵ
    spRange->PutHorizontalAlignment(Align);                     // ˮƽ���뷽ʽ
    spRange->PutVerticalAlignment(Excel::xlVAlignCenter);       // ��ֱ���뷽ʽ
    spRange->Font->Size = lFontSize;                            // �ֺ�
    spRange->Font->Bold = bFontBold;                            // ����
    spRange->GetEntireColumn()->AutoFit();                      // �п���и��Զ�����Ϊ�ʵ���ֵ
}

inline HRESULT ExportToExcel(const CComSafeArray<VARIANT>& saVariant, LPCTSTR lpszTitle = NULL, long lTitleSize = 12L,
                             long lHeaderSize = 9L, long lCellSize = 9L, Excel::XlHAlign TitleHAlign = Excel::xlHAlignCenter,
                             Excel::XlHAlign CellHAlign = Excel::xlHAlignCenter, COLORREF clrHeader = RGB(153, 206, 250))
{
    _COM_ASSERT(saVariant.m_psa != NULL);
    _COM_ASSERT(saVariant.GetDimensions() == 2);

    // ���� Application ����
    Excel::_ApplicationPtr spApp;
    HRESULT hr = spApp.CreateInstance(_T("Excel.Application"));
    if (FAILED(hr))
        return hr;

    try
    {
        Excel::_WorkbookPtr spBook   = spApp->GetWorkbooks()->Add();                    // ��������������ӹ�����
        Excel::_WorksheetPtr spSheet = spBook->GetSheets()->GetItem(_variant_t(1L));    // ��õ�һ��������

        // ��ð�ȫ���������������
        long lRows = saVariant.GetCount(0);
        long lCols = saVariant.GetCount(1);

        Excel::RangePtr spRange;        // ����Ԫ��/��Ԫ��Χ
        Excel::RangePtr spReRange;      // ��������������

        // �ж��Ƿ��б���
        if (lpszTitle != NULL)
        {
            // �ϲ����ⵥԪ��
            spRange   = spSheet->GetRange(_variant_t(_T("A1")));
            spReRange = spRange->GetResize(vtMissing, _variant_t(lCols));
            spReRange->Merge();

            // ���ñ����ı������뷽ʽ�����塢�ֺ�
            SetRangeValue(spReRange, _variant_t(lpszTitle), lTitleSize, TitleHAlign, VARIANT_TRUE);
            spReRange->PutRowHeight(lTitleSize * 2);    // �����и�
            spReRange->Select();                        // ѡ�е�ǰ������
        }

        // ���������ݵķ�Χ
        spRange   = (lpszTitle) ? spSheet->GetRange(_variant_t(_T("A2"))) : spSheet->GetRange(_variant_t(_T("A1")));
        spReRange = spRange->GetResize(_variant_t(lRows), _variant_t(lCols));

        // �������
        _variant_t vtValue;
        vtValue.vt = VT_ARRAY | VT_VARIANT;
        vtValue.parray = saVariant.m_psa;

        // ������ݣ����������塢�߿�Ͷ��뷽ʽ
        SetRangeValue(spReRange, vtValue, lCellSize, CellHAlign, VARIANT_FALSE);
        spReRange->Borders->Weight = Excel::xlThin;                     // ��Ԫ��߿�

        // ���ñ�ͷ���塢��ɫ�͵����п���и�
        spReRange = spRange->GetResize(vtMissing, _variant_t(lCols));
        spReRange->GetInterior()->PutColor(_variant_t(clrHeader));      // ��ͷ��ɫ
        spReRange->Font->Size = lHeaderSize;                            // ��ͷ�ֺ�
        spReRange->Font->Bold = VARIANT_TRUE;                           // ��ͷ����

        // �ͷŰ�ȫ����Ĺ���Ȩ�ޣ���ֹ _variant_t ���������������ͷ�
        vtValue.vt = VT_EMPTY;

        // ��ʾ Excel
        spApp->PutVisible(VARIANT_TRUE);
        hr = S_OK;
    }
    catch (_com_error& e)
    {
    #ifdef _DEBUG
        dump_com_error(_T("Excel"), e);
    #endif
        hr = e.Error();
        spApp->PutDisplayAlerts(VARIANT_FALSE);                     // �ر��ض��ľ������Ϣ
        spApp->GetWindows()->GetItem(_variant_t(1L))->Close();      // �ر�������
        spApp->Quit();                                              // �˳� Excel
    }

    return hr;
}

inline HRESULT ExportToExcel(HWND hWnd, LPCTSTR lpszTitle = NULL, long lBeginCol = 0L, long lEndCol = -1L, long lTitleSize = 12L,
                             long lHeaderSize = 9L, long lCellSize = 9L, Excel::XlHAlign TitleHAlign = Excel::xlHAlignCenter,
                             Excel::XlHAlign CellHAlign = Excel::xlHAlignCenter, COLORREF clrHeader = RGB(153, 206, 250))
{
    _COM_ASSERT(::IsWindow(hWnd));
    _COM_ASSERT(lBeginCol >= 0);
    _COM_ASSERT(::GetWindowLong(hWnd, GWL_STYLE) & LVS_REPORT);

    // ��ñ�ͷ�ؼ��ľ��
    HWND hWndHD = ListView_GetHeader(hWnd);
    _COM_ASSERT(::IsWindow(hWndHD));

    // �������������
    long lRows  = ListView_GetItemCount(hWnd) + 1;      // ���������������ͷ��
    long lCount = Header_GetItemCount(hWndHD);
    long lCols  = ((lEndCol == -1 || lEndCol > lCount) ? lCount : lEndCol) - lBeginCol;
    _COM_ASSERT(lCols > 0);

    // ������ȫ����
    CComSafeArray<VARIANT> saVariant;
    CComSafeArrayBound bound[2];
    bound[0] = lRows;
    bound[1] = lCols;

    HRESULT hr = saVariant.Create(bound, 2);
    if (FAILED(hr))
        return hr;

    // ��ñ�ͷ����
    TCHAR szBuffer[_MAX_PATH + 1];
    HDITEM hdItem  = { HDI_TEXT };
    hdItem.pszText = szBuffer;
    hdItem.cchTextMax = _MAX_PATH + 1;
    CComVariant vtValue;
    long lIndex[2];

    // ����ͷ���ݣ����洢����ȫ������
    for (lIndex[0] = 0, lIndex[1] = 0; lIndex[1] < lCols; ++lIndex[1])
    {
        Header_GetItem(hWndHD, lIndex[1] + lBeginCol, &hdItem);
        vtValue = hdItem.pszText;
        CHECK_HRESULT(saVariant.MultiDimSetAt(lIndex, vtValue));
    }

    // ������ݣ����洢����ȫ������
    LVITEM lvItem = { LVIF_TEXT };
    lvItem.cchTextMax = _MAX_PATH + 1;
    for (lIndex[0] = 1; lIndex[0] < lRows; ++lIndex[0])
    {
        for (lIndex[1] = 0; lIndex[1] < lCols; ++lIndex[1])
        {
            // ����б�ؼ���ָ��λ�õ��ı�
            lvItem.iSubItem = lIndex[1] + lBeginCol;
            lvItem.pszText  = szBuffer;
            ::SendMessage(hWnd, LVM_GETITEMTEXT, lIndex[0] - 1, reinterpret_cast<LPARAM>(&lvItem));

            // ��������ӵ���ȫ����
            vtValue = lvItem.pszText;
            CHECK_HRESULT(saVariant.MultiDimSetAt(lIndex, vtValue));
        }
    }

    return ExportToExcel(saVariant, lpszTitle, lTitleSize, lHeaderSize, lCellSize, TitleHAlign, CellHAlign, clrHeader);
}

}  // namespace utility

#endif  // __COMAPI_H__