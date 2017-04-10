
// ������ͼ�λ�����.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "������ͼ�λ�����.h"
#include "MainFrm.h"
#include "graph_schedule.h"
#include "������ͼ�λ�����Doc.h"
#include "������ͼ�λ�����View.h"
#include "DlgWarning.h";
#include "DlgHelp.h"
#include<fstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// C������ͼ�λ�����App

BEGIN_MESSAGE_MAP(C������ͼ�λ�����App, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &C������ͼ�λ�����App::OnAppAbout)
	// �����ļ��ı�׼�ĵ�����
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
END_MESSAGE_MAP()


// C������ͼ�λ�����App ����

C������ͼ�λ�����App::C������ͼ�λ�����App()
{
	// TODO:  ������Ӧ�ó��� ID �ַ����滻ΪΨһ�� ID �ַ�����������ַ�����ʽ
	//Ϊ CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("������ͼ�λ�����.AppID.NoVersion"));

	// TODO:  �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}

// Ψһ��һ�� C������ͼ�λ�����App ����

C������ͼ�λ�����App theApp;


// C������ͼ�λ�����App ��ʼ��

BOOL C������ͼ�λ�����App::InitInstance()
{
	CWinApp::InitInstance();


	EnableTaskbarInteraction(FALSE);

	// ʹ�� RichEdit �ؼ���Ҫ  AfxInitRichEdit2()	
	// AfxInitRichEdit2();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO:  Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));
	LoadStdProfileSettings(4);  // ���ر�׼ INI �ļ�ѡ��(���� MRU)


	// ע��Ӧ�ó�����ĵ�ģ�塣  �ĵ�ģ��
	// �������ĵ�����ܴ��ں���ͼ֮�������
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(C������ͼ�λ�����Doc),
		RUNTIME_CLASS(CMainFrame),       // �� SDI ��ܴ���
		RUNTIME_CLASS(C������ͼ�λ�����View));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	// ������׼ shell ���DDE�����ļ�������������
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);



	// ��������������ָ�������  ���
	// �� /RegServer��/Register��/Unregserver �� /Unregister ����Ӧ�ó����򷵻� FALSE��
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// Ψһ��һ�������ѳ�ʼ���������ʾ����������и���
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	return TRUE;
}

// C������ͼ�λ�����App ��Ϣ�������


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

CAboutDlg MyScheModel;
int AProgramRunCount=0;
int BProgramRunCount=0;
int CProgramRunCount=0;
int ShouYinProgramRunCount=0;

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
, mPersonNum(0)
, mScheduleCount(0)
, mEvSchePerNum(_T(""))
, mEvScheTime(_T(""))
, mOurMeetingTI(0)
, mOurMeetingTimeJ(0)
, mOtherMeetingTimeI(0)
, mOtherMeetingTimeJ(0)
, mOtherMeetingPerNeed(0)
, mUpWorkTime(0)
, mDownWorkTime(0)
, mDiffSchedule(0)
, mMustAwaySche1(0)
, mMustAwaySche2(0)
, mMustAwaySche3(0)
, mMustAwaySche4(0)
, mEvScheWorkTime(_T(""))
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT12, mPersonNum);
	DDX_Text(pDX, IDC_EDIT9, mScheduleCount);
	DDX_Text(pDX, IDC_EDIT1, mEvSchePerNum);
	DDX_Text(pDX, IDC_EDIT2, mEvScheTime);
	DDX_Text(pDX, IDC_EDIT4, mOurMeetingTI);
	DDX_Text(pDX, IDC_EDIT5, mOurMeetingTimeJ);
	DDX_Text(pDX, IDC_EDIT6, mOtherMeetingTimeI);
	DDX_Text(pDX, IDC_EDIT7, mOtherMeetingTimeJ);
	DDX_Text(pDX, IDC_EDIT8, mOtherMeetingPerNeed);
	DDX_Text(pDX, IDC_EDIT10, mUpWorkTime);
	DDX_Text(pDX, IDC_EDIT11, mDownWorkTime);
	DDX_Text(pDX, IDC_EDIT13, mDiffSchedule);
	DDX_Text(pDX, IDC_EDIT14, mMustAwaySche1);
	DDX_Text(pDX, IDC_EDIT15, mMustAwaySche2);
	DDX_Text(pDX, IDC_EDIT16, mMustAwaySche3);
	DDX_Text(pDX, IDC_EDIT17, mMustAwaySche4);
	DDX_Text(pDX, IDC_EDIT18, mEvScheWorkTime);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CAboutDlg::OnBnClickedOk)
	ON_COMMAND(IDC_BUTTON1, &CAboutDlg::OnSetAModel)
	ON_COMMAND(IDC_BUTTON4, &CAboutDlg::OnSetBModel)
	ON_COMMAND(IDC_BUTTON3, &CAboutDlg::OnSetCModel)
	ON_COMMAND(IDC_BUTTON2, &CAboutDlg::OnSetShouYinModel)
