
#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "班表编排图形化界面.h"
#include "MainFrm.h"

#include "班表编排图形化界面Doc.h"
#include "班表编排图形化界面View.h"
# include<iostream>
using namespace std;
# include<vector>
# include<fstream>
# include<Windows.h>
//# include<map>
//# include<string.h>

//个性化员工适合工时设计
//某些班次需要特殊人数处理，例如其他区例会代班
//手动换班、加班、减班操作
# include<string>
class cStudent
{
public:

	vector<char> mName;
	float mWorkTime;//工时累计
	float mLeftSpareTime;//剩余空闲时间
	float mSpareTime;//总空闲时间
	int **mppSpareTimeMatrix;//空闲时间矩阵
	int **mppTimeArrangeMatrix;//安排矩阵
	int mTimeWeight;//工时权值
	float mMinTime;//最少工时
	float mSuitableTime;//适合工时
	float mMaxTime;//最大工时
	int mCourseCount;//班次累计（2-5之间）
	int *mpWorkTimeArray;//班次对应工时
	int mScheduleCount;//班次
	int mDayCount;//天数
	int mSpecailID;
	float mAverageWorkTime;//平均上班时间
	vector<int> mSaveIJ;//用来记录该学生上班的具体班次
public:
	cStudent(int _ScheduleCount, int _DayCount)//初始化数据
	{
		mTimeWeight = 0;
		mSpecailID = 0;
		mCourseCount = 0;
		mWorkTime = 0;
		mSpareTime = 0;
		mLeftSpareTime = 0;
		mScheduleCount = _ScheduleCount;
		mDayCount = _DayCount;
		mpWorkTimeArray = new int[_ScheduleCount];
		mppSpareTimeMatrix = new int*[_ScheduleCount];
		mppTimeArrangeMatrix = new int*[_ScheduleCount];
		for (int i = 0; i < _ScheduleCount; i++)
		{
			mppSpareTimeMatrix[i] = new int[_DayCount];
			mppTimeArrangeMatrix[i] = new int[_DayCount];
		}
	}

	void InitWorkTimeArray(float *_WorkTime)//初始化班次对应工时
	{
		for (int i = 0; i < mScheduleCount; i++)
		{
			mpWorkTimeArray[i] = _WorkTime[i];
		}
	}

	void DealWithData()//计算总空闲时间
	{
		for (int i = 0; i < mScheduleCount; i++)
		{
			for (int j = 0; j < mDayCount; j++)
			{
				if (mppSpareTimeMatrix[i][j] == 1)
				{
					mSpareTime += mpWorkTimeArray[i];
				}
			}
		}
		mLeftSpareTime = mSpareTime;
	}

	void SetMinMaxTime(float average, float up, float down)//设置最小最大工时
	{
		if (mTimeWeight == 0)
		{
			mMinTime = average - down;
			mSuitableTime = average;
			mMaxTime = average + up;
		}
		else
		{
			if (mTimeWeight == 1)
			{
				mMinTime = average;
				mSuitableTime = average + 1;
				mMaxTime = average + 2;
			}
			else
			{
				if (mTimeWeight == 2)
				{
					mMinTime = average - 2.5;
					mSuitableTime = average - 1;
					mMaxTime = average;
				}
				else
				{
					mMinTime = average - 3.5;
					mSuitableTime = average - 2;
					mMaxTime = average - 1;
				}
			}
		}
	}

	void InitTimeArrangeMatrix()//初始化班次安排矩阵
	{
		for (int i = 0; i < mScheduleCount; i++)
		{
			for (int j = 0; j < mDayCount; j++)
			{
				mppTimeArrangeMatrix[i][j] = 0;
			}
		}
	}

	inline void PressAndAddIJ(int i, int j)
	{
		mSaveIJ.push_back(i*mDayCount + j);
	}

	void UnPressAndDeleteIJ(int i, int j)
	{
		int I_J = i*mDayCount + j;
		for (vector<int>::iterator iter = mSaveIJ.begin(); iter != mSaveIJ.end(); iter++)
		{
			if (*iter == I_J)
			{
				mSaveIJ.erase(iter);
				break;
			}
		}
	}

};

class cAdminstrator
{
public:
	cStudent** mStudentArray;
	float *mWorkTime;//班次对应工时
	int *mPersonNeed;//班次需要人数
	vector<cStudent*>**mArrangePersonMatrix;//安排的人矩阵
	int **mScheduleAbleNum;//每个班能上的人数
	int mStudnetNumber;//学生人数
	int mScheduleCount;//班次
	int mDayCount;//天数
	//一视同仁处理
	int mMaxScheduleCount;//最大班次
	int mMinScheduleCount;//最小班次
	float mMaxWTime;//最大工作时间
	float mMinWTime;//最小工作时间
	float mSuitableWTime;//合适工作时间
	//特殊情况，某个班次不用上
	//map<int, int> mSpecialScheduleIJ;
	int mSpecialI;//例会时间
	int mSpecialJ;
	int mDifficultI;//大家都不愿上的班次
	int mLinkI;//联系班次
	int mLinkJ;
	int mNL1;//mNL1不能和mNL11连续
	int mNL11;
	int mNL2;
	int mNL22;
	float mAverageWorkTime;//平均上班时间
	vector<cStudent*>* mSpecailPerson;//其他区例会安排的人
	int mSchedule_i;//其他区例会班次及时间
	int mDay_j;
	int mSPersonneed;
public:
	cAdminstrator(int _StudentNumber, int _SchduleCount, int _DayCount, int _SI, int _SJ, int _DiffI, int _mNL1, int _mNL11, int _mNL2, int _mNL22, int schedule_i, int day_j, int person_need)//初始化学生人数，班次需要人数，及对应工时，安排人的矩阵大小
	{
		//SI,SJ表示不用上班的时间
		mSchedule_i = schedule_i;
		mDay_j = day_j;
		mSPersonneed = person_need;
		mSpecailPerson = new vector<cStudent*>();
		mLinkI = -1;
		mLinkJ = -1;
		mNL1 = _mNL1;
		mNL11 = _mNL11;
		mNL2 = _mNL2;
		mNL22 = _mNL22;
		mDifficultI = _DiffI;
		mSpecialI = _SI, mSpecialJ = _SJ;
		mMaxScheduleCount = 5;
		mMinScheduleCount = 2;
		mMaxWTime = 10;
		mMinWTime = 6;
		mSuitableWTime = 8;
		mScheduleCount = _SchduleCount;
		mDayCount = _DayCount;
		mStudnetNumber = _StudentNumber;
		mPersonNeed = new int[_SchduleCount];
		mWorkTime = new float[_SchduleCount];
		mStudentArray = new cStudent*[_StudentNumber];
		for (int i = 0; i < _StudentNumber; i++)
		{
			mStudentArray[i] = new cStudent(_SchduleCount, _DayCount);
		}
		mStudentArray[0]->mSpecailID = 1;
		mStudentArray[1]->mSpecailID = 1;
		mArrangePersonMatrix = new vector<cStudent*>*[_SchduleCount];
		mScheduleAbleNum = new int*[_SchduleCount];
		for (int i = 0; i < _SchduleCount; i++)
		{
			mArrangePersonMatrix[i] = new vector<cStudent*>[_DayCount];
			mScheduleAbleNum[i] = new int[_DayCount];
		}
	}

	void CountAverageWorkTime()
	{
		float num = 0;
		for (int i = 0; i < mScheduleCount; i++)//一天需要上班的总时间
		{
			num += mWorkTime[i] * mPersonNeed[i];
		}
		num = num*mDayCount;//一周需要上班的总时间
		num = num - mWorkTime[mSpecialI] * mPersonNeed[mSpecialI];
		num += mWorkTime[mSchedule_i] * mSPersonneed;
		mAverageWorkTime = num / mStudnetNumber;
	}

	bool AvoidSeq(int k, int r, int c)
	{
		if (r == mNL1)
		{
			if (mStudentArray[k]->mppTimeArrangeMatrix[mNL11][c] == 1)
				return false;
		}
		if (r == mNL11)
		{
			if (mStudentArray[k]->mppTimeArrangeMatrix[mNL1][c] == 1)
				return false;
		}
		if (r == mNL2)
		{
			if (mStudentArray[k]->mppTimeArrangeMatrix[mNL22][c] == 1)
				return false;
		}
		if (r == mNL22)
		{
			if (mStudentArray[k]->mppTimeArrangeMatrix[mNL2][c] == 1)
				return false;
		}
		return true;
	}

	void InitWorkTPersonN(float* _WorkTime, int* _PersonNeed)//初始化班次对应工时及需要人数数据（包括学生）
	{
		for (int i = 0; i < mScheduleCount; i++)
		{
			mWorkTime[i] = _WorkTime[i];
			mPersonNeed[i] = _PersonNeed[i];
		}
		for (int i = 0; i < mStudnetNumber; i++)
		{
			mStudentArray[i]->InitWorkTimeArray(_WorkTime);//初始化每个学生里面班次对应工时
			//mStudentArray[i]->SetMinMaxTime();//设置每个学生的最大最小工时
		}
	}

	void InitPersonTime(float aver, float up, float down)
	{
		for (int i = 0; i < mStudnetNumber; i++)
		{
			//mStudentArray[i]->InitWorkTimeArray(_WorkTime);//初始化每个学生里面班次对应工时
			mStudentArray[i]->SetMinMaxTime(aver, up, down);//设置每个学生的最大最小工时
		}
	}

