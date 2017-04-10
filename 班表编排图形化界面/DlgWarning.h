#pragma once


// CDlgWarning 对话框

class CDlgWarning : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgWarning)

public:
	CDlgWarning(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgWarning();

// 对话框数据
	enum { IDD = IDD_DIALOG3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
extern CDlgWarning WarningDlg;