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
#define mc(x, y, sz) memcpy(x, y, (sz)*sizeof(int))
#define ll long long
#define ull unsigned long long
#define _for(i, a, b) for(re int (i) = (a); (i) < (b); ++(i))
#define _rfor(i, a, b) for(re int (i) = (a); (i) <= (b); ++(i))
#define _fev(p, u) for(re int (p) = head[(u)]; (p); (p) = nex[(p)])
#define inf 0x7fffffff
#define maxn 800005
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

int fp(re int x, re int n){
	re int y = 1;
	for(; n; x=(ll)x*x%mod, n>>=1)if(n&1)y=(ll)x*y%mod;
	return y;
}

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

int get_inv(re int a){ return fp(a, mod-2); }

int lb[maxn], g;
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
	lb[0] = -1;
	_rfor(i, 1, n) lb[i] = lb[i>>1]+1;
}

void ntt(re int *arr, re int n, re bool fg = false){ //3nlogn
	assert(n == (n&-n));
	static ull a[maxn]; static int tr[maxn], omg[maxn]; 
	re int lbn = lb[n]; tr[0] = 0; omg[0] = 1;
	_for(i, 1, n) tr[i] = (tr[i>>1]>>1)|((i&1)<<lbn-1);
	_for(i, 0, n) a[i] = arr[tr[i]];
	for(re int b = 1, t; b < n; b <<= 1){
		omg[1] = fp(g, (mod-1)/(2*b));
		_for(k, 2, b) omg[k] = (ll)omg[k-1]*omg[1]%mod;
		for(re ull *p = a; p < a + n; p += b<<1) _for(k, 0, b)
			t = p[b|k]*omg[k]%mod, p[b|k] = p[k]+mod-t, p[k] += t;
		if (b == (1<<17)) _for(i, 0, n) a[i] %= mod;
	}
	if(fg){
		re ull d = get_inv(n); arr[0] = a[0]*d%mod;
		_for(i, 1, n) arr[i] = a[n-i]%mod*d%mod;
	}
	else _for(i, 0, n) arr[i] = a[i]%mod;
}

#define intt(arr, n) ntt(arr, n, true)
#define dot(a, b, c, n) _for(i, 0, n) (c)[i] = (ll)(a)[i]*(b)[i]%mod

