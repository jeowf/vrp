#ifndef GA
#define GA

#include <bits/stdc++.h>

#include "union-find.cpp"

using namespace std;

float epsilon = 5.0;
int population_size = 100;
int selection_size = 30;
int total_pop_size = 150;

float mut_percent = 0.5;



//unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
unsigned seed = 2;
auto aux_seed = seed;


struct cromossome{
	vector<int> sol;
	vector<float> demand;
	float cost;
	float fitness;
	bool feasible;
	bool mutation;

	cromossome(){ 
		mutation = false;
	}

	cromossome(vector<int> s, vector<float> d, float c, float fi, bool f = true, bool m = false){
		sol = s;
		demand = d;
		cost = c;
		fitness = fi;
		feasible = f;
		mutation = m;

	}

	cromossome(const cromossome & c){
		sol = c.sol;
		demand = c.demand;
		cost = c.cost;
		fitness = c.fitness;
		feasible = c.feasible;
		mutation = c.mutation;
	}
};

float fitness_function(float cost){
	return 1.0/cost;
}

void calcule_fitness(cromossome & c, graph & g){

	float res = 0.0;

	float dist = 0.0;	

	dist += g.matrix[1][c.sol[0]];

	for (int i = 0; i < c.sol.size()-1; i++){
		dist+= g.matrix[c.sol[i]][c.sol[i+1]];
	}

	dist += g.matrix[c.sol[c.sol.size()-1]][1];

	c.cost = dist;


	int vehicle = 0;
	vector<float> demands;

	float local_demand = 0.0;

	float penalty = 0.0;

	for (int i = 0; i < c.sol.size(); i++){
		//cout << local_demand << endl;
		if (c.sol[i] == 1 or i == c.sol.size()-1){
			//cout << local_demand << endl;

			if (local_demand > g.capacity){
				penalty += local_demand - g.capacity;
				//cout << penalty << endl ;

			}

			demands.push_back(local_demand);
			local_demand = 0;
		} else {
			local_demand += g.nodes[c.sol[i]].demand;
		}
	}

	c.demand = demands;

	if (penalty > 0.0)
		c.feasible = false;
	else
		c.feasible = true;

	c.fitness = fitness_function(dist + penalty*epsilon);
	c.mutation = false;

}

void fitness(vector<cromossome> & population, graph & g){
	for (auto & e : population)
		calcule_fitness(e, g);
}



vector<cromossome> selection(vector<cromossome> & population, graph & g){

	int n = population.size();

	vector<cromossome> res;

	for (int i = 0; i < selection_size; i++){
		res.push_back(population[rand() % n]);
		res.push_back(population[rand() % n]);
	}

	return res;

}

vector<int> head_tail( vector<int> & head, vector<int> & tail, int cut_point){
	vector<int> res;

	int i = 0;

	vector<int> ctrl(max(head.size(), tail.size()) + 1, 0);
	vector<int> use_after;

	while (i < cut_point){
		res.push_back(head[i]);
		ctrl[head[i]] = -1; // proibido

		if (ctrl[tail[i]] == 0){
			ctrl[tail[i]] = 1; // permitido

			if (tail[i] != 1)
				use_after.push_back(tail[i]);
		}

		i++;
	}

	ctrl[1] = 0;

	while (i < tail.size()){
		int v = 0;

		if (ctrl[tail[i]] == -1){
			for (int j = 0; j < use_after.size(); j++){
				if (ctrl[use_after[j]] == 1){
					v = use_after[j];
					ctrl[use_after[j]] = -1;
					break;
				}

			}
		} else {
			v = tail[i];
		}

		res.push_back(v);
		i++;
	}

	// caso falte algum valor nos nós
	for (int j = 0; j < use_after.size(); j++){
		if (ctrl[use_after[j]] == 1){
			int v = use_after[j];
			ctrl[v] = -1;
			
			for (int i = cut_point; i < res.size(); i++){
				if (res[i] == 1){
					res.insert(res.begin()+i, v);
					//res[i] = v;
					break;
				}

			}
		}
	}

	// caso sobre algum valor

	for (int i = 0; i < res.size(); ){
		if (res[i] == 0){
			res.erase(res.begin() + i);
		} else{
			i++;
		}
	}

	return res;
}

