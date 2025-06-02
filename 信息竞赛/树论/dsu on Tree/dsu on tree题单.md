[CF600E Lomsat gelral](https://www.luogu.com.cn/problem/CF600E) 模板题，开一个桶记录颜色出现次数即可。

[CF741D Arpa’s letter-marked tree and Mehrdad’s Dokhtar-kosh paths](https://www.luogu.com.cn/problem/CF741D)   用一个二进制数维护每个节点其前缀链上每个字母出现次数的奇偶性，并用全局桶维护全局节点集中每一种奇偶性下长度最长的节点的长度，注意在合并进一颗新的子树时先统计完对答案的贡献再用整颗新树更新全局桶。

[oiclass P3740 祖先](..\..\题面\oiclass P3740 祖先.png) 每一个名字相当于颜色，用 ```unordered_map``` 映射为整数就行。先将询问离线，将对每颗子树的询问挂在对应子树根节点的 ```vector``` 上，每个询问相当于在全局节点集为当前子树时，询问某一深度的所有节点中有多少中颜色，这样可以在全局对每一深度开一个 ```set``` 来维护，这种将对 $k$ 级儿子的询问转化为对某一深度的询问的技巧值得重视。

[CF570D Tree Requests](https://www.luogu.com.cn/problem/CF570D) 技巧同上一题一样。

[CF246E Blood Cousins Return](https://www.luogu.com.cn/problem/CF246E) oiclass P3740祖先 原题。

[CF1009F Dominant Indices](https://www.luogu.com.cn/problem/CF1009F) 开一个桶和一个变量，维护每一深度有多少节点和哪一个深度节点数最多。

[CF375D Tree and Queries](https://www.luogu.com.cn/problem/CF375D) 用树状数组维护出现次数为特定次数的颜色的个数即可。

[P7581 「RdOI R2」路径权值(distance)](https://www.luogu.com.cn/problem/P7581) 长链剖分 + $\text{dsu}$，首先实现 $ O(s) $ 长链合并，$s$ 是子树大小。

[树上统计](..\..\题面\树上统计.png) 直接 $O(n^2)$ 枚举区间过不了，且基本上每个区间的贡献都不同，无法合并同类项。但注意到边数只有 $O(n)$，于是考虑枚举每条边求其对答案的贡献，直接求该边在多少个区间内出现有点难，不妨容斥一下求该边不在多少个连续区间内出现，这样就容易许多了。对于全局节点集，用并查集维护全局节点集内的极长区间连续段（因为要加入元素），用 ```set``` 维护全局节点集之外的极长区间连续段（因为要删除元素，所以无法使用并查集），然后在维护的过程中顺便维护并查集中与 ```set``` 中分别有多少个连续区间，当全局节点集为一颗子树时两者中的连续区间数就是该子树对应边不出现的连续区间数。

[P4551 最长异或路径](https://www.luogu.com.cn/problem/P4551) $\text{01-Trie}$ + $\text{dsu on tree}$。

[P3714 [BJOI2017]树的难题](https://www.luogu.com.cn/problem/P3714) 对于两条路径的合并，若合并处的两条边颜色相同则要减去误差。不要漏掉单独从根向下的路径。

[P4292 [WC2010]重建计划](https://www.luogu.com.cn/problem/P4292) 分数规划后直接长链剖分，然后不断合并，复杂度 $O(n\log n\log w)$。
