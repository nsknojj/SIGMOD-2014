#include <iostream>
#include <string>
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <algorithm>
#include <cstring>
#include <set>
#include <vector>
#include <queue>
#include <time.h>

#include <unistd.h>

using namespace std;

const int 
	MAXN=100010,
	MAXLINE=5000,
	MAXTERM=100,
	INF=1000000000;

map<string , vector<int> > place;
int fa_place[MAXN],L_per[MAXN],L_org[MAXN];
vector<int> W_per[MAXN],S_per[MAXN];
vector<int> itag[MAXN];
vector<int> edge[MAXN];
bool exist[MAXN],sorted[MAXN];
int n;

void getplace(char *path) {
	char str[MAXLINE];
	char name[MAXTERM];
	char *s;
	char file[MAXTERM];
	
	sprintf(file, "%s/place.csv", path);
	freopen(file, "r", stdin);
	gets(str);
	while (gets(str)!=NULL) {
		int i=0,id=0;
		s=str;
		while (*s!='|') id=id*10+*(s++)-48;
		s++;
		while (*s!='|') name[i++]=*(s++);
		name[i] = 0;
		place[string(name)].push_back(id);
	}
}

void getplace_isPartOf_place(char *path) {
	char str[MAXLINE];
	char *s;
	char file[MAXTERM];
	
	sprintf(file, "%s/place_isPartOf_place.csv", path);
	freopen(file, "r", stdin);
	gets(str);
	while (gets(str)!=NULL) {
		int id=0,id2=0;
		s=str;
		while (*s!='|') id=id*10+*(s++)-48;
		s++;
		while (*s!=0) id2=id2*10+*(s++)-48;
		fa_place[id]=id2;
	}
}

void getperson_isLocatedIn_place(char *path) {
	char str[MAXLINE];
	char *s;
	char file[MAXTERM];
	
	sprintf(file, "%s/person_isLocatedIn_place.csv", path);
	freopen(file, "r", stdin);
	gets(str);
	while (gets(str)!=NULL) {
		int id=0,id2=0;
		s=str;
		while (*s!='|') id=id*10+*(s++)-48;
		s++;
		while (*s!=0) id2=id2*10+*(s++)-48;
		L_per[id]=id2;
	}
}

void getorganisation_isLocatedIn_place(char *path) {
	char str[MAXLINE];
	char *s;
	char file[MAXTERM];
	
	sprintf(file, "%s/organisation_isLocatedIn_place.csv", path);
	freopen(file, "r", stdin);
	gets(str);
	while (gets(str) != NULL) {
		int id=0,id2=0;
		s=str;
		while (*s!='|') id=id*10+*(s++)-48;
		s++;
		while (*s!=0) id2=id2*10+*(s++)-48;
		L_org[id]=id2;
	}
}

void getperson_workAt_organisation(char *path) {
	char str[MAXLINE];
	char *s;
	char file[MAXTERM];
	
	sprintf(file, "%s/person_workAt_organisation.csv", path);
	freopen(file, "r", stdin);
	gets(str);
	while (gets(str) != NULL) {
		int id=0,id2=0;
		s=str;
		while (*s!='|') id=id*10+*(s++)-48;
		s++;
		while (*s!='|') id2=id2*10+*(s++)-48;
		W_per[id].push_back(id2);
	}
}

void getperson_studyAt_organisation(char *path) {
	char str[MAXLINE];
	char *s;
	char file[MAXTERM];
	
	sprintf(file, "%s/person_studyAt_organisation.csv", path);
	freopen(file, "r", stdin);
	gets(str);
	while (gets(str) != NULL) {
		int id=0,id2=0;
		s=str;
		while (*s!='|') id=id*10+*(s++)-48;
		s++;
		while (*s!='|') id2=id2*10+*(s++)-48;
		S_per[id].push_back(id2);
	}
}

void getperson_hasInterest_tag(char *path) {
	char str[MAXLINE];
	char *s;
	char file[MAXTERM];
	
	sprintf(file, "%s/person_hasInterest_tag.csv", path);
	freopen(file, "r", stdin);
	gets(str);
	while (gets(str) != NULL) {
		int id=0,id2=0;
		s=str;
		while (*s!='|') id=id*10+*(s++)-48;
		s++;
		while (*s!=0) id2=id2*10+*(s++)-48;		
		itag[id].push_back(id2);
	}
}

void getperson_knows_person(char *path) {
	char str[MAXLINE];
	char *s;
	char file[MAXTERM];
	
	sprintf(file, "%s/person_knows_person.csv", path);
	freopen(file, "r", stdin);
	gets(str);
	while (gets(str)!=NULL) {
		int id=0,id2=0;
		s=str;
		while (*s!='|') id=id*10+*(s++)-48;
		s++;
		while (*s!=0) id2=id2*10+*(s++)-48;
		edge[id].push_back(id2);
		edge[id2].push_back(id);
	}
}

