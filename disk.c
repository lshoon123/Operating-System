#include<stdio.h>
#include<stdlib.h>

typedef struct disk
{
	int time;
	int head;
	int check;//sstf에서 가까운거 아닌거 체크하는 변수
}disk;

void FCFS(FILE *output,int requesttime, disk *disk)//먼저 들어온 대로 요청처리
{
	int time=0,movetime=0;
	int head = 0;
	int i;
	for (i=0;;i++)
	{
		if (disk[i].time == -1)//마지막부분이면 종료
		{
			break;
		}
		if (time<disk[i].time)// 명령요청시간이 현재시간보다 클경우 현재시간을 요청시간으로 맞춰준다.
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

void SSTF(FILE *output, int requesttime, disk *disk,int last)//헤더에서 거리가 가장 가까운 쪽으로 이동하는 함수
{
	int i = 0,j, check,time =0,movetime =0;
	int minimum=199, head=0,standard;
	int point=0;//대기큐에 명령이 안들어오는지 안들어왔는지 체크
	for (i = 0;i<last; i++)
	{
		
		for (j = 0; j < last; j++)
		{
			standard = disk[j].head - head;//절대값으로 바꿔줌
			if (standard < 0)
			{
				standard *= -1;
			}
			if (minimum>standard && time>=disk[j].time && disk[j].check ==0)//거리가 가장작은 입력값 찾기
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
		else//대기큐에 요청이 없을경우
		{
			standard = disk[i].head - head;//절대값으로 바꿔줌
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
		if (way == 1)//내리막
		{
			closer = 0;
			for (j = 0; j < last; j++)//큰쪽으로 갈때 가장 가까운거 체크해준다.
			{
				if (closer <= disk[j].head && time >= disk[j].time && disk[j].check == 0 && head >= disk[j].head)
				{
					closer = disk[j].head;
					check = j;
					point = 1;
				}
			}
		}
		else//오르막
		{
			for (j = 0; j < last; j++)//큰쪽으로 갈때 가장 가까운거 체크해준다.
			{
				if (closer >= disk[j].head && time >= disk[j].time && disk[j].check == 0 && head <= disk[j].head)
				{
					closer = disk[j].head;
					check = j;
					point = 1;
				}
			}
		}

		movetime = disk[check].head - head;//절대값으로 바꿔줌
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
			if (disk[i].time > time)//시간이 늦게들어올 경우
			{
				if (disk[i].head < head && way == 0)
				{
					way = 1;
				}
				movetime = disk[i].head - head;//절대값으로 바꿔줌
				if (movetime < 0)
				{
					movetime *= -1;
				}
				time = disk[i].time + movetime + requesttime;
				head = disk[i].head;
				disk[i].check = 1;
			}
			else//시간이 늦게 들어온게 아닐경우
			{
				if (way == 0)//최대로 올라간 경우
				{
					closer = 0;
					for (j = 0; j < last; j++)//큰쪽으로 갈때 가장 가까운거 체크해준다.
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
					movetime = disk[check].head - head;//절대값으로 바꿔줌
					if (movetime < 0)
					{
						movetime *= -1;
					}
					time += (movetime + requesttime);
					head = disk[check].head;
					disk[check].check = 1;
					way = 1;
				}
				else//끝까지 내려간 경우
				{
					way = 0;
					closer = 199;
					for (j = 0; j < last; j++)//큰쪽으로 갈때 가장 가까운거 체크해준다.
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
					movetime = disk[check].head - head;//절대값으로 바꿔줌
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
	
	for (i=0;;i++)//값 받기
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