#include <bits/stdc++.h>
#include <dirent.h>

#include "vrp.cpp"

using namespace std;



int main(int argc, char const *argv[]){


	DIR *dir;
	struct dirent *ent;


	ifstream config("./config.txt");
	string instances_folder;
	string solution_folder;
	string trash;

	config >> trash >> instances_folder;
	config >> trash >> solution_folder;



	if ((dir = opendir (instances_folder.c_str())) != NULL) {
		cout << "Input folder: " << instances_folder << endl;

		cout << "Choose the algorithm: \n";
		cout << " Heuristic \n";
		cout << "  1 - Clarke & Wright \n";
		cout << " Exact \n";
		cout << "  * <none> \n";
		cout << " Meta-heuristic \n";
		cout << "  * <none> \n";
		cout << endl;

		int option;
		cin >> option;

		if (option == 1){
			cout << "Using Clarke & Wright Heuristic\n";
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
		    	}

		    	auto end = chrono::steady_clock::now();

		    	float elapsed_time = (std::chrono::duration <double, std::milli> (end-start).count());
		    	cout << " " << file << " (" << elapsed_time <<"ms)\n";

		    	save_solution(instance, res, solution_folder);

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