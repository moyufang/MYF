#include <cstdio>
#include <cstring>
#include <algorithm>

#define Min(a,b) ((a)<(b)?(a):(b))

using namespace std;

const int N=10005,M=31,mo=1e9+7,NN=100000;

typedef long long LL;

int n,m,Fac[N],Inv[N],F_Inv[N],g[N],tot,p[N],f[M][NN],cnt[N],st[N],sum,ans;

bool bz[N];

void init()
{
    scanf("%d%d",&n,&m);
    for (int i=2;i<=n;i++)
    {
        if (!bz[i]) p[tot++]=i;
        for (int j=0;j<tot && i*p[j]<=n;j++)
        {
            bz[i*p[j]]=1;
            if (i%p[j]==0) break;
        }
    }
    for (int i=2;i<=n;i++)
        for (int j=0,x=i;j<tot;j++)
            for (;x%p[j]==0;x/=p[j]) cnt[j]++;
    for (int i=1;i<=tot;i++) st[i]=st[i-1]+cnt[i-1]+1;
    Fac[0]=F_Inv[0]=Fac[1]=Inv[1]=F_Inv[1]=1;
    for (int i=2;i<=n;i++)
    {
        Fac[i]=(LL)Fac[i-1]*i%mo;
        Inv[i]=(LL)Inv[mo%i]*(mo-mo/i)%mo;
        F_Inv[i]=(LL)F_Inv[i-1]*Inv[i]%mo;
    }
}

void dfs(int x,int y,int m,int sum,int t)
{
    if (m==0)
    {
        for (int i=0;i<tot;i++) sum=(LL)sum*f[x-1][st[i+1]-1]%mo;
        ans=(ans+sum)%mo;
        return;
    }
    if (y>m) return;
    for (int i=0;i<tot;i++)
    {
        for (int j=st[i];j<st[i]+y;j++) f[x][j]=f[x-1][j];
        for (int j=st[i]+y;j<st[i+1];j++) f[x][j]=(f[x-1][j]+f[x][j-y])%mo;
    }
    dfs(x+1,y,m-y,(LL)sum*F_Inv[y]%mo*Inv[t+1]%mo*g[y]%mo,t+1);
    if (y==m) return;
    for (int k=y+1;k*2<=m;k++)
    {
        for (int i=0;i<tot;i++)
        {
            f[x][st[i]+k-1]=f[x-1][st[i]+k-1];
            for (int j=st[i]+k;j<st[i+1];j++) f[x][j]=(f[x-1][j]+f[x][j-k])%mo;
        }
        dfs(x+1,k,m-k,(LL)sum*F_Inv[k]%mo*g[k]%mo,1);
    }
    for (int i=0;i<tot;i++)
    {
        for (int j=st[i];j<st[i]+m;j++) f[x][j]=f[x-1][j];
        for (int j=st[i]+m;j<st[i+1];j++) f[x][j]=(f[x-1][j]+f[x][j-m])%mo;
    }
    dfs(x+1,m,0,(LL)sum*F_Inv[m]%mo*g[m]%mo,1);
}

void work()
{
    g[1]=1;
    for (int i=2;i<=m;i++) g[i]=(-(LL)(i-1)*g[i-1])%mo;
    ans=0;
    for (int i=0;i<tot;i++) f[0][st[i]]=1;
    dfs(1,1,m,Fac[m],0);
    ans=(LL)(ans+mo)%mo*F_Inv[m]%mo;
    printf("%d\n",ans);
}

int main()
{
    freopen("in.txt","r",stdin);// freopen("jdt.out","w",stdout);
    init();
    work();
    fclose(stdin); fclose(stdout);
    return 0;
}

