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
#define maxnn (2*maxn)

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

int ecnt = 2,
	uf[maxn],
	eu[maxn],
	ev[maxn];

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

bool intersect(re int u, re int v, re int tu, re int tv){
	if (u > tu) swap(u, tu), swap(v, tv);
	return u == tu || u == tv || (v >= tu && v <= tv);
}

int main(){
	#ifndef ONLINE_JUDGE
	freopen("sample.in", "r", stdin);
	//freopen("sample.out", "w", stdout);
	#endif
	
	ms(uf, -1);
	re int n = rdnt(), m = rdnt();
	_rfor(i, 1, m){
		re int opt = rdnt(), l = rdnt(), r = rdnt();
		if (l > r) swap(l, r);
		if (opt == 2) pf("%d\n", find(l) == find(r));
		else if (opt == 1){
			re int u = find(l), v = find(r);
			if (u == v) continue;
			unite(u, v);
			_for(j, 1, ecnt){
				re int tu = eu[j], tv = ev[j];
				if (intersect(l, r, tu, tv)) unite(u, tv);
			} 
			++ecnt; eu[ecnt] = l; ev[ecnt] = r;
		}
		//if(i%1000==0) pf("i:%d\n", i);
	}

	return 0;
}







































