P1352 没有上司的舞会

P2016 战略游戏

P1131 [ZJOI2007] 时态同步

P4438 [HNOIAHOI2018]道路

P4084 [USACO17DEC]Barn Painting G

P2279 [HNOI2003]消防局的设立

P2458 [SDOI2006]保安站岗

P2585 [ZJOI2006]三色二叉树

P4084 [USACO17DEC]Barn Painting G

P4516 [JSOI2018]潜入行动

[P5007 DDOSvoid 的疑惑](https://www.luogu.com.cn/problem/P5007)

[P8485 「HGOI-1」Water](https://www.luogu.com.cn/problem/P8485)

[hydro.ac_d_nsas_p81](..\..\题面\hydro.ac_d_nsas_p81.png) 以 $1$ 为根。考虑每个点 $x$ 走到 $1$ 是个什么过程：它先走子树内能走的不超过父亲权值的点，然后走到父亲 $fa_x$，再走 $fa_x$ 其他子树中不超过 $fa_{fa_x}$ 权值的点，再走到 $fa_{fa_x}$。进而发现，每个点预处理一个“只走权值不超过它爷爷到根的最大权值，能走多少”，记为 $s_i$，答案就是 $\sum_{j \in i的儿子} s_j + \sum_{j \in i祖先链上其他儿子}s_j + |i的祖先链|$。这个 $s_i$ 的预处理可以简单粗暴数据结构 $O(n\log n)$ 也可以精细地 $O(n)$。

#### 树上背包

P3177 [HAOI2015]树上染色

P2014 [CTSC1997]选课

P1064 [NOIP2006 提高组] 金明的预算方案

P3961 [TJOI2013]黄金矿工

P1270 “访问”美术馆

P1272 重建道路

P1273 有线电视网

P3354 [IOI2005]Riv 河流

P4322 [JSOI2016]最佳团体

#### 换根 dp

P1395 会议

P1364 医院设置

P3478 [POI2008]STA-Station

P2986 [USACO10MAR]Great Cow Gathering G

P2726 [SHOI2005]树的双中心

P3047 [USACO12FEB]Nearby Cows G

P6419 [COCI 2015]Kamp
