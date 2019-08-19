#include <bits/stdc++.h>
#include <dirent.h>

#include "vrp.cpp"

using namespace std;


int main(){


	DIR *dir;
	struct dirent *ent;

	string instances_folder = "./instances/A/cases/";
	string solution_folder = "./solutions/A/";

	
	if ((dir = opendir (instances_folder.c_str())) != NULL) {
	  while ((ent = readdir (dir)) != NULL) {
	  	if (ent->d_name[0] != '.'){

	  		string file = ent->d_name;
	  		string full_path = instances_folder + file;

	    	auto instance = load_graph(full_path);

	    	//print_graph(instance);
	    	auto x = find_solution_CW(instance);

	  	}
	  		
	  }
		closedir (dir);
	} else {
		perror ("");
		return EXIT_FAILURE;
	}
	
	return 0;
}