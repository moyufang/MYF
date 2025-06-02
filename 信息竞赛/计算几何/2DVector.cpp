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

const db eps = 1e-10, Pi = 3.14159265358979323846264;
int sgn(db x){ return x > eps ? 1 : x < -eps ? -1 : 0; }
bool zr(db x){ return -eps <= x && x <= eps; }
#define cl(x, y) ((x)+eps<(y))
#define cle(x, y) ((x)<=(y)+eps)
#define cr(x, y) cl(y, x)
#define cre(x, y) cle(y, x)
struct Point{
	db x, y;
	Point operator*(db d)const{ return {x*d, y*d}; }
	Point operator/(db d)const { return {x/d, y/d}; }
	Point operator+(const Point &t)const{ return {x+t.x, y+t.y}; }
	Point operator-(const Point &t)const{ return {x-t.x, y-t.y}; }
	Point operator*(const Point &t)const{ return {x*t.x, y*t.y}; }
	Point operator/(const Point &t)const{ return {x/t.x, y/t.y}; }
	db operator|(const Point &t)const{ return x*t.x+y*t.y; }
	db operator^(const Point &t)const{ return x*t.y-y*t.x; }
	bool operator==(const Point &t)const{ return (*this-t).dis() <= eps; }
	db dis()const{ return sqrt(x*x+y*y); }
};
db dis(const Point &a){ return sqrt(a.x*a.x+a.y*a.y); }
db dis(const Point &a, const Point &b){ return sqrt((a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y)); }
db cos(const Point &a, const Point &b){ return (a|b)/a.dis()/b.dis(); }
Point rotate(const Point &a, db t){ db c = cos(t), s = sin(t); return {a.x*c-a.y*s, a.x*s+a.y*c}; }
Point norm(const Point &a){ return a/a.dis(); } //a 不能为原点 

struct Line{
	Point a, b;
	inline db mix()const{ return min(a.x, b.x); }
	inline db miy()const{ return min(a.y, b.y); }
	inline db mxx()const{ return max(a.x, b.x); }
	inline db mxy()const{ return max(a.y, b.y); }
}; //一般要保证 a != b 
db len(const Line &l){ return dis(l.a, l.b); }
bool onl0(const Point &p, const Line &l){ return zr((p-l.a)^(l.b-p)); }
bool onl2(const Point &p, const Line &l){
	return onl0(p, l) &&
		cle(l.mix(), p.x) && cle(p.x, l.mxx()) &&
		cle(l.miy(), p.y) && cle(p.y, l.mxy());
}
db dis0(const Point &p, const Line &l){ return fabs(((l.a-p)^(l.b-p))/dis(l.a-l.b)); }
db dis2(const Point &p, const Line &l){
	if (sgn(p-l.a | l.b-l.a) <= 0) return dis(p, l.a);
	else if (sgn(p-l.b | l.a-l.b) <= 0) return dis(p, l.b);
	else return dis0(p, l);
}
bool para(const Line &l1, const Line &l2){ return zr(l1.b-l1.a ^ l2.b-l2.a); }
Point inter(const Line &l1, const Line &l2){
	db ls = l1.b-l1.a ^ l2.a-l1.a, rs = l1.b-l1.a ^ l2.b-l1.a;
	return l2.a+(l2.b-l2.a)*(ls/(ls-rs));
}
bool is_inter(const Line &l1, const Line &l2){
	if (
		cl(l1.mxx(), l2.mix()) || cl(l1.mxy(), l2.miy()) ||
		cl(l2.mxx(), l1.mix()) || cl(l2.mxy(), l1.miy())) return false;
	else return
		sgn(l1.b-l1.a^l2.a-l1.a)*sgn(l1.b-l1.a^l2.b-l1.a) <= 0 &&
		sgn(l2.b-l2.a^l1.a-l2.a)*sgn(l2.b-l2.a^l1.b-l2.a) <= 0;
}
void print(const Point &a){
	pf("%.3lf %.3lf\n", a.x, a.y);
}

void test1(){
	Point a=(Point){1,2},b=(Point){4,5};
	printf("%.3lf ",dis(a,b));
	a=rotate(a,Pi/4);b=rotate(b,Pi/2);
	printf("%.3lf\n",dis(a,b));
	//4.243 4.686
	printf("%.3lf\n", cos(b, rotate(b, Pi/4)));
	//0.707
	puts("");
}
void test2(){
	Point a=(Point){1,2},b=(Point){4,5},c=(Point){3,3},d=(Point){1.5,2.5};
	Line l=(Line){(Point){0,1},(Point){3,4}};
	printf("%d %d %d %d %d\n",onl2(a,l),onl2(b,l),onl2(c,l),onl0(b,l),onl2(d,l));
	//1 0 0 1 1
	puts("");
}
void test3(){
	Line l1=(Line){(Point){0,1},(Point){3,4}};
	Line l2=(Line){(Point){-2,1},(Point){-1,3}};
	print(inter(l1,l2));
	//(-4.000,-3.000)
	l1.b=l1.b*2;
	print(inter(l1,l2));
	//(-4.800,-4.600)
	puts("");
}
void test4(){
	Point a=(Point){0,2},b={-2,2},c={4,5};
	Line l=(Line){(Point){0,1},(Point){3,4}};
	printf("%.3lf %.3lf %.3lf\n",dis0(a,l),dis0(b,l),dis0(c,l));
	//0.707 2.121 0.000
	printf("%.3lf %.3lf %.3lf\n",dis2(a,l),dis2(b,l),dis2(c,l));
	//0.707 2.236 1.414
	puts("");
}

void test5(){
	Line
		l1 = {{0, 0}, {1, 1}},
		l2 = {{0, 1}, {1, 0}},
		l3 = {{-0.1, -0.1}, {0.9, 0.9}},
		l4 = {{1.1, 1.1}, {1.2, 1.2}},
		l5 = {{0, 1}, {1, 2}},
		l6 = {{-1, 2}, {0, 1}},
		l7 = {{-1, 2}, {0.5, 0.5}},
		l8 = {{1, 1}, {0, 2}};
	pf("%d %d %d %d %d %d %d\n",
		is_inter(l1, l2),
		is_inter(l1, l3),
		is_inter(l3, l4),
		is_inter(l1, l5),
		is_inter(l1, l6),
		is_inter(l1, l7),
		is_inter(l1, l8));
	//1 1 0 0 0 1 1
	puts("");
}

int main(){
	#ifndef ONLINE_JUDGE
	freopen("sample.in", "r", stdin);
	//freopen("sample.out", "w", stdout);
	#endif
	
	test1();
	test2();
	test3();
	test4();
	test5();


	return 0;
}





































