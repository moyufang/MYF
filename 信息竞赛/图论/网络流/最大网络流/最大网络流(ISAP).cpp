#include <queue>
#include <iostream>
using namespace std;
struct Edge{
	typedef int size_t;
	typedef int flow_t;
	Edge():to(0), nex(0), cap(0), flow(0){}
	size_t to, nex;
	flow_t cap, flow;
};

struct Graph{
	typedef Edge::size_t size_t;
	typedef Edge::flow_t flow_t;
	
	Graph(size_t n):
	n(n),
	head(new size_t[n+1]()),
	edges(new Edge[2*n*n + 5]()),
	ecnt(1),
	pre(new size_t[n+1]()),
	h(new size_t[n+1]()),
	g(new size_t[n+1]()),
	max_flow(0){}
	
	~Graph(){
		delete[] head;
		delete[] edges;
		delete[] pre;
		delete[] h;
		delete[] g;
	}
	
	void add_single_edge(size_t u, size_t v, flow_t cap){
		edges[++ecnt].to = v;
		edges[ecnt].cap = cap;
		edges[ecnt].nex = head[u];
		head[u] = ecnt;
	}
	
	void add_edge(size_t u, size_t v, flow_t cap){
		add_single_edge(u, v, cap);
		add_single_edge(v, u, 0);
	}
	
	void set_h(size_t t){
		for(size_t i = 1; i <= n; ++i){
			h[i] = n;
			g[i] = 0;
		}
		
		size_t u, v;
		h[t] = 0;
		std::queue<size_t> que;
		que.push(t);
		while(!que.empty()){
			u = que.front(); que.pop();
			for(size_t p = head[u]; p; p = edges[p].nex){
				v = edges[p].to;
				if (h[v] == n){
					h[v] = h[u] + 1;
					++g[h[v]];
					que.push(v);
				}
			}
		}
		
	}
	
	void isap(size_t s, size_t t){
		max_flow = 0;
		set_h(t);
		//print_gh();
		
		size_t u = s, v, min_h, p;
		flow_t min_flow = flow_t_inf;
		while(h[s] < n){
			//cout << u << endl;
			//print_graph();
			//print_gh();
			//cout<<endl;
			for(p = head[u]; p; p = edges[p].nex){
				v = edges[p].to;
				if (edges[p].flow < edges[p].cap && h[u] == h[v] + 1){
					if (edges[p].cap - edges[p].flow < min_flow)
						min_flow = edges[p].cap - edges[p].flow;
					pre[v] = p;
					u = v;
					break;
				}
			}
			if (u == t){
				do{
					p = pre[u];
					edges[p].flow += min_flow;
					edges[p^1].flow -= min_flow;
					u = edges[p^1].to;
				}while(u != s);
				max_flow += min_flow;
				min_flow = flow_t_inf;
			}
			else if (p == 0){
				if (--g[h[u]] == 0)
					break;
				
				min_h = n-1;
				for(p = head[u]; p; p = edges[p].nex){
					v = edges[p].to;
					if (edges[p].flow < edges[p].cap && h[v] < min_h) 
						min_h = h[v];
				}
				h[u] = min_h + 1;
				++g[h[u]];
				
				if (u != s)
					u = edges[pre[u]^1].to;
			}
		}
	}
	
	void refresh(){
		for(size_t u = 1; u <= n; ++u){
			for(size_t p = head[u]; p; p = edges[p].nex){
				edges[p].flow = 0;
			}
		}
		max_flow = 0;
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
				cout<<" "<<edges[p].to<<"|"<<edges[p].flow<<"|"<<edges[p].cap;
			}
			cout <<endl;
		}
		cout << endl;
	}
	
	void print_gh(){
		for(size_t u = 1; u <= n; ++u){
			cout <<h[u]<<" ";
		}
		cout <<endl;
		for(size_t u = 1; u <= n; ++u){
			cout <<g[u]<<" ";
		}
		cout <<endl;
	}
	
	size_t n;
	size_t *head;
	Edge *edges;
	size_t ecnt;
	size_t *pre;
	size_t *h;
	size_t *g;
	flow_t max_flow;
	static flow_t flow_t_inf;
};

Graph::size_t Graph::flow_t_inf = 0x7fffffff;


void test(){
	int n, m, s, t, u, v, cap;
	cin>>n>>m>>s>>t;
	
	Graph graph(n);
	
	for(int i = 0; i != m; ++i){
		cin>>u>>v>>cap;
		graph.add_edge(u, v, cap);
	}
	//graph.print_graph();
	graph.isap(s, t);
	
	cout<<graph.max_flow<<endl;
	graph.print_flow_graph();
	
	graph.refresh();
	graph.print_graph();
	graph.isap(s, t);
	cout<<graph.max_flow<<endl;
	graph.print_flow_graph();
}

int main(){
	freopen("in.txt", "r", stdin);
	test();	
	return 0;
}









