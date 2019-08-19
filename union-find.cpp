#ifndef UNION_FIND
#define UNION_FIND

#include <bits/stdc++.h>

using namespace std;


class disjoint_set{
	private:
	struct subset{
		int parent; 
    	int rank; 

    	subset(){}

    	subset(int p, int r){
    		parent = p;
    		rank = r;
    	}
	};


	subset * subsets;


	public:
	disjoint_set(int n){
		subsets = new subset[n];
		for (int i = 0; i < n;i++){
			subset s(i, 0);
			subsets[i] = s;
		}
	}

	~disjoint_set(){
		delete [] subsets;
	}


	void make_set(int i){
		subset s(i,0);
		subsets[i] = s;
	}

	int find(int i) { 
	    if (subsets[i].parent != i) 
	        subsets[i].parent = find(subsets[i].parent); 
	  
	    return subsets[i].parent; 
	} 
	  

	// por algum motivo union é palavra reservada, entao vai uni
	void uni(int x, int y) 
	{ 
	    int xroot = find(x); 
	    int yroot = find(y); 
	  

	    if (subsets[xroot].rank < subsets[yroot].rank) {
	        subsets[xroot].parent = yroot; 
	    }
	    else if (subsets[xroot].rank > subsets[yroot].rank) {
	        subsets[yroot].parent = xroot; 
	    } else { 

	        subsets[yroot].parent = xroot; 
	        subsets[xroot].rank++; 
	    } 
	} 
};


#endif