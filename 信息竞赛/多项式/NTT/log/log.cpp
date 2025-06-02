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
#define maxn ((1<<18)+5)
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

mt19937 engine(315);
uniform_int_distribution<ll> dstr(0, 0x7fffffffffffffffll);
ll randnt(){ return dstr(engine); }

struct Cplx{
    static int w; int x, y;
    Cplx operator*(const Cplx &t)const{ return {((ll)x*t.x+(ll)w*y%mod*t.y)%mod, ((ll)x*t.y+(ll)y*t.x)%mod}; }
};
int Cplx::w;

int fp(re int x, re int n){ re int y=1; for(;n;x=(ll)x*x%mod,n>>=1)if(n&1)y=(ll)x*y%mod; return y; }
int get_inv(re int a){ return fp(a, mod-2); }
int lb(re int n){ re int lbn = -1; while(n) ++lbn, n>>=1; return lbn; } 

Cplx fp(Cplx x, int n){
    Cplx y = {1, 0};
    for(; n; x = x*x, n >>= 1) if (n&1) y = x*y;
    return y;
}

int quandratic(int a){
    if (a == 0) return 0;
    if (fp(a, (mod-1)/2) != 1) return -1;
    re int b; a %= mod;
    do{ b = randnt()%mod; }while(fp(Cplx::w=((ll)b*b%mod+mod-a)%mod, (mod-1)/2) == 1);
    Cplx as = fp((Cplx){b, 1}, (mod+1)/2); assert(as.y == 0);
    return as.x*2 < mod ? as.x : mod-as.x;
}

int g, iv[maxn], fct[maxn], ivf[maxn];
void init_ntt(re int n){
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
	iv[1] = ivf[1] = ivf[0] = fct[1] = fct[0] = 1;
	_rfor(i, 2, n){
		iv[i] = (ll)iv[mod%i]*(mod-mod/i)%mod;
		fct[i] = (ll)fct[i-1]*i%mod;
		ivf[i] = (ll)ivf[i-1]*iv[i]%mod;
	}
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
	Poly &inv(){ //2*6ntt(n)
		static int b[maxn], c[maxn];
		re int nn = 1<<lb(n-1)+1; resize(nn);
		b[0] = get_inv(a[0]);
		for(re int n = 2; n <= nn; n <<= 1){
			_for(i, n/2, n) b[i] = 0;
			_for(i, 0, n) c[i] = a[i];
			ntt(b, n); ntt(c, n);
			_for(i, 0, n) c[i] = (ll)b[i]*c[i]%mod;
			intt(c, n);
			_for(i, 0, n) c[i] = i<n/2?c[i+n/2]:0;
			ntt(c, n);
			_for(i, 0, n) c[i] = (ll)b[i]*c[i]%mod;
			intt(b, n); intt(c, n);
			_for(i, n/2, n) b[i] = (mod-c[i-n/2])%mod;
		}
		_for(i, 0, nn) a[i] = b[i];
		return *this; 
	}
	bool sqr_success;
	Poly &sqr(){ //2*(inv(n)+mul(n)) = 36ntt(n)
		static Poly p, q;
		re int nn = 1<<lb(n-1)+1, iv2 = get_inv(2); resize(nn);
		q.a[0] = quandratic(a[0]); q.n = 1;
		if (q.a[0] == -1){ sqr_success = false; return *this; }
		for(re int n = 2; n <= nn; n <<= 1){
			this->n = n;
			(p = q).resize(n).inv().mul(*this);
			_for(i, n/2, n) q.a[i] = (ll)iv2*p.a[i]%mod; q.n = n;
		}
		sqr_success = true;
		return *this = q;
	}
	Poly &dif(){
		if (n) --n;
		_for(i, 0, n) a[i] = (ll)a[i+1]*(i+1)%mod;
		return *this;
	}
	Poly &ing(){
		for(re int i = n++; i >= 1; --i) a[i] = (ll)a[i-1]*iv[i]%mod; a[0] = 0;
		return *this;
	}
	bool log_success;
	Poly &log(){
		static Poly p;
		if (a[0] != 1){ log_success = false; return *this; }
		p = *this; re int tn = n;
		dif().mul(p.inv().resize(tn-1)).resize(tn-1).ing();
		log_success = true;
		return *this;
	}
}p;

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

	re int n = p.n = rdnt(), nn = 1<<lb(n-1)+2;
	init_ntt(nn);
	_for(i, 0, n) p.a[i] = rdnt();
	p.log().resize(n);
	//print(p);
		
	unsigned long long tmp = 0;
	for(int i = 0; i < p.n; ++i) tmp = tmp*mod+p.a[i];
	printf("%llu\n", tmp);
	
	return 0;
}





























