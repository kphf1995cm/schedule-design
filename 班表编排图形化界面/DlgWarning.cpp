// DlgWarning.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "������ͼ�λ�����.h"
#include "DlgWarning.h"
#include "afxdialogex.h"


// CDlgWarning �Ի���

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


// CDlgWarning ��Ϣ�������
CDlgWarning WarningDlg;
