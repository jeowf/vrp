#ifndef VRP
#define VRP

#include <bits/stdc++.h>

using namespace std;

typedef float coord_t;
typedef float distance_t;
typedef float demand_t;

struct node{
	coord_t x;
	coord_t y;

	demand_t demand;

	node(){ }
};

struct graph{
	string name;
	int dimension;
	demand_t capacity;

	int deposit;

	vector<node> nodes;
	vector<vector<distance_t>> matrix;

	graph(){ }
}; 

distance_t dist(node & a, node & b){
	return sqrt(pow(a.x - b.x ,2) + pow(a.y - b.y ,2));
}

graph load_graph(string path){

	graph g;

	ifstream instance(path);

	// lendo e tratando os arquivos
	while (!instance.eof()){
		string s;

		instance >> s;

		if (s == "NAME"){
			instance >> s;
			instance >> s;
			g.name = s;
		}

		if (s == "DIMENSION"){
			instance >> s;

			int dim;

			instance >> dim;
			g.dimension = dim+1;

			g.nodes.resize(dim+1);
			g.matrix.resize(dim+1);
			for (auto & e : g.matrix)
				e.resize(dim+1);
		}

		if (s == "CAPACITY"){
			instance >> s;

			int cap;

			instance >> cap;
			g.capacity = cap;
		}

		if (s == "NODE_COORD_SECTION"){
			int n, x, y;

			instance >> n >> x >> y;

			g.nodes[n].x = x;
			g.nodes[n].y = y;
		}

		if (s == "DEMAND_SECTION"){
			int n, d;

			instance >> n >> d;

			g.nodes[n].demand = d;
		}

		if (s == "DEPOT_SECTION "){
			int n;

			instance >> n;

			g.deposit = n;
		}

	}

	instance.close();

	// gerando matriz

	for (int i = 1; i < g.dimension; i++){
		for (int j = 1; j < g.dimension; j++){
			if (i == j){
				g.matrix[i][j] = -1;
			} else {
				g.matrix[i][j] = dist(g.nodes[i], g.nodes[j]);
			}

		}
	}

	return g;
}

void print_graph(graph & g){
	cout << g.name << " : " << g.dimension <<  endl;
}



#endif