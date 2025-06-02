#include <stdio.h>
#include <algorithm>
#include <string.h>
#include <iostream>
#include <ctime>
#include <random>
#include <unordered_map>
#include <assert.h>
#include <windows.h>
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
#define maxn 500005
#define inf 0x7fffffffffffffffll
typedef int type;

std::mt19937 *engine;
std::uniform_int_distribution<ll> dstr(0, inf);
ll randnt(){ return	dstr(*engine); }

int uf[maxn];
int stk[maxn];
int find(re int u){
	re int top = 0;
	while(uf[u] > 0) stk[top++] = u, u = uf[u];
	while(top) uf[stk[--top]] = u;
	return u;
}

bool unite(re int u, re int v){
	u = find(u); v = find(v);
	if (u == v) return false;
	if (uf[u] > uf[v]) swap(u, v);
	uf[u] += uf[v];
	uf[v] = u;
	return true;
}

int main(){
	std::mt19937 eg(GetTickCount()); engine = &eg;
	freopen("sample.in", "w", stdout);
	
	re int n = 2000, m = 10000;
	pf("%d %d\n", n, m);
	_rfor(i, 1, m){
		re int e = randnt()%2+1, l = randnt()%n+1, r = randnt()%n+1;
		if (l > r) swap(l, r);
		pf("%d %d %d\n", e, l, r);
	} 
	
	
	return 0;
}




































