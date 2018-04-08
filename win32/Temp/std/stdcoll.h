/////////////////////////////////////////////////////////////////////
// stdcoll.h : 头文件
//
// Copyright(c) 2007, Garfield. All rights reserved.
// Author  : Garfield
// Version : 1.0
// Finish Time : 2007/3/4

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __STDCOLL_H__
#define __STDCOLL_H__

#ifndef _STDEXCEPT_
    #error stdcoll.h requires std - stdexcept to be included first 
#endif

namespace stdcoll {

/////////////////////////////////////////////////////////////////////
// CElementTraitsBase<_Ty> 类的定义
//

template <typename _Ty>
struct CElementTraitsBase
{
    static void Relocate(_Ty* _Dest, const _Ty* _Source, size_t nCount)
    {
        ::MoveMemory(_Dest, _Source, nCount * sizeof(_Ty));
    }
};


/////////////////////////////////////////////////////////////////////
// CElementTraits<_Ty> 类的定义
//

template <typename _Ty>
struct CElementTraits : public CElementTraitsBase<_Ty>
{
    typedef typename const _Ty& const_reference;
    typedef typename _Ty& reference;
};

#ifdef __ATLSTR_H__

template <>
struct CElementTraits<CString> : public CElementTraitsBase<CString>
{
    typedef LPCTSTR const_reference;
    typedef CString& reference;
};

#endif  // __ATLSTR_H__

#define DECLARE_ELEMENT_TRAITS(_Type) \
template <> \
struct CElementTraits<_Type> : public CElementTraitsBase<_Type> \
{ \
    typedef _Type const_reference; \
    typedef _Type& reference; \
};

DECLARE_ELEMENT_TRAITS(char)
DECLARE_ELEMENT_TRAITS(unsigned char)
DECLARE_ELEMENT_TRAITS(bool)
DECLARE_ELEMENT_TRAITS(short)
DECLARE_ELEMENT_TRAITS(unsigned short)
DECLARE_ELEMENT_TRAITS(int)
DECLARE_ELEMENT_TRAITS(unsigned int)
DECLARE_ELEMENT_TRAITS(long)
DECLARE_ELEMENT_TRAITS(unsigned long)
DECLARE_ELEMENT_TRAITS(__int64)
DECLARE_ELEMENT_TRAITS(unsigned __int64)
DECLARE_ELEMENT_TRAITS(float)
DECLARE_ELEMENT_TRAITS(double)
#ifdef _NATIVE_WCHAR_T_DEFINED
DECLARE_ELEMENT_TRAITS(wchar_t)
#endif  // _NATIVE_WCHAR_T_DEFINED


/////////////////////////////////////////////////////////////////////
// CArray 类的定义
//

template <typename _Ty, typename _Traits = CElementTraits<_Ty> >
class CArray
{
public:
    typedef typename _Traits::const_reference const_reference;
    typedef typename _Traits::reference reference;

// 构造/销毁
public:
    CArray() throw();
    ~CArray();

// 操作
public:
    size_t Add(const_reference value);

    const_reference GetAt(size_t nIndex) const;
    reference GetAt(size_t nIndex);

    const_reference operator[](size_t nIndex) const;
    reference operator[](size_t nIndex);

    void SetAt(size_t nIndex, const_reference value);
    void InsertAt(size_t nIndex, const_reference value);

    const _Ty* GetBuffer() const throw();
    _Ty* GetBuffer() throw();

// 属性
public:
    bool IsEmpty() const throw();
    size_t GetCount() const throw();
    bool SetCount(size_t nCount);

// 实现
private:
    CArray(const CArray& that);
    CArray& operator=(const CArray& that);

    bool GrowBuffer(size_t nCount);

