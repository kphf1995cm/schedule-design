
// 班表编排图形化界面.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "班表编排图形化界面.h"
#include "MainFrm.h"
#include "graph_schedule.h"
#include "班表编排图形化界面Doc.h"
#include "班表编排图形化界面View.h"
#include "DlgWarning.h";
#include "DlgHelp.h"
#include<fstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// C班表编排图形化界面App

BEGIN_MESSAGE_MAP(C班表编排图形化界面App, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &C班表编排图形化界面App::OnAppAbout)
	// 基于文件的标准文档命令
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
END_MESSAGE_MAP()


// C班表编排图形化界面App 构造

C班表编排图形化界面App::C班表编排图形化界面App()
{
	// TODO:  将以下应用程序 ID 字符串替换为唯一的 ID 字符串；建议的字符串格式
	//为 CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("班表编排图形化界面.AppID.NoVersion"));

	// TODO:  在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}

// 唯一的一个 C班表编排图形化界面App 对象

C班表编排图形化界面App theApp;


// C班表编排图形化界面App 初始化

BOOL C班表编排图形化界面App::InitInstance()
{
	CWinApp::InitInstance();


	EnableTaskbarInteraction(FALSE);

	// 使用 RichEdit 控件需要  AfxInitRichEdit2()	
	// AfxInitRichEdit2();

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO:  应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));
	LoadStdProfileSettings(4);  // 加载标准 INI 文件选项(包括 MRU)


	// 注册应用程序的文档模板。  文档模板
	// 将用作文档、框架窗口和视图之间的连接
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(C班表编排图形化界面Doc),
		RUNTIME_CLASS(CMainFrame),       // 主 SDI 框架窗口
		RUNTIME_CLASS(C班表编排图形化界面View));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	// 分析标准 shell 命令、DDE、打开文件操作的命令行
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);



	// 调度在命令行中指定的命令。  如果
	// 用 /RegServer、/Register、/Unregserver 或 /Unregister 启动应用程序，则返回 FALSE。
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// 唯一的一个窗口已初始化，因此显示它并对其进行更新
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	return TRUE;
}

// C班表编排图形化界面App 消息处理程序


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

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

// 用于运行对话框的应用程序命令
void C班表编排图形化界面App::OnAppAbout()
{
	HelpDlg.DoModal();
}

// C班表编排图形化界面App 消息处理程序


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
			Admin.ArrangeSTimeSchedule();//安排其他区例会
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
	// TODO:  在此添加控件通知处理程序代码
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
			Admin.ArrangeSTimeSchedule();//安排其他区例会
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

//CString mEvSchePerNum;CString mEvScheWorkTime;//每个班次工时
//CString mEvScheTime;//每个班次的起始时间
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
	// TODO:  在此添加命令处理程序代码
	UpdateData();
	fstream outfile;
	outfile.open("AScheduleModel.txt", ios::out);
	if (!outfile)
	{
		//添加提醒
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
	// TODO:  在此添加命令处理程序代码
	UpdateData();
	fstream outfile;
	outfile.open("BScheduleModel.txt", ios::out);
	if (!outfile)
	{
		//添加提醒
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
	// TODO:  在此添加命令处理程序代码
	UpdateData();
	fstream outfile;
	outfile.open("CScheduleModel.txt", ios::out);
	if (!outfile)
	{
		//添加提醒
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
	// TODO:  在此添加命令处理程序代码
	UpdateData();
	fstream outfile;
	outfile.open("ShouYinScheduleModel.txt", ios::out);
	if (!outfile)
	{
		//添加提醒
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
