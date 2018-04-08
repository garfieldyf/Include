///////////////////////////////////////////////////////////////////////////////
// xtpext.h : 头文件
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
// CXTPPropertyGridItemIcon 类的定义
//

class CXTPPropertyGridItemIcon : public CXTPPropertyGridItem
{
// 构造/销毁
public:
    explicit CXTPPropertyGridItemIcon(UINT nID, HICON hIcon = NULL, UINT nDlgFilter = 0);
    explicit CXTPPropertyGridItemIcon(LPCTSTR pszCaption, HICON hIcon = NULL, LPCTSTR pszDlgFilter = NULL);
    virtual ~CXTPPropertyGridItemIcon();

// 属性
public:
    HICON GetIcon() const;
    void SetIcon(HICON hIcon);

// 操作
public:
    void SetDialogFilter(UINT nID);
    void SetDialogFilter(LPCTSTR pszFilter);

// 重写
protected:
    virtual void OnInplaceButtonDown();
    virtual BOOL OnDrawItemValue(CDC& dc, CRect rcValue);

// 成员
protected:
    HICON m_hIcon;
    CString m_strFilter;
};


///////////////////////////////////////////////////////////////////////////////
// CXTPPropertyGridItemFolder 类的定义
//

class CXTPPropertyGridItemFolder : public CXTPPropertyGridItem
{
// 构造/销毁
public:
    explicit CXTPPropertyGridItemFolder(UINT nID, LPCTSTR pszValue = NULL, CString* pBindString = NULL, UINT nDlgTitle = 0);
    explicit CXTPPropertyGridItemFolder(LPCTSTR pszCaption, LPCTSTR pszValue = NULL, CString* pBindString = NULL, LPCTSTR pszDlgTitle = NULL);
    virtual ~CXTPPropertyGridItemFolder();

// 操作
public:
    void SetDialogTitle(UINT nID);
    void SetDialogTitle(LPCTSTR pszTitle);

// 重写
protected:
    virtual void OnInplaceButtonDown();

// 成员
protected:
    CString m_strTitle;
};


///////////////////////////////////////////////////////////////////////////////
// CXTPPropertyGridItemFile 类的定义
//

class CXTPPropertyGridItemFile : public CXTPPropertyGridItem
{
// 构造/销毁
public:
    explicit CXTPPropertyGridItemFile(UINT nID, LPCTSTR pszValue = NULL, CString* pBindString = NULL, UINT nDlgFilter = 0);
    explicit CXTPPropertyGridItemFile(LPCTSTR pszCaption, LPCTSTR pszValue = NULL, CString* pBindString = NULL, LPCTSTR pszDlgFilter = NULL);
    virtual ~CXTPPropertyGridItemFile();

// 操作
public:
    void SetDialogFilter(UINT nID);
    void SetDialogFilter(LPCTSTR pszFilter);

// 重写
protected:
    virtual void OnInplaceButtonDown();

// 成员
protected:
    CString m_strFilter;
};


///////////////////////////////////////////////////////////////////////////////
// CXTPPropertyGridItemRect 类的定义
//

class CXTPPropertyGridItemRect : public CXTPPropertyGridItem
{
// 构造/销毁
public:
    explicit CXTPPropertyGridItemRect(UINT nID, LPRECT lpRect = NULL, LPRECT lpBindRect = NULL);
    explicit CXTPPropertyGridItemRect(LPCTSTR pszCaption, LPRECT lpRect = NULL, LPRECT lpBindRect = NULL);
    virtual ~CXTPPropertyGridItemRect();

// 属性
public:
    const RECT& GetRect() const;
    void SetRect(const RECT& rect);
    void SetRect(int left, int top, int right, int bottom);

// 操作
public:
    void BindToRect(LPRECT lpRect);

// 重写
protected:
    virtual void OnAddChildItem();
    virtual void SetValue(CString strValue);

// 实现
protected:
    void UpdateChilds();

    RECT StringToRect(LPCTSTR pszValue);
    CString RectToString(const RECT& rect);

    ///////////////////////////////////////////////////////////////////////////////
    // CXTPPropertyGridItemPad 类的定义
    //

    class CXTPPropertyGridItemPad : public CXTPPropertyGridItemNumber
    {
    // 构造
    public:
        explicit CXTPPropertyGridItemPad(LPCTSTR pszCaption, long* pBindNumber);

    // 重写
    public:
        virtual void OnValueChanged(CString strValue);
    };

    ///////////////////////////////////////////////////////////////////////////////
    // CXTPPropertyGridItemAll 类的定义
    //

    class CXTPPropertyGridItemAll : public CXTPPropertyGridItemNumber
    {
    // 构造
    public:
        explicit CXTPPropertyGridItemAll(LPCTSTR pszCaption);

    // 重写
    public:
        virtual void OnValueChanged(CString strValue);
    };

    friend class CXTPPropertyGridItemPad;
    friend class CXTPPropertyGridItemAll;

// 成员
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
// CXTPPropertyGridItemSpin 类的定义
//

class CXTPPropertyGridItemSpin : public CXTPPropertyGridItemNumber
{
// 构造/销毁
public:
    explicit CXTPPropertyGridItemSpin(UINT nID, long lValue = 0, long* pBindValue = NULL, int nLower = 0, int nUpper = 100);
    explicit CXTPPropertyGridItemSpin(LPCTSTR pszCaption, long lValue = 0, long* pBindValue = NULL, int nLower = 0, int nUpper = 100);
    virtual ~CXTPPropertyGridItemSpin();

// 操作
public:
    int GetRange(int& nLower) const;
    void SetRange(int nLower, int nUpper);

// 重写
protected:
    virtual void OnSelect();
    virtual void OnDeselect();
    virtual CRect GetValueRect();
    virtual void SetValue(CString strValue);

// 实现
protected:
    ///////////////////////////////////////////////////////////////////////////////
    // CInplaceSpinButton 类的定义
    //

    class CInplaceSpinButton : public CSpinButtonCtrl
    {
    // 构造
    public:
        CInplaceSpinButton();

    // 成员
    protected:
        CXTPPropertyGridItemSpin* m_pItem;
        friend class CXTPPropertyGridItemSpin;

    // 生成的消息映射函数
    public:
        afx_msg void OnDelTapos(NMHDR* pNMHDR, LRESULT* pResult);
        DECLARE_MESSAGE_MAP()
    };

// 成员
protected:
    int m_nLower, m_nUpper;
    CInplaceSpinButton m_wndSpinButton;
};

}  // namespace XTP

#include "xtpext.inl"

#endif  // __XTPEXT_H__