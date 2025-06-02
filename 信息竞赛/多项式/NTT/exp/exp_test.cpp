#include<algorithm>
#include<cstring>
#include<cstdio>
#define ll long long
#define ull unsigned long long
#define clr(f,n) memset(f,0,sizeof(int)*(n))
#define cpy(f,g,n) memcpy(f,g,sizeof(int)*(n))
const int _G=3,mod=998244353,Maxn=135000;
int read(){
  int X=0;char ch=0;
  while(ch<48||ch>57)ch=getchar();
  while(ch>=48&&ch<=57)X=X*10+(ch^48),ch=getchar();
  return X;
}
inline void print(int *f,int len){
  for (int i=0;i<len;i++)
    printf("%d ",f[i]);
  puts("");
}
ll powM(ll a,ll t=mod-2){
  ll ans=1;
  while(t){
    if(t&1)ans=ans*a%mod;
    a=a*a%mod;t>>=1;
  }return ans;
}
const int invG=powM(_G);
int tr[Maxn<<1],tf;
void tpre(int n){
  if (tf==n)return ;tf=n;
  for(int i=0;i<n;i++)
    tr[i]=(tr[i>>1]>>1)|((i&1)?n>>1:0);
}
void NTT(int *g,bool op,int n)
{
  static ull f[Maxn<<1],w[Maxn]={1};
  tpre(n);
  for (int i=0;i<n;i++)f[i]=(((ll)mod<<5)+g[tr[i]])%mod;
  for(int l=1;l<n;l<<=1){
    ull tG=powM(op?_G:invG,(mod-1)/(l+l));
    for (int i=1;i<l;i++)w[i]=w[i-1]*tG%mod;
    for(int k=0;k<n;k+=l+l)
      for(int p=0;p<l;p++){
        int tt=w[p]*f[k|l|p]%mod;
        f[k|l|p]=f[k|p]+mod-tt;
        f[k|p]+=tt;
      }      
    if (l==(1<<10))
      for (int i=0;i<n;i++)f[i]%=mod;
  }if (!op){
    ull invn=powM(n);
    for(int i=0;i<n;++i)
      g[i]=f[i]%mod*invn%mod;
  }else for(int i=0;i<n;++i)g[i]=f[i]%mod;
}
void px(int *f,int *g,int n)
{for(int i=0;i<n;++i)f[i]=1ll*f[i]*g[i]%mod;}
int _g1[Maxn<<1];
#define sav _g1
void times(int *f,int *g,int len,int lim)
{
  int n=1;while(n<len+len)n<<=1;
  cpy(sav,g,n);clr(sav+len,n-len);
  NTT(f,1,n);NTT(sav,1,n);
  px(f,sav,n);NTT(f,0,n);
  clr(f+lim,n-lim);clr(sav,n);
}
void invp(int *f,int m)
{
  int n;for (n=1;n<m;n<<=1);
  static int w[Maxn<<1],r[Maxn<<1];
  w[0]=powM(f[0]);
  for (int len=2;len<=n;len<<=1){
  	for (int i=0;i<(len>>1);i++)r[i]=w[i];
    cpy(sav,f,len);NTT(sav,1,len);
    NTT(r,1,len);px(r,sav,len);
    NTT(r,0,len);clr(r,len>>1);
    cpy(sav,w,len);NTT(sav,1,len);
  	NTT(r,1,len);px(r,sav,len);
  	NTT(r,0,len);
  	for (int i=len>>1;i<len;i++)
      w[i]=(w[i]*2ll-r[i]+mod)%mod;
  }cpy(f,w,m);clr(sav,n);clr(w,n);clr(r,n);
}
void dao(int *f,int m){
  for (int i=1;i<m;i++)
    f[i-1]=1ll*f[i]*i%mod;
  f[m-1]=0;
}
int inv[Maxn];
void Init(int lim){
  inv[1]=1;
  for (int i=2;i<=lim;i++)
    inv[i]=1ll*inv[mod%i]*(mod-mod/i)%mod;
}
void jifen(int *f,int m){
  for (int i=m;i;i--)
    f[i]=1ll*f[i-1]*inv[i]%mod;
  f[0]=0;
}
void lnp(int *f,int m)
{
  static int g[Maxn<<1];
  cpy(g,f,m);
  invp(g,m);dao(f,m);
  times(f,g,m,m);
  jifen(f,m-1);
  clr(g,m);
}
void exp(int *f,int m)
{
  static int s[Maxn<<1],s2[Maxn<<1];
  int n=1;for(;n<m;n<<=1);
  s2[0]=1;
  for (int len=2;len<=n;len<<=1){
    cpy(s,s2,len>>1);lnp(s,len);
  	for (int i=0;i<len;i++)
  	  s[i]=(f[i]-s[i]+mod)%mod;
  	s[0]=(s[0]+1)%mod;
  	times(s2,s,len,len);
  }cpy(f,s2,m);clr(s,n);clr(s2,n);
}
int n,m,F[Maxn<<1],G[Maxn<<1];
int main()
{
	freopen("sample.in", "r", stdin);
  Init(n=read());
  for (int i=0;i<n;i++)F[i]=read();
  exp(F,n);//print(F,n);
  	unsigned long long tmp = 0;
	for(int i = 0; i < n; ++i) tmp = tmp*mod+F[i];
	printf("%llu\n", tmp);
  return 0;
}
