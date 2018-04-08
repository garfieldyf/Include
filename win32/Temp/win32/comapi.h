///////////////////////////////////////////////////////////////////////////////
// comapi.h : 头文件
//
// Copyright(c) 2006, Garfield. All rights reserved.
// Author  : Garfield
// Version : 1.0
// Finish Time : 2007/9/10
// Summary :
// 1. 导出 Excel 中的 COM 接口
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
// 2. 导出 Word 中的 COM 接口
//    #import "D:\Microsoft Office\OFFICE11\MSWORD.OLB" rename_namespace("Word") auto_rename
//          no_dual_interfaces
//
// 3. 导出 XML 中的 COM 接口
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
    ::MessageBox(::GetActiveWindow(), strError, _T("错误"), MB_OK | MB_ICONERROR);
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

    // 获得表头控件的句柄
    HWND hWndHD = ListView_GetHeader(hWnd);
    _COM_ASSERT(::IsWindow(hWndHD));

    // 清除 ListCtrl 中原来的项
    ListView_DeleteAllItems(hWnd);
    if (bDeleteAllCols)
    {
        for (int i = Header_GetItemCount(hWndHD) - 1; i >= 0; --i)
            ListView_DeleteColumn(hWnd, i);
    }

    // 获得 ListCtrl 中列数
    long lCount = Header_GetItemCount(hWndHD);
    VARIANT vtIndex = { VT_I4 };

    // 如果 ListCtrl 中有表头就不插入表头
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
            // 获得字段名称
            vtIndex.lVal = i;
            if (SUCCEEDED(spFields->get_Item(vtIndex, &spField)))
                CHECK_HRESULT(spField->get_Name(bstrName.GetAddress()));

            // 将字段名称插入到列表控件中
            lvColumn.cx = (i >= nBeginCol && i < nEndCol) ? nColWidth : 0;
            lvColumn.pszText = static_cast<LPTSTR>(bstrName);
            ListView_InsertColumn(hWnd, i, &lvColumn);
        }
    }

    int nIndex = 0;
    _bstr_t bstrValue;
    _variant_t vtValue;
    LVITEM lvItem = { LVIF_TEXT };

    // 获得记录集中字段的值
    while (!spRecordset->GetADOEOF())
    {
        for (long i = 0; i < lCount; ++i)
        {
            // 获得字段值
            vtIndex.lVal = i;
            CHECK_HRESULT(spRecordset->get_Collect(vtIndex, vtValue.GetAddress()));
            if (vtValue.vt != VT_NULL && vtValue.vt != VT_EMPTY)
            {
                bstrValue       = vtValue;
                lvItem.iItem    = nIndex;
                lvItem.iSubItem = i;
                lvItem.pszText  = static_cast<LPTSTR>(bstrValue);

                // 将字段值插入到列表控件中
                (i == 0) ? ListView_InsertItem(hWnd, &lvItem) : ::SendMessage(hWnd, LVM_SETITEMTEXT, nIndex, reinterpret_cast<LPARAM>(&lvItem));
            }
        }

        // 更新当前记录
        ListView_Update(hWnd, nIndex);

        // 移动到下一条记录
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
    spRange->PutValue(Excel::xlRangeValueDefault, vtValue);     // 值
    spRange->PutHorizontalAlignment(Align);                     // 水平对齐方式
    spRange->PutVerticalAlignment(Excel::xlVAlignCenter);       // 垂直对齐方式
    spRange->Font->Size = lFontSize;                            // 字号
    spRange->Font->Bold = bFontBold;                            // 粗体
    spRange->GetEntireColumn()->AutoFit();                      // 列宽和行高自动调整为适当的值
}

