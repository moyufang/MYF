int nx, ny, tarv,
	fa[2*maxn],
	tar[maxn],
	match[maxn];
ll	ans,
	lx[maxn],
	ly[maxn],
	we[maxn][maxn],
	slack[maxn];
bool
	visx[maxn],
	visy[maxn];

bool dfs(int u){
	visx[u] = true;
	ll dlt;
	_rfor(v, 1, ny){
		if (we[u][v] == ninf || visy[v]) continue;
		dlt = lx[u] + ly[v] - we[u][v];
		//pf("u:%d dlt:%lld\n", u, dlt);
		if (dlt > 0){
			if (dlt < slack[v])
				slack[v] = dlt, tar[v] = u;
		}
		else{
			visy[v] = true;
			if (!match[v] || dfs(match[v])){
				match[v] = u;
				//pf("match[%d]=%d\n", v, u);
				return true;
			}
			else{
				fa[match[v]] = v;
				fa[v+nx] = u;
			}
		}
	}
	return false;
}

bool update(){
	re ll dlt = inf; tarv = 0;
	_rfor(v, 1, ny){
		//pf("visy[v]:%d slack[v]:%lld\n", visy[v], slack[v]);
		if (!visy[v] && slack[v] < dlt)
			dlt = slack[v], tarv = v; 
	}
	if (dlt == inf) return false;
	_rfor(v, 1, ny){
		if (visy[v]) ly[v] += dlt;
		else if (slack[v] < inf) slack[v] -= dlt;
	}
	_rfor(u, 1, nx)
		if (visx[u]) lx[u] -= dlt;
	return true;
}

bool KM(){
	re int x, y;
	ms(match, 0);
	_rfor(i, 1, nx) lx[i] = ninf;
	_rfor(i, 1, ny) ly[i] = 0;
	_rfor(i, 1, nx){
		_rfor(j, 1, ny)
			if (lx[i] < we[i][j]) lx[i] = we[i][j];
		if (lx[i] == ninf){
			//pf("lx\n");
			return false;
		}
	}
	_rfor(u, 1, nx){
		//pf("u:%d\n", u);
		ms(visx, false); ms(visy, false); ms(fa, 0);
		_rfor(v, 1, ny) slack[v] = inf;
		bool fir = true;
		while(true){
			if (fir){
				fir = false;
				if (dfs(u)){
					//pf("u:%d in break\n", u);
					break;
				}
				else if (!update()){
					//pf("update1\n");
					return false;
				}
			}
			else{
				x = tar[y = tarv]; visy[y] = true;
				if (!match[y] || dfs(match[y])){
					do{
						match[y] = x;
						//pf("match[%d]=%d\n", y, x);
					}while((y = fa[x]) && (x = fa[y+nx]));
					break;
				}
				else{
					fa[y+nx] = x; fa[match[y]] = y;
					if (!update()){
						//pf("update2\n");
						return false;
					}
				}
			}
		}
	}
	ans = 0;
//	print("lx", lx, nx);
//	print("ly", ly, ny);
	_rfor(i, 1, nx) ans += lx[i];
	_rfor(i, 1, ny) ans += ly[i];
	return true;
}
