#pragma once


// CDlgWarning �Ի���

class CDlgWarning : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgWarning)

public:
	CDlgWarning(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgWarning();

// �Ի�������
	enum { IDD = IDD_DIALOG3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
extern CDlgWarning WarningDlg;