///////////////////////////////////////////////////////////////////////////////////////////////////
// ado.inl : 实现文件
//
// Copyright(c) 2012, Garfield. All rights reserved.
// Author  : Garfield
// Version : 4.1.1
// Finish Time : 2007/9/10

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __ADO_INL__
#define __ADO_INL__

namespace ADO {

///////////////////////////////////////////////////////////////////////////////////////////////////
// CConnection 类的实现
//

inline CConnection::CConnection() throw()
{
}

inline CConnection::CConnection(const CConnection& that) throw()
    : m_spConnection(that.m_spConnection)
{
}

inline CConnection::CConnection(const ADODB::_ConnectionPtr& spConnection) throw()
    : m_spConnection(spConnection)
{
}

inline CConnection& CConnection::operator=(const CConnection& that) throw()
{
    m_spConnection = that.m_spConnection;
    return *this;
}

inline CConnection& CConnection::operator=(const ADODB::_ConnectionPtr& spConnection) throw()
{
    m_spConnection = spConnection;
    return *this;
}

inline HRESULT CConnection::Close()
{
    return (GetState() != ADODB::adStateClosed) ? _CheckConnectionError(m_spConnection->raw_Close()) : S_OK;
}

inline bool CConnection::IsOpen() const
{
    return GetState() == ADODB::adStateOpen;
}

inline HRESULT CConnection::CreateInstance(IUnknown* pUnkOuter/* = NULL*/, DWORD dwClsContext/* = CLSCTX_ALL*/) throw()
{
    return m_spConnection.CreateInstance(__uuidof(ADODB::Connection), pUnkOuter, dwClsContext);
}

inline long CConnection::BeginTrans()
{
    _COM_ASSERT(m_spConnection != NULL);

    long _result = 0L;
    _CheckConnectionError(m_spConnection->raw_BeginTrans(&_result));
    return _result;
}

inline HRESULT CConnection::CommitTrans()
{
    _COM_ASSERT(m_spConnection != NULL);
    return _CheckConnectionError(m_spConnection->raw_CommitTrans());
}

inline HRESULT CConnection::RollbackTrans()
{
    _COM_ASSERT(m_spConnection != NULL);
    return _CheckConnectionError(m_spConnection->raw_RollbackTrans());
}

inline ADODB::_RecordsetPtr CConnection::Execute(BSTR bstrCommandText, long lOptions/* = ADODB::adCmdText*/, VARIANT* RecordsAffected/* = NULL*/)
{
    _COM_ASSERT(bstrCommandText);
    _COM_ASSERT(m_spConnection != NULL);

    ADODB::_Recordset* _result = NULL;
    _CheckConnectionError(m_spConnection->raw_Execute(bstrCommandText, RecordsAffected, lOptions, &_result));
    return ADODB::_RecordsetPtr(_result, false);
}

inline HRESULT CConnection::ExecuteNonQuery(BSTR bstrCommandText, long lOptions/* = ADODB::adCmdText*/, VARIANT* RecordsAffected/* = NULL*/)
{
    _COM_ASSERT(bstrCommandText);
    _COM_ASSERT(m_spConnection != NULL);

    return _CheckConnectionError(m_spConnection->raw_Execute(bstrCommandText, RecordsAffected, ADODB::adExecuteNoRecords | lOptions, NULL));
}

inline _variant_t CConnection::ExecuteScalar(BSTR bstrCommandText, long lOptions/* = ADODB::adCmdText*/, VARIANT* RecordsAffected/* = NULL*/)
{
    _COM_ASSERT(bstrCommandText);
    _COM_ASSERT(m_spConnection != NULL);

    VARIANT _result;
    ::VariantInit(&_result);
    ADODB::_RecordsetPtr spRecordset;

    if (SUCCEEDED(_CheckConnectionError(m_spConnection->raw_Execute(bstrCommandText, RecordsAffected, lOptions, &spRecordset))))
    {
        if (!spRecordset->GetEOF())
            _COM_VERIFY(spRecordset->get_Collect(_variant_t(0L, VT_I4), &_result));

        _COM_VERIFY(spRecordset->raw_Close());
    }

    return _variant_t(_result, false);
}

template <typename _Ty>
inline _Ty CConnection::ExecuteScalar(BSTR bstrCommandText, long lOptions/* = ADODB::adCmdText*/, VARIANT* RecordsAffected/* = NULL*/)
{
    return stdutil::VariantTo<_Ty>(ExecuteScalar(bstrCommandText, lOptions, RecordsAffected));
}

inline HRESULT CConnection::Open(BSTR bstrConnectionString/* = NULL*/, BSTR bstrUserID/* = NULL*/, BSTR bstrPassword/* = NULL*/, long lOptions/* = ADODB::adConnectUnspecified*/)
{
    _COM_ASSERT(m_spConnection != NULL);
    return _CheckConnectionError(m_spConnection->raw_Open(bstrConnectionString, bstrUserID, bstrPassword, lOptions));
}

inline ADODB::_RecordsetPtr CConnection::OpenSchema(ADODB::SchemaEnum Schema/* = ADODB::adSchemaTables*/, const VARIANT& vtRestrictions/* = vtMissing*/, const VARIANT& vtSchemaID/* = vtMissing*/)
{
    _COM_ASSERT(m_spConnection != NULL);

    ADODB::_Recordset* _result = NULL;
    _CheckConnectionError(m_spConnection->raw_OpenSchema(Schema, vtRestrictions, vtSchemaID, &_result));
    return ADODB::_RecordsetPtr(_result, false);
}

inline long CConnection::GetState() const
{
    _COM_ASSERT(m_spConnection != NULL);

    long _result = 0L;
    _CheckConnectionError(m_spConnection->get_State(&_result));
    return _result;
}

inline ADODB::_ConnectionPtr& CConnection::GetConnection() throw()
{
    return m_spConnection;
}

inline const ADODB::_ConnectionPtr& CConnection::GetConnection() const throw()
{
    return m_spConnection;
}

inline long CConnection::GetCommandTimeout() const
{
    _COM_ASSERT(m_spConnection != NULL);

    long _result = 0L;
    _CheckConnectionError(m_spConnection->get_CommandTimeout(&_result));
    return _result;
}

inline HRESULT CConnection::SetCommandTimeout(long lCommandTimeout)
{
    _COM_ASSERT(m_spConnection != NULL);
    return _CheckConnectionError(m_spConnection->put_CommandTimeout(lCommandTimeout));
}

inline long CConnection::GetConnectionTimeout() const
{
    _COM_ASSERT(m_spConnection != NULL);

    long _result = 0L;
    _CheckConnectionError(m_spConnection->get_ConnectionTimeout(&_result));
    return _result;
}

inline HRESULT CConnection::SetConnectionTimeout(long lConnectionTimeout)
{
    _COM_ASSERT(m_spConnection != NULL);
    return _CheckConnectionError(m_spConnection->put_ConnectionTimeout(lConnectionTimeout));
}

inline _bstr_t CConnection::GetConnectionString() const
{
    _COM_ASSERT(m_spConnection != NULL);

    BSTR _result = NULL;
    _CheckConnectionError(m_spConnection->get_ConnectionString(&_result));
    return _bstr_t(_result, false);
}

inline HRESULT CConnection::SetConnectionString(BSTR bstrConnectionString)
{
    _COM_ASSERT(bstrConnectionString);
    _COM_ASSERT(m_spConnection != NULL);

    return _CheckConnectionError(m_spConnection->put_ConnectionString(bstrConnectionString));
}

inline ADODB::CursorLocationEnum CConnection::GetCursorLocation() const
{
    _COM_ASSERT(m_spConnection != NULL);

    ADODB::CursorLocationEnum _result;
    _CheckConnectionError(m_spConnection->get_CursorLocation(&_result));
    return _result;
}

inline HRESULT CConnection::SetCursorLocation(ADODB::CursorLocationEnum CursorLocation)
{
    _COM_ASSERT(m_spConnection != NULL);
    return _CheckConnectionError(m_spConnection->put_CursorLocation(CursorLocation));
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// CRecordset 类的实现
//

inline CRecordset::CRecordset() throw()
{
}

inline CRecordset::CRecordset(const CRecordset& that) throw()
    : m_spRecordset(that.m_spRecordset)
{
}

inline CRecordset::CRecordset(const ADODB::_RecordsetPtr& spRecordset) throw()
    : m_spRecordset(spRecordset)
{
}

inline CRecordset::~CRecordset() throw()
{
    Close();
}

inline CRecordset& CRecordset::operator=(const CRecordset& that) throw()
{
    m_spRecordset = that.m_spRecordset;
    return *this;
}

inline CRecordset& CRecordset::operator=(const ADODB::_RecordsetPtr& spRecordset) throw()
{
    m_spRecordset = spRecordset;
    return *this;
}

inline HRESULT CRecordset::Close() throw()
{
    return (GetState() != ADODB::adStateClosed) ? m_spRecordset->raw_Close() : S_OK;
}

inline bool CRecordset::IsOpen() const throw()
{
    return GetState() == ADODB::adStateOpen;
}

inline HRESULT CRecordset::CreateInstance(IUnknown* pUnkOuter/* = NULL*/, DWORD dwClsContext/* = CLSCTX_ALL*/) throw()
{
    return m_spRecordset.CreateInstance(__uuidof(ADODB::Recordset), pUnkOuter, dwClsContext);
}

inline HRESULT CRecordset::MoveLast()
{
    _COM_ASSERT(m_spRecordset != NULL);
    return _CheckRecordsetError(m_spRecordset->raw_MoveLast());
}

inline HRESULT CRecordset::MoveNext()
{
    _COM_ASSERT(m_spRecordset != NULL);
    return _CheckRecordsetError(m_spRecordset->raw_MoveNext());
}

inline HRESULT CRecordset::MoveFirst()
{
    _COM_ASSERT(m_spRecordset != NULL);
    return _CheckRecordsetError(m_spRecordset->raw_MoveFirst());
}

inline HRESULT CRecordset::MovePrevious()
{
    _COM_ASSERT(m_spRecordset != NULL);
    return _CheckRecordsetError(m_spRecordset->raw_MovePrevious());
}

inline HRESULT CRecordset::Move(long lNumRecords, const VARIANT& vtStart/* = vtMissing*/)
{
    _COM_ASSERT(m_spRecordset != NULL);
    return _CheckRecordsetError(m_spRecordset->raw_Move(lNumRecords, vtStart));
}

inline HRESULT CRecordset::CancelUpdate()
{
    _COM_ASSERT(m_spRecordset != NULL);
    return _CheckRecordsetError(m_spRecordset->raw_CancelUpdate());
}

inline HRESULT CRecordset::CancelBatch(ADODB::AffectEnum AffectRecords/* = ADODB::adAffectAll*/)
{
    _COM_ASSERT(m_spRecordset != NULL);
    return _CheckRecordsetError(m_spRecordset->raw_CancelBatch(AffectRecords));
}

inline HRESULT CRecordset::UpdateBatch(ADODB::AffectEnum AffectRecords/* = ADODB::adAffectAll*/)
{
    _COM_ASSERT(m_spRecordset != NULL);
    return _CheckRecordsetError(m_spRecordset->raw_UpdateBatch(AffectRecords));
}

inline HRESULT CRecordset::Update(const VARIANT& vtFields/* = vtMissing*/, const VARIANT& vtValues/* = vtMissing*/)
{
    _COM_ASSERT(m_spRecordset != NULL);
    return _CheckRecordsetError(m_spRecordset->raw_Update(vtFields, vtValues));
}

inline VARIANT_BOOL CRecordset::Supports(ADODB::CursorOptionEnum CursorOptions)
{
    _COM_ASSERT(m_spRecordset != NULL);

    VARIANT_BOOL _result = VARIANT_FALSE;
    _CheckRecordsetError(m_spRecordset->raw_Supports(CursorOptions, &_result));
    return _result;
}

inline HRESULT CRecordset::Delete(ADODB::AffectEnum AffectRecords/* = ADODB::adAffectCurrent*/)
{
    _COM_ASSERT(m_spRecordset != NULL);
    return _CheckRecordsetError(m_spRecordset->raw_Delete(AffectRecords));
}

inline HRESULT CRecordset::AddNew(const VARIANT& vtFields/* = vtMissing*/, const VARIANT& vtValues/* = vtMissing*/)
{
    _COM_ASSERT(m_spRecordset != NULL);
    return _CheckRecordsetError(m_spRecordset->raw_AddNew(vtFields, vtValues));
}

inline HRESULT CRecordset::Find(BSTR bstrCriteria, ADODB::SearchDirectionEnum SearchDirection/* = ADODB::adSearchForward*/, long lSkipRecords/* = 0L*/, const VARIANT& vtStart/* = vtMissing*/)
{
    _COM_ASSERT(bstrCriteria);
    _COM_ASSERT(m_spRecordset != NULL);

    return _CheckRecordsetError(m_spRecordset->raw_Find(bstrCriteria, lSkipRecords, SearchDirection, vtStart));
}

inline _variant_t CRecordset::GetFieldValue(const VARIANT& vtIndex)
{
    _COM_ASSERT(m_spRecordset != NULL);

    VARIANT _result;
    ::VariantInit(&_result);
    _CheckRecordsetError(m_spRecordset->get_Collect(vtIndex, &_result));

    return _variant_t(_result, false);
}

inline HRESULT CRecordset::GetFieldValue(const VARIANT& vtIndex, VARIANT* Value)
{
    _COM_ASSERT(m_spRecordset != NULL);
    return _CheckRecordsetError(m_spRecordset->get_Collect(vtIndex, Value));
}

template <typename _Ty>
inline _Ty CRecordset::GetFieldValue(const VARIANT& vtIndex)
{
    return stdutil::VariantTo<_Ty>(GetFieldValue(vtIndex));
}

inline IStreamPtr CRecordset::GetFieldValue(const VARIANT& vtIndex, BOOL bDeleteOnRelease)
{
    _variant_t _Value;
    IStream* _result = NULL;

    if (SUCCEEDED(_CheckRecordsetError(m_spRecordset->get_Collect(vtIndex, &_Value))))
    {
        _COM_ASSERT(_Value.vt == (VT_ARRAY | VT_UI1));

        LPVOID pData = NULL;
        if (SUCCEEDED(::SafeArrayAccessData(_Value.parray, &pData)))
        {
            _COM_VERIFY(stdutil::CreateStream(pData, _Value.parray->rgsabound[0].cElements, &_result, bDeleteOnRelease));
            _COM_VERIFY(::SafeArrayUnaccessData(_Value.parray));
        }
    }

    return IStreamPtr(_result, false);
}

inline HRESULT CRecordset::SetFieldValue(const VARIANT& vtIndex, BSTR bstrValue)
{
    _COM_ASSERT(m_spRecordset != NULL);

    VARIANT _Value = { VT_BSTR };
    _Value.bstrVal = bstrValue;

    return _CheckRecordsetError(m_spRecordset->put_Collect(vtIndex, _Value));
}

inline HRESULT CRecordset::SetFieldValue(const VARIANT& vtIndex, const VARIANT& vtValue/* = vtMissing*/)
{
    _COM_ASSERT(m_spRecordset != NULL);
    return _CheckRecordsetError(m_spRecordset->put_Collect(vtIndex, vtValue));
}

#ifdef __ATLSAFE_H__
inline HRESULT CRecordset::GetFieldValue(const VARIANT& vtIndex, CComSafeArray<BYTE>& saValue)
{
    _COM_ASSERT(m_spRecordset != NULL);

    VARIANT _result;
    ::VariantInit(&_result);
    HRESULT _hr = _CheckRecordsetError(m_spRecordset->get_Collect(vtIndex, &_result));

    if (SUCCEEDED(_hr))
    {
        _COM_ASSERT(_result.vt == (VT_ARRAY | VT_UI1));
        _hr = saValue.Attach(_result.parray);
        if (FAILED(_hr)) ::VariantClear(&_result);
    }

    return _CheckError(_hr);
}

inline HRESULT CRecordset::SetFieldValue(const VARIANT& vtIndex, LPSTREAM pValue)
{
    _COM_ASSERT(pValue);

    CComSafeArray<BYTE> saValue;
    HRESULT _hr = stdutil::CreateSafeArray(pValue, saValue);

    return SUCCEEDED(_CheckError(_hr)) ? SetFieldValue(vtIndex, saValue) : _hr;
}

inline HRESULT CRecordset::SetFieldValue(const VARIANT& vtIndex, const CComSafeArray<BYTE>& saValue)
{
    _COM_ASSERT(saValue.m_psa);
    _COM_ASSERT(m_spRecordset != NULL);
    _COM_ASSERT(saValue.GetDimensions() == 1);

    VARIANT _Value = { VT_UI1 | VT_ARRAY };
    _Value.parray  = saValue.m_psa;

    return _CheckRecordsetError(m_spRecordset->put_Collect(vtIndex, _Value));
}
#endif  // __ATLSAFE_H__

inline HRESULT CRecordset::Load(const VARIANT& vtSource)
{
    _COM_ASSERT(m_spRecordset != NULL);
    return _CheckRecordsetError(m_spRecordset->raw_Open(vtSource, _variant_t(_T("Provider=MSPersist")), ADODB::adOpenForwardOnly, ADODB::adLockOptimistic, ADODB::adCmdFile));
}

inline HRESULT CRecordset::Save(const VARIANT& vtSource, ADODB::PersistFormatEnum PersistFormat/* = ADODB::adPersistXML*/)
{
    _COM_ASSERT(m_spRecordset != NULL);
    return _CheckRecordsetError(m_spRecordset->raw_Save(vtSource, PersistFormat));
}

inline HRESULT CRecordset::Open(const VARIANT& vtActiveConnection, const VARIANT& vtSource, ADODB::CursorTypeEnum CursorType/* = ADODB::adOpenDynamic*/,
                                ADODB::LockTypeEnum LockType/* = ADODB::adLockOptimistic*/, long lOptions/* = ADODB::adCmdText*/)
{
    _COM_ASSERT(m_spRecordset != NULL);
    return _CheckRecordsetError(m_spRecordset->raw_Open(vtSource, vtActiveConnection, CursorType, LockType, lOptions));
}

inline HRESULT CRecordset::Open(const ADODB::_ConnectionPtr& spConnection, const VARIANT& vtSource, ADODB::CursorTypeEnum CursorType/* = ADODB::adOpenDynamic*/,
                                ADODB::LockTypeEnum LockType/* = ADODB::adLockOptimistic*/, long lOptions/* = ADODB::adCmdText*/)
{
    _COM_ASSERT(spConnection != NULL);
    _COM_ASSERT(m_spRecordset != NULL);

    return _CheckRecordsetError(m_spRecordset->raw_Open(vtSource, _variant_t(static_cast<IDispatch*>(spConnection)), CursorType, LockType, lOptions));
}

inline long CRecordset::GetState() const throw()
{
    _COM_ASSERT(m_spRecordset != NULL);

    long _result = 0L;
    m_spRecordset->get_State(&_result);
    return _result;
}

inline _bstr_t CRecordset::GetFieldName(long lIndex)
{
    _COM_ASSERT(lIndex >= 0L);

    BSTR _result = NULL;
    if (ADODB::FieldPtr spField = GetField(_variant_t(lIndex, VT_I4)))
        _CheckFieldError(spField->get_Name(&_result), spField);

    return _bstr_t(_result, false);
}

inline ADODB::_RecordsetPtr& CRecordset::GetRecordset() throw()
{
    return m_spRecordset;
}

inline const ADODB::_RecordsetPtr& CRecordset::GetRecordset() const throw()
{
    return m_spRecordset;
}

inline VARIANT_BOOL CRecordset::GetBOF() const
{
    _COM_ASSERT(m_spRecordset != NULL);

    VARIANT_BOOL _result = VARIANT_FALSE;
    _CheckRecordsetError(m_spRecordset->get_BOF(&_result));
    return _result;
}

inline VARIANT_BOOL CRecordset::GetEOF() const
{
    _COM_ASSERT(m_spRecordset != NULL);

    VARIANT_BOOL _result = VARIANT_FALSE;
    _CheckRecordsetError(m_spRecordset->get_EOF(&_result));
    return _result;
}

inline ADODB::FieldPtr CRecordset::GetField(const VARIANT& vtIndex)
{
    ADODB::Field* _result = NULL;
    if (ADODB::FieldsPtr spFeilds = GetFields())
        _CheckFieldsError(spFeilds->get_Item(vtIndex, &_result), spFeilds);

    return ADODB::FieldPtr(_result, false);
}

inline ADODB::FieldsPtr CRecordset::GetFields()
{
    _COM_ASSERT(m_spRecordset != NULL);

    ADODB::Fields* _result = NULL;
    _CheckRecordsetError(m_spRecordset->get_Fields(&_result));
    return ADODB::FieldsPtr(_result, false);
}

inline ADODB::LockTypeEnum CRecordset::GetLockType() const
{
    _COM_ASSERT(m_spRecordset != NULL);

    ADODB::LockTypeEnum _result;
    _CheckRecordsetError(m_spRecordset->get_LockType(&_result));
    return _result;
}

inline HRESULT CRecordset::SetLockType(ADODB::LockTypeEnum LockType)
{
    _COM_ASSERT(m_spRecordset != NULL);
    return _CheckRecordsetError(m_spRecordset->put_LockType(LockType));
}

inline ADODB::CursorLocationEnum CRecordset::GetCursorLocation() const
{
    _COM_ASSERT(m_spRecordset != NULL);

    ADODB::CursorLocationEnum _result;
    _CheckRecordsetError(m_spRecordset->get_CursorLocation(&_result));
    return _result;
}

inline HRESULT CRecordset::SetCursorLocation(ADODB::CursorLocationEnum CursorLocation)
{
    _COM_ASSERT(m_spRecordset != NULL);
    return _CheckRecordsetError(m_spRecordset->put_CursorLocation(CursorLocation));
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// CCommand 类的实现
//

inline CCommand::CCommand() throw()
{
}

inline CCommand::CCommand(const CCommand& that) throw()
    : m_spCommand(that.m_spCommand)
{
}

inline CCommand::CCommand(const ADODB::_CommandPtr& spCommand) throw()
    : m_spCommand(spCommand)
{
}

inline CCommand& CCommand::operator=(const CCommand& that) throw()
{
    m_spCommand = that.m_spCommand;
    return *this;
}

inline CCommand& CCommand::operator=(const ADODB::_CommandPtr& spCommand) throw()
{
    m_spCommand = spCommand;
    return *this;
}

inline _variant_t CCommand::GetPropValue(const VARIANT& vtIndex)
{
    VARIANT _result;
    ::VariantInit(&_result);

    if (ADODB::PropertyPtr spProperty = GetProperty(vtIndex))
        _CheckPropertyError(spProperty->get_Value(&_result), spProperty);

    return _variant_t(_result, false);
}

template <typename _Ty>
inline _Ty CCommand::GetPropValue(const VARIANT& vtIndex)
{
    return stdutil::VariantTo<_Ty>(GetPropValue(vtIndex));
}

inline HRESULT CCommand::SetPropValue(const VARIANT& vtIndex, BSTR bstrValue)
{
    VARIANT _Value = { VT_BSTR };
    _Value.bstrVal = bstrValue;

    return SetPropValue(vtIndex, _Value);
}

inline HRESULT CCommand::SetPropValue(const VARIANT& vtIndex, const VARIANT& vtValue)
{
    HRESULT _hr = E_FAIL;
    if (ADODB::PropertyPtr spProperty = GetProperty(vtIndex))
        _hr = _CheckPropertyError(spProperty->put_Value(vtValue), spProperty);

    return _hr;
}

inline ADODB::_RecordsetPtr CCommand::Execute(long lOptions/* = ADODB::adCmdText*/, VARIANT* RecordsAffected/* = NULL*/)
{
    _COM_ASSERT(m_spCommand != NULL);
    _COM_ASSERT(GetActiveConnection() != NULL);

    ADODB::_Recordset* _result = NULL;
    _CheckCommandError(m_spCommand->raw_Execute(RecordsAffected, NULL, lOptions, &_result));
    return ADODB::_RecordsetPtr(_result, false);
}

inline HRESULT CCommand::ExecuteNonQuery(long lOptions/* = ADODB::adCmdText*/, VARIANT* RecordsAffected/* = NULL*/)
{
    _COM_ASSERT(m_spCommand != NULL);
    _COM_ASSERT(GetActiveConnection() != NULL);

    return _CheckCommandError(m_spCommand->raw_Execute(RecordsAffected, NULL, ADODB::adExecuteNoRecords | lOptions, NULL));
}

inline _variant_t CCommand::ExecuteScalar(long lOptions/* = ADODB::adCmdText*/, VARIANT* RecordsAffected/* = NULL*/)
{
    _COM_ASSERT(m_spCommand != NULL);
    _COM_ASSERT(GetActiveConnection() != NULL);

    VARIANT _result;
    ::VariantInit(&_result);
    ADODB::_RecordsetPtr spRecordset;

    if (SUCCEEDED(_CheckCommandError(m_spCommand->raw_Execute(RecordsAffected, NULL, lOptions, &spRecordset))))
    {
        if (!spRecordset->GetEOF())
            _COM_VERIFY(spRecordset->get_Collect(_variant_t(0L, VT_I4), &_result));

        _COM_VERIFY(spRecordset->raw_Close());
    }

    return _variant_t(_result, false);
}

template <typename _Ty>
inline _Ty CCommand::ExecuteScalar(long lOptions/* = ADODB::adCmdText*/, VARIANT* RecordsAffected/* = NULL*/)
{
    return stdutil::VariantTo<_Ty>(ExecuteScalar(lOptions, RecordsAffected));
}

inline HRESULT CCommand::AddParameter(BSTR bstrName, BSTR bstrValue)
{
    _COM_ASSERT(bstrValue);

    VARIANT _Value = { VT_BSTR };
    _Value.bstrVal = bstrValue;

    return AddParameter(bstrName, ADODB::adVarWChar, ::SysStringLen(bstrValue), _Value);
}

inline HRESULT CCommand::AddParameter(const ADODB::_ParameterPtr& spParameter)
{
    _COM_ASSERT(spParameter != NULL);
    _COM_ASSERT(m_spCommand != NULL);

    HRESULT _hr = E_FAIL;
    if (ADODB::ParametersPtr spParameters = GetParameters())
        _hr = _CheckParametersError(spParameters->raw_Append(static_cast<IDispatch*>(spParameter)), spParameters);

    return _hr;
}

inline HRESULT CCommand::AddParameter(BSTR bstrName, ADODB::DataTypeEnum Type, long lSize, const VARIANT& vtValue/* = vtMissing*/)
{
    _COM_ASSERT(bstrName);
    _COM_ASSERT(m_spCommand != NULL);

    ADODB::_ParameterPtr _result = CreateParameter(bstrName, Type, lSize, ADODB::adParamInput, vtValue);
    return (_result != NULL) ? AddParameter(_result) : E_FAIL;
}

#ifdef __ATLSAFE_H__
inline HRESULT CCommand::AddParameter(BSTR bstrName, LPSTREAM pValue)
{
    _COM_ASSERT(pValue);

    CComSafeArray<BYTE> saValue;
    HRESULT _hr = stdutil::CreateSafeArray(pValue, saValue);

    return SUCCEEDED(_CheckError(_hr)) ? AddParameter(bstrName, saValue) : _hr;
}

inline HRESULT CCommand::AddParameter(BSTR bstrName, const CComSafeArray<BYTE>& saValue)
{
    _COM_ASSERT(saValue.m_psa);
    _COM_ASSERT(saValue.GetDimensions() == 1);

    VARIANT _Value = { VT_UI1 | VT_ARRAY };
    _Value.parray  = saValue.m_psa;

    return AddParameter(bstrName, ADODB::adBinary, saValue.GetCount(), _Value);
}
#endif  // __ATLSAFE_H__

inline HRESULT CCommand::CreateInstance(IUnknown* pUnkOuter/* = NULL*/, DWORD dwClsContext/* = CLSCTX_ALL*/) throw()
{
    return m_spCommand.CreateInstance(__uuidof(ADODB::Command), pUnkOuter, dwClsContext);
}

inline ADODB::_ParameterPtr CCommand::CreateParameter(BSTR bstrName, ADODB::DataTypeEnum Type, long lSize, ADODB::ParameterDirectionEnum Direction/* = ADODB::adParamOutput*/, const VARIANT& vtValue/* = vtMissing*/)
{
    _COM_ASSERT(bstrName);
    _COM_ASSERT(m_spCommand != NULL);

    ADODB::_Parameter* _result = NULL;
    _CheckCommandError(m_spCommand->raw_CreateParameter(bstrName, Type, Direction, lSize, vtValue, &_result));
    return ADODB::_ParameterPtr(_result, false);
}

inline long CCommand::GetState() const
{
    _COM_ASSERT(m_spCommand != NULL);

    long _result = 0L;
    _CheckCommandError(m_spCommand->get_State(&_result));
    return _result;
}

inline ADODB::ParametersPtr CCommand::GetParameters()
{
    _COM_ASSERT(m_spCommand != NULL);

    ADODB::Parameters* _result = NULL;
    _CheckCommandError(m_spCommand->get_Parameters(&_result));
    return ADODB::ParametersPtr(_result, false);
}

inline ADODB::PropertiesPtr CCommand::GetProperties()
{
    _COM_ASSERT(m_spCommand != NULL);

    ADODB::Properties* _result = NULL;
    _CheckCommandError(m_spCommand->get_Properties(&_result));
    return ADODB::PropertiesPtr(_result, false);
}

inline ADODB::PropertyPtr CCommand::GetProperty(const VARIANT& vtIndex)
{
    ADODB::Property* _result = NULL;
    if (ADODB::PropertiesPtr spProperties = GetProperties())
        _CheckPropertiesError(spProperties->get_Item(vtIndex, &_result), spProperties);

    return ADODB::PropertyPtr(_result, false);
}

inline ADODB::_CommandPtr& CCommand::GetCommand() throw()
{
    return m_spCommand;
}

inline const ADODB::_CommandPtr& CCommand::GetCommand() const throw()
{
    return m_spCommand;
}

inline _bstr_t CCommand::GetCommandText() const
{
    _COM_ASSERT(m_spCommand != NULL);

    BSTR _result = NULL;
    _CheckCommandError(m_spCommand->get_CommandText(&_result));
    return _bstr_t(_result, false);
}

inline HRESULT CCommand::SetCommandText(BSTR bstrCommandText)
{
    _COM_ASSERT(bstrCommandText);
    _COM_ASSERT(m_spCommand != NULL);

    return _CheckCommandError(m_spCommand->put_CommandText(bstrCommandText));
}

inline long CCommand::GetCommandTimeout() const
{
    _COM_ASSERT(m_spCommand != NULL);

    long _result = 0L;
    _CheckCommandError(m_spCommand->get_CommandTimeout(&_result));
    return _result;
}

inline HRESULT CCommand::SetCommandTimeout(long lCommandTimeout)
{
    _COM_ASSERT(m_spCommand != NULL);
    return _CheckCommandError(m_spCommand->put_CommandTimeout(lCommandTimeout));
}

inline ADODB::CommandTypeEnum CCommand::GetCommandType() const
{
    _COM_ASSERT(m_spCommand != NULL);

    ADODB::CommandTypeEnum _result;
    _CheckCommandError(m_spCommand->get_CommandType(&_result));
    return _result;
}

inline HRESULT CCommand::SetCommandType(ADODB::CommandTypeEnum CommandType)
{
    _COM_ASSERT(m_spCommand != NULL);
    return _CheckCommandError(m_spCommand->put_CommandType(CommandType));
}

inline ADODB::_ConnectionPtr CCommand::GetActiveConnection()
{
    _COM_ASSERT(m_spCommand != NULL);

    ADODB::_Connection* _result = NULL;
    _CheckCommandError(m_spCommand->get_ActiveConnection(&_result));
    return ADODB::_ConnectionPtr(_result, false);
}

inline HRESULT CCommand::SetActiveConnection(const ADODB::_ConnectionPtr& spConnection)
{
    _COM_ASSERT(m_spCommand != NULL);
    _COM_ASSERT(spConnection != NULL);

    return _CheckCommandError(m_spCommand->putref_ActiveConnection(spConnection));
}

} // namespace ADO

#endif  // __ADO_INL__