struct Poly{
	int n, a[maxn];
	Poly(){}
	Poly(const Poly &t){ *this = t; }
	Poly &normal(){ while(n && a[n-1] == 0) --n; return *this; }
	Poly &operator=(const Poly &t){ n = t.n; mc(a, t.a, t.n); return *this; }
	Poly &resize(re int nn){ _for(i, n, nn) a[i] = 0; n = nn; return *this; }
	Poly &rev(){ _for(i, 0, n/2) swap(a[i], a[n-i-1]); return *this; }
	Poly &mul(const Poly &t){//3ntt(2n)
		static int tmp[maxn];
		re int nn = 1<<(lb[n+t.n-2]+1);
		_for(i, n, nn) a[i] = 0;
		_for(i, 0, nn) tmp[i] = i<t.n?t.a[i]:0;
		ntt(a, nn); ntt(tmp, nn);
		dot(a, tmp, a, nn);
		intt(a, nn); n = nn;
		return normal();
	}
	Poly &inv(){//2*3ntt(2n)
		static int b[maxn], c[maxn];
		re int nn = 1<<(lb[n-1]+1);
		b[0] = get_inv(a[0]);
		for(re int n = 2; n <= nn; n <<= 1){
			_for(i, 0, 2*n) c[i] = i<n?a[i]:0;
			_for(i, n/2, 2*n) b[i] = 0;
			ntt(c, 2*n); ntt(b, 2*n);
			_for(i, 0, 2*n) b[i] = (ll)b[i]*(2+mod-(ll)b[i]*c[i]%mod)%mod;
			intt(b, 2*n);
		}
		_for(i, 0, nn) a[i] = b[i]; n = nn;
		return normal();
	}
	bool sqr_success;
	Poly &sqr(){ //2*(6+3)ntt(2n)
		re int nn = 1<<(lb[n-1]+1), inv2 = get_inv(2);
		static Poly p, q;
		q.a[0] = quandratic(a[0]); q.n = 1;
		if (q.a[0] == 0 || q.a[0] == -1){ sqr_success = false; return *this; }
		assert((ll)q.a[0]*q.a[0]%mod==a[0]%mod);
		for(re int n = 2; n <= nn; n <<= 1){
			p = q; q.resize(this->n = n).inv().mul(*this).resize(n);
			_for(i, 0, n) q.a[i] = (ll)inv2*((i<p.n?p.a[i]:0)+q.a[i])%mod;
		}
		sqr_success = true;
		return *this = q.normal();
	}
	Poly &div(const Poly &d, Poly &q, Poly &r){
		assert(d.n <= n); re int tn = n-d.n+1;
		(q = d).rev().resize(tn).inv().mul((r = *this).rev().resize(tn)).resize(tn).rev();
		(r = q).mul(d);
		_for(i, 0, d.n-1) r.a[i] = (a[i]+mod-r.a[i])%mod; 
		_for(i, d.n-1, r.n) assert((a[i]-r.a[i])%mod == 0);
		r.resize(d.n-1);
		q.normal(); r.normal();
		return *this;
	}
	Poly &dif(){
		if (n) --n;
		_for(i, 0, n) a[i] = (ll)a[i+1]*(i+1)%mod;
		return *this;
	}
	Poly &ing(){
		for(re int i = n++; i >= 1; --i) a[i] = (ll)a[i-1]*get_inv(i)%mod;
		a[0] = 0;
		return *this;
	}
	bool log_success;
	Poly &log(){
		static Poly p; p = *this; re int tn = n;
		if (a[0] != 1){ log_success = false; return normal(); }
		dif().mul(p.inv()).resize(tn-1).ing(); 
		log_success = true;
		return normal();
	}
	bool exp_success;
	Poly &exp(){
		static Poly p, q;
		re int nn = 1<<(lb[n-1]+1); resize(nn);
		if (a[0] != 0){ exp_success = false; return normal(); }
		q.a[0] = 1; q.n = 1;
		for(re int n = 2; n <= nn; n <<= 1){
			p = q;
			q.resize(n).log().resize(n);
			_for(i, 0, n) q.a[i] = (mod-q.a[i]+a[i])%mod;
			q.a[0] = (q.a[0]+1)%mod; 
			q.mul(p).resize(n);
		}
		exp_success = true;
		return *this = q.normal();
	}
	Poly &left(re int m){
		if (m >= n) return resize(0);
		n = n-m;
		_for(i, 0, n) a[i] = a[i+m]; 
		return *this;
	}
	Poly &right(re int m){
		assert(n + m < maxn-5);
		n += m;
		for(re int i = n-1; i >= m; --i) a[i] = a[i-m];
		_for(i, 0, m) a[i] = 0;
		return *this;
	}
	Poly &fpp(re int k, re int k_, re int m){
		n = min(n, m);
		re int b = 0, d;
		while(b < n && a[b] == 0) ++b;
		if (b == n || (ll)k*b >= m) return resize(0);
		left(b); resize(m-k*b);
		d = get_inv(a[0]); a[0] = 1;
		_for(i, 1, n) a[i] = (ll)a[i]*d%mod;
		log().resize(n);
		_for(i, 1, n) a[i] = (ll)a[i]*k%mod;
		exp(); resize(m-k*b); right(k*b);
		d = fp(get_inv(d), k_);
		_for(i, 0, n) a[i] = (ll)a[i]*d%mod;
		return normal();
	}
}p;

void print(const Poly &p){
	_for(i, 0, p.n) pf("%d ", p.a[i]); nl();
}

void print(string name, const Poly &p){
	cout<<name<<": "; print(p);
}

bool read_flag;
int y = 0;
int read(){
	re int x = 0, s = 0; re char c = getchar();
	while(c < '0' || c > '9') c = getchar();
	while(c >= '0' && c <= '9')
		++s,
		x = ((ll)x*10+(c^48))%mod,
		y = ((ll)y*10+(c^48))%(mod-1),
		c = getchar();
	read_flag = (s >= 6);
	return x;
}

int main(){
	#ifndef ONLINE_JUDGE
	freopen("sample.in", "r", stdin);
	//freopen("sample.out", "w", stdout);
	#endif

	init_ntt(maxn-5);
	re int n = p.n = rdnt(), k = read();
	_for(i, 0, n) p.a[i] = rdnt();
	if (p.a[0] == 0 && read_flag){ _for(i, 0, n) pf("%d ", 0); nl(); return 0; }
	p.fpp(k, y, n); p.resize(n);
	
	//print(p);

	unsigned long long tmp = 0;
	for(int i = 0; i < p.n; ++i) tmp = tmp*mod+p.a[i];
	printf("%llu\n", tmp);
	
	
	
	return 0;
}



