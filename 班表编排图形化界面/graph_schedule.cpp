
#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "������ͼ�λ�����.h"
#include "MainFrm.h"

#include "������ͼ�λ�����Doc.h"
#include "������ͼ�λ�����View.h"
# include<iostream>
using namespace std;
# include<vector>
# include<fstream>
# include<Windows.h>
//# include<map>
//# include<string.h>

//���Ի�Ա���ʺϹ�ʱ���
//ĳЩ�����Ҫ�����������������������������
//�ֶ����ࡢ�Ӱࡢ�������
# include<string>
class cStudent
{
public:

	vector<char> mName;
	float mWorkTime;//��ʱ�ۼ�
	float mLeftSpareTime;//ʣ�����ʱ��
	float mSpareTime;//�ܿ���ʱ��
	int **mppSpareTimeMatrix;//����ʱ�����
	int **mppTimeArrangeMatrix;//���ž���
	int mTimeWeight;//��ʱȨֵ
	float mMinTime;//���ٹ�ʱ
	float mSuitableTime;//�ʺϹ�ʱ
	float mMaxTime;//���ʱ
	int mCourseCount;//����ۼƣ�2-5֮�䣩
	int *mpWorkTimeArray;//��ζ�Ӧ��ʱ
	int mScheduleCount;//���
	int mDayCount;//����
	int mSpecailID;
	float mAverageWorkTime;//ƽ���ϰ�ʱ��
	vector<int> mSaveIJ;//������¼��ѧ���ϰ�ľ�����
public:
	cStudent(int _ScheduleCount, int _DayCount)//��ʼ������
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

	void InitWorkTimeArray(float *_WorkTime)//��ʼ����ζ�Ӧ��ʱ
	{
		for (int i = 0; i < mScheduleCount; i++)
		{
			mpWorkTimeArray[i] = _WorkTime[i];
		}
	}

	void DealWithData()//�����ܿ���ʱ��
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

	void SetMinMaxTime(float average, float up, float down)//������С���ʱ
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

	void InitTimeArrangeMatrix()//��ʼ����ΰ��ž���
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
	float *mWorkTime;//��ζ�Ӧ��ʱ
	int *mPersonNeed;//�����Ҫ����
	vector<cStudent*>**mArrangePersonMatrix;//���ŵ��˾���
	int **mScheduleAbleNum;//ÿ�������ϵ�����
	int mStudnetNumber;//ѧ������
	int mScheduleCount;//���
	int mDayCount;//����
	//һ��ͬ�ʴ���
	int mMaxScheduleCount;//�����
	int mMinScheduleCount;//��С���
	float mMaxWTime;//�����ʱ��
	float mMinWTime;//��С����ʱ��
	float mSuitableWTime;//���ʹ���ʱ��
	//���������ĳ����β�����
	//map<int, int> mSpecialScheduleIJ;
	int mSpecialI;//����ʱ��
	int mSpecialJ;
	int mDifficultI;//��Ҷ���Ը�ϵİ��
	int mLinkI;//��ϵ���
	int mLinkJ;
	int mNL1;//mNL1���ܺ�mNL11����
	int mNL11;
	int mNL2;
	int mNL22;
	float mAverageWorkTime;//ƽ���ϰ�ʱ��
	vector<cStudent*>* mSpecailPerson;//���������ᰲ�ŵ���
	int mSchedule_i;//�����������μ�ʱ��
	int mDay_j;
	int mSPersonneed;
public:
	cAdminstrator(int _StudentNumber, int _SchduleCount, int _DayCount, int _SI, int _SJ, int _DiffI, int _mNL1, int _mNL11, int _mNL2, int _mNL22, int schedule_i, int day_j, int person_need)//��ʼ��ѧ�������������Ҫ����������Ӧ��ʱ�������˵ľ����С
	{
		//SI,SJ��ʾ�����ϰ��ʱ��
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
		for (int i = 0; i < mScheduleCount; i++)//һ����Ҫ�ϰ����ʱ��
		{
			num += mWorkTime[i] * mPersonNeed[i];
		}
		num = num*mDayCount;//һ����Ҫ�ϰ����ʱ��
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

	void InitWorkTPersonN(float* _WorkTime, int* _PersonNeed)//��ʼ����ζ�Ӧ��ʱ����Ҫ�������ݣ�����ѧ����
	{
		for (int i = 0; i < mScheduleCount; i++)
		{
			mWorkTime[i] = _WorkTime[i];
			mPersonNeed[i] = _PersonNeed[i];
		}
		for (int i = 0; i < mStudnetNumber; i++)
		{
			mStudentArray[i]->InitWorkTimeArray(_WorkTime);//��ʼ��ÿ��ѧ�������ζ�Ӧ��ʱ
			//mStudentArray[i]->SetMinMaxTime();//����ÿ��ѧ���������С��ʱ
		}
	}

	void InitPersonTime(float aver, float up, float down)
	{
		for (int i = 0; i < mStudnetNumber; i++)
		{
			//mStudentArray[i]->InitWorkTimeArray(_WorkTime);//��ʼ��ÿ��ѧ�������ζ�Ӧ��ʱ
			mStudentArray[i]->SetMinMaxTime(aver, up, down);//����ÿ��ѧ���������С��ʱ
		}
	}

	bool TestError()//������������Ƿ���ȷ
	{
		fstream infile;
		char ch;
		int SuitNumberNum = mScheduleCount*mDayCount;
		int CurRow = 1;//��ǰ��������
		int NameStartFlag = 0;
		int NumberStartFlag = 0;
		int PStudent = 0;//ѧ������
		int NameCount = 0;//��������
		int NumberCount = 0;//���ָ���
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
			switch (ch)//���������Ż�
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
					cout << "��" << CurRow << "��" << "���ֳ��ȹ���!!!" << endl;
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
								cout << "��" << CurRow << "��" << "01�������ڱ�׼ֵ!!!" << endl;
								//right = false;
								return false;
							}
							if (NumberCount > SuitNumberNum)
							{
								cout << "��" << CurRow << "��" << "01���ֶ��ڱ�׼ֵ!!!" << endl;
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
			cout << "��ȡ������ѧ��������ʵ���������ڲ��죬������������!!!" << endl;
			return false;
		}
		return true;
	}

