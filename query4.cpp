#include<cstdio>
#include<iostream>
#include<cmath>
#include<cstdlib>
#include<algorithm>
#include<cstring>
#include<unistd.h>
#include<fcntl.h>
#include<map>

#define FOR(i,a,b) for(int i=(a);i<=(b);i++)
#define FORD(i,a,b) for(int i=(a);i>=(b);i--)

#define MAXM 10000100


typedef long long LL;
using namespace std;


static char inp_temp[MAXM*20];
static struct know_edge{
    int v1;int v2;
    know_edge(){v1=v2=-1;}

}ke[MAXM],tfe[MAXM],fpe[MAXM];
static bool operator < (const know_edge & a , const know_edge & b){
    if(a.v1==b.v1)return a.v2<b.v2;
    else return a.v1<b.v1;
}
static int km,pn,tfm,fpm,len;
static int k_fi[MAXM],tf_fi[MAXM],fp_fi[MAXM];
static int ki[MAXM],degree[MAXM];
static char q_dir[300];

static int get_num(int &p){

    int re=0;
    while(inp_temp[p]!='|' && inp_temp[p]!='\n'/*&&p<len-1*/)
    {
        re=re*10+inp_temp[p++]-'0';
    }
    p++;
    return re;
}


static map <string,int> tag_map;

static void get_str(int &p,char *s){
    int len=0;
    while(inp_temp[p]!='|'&&inp_temp[p]!='\n'/*&&p<len-1*/)
    {
        s[len++]=inp_temp[p++];
    }
    s[len]=0;
    p++;
}





void input(){
    // deal with person_knows_person

    freopen("data/person_knows_person.csv","r",stdin);
    len=fread(inp_temp,1,MAXM,stdin);

    //printf("%d\n",len);
    int p=0;
    get_num(p); get_num(p);
    while(p!=len)
    {
        ke[ ++km ] .v1=get_num(p);
        ke[ km ].v2=get_num(p);
        ++km;
        ke[km].v1=ke[km-1].v2;
        ke[km].v2=ke[km-1].v1;
    }

    int z=1;
    sort(ke+1,ke+km+1);
    pn=ke[km].v1+1;
    FOR(i,1,km)if(k_fi[ ke[i].v1 ] == 0 ) k_fi[ke[i].v1]=i;
    FORD(i,km,1)if(degree[ke[i].v1]==0  ) degree[ke[i].v1]=i-k_fi[ke[i].v1]+1;

    //deal with tag
    freopen("data/tag.csv","r",stdin);
    len=fread(inp_temp,1,MAXM,stdin);
    p=0;
    get_num(p);get_num(p);get_num(p);

    while(p<len)
    {
        static char ss[50];
        static int id;
        id=get_num(p);  get_str(p,ss);  get_num(p);
        tag_map[ss]=id;

    }


    //deal with forum_hasTag_tag
    freopen("data/forum_hasTag_tag.csv","r",stdin);
    len=fread(inp_temp,1,MAXM,stdin); p=0; get_num(p); get_num(p);

    while(p<len)
    {
        tfe[++tfm].v2=get_num(p);
        tfe[tfm].v1=get_num(p);
        //if(tfe[tfm].v1==0)cout<<tfm<<endl;
    }

    sort(tfe+1,tfe+tfm+1);
    FOR(i,1,tfm)if(tf_fi[tfe[i].v1] == 0 ) tf_fi[tfe[i].v1]=i;

    //deal with forum_hasMember_person
    freopen("data/forum_hasMember_person.csv","r",stdin);
    len=fread(inp_temp,1,MAXM*20,stdin); p=0; get_num(p); get_num(p);get_num(p);

    while(p<len)
    {
        fpe[++fpm].v1=get_num(p);   fpe[fpm].v2=get_num(p); get_num(p);
    }
    --fpm;
    sort(fpe+1,fpe+fpm+1);
    FOR(i,1,fpm)
        if(fp_fi[fpe[i].v1] == 0 ) 
            fp_fi[fpe[i].v1]=i;
}




//----------------------------------------------------------------------


static void analyse(char *s,int &K,char *T){
    K=0;
    int p=7;
    while(s[p]!=',')K=K*10+s[p++]-'0';

    int len=0;
    p++;p++;
    while(s[p]!=')')T[len++]=s[p++];
    T[len]=0;
}

static int in_p_q[MAXM],p_q[MAXM],f_q[MAXM],in_f_q[MAXM];
static struct ans_struct{
    int id; double cv;
}ans[1000];
bool operator  < (const ans_struct & a, const ans_struct & b){
    if(fabs(a.cv-b.cv)<1e-10)return a.id>b.id;
    return a.cv<b.cv;
}

