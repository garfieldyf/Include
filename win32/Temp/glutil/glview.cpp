// glview.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "glview.h"

///////////////////////////////////////////////////////////////////////////////
// CGLView ���ʵ��
//

IMPLEMENT_DYNCREATE(CGLView, CView)

BEGIN_MESSAGE_MAP(CGLView, CView)
    ON_WM_SIZE()
    ON_WM_CREATE()
    ON_WM_DESTROY()
    ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CGLView ��Ա����

BOOL CGLView::CreateGLContext(HDC hDC)
{
    ASSERT(hDC);

    PIXELFORMATDESCRIPTOR pixelDesc = 
    {
        sizeof(PIXELFORMATDESCRIPTOR),                                  // �ṹ��С
        1,                                                              // �汾 (����Ϊ 1)
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,     // ֧�ִ��ڡ�֧�� OpenGL��֧��˫����
        PFD_TYPE_RGBA,                                                  // RGBA ���ظ�ʽ
        24,                                                             // 24 λ��ɫ
        0, 0, 0, 0, 0, 0, 0, 0,                                         // ���� R��G��B��A ����ɫλ����ƫ���� (Shift Bit)
        0, 0, 0, 0, 0,                                                  // �޾ۼ����桢���Ծۼ�λ
        32,                                                             // 32 λ Z-���� (��Ȼ���)
        0, 0,                                                           // ��ģ�建�桢�޸�������
        PFD_MAIN_PLANE,                                                 // ����ͼ��
        0,                                                              // ����
        0, 0, 0                                                         // ���Բ�����
    };

    // ѡ��һ���� PIXELFORMATDESCRIPTOR ������ƥ������ظ�ʽ����
    int nPixelFormat = ::ChoosePixelFormat(hDC, &pixelDesc);
    if (nPixelFormat == 0)
    {
        // �������ظ�ʽ����������д PIXELFORMATDESCRIPTOR
        nPixelFormat = 1;
        if (::DescribePixelFormat(hDC, nPixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pixelDesc) == 0)
            return FALSE;
    }

    // �����ظ�ʽ���������� HDC �����ظ�ʽ
    if (!::SetPixelFormat(hDC, nPixelFormat, &pixelDesc))
        return FALSE;

    // ����һ�� OpenGL ����Ⱦ������ (HGLRC)
    HGLRC hGLContext = ::wglCreateContext(hDC);
    if (hGLContext == NULL)
        return FALSE;

    // ���� HDC �� HGLRC��ʹ HGLRC ��Ϊ��ǰ�����̵߳� RC 
    return ::wglMakeCurrent(hDC, hGLContext);
}

// CGLView ���

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

// CGLView ��Ϣ�������

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

    // �����ӿ�
    ::glViewport(0, 0, cx, cy);

    // ����ͶӰ����
    cy = (cy <= 0 ? 1 : cy);
    ::glMatrixMode(GL_PROJECTION);      // ѡ��ͶӰ����
    ::glLoadIdentity();                 // ����ͶӰ����
    ::gluPerspective(45.0, static_cast<GLdouble>(cx) / static_cast<GLdouble>(cy), 0.1, 100.0);

    // ����ģ����ͼ����
    ::glMatrixMode(GL_MODELVIEW);       // ѡ��ģ����ͼ����
    ::glLoadIdentity();                 // ����ģ����ͼ����
}