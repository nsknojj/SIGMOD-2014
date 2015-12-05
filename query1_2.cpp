/*
 *author:zz 
 *未判断输入，暂时只支持 1k person
 */
#include "q1.cpp"
#include "q2.cpp"
#include "var.h"

#include<stdio.h>
#include<map>
#include<memory.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/mman.h>
#include<sys/stat.h>
#include<string.h>
#include <sys/types.h>
#include <fcntl.h>
using namespace std;

extern int path_len;
extern char *q_dir,d_dir[C_Path_Len];
void parse1(char *data,int *a, int *b, int *x)
{
	char *tmp = data + 7;
	int num,flag = 1;
	num = 0;
	while(*tmp != ',')
	{
		num = num * 10 + *tmp - '0';
		tmp++;
	}
	*a = num;
	tmp += 2;
	num = 0;
	while(*tmp != ',')
	{
		num = num * 10 + *tmp - '0';
		tmp++;
	}
	*b = num;
	tmp += 2;
	num = 0;
	if(*tmp == '-')
	{
		flag = -1;
		tmp++;
	}
	while(*tmp != ')')
	{
		num = num * 10 + *tmp - '0';
		tmp++;
	}
	*x = num * flag;
}
void parse2(char *data,int *k, int *d)
{
	char *tmp = data + 7;
	int num,flag = 1;
	num = 0;
	while(*tmp != ',')
	{
		num = num * 10 + *tmp - '0';
		tmp++;
	}
	*k = num;
	tmp += 2;
	num = (tmp[0] - '0') * 10000000 + (tmp[1] - '0') * 1000000 + (tmp[2] - '0') * 100000 + (tmp[3] - '0') * 10000 + (tmp[5] - '0') * 1000 + (tmp[6] - '0') * 100 + (tmp[8] - '0') * 10 + (tmp[9] - '0');
	*d = num;
}
int main(int argc,char *argv[])
{
	strcpy(d_dir,argv[1]);
	printf("%s\n",d_dir);
	path_len = strlen(argv[1]);
	
	q_dir = argv[2];		//get query dir
	initialize_1();			//initialize for query1
	initialize_2();			//initialize for query2
	int a,b,x,res,k,d;
	size_t query_len;
	char *input_query;
	int fd;	
	struct stat info;
	/* loading query file */
	if((fd = open(q_dir,O_RDONLY,0)) < 0)
	{
		printf("open query file err:%s\n",q_dir);
		exit(0);
	}
	fstat(fd, &info);
	query_len = info.st_size;
	input_query = (char*)mmap(NULL,info.st_size, PROT_READ, MAP_PRIVATE,fd, 0);
	close(fd);
	freopen("query1_2.out", "w", stdout);
	char *cur = input_query;
	char *line_end;
	while(1)
	{		
		switch(cur[5])
		{
		case '1':
			parse1(cur,&a,&b,&x);
			res = query1(a,b,x);	
			printf("%d\n",res);			
			break;
		case '2':
			parse2(cur,&k,&d);
			query2(k,d);
			break;
		default:break;
		}
		if((line_end = strstr(cur,"\n")) == NULL)
		{
			break;
		}
		cur = line_end + 1;
	}
	return 0;
}





