#include <stdio.h>
#include <algorithm>
#include <string.h>
#include <iostream>
#include <assert.h>
#include <random> 
using namespace std;

#define re register
#define sf scanf
#define pf printf
#define nl() putchar('\n')
#define ms(x, val) memset(x, val, sizeof(x))
#define ll long long
#define _for(i, a, b) for(re int (i) = (a); (i) < (b); ++(i))
#define _rfor(i, a, b) for(re int (i) = (a); (i) <= (b); ++(i))
#define _fev(p, u) for(re int (p) = head[(u)]; (p); (p) = nex[(p)])
#define inf 0x7fffffffffffffffll
#define maxn 200005
#define maxnn (4*maxn)

template <class T>
void print(string name, T arr[], int n, int flag = 1){
	cout<<name<<":";
	_for(i, 0, n)cout<<" "<<arr[i+flag];
	cout<<endl;
}

int rdnt(){
	re int x = 0, sign = 1;
	re char c = getchar();
	while (c < '0' || c > '9') { if (c == '-') sign = -1; c = getchar(); }
	while (c >= '0' && c <= '9') x = (x<<3) + (x<<1) + (c ^ 48), c = getchar();
	return x * sign;
}

mt19937 engine(315);
uniform_int_distribution<ll> dstr(0, inf);
ll randnt(){ return dstr(engine); }

int pcnt = 0, pool[maxnn];
struct Union{ int nd, uf, le, ri, rt; } union_[maxn];
struct Treap{ int bl, fa, vl, sz, lc, rc, L, R; bool tag; ll pri; } treap[maxnn];
#define nd(x) union_[x].nd
#define uf(x) union_[x].uf
#define le(x) union_[x].le
#define ri(x) union_[x].ri
#define rt(x) union_[x].rt
#define bl(x) treap[x].bl
#define fa(x) treap[x].fa
#define vl(x) treap[x].vl
#define sz(x) treap[x].sz
#define lc(x) treap[x].lc
#define rc(x) treap[x].rc
#define tag(x) treap[x].tag
#define R(x) treap[x].R
#define L(x) treap[x].L
#define pri(x) treap[x].pri
int new_node(re int nn){ re int x = pool[pcnt--]; treap[x] = treap[nn]; pri(x) = randnt(); return x; }
void del_node(re int x){ pool[++pcnt] = x; }

int stk[maxn];
int find(re int u){
	re int top = 0;
	while(uf(u) > 0) stk[top++] = u, u = uf(u);
	while(top) uf(stk[--top]) = u;
	return u;
}

//void print(int x, int dep = 0){
//	_for(i, 0, dep) pf("\t");
//	if (x == 0){ pf("none\n"); return; }
//	pf("bl:%d vl:%d sz:%d tag:%d L:%d R:%d\n", bl(x), vl(x), sz(x), tag(x), L(x), R(x));
//	print(lc(x), dep+1);
//	print(rc(x), dep+1);
//}
//
//void dfs(int x, vector<int> &vec){
//	if (!x) return;
//	dfs(lc(x), vec);
//	vec.push_back(vl(x));
//	dfs(rc(x), vec);
//}

bool cmp(re int v1, re int v2){ return le(v1) < le(v2); }

void ud(re int x){
	sz(x) = 1; tag(x) = true;
	if (lc(x)){
		tag(x) &= tag(lc(x)) && R(lc(x))+1 == le(vl(x));
		sz(x) += sz(lc(x));
		L(x) = L(lc(x));
		fa(lc(x)) = x;
		bl(lc(x)) = -1;
	}
	else L(x) = le(vl(x));
	if (rc(x)){
		tag(x) &= tag(rc(x)) && ri(vl(x))+1 == L(rc(x));
		sz(x) += sz(rc(x));
		R(x) = R(rc(x));
		fa(rc(x)) = x;
		bl(rc(x)) = -1;
	}
	else R(x) = ri(vl(x));
	fa(x) = 0;
}

void split(int x, int v, int &ltr, int &rtr, bool fg){
	if (!x){ ltr = rtr = 0; return; }
	bool judge = (fg?vl(x)==v:false) || cmp(vl(x), v);
	if (judge){
		ltr = x;
		split(rc(x), v, rc(ltr), rtr, fg);
		ud(ltr);
	}
	else{
		rtr = x;
		split(lc(x), v, ltr, lc(rtr), fg);
		ud(rtr);
	}
}

int merge(int x, int y){
	if (!x || !y){ if (x+y != 0) ud(x+y); return x+y; }
	if (pri(x) <= pri(y)){
		rc(x) = merge(rc(x), y); ud(x);
		return x;
	}
	else{
		lc(y) = merge(x, lc(y)); ud(y);
		return y;
	}
}

