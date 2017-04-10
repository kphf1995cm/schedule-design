
// 班表编排图形化界面View.cpp : C班表编排图形化界面View 类的实现
//


#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "班表编排图形化界面.h"
#endif

#include "班表编排图形化界面Doc.h"
#include "班表编排图形化界面View.h"
#include  "EasyModelDlg.h"
//#include<iostream>
//using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// C班表编排图形化界面View

IMPLEMENT_DYNCREATE(C班表编排图形化界面View, CView)

BEGIN_MESSAGE_MAP(C班表编排图形化界面View, CView)
	ON_COMMAND(ID_EDIT_UNDO, &C班表编排图形化界面View::OnSetModel)
	ON_COMMAND(ID_32775, &C班表编排图形化界面View::OnStartSchedule)
END_MESSAGE_MAP()

// C班表编排图形化界面View 构造/析构

C班表编排图形化界面View::C班表编排图形化界面View()
{
	// TODO:  在此处添加构造代码

}

C班表编排图形化界面View::~C班表编排图形化界面View()
{
}

BOOL C班表编排图形化界面View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO:  在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// C班表编排图形化界面View 绘制

void C班表编排图形化界面View::OnDraw(CDC* /*pDC*/)
{
	C班表编排图形化界面Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	

	// TODO:  在此处为本机数据添加绘制代码
}


// C班表编排图形化界面View 诊断

#ifdef _DEBUG
void C班表编排图形化界面View::AssertValid() const
{
	CView::AssertValid();
}

void C班表编排图形化界面View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

C班表编排图形化界面Doc* C班表编排图形化界面View::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(C班表编排图形化界面Doc)));
	return (C班表编排图形化界面Doc*)m_pDocument;
}
#endif //_DEBUG


// C班表编排图形化界面View 消息处理程序


void C班表编排图形化界面View::OnSetModel()
{
	// TODO:  在此添加命令处理程序代码
	MyScheModel.DoModal();
}


void C班表编排图形化界面View::OnStartSchedule()
{
	// TODO:  在此添加命令处理程序代码
	SimpleModel.DoModal();
}
