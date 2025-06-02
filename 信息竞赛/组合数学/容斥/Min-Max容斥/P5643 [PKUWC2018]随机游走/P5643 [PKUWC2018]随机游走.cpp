#include <stdio.h>
#include <algorithm>
#include <string.h>
#include <iostream>
#include <assert.h>
using namespace std;

#define re register
#define sf scanf
#define pf printf
#define nl() putchar('\n')
#define ms(x, val) memset(x, val, sizeof(x))
#define ll long long
#define _for(i, a, b) for(re int (i) = (a); (i) < (b); ++(i))
#define _rfor(i, a, b) for(re int (i) = (a); (i) <= (b); ++(i))
#define _fev(p, u) for(re int (p) = head[(u)]; (p); (p) = nex[(p)])
#define maxn 18
#define mod 998244353ll

template <class T>
void print(string name, T arr[], int n, int flag = 1){
	cout<<name<<":";
	if (flag) _rfor(i, 1, n)cout<<" "<<arr[i];
	else _for(i, 0, n)cout<<" "<<arr[i];
	cout<<endl;
}

int rdnt(){
	re int x = 0, sign = 1;
	re char c = getchar();
	while (c < '0' || c > '9') { if (c == '-') sign = -1; c = getchar(); }
	while (c >= '0' && c <= '9') x = (x<<3) + (x<<1) + (c ^ 48), c = getchar();
	return x * sign;
}

int ecnt = 1,
    head[maxn],
    to[maxn*3],
    nex[maxn*3],
    in[maxn],
    dp[maxn],
    A[maxn],
    B[maxn],
    Emi[1<<maxn],
    Ema[1<<maxn],
    sz[1<<maxn],
    inv[maxn+1],
    tsz[maxn];
bool yes[1<<maxn];

void add_edge(re int u, re int v){
    to[++ecnt] = v; nex[ecnt] = head[u]; head[u] = ecnt; in[v]++;
    to[++ecnt] = u; nex[ecnt] = head[v]; head[v] = ecnt; in[u]++;
}

int fp(re int x, re int n){
    re int y = 1;
    for(; n; x = (ll)x*x%mod, n>>=1) if (n&1) y = (ll)x*y%mod;
    return y;
}
int get_inv(re int x){ return fp(x, mod-2); }
void add(re int &x, re int y){ if ((x+=y) >= mod) x -= mod; }

int ts;
void get_AB(int u, int pa){
    if ((1<<u)&ts){
        dp[u] = A[u] = B[u] = 0;
        return;
    }
    int v, sumA = 0, sumB = 0;
    _fev(p, u){
        if (p/2 == pa/2) continue;
        v = to[p];
        get_AB(v, p);
        add(sumA, A[v]);
        add(sumB, B[v]);
    }
    A[u] = get_inv((mod + in[u] - sumA)%mod);
    B[u] = (ll)A[u]*(sumB + in[u])%mod;
}

void dfs(int u, int pa){
    if (pa == 0) dp[u] = B[u];
    else dp[u] = ((ll)A[u]*dp[to[pa^1]] + B[u])%mod;
    _fev(p, u) if (p/2 != pa/2 && !((1<<to[p])&ts)) dfs(to[p], p);
}

int main(){
	//freopen("sample1.in", "r", stdin);
	
    inv[1] = 1;
    _rfor(i, 2, maxn) inv[i] = (mod-mod/i)*inv[mod%i]%mod;

    re int n = rdnt(), q = rdnt(), x = rdnt()-1, lim = 1<<n;
    _for(i, 1, n) add_edge(rdnt()-1, rdnt()-1);
    _for(i, 1, lim) sz[i] = sz[i>>1] + (i&1);
    //pf("F1\n");
    _for(s, 1, lim){
        
        if (s & (1<<x)) Emi[s] = 0;
        else{
            ts = s;
            get_AB(x, 0);
            dfs(x, 0);
            Emi[s] = dp[x];
        }
        //pf("s:%d Emi[s]:%d\n", s, Emi[s]);
    }
    //pf("F2\n");
    re int k, ns, ans;
    while(q--){
        k = rdnt(); ans = 0; ns = 0;
        while(k--) ns |= 1<<(rdnt()-1);
        if (yes[ns]){ pf("%d\n", Ema[ns]); continue; }
        yes[ns] = true;
        
        for(re int s = ns; s; s = (s-1)&ns){
            add(ans, (ll)((sz[s]&1)?1:(mod-1))*Emi[s]%mod);
        }
        Ema[ns] = ans;
        pf("%d\n", ans);
    }
    
	
	return 0;
}
