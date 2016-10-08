#include<stdio.h>
#include<stdlib.h>

int totaltime = 0;

void choose(int **box, int n)
{
	int i, j = 0, a;
	int temp;
	for (i = 0; i < n - 1; i++)
	{
		for (j = 0; j < n - 1; j++)
		{
			if (box[j][1] > box[j + 1][1])
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
	for (i = 0; i < n - 1; i++)
	{
		for (j = 0; j < n - 1; j++)
		{
			if (box[j][1] == box[j + 1][1])
			{
				if (box[j][2] > box[j + 1][2])
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
	for (i = 0; i < n - 1; i++)
	{
		for (j = 0; j < n - 1; j++)
		{
			if (box[j][1] == box[j + 1][1])
			{
				if (box[j][2] == box[j + 1][2])
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
}

void cal(int **box, int n, FILE *oup)
{
	int i, j, k,temp,m;
	int che = 0;
	int wait = 0;

	if (box[0][1] == 0 && totaltime == 0)
	{
		totaltime = box[0][2];
	}
	else if (box[0][1] != 0 && totaltime == 0)
	{
		totaltime = box[0][2];
	}

	for (i = 1; i < n; i++)
	{
		if (totaltime >= box[i][1])
		{
			for (j = i; j < n;j++)
			{
				if (box[j][1]<= totaltime)
				{
					che++;
				}
			}
			for (j = i; j < i + che-1; j++)
			{
				for (k = i; k < i + che-1; k++)
				{
					if (box[k][2] > box[k + 1][2])
					{
						temp = box[k][1];
						box[k][1] = box[k + 1][1];
						box[k + 1][1] = temp;
						temp = box[k][0];
						box[k][0] = box[k + 1][0];
						box[k + 1][0] = temp;
						temp = box[k][2];
						box[k][2] = box[k + 1][2];
						box[k + 1][2] = temp;
					}
				}
			}
			for (k = i; k < i + che-1; k++)
			{
				for (j = i; j < i + che-1; j++)
				{
					if (box[j][2] == box[j + 1][2])
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
			wait = wait + totaltime - box[i][1];
			totaltime = totaltime + box[i][2];
		}
		else if (totaltime < box[i][1])
		{
			m = box[i][1] - totaltime;
			totaltime = totaltime + m + box[i][2];
		}
		che = 0;
	}
	fprintf(oup, "%d", wait);
}

int main()
{
	FILE *inp, *oup;
	inp = fopen("sjf.inp", "r");
	oup = fopen("sjf.out", "w");
	int **box;
	int n, i, j, k;
	fscanf(inp, "%d\n", &n);
	box = (int**)malloc(sizeof(int*)*n);
	for (k = 0; k<n; k++) {
		box[k] = (int*)malloc(sizeof(int) * 3);
	}
	for (i = 0; i<n; i++)
	{
		fscanf(inp, "%d %d %d\n", &box[i][0], &box[i][1], &box[i][2]);
	}
	choose(box, n);
	/*for (i = 0; i < n; i++)
	{
		for (j = 0; j < 3; j++)
		{
			printf("%d ", box[i][j]);
		}
		printf("\n");
	}*/
	cal(box, n, oup);
	fclose(inp);
	fclose(oup);
	return 0;

}