END_MESSAGE_MAP()

// �������жԻ����Ӧ�ó�������
void C������ͼ�λ�����App::OnAppAbout()
{
	HelpDlg.DoModal();
}

// C������ͼ�λ�����App ��Ϣ�������


void CAboutDlg::StartSchedule(CString filename)
{

	if (ChangeCStringToFloatTime() && ChangeCStringToIntNum() && ChangeCStringToStringTime())
	{
		float up = mUpWorkTime;
		float down = mDownWorkTime;
		int n = mPersonNum;
		cAdminstrator Admin(mPersonNum, mScheduleCount, 7, mOurMeetingTI, mOurMeetingTimeJ, mDiffSchedule, mMustAwaySche1, mMustAwaySche2, mMustAwaySche3, mMustAwaySche4, mOtherMeetingTimeI, mOtherMeetingTimeJ, mOtherMeetingPerNeed);
		Admin.InitWorkTPersonN(EvScheWorkTime, SchePersonNeed);
		if (Admin.TestErrorWithWeight())
		{
			Admin.ImportDataWithWeight();
			Admin.CountAverageWorkTime();
			Admin.InitPersonTime(Admin.mAverageWorkTime, mUpWorkTime, mDownWorkTime);
			Admin.ShowStudentWeight();
			Admin.DealStudentData();
			Admin.CountScheduleNum();
			Admin.ArrangeSpecial();
			Admin.ArrangeMinPersonSchedule();
			Admin.ArrangePersonMinSTimeSchedule();
			Admin.ArrangeSTimeSchedule();//��������������
			Admin.ShowEveryStudentWork();
			Admin.ShowFinalArrangeSchedule();
			Admin.ShowFinalArrangeSchedule();
			Admin.ArrangeLWPLeftSchedule();
			Admin.ShowEStudentDSchedule();
			Admin.ShowEveryStudentWork();
			Admin.ShowFinalArrangeSchedule();
			Admin.ArrangeLeftSchedule();
			Admin.ShowFinalArrangeSchedule();
			Admin.ArrangeLeftSchedule();
			Admin.BalanceFewPersonWorkTime();
			Admin.ShowFinalArrangeSchedule();
			Admin.ChangeScheduleAsPerson();
			Admin.ShowEStudentDSchedule();
			Admin.ShowEveryStudentWork();
			Admin.ShowFinalArrangeSchedule();
			Admin.ShowFinalSArrangeSchedule();
			Admin.WriteSchedule(EvScheTime, 12, filename);
			//cout << Admin.mAverageWorkTime << endl;
		}
	}
	else
		WarningDlg.DoModal();
}


