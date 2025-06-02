[BZOJ 3526 [POI2014]Card](https://acm.taifua.com/bzoj/p/3526.html) 将每张卡牌的两面都看作点，按条件连边，则题目转化为图是否连通，于是可以用线段树维护是否可以从某一区间的左端点走向右端点。

[P5024 [NOIP2018 提高组] 保卫王国](https://www.luogu.com.cn/problem/P5024) 先考虑链上问题怎么用线段树优化，然后用树链剖分拓展到树上问题。

 [P8505 No voice to cry suffering](https://www.luogu.com.cn/problem/P8505) 首先确定 $f$ 数组的充要条件，定义 $g_x=f_x-f_{x+1}$ ，因而得到 $g$ 数组的充要条件，然后用 $\text{dp}$ 计数即可。$\text{dp}$ 转移可以通过矩阵快速幂优化，因为带修改，所以要用线段树实现动态 $\text{dp}$。

[P9192 [USACO23OPEN] Pareidolia P](https://www.luogu.com.cn/problem/P9192) 只考虑一个字符串的 bessie 数时，贪心扫一遍即可，我们称这个过程叫贪心匹配，设 $g_{i,j}$ 表示字符串 $t[1,i]$ 的后缀们中贪心匹配了前 $j$ 个字符的后缀个数，因为 $t[1,i]$ 的后缀个数为 $i$，也就是说 $\displaystyle \sum_{j=0}^{5} g_{i,j} = i$。

则 $g$ 的转移方程为
$$
\left\{\begin{aligned}
g_{i-1,k}\rightarrow g_{i,k+1}&\quad(t[i]=s[k])\\
g_{i-1,k}\rightarrow g_{i,k}&\quad(t[i]\neq s[k])\\
\end{aligned}\right.
$$
每添加一个字符数时，$t[1,i]$ 的后缀数比 $t[1,i-1]$ 多 $1$，即新增了一个单字符后缀 $t[i,i]$，故还要加上
$$
\left\{\begin{aligned}
1\rightarrow g_{i,1}&\quad(t[i] = s[0])\\
1\rightarrow g_{i,0}&\quad(t[i] \neq s[0])\\
\end{aligned}\right.
$$
设 $f_i$ 表示 $t[1,i]$ 的后缀们的 bessie 数之和，$a_i$ 表示 $t[1,i]$ 的子串们的 bessie 数之和，则
$$
f_i=f_{i-1}+\left\{\begin{aligned}
g_{i-1,5}&\quad(t[i]=s[5])\\
0&\quad(t[i]=s[6])
\end{aligned}\right.\\
a_i=f_i+a_{i-1}
$$
这些转移都是线性转移，$g_{*,0\text{-}5},f_{*},a_{*}$ 共 $8$ 个量，再多一个量转移常数 $1$，用 $9\times 9$ 矩阵辅助转移，然后线段树单点修改即可，复杂度 $O(9^3qn\log n)$。
