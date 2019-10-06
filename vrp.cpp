#ifndef VRP
#define VRP

#include <bits/stdc++.h>

#include "union-find.cpp"

using namespace std;

typedef float coord_t;
typedef float distance_t;
typedef float demand_t;


const int MAX_SIZE = 1000;
const float INF = 999999999.9;

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















// HEURISTICAS


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
				g.matrix[j][i] = dist(g.nodes[i], g.nodes[j]);
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
map<list<int> *, int> find_solution_CW(graph & g){
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

	//cout << g.name << endl;

	map<list<int> *, int> res;
	for (int x = 2; x <= n; x++){
		res[routes[x]]++;
	}

	return res;

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











// EXATOS





// BACKTRACKING

demand_t bt_min_cost = INF;
long int bt_iters = 0;
vector<int> bt_sol;

float sub_rout_cost(vector<int> & sub, graph & g, int b, int e){

	float r = 0.0;

	if (b != e){
		r += g.matrix[1][sub[b]];

		for (int i = b; i < e-1; i++){
			r += g.matrix[sub[i]][sub[i+1]];
		}

		r += g.matrix[sub[e-1]][1];
	}

	//cout << r << endl;


	//cout << "[" << b <<";" << e<< "]\n"; 


	return r;

}

float solution_cost(vector<int> & sol, graph & g){


	float c = 0.0;

	int it = 2;
	int b_it = 0, e_it = g.dimension;
	float atual_c = 0.0;
	while (it < g.dimension-1){

		if (atual_c == 0){
			b_it = it;
		}

		if (atual_c + g.nodes[sol[it]].demand > g.capacity){

			e_it = it;

			c += sub_rout_cost(sol, g, b_it, e_it);
			atual_c = 0;

		} else {
			atual_c += g.nodes[sol[it]].demand;
			it++;
		}

		
	}

	c += sub_rout_cost(sol, g, b_it, g.dimension);

	return c;

}

void backtracking( vector<bool> & mask, graph & g, demand_t cost, int customers, int previous, vector<int> & sol){

	//cout << bt_iters++ << endl;
	// chegou numa folha


	if (customers == g.dimension - 2){

		float c = solution_cost(sol, g);

		// for (int i = 0; i < g.dimension; i++){
		// 	//bt_sol[i] = sol[i];
		// 	cout << sol[i] << " ";
		// }
		// cout << c << "/" << bt_min_cost << endl; 

		//cout << endl;

		if ( c < bt_min_cost){
			bt_min_cost = c;

			
			for (int i = 0; i < g.dimension; i++){
				bt_sol[i] = sol[i];
				//cout << sol[i] << " ";
			}

			//cout << bt_min_cost << endl;


		}

		//cout << c << "/" << bt_min_cost << endl; 


		//cout << "fim\n";

		return;

	}

	// poda simples
	if (cost > bt_min_cost){
		//cout << "poda\n";
		return;
	}


	// chamada do bt
	for (int i = 2; i < g.dimension; i++){
		if (!mask[i]){

			mask[i] = true;
			sol[customers+2] = i;

			backtracking(mask, g, cost + g.matrix[previous][i], customers + 1, i, sol);
			
			//sol[customers+2] = 0;
			mask[i] = false;
		}
	}

}


map<list<int> *, int> find_solution_backtracking(graph & g){
	
	bt_min_cost = INF;
	bt_sol.resize(g.dimension);


	map<list<int> *, int> res;

	vector<bool> mask(g.dimension, false);
	vector<int> sol(g.dimension, 0);

	backtracking(mask, g, 0.0, 0, 1, sol);



	float c = 0.0;

	int it = 2;
	//int b_it = 0, e_it = g.dimension;
	float atual_c = 0.0;

	list<int> * route;

	while (it < g.dimension){

		if (atual_c == 0){
			//b_it = it;
			route = new list<int>;
		}

		if (atual_c + g.nodes[bt_sol[it]].demand > g.capacity){

			res[route] = atual_c;

			atual_c = 0;

		} else {
			atual_c += g.nodes[bt_sol[it]].demand;

			route->push_back(bt_sol[it]);

			it++;
		}

		
	}

	if (atual_c > 0){
		res[route] = atual_c;

	}



	//c += sub_rout_cost(sol, g, b_it, g.dimension);

	// cout <<  "MIN COST : " << bt_min_cost << endl ;

	return res;


}






// PROGRAMACAO DINAMICA


/*
map<list<int> *, int> find_solution_dp(graph & g){
	map<list<int> *, int> res;

	for (int l = 0; l < g.dimension-1; l++){



	}


	return res;
}


*/







// BRANCH AND BOUND

enum status{
	NOT_LEAF,
	VALID_LEAF,
	NOT_VALID_LEAF
};

// node of the BB tree
struct solution{
	int atual_node;

	float cost; // limite inferior

	float atual_distance;
	float demand;

	solution * previous;

	int nivel;
	status st;

	solution(int a, float c = 0.0, float d = 0.0, float e = 0.0, solution * p = nullptr, int n = 0, status s = NOT_LEAF){
		atual_node = a;
		cost = c;
		atual_distance = d;
		demand = e;
		previous = p;
		nivel = n;
		st = s;	
	}
};

struct extended_graph{
	string name;
	int vehicles;
	int dimension;
	demand_t capacity;

	int deposit;

	vector<node> nodes;
	vector<vector<distance_t>> matrix;

	extended_graph(){}

	extended_graph(graph & g, int v){ 
		name = g.name;
		vehicles = v;
		dimension = g.dimension + v;
		capacity = g.capacity;

		deposit = g.deposit;

		nodes.resize(g.dimension + vehicles);
		for (int i = 0; i < dimension; ++i) {
			if (i >= g.dimension){
				node n;
				n.x = g.nodes[1].x;
				n.y = g.nodes[1].y;
				n.demand = g.nodes[1].demand;

				nodes[i] = n;
			} else {
				nodes[i] = g.nodes[i];
			}
		}

		matrix.resize(dimension);
		for (auto & e : matrix)
			e.resize(dimension);

		for(int i = 0; i < dimension; i++){
			for(int j = 0; j < dimension; j++){
				if (i == j){
					matrix[i][j] = INF;
				} else {
					if (i < g.dimension and j < g.dimension){
						matrix[i][j] = g.matrix[i][j];
					} else if (i < g.dimension and j >= g.dimension){
						matrix[i][j] = g.matrix[i][1];
					} else if (i >= g.dimension and j < g.dimension){
						matrix[i][j] = g.matrix[1][j];

					} else {
						matrix[i][j] = 0;
					}

				}

				//cout << matrix[i][j] << " ";
			}
			//cout << endl;


		}

	}
};

vector<int> get_mask_of_path(solution * sol, int size){

	vector<int> res(size, 0);

	solution * it = sol;

	while (it->previous != nullptr){
		res[it->atual_node]+=2;
		res[it->previous->atual_node]+=1;
		it = it->previous;
	}


	// se o valor na mascara for 1, quer dizer que a linha está bloqueada
	// se for 2, entao a coluna está bloqueada
	// se for 3, ambos estão bloqueados
	return res;
}

// lower bound
float get_cost_of_solution(solution * sol, extended_graph & eg){
	float res = 0.0;

	auto mask = get_mask_of_path(sol, eg.dimension);

	for (int i = 2; i < eg.dimension; ++i) {

		if (mask[i] != 1 or mask[i] != 3){

			float m = INF;

			for (int j = 2; j < eg.dimension; ++j) {
				if (mask[j] != 2 or mask[j] != 3){

					if (eg.matrix[i][j] < m){
						m = eg.matrix[i][j];
					}

				}
			}

			res += m;
		}
		
	}

	res += sol->atual_distance;

	return res;
}

class Compare {
	public:
    bool operator() (solution * lhs, solution * rhs)
    {
        return lhs->cost > rhs->cost;
    }
};


list<int> get_path(solution * sol, int depot){

	list<int> res;

	res.push_front(1);

	solution * it = sol;

	while (it != nullptr){
		// res[it->atual_node]+=2;
		// res[it->previous->atual_node]+=1;
		if (it->atual_node >= depot)
			res.push_front(1);
		else
			res.push_front(it->atual_node);

		it = it->previous;
	}


	return res;


}



map<list<int> *, int> find_solution_bb(graph & g){

	// int atual_node;
	// float cost; // limite inferior
	// float atual_distance;
	// float demand;
	// solution * previous;
	// int nivel
	// status st;

	map<list<int> *, int> res;

	int k = 5;
	if (g.dimension <= 20)
		k = 5;
	else if (g.dimension > 20 and g.dimension < 32)
		k = 7; 
	else if ( g.dimension > 32 and g.dimension < 39)
		k = 9;
	else
		k = 10;

	cout << k << endl;
	extended_graph eg(g,k);
	priority_queue<solution *, vector<solution *>, Compare> pq;

	int depot = g.dimension;

	solution * s = new solution(depot);
	s->cost = get_cost_of_solution(s, eg);
	//pq.push(s);

	vector<int> mask_ini(eg.dimension, 0);
	mask_ini[depot] = 1;

	for (int i = 2; i < eg.dimension; i++){
		if (mask_ini[i] == 0){
			solution * x = new solution(i, 0.0, eg.matrix[depot][i], eg.nodes[i].demand, s, 1);
			x->cost = get_cost_of_solution(x, eg);
			pq.push(x);
		}
	}

	int max_nivel = 0;

	while (!pq.empty()){
		auto t = pq.top();
		auto t_mask = get_mask_of_path(t, eg.dimension);

		//cout << t->atual_node << " (dist:" << t->atual_distance << ", dem:" << t->demand << ") PREV " << t->previous->atual_node << " NIVEL" << t->nivel << endl;
		//bool leaf = true;


		if (t->st==NOT_LEAF){
			for (int i = 2; i < eg.dimension; i++){
				if (t_mask[i] == 0){
					//leaf = false;

					solution * x = new solution(i, 0.0,  
												t->atual_distance + eg.matrix[t->atual_node][i], 
												t->demand + eg.nodes[i].demand, 
												t,
												t->nivel+1);
					x->cost = get_cost_of_solution(x, eg);

					if (x->atual_node >= depot){
						x->demand = 0;
					}

					if (x->nivel == eg.dimension-3){
						x->st = VALID_LEAF;
					}

					if (x->demand > eg.capacity){
						x->st = NOT_VALID_LEAF;
					}

					// if (x->nivel > max_nivel){
					// 	max_nivel = x->nivel;
					// 	cout << max_nivel << endl;
					// }
					pq.push(x);
				}
			}

		} else if (t->st == VALID_LEAF){
			//cout << "MIN " <<  (t->atual_distance + eg.matrix[t->atual_node][depot]) << endl;

			auto r_list = get_path(t, depot);

			bool ro = true;

			list<int> * aux = new list<int>;
			float dem = 0.0;
			
			//cout << r_list.front() << endl;

			r_list.pop_front();



			while(!r_list.empty()){
				auto f = r_list.front();
				//cout << f << endl;

				if (f == 1){
					if (!aux->empty()){

						// for (auto & e : *aux){
						// 	cout << e << " ";
						// }
						// cout << endl;
						res[aux] = dem;

					}

					dem = 0.0;
					aux = new list<int>;

				} else {
					dem += g.nodes[f].demand;
					aux->push_back(f);
				}


				r_list.pop_front();

			}

			break;
		}
		

		//cout << pq.top()->atual_node << " cost = " << pq.top()->demand  << endl;
		pq.pop();
	}

	// cout << "FINISHED\n";

	// distance_t total_cost = 0;

	// for (auto & x : res){
	// 	if (x.first->size() > 0){
	// 		demand_t d = 0;
	// 		vector<int> gamb;
	// 		gamb.push_back(1);

	// 		for (auto & p : *x.first){
	// 			d += g.nodes[p].demand;
	// 			cout << p << " ";
	// 			gamb.push_back(p);
	// 		}

	// 		gamb.push_back(1);

	// 		distance_t co = 0;

	// 		for (int r = 0; r < gamb.size()-1; r++){
	// 			co += g.matrix[gamb[r]][gamb[r+1] ];
	// 		}

	// 		cout << " (" << d <<  ")" << endl;
	// 		total_cost += co;
	// 	}
		
	// }

	// cout << "Cost " << total_cost << endl;

	// cout << endl;

	return res;

}












void save_solution(graph & g, map<list<int> *, int> & res, string out){

	string of_file = out + g.name;
	
	ofstream ofs(of_file);

	//ofs << g.name << endl;

	distance_t total_cost = 0;

	for (auto & x : res){
		if (x.first->size() > 0){
			demand_t d = 0;
			vector<int> gamb;
			gamb.push_back(1);

			for (auto & p : *x.first){
				d += g.nodes[p].demand;
				ofs << p << " ";
				gamb.push_back(p);
			}

			gamb.push_back(1);

			distance_t co = 0;

			for (int r = 0; r < gamb.size()-1; r++){
				co += g.matrix[gamb[r]][gamb[r+1] ];
			}

			ofs << " (" << d <<  ")" << endl;
			total_cost += co;
		}
		
	}

	ofs << "Cost " << total_cost << endl;

	ofs << endl;

}

void print_solution(graph & g, map<list<int> *, int> & res){

	//ofs << g.name << endl;

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

}

void show_solution(graph & g, list<list<int>> & l){

	//cout << g.name << " : " << g.dimension <<  endl;
}

#endif