///////////////////////////////////////////////////////////////////////////////
// crtdbgutil.inl
//
// Copyright(c) 2019, Garfield. All rights reserved.
// Author  : Garfield
// Version : 1.0
// Creation Date : 2008/11/23

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __CRTDBGUTIL_INL__
#define __CRTDBGUTIL_INL__

#ifndef __CRTDBGUTIL_H__
    #error crtdbgutil.inl requires crtdbgutil.h to be included first
#endif

namespace stdutil {

#if (defined(__CRT_CONSOLE_TRACE__) && !defined(_WIN32_WCE))
///////////////////////////////////////////////////////////////////////////////
// Implementation of the CCrtConsole class
//

__declspec(selectany) CCrtConsole CCrtConsole::_CrtConsole;

ATL_NOINLINE inline CCrtConsole::CCrtConsole()
{
    ::AllocConsole();
    m_hConsole = ::GetStdHandle(STD_OUTPUT_HANDLE);
    ATLASSERT(m_hConsole);

    if (HMENU hMenu = ::GetSystemMenu(::GetConsoleWindow(), FALSE))
        ::DeleteMenu(hMenu, SC_CLOSE, MF_BYCOMMAND);
}

ATL_NOINLINE inline CCrtConsole::~CCrtConsole()
{
    ::FreeConsole();
}

ATL_NOINLINE inline void __cdecl CCrtConsole::operator()(LPCSTR pszFormat, ...) const
{
    ATLASSERT(pszFormat);

    va_list _Args;
    va_start(_Args, pszFormat);

    CStringA strMessage;
    strMessage.FormatV(pszFormat, _Args);
    va_end(_Args);

    DWORD dwWritten = 0;
    ATLVERIFY(::WriteConsoleA(m_hConsole, strMessage, strMessage.GetLength(), &dwWritten, NULL));
}

ATL_NOINLINE inline void __cdecl CCrtConsole::operator()(LPCWSTR pszFormat, ...) const
{
    ATLASSERT(pszFormat);

    va_list _Args;
    va_start(_Args, pszFormat);

    CStringW strMessage;
    strMessage.FormatV(pszFormat, _Args);
    va_end(_Args);

    DWORD dwWritten = 0;
    ATLVERIFY(::WriteConsoleW(m_hConsole, strMessage, strMessage.GetLength(), &dwWritten, NULL));
}

ATL_NOINLINE inline void CCrtConsole::operator()(DWORD dwErrorCode/* = ::GetLastError()*/) const
{
    TCHAR szMessage[MAX_PATH];
    if (::FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, dwErrorCode, LANG_SYSTEM_DEFAULT, szMessage, _countof(szMessage), NULL) == 0)
        ::_tssprintf(szMessage, _countof(szMessage), _T("Unknown Error Code = 0x%8.8x"), dwErrorCode);

    DWORD dwWritten = 0;
    ATLVERIFY(::WriteConsole(m_hConsole, szMessage, ::lstrlen(szMessage), &dwWritten, NULL));
}

ATL_NOINLINE inline void __cdecl CCrtConsole::LogPrint(LPCSTR pszTag, LPCSTR pszFormat, ...) const
{
    ATLASSERT(pszTag);
    ATLASSERT(pszFormat);

    SYSTEMTIME st;
    ::GetLocalTime(&st);

    CStringA strMessage;
    strMessage.Format("| %04d-%02d-%02d %02d:%02d:%02d.%03d | %s | ", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds, pszTag);

    va_list _Args;
    va_start(_Args, pszFormat);

    strMessage.AppendFormatV(pszFormat, _Args);
    va_end(_Args);

    DWORD dwWritten = 0;
    ATLVERIFY(::WriteConsoleA(m_hConsole, strMessage, strMessage.GetLength(), &dwWritten, NULL));
}

