#ifndef GA
#define GA

#include <bits/stdc++.h>

#include "union-find.cpp"

using namespace std;

float epsilon = 1.0;
int population_size = 40;



unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
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
	return 1/cost;
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

void fitness(vector<cromossome> population, graph & g){
	for (auto & e : population)
		calcule_fitness(e, g);
}


vector<cromossome> initialize_population(graph & g, int pop_size){
	vector<cromossome> population;

	for (int i = 0; i < pop_size; ++i) {
		cromossome c;
		vector<int> sol;

		for (int j = 2; j < g.dimension; ++j) {
			sol.push_back(j);
		}

		aux_seed = (unsigned) default_random_engine(aux_seed);

		shuffle(sol.begin(), sol.end(), default_random_engine(aux_seed) );

		c.sol = sol;


		for (auto & e : c.sol) {
			cout << e << " ";
		}
		cout << endl;
	}


	return population;
}


cromossome genetic_algorithm(graph & g){

	cout << "Using " << seed << " as seed\n";

	cromossome res;

	vector<cromossome> population = initialize_population(g, population_size);

	cout << endl << endl;

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