//------------------get reach----------------------V
static int fa[MAXM],reach[MAXM];

static int getfa(int v){
    if(fa[v]==-1)return v;
    return fa[v]=getfa(fa[v]);
}

void get_reach(){
    FOR(i,1,p_q[0])fa[p_q[i]]=-1,reach[p_q[i]]=1;
    FOR(i,1,p_q[0])FOR(j,k_fi[p_q[i]],km){
        if(p_q[i]!=ke[j].v1)break;
        if(in_p_q[ke[j].v2]==0)continue;
        int fx=getfa(ke[j].v1),fy=getfa(ke[j].v2);
        if(fx!=fy)
        {
            reach[fx]+=reach[fy];
            fa[fy]=fx;
        }
    }
    FOR(i,1,p_q[0])reach[p_q[i]]=reach[getfa(p_q[i])];
}


LL get_sumdist(int v,double li){
    LL sumdist;
    static int ki[MAXM/10],q[MAXM/10],di[MAXM/10];
    static int he,ta;
    sumdist=0;
    ki[q[he=ta=1]=v]=1;
    di[v]=0;
    //reach[v]=1;
    while(he<=ta)
    {
        if(sumdist>li)break;
        int x=q[he];
        FOR(i,k_fi[x],km){
            if(ke[i].v1!=x)break;
            int y=ke[i].v2;
            if(ki[y]||!in_p_q[y])continue;

            ki[y]=1;
            di[y]=di[x]+1;
            q[++ta]=y;
            sumdist+=di[y];
            //reach[v]++;

            if(sumdist+ LL(di[y])*(reach[v]-ta) > li )
            {
                sumdist= LL(1000000000)*1000000000;
                break;
            }

        }
        he++;
    }
    FOR(i,1,ta)ki[q[i]]=0;
    return sumdist;
}


static inline bool cmp2(int a,int b){return degree[a]>degree[b];}


void do_query4(char *s){
    static int K,t_id;
    static char T[500];
    analyse(s,K,T);
    int z=1;
    //------------algorithm-------------------V
    t_id=tag_map[T];
    f_q[0]=0;
    FOR(i,tf_fi[t_id],tfm){
        if(tfe[i].v1!=t_id)break;
        if(in_f_q[tfe[i].v2] == 0 )
        {
            in_f_q[tfe[i].v2]=1;
            f_q[++f_q[0]] = tfe[i].v2;
        }
    }
    p_q[0]=0;
    FOR(i,1,f_q[0]){
        in_f_q[ f_q[i] ] = 0;
        FOR(j,fp_fi[ f_q[i] ], fpm){
            if(fpe[j].v1!=f_q[i])break;
            if(in_p_q[fpe[j].v2]==0)
            {
                in_p_q[fpe[j].v2]=1;
                p_q[ ++ p_q[0] ]=fpe[j].v2;
            }
        }
    }

    //              get reach                   V
    get_reach();
    FOR(i,1,K)ans[i].cv=0;


    //
    //p_q[1]=385;
    sort(p_q+1,p_q+p_q[0]+1,cmp2);
    FOR(i,1,p_q[0]){
        int x=p_q[i];

        double li=ans[K].cv/(reach[x]-1)*(p_q[0]-1)/(reach[x]-1);
        if(li<1e-7)li=1e50;
        else li=1/li;

        LL sumdist=get_sumdist(x,li);

        double cv=double(reach[x]-1)*(reach[x]-1)/ (p_q[0]-1) / sumdist;
        ans[K+1].cv=cv;ans[K+1].id=x;
        FORD(j,K+1,2)if(ans[j-1] < ans[j])swap(ans[j],ans[j-1]);
    }

    FOR(i,1,p_q[0])in_p_q[p_q[i]]=0;

    FOR(i,1,K)printf("%d ",ans[i].id);printf("%% centrality values");FOR(i,1,K)printf(" %.16f",ans[i].cv);printf("\n");

}

void work(){
    freopen(q_dir,"r",stdin);
    freopen("query4.out","w",stdout);
    int len=fread(inp_temp,1,MAXM,stdin); int p=0;
    while(p<len)
    {
        static char ss[500];
        get_str(p,ss);
        if(ss[5]=='4')do_query4(ss);
    }
}

int main(int argc, char **argv){
    strcpy(q_dir,argv[1]);	
    input();
    get_reach();
    work();
    return 0;
}