	bool TestError()//检查输入数据是否正确
	{
		fstream infile;
		char ch;
		int SuitNumberNum = mScheduleCount*mDayCount;
		int CurRow = 1;//当前所在行数
		int NameStartFlag = 0;
		int NumberStartFlag = 0;
		int PStudent = 0;//学生人数
		int NameCount = 0;//姓名字数
		int NumberCount = 0;//数字个数
		//bool right = true;
		//int Row, Column;
		//int StudentNum = 0;
		infile.open("D:\\test.txt", ios::in);
		if (!infile)
		{
			cout << "file can't open" << endl;
		}
		while (infile.get(ch))
		{
			switch (ch)//错误处理，可优化
			{
			case '#':
			{   if (NameStartFlag == 0)
			{
				PStudent++;
				NameStartFlag = 1;
				NameCount = 0;
			}
			else
			{
				if (NameCount > 8)
				{
					cout << "第" << CurRow << "行" << "名字长度过长!!!" << endl;
					//right = false;
					return false;
				}
				NameStartFlag = 0;
			}
			break;
			}
			case '$':
			{
						if (NumberStartFlag == 0)
						{
							NumberStartFlag = 1;
							NumberCount = 0;
						}
						else
						{
							NumberStartFlag = 0;
							if (NumberCount < SuitNumberNum)
							{
								cout << "第" << CurRow << "行" << "01数字少于标准值!!!" << endl;
								//right = false;
								return false;
							}
							if (NumberCount > SuitNumberNum)
							{
								cout << "第" << CurRow << "行" << "01数字多于标准值!!!" << endl;
								//right = false;
								return false;
							}
						}
						break;
			}
			case ' ':break;
			case '\n':CurRow++; break;
			default:
			{
					   if (NameStartFlag == 1)
					   {
						   //mStudentArray[PStudent]->mName.push_back(ch);
						   NameCount++;

					   }
					   else
					   {
						   //Row = NumberCount / mDayCount;
						   // Column = NumberCount - Row*mDayCount;
						   //mStudentArray[PStudent]->mppSpareTimeMatrix[Row][Column] = ch - '0';
						   NumberCount++;
					   }
					   break;
			}
			}
		}
		if (PStudent != mStudnetNumber)
		{
			cout << "读取数据中学生人数与实际人数存在差异，请检查输入数据!!!" << endl;
			return false;
		}
		return true;
	}

	bool TestErrorWithWeight()
	{
		fstream infile;
		char ch;
		int SuitNumberNum = mScheduleCount*mDayCount;
		int CurRow = 1;//当前所在行数
		int NameStartFlag = 0;
		int NumberStartFlag = 0;
		int WeightStartFlag = 0;
		int PStudent = 0;//学生人数
		int NameCount = 0;//姓名字数
		int NumberCount = 0;//数字个数
		int WeightCount = 0;//权值个数
		//bool right = true;
		//int Row, Column;
		//int StudentNum = 0;
		infile.open("D:\\test2.txt", ios::in);
		if (!infile)
		{
			cout << "file can't open" << endl;
		}
		while (infile.get(ch))
		{
			switch (ch)//错误处理，可优化
			{
			case '#':
			{   if (NameStartFlag == 0)
			{
				PStudent++;
				NameStartFlag = 1;
				NameCount = 0;
			}
			else
			{
				if (NameCount > 8)
				{
					cout << "第" << CurRow << "行" << "名字长度过长!!!" << endl;
					//right = false;
					return false;
				}
				NameStartFlag = 0;
			}
			break;
			}
			case '%':
			{
						if (WeightStartFlag == 0)
						{
							WeightStartFlag = 1;
							WeightCount = 0;
						}
						else
						{
							if (WeightCount != 1)
							{
								cout << "第" << CurRow << "行" << "权值数目不对!!!" << endl;
								return false;
							}
							WeightStartFlag = 0;
						}
						break;
			}
			case '$':
			{
						if (NumberStartFlag == 0)
						{
							NumberStartFlag = 1;
							NumberCount = 0;
						}
						else
						{
							NumberStartFlag = 0;
							if (NumberCount < SuitNumberNum)
							{
								cout << "第" << CurRow << "行" << "01数字少于标准值!!!" << endl;
								//right = false;
								return false;
							}
							if (NumberCount > SuitNumberNum)
							{
								cout << "第" << CurRow << "行" << "01数字多于标准值!!!" << endl;
								//right = false;
								return false;
							}
						}
						break;
			}
			case ' ':break;
			case '\n':CurRow++; break;
			default:
			{
					   if (WeightStartFlag == 1)
					   {
						   WeightCount++;
					   }
					   else
					   {
						   if (NameStartFlag == 1)
						   {
							   //mStudentArray[PStudent]->mName.push_back(ch);
							   NameCount++;

						   }
						   else
						   {
							   //Row = NumberCount / mDayCount;
							   // Column = NumberCount - Row*mDayCount;
							   //mStudentArray[PStudent]->mppSpareTimeMatrix[Row][Column] = ch - '0';
							   NumberCount++;
						   }
					   }
					   break;
			}
			}
		}
		if (PStudent != mStudnetNumber)
		{
			cout << "读取数据中学生人数与实际人数存在差异，请检查输入数据!!!" << endl;
			return false;
		}
		return true;
	}

	void ImportData()//导入学生数据
	{
		fstream infile;
		char ch;
		int NameStartFlag = 0;
		int NumberStartFlag = 0;
		int PStudent = -1;
		int NameCount = 0;//姓名字数
		int NumberCount = 0;//数字个数
		int Row, Column;
		infile.open("D:\\test.txt", ios::in);
		if (!infile)
		{
			cout << "file can't open" << endl;
		}
		while (infile.get(ch))
		{
			switch (ch)//错误处理，可优化
			{
			case '#':
			{       if (NameStartFlag == 0)
			{
				PStudent++;
				NameStartFlag = 1;
				NameCount = 0;
			}
			else
				NameStartFlag = 0;
			break;
			}
			case '$':
			{
						if (NumberStartFlag == 0)
						{
							NumberStartFlag = 1;
							NumberCount = 0;
						}
						else
							NumberStartFlag = 0;
						break;
			}
			case ' ':break;
			case '\n':break;
			default:
			{
					   if (NameStartFlag == 1)
					   {
						   mStudentArray[PStudent]->mName.push_back(ch);
						   NameCount++;
					   }
					   else
					   {
						   Row = NumberCount / mDayCount;
						   Column = NumberCount - Row*mDayCount;
						   mStudentArray[PStudent]->mppSpareTimeMatrix[Row][Column] = ch - '0';
						   NumberCount++;
					   }
					   break;
			}
			}
		}
	}

	void ImportDataWithWeight()//导入学生数据
	{
		fstream infile;
		char ch;
		int NameStartFlag = 0;
		int NumberStartFlag = 0;
		int WeightStartFlag = 0;
		int PStudent = -1;
		int NameCount = 0;//姓名字数
		int NumberCount = 0;//数字个数
		int Row, Column;
		infile.open("D:\\test2.txt", ios::in);
		if (!infile)
		{
			cout << "file can't open" << endl;
		}
		while (infile.get(ch))
		{
			switch (ch)//错误处理，可优化
			{
			case '#':
			{       if (NameStartFlag == 0)
			{
				PStudent++;
				NameStartFlag = 1;
				NameCount = 0;
			}
			else
				NameStartFlag = 0;
			break;
			}
			case '%':
			{
						if (WeightStartFlag == 0)
						{
							WeightStartFlag = 1;
						}
						else
							WeightStartFlag = 0;
						break;
			}
			case '$':
			{
						if (NumberStartFlag == 0)
						{
							NumberStartFlag = 1;
							NumberCount = 0;
						}
						else
							NumberStartFlag = 0;
						break;
			}
			case ' ':break;
			case '\n':break;
			default:
			{
					   if (WeightStartFlag == 1)
					   {
						   mStudentArray[PStudent]->mTimeWeight = ch - '0';
					   }
					   else
					   {
						   if (NameStartFlag == 1)
						   {
							   mStudentArray[PStudent]->mName.push_back(ch);
							   NameCount++;
						   }
						   else
						   {
							   Row = NumberCount / mDayCount;
							   Column = NumberCount - Row*mDayCount;
							   mStudentArray[PStudent]->mppSpareTimeMatrix[Row][Column] = ch - '0';
							   NumberCount++;
						   }
					   }
					   break;
			}
			}
		}
	}

	void DealStudentData()//计算学生空闲时间
	{
		for (int i = 0; i < mStudnetNumber; i++)
		{
			mStudentArray[i]->DealWithData();//计算每个学生的总空闲时间
			mStudentArray[i]->InitTimeArrangeMatrix();//初始化学生时间安排矩阵
		}
	}

	void CountScheduleNum()//计算每个班能上的人数
	{
		for (int j = 0; j < mScheduleCount; j++)
		{
			for (int k = 0; k < mDayCount; k++)
			{
				mScheduleAbleNum[j][k] = 0;
			}
		}
		for (int i = 0; i < mStudnetNumber; i++)
		{
			for (int j = 0; j < mScheduleCount; j++)
			{
				for (int k = 0; k < mDayCount; k++)
				{
					if (mStudentArray[i]->mppSpareTimeMatrix[j][k] == 1)
						mScheduleAbleNum[j][k]++;
				}
			}
		}
	}

