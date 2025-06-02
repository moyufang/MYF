多项式 $\text{dp}$ 说白了就是 $\text{dp}$ 的对象从数值转变成了多项式罢了。

[传球](传球\传球.md) 断链成环后就是个多项式 $\text{dp}$。

[P6835 [Cnoi2020]线形生物](https://www.luogu.com.cn/problem/P6835) 设当前在第 $i$ 个点时期望还要多少步停止，显然有 $f(n+1)=0$，设 $d(i)$ 表示第 $i$ 个点的出边数量（包括自环和边 $(i,i+1)$），设 $S(i)$ 表示 $i$ 所有出边能到的点的可重集（可重集是因为允许有重边），则转移式为
$$
f(i)=1+\frac{1}{d(i)}\cdot\sum_{j\in S(i)}f(j)\\
f(i)\cdot d(i)=d(i)+f(i+1)+\sum_{j\in S(i)\wedge j\neq i+1}f(j)\\
f(i+1)=f(i)\cdot d(i)-d(i)-\sum_{j\in S(i)\wedge j\neq i+1}f(j)\\
$$
于是 $\text{dp}$ 转移就成了 $\text{DAG}$ 了，设 $f(i)=a_i\cdot f(1)+b_i$，把向量 $(a_i,b_i)$ 看成转移的对象，最终求出 $f(n+1)=a_{n+1}f(1)+b_{n+1}$，又因为 $f(n+1)=0$，则有 $f(1)=-b_{n+1}/a_{n+1}$，是为答案。