void CAboutDlg::OnBnClickedOk()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	//ChangeCStringToFloatTime();
	//ChangeCStringToIntNum();
	//ChangeCStringToStringTime();
	if (ChangeCStringToFloatTime() && ChangeCStringToIntNum() && ChangeCStringToStringTime())
	{
		cAdminstrator Admin(mPersonNum, mScheduleCount, 7, mOurMeetingTI, mOurMeetingTimeJ, mDiffSchedule, mMustAwaySche1, mMustAwaySche2, mMustAwaySche3, mMustAwaySche4, mOtherMeetingTimeI, mOtherMeetingTimeJ, mOtherMeetingPerNeed);
		//float WorkTime[7] = { 2.5, 1.5, 3.5, 2.5, 1.5, 2.5, 1.75 };
		//int PersonNeed[7] = { 2, 1, 2, 2, 1, 2, 3 };
		//float WorkTime[5] = { 2.5, 3.5, 2.5, 2.5, 1.75 };
		//int PersonNeed[5] = { 2, 2, 2, 2, 3 };
		Admin.InitWorkTPersonN(EvScheWorkTime, SchePersonNeed);
		if (Admin.TestErrorWithWeight())
		{
			Admin.ImportDataWithWeight();
			Admin.CountAverageWorkTime();
			Admin.InitPersonTime(Admin.mAverageWorkTime, 2, 1.5);
			Admin.ShowStudentWeight();
			Admin.DealStudentData();
			Admin.CountScheduleNum();
			Admin.ArrangeSpecial();
			Admin.ArrangeMinPersonSchedule();
			Admin.ArrangePersonMinSTimeSchedule();
			Admin.ArrangeSTimeSchedule();//��������������
			Admin.ShowEveryStudentWork();
			Admin.ShowFinalArrangeSchedule();
			Admin.ShowFinalArrangeSchedule();
			Admin.ArrangeLWPLeftSchedule();
			Admin.ShowEStudentDSchedule();
			Admin.ShowEveryStudentWork();
			Admin.ShowFinalArrangeSchedule();
			Admin.ArrangeLeftSchedule();
			Admin.ShowFinalArrangeSchedule();
			Admin.ArrangeLeftSchedule();
			Admin.BalanceFewPersonWorkTime();
			Admin.ShowFinalArrangeSchedule();
			Admin.ChangeScheduleAsPerson();
			Admin.ShowEStudentDSchedule();
			Admin.ShowEveryStudentWork();
			Admin.ShowFinalArrangeSchedule();
			Admin.ShowFinalSArrangeSchedule();
			Admin.WriteSchedule(EvScheTime, 12, CString("D:\\schedule.txt"));
			//cout << Admin.mAverageWorkTime << endl;
			//OnSetAModel();

		}
		CDialogEx::OnOK();
	}
	else
		WarningDlg.DoModal();
}

