[oiclass P3956 随机游走](..\..\..\题面\oiclass P3956 随机游走.png)

首先，naive 的套路 $\text{dp}$ 很容易想，不多赘述，可得 $60pts$。

修改操作中对点权的修改很少，对边权的修改很多，这启发我们将边权作为自变量，去寻找答案与边权的线性关系（系数）。

为了方便，设节点 $u$ 的出度为 $out(u)$。

考虑一条边 $w(u,v)\ \ (u\neq n, v\neq n)$ 在什么时候会被经过以及经过多少次，显然当从 $u$ 出去有 $1/out(u)$ 的概率被经过，当从 $v$ 出去有 $1/out(v)$ 的概率被经过，于是 $w(u,v)$ 被经过的期望次数就与从 $u$ 出去多少次与从 $v$ 出去多少次相关。

于是设 $f(u)$ 表示从 $u$ 出去的期望次数，因为不能从节点 $n$ 出去，所以显然有 $f(n)=0$。

则 $w(u,v)$ 被经过的期望次数 $g(u,v)$ 为：
$$
\frac{f(u)}{out(u)}+\frac{f(v)}{out(v)}
$$
而关于 $f$ 转移方程为：
$$
f(u) = \frac{a(u)}{\sum_{i=1}^{n-1} a(i)} + \sum_{v\in adj(u)} \frac{f(v)}{out(v)}\quad (u\neq n)\\
f(n) = 0
$$
其中 $adj(u)$ 表示与 $u$ 邻接的点的集合。

因为最多有 $n$ 次对 $a$ 的修改，所以最多只用解 $n+1$ 次方程组求得 $n+1$ 组 $f$，而且每次方程组变化的只有常数项，故可以先离线，将所有组 $a$ 保存下来，然后用增广矩阵版的高斯消元一次性把所有组的 $f$ 求出来，时间复杂度为 $O(n^2\cdot s)$，其中 $s$ 为对点权的修改次数。

每一次对边权修改时，因为已经知道了该边被经过的期望次数 $g(u,v$，于是可以以 $O(1)$ 计算出边权修改后对答案的影响。

每一次对点权修改时，找到对应组的 $f$，然后 $O(n^2)$ 更新 $g(u,v)$，并用 $O(n^2)$ 重新计算答案。

算法的总时间复杂度为 $O(n^2\cdot s + n^3 + q)$，其中 $s$ 为对点权的修改次数。

```cpp
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
#define inf 0x7fffffff
#define maxn 505
#define mod 998244353ll
#define maxq 1000005

template <class T>
void print(string name, T arr[], int n, int flag = 1){
	cout<<name<<":";
	_for(i, 0, n)cout<<" "<<arr[i+flag];
	cout<<endl;
}

int rdnt(){
	re int x = 0, sign = 1;
	re char c = getchar();
	while (c < '0' || c > '9') { if (c == '-') sign = -1; c = getchar(); }
	while (c >= '0' && c <= '9') x = (x<<3) + (x<<1) + (c ^ 48), c = getchar();
	return x * sign;
}

int fp(re int x, re int n){
    re int y = 1;
    for(; n; x = (ll)x*x%mod, n>>=1) if (n&1) y = (ll)x*y%mod;
    return y;
}

int get_inv(re int x){
    return fp(x, mod-2);
}

inline void add(re int &x, re int y){ if ((x+=y)>=mod) x-=mod; }

int ans, sum = 0, ivs,
    a[maxn],
    out[maxn],
    ivo[maxn],
    nivo[maxn],
    we[maxn][maxn],
    f[maxn][maxn],
    g[maxn][maxn<<1],

    qry[maxq][4];

inline void agm(re int n, re int tn){
    _rfor(i, 1, n) g[i][tn] = (ll)a[i]*ivs%mod;
}

bool gauss(re int n, re int tn){
    re int tar, d;
    _rfor(i, 1, n){
        tar = 0;
        _rfor(j, i, n) if (g[j][i] != 0){ tar = j; break; }
        if (!tar) return false;
        if (tar != i) _rfor(j, i, tn) swap(g[tar][j], g[i][j]);
        d = get_inv(g[i][i]); g[i][i] = 1;
        if (d != 1) _rfor(j, i+1, tn) g[i][j] = (ll)g[i][j]*d%mod;
        _rfor(j, i+1, n){
            d = g[j][i]; g[j][i] = 0;
            if (d != 0) _rfor(k, i+1, tn) g[j][k] = (mod+g[j][k]-(ll)g[i][k]*d%mod)%mod;
        }
    }
    for(re int i = n; i >= 1; --i) for(re int j = i-1; j >= 1; --j) _rfor(k, n+1, tn)
        g[j][k] = (mod+g[j][k]-(ll)g[j][i]*g[i][k]%mod)%mod;
    return true;
}

void get_f(re int n, re int k){
    _rfor(u, 1, n) _rfor(v, u+1, n)
        if (we[u][v] == -1) f[u][v] = f[v][u] = 0;
        else f[u][v] = f[v][u] = ((ll)g[u][k]*ivo[u] + (ll)g[v][k]*ivo[v])%mod;
}

void get_ans(re int n){
    ans = 0;
    _rfor(u, 1, n) _rfor(v, u+1, n) if (we[u][v] >= 0)
        ans = (ans + (ll)f[u][v]*we[u][v])%mod;
}

int main(){
	//freopen("sample.in", "r", stdin);
	//freopen(".out", "w", stdout);

	ms(we, -1);
    re int n = rdnt(), tn = n, m = rdnt(), q = rdnt(), u, v, w;
    _rfor(i, 1, n-1) a[i] = rdnt(), add(sum, a[i]); ivs = get_inv(sum);
    _rfor(i, 1, m){
        u = rdnt(); v = rdnt();
        we[u][v] = we[v][u] = rdnt();
        ++out[u]; ++out[v];
    }

    _rfor(i, 1, n) ivo[i] = get_inv(out[i]), nivo[i] = (mod-ivo[i])%mod;
    _rfor(i, 1, n-1) _rfor(j, 1, n-1)
        if (i == j) g[i][j] = 1;
        else if (we[i][j] >= 0) g[i][j] = nivo[j];
        else g[i][j] = 0;
    agm(n-1, tn);
    _rfor(i, 1, q){
        _rfor(j, 0, 2) qry[i][j] = rdnt();
        if (qry[i][0] == 1) qry[i][3] = rdnt();
        else{
            assert(qry[i][0] == 2);
            sum = ((ll)sum + mod - a[qry[i][1]] + qry[i][2])%mod;
            ivs = get_inv(sum);
            a[qry[i][1]] = qry[i][2];
            agm(n-1, ++tn);
        }
    }
    bool fg = gauss(n-1, tn);
    assert(fg);
    re int cur = n-1;
    _rfor(i, 0, q){
        if (i && qry[i][0] == 1){
            u = qry[i][1], v = qry[i][2], w = qry[i][3];
            ans = ((ll)ans + (ll)f[u][v]*(mod+w-we[u][v])%mod)%mod;
            we[u][v] = we[v][u] = w;
        }
        else get_f(n, ++cur), get_ans(n);
        pf("%d\n", ans);
    }

	return 0;
}

```

