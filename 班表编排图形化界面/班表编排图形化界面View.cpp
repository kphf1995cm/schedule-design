
// ������ͼ�λ�����View.cpp : C������ͼ�λ�����View ���ʵ��
//


#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "������ͼ�λ�����.h"
#endif

#include "������ͼ�λ�����Doc.h"
#include "������ͼ�λ�����View.h"
#include  "EasyModelDlg.h"
//#include<iostream>
//using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// C������ͼ�λ�����View

IMPLEMENT_DYNCREATE(C������ͼ�λ�����View, CView)

BEGIN_MESSAGE_MAP(C������ͼ�λ�����View, CView)
	ON_COMMAND(ID_EDIT_UNDO, &C������ͼ�λ�����View::OnSetModel)
	ON_COMMAND(ID_32775, &C������ͼ�λ�����View::OnStartSchedule)
END_MESSAGE_MAP()

// C������ͼ�λ�����View ����/����

C������ͼ�λ�����View::C������ͼ�λ�����View()
{
	// TODO:  �ڴ˴���ӹ������

}

C������ͼ�λ�����View::~C������ͼ�λ�����View()
{
}

BOOL C������ͼ�λ�����View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO:  �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// C������ͼ�λ�����View ����

void C������ͼ�λ�����View::OnDraw(CDC* /*pDC*/)
{
	C������ͼ�λ�����Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	

	// TODO:  �ڴ˴�Ϊ����������ӻ��ƴ���
}


// C������ͼ�λ�����View ���

#ifdef _DEBUG
void C������ͼ�λ�����View::AssertValid() const
{
	CView::AssertValid();
}

void C������ͼ�λ�����View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

C������ͼ�λ�����Doc* C������ͼ�λ�����View::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(C������ͼ�λ�����Doc)));
	return (C������ͼ�λ�����Doc*)m_pDocument;
}
#endif //_DEBUG


// C������ͼ�λ�����View ��Ϣ�������


void C������ͼ�λ�����View::OnSetModel()
{
	// TODO:  �ڴ���������������
	MyScheModel.DoModal();
}


void C������ͼ�λ�����View::OnStartSchedule()
{
	// TODO:  �ڴ���������������
	SimpleModel.DoModal();
}
