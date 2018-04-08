///////////////////////////////////////////////////////////////////////////////
// stdcolls.h : ͷ�ļ�
//
// Copyright(c) 2012, Garfield. All rights reserved.
// Author  : Garfield
// Version : 2.0
// Finish Time : 2007/3/22
// Summary :
// 1. container_ptr��CPtrArray��Ϊָ������, �ֱ������� stl��ATL ������
//    ���е�Ԫ�ر���Ϊָ�� (Ԫ���� new ����, �������¶��� allocator ��
//    traits ��, ��������) �����Զ������ڴ�, �����ֶ��ͷ�
//
// 2. map_ptr��CPtrMap Ϊָ���ֵ�, �ֱ������� stl��ATL ���ֵ䣬�ֵ��е�
//    ֵ (Value) ����Ϊָ�� (Ԫ���� new ����, �������¶��� allocator ��
//    traits ��, ��������), �ֵ��Զ������ڴ�, �����ֶ��ͷ�
//
// 3. container_ptr �� map_ptr ��������ѡ�� stl ��������Ϊ����
// ��: stdutil::container_ptr<std::vector<int*, stdutil::allocator_ptr<int*> > > vecInt;
//     stdutil::container_ptr<std::deque<int*, stdutil::allocator_ptr<int*> > > deqInt;
//     stdutil::container_ptr<std::list<int*, stdutil::allocator_ptr<int*> > > listInt;
//     ... ...
//     stdutil::map_ptr<std::map<int, int*, std::less<int>, stdutil::allocator_ptr<std::pair<int, int*> > > > mapInt;

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __STDCOLLS_H__
#define __STDCOLLS_H__

///////////////////////////////////////////////////////////////////////////////
// Classes in this file:
//
// allocator_ptr<_Ty>
// CPtrElementTraits<_Ty>
//
// container_ptr<_TBase>
// map_ptr<_TBase>
// CPtrArray<_Ty, _Traits>
// CPtrMap<_Key, _Value, _KeyTraits, _ValTraits>

namespace stdutil {

#ifdef _XMEMORY_
///////////////////////////////////////////////////////////////////////////////
// allocator_ptr ��Ķ���
//

template <typename _Ty>
class allocator_ptr : public std::allocator<_Ty>
{
public:
    template<class _Other>
    struct rebind
    {
        typedef allocator_ptr<_Other> other;
    };

// ����
public:
    allocator_ptr();
    allocator_ptr(const allocator_ptr<_Ty>& _Right);
    template<class _Other> allocator_ptr(const allocator_ptr<_Other>& _Right);

// ����
public:
    static void destroy_ptr(void* _Pointer);
    template<class _Other> allocator_ptr<_Ty>& operator=(const allocator_ptr<_Other>& _Right);
};
#endif  // _XMEMORY_


///////////////////////////////////////////////////////////////////////////////
// container_ptr ��Ķ���
//

template <typename _TBase>
class container_ptr : public _TBase
{
public:
    typedef typename _TBase::iterator iterator;
    typedef typename _TBase::size_type size_type;
    typedef typename _TBase::value_type value_type;
    typedef typename _TBase::allocator_type _Alloc;

// ����/����
public:
    container_ptr();
    ~container_ptr();

// ����
public:
    void clear_ptr();

    iterator erase_ptr(iterator _Where);
    iterator erase_ptr(iterator _First, iterator _Last);

    void resize_ptr(size_type _Newsize);
    void resize_ptr(size_type _Newsize, value_type _Val);

#ifdef _CRT_DEBUG_DUMP
    template <typename _Pr> void dump(const _Pr& _Printer) const;
#endif

// ʵ��
private:
    container_ptr(const container_ptr& _Right);
    container_ptr& operator=(const container_ptr& _Right);