//CString mEvSchePerNum;CString mEvScheWorkTime;//ÿ����ι�ʱ
//CString mEvScheTime;//ÿ����ε���ʼʱ��
//int mOurMeetingTI;int mOurMeetingTimeJ;int mOtherMeetingTimeI;int mPersonNum;int mScheduleCount;int mOtherMeetingTimeJ;int mOtherMeetingPerNeed;
//int mDiffSchedule;int mMustAwaySche1;int mMustAwaySche2;int mMustAwaySche3;int mMustAwaySche4;
//float mUpWorkTime;float mDownWorkTime;
//
//int *SchePersonNeed;char** EvScheTime;float *EvScheWorkTime;
//void CAboutDlg::SetModel(fstream& outfile)
//{
//	char ch;
//	//save mEvSchePerNum
//	for (int i = 0; i < mEvSchePerNum.GetLength(); i++)
//	{
//		outfile.put(mEvSchePerNum[i]);
//	}
//	outfile.put('\n');
//	//save mEvScheWorkTime
//	for (int i = 0; i < mEvScheWorkTime.GetLength(); i++)
//	{
//		outfile.put(mEvScheWorkTime[i]);
//	}
//	outfile.put('\n');
//	//save mEvScheTime
//	for (int i = 0; i < mEvScheTime.GetLength(); i++)
//	{
//		outfile.put(mEvScheTime[i]);
//	}
//	outfile.put('\n');
//	outfile.put(mOurMeetingTI + '0'); outfile.put('\n');
//	outfile.put(mOurMeetingTimeJ + '0'); outfile.put('\n');
//	outfile.put(mOtherMeetingTimeI + '0'); outfile.put('\n');
//	outfile.put(mOtherMeetingTimeJ + '0'); outfile.put('\n');
//	outfile.put(mPersonNum + '0'); outfile.put('\n');
//	outfile.put(mScheduleCount + '0'); outfile.put('\n');
//	outfile.put(mOtherMeetingPerNeed + '0'); outfile.put('\n');
//	outfile.put(mDiffSchedule + '0'); outfile.put('\n');
//	outfile.put(mMustAwaySche1 + '0'); outfile.put('\n');
//	outfile.put(mMustAwaySche2 + '0'); outfile.put('\n');
//	outfile.put(mMustAwaySche3 + '0'); outfile.put('\n');
//	outfile.put(mMustAwaySche4 + '0'); outfile.put('\n');
//	// save mUpWorkTime
//	float temp=mUpWorkTime;
//	int count = 0;
//	while (temp >= 0)
//	{
//		outfile.put(int(temp)+'0');
//		temp = temp - int(temp);
//		temp = temp * 10;
//		if (count == 0 && temp >= 0)
//			outfile.put('.');
//		count++;
//	}
//	outfile.put('\n');
//	// save mDownWorkTime
//	temp = mDownWorkTime;
//	count = 0;
//	while (temp >= 0)
//	{
//		outfile.put(int(temp) + '0');
//		temp = temp - int(temp);
//		temp = temp * 10;
//		if (count == 0 && temp >= 0)
//			outfile.put('.');
//		count++;
//	}
//	outfile.put('\n');
//}

void CAboutDlg::OnSetAModel()
{
	// TODO:  �ڴ���������������
	UpdateData();
	fstream outfile;
	outfile.open("AScheduleModel.txt", ios::out);
	if (!outfile)
	{
		//�������
		abort();
	}
	//MySetModel(model);
	for (int i = 0; i < mEvSchePerNum.GetLength(); i++)
	{
		outfile.put(mEvSchePerNum[i]);
	}
	outfile.put('\n');
	//save mEvScheWorkTime
	for (int i = 0; i < mEvScheWorkTime.GetLength(); i++)
	{
		outfile.put(mEvScheWorkTime[i]);
	}
	outfile.put('\n');
	//save mEvScheTime
	for (int i = 0; i < mEvScheTime.GetLength(); i++)
	{
		outfile.put(mEvScheTime[i]);
	}
	outfile.put('\n');
	outfile.put(mOurMeetingTI + '0'); outfile.put('\n');
	outfile.put(mOurMeetingTimeJ + '0'); outfile.put('\n');
	outfile.put(mOtherMeetingTimeI + '0'); outfile.put('\n');
	outfile.put(mOtherMeetingTimeJ + '0'); outfile.put('\n');
	outfile.put(mPersonNum + '0'); outfile.put('\n');
	outfile.put(mScheduleCount + '0'); outfile.put('\n');
	outfile.put(mOtherMeetingPerNeed + '0'); outfile.put('\n');
	outfile.put(mDiffSchedule + '0'); outfile.put('\n');
	outfile.put(mMustAwaySche1 + '0'); outfile.put('\n');
	outfile.put(mMustAwaySche2 + '0'); outfile.put('\n');
	outfile.put(mMustAwaySche3 + '0'); outfile.put('\n');
	outfile.put(mMustAwaySche4 + '0'); outfile.put('\n');
	// save mUpWorkTime
	float temp = mUpWorkTime;
	int count = 0;
	while (temp >0)
	{
		outfile.put(int(temp) + '0');
		temp = temp - int(temp);
		temp = temp * 10;
		if (count == 0 && temp >0)
			outfile.put('.');
		count++;
	}
	outfile.put('\n');
	// save mDownWorkTime
	temp = mDownWorkTime;
	count = 0;
	while (temp >0)
	{
		outfile.put(int(temp) + '0');
		temp = temp - int(temp);
		temp = temp * 10;
		if (count == 0 && temp >0)
			outfile.put('.');
		count++;
	}
	outfile.put('\n');
	outfile.close();
	CDialogEx::OnOK();
}


