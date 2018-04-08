// utility.cpp : 实现文件
//

#include "stdafx.h"
#include "utility.h"

namespace utility {

void DataBind(HWND hWnd, const ADODB::_ConnectionPtr& spConnection, BSTR bstrCommandText, bool bAddItemData, long lOptions)
{
    _COM_ASSERT(bstrCommandText);
    _COM_ASSERT(::IsWindow(hWnd));
    _COM_ASSERT(spConnection != NULL);
    _COM_ASSERT(spConnection->GetState() == ADODB::adStateOpen);

    // 执行查询
    ADODB::_RecordsetPtr spRecordset;
    if (SUCCEEDED(spConnection->raw_Execute(bstrCommandText, NULL, lOptions, &spRecordset)))
    {
        int nIndex = 0;
        _variant_t vtValue;
        VARIANT vtIndex = { VT_I4 };

        // 获得记录集中字段的值
        while (!spRecordset->GetADOEOF())
        {
            vtIndex.lVal = 0L;
            CHECK_HRESULT(spRecordset->get_Collect(vtIndex, vtValue.GetAddress()));
            if (vtValue.vt != VT_NULL && vtValue.vt != VT_EMPTY)
            {
                ::SendMessage(hWnd, CB_INSERTSTRING, nIndex, reinterpret_cast<LPARAM>(static_cast<LPTSTR>(_bstr_t(vtValue))));

                if (bAddItemData)
                {
                    vtIndex.lVal = 1L;
                    CHECK_HRESULT(spRecordset->get_Collect(vtIndex, vtValue.GetAddress()));
                    if (vtValue.vt != VT_NULL && vtValue.vt != VT_EMPTY)
                        ::SendMessage(hWnd, CB_SETITEMDATA, nIndex, static_cast<long>(vtValue));
                }

                ++nIndex;
            }

            CHECK_HRESULT(spRecordset->raw_MoveNext());
        }

        CHECK_HRESULT(spRecordset->raw_Close());
        ::SendMessage(hWnd, CB_SETCURSEL, 0, 0L);
    }
}

void DataBind(HWND hWnd, const ADODB::_RecordsetPtr& spRecordset, int nColumnWidth, int nFormat, int nBeginColumn,
              int nEndColumn, bool bDeleteAllColumns)
{
    _COM_ASSERT(::IsWindow(hWnd));
    _COM_ASSERT(nBeginColumn >= 0);
    _COM_ASSERT(spRecordset != NULL);
    _COM_ASSERT(spRecordset->GetState() == ADODB::adStateOpen);
    _COM_ASSERT(::GetWindowLong(hWnd, GWL_STYLE) & LVS_REPORT);

    // 获得表头控件的句柄
    HWND hWndHD = ListView_GetHeader(hWnd);
    _COM_ASSERT(::IsWindow(hWndHD));

    // 清除 ListCtrl 中原来的项
    ListView_DeleteAllItems(hWnd);
    if (bDeleteAllColumns)
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
        nEndColumn = (nEndColumn == -1) ? lCount : nEndColumn;
        _COM_ASSERT(nEndColumn >= 0);

        LVCOLUMN lvColumn = { LVCF_TEXT | LVCF_FMT | LVCF_WIDTH };
        lvColumn.fmt = nFormat;

        for (long i = 0; i < lCount; ++i)
        {
            // 获得字段名称
            vtIndex.lVal = i;
            if (SUCCEEDED(spFields->get_Item(vtIndex, &spField)))
                CHECK_HRESULT(spField->get_Name(bstrName.GetAddress()));

            // 将字段名称插入到列表控件中
            lvColumn.cx = (i >= nBeginColumn && i < nEndColumn) ? nColumnWidth : 0;
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
                if (i == 0)
                    ListView_InsertItem(hWnd, &lvItem);
                else
                    ::SendMessage(hWnd, LVM_SETITEMTEXT, nIndex, reinterpret_cast<LPARAM>(&lvItem));
            }
        }

        // 更新当前记录
        ListView_Update(hWnd, nIndex);

        // 移动到下一条记录
        CHECK_HRESULT(spRecordset->raw_MoveNext());
        ++nIndex;
    }
}

