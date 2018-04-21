#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<algorithm>
using namespace std;
int a[9][9], ans[9][9];
int m = 0, sum = 0, n,flag;
char c;
int str[9] = { 0, 3, 6, 1, 4, 7, 2, 5, 8 };
int F[10] = { 0,2,1,3,4,5,6,7,8,9 };
int Fist[40000][10];
void result(int i,int j)//i和j分别表示当前所处的位置
{
	if (i == 9 && j == 0)
	{
		flag = 1;
		return;
	}
	if (a[i][j] != 0)//判断当前位置是否有数字，有则跳过
	{		ans[i][j] = a[i][j];
		if (j < 8)
			result(i, j + 1);
		else
			result(i + 1, 0);
		if (flag)
			return;
	}
	else
	{
		for (int k = 1,x,y; k <= 9; k++)
		{
			for (x = 0; x <= 8; x++)
				if (a[x][j] == k)
					break;//对第j列判断是否满足
			for (y = 0; y <= 8; y++)
				if (a[i][y] == k)
					break;//对第i行判断是否满足
			if (y == 9 && x == 9)
			{
				for (x = (i / 3) * 3; x <= (i / 3) * 3 + 2; x++)
				{
					for (y = (j / 3) * 3; y <= (j / 3) * 3 + 2; y++)
					{
						if (a[x][y] == k)
							y = x = 100;
					}
				}//对std[i][j]所在的3*3区域判断是否满足
		   if (x == (i / 3) * 3 + 3 && y == (j / 3) * 3 + 3)
				  {//
					  a[i][j] = k;
					  ans[i][j] = a[i][j];
					  if (j < 8)
						  result(i, j + 1);
					  else
						  result(i + 1, 0);//将k填入当前位置并继续调用回溯函数
					  if (flag)
						  return;//如果已经找到一组解则不再继续递归下去
					  a[i][j] = 0;
				  }
			}
		}
	}
}
void solve(FILE*read)
{
	FILE*write = fopen("stdku.txt", "w");
	int i=0,j=0,first=0;
	char ch;
	while (!feof(read))//读取目标文件
	{
		ch = fgetc(read);
        if (ch == ' ')
			continue;
		else if (ch >= '0'&&ch <= '9')
		{
			a[i][j] = ch - '0';
			j++;
			if (i == 8 && j == 9)
			{
				flag = 0;
				result(0,0);
				if (first)
				{
					fputc('\n', write);
					fputc('\n', write);
				}
				for (int x = 0; x <= 8; x++)
					for (int y = 0; y <= 8; y++)//将答案写入stdku.txt文件
					{	fputc(ans[x][y]+'0', write);
						if (y < 8)
							fputc(' ', write);
						else if (x < 8)
							fputc('\n', write);
					}
				first = 1;
				i = j = 0;
			}
		}
		else if (ch == '\n')
		{
			if (j == 9)
			{
				i++;
				j = 0;
			}
		}
	}
	fclose(write);
}

void quanpai(int x)//生成8位全排列，首位固定，x为第几位数字，每次从还没被选用的数字里选用新数字
{
	if (x == 9)
	{
		for (int i = 1; i <= 9; i++)
			Fist[m][i] = F[i];
		m++;
		return ;
	}
	else
	{
		int k;
		for (int i = x; i <=9; i++)
		{
			k = F[x];
			F[x] = F[i];
			F[i] = k;
			quanpai(x + 1);
			k = F[i];
			F[i] = F[x];
			F[x] = k;
		}
		return ;
	}
}
void create()//创建数独终局
{
	quanpai(2);
	FILE* fp = fopen("stdku.txt", "w");
	for (int m = 0; m < 6; m++)
	{
		if (m)
			std::next_permutation(str + 3, str + 6);
		for (int h = 0; h < 6; h++)
		{
			if (h)
				std::next_permutation(str + 6, str + 9);
			for (int i = 0; i < m; i++)
			{
				for (int s = 0; s < 9; s++)
				{
					for (int j = 0; j <= 8; j++)
					{
						if (j)
							fputc(' ', fp);
						c = '0' + Fist[i][(j + str[s]) % 9 + 1];					
fputc(c, fp);
					}
					if (s < 8)
						fputc('\n', fp);
				}
				sum++;
				if (sum == n)
				{
					fclose(fp);
					return;
				}
				fputc('\n', fp);
				fputc('\n', fp);
			}
		}
	}
}

int main(int rec, char *rew[])//对rew[1]进行判断以确定是对应于哪种要求

{	
if (strcmp(rew[1], "-c") == 0)
	{
		n = atoi(rew[2]);
		if (n < 1)//处理不合理输入
			printf("ERROR:-c N to create N different stdku final-status.\n");
		else
		{
			double start_time = clock();
			create();
			printf("%d stdku final games created. Execution time : %.03lf s\n", 
				n, (clock() - start_time) / CLOCKS_PER_SEC);
		}
	}
	else if (strcmp(rew[1], "-s") == 0)
	{
		FILE*read = fopen(rew[2], "r");
		if (read == NULL)//处理文件路径不合法的情况
			printf("cannot open the file\n");
		else
		{
			double start_time = clock();
			solve(read);
			printf("those stdku puzzles solved. Execution time : %.03lf s\n",
				(clock() - start_time) / CLOCKS_PER_SEC);

		}
		fclose(read);
	}
	return 0;
}





