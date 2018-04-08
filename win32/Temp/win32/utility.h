///////////////////////////////////////////////////////////////////////////////////////////////////
// utility.h : 头文件
//
// Copyright(c) 2006, Garfield. All rights reserved.
// Author  : Garfield
// Version : 1.5
// Finish Time : 2007/1/12
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
//
// 4. Windows sockets API 的使用  
//    必需附加 Ws2_32.lib 库, 链接器->输入->附加依赖项

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __UTILITY_H__
#define __UTILITY_H__

#ifndef __CONVERTAPI_H__
    #error utility.h requires convert.h to be included first
#endif

#ifndef _INC_COMMCTRL
    #error utility.h requires commctrl.h to be included first
#endif

#ifndef CHECK_HRESULT
  #ifdef _DEBUG
    #ifndef VERIFY
    #define VERIFY                  ATLVERIFY
    #endif
    #define CHECK_HRESULT(hr)       VERIFY(SUCCEEDED(hr))
  #else
    #define CHECK_HRESULT(hr)       hr
  #endif
#endif

#ifndef UNUSED_PARAMETER
  #ifdef _DEBUG
  #define UNUSED_PARAMETER(param)
  #else
  #define UNUSED_PARAMETER(param)   param
  #endif
#endif

namespace utility {

    #define QUERYINTERFACE_ARGUMENTS(Interface, Expression) \
        __uuidof(Interface), reinterpret_cast<void**>(static_cast<Interface**>(Expression))

    //
    // ADO Methods
    //
    inline ADODB::_RecordsetPtr Execute(const ADODB::_ConnectionPtr& spConnection, BSTR bstrCommandText,
                                        long lOptions = ADODB::adCmdText, VARIANT* RecordsAffected = NULL) throw()
    {
        _COM_ASSERT(bstrCommandText);
        _COM_ASSERT(spConnection != NULL);

        ADODB::_Recordset* pRecordset = NULL;
        CHECK_HRESULT(spConnection->raw_Execute(bstrCommandText, RecordsAffected, lOptions, &pRecordset));
        return ADODB::_RecordsetPtr(pRecordset, false);
    }

    inline HRESULT ExecuteNonQuery(const ADODB::_ConnectionPtr& spConnection, BSTR bstrCommandText,
                                   long lOptions = ADODB::adCmdText, VARIANT* RecordsAffected = NULL) throw()
    {
        _COM_ASSERT(bstrCommandText);
        _COM_ASSERT(spConnection != NULL);

        return spConnection->raw_Execute(bstrCommandText, RecordsAffected, ADODB::adExecuteNoRecords | lOptions, NULL);
    }

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

    inline ADODB::_RecordsetPtr Execute(const ADODB::_CommandPtr& spCommand, long lOptions = ADODB::adCmdText,
                                        VARIANT* RecordsAffected = NULL) throw()
    {
        _COM_ASSERT(spCommand != NULL);

        ADODB::_Recordset* pRecordset = NULL;
        CHECK_HRESULT(spCommand->raw_Execute(RecordsAffected, NULL, lOptions, &pRecordset));
        return ADODB::_RecordsetPtr(pRecordset, false);
    }