void ExportToExcel(const CComSafeArray<VARIANT>& saVariant, LPCTSTR lpszTitle, long lTitleSize, long lHeaderSize,
                   long lCellSize, Excel::XlHAlign TitleHAlign, Excel::XlHAlign CellHAlign, COLORREF clrHeader)
{
    _ASSERTE(saVariant.m_psa != NULL);
    _ASSERTE(saVariant.GetDimensions() == 2);

    // 代表整个 Excel 应用程序
    Excel::_ApplicationPtr spApp;

    // 创建 Application 对象
    if (FAILED(spApp.CreateInstance(_T("Excel.Application"))))
    {
        ::MessageBox(::GetActiveWindow(), _T("无法启动 Excel，请确定您的计算机上是否安装了 Excel！"), _T("提示"),
                     MB_OK | MB_ICONINFORMATION);
        return;
    }

    try
    {
        // 向工作簿集合中添加工作表
        Excel::_WorkbookPtr spBook = spApp->GetWorkbooks()->Add();

        // 获得第一个工作表
        Excel::_WorksheetPtr spSheet = spBook->GetSheets()->GetItem(_variant_t(1L));

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
            spReRange->PutRowHeight(lTitleSize * 2);        // 设置行高
            spReRange->Select();                            // 选中当前表格标题
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
    }
    catch (_com_error& e)
    {
    #ifdef _DEBUG
        dump_com_error(_T("Excel"), e);
    #else
        UNUSED_PARAMETER(e);
        ::MessageBox(::GetActiveWindow(), _T("导入数据到 Excel 失败！"), _T("提示"), MB_OK | MB_ICONWARNING);
    #endif

        // 出现异常，退出 Excel
        spApp->PutDisplayAlerts(VARIANT_FALSE);     // 关闭特定的警告和消息
        spApp->Quit();
    }
}

void ExportToExcel(HWND hWnd, LPCTSTR lpszTitle, long lBeginColumn, long lEndColumn, long lTitleSize, long lHeaderSize,
                   long lCellSize, Excel::XlHAlign TitleHAlign, Excel::XlHAlign CellHAlign, COLORREF clrHeader)
{
    _ASSERTE(::IsWindow(hWnd));
    _ASSERTE(lBeginColumn >= 0);
    _ASSERTE(::GetWindowLong(hWnd, GWL_STYLE) & LVS_REPORT);

    // 获得表头控件的句柄
    HWND hWndHD = ListView_GetHeader(hWnd);
    _ASSERTE(::IsWindow(hWndHD));

    // 获得行数和列数
    long lRows  = ListView_GetItemCount(hWnd) + 1;      // 获得行数（包含表头）
    long lCount = Header_GetItemCount(hWndHD);
    long lCols  = ((lEndColumn == -1 || lEndColumn > lCount) ? lCount : lEndColumn) - lBeginColumn;
    _ASSERTE(lCols > 0);

    // 创建安全数组
    CComSafeArray<VARIANT> saVariant;
    CComSafeArrayBound bound[2];
    bound[0] = lRows;
    bound[1] = lCols;

    if (SUCCEEDED(saVariant.Create(bound, 2)))
    {
        // 获得表头数据
        TCHAR szBuffer[_MAX_PATH + 1] = _T("");
        HDITEM hdItem  = { HDI_TEXT };
        hdItem.pszText = szBuffer;
        hdItem.cchTextMax = _MAX_PATH + 1;
        CComVariant vtValue;
        long lIndex[2];

        // 将表头数据，并存储到安全数组中
        for (lIndex[0] = 0, lIndex[1] = 0; lIndex[1] < lCols; ++lIndex[1])
        {
            Header_GetItem(hWndHD, lIndex[1] + lBeginColumn, &hdItem);
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
                lvItem.iSubItem = lIndex[1] + lBeginColumn;
                lvItem.pszText  = szBuffer;
                ::SendMessage(hWnd, LVM_GETITEMTEXT, lIndex[0] - 1, reinterpret_cast<LPARAM>(&lvItem));

                // 将数据添加到安全数组
                vtValue = lvItem.pszText;
                CHECK_HRESULT(saVariant.MultiDimSetAt(lIndex, vtValue));
            }
        }

        ExportToExcel(saVariant, lpszTitle, lTitleSize, lHeaderSize, lCellSize, TitleHAlign, CellHAlign, clrHeader);
    }
}

