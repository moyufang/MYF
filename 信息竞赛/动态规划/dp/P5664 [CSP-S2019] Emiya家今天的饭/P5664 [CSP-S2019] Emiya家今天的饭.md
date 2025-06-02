首先很容易意识到满足 Emiya 和 Rin 的条件比较容易，但 Yazid 的条件有点麻烦，于是考虑在满足前两个条件的限制下求出满足第三个条件的方案数。

然而直接 $\text{dp}$ 根本行不通，状态太多了，于是考虑正难则反，先求出总方案数再减去不满足 Yazid 的条件的方案数。

设 $A_i = \sum_{j=1}^{m}a_{ij}$ ，则在满足 Emiya 和 Rin 的条件下的总方案数为：
$$
tot = \prod_{i=1}^{n}(1+A_i)-1
$$
现在考虑如何求出不满足 Yazid 的条件的方案数。

考虑到不满足 Yazid 的条件的方案中一定仅有一种主食 $j$ 超过一半，而其它的主食均不超过一半 ，所以各个主食超过一半的方案数互斥，于是可以枚举主食 $j$ 并求出其超过一半的方案数，然后再用总方案数减去它们。

现在考虑如何求出主食 $j$​​ 超过一半的方案数。

很容易想到的一个 $\text{dp}$ 是设 $f(i,k,x)$ 表示已经考虑了前 $i$ 中烹饪方式，做了 $k$ 道菜，且用主食 $j$​ ​做了 $x$ 道菜的方案数。

转移方程很显然：
$$
f(i,k,x)=\sum\left\{\begin{aligned}
& f(i-1,k,x) & (k > i)\\
& f(i-1,k-1,x-1)\cdot a_{ij} & (x > 0)\\
& f(i-1,k-1,x)\cdot (A[j]-a_{ij}) &
\end{aligned}\right.
$$
然后再让总方案数减去主食 $j$​ 超过一半的方案数：
$$
tot:=tot-\sum_{k=1}^{n}\sum_{x=\lfloor k/2 \rfloor+1}^{k}f(n,k,x)
$$
这样空间复杂度是 $O(n^3)$ 但时间复杂度是 $O(n^3\cdot m)$ ，肯定过不了。

于是考虑如何优化 $\text{dp}$，看起来好像难以用数据结构去优化，所以只能找规律，想一个新的 $\text{dp}$​ 了。

考虑一个数的出现次数超过总数一半有什么性质，一个结论就是这个数的出现次数减去其它所有数的出现次数后仍大于 $0$，所以依据此我们可以去掉 $\text{dp}$ 中的一维。

设 $f(i,x)$ 表示考虑了前 $i$​ 中烹饪方式，且主食 $j$​ 的出现次数减去其它主食的出现次数为 $x$ 时的方案数。

显然可知 $-n \leq x \leq n$。

转移方程为：
$$
f(i,x)=\sum \left\{\begin{aligned}
& f(i-1,x) &\\
& f(i-1,x-1)\cdot a_{ij} &\\
& f(i-1,x+1)\cdot (A_i - A_{ij}) &
\end{aligned}\right.

$$
总方案数减去主食 $j$​ 超过一半的方案数：
$$
tot:=tot- \sum_{i=1}^{n}f(n,i)
$$
边界条件为 $f(0,0) = 1,f(0,x)=0(x\neq 0)$​。

复杂度为 $O(n^2\cdot m)$ ，可以过。

注意事项：

1. 对于负数索引的处理。
2. 若 $\text{dp}$ 时不使用填表法而使用刷表法则要注意对每个主食 $j$ 进行 $\text{dp}$ 前要记得清空数组。 

```cpp
#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <string.h>
#include <assert.h>
using namespace std;

#define re register
#define sf scanf
#define pf printf
#define ll long long
#define db double
#define nl() putchar('\n')
#define ms(x, vl) memset(x, vl, sizeof(x))
#define _for(i, a, b) for(re int i = (a); i < (b); ++i)
#define _rfor(i,a ,b) for(re int i = (a); i <= (b); ++i)
#define _fev(p, u) for(re int p = head[u]; p; p = nex[p])
#define maxn 120
#define maxm 2005
#define base 110
#define mod 998244353ll
#define inf 0x7fffffff

int rdnt(){
    re int x = 0, sign = 1; re char c = getchar();
    while(c < '0' || c > '9') {if (c == '-') sign = -1; c =getchar(); }
    while(c >= '0' && c <= '9') x = (x<<3) + (x<<1) + (c^48), c =getchar();
    return x * sign;
}

int tot = 1,
    f[maxn][2*maxn],
    A[maxn],
    a[maxn][maxm];

void add(re int &x, re int y){ if ((x+=y)>=mod) x-=mod; }

int main(){
//    freopen("meal.in", "r", stdin);
//    freopen("meal.out", "w", stdout);

    re int n = rdnt(), m = rdnt();
    _rfor(i, 1, n) _rfor(j, 1, m) a[i][j] = rdnt(), add(A[i], a[i][j]);
    _rfor(i, 1, n) tot = (ll)tot * (1+A[i]) % mod;
    --tot;

    _rfor(j, 1, m){
        _rfor(i, 0, n) ms(f[i], 0);
        f[0][base] = 1;
        _rfor(i, 1, n){
            _rfor(x, -n, n){
                re int tmp = (
                    (ll)f[i-1][x+base] +
                    (ll)f[i-1][x-1+base]*a[i][j] % mod +
                    (ll)f[i-1][x+1+base]*(A[i] - a[i][j]) % mod + mod
                ) % mod;
                add(f[i][x+base], tmp);
            }
        }
        _rfor(x, 1, n) add(tot, (mod-f[n][x+base])%mod);
    }

    pf("%d\n", tot);

    return 0;
}

```