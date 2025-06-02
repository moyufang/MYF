### 单调队列、栈优化

[P1295 [TJOI2011]书架](https://www.luogu.com.cn/problem/P1295) 

[P2254 [NOI2005] 瑰丽华尔兹](https://www.luogu.com.cn/problem/P2254)

[P4697 [CEOI2011]Balloons](https://www.luogu.com.cn/problem/P4697) 仔细分析当前气球不需要跟前面哪些气球判断碰撞，因为当前气球一定不可能与哪些气球碰撞。

[P2569 [SCOI2010]股票交易](https://www.luogu.com.cn/problem/P2569)首先写出一个普通的 $O(TP^2)$ 时间 $O(TP)$ 空间的 $\text{dp}$，然后可以发现 $\text{dp}$ 其中一个维度可以用单调队列优化。 

[P5665 [CSP-S2019] 划分](https://www.luogu.com.cn/problem/P5665)

[oiclass P3154 走夜路](http://oiclass.com/problem.php?id=3154) 如果从充电站 $i$ 能向后到达一个最近的比它单位花费小的充电站 $j$ ，那么肯定只在 $i$ 买能够到 $j$ 的最小电量，然后跳到 $j$ 。如果从充电站 $i$ 向后能到的充电站的单位花费都大于等于它，则应该先在 $i$ 买满电量然后跳到单位花费最小的 $j$ 。用单调队列+二分预处理出每个充电站要跳的下一个充电站 $nxt(i)$​，然后模拟过程求出答案。

[P3089 [USACO13NOV]Pogo-Cow S](https://www.luogu.com.cn/problem/P3089) 分别对两个方向 $\text{dp}$，设 $f(j,i)$ 表示跳到第 $i$ 个目标点且上一个目标点是 $j$ 的最小花费，显然有 $f(j,i) = \min\{f(k,j)|1\leq k \leq j,2s(j)-s(k)\leq s(i)\}$，其中 $s(i)$ 是距离的前缀和，然后先从 $1$ 到 $n$ 枚举 $j$ 再从 $n$ 到 $j+1$ 枚举 $i$（$i > j$），用单调队列 $O(1)$ 求出在 $j$ 固定时的每个 $f(j,i)$ 。

