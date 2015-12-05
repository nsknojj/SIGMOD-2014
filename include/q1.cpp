/*
 *author:zz 
 *鏈垽鏂緭鍏ワ紝鏆傛椂鍙敮鎸?1k person
 */
#include<stdio.h>
#include<map>
#include<set>
#include<memory.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/mman.h>
#include<sys/stat.h>
#include<string.h>
#include <sys/types.h>
#include <fcntl.h>
#include "var.h"
#include<tr1/unordered_map>
using namespace std::tr1;
using namespace std;
unordered_map<int,int> getev[N];
/*extern char FN_Tag;
extern char FN_Person_hasTag;
extern char FN_Person;
extern int path_len;
extern char FN_Person_Knows;
extern char FN_Comment_Creator;
extern char FN_Comment_Reply;
extern char *q_dir,d_dir[];*/
void initialize_1()
{
	char *input;
	int i,id1,id2,tot,ev1,ev2,j,pid,cid,tot_p = 0,k;
	size_t len;
	//initialize person knows person
	if(!(input = mymap(FN_Person_Knows,&len)))
	{
		printf("open file err: %s\n",FN_Person_Knows);
		exit(0);
	}
	i = 20;
	tot = 0;
	memset(la,0,sizeof(la));
	memset(pre,0,sizeof(pre));
	while(i < len)
	{
		id1 = 0;		
		while(input[i] >= '0' && input[i] <= '9')
		{
			id1 = id1 * 10 + input[i] - '0';
			i++;
		}
		if(id1 > tot_p)
		{
			tot_p = id1;
		}
		i++;
		id2 = 0;
		while(input[i] >= '0' && input[i] <= '9')
		{
			id2 = id2 * 10 + input[i] - '0';
			i++;
		}
		if(id2 > tot_p)
		{
			tot_p = id2;
		}
		i++;
		if(id1 < id2)
		{
			ev1 = ++tot;						//ev1 id1 to id2
			ev[ev1] = id2;						//ev2 id2 to id1
			ev2 = ++tot;
			ev[ev2] = id1;
			getev[id1].insert(make_pair<int,int>(id2,ev1));
			getev[id2].insert(make_pair<int,int>(id1,ev2));
			comment[ev1] = comment[ev2] = 0;
			pre[ev1] = la[id1];
			pre[ev2] = la[id2];
			la[id1] = ev1;
			la[id2] = ev2;
		}
	}
	munmap(input, len);
	//initialize cid2pid
	if(!(input = mymap(FN_Comment_Creator,&len)))
	{
		printf("open file err: %s\n",FN_Comment_Creator);
		exit(0);
	}
	i = 21;
	unordered_map<int,int> cid2pid;
	int ma=0;
	cid2pid.clear();
	while(i < len)
	{
		cid = 0;		
		while(input[i] >= '0' && input[i] <= '9')
		{
			cid = cid * 10 + input[i] - '0';
			i++;
		}
		i++;
		pid = 0;
		while(input[i] >= '0' && input[i] <= '9')
		{
			pid = pid * 10 + input[i] - '0';
			i++;
		}
		i++;
		ma=max(cid,ma);
		cid2pid.insert(pair<int,int>(cid,pid));
	}
	//printf("aaaa%d\n",ma);
	munmap(input, len);
	//add comment
	if(!(input = mymap(FN_Comment_Reply,&len)))
	{
		printf("open file err: %s\n",FN_Comment_Reply);
		exit(0);
	}
	i = 22;
	unordered_map<int,int>::iterator l;
	while(i < len)
	{
		cid = 0;		
		while(input[i] >= '0' && input[i] <= '9')
		{
			cid = cid * 10 + input[i] - '0';
			i++;
		}
		i++;
		id1 = cid2pid[cid];
		cid = 0;
		while(input[i] >= '0' && input[i] <= '9')
		{
			cid = cid * 10 + input[i] - '0';
			i++;
		}
		i++;
		id2 = cid2pid[cid];
		l = getev[id1].find(id2);
		if(l != getev[id1].end())
		{
			comment[l->second]++;
		}
	}
	munmap(input, len);
	for(i = 1; i <= tot; i+=2)
	{
		if(comment[i] < comment[i+1])
		{
			comment[i+1] = comment[i];
		}
		else if(comment[i+1] < comment[i])
		{
			comment[i] = comment[i+1];
		}
	}
	
	for(i = 0; i <= tot_p; i++)				//sort comment
	{
		int j_num = 0;
		for(j = la[i]; j; j = pre[j])
		{
			j_num++;
		}
		for(k = j_num; k > 1; k--)
		{
			j = la[i];
			int prej = pre[j];
			if(prej && comment[prej] > comment[j])
			{
				la[i] = prej;
				pre[j] = pre[prej];
				pre[prej] = j;
			}
			int save_j = la[i];
			int flag = 1;
			for(j = pre[save_j]; (j) && (flag < k); save_j = j,j = pre[j],flag++)
			{
				prej = pre[j];
				if(prej && comment[prej] > comment[j])
				{
					pre[save_j] = prej;
					pre[j] = pre[prej];
					pre[prej] = j;
					j = prej;
				}
			}
		}
	}
	
}
int query1(int id1, int id2, int x)
{
	int fp = 0;
	int rp = 1;
	int i;
	memset(visit,0,sizeof(visit));
	que[0] = id1;
	que[1] = id2;
	visit[id1] = 1;
	visit[id2] = 2;
	dist[id1] = dist[id2] = 0;
	unsigned int res = 0xffffffff;
	while(fp <= rp)
	{
		int v = que[fp++];													//褰撳墠椤剁偣
		unsigned int cur_dist = dist[v];										//褰撳墠璺濈
		for(i = la[v];i;i = pre[i])											//閬嶅巻褰撳墠椤剁偣杈?
		{
			if(comment[i] <= x)
			{
				break;
			}
			int nextv = ev[i];												//涓嬩竴涓《鐐?	
			if(visit[nextv] == 0)				//涓嬩竴涓《鐐?
			{					
					que[++rp] = nextv;
					dist[nextv] = cur_dist + 1;
					visit[nextv] = visit[v];
			}
			else if(visit[nextv] != visit[v])
			{
				return dist[v] + dist[nextv] + 1;			
			}
		}
	}
	return -1;
}