	bool TestErrorWithWeight()
	{
		fstream infile;
		char ch;
		int SuitNumberNum = mScheduleCount*mDayCount;
		int CurRow = 1;//��ǰ��������
		int NameStartFlag = 0;
		int NumberStartFlag = 0;
		int WeightStartFlag = 0;
		int PStudent = 0;//ѧ������
		int NameCount = 0;//��������
		int NumberCount = 0;//���ָ���
		int WeightCount = 0;//Ȩֵ����
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
			switch (ch)//���������Ż�
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
					cout << "��" << CurRow << "��" << "���ֳ��ȹ���!!!" << endl;
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
								cout << "��" << CurRow << "��" << "Ȩֵ��Ŀ����!!!" << endl;
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
								cout << "��" << CurRow << "��" << "01�������ڱ�׼ֵ!!!" << endl;
								//right = false;
								return false;
							}
							if (NumberCount > SuitNumberNum)
							{
								cout << "��" << CurRow << "��" << "01���ֶ��ڱ�׼ֵ!!!" << endl;
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
			cout << "��ȡ������ѧ��������ʵ���������ڲ��죬������������!!!" << endl;
			return false;
		}
		return true;
	}

	void ImportData()//����ѧ������
	{
		fstream infile;
		char ch;
		int NameStartFlag = 0;
		int NumberStartFlag = 0;
		int PStudent = -1;
		int NameCount = 0;//��������
		int NumberCount = 0;//���ָ���
		int Row, Column;
		infile.open("D:\\test.txt", ios::in);
		if (!infile)
		{
			cout << "file can't open" << endl;
		}
		while (infile.get(ch))
		{
			switch (ch)//���������Ż�
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

	void ImportDataWithWeight()//����ѧ������
	{
		fstream infile;
		char ch;
		int NameStartFlag = 0;
		int NumberStartFlag = 0;
		int WeightStartFlag = 0;
		int PStudent = -1;
		int NameCount = 0;//��������
		int NumberCount = 0;//���ָ���
		int Row, Column;
		infile.open("D:\\test2.txt", ios::in);
		if (!infile)
		{
			cout << "file can't open" << endl;
		}
		while (infile.get(ch))
		{
			switch (ch)//���������Ż�
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

	void DealStudentData()//����ѧ������ʱ��
	{
		for (int i = 0; i < mStudnetNumber; i++)
		{
			mStudentArray[i]->DealWithData();//����ÿ��ѧ�����ܿ���ʱ��
			mStudentArray[i]->InitTimeArrangeMatrix();//��ʼ��ѧ��ʱ�䰲�ž���
		}
	}

	void CountScheduleNum()//����ÿ�������ϵ�����
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
		mArrangePersonMatrix[BestI][BestJ].push_back(mStudentArray[0]);//����k��
		mStudentArray[0]->mppTimeArrangeMatrix[BestI][BestJ] = 1;//���ž������
		mStudentArray[0]->mLeftSpareTime = mStudentArray[0]->mSpareTime - mWorkTime[BestI];//ʣ�����ʱ�����
		mStudentArray[0]->mCourseCount++;//����ۼ�
		mStudentArray[0]->mWorkTime += mWorkTime[BestI];//��ʱ�ۼ�
		mStudentArray[0]->PressAndAddIJ(BestI, BestJ);//��ӣ�i,j�������ѧ�����ϵİ����

		mArrangePersonMatrix[BestI][BestJ].push_back(mStudentArray[1]);//����k��
		mStudentArray[1]->mppTimeArrangeMatrix[BestI][BestJ] = 1;//���ž������
		mStudentArray[1]->mLeftSpareTime = mStudentArray[1]->mSpareTime - mWorkTime[BestI];//ʣ�����ʱ�����
		mStudentArray[1]->mCourseCount++;//����ۼ�
		mStudentArray[1]->mWorkTime += mWorkTime[BestI];//��ʱ�ۼ�
		mStudentArray[1]->PressAndAddIJ(BestI, BestJ);//��ӣ�i,j�������ѧ�����ϵİ����
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
					mArrangePersonMatrix[i][j].push_back(mStudentArray[0]);//����k��
					mStudentArray[0]->mppTimeArrangeMatrix[i][j] = 1;//���ž������
					mStudentArray[0]->mLeftSpareTime = mStudentArray[0]->mSpareTime - mWorkTime[i];//ʣ�����ʱ�����
					mStudentArray[0]->mCourseCount++;//����ۼ�
					mStudentArray[0]->mWorkTime += mWorkTime[i];//��ʱ�ۼ�
					mStudentArray[0]->PressAndAddIJ(i, j);//��ӣ�i,j�������ѧ�����ϵİ����

					mArrangePersonMatrix[i][j].push_back(mStudentArray[1]);//����k��
					mStudentArray[1]->mppTimeArrangeMatrix[i][j] = 1;//���ž������
					mStudentArray[1]->mLeftSpareTime = mStudentArray[1]->mSpareTime - mWorkTime[i];//ʣ�����ʱ�����
					mStudentArray[1]->mCourseCount++;//����ۼ�
					mStudentArray[1]->mWorkTime += mWorkTime[i];//��ʱ�ۼ�
					mStudentArray[1]->PressAndAddIJ(i, j);//��ӣ�i,j�������ѧ�����ϵİ����
					mLinkI = i;
					mLinkJ = j;
					return true;
				}
			}
		}
		return false;
	}

	void SortStudentAsSpareTime()//����ѧ������ʱ������
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

	//��һ��
	void ArrangeMinPersonSchedule()//�������������ٵİ�� ���Ż����������Ҫ������һ��Ҳ�ɼ�������
	{
		SortStudentAsSpareTime();
		for (int j = 0; j < mDayCount; j++)
		{
			for (int i = 0; i < mScheduleCount; i++)
			{
				if (i == mSpecialI&&j == mSpecialJ)
					continue;
				if (mScheduleAbleNum[i][j] <= mPersonNeed[i])//ÿ������������С�ڻ������Ҫ�����������Ű�
				{
					for (int k = 0; k < mStudnetNumber; k++)
					{
						if (mStudentArray[k]->mppSpareTimeMatrix[i][j] == 1 && mStudentArray[k]->mCourseCount<mMaxScheduleCount&&mStudentArray[k]->mppTimeArrangeMatrix[i][j] == 0)
						{
							//bool flag = true;//����������
							//for (int t = 0; i>0 && t < mArrangePersonMatrix[i - 1][j].size(); t++)
							//{
							//	if (mArrangePersonMatrix[i - 1][j][t] == mStudentArray[k])
							//	{
							//		flag = false;
							//		break;
							//	}
							//}
							if (!AvoidSeq(k, i, j)) continue;//��������������
							//����������
							if (!(i>0 && mStudentArray[k]->mppTimeArrangeMatrix[i - 1][j] == 1 || (i + 1 < mScheduleCount&&mStudentArray[k]->mppTimeArrangeMatrix[i + 1][j] == 1)))
							{
								mArrangePersonMatrix[i][j].push_back(mStudentArray[k]);//����k��
								mStudentArray[k]->mppTimeArrangeMatrix[i][j] = 1;//���ž������
								mStudentArray[k]->mLeftSpareTime = mStudentArray[k]->mSpareTime - mWorkTime[i];//ʣ�����ʱ�����
								mStudentArray[k]->mCourseCount++;//����ۼ�
								mStudentArray[k]->mWorkTime += mWorkTime[i];//��ʱ�ۼ�
								mStudentArray[k]->PressAndAddIJ(i, j);//��ӣ�i,j�������ѧ�����ϵİ����
								if (mPersonNeed[i] == mArrangePersonMatrix[i][j].size())//�жϴ˰��Ƿ�����
									break;
							}
						}
					}
				}
			}
		}
	}
	//�ڶ���
	void ArrangePersonMinSTimeSchedule()
	{
		for (int n = 0; n < mStudnetNumber; n++)
		{
			bool empty = true;//�Ƿ�û������
			for (int j = 0; j <mDayCount&&empty; j++)//��һ�飬�����ָ�����Σ�ʹ���ڲ�ͬ������
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
							//bool flag = true;//����������
							//for (int t = 0; i>0 && t < mArrangePersonMatrix[i - 1][j].size(); t++)
							//{
							//	if (mArrangePersonMatrix[i - 1][j][t] == mStudentArray[n])
							//	{
							//		flag = false;
							//		break;
							//	}
							//}
							if (!AvoidSeq(n, i, j)) continue;//��������������
							//����������
							if (!(i>0 && mStudentArray[n]->mppTimeArrangeMatrix[i - 1][j] == 1 || (i + 1 < mScheduleCount&&mStudentArray[n]->mppTimeArrangeMatrix[i + 1][j] == 1)))
							{
								mArrangePersonMatrix[i][j].push_back(mStudentArray[n]);//����n��
								mStudentArray[n]->mppTimeArrangeMatrix[i][j] = 1;//���ž������
								mStudentArray[n]->mLeftSpareTime = mStudentArray[n]->mSpareTime - mWorkTime[i];//ʣ�����ʱ�����
								mStudentArray[n]->mCourseCount++;//����ۼ�
								mStudentArray[n]->mWorkTime += mWorkTime[i];//��ʱ�ۼ�
								mStudentArray[n]->PressAndAddIJ(i, j);//��ӣ�i,j�������ѧ�����ϵİ����
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
				for (int j = 0; j < mDayCount&&empty; j++)//�ڶ��飬�����ָ�����Σ�ʹ�䲻����
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

								if (!AvoidSeq(n, i, j)) continue;//��������������
								//����������
								if (!(i>0 && mStudentArray[n]->mppTimeArrangeMatrix[i - 1][j] == 1 || (i + 1 < mScheduleCount&&mStudentArray[n]->mppTimeArrangeMatrix[i + 1][j] == 1)))
								{
									mArrangePersonMatrix[i][j].push_back(mStudentArray[n]);//����n��
									mStudentArray[n]->mppTimeArrangeMatrix[i][j] = 1;//���ž������
									mStudentArray[n]->mLeftSpareTime = mStudentArray[n]->mSpareTime - mWorkTime[i];//ʣ�����ʱ�����
									mStudentArray[n]->mCourseCount++;//����ۼ�
									mStudentArray[n]->mWorkTime += mWorkTime[i];//��ʱ�ۼ�
									mStudentArray[n]->PressAndAddIJ(i, j);//��ӣ�i,j�������ѧ�����ϵİ����
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
		int preweight = 0, curweight = 0;//�Ƚ����߹�ʱ����Ρ��Լ�����Ƿ��������ҳ����Ž�
		//for (int k = 0; k < mStudnetNumber; k++)
		for (int k = mStudnetNumber - 1; k >= 0; k--)
		{
			if (mStudentArray[k]->mppSpareTimeMatrix[i][j] == 1 && mStudentArray[k]->mppTimeArrangeMatrix[i][j] == 0)//�п���û������
			{
				if (!AvoidSeq(k, i, j)) continue;//��������������
				//����������
				if (i>0 && mStudentArray[k]->mppTimeArrangeMatrix[i - 1][j] == 1 || (i + 1 < mScheduleCount&&mStudentArray[k]->mppTimeArrangeMatrix[i + 1][j] == 1))
					continue;
				if (BestAnswer == -1)
				{
					BestAnswer = k;
					//�������ȨֵΪ2
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
					//	curweight = curweight + 3;//�������ȨֵΪ2
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
			mArrangePersonMatrix[i][j].push_back(mStudentArray[BestAnswer]);//����n��
			mStudentArray[BestAnswer]->mppTimeArrangeMatrix[i][j] = 1;//���ž������
			mStudentArray[BestAnswer]->mLeftSpareTime = mStudentArray[BestAnswer]->mSpareTime - mWorkTime[i];//ʣ�����ʱ�����
			mStudentArray[BestAnswer]->mCourseCount++;//����ۼ�
			mStudentArray[BestAnswer]->mWorkTime += mWorkTime[i];//��ʱ�ۼ�
			mStudentArray[BestAnswer]->PressAndAddIJ(i, j);//��ӣ�i��j��
		}
		if (preweight > 0)//˵���������⣬Ҫô��ʱ���˻��γ���
		{
			//if (i > 0 && mStudentArray[BestAnswer]->mppTimeArrangeMatrix[i - 1][j] == 1)//˵���������
			//{
			//	//mArrangePersonMatrix[i - 1][j].clear(mStudentArray[k]);
			//	//vector<cStudent*>::iterator move;
			//	//ɾ����һ��ΰ���
			//	for (vector<cStudent*>::iterator iter = mArrangePersonMatrix[i - 1][j].begin(); iter != mArrangePersonMatrix[i - 1][j].end(); iter++)
			//	{
			//		if (*iter == mStudentArray[BestAnswer])
			//		{
			//			mArrangePersonMatrix[i - 1][j].erase(iter);
			//			break;
			//		}
			//	}
			//	mStudentArray[BestAnswer]->mppTimeArrangeMatrix[i - 1][j] = 0;//���ž������
			//	mStudentArray[BestAnswer]->mLeftSpareTime = mStudentArray[BestAnswer]->mSpareTime + mWorkTime[i - 1];//ʣ�����ʱ�����
			//	mStudentArray[BestAnswer]->mCourseCount--;//����ۼ�
			//	mStudentArray[BestAnswer]->mWorkTime -= mWorkTime[i - 1];//��ʱ�ۼ�
			//	return (i - 1)*mDayCount + j;//����ѹ��
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
					if (r == mLinkI&&c == mLinkJ)//���������λ���
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
			mStudentArray[BestAnswer]->mppTimeArrangeMatrix[rr][cc] = 0;//���ž������
			mStudentArray[BestAnswer]->mLeftSpareTime = mStudentArray[BestAnswer]->mSpareTime + mWorkTime[rr];//ʣ�����ʱ�����
			mStudentArray[BestAnswer]->mCourseCount--;//����ۼ�
			mStudentArray[BestAnswer]->mWorkTime -= mWorkTime[rr];//��ʱ�ۼ�
			mStudentArray[BestAnswer]->UnPressAndDeleteIJ(rr, cc);//ɾȥ��rr��cc��
			return (rr)*mDayCount + cc;

		}
		return -1;
	}

	int ArrangePointSchedule(int i, int j)
	{
		int BestAnswer = -1, PreAnswer = -1;
		int preweight = 0, curweight = 0;//�Ƚ����߹�ʱ����Ρ��Լ�����Ƿ��������ҳ����Ž�
		for (int k = 0; k < mStudnetNumber; k++)
		{
			if (mStudentArray[k]->mppSpareTimeMatrix[i][j] == 1 && mStudentArray[k]->mppTimeArrangeMatrix[i][j] == 0)//�п���û������
			{
				if (BestAnswer == -1)
				{
					BestAnswer = k;
					if (!AvoidSeq(k, i, j))
						curweight = curweight + 3; //����������
					if (i>0 && mStudentArray[k]->mppTimeArrangeMatrix[i - 1][j] == 1 || (i + 1<mScheduleCount&&mStudentArray[k]->mppTimeArrangeMatrix[i + 1][j] == 1))
						curweight = curweight + 3;//�������ȨֵΪ2
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
						curweight = curweight + 3;//�������ȨֵΪ2
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
			mArrangePersonMatrix[i][j].push_back(mStudentArray[BestAnswer]);//����n��
			mStudentArray[BestAnswer]->mppTimeArrangeMatrix[i][j] = 1;//���ž������
			mStudentArray[BestAnswer]->mLeftSpareTime = mStudentArray[BestAnswer]->mSpareTime - mWorkTime[i];//ʣ�����ʱ�����
			mStudentArray[BestAnswer]->mCourseCount++;//����ۼ�
			mStudentArray[BestAnswer]->mWorkTime += mWorkTime[i];//��ʱ�ۼ�
			mStudentArray[BestAnswer]->PressAndAddIJ(i, j);//��ӣ�i��j��
		}
		if (preweight > 0)//˵���������⣬Ҫô���������Ҫô��ʱ���˻��γ���
		{
			if (i > 0 && mStudentArray[BestAnswer]->mppTimeArrangeMatrix[i - 1][j] == 1)//˵���������
			{
				//mArrangePersonMatrix[i - 1][j].clear(mStudentArray[k]);
				//vector<cStudent*>::iterator move;
				//ɾ����һ��ΰ���
				for (vector<cStudent*>::iterator iter = mArrangePersonMatrix[i - 1][j].begin(); iter != mArrangePersonMatrix[i - 1][j].end(); iter++)
				{
					if (*iter == mStudentArray[BestAnswer])
					{
						mArrangePersonMatrix[i - 1][j].erase(iter);
						break;
					}
				}
				mStudentArray[BestAnswer]->mppTimeArrangeMatrix[i - 1][j] = 0;//���ž������
				mStudentArray[BestAnswer]->mLeftSpareTime = mStudentArray[BestAnswer]->mSpareTime + mWorkTime[i - 1];//ʣ�����ʱ�����
				mStudentArray[BestAnswer]->mCourseCount--;//����ۼ�
				mStudentArray[BestAnswer]->mWorkTime -= mWorkTime[i - 1];//��ʱ�ۼ�
				mStudentArray[BestAnswer]->UnPressAndDeleteIJ(i - 1, j);//ɾȥ��i-1��j��
				return (i - 1)*mDayCount + j;//����ѹ��
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
						if (r == mLinkI&&c == mLinkJ)//���������λ���
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
				mStudentArray[BestAnswer]->mppTimeArrangeMatrix[rr][cc] = 0;//���ž������
				mStudentArray[BestAnswer]->mLeftSpareTime = mStudentArray[BestAnswer]->mSpareTime + mWorkTime[rr];//ʣ�����ʱ�����
				mStudentArray[BestAnswer]->mCourseCount--;//����ۼ�
				mStudentArray[BestAnswer]->mWorkTime -= mWorkTime[rr];//��ʱ�ۼ�
				mStudentArray[BestAnswer]->UnPressAndDeleteIJ(rr, cc);//ɾȥ��rr��cc��
				return (rr)*mDayCount + cc;
			}
		}
		return -1;
	}
	//������
	//ȫ������
	void ArrangeLeftSchedule()
	{
		for (int j = 0; j < mDayCount; j++)
		{
			for (int i = 0; i < mScheduleCount; i++)
			{
				if (i == mSpecialI&&j == mSpecialJ) continue;
				if (mPersonNeed[i]>mScheduleAbleNum[i][j]) continue;//�����Ҫ��������ʵ����������������
				if (mArrangePersonMatrix[i][j].size() < mPersonNeed[i])//�жϰ���Ƿ�����
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
	//С���Ȳ�����
	void ArrangeLWPLeftSchedule()
	{
		for (int j = 0; j < mDayCount; j++)
		{
			for (int i = 0; i < mScheduleCount; i++)
			{
				if (i == mSpecialI&&j == mSpecialJ) continue;
				if (mPersonNeed[i]>mScheduleAbleNum[i][j]) continue;//�����Ҫ��������ʵ����������������
				if (mArrangePersonMatrix[i][j].size() < mPersonNeed[i])//�жϰ���Ƿ�����
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
	//���Ĳ� ƽ��ÿ�����ϰ�ʱ��
	//���ƽ��
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
						if (i == mLinkI&&j == mLinkJ) continue;//����������
						if (i == mSpecialI&&j == mSpecialJ) continue;//���������Ű�
						if (mStudentArray[k]->mppSpareTimeMatrix[i][j] == 1 && mStudentArray[k]->mppTimeArrangeMatrix[i][j] == 0)
						{
							if (!AvoidSeq(k, i, j))
								continue;//��������������
							//����������
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
								if (tpWTime - mWorkTime[i] >= tempStudent->mSuitableTime - 1 && mStudentArray[k]->mWorkTime + mWorkTime[i] <= mStudentArray[k]->mMaxTime)//�����ϰ���
								{
									tempStudent->mppTimeArrangeMatrix[i][j] = 0;//���ž������
									tempStudent->mLeftSpareTime = tempStudent->mSpareTime + mWorkTime[i];//ʣ�����ʱ�����
									tempStudent->mCourseCount--;//����ۼ�
									tempStudent->mWorkTime -= mWorkTime[i];//��ʱ�ۼ�
									tempStudent->UnPressAndDeleteIJ(i, j);//ɾ����i��j��
									mArrangePersonMatrix[i][j].erase(moveiter);

									mArrangePersonMatrix[i][j].push_back(mStudentArray[k]);//����k��
									mStudentArray[k]->mppTimeArrangeMatrix[i][j] = 1;//���ž������
									mStudentArray[k]->mLeftSpareTime = mStudentArray[k]->mSpareTime - mWorkTime[i];//ʣ�����ʱ�����
									mStudentArray[k]->mCourseCount++;//����ۼ�
									mStudentArray[k]->mWorkTime += mWorkTime[i];//��ʱ�ۼ�
									mStudentArray[k]->PressAndAddIJ(i, j);//��ӣ�i��j��
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
	//�ض�ƽ��
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
						if (i == mLinkI&&j == mLinkJ) continue;//����������
						if (i == mSpecialI&&j == mSpecialJ) continue;//���������Ű�
						if (mStudentArray[k]->mppSpareTimeMatrix[i][j] == 1 && mStudentArray[k]->mppTimeArrangeMatrix[i][j] == 0)
						{
							if (!AvoidSeq(k, i, j))
								continue;//��������������
							//����������
							if (!(i>0 && mStudentArray[k]->mppTimeArrangeMatrix[i - 1][j] == 1 || (i + 1 < mScheduleCount&&mStudentArray[k]->mppTimeArrangeMatrix[i + 1][j] == 1)))
							{
								cStudent* tempStudent = NULL;
								float tpWTime = 0;
								vector<cStudent*>::iterator moveiter;
								//�ҳ����ˣ�i��j������ϰ�ʱ��������
								for (vector<cStudent*>::iterator iter = mArrangePersonMatrix[i][j].begin(); iter != mArrangePersonMatrix[i][j].end(); iter++)
								{
									if (tpWTime < (*iter)->mWorkTime)
									{
										tpWTime = (*iter)->mWorkTime;
										tempStudent = *iter;
										moveiter = iter;
									}
								}
								if ((tpWTime - mWorkTime[i] >= tempStudent->mMinTime + 0.5 || tpWTime - mWorkTime[i] >= mStudentArray[k]->mWorkTime + mWorkTime[i]) && mStudentArray[k]->mWorkTime + mWorkTime[i] <= mStudentArray[k]->mMaxTime)//�����ϰ���
								{
									tempStudent->mppTimeArrangeMatrix[i][j] = 0;//���ž������
									tempStudent->mLeftSpareTime = tempStudent->mSpareTime + mWorkTime[i];//ʣ�����ʱ�����
									tempStudent->mCourseCount--;//����ۼ�
									tempStudent->mWorkTime -= mWorkTime[i];//��ʱ�ۼ�
									tempStudent->UnPressAndDeleteIJ(i, j);//ɾȥ��i��j��
									mArrangePersonMatrix[i][j].erase(moveiter);//(i,j)���ɾ�������

									mArrangePersonMatrix[i][j].push_back(mStudentArray[k]);//����k��
									mStudentArray[k]->mppTimeArrangeMatrix[i][j] = 1;//���ž������
									mStudentArray[k]->mLeftSpareTime = mStudentArray[k]->mSpareTime - mWorkTime[i];//ʣ�����ʱ�����
									mStudentArray[k]->mCourseCount++;//����ۼ�
									mStudentArray[k]->mWorkTime += mWorkTime[i];//��ʱ�ۼ�
									mStudentArray[k]->PressAndAddIJ(i, j);//��ӣ�i��j��
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
	//���岽
	//���࣬�����ϵ�ͬһ�����͵İ�Σ���������������

	//���������ᴦ��
	float Myfabs(float i, float j)
	{
		if (i > j)
			return i - j;
		else
			return j - i;
	}
	int ArrangeSTimePointSchedule(int i, int j)//����������
	{
		int BestPesonSelect = -1;
		int preweight = 0;
		int curweight = 0;
		float pretime_away = 100;//�ʵ���ʱ����ɢֵ
		float curtime_away = 100;
		for (int n = 0; n<mStudnetNumber; n++)
		{
			if (!AvoidSeq(n, i, j)) continue;//��������������
			//����������
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
			mSpecailPerson->push_back(mStudentArray[BestPesonSelect]);//����n��
			//mArrangePersonMatrix[i][j].push_back(mStudentArray[BestPesonSelect]);
			mStudentArray[BestPesonSelect]->mppTimeArrangeMatrix[i][j] = 1;//���ž������
			mStudentArray[BestPesonSelect]->mLeftSpareTime = mStudentArray[BestPesonSelect]->mSpareTime - mWorkTime[i];//ʣ�����ʱ�����
			mStudentArray[BestPesonSelect]->mCourseCount++;//����ۼ�
			mStudentArray[BestPesonSelect]->mWorkTime += mWorkTime[i];//��ʱ�ۼ�
			mStudentArray[BestPesonSelect]->PressAndAddIJ(i, j);//��ӣ�i��j��
		}
		if (preweight > 0)//˵���������⣬Ҫô��ʱ���˻��γ���
		{
			int BestManyPerson = -1;
			int rr, cc;
			for (int c = 0; c < mDayCount; c++)
			{
				for (int r = 0; r < mScheduleCount; r++)
				{
					if (r == i&&c == j)
						continue;
					if (r == mLinkI&&c == mLinkJ)//���������λ���
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
			mStudentArray[BestPesonSelect]->mppTimeArrangeMatrix[rr][cc] = 0;//���ž������
			mStudentArray[BestPesonSelect]->mLeftSpareTime = mStudentArray[BestPesonSelect]->mSpareTime + mWorkTime[rr];//ʣ�����ʱ�����
			mStudentArray[BestPesonSelect]->mCourseCount--;//����ۼ�
			mStudentArray[BestPesonSelect]->mWorkTime -= mWorkTime[rr];//��ʱ�ۼ�
			mStudentArray[BestPesonSelect]->UnPressAndDeleteIJ(rr, cc);//ɾȥ��rr��cc��
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

	bool ChangeScheduleIJ(int k, int n, int i, int j, int jj, int DifficultI)//����n�ܷ�k�ģ�i��j�����
	{
		if (i == mLinkI&&j == mLinkJ)
			return false;//���������α�����

		if (!AvoidSeq(n, i, j))//��������������,���ų�n���������
			return false;
		//���ų�n���������
		if (i > 0 && mStudentArray[n]->mppTimeArrangeMatrix[i - 1][j] == 1 || (i + 1 < mScheduleCount&&mStudentArray[n]->mppTimeArrangeMatrix[i + 1][j] == 1))
			return false;
		int size = mStudentArray[n]->mSaveIJ.size();
		int r, c, i_j;
		for (int t = 0; t < size; t++)//����n���ϵİ��
		{
			i_j = mStudentArray[n]->mSaveIJ[t];
			r = i_j / mDayCount;
			c = i_j%mDayCount;
			if (r == mLinkI&&c == mLinkJ) continue;
			if (mStudentArray[k]->mppSpareTimeMatrix[r][c] == 0)//kû��
				continue;
			//k�Ϻ������
			if (r > 0 && mStudentArray[k]->mppTimeArrangeMatrix[r - 1][c] == 1 || (r + 1 < mScheduleCount&&mStudentArray[k]->mppTimeArrangeMatrix[r + 1][c] == 1))
				continue;
			if (!AvoidSeq(k, i, j))//����K����������
				continue;
			//����k���Ϲ���r��c��
			if (mStudentArray[k]->mppTimeArrangeMatrix[r][c] == 1)
				continue;
			if (mStudentArray[n]->mppTimeArrangeMatrix[i][j] == 1)
				continue;
			if (i == r)
			{
				if (Myabs(jj, c) >= Myabs(jj, j))//��˭���ø�Զ
					continue;
			}
			//����ʱ����
			if (i != DifficultI)
			{
				if (mStudentArray[k]->mWorkTime > mStudentArray[n]->mWorkTime&&mWorkTime[r] > mWorkTime[i])
					continue;
				if (mStudentArray[k]->mWorkTime < mStudentArray[n]->mWorkTime&&mWorkTime[r] < mWorkTime[i])
					continue;
			}
			//����ʱ���ж��Ƿ���Ի���
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
				mStudentArray[k]->mppTimeArrangeMatrix[i][j] = 0;//���ž������
				mStudentArray[k]->mLeftSpareTime = mStudentArray[k]->mSpareTime + mWorkTime[i];//ʣ�����ʱ�����
				mStudentArray[k]->mCourseCount--;//����ۼ�
				mStudentArray[k]->mWorkTime -= mWorkTime[i];//��ʱ�ۼ�
				mStudentArray[k]->UnPressAndDeleteIJ(i, j);//ɾ����i��j��
				for (vector<cStudent*>::iterator iter = mArrangePersonMatrix[r][c].begin(); iter != mArrangePersonMatrix[r][c].end(); iter++)
				{
					if (*iter == mStudentArray[n])
					{
						mArrangePersonMatrix[r][c].erase(iter);
						break;
					}
				}
				mStudentArray[n]->mppTimeArrangeMatrix[r][c] = 0;//���ž������
				mStudentArray[n]->mLeftSpareTime = mStudentArray[n]->mSpareTime + mWorkTime[r];//ʣ�����ʱ�����
				mStudentArray[n]->mCourseCount--;//����ۼ�
				mStudentArray[n]->mWorkTime -= mWorkTime[r];//��ʱ�ۼ�
				mStudentArray[n]->UnPressAndDeleteIJ(r, c);//ɾ����r��c��

				mArrangePersonMatrix[i][j].push_back(mStudentArray[n]);//����k��
				mStudentArray[n]->mppTimeArrangeMatrix[i][j] = 1;//���ž������
				mStudentArray[n]->mLeftSpareTime = mStudentArray[n]->mSpareTime - mWorkTime[i];//ʣ�����ʱ�����
				mStudentArray[n]->mCourseCount++;//����ۼ�
				mStudentArray[n]->mWorkTime += mWorkTime[i];//��ʱ�ۼ�
				mStudentArray[n]->PressAndAddIJ(i, j);//��ӣ�i��j��

				mArrangePersonMatrix[r][c].push_back(mStudentArray[k]);//����k��
				mStudentArray[k]->mppTimeArrangeMatrix[r][c] = 1;//���ž������
				mStudentArray[k]->mLeftSpareTime = mStudentArray[k]->mSpareTime - mWorkTime[r];//ʣ�����ʱ�����
				mStudentArray[k]->mCourseCount++;//����ۼ�
				mStudentArray[k]->mWorkTime += mWorkTime[r];//��ʱ�ۼ�
				mStudentArray[k]->PressAndAddIJ(r, c);//��ӣ�r��c��
				return true;
			}
		}
		return false;
	}

	bool ChangeScheduleAsPerson()//����Ϊ�棬˵�����˻�����
	{
		int Count = 0;//��¼�������
		int TypeFlag = 0;//��ʶ�Ƿ����ͬ���͵İ��
		int RecordJ;//��¼��һ��ͬ���͵İ��
		bool UnDone = true;
		for (int k = 0; k < mStudnetNumber; k++)
		{
			for (int i = 0; i < mScheduleCount; i++)
			{
				TypeFlag = 0; UnDone = true;
				for (int j = 0; j < mDayCount&&UnDone; j++)
				{
					if (i == mLinkI&&j == mLinkJ)
						continue;//�����������
					if (mStudentArray[k]->mppTimeArrangeMatrix[i][j] == 1)
					{
						TypeFlag++;
						RecordJ = j;
						if (TypeFlag >= 2)//������ͬ���͵İ��,׼������
						{
							for (int n = 0; n < mStudnetNumber; n++)
							{
								if (n != k)
								{
									if (mStudentArray[n]->mppSpareTimeMatrix[i][j] == 1)
									{
										if (ChangeScheduleIJ(k, n, i, j, RecordJ, mDifficultI)) //����n�ܷ�k�ģ�i��j�����
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
											if (ChangeScheduleIJ(k, n, i, RecordJ, j, mDifficultI)) //����n�ܷ�k�ģ�i��RecordJ�����
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
	//������յ��Ű��
	void ShowFinalArrangeSchedule()
	{
		//string time[7] = { "10:00-12:30|", "11:30-13:00|", "12:30-16:00|", "16:00-18:30|", "17:00-18:30|", "18:30-21:00|", "21:00-22:45|" };
		string time[5] = { "10:00-12:30|", "12:30-16:00|", "16:00-18:30|", "18:30-21:00|", "21:00-22:45|" };
		int count = 0;
		cout << "..........................................................................." << endl;
		cout << "***********|" << " ����һ |" << " ���ڶ� |" << " ������ |" << " ������ |" << " ������ |";
		cout << " ������ |" << " ������ |" << endl;
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

	void ShowFinalSArrangeSchedule()//�������������
	{
		//string time[7] = { "10:00-12:30|", "11:30-13:00|", "12:30-16:00|", "16:00-18:30|", "17:00-18:30|", "18:30-21:00|", "21:00-22:45|" };
		string time[5] = { "10:00-12:30|", "12:30-16:00|", "16:00-18:30|", "18:30-21:00|", "21:00-22:45|" };
		int count = 0;
		cout << "..........................................................................." << endl;
		cout << "***********|" << " ����һ |" << " ���ڶ� |" << " ������ |" << " ������ |" << " ������ |";
		cout << " ������ |" << " ������ |" << endl;
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
		cout << "***********|" << " ����һ |" << " ���ڶ� |" << " ������ |" << " ������ |" << " ������ |";
		cout << " ������ |" << " ������ |" << endl;
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
			cout << ":" << "��ʱ " << mStudentArray[i]->mWorkTime << " " << "��� " << mStudentArray[i]->mCourseCount << endl;
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
			cout << ": ---�ܰ��:" << size << " ";
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
	cout << "������Ա��������";
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
		Admin.ArrangeSTimeSchedule();//��������������
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