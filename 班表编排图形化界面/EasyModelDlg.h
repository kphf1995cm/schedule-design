#pragma once


// CEasyModelDlg 对话框

class CEasyModelDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CEasyModelDlg)

public:
	CEasyModelDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CEasyModelDlg();

	void Start(CString filename)//开始排班
	{
		MyScheModel.TrenferValue(mPersonNum, mUpWorkTime, mDownWorkTime,mScheduleCount,mEvSchePerNum,mEvScheTime,
			mEvScheWorkTime, mOurMeetingTI,  mOurMeetingTimeJ,mOtherMeetingTimeI, mOtherMeetingTimeJ,
			mOtherMeetingPerNeed,  mDiffSchedule, mMustAwaySche1, mMustAwaySche2,mMustAwaySche3, mMustAwaySche4);//先传值
		MyScheModel.StartSchedule(filename);
	}
// 对话框数据
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int mPersonNum;
	int mScheduleCount;
	CString mEvSchePerNum;
	CString mEvScheTime;//每个班次的起始时间
	int mOurMeetingTI;
	int mOurMeetingTimeJ;
	int mOtherMeetingTimeI;
	int mOtherMeetingTimeJ;
	int mOtherMeetingPerNeed;
	float mUpWorkTime;
	float mDownWorkTime;
	int mDiffSchedule;
	int mMustAwaySche1;
	int mMustAwaySche2;
	int mMustAwaySche3;
	int mMustAwaySche4;
	CString mEvScheWorkTime;//每个班次工时
	afx_msg void OnBnClickedA();
	afx_msg void OnBnClickedB();
	afx_msg void OnBnClickedC();
	afx_msg void OnBnClickedShouYin();
};
extern CEasyModelDlg SimpleModel;