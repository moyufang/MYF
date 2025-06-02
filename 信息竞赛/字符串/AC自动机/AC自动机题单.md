### AC自动机

[P3121 [USACO15FEB]Censoring G](https://www.luogu.com.cn/problem/P3121) 用栈保存当前节点编号，删除时 $pop$​ 掉对应的那些节点。

[P5231 [JSOI2012]玄武密码](https://www.luogu.com.cn/problem/P5231) 将 $m$ 段文字插入 $Trie$，然后对母串匹配，把经过的每个点标记一下，匹配完后统一把所有已标记的点在失配树上的前缀链上的所有点标记，然后 $\text{dfs}$​ 求出答案。注意在 $\text{dfs}$ 时 ```nxt``` 中非字典树边已被占，无法用是否为 $0$ 区分，需要提前标记哪些是树边。

[P2414 [NOI2011] 阿狸的打字机](https://www.luogu.com.cn/problem/P2414) 树上差分 + $dfn$ 序 + 树桩数组。

[P5840 [COCI2015]Divljak](https://www.luogu.com.cn/problem/P5840) 与阿狸的打字机那题类似。

[P4045 [JSOI2009]密码](https://www.luogu.com.cn/problem/P4045) 状压dp，设 $f(u,i,s)$ 表示匹配到节点编号 $i$、字串长度为 $i$ 且已包含的单词的集合为 $s$ 时方案的总数，然后转移即可。

[P3715 [BJOI2017]魔法咒语](https://www.luogu.com.cn/problem/P3715) 矩阵快速幂。

[P3311 [SDOI2014] 数数](https://www.luogu.com.cn/problem/P3311) 数位 $\text{dp}$ 。

[UVA11019 Matrix Matcher](https://www.luogu.com.cn/problem/UVA11019) 技巧很经典。

[P7582 「RdOI R2」风雨(rain)](https://www.luogu.com.cn/problem/P7582) 分块 + 树状数组。

[P3167 [CQOI2014]通配符匹配](https://www.luogu.com.cn/problem/P3167) 将串按通配符分段，每段是且一定是最前面的一个通配符加上一个字符串段，字符串段可为空段，如果第一个字符不是通配符则用补加占位符 ```#``` 作为通配符，对于每次查询，记录每个前缀的后缀能匹配的段是哪些，然后 $\text{dp}$ 求解答案。 注意到段的个数不超过 $11$，若想知道 $\text{AC}$ 自动机上某个结点可以匹配那些段，可以状态压缩，即对结点 $u$ 设一个二进制数 $pat_u$，满足 $pat_u$ 的第 $j$ 位为 $1$ 当且仅当结点 $u$ 可匹配第 $j$ 段。

 [P7456 [CERC2018] The ABCD Murderer](https://www.luogu.com.cn/problem/P7456) $\text{AC}$ 自动机预处理然后 $\text{st}$ 优化 $\text{dp}$​。

[P3041 [USACO12JAN]Video Game G](https://www.luogu.com.cn/problem/P3041) $\text{AC}$ 自动机预处理后 $\text{dp}$。

[P2336 [SCOI2012]喵星球上的点名](https://www.luogu.com.cn/problem/P2336) 用 $\text{AC}$ 自动机 + $\text{dfn}$ 序排序 + 树上差分完成，经过字符集大，但可以用可持久化数组优化。

[P5599 【XR-4】文本编辑器](https://www.luogu.com.cn/problem/P5599) 

[oiclass P4009 给国的语言](..\..\题面\oiclass P4009 给国的语言.png) 将长度相同的单词插入到同一个 $\text{AC}$ 自动机，因为最多有 $\sqrt{\sum t}=\sqrt{n}$ 种长度不同的字符串，所以 $\text{AC}$ 自动机的个数至多为 $\sqrt{n}$。在每一个 $\text{AC}$ 自动机上以 $O(n)$ 跑一遍原串，这样只有到达接收状态才需要更新答案，每到一个状态不用跳失配指针，总时间复杂度为 $O(n\sqrt{n})$。

[P6125 [JSOI2009]有趣的游戏](https://www.luogu.com.cn/problem/P6125) $\text{AC}$ 自动机 + $\text{Gauss}$ 消元

### 后缀平衡树 

[P6164 【模板】后缀平衡树](https://www.luogu.com.cn/problem/P6164)

[P5353 树上后缀排序](https://www.luogu.com.cn/problem/P5353)

