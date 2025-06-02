
int ncnt = 2, curs, lst, 
	str[maxn],
	nxt[maxn][maxc], fail[maxn], len[maxn];
	
void init(){
	ncnt = 2;
	fail[1] = 0; len[1] = -1;
	fail[2] = 1; len[2] = 0;
}

int up(int p){
	while(str[curs-1-len[p]] != str[curs]) p = fail[p];
	return p;
}

void ins(int c){
	str[++curs] = c;
	int p = lst;
	p = up(p);
	int &q = nxt[p][c];
	if (!q){
		q = ++ncnt;
		len[q] = len[p]+2;
		fail[q] = p == 1 ? 2 : nxt[up(fail[p])][c];
	}
	lst = q;
}

void ins(char *str){
	int u = 1, c;
	while(c = *str++){
		if (!nxt[u][c -= 'a']) nxt[u][c] = ++ncnt;
		u = nxt[u][c];
	}
	tag[u] = true;
}

void build(){
	static int que[maxn];
	int u, v, fro = 0, bac = 0;
	_for(i, 0, maxc)
		if (v = nxt[1][i]) fail[que[bac++] = v] = 1;
		else nxt[1][i] = 1;
	while(bac > fro){
		u = que[fro++];
		_for(i, 0, maxc)
			if (v = nxt[u][i]) fail[que[bac++] = v] = nxt[fail[u]][i];
			else nxt[u][i] = nxt[fail[u]][i];
	}
}

int main(){
	
	return 0;
}

