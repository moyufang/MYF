[P4735 最大异或和](https://www.luogu.com.cn/problem/P4735) 

[P5283 [十二省联考2019]异或粽子](https://www.luogu.com.cn/problem/P5283) 先用前缀和差分问题转化为寻找异或后前 $k$ 大的 $k$ 对数，然后固定其中一个数，用优先队列求出前 $k$ 大，总时间复杂度为 $O((k+n)\cdot w)$，其中 $w = 32$。

[CF241B Friends](https://www.luogu.com.cn/problem/CF241B) 原题目求异或起来前 $k$ 大的 $k$ 对 $(i,j)(i<j)$ ，转化为求异或起来前 $2k$ 大的 $2k$ 对 $(i,j)$。二分数值 $vl$，求大于等于 $vl$ 的对数，确定了对数 $x$ 大于等于 $k$ 的最小的 $vl$ 后，再求出所有异或起来大于等于 $vl$ 的 $(i,j)$ 的异或值之和，然后减去 $(x-k)\cdot vl$，就是答案，时间复杂度 $(nw^2)$。

另一种解法不用二分，考虑从高到低确定 $vl$ 的每一位，对每个 $a_i$ 维护对应节点 $u_i$（伊始时全为根节点），每确定一位时更新所有的 $u_i$（$u_i$ 们会渐渐变得互不相同）。

[P7717 「EZEC-10」序列](https://www.luogu.com.cn/problem/P7717) 在 $\text{01-Trie}$ 上进行数位 $\text{dp}$。

[P4551 最长异或路径](https://www.luogu.com.cn/problem/P4551) $\text{01-Trie}$ + $\text{dsu on tree}$。 

[P6623 [省选联考 2020 A 卷] 树](https://www.luogu.com.cn/problem/P6623) 将数从低位向高位插入，这样就可以 $O(w)$ 做到整体加一，其中 $w$ 是值域的位长。

[[2023ICPCAsiaOnlineContestOne]F.Alice and Bod ](../题面/[2023ICPCAsiaOnlineContestOne]F.Alice and Bod.png) 通过博弈论分析后，关键要求的是有多少对 $(x,y)$ 满足，$|x-y|$ 在质因数分解后 $2$ 的幂次为偶数。不难发现，一个数字包含多少个 $2$ 只与最低位的 $0$ 在哪有关，那么两数之差对应的就是两个数最低在哪一位出现不同。那么依次把数字从低到高位插入 $\text{01-Trie}$ 就可以做统计了。这个方法稍稍拓展可以做到，计算有多少对 $(x,y)$ 满足，$|x-y|$ 是 $r^k$ 的倍数。
