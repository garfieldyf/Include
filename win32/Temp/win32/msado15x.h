///////////////////////////////////////////////////////////////////////////////////////////////////
// msado15x.h : ͷ�ļ�
//
// Copyright(c) 2012, Garfield. All rights reserved.
// Author  : Garfield
// Version : 1.0
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

///////////////////////////////////////////////////////////////////////////////////////////////////
// interface Connection15
//

    //
    // CUSTOM Wrapper methods for error-handling
    //

    bool IsOpen ( );
    void ExecuteNonQuery (
        BSTR CommandText,
        long Options = adCmdText,
        VARIANT * RecordsAffected = NULL );
    _variant_t ExecuteScalar (
        BSTR CommandText,
        long Options = adCmdText,
        VARIANT * RecordsAffected = NULL );
#ifdef __COMCONV_H__
    template <typename _Ty> _Ty ExecuteScalar (
        BSTR CommandText,
        long Options = adCmdText,
        VARIANT * RecordsAffected = NULL );
#endif  // __COMCONV_H__

//
// interface Connection15 CUSTOM wrapper method implementations
//

inline bool Connection15::IsOpen ( ) {
    long _result = 0;
    HRESULT _hr = get_State(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return (_result == adStateOpen);
}

inline void Connection15::ExecuteNonQuery ( BSTR CommandText, long Options/* = adCmdText*/, VARIANT * RecordsAffected/* = NULL*/ ) {
    HRESULT _hr = raw_Execute(CommandText, RecordsAffected, adExecuteNoRecords | Options, NULL);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline _variant_t Connection15::ExecuteScalar ( BSTR CommandText, long Options/* = adCmdText*/, VARIANT * RecordsAffected/* = NULL*/ ) {
    _RecordsetPtr spRecordset;
    HRESULT _hr = raw_Execute(CommandText, RecordsAffected, Options, &spRecordset);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));

    VARIANT _result;
    ::VariantInit(&_result);

#ifdef _DEBUG
    if (!spRecordset->GetEOF())
        _COM_ASSERT(SUCCEEDED(spRecordset->get_Collect(_variant_t(0L, VT_I4), &_result)));
    _COM_ASSERT(SUCCEEDED(spRecordset->raw_Close()));
#else
    if (!spRecordset->GetEOF())
        spRecordset->get_Collect(_variant_t(0L, VT_I4), &_result);
    spRecordset->raw_Close();
#endif  // _DEBUG

    return _variant_t(_result, false);
}

#ifdef __COMCONV_H__
template <typename _Ty> inline _Ty Connection15::ExecuteScalar ( BSTR CommandText, long Options/* = adCmdText*/, VARIANT * RecordsAffected/* = NULL*/ ) {
    return _com_util::VariantTo<_Ty>(ExecuteScalar(CommandText, Options, RecordsAffected));
}
#endif  // __COMCONV_H__

///////////////////////////////////////////////////////////////////////////////////////////////////
// interface Recordset15
//

//  __declspec(property(get=GetCollect,put=PutCollect))
//  _variant_t Collect[];

    //
    // CUSTOM Wrapper methods for error-handling
    //

    bool IsOpen ( );
#ifdef __COMCONV_H__
    template <typename _Ty> _Ty GetCollect (
        const VARIANT & Index );
#endif  // __COMCONV_H__
    void PutCollect (
        const VARIANT & Index,
        BSTR Value );
#ifdef __ATLSAFE_H__
    void GetCollect (
        const VARIANT & Index,
        ATL::CComSafeArray<BYTE>& Value );
    void PutCollect (
        const VARIANT & Index,
        const ATL::CComSafeArray<BYTE>& Value );
#endif  // __ATLSAFE_H__

//
// interface Recordset15 CUSTOM wrapper method implementations
//

