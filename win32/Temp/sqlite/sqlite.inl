///////////////////////////////////////////////////////////////////////////////
// sqlite.inl : 实现文件
//
// Copyright(c) 2012, Garfield. All rights reserved.
// Author  : Garfield
// Version : 1.0
// Finish Time : 2009/4/20

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __SQLITE_INL__
#define __SQLITE_INL__

namespace sqlite {

///////////////////////////////////////////////////////////////////////////////
// CConnection 类的实现
//

inline CConnection::CConnection()
    : m_hDatabase(NULL)
{
}

inline CConnection::~CConnection()
{
    Close();
}

inline BOOL CConnection::Open(LPCTSTR pszFileName)
{
    ATLASSERT(m_hDatabase == NULL);
    return (sqlite3_open16(CT2CW(pszFileName, CP_UTF8), &m_hDatabase) == SQLITE_OK);
}

inline void CConnection::Close()
{
    if (m_hDatabase != NULL && sqlite3_close(m_hDatabase) == SQLITE_OK)
        m_hDatabase = NULL;
}

inline CConnection::operator sqlite3*() const
{
    return m_hDatabase;
}

inline BOOL CConnection::BeginTrans()
{
    ATLASSERT(m_hDatabase);

    char* pszErrorMsg = NULL;
    BOOL bSuccessful = (sqlite3_exec(m_hDatabase, "begin transaction;", NULL, NULL, &pszErrorMsg) == SQLITE_OK);
    sqlite3_free(pszErrorMsg);

    return bSuccessful;
}

inline BOOL CConnection::CommitTrans()
{
    ATLASSERT(m_hDatabase);

    char* pszErrorMsg = NULL;
    BOOL bSuccessful = (sqlite3_exec(m_hDatabase, "commit transaction;", NULL, NULL, &pszErrorMsg) == SQLITE_OK);
    sqlite3_free(pszErrorMsg);

    return bSuccessful;
}

inline BOOL CConnection::RollbackTrans()
{
    ATLASSERT(m_hDatabase);

    char* pszErrorMsg = NULL;
    BOOL bSuccessful = (sqlite3_exec(m_hDatabase, "rollback transaction;", NULL, NULL, &pszErrorMsg) == SQLITE_OK);
    sqlite3_free(pszErrorMsg);

    return bSuccessful;
}

inline BOOL CConnection::ExecuteNonQuery(LPCTSTR pszCommandText)
{
    ATLASSERT(m_hDatabase);
    ATLASSERT(pszCommandText);

    char* pszErrorMsg = NULL;
    BOOL bSuccessful = (sqlite3_exec(m_hDatabase, CT2CA(pszCommandText, CP_UTF8), NULL, NULL, &pszErrorMsg) == SQLITE_OK);
    sqlite3_free(pszErrorMsg);

    return bSuccessful;
}

#ifdef __ATLSTR_H__
inline BOOL CConnection::ExecuteScalar(LPCTSTR pszCommandText, CString& strValue)
{
    ATLASSERT(m_hDatabase);
    ATLASSERT(pszCommandText);

    strValue.Empty();
    char* pszErrorMsg = NULL;
    char** pazResult  = NULL;
    int nRows = 0, nCols = 0;

    BOOL bSuccessful = (sqlite3_get_table(m_hDatabase, CT2CA(pszCommandText, CP_UTF8), &pazResult, &nRows, &nCols, &pszErrorMsg) == SQLITE_OK);
    if (bSuccessful)
    {
        if (nCols > 0)
            strValue = CA2CT(pazResult[1], CP_UTF8);

        sqlite3_free_table(pazResult);
    }

    sqlite3_free(pszErrorMsg);
    return bSuccessful;
}
#endif  // __ATLSTR_H__

inline BOOL CConnection::Execute(LPCTSTR pszCommandText, char*** pazResult, int& nRows, int& nCols)
{
    ATLASSERT(m_hDatabase);
    ATLASSERT(pszCommandText);

    char* pszErrorMsg = NULL;
    BOOL bSuccessful = (sqlite3_get_table(m_hDatabase, CT2CA(pszCommandText, CP_UTF8), pazResult, &nRows, &nCols, &pszErrorMsg) == SQLITE_OK);
    sqlite3_free(pszErrorMsg);

    return bSuccessful;
}


///////////////////////////////////////////////////////////////////////////////
// CCommand 类的实现
//

inline CCommand::CCommand()
    : m_hStmt(NULL)
{
}

inline CCommand::~CCommand()
{
    Close();
}

inline BOOL CCommand::Open(sqlite3* m_hDatabase, LPCTSTR pszCommandText)
{
    ATLASSERT(m_hDatabase);
    ATLASSERT(m_hStmt == NULL);

    return (sqlite3_prepare(m_hDatabase, CT2CA(pszCommandText, CP_UTF8), -1, &m_hStmt, NULL) == SQLITE_OK);
}

inline void CCommand::Close()
{
    if (m_hStmt != NULL && sqlite3_finalize(m_hStmt) == SQLITE_OK)
        m_hStmt = NULL;
}

inline int CCommand::Execute()
{
    ATLASSERT(m_hStmt);
    return sqlite3_step(m_hStmt);
}

inline CCommand::operator sqlite3_stmt*() const
{
    return m_hStmt;
}

}  // namespace sqlite

#endif  // __SQLITE_INL__