ATL_NOINLINE inline void __cdecl CCrtConsole::LogPrint(LPCWSTR pszTag, LPCWSTR pszFormat, ...) const
{
    ATLASSERT(pszTag);
    ATLASSERT(pszFormat);

    SYSTEMTIME st;
    ::GetLocalTime(&st);

    CStringW strMessage;
    strMessage.Format(L"| %04d-%02d-%02d %02d:%02d:%02d.%03d | %s | ", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds, pszTag);

    va_list _Args;
    va_start(_Args, pszFormat);

    strMessage.AppendFormatV(pszFormat, _Args);
    va_end(_Args);

    DWORD dwWritten = 0;
    ATLVERIFY(::WriteConsoleW(m_hConsole, strMessage, strMessage.GetLength(), &dwWritten, NULL));
}
#endif  // (defined(__CRT_CONSOLE_TRACE__) && !defined(_WIN32_WCE))


#if (defined(__CRT_TRACE__) || defined(_DEBUG))
///////////////////////////////////////////////////////////////////////////////
// Implementation of the CCrtTrace class
//

__declspec(selectany) CCrtTrace CCrtTrace::_CrtTrace;

ATL_NOINLINE inline CCrtTrace::CCrtTrace()
{
}

ATL_NOINLINE inline CCrtTrace::~CCrtTrace()
{
}

ATL_NOINLINE inline void __cdecl CCrtTrace::operator()(LPCSTR pszFormat, ...) const
{
    ATLASSERT(pszFormat);

    va_list _Args;
    va_start(_Args, pszFormat);

    CStringA strText;
    strText.FormatV(pszFormat, _Args);
    va_end(_Args);

#ifndef _WIN32_WCE
    ::OutputDebugStringA(strText);
#else
    ::OutputDebugStringW(ATL::CA2W(strText));
#endif  // _WIN32_WCE
}

ATL_NOINLINE inline void __cdecl CCrtTrace::operator()(LPCWSTR pszFormat, ...) const
{
    ATLASSERT(pszFormat);

    va_list _Args;
    va_start(_Args, pszFormat);

    CStringW strText;
    strText.FormatV(pszFormat, _Args);
    va_end(_Args);

    ::OutputDebugStringW(strText);
}

ATL_NOINLINE inline void CCrtTrace::operator()(DWORD dwErrorCode/* = ::GetLastError()*/) const
{
    TCHAR szMessage[MAX_PATH];
    if (::FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, dwErrorCode, LANG_SYSTEM_DEFAULT, szMessage, _countof(szMessage), NULL) == 0)
        ::_tssprintf(szMessage, _countof(szMessage), _T("Unknown Error Code = 0x%8.8x"), dwErrorCode);

    ::OutputDebugString(szMessage);
}

ATL_NOINLINE inline void __cdecl CCrtTrace::LogPrint(LPCSTR pszTag, LPCSTR pszFormat, ...) const
{
    ATLASSERT(pszTag);
    ATLASSERT(pszFormat);

    SYSTEMTIME st;
    ::GetLocalTime(&st);

    CStringA strMessage;
    strMessage.Format("| %04d-%02d-%02d %02d:%02d:%02d.%03d | %s | ", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds, pszTag);

    va_list _Args;
    va_start(_Args, pszFormat);

    strMessage.AppendFormatV(pszFormat, _Args);
    va_end(_Args);

    ::OutputDebugStringA(strMessage);
}

ATL_NOINLINE inline void __cdecl CCrtTrace::LogPrint(LPCWSTR pszTag, LPCWSTR pszFormat, ...) const
{
    ATLASSERT(pszTag);
    ATLASSERT(pszFormat);

    SYSTEMTIME st;
    ::GetLocalTime(&st);

    CStringW strMessage;
    strMessage.Format(L"| %04d-%02d-%02d %02d:%02d:%02d.%03d | %s | ", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds, pszTag);

    va_list _Args;
    va_start(_Args, pszFormat);

    strMessage.AppendFormatV(pszFormat, _Args);
    va_end(_Args);

    ::OutputDebugStringW(strMessage);
}
#endif  // (defined(__CRT_TRACE__) || defined(_DEBUG))


