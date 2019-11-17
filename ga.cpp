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
	float cost;
	float fitness;
	bool feasible;
	bool mutation;

	cromossome(){ 
		mutation = false;


		sol.resize(100);
	}

	cromossome(vector<int> s, float c, float fi, bool f = true, bool m = false){
		sol.resize(100);
		
		sol = s;
		cost = c;
		fitness = fi;
		feasible = f;
		mutation = m;

	}

	cromossome(const cromossome & c){
		sol.resize(100);
		sol = c.sol;
		cost = c.cost;
		fitness = c.fitness;
		feasible = c.feasible;
		mutation = c.mutation;
	}
};


typedef vector<cromossome *> pop_t;
pop_t population;

pop_t sel;




float fitness_function(float cost){
	return 1.0/cost;
}

void calcule_fitness(cromossome * c, graph & g){

	float res = 0.0;

	float dist = 0.0;	

	dist += g.matrix[1][c->sol[0]];

	for (int i = 0; i < c->sol.size()-1; i++){
		dist+= g.matrix[c->sol[i]][c->sol[i+1]];
	}

	dist += g.matrix[c->sol[c->sol.size()-1]][1];

	c->cost = dist;


	int vehicle = 0;
	vector<float> demands;

	float local_demand = 0.0;

	float penalty = 0.0;

	for (int i = 0; i < c->sol.size(); i++){
		//cout << local_demand << endl;
		if (c->sol[i] == 1 or i == c->sol.size()-1){
			//cout << local_demand << endl;

			if (local_demand > g.capacity){
				penalty += local_demand - g.capacity;
				//cout << penalty << endl ;

			}

			demands.push_back(local_demand);
			local_demand = 0;
		} else {
			local_demand += g.nodes[c->sol[i]].demand;
		}
	}

	//c->demand = demands;

	if (penalty > 0.0)
		c->feasible = false;
	else
		c->feasible = true;

	c->fitness = fitness_function(dist + penalty*epsilon);
	c->mutation = false;

}

void fitness(graph & g){
	for (auto & e : population)
		calcule_fitness(e, g);
}



void selection(graph & g){
	sel.clear();

	int n = population.size();


	for (int i = 0; i < selection_size; i++){
		sel.push_back(population[rand() % n]);
		sel.push_back(population[rand() % n]);
	}


}

vector<int> head_tail(const vector<int> & head, const vector<int> & tail, int cut_point){
	
	cout << "a.-1\n";

	vector<int> res;
	res.reserve(100);

	int i = 0;

	vector<int> ctrl(max(head.size(), tail.size()) + 1, 0);
	vector<int> use_after;
	use_after.reserve(100);

	cout << "a.0\n";


	while (i < cut_point){

		int x = head[i];
		res.push_back(x);

		ctrl[head[i]] = -1; // proibido

		if (ctrl[tail[i]] == 0){

			ctrl[tail[i]] = 1; // permitido


			if (tail[i] != 1)
				use_after.push_back(tail[i]);
		}


		i++;
	}

	ctrl[1] = 0;

	cout << "a.1\n";


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

	cout << "a.2\n";


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

	cout << "a.3\n";

	// caso sobre algum valor

	for (int i = 0; i < res.size(); ){
		if (res[i] == 0){
			res.erase(res.begin() + i);
		} else{
			i++;
		}
	}

	cout << "a.4\n";



	return res;
}

