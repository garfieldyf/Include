// mainfrm.h : CMainFrame ��Ľӿ�
//

#pragma once

// CMainFrame ��Ķ���

class CMainFrame : public CFrameWnd
{
// ����
protected: // �������л�����
    CMainFrame();
    DECLARE_DYNCREATE(CMainFrame)

// ��д
public:
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
    virtual void ActivateFrame(int /*nCmdShow*/ = -1);

// ����
public:
    virtual ~CMainFrame();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

// ʵ��
protected:
    BOOL CreateReBar();             // ���� IE �ؼ���
    BOOL CreateToolBar();           // ����������
    BOOL CreateStatusBar();         // ����״̬��

// ��Ա
protected:
    BOOL m_bFirstShow;              // ��һ����ʾ

    // �ؼ���Ƕ���Ա
    CReBar     m_wndReBar;          // IE �ؼ���
    CToolBar   m_wndToolBar;        // ������
    CStatusBar m_wndStatusBar;      // ״̬��

// ���ɵ���Ϣӳ�亯��
public:
    afx_msg void OnDestroy();
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    DECLARE_MESSAGE_MAP()
};