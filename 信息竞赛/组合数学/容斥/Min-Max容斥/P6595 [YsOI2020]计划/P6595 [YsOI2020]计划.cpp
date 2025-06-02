#include <stdio.h>
#include <algorithm>
#include <string.h>
#include <iostream>
#include <assert.h>
using namespace std;

#define re register
#define sf scanf
#define pf printf
#define nl() putchar('\n')
#define ms(x, val) memset(x, val, sizeof(x))
#define ll long long
#define _for(i, a, b) for(re int (i) = (a); (i) < (b); ++(i))
#define _rfor(i, a, b) for(re int (i) = (a); (i) <= (b); ++(i))
#define maxn 2000005
#define mod 998244353ll

int rdnt(){
	re int x = 0, sign = 1;
	re char c = getchar();
	while (c < '0' || c > '9') { if (c == '-') sign = -1; c = getchar(); }
	while (c >= '0' && c <= '9') x = (x<<3) + (x<<1) + (c ^ 48), c = getchar();
	return x * sign;
}

ll	n, m, t,
	p, q,
	fac[maxn],
	ivf[maxn];

ll	get_C(re int n, re int m){
	return fac[n]*ivf[m]%mod*ivf[n-m]%mod;
}

ll	fp(re ll x, re int n){
	re ll y = 1;
	for(; n; x = x*x%mod, n>>=1) if (n&1) y = x*y%mod;
	return y;
}

ll	get_inv(re ll x){
	return fp(x, mod-2);
}

void add(ll &x, ll y){
	if ((x+=y) >= mod) x -= mod;
}

ll	get_E(re int n){
	re ll
		a = (mod+1-fp(p, n))%mod,
		b = (mod+1-fp((mod+1-q)%mod, n)) % mod,
		w = (2*mod+1-a-b+a*b%mod)%mod,
		tmp = fp((mod+1-a)%mod, t),
		ans = 0;
	add(ans, (mod+1-tmp)%mod * get_inv(a)%mod);
	add(ans, tmp * get_inv(mod+1-w) % mod);
	return ans;
}

int main(){
	#ifndef ONLINE_JUDGE
	freopen("in.txt", "r", stdin);
	#endif
	
	fac[0] = fac[1] = ivf[0] = ivf[1] = 1;
	_for(i, 2, maxn) fac[i] = fac[i-1]*i%mod, ivf[i] = (mod-mod/i)*ivf[mod%i]%mod;
	_for(i, 2, maxn) ivf[i] = (ivf[i]*ivf[i-1])%mod;
	
	n = rdnt(); m = rdnt(); t = rdnt(); q = rdnt(); p = 1;
	_rfor(i, 1, m) p = (p*(mod+1-rdnt()))%mod;
	
	re ll ans1 = 0, ans2 = 0, tmp;
	ans1 = n*q%mod*fp(p, t)%mod * get_inv((mod+1-p*(1-q)%mod)%mod)%mod;
	
	_rfor(i, 1, n){
		tmp = get_C(n, i) * get_E(i) % mod;
		if (!(i&1)) tmp = tmp * (mod-1) % mod;
		add(ans2, tmp);
	}
	
	pf("%lld %lld", ans1, ans2);
	
	return 0;
}