void ins(int &x, int v){
	re int ltr = 0, rtr = 0, d = new_node(0);
	sz(d) = 1; vl(nd(v) = d) = v; tag(d) = true; L(d) = le(v); R(d) = ri(v);
	split(x, v, ltr, rtr, false);
	x = merge(merge(ltr, d), rtr);
}

void del(re int &rt, re int v){
	if (!rt) return;
	re int ltr = 0, mtr = 0, rtr = 0;
	split(rt, v, ltr, mtr, false);
	split(mtr, v, mtr, rtr, true);
	rt = merge(ltr, rtr);
	nd(v) = 0;
	if (mtr) del_node(mtr);
}

bool unite(re int u, re int v){
	uf(u) += uf(v);
	uf(v) = u;
	nd(v) = le(v) = ri(v) = 0;
	return true;
}

void ud_bl(re int x){
	if (rt(x)) bl(rt(x)) = x;
}

int get_uf_fa(re int x){
	x = nd(x);
	while(fa(x)) x = fa(x);
	return bl(x);
}

void go_up(re int x, re int v){
	while(x != v){
		re int pa = get_uf_fa(x), ltr = 0, mtr = 0, rtr = 0;
		split(rt(pa), vl(x), ltr, mtr, false);
		split(mtr, vl(x), mtr, rtr, true);
		del_node(mtr);
		rt(pa) = merge(merge(ltr, rt(x)), rtr);
		ud_bl(pa); unite(pa, x); rt(x) = 0;
		x = pa;
	}
}

//void print_tree(int x, int dep = 0){
//	_for(i, 0, dep) pf("\t");
//	pf("x:%d le:%d ri:%d fa:%d\n", x, le(x), ri(x), x==0?-1:get_uf_fa(x)); 
//	vector<int> vec;
//	dfs(rt(x), vec);
//	for(auto i : vec) print_tree(i, dep+1);
//}

int main(){
//	#ifndef ONLINE_JUDGE
//	freopen("sample.in", "r", stdin);
//	freopen("sample.out", "w", stdout);
//	#endif
	
	uf(0) = -1;
	_for(i, 1, maxnn) pool[++pcnt] = maxnn-i; 
	bl(0) = fa(0) = -1; sz(0) = vl(0) = lc(0) = rc(0) = tag(0) = L(0) = R(0) = 0;
	
	re int n = rdnt(), m = rdnt();
	le(0) = 0; ri(0) = n+1;
	_rfor(i, 1, n){
		uf(i) = -1, le(i) = ri(i) = i, rt(i) = 0;
		ins(rt(0), i);
	}
	ud_bl(0);
	
	_rfor(i, 1, m){
		re int opt = rdnt(), l = rdnt(), r = rdnt();
		if (opt == 2) pf("%d", find(l) == find(r));
		else if (opt == 1){
			if (l > r) swap(l, r);
			re int u = find(l), v = find(r), tu, tv, lca, tar, tmp = u;
			if (u == v) continue;
			
			while(!(l >= le(tmp) && r <= ri(tmp))) tmp = get_uf_fa(tmp);
			if (tmp == u || tmp == v){
				lca = tar = tmp;
				go_up(u, lca); go_up(v, lca);
			}
			else{
				re int ltr = 0, mtr = 0, rtr = 0;
				lca = tmp; tu = u; tv = v;
				while((tmp = get_uf_fa(tu)) != lca) tu = tmp;
				while((tmp = get_uf_fa(tv)) != lca) tv = tmp;
				split(rt(lca), tu, ltr, mtr, false);
				split(mtr, tv, mtr, rtr, true);
				if (tag(mtr)){
					ri(tu) = ri(tv);
					del(mtr, tu); del(mtr, tv);
					ud_bl(tu); ud_bl(tv);
					go_up(u, tu); go_up(v, tv);
					rt(tu) = merge(merge(rt(tu), mtr), rt(tv));
					rt(tv) = 0;
					unite(tu, tv); ud_bl(tu);
					re int d = new_node(0);
					sz(d) = 1; vl(nd(tu) = d) = tu; tag(d) = true; L(d) = le(tu); R(d) = ri(tu);
					rt(lca) = merge(merge(ltr, d), rtr);
				}
				else{
					tar = lca;
					rt(lca) = merge(merge(ltr, mtr), rtr);
					go_up(u, lca); go_up(v, lca);
				}
			}
			ud_bl(lca);
		}
	}

	return 0;
}




