#include <iostream>
#include <string>
#include <queue>
#include <stack>
#include <forward_list>

using namespace std;

class Edge
{
public:
	const int v, w;
	const double weight;
	Edge(int v, int w, double weight) : v(v), w(w), weight(weight) {}	
};

class DFSPaths;
class BFSPaths;

// 无向图定义
class Graph
{
private:
vector<forward_list<int>> adj;
const int V;
int E;
public:
	Graph(int V) : V(V), E(0), adj(vector<forward_list<int>>(V)) {
		
	}

	void addEdge(int v, int w) {
		adj[v].push_front(w);
		adj[w].push_front(v);
		E++;
	}

	friend class DFSPaths;
	friend class BFSPaths;

	string toString() {
		string s;
		s += to_string(V);
		s += " vertices ";
		s += to_string(E);
		s += " edges\n";
		for (int v = 0; v < V; v++){
			s += to_string(v) + ": ";
			for (int w : adj[v]) {
				s += to_string(w);
				s += " ";
			}
			s += "\n";
		}
		return s;
	}

	~Graph() {
	}
	
};

class DFSPaths
{
private:
	vector<bool> marked;
	vector<int> edge_to;
	int source;

public:
	DFSPaths(Graph &G, int s) : source(s), marked(vector<bool>(G.V, false)), edge_to(vector<int>(G.V, -1)){
		DFS_(G, s);
	}

	void DFS_(Graph &G, int v){
		marked[v] = true;
		for (auto w : G.adj[v]) {
			if (!marked[w]) {
				edge_to[w] = v;
				DFS_(G, w);
			}
		}
	}

	void path_to(int t) {
		stack<int> s;
		for (int x=t; x != source; x = edge_to[x]) {
			s.push(x);
		}
		s.push(source);
		while (!s.empty()) {
			cout << s.top() << " " ;
			s.pop();
		}
	}

};

class BFSPaths
{
private:
	vector<bool> marked;
	vector<int> edge_to;
	int source;
	int furthest;
public:
	BFSPaths(Graph &G, int s) : source(s), marked(vector<bool>(G.V, false)), edge_to(vector<int>(G.V, -1)) {
		BFS_(G, s);
	}

	void BFS_(Graph &G, int s) {
		queue<int> q;
		q.push(s);
		while(!q.empty()) {
			int v = q.front(); q.pop();
			// 最后一个入队列（出队列）的即为距离源点最远的点
			furthest = v;
			for (auto w : G.adj[v])
				if (!marked[w]) {
					q.push(w);
					marked[w] = true;
					edge_to[w] = v;
				}
		}
	}

	bool has_path_to(int v) {
		return marked[v];
	}

	int furthest_to() {
		return furthest;
	}

	void path_to(int t) {
		stack<int> s;
		for (int x=t; x != source; x = edge_to[x]) {
			s.push(x);
		}
		s.push(source);
		while (!s.empty()) {
			cout << s.top() << " " ;
			s.pop();
		}
	}
};


int main(int argc, char const *argv[])
{
	Graph G(6);
	G.addEdge(0,2);
	G.addEdge(0,1);
	G.addEdge(1,2);
	G.addEdge(3,5);
	G.addEdge(3,4);
	G.addEdge(2,3);
	G.addEdge(2,4);
	G.addEdge(0,5);
	// BFSPaths(G, 0).path_to(5);
	cout << endl << G.toString() << endl;
	cout << "距离0最远的结点是" << BFSPaths(G, 0).furthest_to();

	return 0;
}