    void _Release(iterator _First, iterator _Last);
    void assign(size_type _Count, const value_type& _Val);
    template <typename _Iter> void assign(_Iter _First, _Iter _Last);
};


///////////////////////////////////////////////////////////////////////////////
// map_ptr ��Ķ���
//

template <typename _TBase>
class map_ptr : public _TBase
{
public:
    typedef typename _TBase::_Pairii _Pairii;
    typedef typename _TBase::iterator iterator;
    typedef typename _TBase::key_type key_type;
    typedef typename _TBase::size_type size_type;
    typedef typename _TBase::allocator_type _Alloc;

// ����/����
public:
    map_ptr();
    ~map_ptr();

// ����
public:
    void clear_ptr();

    iterator erase_ptr(iterator _Where);
    iterator erase_ptr(iterator _First, iterator _Last);
    size_type erase_ptr(const key_type& _Keyval);
    void erase_ptr(const key_type* _First, const key_type* _Last);

// ʵ��
private:
    map_ptr(const map_ptr& _Right);
    map_ptr& operator=(const map_ptr& _Right);

    void _Release(iterator _First, iterator _Last);
};


#ifdef __ATLCOLL_H__
///////////////////////////////////////////////////////////////////////////////
// CPtrElementTraits ��Ķ���
//

template <typename _Ty>
struct CPtrElementTraits : public ATL::CDefaultElementTraits<_Ty>
{
    typedef const _Ty INARGTYPE;
    typedef _Ty& OUTARGTYPE;

    static void destroy(_Ty _Pointer)
    {
        delete (_Pointer);
    }
};


///////////////////////////////////////////////////////////////////////////////
// CPtrArray ��Ķ���
//

template <typename _Ty, typename _Traits = CPtrElementTraits<_Ty> >
class CPtrArray : public ATL::CAtlArray<_Ty, _Traits>
{
public:
    typedef ATL::CAtlArray<_Ty, _Traits> _BaseClass;
    typedef typename _BaseClass::INARGTYPE INARGTYPE;
    typedef typename _BaseClass::OUTARGTYPE OUTARGTYPE;

// ����/����
public:
    CPtrArray();
    ~CPtrArray();

// ����
public:
    void removeAll();
    void removeAt(size_t nIndex, size_t nCount = 1);

    void setAt(size_t nIndex, INARGTYPE Value);
    void setAtGrow(size_t nIndex, INARGTYPE Value);

#ifdef _CRT_DEBUG_DUMP
    template <typename _Pr> void Dump(const _Pr& _Printer) const;
#endif

// ʵ��
private:
    void _Release(size_t nIndex, size_t nCount);

    void Copy(const CPtrArray& _Right);
    size_t Append(const CPtrArray& _Right);
    void InsertArrayAt(size_t nStart, const CPtrArray* pNewArray);
};


///////////////////////////////////////////////////////////////////////////////
// CPtrMap ��Ķ���
//

template <typename _Key, typename _Value, typename _KeyTraits = ATL::CElementTraits<_Key>, typename _ValTraits = CPtrElementTraits<_Value> >
class CPtrMap : public ATL::CAtlMap<_Key, _Value, _KeyTraits, _ValTraits>
{
public:
    typedef ATL::CAtlMap<_Key, _Value, _KeyTraits, _ValTraits> _BaseClass;
    typedef typename _BaseClass::KINARGTYPE KINARGTYPE;
    typedef typename _BaseClass::VINARGTYPE VINARGTYPE;
    typedef typename _BaseClass::KOUTARGTYPE KOUTARGTYPE;
    typedef typename _BaseClass::VOUTARGTYPE VOUTARGTYPE;
    typedef _BaseClass::CPair CPair;

// ����/����
public:
    explicit CPtrMap(UINT nBins = 17, float fOptimalLoad = 0.75f, float fLoThreshold = 0.25f, float fHiThreshold = 2.25f, UINT nBlockSize = 10);
    ~CPtrMap();

// ����
public:
    void removeAll();
    bool removeKey(KINARGTYPE key);
    void removeAtPos(POSITION pos);

    POSITION setAt(KINARGTYPE key, VINARGTYPE value);
    void setValueAt(POSITION pos, VINARGTYPE value);

// ʵ��
private:
    void _Release();
};
#endif  // __ATLCOLL_H__

}  // namespace stdutil

#include "stdcolls.inl"

#endif  // __STDCOLLS_H__