	bool ArrangeSpecialSchedule()
	{
		int BestI = -1, BestJ = -1;
		if (mStudnetNumber < 2) return false;
		for (int i = 0; i < mScheduleCount; i++)
		{
			for (int j = 0; j < mDayCount; j++)
			{
				if (mStudentArray[0]->mppSpareTimeMatrix[i][j] == 1 && mStudentArray[1]->mppSpareTimeMatrix[i][j] == 1 && mPersonNeed[i] >= 2)
				{
					if (BestI = -1)
					{
						BestI = i; BestJ = j;
					}
					else
					{
						if (mWorkTime[i]>mWorkTime[BestI] && mPersonNeed[i] == 2)
						{
							BestI = i;
							BestJ = j;
						}
						else
						{
							if (j > BestJ&&j != 5 && j != 6 && mPersonNeed[i] == 2)
							{
								BestI = i;
								BestJ = j;
							}
						}
					}
				}
			}
		}
		mArrangePersonMatrix[BestI][BestJ].push_back(mStudentArray[0]);//安排k上
		mStudentArray[0]->mppTimeArrangeMatrix[BestI][BestJ] = 1;//安排矩阵更新
		mStudentArray[0]->mLeftSpareTime = mStudentArray[0]->mSpareTime - mWorkTime[BestI];//剩余空闲时间更新
		mStudentArray[0]->mCourseCount++;//班次累计
		mStudentArray[0]->mWorkTime += mWorkTime[BestI];//工时累计
		mStudentArray[0]->PressAndAddIJ(BestI, BestJ);//添加（i,j）进入该学生已上的班次中

		mArrangePersonMatrix[BestI][BestJ].push_back(mStudentArray[1]);//安排k上
		mStudentArray[1]->mppTimeArrangeMatrix[BestI][BestJ] = 1;//安排矩阵更新
		mStudentArray[1]->mLeftSpareTime = mStudentArray[1]->mSpareTime - mWorkTime[BestI];//剩余空闲时间更新
		mStudentArray[1]->mCourseCount++;//班次累计
		mStudentArray[1]->mWorkTime += mWorkTime[BestI];//工时累计
		mStudentArray[1]->PressAndAddIJ(BestI, BestJ);//添加（i,j）进入该学生已上的班次中
		mLinkI = BestI;
		mLinkJ = BestJ;
		return true;
		return false;
	}
	bool ArrangeSpecial()
	{
		//int BestI = -1, BestJ = -1;
		if (mStudnetNumber < 2) return false;
		for (int i = 0; i < mScheduleCount; i++)
		{
			for (int j = 0; j < mDayCount; j++)
			{
				if (mStudentArray[0]->mppSpareTimeMatrix[i][j] == 1 && mStudentArray[1]->mppSpareTimeMatrix[i][j] == 1 && mPersonNeed[i] >= 2)
				{
					mArrangePersonMatrix[i][j].push_back(mStudentArray[0]);//安排k上
					mStudentArray[0]->mppTimeArrangeMatrix[i][j] = 1;//安排矩阵更新
					mStudentArray[0]->mLeftSpareTime = mStudentArray[0]->mSpareTime - mWorkTime[i];//剩余空闲时间更新
					mStudentArray[0]->mCourseCount++;//班次累计
					mStudentArray[0]->mWorkTime += mWorkTime[i];//工时累计
					mStudentArray[0]->PressAndAddIJ(i, j);//添加（i,j）进入该学生已上的班次中

					mArrangePersonMatrix[i][j].push_back(mStudentArray[1]);//安排k上
					mStudentArray[1]->mppTimeArrangeMatrix[i][j] = 1;//安排矩阵更新
					mStudentArray[1]->mLeftSpareTime = mStudentArray[1]->mSpareTime - mWorkTime[i];//剩余空闲时间更新
					mStudentArray[1]->mCourseCount++;//班次累计
					mStudentArray[1]->mWorkTime += mWorkTime[i];//工时累计
					mStudentArray[1]->PressAndAddIJ(i, j);//添加（i,j）进入该学生已上的班次中
					mLinkI = i;
					mLinkJ = j;
					return true;
				}
			}
		}
		return false;
	}

	void SortStudentAsSpareTime()//根据学生空闲时间排序
	{
		int k;
		cStudent* temp;
		for (int i = 0; i < mStudnetNumber - 1; i++)
		{
			k = i;
			for (int j = k + 1; j < mStudnetNumber; j++)
			{
				if (mStudentArray[k]->mSpareTime>mStudentArray[j]->mSpareTime)
					k = j;
			}
			if (k != i)
			{
				temp = mStudentArray[i];
				mStudentArray[i] = mStudentArray[k];
				mStudentArray[k] = temp;
			}
		}
	}

	//第一步
	void ArrangeMinPersonSchedule()//安排能上人数少的班次 可优化，比如比需要人数多一个也可加入其中
	{
		SortStudentAsSpareTime();
		for (int j = 0; j < mDayCount; j++)
		{
			for (int i = 0; i < mScheduleCount; i++)
			{
				if (i == mSpecialI&&j == mSpecialJ)
					continue;
				if (mScheduleAbleNum[i][j] <= mPersonNeed[i])//每个班能上人数小于或等于需要人数，优先排班
				{
					for (int k = 0; k < mStudnetNumber; k++)
					{
						if (mStudentArray[k]->mppSpareTimeMatrix[i][j] == 1 && mStudentArray[k]->mCourseCount<mMaxScheduleCount&&mStudentArray[k]->mppTimeArrangeMatrix[i][j] == 0)
						{
							//bool flag = true;//避免班次连续
							//for (int t = 0; i>0 && t < mArrangePersonMatrix[i - 1][j].size(); t++)
							//{
							//	if (mArrangePersonMatrix[i - 1][j][t] == mStudentArray[k])
							//	{
							//		flag = false;
							//		break;
							//	}
							//}
							if (!AvoidSeq(k, i, j)) continue;//避免特殊班次连续
							//避免班次连续
							if (!(i>0 && mStudentArray[k]->mppTimeArrangeMatrix[i - 1][j] == 1 || (i + 1 < mScheduleCount&&mStudentArray[k]->mppTimeArrangeMatrix[i + 1][j] == 1)))
							{
								mArrangePersonMatrix[i][j].push_back(mStudentArray[k]);//安排k上
								mStudentArray[k]->mppTimeArrangeMatrix[i][j] = 1;//安排矩阵更新
								mStudentArray[k]->mLeftSpareTime = mStudentArray[k]->mSpareTime - mWorkTime[i];//剩余空闲时间更新
								mStudentArray[k]->mCourseCount++;//班次累计
								mStudentArray[k]->mWorkTime += mWorkTime[i];//工时累计
								mStudentArray[k]->PressAndAddIJ(i, j);//添加（i,j）进入该学生已上的班次中
								if (mPersonNeed[i] == mArrangePersonMatrix[i][j].size())//判断此班是否排满
									break;
							}
						}
					}
				}
			}
		}
	}
	//第二步
	void ArrangePersonMinSTimeSchedule()
	{
		for (int n = 0; n < mStudnetNumber; n++)
		{
			bool empty = true;//是否还没安排满
			for (int j = 0; j <mDayCount&&empty; j++)//第一遍，尽量分隔开班次，使其在不同的日期
			{
				for (int i = 0; i < mScheduleCount; i++)
				{
					if (i == mSpecialI&&j == mSpecialJ)
						continue;
					if (mStudentArray[n]->mppSpareTimeMatrix[i][j] == 1 && mStudentArray[n]->mppTimeArrangeMatrix[i][j] == 0 && mArrangePersonMatrix[i][j].size() < mPersonNeed[i])
					{
						float TempTime = mStudentArray[n]->mWorkTime + mWorkTime[i];
						if (TempTime < mStudentArray[n]->mSuitableTime + 1 && mStudentArray[n]->mCourseCount<mMaxScheduleCount - 1)
						{
							//bool flag = true;//避免班次连续
							//for (int t = 0; i>0 && t < mArrangePersonMatrix[i - 1][j].size(); t++)
							//{
							//	if (mArrangePersonMatrix[i - 1][j][t] == mStudentArray[n])
							//	{
							//		flag = false;
							//		break;
							//	}
							//}
							if (!AvoidSeq(n, i, j)) continue;//避免特殊班次连续
							//避免班次连续
							if (!(i>0 && mStudentArray[n]->mppTimeArrangeMatrix[i - 1][j] == 1 || (i + 1 < mScheduleCount&&mStudentArray[n]->mppTimeArrangeMatrix[i + 1][j] == 1)))
							{
								mArrangePersonMatrix[i][j].push_back(mStudentArray[n]);//安排n上
								mStudentArray[n]->mppTimeArrangeMatrix[i][j] = 1;//安排矩阵更新
								mStudentArray[n]->mLeftSpareTime = mStudentArray[n]->mSpareTime - mWorkTime[i];//剩余空闲时间更新
								mStudentArray[n]->mCourseCount++;//班次累计
								mStudentArray[n]->mWorkTime += mWorkTime[i];//工时累计
								mStudentArray[n]->PressAndAddIJ(i, j);//添加（i,j）进入该学生已上的班次中
								if (TempTime >= mStudentArray[n]->mSuitableTime)
									empty = false;
								break;
							}
						}
					}
				}
			}
			if (empty)
			{
				for (int j = 0; j < mDayCount&&empty; j++)//第二遍，尽量分隔开班次，使其不连续
				{
					for (int i = 0; i < mScheduleCount&&empty; i++)
					{
						if (i == mSpecialI&&j == mSpecialJ)
							continue;
						if (mStudentArray[n]->mppSpareTimeMatrix[i][j] == 1 && mStudentArray[n]->mppTimeArrangeMatrix[i][j] == 0 && mArrangePersonMatrix[i][j].size() < mPersonNeed[i])
						{
							float TempTime = mStudentArray[n]->mWorkTime + mWorkTime[i];
							if (TempTime < mStudentArray[n]->mSuitableTime + 1 && mStudentArray[n]->mCourseCount<mMaxScheduleCount - 1)
							{

								if (!AvoidSeq(n, i, j)) continue;//避免特殊班次连续
								//避免班次连续
								if (!(i>0 && mStudentArray[n]->mppTimeArrangeMatrix[i - 1][j] == 1 || (i + 1 < mScheduleCount&&mStudentArray[n]->mppTimeArrangeMatrix[i + 1][j] == 1)))
								{
									mArrangePersonMatrix[i][j].push_back(mStudentArray[n]);//安排n上
									mStudentArray[n]->mppTimeArrangeMatrix[i][j] = 1;//安排矩阵更新
									mStudentArray[n]->mLeftSpareTime = mStudentArray[n]->mSpareTime - mWorkTime[i];//剩余空闲时间更新
									mStudentArray[n]->mCourseCount++;//班次累计
									mStudentArray[n]->mWorkTime += mWorkTime[i];//工时累计
									mStudentArray[n]->PressAndAddIJ(i, j);//添加（i,j）进入该学生已上的班次中
									if (TempTime >= mStudentArray[n]->mSuitableTime)
										empty = false;
									i++;
								}
							}
						}
					}
				}
			}

		}
	}

