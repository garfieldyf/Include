// utility.cpp : ʵ���ļ�
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

    // ִ�в�ѯ
    ADODB::_RecordsetPtr spRecordset;
    if (SUCCEEDED(spConnection->raw_Execute(bstrCommandText, NULL, lOptions, &spRecordset)))
    {
        int nIndex = 0;
        _variant_t vtValue;
        VARIANT vtIndex = { VT_I4 };

        // ��ü�¼�����ֶε�ֵ
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

    // ��ñ�ͷ�ؼ��ľ��
    HWND hWndHD = ListView_GetHeader(hWnd);
    _COM_ASSERT(::IsWindow(hWndHD));

    // ��� ListCtrl ��ԭ������
    ListView_DeleteAllItems(hWnd);
    if (bDeleteAllColumns)
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
        nEndColumn = (nEndColumn == -1) ? lCount : nEndColumn;
        _COM_ASSERT(nEndColumn >= 0);

        LVCOLUMN lvColumn = { LVCF_TEXT | LVCF_FMT | LVCF_WIDTH };
        lvColumn.fmt = nFormat;

        for (long i = 0; i < lCount; ++i)
        {
            // ����ֶ�����
            vtIndex.lVal = i;
            if (SUCCEEDED(spFields->get_Item(vtIndex, &spField)))
                CHECK_HRESULT(spField->get_Name(bstrName.GetAddress()));

            // ���ֶ����Ʋ��뵽�б�ؼ���
            lvColumn.cx = (i >= nBeginColumn && i < nEndColumn) ? nColumnWidth : 0;
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
                if (i == 0)
                    ListView_InsertItem(hWnd, &lvItem);
                else
                    ::SendMessage(hWnd, LVM_SETITEMTEXT, nIndex, reinterpret_cast<LPARAM>(&lvItem));
            }
        }

        // ���µ�ǰ��¼
        ListView_Update(hWnd, nIndex);

        // �ƶ�����һ����¼
        CHECK_HRESULT(spRecordset->raw_MoveNext());
        ++nIndex;
    }
}

void ExportToExcel(const CComSafeArray<VARIANT>& saVariant, LPCTSTR lpszTitle, long lTitleSize, long lHeaderSize,
                   long lCellSize, Excel::XlHAlign TitleHAlign, Excel::XlHAlign CellHAlign, COLORREF clrHeader)
{
    _ASSERTE(saVariant.m_psa != NULL);
    _ASSERTE(saVariant.GetDimensions() == 2);

    // �������� Excel Ӧ�ó���
    Excel::_ApplicationPtr spApp;

    // ���� Application ����
    if (FAILED(spApp.CreateInstance(_T("Excel.Application"))))
    {
        ::MessageBox(::GetActiveWindow(), _T("�޷����� Excel����ȷ�����ļ�������Ƿ�װ�� Excel��"), _T("��ʾ"),
                     MB_OK | MB_ICONINFORMATION);
        return;
    }

    try
    {
        // ��������������ӹ�����
        Excel::_WorkbookPtr spBook = spApp->GetWorkbooks()->Add();

        // ��õ�һ��������
        Excel::_WorksheetPtr spSheet = spBook->GetSheets()->GetItem(_variant_t(1L));

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
            spReRange->PutRowHeight(lTitleSize * 2);        // �����и�
            spReRange->Select();                            // ѡ�е�ǰ������
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
    }
    catch (_com_error& e)
    {
    #ifdef _DEBUG
        dump_com_error(_T("Excel"), e);
    #else
        UNUSED_PARAMETER(e);
        ::MessageBox(::GetActiveWindow(), _T("�������ݵ� Excel ʧ�ܣ�"), _T("��ʾ"), MB_OK | MB_ICONWARNING);
    #endif

        // �����쳣���˳� Excel
        spApp->PutDisplayAlerts(VARIANT_FALSE);     // �ر��ض��ľ������Ϣ
        spApp->Quit();
    }
}

