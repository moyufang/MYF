$\text{Boruvka}$ 算法是生成树算法中最古老的算法，它便于理解、实现简单且能完成某些 $\text{Kruskal}$ 和 $\text{Prim}$ 未所能及的生成树题。

$\text{Boruvka}$ 算法与 $\text{Kruskal}$ 算法同样使用了并查集作为辅助数据结构，其基本思想是维护最小生成树森林，然后分轮次不断加边，每一轮加边结束后连通块至少减半，直到不存在任何边能将某两个连通块连起来，这时或只有一个连通块，生成树建好，或整个图不连通，只能形成生成树森林。

以构建最小生成树为例，$\text{Boruvka}$ 算法中每一轮次的算法流程如下：

1. 枚举每条边，若它连接的两个点在同一个连通块内则删除该边（代码中打上删除标记），否则它连接了两个连通块，用这条边去分别更新这两个连通块所连接的不在连通块内的最小的边。
2. 枚举每一个连通块，然后将与这个连通块相连且不在连通块内的最小的边加入生成树边。
3. 判断是否不存在任何边能将某两个连通块连起来，若是则退出循环，否则继续。

因为轮次最多只有 $\log n$，每轮需要 $O(m)$ 枚举边和 $O(n)$ 枚举点，故总时间复杂度为 $O((m+n)\log n)$，且轮次很难跑满 $\log n$。

```cpp
struct Edge{ int u, v, w; } ed[maxm];
int	ecnt = 1,
	head[maxn],
	to[maxn*2],
	nex[maxn*2],
	we[maxn*2],
	uf[maxn],
	stk[maxn],
	key[maxn];
bool del_tag[maxn];

void add_edge(int u, int v, int w){
    to[++ecnt] = v; nex[ecnt] = head[u]; head[u] = ecnt;
    to[++ecnt] = u; nex[ecnt] = head[v]; head[v] = ecnt;
}

int find(int u){
    int top = 0;
    while(uf[u] > 0) stk[top++] = u, u = uf[u];
    while(top > 0) uf[stk[--top]] = u;
    return u;
}

bool unite(int u, int v, int w){
    int u_ = u, v_ = v;
    u = find(u); v = find(v);
    if (u == v) return false;
    if (uf[u] > uf[v]) u^=v^=u^=v;
    uf[u] += uf[v];
    uf[v] = u;
    add_edge(u_, v_, w);
    return true;
}

void boruvka(int n, int m){
    ms(uf, -1);
    int cnt = 0;
    while(cnt != n-1){
        _rfor(i, 1, n) key[i] = 0;
        bool flag = false;
        _rfor(i, 1, m){
            if (del_tag[i]) continue;
            int u = ed[i].u, v = ed[i].v, w = ed[i].w;
            if ((u = find(u)) == (v = find(v))){ del_tag[i] = true; continue; }
            if (!key[u] || ed[key[u]].w < w) key[u] = i;
            if (!key[v] || ed[key[v]].w < w) key[v] = i;
        	flag = true;
        }
        if (!flag) break; //整个图不连通
        _rfor(i, 1, n) if (uf[i] < 0 && key[i]) cnt += unite(ed[key[i]].u, ed[key[i]].v, ed[key[i]].w);
    }
    if (cnt < n-1){
        int u = 0;
        _rfor(i, 1, n){
            if (uf[i] > 0) continue;
            if (u) cnt += unite(u, i, inf);
            u = i;
        }
        assert(cnt == n-1);
    }
}
    
```