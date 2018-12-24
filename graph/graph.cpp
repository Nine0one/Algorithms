#include <iostream>
#include <string>
#include <queue>
#include <stack>
#include <list>

using namespace std;

class DFS;
class BFS;

class Graph
{
private:
vector<list<int>> adj;
const int V;
int E;
public:
	Graph(int V) : V(V), E(0), adj(vector<list<int>>(V)) {
		
	}

	void addEdge(int v, int w) {
		adj[v].push_back(w);
		adj[w].push_back(v);
		E++;
	}

	friend class DFS;
	friend class BFS;

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

class DFS
{
private:
	vector<bool> marked;

public:
	DFS(Graph &G, int s) : marked(vector<bool>(G.V, false)){
		DFS_(G, s);
	}

	void DFS_(Graph &G, int v){
		marked[v] = true;
		cout << v << " ";
		for (auto w : G.adj[v]) {
			if (!marked[w])
				DFS_(G, w);
		}
	}

	~DFS() {
	}
};

class BFS
{
private:
	vector<bool> marked;
public:
	BFS(Graph &G, int s) : marked(vector<bool>(G.V, false)) {
		BFS_(G, s);
	}

	void BFS_(Graph &G, int s) {
		queue<int> q;
		q.push(s);
		marked[s] = true;
		while(!q.empty()) {
			int v = q.front(); q.pop();
			cout << v << " ";
			for (auto w : G.adj[v])
				if (!marked[w]) {
					q.push(w);
					marked[w] = true;
				}
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
	BFS bfs(G, 0);
	cout << endl;
	DFS dfs(G, 0);
	cout << endl << G.toString();

	return 0;
}

