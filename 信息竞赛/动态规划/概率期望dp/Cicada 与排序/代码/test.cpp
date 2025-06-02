#include <bits/stdc++.h>
#define re register
#define int long long
using namespace std;
const int maxn=1010;
const int INF=1e9;
const int inv=499122177;
const int mol=998244353;
char buf[1<<21], *p1=buf, *p2=buf;
#define getchar() (p1==p2&&(p2=(p1=buf)+fread(buf, 1, 1<<21, stdin)), p1==p2?EOF:*p1++)
inline int read() {
    int s=0,w=1; char ch=getchar();
    while(ch<'0'||ch>'9') { if(ch=='-') w=-1;ch=getchar(); }
    while(ch>='0'&&ch<='9') { s=s*10+ch-'0'; ch=getchar(); }
    return s*w;
}
int n,lx[maxn],rx[maxn],a[maxn],rk[maxn][maxn],tmp[maxn],dp[maxn][maxn][2];
inline void merge_sort(int l,int r) {
	if(l==r) { rk[l][1]=1; return; }
	int mid=(l+r)>>1;
	merge_sort(l,mid); merge_sort(mid+1,r);
	vector<int>ql[maxn],qr[maxn];
	for(re int i=l;i<=mid;i++) { lx[a[i]]++; ql[a[i]].push_back(i); }
	for(re int i=mid+1;i<=r;i++) { rx[a[i]]++; qr[a[i]].push_back(i); }
	for(re int i=1;i<=1000;i++) {
		for(re int j=0;j<=lx[i];j++) for(re int k=0;k<=rx[i];k++) dp[j][k][0]=dp[j][k][1]=0;
		dp[0][0][0]=1;
		for(re int j=0;j<=lx[i];j++)
			for(re int k=0;k<=rx[i];k++) {
				if(j!=lx[i]&&k!=rx[i]) (dp[j+1][k][0]+=(dp[j][k][0]+dp[j][k][1])*inv)%=mol,
									   (dp[j][k+1][1]+=(dp[j][k][0]+dp[j][k][1])*inv)%=mol;
				else if(j!=lx[i]) (dp[j+1][k][0]+=(dp[j][k][0]+dp[j][k][1]))%=mol;
				else if(k!=rx[i]) (dp[j][k+1][1]+=(dp[j][k][0]+dp[j][k][1]))%=mol;
			}
		for(re int j=0;j<ql[i].size();j++) {
			for(re int lnum=1;lnum<=lx[i];lnum++)
				for(re int rnum=0;rnum<=rx[i];rnum++) {
					(tmp[lnum+rnum]+=rk[ql[i][j]][lnum]*dp[lnum][rnum][0])%=mol;
					//cout<<tmp[]<<endl;
				}
			for(re int k=1;k<=lx[i]+rx[i];k++) rk[ql[i][j]][k]=tmp[k],tmp[k]=0;
		}
		for(re int j=0;j<qr[i].size();j++) {
			for(re int rnum=1;rnum<=rx[i];rnum++)
				for(re int lnum=0;lnum<=lx[i];lnum++)
					(tmp[lnum+rnum]+=rk[qr[i][j]][rnum]*dp[lnum][rnum][1])%=mol;
			for(re int k=1;k<=lx[i]+rx[i];k++) rk[qr[i][j]][k]=tmp[k],tmp[k]=0;
		}
		ql[i].clear(); qr[i].clear();
		lx[i]=rx[i]=0;
	}
}
int sum[maxn];
signed main(void) {
	freopen("sample.in","r",stdin);
	//freopen("cs.txt","w",stdout);
	n=read();
	for(re int i=1;i<=n;i++) { a[i]=read(); sum[a[i]]++; }
	for(re int i=1;i<=1000;i++) sum[i]+=sum[i-1];
	//for(re int i=1;i<=1000;i++) cout<<sum[i]<<endl;
	merge_sort(1,n);
	for(re int i=1,res;i<=n;i++) {
		res=0;
		for(re int j=1;j<=sum[a[i]]-sum[a[i]-1];j++) {
			(res+=rk[i][j]*(j+sum[a[i]-1])%mol)%=mol;
		}
		printf("%lld ",(long long)res*1%mol);
	}
}
