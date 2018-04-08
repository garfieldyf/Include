///////////////////////////////////////////////////////////////////////////////
// Windows Mobile 注册
//

// 获取未读短信
// HKEY_CURRENT_USER\\System\\State\\Messages\\sms\\Unread\\Count

// 获取未读彩信
// HKEY_CURRENT_USER\\System\\State\\Messages\\mms\\Unread\\Count

// 获取未读 E-mail
// HKEY_CURRENT_USER\\System\\State\\Messages\\mms\\Unread\\Count

// 获取未接电话
// HKEY_CURRENT_USER\\System\\State\\Phone\\Missed Call Count

// 获取闹钟设置信息
// HKEY_LOCAL_MACHINE\\Software\\Microsoft\\Clock\\0\\AlarmFlags
// HKEY_LOCAL_MACHINE\\Software\\Microsoft\\Clock\\1\\AlarmFlags
// HKEY_LOCAL_MACHINE\\Software\\Microsoft\\Clock\\2\\AlarmFlags
//
// See snapi.h file

///////////////////////////////////////////////////////////////////////////////
// WM_GESTURE
//
//  GID_SELECT
//  GID_BEGIN  GID_SELECT  GID_END

//  GID_DOUBLESELECT
//  GID_BEGIN  GID_SELECT  GID_END
//  GID_BEGIN  GID_DOUBLESELECT  GID_END

//  GID_HOLD
//  GID_BEGIN  GID_HOLD  GID_END

//  GID_PAN
//  GID_BEGIN  GID_PAN  GID_PAN  GID_PAN ...  GID_SCROLL GID_END

///////////////////////////////////////////////////////////////////////////////
// 单实例运行
//

// CMyApp 的Implementation文件中
BOOL CMyApp::InitInstance()
{
    if (HWND hWnd = ::FindWindow(_T("className"), _T("windowName")))
    {
        if (::IsIconic(hWnd))
            ::ShowWindowAsync(hWnd, SW_RESTORE);

        ::SetForegroundWindow(hWnd);
        return FALSE;
    }

    // ... ...
}

///////////////////////////////////////////////////////////////////////////////
// 输出目录
//

//  DEBUG   - $(OutDir)\$(ProjectName)D.exe
//  RELEASE - $(OutDir)\$(ProjectName).exe

//  Win32
//  $(SolutionDir)Bin
//  $(SolutionDir)Bin\$(ProjectName)\$(ConfigurationName)

//  WinCE
//  $(SolutionDir)Bin\$(PlatformName)
//  $(SolutionDir)Bin\$(ProjectName)\$(PlatformName)\$(ConfigurationName)


///////////////////////////////////////////////////////////////////////////////
// 输出类的对象布局 (VS 2008)
//
// /d1reportSingleClassLayout or /d1reportSingleClassLayoutDerived


///////////////////////////////////////////////////////////////////////////////
// 创建共享节
//

#pragma data_seg(".shared")
__declspec(selectany) HWND g_hWnd = NULL;
#pragma data_seg()

#pragma comment(linker, "/section:.shared, rws")


///////////////////////////////////////////////////////////////////////////////
// 函数转发器
//

#pragma comment(linker, "/export:_FuncName=_DllName._OtherFuncName")

///////////////////////////////////////////////////////////////////////////////
// 判断文件或文件夹是否存在
//

BOOL PathFileExists(LPCTSTR pszPath);


///////////////////////////////////////////////////////////////////////////////
// AxtiveX 事件
//

// Implementation IConnectionPointContainer 接口、IProvideClassInfo2 接口和 IProvideClassInfo 接口


///////////////////////////////////////////////////////////////////////////////
// 格式化函数
//

LPTSTR StrFormatByteSize64(LONGLONG nValue, LPTSTR pszBuffer, UINT cchBuffer);
LPTSTR StrFormatKBSize(LONGLONG nValue, LPTSTR pszBuffer, UINT cchBuffer);
int StrFromTimeInterval(LPTSTR pszBuffer, UINT cchMax, DWORD dwTimeMilliseconds, int nDigits);
int GetNumberFormat(LCID locale, DWORD dwFlags, LPCTSTR pszValue, const NUMBERFMT* pFormat, LPTSTR pszNumber, int cchNumber);
int GetDateFormat(LCID locale, DWORD dwFlags, const SYSTEMTIME* pDate, LPCTSTR pszFormat, LPTSTR pszDate, int cchDate);
int GetTimeFormat(LCID locale, DWORD dwFlags, const SYSTEMTIME* pTime, LPCTSTR pszFormat, LPTSTR pszTime, int cchTime);
int GetCurrencyFormat(LCID locale, DWORD dwFlags, LPCTSTR pszValue, const CURRENCYFMT* pFormat, LPTSTR pszCurrency, int cchCurrency);


///////////////////////////////////////////////////////////////////////////////
// 设置顶层窗口
//

//  1、创建窗口是指定 WS_EX_TOPMOST 风格
//  或
//  2、SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);       // WIN32
//     SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);              // MFC


///////////////////////////////////////////////////////////////////////////////
// ListView 选择发生改变
//

inline LRESULT OnItemChanged(int /*nIDCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/)
{
    LPNMLISTVIEW pNMListView = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
    ATLASSERT(pNMListView);
    ATLASSERT(::IsWindow(pNMListView->hdr.hwndFrom));

    if (pNMListView->uChanged & LVIF_STATE)
    {
        if (pNMListView->uOldState & LVIS_SELECTED)
        {
            ATLTRACE(_T("old select item = %d\n"), pNMListView->iItem);
        }
        else if (pNMListView->uNewState & (LVIS_FOCUSED | LVIS_SELECTED))
        {
            ATLTRACE(_T("new select item = %d\n"), pNMListView->iItem);
        }
    }

    return 0L;
}


///////////////////////////////////////////////////////////////////////////////
// TTN_GETDISPINFO 消息的处理
//

LRESULT OnToolTipText(int nIDCtrl, LPNMHDR pNMHDR, BOOL& /*bHandled*/)
{
    LPNMTTDISPINFO pDispInfo = reinterpret_cast<LPNMTTDISPINFO>(pNMHDR);
    ATLASSERT(pDispInfo);
    ATLASSERT(::IsWindow(pDispInfo->hdr.hwndFrom));

    if (nIDCtrl != 0 && (pDispInfo->uFlags & TTF_IDISHWND) == 0)
    {
        ::lstrcpyn(pDispInfo->szText, _T("add tip text"), _countof(pDispInfo->szText));
        pDispInfo->uFlags |= TTF_DI_SETITEM;    // 如果设置了该标志，则 TTN_GETDISPINFO 消息只发生一次
    }

    return 0L;
}


///////////////////////////////////////////////////////////////////////////////
// 设置树控件和列表控件的 Overlay Image
//

// 1、设置 ImageList 的 Overlay Image Index
//    ::ImageList_SetOverlayImage(hImageList, /* ImageList 中图像的 Index */, /* Overlay Mask Index, 以 1 开始, Index 自定义 */);
//    可以设置多个 Overlay Image Index
//
// 2、调用树或列表控件的 SetItemState
//    m_wndTree.SetItemState(hItem, INDEXTOOVERLAYMASK(/* Overlay Mask Index、即 ImageList_SetOverlayImage 自定义的 Index */), LVIS_OVERLAYMASK);
//
// 3、绘制 Overlay Image
//  ::ImageList_Draw(hImageList, nIndex, x, y, INDEXTOOVERLAYMASK(/*Overlay Mask Index、即 ImageList_SetOverlayImage 自定义的 Index*/) | ILD_NORMAL);


///////////////////////////////////////////////////////////////////////////////
// MsgWaitForMultipleObjects 函数
//

int Run(LPTSTR pszCmdLine, int nCmdShow)
{
    DWORD dwWaitObject = 0;
    for (; ;)
    {
        dwWaitObject = ::MsgWaitForMultipleObjects(/* objects count */, /* objects handle */, /* FALSE */, /* INFINITE */, /* QS_ALLINPUT */);
        if (dwWaitObject == WAIT_OBJECT_0 + /* objects count */)
        {
            // Receives new message.
            MSG msg;
            while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
            {
                // Exit Run, if received WM_QUIT message.
                if (msg.message == WM_QUIT)
                    return (int)msg.wParam;

                // Processing message.
                if (!PreTranslateMessage(&msg))
                {
                    ::TranslateMessage(&msg);
                    ::DispatchMessage(&msg);
                }
            }

            // Processing idle.
        }
        else if (dwWaitObject >= WAIT_OBJECT_0 && dwWaitObject < WAIT_OBJECT_0 + /* objects count */)
        {
            // Watiing successful, the specified object is signaled, index = dwWaitObject - WAIT_OBJECT_0.
        }
        else if (dwWaitObject >= WAIT_ABANDONED_0 && dwWaitObject < WAIT_ABANDONED_0 + /* objects count */)
        {
            // Waiting abandoned, the specified mutex is abandoned, index = dwWaitObject - WAIT_ABANDONED_0.
        }
        else if (dwWaitObject == WAIT_TIMEOUT)
        {
            // Waiting timeout.
        }
        else if (dwWaitObject == WAIT_FAILED)
        {
            // Waiting failed.
        }
    }
}


///////////////////////////////////////////////////////////////////////////////
// Windows Internet Function
//

//  1. InternetAttemptConnect(0);
//  2. InternetOpen();
//  3. InternetConnect();
//  4. HttpOpenRequest() / HttpOpenRequestEx() / HttpAddRequestHeaders();
//  5. HttpSendRequest() / HttpSendRequestEx();
//  6. HttpQueryInfo();
//  7. InternetReadFile() / InternetWriteFile();
//  8. InternetCloseHandle();

//  OR

//  1. InternetAttemptConnect(0);
//  2. InternetOpen();
//  3. InternetOpenUrl();
//  4. HttpQueryInfo();
//  5. InternetReadFile() / InternetWriteFile();
//  6. InternetCloseHandle();

