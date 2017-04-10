
// 班表编排图形化界面View.h : C班表编排图形化界面View 类的接口
//

#pragma once


class C班表编排图形化界面View : public CView
{
protected: // 仅从序列化创建
	C班表编排图形化界面View();
	DECLARE_DYNCREATE(C班表编排图形化界面View)

// 特性
public:
	C班表编排图形化界面Doc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// 实现
public:
	virtual ~C班表编排图形化界面View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSetModel();
	afx_msg void OnStartSchedule();
};

#ifndef _DEBUG  // 班表编排图形化界面View.cpp 中的调试版本
inline C班表编排图形化界面Doc* C班表编排图形化界面View::GetDocument() const
   { return reinterpret_cast<C班表编排图形化界面Doc*>(m_pDocument); }
#endif

