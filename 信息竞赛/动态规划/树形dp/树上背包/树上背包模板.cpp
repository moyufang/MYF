

int tmp[maxw];

int dfs(int u){
	////dp数组的初始条件处理 
	 
	////
	int v
		szu = 1, //u子树的大小
		//szu = dot_weight[u] //子树大小可以不是子树节点树，也可以是子树节点权值和 
		szv, //v子树的大小
		;
	_fev(p, u){
		v = to[p];
		szu += dfs(v);
		//用子节点 v 更新 f[u][i] 
		_for(i, beg1, end1){//背包体积的范围 
			//tmp[i] 数组用来保存更新过后的 f[u][i] 
			tmp[i] = init; //初始值
			//可能还有其它的初始操作
			
			_for(j, beg1, end2){
				tmp[i] = better(tmp[i], func(f[u][i-j], f[v][j])) //更新 
				//可能还有其它的更新操作				
			}
		}

		_for(i, beg1, end1){
			f[u][i] = tmp[i]; //最后的更新 
		} 
	}
	
	return szu; //返回子树大小 
}

//注意：有的题目可以将 tmp 数组省略，直接在 f[u] 数组上做更新操作
//但 tmp 数组更保险 
 
 
