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
#define _for(i, a, b) for(re int (i) = (a); (i) < (b); ++(i))
#define _rfor(i, a, b) for(re int (i) = (a); (i) <= (b); ++(i))
#define ll long long
#define maxn 10005
#define maxk 31
#define mod 1000000007ll

int n, k, pos, maxf, pcnt = 0,
	sz[maxk],
	cnt[maxk], 
	ftr[maxn],
	prm[maxn];
ll	ans, 
	f[maxk][maxn],
	h[maxk],
	ivf[maxk],
	fac[maxk];	
bool
	vis[maxn];

ll fp(re ll x, re ll n){
	re ll y = 1;
	for(; n; x = x*x%mod, n >>= 1) if (n&1) y = x*y%mod;
	return y;
}

ll get_inv(re ll x){ return fp(x, mod-2); }

void add(ll &x, ll &y){
	if ((x+=y) >= mod) x -= mod;
}

void upf(){
	f[pos][0] = 1;
	_rfor(i, 0, maxf){
		if (i >= sz[pos]) f[pos][i] = (f[pos][i-sz[pos]] + f[pos-1][i]) % mod;
		else f[pos][i] = f[pos-1][i];
	}
}

void dfs(int up){
	for(re int i = sz[pos]; 2*i <= k - up; ++i){
		++cnt[sz[++pos] = i]; upf();
		dfs(up+i);
		--pos; --cnt[i];
	}
	++cnt[sz[++pos] = k-up]; upf();
	re ll tmp_ans = fac[k];
	_rfor(i, 1, pcnt) 
		if (tmp_ans) tmp_ans = (tmp_ans*f[pos][ftr[i]]) % mod;
		else break;
	if (tmp_ans){
		_rfor(i, 1, pos) if (sz[i] > 1) tmp_ans = tmp_ans * ivf[sz[i]] % mod * h[sz[i]] % mod;
		_rfor(i, 1, k) if (cnt[i] > 1) tmp_ans = tmp_ans * ivf[cnt[i]] % mod;
	}
	add(ans, tmp_ans);
	--cnt[sz[pos--]];
}

int main(){
	#ifndef ONLINE_JUDGE
	freopen("in.txt", "r", stdin);
	#endif
	
	ivf[0] = fac[0] = ivf[1] = fac[1] = 1;
	_for(i, 2, maxk) fac[i] = i*fac[i-1]%mod, ivf[i] = get_inv(fac[i]);
	
	sf("%d %d", &n, &k);
	_rfor(i, 2, n){
		if (!vis[i]){
			prm[++pcnt] = i;
			for(re int j = 2*i; j <= n; j += i) vis[j] = true;
			for(re int j = i; j <= n; j *= i) ftr[pcnt] += n / j;
			maxf = max(maxf, ftr[pcnt]);
		}
	}
	//pf("%d\n", maxf);
	
	f[0][0] = sz[0] = h[1] = 1;
	ans = pos = h[0] = 0; 
	_rfor(i, 2, k) h[i] = (mod-1)*h[i-1]%mod*(i-1)%mod;
	dfs(0);
	
	pf("%lld\n", ans*ivf[k]%mod);
	
	return 0;
}
























