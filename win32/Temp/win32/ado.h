///////////////////////////////////////////////////////////////////////////////////////////////////
// ado.h : ͷ�ļ�
//
// Copyright(c) 2012, Garfield. All rights reserved.
// Author  : Garfield
// Version : 4.1.1
// Finish Time : 2007/9/10
// Summary :
// 1. #import "C:\Program Files\Common Files\System\ADO\MSADO15.DLL" rename_namespace("ADODB") auto_rename
//
// 2. Microsoft OLE DB Provider for ODBC
//    ��: "Provider=MSDASQL;DSN=[dsnName];UID=[userName];PWD=[password];"
//
//    ˵��: Provider ָ�� OLE DB Provider for ODBC; DSN ָ������Դ����; UID ָ���û�����;
//          PWD ָ���û�����; URL ָ���� Web �ļ����з������ļ���Ŀ¼�� URL
//
// 3. Microsoft OLE DB Provider for SQL Server
//    ��: "Provider=SQLOLEDB;Data Source=[serverName];Initial Catalog=[db];User ID=[userName];Password=[pwd];"
//
//    ˵��: Provider ָ�� OLE DB Provider for SQL Server; Data Source ������������; Initial Catalog ������
//          �ϵ����ݿ�����; User ID ָ���û���(����SQL Server Authentication); Password ָ���û�����
//
// 4. Microsoft OLE DB Provider for Oracle
//    ��: "Provider=OraOLEDB.Oracle;Data Source=[serverName];User ID=[userName];Password=[pwd];"
//
//    ˵��: Provider ָ�� OLE DB Provider for Oracle; Data Source ������������; User ID ָ���û�����;
//    Password ָ���û�����
//
// 5. Microsoft OLE DB Persistence Provider
//    ��: "Provider=MSPersist"
//
//    ˵��: ���� Recordset ���󱣴浽�ļ���, �������ļ��лָ��� Recordset ����
//
// 6. Oracle ����
//    PLSQLRSet ���ڵ��� PL/SQL �Ĵ�����̻���
//    NDatatype ������� UNICODE �ַ�
//    SPPrmsLOB ������� BLOB/CLOB/LOB �����
//
//    ��: ADODB::_CommandPtr spCmd(__uuidof(ADODB::Command));
//        spCmd->Properties->Item["PLSQLRSet"] = true;
//        spCmd->Parameters->Append(Param1);
//        spCmd->Parameters->Append(Param2);
//        spCmd->CommandText = "{ call Employees.GetEmpRecord(?, ?) }";
//        spCmd->Execute();
//        ...
//        spCmd->Properties->Item["PLSQLRSet"] = false;
//
//    ˵��: ��������ֻ������ Command ����

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __ADO_H__
#define __ADO_H__

#ifndef __CONVERT_H__
#include "convert.h"
#endif

#ifndef __STDUTIL_H__
#include "stdutil.h"
#endif

#ifndef _ADO_NO_THROW_EXCEPTION
#define _CheckConnectionError(hr)               _AdoCheckError((hr), m_spConnection, __uuidof(ADODB::_Connection))
#define _CheckRecordsetError(hr)                _AdoCheckError((hr), m_spRecordset, __uuidof(ADODB::_Recordset))
#define _CheckCommandError(hr)                  _AdoCheckError((hr), m_spCommand, __uuidof(ADODB::_Command))
#define _CheckFieldsError(hr, pUnknown)         _AdoCheckError((hr), (pUnknown), __uuidof(ADODB::Fields))
#define _CheckFieldError(hr, pUnknown)          _AdoCheckError((hr), (pUnknown), __uuidof(ADODB::Field))
#define _CheckPropertiesError(hr, pUnknown)     _AdoCheckError((hr), (pUnknown), __uuidof(ADODB::Properties))
#define _CheckPropertyError(hr, pUnknown)       _AdoCheckError((hr), (pUnknown), __uuidof(ADODB::Property))
#define _CheckParametersError(hr, pUnknown)     _AdoCheckError((hr), (pUnknown), __uuidof(ADODB::Parameters))
#define _CheckError(hr)                         _AdoCheckError((hr))
#else
#define _CheckConnectionError(hr)               (hr)
#define _CheckRecordsetError(hr)                (hr)
#define _CheckCommandError(hr)                  (hr)
#define _CheckFieldsError(hr, pUnknown)         (hr)
#define _CheckFieldError(hr, pUnknown)          (hr)
#define _CheckPropertiesError(hr, pUnknown)     (hr)
#define _CheckPropertyError(hr, pUnknown)       (hr)
#define _CheckParametersError(hr, pUnknown)     (hr)
#define _CheckError(hr)                         (hr)
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////
// Classes in this file:
//
// CConnection
// CRecordset
// CCommand
//
// Global functions in this file:
//
// _AdoCheckError()