#ifdef __CRT_REPORT_LOG__
///////////////////////////////////////////////////////////////////////////////
// Implementation of the CCrtReportLog class
//

__declspec(selectany) CCrtReportLog CCrtReportLog::_CrtReportLog;

ATL_NOINLINE inline CCrtReportLog::CCrtReportLog()
    : m_hFile(INVALID_HANDLE_VALUE)
{
}

ATL_NOINLINE inline CCrtReportLog::~CCrtReportLog()
{
    if (m_hFile != INVALID_HANDLE_VALUE)
        ATLVERIFY(::CloseHandle(m_hFile));
}

ATL_NOINLINE inline void CCrtReportLog::SetReportLogFile(LPCTSTR pszFileName)
{
    ATLASSERT(pszFileName);
    ATLASSERT(m_hFile == INVALID_HANDLE_VALUE);

    m_hFile = ::CreateFile(pszFileName, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    ATLASSERT(m_hFile != INVALID_HANDLE_VALUE);

    if (::GetLastError() == ERROR_ALREADY_EXISTS)
    {
        ::SetFilePointer(m_hFile, 0, NULL, FILE_END);
    }
    else
    {
        DWORD dwWritten = 0;
        const WORD wBom = 0xFEFF;
        ATLVERIFY(::WriteFile(m_hFile, &wBom, sizeof(WORD), &dwWritten, NULL));
    }
}

ATL_NOINLINE inline void __cdecl CCrtReportLog::operator()(LPCSTR pszTag, LPCSTR pszFileName, int nLine, LPCSTR pszFormat, ...) const
{
    ATLASSERT(pszTag);
    ATLASSERT(pszFormat);
    ATLASSERT(pszFileName);

    va_list _Args;
    va_start(_Args, pszFormat);

    CStringA strMessage;
    strMessage.FormatV(pszFormat, _Args);
    va_end(_Args);

    WriteMessage(ATL::CA2W(pszTag), pszFileName, nLine, ATL::CA2W(strMessage));
}

ATL_NOINLINE inline void __cdecl CCrtReportLog::operator()(LPCWSTR pszTag, LPCSTR pszFileName, int nLine, LPCWSTR pszFormat, ...) const
{
    ATLASSERT(pszTag);
    ATLASSERT(pszFormat);
    ATLASSERT(pszFileName);

    va_list _Args;
    va_start(_Args, pszFormat);

    CStringW strMessage;
    strMessage.FormatV(pszFormat, _Args);
    va_end(_Args);

    WriteMessage(pszTag, pszFileName, nLine, strMessage);
}

ATL_NOINLINE inline void CCrtReportLog::WriteMessage(LPCWSTR pszTag, LPCSTR pszFileName, int nLine, LPCWSTR pszMessage) const
{
    ATLASSERT(pszTag);
    ATLASSERT(pszMessage);
    ATLASSERT(pszFileName);

    SYSTEMTIME st;
    ::GetLocalTime(&st);

    CStringW strMessage;
    strMessage.Format(L"| %04d-%02d-%02d %02d:%02d:%02d.%03d | PID = 0x%08lX | TID = 0x%08lX | %S(%d) | %s | %s", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute,
                      st.wSecond, st.wMilliseconds, ::GetCurrentProcessId(), ::GetCurrentThreadId(), (::strrchr(pszFileName, '\\') + 1), nLine, pszTag, pszMessage);
    ::OutputDebugStringW(strMessage);

    if (m_hFile != INVALID_HANDLE_VALUE)
    {
        DWORD dwWritten = 0;
        strMessage.Replace(L"\n", L"\r\n");
        ATLVERIFY(::WriteFile(m_hFile, (LPCWSTR)strMessage, strMessage.GetLength() * sizeof(WCHAR), &dwWritten, NULL));
    }
}
#endif  // __CRT_REPORT_LOG__

}  // namespace stdutil

#endif  // __CRTDBGUTIL_INL__