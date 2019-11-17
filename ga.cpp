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

	int i;
	int e;

	~cromossome(){}

	cromossome(){ 
		mutation = false;

		i = e = 0;
	}

	cromossome(vector<int> s, float c, float fi, bool f = true, bool m = false){
		
		sol = s;
		cost = c;
		fitness = fi;
		feasible = f;
		mutation = m;

		i = e = 0;

	}

	cromossome(const cromossome & c){
		sol = c.sol;
		cost = c.cost;
		fitness = c.fitness;
		feasible = c.feasible;
		mutation = c.mutation;
		i = c.i;
		e = c.e;
	}

};

bool comp(cromossome lhs, cromossome rhs){
	return lhs.fitness > rhs.fitness;
}



struct ga{

	graph g;

	typedef vector<cromossome> pop_t;
	pop_t population;

	pop_t sel;

	ga(graph & h){
		g = h;
	}

	~ga(){ }

	float fitness_function(float cost){
		return 1.0/cost;
	}

	void calcule_fitness(cromossome & c){

		float res = 0.0;

		float dist = 0.0;	

		dist += g.matrix[1][c.sol[0]];

		for (int i = 0; i < c.sol.size()-1; i++){
			dist+= g.matrix[c.sol[i]][c.sol[i+1]];
		}

		dist += g.matrix[c.sol[c.sol.size()-1]][1];

		c.cost = dist;


		int vehicle = 0;
		//vector<float> demands;

		float local_demand = 0.0;

		float penalty = 0.0;

		int x_i = 0;
		int x_e = 0;

		int a_i = 0;
		int a_e = 0;

		for (int i = 0; i < c.sol.size(); i++){

			if (c.sol[i] == 1 or i == c.sol.size()-1){

				if (i == c.sol.size()-1){
					a_e = i + 1;
				} else {
					a_e = i;
				}

				if (local_demand > g.capacity){
					penalty += local_demand - g.capacity;

					x_i = a_i;
					x_e = a_e;

				}


				local_demand = 0;

				a_i = i+1;

			} else {
				local_demand += g.nodes[c.sol[i]].demand;
			}
		}

		//c.demand = demands;

		if (penalty > 0.0){
			c.feasible = false;
			c.i = x_i;
			c.e = x_e;
		} else{
			c.feasible = true;

		}

		c.fitness = fitness_function(dist + penalty*epsilon);
		c.mutation = false;

	}

	void fitness(){
		for (auto & e : population)
			calcule_fitness(e);
	}


	void selection(){
		sel.clear();

		int n = population.size();

		for (int i = 0; i < selection_size; i++){
			cromossome a = population[rand() % n];
			cromossome b = population[rand() % n];
			sel.push_back(a);
			sel.push_back(b);
		}


	}

	void make_feasible(cromossome & c){

		bool fe = c.feasible;
/*
		for (auto & e : c.sol)
			cout << e << " ";
		cout << endl;
*/
		while(!fe){
			vector<int> ve;

			int d =  c.e - c.i - 2;
			int r = 0;

			if (d > 0)
				r = rand() & d;
			//cout << "r :" << r << endl;

			int i = 0;
			for (; i <= c.i + r; i++){
				ve.push_back(c.sol[i]);
				//cout << c.sol[i] <<" ";
			}
			ve.push_back(1);
			//cout << "1 ";
			for(;i<c.sol.size(); i++){
				ve.push_back(c.sol[i]);
				//cout << c.sol[i] <<" ";

			}
			//cout << endl;

			c.sol = ve;
/*
			for (auto & e : c.sol)
				cout << e << " ";
			cout << endl;
*/
			calcule_fitness(c);
			fe = c.feasible;
		}

		//int x;
		//cin >> x;
	}


	void clean_sol(cromossome & c){
		vector<int> cl;
		int last_e = 1;
		for (int i = 0; i < c.sol.size(); i++){

			if (c.sol[i] == 1 ){
				if (last_e != 1){
					last_e = c.sol[i];
					cl.push_back(last_e);
				}

			} else {
				last_e = c.sol[i];
				cl.push_back(last_e);
			}
		}

		if (cl.back() == 1)
			cl.pop_back();

		c.sol = cl;



	}


	void clean_pop(){
		for (auto & e : population){
			clean_sol(e);
		}
	}

