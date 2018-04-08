///////////////////////////////////////////////////////////////////////////////
// glview.h : ͷ�ļ�
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
// CGLView ��Ķ���
//

class CGLView : public CView
{
    DECLARE_DYNCREATE(CGLView)
// ����
protected:
    CGLView();

// ��д
public:
    virtual void OnInitialUpdate();
    virtual void OnDraw(CDC* /*pDC*/);
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// ����
public:
    BOOL CreateGLContext(HDC hDC);

// ʵ��
public:
    virtual ~CGLView();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

// ���ɵ���Ϣӳ�亯��
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

    ::glShadeModel(GL_SMOOTH);                              // ������Ӱƽ��
    ::glClearColor(0.0f, 0.0f, 0.0f, 0.0f);                 // �����ɫ������
    ::glClearDepth(1.0);                                    // �����Ȼ�����
    ::glEnable(GL_DEPTH_TEST);                              // ������Ȳ���
    ::glDepthFunc(GL_LEQUAL);                               // ������Ȳ�������
    ::glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);    // ������ϸ��͸������
    //::glEnable(GL_POLYGON_SMOOTH);                        // ���ö���Ϳ����
    //::glEnable(GL_BLEND);                                 // ���û��
    //::glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  // ���û������
}

inline void CGLView::OnDestroy()
{
    CView::OnDestroy();

    HGLRC hGLContext = ::wglGetCurrentContext();    // ��õ�ǰ�� OpenGL ��Ⱦ������
    ::wglMakeCurrent(NULL, NULL);                   // ȡ�� HDC �� HGLRC �Ĺ���
    ::wglDeleteContext(hGLContext);                 // ɾ�����е� OpenGL ��Ⱦ������
}

#endif  // __GLVIEW_H__