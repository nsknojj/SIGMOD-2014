#ifndef VAR
#define VAR

#define C_Path_Len 50		//expected path length
#define M 9000000
#define N 1001000
#define TAGNUM 25000

char FN_Tag[8] = "tag.csv";
char FN_Person_hasTag[27] = "person_hasInterest_tag.csv";
char FN_Person[11] = "person.csv";
int path_len;
char FN_Person_Knows[24] ="person_knows_person.csv";
char FN_Comment_Creator[30] = "comment_hasCreator_person.csv";
char FN_Comment_Reply[28] = "comment_replyOf_comment.csv";
char *q_dir,d_dir[C_Path_Len];



int comment[2*M],pre[2*M],la[N],ev[2*M],tot;
int que[4*N];
int visit[N];
unsigned int dist[N];


char *mymap(const char filename[],size_t *len)	//map the filename to the targets ptr
{
	int fd;
	struct stat info;
	strcpy(d_dir+path_len,filename);
	if((fd = open(d_dir,O_RDONLY,0)) < 0)
	{
		printf("%s\n",d_dir);
		return NULL;
	}
	fstat(fd, &info);
	*len = info.st_size;
	char *ret = (char*)mmap(NULL,info.st_size, PROT_READ, MAP_PRIVATE,fd, 0);
	close(fd);
	return ret;
}



#endif
