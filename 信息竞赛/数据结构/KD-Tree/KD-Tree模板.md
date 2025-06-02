#### $\text{KD-Tree}$

以下均是二维 $\text{KD-Tree}$ 的实现。

##### 静态 $\text{KD-Tree}$ 树

```cpp
struct Point{ int x, y; /*...*/ } pnt[maxn];
struct Range{ int xl, xr, yl, yr; };
struct KDNode{ Range rg; /*...*/ } kdt[maxn<<2];
#define rg(x) kdt[x].rg
#define xl(x) kdt[x].rg.xl
#define xr(x) kdt[x].rg.xr
#define yl(x) kdt[x].rg.yl
#define yr(x) kdt[x].rg.yr
#define lx (x<<1)
#define rx (x<<1|1)

inline bool cmpx(const Point &p1, const Point &p2){ return p1.x < p2.x; }
inline bool cmpy(const Point &p1, const Point &p2){ return p1.y < p2.y; }

inline void ud(int x){
    rg(x) = {
    	min(xl(lx), xl(rx)), max(xr(lx), xr(rx)),
		min(yl(lx), yl(rx)), max(yr(lx), yr(rx))
    };
    /*...*/
}

//轮换选轴法
void build(int x, int tl, int tr, bool kd = false){
    if (tl == tr){ rg(x) = {pnt[tl].x, pnt[tl].x, pnt[tl].y, pnt[tl].y}; return; }
	int mi = (tl+tr)>>1;
    nth_element(pnt+tl, pnt+mi, pnt+tr+1, kd?cmpy:cmpx);
    build(lx, tl, mi, !kd);
    build(rx, mi+1, tr, !kd);
    ud(x);
}

//方差选轴法
void build(int x, int tl, int tr){
    if (tl == tr){ rg(x) = {pnt[tl].x, pnt[tl].x, pnt[tl].y, pnt[tl].y}; return; }
	int mi = (tl+tr)>>1;
    db ax = 0.0, ay = 0.0, vx = 0.0, vy = 0.0;
	_rfor(i, tl, tr) ax += pnt[i].x, ay += pnt[i].y; ax /= tr-tl+1; ay /= tr-tl+1;
	_rfor(i, tl, tr) vx += (pnt[i].x-ax)*(pnt[i].x-ax), vy += (pnt[i].y-ay)*(pnt[i].y-ay);
    nth_element(pnt+tl, pnt+mi, pnt+tr+1, vy>vx?cmpy:cmpx);
    build(lx, tl, mi);
    build(rx, mi+1, tr);
    ud(x);
}

//对矩阵的查询
Range q; int qas; /*...*/
void qry(int x, int tl, int tr){
    if (xr(x)<q.xl || xl(x)>q.xr || yr(x)<q.yl || yl(x)>q.yr) return;
    if (xl(x)>=q.xl && xr(x)<=q.xr && yl(x)>=q.yl && yr(x)<=q.yr){ /*...*/ return; }
    int mi = (tl+tr)>>1;
    qry(lx, tl, mi);
    qry(rx, mi+1, tr);
}
```

##### 动态 $\text{KD-Tree}$

```cpp
#include <vector>

int pcnt, pool[maxnn];
struct Point{ int x, y; /*...*/ } pnt[maxn];
struct Range{ int xl, xr, yl, yr; };
struct KDNode{ int lc, rc; Range rg; /*...*/ } kdtnode[maxnn] = {{0, 0, {0, 0, 0}}};

void init_pool(int n){ pcnt = 0; _rfor(i, 1, n) pool[++pcnt] = n-i+1; }
int new_node(){ int x = pool[pcnt--]; kdtnode[x] = kdtnode[0]; return x; }
int del(int x){ pool[++pcnt] = x; }

inline bool cmpx(const Point &p1, const Point &p2){ return p1.x < p2.x; }
inline bool cmpy(const Point &p1, const Point &p2){ return p1.y < p2.y; }
inline bool inner(const Point &p, const Range &rg){
    return p.x>=rg.xl && p.x<=rg.xr && p.y>=rg.yl && p.y<=rg.yr;
}

struct KDT{
    #define rg(x) kdtnode[x].rg
    #define xl(x) kdtnode[x].rg.xl
    #define xr(x) kdtnode[x].rg.xr
    #define yl(x) kdtnode[x].rg.yl
    #define yr(x) kdtnode[x].rg.yr
    #define lc(x) kdtnode[x].lc
    #define rc(x) kdtnode[x].rc
    int rt;
	vector<Point> *pnt_;
    #define pnt (*pnt_)
    inline void ud(int x){
        rg(x) = {
            min(xl(lx), xl(rx)), max(xr(lx), xr(rx)),
            min(yl(lx), yl(rx)), max(yr(lx), yr(rx))
        };
        /*...*/
    }
    void build(int &x, int tl, int tr){
        if (!x) x = new_node();
        if (tl == tr){ rg(x) = {pnt[tl].x, pnt[tl].x, pnt[tl].y, pnt[tl].y}; return; }
        int mi = (tl+tr)>>1;
        db ax = 0.0, ay = 0.0, vx = 0.0, vy = 0.0;
        _rfor(i, tl, tr) ax += pnt[i].x, ay += pnt[i].y; ax /= tr-tl+1; ay /= tr-tl+1;
        _rfor(i, tl, tr) 
            vx += (pnt[i].x-ax)*(pnt[i].x-ax), vy += (pnt[i].y-ay)*(pnt[i].y-ay);
        nth_element(pnt.begin()+tl, pnt.begin()+mi, pnt.begin()+tr+1, vy>vx?cmpy:cmpx);
        build(lc(x), tl, mi);
        build(rc(x), mi+1, tr);
        ud(x);
    }
    Range q; int qas; /*...*/
    void qry(int x, int tl, int tr){
        if (xr(x)<q.xl || xl(x)>q.xr || yr(x)<q.yl || yl(x)>q.yr) return;
        if (xl(x)>=q.xl && xr(x)<=q.xr && yl(x)>=q.yl && yr(x)<=q.yr){ /*...*/ return; }
        int mi = (tl+tr)>>1;
        qry(lc(x), tl, mi);
        qry(rc(x), mi+1, tr);
    }
    void clr(int x){
        if (!x) return;
        clr(lc(x));
        clr(rc(x));
        del(x);
    }
    void clear(){
        clr(rt);
        rt = 0;
    }
    #undef rg
    #undef xl
    #undef xr
    #undef yl
    #undef yr
    #undef lc
    #undef rc
};

struct KDTree{
    KDT T0, T1; int p0, p1, p2;
    vector<Point> pnt;
    init(){ pnt.clear(); pnt.push_back(Point()); p0 = p1 = p2 = 0; T0.pnt = T1.pnt = &pnt; }
    void ins(const Point &point){
        pnt.push_back(point); ++p2;
        if ((p2-p1)*(p2-p1) > p2*1.5){
            T0.clear();
            T0.build(T0.rt, p0+1, p1=p2);
        }
        else if (p1-p0 > pow(p2, 0.75)*1.2){
            T0.clear(); T1.clear();
            T1.build(T1.rt, 1, p0=p2-1);
            T0.build(T0.rt, p2, p1=p2);
        }
    }
    //对矩阵的查询
    int qry(const Range &rg){
        int ans;
        T0.q = T1.q = rg; T0.qas = T1.qas = 0;
        if (p1 > p0) T0.qry(1, p0+1, p1);
        if (p0 > 1) T1.qry(1, 1, p0);
        _rfor(i, p1+1, p2){
            if (inner(pnt[i], rg)){
                /*...*/
            }
        }
        return ans;
    }
};

```

























