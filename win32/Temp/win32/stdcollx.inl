///////////////////////////////////////////////////////////////////////////////
// stdcollx.inl : 实现文件
//
// Copyright(c) 2012, Garfield. All rights reserved.
// Author  : Garfield
// Version : 1.5
// Finish Time : 2007/6/15

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __STDCOLLX_INL__
#define __STDCOLLX_INL__

namespace stdutil {

///////////////////////////////////////////////////////////////////////////////
// CTreeNode 类的实现
//

inline CTreeNode::CTreeNode(CComVariant* pParent, CComVariant* pValue, CComVariant* pText/* = NULL*/, long lImage/* = -1*/, long lSelectedImage/* = -1*/) throw()
    : m_pParent(pParent), m_pValue(pValue), m_pText(pText), m_lImage(lImage), m_lSelectedImage(lSelectedImage), m_pNext(NULL), m_pPrev(NULL)
{
}

inline CTreeNode::~CTreeNode()
{
    delete m_pText;
    delete m_pValue;
    delete m_pParent;
}

inline CComVariant* CTreeNode::GetParent() const throw()
{
    return m_pParent;
}

inline CComVariant* CTreeNode::GetValue() const throw()
{
    return m_pValue;
}

inline CComVariant* CTreeNode::GetText() const throw()
{
    return m_pText;
}

inline long CTreeNode::GetImage() const throw()
{
    return m_lImage;
}

inline long CTreeNode::GetSelectedImage() const throw()
{
    return m_lSelectedImage;
}

inline CTreeNode* CTreeNode::GetNext() const throw()
{
    return m_pNext;
}

inline void CTreeNode::SetNext(CTreeNode* pNext) throw()
{
    m_pNext = pNext;
}

inline CTreeNode* CTreeNode::GetPrev() const throw()
{
    return m_pPrev;
}

inline void CTreeNode::SetPrev(CTreeNode* pPrev) throw()
{
    m_pPrev = pPrev;
}

inline CComVariant* CTreeNode::DetachValue() throw()
{
    CComVariant* pValue = m_pValue;
    m_pValue = NULL;

    return pValue;
}


///////////////////////////////////////////////////////////////////////////////
// CTreeNodeList 类的实现
//

inline CTreeNodeList::CTreeNodeList()
    : m_pHead(new CTreeNode(NULL, NULL, NULL))
{
    ATLASSERT(m_pHead);
    m_pTail = m_pHead;
}

inline CTreeNodeList::~CTreeNodeList()
{
    for (CTreeNode* pNode = m_pHead; pNode != NULL; pNode = m_pHead)
    {
        m_pHead = pNode->GetNext();
        delete pNode;
    }
}

inline CTreeNode* CTreeNodeList::GetHead() const throw()
{
    ATLASSERT(m_pHead);
    return m_pHead->GetNext();
}

inline void CTreeNodeList::AddNode(CTreeNode* pNode) throw()
{
    ATLASSERT(pNode);
    ATLASSERT(m_pTail);

    pNode->SetPrev(m_pTail);
    m_pTail->SetNext(pNode);
    m_pTail = pNode;
}

inline CTreeNode* CTreeNodeList::DeleteNode(CTreeNode* pDelete)
{
    ATLASSERT(pDelete);

    CTreeNode* pPrev = pDelete->GetPrev();
    ATLASSERT(pPrev);

    pPrev->SetNext(pDelete->GetNext());
    if (pDelete->GetNext() != NULL)
        pDelete->GetNext()->SetPrev(pPrev);

    delete pDelete;
    return pPrev;
}

template <typename _Predicate>
inline CTreeNode* CTreeNodeList::FindNode(CTreeNode* pStart, _Predicate _Pred) const
{
    for (; pStart != NULL; pStart = pStart->GetNext())
    {
        if (_Pred(pStart))
            break;
    }

    return pStart;
}

inline HRESULT CTreeNodeList::DataBind(_Connection* pConnection, BSTR bstrCommandText, const VARIANT& vtParentIndex, const VARIANT& vtValueIndex,
                                       const VARIANT* pTextIndex, const VARIANT* pImageIndex, const VARIANT* pSelImageIndex, long lOptions)
{
    ATLASSERT(bstrCommandText);
    ATLASSERT(pConnection != NULL);
    ATLASSERT(vtParentIndex.vt != VT_EMPTY && vtValueIndex.vt != VT_EMPTY);

    CComPtr<_Recordset> spRecordset;
    HRESULT hr = pConnection->Execute(bstrCommandText, NULL, lOptions, &spRecordset);
    if (FAILED(hr))
        return hr;

    VARIANT_BOOL bEOF = VARIANT_TRUE;
    hr = spRecordset->get_EOF(&bEOF);

    while (SUCCEEDED(hr) && !bEOF)
    {
        CComVariant* pParent = new CComVariant();
        ATLASSERT(pParent);
        _COM_VERIFY(spRecordset->get_Collect(vtParentIndex, pParent));

        CComVariant* pValue = new CComVariant();
        ATLASSERT(pValue);
        _COM_VERIFY(spRecordset->get_Collect(vtValueIndex, pValue));

        CComVariant* pText = NULL;
        if (pTextIndex != NULL)
        {
            pText = new CComVariant();
            ATLASSERT(pText);
            _COM_VERIFY(spRecordset->get_Collect(*pTextIndex, pText));
        }

        long lImage = -1;
        if (pImageIndex != NULL)
        {
            CComVariant vtImage;
            _COM_VERIFY(spRecordset->get_Collect(*pImageIndex, &vtImage));
            _COM_VERIFY(vtImage.ChangeType(VT_I4));
            lImage = vtImage.lVal;
        }

        long lSelImage = -1;
        if (pSelImageIndex != NULL)
        {
            CComVariant vtSelImage;
            _COM_VERIFY(spRecordset->get_Collect(*pSelImageIndex, &vtSelImage));
            _COM_VERIFY(vtSelImage.ChangeType(VT_I4));
            lSelImage = vtSelImage.lVal;
        }

        AddNode(new CTreeNode(pParent, pValue, pText, lImage, lSelImage));
        _COM_VERIFY(spRecordset->MoveNext());
        hr = spRecordset->get_EOF(&bEOF);
    }

    _COM_VERIFY(spRecordset->Close());
    return hr;
}


///////////////////////////////////////////////////////////////////////////////
// CFindParent 类的实现
//

inline CTreeNodeList::CFindParent::CFindParent(const CComVariant* pVariant) throw()
    : m_pVariant(pVariant)
{
    ATLASSERT(pVariant);
}

inline bool CTreeNodeList::CFindParent::operator()(const CTreeNode* pNode) const
{
    ATLASSERT(pNode);
    ATLASSERT(pNode->GetValue());

    return (*m_pVariant) == (*(pNode->GetValue()));
}


///////////////////////////////////////////////////////////////////////////////
// CFindChild 类的实现
//

inline CTreeNodeList::CFindChild::CFindChild(const CComVariant* pVariant) throw()
    : m_pVariant(pVariant)
{
    ATLASSERT(pVariant);
}

inline bool CTreeNodeList::CFindChild::operator()(const CTreeNode* pNode) const
{
    ATLASSERT(pNode);
    ATLASSERT(pNode->GetParent());

    return (*m_pVariant) == (*(pNode->GetParent()));
}

}   // namespace stdutil

#endif  // __STDCOLLX_INL__