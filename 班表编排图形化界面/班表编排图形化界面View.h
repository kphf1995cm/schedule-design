
// ������ͼ�λ�����View.h : C������ͼ�λ�����View ��Ľӿ�
//

#pragma once


class C������ͼ�λ�����View : public CView
{
protected: // �������л�����
	C������ͼ�λ�����View();
	DECLARE_DYNCREATE(C������ͼ�λ�����View)

// ����
public:
	C������ͼ�λ�����Doc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// ʵ��
public:
	virtual ~C������ͼ�λ�����View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSetModel();
	afx_msg void OnStartSchedule();
};

#ifndef _DEBUG  // ������ͼ�λ�����View.cpp �еĵ��԰汾
inline C������ͼ�λ�����Doc* C������ͼ�λ�����View::GetDocument() const
   { return reinterpret_cast<C������ͼ�λ�����Doc*>(m_pDocument); }
#endif