	int ArrangeLWPSchedule(int i, int j)
	{
		int BestAnswer = -1, PreAnswer = -1;
		int preweight = 0, curweight = 0;//比较两者工时、班次、以及班次是否连续，找出较优解
		//for (int k = 0; k < mStudnetNumber; k++)
		for (int k = mStudnetNumber - 1; k >= 0; k--)
		{
			if (mStudentArray[k]->mppSpareTimeMatrix[i][j] == 1 && mStudentArray[k]->mppTimeArrangeMatrix[i][j] == 0)//有空且没被安排
			{
				if (!AvoidSeq(k, i, j)) continue;//避免特殊班次连续
				//避免班次连续
				if (i>0 && mStudentArray[k]->mppTimeArrangeMatrix[i - 1][j] == 1 || (i + 1 < mScheduleCount&&mStudentArray[k]->mppTimeArrangeMatrix[i + 1][j] == 1))
					continue;
				if (BestAnswer == -1)
				{
					BestAnswer = k;
					//班次连续权值为2
					if (mStudentArray[k]->mWorkTime + mWorkTime[i] > mStudentArray[k]->mMaxTime - 0.5)
						curweight++;
					if (mStudentArray[k]->mCourseCount + 1 > mMaxScheduleCount - 1)
						curweight++;
					preweight = curweight;
					curweight = 0;
				}
				else
				{

					//if (i>0 && mStudentArray[k]->mppTimeArrangeMatrix[i - 1][j] == 1 || (i + 1<mScheduleCount&&mStudentArray[k]->mppTimeArrangeMatrix[i + 1][j] == 1))
					//	curweight = curweight + 3;//班次连续权值为2
					if (mStudentArray[k]->mWorkTime + mWorkTime[i] > mStudentArray[k]->mMaxTime - 0.5)
						curweight++;
					if (mStudentArray[k]->mCourseCount + 1 > mMaxScheduleCount - 1)
						curweight++;
					if (curweight == preweight)
					{
						if (mStudentArray[k]->mWorkTime < mStudentArray[BestAnswer]->mWorkTime)
						{
							//PreAnswer = BestAnswer;
							BestAnswer = k;
						}
					}
					if (curweight < preweight)
					{
						//PreAnswer = BestAnswer;
						BestAnswer = k;
						preweight = curweight;
					}
					curweight = 0;
				}
			}
		}
		if (BestAnswer != -1)
		{
			mArrangePersonMatrix[i][j].push_back(mStudentArray[BestAnswer]);//安排n上
			mStudentArray[BestAnswer]->mppTimeArrangeMatrix[i][j] = 1;//安排矩阵更新
			mStudentArray[BestAnswer]->mLeftSpareTime = mStudentArray[BestAnswer]->mSpareTime - mWorkTime[i];//剩余空闲时间更新
			mStudentArray[BestAnswer]->mCourseCount++;//班次累计
			mStudentArray[BestAnswer]->mWorkTime += mWorkTime[i];//工时累计
			mStudentArray[BestAnswer]->PressAndAddIJ(i, j);//添加（i，j）
		}
		if (preweight > 0)//说明存在问题，要么工时超了或班次超了
		{
			//if (i > 0 && mStudentArray[BestAnswer]->mppTimeArrangeMatrix[i - 1][j] == 1)//说明班次连续
			//{
			//	//mArrangePersonMatrix[i - 1][j].clear(mStudentArray[k]);
			//	//vector<cStudent*>::iterator move;
			//	//删除上一班次安排
			//	for (vector<cStudent*>::iterator iter = mArrangePersonMatrix[i - 1][j].begin(); iter != mArrangePersonMatrix[i - 1][j].end(); iter++)
			//	{
			//		if (*iter == mStudentArray[BestAnswer])
			//		{
			//			mArrangePersonMatrix[i - 1][j].erase(iter);
			//			break;
			//		}
			//	}
			//	mStudentArray[BestAnswer]->mppTimeArrangeMatrix[i - 1][j] = 0;//安排矩阵更新
			//	mStudentArray[BestAnswer]->mLeftSpareTime = mStudentArray[BestAnswer]->mSpareTime + mWorkTime[i - 1];//剩余空闲时间更新
			//	mStudentArray[BestAnswer]->mCourseCount--;//班次累计
			//	mStudentArray[BestAnswer]->mWorkTime -= mWorkTime[i - 1];//工时累计
			//	return (i - 1)*mDayCount + j;//行列压缩
			//}
			//else

			int BestManyPerson = -1;
			int rr, cc;
			for (int c = 0; c < mDayCount; c++)
			{
				for (int r = 0; r < mScheduleCount; r++)
				{
					if (r == i&&c == j)
						continue;
					if (r == mLinkI&&c == mLinkJ)//避免特殊班次换掉
						continue;
					if (mStudentArray[BestAnswer]->mppTimeArrangeMatrix[r][c] == 1 && mScheduleAbleNum[r][c]>BestManyPerson)
					{
						rr = r;
						cc = c;
						BestManyPerson = mScheduleAbleNum[r][c];
					}
				}
			}
			for (vector<cStudent*>::iterator iter = mArrangePersonMatrix[rr][cc].begin(); iter != mArrangePersonMatrix[rr][cc].end(); iter++)
			{
				if (*iter == mStudentArray[BestAnswer])
				{
					mArrangePersonMatrix[rr][cc].erase(iter);
					break;
				}
			}
			mStudentArray[BestAnswer]->mppTimeArrangeMatrix[rr][cc] = 0;//安排矩阵更新
			mStudentArray[BestAnswer]->mLeftSpareTime = mStudentArray[BestAnswer]->mSpareTime + mWorkTime[rr];//剩余空闲时间更新
			mStudentArray[BestAnswer]->mCourseCount--;//班次累计
			mStudentArray[BestAnswer]->mWorkTime -= mWorkTime[rr];//工时累计
			mStudentArray[BestAnswer]->UnPressAndDeleteIJ(rr, cc);//删去（rr，cc）
			return (rr)*mDayCount + cc;

		}
		return -1;
	}