void Download(LPCTSTR szUrl);
{
    HINTERNET hConnect  = NULL;
    HINTERNET hInternet = NULL;

    __try
    {
        if ((hInternet = ::InternetOpen(NULL, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0)) == NULL)
            __leave;

        if ((hConnect = ::InternetOpenUrl(hInternet, szUrl, NULL, 0, INTERNET_FLAG_DONT_CACHE | INTERNET_FLAG_KEEP_CONNECTION | INTERNET_FLAG_RELOAD | INTERNET_FLAG_PRAGMA_NOCACHE, 0)) == NULL)
            __leave;

        //// 查询 HTTP 状态
        //DWORD dwValue, dwNumberOfBytesRead = sizeof(DWORD);
        //if (!::HttpQueryInfo(hConnect, HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER, &dwValue, &dwNumberOfBytesRead, NULL) || dwValue != HTTP_STATUS_OK)
        //  __leave;
        //ATLASSERT(dwNumberOfBytesRead == sizeof(DWORD));

        //// 查询数据大小
        //if (!::HttpQueryInfo(hConnect, HTTP_QUERY_CONTENT_LENGTH | HTTP_QUERY_FLAG_NUMBER, &dwValue, &dwNumberOfBytesRead, NULL))
        //  __leave;
        //ATLASSERT(dwNumberOfBytesRead == sizeof(DWORD));

        const int nSize = 8129;
        BYTE nBuffer[nSize];

        do
        {
            if (!::InternetReadFile(hConnect, &nBuffer, nSize, &dwNumberOfBytesRead))
                __leave;

            if (dwNumberOfBytesRead != 0)
            {
                // do something ... ...
            }
        }
        while (dwNumberOfBytesRead != 0);
    }
    __finally
    {
        ::InternetCloseHandle(hConnect);
        ::InternetCloseHandle(hInternet);
    }
}

///////////////////////////////////////////////////////////////////////////////
// _NewEnum Implementation
//

typedef CComObject<atlutil::CComContainerCopy<std::vector<LONG> > > CComContainerCopyObject;
typedef CComEnumOnSTL<IEnumVARIANT, &__uuidof(IEnumVARIANT), VARIANT, _CopyVariant, std::vector<LONG> > CComEnumVariant
typedef CComObject<CComEnumVariant> CComEnumVariantObject;

STDMETHOD(get__NewEnum)(IUnknown** ppUnkEnum)
{
    if (ppUnkEnum == NULL)
        return E_INVALIDARG;

    *ppUnkEnum = NULL;
    CComEnumVariantObject* pObject = NULL;
    HRESULT hr = CComEnumVariantObject::CreateInstance(&pObject);

    if (SUCCEEDED(hr))
    {
        pObject->AddRef();

        CComContainerCopyObject* pCopy = NULL;
        hr = atlutil::CComContainerCopy<std::vector<LONG> >::CreateInstance(&pCopy, m_vecLong);
        if (SUCCEEDED(hr))
        {
            pCopy->AddRef();
            hr = pObject->Init(pCopy->GetUnknown(), pCopy->m_coll);
            if (SUCCEEDED(hr))
                hr = pObject->QueryInterface(ppUnkEnum);
            pCopy->Release();
        }

        pObject->Release();
    }

    return hr;
}


///////////////////////////////////////////////////////////////////////////////
// 解析 HTML 文档
//

inline HRESULT _AtlParseHTML(IMarkupServices* pServices, IHTMLDocument2** ppDoc, LPOLESTR pszHTML, HGLOBAL hGlobalHTML = NULL, DWORD dwFlags = 0)
{
    ATLASSERT(ppDoc);
    ATLASSERT(pServices);
    ATLASSERT(pszHTML != NULL || hGlobalHTML != NULL);
    ATLASSERT(!(pszHTML != NULL && hGlobalHTML != NULL));

    HRESULT hr = E_NOINTERFACE;
    if (CComQIPtr<IPersistStreamInit> spPersist = pServices)
    {
        hr = spPersist->InitNew();
        if (SUCCEEDED(hr))
        {
            CComPtr<IMarkupPointer> spPointerStart, spPointerFinish;
            _COM_VERIFY(pServices->CreateMarkupPointer(&spPointerStart));
            _COM_VERIFY(pServices->CreateMarkupPointer(&spPointerFinish));

            CComPtr<IMarkupContainer> spContainer;
            hr = (pszHTML != NULL) ? pServices->ParseString(pszHTML, dwFlags, &spContainer, spPointerStart, spPointerFinish)
                                   : pServices->ParseGlobal(hGlobalHTML, dwFlags, &spContainer, spPointerStart, spPointerFinish);
            if (SUCCEEDED(hr))
                hr = spContainer.QueryInterface(ppDoc);
        }
    }

    return hr;
}


///////////////////////////////////////////////////////////////////////////////
// 使用 CBalloonTipCtrl
//

// 1、改写主窗口的 PreTranslateMessage，调用 CBalloonTipCtrl::RelayEventEx() 函数

BOOL CXXXDlg::PreTranslateMessage(MSG* pMsg)
{
    m_wndBalloonTip.RelayEventEx(pMsg);
    return CDialog::PreTranslateMessage(pMsg);
}

// 2、处理主窗口的 WM_ACTIVATE 消息

void CXXXDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
    if (bMinimized || nState == WA_INACTIVE)
        m_wndBalloonTip.HideBalloonTip();

    CDialog::OnActivate(nState, pWndOther, bMinimized);
}


///////////////////////////////////////////////////////////////////////////////
// read file to safearray function
//

inline BOOL ReadFile(LPCTSTR pszFileName, CComSafeArray<BYTE>& saChunk)
{
    ATLASSERT(pszFileName);
    _COM_VERIFY(saChunk.Destroy());

    BOOL bSuccessful = FALSE;
    HANDLE hFile = INVALID_HANDLE_VALUE;

    __try
    {
        // 打开文件
        hFile = ::CreateFile(pszFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hFile == INVALID_HANDLE_VALUE)
            __leave;

        // 获得文件的大小
        LARGE_INTEGER liSize = { 0 };
        if (!::GetFileSizeEx(hFile, &liSize) || liSize.QuadPart <= 0)
            __leave;

        // 创建安全数组
        if (FAILED(saChunk.Create((ULONG)liSize.QuadPart)))
            __leave;

        // 将文件读入安全数组中
        DWORD dwRead = 0;
        LPVOID lpData = NULL;
        if (SUCCEEDED(::SafeArrayAccessData(saChunk.m_psa, &lpData)))
        {
            bSuccessful = ::ReadFile(hFile, lpData, (ULONG)liSize.QuadPart, &dwRead, NULL);
            _COM_VERIFY(::SafeArrayUnaccessData(saChunk.m_psa));
        }
    }
    __finally
    {
        ::CloseHandle(hFile);
    }

    return bSuccessful;
}


///////////////////////////////////////////////////////////////////////////////
// write safearray data to file function
//

inline BOOL WriteFile(LPCTSTR pszFileName, LPCVOID lpBuffer, DWORD dwNumberOfBytes)
{
    ATLASSERT(lpBuffer);
    ATLASSERT(pszFileName);

    BOOL bSuccessful = FALSE;
    HANDLE hFile = ::CreateFile(pszFileName, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hFile != INVALID_HANDLE_VALUE)
    {
        DWORD dwWritten = 0;
        bSuccessful = ::WriteFile(hFile, lpBuffer, dwNumberOfBytes, &dwWritten, NULL);
        ATLVERIFY(::CloseHandle(hFile));
    }

    return bSuccessful;
}

inline BOOL WriteFile(LPCTSTR pszFileName, const CComSafeArray<BYTE>& saChunk)
{
    ATLASSERT(pszFileName);
    ATLASSERT(saChunk.m_psa != NULL);

    LPVOID lpData = NULL;
    BOOL bSuccessful = FALSE;

    if (SUCCEEDED(::SafeArrayAccessData(saChunk.m_psa, &lpData)))
    {
        bSuccessful = WriteFile(pszFileName, lpData, saChunk.GetCount(0));
        _COM_VERIFY(::SafeArrayUnaccessData(saChunk.m_psa));
    }

    return bSuccessful;
}


///////////////////////////////////////////////////////////////////////////////
// InjectLibrary / EjectLibrary
//

