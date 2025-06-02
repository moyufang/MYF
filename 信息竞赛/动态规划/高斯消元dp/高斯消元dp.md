#### 高斯消元 $\text{dp}$：

有一类概率期望 $\text{dp}$ 的转移明显不是 $\text{DAG}$，所以不能用普通 $\text{dp}$ 解决，只能通过解方程组的方法求解，当转移是线性时就可以用高斯消元，复杂度为 $O(n^3)$ 。

#### 树上消元 $\text{dp}$：

如果该 $\text{dp}$ 是在树上的，那么有一种特殊的消元技巧可以在 $O(n)$ 时间内解决方程组。

若树为无根树则先将树转化为有根树。

设题目中要求解的是转移式即边界条件是：
$$
f(u)=\left\{\begin{aligned}
& \sum_{adj(u,v)}(a(u,v)\cdot f(v) + b(u,v)) & (u\notin S)\\
& c(u) & (u\in S)
\end{aligned}\right.
$$
 其中 $a(u,v),b(u,v),c(u)$ 均已知，$adj(u,v)$ 表示 $u,v$ 之间有连边，$S$ 为边界点集合，$S$ 之外的结点的 $f(u)$ 为未知数。

不妨转换一下转移式：
$$
f(u)=\left\{\begin{aligned}
& a(u,fa_u)\cdot f(fa_u) + b(u,fa_u) + \sum_{v\in son(u)}(a(u,v)\cdot f(v) + b(u,v)) & (u\notin S)\\
& c(u) & (u\in S)
\end{aligned}\right.
$$
其中 $fa_u$ 为结点 $u$ 的父结点，$son(u)$ 为结点 $u$ 的儿子集合。

使用待定系数法，对于每个非边界点 $u$ ，设有系数 $A(u)$ 和 $B(u)$满足：
$$
f(u)=A(u)\cdot f(fa_u) + B(u)
$$
将转移式中的 $f(v)$ 按照待定系数式展开：
$$
\sum_{v\in son(u)}(a(u,v)\cdot f(v) + b(u,v)) = \\
f(u)\cdot \sum_{v\in son(u)}a(u,v)\cdot A(v) + \sum_{v\in son(u)}(a(u,v)\cdot B(v)+b(u,v))\\
$$


在转移式中合并 $f(u)$ 的系数并移项后得到：
$$
f(u)=\frac{a(u,fa_u)\cdot f(fa_u) + b(u,fa_u) + \sum_{v\in son(u)}(a(u,v)\cdot B(v) + b(u,v))}{1-\sum_{v\in son(u)}a(u,v)\cdot A(v)}
$$
于是对比待定式可以看到：
$$
t(u) = 1-\sum_{v\in son(u)}a(u,v)\cdot A(v)\\
A(u) = \frac{a(u,fa_u)}{t(u)}\\
B(u) = \frac{b(u,fa_u)+\sum_{v\in son(u)}(a(u,v)\cdot B(v) + b(u,v))}{t(u)}
$$


注意到 $A(u)$ 和 $B(u)$ 与 $fa_u$ 无关，于是可以树型 $\text{dp}$ 解决。

边界条件是：
$$
A(u) = 0,B(u) = c(u)\quad(u\in S)
$$
由于根 $rt$ 没有父亲于是可以认为 $f(fa_{rt})=b(rt,fa_{rt})=0$，即 $f(rt) = B(rt)$。