#include<stdio.h>
#include<stdlib.h>
#include<string.h>

char check[8];
char check1[]="request";
char check2[] = "release";
char check3[] = "quit";

typedef struct banker
{
	int A[100];
	int n;
}banker;

int deadlockcheck(banker *Allocation, banker *Need, banker Available, int a, int b, banker Request, int pn)//데드락 판정 알고리즘
{
	banker AvailableCopy;
	banker AllocationCopy[100];
	banker NeedCopy[100];
	
	int i, j, count = 0;
	int finish[100] = { 0, }; // 가능한지 체크하는 변수
	for (i = 0; i < a; i++)//Allocation과 Neeed 복사
	{
		for (j = 0; j < b; j++)
		{
			AllocationCopy[i].A[j] = Allocation[i].A[j];
			NeedCopy[i].A[j] = Need[i].A[j];
		}
	}
	for (j = 0; j < b; j++)//Available 복사
	{
		AvailableCopy.A[j] = Available.A[j];
	}

	for (j = 0; j < b; j++)//자원을 요청한다.
	{

		AllocationCopy[pn].A[j] += Request.A[j];
		AvailableCopy.A[j] -= Request.A[j];
		NeedCopy[pn].A[j] -= Request.A[j];
	}

	for (i = 0; i < a; i++)//시뮬레이션 알고리즘
	{
		for (j = 0; j < b; j++)
		{
			if (NeedCopy[i].A[j] >= 0 && NeedCopy[i].A[j] <= AvailableCopy.A[j] && finish[i] == 0 && Allocation[i].n != 51)//이용가능한자원이 필요한 자원보다 많은 지를 검사한다.
			{
				count++;
			}
		}
		if (count == b)//조건이 만족하면
		{
			finish[i] = 1;
			for (int k = 0; k < b; k++)
			{
				AvailableCopy.A[k] += AllocationCopy[i].A[k] + NeedCopy[i].A[k];
				AllocationCopy[i].A[k] = 0;
				NeedCopy[i].A[k] = 0;
			}
			i = -1;
		}
		count = 0;
	}
	for (i = 0; i < a; i++)//finish가 전부 1인지 검사
	{
		if (finish[i] == 1)
		{
			count++;
		}
	}
	if (count == a)
	{
		return 1;//참이면 1
	}
	else
	{
		return 0;//거짓 교착 상태이면 0
	}
}

