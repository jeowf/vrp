#ifndef TABU
#define TABU

#include <bits/stdc++.h>

#include "union-find.cpp"

using namespace std;
















map<list<int> *, int> find_solution_tabu(graph & g){
	map<list<int> *, int> res;

	auto cw = find_solution_CW(g);

	vector<int> path;
	path.push_back(1);

	for (auto & e : cw){
		if (!e.first->empty()){
			for (auto & f : *e.first)
				path.push_back(f);
			path.push_back(1);

		}
	}

	// Apaga isso dps \/
	for (auto & e : path)
		cout << e << " ";
	cout << endl;
	// apaga /\

	return res;

}


#endif