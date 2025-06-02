#include <stdio.h>
#include <algorithm>
#include <string.h>
#include <iostream>
#include <assert.h>
#include <cmath>
using namespace std;

#define sf scanf
#define pf printf
#define nl() putchar('\n')
#define ms(x, val) memset(x, val, sizeof(x))
#define ll long long
#define db double
#define ull unsigned long long
#define _for(i, a, b) for(int i = (a); i < (b); ++i)
#define _rfor(i, a, b) for(int i = (a); i <= (b); ++i)
#define _dfor(i, b, a) for(int i = (b); i >= (a); --i)
#define _fev(p, u) for(int p = head[u]; p; p = nex[p])
#define inf 0x7fffffff
#define maxn 100005
#define mod 998244353ll

template <class T>
void print(string name, T arr[], int n, int flag = 1){
	cout<<name<<":";
	_for(i, 0, n)cout<<" "<<arr[i+flag];
	cout<<endl;
}

void tab(int n){
	assert(n >= 0);
	while(n--) putchar('\t');
}

int read(){
	int x = 0, sign = 1;
	char c = getchar();
	while (c < '0' || c > '9') { if (c == '-') sign = -1; c = getchar(); }
	while (c >= '0' && c <= '9') x = (x<<3) + (x<<1) + (c ^ 48), c = getchar();
	return x * sign;
}

#define eps 1e-6
#define eq(x, y)  ((y)-eps <= (x) && (x) <= (y)+eps)
#define cl(x, y) (x + eps < y)
#define cle(x, y) (x <= y + eps)
#define cr(x, y) cl(y, x)
#define cre(x, y) cr(y, x)
bool zr(db x){ return -eps <= x && x <= eps; }
int sgn(db x){ return x > eps ? 1 : x < -eps ? -1 : 0; }
db sqr(db x){ return x*x; }

struct Point{
	db x, y;
	//Point &operator=(const Point &t){ x = t.x; y = t.y; return *this; }
	Point operator-(const Point &t)const{ return {x-t.x, y-t.y}; }
	db operator^(const Point &t)const{ return x*t.y-y*t.x; }
} pnt[maxn], h[maxn];

struct Line{
	Point a, b;
};

bool anticlock(const Point &a, const Point &b, const Point &c){ return (b-a^c-b) >= -eps; }

bool convex_hull_cmp(const Point &p1, const Point &p2){
	return eq(p1.x, p2.x) ? cl(p1.y, p2.y) : cl(p1.x, p2.x);
}

int hn, mid;
void get_hull(Point *hull, int &hn, Point *pnt, int n){
	sort(pnt+1, pnt+1+n, convex_hull_cmp);
	db Y[2] = {min(pnt[1].y, pnt[n].y), max(pnt[1].y, pnt[n].y)};
	int k = 2; hn = 0;
	_for(o, 0, 2){
		_rfor(i, 1, n-o){
			Point &p = pnt[o?n-i:i];
			if (!o && cl(p.y, Y[o]) || o && cr(p.y, Y[o])) continue;
			while(hn >= k && anticlock(hull[hn-1], hull[hn], p)) --hn;
			hull[++hn] = p;
		}
		if (!o) mid = hn;
		k = hn+1;
	}
}

db area(const Point &p, const Line &l){ return fabs(p-l.a^l.b-l.a); }

int main(){
	freopen("sample.in", "r", stdin);

	int n; sf("%d", &n);
	_rfor(i, 1, n) sf("%lf%lf", &pnt[i].x, &pnt[i].y);
	
	get_hull(h, hn, pnt, n);
	
	//pf("hn:%d\n", hn);
	
//	db ans = 0;
//	int i = 1, j = mid, a = 1, b = mid;
//	while(true){
//		Line l = (Line){h[i], h[j]};
//		a = max(i, a); b = max(j, b);
//		while(a < j && area(h[a], l) <= area(h[a+1], l)) ++a;
//		while(b < hn && area(h[b], l) <= area(h[b+1], l)) ++b;
//		
//		//pf("i:%d a:%d j:%d b:%d\n", i, a, j, b);
//		
//		ans = max(ans, area(h[a], l)+area(h[b], l));
//		
//		if (i == mid && j == hn) break;
//		
//		if (j == hn || i < mid && (h[i+1]-h[i]^h[j]-h[j+1]) <= eps) ++i;
//		else ++j;
//	}
//	ans /= 2.0;

	db ans = 0; --hn;
	_rfor(i, 1, hn){
		int a = i, b = i%hn+1;
		_rfor(j, i+1, hn){
			Line l = (Line){h[i], h[j]};
			if (i <= b && b <= j) b = j; 
			while(a < j && area(h[a], l) <= area(h[a+1], l)) ++a;
			while(b != i && area(h[b], l) <= area(h[b%hn+1], l)) b = b%hn+1;
			
			db tmp = area(h[a], l)+area(h[b], l);
			//if (tmp > ans) ans = tmp, pf("i:%d j:%d tmp:%lf\n", i, j, tmp);
			ans = max(ans, tmp);
		}
	}
	ans /= 2.0;

	pf("%.3lf\n", ans);

	return 0;
}








