inline BOOL WINAPI InjectLibrary(LPCWSTR pszLibFileName, DWORD dwProcessId, DWORD& dwInjectId, DWORD dwTimeout = INFINITE)
{
    ATLASSERT(pszLibFileName);

    HANDLE hProcess  = NULL;
    HANDLE hThread   = NULL;
    BOOL bSuccessful = FALSE;
    LPWSTR pszParameter = NULL;

    __try
    {
        if ((hProcess = ::OpenProcess(PROCESS_CREATE_THREAD | PROCESS_VM_OPERATION | PROCESS_VM_WRITE, FALSE, dwProcessId)) == NULL)
            __leave;

        SIZE_T cbSize = (::lstrlenW(pszLibFileName) + 1) * sizeof(WCHAR);
        pszParameter = (LPWSTR)::VirtualAllocEx(hProcess, NULL, cbSize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
        if (pszParameter == NULL || !::WriteProcessMemory(hProcess, pszParameter, pszLibFileName, cbSize, NULL))
            __leave;

        LPTHREAD_START_ROUTINE pfnThreadProc = (LPTHREAD_START_ROUTINE)::GetProcAddress(::GetModuleHandleW(L"kernel32"), "LoadLibraryW");
        if (pfnThreadProc == NULL || (hThread = ::CreateRemoteThread(hProcess, NULL, 0, pfnThreadProc, pszParameter, 0, NULL)) == NULL)
            __leave;

        if (::WaitForSingleObject(hThread, dwTimeout) == WAIT_OBJECT_0)
            bSuccessful = ::GetExitCodeThread(hThread, &dwInjectId);
    }
    __finally
    {
        ::VirtualFreeEx(hProcess, pszParameter, 0, MEM_RELEASE);
        ::CloseHandle(hThread);
        ::CloseHandle(hProcess);
    }

    return bSuccessful;
}

inline BOOL WINAPI InjectLibrary(LPCSTR pszLibFileName, DWORD dwProcessId, DWORD& dwInjectId, DWORD dwTimeout = INFINITE)
{
    ATLASSERT(pszLibFileName);

    HANDLE hProcess  = NULL;
    HANDLE hThread   = NULL;
    BOOL bSuccessful = FALSE;
    LPSTR pszParameter = NULL;

    __try
    {
        if ((hProcess = ::OpenProcess(PROCESS_CREATE_THREAD | PROCESS_VM_OPERATION | PROCESS_VM_WRITE, FALSE, dwProcessId)) == NULL)
            __leave;

        SIZE_T cbSize = (::lstrlenA(pszLibFileName) + 1) * sizeof(CHAR);
        pszParameter = (LPSTR)::VirtualAllocEx(hProcess, NULL, cbSize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
        if (pszParameter == NULL || !::WriteProcessMemory(hProcess, pszParameter, pszLibFileName, cbSize, NULL))
            __leave;

        LPTHREAD_START_ROUTINE pfnThreadProc = (LPTHREAD_START_ROUTINE)::GetProcAddress(::GetModuleHandleW(L"kernel32"), "LoadLibraryA");
        if (pfnThreadProc == NULL || (hThread = ::CreateRemoteThread(hProcess, NULL, 0, pfnThreadProc, pszParameter, 0, NULL)) == NULL)
            __leave;

        if (::WaitForSingleObject(hThread, dwTimeout) == WAIT_OBJECT_0)
            bSuccessful = ::GetExitCodeThread(hThread, &dwInjectId);
    }
    __finally
    {
        ::VirtualFreeEx(hProcess, pszParameter, 0, MEM_RELEASE);
        ::CloseHandle(hThread);
        ::CloseHandle(hProcess);
    }

    return bSuccessful;
}

inline BOOL WINAPI EjectLibrary(DWORD dwProcessId, DWORD dwInjectId, DWORD dwTimeout = INFINITE)
{
    ATLASSERT(dwInjectId);

    HANDLE hProcess  = NULL;
    HANDLE hThread   = NULL;
    BOOL bSuccessful = FALSE;

    __try
    {
        if ((hProcess = ::OpenProcess(PROCESS_CREATE_THREAD, FALSE, dwProcessId)) == NULL)
            __leave;

        LPTHREAD_START_ROUTINE pfnThreadProc = (LPTHREAD_START_ROUTINE)::GetProcAddress(::GetModuleHandleW(L"kernel32"), "FreeLibrary");
        if (pfnThreadProc == NULL || (hThread = ::CreateRemoteThread(hProcess, NULL, 0, pfnThreadProc, (LPVOID)dwInjectId, 0, NULL)) == NULL)
            __leave;

        if (::WaitForSingleObject(hThread, dwTimeout) == WAIT_OBJECT_0)
            ATLVERIFY(::GetExitCodeThread(hThread, (LPDWORD)&bSuccessful));
    }
    __finally
    {
        ::CloseHandle(hThread);
        ::CloseHandle(hProcess);
    }

    return bSuccessful;
}

#ifdef _DBGHELP_
inline BOOL WINAPI ImageImportTableEntry(HINSTANCE hInstance, LPCSTR pszLibFileName, PROC pfnOriginal, PROC pfnNew)
{
    ATLASSERT(pfnNew);
    ATLASSERT(hInstance);
    ATLASSERT(pfnOriginal);
    ATLASSERT(pszLibFileName);

    ULONG ulSize = 0;
    PIMAGE_IMPORT_DESCRIPTOR pImageDesc = (PIMAGE_IMPORT_DESCRIPTOR)::ImageDirectoryEntryToData(hInstance, TRUE, IMAGE_DIRECTORY_ENTRY_IMPORT, &ulSize);
    ATLASSERT(pImageDesc);

    for (; pImageDesc->Name; ++pImageDesc)
    {
        if (::lstrcmpiA(pszLibFileName, (LPCSTR)((LPBYTE)hInstance + pImageDesc->Name)) == 0)
        {
            for (PIMAGE_THUNK_DATA pThunk = (PIMAGE_THUNK_DATA)((LPBYTE)hInstance + pImageDesc->FirstThunk); pThunk->u1.Function; ++pThunk)
            {
                if ((PROC)pThunk->u1.Function == pfnOriginal)
                    return ::WriteProcessMemory(::GetCurrentProcess(), &pThunk->u1.Function, &pfnNew, sizeof(PROC), NULL);
            }
        }
    }

    return FALSE;
}
#endif  // _DBGHELP_


///////////////////////////////////////////////////////////////////////////////
// 运行时创建 ActiveX 控件
//

LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    CWindow window = GetDlgItem(IDC_PLACEHOLDER);
    RECT rect;
    window.GetWindowRect(&rect);
    ScreenToClient(&rect);

    CAxWindow wndBrowser;
    wndBrowser.Create(m_hWnd, rect, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN);

    CComPtr<IUnknown> spUnk;
    wndBrowser.CreateControlEx(L"{8856F961-340A-11D0-A96B-00C04FD705A2}"/* ActiveX 控件的 CLSID */, NULL, NULL, &spUnk);

    spUnk->QueryInterface(&m_spBrowser);
    m_spBrowser->Navigate(CComBSTR(_T("http://www.google.cn")), NULL, NULL, NULL, NULL);

    return 1L;
}


///////////////////////////////////////////////////////////////////////////////
// ExportToExcel
//

#ifdef _DEBUG
inline void WINAPI dump_com_error(LPCTSTR pszSource, const _com_error& e)
{
    TCHAR szBuffer[1024];
    ::_tssprintf(szBuffer, _countof(szBuffer), _T("%s Error\nCode = %08lx\nCode meaning = %s\nSource = %s\nDescription = %s"), pszSource, e.Error(), e.ErrorMessage(), (LPCTSTR)e.Source(), (LPCTSTR)e.Description());
    ::MessageBox(::GetActiveWindow(), szBuffer, _T("错误"), MB_OK | MB_ICONERROR);
}
#endif  // _DEBUG

HRESULT WINAPI ExportToExcel(const ATL::CComSafeArray<VARIANT>& saValue, LPCTSTR pszTitle = NULL, LONG lTitleSize = 12L, LONG lHeaderSize = 9L, LONG lCellSize = 9L,
                             Excel::XlHAlign TitleHAlign = Excel::xlHAlignCenter, Excel::XlHAlign CellHAlign = Excel::xlHAlignCenter, COLORREF clrHeader = RGB(153, 206, 250))
{
    ATLASSERT(saValue.m_psa != NULL);
    ATLASSERT(saValue.GetDimensions() == 2);

    // 创建 Application 对象
    Excel::_ApplicationPtr spApp;
    HRESULT hr = spApp.CreateInstance(_T("Excel.Application"));
    if (FAILED(hr))
        return hr;

    try
    {
        Excel::_WorkbookPtr spBook   = spApp->GetWorkbooks()->Add();                    // 向工作簿集合中添加工作表
        Excel::_WorksheetPtr spSheet = spBook->GetSheets()->GetItem(_variant_t(1L));    // 获得第一个工作表

        // 获得数组的行数和列数
        LONG lRows = saValue.GetCount(0);
        LONG lCols = saValue.GetCount(1);

        Excel::RangePtr spRange, spMerger;
        if (pszTitle != NULL)
        {
            // 合并标题单元格
            spRange  = spSheet->GetRange(_variant_t(_T("A1")));
            spMerger = spRange->GetResize(vtMissing, _variant_t(lCols));
            spMerger->Merge();

            // 设置标题、字体、字号和对齐方式
            spMerger->PutValue(_variant_t(Excel::xlRangeValueDefault), _variant_t(pszTitle));
            spMerger->PutHorizontalAlignment(_variant_t(TitleHAlign));
            spMerger->PutVerticalAlignment(_variant_t(Excel::xlVAlignCenter));
            spMerger->Font->Size = lTitleSize;
            spMerger->Font->Bold = VARIANT_TRUE;
            spMerger->PutRowHeight(_variant_t(lTitleSize * 2L));
            spMerger->Select();
        }

        // 获得数据的填充范围
        spRange  = (pszTitle != NULL ? spSheet->GetRange(_variant_t(_T("A2"))) : spSheet->GetRange(_variant_t(_T("A1"))));
        spMerger = spRange->GetResize(_variant_t(lRows), _variant_t(lCols));

        // 设置数据、字体、字号、边框和对齐方式
        _variant_t vtValue;
        vtValue.vt = VT_ARRAY | VT_VARIANT;
        vtValue.parray = saValue.m_psa;

        spMerger->PutValue(_variant_t(Excel::xlRangeValueDefault), vtValue);
        spMerger->PutHorizontalAlignment(_variant_t(CellHAlign));
        spMerger->PutVerticalAlignment(_variant_t(Excel::xlVAlignCenter));
        spMerger->Font->Size = lCellSize;
        spMerger->GetEntireColumn()->AutoFit();
        spMerger->GetBorders()->PutWeight(_variant_t(Excel::xlThin));

        // 设置表头字体、字号和颜色
        spMerger = spRange->GetResize(vtMissing, _variant_t(lCols));
        spMerger->GetInterior()->PutColor(_variant_t(clrHeader));
        spMerger->Font->Size = lHeaderSize;
        spMerger->Font->Bold = VARIANT_TRUE;

        vtValue.vt = VT_EMPTY;                                          // 释放安全数组的管理权限，防止 _variant_t 的析构函数将其释放
        spApp->PutVisible(LOCALE_SYSTEM_DEFAULT, VARIANT_TRUE);         // 显示 Excel
    }
    catch (_com_error& e)
    {
    #ifdef _DEBUG
        dump_com_error(_T("Excel"), e);
    #endif
        hr = e.Error();
        spApp->PutDisplayAlerts(LOCALE_SYSTEM_DEFAULT, VARIANT_FALSE);  // 关闭特定的警告和消息
        spApp->GetWindows()->GetItem(_variant_t(1L))->Close();          // 关闭主窗口
        spApp->Quit();                                                  // 退出 Excel
    }

    return hr;
}

HRESULT WINAPI ExportToExcel(HWND hWnd, LPCTSTR pszTitle = NULL, LONG lBeginCol = 0L, LONG lEndCol = -1L, LONG lTitleSize = 12L, LONG lHeaderSize = 9L, LONG lCellSize = 9L,
                             Excel::XlHAlign TitleHAlign = Excel::xlHAlignCenter, Excel::XlHAlign CellHAlign = Excel::xlHAlignCenter, COLORREF clrHeader = RGB(153, 206, 250))
{
    ATLASSERT(lBeginCol >= 0);
    ATLASSERT(::IsWindow(hWnd));
    ATLASSERT(::IsWindow(ListView_GetHeader(hWnd)));

    // 获得行数和列数
    LONG lRows  = ListView_GetItemCount(hWnd) + 1;      // 获得行数（包含表头）
    LONG lCount = Header_GetItemCount(ListView_GetHeader(hWnd));
    LONG lCols  = (lEndCol == -1 || lEndCol > lCount ? lCount : lEndCol) - lBeginCol;
    ATLASSERT(lCols > 0 && lCols <= lCount);

    // 创建安全数组
    ATL::CComSafeArray<VARIANT> saValue;
    SAFEARRAYBOUND bound[] = { { lRows, 0 }, { lCols, 0 } };
    HRESULT hr = saValue.Create(bound, _countof(bound));
    if (SUCCEEDED(hr))
    {
        LONG lIndex[2] = { 0 };
        TCHAR szText[MAX_PATH];
        LVCOLUMN column = { LVCF_TEXT, 0, 0, szText, _countof(szText) };
        LVITEM lvItem = { LVIF_TEXT, 0, 0, 0, 0, szText, _countof(szText) };

        // 获得表头数据
        for (lIndex[1] = 0; lIndex[1] < lCols; ++lIndex[1], lIndex[0] = 0)
        {
            ATLVERIFY(ListView_GetColumn(hWnd, lIndex[1] + lBeginCol, &column));
            _COM_VERIFY(saValue.MultiDimSetAt(lIndex, _variant_t(szText)));

            // 获得数据
            for (lIndex[0] = 1; lIndex[0] < lRows; ++lIndex[0])
            {
                lvItem.iSubItem = lIndex[1] + lBeginCol;
                ::SendMessage(hWnd, LVM_GETITEMTEXT, lIndex[0] - 1, (LPARAM)&lvItem);
                _COM_VERIFY(saValue.MultiDimSetAt(lIndex, _variant_t(szText)));
            }
        }

        hr = ExportToExcel(saValue, pszTitle, lTitleSize, lHeaderSize, lCellSize, TitleHAlign, CellHAlign, clrHeader);
    }

    return hr;
}


///////////////////////////////////////////////////////////////////////////////
// 创建工具栏的子控件
//

typedef ATL::CWinTraitsOR<TBSTYLE_FLAT | TBSTYLE_TRANSPARENT | TBSTYLE_TOOLTIPS | CCS_NODIVIDER, WS_EX_TOOLWINDOW> CToolBarTraits;

class CToolBarEx : public ATL::CWindowImpl<CToolBarEx, WTL::CToolBarCtrl, CToolBarTraits>
{
public:
    CToolBarEx()
    {
    }

public:
    BEGIN_MSG_MAP(CToolBarEx)
        MESSAGE_HANDLER(WM_SIZE, OnSize)
        MESSAGE_HANDLER(WM_CREATE, OnCreate)
        MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
        FORWARD_NOTIFICATIONS()
    END_MSG_MAP()

public:
    LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        TBBUTTON buttons[] =
        {
            { 56, ID_STATIC_ADDRESS, TBSTATE_ENABLED, TBSTYLE_SEP },
            { 200, ID_TAGET_URL, TBSTATE_ENABLED, TBSTYLE_SEP },
            { 0, ID_WEB_PREVIEW, TBSTATE_ENABLED, TBSTYLE_DROPDOWN }
        };

        WTL::CBitmap bitmap;
        bitmap.LoadBitmap(IDB_PREVIEW);

        ATLVERIFY(m_image.Create(16, 16, ILC_MASK | ILC_COLOR24, 1, 0));
        m_image.Add(bitmap, RGB(255, 0, 255));

        SetExtendedStyle(GetExtendedStyle() | TBSTYLE_EX_DRAWDDARROWS);
        SetImageList(m_image);
        AddButtons(__countof(buttons), buttons);

        CRect rect;
        GetItemRect(CommandToIndex(ID_STATIC_ADDRESS), rect);
        rect.top += 3;
        rect.bottom -= 1;
        m_wndStatic.Create(m_hWnd, rect, NULL, WS_CHILD | WS_VISIBLE | WS_TABSTOP | SS_LEFT);
        m_wndStatic.SetFont(GetFont());
        m_wndStatic.SetWindowText(_T("设置地址"));

        rect;
        GetItemRect(CommandToIndex(ID_TAGET_URL), rect);
        rect.top += 1;
        rect.bottom -= 1;
        m_wndEdit.Create(m_hWnd, rect, NULL, WS_CHILD | WS_VISIBLE | WS_TABSTOP, WS_EX_CLIENTEDGE, IDC_EDIT_TARGET_URL);
        m_wndEdit.SetFont(GetFont());

        return 1;
    }

    void ShowWindowEx(int nCmdShow)
    {
        m_wndStatic.ShowWindow(nCmdShow);
        m_wndEdit.ShowWindow(nCmdShow);
        ShowWindow(nCmdShow);
    }

    LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        if (m_wndEdit.m_hWnd != NULL)
            ATLVERIFY(m_wndEdit.DestroyWindow());

        if (m_wndStatic.m_hWnd != NULL)
            ATLVERIFY(m_wndStatic.DestroyWindow());

        if (m_image.m_hImageList != NULL)
            ATLVERIFY(m_image.Destroy());

        bHandled = FALSE;
        return 0L;
    }

    LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        bHandled = FALSE;
        if (!IsWindow())
            return 0L;

        CRect rcStatic(0, 0, 0, 0);
        GetItemRect(CommandToIndex(ID_STATIC_ADDRESS), rcStatic);

        CRect rcWebPreview(0, 0, 0, 0);
        GetItemRect(CommandToIndex(ID_WEB_PREVIEW), rcWebPreview);

        TBBUTTONINFO tbbi = { sizeof(TBBUTTONINFO) };
        tbbi.dwMask = TBIF_SIZE;
        tbbi.cx = LOWORD(lParam) - rcWebPreview.Width() - rcStatic.Width();
        SetButtonInfo(ID_TAGET_URL, &tbbi);

        if (m_wndEdit.IsWindow())
        {
            const int nOffset = 5;
            CRect rcEdit(0, 0, 0, 0);
            m_wndEdit.GetWindowRect(rcEdit);
            ScreenToClient(rcEdit);
            rcEdit.right = rcEdit.left + (tbbi.cx - nOffset);

            m_wndEdit.SetWindowPos(NULL, rcEdit, SWP_NOZORDER | SWP_NOACTIVATE);
        }

        return 0L;
    }

