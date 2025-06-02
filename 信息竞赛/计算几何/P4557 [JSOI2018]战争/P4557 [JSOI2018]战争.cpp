#include <stdio.h>
#include <algorithm>
#include <string.h>
#include <iostream>
#include <assert.h>
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
#define cl(x, y) ((x) + eps < (y))
#define cle(x, y) ((x) <= (y) + eps)
#define cr(x, y) cl(y, x)
#define cre(x, y) cle(y, x)
#define zr(x) (-eps <= (x) && (x) <= eps)
#define eq(x, y) ((y)-eps <= (x) && (x) <= (y)+eps)

int sgn(db x){ return x > eps ? 1 : x < -eps ? -1 : 0; }

struct Point{
	db x, y; int id;
	Point(db x = 0, db y = 0, int id = 0):x(x), y(y), id(id){}
	Point &operator=(const Point &t){ x = t.x; y = t.y; id = t.id; return *this; }
	Point operator+(const Point &t)const{ return {x+t.x, y+t.y}; }
	Point operator-(const Point &t)const{ return {x-t.x, y-t.y}; }
	Point operator-()const{return {-x, -y}; }
	db operator^(const Point &t)const{ return x*t.y-y*t.x; }
} pnt[3][maxn], Q[maxn];
bool yes[maxn], ans[maxn];

int hn[3];
Point hull[3][maxn];

bool anticlock(const Point &a, const Point &b, const Point &c){
	return sgn(b-a^c-b) >= 0;
}

bool clockwise(const Point &a, const Point &b, const Point &c){
	return sgn(b-a^c-b) <= 0;
}

bool sort_hull_cmp(const Point &p1, const Point &p2){
	return eq(p1.x, p2.x) ? cl(p1.y, p2.y) : cl(p1.x, p2.x);
}

int mid;
void get_hull(Point *hull, int &hn, Point *pnt, int n){
	sort(pnt+1, pnt+1+n, sort_hull_cmp);
	db Y1 = min(pnt[1].y, pnt[n].y), Y2 = max(pnt[1].y, pnt[n].y);
	int k = 2; hn = 0;
	_for(o, 0, 2){
		_rfor(i, 1, n-o){
			Point &p = pnt[o?n-i:i];
			if (!o && cl(p.y, Y1) || o && cr(p.y, Y2)) continue;
			while(hn >= k && anticlock(hull[hn-1], hull[hn], p)) --hn;
			hull[++hn] = p;
		}
		if (!o) mid = hn;
		k = hn+1;
	}
}

void get_sum_hull(Point h[3][maxn], int hn[3]){
    static Point hull[maxn];
	int i[2] = {2, 2}, t = 0;
	hull[++t] = h[0][1] + h[1][1];
	while(i[0] <= hn[0] || i[1] <= hn[1]){
		int o = i[0] > hn[0] || i[1] <= hn[1] &&
			sgn(h[0][i[0]]-h[0][i[0]-1]^h[1][i[1]]-h[1][i[1]-1]) > 0;
		hull[t+1] = h[o][i[o]]-h[o][i[o]-1]+hull[t];
		++t; ++i[o];
	}
    get_hull(h[2], hn[2], hull, t);
}

int main(){
	//freopen("sample.in", "r", stdin);
	
	int n[3] = {read(), read(), 0},
		q = read();
		
	_for(o, 0, 2) _rfor(i, 1, n[o]) pnt[o][i] = (Point){read(), read()};
	_rfor(i, 1, n[1]) pnt[1][i] = -pnt[1][i];
	
	_for(o, 0, 2) get_hull(hull[o], hn[o], pnt[o], n[o]);
	
	//_rfor(i, 1, hn[0]) pf("hx:%.1lf hy:%.1lf\n", hull[0][i].x, hull[0][i].y); nl();
	//_rfor(i, 1, hn[1]) pf("hx:%.1lf hy:%.1lf\n", hull[1][i].x, hull[1][i].y); nl();
	
	get_sum_hull(hull, hn);
	
	_rfor(i, 1, q) Q[i] = (Point){read(), read(), i};
	sort(Q+1, Q+1+q, sort_hull_cmp);
	
	
	Point *pn = hull[2]; int nn = hn[2], cur = 1;
	
	//_rfor(i, 1, nn) pf("px:%.1lf py:%.1lf\n", pn[i].x, pn[i].y);
	//_rfor(i, 1, q) pf("x:%.1lf y:%.1lf id:%d\n", Q[i].x, Q[i].y, Q[i].id);

	while(cur <= q && cl(Q[cur].x, pn[1].x)) yes[cur++] |= true;
	_rfor(i, 2, mid){
		while(cur <= q && cle(Q[cur].x, pn[i].x)){
			yes[cur] |= !clockwise(pn[i-1], pn[i], Q[cur]);
			++cur;
		}
	}
	cur = q;
	while(cur >= 1 && cr(Q[cur].x, pn[mid].x)) yes[cur--] |= true;
	_rfor(i, mid+1, nn){
		while(cur >= 1 && cre(Q[cur].x, pn[i].x)){
			yes[cur] |= !clockwise(pn[i-1], pn[i], Q[cur]);
			--cur;
		}
	}
	
	//print("yes", yes, q);
	
	_rfor(i, 1, q) ans[Q[i].id] = yes[i]^1;
	
	_rfor(i, 1, q) pf("%d\n", ans[i]);

	return 0;
}









