void CAboutDlg::OnSetBModel()
{
	// TODO:  �ڴ���������������
	UpdateData();
	fstream outfile;
	outfile.open("BScheduleModel.txt", ios::out);
	if (!outfile)
	{
		//�������
		abort();
	}
	//MySetModel(model);
	for (int i = 0; i < mEvSchePerNum.GetLength(); i++)
	{
		outfile.put(mEvSchePerNum[i]);
	}
	outfile.put('\n');
	//save mEvScheWorkTime
	for (int i = 0; i < mEvScheWorkTime.GetLength(); i++)
	{
		outfile.put(mEvScheWorkTime[i]);
	}
	outfile.put('\n');
	//save mEvScheTime
	for (int i = 0; i < mEvScheTime.GetLength(); i++)
	{
		outfile.put(mEvScheTime[i]);
	}
	outfile.put('\n');
	outfile.put(mOurMeetingTI + '0'); outfile.put('\n');
	outfile.put(mOurMeetingTimeJ + '0'); outfile.put('\n');
	outfile.put(mOtherMeetingTimeI + '0'); outfile.put('\n');
	outfile.put(mOtherMeetingTimeJ + '0'); outfile.put('\n');
	outfile.put(mPersonNum + '0'); outfile.put('\n');
	outfile.put(mScheduleCount + '0'); outfile.put('\n');
	outfile.put(mOtherMeetingPerNeed + '0'); outfile.put('\n');
	outfile.put(mDiffSchedule + '0'); outfile.put('\n');
	outfile.put(mMustAwaySche1 + '0'); outfile.put('\n');
	outfile.put(mMustAwaySche2 + '0'); outfile.put('\n');
	outfile.put(mMustAwaySche3 + '0'); outfile.put('\n');
	outfile.put(mMustAwaySche4 + '0'); outfile.put('\n');
	// save mUpWorkTime
	float temp = mUpWorkTime;
	int count = 0;
	while (temp >0)
	{
		outfile.put(int(temp) + '0');
		temp = temp - int(temp);
		temp = temp * 10;
		if (count == 0 && temp >0)
			outfile.put('.');
		count++;
	}
	outfile.put('\n');
	// save mDownWorkTime
	temp = mDownWorkTime;
	count = 0;
	while (temp >0)
	{
		outfile.put(int(temp) + '0');
		temp = temp - int(temp);
		temp = temp * 10;
		if (count == 0 && temp >0)
			outfile.put('.');
		count++;
	}
	outfile.put('\n');
	outfile.close();
	CDialogEx::OnOK();
}


