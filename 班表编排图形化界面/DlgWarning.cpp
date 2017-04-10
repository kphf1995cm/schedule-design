// DlgWarning.cpp : 实现文件
//

#include "stdafx.h"
#include "班表编排图形化界面.h"
#include "DlgWarning.h"
#include "afxdialogex.h"


// CDlgWarning 对话框

IMPLEMENT_DYNAMIC(CDlgWarning, CDialogEx)

CDlgWarning::CDlgWarning(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgWarning::IDD, pParent)
{

}

CDlgWarning::~CDlgWarning()
{
}

void CDlgWarning::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgWarning, CDialogEx)
END_MESSAGE_MAP()


// CDlgWarning 消息处理程序
CDlgWarning WarningDlg;
