///////////////////////////////////////////////////////////////////////////////
// stdcolls.inl : 实现文件
//
// Copyright(c) 2012, Garfield. All rights reserved.
// Author  : Garfield
// Version : 2.0
// Finish Time : 2007/3/22

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __STDCOLLS_INL__
#define __STDCOLLS_INL__

namespace stdutil {

#ifdef _XMEMORY_
///////////////////////////////////////////////////////////////////////////////
// allocator_ptr 类的实现
//

template <typename _Ty>
inline allocator_ptr<_Ty>::allocator_ptr()
{
}

template <typename _Ty>
inline allocator_ptr<_Ty>::allocator_ptr(const allocator_ptr<_Ty>& /*_Right*/)
{
}

template <typename _Ty> template <typename _Other>
inline allocator_ptr<_Ty>::allocator_ptr(const allocator_ptr<_Other>& /*_Right*/)
{
}

template <typename _Ty>
inline void allocator_ptr<_Ty>::destroy_ptr(void* _Pointer)
{
    delete (_Pointer);
}

template <typename _Ty> template <typename _Other>
inline allocator_ptr<_Ty>& allocator_ptr<_Ty>::operator=(const allocator_ptr<_Other>& /*_Right*/)
{
    return (*this);
}
#endif  // _XMEMORY_


///////////////////////////////////////////////////////////////////////////////
// container_ptr 类的实现
//

template <typename _TBase>
inline container_ptr<_TBase>::container_ptr()
{
}

template <typename _TBase>
inline container_ptr<_TBase>::~container_ptr()
{
    _Release(begin(), end());
}

template <typename _TBase>
inline void container_ptr<_TBase>::clear_ptr()
{
    _Release(begin(), end());
    _TBase::clear();
}

template <typename _TBase>
inline typename container_ptr<_TBase>::iterator container_ptr<_TBase>::erase_ptr(iterator _Where)
{
    _Alloc::destroy_ptr(*_Where);
    return _TBase::erase(_Where);
}

template <typename _TBase>
inline typename container_ptr<_TBase>::iterator container_ptr<_TBase>::erase_ptr(iterator _First, iterator _Last)
{
    _Release(_First, _Last);
    return _TBase::erase(_First, _Last);
}

template <typename _TBase>
inline void container_ptr<_TBase>::resize_ptr(size_type _Newsize)
{
    resize_ptr(_Newsize, value_type());
}

template <typename _TBase>
inline void container_ptr<_TBase>::resize_ptr(size_type _Newsize, value_type _Val)
{
    if (_Newsize < size())
    {
        iterator _Where = begin();
        std::advance(_Where, _Newsize);
        _Release(_Where, end());
    }

    _TBase::resize(_Newsize, _Val);
}

#ifdef _CRT_DEBUG_DUMP
template <typename _TBase> template <typename _Pr>
inline void container_ptr<_TBase>::dump(const _Pr& _Printer) const
{
    const_iterator _First = begin(), _Last = end();
    _Printer(_T("\nELEMENT COUNT = %u\n[ "), size());

    if (_First != _Last)
    {
        _Printer(_T("%08lx"), (LPVOID)(*_First));
        for (++_First; _First != _Last; ++_First)
            _Printer(_T(", %08lx"), (LPVOID)(*_First));
    }

    _Printer(_T(" ]\n\n"));
}
#endif  // _CRT_DEBUG_DUMP

template <typename _TBase>
inline void container_ptr<_TBase>::_Release(iterator _First, iterator _Last)
{
    for (; _First != _Last; ++_First)
        _Alloc::destroy_ptr(*_First);
}


///////////////////////////////////////////////////////////////////////////////
// map_ptr 类的实现
//

template <typename _TBase>
inline map_ptr<_TBase>::map_ptr()
{
}

template <typename _TBase>
inline map_ptr<_TBase>::~map_ptr()
{
    _Release(begin(), end());
}

template <typename _TBase>
inline void map_ptr<_TBase>::clear_ptr()
{
    _Release(begin(), end());
    _TBase::clear();
}

template <typename _TBase>
inline typename map_ptr<_TBase>::iterator map_ptr<_TBase>::erase_ptr(iterator _Where)
{
    _Alloc::destroy_ptr((*_Where).second);
    return _TBase::erase(_Where);
}

template <typename _TBase>
inline typename map_ptr<_TBase>::iterator map_ptr<_TBase>::erase_ptr(iterator _First, iterator _Last)
{
    _Release(_First, _Last);
    return _TBase::erase(_First, _Last);
}

template <typename _TBase>
inline typename map_ptr<_TBase>::size_type map_ptr<_TBase>::erase_ptr(const key_type& _Keyval)
{
    _Pairii _Where = equal_range(_Keyval);
    _Release(_Where.first, _Where.second);

    return _TBase::erase(_Keyval);
}

template <typename _TBase>
inline void map_ptr<_TBase>::erase_ptr(const key_type* _First, const key_type* _Last)
{
    for (; _First != _Last; ++_First)
        erase_ptr(*_First);
}

template <typename _TBase>
inline void map_ptr<_TBase>::_Release(iterator _First, iterator _Last)
{
    for (; _First != _Last; ++_First)
        _Alloc::destroy_ptr((*_First).second);
}


#ifdef __ATLCOLL_H__
///////////////////////////////////////////////////////////////////////////////
// CPtrArray 类的实现
//

template <typename _Ty, typename _Traits>
inline CPtrArray<_Ty, _Traits>::CPtrArray()
{
}

template <typename _Ty, typename _Traits>
inline CPtrArray<_Ty, _Traits>::~CPtrArray()
{
    _Release(0, GetCount());
}

template <typename _Ty, typename _Traits>
inline void CPtrArray<_Ty, _Traits>::removeAll()
{
    _Release(0, GetCount());
    _BaseClass::RemoveAll();
}

template <typename _Ty, typename _Traits>
inline void CPtrArray<_Ty, _Traits>::removeAt(size_t nIndex, size_t nCount/* = 1*/)
{
    _Release(nIndex, nCount);
    _BaseClass::RemoveAt(nIndex, nCount);
}

template <typename _Ty, typename _Traits>
inline void CPtrArray<_Ty, _Traits>::setAt(size_t nIndex, INARGTYPE Value)
{
    _Traits::destroy(GetAt(nIndex));
    _BaseClass::SetAt(nIndex, Value);
}

template <typename _Ty, typename _Traits>
inline void CPtrArray<_Ty, _Traits>::setAtGrow(size_t nIndex, INARGTYPE Value)
{
    if (nIndex < GetCount())
        _Traits::destroy(GetAt(nIndex));

    _BaseClass::SetAtGrow(nIndex, Value);
}

#ifdef _CRT_DEBUG_DUMP
template <typename _Ty, typename _Traits> template <typename _Pr>
inline void CPtrArray<_Ty, _Traits>::Dump(const _Pr& _Printer) const
{
    size_t nCount = GetCount();
    _Printer(_T("\nELEMENT COUNT = %u\n[ "), nCount);

    if (nCount > 0)
    {
        _Printer(_T("%08lx"), (LPVOID)GetAt(0));
        for (size_t i = 1; i < nCount; ++i)
            _Printer(_T(", %08lx"), (LPVOID)GetAt(i));
    }

    _Printer(_T(" ]\n"));
}
#endif  // _CRT_DEBUG_DUMP

template <typename _Ty, typename _Traits>
inline void CPtrArray<_Ty, _Traits>::_Release(size_t nIndex, size_t nCount)
{
    ATLASSERT_VALID(this);
    ATLASSERT(nIndex >= 0);
    ATLASSERT(nCount >= 0);
    ATLASSERT(nIndex + nCount <= GetCount());

    for (size_t nLast = nIndex + nCount; nIndex < nLast; ++nIndex)
        _Traits::destroy(GetAt(nIndex));
}


///////////////////////////////////////////////////////////////////////////////
// CPtrMap 类的实现
//

template <typename _Key, typename _Value, typename _KeyTraits, typename _ValTraits>
inline CPtrMap<_Key, _Value, _KeyTraits, _ValTraits>::CPtrMap(UINT nBins/* = 17*/, float fOptimalLoad/* = 0.75f*/, float fLoThreshold/* = 0.25f*/, float fHiThreshold/* = 2.25f*/, UINT nBlockSize/* = 10*/)
    : _BaseClass(nBins, fOptimalLoad, fLoThreshold, fHiThreshold, nBlockSize)
{
}

template <typename _Key, typename _Value, typename _KeyTraits, typename _ValTraits>
inline CPtrMap<_Key, _Value, _KeyTraits, _ValTraits>::~CPtrMap()
{
    _Release();
}

template <typename _Key, typename _Value, typename _KeyTraits, typename _ValTraits>
inline void CPtrMap<_Key, _Value, _KeyTraits, _ValTraits>::removeAll()
{
    _Release();
    _BaseClass::RemoveAll();
}

template <typename _Key, typename _Value, typename _KeyTraits, typename _ValTraits>
inline bool CPtrMap<_Key, _Value, _KeyTraits, _ValTraits>::removeKey(KINARGTYPE key)
{
    if (const CPair* pPair = Lookup(key))
        _ValTraits::destroy(pPair->m_value);

    return _BaseClass::RemoveKey(key);
}

template <typename _Key, typename _Value, typename _KeyTraits, typename _ValTraits>
inline void CPtrMap<_Key, _Value, _KeyTraits, _ValTraits>::removeAtPos(POSITION pos)
{
    ATLASSERT(pos != NULL);

    _ValTraits::destroy(GetValueAt(pos));
    _BaseClass::RemoveAtPos(pos);
}

template <typename _Key, typename _Value, typename _KeyTraits, typename _ValTraits>
inline POSITION CPtrMap<_Key, _Value, _KeyTraits, _ValTraits>::setAt(KINARGTYPE key, VINARGTYPE value)
{
    if (const CPair* pPair = Lookup(key))
        _ValTraits::destroy(pPair->m_value);

    return _BaseClass::SetAt(key, value);
}

template <typename _Key, typename _Value, typename _KeyTraits, typename _ValTraits>
inline void CPtrMap<_Key, _Value, _KeyTraits, _ValTraits>::setValueAt(POSITION pos, VINARGTYPE value)
{
    ATLASSERT(pos != NULL);

    _ValTraits::destroy(GetValueAt(pos));
    _BaseClass::SetValueAt(pos, value);
}

template <typename _Key, typename _Value, typename _KeyTraits, typename _ValTraits>
inline void CPtrMap<_Key, _Value, _KeyTraits, _ValTraits>::_Release()
{
    for (POSITION pos = GetStartPosition(); pos != NULL; )
        _ValTraits::destroy(GetNextValue(pos));
}
#endif  // __ATLCOLL_H__

}  // namespace stdutil

#endif  // __STDCOLLS_INL__