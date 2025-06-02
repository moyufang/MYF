本文起笔于```2021.11.25```。



给定一颗 $n$ 个节点的有根树，根节点编号为 $rt$，对于树上任意两个节点 $u$ 和 $v$，它们的 $\text{LCA}$ 指的是它们深度最小的共同祖先，记作 $lca(u,v)$。

一般 $\text{LCA}$ 算法的要求是任意给定 $u$ 和 $v$ 快速求出它们的 $\text{LCA}$，有时还要求在 $u$ 到 $v$ 的链上统计一些信息。



#### 算法一 倍增

预处理 $O(n\log n)$，单次询问 $O(\log n)$，可以统计链上信息。

```cpp
#define maxn 1000005
#define lbmaxn 24
int	ecnt = 1, rt, lbn,
	head[maxn],
	to[maxn*2],
	nex[maxn*2],
	wi[maxn*2],
	dep[maxn],
	ddp[maxn],
    fa[lbmaxn][maxn],
	we[lbmaxn][maxn];

void dfs(int u, int pa){
    _fev(p, u){
        int v = to[p];
        if (v == pa) continue;
        dep[v] = dep[u] + 1;
        ddp[v] = ddp[u] + wi[p];
        fa[0][v] = u;
        we[0][v] = wi[p];
        dfs(v, u);
    }
}

void prepare(int n){
    fa[0][rt] = rt;
    int t = n; lbn = 0; while(t) ++lbn, t>>=1;
    _rfor(i, 1, lbn){
        _rfor(j, 1, n){
            fa[i][j] = fa[i-1][fa[i-1][j]];
            we[i][j] = we[i-1][fa[i-1][j]] + we[i-1][j];
        }
    }
}

int get_lca(int u, int v){
    if (u == v) return u;
    if (dep[u] > dep[v]) u^=v^=u^=v;
    int h = dep[v] - dep[u];
    _rfor(i, 0, lbn) if ((h>>i)&1) v = fa[i][v];
    if (u == v) return v;
	for(int i = lbn; i >= 0; --i) if (fa[i][u] != fa[i][v]) u = fa[i][u], v = fa[i][v];
	return fa[0][u];
}

int get_dis(int u, int v){ return ddp[u] + ddp[v] - 2*ddp[get_lca(u, v)]; }
```



#### 算法二 欧拉序+st表

预处理 $O(n\log n)$，单次询问 $O(1)$，不可以统计链上信息。

```cpp
#define maxn 1000005
#define lbmaxn 24

int rt, ecnt = 1, elr_clk = 0,
	head[maxn], to[maxn*2], nex[maxn*2], dep[maxn], pre[maxn], pst[maxn],
	lb[maxn], st[lbmaxn][maxn];

void dfs(int u, int pa){
    st[0][pre[u] = ++elr_clk] = u;
    _fev(p, u){
        int v = to[p];
        if (v == pa) continue;
        dep[v] = dep[u] + 1;
        dfs(v, u);
        st[0][++elr_clk] = u;
    }
    pst[u] = elr_clk;
}

void prepare(){
    lb[0] = -1;
    _rfor(i, 1, elr_clk) lb[i] = lb[i>>1]+1;
    _rfor(i, 1, lb[elr_clk]){
        _rfor(j, 1, elr_clk-(1<<i)+1){
            int u = st[i-1][j], v = st[i-1][j+(1<<i-1)];
            st[i][j] = (dep[u] <= dep[v]) ? u : v;
        }
    }
}

int qry_st(int l, int r){
    int lblen = lb[r-l+1], u = st[lblen][l], v = st[lblen][r-(1<<lblen)+1];
    return (dep[u] <= dep[v]) ? u : v;
}

int get_lca(int u, int v){
    if (u == v) return u;
    if (pre[u] > pre[v]) u^=v^=u^=v;
    if (pst[u] > pst[v]) return u;
    else return qry_st(pst[u], pre[v]);
}
```

欧拉序求 lca 已经被 dfn 序求 lca 彻底单调队列了。

[冷门科技 —— DFS 序求 LCA](https://www.luogu.com.cn/article/pu52m9ue) 

```cpp
int ecnt = 1, dfn_clk = 0, //ecnt 必须初始值为 1
	head[maxn], to[maxn*2], nex[maxn*2], dfn[maxn],
	lb[maxn], st[lbmaxn][maxn]; 
	
void add_edge(int u, int v){
	to[++ecnt] = v; nex[ecnt] = head[u]; head[u] = ecnt;
	to[++ecnt] = u; nex[ecnt] = head[v]; head[v] = ecnt;
}

void dfs(int u, int pa){
	st[0][dfn[u] = ++dfn_clk] = to[pa^1];
	_fev(p, u){
		if (p/2 == pa/2) continue;
		int v = to[p];
		dfs(v, p);
	}
}

int udst(int u, int v){ return dfn[u] < dfn[v] ? u : v; }

void init(int n){
	lb[0] = -1;
	_rfor(i, 1, n) lb[i] = lb[i>>1]+1;
	_rfor(i, 1, lb[n]) _rfor(j, 1, n-(1<<i)+1) 
		st[i][j] = udst(st[i-1][j], st[i-1][j+(1<<i-1)]);
}

//来自 Alex_Wei https://www.luogu.com.cn/article/pu52m9ue
int get_lca(int u, int v){
	if (u == v) return u;
	if ((u = dfn[u]) > (v = dfn[v])) u^=v^=u^=v;
	int lblen = lb[v-u++];
	return udst(st[lblen][u], st[lblen][v-(1<<lblen)+1]);
}

```



#### 算法三 树链剖分

预处理 $O(n)$，单次询问 $O(\log n)$，可以统计链上信息。

参见[树链剖分](..\..\数据结构\树链剖分\树链剖分详解.md)。



#### 算法四 tarjan

离线算法 $O(n+m)$，其中 $m$ 为查询次数。

实用意义不大，略过。



#### 算法五 Four Russian's Algorithm（四毛子算法）

预处理 $O(n)$，单次询问 $O(1)$，不可以统计链上信息。

实用意义不大，略过。



#### $\text{LCA}$ 的性质

（1）

对于两个节点 $u,v$ 它们的 $\text{dfn}$ 序越近，它们的 $\text{LCA}$ 越浅。

（2）

对于两条链 $u,v$ 和 $x,y$，它们相交当且仅当其中一条链的 $\text{LCA}$ 是另一条链上的节点。

判断两条链是否相交一次要求四次 $\text{LCA}$。

```cpp
bool is_intersect(int u, int v, int x, int y){
    int a = get_lca(u, v), b = get_lca(x, y);
    if (dep[u] < dep[v]) swap(u, x), swap(v, y), swap(a, b);
    return get_lca(a, x) == a || get_lca(a, y) == a;
}
```