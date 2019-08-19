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
vector<cicle> find_solution_CW(graph & g){
	cout << g.name << ":\n";
	// iniciando variaveis

	int n = g.dimension - 1; //
	int n_ini = g.deposit;
	demand_t cap = g.capacity;

	vector<cicle> sol(n+1);


	vector<vector<distance_t>> savings;


	savings.resize(n+1);
	for (auto & e : savings)
		e.resize(n+1);


	// cada no eh atendido por um carro

	for (int i = 1; i <= n; i++){
		if (i != n_ini){
			vector<int> nodes;
			nodes.push_back(n_ini);
			nodes.push_back(i);
			nodes.push_back(n_ini);


			cicle c;
			c.nodes = nodes;
			c.cost = 2*g.matrix[n_ini][i];
			c.total_demand = g.nodes[i].demand;

			sol[i] = c;
		}
		
	}

	//cout << "opa";


	vector<edge> list_savings;
	// calculando economia de custo par a par
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




	disjoint_set ds(n+1);

	//vector<bool> mask(n+1);

	for(auto & e : list_savings){

		int a = ds.find(e.i);
		int b = ds.find(e.j);

		if ( a != b ){
			//cout << "tnc";
			if ( sol[a].nodes.size() > 0 and sol[a].nodes.size() < MAX_SIZE and
			     sol[b].nodes.size() > 0 and sol[b].nodes.size() < MAX_SIZE ){
				//cout << "a: " << a << ", b: " << b << "   ";
				//cout << sol[a].nodes.size() << ", " << sol[b].nodes.size() << "  |||" << n  <<  endl;
			
				//cout << last_e(sol[a]) << " == " << a << ", " << first_e(sol[b]) << " == " << b << endl;

				if ( (last_e(sol[a]) == a and first_e(sol[b]) == b) 
					//and ( (sol[a].total_demand + sol[b].total_demand) <= cap ) 
					) {
					//cout << "opa";
					merge_cicle(sol[a],sol[b],savings,g);
					ds.uni(a,b);

				}


			}


		}

	}





	set<int> cicles;
	for (int k = 2; k <= n; k++){
		auto a = ds.find(k);
		cicles.insert(a);
	}



	for (auto & k : cicles){
		if (sol[k].nodes.size() > 0 and sol[k].nodes.size() < MAX_SIZE){
			cout << "> " ;
			for (auto & g : sol[k].nodes){
				cout <<  g << " ";
			}
			cout << endl;
		}
		
	}

	 	cout << endl;
		cout << endl;
		cout << endl;




	

	return sol;

}

void print_graph(graph & g){
	cout << g.name << " : " << g.dimension <<  endl;
}



#endif