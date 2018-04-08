///////////////////////////////////////////////////////////////////////////////
// stdcollx.h : ͷ�ļ�
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
// CTreeNode ��Ķ���
//

class CTreeNode
{
// ����/����
public:
    CTreeNode(CComVariant* pParent, CComVariant* pValue, CComVariant* pText = NULL, long lImage = -1, long lSelectedImage = -1) throw();
    ~CTreeNode();

// ����
public:
    CComVariant* DetachValue() throw();

// ����
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

// ʵ��
private:
    CTreeNode(const CTreeNode& that);
    CTreeNode& operator=(const CTreeNode& that);

// ��Ա
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
// CTreeNodeList ��Ķ���
//

class CTreeNodeList
{
// ����/����
public:
    CTreeNodeList();
    ~CTreeNodeList();

// ����
public:
    HRESULT DataBind(_Connection* pConnection, BSTR bstrCommandText, const VARIANT& vtParentIndex, const VARIANT& vtValueIndex, const VARIANT* pTextIndex = NULL,
                     const VARIANT* pImageIndex = NULL, const VARIANT* pSelImageIndex = NULL, long lOptions = adCmdText);

    void AddNode(CTreeNode* pNode) throw();
    CTreeNode* DeleteNode(CTreeNode* pDelete);

    template <typename _Predicate>
    CTreeNode* FindNode(CTreeNode* pStart, _Predicate _Pred) const;

// ����
public:
    CTreeNode* GetHead() const throw();

// ʵ��
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

// ��Ա
private:
    CTreeNode* m_pHead;
    CTreeNode* m_pTail;
};

}   // namespace stdutil

#include "stdcollx.inl"

#endif  // __STDCOLLX_H__