    inline HRESULT ExecuteNonQuery(const ADODB::_CommandPtr& spCommand, long lOptions = ADODB::adCmdText,
                                   VARIANT* RecordsAffected = NULL) throw()
    {
        _COM_ASSERT(spCommand != NULL);
        return spCommand->raw_Execute(RecordsAffected, NULL, ADODB::adExecuteNoRecords | lOptions, NULL);
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

    //
    // XML Methods
    //
    inline _bstr_t GetNodeName(const XML::IXMLDOMNodePtr& spNode)
    {
        _COM_ASSERT(spNode != NULL);

        BSTR _result;
        CHECK_HRESULT(spNode->get_nodeName(&_result));
        return _bstr_t(_result, false);
    }

    inline _bstr_t GetNodeValue(const XML::IXMLDOMNodePtr& spNode)
    {
        _COM_ASSERT(spNode != NULL);

        BSTR _result;
        CHECK_HRESULT(spNode->get_text(&_result));
        return _bstr_t(_result, false);
    }

    inline HRESULT SetNodeValue(const XML::IXMLDOMNodePtr& spNode, BSTR bstrValue) throw()
    {
        _COM_ASSERT(bstrValue);
        _COM_ASSERT(spNode != NULL);

        return spNode->put_text(bstrValue);
    }

    inline _variant_t GetNodeAttribute(const XML::IXMLDOMNodePtr& spNode, BSTR bstrName)
    {
        _COM_ASSERT(bstrName);
        _COM_ASSERT(spNode != NULL);

        VARIANT _result;
        ::VariantInit(&_result);
        XML::IXMLDOMElementPtr spElement;
        
        if (SUCCEEDED(spNode->QueryInterface(QUERYINTERFACE_ARGUMENTS(XML::IXMLDOMElement, &spElement))))
            CHECK_HRESULT(spElement->raw_getAttribute(bstrName, &_result));

        return _variant_t(_result, false);
    }

    inline HRESULT SetNodeAttribute(const XML::IXMLDOMNodePtr& spNode, BSTR bstrName, const VARIANT& vtAttrib) throw()
    {
        _COM_ASSERT(bstrName);
        _COM_ASSERT(spNode != NULL);

        XML::IXMLDOMElementPtr spElement;
        HRESULT _hr = spNode->QueryInterface(QUERYINTERFACE_ARGUMENTS(XML::IXMLDOMElement, &spElement));

        return SUCCEEDED(_hr) ? spElement->raw_setAttribute(bstrName, vtAttrib) : _hr;
    }

    //
    // Excel Methods
    //
    inline Excel::_WorkbookPtr OpenTemplate(const Excel::_ApplicationPtr& spApp, BSTR bstrFileName)
    {
        _COM_ASSERT(bstrFileName);
        _COM_ASSERT(spApp != NULL);
        
        return spApp->GetWorkbooks()->Open(bstrFileName);
    }

    inline _variant_t CopyFrom(const Excel::_WorkbookPtr& spSrcBook, const Excel::_WorksheetPtr& spDstSheet,
                               const _variant_t& vtIndex, const _variant_t& vtSrcBegin, const _variant_t& vtSrcEnd,
                               const _variant_t& vtDstBegin, const _variant_t& vtDstEnd)
    {
        _COM_ASSERT(spSrcBook != NULL);
        _COM_ASSERT(spDstSheet != NULL);

        Excel::_WorksheetPtr spSrcSheet = spSrcBook->GetSheets()->GetItem(vtIndex);
        Excel::RangePtr spSrcRange = spSrcSheet->GetRange(vtSrcBegin, vtSrcEnd);
        Excel::RangePtr spDstRange = spDstSheet->GetRange(vtDstBegin, vtDstEnd);
        
        return spSrcRange->Copy(_variant_t(static_cast<IDispatch*>(spDstRange)));
    }

    //
    // Word Methods
    //
    inline Word::_DocumentPtr OpenTemplate(const Word::_ApplicationPtr& spApp, VARIANT* vtTemplate = &vtMissing)
    {
        _COM_ASSERT(spApp != NULL);
        return spApp->GetDocuments()->Add(vtTemplate);
    }

    inline void SetCellValue(const Word::TablePtr& spTable, long lRow, long lColumn, const _bstr_t& bstrValue)
    {
        _COM_ASSERT(spTable != NULL);
        spTable->Cell(lRow, lColumn)->GetRange()->PutText(bstrValue);
    }

    inline void SetText(const Word::BookmarkPtr& spBookmark, const _bstr_t& bstrText)
    {
        _COM_ASSERT(spBookmark != NULL);
        spBookmark->GetRange()->PutText(bstrText);
    }

    inline Word::BookmarkPtr FindBookmark(const Word::BookmarksPtr& spBookmarks, const _bstr_t& bstrBookmark)
    {
        _COM_ASSERT(spBookmarks != NULL);

        VARIANT _Index = { VT_I4 };
        Word::BookmarkPtr spBookmark;
        long lCount = spBookmarks->GetCount();
        
        for (long i = 1; i <= lCount; ++i)
        {
            _Index.lVal = i;
            spBookmark = spBookmarks->Item(&_Index);
            if (spBookmark->GetName() == bstrBookmark)
                return spBookmark;
        }

        return Word::BookmarkPtr();
    }

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

    void DataBind(HWND hWnd, const ADODB::_ConnectionPtr& spConnection, BSTR bstrCommandText, bool bAddItemData = true,
                  long lOptions = ADODB::adCmdText);

    void DataBind(HWND hWnd, const ADODB::_RecordsetPtr& spRecordset, int nColumnWidth, int nFormat = LVCFMT_CENTER,
                  int nBeginColumn = 0, int nEndColumn = -1, bool bDeleteAllColumns = false);

    void ExportToWord(HWND hWnd, LPCTSTR lpszTitle = NULL, long lBeginColumn = 0L, long lEndColumn = -1L,
                      Word::WdOrientation Orientation = Word::wdOrientPortrait, float fltTitleSize = 14.0F,
                      float fltTextSize = 9.0F);

    void ExportToExcel(const CComSafeArray<VARIANT>& saVariant, LPCTSTR lpszTitle = NULL, long lTitleSize = 12L,
                       long lHeaderSize = 9L, long lCellSize = 9L, Excel::XlHAlign TitleHAlign = Excel::xlHAlignCenter,
                       Excel::XlHAlign CellHAlign = Excel::xlHAlignCenter, COLORREF clrHeader = RGB(153, 206, 250));

    void ExportToExcel(HWND hWnd, LPCTSTR lpszTitle = NULL, long lBeginColumn = 0L, long lEndColumn = -1L,
                       long lTitleSize = 12L, long lHeaderSize = 9L, long lCellSize = 9L,
                       Excel::XlHAlign TitleHAlign = Excel::xlHAlignCenter, Excel::XlHAlign CellHAlign = Excel::xlHAlignCenter,
                       COLORREF clrHeader = RGB(153, 206, 250));

#ifdef _DEBUG
    inline void dump_com_error(LPCTSTR lpszSource, _com_error& e)
    {
        CString strError;
        strError.Format(_T("%s Error\nCode = %08lx\nCode meaning = %s\nSource = %s\nDescription = %s"), lpszSource,
                        e.Error(), e.ErrorMessage(), static_cast<LPCTSTR>(e.Source()), static_cast<LPCTSTR>(e.Description()));
        ::MessageBox(::GetActiveWindow(), strError, _T("错误"), MB_OK | MB_ICONERROR);
    }
#endif

} // namespace utility

#endif  // __UTILITY_H__