#ifndef GA
#define GA

#include <bits/stdc++.h>

#include "union-find.cpp"

using namespace std;

float epsilon = 1.0;


struct cromossome{
	vector<int> sol;
	vector<float> demand;
	float cost;
	float fitness;
	bool feasible;

	cromossome(){ }

	cromossome(vector<int> s, vector<float> d, float c, floa fi, bool f = true){
		sol = s;
		demand = d;
		cost = c;
		fitness = fi;
		feasible = f;

	}
};


float sol_cost(cromossome & c, graph & g){

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

	return dist + penalty*epsilon;

	//return res;


}



cromossome genetic_algorithm(graph & g){

	cromossome res;



	return res;

}





map<list<int> *, int> find_solution_GA(graph & g){


}






#endif