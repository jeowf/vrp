#ifndef GA
#define GA

#include <bits/stdc++.h>

#include "union-find.cpp"

using namespace std;

float epsilon = 1.0;
int population_size = 50;



//unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
unsigned seed = 1;
auto aux_seed = seed;


struct cromossome{
	vector<int> sol;
	vector<float> demand;
	float cost;
	float fitness;
	bool feasible;

	cromossome(){ }

	cromossome(vector<int> s, vector<float> d, float c, float fi, bool f = true){
		sol = s;
		demand = d;
		cost = c;
		fitness = fi;
		feasible = f;

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
		if (c.sol[i] == 1 or i == c.sol.size()-1){

			if (local_demand > g.capacity)
				penalty += g.capacity - local_demand;

			demands.push_back(local_demand);
			local_demand = 0;
		} else {
			local_demand += c.sol[i];
		}
	}

	c.demand = demands;

	if (penalty > 0)
		c.feasible = false;

	c.fitness = fitness_function(dist + penalty*epsilon);

}

void fitness(vector<cromossome> & population, graph & g){
	for (auto & e : population)
		calcule_fitness(e, g);
}


void one_point_crossover(cromossome & lhs, cromossome & rhs, graph & g){
	int n = min(lhs.sol.size(), rhs.sol.size());
	int N = max(lhs.sol.size(), rhs.sol.size());

	int point = rand() % n;

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

	vector<int> a;
	vector<int> b;

	vector<int> aux_a(N+1,0);
	vector<int> aux_b(N+1,0);

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
	cout << endl;

}



void crossover(cromossome & lhs, cromossome & rhs, graph & g, int crossover_type = 1){

	if (crossover_type == 1)
		one_point_crossover(lhs, rhs, g);

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


cromossome genetic_algorithm(graph & g){

	cout << "Using " << seed << " as seed\n";

	srand(seed);
	aux_seed = seed;

	cromossome res;

	vector<cromossome> population = initialize_population(g, population_size);
	fitness(population, g);

	int it_limit = 500;
	int i = 0;


	while (i < 500){

		crossover(population[rand() % population_size],population[rand() % population_size],g);


		i++;
	}



	int a;
	cin >> a;

	//cout << endl << endl;

	return res;

}





map<list<int> *, int> find_solution_GA(graph & g){
	map<list<int> *, int> res;

	auto c = genetic_algorithm(g);

	int a;
	cin >> a;

	return res;

}






#endif