public:
    WTL::CEdit m_wndEdit;
    WTL::CStatic m_wndStatic;
    WTL::CImageList m_image;
};


#if defined(__IMAGE_H__) && (_WIN32_WINNT >= 0x0501)
///////////////////////////////////////////////////////////////////////////////
// Interface of the CImageButtonBase class
//

class ATL_NO_VTABLE CImageButtonBase
{
// Constructors
protected:
    CImageButtonBase();

// Operations
public:
    void SetImages(int nNormal, int nHover = 0, int nSelected = 0, int nDisabled = 0);

// Data members
public:
    int m_nImages[4];
    atlutil::CImageList* m_pImageList;

// Message map functions
public:
    LRESULT OnCustomDraw(int nIDCtrl, LPNMHDR pNMHDR, BOOL& bHandled);
    LRESULT OnCtlColor(HWND hWnd, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnEraseBkgnd(HWND hWnd, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
};
#endif  // defined(__IMAGE_H__) && (_WIN32_WINNT >= 0x0501)


#if defined(__IMAGE_H__) && (_WIN32_WINNT >= 0x0501)
///////////////////////////////////////////////////////////////////////////////
// Implementation of the CImageButtonBase class
//

inline CImageButtonBase::CImageButtonBase()
    : m_pImageList(NULL)
{
    ::memset(m_nImages, 0, sizeof(m_nImages));
}

inline void CImageButtonBase::SetImages(int nNormal, int nHover/* = 0*/, int nSelected/* = 0*/, int nDisabled/* = 0*/)
{
    m_nImages[IMAGE_NORMAL]   = nNormal;
    m_nImages[IMAGE_HOVER]    = nHover;
    m_nImages[IMAGE_SELECTED] = nSelected;
    m_nImages[IMAGE_DISABLED] = nDisabled;
}

inline LRESULT CImageButtonBase::OnCustomDraw(int /*nIDCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/)
{
    LPNMCUSTOMDRAW pNMDraw = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
    ATLASSERT(pNMDraw);
    ATLASSERT(pNMDraw->hdc);
    ATLASSERT(m_pImageList);
    ATLASSERT(!m_pImageList->IsEmpty());

    int nImage = m_nImages[IMAGE_NORMAL];
    if (pNMDraw->uItemState & CDIS_SELECTED)
        nImage = m_nImages[IMAGE_SELECTED];
    else if (pNMDraw->uItemState & CDIS_HOT)
        nImage = m_nImages[IMAGE_HOVER];
    else if (pNMDraw->uItemState & CDIS_DISABLED)
        nImage = m_nImages[IMAGE_DISABLED];

    stdutil::DrawBkgnd(pNMDraw->hdr.hwndFrom, pNMDraw->hdc, pNMDraw->rc);
    ATLVERIFY(m_pImageList->Draw(pNMDraw->hdc, nImage, pNMDraw->rc));

    return CDRF_SKIPDEFAULT;
}

inline LRESULT CImageButtonBase::OnCtlColor(HWND /*hWnd*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    return 0L;
}

inline LRESULT CImageButtonBase::OnEraseBkgnd(HWND /*hWnd*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    return 1L;
}
#endif  // defined(__IMAGE_H__) && (_WIN32_WINNT >= 0x0501)


#if defined(__IMAGE_H__) && (_WIN32_WINNT >= 0x0501)
///////////////////////////////////////////////////////////////////////////////
// Interface of the CImageButtonImpl class
//

template <typename T, typename TBase = ATL::CWindow, typename TWinTraits = ATL::CControlWinTraits>
class ATL_NO_VTABLE CImageButtonImpl : public ATL::CWindowImpl<T, TBase, TWinTraits>, public CImageButtonBase
{
// Constructors
protected:
    CImageButtonImpl();

// Implementation
public:
    BEGIN_MSG_MAP(CImageButtonImpl)
        MESSAGE_HANDLER_EX(OCM_CTLCOLORBTN, OnCtlColor)
        MESSAGE_HANDLER_EX(WM_ERASEBKGND, OnEraseBkgnd)
        REFLECTED_NOTIFY_CODE_HANDLER(NM_CUSTOMDRAW, OnCustomDraw)
    END_MSG_MAP()

// Operations
public:
    BOOL SizeToContent();
};
#endif  // defined(__IMAGE_H__) && (_WIN32_WINNT >= 0x0501)


#if defined(__IMAGE_H__) && (_WIN32_WINNT >= 0x0501)
///////////////////////////////////////////////////////////////////////////////
// Implementation of the CImageButtonImpl class
//

template <typename T, typename TBase, typename TWinTraits>
inline CImageButtonImpl<T, TBase, TWinTraits>::CImageButtonImpl()
{
}

template <typename T, typename TBase, typename TWinTraits>
inline BOOL CImageButtonImpl<T, TBase, TWinTraits>::SizeToContent()
{
    ATLASSERT(IsWindow());
    ATLASSERT(m_pImageList);
    ATLASSERT(!m_pImageList->IsEmpty());

    return SetWindowPos(NULL, 0, 0, m_pImageList->GetImageWidth(), m_pImageList->GetImageHeight(), SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
}
#endif  // defined(__IMAGE_H__) && (_WIN32_WINNT >= 0x0501)


#if defined(__IMAGE_H__) && (_WIN32_WINNT >= 0x0501)
///////////////////////////////////////////////////////////////////////////////
// Interface of the CImageButton class
//

class CImageButton : public CImageButtonImpl<CImageButton, WTL::CButton>
{
public:
    DECLARE_WND_SUPERCLASS(_T("ATL_ImageButton"), GetWndClassName());
};
#endif  // defined(__IMAGE_H__) && (_WIN32_WINNT >= 0x0501)



//  _ATL_SUPPORT_CIB_MOUSENOTIFY        Enable CImageButtonImpl BN_MOUSELEAVE and BN_MOUSEHOVER notify message
typedef ATL::CWinTraitsOR<BS_OWNERDRAW> CImageButtonTraits;

#ifdef __IMAGE_H__
///////////////////////////////////////////////////////////////////////////////
// Interface of the CImageButtonBase class
//

class ATL_NO_VTABLE CImageButtonBase
{
// Constructors
protected:
    CImageButtonBase();

// Operations
public:
    void SetImages(int nNormal, int nHover = 0, int nSelected = 0, int nDisabled = 0);

// Data members
public:
    int m_nImages[4];
    BOOL m_bMouseHover;
    atlutil::CImageList* m_pImageList;

// Message map functions
public:
    LRESULT OnDrawItem(HWND hWnd, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnMouseMove(HWND hWnd, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnMouseLeave(HWND hWnd, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnEraseBkgnd(HWND hWnd, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
};
#endif  // __IMAGE_H__


#ifdef __IMAGE_H__
///////////////////////////////////////////////////////////////////////////////
// Implementation of the CImageButtonBase class
//

inline CImageButtonBase::CImageButtonBase()
    : m_bMouseHover(FALSE), m_pImageList(NULL)
{
    ::memset(m_nImages, 0, sizeof(m_nImages));
}

inline void CImageButtonBase::SetImages(int nNormal, int nHover/* = 0*/, int nSelected/* = 0*/, int nDisabled/* = 0*/)
{
    m_nImages[IMAGE_NORMAL]   = nNormal;
    m_nImages[IMAGE_HOVER]    = nHover;
    m_nImages[IMAGE_SELECTED] = nSelected;
    m_nImages[IMAGE_DISABLED] = nDisabled;
}

inline LRESULT CImageButtonBase::OnDrawItem(HWND /*hWnd*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
    LPDRAWITEMSTRUCT pDrawItem = reinterpret_cast<LPDRAWITEMSTRUCT>(lParam);
    ATLASSERT(pDrawItem);
    ATLASSERT(m_pImageList);
    ATLASSERT(pDrawItem->hDC);
    ATLASSERT(!m_pImageList->IsEmpty());
    ATLASSERT(pDrawItem->CtlType == ODT_BUTTON);

    int nImage = m_nImages[IMAGE_NORMAL];
    if (pDrawItem->itemState & ODS_SELECTED)
        nImage = m_nImages[IMAGE_SELECTED];
    else if (pDrawItem->itemState & ODS_DISABLED)
        nImage = m_nImages[IMAGE_DISABLED];
    else if (m_bMouseHover)
        nImage = m_nImages[IMAGE_HOVER];

    if (!stdutil::DrawBkgnd(pDrawItem->hwndItem, pDrawItem->hDC, pDrawItem->rcItem))
        ATLVERIFY(::FillRect(pDrawItem->hDC, &pDrawItem->rcItem, (HBRUSH)::SendMessage(::GetParent(pDrawItem->hwndItem), WM_CTLCOLORBTN, (WPARAM)pDrawItem->hDC, (LPARAM)pDrawItem->hwndItem)));

    return m_pImageList->Draw(pDrawItem->hDC, nImage, pDrawItem->rcItem);
}

inline LRESULT CImageButtonBase::OnMouseMove(HWND hWnd, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
    ATLASSERT(::IsWindow(hWnd));

    if (!m_bMouseHover)
    {
        TRACKMOUSEEVENT tme = { sizeof(TRACKMOUSEEVENT), TME_LEAVE, hWnd, 0 };
        m_bMouseHover = ::TrackMouseEvent(&tme);

        ATLVERIFY(::InvalidateRect(hWnd, NULL, FALSE));
        ATLVERIFY(::UpdateWindow(hWnd));

    #ifdef _ATL_SUPPORT_CIB_MOUSENOTIFY
        ATLVERIFY(::PostMessage(::GetParent(hWnd), WM_COMMAND, MAKEWPARAM(::GetDlgCtrlID(hWnd), BN_MOUSEHOVER), (LPARAM)hWnd));
    #endif  // _ATL_SUPPORT_CIB_MOUSENOTIFY
    }

    return bHandled = FALSE;
}

inline LRESULT CImageButtonBase::OnMouseLeave(HWND hWnd, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    ATLASSERT(::IsWindow(hWnd));

    m_bMouseHover = FALSE;
    ATLVERIFY(::InvalidateRect(hWnd, NULL, FALSE));
    ATLVERIFY(::UpdateWindow(hWnd));

#ifdef _ATL_SUPPORT_CIB_MOUSENOTIFY
    ATLVERIFY(::PostMessage(::GetParent(hWnd), WM_COMMAND, MAKEWPARAM(::GetDlgCtrlID(hWnd), BN_MOUSELEAVE), (LPARAM)hWnd));
#endif  // _ATL_SUPPORT_CIB_MOUSENOTIFY

    return 0L;
}

inline LRESULT CImageButtonBase::OnEraseBkgnd(HWND /*hWnd*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    return 1L;
}
#endif  // __IMAGE_H__


#ifdef __IMAGE_H__
///////////////////////////////////////////////////////////////////////////////
// Interface of the CImageButtonImpl class
//

template <typename T, typename TBase = ATL::CWindow, typename TWinTraits = CImageButtonTraits>
class ATL_NO_VTABLE CImageButtonImpl : public ATL::CWindowImpl<T, TBase, TWinTraits>, public CImageButtonBase
{
// Constructors
protected:
    CImageButtonImpl();

// Implementation
public:
    BEGIN_MSG_MAP(CImageButtonImpl)
        MESSAGE_HANDLER_EX(OCM_DRAWITEM, OnDrawItem)
        MESSAGE_HANDLER_EX(WM_MOUSEMOVE, OnMouseMove)
        MESSAGE_HANDLER_EX(WM_MOUSELEAVE, OnMouseLeave)
        MESSAGE_HANDLER_EX(WM_ERASEBKGND, OnEraseBkgnd)
    END_MSG_MAP()

// Overrides
public:
    BOOL SubclassWindow(HWND hWnd);

// Operations
public:
    BOOL SizeToContent();
};
#endif  // __IMAGE_H__


#ifdef __IMAGE_H__
///////////////////////////////////////////////////////////////////////////////
// Implementation of the CImageButtonImpl class
//

template <typename T, typename TBase, typename TWinTraits>
inline CImageButtonImpl<T, TBase, TWinTraits>::CImageButtonImpl()
{
}

template <typename T, typename TBase, typename TWinTraits>
inline BOOL CImageButtonImpl<T, TBase, TWinTraits>::SubclassWindow(HWND hWnd)
{
    ATLASSERT(::IsWindow(hWnd));

    BOOL bSuccessful = ATL::CWindowImpl<T, TBase, TWinTraits>::SubclassWindow(hWnd);
    if (bSuccessful)
        SetWindowLong(GWL_STYLE, GetWindowLong(GWL_STYLE) | TWinTraits::GetWndStyle(0));

    return bSuccessful;
}

template <typename T, typename TBase, typename TWinTraits>
inline BOOL CImageButtonImpl<T, TBase, TWinTraits>::SizeToContent()
{
    ATLASSERT(IsWindow());
    ATLASSERT(m_pImageList);
    ATLASSERT(!m_pImageList->IsEmpty());

    return SetWindowPos(NULL, 0, 0, m_pImageList->GetImageWidth(), m_pImageList->GetImageHeight(), SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
}
#endif  // __IMAGE_H__


#ifdef __IMAGE_H__
///////////////////////////////////////////////////////////////////////////////
// Interface of the CImageButton class
//

class CImageButton : public CImageButtonImpl<CImageButton, WTL::CButton>
{
public:
    DECLARE_WND_SUPERCLASS(_T("ATL_ImageButton"), GetWndClassName());
};
#endif  // __IMAGE_H__


///////////////////////////////////////////////////////////////////////////////
// UpdateLayeredWindow
//

#ifdef _GDIPLUS_H
inline BOOL WINAPI UpdateLayeredWindow(HWND hWnd, Gdiplus::Image* pImage, BYTE nSourceAlpha = 255)
{
    ATLASSERT(pImage);
    ATLASSERT(::IsWindow(hWnd));
    ATLASSERT(::GetWindowLong(hWnd, GWL_EXSTYLE) & WS_EX_LAYERED);

    BOOL bSuccessful = FALSE;
    HDC hDC = ::GetDC(NULL);
    ATLASSERT(hDC);

    Gdiplus::Rect rect(0, 0, pImage->GetWidth(), pImage->GetHeight());
    if (HBITMAP hBitmap = ::CreateCompatibleBitmap(hDC, rect.Width, rect.Height))
    {{
        atlutil::CBitmapDC dc(hBitmap);
        Gdiplus::Graphics graphics(dc);

        if (graphics.DrawImage(pImage, rect, 0, 0, rect.Width, rect.Height, Gdiplus::UnitPixel) == Gdiplus::Ok)
        {
            BLENDFUNCTION blend = { AC_SRC_OVER, 0, nSourceAlpha, AC_SRC_ALPHA };
            bSuccessful = ::UpdateLayeredWindow(hWnd, hDC, NULL, ((LPSIZE)&rect) + 1, dc, (LPPOINT)&rect, 0, &blend, ULW_ALPHA);
        }}

        ATLVERIFY(::DeleteObject(hBitmap));
    }

    ATLVERIFY(::ReleaseDC(NULL, hDC));
    return bSuccessful;
}

inline BOOL WINAPI UpdateLayeredWindow(HWND hWnd, LPCWSTR pszFileName, BYTE nSourceAlpha = 255)
{
    ATLASSERT(pszFileName);
    ATLASSERT(::IsWindow(hWnd));
    ATLASSERT(::GetWindowLong(hWnd, GWL_EXSTYLE) & WS_EX_LAYERED);

    Gdiplus::Image image(pszFileName);
    return (image.GetLastStatus() == Gdiplus::Ok && UpdateLayeredWindow(hWnd, &image, nSourceAlpha));
}

inline BOOL WINAPI UpdateLayeredWindow(HWND hWnd, IStream* pStream, BYTE nSourceAlpha = 255)
{
    ATLASSERT(pStream);
    ATLASSERT(::IsWindow(hWnd));
    ATLASSERT(::GetWindowLong(hWnd, GWL_EXSTYLE) & WS_EX_LAYERED);

    Gdiplus::Image image(pStream);
    return (image.GetLastStatus() == Gdiplus::Ok && UpdateLayeredWindow(hWnd, &image, nSourceAlpha));
}
#endif  // _GDIPLUS_H


///////////////////////////////////////////////////////////////////////////////
// APIs
//

inline HBITMAP WINAPI ImageList_GetBitmap(HIMAGELIST hImageList, int nIndex, HBITMAP* phbmMask = NULL)
{
    ATLASSERT(hImageList);
    ATLASSERT(nIndex >= 0 && nIndex < ::ImageList_GetImageCount(hImageList));

    ICONINFO iconInfo = { 0 };
    if (HICON hIcon = ::ImageList_GetIcon(hImageList, nIndex, ILD_NORMAL))
    {
        if (::GetIconInfo(hIcon, &iconInfo))
        {
            if (phbmMask != NULL)
                *phbmMask = iconInfo.hbmMask;
            else
                ATLVERIFY(::DeleteObject(iconInfo.hbmMask));
        }

        ATLVERIFY(::DestroyIcon(hIcon));
    }

    return iconInfo.hbmColor;
}

#if (_WIN32_WINNT >= 0x0501) && (defined(__ATLSTR_H__) || defined(__AFXSTR_H__))
// The pszText and pszTitle parameters can pass pointer to a null-terminated string, or resource ID of the string.
inline void WINAPI AtlMessageBox(HWND hWnd, LPCTSTR pszText, LPCTSTR pszTitle, INT nIcon = TTI_ERROR, UINT nType = MB_OK | MB_ICONERROR)
{
    ATLASSERT(pszText);
    ATLASSERT(pszTitle);

    CStringW strTitle(pszTitle), strText(pszText);
    EDITBALLOONTIP balloonTip = { sizeof(EDITBALLOONTIP), strTitle, strText, nIcon };
    if (!::SendMessage(hWnd, EM_SHOWBALLOONTIP, 0, (LPARAM)&balloonTip))
        ::MessageBoxW(hWnd, strTitle, strText, nType);

    ::SetFocus(hWnd);
}
#endif  // (_WIN32_WINNT >= 0x0501) && (defined(__ATLSTR_H__) || defined(__AFXSTR_H__))

#ifdef _INC_SHLWAPI
inline void WINAPI DrawMenuText(HDC hDC, LPCTSTR pszText, LPCRECT pRect, COLORREF clrText = ::GetSysColor(COLOR_MENUTEXT), BOOL bShowKeyboardCues = TRUE)
{
    ATLASSERT(hDC);
    ATLASSERT(pRect);
    ATLASSERT(pszText);
    UNUSED_PARAMETER(bShowKeyboardCues);

    ::SetTextColor(hDC, clrText);
    ::SetBkMode(hDC, TRANSPARENT);

    int nLength = lstrlen(pszText);
    if (LPCTSTR pchTab = ::_tsstrrchr(pszText, pszText + nLength, _T('\t')))
    {
        nLength = pchTab - pszText;
        ::DrawText(hDC, pchTab + 1, -1, (LPRECT)pRect, DT_RIGHT | DT_SINGLELINE | DT_VCENTER);
    }

#ifndef _WIN32_WCE
    ::DrawText(hDC, pszText, nLength, (LPRECT)pRect, DT_LEFT | DT_SINGLELINE | DT_VCENTER | (bShowKeyboardCues ? 0 : DT_HIDEPREFIX));
#else
    ::DrawText(hDC, pszText, nLength, (LPRECT)pRect, DT_LEFT | DT_SINGLELINE | DT_VCENTER);
#endif  // _WIN32_WCE
}
#endif  // _INC_SHLWAPI


#ifndef _WIN32_WCE
typedef struct _SHNAMEMAPPINGS
{
    UINT            nNumberOfMappings;
    LPSHNAMEMAPPING pNameMapping;
} SHNAMEMAPPINGS, *LPSHNAMEMAPPINGS;
#endif  // _WIN32_WCE

// _EnumNameMapping prototype : BOOL EnumNameMapping(LPSHNAMEMAPPING pMapping, DWORD_PTR dwData);
template <typename _Predicate>
inline UINT SHEnumNameMappings(HANDLE hNameMappings, _Predicate _EnumNameMapping, DWORD_PTR dwData)
{
    LPSHNAMEMAPPINGS pMappings = reinterpret_cast<LPSHNAMEMAPPINGS>(hNameMappings);
    ATLASSERT(pMappings);

    UINT nCount = 0;
    for (LPSHNAMEMAPPING pMapping = pMappings->pNameMapping; nCount < pMappings->nNumberOfMappings; ++nCount, ++pMapping)
    {
        if (!_EnumNameMapping(pMapping, dwData))
            break;
    }

    return nCount;
}


///////////////////////////////////////////////////////////////////////////////
// Bitmaps AlphaBlt
//

Dst.R = (Src.R × A + Dst.R × (255 – A)) >> 8;
Dst.G = (Src.G × A + Dst.G × (255 – A)) >> 8;
Dst.B = (Src.B × A + Dst.B × (255 – A)) >> 8;

BITMAP bmp;
-bmp.bmHeight;
bmp.bmBits = (LPBYTE)bmp.bmBits + ((bmp.bmHeight - 1) * bmp.bmWidthBytes);

inline HBITMAP WINAPI CreateDIBitmap(HDC hDestDC, int cx, int cy, HDC hSrcDC, int srcX, int srcY, LPVOID* ppBits)
{
    ATLASSERT(ppBits);

    HBITMAP hBitmap = NULL;
    BITMAPINFO bitmapInfo = { { sizeof(BITMAPINFOHEADER), cx, cy, 1, 32, BI_RGB } };

    if ((hBitmap = ::CreateDIBSection(NULL, &bitmapInfo, DIB_RGB_COLORS, ppBits, NULL, 0)) != NULL)
    {
        HGDIOBJ hOldBitmap = ::SelectObject(hDestDC, hBitmap);
        ATLVERIFY(::BitBlt(hDestDC, 0, 0, cx, cy, hSrcDC, srcX, srcY, SRCCOPY));
        ::SelectObject(hDestDC, hOldBitmap);
    }

    return hBitmap;
}

#define MAKEARGB(_SrcColor, _DestColor, _Alpha) \
    RGB(GetRValue(_SrcColor) * (double)(_Alpha) + GetRValue(_DestColor) * (1.0 - (double)(_Alpha)), \
    GetGValue(_SrcColor) * (double)(_Alpha) + GetGValue(_DestColor) * (1.0 - (double)(_Alpha)), \
    GetBValue(_SrcColor) * (double)(_Alpha) + GetBValue(_DestColor) * (1.0 - (double)(_Alpha)))

inline BOOL WINAPI AlphaBlt(HDC hDestDC, int x, int y, int nWidth, int nHeight, HDC hSrcDC, int srcX, int srcY, double dblSourceAlpha)
{
    ATLASSERT(hSrcDC);
    ATLASSERT(hDestDC);

    BOOL bSuccessful = FALSE;
    HDC hMemDC = atlutil::CMemoryDCPool::GetDC();
    ATLASSERT(hMemDC);

    LPDWORD pSrcBits = NULL, pDestBits = NULL;
    if (HBITMAP hSrcDIB  = CreateDIBitmap(hMemDC, nWidth, nHeight, hSrcDC, srcX, srcY, (LPVOID*)&pSrcBits))
    {
        if (HBITMAP hDestDIB = CreateDIBitmap(hMemDC, nWidth, nHeight, hDestDC, x, y, (LPVOID*)&pDestBits))
        {
            for (int i = 0; i < nWidth * nHeight; ++i)
                pDestBits[i] = MAKEARGB(pSrcBits[i], pDestBits[i], dblSourceAlpha);

            HGDIOBJ hOldBitmap = ::SelectObject(hMemDC, hDestDIB);
            bSuccessful = ::BitBlt(hDestDC, x, y, nWidth, nHeight, hMemDC, 0, 0, SRCCOPY);

            ::SelectObject(hMemDC, hOldBitmap);
            ATLVERIFY(::DeleteObject(hDestDIB));
        }

        ATLVERIFY(::DeleteObject(hSrcDIB));
    }

    atlutil::CMemoryDCPool::ReleaseDC(hMemDC);
    return bSuccessful;
}

inline BOOL WINAPI AlphaDraw(HDC hDestDC, int x, int y, int nWidth, int nHeight, HDC hSrcDC, int srcX, int srcY, double dblSourceAlpha)
{
    ATLASSERT(hSrcDC);
    ATLASSERT(hDestDC);

    BOOL bSuccessful = FALSE;
    HDC hMemDC = atlutil::CMemoryDCPool::GetDC();
    ATLASSERT(hMemDC);

    LPDWORD pSrcBits = NULL, pDestBits = NULL;
    if (HBITMAP hSrcDIB  = CreateDIBitmap(hMemDC, nWidth, nHeight, hSrcDC, srcX, srcY, (LPVOID*)&pSrcBits))
    {
        if (HBITMAP hDestDIB = CreateDIBitmap(hMemDC, nWidth, nHeight, hDestDC, x, y, (LPVOID*)&pDestBits))
        {
            for (int i = 0; i < nWidth * nHeight; ++i)
            {
                double dblAplha = (double)LOBYTE(pSrcBits[i] >> 24) / 255.0 * dblSourceAlpha;
                pDestBits[i] = MAKEARGB(pSrcBits[i], pDestBits[i], dblAplha);
            }

            HGDIOBJ hOldBitmap = ::SelectObject(hMemDC, hDestDIB);
            bSuccessful = ::BitBlt(hDestDC, x, y, nWidth, nHeight, hMemDC, 0, 0, SRCCOPY);

            ::SelectObject(hMemDC, hOldBitmap);
            ATLVERIFY(::DeleteObject(hDestDIB));
        }

        ATLVERIFY(::DeleteObject(hSrcDIB));
    }

    atlutil::CMemoryDCPool::ReleaseDC(hMemDC);
    return bSuccessful;
}

inline BOOL WINAPI AlphaFillRect(HDC hDC, const RECT& rect, DWORD color)
{
    HDC hMemDC = atlutil::CMemoryDCPool::GetDC();
    ATLASSERT(hMemDC);

    int cx = rect.right - rect.left, cy = rect.bottom - rect.top;

    LPDWORD pBits = NULL;
    BITMAPINFO bitmapInfo = { { sizeof(BITMAPINFOHEADER), cx, cy, 1, 32, BI_RGB } };
    HBITMAP hBitmap = ::CreateDIBSection(NULL, &bitmapInfo, DIB_RGB_COLORS, (LPVOID*)&pBits, NULL, 0);

    HGDIOBJ hOldBitmap = ::SelectObject(hMemDC, hBitmap);
    ATLVERIFY(::BitBlt(hMemDC, 0, 0, cx, cy, hDC, rect.left, rect.top, SRCCOPY));

    double dblAplha = (LOBYTE(color >> 24) / 255.0);
    double Red   = GetBValue(color) * dblAplha;
    double Green = GetGValue(color) * dblAplha;
    double Blue  = GetRValue(color) * dblAplha;

    for (int i = 0; i < cx * cy; ++i)
    {
        pBits[i] = RGB(Red   + GetRValue(pBits[i]) * (1.0 - dblAplha),
            Green + GetGValue(pBits[i]) * (1.0 - dblAplha),
            Blue  + GetBValue(pBits[i]) * (1.0 - dblAplha));
    }

    BOOL bSuccessful = ::BitBlt(hDC, rect.left, rect.top, cx, cy, hMemDC, 0, 0, SRCCOPY);

    ::SelectObject(hMemDC, hOldBitmap);
    ATLVERIFY(::DeleteObject(hBitmap));
    atlutil::CMemoryDCPool::ReleaseDC(hMemDC);

    //double dblAplha = (LOBYTE(color >> 24) / 255.0);
    //double Blue  = GetRValue(color) * dblAplha;
    //double Green = GetGValue(color) * dblAplha;
    //double Red   = GetBValue(color) * dblAplha;
    //COLORREF clrDest;

    //for (int x = rect.left; x < rect.right; ++x)
    //{
    //    for (int y = rect.top; y < rect.bottom; ++y)
    //    {
    //        clrDest = ::GetPixel(hDC, x, y);
    //        ::SetPixel(hDC, x, y, RGB(Red   + GetRValue(clrDest) * (1.0 - dblAplha),
    //                                  Green + GetGValue(clrDest) * (1.0 - dblAplha),
    //                                  Blue  + GetBValue(clrDest) * (1.0 - dblAplha)));
    //    }
    //}

    return bSuccessful;
}


///////////////////////////////////////////////////////////////////////////////
// GradientFillRgn
//

inline void WINAPI GradientHoriFill(HDC hDC, const RGNDATA* pRgnData, COLORREF clrFrom, COLORREF clrTo)
{
    ATLASSERT(hDC);
    ATLASSERT(pRgnData);
    ATLASSERT(pRgnData->rdh.nCount != 0);
    ATLASSERT(pRgnData->rdh.iType == RDH_RECTANGLES);

    LPRECT pRect = (LPRECT)pRgnData->Buffer;
    ATLASSERT(pRect);

    const int nWidth   = pRgnData->rdh.rcBound.right - pRgnData->rdh.rcBound.left;
    const int nColor[] = { GetRValue(clrFrom), GetGValue(clrFrom), GetBValue(clrFrom) };
    const int nDelta[] = { GetRValue(clrTo) - nColor[0], GetGValue(clrTo) - nColor[1], GetBValue(clrTo) - nColor[2] };

#ifndef _WIN32_WCE
    HGDIOBJ hOldPen = ::SelectObject(hDC, ::GetStockObject(DC_PEN));
    for (DWORD i = 0; i < pRgnData->rdh.nCount; ++i)
    {
        for (int x = pRect[i].left, dx = pRect[i].left - pRgnData->rdh.rcBound.left; x < pRect[i].right; ++x, ++dx)
        {
            ::SetDCPenColor(hDC, RGB(nColor[0] + (dx * nDelta[0] / nWidth), nColor[1] + (dx * nDelta[1] / nWidth), nColor[2] + (dx * nDelta[2] / nWidth)));
            ATLVERIFY(::MoveToEx(hDC, x, pRect[i].top, NULL));
            ATLVERIFY(::LineTo(hDC, x, pRect[i].bottom));
        }
    }

    ::SelectObject(hDC, hOldPen);
#else
    for (DWORD i = 0; i < pRgnData->rdh.nCount; ++i)
    {
        for (int x = pRect[i].left, dx = pRect[i].left - pRgnData->rdh.rcBound.left; x < pRect[i].right; ++x, ++dx)
        {
            HPEN hPen = ::CreatePen(PS_SOLID, 1, RGB(nColor[0] + (dx * nDelta[0] / nWidth), nColor[1] + (dx * nDelta[1] / nWidth), nColor[2] + (dx * nDelta[2] / nWidth)));
            ATLASSERT(hPen);

            HGDIOBJ hOldPen = ::SelectObject(hDC, hPen);
            ATLVERIFY(::MoveToEx(hDC, x, pRect[i].top, NULL));
            ATLVERIFY(::LineTo(hDC, x, pRect[i].bottom));

            ::SelectObject(hDC, hOldPen);
            ATLVERIFY(::DeleteObject(hPen));
        }
    }
#endif  // _WIN32_WCE
}

inline void WINAPI GradientVertFill(HDC hDC, const RGNDATA* pRgnData, COLORREF clrFrom, COLORREF clrTo)
{
    ATLASSERT(hDC);
    ATLASSERT(pRgnData);
    ATLASSERT(pRgnData->rdh.nCount != 0);
    ATLASSERT(pRgnData->rdh.iType == RDH_RECTANGLES);

    LPRECT pRect = (LPRECT)pRgnData->Buffer;
    ATLASSERT(pRect);

    const int nHeight  = pRgnData->rdh.rcBound.bottom - pRgnData->rdh.rcBound.top;
    const int nColor[] = { GetRValue(clrFrom), GetGValue(clrFrom), GetBValue(clrFrom) };
    const int nDelta[] = { GetRValue(clrTo) - nColor[0], GetGValue(clrTo) - nColor[1], GetBValue(clrTo) - nColor[2] };

#ifndef _WIN32_WCE
    HGDIOBJ hOldPen = ::SelectObject(hDC, ::GetStockObject(DC_PEN));
    for (DWORD i = 0; i < pRgnData->rdh.nCount; ++i)
    {
        for (int y = pRect[i].top, dy = pRect[i].top - pRgnData->rdh.rcBound.top; y < pRect[i].bottom; ++y, ++dy)
        {
            ::SetDCPenColor(hDC, RGB(nColor[0] + (dy * nDelta[0] / nHeight), nColor[1] + (dy * nDelta[1] / nHeight), nColor[2] + (dy * nDelta[2] / nHeight)));
            ATLVERIFY(::MoveToEx(hDC, pRect[i].left, y, NULL));
            ATLVERIFY(::LineTo(hDC, pRect[i].right, y));
        }
    }

    ::SelectObject(hDC, hOldPen);
#else
    for (DWORD i = 0; i < pRgnData->rdh.nCount; ++i)
    {
        for (int y = pRect[i].top, dy = pRect[i].top - pRgnData->rdh.rcBound.top; y < pRect[i].bottom; ++y, ++dy)
        {
            HPEN hPen = ::CreatePen(PS_SOLID, 1, RGB(nColor[0] + (dy * nDelta[0] / nHeight), nColor[1] + (dy * nDelta[1] / nHeight), nColor[2] + (dy * nDelta[2] / nHeight)));
            ATLASSERT(hPen);

            HGDIOBJ hOldPen = ::SelectObject(hDC, hPen);
            ATLVERIFY(::MoveToEx(hDC, pRect[i].left, y, NULL));
            ATLVERIFY(::LineTo(hDC, pRect[i].right, y));

            ::SelectObject(hDC, hOldPen);
            ATLVERIFY(::DeleteObject(hPen));
        }
    }
#endif  // _WIN32_WCE
}

inline BOOL WINAPI GradientFillRgn(HDC hDC, HRGN hRgn, COLORREF clrFrom, COLORREF clrTo, UINT nMode = GRADIENT_FILL_REGION_H)
{
    ATLASSERT(hDC);
    ATLASSERT(hRgn);

    ATL::CTempBuffer<RGNDATA, 512> rgnData;
    DWORD cbSize = ::GetRegionData(hRgn, 0, NULL);

    BOOL bSuccessful = (cbSize != 0 && rgnData.AllocateBytes(cbSize) != NULL && (::GetRegionData(hRgn, cbSize, rgnData) == cbSize));
    if (bSuccessful)
    {
        if (nMode == GRADIENT_FILL_REGION_V)
            GradientVertFill(hDC, rgnData, clrFrom, clrTo);
        else
            GradientHoriFill(hDC, rgnData, clrFrom, clrTo);
    }

    return bSuccessful;
}

typedef union _COLOR
{
    COLORREF Value;
    struct { BYTE Red; BYTE Green; BYTE Blue; BYTE Reserved; };
} COLOR, *LPCOLOR;

typedef struct _GRADIENT_COLOR
{
    COLOR From;
    COLOR To;
} GRADIENT_COLOR, *LPGRADIENT_COLOR;

inline void WINAPI GradientHoriFill(HDC hDC, const RGNDATA* pRgnData, const GRADIENT_COLOR* pColors, UINT nCount)
{
    ATLASSERT(hDC);
    ATLASSERT(pColors);
    ATLASSERT(pRgnData);
    ATLASSERT(nCount != 0);
    ATLASSERT(pRgnData->rdh.nCount != 0);
    ATLASSERT(pRgnData->rdh.iType == RDH_RECTANGLES);

    LPRECT pRect = (LPRECT)pRgnData->Buffer;
    ATLASSERT(pRect);

    int nOffset = pRect->left - pRgnData->rdh.rcBound.left;
    int nWidth  = (pRgnData->rdh.rcBound.right - pRgnData->rdh.rcBound.left) / nCount;

    HGDIOBJ hOldPen = ::SelectObject(hDC, ::GetStockObject(DC_PEN));
    for (DWORD i = 0, nIndex = 0; i < pRgnData->rdh.nCount; ++i)
    {
        for (int x = pRect[i].left; x < pRect[i].right; ++x)
        {
            ::SetDCPenColor(hDC, RGB(pColors[nIndex].From.Red + (nOffset * (pColors[nIndex].To.Red - pColors[nIndex].From.Red) / nWidth), pColors[nIndex].From.Green + (nOffset * (pColors[nIndex].To.Green - pColors[nIndex].From.Green) / nWidth), pColors[nIndex].From.Blue + (nOffset * (pColors[nIndex].To.Blue - pColors[nIndex].From.Blue) / nWidth)));
            ATLVERIFY(::MoveToEx(hDC, x, pRect[i].top, NULL));
            ATLVERIFY(::LineTo(hDC, x, pRect[i].bottom));

            nIndex  = (x - pRgnData->rdh.rcBound.left) / nWidth;
            nOffset = (x - pRgnData->rdh.rcBound.left) % nWidth;
        }
    }

    ::SelectObject(hDC, hOldPen);
}

inline void WINAPI GradientVertFill(HDC hDC, const RGNDATA* pRgnData, const GRADIENT_COLOR* pColors, UINT nCount)
{
    ATLASSERT(hDC);
    ATLASSERT(pColors);
    ATLASSERT(pRgnData);
    ATLASSERT(nCount != 0);
    ATLASSERT(pRgnData->rdh.nCount != 0);
    ATLASSERT(pRgnData->rdh.iType == RDH_RECTANGLES);

    LPRECT pRect = (LPRECT)pRgnData->Buffer;
    ATLASSERT(pRect);

    int nOffset = pRect->top - pRgnData->rdh.rcBound.top;
    int nHeight = (pRgnData->rdh.rcBound.bottom - pRgnData->rdh.rcBound.top) / nCount;

    HGDIOBJ hOldPen = ::SelectObject(hDC, ::GetStockObject(DC_PEN));
    for (DWORD i = 0, nIndex = 0; i < pRgnData->rdh.nCount; ++i)
    {
        for (int y = pRect[i].top; y < pRect[i].bottom; ++y)
        {
            ::SetDCPenColor(hDC, RGB(pColors[nIndex].From.Red + (nOffset * (pColors[nIndex].To.Red - pColors[nIndex].From.Red) / nHeight), pColors[nIndex].From.Green + (nOffset * (pColors[nIndex].To.Green - pColors[nIndex].From.Green) / nHeight), pColors[nIndex].From.Blue + (nOffset * (pColors[nIndex].To.Blue - pColors[nIndex].From.Blue) / nHeight)));
            ATLVERIFY(::MoveToEx(hDC, pRect[i].left, y, NULL));
            ATLVERIFY(::LineTo(hDC, pRect[i].right, y));

            nIndex  = (y - pRgnData->rdh.rcBound.top) / (nHeight + 1);
            nOffset = (y - pRgnData->rdh.rcBound.top) % (nHeight + 1);
        }
    }

    ::SelectObject(hDC, hOldPen);
}

inline BOOL WINAPI GradientFillRgn(HDC hDC, HRGN hRgn, const GRADIENT_COLOR* pColors, UINT nCount, UINT nMode = GRADIENT_FILL_REGION_H)
{
    ATLASSERT(hDC);
    ATLASSERT(hRgn);

    ATL::CTempBuffer<RGNDATA, 512> rgnData;
    DWORD cbSize = ::GetRegionData(hRgn, 0, NULL);

    BOOL bSuccessful = (cbSize != 0 && rgnData.AllocateBytes(cbSize) != NULL && (::GetRegionData(hRgn, cbSize, rgnData) == cbSize));
    if (bSuccessful)
    {
        if (nMode == GRADIENT_FILL_REGION_V)
            GradientVertFill(hDC, rgnData, pColors, nCount);
        else
            GradientHoriFill(hDC, rgnData, pColors, nCount);
    }

    return bSuccessful;
}

inline BOOL WINAPI GradientFillRgn(HDC hDC, HRGN hRgn, COLORREF clrFrom, COLORREF clrTo, UINT nMode = GRADIENT_FILL_REGION_H)
{
    ATLASSERT(hDC);
    ATLASSERT(hRgn);

    GRADIENT_COLOR color = { clrFrom, clrTo };
    return GradientFillRgn(hDC, hRgn, &color, 1, nMode);
}

inline BOOL WINAPI GradientFillRgn(HDC hDC, HRGN hRgn, COLORREF clrFrom, COLORREF clrMiddle, COLORREF clrTo, UINT nMode)
{
    ATLASSERT(hDC);
    ATLASSERT(hRgn);

    GRADIENT_COLOR colors[] = { clrFrom, clrMiddle, clrMiddle, clrTo };
    return GradientFillRgn(hDC, hRgn, colors, _countof(colors), nMode);
}


///////////////////////////////////////////////////////////////////////////////
// CreateRegion
//

inline HRGN WINAPI CreateRegion(const COLORREF* pColors, int x, int y, int nWidth, int nHeight, COLORREF clrMask, UINT nInitCount = 64)
{
    ATLASSERT(pColors);
    ATLASSERT(x >= 0 && y >= 0);
    ATLASSERT(nWidth > 0 && nHeight > 0);

    // Convert COLORREF to RGBQUAD
    clrMask = RGB(GetBValue(clrMask), GetGValue(clrMask), GetRValue(clrMask));
    RECT rcPreLine = { 0 }, rcLine = { sizeof(RGNDATAHEADER), RDH_RECTANGLES };

    std::vector<RECT> vecRects;
    vecRects.reserve(nInitCount);       // Reserves nInitCount Rectangles.
    vecRects.push_back(rcLine);         // Add the RGNDATAHEADER.
    vecRects.push_back(rcLine);

    for (int i = 0, nStart = 0; i < nHeight; ++i, pColors += nWidth)
    {
        for (int j = 0; j < nWidth; ++j)
        {
            for (; j < nWidth && pColors[j] == clrMask; ++j);   // Find start index.
            nStart = j;
            for (; j < nWidth && pColors[j] != clrMask; ++j);   // Find end index.

            if (j > nStart)
            {
                SetRect(rcLine, x + nStart, y + i, x + j, y + i + 1);
                if (rcPreLine.left == rcLine.left && rcPreLine.right == rcLine.right)
                    ATLVERIFY(::UnionRect(&vecRects.back(), &vecRects.back(), &rcLine));
                else
                    rcPreLine = rcLine, vecRects.push_back(rcLine);
            }
        }
    }

    LPRGNDATA pRgnData = (LPRGNDATA)&vecRects.front();
    pRgnData->rdh.nCount = vecRects.size() - 2;
    SetRect(pRgnData->rdh.rcBound, x, y, x + nWidth, y + nHeight);

    ATLTRACE(_T("The region contains %u rectangles.\n"), pRgnData->rdh.nCount);
    return ::ExtCreateRegion(NULL, vecRects.size() * sizeof(RECT), pRgnData);
}

inline HRGN WINAPI CreateRegion(HBITMAP hBitmap, COLORREF clrMask, int x = 0, int y = 0, UINT nInitCount = 64)
{
    ATLASSERT(hBitmap);
    ATLASSERT(x >= 0 && y >= 0);
    HRGN hRgn = NULL;

    BITMAP bmp;
    ATLVERIFY(::GetObject(hBitmap, sizeof(BITMAP), &bmp));

#ifndef _WIN32_WCE
    if (LPVOID pColors = stdutil::GetDIBits(hBitmap, bmp.bmWidth, -bmp.bmHeight))
    {
        hRgn = CreateRegion((LPCOLORREF)pColors, x, y, bmp.bmWidth, bmp.bmHeight, clrMask, nInitCount);
        ::free(pColors);
    }
#else
    LPVOID pColors = NULL;
    if (HBITMAP hbmDIB = CreateDIBitmap(hBitmap, 0, 0, bmp.bmWidth, -bmp.bmHeight, &pColors))
    {
        hRgn = CreateRegion((LPCOLORREF)pColors, x, y, bmp.bmWidth, bmp.bmHeight, clrMask, nInitCount);
        ATLVERIFY(::DeleteObject(hbmDIB));
    }
#endif  // _WIN32_WCE

    return hRgn;
}

inline HBITMAP WINAPI CreateMonochromeBitmap(HBITMAP hBitmap, COLORREF clrMask, int nWidth, int nHeight, LPVOID* ppBits)
{
    ATLASSERT(ppBits);
    ATLASSERT(hBitmap);

    // Initilize bitmap info.
    BYTE nBitmapInfo[sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 2] = { 0 };
    LPBITMAPINFO pBitmapInfo = (LPBITMAPINFO)nBitmapInfo;
    pBitmapInfo->bmiHeader.biSize    = sizeof(BITMAPINFOHEADER);
    pBitmapInfo->bmiHeader.biWidth   = nWidth;
    pBitmapInfo->bmiHeader.biHeight  = nHeight;
    pBitmapInfo->bmiHeader.biPlanes  = pBitmapInfo->bmiHeader.biBitCount  = 1;
    pBitmapInfo->bmiColors[1].rgbRed = pBitmapInfo->bmiColors[1].rgbGreen = pBitmapInfo->bmiColors[1].rgbBlue = 0xFF;

    // Create monochrome DIB section.
    HBITMAP hbmDIB = NULL;
    if ((hbmDIB = ::CreateDIBSection(NULL, pBitmapInfo, DIB_RGB_COLORS, ppBits, NULL, 0)) != NULL)
    {
        // Draw hBitmap. When the function returns, the ppBits parameter contains DIB bit values.
        atlutil::CBitmapDC dcDest(hbmDIB), dcSrc(hBitmap);
        ::SetBkColor(dcSrc, clrMask);
        ATLVERIFY(::BitBlt(dcDest, 0, 0, nWidth, ::abs(nHeight), dcSrc, 0, 0, SRCCOPY));
    }

    return hbmDIB;
}

inline HRGN WINAPI CreateRegion(const BYTE* pBits, int x, int y, int nWidth, int nHeight)
{
    ATLASSERT(pBits);
    ATLASSERT(nWidth > 0 && nHeight > 0);

    HRGN hRgn = ::CreateRectRgn(x, y, x + nWidth, y + nHeight), hRgnMask = ::CreateRectRgn(0, 0, 0, 0);
    ATLASSERT(hRgn);
    ATLASSERT(hRgnMask);

    const UINT nStride = ComputeStride(nWidth, 1);
    const BYTE nMask[] = { 0x80, 0x40, 0x20, 0x10, 0x8, 0x4, 0x2, 0x1 };

    for (int i = 0, nStart = 0; i < nHeight; ++i, pBits += nStride)
    {
        for (int j = 0; j < nWidth; ++j)
        {
            for (nStart = j; j < nWidth; ++j)
            {
                if (pBits[j >> 3] == 0xFF)      // skips all invalid bits.
                    j += 7;
                else if ((pBits[j >> 3] & nMask[j % 8]) == 0)
                    break;
            }

            if (j > nStart)
            {
                ATLVERIFY(::SetRectRgn(hRgnMask, nStart + x, i + y, j + x, i + y + 1));
                ATLVERIFY(::CombineRgn(hRgn, hRgn, hRgnMask, RGN_DIFF));
            }
        }
    }

    ATLVERIFY(::DeleteObject(hRgnMask));
    return hRgn;
}

inline HRGN WINAPI CreateRegion(HBITMAP hBitmap, COLORREF clrMask, int x = 0, int y = 0)
{
    ATLASSERT(hBitmap);

    HRGN hRgn    = NULL;
    LPVOID pBits = NULL;

    BITMAP bmp;
    ATLVERIFY(::GetObject(hBitmap, sizeof(BITMAP), &bmp));

    if (HBITMAP hbmDIB = CreateMonochromeBitmap(hBitmap, clrMask, bmp.bmWidth, -bmp.bmHeight, &pBits))
    {
        hRgn = CreateRegion((LPBYTE)pBits, x, y, bmp.bmWidth, bmp.bmHeight);
        ATLVERIFY(::DeleteObject(hbmDIB));
    }

    return hRgn;
}