namespace ADO {

///////////////////////////////////////////////////////////////////////////////////////////////////
// ȫ�ֺ���
//

#ifndef _ADO_NO_THROW_EXCEPTION
inline HRESULT _AdoCheckError(HRESULT hr, IUnknown* pUnk, const IID& iid)
{
    if (FAILED(hr)) _com_issue_errorex(hr, pUnk, iid);
    return hr;
}

inline HRESULT _AdoCheckError(HRESULT hr)
{
    if (FAILED(hr)) _com_issue_error(hr);
    return hr;
}
#endif  // _ADO_NO_THROW_EXCEPTION


///////////////////////////////////////////////////////////////////////////////////////////////////
// CConnection ��Ķ���
//

class CConnection
{
// ����
public:
    CConnection() throw();
    CConnection(const CConnection& that) throw();
    explicit CConnection(const ADODB::_ConnectionPtr& spConnection) throw();

// ����
public:
    CConnection& operator=(const CConnection& that) throw();
    CConnection& operator=(const ADODB::_ConnectionPtr& spConnection) throw();

    HRESULT Close();
    bool IsOpen() const;
    HRESULT CreateInstance(IUnknown* pUnkOuter = NULL, DWORD dwClsContext = CLSCTX_ALL) throw();

    long BeginTrans();
    HRESULT CommitTrans();
    HRESULT RollbackTrans();

    ADODB::_RecordsetPtr Execute(BSTR bstrCommandText, long lOptions = ADODB::adCmdText, VARIANT* RecordsAffected = NULL);
    HRESULT ExecuteNonQuery(BSTR bstrCommandText, long lOptions = ADODB::adCmdText, VARIANT* RecordsAffected = NULL);
    _variant_t ExecuteScalar(BSTR bstrCommandText, long lOptions = ADODB::adCmdText, VARIANT* RecordsAffected = NULL);
    template <typename _Ty> _Ty ExecuteScalar(BSTR bstrCommandText, long lOptions = ADODB::adCmdText, VARIANT* RecordsAffected = NULL);

    HRESULT Open(BSTR bstrConnectionString = NULL, BSTR bstrUserID = NULL, BSTR bstrPassword = NULL, long lOptions = ADODB::adConnectUnspecified);
    ADODB::_RecordsetPtr OpenSchema(ADODB::SchemaEnum Schema = ADODB::adSchemaTables, const VARIANT& vtRestrictions = vtMissing, const VARIANT& vtSchemaID = vtMissing);

// ����
public:
    long GetState() const;

    ADODB::_ConnectionPtr& GetConnection() throw();
    const ADODB::_ConnectionPtr& GetConnection() const throw();

    long GetCommandTimeout() const;
    HRESULT SetCommandTimeout(long lCommandTimeout);

    long GetConnectionTimeout() const;
    HRESULT SetConnectionTimeout(long lConnectionTimeout);

    _bstr_t GetConnectionString() const;
    HRESULT SetConnectionString(BSTR bstrConnectionString);

