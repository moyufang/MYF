struct Graph1{
	typedef int gtype;
	int ecnt, ncnt, s, t,
		head[maxn],
		to[maxm],
		nex[maxm],
		pre[maxn],
		que[2*maxn],
		cnt[maxn];
	gtype
		mf, mc,
		dis[maxn],
		cap[maxm],
		cst[maxm];
	bool inque[2*maxn];
	void init(){ s = ecnt = 1; t = ncnt = 2; ms(head, 0); }
	void add_edge(int u, int v, gtype cp, gtype cs){
		to[++ecnt] = v; nex[ecnt] = head[u]; head[u] = ecnt; cap[ecnt] = cp; cst[ecnt] = cs;
		to[++ecnt] = u; nex[ecnt] = head[v]; head[v] = ecnt; cap[ecnt] = 0; cst[ecnt] = -cs;
	}
	bool spfa(){
		re int fro = 0, bac = 0, u, v; ms(pre, 0); ms(cnt, 0); ms(inque, false);
		_rfor(i, 1, ncnt) dis[i] = inf;
		dis[s] = 0; inque[que[bac++] = s] = true;
		while(bac != fro){
			inque[u = que[fro++]] = false; fro %= 2*maxn;
			_fev(p, u){
				if (cap[p] && dis[u] + cst[p] < dis[v = to[p]]){
					dis[v] = dis[u] + cst[p];
					pre[v] = p;
					if (!inque[v])
						inque[que[bac++] = v] = true, bac %= 2*maxn, cnt[v]++;
					if (cnt[v] > ncnt){
						pf("Wrong in Negative Circle!\n");
						return false;
					}
				}
			}
		}
		return dis[t] < inf;
	}
	void augment(){
		gtype af = inf;
		int p, u;
		for(p = pre[u = t]; u != s; p = pre[u])
			af = mmin(af, cap[p]), u = to[p^1];
		for(p = pre[u = t]; u != s; p = pre[u])
			cap[p] -= af, cap[p^1] += af, u = to[p^1];
		mf += af; mc += af*dis[t];
	}
	void min_cost_max_flow(){
		mf = 0; mc = 0;
		while(spfa()) augment();
	}
	void min_cost(){
		mf = 0; mc = 0;
		while(spfa() && dis[t] < 0) augment();
	}
};

struct BoundFlow{
	typedef Graph1::gtype gtype;
	int t_s, s, t, n;
	gtype
		avac, avaf, tot, dlt[maxn];
	Graph1 g;
	void init(){ g.init(); ms(dlt, 0); n = tot = avac = avaf = 0; }
	void add_point(int num){ n += num; g.ncnt += num; }
	void ae(int u, int v, gtype cp, gtype cst){
		if (!cp) return;
		if (cst >= 0) g.add_edge(u+2, v+2, cp, cst);
		else{
			avac += cp*cst;
			dlt[u] -= cp;
			dlt[v] += cp;
			g.add_edge(v+2, u+2, cp, -cst);
		}
	}
	void add_edge(int u, int v, gtype cp, gtype cst, gtype lw){
		if (lw > cp) pf("Wrong in lw > cp!!!\n");
		if (!lw) ae(u, v, cp, cst);
		else{
			avac += lw*cst;
			dlt[u] -= lw;
			dlt[v] += lw;
			ae(u, v, cp-lw, cst);
		}
	}
	void build_graph(int s_, int t_){
		s = s_; t = t_; t_s = g.ecnt+1;
		g.add_edge(t+2, s+2, inf, 0);
		_rfor(i, 1, n){
			if (dlt[i] > 0)
				tot += dlt[i],
				g.add_edge(g.s, i+2, dlt[i], 0);
			else if (dlt[i] < 0)
				g.add_edge(i+2, g.t, -dlt[i], 0);
		}
	}
	bool available_flow(){
		g.min_cost_max_flow();
		if (g.mf != tot) return false;
		avaf += g.cap[t_s^1]; avac += g.mc;
		return true;
	}
	void min_cost_flow(){
		g.s = s+2; g.t = t+2; g.cap[t_s] = g.cap[t_s^1] = 0;
		g.min_cost();
		avaf += g.mf; avac += g.mc;
	}
	void min_cost_max_flow(){
		g.s = s+2; g.t = t+2; g.cap[t_s] = g.cap[t_s^1] = 0;
		g.min_cost_max_flow();
		avaf += g.mf; avac += g.mc;
	}	
} g;