vector<cromossome> one_point_crossover(cromossome & lhs, cromossome & rhs, graph & g, vector<cromossome> & population){
	vector<cromossome> res;

	int n = min(lhs.sol.size(), rhs.sol.size());
	int N = max(lhs.sol.size(), rhs.sol.size());

	int point = rand() % n;

	/*	
	cout << "Crossover\n";

	cout << " (1) ";
	for(auto & e : lhs.sol){
		cout << e << " ";
	}
	cout << endl;

	cout << " (2) ";
	for(auto & e : rhs.sol){
		cout << e << " ";
	}
	cout << endl;


	cout << " Selected point was " << point << endl;
	*/

	auto a = head_tail(rhs.sol, lhs.sol, point);
	auto b = head_tail(lhs.sol, rhs.sol, point);

	/*	
	vector<int> a;
	vector<int> b;

	vector<int> aux_a(N+1, 0);
	vector<int> aux_b(N+1, 0);

	// 2 3 6 | 9 5 8 4
	// 4 2 5 | 8 6 9 3 

	// 2 3 6 | 8 6 9 3
	// 4 2 5 | 9 3 8 4

	for(int i = 0; i < point; i++){
		a.push_back(lhs.sol[i]);
		aux_a[lhs.sol[i]]++;

		b.push_back(rhs.sol[i]);
		aux_b[rhs.sol[i]]++;
	}

	aux_a[1] = 0;
	aux_b[1] = 0;

	for(int i = point; i < lhs.sol.size(); i++){
		int v;

		if(aux_b[lhs.sol[i]] == 1){

			for (int j = 0; j < point; j++){
				if (aux_b[lhs.sol[j]] == 0){
					v = lhs.sol[j];
					break;
				}
			}

		} else {
			v = lhs.sol[i];
		}

		aux_b[v]++;
		b.push_back(v);
	}

	for(int i = point; i < rhs.sol.size(); i++){
		int v;

		if(aux_a[rhs.sol[i]] == 1){
			
			for (int j = 0; j < point; j++){
				if (aux_a[rhs.sol[j]] == 0){
					v = rhs.sol[j];
					break;
				}
			}

		} else {
			v = rhs.sol[i];
		}

		aux_a[v]++;
		a.push_back(v);
	}


	//pair<cromossome, cromossome> res = make
	*/
	cromossome c_a;
	cromossome c_b;
	c_a.sol = a;
	c_b.sol = b;

	calcule_fitness(c_a, g);
	calcule_fitness(c_b, g);

	res.push_back(c_a);
	res.push_back(c_b);

	return res;

	//return make_pair(c_a,c_b);
/*
	
	cout << " generates " << endl;

	cout << " (a) ";
	for(auto & e : a){
		cout << e << " ";
	}
	cout << endl;

	cout << " (b) ";
	for(auto & e : b){
		cout << e << " ";
	}
	cout << endl;*/
	
}



vector<cromossome> crossover(cromossome & lhs, cromossome & rhs, graph & g, vector<cromossome> & population, int crossover_type = 1){

	if (crossover_type == 1)
		return one_point_crossover(lhs, rhs, g, population);



}



vector<cromossome> crossover(vector<cromossome> & population, vector<cromossome> & pairs, graph & g){

	vector<cromossome> res;

	for (int i = 0; i < pairs.size(); i += 2){
		auto cs = crossover(pairs[i], pairs[i+1], g, population);
		res.push_back(cs[0]);
		res.push_back(cs[1]);
	}

	return res;

}


