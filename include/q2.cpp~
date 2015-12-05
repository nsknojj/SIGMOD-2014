/*
 *author:zz
 *未判断输入数据大小，暂时只支持 1k person
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
#include <tr1/unordered_map>
using namespace std;
using namespace std::tr1;



#include "var.h"


/*extern char FN_Tag;
extern char FN_Person_hasTag;
extern char FN_Person;
extern int path_len;
extern char FN_Person_Knows;
extern char FN_Comment_Creator;
extern char FN_Comment_Reply;
extern char *q_dir, d_dir[];*/


static unordered_map<int,int> tagid2string;
static unordered_map<int,int>tagid2mapid;
static unordered_map<int,char>personid2tag[TAGNUM];
static int mapidlist[TAGNUM];
//static unsigned int dist[N];
static unsigned int birthday[N];
static int mapid_tot;
static char tag_string[TAGNUM*30];
//unordered_map<int,char>::iterator l2;
struct q2_ans
{	
	int strpos;
	int range;
};
struct myless
{
	bool operator ()(const struct q2_ans a, const struct q2_ans b)
	{
		if(a.range > b.range)
		{
			return true;
		}
		else if(a.range < b.range)
		{
			return false;
		}
		else
		{			
			if(strcmp(tag_string+a.strpos,tag_string+b.strpos) < 0)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	}
};
void initialize_2()
{
	char *input;
	int i,id1,id2,tot,ev1,ev2,j,pid,tagid,mapid;
	size_t len;
	//initialize tagid2string
	if(!(input = mymap(FN_Tag,&len)))
	{
		printf("open file err: %s\n",FN_Tag);
		exit(0);
	}
	i = 12;
	int str_cnt = 0;
	while(i < len)
	{
		tagid = 0;		
		while(input[i] != '|')
		{
			tagid = tagid * 10 + input[i++] - '0';
		}
		i++;
		tagid2string.insert(unordered_map<int,int>::value_type(tagid,str_cnt));
		while(input[i] != '|')
		{
			tag_string[str_cnt++] = input[i++];
		}
		tag_string[str_cnt++] = 0;
		while(input[i++] != '\n' && i < len)
		{}
	}
	munmap(input, len);

	//initialize tagid2mapid
	unordered_map<int,int>::iterator l;
	if(!(input = mymap(FN_Person_hasTag,&len)))
	{
		printf("open file err: %s\n",FN_Person_hasTag);
		exit(0);
	}
	i = 17;
	mapid_tot = 0;
	while(i < len)
	{
		pid = 0;
		while(input[i] != '|')
		{
			pid = pid * 10 + input[i++] - '0';
		}
		i++;
		tagid = 0;
		while(input[i] != '\n' && i < len)
		{
			tagid = tagid * 10 + input[i++] - '0';
		}
		i++;
		l = tagid2mapid.find(tagid);
		if(l == tagid2mapid.end())
		{
			tagid2mapid.insert(make_pair<int,int>(tagid,mapid_tot));
			personid2tag[mapid_tot].insert(pair<int,char>(pid,1));
			mapidlist[mapid_tot++] = tagid;
		}
		else
		{		
			mapid = l->second;
			personid2tag[mapid].insert(make_pair<int,char>(pid,1));
		}
	}
	munmap(input, len);


	//initialize birthday
	if(!(input = mymap(FN_Person,&len)))
	{
		printf("open file err: %s\n",FN_Person);
		exit(0);
	}
	i = 74;
	while(i < len)
	{
		pid = 0;
		while(input[i] != '|')
		{
			pid = pid * 10 + input[i++] - '0';
		}
		i++;
		while(input[i++] != '|')
		{}
		while(input[i++] != '|')
		{}
		while(input[i++] != '|')
		{}
		int birth = 0;
		while(input[i] != '-')
		{
			birth = birth * 10 + input[i++] - '0';
		}
		i++;
		while(input[i] != '-')
		{
			birth = birth * 10 + input[i++] - '0';
		}
		i++;
		while(input[i] != '|')
		{
			birth = birth * 10 + input[i++] - '0';
		}
		birthday[pid] = birth;
		while(input[i++] != '|')
		{}
		while(input[i++] != '|')
		{}
		while(input[i++] != '\n' && i < len)
		{}
	}
	munmap(input, len);
}
static int findcon(int mapid, int pid,int d)
{
	int fp = 0;
	int rp = 0;
	int i;
	que[0] = pid;
	visit[pid] = 1;
	unordered_map<int,char>::iterator l;
	while(fp <= rp)
	{
		int v = que[fp++];														//当前顶点
		for(i = la[v];i;i = pre[i])												//遍历当前顶点边
		{
			int nextv = ev[i];													//下一个顶点
			l = personid2tag[mapid].find(nextv);
			if(l != personid2tag[mapid].end() && !visit[nextv] && birthday[nextv] >= d)				//下一个顶点
			{
				que[++rp] = nextv;
				visit[nextv] = 1;
			}
		}
	}
	return rp + 1;
}
static int max_connected(int mapid,int tag,int d)
{
	unordered_map<int,char>::iterator l;
	unordered_map<int,char>::iterator l1 = personid2tag[mapid].begin();
	int tot_exist = 0;
	for(l = l1; l != personid2tag[mapid].end(); ++l)
	{		
		if(birthday[l->first] >= d)
		{
			tot_exist++;
		}
	}
	if(tot_exist == 1)
	{
		return 1;
	}
	int tot_found = 0,cur,res = 0;
	memset(visit,0,sizeof(visit));
	for(l = personid2tag[mapid].begin(); l != personid2tag[mapid].end(); ++l)
	{		
		if(visit[l->first] || birthday[l->first] < d)
		{
			continue;
		}
		cur = findcon(mapid,l->first,d);	
		if(cur > res)
		{
			res = cur;
		}
		tot_found += cur;
		if(tot_exist - tot_found <= res)
		{
			break;
		}
	}
	return res;
}
void query2(int k, int d)
{
	int i,cur;
	set<q2_ans,myless> answer;
	answer.clear();
	set<q2_ans,myless>::iterator l;
	myless cmp;
	for(i = 0; i < mapid_tot; i++)
	{
		if(answer.size() == k && personid2tag[i].size() < (--answer.end())->range)
		{
			continue;
		}		
		cur = max_connected(i,mapidlist[i],d);
		struct q2_ans tmp;
		tmp.range = cur;
		tmp.strpos = tagid2string[mapidlist[i]];
		if(answer.size() < k)
		{			
			answer.insert(tmp);
		}
		else
		{
			l = --answer.end();		
			if(cmp(tmp,*l))
			{				
				answer.erase(l);			
				answer.insert(tmp);
			}
		}
	}
	set<q2_ans,myless>::iterator l1,l2;
	l1 = answer.begin();
	l2 = answer.end();
	for(l = l1; l != l2; l++)
	{
		printf("%s ",tag_string+l->strpos);
	}
	printf("\n");
	
}






