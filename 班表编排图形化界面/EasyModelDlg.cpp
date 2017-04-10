// EasyModelDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "������ͼ�λ�����.h"
#include "EasyModelDlg.h"
#include "afxdialogex.h"
#include<fstream>
using namespace std;


// CEasyModelDlg �Ի���

IMPLEMENT_DYNAMIC(CEasyModelDlg, CDialogEx)

CEasyModelDlg::CEasyModelDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CEasyModelDlg::IDD, pParent)
	, mPersonNum(0)
	, mUpWorkTime(0)
	, mDownWorkTime(0)
{

}

CEasyModelDlg::~CEasyModelDlg()
{
}

void CEasyModelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, mPersonNum);
	DDX_Text(pDX, IDC_EDIT2, mUpWorkTime);
	DDX_Text(pDX, IDC_EDIT3, mDownWorkTime);
}


BEGIN_MESSAGE_MAP(CEasyModelDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CEasyModelDlg::OnBnClickedA)
	ON_BN_CLICKED(IDC_BUTTON3, &CEasyModelDlg::OnBnClickedB)
	ON_BN_CLICKED(IDC_BUTTON4, &CEasyModelDlg::OnBnClickedC)
	ON_BN_CLICKED(IDC_BUTTON5, &CEasyModelDlg::OnBnClickedShouYin)
END_MESSAGE_MAP()


// CEasyModelDlg ��Ϣ�������
CEasyModelDlg SimpleModel;

void CEasyModelDlg::OnBnClickedA()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (AProgramRunCount == 0)//Ϊ��ʹ�ÿ������Ű�������޸�ģ���趨����Ϊ>=
	{
		UpdateData();
		fstream outfile;
		outfile.open("AScheduleModel.txt", ios::in);
		if (!outfile)
		{
			//�������
			abort();
		}
		char ch;
		//MySetModel(model);
		char flag = '0';
		while (outfile.get(ch))
		{
			//if (ch == ' ') continue;
			if (ch == '\n') { flag++; continue; }
			switch (flag)
			{
			case '0':mEvSchePerNum.AppendChar(ch); break;
			case '1':mEvScheWorkTime.AppendChar(ch); break;
			case '2':mEvScheTime.AppendChar(ch); break;
			case '3':mOurMeetingTI = ch - '0'; break;
			case '4':mOurMeetingTimeJ = ch - '0'; break;
			case '5':mOtherMeetingTimeI = ch - '0'; break;
			case '6':mOtherMeetingTimeJ = ch - '0'; break;
				/*case '7': break;
				case '8':mScheduleCount = ch - '0'; break;
				case '9':mOtherMeetingTimeI = ch - '0'; break;
				case ':':mOtherMeetingTimeJ = ch - '0'; break;*/
			case '7':break;//mPersonNum = ch - '0'; 
			case '8':mScheduleCount = ch - '0'; break;
			case '9':mOtherMeetingPerNeed = ch - '0'; break;
			case ':':mDiffSchedule = ch - '0'; break;
			case ';':mMustAwaySche1 = ch - '0'; break;
			case '<':mMustAwaySche2 = ch - '0'; break;
			case '=':mMustAwaySche3 = ch - '0'; break;
			case '>':mMustAwaySche4 = ch - '0'; break;
			case '?':break;
			case '@':break;
			}
		}
		outfile.close();
		Start(CString("D:\\Aschedule.txt"));
		CDialogEx::OnOK();
		AProgramRunCount++;
	}
	else
	{
		UpdateData();
		Start(CString("D:\\Aschedule.txt"));
		CDialogEx::OnOK();
	}
}


void CEasyModelDlg::OnBnClickedB()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (BProgramRunCount == 0)
	{
		UpdateData();
		fstream outfile;
		outfile.open("BScheduleModel.txt", ios::in);
		if (!outfile)
		{
			//�������
			abort();
		}
		char ch;
		//MySetModel(model);
		char flag = '0';
		while (outfile.get(ch))
		{
			//if (ch == ' ') continue;
			if (ch == '\n') { flag++; continue; }
			switch (flag)
			{
			case '0':mEvSchePerNum.AppendChar(ch); break;
			case '1':mEvScheWorkTime.AppendChar(ch); break;
			case '2':mEvScheTime.AppendChar(ch); break;
			case '3':mOurMeetingTI = ch - '0'; break;
			case '4':mOurMeetingTimeJ = ch - '0'; break;
			case '5':mOtherMeetingTimeI = ch - '0'; break;
			case '6':mOtherMeetingTimeJ = ch - '0'; break;
				/*case '7': break;
				case '8':mScheduleCount = ch - '0'; break;
				case '9':mOtherMeetingTimeI = ch - '0'; break;
				case ':':mOtherMeetingTimeJ = ch - '0'; break;*/
			case '7':break;//mPersonNum = ch - '0'; 
			case '8':mScheduleCount = ch - '0'; break;
			case '9':mOtherMeetingPerNeed = ch - '0'; break;
			case ':':mDiffSchedule = ch - '0'; break;
			case ';':mMustAwaySche1 = ch - '0'; break;
			case '<':mMustAwaySche2 = ch - '0'; break;
			case '=':mMustAwaySche3 = ch - '0'; break;
			case '>':mMustAwaySche4 = ch - '0'; break;
			case '?':break;
			case '@':break;
			}
		}
		outfile.close();
		Start(CString("D:\\Bschedule.txt"));
		CDialogEx::OnOK();
		BProgramRunCount++;
	}
	else
	{
		UpdateData();
		Start(CString("D:\\Bschedule.txt"));
		CDialogEx::OnOK();
	}
}


