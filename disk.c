#include<stdio.h>
#include<stdlib.h>

typedef struct disk
{
	int time;
	int head;
	int check;//sstf���� ������ �ƴѰ� üũ�ϴ� ����
}disk;

void FCFS(FILE *output,int requesttime, disk *disk)//���� ���� ��� ��ûó��
{
	int time=0,movetime=0;
	int head = 0;
	int i;
	for (i=0;;i++)
	{
		if (disk[i].time == -1)//�������κ��̸� ����
		{
			break;
		}
		if (time<disk[i].time)// ��ɿ�û�ð��� ����ð����� Ŭ��� ����ð��� ��û�ð����� �����ش�.
		{
			time = disk[i].time;
		}
		movetime = disk[i].head - head;
		head = disk[i].head;
		if (movetime >= 0)
		{
			time += (movetime+requesttime);
		}
		else
		{
			time = time - movetime+requesttime;
		}
	}
	fprintf(output,"%d %d", time, head);
}

void SSTF(FILE *output, int requesttime, disk *disk,int last)//������� �Ÿ��� ���� ����� ������ �̵��ϴ� �Լ�
{
	int i = 0,j, check,time =0,movetime =0;
	int minimum=199, head=0,standard;
	int point=0;//���ť�� ����� �ȵ������� �ȵ��Դ��� üũ
	for (i = 0;i<last; i++)
	{
		
		for (j = 0; j < last; j++)
		{
			standard = disk[j].head - head;//���밪���� �ٲ���
			if (standard < 0)
			{
				standard *= -1;
			}
			if (minimum>standard && time>=disk[j].time && disk[j].check ==0)//�Ÿ��� �������� �Է°� ã��
			{
				minimum = standard;
				check = j;
				point = 1;
			}
		}
		if (point ==1)
		{
			movetime = minimum;
			head = disk[check].head;
			disk[check].check = 1;
			time += (movetime + requesttime);
		}
		else//���ť�� ��û�� �������
		{
			standard = disk[i].head - head;//���밪���� �ٲ���
			if (standard < 0)
			{
				standard *= -1;
			}
			time = disk[i].time+standard+requesttime;
			head = disk[i].head;
			disk[i].check = 1;
		}
		check = 0;
		minimum = 199;
		point = 0;
	}
	fprintf(output, "%d %d", time, head);
}

void LOOK(FILE *output, int requesttime, disk *disk, int last)
{
	int i = 0, j, check, time = 0, movetime = 0;
	int head = 0, closer = 199, point = 0, way =0, standard=0;

	for (i = 0; i<last; i++)
	{
		if (way == 1)//������
		{
			closer = 0;
			for (j = 0; j < last; j++)//ū������ ���� ���� ������ üũ���ش�.
			{
				if (closer <= disk[j].head && time >= disk[j].time && disk[j].check == 0 && head >= disk[j].head)
				{
					closer = disk[j].head;
					check = j;
					point = 1;
				}
			}
		}
		else//������
		{
			for (j = 0; j < last; j++)//ū������ ���� ���� ������ üũ���ش�.
			{
				if (closer >= disk[j].head && time >= disk[j].time && disk[j].check == 0 && head <= disk[j].head)
				{
					closer = disk[j].head;
					check = j;
					point = 1;
				}
			}
		}

		movetime = disk[check].head - head;//���밪���� �ٲ���
		if (movetime < 0)
		{
			movetime *= -1;
		}
		if (point == 1)
		{
			time += (movetime + requesttime);
			head = disk[check].head;
			disk[check].check = 1;
		}
		else
		{
			if (disk[i].time > time)//�ð��� �ʰԵ��� ���
			{
				if (disk[i].head < head && way == 0)
				{
					way = 1;
				}
				movetime = disk[i].head - head;//���밪���� �ٲ���
				if (movetime < 0)
				{
					movetime *= -1;
				}
				time = disk[i].time + movetime + requesttime;
				head = disk[i].head;
				disk[i].check = 1;
			}
			else//�ð��� �ʰ� ���°� �ƴҰ��
			{
				if (way == 0)//�ִ�� �ö� ���
				{
					closer = 0;
					for (j = 0; j < last; j++)//ū������ ���� ���� ������ üũ���ش�.
					{
						if (closer <= disk[j].head && disk[j].check == 0)
						{
							standard = disk[j].head - head;
							if (standard < 0)
							{
								standard *= -1;
							}
							if (time+standard > disk[j].time)
							{
								closer = disk[j].head;
								check = j;
							}							
						}
					}
					movetime = disk[check].head - head;//���밪���� �ٲ���
					if (movetime < 0)
					{
						movetime *= -1;
					}
					time += (movetime + requesttime);
					head = disk[check].head;
					disk[check].check = 1;
					way = 1;
				}
				else//������ ������ ���
				{
					way = 0;
					closer = 199;
					for (j = 0; j < last; j++)//ū������ ���� ���� ������ üũ���ش�.
					{
						if (closer >= disk[j].head && disk[j].check == 0)
						{
							standard = disk[j].head - head;
							if (standard < 0)
							{
								standard *= -1;
							}
							if (time + standard > disk[j].time)
							{
								closer = disk[j].head;
								check = j;
							}							
						}
					}
					movetime = disk[check].head - head;//���밪���� �ٲ���
					if (movetime < 0)
					{
						movetime *= -1;
					}
					time += (movetime + requesttime);
					head = disk[check].head;
					disk[check].check = 1;
				}
			}
		}
		point = 0;
		closer = 199;
	}
	fprintf(output, "%d %d", time, head);
}

int main()
{
	FILE *input, *output;
	input = fopen("disk.inp", "r");
	output = fopen("disk.out", "w");

	disk disk[100];
	int requesttime, i = 0;
	char type[5];

	for (i = 0; i < 100;i++)
	{
		disk[i].check = 0;
	}

	

	fscanf(input,"%s %d",type,&requesttime);
	
	for (i=0;;i++)//�� �ޱ�
	{
		fscanf(input,"%d %d",&disk[i].time,&disk[i].head);
		if (disk[i].time == -1 && disk[i].head == -1)
		{
			break;
		}
	}
	
	if(type[0] == 'F')
	{
		FCFS(output,requesttime,disk);
	}
	else if (type[0] == 'S')
	{
		SSTF(output, requesttime, disk,i);
	}
	else
	{
		LOOK(output, requesttime, disk, i);
	}



	fclose(input);
	fclose(output);
	return 0;
}