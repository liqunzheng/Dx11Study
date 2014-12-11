// ��� MFC ʾ��Դ������ʾ���ʹ�� MFC Microsoft Office Fluent �û����� 
// (��Fluent UI��)����ʾ�������ο���
// ���Բ��䡶Microsoft ������ο����� 
// MFC C++ �������渽����ص����ĵ���  
// ���ơ�ʹ�û�ַ� Fluent UI �����������ǵ����ṩ�ġ�  
// ��Ҫ�˽��й� Fluent UI ���ɼƻ�����ϸ��Ϣ�������  
// http://go.microsoft.com/fwlink/?LinkId=238214��
//
// ��Ȩ����(C) Microsoft Corporation
// ��������Ȩ����

// MFCClientView.cpp : CMFCClientView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "MFCClient.h"
#endif

#include "MFCClientDoc.h"
#include "MFCClientView.h"
#include "D3DManager.h"
#include "Camera.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCClientView

IMPLEMENT_DYNCREATE(CMFCClientView, CView)

BEGIN_MESSAGE_MAP(CMFCClientView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_CHAR()
END_MESSAGE_MAP()

// CMFCClientView ����/����

CMFCClientView::CMFCClientView()
{
	// TODO:  �ڴ˴����ӹ������

}

CMFCClientView::~CMFCClientView()
{
}

BOOL CMFCClientView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO:  �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CMFCClientView ����

void CMFCClientView::OnDraw(CDC* /*pDC*/)
{
	CMFCClientDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO:  �ڴ˴�Ϊ�����������ӻ��ƴ���
}

void CMFCClientView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CMFCClientView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CMFCClientView ���

#ifdef _DEBUG
void CMFCClientView::AssertValid() const
{
	CView::AssertValid();
}

void CMFCClientView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFCClientDoc* CMFCClientView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCClientDoc)));
	return (CMFCClientDoc*)m_pDocument;
}
#endif //_DEBUG


// CMFCClientView ��Ϣ��������


void CMFCClientView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// ��ʼ��D3D m_hWnd
	CD3DManager::Instance()->InitEnger(m_hWnd);
}


void CMFCClientView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// ���ڴ�С�ı�ʱ,���³�ʼ������
	CD3DManager::Instance()->InitEnger(m_hWnd);
}


BOOL CMFCClientView::OnEraseBkgnd(CDC* pDC)
{
	return FALSE;
}


void CMFCClientView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO:  �ڴ�������Ϣ������������/�����Ĭ��ֵ
	CView::OnChar(nChar, nRepCnt, nFlags);
	switch (nChar)
	{
	case 'W':case 'w':
	CD3DManager::Instance()->getCamera()->fly(-0.1f);
	break;
	case 'S':case 's':
	CD3DManager::Instance()->getCamera()->fly(0.1f);
	break;
	case 'A':case 'a':
	CD3DManager::Instance()->getCamera()->strafe(0.1f);
	break;
	case 'D':case 'd':
	CD3DManager::Instance()->getCamera()->strafe(-0.1f);
	break;

	default:
	break;
	}

	//if (GetAsyncKeyState('W') & 0x8000)    //����
	//	m_Graphics->getCamera()->fly(-0.1f);
	//if (GetAsyncKeyState('S') & 0x8000)
	//	m_Graphics->getCamera()->fly(0.1f);
	//if (GetAsyncKeyState('A') & 0x8000)    //���� 
	//	m_Graphics->getCamera()->strafe(0.1f);
	//if (GetAsyncKeyState('D') & 0x8000)
	//	m_Graphics->getCamera()->strafe(-0.1f);
	//if (GetAsyncKeyState('Z') & 0x8000)
	//	m_Graphics->getCamera()->pitch(PI / 180);
	//if (GetAsyncKeyState('X') & 0x8000)
	//	m_Graphics->getCamera()->yaw(PI / 180);
	//if (GetAsyncKeyState('C') & 0x8000)
	//	m_Graphics->getCamera()->roll(PI / 180);
	//if (GetAsyncKeyState('R') & 0x8000)
	//	m_Graphics->getCamera()->reCorver();
}