	int ArrangePointSchedule(int i, int j)
	{
		int BestAnswer = -1, PreAnswer = -1;
		int preweight = 0, curweight = 0;//比较两者工时、班次、以及班次是否连续，找出较优解
		for (int k = 0; k < mStudnetNumber; k++)
		{
			if (mStudentArray[k]->mppSpareTimeMatrix[i][j] == 1 && mStudentArray[k]->mppTimeArrangeMatrix[i][j] == 0)//有空且没被安排
			{
				if (BestAnswer == -1)
				{
					BestAnswer = k;
					if (!AvoidSeq(k, i, j))
						curweight = curweight + 3; //特殊班次连续
					if (i>0 && mStudentArray[k]->mppTimeArrangeMatrix[i - 1][j] == 1 || (i + 1<mScheduleCount&&mStudentArray[k]->mppTimeArrangeMatrix[i + 1][j] == 1))
						curweight = curweight + 3;//班次连续权值为2
					if (mStudentArray[k]->mWorkTime + mWorkTime[i] > mStudentArray[k]->mMaxTime - 0.5)
						curweight++;
					if (mStudentArray[k]->mCourseCount + 1 > mMaxScheduleCount)
						curweight++;
					preweight = curweight;
					curweight = 0;
				}
				else
				{
					if (!AvoidSeq(k, i, j))
						curweight = curweight + 3;
					if (i>0 && mStudentArray[k]->mppTimeArrangeMatrix[i - 1][j] == 1 || (i + 1<mScheduleCount&&mStudentArray[k]->mppTimeArrangeMatrix[i + 1][j] == 1))
						curweight = curweight + 3;//班次连续权值为2
					if (mStudentArray[k]->mWorkTime + mWorkTime[i] > mStudentArray[k]->mMaxTime)
						curweight++;
					if (mStudentArray[k]->mCourseCount + 1 > mMaxScheduleCount)
						curweight++;
					if (curweight < preweight)
					{
						//PreAnswer = BestAnswer;
						BestAnswer = k;
						preweight = curweight;
					}
					if (curweight == preweight)
					{
						if (mStudentArray[k]->mWorkTime < mStudentArray[BestAnswer]->mWorkTime)
						{
							//PreAnswer = BestAnswer;
							BestAnswer = k;
						}
					}
					curweight = 0;
				}
			}
		}
		if (BestAnswer != -1)
		{
			mArrangePersonMatrix[i][j].push_back(mStudentArray[BestAnswer]);//安排n上
			mStudentArray[BestAnswer]->mppTimeArrangeMatrix[i][j] = 1;//安排矩阵更新
			mStudentArray[BestAnswer]->mLeftSpareTime = mStudentArray[BestAnswer]->mSpareTime - mWorkTime[i];//剩余空闲时间更新
			mStudentArray[BestAnswer]->mCourseCount++;//班次累计
			mStudentArray[BestAnswer]->mWorkTime += mWorkTime[i];//工时累计
			mStudentArray[BestAnswer]->PressAndAddIJ(i, j);//添加（i，j）
		}
		if (preweight > 0)//说明存在问题，要么班次连续，要么工时超了或班次超了
		{
			if (i > 0 && mStudentArray[BestAnswer]->mppTimeArrangeMatrix[i - 1][j] == 1)//说明班次连续
			{
				//mArrangePersonMatrix[i - 1][j].clear(mStudentArray[k]);
				//vector<cStudent*>::iterator move;
				//删除上一班次安排
				for (vector<cStudent*>::iterator iter = mArrangePersonMatrix[i - 1][j].begin(); iter != mArrangePersonMatrix[i - 1][j].end(); iter++)
				{
					if (*iter == mStudentArray[BestAnswer])
					{
						mArrangePersonMatrix[i - 1][j].erase(iter);
						break;
					}
				}
				mStudentArray[BestAnswer]->mppTimeArrangeMatrix[i - 1][j] = 0;//安排矩阵更新
				mStudentArray[BestAnswer]->mLeftSpareTime = mStudentArray[BestAnswer]->mSpareTime + mWorkTime[i - 1];//剩余空闲时间更新
				mStudentArray[BestAnswer]->mCourseCount--;//班次累计
				mStudentArray[BestAnswer]->mWorkTime -= mWorkTime[i - 1];//工时累计
				mStudentArray[BestAnswer]->UnPressAndDeleteIJ(i - 1, j);//删去（i-1，j）
				return (i - 1)*mDayCount + j;//行列压缩
			}
			else
			{
				int BestManyPerson = -1;
				int rr, cc;
				for (int c = 0; c < mDayCount; c++)
				{
					for (int r = 0; r < mScheduleCount; r++)
					{
						if (r == i&&c == j)
							continue;
						if (r == mLinkI&&c == mLinkJ)//避免特殊班次换掉
							continue;
						if (mStudentArray[BestAnswer]->mppTimeArrangeMatrix[r][c] == 1 && mScheduleAbleNum[r][c]>BestManyPerson)
						{
							rr = r;
							cc = c;
							BestManyPerson = mScheduleAbleNum[r][c];
						}
					}
				}
				for (vector<cStudent*>::iterator iter = mArrangePersonMatrix[rr][cc].begin(); iter != mArrangePersonMatrix[rr][cc].end(); iter++)
				{
					if (*iter == mStudentArray[BestAnswer])
					{
						mArrangePersonMatrix[rr][cc].erase(iter);
						break;
					}
				}
				mStudentArray[BestAnswer]->mppTimeArrangeMatrix[rr][cc] = 0;//安排矩阵更新
				mStudentArray[BestAnswer]->mLeftSpareTime = mStudentArray[BestAnswer]->mSpareTime + mWorkTime[rr];//剩余空闲时间更新
				mStudentArray[BestAnswer]->mCourseCount--;//班次累计
				mStudentArray[BestAnswer]->mWorkTime -= mWorkTime[rr];//工时累计
				mStudentArray[BestAnswer]->UnPressAndDeleteIJ(rr, cc);//删去（rr，cc）
				return (rr)*mDayCount + cc;
			}
		}
		return -1;
	}
	//第三步
	//全部补齐
	void ArrangeLeftSchedule()
	{
		for (int j = 0; j < mDayCount; j++)
		{
			for (int i = 0; i < mScheduleCount; i++)
			{
				if (i == mSpecialI&&j == mSpecialJ) continue;
				if (mPersonNeed[i]>mScheduleAbleNum[i][j]) continue;//班次需要人数大于实际能上人数，跳过
				if (mArrangePersonMatrix[i][j].size() < mPersonNeed[i])//判断班次是否排满
				{
					int count = 0, i_j, r, c;
					i_j = ArrangePointSchedule(i, j);
					while (i_j != -1 && count<mScheduleCount*mDayCount)
					{
						r = i_j / mDayCount;
						c = i_j%mDayCount;
						count++;
						i_j = ArrangePointSchedule(r, c);
					}
				}
			}
		}
	}
	//小幅度补齐班表
	void ArrangeLWPLeftSchedule()
	{
		for (int j = 0; j < mDayCount; j++)
		{
			for (int i = 0; i < mScheduleCount; i++)
			{
				if (i == mSpecialI&&j == mSpecialJ) continue;
				if (mPersonNeed[i]>mScheduleAbleNum[i][j]) continue;//班次需要人数大于实际能上人数，跳过
				if (mArrangePersonMatrix[i][j].size() < mPersonNeed[i])//判断班次是否排满
				{
					int count = 0, i_j, r, c;
					i_j = ArrangeLWPSchedule(i, j);
					while (i_j != -1 && count<mScheduleCount*mDayCount)
					{
						r = i_j / mDayCount;
						c = i_j%mDayCount;
						count++;
						i_j = ArrangeLWPSchedule(r, c);
					}
				}
			}
		}
	}
	//第四步 平衡每个人上班时间
	//轻度平衡
	void BalancePersonWorkTime()
	{
		for (int k = 0; k < mStudnetNumber; k++)
		{
			if (mStudentArray[k]->mWorkTime < mStudentArray[k]->mMinTime + 1)
			{
				bool workempty = true;
				for (int j = 0; j < mDayCount&&workempty; j++)
				{
					for (int i = 0; i < mScheduleCount&&workempty; i++)
					{
						if (i == mLinkI&&j == mLinkJ) continue;//避免特殊班次
						if (i == mSpecialI&&j == mSpecialJ) continue;//避免例会排班
						if (mStudentArray[k]->mppSpareTimeMatrix[i][j] == 1 && mStudentArray[k]->mppTimeArrangeMatrix[i][j] == 0)
						{
							if (!AvoidSeq(k, i, j))
								continue;//避免特殊班次连续
							//避免班次连续
							if (!(i>0 && mStudentArray[k]->mppTimeArrangeMatrix[i - 1][j] == 1 || (i + 1 < mScheduleCount&&mStudentArray[k]->mppTimeArrangeMatrix[i + 1][j] == 1)))
							{
								cStudent* tempStudent;
								float tpWTime = 0;
								vector<cStudent*>::iterator moveiter;
								for (vector<cStudent*>::iterator iter = mArrangePersonMatrix[i][j].begin(); iter != mArrangePersonMatrix[i][j].end(); iter++)
								{
									if (tpWTime < (*iter)->mWorkTime)
									{
										tpWTime = (*iter)->mWorkTime;
										tempStudent = *iter;
										moveiter = iter;
									}
								}
								if (tpWTime - mWorkTime[i] >= tempStudent->mSuitableTime - 1 && mStudentArray[k]->mWorkTime + mWorkTime[i] <= mStudentArray[k]->mMaxTime)//更换上班人
								{
									tempStudent->mppTimeArrangeMatrix[i][j] = 0;//安排矩阵更新
									tempStudent->mLeftSpareTime = tempStudent->mSpareTime + mWorkTime[i];//剩余空闲时间更新
									tempStudent->mCourseCount--;//班次累计
									tempStudent->mWorkTime -= mWorkTime[i];//工时累计
									tempStudent->UnPressAndDeleteIJ(i, j);//删除（i，j）
									mArrangePersonMatrix[i][j].erase(moveiter);

									mArrangePersonMatrix[i][j].push_back(mStudentArray[k]);//安排k上
									mStudentArray[k]->mppTimeArrangeMatrix[i][j] = 1;//安排矩阵更新
									mStudentArray[k]->mLeftSpareTime = mStudentArray[k]->mSpareTime - mWorkTime[i];//剩余空闲时间更新
									mStudentArray[k]->mCourseCount++;//班次累计
									mStudentArray[k]->mWorkTime += mWorkTime[i];//工时累计
									mStudentArray[k]->PressAndAddIJ(i, j);//添加（i，j）
									if (mStudentArray[k]->mWorkTime >= mStudentArray[k]->mSuitableTime - 0.5)
										workempty = false;
								}
							}
						}
					}
				}
			}
		}
	}
	//重度平衡
	void BalanceFewPersonWorkTime()
	{
		for (int k = 0; k < mStudnetNumber; k++)
		{
			if (mStudentArray[k]->mWorkTime <= mStudentArray[k]->mMinTime + 0.5)
			{
				bool workempty = true;
				for (int j = 0; j < mDayCount&&workempty; j++)
				{
					for (int i = 0; i < mScheduleCount&&workempty; i++)
					{
						if (i == mLinkI&&j == mLinkJ) continue;//避免特殊班次
						if (i == mSpecialI&&j == mSpecialJ) continue;//避免例会排班
						if (mStudentArray[k]->mppSpareTimeMatrix[i][j] == 1 && mStudentArray[k]->mppTimeArrangeMatrix[i][j] == 0)
						{
							if (!AvoidSeq(k, i, j))
								continue;//避免特殊班次连续
							//避免班次连续
							if (!(i>0 && mStudentArray[k]->mppTimeArrangeMatrix[i - 1][j] == 1 || (i + 1 < mScheduleCount&&mStudentArray[k]->mppTimeArrangeMatrix[i + 1][j] == 1)))
							{
								cStudent* tempStudent = NULL;
								float tpWTime = 0;
								vector<cStudent*>::iterator moveiter;
								//找出上了（i，j）班次上班时间最多的人
								for (vector<cStudent*>::iterator iter = mArrangePersonMatrix[i][j].begin(); iter != mArrangePersonMatrix[i][j].end(); iter++)
								{
									if (tpWTime < (*iter)->mWorkTime)
									{
										tpWTime = (*iter)->mWorkTime;
										tempStudent = *iter;
										moveiter = iter;
									}
								}
								if ((tpWTime - mWorkTime[i] >= tempStudent->mMinTime + 0.5 || tpWTime - mWorkTime[i] >= mStudentArray[k]->mWorkTime + mWorkTime[i]) && mStudentArray[k]->mWorkTime + mWorkTime[i] <= mStudentArray[k]->mMaxTime)//更换上班人
								{
									tempStudent->mppTimeArrangeMatrix[i][j] = 0;//安排矩阵更新
									tempStudent->mLeftSpareTime = tempStudent->mSpareTime + mWorkTime[i];//剩余空闲时间更新
									tempStudent->mCourseCount--;//班次累计
									tempStudent->mWorkTime -= mWorkTime[i];//工时累计
									tempStudent->UnPressAndDeleteIJ(i, j);//删去（i，j）
									mArrangePersonMatrix[i][j].erase(moveiter);//(i,j)班次删除这个人

									mArrangePersonMatrix[i][j].push_back(mStudentArray[k]);//安排k上
									mStudentArray[k]->mppTimeArrangeMatrix[i][j] = 1;//安排矩阵更新
									mStudentArray[k]->mLeftSpareTime = mStudentArray[k]->mSpareTime - mWorkTime[i];//剩余空闲时间更新
									mStudentArray[k]->mCourseCount++;//班次累计
									mStudentArray[k]->mWorkTime += mWorkTime[i];//工时累计
									mStudentArray[k]->PressAndAddIJ(i, j);//添加（i，j）
									if (mStudentArray[k]->mWorkTime >= mStudentArray[k]->mSuitableTime - 0.5)
										workempty = false;
								}
							}
						}
					}
				}
			}
		}
	}
	//第五步
	//换班，避免上到同一种类型的班次，例如上两个长班

