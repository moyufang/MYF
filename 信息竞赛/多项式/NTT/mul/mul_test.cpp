#include<algorithm>
#include<cstdio>
#define ll long long
#define ull unsigned long long
using namespace std;
const int _G=3,mod=998244353,Maxn=1050000;
inline int read(){
  int X=0;char ch=0;
  while(ch<48||ch>57)ch=getchar();
  while(ch>=48&&ch<=57)X=X*10+(ch^48),ch=getchar();
  return X;
}
ll powM(ll a,int t=mod-2){
  ll ans=1;
  while(t){
    if(t&1)ans=ans*a%mod;
    a=a*a%mod;t>>=1;
  }return ans;
}
const int invG=powM(_G);
int tr[Maxn<<1];
void NTT(int *g,bool op,int n)
{
  static ull f[Maxn<<1],w[Maxn<<1]={1};
  for (int i=0;i<n;i++)f[i]=g[tr[i]];
  for(int l=1;l<n;l<<=1){
    ull tG=powM(op?_G:invG,(mod-1)/(l+l));
    for (int i=1;i<l;i++)w[i]=w[i-1]*tG%mod;
    for(int k=0;k<n;k+=l+l)
      for(int p=0;p<l;p++){
        int tt=w[p]*f[k|l|p]%mod;
        f[k|l|p]=f[k|p]+mod-tt;
        f[k|p]+=tt;
      }      
    if (l==(1<<17))
      for (int i=0;i<n;i++)f[i]%=mod;
  }if (!op){
    ull invn=powM(n);
    for(int i=0;i<n;++i)
      g[i]=f[i]%mod*invn%mod;
  }else for(int i=0;i<n;++i)g[i]=f[i]%mod;
}
int n,m,F[Maxn<<1],G[Maxn<<1];
int main()
{
	freopen("sample.in", "r", stdin);
  n=read();
  for (int i=0;i<n;i++)F[i]=read();
  m=read();
  for (int i=0;i<m;i++)G[i]=read();
  int len=1;for (;len<n+m;len<<=1);
  for(int i=0;i<len;i++)
    tr[i]=(tr[i>>1]>>1)|((i&1)?len>>1:0);
  NTT(F,1,len);NTT(G,1,len);
  for(int i=0;i<len;++i)F[i]=1ll*F[i]*G[i]%mod;
  NTT(F,0,len);
  int tn = n+m-1;
  while(tn && F[tn-1] == 0) --tn;
//  for (int i=0;i<tn;i++)
//    printf("%d ",(int)F[i]);
  unsigned long long tmp = 0;
  for(int i = 0; i < tn; ++i)tmp = tmp*mod+F[i];
	printf("%llu\n", tmp);
    
  return 0;
}
