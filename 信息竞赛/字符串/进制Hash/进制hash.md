进制 $\text{hash}$ 一般基于字符串，所以可用 $131$ 这个大于 $128$ 的最小质数作为进制。

单 $\text{hash}$ 可以考虑这个大质数 $10^{15}+37$ 作为模数，由于它的量级如此大，所以必须用快速乘。

```cpp
#define mod ((ll)1e15+37)
char str[maxn];
ll
    hsh[maxn],
	epw[maxn];

ll mul(ll x, ll y){ return ((x*y-(ll)((long double)x/mod*y)*mod)%mod+mod)%mod; }

void prepare_hsh(int n){
    epw[0] = 1; hsh[0] = 0;
    _rfor(i, 1, n){
        hsh[i] = (hsh[i-1]*131+str[i])%mod;
    	epw[i] = epw[i-1]*131%mod;
    }
}

ll get_hsh(int l, int r){
    return (hsh[r]-mul(hsh[l-1], epw[r-l+1])+mod)%mod;
}
```

双 $\text{hash}$ 则可取这两个质数 $10^{15}-11$ 和 $10^{15}+37$，它们分别是最靠近 $10^{15}$ 的两个质数。

```cpp
#define mod1 ((ll)1e15-11)
#define mod2 ((ll)1e15+37)
char str[maxn];
ll
    hsh1[maxn],
	hsh2[maxn],
	epw1[maxn],
	epw2[maxn];

ll mul1(ll x, ll y){ return ((x*y-(ll)((long double)x/mod1*y)*mod1)%mod1+mod1)%mod1; }
ll mul2(ll x, ll y){ return ((x*y-(ll)((long double)x/mod2*y)*mod2)%mod2+mod2)%mod2;}

void prepare_hsh(int n){
    epw1[0] = epw2[0] = 1; hsh1[0] = hsh2[0] = 0;
    _rfor(i, 1, n){
        hsh1[i] = (hsh[i-1]*131+str[i])%mod1;
        hsh2[i] = (hsh[i-1]*131+str[i])%mod2;
    	epw1[i] = epw1[i-1]*131%mod1;
        epw2[i] = epw2[i-1]*131%mod2;
    }
}

ll get_hsh1(int l, int r){ return (hsh1[r]-mul1(hsh1[l-1], epw1[r-l+1])+mod1)%mod1; }
ll get_hsh2(int l, int r){ return (hsh2[r]-mul2(hsh2[l-1], epw2[r-l+1])+mod2)%mod2; }
```