void judge(FILE *input, FILE *output,int a, int b,banker Request, banker Available, banker *Need, banker *Allocation, banker *Wait, banker *Max)
{
	int i,j;
	int k = 0;
	int q = 0;
	int pn;
	int count =0;
	int count1 = 0;
	int count2 = 0;
	int stop=0;
	int pp;
	int dead;
	int count4 = 0;
	for (;;)
	{
		fscanf(input, "%s %d", check,&pn);//request,release 구분
		for (i = 0; i < b; i++)
		{
			fscanf(input, "%d", &Request.A[i]);
		}
		if (strcmp(check,check1) == 0)//request일 경우
		{
			for (i = 0; i < b; i++)//요청이 현재이용할수있는 자원보다 많은지 검사
			{
				if (Available.A[i] >= Request.A[i])
				{
					count++;
				}
			}
			for (i = 0; i < b; i++)//필요값보다 요청이 더 많은지 검사
			{
				if (Need[pn].A[i] >= Request.A[i])
				{
					count1++;
				}
			}
			if (count == b && count1 == b)//위의 두조건을 만족할경우
			{
				dead = deadlockcheck(Allocation, Need, Available, a, b, Request, pn);//데드락 체크를 통해 자원을 할당해도 되는지 검사한다.
				if (dead == 1)
				{
					for (i = 0; i < b; i++)
					{
						Allocation[pn].A[i] += Request.A[i];
						Available.A[i] -= Request.A[i];
						Need[pn].A[i] -= Request.A[i];
					}
				}
				else
				{
					for (i = 0; i < b; i++)
					{
						Wait[k].A[i] = Request.A[i];
					}
					Wait[k].n = pn;
					k++;
				}
			}
			else if(count1 == b)//만족못할경우 Wait큐에 넣는다.
			{
				for (i = 0; i < b;i++)
				{
					Wait[k].A[i] = Request.A[i];
				}
				Wait[k].n = pn;
				k++;				
			}
			for (i = 0; i < b; i++)
			{
				fprintf(output, "%d ", Available.A[i]);
			}
			fprintf(output, "\n");
		}
		else if (strcmp(check, check2) == 0)//release일 경우
		{
			for (i = 0; i < b;i++)//해당 프로세스의 사용 자원 조정
			{
				Available.A[i] += Request.A[i];
				Allocation[pn].A[i] -= Request.A[i];
			}			
			for (i = 0; i < b; i++)//완전반환인지 아닌지 체크
			{
				if (Allocation[pn].A[i] == 0)//완전
				{
					count4++;
				}
			}
			/*if (count4 == b)
			{
				Allocation[pn].n = 51;
			}*/
			//else
			//{
				for (i = 0; i < b; i++)//해당 프로세스의 사용 자원 조정
				{
					Need[pn].A[i] += Request.A[i];
				}
		//	}
			if (k >= 1)//Wait 큐에 대기중인 요청이 있을경우
			{
				for (;;)//일단 큐검사를 위해 무한대로 자원을 돌린다.
				{
					for (q; q < k; q++)
					{
						for (i = 0; i < b; i++)
						{
							if (Available.A[i] >= Wait[q].A[i] && Wait[q].n != 51 && Wait[q].A[i] >= 0)//Wait 큐에서 할당이 가능한지를 체크
							{
								count++;
							}
						}
						if (count == b)
						{
							dead = 0;
							dead = deadlockcheck(Allocation, Need, Available, a, b, Wait[q], Wait[q].n);
							if (dead == 1)
							{
								pp = Wait[q].n;
								for (i = 0; i < b; i++)
								{
									Allocation[pp].A[i] += Wait[q].A[i];
									Available.A[i] -= Wait[q].A[i];
									Need[pp].A[i] -= Wait[q].A[i];
									Wait[q].A[i] = 0;
								}
								Wait[q].n = 51;
								q = -1;
							}
						}
						if (q == k - 1)
						{
							stop = 1;
						}
						count = 0;
					}
					if (stop == 1)
					{
						stop = 0;
						q = 0;
						break;
					}
					else if (q == k)
					{
						q = 0;
						k = 0;
						for (i = 0; i < a; i++)
						{
							Wait[i].n = 0;
						}
						break;
					}
				}
			}
			for (i = 0; i < b; i++)
			{
				fprintf(output, "%d ", Available.A[i]);
			}
			fprintf(output, "\n");
		}
		else if (strcmp(check, check3) == 0)//quit일 경우 종료
		{
			break;
		}
		count = 0;
		count1 = 0;
		count4 = 0;
	}
}



int main()
{
	FILE *input = fopen("banker.inp", "r");
	FILE *output = fopen("banker.out", "w");
	
	banker Available, Max[100], Allocation[100], Need[100],Wait[1000],Request;//need =  max-allocation
	int a, b;
	int i,j;
	fscanf(input,"%d %d\n",&a,&b);
	for (i = 0; i < b;i++)//시스템값 할당 그리고 리퀘스트값 초기화
	{
		fscanf(input, "%d", &Available.A[i]);
		Request.A[i] = 0;
	}
	for (i = 0; i < a;i++)//MAX값 할당
	{
		for (j = 0; j < b;j++)
		{
			fscanf(input, "%d", &Max[i].A[j]);
		}
	}
	for (i = 0; i < a; i++)//Allocation값 할당
	{
		for (j = 0; j < b; j++)
		{
			fscanf(input, "%d", &Allocation[i].A[j]);
		}
	}
	for (i = 0; i < a; i++)//Need값 할당
	{
		for (j = 0; j < b; j++)
		{
			Need[i].A[j] = Max[i].A[j] - Allocation[i].A[j];
		}
	}
	for (i = 0; i < b; i++)//Available값 할당
	{
		for (j = 0; j < a; j++)
		{
			Available.A[i] -= Allocation[j].A[i];
		}
	}

	judge(input, output, a, b, Request, Available, Need, Allocation, Wait, Max);


	fclose(input);
	fclose(output);
	return 0;
}