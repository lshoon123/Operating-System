#include<stdio.h>
#include<stdlib.h>

int totaltime=0;

void choose(int **box,int n)
{
	int i,j =0,a;
	int temp;
	for (i = 0; i < n - 1; i++)
	{
		for (j = 0; j < n - 1; j++)
		{
			if (box[j][1] > box[j + 1][1])
			{
				temp = box[j][1];
				box[j][1] = box[j+1][1];
				box[j + 1][1] = temp;
				temp = box[j][0];
				box[j][0] = box[j + 1][0];
				box[j + 1][0] = temp;
				temp = box[j][2];
				box[j][2] = box[j + 1][2];
				box[j + 1][2] = temp;
			}
		}
	}
	for (i = 0; i < n - 1; i++)
	{
		for (j = 0; j < n - 1; j++)
		{
			if (box[j][1] == box[j + 1][1])
			{
				if (box[j][0] > box[j + 1][0])
				{
					temp = box[j][1];
					box[j][1] = box[j + 1][1];
					box[j + 1][1] = temp;
					temp = box[j][0];
					box[j][0] = box[j + 1][0];
					box[j + 1][0] = temp;
					temp = box[j][2];
					box[j][2] = box[j + 1][2];
					box[j + 1][2] = temp;
				}
			}
		}
	}
}

void cal(int **box,int n, FILE *oup)
{
	int i,m;
	int wait=0;
	for (i = 0; i < n;i++)
	{
		if (box[i][1] == 0 && totaltime == 0)
		{
			totaltime += box[i][2];
		}
		else if (box[i][1] != 0 && totaltime == 0)
		{
			totaltime = box[i][2];
		}
		else if (totaltime < box[i][1])
		{
			m = box[i][1] - totaltime;
			totaltime = totaltime + m + box[i][2];
		}
		else
		{
			wait = wait + totaltime - box[i][1];
			totaltime = totaltime + box[i][2];
		}
	}
	fprintf(oup,"%d", wait);
}

int main()
{
	FILE *inp, *oup;
	inp = fopen("fcfs.inp","r");
	oup = fopen("fcfs.out", "w");
	int **box;
	int n,i,j,k;
	fscanf(inp, "%d\n", &n);
	box = (int**)malloc(sizeof(int*)*n);
	for (k = 0; k<n; k++) {
		box[k] = (int*)malloc(sizeof(int) * 3);
	}
	for (i=0;i<n;i++)
	{
		fscanf(inp,"%d %d %d\n",&box[i][0],&box[i][1],&box[i][2]);
	}
	choose(box, n);
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < 3; j++)
		{
			printf("%d ", box[i][j]);
		}
		printf("\n");
	}
	cal(box,n,oup);
	fclose(inp);
	fclose(oup);
	return 0;

}