void ExportToWord(HWND hWnd, LPCTSTR lpszTitle, long lBeginColumn, long lEndColumn, Word::WdOrientation Orientation,
                  float fltTitleSize, float fltTextSize)
{
    _ASSERTE(::IsWindow(hWnd));
    _ASSERTE(lBeginColumn >= 0);
    _ASSERTE(::GetWindowLong(hWnd, GWL_STYLE) & LVS_REPORT);

    // 获得表头控件的句柄
    HWND hWndHD = ListView_GetHeader(hWnd);
    _ASSERTE(::IsWindow(hWndHD));

    // 获得行数和列数
    long lRows  = ListView_GetItemCount(hWnd) + 1;  // 获得行数（包含表头）
    long lCount = Header_GetItemCount(hWndHD);
    long lCols  = ((lEndColumn == -1 || lEndColumn > lCount) ? lCount : lEndColumn) - lBeginColumn;
    _ASSERTE(lCols > 0);

    // 代表整个 Word 应用程序
    Word::_ApplicationPtr spApp;

    // 创建 Application 对象
    if (FAILED(spApp.CreateInstance(_T("Word.Application"))))
    {
        ::MessageBox(::GetActiveWindow(), _T("无法启动 Word，请确定您的计算机上是否安装了 Word！"), _T("提示"),
                     MB_OK | MB_ICONINFORMATION);
        return;
    }

    try
    {
        Word::_DocumentPtr spDoc = spApp->GetDocuments()->Add();                        // 添加文档
        Word::SelectionPtr spSelection = spDoc->GetActiveWindow()->GetSelection();      // 获得当前光标
        spSelection->GetPageSetup()->PutOrientation(Orientation);                       // 设置页面方向
        Word::RangePtr spRange = spSelection->GetRange();

        // 判断是否有标题
        if (lpszTitle != NULL)
        {
            spSelection->PutText(_bstr_t(lpszTitle));                                   // 标题文本
            spSelection->GetFont()->PutSize(fltTitleSize);                              // 设置字号
            spSelection->GetFont()->PutBold(VARIANT_TRUE);                              // 设置粗体
            spSelection->GetParagraphs()->PutAlignment(Word::wdAlignParagraphCenter);   // 标题居中对齐
            spSelection->EndKey();                                                      // 光标移动到标题末尾
            spRange = spSelection->GetRange();
        }

        // 设置光标后面的文本字体
        spSelection->GetFont()->PutSize(fltTextSize);           // 字号
        spSelection->GetFont()->PutBold(VARIANT_FALSE);         // 正常

        // 绘制表格
        _variant_t vtTableBehavior(Word::wdWord9TableBehavior), vtAutoFit(Word::wdAutoFitContent);
        Word::TablePtr spTable = spDoc->GetTables()->Add(spRange, lRows, lCols, &vtTableBehavior, &vtAutoFit);

        // 表格中行居中对齐
        spTable->GetRows()->PutAlignment(Word::wdAlignRowCenter);

        // 获得表头数据
        TCHAR szBuffer[_MAX_PATH + 1] = _T("");
        HDITEM hdItem  = { HDI_TEXT };
        hdItem.pszText = szBuffer;
        hdItem.cchTextMax = _MAX_PATH + 1;
        _bstr_t bstrText;

        // 将表头数据添加到表格的第一行
        for (long i = 1; i <= lCols; ++i)
        {
            Header_GetItem(hWndHD, i - 1 + lBeginColumn, &hdItem);
            bstrText = hdItem.pszText;
            spTable->Cell(1L, i)->GetRange()->PutText(bstrText);
        }

        // 获得数据
        LVITEM lvItem;
        lvItem.cchTextMax = _MAX_PATH + 1;
        for (long i = 2; i <= lRows; ++i)
        {
            for (long j = 1; j <= lCols; ++j)
            {
                // 获得列表控件上指定位置的文本
                lvItem.iSubItem = j - 1 + lBeginColumn;
                lvItem.pszText  = szBuffer;
                ::SendMessage(hWnd, LVM_GETITEMTEXT, i - 2, reinterpret_cast<LPARAM>(&lvItem));
                
                // 将数据添加到表格中
                bstrText = lvItem.pszText;
                spTable->Cell(i, j)->GetRange()->PutText(bstrText);
            }
        }

        spRange->SetRange(spTable->Cell(1L, 1L)->GetRange()->GetStart(), spTable->Cell(1L, lCols)->GetRange()->GetEnd());
        spRange->GetFont()->PutBold(VARIANT_TRUE);

        // 显示 Word
        spApp->PutVisible(VARIANT_TRUE);
    }
    catch (_com_error& e)
    {
    #ifdef _DEBUG
        dump_com_error(_T("Word"), e);
    #else
        UNUSED_PARAMETER(e);
        ::MessageBox(::GetActiveWindow(), _T("导入数据到 Word 失败！"), _T("提示"), MB_OK | MB_ICONWARNING);
    #endif

        // 出现异常，退出 Word
        _variant_t vtSaveChanges(Word::wdDoNotSaveChanges);
        spApp->Quit(&vtSaveChanges);
    }
}

} // namespace utility