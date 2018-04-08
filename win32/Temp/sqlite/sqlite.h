///////////////////////////////////////////////////////////////////////////////
// sqlite.h : ͷ�ļ�
//
// Copyright(c) 2012, Garfield. All rights reserved.
// Author  : Garfield
// Version : 1.0
// Finish Time : 2009/4/20

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __SQLITE_H__
#define __SQLITE_H__

#ifndef _SQLITE3_H_
    #error sqlite.h requires sqlite3.h to be included first
#endif

#include <atlconv.h>

///////////////////////////////////////////////////////////////////////////////
// Classes in this file:
//
// CConnection
// CCommand

namespace sqlite {

///////////////////////////////////////////////////////////////////////////////
// CConnection ��Ķ���
//

class CConnection
{
// ����/����
public:
    CConnection();
    ~CConnection();

// ����
public:
    BOOL Open(LPCTSTR pszFileName);
    void Close();
    operator sqlite3*() const;

    BOOL BeginTrans();
    BOOL CommitTrans();
    BOOL RollbackTrans();

    BOOL ExecuteNonQuery(LPCTSTR pszCommandText);
#ifdef __ATLSTR_H__
    BOOL ExecuteScalar(LPCTSTR pszCommandText, CString& strValue);
#endif
    BOOL Execute(LPCTSTR pszCommandText, char*** pazResult, int& nRows, int& nCols);

// ʵ��
private:
    CConnection(const CConnection& that);
    CConnection& operator=(const CConnection& that);

// ��Ա
protected:
    sqlite3* m_hDatabase;
};


///////////////////////////////////////////////////////////////////////////////
// CCommand ��Ķ���
//

class CCommand
{
// ����/����
public:
    CCommand();
    ~CCommand();

// ����
public:
    BOOL Open(sqlite3* hDB, LPCTSTR pszCommandText);
    void Close();

    int Execute();
    operator sqlite3_stmt*() const;

// ʵ��
private:
    CCommand(const CCommand& that);
    CCommand& operator=(const CCommand& that);

// ��Ա
public:
    sqlite3_stmt* m_hStmt;
};

}  // namespace sqlite

#include "sqlite.inl"

#endif  // __SQLITE_H__