本文起笔于```2021.10.11```。

虽然 $\text{st}$​ 表作为静态数据结构是难以修改的，但它可以以 $O(\log n)$​ 从末尾添加新元素。

具体操作看代码。

```cpp
#define maxn (1<<22)
#define lbmaxn(23)
int n,
	lb[maxn],
    st[lbmaxn][maxn];

void insert(int vl){
    int lbn = lb[++n];
    st[0][n] = vl;
    _rfor(i, 1, lbn){
        re int j = n-(1<<i)+1;
        st[i][j] = min(st[i-1][j], st[i-1][j+(1<<i-1)]);
    }
}

```

 相关例题：

[P7707 「Wdsr-2.7」百花齐放的太阳花田](https://www.luogu.com.cn/problem/P7707) 虽然主要解法不在与 $\text{st}$ 表，但其中涉及到从 $\text{st}$ 表的末尾添加新元素。

[P7456 [CERC2018] The ABCD Murderer](https://www.luogu.com.cn/problem/P7456) $f(i)=\min\{f(j)|i-mx\leq j < i\} + 1$​，其中 $mx$​ 为前缀 $i$​ 的后缀所能匹配到的最长的单词的长度，显然可以通过从 $\text{st}$ 表末尾加入新元素优化 $\text{dp}$。

[oiclass P4017 淘淘蓝蓝之幻影树](..\..\题面\oiclass P4017 淘淘蓝蓝之幻影树.png) 离线，将询问挂在节点的 ```vector``` 上，然后 $\text{dfs}$ 回答询问，每到一个节点就新加入一个普通神奇宝贝，每退出就去掉一个普通神奇宝贝。考虑一次询问如何回答，直接模拟不行，但可以二分求出每个幻影神奇宝贝最多能打败到哪个普通神奇宝贝，然后不断地跳，这涉及到某个幻影神奇宝贝是否能打败一区间的普通神奇宝贝，考虑到 $m$ 较小，状态压缩一下然后通过 $\text{st}$ 表可以以 $O(1)$ 判断，而 $\text{dfs}$ 时对 $\text{st}$ 表的维护就涉及到 $\text{st}$ 表末尾的增删。总时间复杂度为 $O(q\log n)$。
