#include<stdio.h>
#include<stdlib.h>
typedef struct ListNode					//메모리상태를 나타낼 리스트
{
	int address;
	int pid;
	int size;
	struct ListNode *link;
} ListNode;

typedef struct Input					//입력파일을 저장할 구조체
{
	int start_time;
	int time;
	int size;
	int end_time;
} Input;


void error(char* message)
{
	fprintf(stderr, "%s\n", message);
	exit(1);
}

void insert_node(ListNode** phead, ListNode* p, ListNode* new_node)
{
	if (*phead == NULL)
	{
		new_node->link = NULL;
		*phead = new_node;
	}
	else if (p == NULL)
	{
		new_node->link = *phead;
		*phead = new_node;
	}
	else
	{
		new_node->link = p->link;
		p->link = new_node;
	}
}

void remove_node(ListNode** phead, ListNode* p, ListNode* removed)
{
	if (p == NULL)
	{
		*phead = (*phead)->link;
	}
	else
		p->link = removed->link;
	free(removed);
}

void display(ListNode* head)
{
	ListNode* p = head;
	while (p != NULL)
	{
		printf("%d->", p->pid);
		p = p->link;
	}
	printf("\n");
}

ListNode* pid_minus(ListNode* head, int x)
{
	ListNode* p;
	p = head;
	while (p != NULL)
	{
		if (p->pid == x)
		{
			p->pid = -1;
		}
		p = p->link;
	}
}

ListNode *create_node(int address, int pid, int size, ListNode* link)
{
	ListNode* new_node;
	new_node = (ListNode*)malloc(sizeof(ListNode));
	if (new_node == NULL)
	{
		error("메모리 할당 에러");
	}
	new_node->address = address;
	new_node->pid = pid;
	new_node->size = size;
	new_node->link = link;
	return(new_node);
}
int last_end_time(Input input[1000], int num)
{
	int i = 0;
	int long_time = 0;

	for (i = 0; i<num; i++)
	{
		if (long_time < input[i].end_time)
			long_time = input[i].end_time;
	}

	return long_time;
}
void Firstfit(Input input[1000], int num, int last_time, ListNode* head)
{
	int i = 0, j = 0, k = 0;
	int system_time = 0;
	ListNode* first;
	first = head;

	while (system_time <= last_time)
	{
		for (i = 0; i<num; i++)
		{
			if (system_time == input[i].end_time)				//메모리 할당이 끝나는 시간
			{
				if (first->pid == -1 && first->size >= input[i].size)
				{
					pid_minus(first, i);
				}
			}
			else if (system_time == input[i].start_time)			//메모리 할당이 필요한 시간
			{
				if (first->pid == -1 && first->size >= input[i].size)
				{
					//리스트삽입

				}
				else
					first = first->link;
			}

		}
	}
}
int main(void)
{
	int i = 0, j = 0, k = 0;
	int last_time = 0;
	int num = 0;									//입력 개수
	Input input[1000];							//입력을 저장해 놓을 배열
	ListNode *firstfit = NULL;

	FILE* file1 = fopen("allocation.inp", "rt");
	FILE* file2 = fopen("allocation.out", "wt");

	fscanf(file1, "%d", &num);					//입력 개수 받기

	for (i = 0; i<num; i++)							//입력 받기
	{
		fscanf(file1, "%d %d %d", &input[i].start_time, &input[i].time, &input[i].size);
		input[i].end_time = input[i].start_time + input[i].time;
	}

	last_time = last_end_time(input, num);			//제일마지막end time구하기
	fprintf(file2, "%d", last_time);

	return 0;
}