void getperson(char *path) {
	char str[MAXLINE];
	char *s;
	char file[MAXTERM];
	
	sprintf(file, "%s/person.csv", path);
	freopen(file, "r", stdin);
	memset(exist, 0, sizeof exist);
	gets(str);
	while (gets(str)!=NULL) {
		int id=0;
		s = str;
		while (*s!='|') id=id*10-48+*(s++);
		exist[id]=1;
		n=max(id+1, n);
	}
}

/* Check p in place */
bool check_place(int p, int place) {
	if (p==place) return 1;
	if (fa_place[p]) return check_place(fa_place[p], place);
	return 0;
}

/* Check if person i live, work or study at place p */
bool check(int i, vector<int> p) {
	
	for (int j=0;j<p.size();j++)
		if (check_place(L_per[i], p[j])) return 1;
		
	for (int k=0;k<p.size();k++)
		for (int j=0;j<S_per[i].size();j++)
			if (check_place(L_org[S_per[i][j]], p[k])) return 1;
	
	for (int k=0;k<p.size();k++)	
		for (int j=0;j<W_per[i].size();j++)		
		if (check_place(L_org[W_per[i][j]], p[k])) return 1;
		
	return 0;
}

typedef pair< int, pair<int,int> > PI;
int q[MAXN],d[MAXN],l,r,sz;
int plist[MAXN][2],tp;
vector<int> ps;
int flag[MAXN];
struct CMP {
	bool operator() (const PI& x, const PI& y) const {
		if (x.first!=y.first) return x.first>y.first;
  		if (x.second.first!=y.second.first) return x.second.first<y.second.first;
		return x.second.second<y.second.second;
	}
};

priority_queue< PI, vector<PI>, CMP > heap;

void bfs(int x, int h, int p) {
	memset(d, 63, n*sizeof(int));
	d[x]=0;
	for (q[l=r=1]=x;l<=r;l++) {
		x=q[l];
		if (d[x]==h) continue;
		int t=edge[x].size();
		for (int i=0;i<t;i++){
			int to=edge[x][i];
			if (d[to]>=INF) {
				d[to]=d[x]+1;
				q[++r]=to;
			}
		}
	}
}

int compare(int x, int y) {
	int ret=0;
	if (!sorted[x]) {
		sort(itag[x].begin(), itag[x].end());
		sorted[x]=1;
	}
	if (!sorted[y]) {
		sort(itag[x].begin(), itag[x].end());	
		sorted[x]=1;
	}
	vector<int>::iterator it1=itag[x].begin(),it2=itag[y].begin();
	while (it1!=itag[x].end()&&it2!=itag[y].end()) {
		if (*it1==*it2) {
			ret++;
			it1++;
			it2++;
		}
		else if (*it1<*it2) it1++;
		else it2++;
	}
	return ret;
}

void query3(int k, int h, vector<int> p) {
	
	ps.clear();
	while (!heap.empty()) heap.pop();
	for (int i=0;i<n;i++)
		if (exist[i]&&check(i,p)) {
			flag[i]=p[0];
			ps.push_back(i);
		}
		
	sz=ps.size();
	CMP cmp;
	for (int i=0;i<sz;i++) {
		if (ps[i]==558)
			int xx=1;
		bfs(ps[i], h, p[0]);
		for (int j=2;j<=r;j++)
			if (q[j]>ps[i]&&flag[q[j]]==p[0]) {
				int sum=compare(ps[i], q[j]);
				PI pr;
				pr=make_pair(sum, make_pair(ps[i],q[j]));

				if (heap.size()<k) heap.push(pr);
				else {
					if (!cmp(heap.top(), pr)) {
						heap.pop();
						heap.push(pr);
					}
				}
			}
	}
	tp=0;
	while (!heap.empty()) {
		plist[++tp][0]=heap.top().second.first;
		plist[tp][1]=heap.top().second.second;
		heap.pop();
	}
	for(int i=tp;i>=1;i--)
		printf("%d|%d ", plist[i][0], plist[i][1]);
	printf("\n");
}

int main(int argc, char** argv) {
	char path[MAXTERM];
	sprintf(path,"data");
	getperson(path);
	getplace(path);
	getplace_isPartOf_place(path);
	getperson_isLocatedIn_place(path);
	getorganisation_isLocatedIn_place(path);
	getperson_workAt_organisation(path);
	getperson_studyAt_organisation(path);
	getperson_hasInterest_tag(path);
	getperson_knows_person(path);	
	freopen(argv[1], "r", stdin);
	freopen("query3.out", "w", stdout);
	char str[MAXLINE],name[MAXLINE];
	int k,h;
	while (gets(str) != NULL) {
		char *s=str;
		if (s[5]=='3') {
			sscanf(s+7, "%d,%d,%s)", &k, &h, name);
			name[strlen(name)-1]=0;
			query3(k, h, place[name]);
		}
	}
	return 0;
}
