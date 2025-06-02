struct Graph{
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
	void init(int n){ s = ecnt = 1; t = 2; ncnt = n; ms(head, 0); }
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
			af = min(af, cap[p]), u = to[p^1];
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
} g;
