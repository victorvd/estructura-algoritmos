#include <bits/stdc++.h>

using namespace std;

void print_vector(string name, vector<vector<int>> vec){

	cout << name << " = ";
	cout << "[";
	
	for (int i = 0; i < vec.size(); i++) {
		cout << "[";
		
		for (int j = 0; j < vec[i].size(); j++) {
			cout << vec[i][j];
			if ( j != vec[i].size() - 1)
				cout << ", ";
		}
		
		cout << "]";
		
		if ( i != vec.size() - 1)
			cout << ", ";
	}
	
	cout << "]"<<endl;
}

class Solution {
 public:
	int shrPathLng(vector <vector <int>>& graph){
		queue <vector <int>> que;
		int n = graph.size();
		int req = (1 << n) - 1;
		map<int, set<int> > visited;
		
		for (int i = 0; i < n; i++) {
			 que.push({ 0 | (1 << i), i });
		}
		
		if (n == 1)
			return 0;
		
		for (int lvl = 1; !que.empty(); lvl++) {
			int sz = que.size();
			while (sz--) {
				vector<int> actl = que.front();
				que.pop();
				
				for (int i = 0; i < graph[actl[1]].size(); i++) {
					int u = graph[actl[1]][i];
					int newMsk = (actl[0] | (1 << u));
					
					if (newMsk == req)
						return lvl;
						
					if (visited[u].count(newMsk))
						continue;
						
					visited[u].insert(newMsk);
					que.push({ newMsk, u });
				}
			}
		}
		return -1;
	}
};

int main()
{
	Solution spl;
	
	vector<vector<int>> graph_1 = {{1,2,3},{0},{0},{0}};
	
	vector<vector<int>> graph_2 = {{1},{0,2,4},{1,3,4},{2},{1,2}};
	
	print_vector("graph_1", graph_1);
	
	cout << "Length of the shortest path: " << (spl.shrPathLng(graph_1)) << endl;
	
	print_vector("graph_2", graph_2);
	
	cout << "length of the shortest path: " << (spl.shrPathLng(graph_2)) << endl;
	
	return 0;
}