	//其他区例会处理
	float Myfabs(float i, float j)
	{
		if (i > j)
			return i - j;
		else
			return j - i;
	}
	int ArrangeSTimePointSchedule(int i, int j)//其他区例会
	{
		int BestPesonSelect = -1;
		int preweight = 0;
		int curweight = 0;
		float pretime_away = 100;//适当工时的离散值
		float curtime_away = 100;
		for (int n = 0; n<mStudnetNumber; n++)
		{
			if (!AvoidSeq(n, i, j)) continue;//避免特殊班次连续
			//避免班次连续
			if (i>0 && mStudentArray[n]->mppTimeArrangeMatrix[i - 1][j] == 1 || (i + 1 < mScheduleCount&&mStudentArray[n]->mppTimeArrangeMatrix[i + 1][j] == 1))
				continue;
			if (mStudentArray[n]->mppSpareTimeMatrix[i][j] == 1 && mStudentArray[n]->mppTimeArrangeMatrix[i][j] == 0)
			{
				if (mStudentArray[n]->mCourseCount > 4)
					curweight++;
				if (mStudentArray[n]->mWorkTime + mWorkTime[i] > mStudentArray[n]->mMaxTime)
					curweight++;
				curtime_away = Myfabs(mStudentArray[n]->mWorkTime + mWorkTime[i], mStudentArray[n]->mSuitableTime);
				if (BestPesonSelect == -1)
				{
					BestPesonSelect = n;
					preweight = curweight;
					pretime_away = curtime_away;
					curtime_away = 100;
					curweight = 0;
				}
				else
				{
					if (curweight < preweight&&curtime_away<pretime_away)
					{
						preweight = curweight;
						pretime_away = curtime_away;
						BestPesonSelect = n;
						curweight = 0;
						curtime_away = 100;
					}
					else
					{
						if (curweight == preweight)
						{
							if (curtime_away<pretime_away)
							{
								preweight = curweight;
								pretime_away = curtime_away;
								BestPesonSelect = n;
								curweight = 0;
								curtime_away = 100;
							}
						}
					}
				}
			}
		}
		if (BestPesonSelect != -1)
		{
			mSpecailPerson->push_back(mStudentArray[BestPesonSelect]);//安排n上
			//mArrangePersonMatrix[i][j].push_back(mStudentArray[BestPesonSelect]);
			mStudentArray[BestPesonSelect]->mppTimeArrangeMatrix[i][j] = 1;//安排矩阵更新
			mStudentArray[BestPesonSelect]->mLeftSpareTime = mStudentArray[BestPesonSelect]->mSpareTime - mWorkTime[i];//剩余空闲时间更新
			mStudentArray[BestPesonSelect]->mCourseCount++;//班次累计
			mStudentArray[BestPesonSelect]->mWorkTime += mWorkTime[i];//工时累计
			mStudentArray[BestPesonSelect]->PressAndAddIJ(i, j);//添加（i，j）
		}
		if (preweight > 0)//说明存在问题，要么工时超了或班次超了
		{
			int BestManyPerson = -1;
			int rr, cc;
			for (int c = 0; c < mDayCount; c++)
			{
				for (int r = 0; r < mScheduleCount; r++)
				{
					if (r == i&&c == j)
						continue;
					if (r == mLinkI&&c == mLinkJ)//避免特殊班次换掉
						continue;
					if (mStudentArray[BestPesonSelect]->mppTimeArrangeMatrix[r][c] == 1 && mScheduleAbleNum[r][c]>BestManyPerson)
					{
						rr = r;
						cc = c;
						BestManyPerson = mScheduleAbleNum[r][c];
					}
				}
			}
			for (vector<cStudent*>::iterator iter = mArrangePersonMatrix[rr][cc].begin(); iter != mArrangePersonMatrix[rr][cc].end(); iter++)
			{
				if (*iter == mStudentArray[BestPesonSelect])
				{
					mArrangePersonMatrix[rr][cc].erase(iter);
					break;
				}
			}
			mStudentArray[BestPesonSelect]->mppTimeArrangeMatrix[rr][cc] = 0;//安排矩阵更新
			mStudentArray[BestPesonSelect]->mLeftSpareTime = mStudentArray[BestPesonSelect]->mSpareTime + mWorkTime[rr];//剩余空闲时间更新
			mStudentArray[BestPesonSelect]->mCourseCount--;//班次累计
			mStudentArray[BestPesonSelect]->mWorkTime -= mWorkTime[rr];//工时累计
			mStudentArray[BestPesonSelect]->UnPressAndDeleteIJ(rr, cc);//删去（rr，cc）
			return (rr)*mDayCount + cc;

		}
		return -1;
	}

	void ArrangeSTimeSchedule()
	{
		int i_j, r, c, count = 0;
		int person_need = mSPersonneed;
		int schedule_i = mSchedule_i;
		int day_j = mDay_j;
		while (person_need)
		{
			i_j = ArrangeSTimePointSchedule(schedule_i, day_j);
			while (i_j != -1 && count<mScheduleCount*mDayCount)
			{
				r = i_j / mDayCount;
				c = i_j%mDayCount;
				count++;
				i_j = ArrangeLWPSchedule(r, c);
			}
			person_need--;
		}
	}

	int Myabs(int i, int j)
	{
		if (i > j)
			return i - j;
		else
			return j - i;
	}