void ExportToExcel(HWND hWnd, LPCTSTR lpszTitle, long lBeginColumn, long lEndColumn, long lTitleSize, long lHeaderSize,
                   long lCellSize, Excel::XlHAlign TitleHAlign, Excel::XlHAlign CellHAlign, COLORREF clrHeader)
{
    _ASSERTE(::IsWindow(hWnd));
    _ASSERTE(lBeginColumn >= 0);
    _ASSERTE(::GetWindowLong(hWnd, GWL_STYLE) & LVS_REPORT);

    // ��ñ�ͷ�ؼ��ľ��
    HWND hWndHD = ListView_GetHeader(hWnd);
    _ASSERTE(::IsWindow(hWndHD));

    // �������������
    long lRows  = ListView_GetItemCount(hWnd) + 1;      // ���������������ͷ��
    long lCount = Header_GetItemCount(hWndHD);
    long lCols  = ((lEndColumn == -1 || lEndColumn > lCount) ? lCount : lEndColumn) - lBeginColumn;
    _ASSERTE(lCols > 0);

    // ������ȫ����
    CComSafeArray<VARIANT> saVariant;
    CComSafeArrayBound bound[2];
    bound[0] = lRows;
    bound[1] = lCols;

    if (SUCCEEDED(saVariant.Create(bound, 2)))
    {
        // ��ñ�ͷ����
        TCHAR szBuffer[_MAX_PATH + 1] = _T("");
        HDITEM hdItem  = { HDI_TEXT };
        hdItem.pszText = szBuffer;
        hdItem.cchTextMax = _MAX_PATH + 1;
        CComVariant vtValue;
        long lIndex[2];

        // ����ͷ���ݣ����洢����ȫ������
        for (lIndex[0] = 0, lIndex[1] = 0; lIndex[1] < lCols; ++lIndex[1])
        {
            Header_GetItem(hWndHD, lIndex[1] + lBeginColumn, &hdItem);
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
                lvItem.iSubItem = lIndex[1] + lBeginColumn;
                lvItem.pszText  = szBuffer;
                ::SendMessage(hWnd, LVM_GETITEMTEXT, lIndex[0] - 1, reinterpret_cast<LPARAM>(&lvItem));

                // ��������ӵ���ȫ����
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

    // ��ñ�ͷ�ؼ��ľ��
    HWND hWndHD = ListView_GetHeader(hWnd);
    _ASSERTE(::IsWindow(hWndHD));

    // �������������
    long lRows  = ListView_GetItemCount(hWnd) + 1;  // ���������������ͷ��
    long lCount = Header_GetItemCount(hWndHD);
    long lCols  = ((lEndColumn == -1 || lEndColumn > lCount) ? lCount : lEndColumn) - lBeginColumn;
    _ASSERTE(lCols > 0);

    // �������� Word Ӧ�ó���
    Word::_ApplicationPtr spApp;

    // ���� Application ����
    if (FAILED(spApp.CreateInstance(_T("Word.Application"))))
    {
        ::MessageBox(::GetActiveWindow(), _T("�޷����� Word����ȷ�����ļ�������Ƿ�װ�� Word��"), _T("��ʾ"),
                     MB_OK | MB_ICONINFORMATION);
        return;
    }

    try
    {
        Word::_DocumentPtr spDoc = spApp->GetDocuments()->Add();                        // ����ĵ�
        Word::SelectionPtr spSelection = spDoc->GetActiveWindow()->GetSelection();      // ��õ�ǰ���
        spSelection->GetPageSetup()->PutOrientation(Orientation);                       // ����ҳ�淽��
        Word::RangePtr spRange = spSelection->GetRange();

        // �ж��Ƿ��б���
        if (lpszTitle != NULL)
        {
            spSelection->PutText(_bstr_t(lpszTitle));                                   // �����ı�
            spSelection->GetFont()->PutSize(fltTitleSize);                              // �����ֺ�
            spSelection->GetFont()->PutBold(VARIANT_TRUE);                              // ���ô���
            spSelection->GetParagraphs()->PutAlignment(Word::wdAlignParagraphCenter);   // ������ж���
            spSelection->EndKey();                                                      // ����ƶ�������ĩβ
            spRange = spSelection->GetRange();
        }

        // ���ù�������ı�����
        spSelection->GetFont()->PutSize(fltTextSize);           // �ֺ�
        spSelection->GetFont()->PutBold(VARIANT_FALSE);         // ����

        // ���Ʊ��
        _variant_t vtTableBehavior(Word::wdWord9TableBehavior), vtAutoFit(Word::wdAutoFitContent);
        Word::TablePtr spTable = spDoc->GetTables()->Add(spRange, lRows, lCols, &vtTableBehavior, &vtAutoFit);

        // ������о��ж���
        spTable->GetRows()->PutAlignment(Word::wdAlignRowCenter);

        // ��ñ�ͷ����
        TCHAR szBuffer[_MAX_PATH + 1] = _T("");
        HDITEM hdItem  = { HDI_TEXT };
        hdItem.pszText = szBuffer;
        hdItem.cchTextMax = _MAX_PATH + 1;
        _bstr_t bstrText;

        // ����ͷ������ӵ����ĵ�һ��
        for (long i = 1; i <= lCols; ++i)
        {
            Header_GetItem(hWndHD, i - 1 + lBeginColumn, &hdItem);
            bstrText = hdItem.pszText;
            spTable->Cell(1L, i)->GetRange()->PutText(bstrText);
        }

        // �������
        LVITEM lvItem;
        lvItem.cchTextMax = _MAX_PATH + 1;
        for (long i = 2; i <= lRows; ++i)
        {
            for (long j = 1; j <= lCols; ++j)
            {
                // ����б�ؼ���ָ��λ�õ��ı�
                lvItem.iSubItem = j - 1 + lBeginColumn;
                lvItem.pszText  = szBuffer;
                ::SendMessage(hWnd, LVM_GETITEMTEXT, i - 2, reinterpret_cast<LPARAM>(&lvItem));
                
                // ��������ӵ������
                bstrText = lvItem.pszText;
                spTable->Cell(i, j)->GetRange()->PutText(bstrText);
            }
        }

        spRange->SetRange(spTable->Cell(1L, 1L)->GetRange()->GetStart(), spTable->Cell(1L, lCols)->GetRange()->GetEnd());
        spRange->GetFont()->PutBold(VARIANT_TRUE);

        // ��ʾ Word
        spApp->PutVisible(VARIANT_TRUE);
    }
    catch (_com_error& e)
    {
    #ifdef _DEBUG
        dump_com_error(_T("Word"), e);
    #else
        UNUSED_PARAMETER(e);
        ::MessageBox(::GetActiveWindow(), _T("�������ݵ� Word ʧ�ܣ�"), _T("��ʾ"), MB_OK | MB_ICONWARNING);
    #endif

        // �����쳣���˳� Word
        _variant_t vtSaveChanges(Word::wdDoNotSaveChanges);
        spApp->Quit(&vtSaveChanges);
    }
}

} // namespace utility