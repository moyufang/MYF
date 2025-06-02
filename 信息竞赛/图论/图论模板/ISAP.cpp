struct Graph{
	typedef ll gtype;
	int ecnt, ncnt, s, t,
		head[maxn],
		to[maxm],
		nex[maxm],
		cur[maxn],
		h[maxn],
		gap[maxn],
		que[maxn];
	gtype
		mf,
		cap[maxm];
	void init(){ s = ecnt = 1; t = ncnt = 2; ms(head, 0); }
	void add_edge(int u, int v, gtype cp){
		to[++ecnt] = v; nex[ecnt] = head[u]; cap[ecnt] = cp; head[u] = ecnt;
		to[++ecnt] = u; nex[ecnt] = head[v]; cap[ecnt] = 0; head[v] = ecnt;
	}
	void set_h(){
		re int fro = 0, bac = 0, u, v;
		ms(h, 0); ms(gap, 0);
		gap[h[t] = 1] = 1; que[bac++] = t;
		while(bac > fro){
			u = que[fro++];
			_fev(p, u)
				if (!h[v = to[p]])
					++gap[h[v] = h[u] + 1], que[bac++] = v;
		}
		if (!h[s]) h[s] = ncnt+1;
	}
	gtype dfs(int u, gtype flow){
		if (u == t){ mf += flow; return flow; }
		int v; gtype tmp, used = 0;
		for(int p = cur[u]; p; p = nex[p]){
			cur[u] = p;
			if (cap[p] && h[v = to[p]] == h[u] - 1){
				tmp = dfs(v, mmin(flow-used, cap[p]));
				if (tmp) cap[p] -= tmp, cap[p^1] += tmp, used += tmp;
				if (used == flow) return flow;
			}
		}
		if (--gap[h[u]] == 0) h[s] = ncnt+1;
		++gap[++h[u]];
		return used;
	}
	void isap(){
		mf = 0;
		set_h();
		while(h[s] <= ncnt)
			memcpy(cur, head, sizeof(head)), dfs(s, inf);
	}
} g;