inline HRESULT ExportToExcel(const CComSafeArray<VARIANT>& saVariant, LPCTSTR lpszTitle = NULL, long lTitleSize = 12L,
                             long lHeaderSize = 9L, long lCellSize = 9L, Excel::XlHAlign TitleHAlign = Excel::xlHAlignCenter,
                             Excel::XlHAlign CellHAlign = Excel::xlHAlignCenter, COLORREF clrHeader = RGB(153, 206, 250))
{
    _COM_ASSERT(saVariant.m_psa != NULL);
    _COM_ASSERT(saVariant.GetDimensions() == 2);

    // 创建 Application 对象
    Excel::_ApplicationPtr spApp;
    HRESULT hr = spApp.CreateInstance(_T("Excel.Application"));
    if (FAILED(hr))
        return hr;

    try
    {
        Excel::_WorkbookPtr spBook   = spApp->GetWorkbooks()->Add();                    // 向工作簿集合中添加工作表
        Excel::_WorksheetPtr spSheet = spBook->GetSheets()->GetItem(_variant_t(1L));    // 获得第一个工作表

        // 获得安全数组的行数和列数
        long lRows = saVariant.GetCount(0);
        long lCols = saVariant.GetCount(1);

        Excel::RangePtr spRange;        // 代表单元格/单元格范围
        Excel::RangePtr spReRange;      // 代表调整后的区域

        // 判断是否有标题
        if (lpszTitle != NULL)
        {
            // 合并标题单元格
            spRange   = spSheet->GetRange(_variant_t(_T("A1")));
            spReRange = spRange->GetResize(vtMissing, _variant_t(lCols));
            spReRange->Merge();

            // 设置标题文本、对齐方式、字体、字号
            SetRangeValue(spReRange, _variant_t(lpszTitle), lTitleSize, TitleHAlign, VARIANT_TRUE);
            spReRange->PutRowHeight(lTitleSize * 2);    // 设置行高
            spReRange->Select();                        // 选中当前表格标题
        }

        // 获得填充数据的范围
        spRange   = (lpszTitle) ? spSheet->GetRange(_variant_t(_T("A2"))) : spSheet->GetRange(_variant_t(_T("A1")));
        spReRange = spRange->GetResize(_variant_t(lRows), _variant_t(lCols));

        // 获得数据
        _variant_t vtValue;
        vtValue.vt = VT_ARRAY | VT_VARIANT;
        vtValue.parray = saVariant.m_psa;

        // 填充数据，并设置字体、边框和对齐方式
        SetRangeValue(spReRange, vtValue, lCellSize, CellHAlign, VARIANT_FALSE);
        spReRange->Borders->Weight = Excel::xlThin;                     // 单元格边框

        // 设置表头字体、颜色和调整列宽和行高
        spReRange = spRange->GetResize(vtMissing, _variant_t(lCols));
        spReRange->GetInterior()->PutColor(_variant_t(clrHeader));      // 表头颜色
        spReRange->Font->Size = lHeaderSize;                            // 表头字号
        spReRange->Font->Bold = VARIANT_TRUE;                           // 表头字体

        // 释放安全数组的管理权限，防止 _variant_t 的析构函数将其释放
        vtValue.vt = VT_EMPTY;

        // 显示 Excel
        spApp->PutVisible(VARIANT_TRUE);
        hr = S_OK;
    }
    catch (_com_error& e)
    {
    #ifdef _DEBUG
        dump_com_error(_T("Excel"), e);
    #endif
        hr = e.Error();
        spApp->PutDisplayAlerts(VARIANT_FALSE);                     // 关闭特定的警告和消息
        spApp->GetWindows()->GetItem(_variant_t(1L))->Close();      // 关闭主窗口
        spApp->Quit();                                              // 退出 Excel
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

    // 获得表头控件的句柄
    HWND hWndHD = ListView_GetHeader(hWnd);
    _COM_ASSERT(::IsWindow(hWndHD));

    // 获得行数和列数
    long lRows  = ListView_GetItemCount(hWnd) + 1;      // 获得行数（包含表头）
    long lCount = Header_GetItemCount(hWndHD);
    long lCols  = ((lEndCol == -1 || lEndCol > lCount) ? lCount : lEndCol) - lBeginCol;
    _COM_ASSERT(lCols > 0);

    // 创建安全数组
    CComSafeArray<VARIANT> saVariant;
    CComSafeArrayBound bound[2];
    bound[0] = lRows;
    bound[1] = lCols;

    HRESULT hr = saVariant.Create(bound, 2);
    if (FAILED(hr))
        return hr;

    // 获得表头数据
    TCHAR szBuffer[_MAX_PATH + 1];
    HDITEM hdItem  = { HDI_TEXT };
    hdItem.pszText = szBuffer;
    hdItem.cchTextMax = _MAX_PATH + 1;
    CComVariant vtValue;
    long lIndex[2];

    // 将表头数据，并存储到安全数组中
    for (lIndex[0] = 0, lIndex[1] = 0; lIndex[1] < lCols; ++lIndex[1])
    {
        Header_GetItem(hWndHD, lIndex[1] + lBeginCol, &hdItem);
        vtValue = hdItem.pszText;
        CHECK_HRESULT(saVariant.MultiDimSetAt(lIndex, vtValue));
    }

    // 获得数据，并存储到安全数组中
    LVITEM lvItem = { LVIF_TEXT };
    lvItem.cchTextMax = _MAX_PATH + 1;
    for (lIndex[0] = 1; lIndex[0] < lRows; ++lIndex[0])
    {
        for (lIndex[1] = 0; lIndex[1] < lCols; ++lIndex[1])
        {
            // 获得列表控件上指定位置的文本
            lvItem.iSubItem = lIndex[1] + lBeginCol;
            lvItem.pszText  = szBuffer;
            ::SendMessage(hWnd, LVM_GETITEMTEXT, lIndex[0] - 1, reinterpret_cast<LPARAM>(&lvItem));

            // 将数据添加到安全数组
            vtValue = lvItem.pszText;
            CHECK_HRESULT(saVariant.MultiDimSetAt(lIndex, vtValue));
        }
    }

    return ExportToExcel(saVariant, lpszTitle, lTitleSize, lHeaderSize, lCellSize, TitleHAlign, CellHAlign, clrHeader);
}

}  // namespace utility

#endif  // __COMAPI_H__