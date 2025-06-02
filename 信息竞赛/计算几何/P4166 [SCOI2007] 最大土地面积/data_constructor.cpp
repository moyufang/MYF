#include <stdio.h>
#include <algorithm>
#include <string.h>
#include <iostream>
#include <assert.h>
#include <random>
#include <windows.h>
#include <cstdlib>
#include <unordered_map>
using namespace std;

#define re register
#define sf scanf
#define pf printf
#define nl() putchar('\n')
#define ms(x, val) memset(x, val, sizeof(x))
#define ll long long
#define db double
#define ull unsigned long long
#define _for(i, a, b) for(re int i = (a); i < (b); ++i)
#define _rfor(i, a, b) for(re int i = (a); i <= (b); ++i)
#define _fev(p, u) for(re int p = head[u]; p; p = nex[p])
#define inf 0x7fffffff
#define maxn 100005
#define mod 998244353ll
std::mt19937 *engine;
std::uniform_int_distribution<ll> dstr(0, inf);
ll randnt(){ return	dstr(*engine); }

int main(){
	std::mt19937 eg(GetTickCount()); engine = &eg;
	srand(randnt());
	freopen("sample.in", "w", stdout);
	
	int n = 2000, maxx = 10000;
	pf("%d\n", n);
	_rfor(i, 1, n){
		int x = randnt()%(2*maxx+1)-maxx,
			y = randnt()%(2*maxx+1)-maxx;
		pf("%d %d\n", x, y);
	}
	
	return 0;
}




































