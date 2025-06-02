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
#define _fev(p, u) for(re int (p) = head[(u)]; (p); (p) = nex[(p)])
#define inf 0x7fffffff
#define maxn 505
#define mod 998244353ll
#define maxx 1000

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

int fp(re int x, re int n){
	re int y = 1;
	for(; n; x = (ll)x*x%mod, n>>=1) if(n&1) y = (ll)x*y%mod;
	return y;
}

int get_inv(re int x){
	return fp(x, mod-2);
}

void add(re int &x, re int y){ if((x+=y)>=mod) x-=mod; }

int
    arr[maxn],
    fbuc[maxx],
    buc[maxx],
    ans[maxn],
	inv[maxn],
	C[maxn][maxn],
	p[maxn][maxn],
	q[maxn][maxn],

	top = 0,
	tmp[100][maxn][maxn];

void init(re int n){
    inv[0] = 1; inv[1] = get_inv(2);
    _rfor(i, 2, n) inv[i] = (ll)inv[i-1]*inv[1] % mod;
    C[0][0] = 1;
    _rfor(i, 1, n){
        C[i][0] = C[i][i] = 1;
        _for(j, 1, i) C[i][j] = (C[i-1][j] + C[i-1][j-1]) % mod;
    }
    _rfor(i, 1, n) q[0][i] = p[i][0] = 0, q[i][0] = 1; p[1][0] = 1;
    _rfor(i, 1, n) _rfor(j, 1, n){
        p[i][j] = (ll)inv[i+j-1]*C[i+j-2][i-1]%mod;
        q[i][j] = (q[i-1][j] + p[i][j]) % mod;
    }
}

void dp(re int (*f)[maxn], re int (*g)[maxn], re int (*h)[maxn], re int a, re int b, re int c){
    _rfor(i, 1, c) _rfor(k, 1, c) h[i][k] = 0;
    _rfor(i, 1, a+1) _rfor(j, 1, b+1){
        if (i == a+1 && j == b+1) continue;
        else if (i == a+1) _rfor(l, 1, b) add(h[a+j][a+l], (ll)g[j][l]*q[j][a]%mod);
        else if (j == b+1) _rfor(k, 1, a) add(h[b+i][k], (ll)f[i][k]*q[i][b]%mod);
        else{
            _rfor(k, 1, a) add(h[i+j-1][k], (ll)f[i][k]*p[i][j]%mod);
            _rfor(l, 1, b) add(h[i+j-1][a+l], (ll)g[j][l]*p[i][j]%mod);
        }
    }
}

int x;
void merge(int &cnt, int (*h)[maxn], int tl, int tr){
    if (tl == tr){
        cnt = arr[tl] == x;
        if (cnt == 1) h[1][1] = 1;
        //pf("cnt:%d\n", cnt);nl();
        return;
    }
    int mi = (tl+tr)>>1;
    int (*f)[maxn] = tmp[top++], (*g)[maxn] = tmp[top++], a, b;
    merge(a, f, tl, mi);
    merge(b, g, mi+1, tr);
    cnt = a+b;
    dp(f, g, h, a, b, cnt);
//    pf("tl:%d tr:%d a:%d b:%d cnt:%d\n", tl, tr, a, b, cnt);
//    _rfor(i, 1, cnt){
//        pf("i|%d: ", i);
//        re int tmp = 0;
//        _rfor(j, 1, cnt){
//            add(tmp, h[i][j]);
//            pf("%d|%d ", j, (ll)h[i][j]*8%mod);
//        }
//        pf("tmp:%d\n", tmp);
//    }
//    nl();
    top -= 2;
}

int main(){
	//freopen("sample.in", "r", stdin);
	//freopen(".out", "w", stdout);

    re int n = rdnt(), cnt, (*f)[maxn] = tmp[top++];
//    re ll sz = sizeof(tmp);
//    pf("sz:%lld\n", sz/1024/1024);
    init(n);
    _rfor(i, 1, n) ++fbuc[arr[i] = rdnt()];
    _rfor(i, 1, maxx) buc[i] = fbuc[i] + buc[i-1];
    _rfor(_, 1, maxx){
        x = _;
        if (!fbuc[x]) continue;
        merge(cnt, f, 1, n);
        re int m = 0;
        _rfor(i, 1, n){
            if (arr[i] != x) continue;
            ++m;
            _rfor(j, 1, cnt) add(ans[i], (ll)(buc[x-1] + j)*f[j][m]%mod);
        }
        assert(m == cnt);
    }

    _rfor(i, 1, n) pf("%d ", ans[i]);
    nl();

	return 0;
}