	vector<int> head_tail(vector<int> & head, vector<int> & tail, int cut_point){
		

		vector<int> res;
		
		map<int, bool> dont;

		int i = 0;

		while (i < cut_point){
			res.push_back(head[i]);
			dont[head[i]] = true;
			i++;
		}


		while (i < tail.size()){
			res.push_back(tail[i]);
			//dont[tail[i]] = true;
			i++;
		}

		dont[1] = false;
/*
		for(int j = 2; j < g.dimension; j++){
			cout << j << "(" << dont[j] << "),";
		}
		cout << endl;*/
		//cout << "cut : " << cut_point << endl;

		int j = 0;
		for (i = cut_point; i < res.size(); i++){

			if (res[i] > 1 and dont[res[i]]){
				//cout << res[i] << endl;
				int v = 1;
				for (; j < cut_point; j++){

					if ( tail[j] > 1 and !dont[tail[j]] ){
						//cout << "subs " << tail[j] << endl;
						v = tail[j];
						dont[tail[j]] = true;
						break;
					}
				}

				res[i] = v;

			} else {
				dont[res[i]] = true;
			}

		}

		vector<int> veacto;
		dont[1] = true;
		i = cut_point;

		for (; j < cut_point; j++){
			
			if ( tail[j] > 1 and !dont[tail[j]] ){

				for (; i < res.size(); i++){
					if (res[i] == 1){

						res[i] = tail[j];

						dont[tail[j]] = true;
						break;

					}
				}
				
			}
		}



		///vector<int> missing;

		for (i = 2; i < g.dimension; i++){
			if (!dont[i]){
				for (int j = 0; j < res.size(); j++){
					if (res[j] == 1){
						res[j] = i;
						dont[i] = true;
					}
				
				}
			}
			
		}


		/*cout << "missing: ";
		for (int i = 2; i < g.dimension; i++){
			if (!dont[i]){
				cout << i << " ";
			}
		}

		cout << endl;
*/

		return res;
	}

	void one_point_crossover(cromossome & lhs, cromossome & rhs){

		/*
		cout << "lhs : ";

		for (auto & e : lhs.sol)
			cout << e << " ";
		cout << "--" << lhs.sol.size() << endl;
		

		cout << "rhs : ";

		for (auto & e : rhs.sol)
			cout << e << " ";
		cout << "--" << rhs.sol.size() << endl;
		*/
		int n = min(lhs.sol.size(), rhs.sol.size());

		int point = rand() % n;

		cromossome c_a;
		vector<int> a = head_tail(rhs.sol, lhs.sol, point);
		c_a.sol = a;

		cromossome c_b;
		vector<int> b = head_tail(lhs.sol, rhs.sol, point);
		c_b.sol = b;


		calcule_fitness(c_a);
		calcule_fitness(c_b);

		/*
		cout << "c_a : ";

		for (auto & e : c_a.sol)
			cout << e << " ";
		cout << "--" << c_a.sol.size() << endl;
		

		cout << "c_b : ";

		for (auto & e : c_b.sol)
			cout << e << " ";
		cout << "--" << c_b.sol.size() << endl;
		*/
		make_feasible(c_a);
		make_feasible(c_b);
/*
		cout << "c_a : ";

		for (auto & e : c_a.sol)
			cout << e << " ";
		cout << "--" << c_a.sol.size() << endl;
		

		cout << "c_b : ";

		for (auto & e : c_b.sol)
			cout << e << " ";
		cout << "--" << c_b.sol.size() << endl;
*/
		population.push_back(c_a);
		population.push_back(c_b);

	}


	void crossover(){

		int x = 0;
		for (int i = 0; i < sel.size(); i += 2){
			one_point_crossover(sel[i], sel[i+1]);
			//cin >> x;
		}
	}


	void initialize_population(int pop_size){

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


			calcule_fitness(c);

			make_feasible(c);

			population.push_back(c);

		}
	}

	

	void execute(){

		cout << "Using " << seed << " as seed\n";

		srand(seed);
		aux_seed = seed;

		cromossome res;

		initialize_population(population_size);
		//fitness();

		int it_limit = 500;
		int i = 0;
		

		// for (auto & v : population){
		// 	for (auto & e : v.sol){
		// 		cout << e << " ";
		// 	}
		// 	cout << endl;
		// }

		/*

		vector<cromossome> sel;

		vector<int> a = { 2, 3, 1, 5, 6, 1, 4, 7};
		vector<int> b = { 7, 1, 2, 1, 5, 6, 1, 3, 4 };
		cromossome c_a;
		cromossome c_b;

		c_a.sol = a;
		c_b.sol = b;

		one_point_crossover(c_a,c_b);
		//mutation(population, g);
*/
		


		while (i < 200){

			
			//crossover(population[rand() % population_size],population[rand() % population_size],g);
			//cout << "SEL\n";
			selection();

			//cout << "CROSS\n";
			crossover();

			clean_pop();

			sort(population.begin(), population.end(), comp);
			population.resize(total_pop_size);


			i++;
		}



		for (auto & e: population[0].sol)
			cout << e << " ";
		cout << endl;

		cout << population[0].cost << endl;

		if (!population[0].feasible){
			cout << population[0].i << ", " << population[0].e << endl;
		}
		
		//int k;
		//cin >> k;

		//cout << endl << endl;
		//population.clear();
	}



};



map<list<int> *, int> find_solution_GA(graph & g){
	map<list<int> *, int> res;

	ga gen(g);
	gen.execute();

	int a;
	//cin >> a;

	return res;

}






#endif