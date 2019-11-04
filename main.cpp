#include <bits/stdc++.h>
#include <dirent.h>

#include "vrp.cpp"
#include "ga.cpp"

using namespace std;



int main(int argc, char const *argv[]){


	DIR *dir;
	struct dirent *ent;

	DIR *dir2;
	struct dirent *ent2;


	ifstream config("./config.txt");
	string instances_folder;
	string solution_folder;
	string opt_folder;
	string trash;

	config >> trash >> instances_folder;
	config >> trash >> solution_folder;
	config >> trash >> opt_folder;


	if ((dir = opendir (instances_folder.c_str())) != NULL) {
		cout << "Input folder: " << instances_folder << endl;

		cout << "Choose the algorithm: \n";
		cout << " Heuristic \n";
		cout << "  1 - Clarke & Wright \n";
		cout << " Exact \n";
		cout << "  2 - Backtracking \n";
		cout << "  3 - Branch and Bound \n";
		cout << " Meta-heuristic \n";
		cout << "  4 - Genetic Algorithm \n";
		cout << "  5 - Tabu Search \n";
		cout << endl;

		float time_limit = 0.0;

		int option;
		cin >> option;

		if (option == 1){
			cout << "Using Clarke & Wright Heuristic\n";
		} else if (option == 2){
			cout << "Using Backtracking\n";
		} else if (option == 3){
			cout << "Using Branch and Bound \n";
		} else if (option == 4){
			cout << "Using Genetic Algorithm \n";
		} else {
			cout << "ERROR: invalid option.\n";
			return EXIT_FAILURE;
		} 


	  	while ((ent = readdir (dir)) != NULL) {
	  		if (ent->d_name[0] != '.'){

		  		string file = ent->d_name;
		  		string full_path = instances_folder + file;

		    	auto instance = load_graph(full_path);

		    	map<list<int> *, int> res;

		    	auto start = chrono::steady_clock::now();

		    	if (option == 1){
		    		res = find_solution_CW(instance);
		    	} else if (option == 2) {
		    		res = find_solution_backtracking(instance);
		    	} else if (option == 3){
		    		
		    		res = find_solution_bb(instance);

		    	} else if (option == 4){

		    		res = find_solution_GA(instance);

		    	}

		    	auto end = chrono::steady_clock::now();

		    	float elapsed_time = (std::chrono::duration <double, std::milli> (end-start).count());
		    	cout << file << " (" << elapsed_time <<"ms)\n";

		    	save_solution(instance, res, solution_folder);

		    	cout << "Solution generated:\n";
		    	print_solution(instance, res);

		    	cout << "Optimal Solution:\n";

		    	ifstream opt(opt_folder+instance.name);

		    	string content( (std::istreambuf_iterator<char>(opt) ),
                       (std::istreambuf_iterator<char>()    ) );
		    	cout << content;
		    	if (content.size() == 0)
		    		cout << "There's no optimal solution file\n";
		    	cout << "-----------------------\n";

		  	}
		  		
	  	}
		closedir (dir);

	} else {
		//perror ("");
		cout << "ERROR: invalid folder.\n";
		return EXIT_FAILURE;
	}
	
	return 0;
}