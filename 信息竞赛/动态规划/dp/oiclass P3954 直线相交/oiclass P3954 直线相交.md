[oiclass P3954 直线相交](..\..\题面\oiclass P3954 直线相交.png) 

对于一个询问 $n,m$，我们需要判断是否能找到一个分组方案，这个方案将给 $n$ 个木棒分为若干组 $a_i$（组数不做要求），同一组木棒相互平行，且满足
$$
\sum_{i}a_i=n,\quad m=\sum_{i<j}a_i\cdot a_j\tag{*1}
$$
考虑到
$$
\sum_{i<j}a_i\cdot a_j={1\over 2}\left(n^2-\sum_{i}a_i^2\right)\tag{*2}
$$
于是 $\text{(*1)}$ 等价于
$$
\sum_{i}a_i^2=n^2-m
$$

#### 第一个 dp

所以容易想到一个 $\text{dp}$，即设 $f(i,j)$ 表示是否存在一个分组方案，使 $i$ 个木棒分组后得到的 $\displaystyle \sum_{i}a_i^2$ 等于 $j$。

转移方程
$$
f(i,j)=\mathrm{OR}_{k=1\wedge k^2<j}^{i} f(i-k,j-k^2)
$$
注意到 $j$ 的范围是 $[0,n^2]$，则单次转移的复杂度为 $O(n)$

边界条件
$$
f(0,0)=\text{true},\quad f(0,j)=\text{false}(j>0)
$$
求 $f$ 的空间复杂度 $O(N^3)$ ，时间复杂度 $O(N^4)$，回答询问的复杂度 $O(Q)$，过不了。

#### 第二个 dp

然而这个 $\text{dp}$ 难以优化，接下来是神之一笔，注意到 $\text{(*2)}$ 还等价于
$$
\sum_{i<j}a_i\cdot a_j={n(n-1)\over 2}-\sum_{i}{a_i(a_i-1)\over2}\tag{*3}
$$
仿照上之前的 $\text{dp}$ 可以设计一个新的 $\text{dp}$，设 $f(i,j)$ 表示是否存在一个分组方案，使 $i$ 个木棒分组后得到的 $\displaystyle \sum_{i}{a_i(a_i-1)\over 2}$ 等于 $j$。

转移方程为
$$
f(i,j)=\mathrm{OR}_{k=1\wedge k^2<j}^{i} f(i-k,j-k(k-1)/2)
$$
根据我们的转移方程可以发现若 $f(i,j)$ 为 true，则 $f(i+1,j)$ 一定为 true，也就是说只要固定了 $j$，则 $f(*,j)$ 一定只有两段，前面的一段全 false，后面的一段全 true，这启发我们设计如下方程。

设 $g(j)$ 表示使得 $f(i,j)$ 为 true 的最小的 $i$ 是多少，相当于使得少相交 $j$ 个点时至少用的棍子数为多少。

转移是
$$
g(i)=\min_{j=1\wedge {j(j-1)\over2}\le i}^{N}\{j+g(i-j(j-1)/2)\}
$$
于是 $n$ 根棍子能产生 $m$ 个交点的充要条件就是 $g(n(n-1)/2-m)\le n$。

空间复杂度为 $O(m)$ 时间复杂度为 $O(nm)$，可以通过。

#### 总结

两个 $\text{dp}$ 差异仅仅只因为 $\displaystyle \sum_{i<j}a_i\cdot a_j\tag{*1}$ 等价恒等变换的方式不同，而前者难以优化，后者可以优化，实在令人唏嘘