	bool ChangeScheduleIJ(int k, int n, int i, int j, int jj, int DifficultI)//看看n能否换k的（i，j）班次
	{
		if (i == mLinkI&&j == mLinkJ)
			return false;//避免特殊班次被换掉

		if (!AvoidSeq(n, i, j))//避免特殊班次连续,先排除n连续的情况
			return false;
		//先排除n连续的情况
		if (i > 0 && mStudentArray[n]->mppTimeArrangeMatrix[i - 1][j] == 1 || (i + 1 < mScheduleCount&&mStudentArray[n]->mppTimeArrangeMatrix[i + 1][j] == 1))
			return false;
		int size = mStudentArray[n]->mSaveIJ.size();
		int r, c, i_j;
		for (int t = 0; t < size; t++)//遍历n所上的班次
		{
			i_j = mStudentArray[n]->mSaveIJ[t];
			r = i_j / mDayCount;
			c = i_j%mDayCount;
			if (r == mLinkI&&c == mLinkJ) continue;
			if (mStudentArray[k]->mppSpareTimeMatrix[r][c] == 0)//k没空
				continue;
			//k上后会连续
			if (r > 0 && mStudentArray[k]->mppTimeArrangeMatrix[r - 1][c] == 1 || (r + 1 < mScheduleCount&&mStudentArray[k]->mppTimeArrangeMatrix[r + 1][c] == 1))
				continue;
			if (!AvoidSeq(k, i, j))//避免K特殊班次连续
				continue;
			//避免k已上过（r，c）
			if (mStudentArray[k]->mppTimeArrangeMatrix[r][c] == 1)
				continue;
			if (mStudentArray[n]->mppTimeArrangeMatrix[i][j] == 1)
				continue;
			if (i == r)
			{
				if (Myabs(jj, c) >= Myabs(jj, j))//看谁隔得更远
					continue;
			}
			//扩大工时跳过
			if (i != DifficultI)
			{
				if (mStudentArray[k]->mWorkTime > mStudentArray[n]->mWorkTime&&mWorkTime[r] > mWorkTime[i])
					continue;
				if (mStudentArray[k]->mWorkTime < mStudentArray[n]->mWorkTime&&mWorkTime[r] < mWorkTime[i])
					continue;
			}
			//根据时间判断是否可以换班
			if (mStudentArray[k]->mWorkTime - mWorkTime[i] + mWorkTime[r] <= mStudentArray[k]->mSuitableTime + 1 && mStudentArray[n]->mWorkTime + mWorkTime[i] - mWorkTime[r] <= mStudentArray[n]->mSuitableTime + 1)
			{
				int i_jsize, r_csize;
				vector<cStudent*>::iterator i_j, r_c;
				for (vector<cStudent*>::iterator iter = mArrangePersonMatrix[i][j].begin(); iter != mArrangePersonMatrix[i][j].end(); iter++)
				{
					if (*iter == mStudentArray[k])
					{
						mArrangePersonMatrix[i][j].erase(iter);
						break;
					}
				}
				mStudentArray[k]->mppTimeArrangeMatrix[i][j] = 0;//安排矩阵更新
				mStudentArray[k]->mLeftSpareTime = mStudentArray[k]->mSpareTime + mWorkTime[i];//剩余空闲时间更新
				mStudentArray[k]->mCourseCount--;//班次累计
				mStudentArray[k]->mWorkTime -= mWorkTime[i];//工时累计
				mStudentArray[k]->UnPressAndDeleteIJ(i, j);//删除（i，j）
				for (vector<cStudent*>::iterator iter = mArrangePersonMatrix[r][c].begin(); iter != mArrangePersonMatrix[r][c].end(); iter++)
				{
					if (*iter == mStudentArray[n])
					{
						mArrangePersonMatrix[r][c].erase(iter);
						break;
					}
				}
				mStudentArray[n]->mppTimeArrangeMatrix[r][c] = 0;//安排矩阵更新
				mStudentArray[n]->mLeftSpareTime = mStudentArray[n]->mSpareTime + mWorkTime[r];//剩余空闲时间更新
				mStudentArray[n]->mCourseCount--;//班次累计
				mStudentArray[n]->mWorkTime -= mWorkTime[r];//工时累计
				mStudentArray[n]->UnPressAndDeleteIJ(r, c);//删除（r，c）

				mArrangePersonMatrix[i][j].push_back(mStudentArray[n]);//安排k上
				mStudentArray[n]->mppTimeArrangeMatrix[i][j] = 1;//安排矩阵更新
				mStudentArray[n]->mLeftSpareTime = mStudentArray[n]->mSpareTime - mWorkTime[i];//剩余空闲时间更新
				mStudentArray[n]->mCourseCount++;//班次累计
				mStudentArray[n]->mWorkTime += mWorkTime[i];//工时累计
				mStudentArray[n]->PressAndAddIJ(i, j);//添加（i，j）

				mArrangePersonMatrix[r][c].push_back(mStudentArray[k]);//安排k上
				mStudentArray[k]->mppTimeArrangeMatrix[r][c] = 1;//安排矩阵更新
				mStudentArray[k]->mLeftSpareTime = mStudentArray[k]->mSpareTime - mWorkTime[r];//剩余空闲时间更新
				mStudentArray[k]->mCourseCount++;//班次累计
				mStudentArray[k]->mWorkTime += mWorkTime[r];//工时累计
				mStudentArray[k]->PressAndAddIJ(r, c);//添加（r，c）
				return true;
			}
		}
		return false;
	}

