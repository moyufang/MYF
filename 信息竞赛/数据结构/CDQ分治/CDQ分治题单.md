[P3810 【模板】三维偏序（陌上花开）](https://www.luogu.com.cn/problem/P3810) 首先分别以 $a,b,c$ 为第一、第二、第三关键字对元素进行排序，然后 $\text{CDQ}$ 分治，在分治的过程中用树状数组辅助统计。然而这样会在有元素完全相同时算漏一些贡献，把这些算漏的贡献加回去就行。总时间复杂度为 $O(n\log^2 n)$。

[P7312 [COCI2018-2019#2] Sunčanje](https://www.luogu.com.cn/problem/P7312) 因为是后面上屋的矩阵会影响前面上屋的矩阵，不妨反转矩阵序列，这样 $\text{CDQ}$ 分治的时候就是左子区间影响右子区间了。考虑左子区间的矩阵如何影响右子区间的矩阵，当且仅当 $bx(a) \le ex(b)$ 且 $ex(a)\ge bx(b)$ 时矩阵 $a$ 才有可能覆盖 $b$。

[P8253 [NOI Online 2022 提高组] 如何正确地排序](https://www.luogu.com.cn/problem/P8253)

[P7982 [JRKSJ R3] 琴琴的树](https://www.luogu.com.cn/problem/P7982) 