void CAboutDlg::OnSetCModel()
{
	// TODO:  �ڴ���������������
	UpdateData();
	fstream outfile;
	outfile.open("CScheduleModel.txt", ios::out);
	if (!outfile)
	{
		//�������
		abort();
	}
	//MySetModel(model);
	for (int i = 0; i < mEvSchePerNum.GetLength(); i++)
	{
		outfile.put(mEvSchePerNum[i]);
	}
	outfile.put('\n');
	//save mEvScheWorkTime
	for (int i = 0; i < mEvScheWorkTime.GetLength(); i++)
	{
		outfile.put(mEvScheWorkTime[i]);
	}
	outfile.put('\n');
	//save mEvScheTime
	for (int i = 0; i < mEvScheTime.GetLength(); i++)
	{
		outfile.put(mEvScheTime[i]);
	}
	outfile.put('\n');
	outfile.put(mOurMeetingTI + '0'); outfile.put('\n');
	outfile.put(mOurMeetingTimeJ + '0'); outfile.put('\n');
	outfile.put(mOtherMeetingTimeI + '0'); outfile.put('\n');
	outfile.put(mOtherMeetingTimeJ + '0'); outfile.put('\n');
	outfile.put(mPersonNum + '0'); outfile.put('\n');
	outfile.put(mScheduleCount + '0'); outfile.put('\n');
	outfile.put(mOtherMeetingPerNeed + '0'); outfile.put('\n');
	outfile.put(mDiffSchedule + '0'); outfile.put('\n');
	outfile.put(mMustAwaySche1 + '0'); outfile.put('\n');
	outfile.put(mMustAwaySche2 + '0'); outfile.put('\n');
	outfile.put(mMustAwaySche3 + '0'); outfile.put('\n');
	outfile.put(mMustAwaySche4 + '0'); outfile.put('\n');
	// save mUpWorkTime
	float temp = mUpWorkTime;
	int count = 0;
	while (temp >0)
	{
		outfile.put(int(temp) + '0');
		temp = temp - int(temp);
		temp = temp * 10;
		if (count == 0 && temp >0)
			outfile.put('.');
		count++;
	}
	outfile.put('\n');
	// save mDownWorkTime
	temp = mDownWorkTime;
	count = 0;
	while (temp >0)
	{
		outfile.put(int(temp) + '0');
		temp = temp - int(temp);
		temp = temp * 10;
		if (count == 0 && temp >0)
			outfile.put('.');
		count++;
	}
	outfile.put('\n');
	outfile.close();
	CDialogEx::OnOK();
}


void CAboutDlg::OnSetShouYinModel()
{
	// TODO:  �ڴ���������������
	UpdateData();
	fstream outfile;
	outfile.open("ShouYinScheduleModel.txt", ios::out);
	if (!outfile)
	{
		//�������
		abort();
	}
	//MySetModel(model);
	for (int i = 0; i < mEvSchePerNum.GetLength(); i++)
	{
		outfile.put(mEvSchePerNum[i]);
	}
	outfile.put('\n');
	//save mEvScheWorkTime
	for (int i = 0; i < mEvScheWorkTime.GetLength(); i++)
	{
		outfile.put(mEvScheWorkTime[i]);
	}
	outfile.put('\n');
	//save mEvScheTime
	for (int i = 0; i < mEvScheTime.GetLength(); i++)
	{
		outfile.put(mEvScheTime[i]);
	}
	outfile.put('\n');
	outfile.put(mOurMeetingTI + '0'); outfile.put('\n');
	outfile.put(mOurMeetingTimeJ + '0'); outfile.put('\n');
	outfile.put(mOtherMeetingTimeI + '0'); outfile.put('\n');
	outfile.put(mOtherMeetingTimeJ + '0'); outfile.put('\n');
	outfile.put(mPersonNum + '0'); outfile.put('\n');
	outfile.put(mScheduleCount + '0'); outfile.put('\n');
	outfile.put(mOtherMeetingPerNeed + '0'); outfile.put('\n');
	outfile.put(mDiffSchedule + '0'); outfile.put('\n');
	outfile.put(mMustAwaySche1 + '0'); outfile.put('\n');
	outfile.put(mMustAwaySche2 + '0'); outfile.put('\n');
	outfile.put(mMustAwaySche3 + '0'); outfile.put('\n');
	outfile.put(mMustAwaySche4 + '0'); outfile.put('\n');
	// save mUpWorkTime
	float temp = mUpWorkTime;
	int count = 0;
	while (temp >0)
	{
		outfile.put(int(temp) + '0');
		temp = temp - int(temp);
		temp = temp * 10;
		if (count == 0 && temp >0)
			outfile.put('.');
		count++;
	}
	outfile.put('\n');
	// save mDownWorkTime
	temp = mDownWorkTime;
	count = 0;
	while (temp >0)
	{
		outfile.put(int(temp) + '0');
		temp = temp - int(temp);
		temp = temp * 10;
		if (count == 0 && temp >0)
			outfile.put('.');
		count++;
	}
	outfile.put('\n');
	outfile.close();
	CDialogEx::OnOK();
}
