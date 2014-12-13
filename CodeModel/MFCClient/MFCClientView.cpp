// 这段 MFC 示例源代码演示如何使用 MFC Microsoft Office Fluent 用户界面 
// (“Fluent UI”)。该示例仅供参考，
// 用以补充《Microsoft 基础类参考》和 
// MFC C++ 库软件随附的相关电子文档。  
// 复制、使用或分发 Fluent UI 的许可条款是单独提供的。  
// 若要了解有关 Fluent UI 许可计划的详细信息，请访问  
// http://go.microsoft.com/fwlink/?LinkId=238214。
//
// 版权所有(C) Microsoft Corporation
// 保留所有权利。

// MFCClientView.cpp : CMFCClientView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "MFCClient.h"
#endif

#include "MFCClientDoc.h"
#include "MFCClientView.h"
#include "D3DManager.h"
#include "Camera.h"
#include "TraceWin.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//按F4刷新工程
UINT WM_CUSTOM_REFRESH_PROJECT = RegisterWindowMessage(_T("WM_CUSTOM_REFRESH_PROJECT"));

// CMFCClientView

IMPLEMENT_DYNCREATE(CMFCClientView, CView)

BEGIN_MESSAGE_MAP(CMFCClientView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_CHAR()
	ON_WM_MOUSEWHEEL()
	ON_WM_HOTKEY()
END_MESSAGE_MAP()

// CMFCClientView 构造/析构

CMFCClientView::CMFCClientView()
{
	// TODO:  在此处添加构造代码
	m_bEngineCreate = false;
}

CMFCClientView::~CMFCClientView()
{
}

BOOL CMFCClientView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO:  在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CMFCClientView 绘制

void CMFCClientView::OnDraw(CDC* /*pDC*/)
{
	CMFCClientDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO:  在此处为本机数据添加绘制代码
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


// CMFCClientView 诊断

#ifdef _DEBUG
void CMFCClientView::AssertValid() const
{
	CView::AssertValid();
}

void CMFCClientView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFCClientDoc* CMFCClientView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCClientDoc)));
	return (CMFCClientDoc*)m_pDocument;
}
#endif //_DEBUG


// CMFCClientView 消息处理程序


void CMFCClientView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// 初始化D3D m_hWnd
	CD3DManager::Instance()->InitEnger(m_hWnd);
	RegisterHotKey(m_hWnd, WM_CUSTOM_REFRESH_PROJECT, NULL, VK_F4);

	m_bEngineCreate = true;
}


void CMFCClientView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	if (m_bEngineCreate)
	{
		DPrintf("需改变大小\n");
	}
}


BOOL CMFCClientView::OnEraseBkgnd(CDC* pDC)
{
	return FALSE;
}


void CMFCClientView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
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

	//if (GetAsyncKeyState('W') & 0x8000)    //上下
	//	m_Graphics->getCamera()->fly(-0.1f);
	//if (GetAsyncKeyState('S') & 0x8000)
	//	m_Graphics->getCamera()->fly(0.1f);
	//if (GetAsyncKeyState('A') & 0x8000)    //左右 
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


BOOL CMFCClientView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// 移动相机位置
	if (zDelta > 0)
	{
		CD3DManager::Instance()->getCamera()->walk(-0.5f);
	}
	else
	{
		CD3DManager::Instance()->getCamera()->walk(0.5f);
	}

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}


void CMFCClientView::OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if (nHotKeyId == WM_CUSTOM_REFRESH_PROJECT)
	{
		CD3DManager::Instance()->InitEnger(m_hWnd);
	}
	CView::OnHotKey(nHotKeyId, nKey1, nKey2);
}
