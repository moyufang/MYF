利用 $\text{dfn}$ 序可以在一颗树上快速判断某一结点是否是另一节点的子节点。

如题目 ![oiclass P3679 管理](..\..\题面\oiclass P3679 管理.png)

首先，每一份文件一定是由一个节点发出的，如果某时刻某上司接受到这个文件，则在当前时间下的森林中该上司一定是该子节点的祖先。

所以对于操作 $\text{3-}A\text{-}i$ 可以首先找到文件对应的子节点 $B$ 以及对应时间的森林，然后在这个森林上询问 $B$ 是否是 $A$ 的子节点。

于是判断出该题用离线算法解决，先把所有 $\text{1}$ 操作执行完，得到森林，然后对每一颗树求 $dfn$ 序。

```cpp
void dfs(int u){
	fir[u] = ++dc;
	_fev(p, u) dfs(to[p]);
	lst[u] = dc;
}
```

```cpp
_rfor(i, 1, n) if (!fa[i]) dfs(i);
```


对所有 $3$ 操作排序，按时间从后往前回答查询，每次回答都先回退到对应的森林，回退相当于断边。

然后，首先判断 $B$ 与 $A$ 是否在同一颗树上，这一步可以用并查集解决。

1. 若不在同一颗子树上则说明 $\text{NO}$。
2. 否则判断 $B$ 的 $\text{dfn}$ 序是否在 $A$ 的 $\text{dfn}$ 内，若在则 $\text{YES}$，否则 $\text{NO}$。

注意不能只进行判断 $2$，因为可能 $A$ 与 $B$ 在当前的森林中不在同一颗树上，但判断 $2$ 却成立。

```cpp
bool check(re int q){
	re int A = qry[q][0], B = fil[qry[q][1]][0];
	if (A == B) return true;
	if (uf.find(A) != uf.find(B)) return false;
	else return (fir[A] < fir[B] && lst[B] <= lst[A]);
}
```

至于并查集的回退只需要用可撤销并查集。