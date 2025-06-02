#### 练手题：

[P3052 [USACO12MAR]Cows in a Skyscraper G](https://www.luogu.com.cn/problem/P3052)

[p3999 [SHOI2013] 二重镇](https://www.luogu.com.cn/problem/P3999) 预处理好状态之间的转移以及转移的收益，这是处理状压dp的一个很有用的技巧。

[P4363 [九省联考2018]一双木棋chess](https://www.luogu.com.cn/problem/P4363) 预处理好状态之间的转移以及转移的收益，用 $map$ 或 $unordered\_map$ 等 $STL$ 工具建立多进制数 ($long long$) 与有效状态的编号 ($int$) 之间的映射关系。(本题看起来是博弈实际上不是博弈) 

#### 棋盘：

[P2704 [NOI2001] 炮兵阵地](https://www.luogu.com.cn/problem/P2704)

[P1896 [SCOI2005]互不侵犯](https://www.luogu.com.cn/problem/P1896)

[P3888 [GDOI2014]拯救莫莉斯](https://www.luogu.com.cn/problem/P3888)

#### 矩阵快速幂优化：

[P3977 [TJOI2015]棋盘](https://www.luogu.com.cn/problem/P3977)

[P3204 [HNOI2010]公交线路](https://www.luogu.com.cn/problem/P3977)

#### 压缩排列：

[P4460 [CQOI2018]解锁屏幕](https://www.luogu.com.cn/problem/P4460) 用子集和头元素作为状态来将排列压缩。

[P4163 [SCOI2007]排列](https://www.luogu.com.cn/problem/P4163) 用子集和余数作为状态来将排列压缩。

[P2322 [HNOI2006]最短母串问题](https://www.luogu.com.cn/problem/P2322) 先分析出问题与子串排列顺序有关，然后状压dp，一种解决字典序办法是用 $STL$ $string$ (虽然会慢一点，但能过且使用方便)。

[P3349 [ZJOI2016]小星星](https://www.luogu.com.cn/problem/P3349)  先是与排列相关的树上 $\text{dp}$，然后容斥优化 $\text{dp}$​。

[P6622 [省选联考 2020 AB 卷] 信号传递](https://www.luogu.com.cn/problem/P6622) 定义第 $i$ 个信号塔在传递序列中出现的位置的集合为 $T(i)$，本题的决策是用颜色 $1$ ~  $n$ 给这 $n$ 个信号塔的 $T(i)$ 染色，第 $i$ 个信号塔染的 $T(i)$ 上颜色 $j$ 代表第 $i$ 个信号塔排在第 $j$ 位。然后套路性地用状压压缩排列，设 $dp(s)$ 表示已经用 $1$ ~ $|s|$ 颜色给信号塔集 $s$ 里的信号塔染了色时的最小花费时间。再设 $f(s,i)$ 表示 $s$ 里的信号塔已经染了色，$T(i)$ 里的上一位是染了色的位置个数，$g(s,i)$ 是上一位未染色的位置的个数，$h(s,i)$ 是下一位是染了色的位置个数，$l(s,i)$ 是下一位未染色的位置的个数，则有转移
$$
dp(s)+|s|(\ (f(s,i)-l(s,i))+k\cdot(g(s,i)-h(s,i))\ )\to dp(s\cup\{i\})
$$
答案为 $dp(all)$。

然后 $f(s,i),g(s,i),h(s,i),l(s,i)$ 可以预处理，总时间复杂度为 $O(m^m)$，但空间复杂度也为 $O(m2^m)$，会超空间。

只能删掉 $f,g,h,l$ 的第一维，保留第二维，在 $dp$ 的过程中动态更新 $f(i),g(i),h(i),l(i)$，而经过一些均摊分析可得时间复杂度仍为 $O(m2^m)$，但空间复杂度降到了 $O(2^m)$。

[P7519 [省选联考 2021 A/B 卷] 滚榜](https://www.luogu.com.cn/problem/P7519) 

#### 其它：

[P2167 [SDOI2009]Bill的挑战](https://www.luogu.com.cn/problem/P2167) 容斥。

[P6239 [JXOI2012]奇怪的道路](https://www.luogu.com.cn/problem/P6239) 通过增加维度来规范转移，以消除重复记数。

[P3736 [HAOI2016]字符合并](https://www.luogu.com.cn/problem/P3736) 区间dp+状压dp。

[P3959 [NOIP2017 提高组] 宝藏](https://www.luogu.com.cn/problem/P3959) 很经典，作法有很多。

[P5933 [清华集训2012]串珠子](https://www.luogu.com.cn/problem/P5933) 前置知识：$n$ 个节点的无向连通图个数的求解算法

[P5492 [PKUWC2018]随机算法](https://www.luogu.com.cn/problem/P5492) 

[P7961 [NOIP2021] 数列](https://www.luogu.com.cn/problem/P7961) 设计一个按 $2$ 的幂次从小到大考虑的状压 $\text{dp}$，在 $\text{dp}$ 的过程中决策的是每一个幂次所选取的个数。

[P8504 No will to break](https://www.luogu.com.cn/problem/P8504) 

