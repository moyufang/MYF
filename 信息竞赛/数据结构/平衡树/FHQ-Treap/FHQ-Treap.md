```cpp
void update(re int x){
	assert(x != 0);
	/*...*/
}

int merge(int &x, int l, int r){
	if (l == 0 || r == 0){
		x = l+r;
		return;
	}
	if (pri[l] <= pri[r]){
		x = l;
		merge(rc(x), rc(l), r);
	}
	else{
		x = r;
		merge(lc(x), l, lc(r));
	}
	update(x);
}

void val_split(int x, int vl, int &ltr, int &rtr){
	if (!x){
		ltr = rtr = 0;
		return; 
	}
	if (val[x] <= vl){
		ltr = x;
		val_split(rc(x), vl, rc(ltr), rtr);
	}
	else{
		rtr = x;
		val_split(lc(x), vl, ltr, lc(rtr));
	}
	update(x);
}

void rk_split(int x, int rk, int &ltr, int &rtr){
	if (!x){
		ltr = rtr = 0;
		return;
	}
	int tmp;
	if (rk <= sz[lc(x)]){
		rtr = x;
		rk_split(lc(x), rk, ltr, lc(rtr));
	}
	else if (rk > (tmp = sz[lc(x)] + cnt[x])){
		ltr = x;
		rk_split(rc(x), rk-tmp, rc(ltr), rtr);
	}
	else{
		rk -= sz[lc(x)]; //非常重要，不能漏 
		/*
		程序处理
		常见处理：分裂该节点 
		*/ 
	} 
	update(x);
}

void del(int &x, int vl){
	assert(x > 0);
	if (vl < val[x]){
		del(lc(x), vl);
	}
	else if (vl > val[x]){
		del(rc(x), vl);
	}
	else{
		if (cnt[x] > 1) --cnt[x];
		else{
			merge(x, lc(x), rc(x));
		}
	}
	update(x);
} 
```



