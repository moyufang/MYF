

int f[maxn][maxw];
int tmp[maxw]; 

int dfs(int u){
	////dp数组的初始条件处理 
	
	////
	int v,
		szu = dot_weight[u],
		szv; 
	
	_fev(p, u){
		v = to[p];
		szu += dfs(v);
		
		_for(i, beg1, end1){
			
			tmp[i] = func(f[u][i], f[v][0]);
			//初始化操作一般是乘法
			//tmp[i] = f[u][i] * f[v][0] 
			_for(j, beg2, end2){
				tmp[i] += func(f[u][i-j], f[v][j]);
				//可能还有其它的更新操作 
			}
		}
		
		_for(i, beg1, end1){
			f[u][i] = tmp[i];
		} 
	}
	
	return szu;
}

