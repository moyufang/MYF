struct Graph{
	typedef int gtype;
	int ecnt, ncnt, s, t,
		head[maxn],
		to[maxe],
		nex[maxe],
		cur[maxn],
		h[maxn],
		gap[maxn],
		que[maxn];
	gtype
		mf,
		cap[maxe];
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
				if (cap[p] >= 0 && !h[v = to[p]])
					++gap[h[v] = h[u] + 1], que[bac++] = v;
		}
		_rfor(i, 1, ncnt)
			if (!h[i]) h[i] = ncnt+1;
	}
	gtype dfs(int u, gtype flow){
		if (u == t){ mf += flow; return flow; }
		int v; gtype tmp, used = 0;
		for(int p = cur[u]; p; p = nex[p]){
			cur[u] = p;
			if (cap[p] > 0 && h[v = to[p]] == h[u] - 1){
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
		while(h[s] < ncnt)
			memcpy(cur, head, sizeof(head)), dfs(s, inf);
	}
};

struct BoundFlow{
	typedef Graph::gtype gtype;
	int s, t, t_s, n;
	gtype avaf, tot, dlt[maxn];
	Graph g;
	void init(){ tot = avaf = n = 0; g.init(); ms(dlt, 0); }
	void add_point(int num) { n += num; g.ncnt += num; }
	void add_edge(int u, int v, int cp, int lw){
		if (lw == 0) g.add_edge(u+2, v+2, cp);
		else{
			dlt[u] -= lw; dlt[v] += lw;
			if (cp > lw) g.add_edge(u+2, v+2, cp-lw);
		}
	}
	void build_graph(int s_, int t_){
		s = s_; t = t_; t_s = g.ecnt+1;
		g.add_edge(t+2, s+2, inf);
		_rfor(i, 1, n){
			if (dlt[i] > 0)
				tot += dlt[i],
				g.add_edge(g.s, i+2, dlt[i]);
			else
				g.add_edge(i+2, g.t, -dlt[i]);
		}
	}
	bool available_flow(){
		g.isap();
		if (g.mf != tot) return false;
		avaf += g.cap[t_s^1];
		return true;
	}
	void min_flow(){
		g.cap[t_s] = g.cap[t_s^1] = -1;
		_rfor(p, t_s, g.ecnt) g.cap[p] = -1;
		g.t = s+2; g.s = t+2;
		g.isap();
		avaf -= g.mf;
	}
	void max_flow(){
		g.cap[t_s] = g.cap[t_s^1] = -1;
		_rfor(p, t_s, g.ecnt) g.cap[p] = -1;
		g.s = s+2; g.t = t+2; 
		g.isap();
		avaf += g.mf;
	}
} g;