    ADODB::CursorLocationEnum GetCursorLocation() const;
    HRESULT SetCursorLocation(ADODB::CursorLocationEnum CursorLocation);

// ��Ա
protected:
    ADODB::_ConnectionPtr m_spConnection;
};


///////////////////////////////////////////////////////////////////////////////////////////////////
// CRecordset ��Ķ���
//

class CRecordset
{
// ����/����
public:
    CRecordset() throw();
    CRecordset(const CRecordset& that) throw();
    explicit CRecordset(const ADODB::_RecordsetPtr& spRecordset) throw();
    ~CRecordset() throw();

// ����
public:
    CRecordset& operator=(const CRecordset& that) throw();
    CRecordset& operator=(const ADODB::_RecordsetPtr& spRecordset) throw();

    HRESULT Close() throw();
    bool IsOpen() const throw();
    HRESULT CreateInstance(IUnknown* pUnkOuter = NULL, DWORD dwClsContext = CLSCTX_ALL) throw();

    HRESULT MoveLast();
    HRESULT MoveNext();
    HRESULT MoveFirst();
    HRESULT MovePrevious();
    HRESULT Move(long lNumRecords, const VARIANT& vtStart = vtMissing);

    HRESULT CancelUpdate();
    HRESULT CancelBatch(ADODB::AffectEnum AffectRecords = ADODB::adAffectAll);
    HRESULT UpdateBatch(ADODB::AffectEnum AffectRecords = ADODB::adAffectAll);
    HRESULT Update(const VARIANT& vtFields = vtMissing, const VARIANT& vtValues = vtMissing);

    VARIANT_BOOL Supports(ADODB::CursorOptionEnum CursorOptions);
    HRESULT Delete(ADODB::AffectEnum AffectRecords = ADODB::adAffectCurrent);
    HRESULT AddNew(const VARIANT& vtFields = vtMissing, const VARIANT& vtValues = vtMissing);
    HRESULT Find(BSTR bstrCriteria, ADODB::SearchDirectionEnum SearchDirection = ADODB::adSearchForward, long lSkipRecords = 0L, const VARIANT& vtStart = vtMissing);

    _variant_t GetFieldValue(const VARIANT& vtIndex);
    HRESULT GetFieldValue(const VARIANT& vtIndex, VARIANT* Value);
    template <typename _Ty> _Ty GetFieldValue(const VARIANT& vtIndex);
    IStreamPtr GetFieldValue(const VARIANT& vtIndex, BOOL bDeleteOnRelease);

    HRESULT SetFieldValue(const VARIANT& vtIndex, BSTR bstrValue);
    HRESULT SetFieldValue(const VARIANT& vtIndex, const VARIANT& vtValue = vtMissing);

#ifdef __ATLSAFE_H__
    HRESULT GetFieldValue(const VARIANT& vtIndex, CComSafeArray<BYTE>& saValue);
    HRESULT SetFieldValue(const VARIANT& vtIndex, LPSTREAM pValue);
    HRESULT SetFieldValue(const VARIANT& vtIndex, const CComSafeArray<BYTE>& saValue);
#endif  // __ATLSAFE_H__

    HRESULT Load(const VARIANT& vtSource);
    HRESULT Save(const VARIANT& vtSource, ADODB::PersistFormatEnum PersistFormat = ADODB::adPersistXML);

    HRESULT Open(const VARIANT& vtActiveConnection, const VARIANT& vtSource, ADODB::CursorTypeEnum CursorType = ADODB::adOpenDynamic,
                 ADODB::LockTypeEnum LockType = ADODB::adLockOptimistic, long lOptions = ADODB::adCmdText);
    HRESULT Open(const ADODB::_ConnectionPtr& spConnection, const VARIANT& vtSource, ADODB::CursorTypeEnum CursorType = ADODB::adOpenDynamic,
                 ADODB::LockTypeEnum LockType = ADODB::adLockOptimistic, long lOptions = ADODB::adCmdText);

// ����
public:
    long GetState() const throw();
    _bstr_t GetFieldName(long lIndex);

    ADODB::_RecordsetPtr& GetRecordset() throw();
    const ADODB::_RecordsetPtr& GetRecordset() const throw();

