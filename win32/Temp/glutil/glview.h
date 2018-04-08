///////////////////////////////////////////////////////////////////////////////
// glview.h : 头文件
//
// Copyright(c) 2006, Garfield. All rights reserved.
// Author  : Garfield
// Version : 1.0
// Finish Time : 2007/8/20

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __GLVIEW_H__
#define __GLVIEW_H__

#include "glutil.h"

///////////////////////////////////////////////////////////////////////////////
// CGLView 类的定义
//

class CGLView : public CView
{
    DECLARE_DYNCREATE(CGLView)
// 构造
protected:
    CGLView();

// 重写
public:
    virtual void OnInitialUpdate();
    virtual void OnDraw(CDC* /*pDC*/);
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 操作
public:
    BOOL CreateGLContext(HDC hDC);

// 实现
public:
    virtual ~CGLView();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

// 生成的消息映射函数
public:
    afx_msg void OnDestroy();
    afx_msg BOOL OnEraseBkgnd(CDC* /*pDC*/);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    DECLARE_MESSAGE_MAP()
};

inline CGLView::CGLView()
{
}

inline CGLView::~CGLView()
{
}

inline void CGLView::OnDraw(CDC* /*pDC*/)
{
}

inline BOOL CGLView::OnEraseBkgnd(CDC* /*pDC*/)
{
    return TRUE;
}

inline BOOL CGLView::PreCreateWindow(CREATESTRUCT& cs)
{
    cs.style |= WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
    return CView::PreCreateWindow(cs);
}

inline void CGLView::OnInitialUpdate()
{
    CView::OnInitialUpdate();

    ::glShadeModel(GL_SMOOTH);                              // 启用阴影平滑
    ::glClearColor(0.0f, 0.0f, 0.0f, 0.0f);                 // 清除颜色缓冲区
    ::glClearDepth(1.0);                                    // 清除深度缓冲区
    ::glEnable(GL_DEPTH_TEST);                              // 启用深度测试
    ::glDepthFunc(GL_LEQUAL);                               // 设置深度测试条件
    ::glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);    // 真正精细的透视修正
    //::glEnable(GL_POLYGON_SMOOTH);                        // 启用多变型抗锯齿
    //::glEnable(GL_BLEND);                                 // 起用混合
    //::glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  // 设置混合因子
}

inline void CGLView::OnDestroy()
{
    CView::OnDestroy();

    HGLRC hGLContext = ::wglGetCurrentContext();    // 获得当前的 OpenGL 渲染上下文
    ::wglMakeCurrent(NULL, NULL);                   // 取消 HDC 与 HGLRC 的关联
    ::wglDeleteContext(hGLContext);                 // 删除现行的 OpenGL 渲染上下文
}

#endif  // __GLVIEW_H__