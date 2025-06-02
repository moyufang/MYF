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

int main(){
	std::mt19937 eg(GetTickCount()); engine = &eg;
	freopen("sample.in", "w", stdout);
	
	re int mx = 100000, n = mx, m = mx, maxa = 998244353;
	pf("%d\n", n);
	re int a0;
	do{ a0 = randnt()%maxa; } while(a0 == 0);
	pf("%d ", a0);
	_for(i, 1, n) pf("%lld ", randnt()%maxa); nl();
	
	
	return 0;
}




































