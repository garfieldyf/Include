///////////////////////////////////////////////////////////////////////////////
// xtpext.h : ͷ�ļ�
//
// Copyright(c) 2012, Garfield. All rights reserved.
// Author  : Garfield
// Version : 1.0
// Finish Time : 2007/10/24

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __XTPEXT_H__
#define __XTPEXT_H__

///////////////////////////////////////////////////////////////////////////////
// Classes in this file:
//
// CXTPPropertyGridItemIcon
// CXTPPropertyGridItemFolder
// CXTPPropertyGridItemFile
// CXTPPropertyGridItemRect
// CXTPPropertyGridItemSpin

namespace XTP {


///////////////////////////////////////////////////////////////////////////////
// CXTPPropertyGridItemIcon ��Ķ���
//

class CXTPPropertyGridItemIcon : public CXTPPropertyGridItem
{
// ����/����
public:
    explicit CXTPPropertyGridItemIcon(UINT nID, HICON hIcon = NULL, UINT nDlgFilter = 0);
    explicit CXTPPropertyGridItemIcon(LPCTSTR pszCaption, HICON hIcon = NULL, LPCTSTR pszDlgFilter = NULL);
    virtual ~CXTPPropertyGridItemIcon();

// ����
public:
    HICON GetIcon() const;
    void SetIcon(HICON hIcon);

// ����
public:
    void SetDialogFilter(UINT nID);
    void SetDialogFilter(LPCTSTR pszFilter);

// ��д
protected:
    virtual void OnInplaceButtonDown();
    virtual BOOL OnDrawItemValue(CDC& dc, CRect rcValue);

// ��Ա
protected:
    HICON m_hIcon;
    CString m_strFilter;
};


///////////////////////////////////////////////////////////////////////////////
// CXTPPropertyGridItemFolder ��Ķ���
//

class CXTPPropertyGridItemFolder : public CXTPPropertyGridItem
{
// ����/����
public:
    explicit CXTPPropertyGridItemFolder(UINT nID, LPCTSTR pszValue = NULL, CString* pBindString = NULL, UINT nDlgTitle = 0);
    explicit CXTPPropertyGridItemFolder(LPCTSTR pszCaption, LPCTSTR pszValue = NULL, CString* pBindString = NULL, LPCTSTR pszDlgTitle = NULL);
    virtual ~CXTPPropertyGridItemFolder();

// ����
public:
    void SetDialogTitle(UINT nID);
    void SetDialogTitle(LPCTSTR pszTitle);

// ��д
protected:
    virtual void OnInplaceButtonDown();

// ��Ա
protected:
    CString m_strTitle;
};


///////////////////////////////////////////////////////////////////////////////
// CXTPPropertyGridItemFile ��Ķ���
//

class CXTPPropertyGridItemFile : public CXTPPropertyGridItem
{
// ����/����
public:
    explicit CXTPPropertyGridItemFile(UINT nID, LPCTSTR pszValue = NULL, CString* pBindString = NULL, UINT nDlgFilter = 0);
    explicit CXTPPropertyGridItemFile(LPCTSTR pszCaption, LPCTSTR pszValue = NULL, CString* pBindString = NULL, LPCTSTR pszDlgFilter = NULL);
    virtual ~CXTPPropertyGridItemFile();

// ����
public:
    void SetDialogFilter(UINT nID);
    void SetDialogFilter(LPCTSTR pszFilter);

// ��д
protected:
    virtual void OnInplaceButtonDown();

// ��Ա
protected:
    CString m_strFilter;
};


///////////////////////////////////////////////////////////////////////////////
// CXTPPropertyGridItemRect ��Ķ���
//

class CXTPPropertyGridItemRect : public CXTPPropertyGridItem
{
// ����/����
public:
    explicit CXTPPropertyGridItemRect(UINT nID, LPRECT lpRect = NULL, LPRECT lpBindRect = NULL);
    explicit CXTPPropertyGridItemRect(LPCTSTR pszCaption, LPRECT lpRect = NULL, LPRECT lpBindRect = NULL);
    virtual ~CXTPPropertyGridItemRect();

// ����
public:
    const RECT& GetRect() const;
    void SetRect(const RECT& rect);
    void SetRect(int left, int top, int right, int bottom);

// ����
public:
    void BindToRect(LPRECT lpRect);

// ��д
protected:
    virtual void OnAddChildItem();
    virtual void SetValue(CString strValue);

// ʵ��
protected:
    void UpdateChilds();

    RECT StringToRect(LPCTSTR pszValue);
    CString RectToString(const RECT& rect);

    ///////////////////////////////////////////////////////////////////////////////
    // CXTPPropertyGridItemPad ��Ķ���
    //

    class CXTPPropertyGridItemPad : public CXTPPropertyGridItemNumber
    {
    // ����
    public:
        explicit CXTPPropertyGridItemPad(LPCTSTR pszCaption, long* pBindNumber);

    // ��д
    public:
        virtual void OnValueChanged(CString strValue);
    };

    ///////////////////////////////////////////////////////////////////////////////
    // CXTPPropertyGridItemAll ��Ķ���
    //

    class CXTPPropertyGridItemAll : public CXTPPropertyGridItemNumber
    {
    // ����
    public:
        explicit CXTPPropertyGridItemAll(LPCTSTR pszCaption);

    // ��д
    public:
        virtual void OnValueChanged(CString strValue);
    };

    friend class CXTPPropertyGridItemPad;
    friend class CXTPPropertyGridItemAll;

// ��Ա
protected:
    CRect  m_rcValue;
    LPRECT m_pBindRect;
    CXTPPropertyGridItemAll* m_pItemAll;
    CXTPPropertyGridItemPad* m_pItemLeft;
    CXTPPropertyGridItemPad* m_pItemTop;
    CXTPPropertyGridItemPad* m_pItemRight;
    CXTPPropertyGridItemPad* m_pItemBottom;
};


///////////////////////////////////////////////////////////////////////////////
// CXTPPropertyGridItemSpin ��Ķ���
//

class CXTPPropertyGridItemSpin : public CXTPPropertyGridItemNumber
{
// ����/����
public:
    explicit CXTPPropertyGridItemSpin(UINT nID, long lValue = 0, long* pBindValue = NULL, int nLower = 0, int nUpper = 100);
    explicit CXTPPropertyGridItemSpin(LPCTSTR pszCaption, long lValue = 0, long* pBindValue = NULL, int nLower = 0, int nUpper = 100);
    virtual ~CXTPPropertyGridItemSpin();

// ����
public:
    int GetRange(int& nLower) const;
    void SetRange(int nLower, int nUpper);

// ��д
protected:
    virtual void OnSelect();
    virtual void OnDeselect();
    virtual CRect GetValueRect();
    virtual void SetValue(CString strValue);

// ʵ��
protected:
    ///////////////////////////////////////////////////////////////////////////////
    // CInplaceSpinButton ��Ķ���
    //

    class CInplaceSpinButton : public CSpinButtonCtrl
    {
    // ����
    public:
        CInplaceSpinButton();

    // ��Ա
    protected:
        CXTPPropertyGridItemSpin* m_pItem;
        friend class CXTPPropertyGridItemSpin;

    // ���ɵ���Ϣӳ�亯��
    public:
        afx_msg void OnDelTapos(NMHDR* pNMHDR, LRESULT* pResult);
        DECLARE_MESSAGE_MAP()
    };

// ��Ա
protected:
    int m_nLower, m_nUpper;
    CInplaceSpinButton m_wndSpinButton;
};

}  // namespace XTP

#include "xtpext.inl"

#endif  // __XTPEXT_H__