设 $AC$ 自动机的字符集大小为 $S$，字典树大小为 $T$，若 $S$  太大则 $AC$ 自动机会炸空间和炸时间。

考虑构建 $AC$ 自动机时关于 $nxt$ 数组和 $fail$ 指针的转移，设 $nxt(u,i)$ 为 $AC$ 自动机上节点 $u$ 接收字符 $i$ 后转移的下一状态，$fail(u)$ 为节点 $u$ 的失配指针，在构建好了字典树后且构建 $AC$ 自动机前，若字典树上 $nxt(u,i)$ 不存在则认为 $nxt(u,i)=0$ 。

则构建 $AC$ 自动机时的转移为：
$$
\begin{aligned}
&v := nxt(u,i)\\
&nxt(u,i):=\left\{
	\begin{aligned}
	&v & (v \neq 0)\\
	&nxt(fail(u), i) & (v = 0)
	\end{aligned}\right.\\
&fail(v) := nxt(fail(u), i)\quad (v \neq 0)
\end{aligned}
$$
 注意到了吗？$nxt(u,*)$ 是从 $nxt(fail(u), *)$ 上经过几次单点修改得来的，$fail(v)$ 是从 $nxt(fail(u),*)$ 里单点查询得到的。

想到了什么？可持久化数组。

也就是说当字符集太大的时候我们可以用可持久化数组优化构建 $AC$ 自动机的这一步，则构建的时间和空间复杂度均为 $O(T\log S)$，

单次状态转移则不再是 $O(1)$ 的了，而是 $O(\log S)$ 了。 

