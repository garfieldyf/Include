// glview.cpp : 实现文件
//

#include "stdafx.h"
#include "glview.h"

///////////////////////////////////////////////////////////////////////////////
// CGLView 类的实现
//

IMPLEMENT_DYNCREATE(CGLView, CView)

BEGIN_MESSAGE_MAP(CGLView, CView)
    ON_WM_SIZE()
    ON_WM_CREATE()
    ON_WM_DESTROY()
    ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CGLView 成员函数

BOOL CGLView::CreateGLContext(HDC hDC)
{
    ASSERT(hDC);

    PIXELFORMATDESCRIPTOR pixelDesc = 
    {
        sizeof(PIXELFORMATDESCRIPTOR),                                  // 结构大小
        1,                                                              // 版本 (必需为 1)
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,     // 支持窗口、支持 OpenGL、支持双缓冲
        PFD_TYPE_RGBA,                                                  // RGBA 像素格式
        24,                                                             // 24 位颜色
        0, 0, 0, 0, 0, 0, 0, 0,                                         // 忽略 R、G、B、A 的颜色位数、偏移量 (Shift Bit)
        0, 0, 0, 0, 0,                                                  // 无聚集缓存、忽略聚集位
        32,                                                             // 32 位 Z-缓存 (深度缓存)
        0, 0,                                                           // 无模板缓存、无辅助缓存
        PFD_MAIN_PLANE,                                                 // 主绘图层
        0,                                                              // 保留
        0, 0, 0                                                         // 忽略层遮罩
    };

    // 选择一个与 PIXELFORMATDESCRIPTOR 参数相匹配的像素格式索引
    int nPixelFormat = ::ChoosePixelFormat(hDC, &pixelDesc);
    if (nPixelFormat == 0)
    {
        // 根据像素格式索引、来填写 PIXELFORMATDESCRIPTOR
        nPixelFormat = 1;
        if (::DescribePixelFormat(hDC, nPixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pixelDesc) == 0)
            return FALSE;
    }

    // 用像素格式索引来设置 HDC 的像素格式
    if (!::SetPixelFormat(hDC, nPixelFormat, &pixelDesc))
        return FALSE;

    // 创建一个 OpenGL 的渲染上下文 (HGLRC)
    HGLRC hGLContext = ::wglCreateContext(hDC);
    if (hGLContext == NULL)
        return FALSE;

    // 关联 HDC 和 HGLRC、使 HGLRC 成为当前调用线程的 RC 
    return ::wglMakeCurrent(hDC, hGLContext);
}

// CGLView 诊断

#ifdef _DEBUG
void CGLView::AssertValid() const
{
    CView::AssertValid();
}

void CGLView::Dump(CDumpContext& dc) const
{
    CView::Dump(dc);
}
#endif //_DEBUG

// CGLView 消息处理程序

int CGLView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CView::OnCreate(lpCreateStruct) == -1)
        return -1;

    CDC* pDC = GetDC();
    ASSERT_VALID(pDC);

    BOOL bSuccessful = CreateGLContext(pDC->GetSafeHdc());
    ReleaseDC(pDC);

    return (bSuccessful ? 0 : -1);
}

void CGLView::OnSize(UINT nType, int cx, int cy)
{
    CView::OnSize(nType, cx, cy);

    // 设置视口
    ::glViewport(0, 0, cx, cy);

    // 设置投影矩阵
    cy = (cy <= 0 ? 1 : cy);
    ::glMatrixMode(GL_PROJECTION);      // 选择投影矩阵
    ::glLoadIdentity();                 // 重置投影矩阵
    ::gluPerspective(45.0, static_cast<GLdouble>(cx) / static_cast<GLdouble>(cy), 0.1, 100.0);

    // 设置模型视图矩阵
    ::glMatrixMode(GL_MODELVIEW);       // 选择模型视图矩阵
    ::glLoadIdentity();                 // 重置模型视图矩阵
}