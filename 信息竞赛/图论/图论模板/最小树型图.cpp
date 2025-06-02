
struct Edge{
	int u, v, b, c;
	Edge(int u = 0, int v = 0, int b = 0, int c = 0)
	:u(u), v(v), b(b), c(c){}
} origin[maxm], e[maxm];

int n, m, cost, mid, ecnt,
	in[maxn],
	pre[maxn],
	id[maxn],
	vis[maxn];

bool zhuliu(int rt = 1){
	re int ans = 0, tn = n, cnt, u, v, b, c, x, y;
	_for(i, 0, ecnt) e[i] = origin[i];
	while(true){
		cnt = 1;
		ms(vis, 0); ms(id, 0); ms(in, -1); ms(pre, 0);
		_for(i, 0, ecnt){
			u = e[i].u; v = e[i].v; b = e[i].b; c = e[i].c;
			if (u == v || b < mid) continue;
			if (in[v] == -1 || c < in[v])
				in[v] = c, pre[v] = u;
		}
		_rfor(i,1, tn){
			if (i == rt) continue;
			if (in[i] == -1) return false;
			ans += in[i];
		}
		_rfor(i, 1, tn){
			if (i == rt) continue;
			y = i;
			while(vis[y] != i && !id[y] && y != rt)
				vis[y] = i, y = pre[y];
			if (!id[y] && y != rt){
				id[y] = ++cnt;
				for(x = pre[y]; x != y; x = pre[x])
					id[x] = cnt;
			}
		}
		if (cnt == 1) return ans <= cost;
		_rfor(i, 1, tn){
			if (i == rt) rt = id[i] = 1;
			else if (!id[i]) id[i] = ++cnt;
		}
		_for(i, 0, ecnt){
			u = e[i].u; v = e[i].v; b = e[i].b; c = e[i].c;
			if (u == v || b < mid) continue;
			if (id[u] != id[v]) e[i].c -= in[v];
			e[i].u = id[u]; e[i].v = id[v];
		}
		tn = cnt;
	}
}
