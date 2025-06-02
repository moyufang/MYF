// luogu-judger-enable-o2
#include<algorithm>
#include<cstdio>
#define mod 998244353
#define G 3
#define Maxn 100500
using namespace std;
inline int read()
{
  register int X=0;
  register char ch=0;
  while(ch<48||ch>57)ch=getchar();
  while(ch>=48&&ch<=57)X=X*10+(ch^48),ch=getchar();
  return X;
}
inline void print(long long *f,int len)
{
  for (int i=0;i<len;i++)
   printf("%lld ",f[i]);
  puts("");
}
int n,m,r[Maxn<<2],invn,invG;
long long powM(long long a,long long t=mod-2)
{
  long long ans=1,buf=a;
  while(t){
  	if(t&1)ans=(ans*buf)%mod;
  	buf=(buf*buf)%mod;
  	t>>=1;
  }return ans;
}
void NTT(long long *f,int n,short op)
{
  for (int i=0;i<n;i++)
   if (r[i]<i)swap(f[r[i]],f[i]);
  for (int p=2;p<=n;p<<=1){
  	int len=p>>1,
  	    w=powM(op==1 ?G:invG,(mod-1)/p);
  	for (int k=0;k<n;k+=p){
  	  long long buf=1;
  	  for (int i=k;i<k+len;i++){
  	  	int sav=f[len+i]*buf%mod;
  	  	f[len+i]=(f[i]-sav+mod)%mod;
  	  	f[i]=(f[i]+sav)%mod;
  	  	buf=buf*w%mod;
  	  }
    }
  }
}
long long _g[Maxn<<2];
void times(long long *f,long long *gg,int len1,int len2,int limit)
{
  int n=1;
  for(;n<len1+len2;n<<=1);
  long long *g=_g;
  for (int i=0;i<len2;i++)g[i]=gg[i];
  for (int i=len2;i<n;i++)g[i]=0;
  invn=powM(n);
  for(int i=0;i<n;i++)
   r[i]=(r[i>>1]>>1)|((i&1)?n>>1:0);
  NTT(f,n,1);NTT(g,n,1);
  for(int i=0;i<n;++i)f[i]=(f[i]*g[i])%mod;
  NTT(f,n,-1);
  for(int i=0;i<limit;++i)f[i]=f[i]*invn%mod;
  for(int i=limit;i<n;++i)f[i]=0;
}
long long _r[Maxn<<2],_rr[Maxn<<2];
void invp(long long *f,int len)
{
  long long *r=_r,*rr=_rr;
  int n=1;for(;n<len;n<<=1);
  for (int i=0;i<n;i++)rr[i]=r[i]=0;
  rr[0]=powM(f[0]);
  for (int len=2;len<=n;len<<=1){
  	for (int i=0;i<len;i++)
  	  r[i]=rr[i]*2%mod;
  	times(rr,rr,len/2,len/2,len);
  	times(rr,f,len,len,len);
  	for (int i=0;i<len;i++)
  	  rr[i]=(r[i]-rr[i]+mod)%mod;
  }for (int i=0;i<len;i++)
    f[i]=rr[i];
}
void dao(long long *f,int m)
{
  for (int i=1;i<m;i++)
   f[i-1]=f[i]*i%mod;
  f[m-1]=0;
}
void jifen(long long *f,int m)
{
  for (int i=m;i;i--)
   f[i]=f[i-1]*powM(i)%mod;
  f[0]=0;
}
long long _lns[Maxn<<2];
void lnp(long long *f,int m)
{
  long long *sav=_lns;
  for (int i=0;i<m;i++)sav[i]=f[i];
  invp(sav,m);dao(f,m);
  times(f,sav,m-1,m,m);
  jifen(f,m-1);
}
long long f[Maxn<<2];
int main()
{
	freopen("sample.in", "r", stdin);
  invG=powM(G);
  scanf("%d",&m);
  for (int i=0;i<m;i++)f[i]=read();
  lnp(f,m);//print(f,m);
  unsigned long long tmp = 0;
  for(int i = 0; i < m; ++i) tmp = tmp*mod+f[i];
  printf("%llu\n", tmp);
  return 0;
}