void one_point_crossover(cromossome * lhs, cromossome * rhs, graph & g){

	cout << "a\n";
	int n = min(lhs->sol.size(), rhs->sol.size());

	int point = rand() % n;

	vector<int> a;
	a.reserve(100);
	a = head_tail(rhs->sol, lhs->sol, point);

	cout << "aaaa\n";
	vector<int> b;
	b.reserve(100);
	b = head_tail(lhs->sol, rhs->sol, point);
	cout << "b\n";


	cromossome * c_a = new cromossome;

	cout << "b.0\n";

	cromossome * c_b = new cromossome;
	cout << "b.1\n";
	c_a->sol = a;

/*
	c_a->sol.resize(a.size());
	for (int i = 0; i < a.size(); i++)
		c_a->sol[i] = a[i];*/
	
	cout << "b.2\n";

	c_b->sol = b;
/*
	c_b->sol.resize(b.size());
	for (int i = 0; i < b.size(); i++)
		c_b->sol[i] = b[i];
	*/
	cout << "b.3\n";


	cout << "c\n";


	calcule_fitness(c_a, g);
	calcule_fitness(c_b, g);

	cout << "d\n";


	population.push_back(c_a);
	population.push_back(c_b);

	cout << "e\n";



}



void crossover(cromossome * lhs, cromossome * rhs, graph & g, int crossover_type = 1){

	if (crossover_type == 1)
		one_point_crossover(lhs, rhs, g);

}



void crossover(graph & g){


	for (int i = 0; i < sel.size(); i += 2){
		crossover(sel[i], sel[i+1], g);
	}

}


void initialize_population(graph & g, int pop_size){


	for (int i = 0; i < pop_size; ++i) {
		cromossome * c = new cromossome;
		vector<int> sol;

		for (int j = 2; j < g.dimension; ++j) {
			sol.push_back(j);
		}

		aux_seed = rand();
		srand(aux_seed);

		shuffle(sol.begin(), sol.end(), default_random_engine(aux_seed) );

		c->sol = sol;
		population.push_back(c);

	}
}

void make_feasible(cromossome * c, graph & g){

	int n = c->sol.size();

	float local_demand = 0.0;

	//float penalty = 0.0;

	int a = 0;
	int b = n;

	for (int i = 0; i < n; i++){
		if (c->sol[i] == 1 or i == n-1){

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

			local_demand += g.nodes[c->sol[i]].demand;
		}
	}

	int r = rand() % (b-a-1);
	c->sol.insert(c->sol.begin() + a + r, 1);

}

void mutation(cromossome * c, graph & g){

	int n = c->sol.size();

	int a = rand() % n;
	int b = a;

	while (b == a)
		b = rand() % n;

	int v_a = c->sol[a];
	int v_b = c->sol[b];

	c->sol[a] = v_b;
	c->sol[b] = v_a;

}

void mutation(graph & g){

	int n = population.size();
	int q = n * mut_percent;

	for(int i = 0; i < q; i++){
		int c_i = rand() % n;
		cromossome * c = population[c_i];

		if (!c->mutation){
			if (c->feasible)
				mutation(c,g);
			else
				make_feasible(c,g);


			c->mutation = true;
			calcule_fitness(c,g);
		}

	}

}


bool comp(cromossome * lhs, cromossome * rhs){
	return lhs->fitness > rhs->fitness;
}

cromossome genetic_algorithm(graph & g){

	cout << "Using " << seed << " as seed\n";

	srand(seed);
	aux_seed = seed;

	cromossome res;

	initialize_population(g, population_size);
	fitness(g);

	int it_limit = 500;
	int i = 0;



	while (i < 200){

		
		//crossover(population[rand() % population_size],population[rand() % population_size],g);
		cout << "SEL\n";
		sel.clear();
		selection(g);

		cout << "CROSS\n";
		crossover(g);
/*
		cout << "MUT\n";
		mutation(population, g);

		cout << "SORT\n";
		sort(population.begin(), population.end(), comp);
		//population.resize(total_pop_size);
*/
		cout << "END\n";
		cout << i << endl;
		i++;
	}

	for (auto & e: population[0]->sol)
		cout << e << " ";
	cout << endl;

	cout << population[0]->cost << endl;
	
	int k;
	cin >> k;

	//cout << endl << endl;
	//population.clear();
	return res;

}






map<list<int> *, int> find_solution_GA(graph & g){
	map<list<int> *, int> res;

	auto c = genetic_algorithm(g);


	population.clear();

	return res;

}






#endif