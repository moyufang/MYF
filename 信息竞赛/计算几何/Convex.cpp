#include <stdio.h>
#include <algorithm>
#include <string.h>
#include <iostream>
#include <assert.h>
#include <cmath>
#include <vector>
#include <cstdlib>
#include <cstdarg>
#include <cstring>
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
#define _dfor(i, b, a) for(re int i = (b); i >= (a); --i)
#define _fev(p, u) for(re int p = head[u]; p; p = nex[p])
#define inf 0x7fffffff
#define maxn 100005
#define mod 998244353ll

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

inline void uad(int &x, const int &y){ if ((x+=y)>=mod) x-=mod; }
template<class T>
inline void umx(T &x, const T &y){ if (y > x) x = y; }
template<class T>
inline void umi(T &x, const T &y){ if (y < x) x = y; }

struct Vec {
	db x, y;
	Vec(db x = 0.0, db y = 0.0) :x(x), y(y) {}
	Vec(const Vec &t):x(t.x), y(t.y){}
	Vec& operator=(const Vec &t){ x = t.x; y = t.y; return *this; }
	Vec operator+(const Vec& t)const { return Vec(x + t.x, y + t.y); }
	Vec operator-(const Vec& t)const { return Vec(x - t.x, y - t.y); }
	Vec operator*(db d)const { return Vec(x * d, y * d); }
	Vec operator/(db d)const { return Vec(x / d, y / d); }
	Vec& operator+=(const Vec& t) { x += t.x; y += t.y; return *this; }
	Vec& operator-=(const Vec& t) { x -= t.x; y -= t.y; return *this; }
	Vec& operator*=(db d) { x *= d; y *= d; return *this; }
	Vec& operator/=(db d) { x /= d; y /= d; return *this; }
	Vec operator-() { return Vec(-x, -y); }
	//dot product
	db operator|(const Vec& t)const { return x * t.x + y * t.y; }
	//cross product
	db operator^(const Vec& t)const { return x * t.y - y * t.x; }

	Vec unit() const { return *this / sqrt(x * x + y * y); }
	Vec norm() const { return Vec(-y, x); }
	db angle() const {
		if (x >= 0.0) return asin(unit().y);
		else if (y >= 0.0) return acos(unit().x);
		else return -acos(unit().x);
	}

	static Vec O;
};

void print(const Vec &v){
	pf("(%.2lf, %.2lf)", v.x, v.y);
}

typedef Vec Pos;

db sqr(const Vec& v){ return v.x*v.x+v.y*v.y; } 
db len(const Vec& v){ return sqrt(v.x*v.x+v.y*v.y); }

struct Poly {
	std::vector<Pos> p;
	Poly() { }
	Poly(const std::vector<Pos> vec):p(vec){}
	Poly(std::initializer_list<Pos> il) { for (Pos pos : il) p.push_back(pos); }
	Poly(const Poly& t):p(t.p) { }
	Poly& operator=(const Poly& t) { p = t.p; return *this; }
//	Poly(int n, ...) {
//		va_list va_ptr;
//		va_start(va_ptr, n);
//		_for(i, 0, n) p.push_back(va_arg(va_ptr, Vec));
//		va_end(va_ptr);
//	}
	bool is_convex_hull() {
		if (p.size() <= 3) return true;
		Vec v[3]; v[0] = p[0]; v[1] = p[1]; v[2] = p[2];
		int t = 2;
		for (; t < p.size()-1 && ((v[1] - v[0]) ^ (v[2] - v[1])) == 0.0 ; )
			++t, v[t % 3] = p[t];
		if (((v[1] - v[0]) ^ (v[2] - v[1])) > 0.0) {
			for (++t; t < p.size(); ++t) {
				v[t % 3] = p[t];
				if (((v[1] - v[0]) ^ (v[2] - v[1])) < 0.0) return false;
			}
		}
		else {
			for (++t; t < p.size(); ++t) {
				v[t % 3] = p[t];
				if (((v[1] - v[0]) ^ (v[2] - v[1])) > 0.0) return false;
			}
		}
		return true;
	}
	static bool convex_hull_cmp(const Vec& v1, const Vec& v2) {
		return v1.x == v2.x ? v1.y < v2.y : v1.x < v2.x;
	}
	void convex_hull() {
		std::sort(p.begin(), p.end(), Poly::convex_hull_cmp);
		if (p.size() <= 2) return;
		std::vector<Vec> tmp1, tmp2;
		tmp1.push_back(p[0]); tmp1.push_back(p[1]);
		tmp2.push_back(p[0]); tmp2.push_back(p[1]);
		int s = 2, t = 2, n = p.size();
		_for(i, 1, n) assert(p[i].x >= p[i-1].x);
		db Y1 = min(p[0].y, p[n-1].y), Y2 = max(p[0].y, p[n-1].y);
		_for(i, 2, n) {
			if (p[i].y < Y1) continue;
			while (s > 1 && ((tmp1[s - 1] - tmp1[s - 2]) ^ (p[i] - tmp1[s - 1])) >= 0.0)
				--s, tmp1.pop_back();
			++s; tmp1.push_back(p[i]);
		}
		
		_for(i, 2, n) {
			if (p[i].y > Y2) continue;
			while (t > 1 && ((tmp2[t - 1] - tmp2[t - 2]) ^ (p[i] - tmp2[t - 1])) <= 0.0)
				--t, tmp2.pop_back();
			++t; tmp2.push_back(p[i]);
		}
		tmp2.pop_back(); --t;
		p.resize(s + t - 1); assert(p.size() == s+t-1);
		_for(i, 0, s) p[i] = tmp1[i];
		_for(i, 1, t) p[s + i - 1] = tmp2[t-i];
	    n = s + t - 1;
	    _for(i, 0, n - 1) assert(p[i].x != p[i+1].x || p[i].y != p[i+1].y);
	    
	}
} pl;

int main(){
	#ifndef ONLINE_JUDGE
	freopen("sample.in", "r", stdin);
	//freopen("sample.out", "w", stdout);
	#endif

	int n; sf("%d", &n);
	_for(i, 0, n){
		db x, y;
		sf("%lf%lf", &x, &y);
		pl.p.push_back({x, y});
	}
	
	//assert(pl.is_convex_hull()==false);
	pl.convex_hull();
	assert(pl.is_convex_hull());
	
	
	int t = pl.p.size();
	//pf("t:%d\n", t);
	db ans = 0.0;
	_for(i, 0, t-1) ans += len(pl.p[i] - pl.p[i+1]);
	ans += len(pl.p[t-1] - pl.p[0]);
	
	pf("%.2lf\n", ans);

	return 0;
}




































