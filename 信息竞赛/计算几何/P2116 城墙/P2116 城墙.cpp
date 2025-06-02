#include <stdio.h>
#include <algorithm>
#include <string.h>
#include <iostream>
#include <assert.h>
#include <cmath>
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

#define eps 1e-6
#define zr(x) (-eps <= (x) && (x) <= eps)
#define eq(x, y)  ((y)-eps <= (x) && (x) <= (y)+eps)
#define cl(x, y) (x + eps < y)
#define cle(x, y) (x <= y + eps)
#define cr(x, y) cl(y, x)
#define cre(x, y) cr(y, x)

int sgn(db x){ return x > eps ? 1 : x < -eps ? -1 : 0; }

db sqr(db x){ return x*x; }

struct Point{
	db x, y;
	Point operator-(const Point &t)const{ return {x-t.x, y-t.y}; }
	db operator^(const Point &t)const{ return x*t.y-y*t.x; }
} pnt[maxn]; 

db dis(const Point &p){ return sqrt(sqr(p.x)+sqr(p.y)); } 

int hn;
Point hull[maxn], tmp[2][maxn];
bool convex_hull_cmp(const Point &p1, const Point &p2){
	return eq(p1.x, p2.x) ? cl(p1.y, p2.y) : cl(p1.x, p2.x);
}

int mid;
void get_hull(Point *pnt, int n){
	sort(pnt+1, pnt+1+n, convex_hull_cmp);
	db Y[2] = {min(pnt[1].y, pnt[n].y), max(pnt[1].y, pnt[n].y)};
	
	int k = 2; hn = 0;
	_for(o, 0, 2){
		_rfor(i, 1, n-o){
			Point &p = pnt[o?n-i:i];
			if (!o && cl(p.y, Y[o]) || o && cr(p.y, Y[o])) continue;
			while(hn >= k && sgn(hull[hn]-hull[hn-1]^p-hull[hn]) >= 0) --hn;
			hull[++hn] = p;
		}
		if (!o) mid = hn;
		k = hn+1;
	}
}

int read(){ int x; sf("%d", &x); return x; }

int main(){
	
	int n = read(), L = read();
	_rfor(i, 1, n){
		sf("%lf%lf", &pnt[i].x, &pnt[i].y);
	}
	
	get_hull(pnt, n);
	
	db ans = 0;
	_rfor(i, 1, hn) ans += dis(hull[i]-hull[i%hn+1]);
	
	ans += 2*3.1415926535*L;
	pf("%d\n", (int)(ans+0.5));
	
	return 0;
}






























