///////////////////////////////////////////////////////////////////////////////
// stdcollx.h : 头文件
//
// Copyright(c) 2012, Garfield. All rights reserved.
// Author  : Garfield
// Version : 1.5
// Finish Time : 2007/6/15

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef _ADOINT_H_
#include <msado15.h>
#endif

#ifndef __STDDEFX_H__
#include "stddefx.h"
#endif

#ifndef __ATLCOMCLI_H__
#include <atlcomcli.h>
#endif

#ifndef __STDCOLLX_H__
#define __STDCOLLX_H__

///////////////////////////////////////////////////////////////////////////////
// Classes in this file:
//
// CTreeNode
// CTreeNodeList

namespace stdutil {

///////////////////////////////////////////////////////////////////////////////
// CTreeNode 类的定义
//

class CTreeNode
{
// 构造/销毁
public:
    CTreeNode(CComVariant* pParent, CComVariant* pValue, CComVariant* pText = NULL, long lImage = -1, long lSelectedImage = -1) throw();
    ~CTreeNode();

// 操作
public:
    CComVariant* DetachValue() throw();

// 属性
public:
    CComVariant* GetParent() const throw();
    CComVariant* GetValue() const throw();
    CComVariant* GetText() const throw();
    long GetImage() const throw();
    long GetSelectedImage() const throw();

    CTreeNode* GetNext() const throw();
    void SetNext(CTreeNode* pNext) throw();

    CTreeNode* GetPrev() const throw();
    void SetPrev(CTreeNode* pPrev) throw();

// 实现
private:
    CTreeNode(const CTreeNode& that);
    CTreeNode& operator=(const CTreeNode& that);

// 成员
private:
    CComVariant* m_pParent;
    CComVariant* m_pValue;
    CComVariant* m_pText;
    long m_lImage;
    long m_lSelectedImage;
    CTreeNode* m_pNext;
    CTreeNode* m_pPrev;
};


///////////////////////////////////////////////////////////////////////////////
// CTreeNodeList 类的定义
//

class CTreeNodeList
{
// 构造/销毁
public:
    CTreeNodeList();
    ~CTreeNodeList();

// 操作
public:
    HRESULT DataBind(_Connection* pConnection, BSTR bstrCommandText, const VARIANT& vtParentIndex, const VARIANT& vtValueIndex, const VARIANT* pTextIndex = NULL,
                     const VARIANT* pImageIndex = NULL, const VARIANT* pSelImageIndex = NULL, long lOptions = adCmdText);

    void AddNode(CTreeNode* pNode) throw();
    CTreeNode* DeleteNode(CTreeNode* pDelete);

    template <typename _Predicate>
    CTreeNode* FindNode(CTreeNode* pStart, _Predicate _Pred) const;

// 属性
public:
    CTreeNode* GetHead() const throw();

// 实现
public:
    struct CFindParent
    {
        explicit CFindParent(const CComVariant* pVariant) throw();
        bool operator()(const CTreeNode* pNode) const;
        const CComVariant* m_pVariant;
    };

    struct CFindChild
    {
        explicit CFindChild(const CComVariant* pVariant) throw();
        bool operator()(const CTreeNode* pNode) const;
        const CComVariant* m_pVariant;
    };

private:
    CTreeNodeList(const CTreeNodeList& that);
    CTreeNodeList& operator=(const CTreeNodeList& that);

// 成员
private:
    CTreeNode* m_pHead;
    CTreeNode* m_pTail;
};

}   // namespace stdutil

#include "stdcollx.inl"

#endif  // __STDCOLLX_H__