#pragma once


// CEasyModelDlg �Ի���

class CEasyModelDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CEasyModelDlg)

public:
	CEasyModelDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CEasyModelDlg();

	void Start(CString filename)//��ʼ�Ű�
	{
		MyScheModel.TrenferValue(mPersonNum, mUpWorkTime, mDownWorkTime,mScheduleCount,mEvSchePerNum,mEvScheTime,
			mEvScheWorkTime, mOurMeetingTI,  mOurMeetingTimeJ,mOtherMeetingTimeI, mOtherMeetingTimeJ,
			mOtherMeetingPerNeed,  mDiffSchedule, mMustAwaySche1, mMustAwaySche2,mMustAwaySche3, mMustAwaySche4);//�ȴ�ֵ
		MyScheModel.StartSchedule(filename);
	}
// �Ի�������
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	int mPersonNum;
	int mScheduleCount;
	CString mEvSchePerNum;
	CString mEvScheTime;//ÿ����ε���ʼʱ��
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
	CString mEvScheWorkTime;//ÿ����ι�ʱ
	afx_msg void OnBnClickedA();
	afx_msg void OnBnClickedB();
	afx_msg void OnBnClickedC();
	afx_msg void OnBnClickedShouYin();
};
extern CEasyModelDlg SimpleModel;