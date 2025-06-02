#include <stdio.h>
#include <algorithm>
#include <assert.h>
#include <string.h>
#include <cmath>
using namespace std;

#define sf scanf
#define pf printf
#define ll long long
#define db double
#define nl() putchar('\n')
#define _for(i, a, b) for(int i = (a); i < (b); ++i)
#define _rfor(i, a, b) for(int i = (a); i <= (b); ++i)
#define _dfor(i, a, b) for(int i = (b); i >= (a); --i)
#define maxn 100005

int read(){ int x; sf("%d", &x); return x; }

#define eps 1e-6
#define eq(x, y) ((y)-eps <= (x) && (x) <= (y)+eps)
#define cl(x, y) ((x)+eps < (y))
#define cle(x, y) ((x) <= (y)+eps)
#define cr(x, y) cl(y, x)
#define cre(x, y) cle(y, x)
int sgn(db x){ return x > eps ? 1 : x < -eps ? -1 : 0; }
bool zr(db x){ return -eps <= x && x <= eps; }
 
struct Point{
	db x, y;
	Point &operator=(const Point &t){ x = t.x; y = t.y; return *this; }
	Point operator+(const Point &t)const{ return {x+t.x, y+t.y}; }
	Point operator-(const Point &t)const{ return {x-t.x, y-t.y}; }
	Point operator*(db d)const{ return {x*d, y*d}; }
	db operator^(const Point &t)const{ return x*t.y-y*t.x; }
};

struct Line{
	Point a, b; db theta;
	void get_theta(){ theta = atan2((b-a).y, (b-a).x); }
} lne[maxn];
int tot = 0;

db crs(const Point &p, const Line &l){ return l.b-l.a^p-l.b; }
bool clockwise(const Point &a, const Point &b, const Point &c){ return (b-a^c-b) <= eps; }
inline bool right(const Point &p, const Line &l){ return clockwise(l.a, l.b, p); } 

bool para(const Line &l1, const Line &l2){ return fabs(l1.b-l1.a^l2.b-l2.a) <= eps; }

Point inter(const Line &l1, const Line &l2){
	db ls = (l1.b-l1.a)^(l2.a-l1.a), rs = (l1.b-l1.a)^(l2.b-l1.a);
	return l2.a+(l2.b-l2.a)*(ls/(ls-rs));
}

bool lne_cmp(const Line &l1, const Line &l2){
	if (eq(l1.theta, l2.theta)) return !right(l1.a, l2);
	else return l1.theta < l2.theta;
}

bool semi_zero = false;
int semi_l, semi_r;
Line que[maxn];
Point tmp[maxn];
void semi_plane(Line *lne, int n){
	int &l = semi_l, &r = semi_r;
	_rfor(i, 1, n) lne[i].get_theta();
	sort(lne+1, lne+1+n, lne_cmp);
	
	que[r] = lne[1];
	_rfor(i, 2, n){
		if (eq(lne[i].theta, lne[i-1].theta)) continue;
		Line &ln = lne[i];
		while(r > l && right(tmp[r], ln)) --r;
		while(r > l && right(tmp[l+1], ln)) ++l;
		if (para(ln, que[r])){ semi_zero = true; return; }
		que[++r] = ln;
		tmp[r] = inter(que[r-1], ln);
	}
	while(r > l && right(tmp[r], que[l])) --r;
	tmp[l] = inter(que[r], que[l]);
}

int main(){
	//freopen("sample.in", "r", stdin);
	int n = read();
	_rfor(_, 1, n){
		int m = read();
		Point one, lst, cur;
		_rfor(i, 1, m){
			cur = (Point){read(), read()};
			if (i == 1) one = lst = cur;
			else lne[++tot] = (Line){lst, cur, 0.}, lst = cur;
		}
		lne[++tot] = (Line){lst, one, 0.};
	}
//	_rfor(i, 1, tot){
//		pf("(%.1lf, %.1lf) (%.1lf, %.1lf) %.5lf\n",
//			lne[i].a.x, lne[i].a.y, lne[i].b.x, lne[i].b.y, lne[i].theta);
////		pf("(%.1lf, %.1lf) %.5lf\n", (lne[i].b-lne[i].a).x, (lne[i].b-lne[i].a).y,
////			lne[i].theta);
//	}nl();
	
	semi_plane(lne, tot);
	if (semi_zero){ pf("0.0"); return 0; }
	int l = semi_l, r = semi_r;
	
	db ans = 0;
	Point one = tmp[l];
	_rfor(i, l+2, r) ans += fabs(tmp[i]-tmp[i-1]^tmp[i-1]-one);
	ans /= 2;
	
	pf("%.3lf\n", ans);
	
	return 0;
	
}


