    static void Constructors(_Ty* pData, size_t nCount);
    static void Destructors(_Ty* pData, size_t nCount);
    static void CheckIndex(size_t nIndex, size_t nCount);

// 成员
private:
    _Ty*   m_pData;
    size_t m_nCount;
    size_t m_nMaxCount;
};

/////////////////////////////////////////////////////////////////////
// CArray 构造/销毁
//

template <typename _Ty, typename _Traits>
inline CArray<_Ty, _Traits>::CArray() throw()
    : m_pData(NULL), m_nCount(0), m_nMaxCount(0)
{
}

template <typename _Ty, typename _Traits>
inline CArray<_Ty, _Traits>::~CArray()
{
    if (m_pData != NULL)
    {
        Destructors(m_pData, m_nCount);
        ::free(m_pData);
    }
}

/////////////////////////////////////////////////////////////////////
// CArray 属性
//

template <typename _Ty, typename _Traits>
inline bool CArray<_Ty, _Traits>::IsEmpty() const throw()
{
    return (m_pData == NULL);
}

template <typename _Ty, typename _Traits>
inline size_t CArray<_Ty, _Traits>::GetCount() const throw()
{
    return m_nCount;
}

template <typename _Ty, typename _Traits>
inline bool CArray<_Ty, _Traits>::SetCount(size_t nCount)
{
    if (nCount == 0)
    {
        if (m_pData == NULL)
        {
            Destructors(m_pData, m_nCount);
            ::free(m_pData);
            m_pData = NULL;
        }

        m_nCount = m_nMaxCount = 0;
    }
    else if (nCount <= m_nMaxCount)
    {
        if (nCount > m_nCount)
            Constructors(m_pData + m_nCount, nCount - m_nCount);
        else if (nCount < m_nCount)
            Destructors(m_pData + nCount, m_nCount - nCount);

        m_nCount = nCount;
    }
    else
    {
        if (!GrowBuffer(nCount))
            return false;

        ATLASSERT(nCount > m_nCount);
        Constructors(m_pData + m_nCount, nCount - m_nCount);
        m_nCount = nCount;
    }

    return true;
}

/////////////////////////////////////////////////////////////////////
// CArray 操作
//

template <typename _Ty, typename _Traits>
inline size_t CArray<_Ty, _Traits>::Add(const_reference value)
{
    return 0;
}

template <typename _Ty, typename _Traits>
inline typename CArray<_Ty, _Traits>::const_reference CArray<_Ty, _Traits>::GetAt(size_t nIndex) const
{
    _ASSERTE(m_pData != NULL);
    CheckIndex(nIndex, m_nCount);

    return m_pData[nIndex];
}

template <typename _Ty, typename _Traits>
inline typename CArray<_Ty, _Traits>::reference CArray<_Ty, _Traits>::GetAt(size_t nIndex)
{
    _ASSERTE(m_pData != NULL);
    CheckIndex(nIndex, m_nCount);

    return m_pData[nIndex];
}

template <typename _Ty, typename _Traits>
inline typename CArray<_Ty, _Traits>::const_reference CArray<_Ty, _Traits>::operator[](size_t nIndex) const
{
    return GetAt(nIndex);
}

template <typename _Ty, typename _Traits>
inline typename CArray<_Ty, _Traits>::reference CArray<_Ty, _Traits>::operator[](size_t nIndex)
{
    return GetAt(nIndex);
}

template <typename _Ty, typename _Traits>
inline void CArray<_Ty, _Traits>::SetAt(size_t nIndex, const_reference value)
{
    _ASSERTE(m_pData != NULL);
    CheckIndex(nIndex, m_nCount);

    m_pData[nIndex] = value;
}

template <typename _Ty, typename _Traits>
inline void CArray<_Ty, _Traits>::InsertAt(size_t nIndex, const_reference value)
{
    if (nIndex >= m_nCount)
    {
        SetCount(nIndex + 1);
    }
    else
    {
        SetCount(m_nCount + 1);
        _Traits::Relocate(m_pData + (nIndex + 1), m_pData + nIndex, m_nCount - nIndex);
        Constructors(m_pData + nIndex, 1);
    }

    m_pData[nIndex] = value;
}

template <typename _Ty, typename _Traits>
inline const _Ty* CArray<_Ty, _Traits>::GetBuffer() const throw()
{
    return m_pData;
}

template <typename _Ty, typename _Traits>
inline _Ty* CArray<_Ty, _Traits>::GetBuffer() throw()
{
    return m_pData;
}


/////////////////////////////////////////////////////////////////////
// CArray 实现
//

template <typename _Ty, typename _Traits>
inline bool CArray<_Ty, _Traits>::GrowBuffer(size_t nCount)
{
    ATLASSERT(nCount >= 1);

    if (nCount > m_nMaxCount)
    {
        if (m_pData == NULL)
        {
            m_pData = static_cast<_Ty*>(::malloc(nCount * sizeof(_Ty)));
            if (m_pData == NULL)
                return false;

            m_nMaxCount = nCount;
        }
        else
        {
            size_t nGrowBy = m_nCount / 8;
            nGrowBy = (nGrowBy < 4) ? 4 : ((nGrowBy > 1024) ? 1024 : nGrowBy);

            size_t nMaxCount = m_nMaxCount + nGrowBy;
            size_t nAllocSize = nCount < nMaxCount ? nMaxCount : nCount;

            ATLASSERT(nMaxCount >= m_nMaxCount);
            _Ty* pData = static_cast<_Ty*>(::malloc(nAllocSize * sizeof(_Ty)));
            if (pData == NULL)
                return false;

            _Traits::Relocate(pData, m_pData, m_nCount);
            ::free(m_pData);
            m_pData = pData;
            m_nMaxCount = nAllocSize;
        }
    }

    return true;
}

#pragma push_macro("new")
#undef new

template <typename _Ty, typename _Traits>
inline void CArray<_Ty, _Traits>::Constructors(_Ty* pData, size_t nCount)
{
    for (size_t i = 0; i < nCount; ++i)
        ::new(pData + i) _Ty;
}

#pragma pop_macro("new")

template <typename _Ty, typename _Traits>
inline void CArray<_Ty, _Traits>::Destructors(_Ty* pData, size_t nCount)
{
    for (size_t i = 0; i < nCount; ++i)
        pData[i].~_Ty();
}

template <typename _Ty, typename _Traits>
inline void CArray<_Ty, _Traits>::CheckIndex(size_t nIndex, size_t nCount)
{
    _ASSERTE(nIndex < nCount);
    if (nIndex >= nCount)
        throw std::invalid_argument("无效的索引！");
}

} // namespace stdcoll

#endif  // __STDCOLL_H__