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
#define ll long long
#define mmin(a, b) (((b) < (a)) ? (b) : (a))
#define mmax(a, b) (((b) > (a)) ? (b) : (a))
#define maxn 2005
#define maxm 300005
#define inf 0x7fffffff

template <class T>
void print(string name, T arr[], int n, int flag = 1){
	cout<<name<<":";
	if (flag) _rfor(i, 1, n)cout<<" "<<arr[i];
	else _for(i, 0, n)cout<<" "<<arr[i];
	cout<<endl;
}

int rdnt(){
	register int x = 0, sign = 1;
	register char c = getchar();
	while (c < '0' || c > '9') { if (c == '-') sign = -1; c = getchar(); }
	while (c >= '0' && c <= '9') x = (x<<3) + (x<<1) + (c ^ 48), c = getchar();
	return x * sign;
}

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

struct Edge{
	int u, v, c;
	Edge(int u = 0, int v = 0, int c = 0):u(u), v(v), c(c){}	
} edge[maxm];

bool ban[maxm];

int main(){
	#ifndef ONLINE_JUDGE
	freopen("in.txt", "r", stdin);
	//freopen("output.txt", "w", stdout);
	#endif
	
	re int n = rdnt(), m = rdnt(), s = rdnt(), t = rdnt(), u, v, c;
	_rfor(i, 1, m){
		u = rdnt(); v = rdnt(); c = rdnt();
		edge[i] = (Edge){u, v, c};
	}
	re int q = rdnt(), k, e;
	_rfor(i, 1, q){
		k = rdnt(); ms(ban, false);
		_rfor(j, 1, k) ban[rdnt()] = true;
		g.init(); g.ncnt = n;
		_rfor(j, 1, m){
			if (!ban[j]){
				g.add_edge(edge[j].u, edge[j].v, edge[j].c);
			}
		}
		g.s = s; g.t = t;
		g.isap();
		pf("%d\n", g.mf);
	}
	
	
	return 0;
}