	bool ChangeScheduleAsPerson()//返回为真，说明有人换过班
	{
		int Count = 0;//记录换班次数
		int TypeFlag = 0;//标识是否出现同类型的班次
		int RecordJ;//记录上一个同类型的班次
		bool UnDone = true;
		for (int k = 0; k < mStudnetNumber; k++)
		{
			for (int i = 0; i < mScheduleCount; i++)
			{
				TypeFlag = 0; UnDone = true;
				for (int j = 0; j < mDayCount&&UnDone; j++)
				{
					if (i == mLinkI&&j == mLinkJ)
						continue;//避免特殊情况
					if (mStudentArray[k]->mppTimeArrangeMatrix[i][j] == 1)
					{
						TypeFlag++;
						RecordJ = j;
						if (TypeFlag >= 2)//出现了同类型的班次,准备换班
						{
							for (int n = 0; n < mStudnetNumber; n++)
							{
								if (n != k)
								{
									if (mStudentArray[n]->mppSpareTimeMatrix[i][j] == 1)
									{
										if (ChangeScheduleIJ(k, n, i, j, RecordJ, mDifficultI)) //看看n能否换k的（i，j）班次
										{
											Count++;
											UnDone = false;
											break;
										}
									}
									else
									{
										if (mStudentArray[n]->mppSpareTimeMatrix[i][RecordJ] == 1)
										{
											if (ChangeScheduleIJ(k, n, i, RecordJ, j, mDifficultI)) //看看n能否换k的（i，RecordJ）班次
											{
												Count++;
												UnDone = false;
												break;
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
		if (Count != 0)
			return true;
		return false;
	}

	void ShowCurrentArrangeSchedule()
	{
		cout << ".............................................." << endl;
		int i = 0;
		while (i < mScheduleCount)
		{
			int j = 0, need = mPersonNeed[i], k = 0;
			while (k < need)
			{
				j = 0;
				while (j < mDayCount)
				{
					if (mArrangePersonMatrix[i][j].size()>k)
					{
						int size = mArrangePersonMatrix[i][j][k]->mName.size();
						for (int t = 0; t < size; t++)
						{
							cout << mArrangePersonMatrix[i][j][k]->mName[t];
						}
						if (size == 4)
						{
							cout << "**|";
						}
						else
							cout << "|";
					}
					else
						cout << "******|";
					j++;
				}
				if (k == need - 1) cout << endl << "----------------------------------------------" << endl;
				else cout << endl;
				k++;
			}
			i++;
		}
		cout << ".............................................." << endl;
	}
	//输出最终的排班表
	void ShowFinalArrangeSchedule()
	{
		//string time[7] = { "10:00-12:30|", "11:30-13:00|", "12:30-16:00|", "16:00-18:30|", "17:00-18:30|", "18:30-21:00|", "21:00-22:45|" };
		string time[5] = { "10:00-12:30|", "12:30-16:00|", "16:00-18:30|", "18:30-21:00|", "21:00-22:45|" };
		int count = 0;
		cout << "..........................................................................." << endl;
		cout << "***********|" << " 星期一 |" << " 星期二 |" << " 星期三 |" << " 星期四 |" << " 星期五 |";
		cout << " 星期六 |" << " 星期日 |" << endl;
		cout << "---------------------------------------------------------------------------" << endl;
		int i = 0;
		while (i < mScheduleCount)
		{
			int j = 0, need = mPersonNeed[i], k = 0;
			while (k < need)
			{
				j = 0;
				while (j < mDayCount)
				{
					if (j == 0 && k == 0)
					{
						for (int p = 0; p<time[count].size(); p++)
							cout << time[count][p];
						count++;
					}
					if (j == 0 && k != 0)
						cout << "***********|";
					if (mArrangePersonMatrix[i][j].size()>k)
					{
						int size = mArrangePersonMatrix[i][j][k]->mName.size();
						if (size == 4) cout << "**";
						if (size == 6) cout << "*";
						for (int t = 0; t < size; t++)
						{
							cout << mArrangePersonMatrix[i][j][k]->mName[t];
						}
						if (size == 4)
						{
							cout << "**|";
						}
						if (size == 6) cout << "*|";
						if (size == 8)
							cout << "|";
					}
					else
						cout << "********|";
					j++;
				}
				if (k == need - 1) cout << endl << "---------------------------------------------------------------------------" << endl;
				else cout << endl;
				k++;
			}
			i++;
		}
		cout << "..........................................................................." << endl;
	}

	void ShowFinalSArrangeSchedule()//带其他区例会的
	{
		//string time[7] = { "10:00-12:30|", "11:30-13:00|", "12:30-16:00|", "16:00-18:30|", "17:00-18:30|", "18:30-21:00|", "21:00-22:45|" };
		string time[5] = { "10:00-12:30|", "12:30-16:00|", "16:00-18:30|", "18:30-21:00|", "21:00-22:45|" };
		int count = 0;
		cout << "..........................................................................." << endl;
		cout << "***********|" << " 星期一 |" << " 星期二 |" << " 星期三 |" << " 星期四 |" << " 星期五 |";
		cout << " 星期六 |" << " 星期日 |" << endl;
		cout << "---------------------------------------------------------------------------" << endl;
		int i = 0;
		while (i < mScheduleCount)
		{
			int j = 0, need = mPersonNeed[i], k = 0;
			while (k < need)
			{
				j = 0;
				while (j < mDayCount)
				{
					if (j == 0 && k == 0)
					{
						for (int p = 0; p<time[count].size(); p++)
							cout << time[count][p];
						count++;
					}
					if (j == 0 && k != 0)
						cout << "***********|";
					if (mArrangePersonMatrix[i][j].size()>k)
					{
						int size = mArrangePersonMatrix[i][j][k]->mName.size();
						if (size == 4) cout << "**";
						if (size == 6) cout << "*";
						for (int t = 0; t < size; t++)
						{
							cout << mArrangePersonMatrix[i][j][k]->mName[t];
						}
						if (size == 4)
						{
							cout << "**|";
						}
						if (size == 6) cout << "*|";
						if (size == 8)
							cout << "|";
					}
					else
						cout << "********|";
					j++;
				}
				if (k == need - 1) cout << endl << "---------------------------------------------------------------------------" << endl;
				else cout << endl;
				k++;
			}
			if (i == mSchedule_i)
			{
				int point = 0;
				while (point<mSPersonneed)
				{
					for (int c = 0; c < mDayCount; c++)
					{
						if (c == 0 && point == 0)
						{
							for (int p = 0; p<time[i].size(); p++)
								cout << time[i][p];
						}
						if (c == 0 && point != 0)
							cout << "***********|";
						if (c != mDay_j)
						{
							cout << "********|";
						}
						else
						{
							int size = (*mSpecailPerson)[point]->mName.size();
							if (size == 4) cout << "**";
							if (size == 6) cout << "*";
							for (int t = 0; t < size; t++)
							{
								cout << (*mSpecailPerson)[point]->mName[t];
							}
							if (size == 4)
							{
								cout << "**|";
							}
							if (size == 6) cout << "*|";
							if (size == 8)
								cout << "|";
						}
					}
					point++;
					cout << endl;
				}
			}
			i++;
		}
		cout << "..........................................................................." << endl;
	}

	void WriteSchedule()
	{
		fstream outfile;
		outfile.open("D:\\schedule.txt", ios::out);
		if (!outfile)
		{
			cout << "schedule.txt can't open!" << endl;
			abort();
		}
		char ch;
		string time[5] = { "10:00-12:30|", "12:30-16:00|", "16:00-18:30|", "18:30-21:00|", "21:00-22:45|" };
		int count = 0;
		cout << "..........................................................................." << endl;
		cout << "***********|" << " 星期一 |" << " 星期二 |" << " 星期三 |" << " 星期四 |" << " 星期五 |";
		cout << " 星期六 |" << " 星期日 |" << endl;
		cout << "---------------------------------------------------------------------------" << endl;
		int i = 0;
		while (i < mScheduleCount)
		{
			int j = 0, need = mPersonNeed[i], k = 0;
			while (k < need)
			{
				j = 0;
				while (j < mDayCount)
				{
					if (j == 0 && k == 0)
					{
						for (int p = 0; p<time[count].size(); p++)
							cout << time[count][p];
						count++;
					}
					if (j == 0 && k != 0)
						cout << "***********|";
					if (mArrangePersonMatrix[i][j].size()>k)
					{
						int size = mArrangePersonMatrix[i][j][k]->mName.size();
						if (size == 4) cout << "**";
						if (size == 6) cout << "*";
						for (int t = 0; t < size; t++)
						{
							cout << mArrangePersonMatrix[i][j][k]->mName[t];
						}
						if (size == 4)
						{
							cout << "**|";
						}
						if (size == 6) cout << "*|";
						if (size == 8)
							cout << "|";
					}
					else
						cout << "********|";
					j++;
				}
				if (k == need - 1) cout << endl << "---------------------------------------------------------------------------" << endl;
				else cout << endl;
				k++;
			}
			if (i == mSchedule_i)
			{
				int point = 0;
				while (point<mSPersonneed)
				{
					for (int c = 0; c < mDayCount; c++)
					{
						if (c == 0 && point == 0)
						{
							for (int p = 0; p<time[i].size(); p++)
								cout << time[i][p];
						}
						if (c == 0 && point != 0)
							cout << "***********|";
						if (c != mDay_j)
						{
							cout << "********|";
						}
						else
						{
							int size = (*mSpecailPerson)[point]->mName.size();
							if (size == 4) cout << "**";
							if (size == 6) cout << "*";
							for (int t = 0; t < size; t++)
							{
								cout << (*mSpecailPerson)[point]->mName[t];
							}
							if (size == 4)
							{
								cout << "**|";
							}
							if (size == 6) cout << "*|";
							if (size == 8)
								cout << "|";
						}
					}
					point++;
					cout << endl;
				}
			}
			i++;
		}
		cout << "..........................................................................." << endl;
	}

	void ShowEveryStudentWork()
	{
		//cout << endl;
		for (int i = 0; i < mStudnetNumber; i++)
		{
			int size = mStudentArray[i]->mName.size();
			for (int t = 0; t < size; t++)
			{
				cout << mStudentArray[i]->mName[t];
			}
			if (size == 4) cout << "  ";
			cout << ":" << "工时 " << mStudentArray[i]->mWorkTime << " " << "班次 " << mStudentArray[i]->mCourseCount << endl;
		}
	}

	void ShowStudent()
	{
		for (int i = 0; i < mStudnetNumber; i++)
		{
			for (int j = 0; j < mStudentArray[i]->mName.size(); j++)
				cout << mStudentArray[i]->mName[j];
			cout << endl;
			for (int r = 0; r < mScheduleCount; r++)
			{
				for (int c = 0; c < mDayCount; c++)
					cout << mStudentArray[i]->mppSpareTimeMatrix[r][c];
				cout << endl;
			}
			cout << endl;
			cout << endl << mStudentArray[i]->mSpareTime << endl;
		}
	}

	void ShowEStudentDSchedule()
	{
		int r, c, i_j;
		for (int k = 0; k < mStudnetNumber; k++)
		{
			int size = mStudentArray[k]->mSaveIJ.size();
			int s = mStudentArray[k]->mName.size();
			for (int t = 0; t < s; t++)
			{
				cout << mStudentArray[k]->mName[t];
			}
			cout << ": ---总班次:" << size << " ";
			for (int t = 0; t < size; t++)
			{
				i_j = mStudentArray[k]->mSaveIJ[t];
				r = i_j / mDayCount;
				c = i_j%mDayCount;
				cout << "(" << r + 1 << "," << c + 1 << ")" << " ";
			}
			cout << endl;
		}
	}

	void ShowStudentWeight()
	{
		for (int k = 0; k < mStudnetNumber; k++)
		{
			for (int j = 0; j < mStudentArray[k]->mName.size(); j++)
				cout << mStudentArray[k]->mName[j];
			cout << mStudentArray[k]->mTimeWeight << " ";
		}
	}
};

void start()
{
	int PersonNum;
	cout << "请输入员工人数：";
	cin >> PersonNum;
	//cAdminstrator Admin(PersonNum, 7, 7, 6, 1, 2, 0, 2, 3, 5);
	cAdminstrator Admin(PersonNum, 5, 7, 4, 1, 1, -1, -1, -1, -1, 4, 5, 2);
	//float WorkTime[7] = { 2.5, 1.5, 3.5, 2.5, 1.5, 2.5, 1.75 };
	//int PersonNeed[7] = { 2, 1, 2, 2, 1, 2, 3 };
	float WorkTime[5] = { 2.5, 3.5, 2.5, 2.5, 1.75 };
	int PersonNeed[5] = { 2, 2, 2, 2, 3 };
	Admin.InitWorkTPersonN(WorkTime, PersonNeed);
	if (Admin.TestErrorWithWeight())
	{
		Admin.ImportDataWithWeight();
		Admin.CountAverageWorkTime();
		Admin.InitPersonTime(Admin.mAverageWorkTime, 2, 1.5);
		Admin.ShowStudentWeight();
		//Admin.ImportData();
		Admin.DealStudentData();
		Admin.CountScheduleNum();
		//Admin.SortStudentAsSpareTime();
		//Admin.ShowStudent();
		//Admin.ArrangeSpecial();
		Admin.ArrangeSpecial();
		Admin.ArrangeMinPersonSchedule();
		//Admin.ShowEStudentDSchedule();
		//Admin.ShowEveryStudentWork();
		//Admin.ShowCurrentArrangeSchedule();
		//Admin.ShowEveryStudentWork();
		//Admin.ShowFinalArrangeSchedule();
		Admin.ArrangePersonMinSTimeSchedule();
		Admin.ArrangeSTimeSchedule();//安排其他区例会
		//Admin.ShowEStudentDSchedule();
		//Admin.ShowEveryStudentWork();
		Admin.ShowEveryStudentWork();
		Admin.ShowFinalArrangeSchedule();
		//Admin.ShowCurrentArrangeSchedule();
		//Admin.ShowEveryStudentWork();
		Admin.ShowFinalArrangeSchedule();
		Admin.ArrangeLWPLeftSchedule();
		//Admin.ShowCurrentArrangeSchedule();
		//Admin.ShowEveryStudentWork();
		Admin.ShowEStudentDSchedule();
		Admin.ShowEveryStudentWork();
		Admin.ShowFinalArrangeSchedule();
		Admin.ArrangeLeftSchedule();
		//Admin.ShowEveryStudentWork();
		Admin.ShowFinalArrangeSchedule();
		Admin.ArrangeLeftSchedule();
		//Admin.ShowCurrentArrangeSchedule();
		//Admin.ShowEveryStudentWork();
		//Admin.ShowEveryStudentWork();
		//Admin.ShowFinalArrangeSchedule();
		/*Admin.BalancePersonWorkTime();
		Admin.ShowCurrentArrangeSchedule();
		Admin.ShowEveryStudentWork();*/

		Admin.BalanceFewPersonWorkTime();
		//Admin.ShowCurrentArrangeSchedule();
		//Admin.ShowEveryStudentWork();
		//Admin.ShowEStudentDSchedule();
		//Admin.ShowEveryStudentWork();
		Admin.ShowFinalArrangeSchedule();
		//Admin.ShowEStudentDSchedule();
		Admin.ChangeScheduleAsPerson();
		Admin.ShowEStudentDSchedule();
		Admin.ShowEveryStudentWork();
		Admin.ShowFinalArrangeSchedule();
		Admin.ShowFinalSArrangeSchedule();
		cout << Admin.mAverageWorkTime << endl;


	}
	system("pause");
}