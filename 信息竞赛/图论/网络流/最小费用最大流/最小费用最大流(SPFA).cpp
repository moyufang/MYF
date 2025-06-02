#include <queue>
#include <iostream>
using namespace std;
struct Edge{
	typedef int size_t;
	typedef int flow_t;
	typedef int cost_t;
	Edge():to(0), nex(0), cap(0), flow(0), cost(0){}
	size_t to, nex;
	flow_t cap, flow;
	cost_t cost;
};

struct Graph{
	typedef Edge::size_t size_t;
	typedef Edge::flow_t flow_t;
	typedef Edge::cost_t cost_t;
	
	Graph(size_t n):
	n(n),
	head(new size_t[n+1]()),
	edges(new Edge[2*n*n + 5]()),
	ecnt(1),
	pre(new size_t[n+1]()),
	vis(new bool[n+1]()),
	cnt(new size_t[n+1]()),
	dis(new cost_t[n+1]()),
	max_flow(0),
	min_cost(0){}
	
	~Graph(){
		delete[] head;
		delete[] edges;
		delete[] pre;
		delete[] vis;
		delete[] cnt;
		delete[] dis;
	}
	
	void add_single_edge(size_t u, size_t v, flow_t cap, cost_t cost){
		edges[++ecnt].to = v;
		edges[ecnt].cap = cap;
		edges[ecnt].cost = cost;
		edges[ecnt].nex = head[u];
		head[u] = ecnt;
	}
	
	void add_edge(size_t u, size_t v, flow_t cap, cost_t cost){
		add_single_edge(u, v, cap, cost);
		add_single_edge(v, u, 0, -cost);
	}
	
	bool spfa(size_t s, size_t t){
		for(size_t i = 1; i <= n; ++i){
			pre[i] = 0;
			vis[i] = false;
			cnt[i] = 0;
			dis[i] = cost_t_inf;
		}
		
		size_t u, v, p;
		queue<size_t> que;
		dis[s] = 0;
		vis[s] = true;
		++cnt[s];
		que.push(s);
		while(!que.empty()){
			u = que.front(); que.pop();
			vis[u] = false;
			for(p = head[u]; p; p = edges[p].nex){
				v = edges[p].to;
				if (edges[p].flow<edges[p].cap && dis[u]+edges[p].cost<dis[v]){
					dis[v] = dis[u]	+ edges[p].cost;
					pre[v] = p;
					if (!vis[v]){
						vis[v] = true;
						++cnt[v];
						if (cnt[v] > n)
							return false;
						que.push(v);
					}
				}
			}
		}
		if (dis[t] == cost_t_inf)
			return false;
		return true;
	}
	
	void mcmf(size_t s, size_t t){
		max_flow = 0;
		min_cost = 0;
		size_t u, v, p;
		flow_t min_flow;
		while(spfa(s, t)){
			//print_dis_pre();
			min_flow = flow_t_inf;
			u = t;
			do{
				p = pre[u];
				if (edges[p].cap-edges[p].flow < min_flow)
					min_flow = edges[p].cap-edges[p].flow;
				u = edges[p^1].to;
			}while(u != s);
			
			u = t;
			do{
				p = pre[u];
				edges[p].flow += min_flow;
				edges[p^1].flow -= min_flow;
				u = edges[p^1].to;
			}while(u != s);
			max_flow += min_flow;
			min_cost += min_flow * dis[t];
		}		
	}
	
	void refresh(){
		for(size_t u = 1; u <= n; ++u){
			for(size_t p = head[u]; p; p = edges[p].nex){
				edges[p].flow = 0;
			}
		}
		max_flow = 0;
		min_cost = 0;
	}
	
	void print_dis_pre(){
		for(size_t u = 1; u <= n; ++u){
			cout << dis[u]<<" ";
		}
		cout <<endl;
		for(size_t u = 1; u <= n; ++u){
			cout << edges[pre[u]^1].to<<" ";
		}
		cout <<endl;
		cout<<endl;
	}
	
	void print_flow_graph(){
		for(size_t u = 1; u <= n; ++u){
			cout << "v"<<u<<":";
			for(size_t p = head[u]; p; p = edges[p].nex){
				if (edges[p].cap>0 && edges[p].flow>0){
					cout<<" "<<edges[p].to<<"|"<<edges[p].flow;
				}
			}
			cout << endl;
		}
		cout << endl;
	}
	
	void print_graph(){
		for(size_t u = 1; u <= n; ++u){
			cout<<"v"<<u<<":";
			for(size_t p = head[u]; p; p = edges[p].nex){
				cout<<" "<<edges[p].to<<"|"<<edges[p].flow<<"|"<<edges[p].cap<<"|"<<edges[p].cost;
			}
			cout <<endl;
		}
		cout << endl;
	}
	
	size_t n;
	size_t *head;
	Edge *edges;
	size_t ecnt;
	size_t *pre;
	bool *vis;
	size_t *cnt;
	cost_t *dis;
	flow_t max_flow;
	cost_t min_cost;
	static flow_t flow_t_inf;
	static cost_t cost_t_inf;
};

Graph::flow_t Graph::flow_t_inf = 0x7fffffff;
Graph::cost_t Graph::cost_t_inf = 0x7fffffff;

void test(){
	int n, m, s, t, u, v, cap, cost;
	cin>>n>>m>>s>>t;
	
	Graph graph(n);
	
	for(int i = 0; i != m; ++i){
		cin>>u>>v>>cap>>cost;
		graph.add_edge(u, v, cap, cost);
	}
	graph.print_graph();
	graph.mcmf(s, t);
	cout<<graph.max_flow<<" "<<graph.min_cost<<endl;
	graph.print_flow_graph();
	
	graph.refresh();
	graph.print_graph();
	
	graph.mcmf(s, t);
	cout<<graph.max_flow<<" "<<graph.min_cost<<endl;
	graph.print_flow_graph();
}

int main(){
	freopen("in.txt", "r", stdin);
	test();	
	return 0;
}