void CEasyModelDlg::OnBnClickedC()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (CProgramRunCount == 0)
	{
		UpdateData();
		fstream outfile;
		outfile.open("CScheduleModel.txt", ios::in);
		if (!outfile)
		{
			//�������
			abort();
		}
		char ch;
		//MySetModel(model);
		char flag = '0';
		while (outfile.get(ch))
		{
			//if (ch == ' ') continue;
			if (ch == '\n') { flag++; continue; }
			switch (flag)
			{
			case '0':mEvSchePerNum.AppendChar(ch); break;
			case '1':mEvScheWorkTime.AppendChar(ch); break;
			case '2':mEvScheTime.AppendChar(ch); break;
			case '3':mOurMeetingTI = ch - '0'; break;
			case '4':mOurMeetingTimeJ = ch - '0'; break;
			case '5':mOtherMeetingTimeI = ch - '0'; break;
			case '6':mOtherMeetingTimeJ = ch - '0'; break;
				/*case '7': break;
				case '8':mScheduleCount = ch - '0'; break;
				case '9':mOtherMeetingTimeI = ch - '0'; break;
				case ':':mOtherMeetingTimeJ = ch - '0'; break;*/
			case '7':break;//mPersonNum = ch - '0'; 
			case '8':mScheduleCount = ch - '0'; break;
			case '9':mOtherMeetingPerNeed = ch - '0'; break;
			case ':':mDiffSchedule = ch - '0'; break;
			case ';':mMustAwaySche1 = ch - '0'; break;
			case '<':mMustAwaySche2 = ch - '0'; break;
			case '=':mMustAwaySche3 = ch - '0'; break;
			case '>':mMustAwaySche4 = ch - '0'; break;
			case '?':break;
			case '@':break;
			}
		}
		outfile.close();
		Start(CString("D:\\Cschedule.txt"));
		CDialogEx::OnOK();
		CProgramRunCount++;
	}
	else
	{
		UpdateData();
		Start(CString("D:\\Cschedule.txt"));
		CDialogEx::OnOK();
	}
}


void CEasyModelDlg::OnBnClickedShouYin()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (ShouYinProgramRunCount == 0)
	{
		UpdateData();
		fstream outfile;
		outfile.open("ShouYinScheduleModel.txt", ios::in);
		if (!outfile)
		{
			//�������
			abort();
		}
		char ch;
		//MySetModel(model);
		char flag = '0';
		while (outfile.get(ch))
		{
			//if (ch == ' ') continue;
			if (ch == '\n') { flag++; continue; }
			switch (flag)
			{
			case '0':mEvSchePerNum.AppendChar(ch); break;
			case '1':mEvScheWorkTime.AppendChar(ch); break;
			case '2':mEvScheTime.AppendChar(ch); break;
			case '3':mOurMeetingTI = ch - '0'; break;
			case '4':mOurMeetingTimeJ = ch - '0'; break;
			case '5':mOtherMeetingTimeI = ch - '0'; break;
			case '6':mOtherMeetingTimeJ = ch - '0'; break;
				/*case '7': break;
				case '8':mScheduleCount = ch - '0'; break;
				case '9':mOtherMeetingTimeI = ch - '0'; break;
				case ':':mOtherMeetingTimeJ = ch - '0'; break;*/
			case '7':break;//mPersonNum = ch - '0'; 
			case '8':mScheduleCount = ch - '0'; break;
			case '9':mOtherMeetingPerNeed = ch - '0'; break;
			case ':':mDiffSchedule = ch - '0'; break;
			case ';':mMustAwaySche1 = ch - '0'; break;
			case '<':mMustAwaySche2 = ch - '0'; break;
			case '=':mMustAwaySche3 = ch - '0'; break;
			case '>':mMustAwaySche4 = ch - '0'; break;
			case '?':break;
			case '@':break;
			}
		}
		outfile.close();
		Start(CString("D:\\ShouYinschedule.txt"));
		CDialogEx::OnOK();
		ShouYinProgramRunCount++;
	}
	else
	{
		UpdateData();
		Start(CString("D:\\ShouYinschedule.txt"));
		CDialogEx::OnOK();
	}
}
