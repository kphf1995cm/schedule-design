// DlgHelp.cpp : 实现文件
//

#include "stdafx.h"
#include "班表编排图形化界面.h"
#include "DlgHelp.h"
#include "afxdialogex.h"


// CDlgHelp 对话框

IMPLEMENT_DYNAMIC(CDlgHelp, CDialogEx)

CDlgHelp::CDlgHelp(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgHelp::IDD, pParent)
{

}

CDlgHelp::~CDlgHelp()
{
}

void CDlgHelp::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgHelp, CDialogEx)
END_MESSAGE_MAP()
CDlgHelp HelpDlg;

// CDlgHelp 消息处理程序
