#include <stdio.h>
#include <algorithm>
#include <string.h>
#include <iostream>
#include <cmath>
#include <assert.h>
#include <random> 
using namespace std;

#define re register
#define sf scanf
#define pf printf
#define nl() putchar('\n')
#define ms(x, val) memset(x, val, sizeof(x))
#define ll long long
#define ull unsigned long long
#define _for(i, a, b) for(re int (i) = (a); (i) < (b); ++(i))
#define _rfor(i, a, b) for(re int (i) = (a); (i) <= (b); ++(i))
#define _fev(p, u) for(re int (p) = head[(u)]; (p); (p) = nex[(p)])
#define inf 0x7fffffff
#define maxn ((1<<20)+5)
#define mod 998244353 

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

int fp(re int x, re int n){ re int y=1; for(;n;x=(ll)x*x%mod,n>>=1)if(n&1)y=(ll)x*y%mod; return y; }
int get_inv(re int a){ return fp(a, mod-2); }
int lb(re int n){ re int lbn = -1; while(n) ++lbn, n>>=1; return lbn; } 

int g;
void init_ntt(){
	int fac[100], fcnt = 0, t = mod-1, sqr = sqrt(t+0.5);
	_rfor(x, 2, sqr){
		if (t == 1) break;
		if (t % x == 0){ fac[++fcnt] = x; while(t % x == 0) t /= x; }
	}
	if (t != 1) fac[++fcnt] = t;
	_rfor(x, 2, 1000){
		g = x;
		_rfor(i, 1, fcnt) if (fp(x, (mod-1)/fac[i]) == 1){ g = 0; break; }
		if (g) break;
	}
	assert(g != 0);
}

void ntt(re int *arr, re int n, re bool fg = false){
	assert(n == (n&-n));
	static ull a[maxn]; static int tr[maxn], w[maxn]; 
	re int lbn = lb(n); tr[0] = 0; w[0] = 1;
	_for(i, 1, n) tr[i] = (tr[i>>1]>>1)|((i&1)<<lbn-1);
	_for(i, 0, n) a[i] = arr[tr[i]];
	for(re int b = 1, t; b < n; b <<= 1){
		w[1] = fp(g, (mod-1)/(2*b));
		_for(k, 2, b) w[k] = (ll)w[k-1]*w[1]%mod;
		for(re ull *p = a; p < a + n; p += b<<1) _for(k, 0, b)
			t = p[b|k]*w[k]%mod, p[b|k] = p[k]+mod-t, p[k] += t;
		if (b == (1<<17)) _for(i, 0, n) a[i] %= mod;
	}
	if(fg){
		re ull d = get_inv(n); arr[0] = a[0]%mod*d%mod;
		_for(i, 1, n) arr[i] = a[n-i]%mod*d%mod;
	}
	else _for(i, 0, n) arr[i] = a[i]%mod;
}

#define intt(arr, n) ntt(arr, n, true)

struct Poly{
	int n, a[maxn];
	Poly &resize(re int nn){ _for(i, n, nn) a[i] = 0; n = nn; return *this; }
	Poly &operator=(const Poly &t){ n = t.n; _for(i, 0, n) a[i] = t.a[i]; return *this; }
	Poly &mul(const Poly &t){ //3ntt(2n)
		static int b[maxn];
		re int nn = 1<<lb(n+t.n-2)+1; resize(nn);
		_for(i, 0, nn) b[i] = i<t.n?t.a[i]:0;
		ntt(a, nn); ntt(b, nn);
		_for(i, 0, nn) a[i] = (ll)a[i]*b[i]%mod;
		intt(a, nn);
		return *this;
	}
}p, q;

void print(const Poly &p){
	_for(i, 0, p.n) pf("%d ", p.a[i]); nl();
}

void print(string name, const Poly &p){
	cout<<name<<": "; print(p);
}

int main(){
	#ifndef ONLINE_JUDGE
	freopen("sample.in", "r", stdin);
	//freopen("sample.out", "w", stdout);
	#endif

	init_ntt();
	re int n = p.n = rdnt();
	_for(i, 0, n) p.a[i] = rdnt();
	re int m = q.n = rdnt();
	_for(i, 0, m) q.a[i] = rdnt();
	p.mul(q).resize(n+m-1);
	//print(p);

	unsigned long long tmp = 0;
	for(int i = 0; i < p.n; ++i) tmp = tmp*mod+p.a[i];
	printf("%llu\n", tmp);
	
	
	
	return 0;
}



