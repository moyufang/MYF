#### 题单

[2022HongKongICPC-Dice Game](..\..\..\题面\2022HongKongICPC-Dice Game.png) 
$$
\begin{aligned}
ans&={1\over n^2}\sum_{x=0}^{n-1}\max\{x,\sum_{y=0}^{n-1}x\oplus y\}\\
&={n-1\over2}+{1\over n^2}\sum_{x=0}^{n-1}\max\{\sum_{y=0}^{n-1}(x\oplus y-x),0\}\\
\end{aligned}
$$
记 $\displaystyle f_x=\sum_{y=0}^{n-1}(x\oplus y-x)$，$w=MSB(n-1)$，$w$ 是 $n-1$ 的最高二进制位

$h_i$ 表示 $[0,n-1]$ 里第 $i$ 位为 $1$ 的数的个数， $h_i$ 可 $O(1)$ 求出。
$$
h_i=2^i\cdot\lfloor {n\over 2^{i+1}}\rfloor+\left\{\begin{aligned}
&0&(n\%2^{i+1}\le2^i)\\
&n\%2^{i+1}-2^i&(n\%2^{i+1}>2^i)
\end{aligned}\right.
$$
则
$$
f_x=\sum_{i=0}^w2^i\cdot h_i\cdot d_i(x)
$$
其中 $d_i(x)$ 是符号函数， 当 $x$ 的第 $i$ 为 $0$ 时，$d_i(x)=1$，否则 $d_i(x)=-1$。

一个 Easy 的想法是根据上式逐一求出 $f_x$，然后根据其符号判断是否加入贡献，喜提 $O(nw)$。

对 $f_x$ 打表可以发现，若将 $[0,n-1]$ 分成极长的几段使得端内的 $f_x$ 符号相同、相邻两端的 $f_x$ 的符号不同，则这样段数非常少，解释如下。

记 $w_i=2^i\cdot h_i$，打表可以发现，当 $0<i<w$ 时，有 $w_i\ge2\cdot w_{i-1}$。

于是可以合理推测，$f_x$ 的符号由其高几位的 01 状况所决定。

若某个区间 $[l,r)$ 的 $f_x$ 的符号为正，则可以优化计算。

设 $g_i(x)$ 表示 $[0,x-1]$ 里的数第 $i$ 位为 $0$ 的数的个数减去第 $i$ 位为 $1$  的数的个数，则
$$
g_i(x)=\left\{\begin{aligned}
&x\%2^{i+1}&(x\%2^{i+1}\le2^i)\\
&2^{i+1}-x\%2^{i+1}&(x\%2^{i+1}>2^i)
\end{aligned}\right.\\
$$

$$
s(i) = \sum_{x=0}^{i-1}f_x=\sum_{i=0}^{w}w_i\cdot g_i(x)
$$

则 $s(r)-s(l)$ 就是 $[l,r]$ 上的 $f_x$ 之和。

剩下唯一的问题就是如何求尽可能少地将 $[0,n-1]$ 划分成不同区间，使得每个区间内的 $f_x$ 符号相同。

记 $\displaystyle sw_i=\sum_{j=0}^iw_j$，采取的方法是在 $\text{01-Trie}$ 上进行 dfs，代码如下

```cpp
#include <stdio.h>
#include <algorithm>
#include <string.h>
#include <iostream>
#include <assert.h>
using namespace std;

#define pf printf
#define ll long long
#define _for(i, a, b) for(re int i = (a); i < (b); ++i)
#define _rfor(i, a, b) for(re int i = (a); i <= (b); ++i)
#define inf 0x7fffffff
#define lbmaxn 30
#define mod 998244353ll

int rdnt(){
	re int x = 0, sign = 1;
	re char c = getchar();
	while (c < '0' || c > '9') { if (c == '-') sign = -1; c = getchar(); }
	while (c >= '0' && c <= '9') x = (x<<3) + (x<<1) + (c ^ 48), c = getchar();
	return x * sign;
}

int lb(int x){ return x <= 0 ? -1 : 31-__builtin_clz(x); }
int fp(re int x, re int n){re int y=1;for(;n;x=(ll)x*x%mod,n>>=1)if(n&1)y=(ll)x*y%mod;return y;}
int get_inv(re int a){ return fp(a, mod-2); }

int n, lbn;
ll	w[lbmaxn], sw[lbmaxn];

int g(int n, int i){
	int x = n&(1<<i+1)-1;
	return (x>>i) ? (1<<i+1)-x : x;
}

int h(int n, int i){
	return (1<<i)*(n>>i+1)+max(0, (n&(1<<i+1)-1)-(1<<i));
}

int sum(int n){
	int ans = 0;
	_rfor(i, 0, lbn){
		ans = (ans+w[i]%mod*g(n, i))%mod;
	}
	return ans;
}

int sum(int l, int r){
	//pf("(%d, %d)\n", l, r, (sum(r)-sum(l)+mod)%mod);
	return (sum(r)-sum(l)+mod)%mod;
}

int dfs(int x, ll v, int h){
	if (x >= n) return 0;
	if (h == -1 && v > 0) return v%mod;
	if (v - sw[h] >= 0)	return sum(x, min(n, x+(1<<h+1)));
	if (v + sw[h] <= 0) return 0;
	return (dfs(x, v+w[h], h-1) + dfs(x|(1<<h), v-w[h], h-1))%mod;
}

int main(){
	int T = rdnt();
	while(T--){
		n = rdnt(); lbn = lb(n-1);
		_rfor(i, 0, lbn){
			w[i] = (1ll<<i)*h(n, i);
			if (i) sw[i] = w[i] + sw[i-1];
			else sw[i] = w[i];
		}
		int ans = dfs(0, 0, lbn);
		pf("%lld\n", (1ll*ans*get_inv(n)%mod*get_inv(n)%mod+1ll*(n-1)*get_inv(2))%mod);
		
	}
	return 0;
}
```