    VARIANT_BOOL GetBOF() const;
    VARIANT_BOOL GetEOF() const;

    ADODB::FieldsPtr GetFields();
    ADODB::FieldPtr GetField(const VARIANT& vtIndex);

    ADODB::LockTypeEnum GetLockType() const;
    HRESULT SetLockType(ADODB::LockTypeEnum LockType);

    ADODB::CursorLocationEnum GetCursorLocation() const;
    HRESULT SetCursorLocation(ADODB::CursorLocationEnum CursorLocation);

// ��Ա
protected:
    ADODB::_RecordsetPtr m_spRecordset;
};


///////////////////////////////////////////////////////////////////////////////////////////////////
// CCommand ��Ķ���
//

class CCommand
{
// ����
public:
    CCommand() throw();
    CCommand(const CCommand& that) throw();
    explicit CCommand(const ADODB::_CommandPtr& spCommand) throw();

// ����
public:
    CCommand& operator=(const CCommand& that) throw();
    CCommand& operator=(const ADODB::_CommandPtr& spCommand) throw();

    _variant_t GetPropValue(const VARIANT& vtIndex);
    template <typename _Ty> _Ty GetPropValue(const VARIANT& vtIndex);

    HRESULT SetPropValue(const VARIANT& vtIndex, BSTR bstrValue);
    HRESULT SetPropValue(const VARIANT& vtIndex, const VARIANT& vtValue);

    ADODB::_RecordsetPtr Execute(long lOptions = ADODB::adCmdText, VARIANT* RecordsAffected = NULL);
    HRESULT ExecuteNonQuery(long lOptions = ADODB::adCmdText, VARIANT* RecordsAffected = NULL);
    _variant_t ExecuteScalar(long lOptions = ADODB::adCmdText, VARIANT* RecordsAffected = NULL);
    template <typename _Ty> _Ty ExecuteScalar(long lOptions = ADODB::adCmdText, VARIANT* RecordsAffected = NULL);

    HRESULT AddParameter(BSTR bstrName, BSTR bstrValue);
    HRESULT AddParameter(const ADODB::_ParameterPtr& spParameter);
    HRESULT AddParameter(BSTR bstrName, ADODB::DataTypeEnum Type, long lSize, const VARIANT& vtValue = vtMissing);

#ifdef __ATLSAFE_H__
    HRESULT AddParameter(BSTR bstrName, LPSTREAM pValue);
    HRESULT AddParameter(BSTR bstrName, const CComSafeArray<BYTE>& saValue);
#endif  // __ATLSAFE_H__

    HRESULT CreateInstance(IUnknown* pUnkOuter = NULL, DWORD dwClsContext = CLSCTX_ALL) throw();
    ADODB::_ParameterPtr CreateParameter(BSTR bstrName, ADODB::DataTypeEnum Type, long lSize, ADODB::ParameterDirectionEnum Direction = ADODB::adParamOutput, const VARIANT& vtValue = vtMissing);

// ����
public:
    long GetState() const;
    ADODB::ParametersPtr GetParameters();
    ADODB::PropertiesPtr GetProperties();
    ADODB::PropertyPtr GetProperty(const VARIANT& vtIndex);

    ADODB::_CommandPtr& GetCommand() throw();
    const ADODB::_CommandPtr& GetCommand() const throw();

    _bstr_t GetCommandText() const;
    HRESULT SetCommandText(BSTR bstrCommandText);

    long GetCommandTimeout() const;
    HRESULT SetCommandTimeout(long lCommandTimeout);

    ADODB::CommandTypeEnum GetCommandType() const;
    HRESULT SetCommandType(ADODB::CommandTypeEnum CommandType);

    ADODB::_ConnectionPtr GetActiveConnection();
    HRESULT SetActiveConnection(const ADODB::_ConnectionPtr& spConnection);

// ��Ա
protected:
    ADODB::_CommandPtr m_spCommand;
};

} // namespace ADO

#include "ado.inl"

#endif  // __ADO_H__