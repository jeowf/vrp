#ifndef GA
#define GA

#include <bits/stdc++.h>

#include "union-find.cpp"

using namespace std;

struct cromossome{
	vector<int> sol;
	float demand;
	float cost;
	float fitness;
	bool feasible;

	cromossome(){ }

	cromossome(vector<int> s, float d, float c, floa fi, bool f = true){
		sol = s;
		demand = d;
		cost = c;
		fitness = fi;
		feasible = f;

	}
};


float sol_cost(cromossome & c, graph & g){

	float res = 0;
	

	res += g.matrix[1][c.sol[0]];

	for (int i = 0; i < c.sol.size()-1; i++){
		res+= g.matrix[c.sol[i]][c.sol[i+1]];
	}

	res += g.matrix[c.sol[c.sol.size()-1]][1];

	return res;


}



cromossome genetic_algorithm(graph & g){

	cromossome res;



	return res;

}





map<list<int> *, int> find_solution_GA(graph & g){


}






#endif