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

int deadlockcheck(banker *Allocation, banker *Need, banker Available, int a, int b, banker Request, int pn)//����� ���� �˰���
{
	banker AvailableCopy;
	banker AllocationCopy[100];
	banker NeedCopy[100];
	
	int i, j, count = 0;
	int finish[100] = { 0, }; // �������� üũ�ϴ� ����
	for (i = 0; i < a; i++)//Allocation�� Neeed ����
	{
		for (j = 0; j < b; j++)
		{
			AllocationCopy[i].A[j] = Allocation[i].A[j];
			NeedCopy[i].A[j] = Need[i].A[j];
		}
	}
	for (j = 0; j < b; j++)//Available ����
	{
		AvailableCopy.A[j] = Available.A[j];
	}

	for (j = 0; j < b; j++)//�ڿ��� ��û�Ѵ�.
	{

		AllocationCopy[pn].A[j] += Request.A[j];
		AvailableCopy.A[j] -= Request.A[j];
		NeedCopy[pn].A[j] -= Request.A[j];
	}

	for (i = 0; i < a; i++)//�ùķ��̼� �˰���
	{
		for (j = 0; j < b; j++)
		{
			if (NeedCopy[i].A[j] >= 0 && NeedCopy[i].A[j] <= AvailableCopy.A[j] && finish[i] == 0 && Allocation[i].n != 51)//�̿밡�����ڿ��� �ʿ��� �ڿ����� ���� ���� �˻��Ѵ�.
			{
				count++;
			}
		}
		if (count == b)//������ �����ϸ�
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
	for (i = 0; i < a; i++)//finish�� ���� 1���� �˻�
	{
		if (finish[i] == 1)
		{
			count++;
		}
	}
	if (count == a)
	{
		return 1;//���̸� 1
	}
	else
	{
		return 0;//���� ���� �����̸� 0
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
		fscanf(input, "%s %d", check,&pn);//request,release ����
		for (i = 0; i < b; i++)
		{
			fscanf(input, "%d", &Request.A[i]);
		}
		if (strcmp(check,check1) == 0)//request�� ���
		{
			for (i = 0; i < b; i++)//��û�� �����̿��Ҽ��ִ� �ڿ����� ������ �˻�
			{
				if (Available.A[i] >= Request.A[i])
				{
					count++;
				}
			}
			for (i = 0; i < b; i++)//�ʿ䰪���� ��û�� �� ������ �˻�
			{
				if (Need[pn].A[i] >= Request.A[i])
				{
					count1++;
				}
			}
			if (count == b && count1 == b)//���� �������� �����Ұ��
			{
				dead = deadlockcheck(Allocation, Need, Available, a, b, Request, pn);//����� üũ�� ���� �ڿ��� �Ҵ��ص� �Ǵ��� �˻��Ѵ�.
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
			else if(count1 == b)//�������Ұ�� Waitť�� �ִ´�.
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
		else if (strcmp(check, check2) == 0)//release�� ���
		{
			for (i = 0; i < b;i++)//�ش� ���μ����� ��� �ڿ� ����
			{
				Available.A[i] += Request.A[i];
				Allocation[pn].A[i] -= Request.A[i];
			}			
			for (i = 0; i < b; i++)//������ȯ���� �ƴ��� üũ
			{
				if (Allocation[pn].A[i] == 0)//����
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
				for (i = 0; i < b; i++)//�ش� ���μ����� ��� �ڿ� ����
				{
					Need[pn].A[i] += Request.A[i];
				}
		//	}
			if (k >= 1)//Wait ť�� ������� ��û�� �������
			{
				for (;;)//�ϴ� ť�˻縦 ���� ���Ѵ�� �ڿ��� ������.
				{
					for (q; q < k; q++)
					{
						for (i = 0; i < b; i++)
						{
							if (Available.A[i] >= Wait[q].A[i] && Wait[q].n != 51 && Wait[q].A[i] >= 0)//Wait ť���� �Ҵ��� ���������� üũ
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
		else if (strcmp(check, check3) == 0)//quit�� ��� ����
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
	for (i = 0; i < b;i++)//�ý��۰� �Ҵ� �׸��� ������Ʈ�� �ʱ�ȭ
	{
		fscanf(input, "%d", &Available.A[i]);
		Request.A[i] = 0;
	}
	for (i = 0; i < a;i++)//MAX�� �Ҵ�
	{
		for (j = 0; j < b;j++)
		{
			fscanf(input, "%d", &Max[i].A[j]);
		}
	}
	for (i = 0; i < a; i++)//Allocation�� �Ҵ�
	{
		for (j = 0; j < b; j++)
		{
			fscanf(input, "%d", &Allocation[i].A[j]);
		}
	}
	for (i = 0; i < a; i++)//Need�� �Ҵ�
	{
		for (j = 0; j < b; j++)
		{
			Need[i].A[j] = Max[i].A[j] - Allocation[i].A[j];
		}
	}
	for (i = 0; i < b; i++)//Available�� �Ҵ�
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