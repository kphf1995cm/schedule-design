
// 班表编排图形化界面.h : 班表编排图形化界面 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"       // 主符号


// C班表编排图形化界面App:
// 有关此类的实现，请参阅 班表编排图形化界面.cpp
//

class C班表编排图形化界面App : public CWinApp
{
public:
	C班表编排图形化界面App();


// 重写
public:
	virtual BOOL InitInstance();

// 实现
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	bool ChangeCStringToIntNum()
	{
		int count = 0;
		SchePersonNeed = new int[mScheduleCount];
		for (int i = 0; i < mEvSchePerNum.GetLength(); i++)
		{
			if (count >= mScheduleCount)
				return false;
			if (mEvSchePerNum[i] == ' ')
				continue;
			SchePersonNeed[count] = mEvSchePerNum[i] - '0';
			count++;
		}
		if (count == mScheduleCount)
			return true;
		else
			return false;
	}

	bool ChangeCStringToFloatTime()
	{
		int count = 0;
		int point_flag = 0;
		float num = 0;
		EvScheWorkTime = new float[mScheduleCount];
		for (int i = 0; i < mEvScheWorkTime.GetLength(); i++)
		{
			if (count >= mScheduleCount)
				return false;
			switch (mEvScheWorkTime[i])
			{
			case ' ':{  break; }
			case '.':point_flag++; break;
			default:{
						if (point_flag == 0)
						{
							num = num + mEvScheWorkTime[i] - '0';
						}
						else
						{
							if (point_flag == 1)
							{
								num = num + float((mEvScheWorkTime[i] - '0')) / 10.0;
								point_flag++;
							}
							else
							{
								if (point_flag == 2)
								{
									num = num + float((mEvScheWorkTime[i] - '0')) / 100.0;
									point_flag++;
								}
								else
									return false;
							}
						}
						if (i == mEvScheWorkTime.GetLength() - 1 || mEvScheWorkTime[i + 1] == ' ')
						{ 
							EvScheWorkTime[count] = num;
						    num = 0;
						    point_flag = 0;
							count++;
						}
						break;
			}
			}
		}
		if (count == mScheduleCount)
			return true;
		else
			return false;
	}

	bool ChangeCStringToStringTime()
	{
		int count = 0;
		int j=0;
		EvScheTime = new char*[mScheduleCount];
		for (int i = 0; i < mScheduleCount; i++)
		{
			EvScheTime[i] = new char[12];//强制限定时间格式为12位（如：10:00-12:30|），后加一个|
		}
		for (int i = 0; i < mEvScheTime.GetLength(); i++)
		{
			if (count >= mScheduleCount)
				return false;
			switch (mEvScheTime[i])
			{
			case ' ':break;
			default:{
						EvScheTime[count][j] = mEvScheTime[i];
						if (i == mEvScheTime.GetLength() - 1 || mEvScheTime[i + 1] == ' ')
						{
							EvScheTime[count][j + 1] = '|';
							count++;
							j = 0;
						}
						else
							j++;
			}
			}
		}
		if (count == mScheduleCount)
			return true;
		else
			return false;
	}

	void StartSchedule(CString filename);//启动排班

	void TrenferValue(int _PersonNum, float _UpTime, float _DownTime, int _ScheduleCount,CString _EvSchePerNum,CString _EvScheTime,
		CString _EvScheWorkTime,int _OurMeetingTI,int _OurMeetingTimeJ,int _OtherMeetingTimeI,int _OtherMeetingTimeJ,
		int _OtherMeetingPerNeed,int _DiffSchedule,int _MustAwaySche1,int _MustAwaySche2,int _MustAwaySche3,int _MustAwaySche4)
	{
		mPersonNum = _PersonNum;
		mUpWorkTime = _UpTime;
		mDownWorkTime = _DownTime;
		mScheduleCount=_ScheduleCount;
		mEvSchePerNum=_EvSchePerNum;
		mEvScheTime=_EvScheTime;//每个班次的起始时间
		mEvScheWorkTime=_EvScheWorkTime;//每个班次工时
		mOurMeetingTI=_OurMeetingTI;
		mOurMeetingTimeJ=_OurMeetingTimeJ;
		mOtherMeetingTimeI=_OtherMeetingTimeI;
		mOtherMeetingTimeJ=_OtherMeetingTimeJ;
		mOtherMeetingPerNeed=_OtherMeetingPerNeed;
		mDiffSchedule=_DiffSchedule;
		mMustAwaySche1=_MustAwaySche1;
		mMustAwaySche2=_MustAwaySche2;
		mMustAwaySche3=_MustAwaySche3;
		mMustAwaySche4=_MustAwaySche4;
	}

	//void MySetModel(fstream & outfile)
	//{
		//char ch;
		//save mEvSchePerNum
		//for (int i = 0; i < mEvSchePerNum.GetLength(); i++)
		//{
		//	outfile.put(mEvSchePerNum[i]);
		//}
		//outfile.put('\n');
		////save mEvScheWorkTime
		//for (int i = 0; i < mEvScheWorkTime.GetLength(); i++)
		//{
		//	outfile.put(mEvScheWorkTime[i]);
		//}
		//outfile.put('\n');
		////save mEvScheTime
		//for (int i = 0; i < mEvScheTime.GetLength(); i++)
		//{
		//	outfile.put(mEvScheTime[i]);
		//}
		//outfile.put('\n');
		//outfile.put(mOurMeetingTI + '0'); outfile.put('\n');
		//outfile.put(mOurMeetingTimeJ + '0'); outfile.put('\n');
		//outfile.put(mOtherMeetingTimeI + '0'); outfile.put('\n');
		//outfile.put(mOtherMeetingTimeJ + '0'); outfile.put('\n');
		//outfile.put(mPersonNum + '0'); outfile.put('\n');
		//outfile.put(mScheduleCount + '0'); outfile.put('\n');
		//outfile.put(mOtherMeetingPerNeed + '0'); outfile.put('\n');
		//outfile.put(mDiffSchedule + '0'); outfile.put('\n');
		//outfile.put(mMustAwaySche1 + '0'); outfile.put('\n');
		//outfile.put(mMustAwaySche2 + '0'); outfile.put('\n');
		//outfile.put(mMustAwaySche3 + '0'); outfile.put('\n');
		//outfile.put(mMustAwaySche4 + '0'); outfile.put('\n');
		//// save mUpWorkTime
		//float temp = mUpWorkTime;
		//int count = 0;
		//while (temp >= 0)
		//{
		//	outfile.put(int(temp) + '0');
		//	temp = temp - int(temp);
		//	temp = temp * 10;
		//	if (count == 0 && temp >= 0)
		//		outfile.put('.');
		//	count++;
		//}
		//outfile.put('\n');
		//// save mDownWorkTime
		//temp = mDownWorkTime;
		//count = 0;
		//while (temp >= 0)
		//{
		//	outfile.put(int(temp) + '0');
		//	temp = temp - int(temp);
		//	temp = temp * 10;
		//	if (count == 0 && temp >= 0)
		//		outfile.put('.');
		//	count++;
		//}
		//outfile.put('\n');
	//}

	// 对话框数据
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
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
	int *SchePersonNeed;
	char** EvScheTime;
	float *EvScheWorkTime;
	afx_msg void OnSetAModel();
	afx_msg void OnSetBModel();
	afx_msg void OnSetCModel();
	afx_msg void OnSetShouYinModel();
};



extern C班表编排图形化界面App theApp;

extern CAboutDlg MyScheModel;

extern int AProgramRunCount;
extern int BProgramRunCount;
extern int CProgramRunCount;
extern int ShouYinProgramRunCount;