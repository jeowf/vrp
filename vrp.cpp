#ifndef VRP
#define VRP

#include <bits/stdc++.h>

#include "union-find.cpp"

using namespace std;

typedef float coord_t;
typedef float distance_t;
typedef float demand_t;


const int MAX_SIZE = 1000;

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

struct cicle {
	vector<int> nodes;
	demand_t total_demand;
	distance_t cost;

	cicle(){
		// nodes.resize(1000);
		// total_demand = 0;
		// cost = 0;
	}
};












struct edge{
	int i;
	int j;
	distance_t cost;

	edge(){}

	edge(int a, int b, distance_t c){
		i = a;
		j = b;
		cost = c;
	}
};


bool comp_dec(edge & lhs, edge & rhs){
	return lhs.cost > rhs.cost;

	//return false;
}


int first_e(cicle & c){
	return c.nodes[1];
}

int last_e(cicle & c){
	auto s = c.nodes.size();
	return c.nodes[s-2];
}

// conecta o ultimo elemento de a com o primeiro de b
void merge_cicle(cicle & a, cicle & b, vector<vector<distance_t>>  & savings, graph & g){
	auto f_a = first_e(a);
	auto l_a = last_e(a);

	auto f_b = first_e(b);
	auto l_b = last_e(b);

	auto dep = a.nodes[0];


	a.nodes.pop_back();
	for (int i = 1; i < b.nodes.size(); i++){
		a.nodes.push_back(b.nodes[i]);
	}

	a.cost = a.cost - (g.matrix[dep][f_a] + g.matrix[dep][l_b]) + g.matrix[l_a][f_b];
	a.total_demand += b.total_demand;

	savings[f_a][l_b] = g.matrix[dep][f_a] + g.matrix[dep][l_b] - g.matrix[l_a][f_b];

	b.nodes.clear();
	//b.nodes = a.nodes;


}

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

			for (int i = 1; i < g.dimension;i++){
				instance >> n >> x >> y;
				//cout << n << " " << x << " " << y << endl;
				g.nodes[n].x = x;
				g.nodes[n].y = y;
			}
		}

		if (s == "DEMAND_SECTION"){
			int n, d;

			for (int i = 1; i < g.dimension;i++){
				instance >> n >> d;

				g.nodes[n].demand = d;
			}
		}

		if (s == "DEPOT_SECTION"){
			int n;
			instance >> n;

			g.deposit = n;
		}

	}

	instance.close();

	// gerando matriz de distancias euclidianas

	for (int i = 1; i < g.dimension; i++){
		for (int j = 1; j < g.dimension; j++){
			if (i == j){
				g.matrix[i][j] = -1;
			} else {
				g.matrix[i][j] = dist(g.nodes[i], g.nodes[j]);
				//cout << dist(g.nodes[i], g.nodes[j]) << " ";
				//printf("[(%f,%f) & (%f,%f) = %f] ", g.nodes[i].x, g.nodes[i].y, g.nodes[j].x, g.nodes[j].y, g.matrix[i][j]);
			}

		}
		//cout << endl;
	}
	// cout << endl;
	// cout << endl;
	// cout << endl;
	// cout << endl;

	return g;
}


// encontrar solução usando método de Clarke e Wright
void find_solution_CW(graph & g){
	// iniciando variaveis

	int n = g.dimension - 1; //
	int n_ini = g.deposit;
	demand_t cap = g.capacity;

	//vector<cicle> sol(n+1);
	list<list<int>> sol;

	vector<vector<distance_t>> savings;

	savings.resize(n+1);
	for (auto & e : savings)
		e.resize(n+1);


	// calculando economia de custo par a par

	vector<edge> list_savings;

	for (int i = 1; i <= n; i++){
		for (int j = i+1; j <= n; j++){
			if (i!= n_ini and j!= n_ini){

				savings[i][j] = g.matrix[n_ini][i] + g.matrix[n_ini][j] - g.matrix[i][j];
				edge e(i, j, savings[i][j]);
				list_savings.push_back(e);
			}
			

		}
	}
	

	//ordenação não crescente dos pares de nos
	sort(list_savings.begin(), list_savings.end(), comp_dec);

	// for (auto & e: list_savings){
	// 	cout << e.i << " " << e.j << " " << e.cost << endl;
	// }


	disjoint_set ds(n+1);
	vector< list<int> * > routes(n+1);

	for (int i = 1; i <= n; i++){
		if (i != n_ini){
			routes[i] = new list<int>;
			routes[i]->push_back(i);
		}
	}

	int actual = -1;

	for (auto & e : list_savings){
		auto a = ds.find(e.i);
		auto b = ds.find(e.j);

		if (a != b){
			demand_t demand_a = 0;
			demand_t demand_b = 0;
			for (auto k : *routes[e.i])
				demand_a += g.nodes[k].demand;

			for (auto k : *routes[e.j])
				demand_b += g.nodes[k].demand;


			//cout << demand_a << " " << demand_b << endl; 

			if ((demand_a + demand_b) <= cap){

				if ( routes[e.i]->front() == e.i and routes[e.j]->back() == e.j ){

					for (auto k : *routes[e.i]){
						routes[e.j]->push_back(k);
					}

					routes[e.i]->clear();
					routes[e.i] = routes[e.j];

					ds.uni(e.j, e.i);

				} else if (routes[e.i]->back() == e.i and routes[e.j]->front() == e.j){

					for (auto k : *routes[e.j]){
						routes[e.i]->push_back(k);
					}

					routes[e.j]->clear();
					routes[e.j] = routes[e.i];

					ds.uni(e.i, e.j);

				}

				//cout << endl;

			}
		}

		//cout << a << " " << b << endl;


		//actual = -1;
	}

	cout << g.name << endl;

	map<list<int> *, int> res;
	for (int x = 2; x <= n; x++){
		res[routes[x]]++;
	}

	distance_t total_cost = 0;

	for (auto & x : res){
		if (x.first->size() > 0){
			demand_t d = 0;
			vector<int> gamb;
			gamb.push_back(1);

			for (auto & p : *x.first){
				d += g.nodes[p].demand;
				cout << p << " ";
				gamb.push_back(p);
			}

			gamb.push_back(1);

			distance_t co = 0;

			for (int r = 0; r < gamb.size()-1; r++){
				co += g.matrix[gamb[r]][gamb[r+1] ];
			}

			cout << " (" << d <<  ")" << endl;
			total_cost += co;
		}
		
	}

	cout << "Cost " << total_cost << endl;

	cout << endl;

	//return sol;

}

void show_solution(graph & g, list<list<int>> & l){

	//cout << g.name << " : " << g.dimension <<  endl;
}



#endif