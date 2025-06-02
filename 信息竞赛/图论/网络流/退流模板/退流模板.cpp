#include <stdio.h>
#include <algorithm>
#include <string.h>
#include <iostream>
using namespace std;

#define re register
#define sf scanf
#define pf printf
#define nl() putchar('\n')
#define ms(x, val) memset(x, val, sizeof(x))
#define _for(i, a, b) for(re int (i) = (a); (i) < (b); ++(i))
#define _rfor(i, a, b) for(re int (i) = (a); (i) <= (b); ++(i))
#define _fev(p, u) for(re int (p) = head[(u)]; (p); (p) = nex[(p)])
#define mmin(a, b) (((b) < (a)) ? (b) : (a))
#define mmax(a, b) (((b) > (a)) ? (b) : (a))
#define maxn 2005
#define maxm 300005
#define maxk 30000
#define inf 0x7fffffff

//debug
//template <class T>
//void print(string name, T arr[], int n, int flag = 1){
//	cout<<name<<":";
//	if (flag) _rfor(i, 1, n)cout<<" "<<arr[i];
//	else _for(i, 0, n)cout<<" "<<arr[i];
//	cout<<endl;
//}

int rdnt(){
	register int x = 0, sign = 1;
	register char c = getchar();
	while (c < '0' || c > '9') { if (c == '-') sign = -1; c = getchar(); }
	while (c >= '0' && c <= '9') x = (x<<3) + (x<<1) + (c ^ 48), c = getchar();
	return x * sign;
}

struct Graph{
	typedef int gtype;
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
	int add_edge(int u, int v, gtype cp){
		to[++ecnt] = v; nex[ecnt] = head[u]; cap[ecnt] = cp; head[u] = ecnt;
		to[++ecnt] = u; nex[ecnt] = head[v]; cap[ecnt] = 0; head[v] = ecnt;
		return ecnt-1;
	}
	void set_h(){
		re int fro = 0, bac = 0, u, v;
		ms(h, 0); ms(gap, 0);
		gap[h[t] = 1] = 1; que[bac++] = t;
		while(bac > fro){
			u = que[fro++];
			_fev(p, u)
				if (!h[v = to[p]] && cap[p^1])
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
};

struct RetFlow{
	typedef Graph::gtype gtype;
	int s, t, n, t_s, ecnt, tp, ntp,
		from[maxm],
		to[maxm],
		iid[maxn],
		oid[maxn],
		eid[maxm],
		stk[maxk],
		stkn[maxn];
	gtype 
		avaf, tot, 
		dlt[maxm],
		ori[maxm];
	bool
		instkn[maxn],
		tag[maxm];
	Graph g;
	void init(){ ntp = tp = ecnt = n = avaf = tot = 0; ms(dlt, 0); ms(tag, false); g.init(); }
	void add_point(int num){ g.ncnt += num; n += num; }
	void add_edge(int u, int v, int c){
		eid[++ecnt] = g.add_edge(u+2, v+2, c);
		from[ecnt] = u; to[ecnt] = v;
	}
	void prepare(int s_, int t_){
		s = s_; t = t_; t_s = g.add_edge(t+2, s+2, inf);
		_rfor(i, 1, n){
			iid[i] = g.add_edge(1, i+2, 0);
			oid[i] = g.add_edge(i+2, 2, 0);
		}
	}
	bool add_tag(int e){
		if (tag[e]) return false;
		stk[tp++] = e;
		tag[e] = true;
		re int id = eid[e], u = from[e], v = to[e], flow = g.cap[id^1];
		ori[e] = g.cap[id] + flow;
		dlt[u] += flow; dlt[v] -= flow;
		g.cap[id] = g.cap[id^1] = 0;
		if (!instkn[u])
			instkn[stkn[ntp++] = u] = true;
		if (!instkn[v])
			instkn[stkn[ntp++] = v] = true;
			
		return true;
	}
	void set_io(){
		tot = 0; re int i;
		_for(p, 0, ntp){
			i = stkn[p];
			if (dlt[i] > 0)
				tot += dlt[i],
				g.cap[iid[i]] = dlt[i];
			else if (dlt[i] < 0)
				g.cap[oid[i]] = -dlt[i];
		}
	}
	void clear_io(){
		re int id, i;
		_for(p, 0, ntp){
			i = stkn[p];
			if (dlt[i] > 0)
				g.cap[id = iid[i]] = 0, g.cap[id^1] = 0, dlt[i] = 0;
			else if (dlt[i] < 0)
				g.cap[id = oid[i]] = 0, g.cap[id^1] = 0, dlt[i] = 0;
			instkn[i] = false;
		}
	}
	void ret_flow(){
		set_io();
		
		g.cap[t_s] = inf-avaf; g.cap[t_s^1] = avaf;
		g.s = 1; g.t = 2;
		g.isap();
		if (g.mf != tot) pf("Wrong in g.mf != tot!!!\n"); //debug
		avaf = g.cap[t_s^1];
		
		clear_io();
		ntp = 0;
	}
	void max_flow(){
		g.cap[t_s] = g.cap[t_s^1] = 0;
		g.s = s+2; g.t = t+2;
		g.isap();
		avaf += g.mf;
	}
	void rmv_tag(){
		re int id, e;
		_for(i, 0, tp){
			e = stk[i];
			id = eid[e];
			g.cap[id] = ori[e];
			tag[e] = false;
		}
		tp = 0;
	}
} rfg;

int main(){
	#ifndef ONLINE_JUDGE
	freopen("in.txt", "r", stdin);
	//freopen("output.txt", "w", stdout);
	#endif
	
	re int n = rdnt(), m = rdnt(), s = rdnt(), t = rdnt(), u, v, c;
	rfg.init(); rfg.add_point(n);
	_rfor(i, 1, m){
		u = rdnt(); v = rdnt(); c = rdnt();
		rfg.add_edge(u, v, c);
	}
	rfg.prepare(s, t);
	
	re int q = rdnt(), k, e;
	while(q--){
		k = rdnt();
		_rfor(i, 1, k){
			e = rdnt();
			rfg.add_tag(e);
		}
		rfg.ret_flow();
		rfg.max_flow();
		rfg.rmv_tag();
		pf("%d\n", rfg.avaf);
	}
	
	return 0;
}



