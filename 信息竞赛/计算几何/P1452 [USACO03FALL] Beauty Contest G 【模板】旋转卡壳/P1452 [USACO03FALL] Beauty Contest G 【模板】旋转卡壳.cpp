#include <stdio.h>
#include <algorithm>
#include <string.h>
#include <iostream>
#include <cmath>
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

int sgn(int x){ return x > 0 ? 1 : x < 0 ? -1 : 0; }

struct Point{
	int x, y;
	Point &operator=(const Point &t){ x = t.x; y = t.y; return *this; }
	Point operator+(const Point &t)const{ return {x+t.x, y+t.y}; }
	Point operator-(const Point &t)const{ return {x-t.x, y-t.y}; }
	Point operator-()const{return {-x, -y}; }
	int operator^(const Point &t)const{ return x*t.y-y*t.x; }
} pnt[maxn];

int dis(const Point &p){ return p.x*p.x+p.y*p.y; }

int hn;
Point hull[maxn];

bool anticlock(const Point &a, const Point &b, const Point &c){
	return sgn(b-a^c-b) >= 0;
}

bool clockwise(const Point &a, const Point &b, const Point &c){
	return sgn(b-a^c-b) <= 0;
}

bool sort_hull_cmp(const Point &p1, const Point &p2){
	return p1.x == p2.x ? p1.y < p2.y : p1.x < p2.x;
}

int mid;
void get_hull(Point *hull, int &hn, Point *pnt, int n){
	sort(pnt+1, pnt+1+n, sort_hull_cmp);
	int Y1 = min(pnt[1].y, pnt[n].y), Y2 = max(pnt[1].y, pnt[n].y);
	int k = 2; hn = 0;
	_for(o, 0, 2){
		_rfor(i, 1, n-o){
			Point &p = pnt[o?n-i:i];
			if (!o && p.y < Y1 || o && p.y > Y2) continue;
			while(hn >= k && anticlock(hull[hn-1], hull[hn], p)) --hn;
			hull[++hn] = p;
		}
		if (!o) mid = hn;
		k = hn+1;
	}
}

int main(){
	//freopen("sample.in", "r", stdin);
	
	int n = read();

	_rfor(i, 1, n) pnt[i] = (Point){read(), read()};
	
	get_hull(hull, hn, pnt, n);
	
	int i = 1, j = mid;
	int ans = dis(hull[i]-hull[j]);
	
	while(i < mid || j < hn){
		if (j == hn || i < mid && (hull[i+1]-hull[i]^hull[j]-hull[j+1]) <= 0) ++i;
		else ++j;
		ans = max(ans, dis(hull[i]-hull[j]));
	}
	
	pf("%d\n", ans);
	

	return 0;
}









































