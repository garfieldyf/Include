// mainfrm.h : CMainFrame 类的接口
//

#pragma once

// CMainFrame 类的定义

class CMainFrame : public CFrameWnd
{
// 构造
protected: // 仅从序列化创建
    CMainFrame();
    DECLARE_DYNCREATE(CMainFrame)

// 重写
public:
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
    virtual void ActivateFrame(int /*nCmdShow*/ = -1);

// 操作
public:
    virtual ~CMainFrame();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

// 实现
protected:
    BOOL CreateReBar();             // 创建 IE 控件条
    BOOL CreateToolBar();           // 创建工具栏
    BOOL CreateStatusBar();         // 创建状态栏

// 成员
protected:
    BOOL m_bFirstShow;              // 第一次显示

    // 控件条嵌入成员
    CReBar     m_wndReBar;          // IE 控件条
    CToolBar   m_wndToolBar;        // 工具栏
    CStatusBar m_wndStatusBar;      // 状态栏

// 生成的消息映射函数
public:
    afx_msg void OnDestroy();
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    DECLARE_MESSAGE_MAP()
};