inline bool Recordset15::IsOpen ( ) {
    long _result = 0;
    HRESULT _hr = get_State(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return (_result == adStateOpen);
}

#ifdef __COMCONV_H__
template <typename _Ty> inline _Ty Recordset15::GetCollect ( const VARIANT & Index ) {
    _variant_t _result;
    HRESULT _hr = get_Collect(Index, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _com_util::VariantTo<_Ty>(_result);
}
#endif  // __COMCONV_H__

inline void Recordset15::PutCollect ( const VARIANT & Index, BSTR Value ) {
    VARIANT _Val = { VT_BSTR };
    _Val.bstrVal = Value;
    HRESULT _hr = put_Collect(Index, _Val);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

#ifdef __ATLSAFE_H__
inline void Recordset15::GetCollect ( const VARIANT & Index, ATL::CComSafeArray<BYTE>& Value ) {
    _variant_t _result;
    HRESULT _hr = get_Collect(Index, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));

    _COM_ASSERT(_result.vt == (VT_ARRAY | VT_UI1));
    _hr = Value.Attach(_result.parray);
    if (FAILED(_hr)) _com_issue_error(_hr);
    _result.vt = VT_EMPTY;
}

inline void Recordset15::PutCollect ( const VARIANT & Index, const ATL::CComSafeArray<BYTE>& Value ) {
    _COM_ASSERT(Value.m_psa);
    _COM_ASSERT(Value.GetDimensions() == 1);

    VARIANT _Val = { VT_UI1 | VT_ARRAY };
    _Val.parray  = Value.m_psa;
    HRESULT _hr = put_Collect(Index, _Val);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}
#endif  // __ATLSAFE_H__


///////////////////////////////////////////////////////////////////////////////////////////////////
// interface Command15
//

    //
    // CUSTOM Wrapper methods for error-handling
    //

    void ExecuteNonQuery (
        long Options = adCmdText,
        VARIANT * RecordsAffected = NULL,
        VARIANT * Parameters = NULL );
    _variant_t ExecuteScalar (
        long Options = adCmdText,
        VARIANT * RecordsAffected = NULL,
        VARIANT * Parameters = NULL );
#ifdef __COMCONV_H__
    template <typename _Ty> _Ty ExecuteScalar (
        long Options = adCmdText,
        VARIANT * RecordsAffected = NULL,
        VARIANT * Parameters = NULL );
#endif  // __COMCONV_H__
    void AddParameter (
        struct _Parameter * Value );
    _ParameterPtr CreateParameter (
        BSTR Name,
        BSTR Value,
        enum DataTypeEnum Type = adVarChar,
        enum ParameterDirectionEnum Direction = adParamInput );
#ifdef __ATLSAFE_H__
    _ParameterPtr CreateParameter (
        BSTR Name,
        const ATL::CComSafeArray<BYTE>& Value );
#endif  // __ATLSAFE_H__

//
// interface Command15 CUSTOM wrapper method implementations
//

inline void Command15::ExecuteNonQuery ( long Options/* = adCmdText*/, VARIANT * RecordsAffected/* = NULL*/, VARIANT * Parameters/* = NULL*/ ) {
    HRESULT _hr = raw_Execute(RecordsAffected, Parameters, adExecuteNoRecords | Options, NULL);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline _variant_t Command15::ExecuteScalar ( long Options/* = adCmdText*/, VARIANT * RecordsAffected/* = NULL*/, VARIANT * Parameters/* = NULL*/ ) {
    _RecordsetPtr spRecordset;
    HRESULT _hr = raw_Execute(RecordsAffected, Parameters, Options, &spRecordset);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));

    VARIANT _result;
    ::VariantInit(&_result);

#ifdef _DEBUG
    if (!spRecordset->GetEOF())
        _COM_ASSERT(SUCCEEDED(spRecordset->get_Collect(_variant_t(0L, VT_I4), &_result)));
    _COM_ASSERT(SUCCEEDED(spRecordset->raw_Close()));
#else
    if (!spRecordset->GetEOF())
        spRecordset->get_Collect(_variant_t(0L, VT_I4), &_result);
    spRecordset->raw_Close();
#endif  // _DEBUG

    return _variant_t(_result, false);
}

#ifdef __COMCONV_H__
template <typename _Ty> inline _Ty Command15::ExecuteScalar ( long Options/* = adCmdText*/, VARIANT * RecordsAffected/* = NULL*/, VARIANT * Parameters/* = NULL*/ ) {
    return _com_util::VariantTo<_Ty>(ExecuteScalar(Options, RecordsAffected, Parameters));
}
#endif  // __COMCONV_H__

inline void Command15::AddParameter ( struct _Parameter * Value ) {
    GetParameters()->Append(Value);
}

inline _ParameterPtr Command15::CreateParameter ( BSTR Name, BSTR Value, enum DataTypeEnum Type/* = adVarChar*/, enum ParameterDirectionEnum Direction/* = adParamInput*/ ) {
    VARIANT _Val = { VT_BSTR };
    _Val.bstrVal = Value;
    struct _Parameter * _result = 0;
    HRESULT _hr = raw_CreateParameter(Name, Type, Direction, (ADO_LONGPTR)::SysStringLen(Value), _Val, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _ParameterPtr(_result, false);
}

#ifdef __ATLSAFE_H__
inline _ParameterPtr Command15::CreateParameter ( BSTR Name, const ATL::CComSafeArray<BYTE>& Value ) {
    _COM_ASSERT(Value.m_psa);
    _COM_ASSERT(Value.GetDimensions() == 1);

    VARIANT _Val = { VT_UI1 | VT_ARRAY };
    _Val.parray  = Value.m_psa;
    struct _Parameter * _result = 0;
    HRESULT _hr = raw_CreateParameter(Name, adBinary, adParamInput, (ADO_LONGPTR)Value.GetCount(), _Val, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _ParameterPtr(_result, false);
}
#endif  // __ATLSAFE_H__