vector<cromossome> initialize_population(graph & g, int pop_size){

	vector<cromossome> population;

	for (int i = 0; i < pop_size; ++i) {
		cromossome c;
		vector<int> sol;

		for (int j = 2; j < g.dimension; ++j) {
			sol.push_back(j);
		}

		aux_seed = rand();
		srand(aux_seed);

		shuffle(sol.begin(), sol.end(), default_random_engine(aux_seed) );

		c.sol = sol;
		population.push_back(c);
		/*

		for (auto & e : c.sol) {
			cout << e << " ";
		}
		cout << endl;*/
	}


	return population;
}

void make_feasible(cromossome & c, graph & g){

	int n = c.sol.size();

	float local_demand = 0.0;

	//float penalty = 0.0;

	int a = 0;
	int b = n;

	for (int i = 0; i < n; i++){
		if (c.sol[i] == 1 or i == n-1){

			b = i;
			if (i == n-1)
				b += 1;

			if (local_demand > g.capacity){
				//penalty += local_demand - g.capacity;
				break;

			}

			a = i+1;
			local_demand = 0;
		} else {

			local_demand += g.nodes[c.sol[i]].demand;
		}
	}

	int r = rand() % (b-a-1);
	c.sol.insert(c.sol.begin() + a + r, 1);

}

void mutation(cromossome & c, graph & g){

	int n = c.sol.size();

	int a = rand() % n;
	int b = a;

	while (b == a)
		b = rand() % n;

	int v_a = c.sol[a];
	int v_b = c.sol[b];

	c.sol[a] = v_b;
	c.sol[b] = v_a;

}

void mutation(vector<cromossome> population,  graph & g){

	int n = population.size();
	int q = n * mut_percent;

	for(int i = 0; i < q; i++){
		int c_i = rand() % n;
		cromossome & c = population[c_i];

		if (!c.mutation){
			if (c.feasible)
				mutation(c,g);
			else
				make_feasible(c,g);


			c.mutation = true;
			calcule_fitness(c,g);
		}
/*
		for (auto & e : c.sol)
			cout << e << " ";
		cout << " -- " << c.fitness << " -- " << c.feasible << endl;
*/
		
/*
		for (auto & e : c.sol)
			cout << e << " ";
		cout << " -- " << c.fitness << " -- " << c.feasible << endl;
*/

		//cin >> c_i;

	}




}


bool comp(cromossome lhs, cromossome rhs){
	return lhs.fitness > rhs.fitness;
}

cromossome genetic_algorithm(graph & g){

	cout << "Using " << seed << " as seed\n";

	srand(seed);
	aux_seed = seed;

	cromossome res;

	vector<cromossome> population = initialize_population(g, population_size);
	fitness(population, g);

	int it_limit = 500;
	int i = 0;
/*
	vector<cromossome> sel;

	vector<int> a = { 2, 3, 1, 5, 6, 1, 4, 7};
	vector<int> b = { 7, 1, 2, 1, 5, 6, 1, 3, 4 };
	cromossome c_a;
	cromossome c_b;

	c_a.sol = a;
	c_b.sol = b;

	calcule_fitness(c_a, g);
	calcule_fitness(c_b, g);

	sel.push_back(c_a);
	sel.push_back(c_b);

	crossover(population, sel, g);
	//mutation(population, g);

*/


	while (i < 200){

		
		//crossover(population[rand() % population_size],population[rand() % population_size],g);
		cout << "SEL\n";
		auto sel = selection(population, g);

		cout << "CROSS\n";
		auto cross = crossover(population, sel, g);
		for (auto & e : cross)
			population.push_back(e);

		cout << "MUT\n";
		mutation(population, g);

		cout << "SORT\n";
		sort(population.begin(), population.end(), comp);
		//population.resize(total_pop_size);

		cout << "END\n";

		i++;
	}

	for (auto & e: population[0].sol)
		cout << e << " ";
	cout << endl;

	cout << population[0].cost << endl;
	
	int k;
	cin >> k;

	//cout << endl << endl;
	//population.clear();
	return res;

}






map<list<int> *, int> find_solution_GA(graph & g){
	map<list<int> *, int> res;

	auto c = genetic_algorithm(g);

	//int a;
	//cin >> a;

	return res;

}






#endif