#ifndef TABU
#define TABU

#include <bits/stdc++.h>

#include "union-find.cpp"

using namespace std;

#define tabu_ternure 5    //tabu list size

struct tabu{
  int x;
  int pos_x;
  int y;
  int pos_y;
};

typedef int db;
typedef vector<db> vd;
typedef vector<vector<db> > vvd;
typedef map<db,vd> mvd;
typedef vector<tabu> vt;
typedef set<vector<int> > svd;

int is_tabu_move(int x, int y, int pos_x, int pos_y);
void add_tabu_nodes(int x, int y, int pos_x, int posy);
db neighbor_cost(graph & g, vd path);
vd exchange(int i, int j, vd &n);
void insertion(int i, int j, db a, vd &n);
void get_neighbor(int i, int &it, int &flag, db &int_best_cost,graph & g);


vd path;             //local solution
vt tabu_list;        //list of tabú movements
mvd div_list;        //map that  contain the best 200 solution.
svd div_tabu;        //set with before taken solution.
vd best_sol;         //global solution
db best_cost=1<<30;  //global solution cost

map<list<int> *, int> find_solution_tabu(graph & g){

  path.clear();
  tabu_list.clear();
  div_list.clear();
  div_tabu.clear();
  best_sol.clear();

  best_cost=1<<30;

  int best_it=0;
  int max_it=100;
  int i=0;
  int flag=1;

    list<int> * final_path;

	map<list<int> *, int> res;

	auto cw = find_solution_CW(g);

	path.push_back(1);

	for (auto & e : cw){
		if (!e.first->empty()){
			for (auto & f : *e.first)
				path.push_back(f);
			path.push_back(1);

		}
	}

    float total_cost = 0;

	for (auto & x : cw){
		if (x.first->size() > 0){
			float d = 0;
			vector<int> gamb;
			gamb.push_back(1);

			for (auto & p : *x.first){
				d += g.nodes[p].demand;
				gamb.push_back(p);
			}

			gamb.push_back(1);

			float co = 0;

			for (int r = 0; r < gamb.size()-1; r++){
				co += g.matrix[gamb[r]][gamb[r+1] ];
			}

			total_cost += co;
		}
		
	}


	best_cost = total_cost;
	best_sol = path;



  while(i++ < max_it){
    db int_best_cost;
    if(div_list.size()>0){
      path = div_list.begin()->second;
      div_list.erase(div_list.begin());
    }

    flag=1;
    while(flag){
      div_tabu.insert(path);
      int_best_cost=neighbor_cost(g, path);
      get_neighbor(i,best_it,flag,int_best_cost,g);
    }
  }

  for (auto i = best_sol.begin(); i != best_sol.end(); i++){ 
    	final_path->push_back(*i);
    }

  res[final_path] = 0;

  return res;

}

void add_tabu_nodes(int x, int y, int pos_x, int pos_y){
  tabu tmp;
  tmp.x = x;
  tmp.y = y;
  tmp.pos_x = pos_x;
  tmp.pos_y = pos_y;
  if(tabu_list.size() < tabu_ternure){
    tabu_list.emplace(tabu_list.begin(),tmp);
  }
  else{
    tabu_list.pop_back();
    tabu_list.emplace(tabu_list.begin(),tmp);
  }
}

//return 0 if is not valid solution otherwise return cost solution
db neighbor_cost(graph & g, vd aux){

	db carga = 0;
	float custo = 0;

  for(db i= 0; i< g.dimension; i++){
    int to = aux[i];
    if(carga > g.capacity){
      return 0;
    }
    if(to == 1){
      carga = 0;
    }
    else{
      int from = aux[i-1];
      carga += g.nodes[to - 1].demand;
      custo += g.matrix[from-1][to-1];
      }
  }
  return custo;
}

//insertion operator for neighbor selection
void insertion(int i, int j, db a, vd &n){
  n.erase(n.begin()+i);
  n.emplace(n.begin()+j,a);
}

//exchange operator for neighbor selection
vd exchange(int i, int j, vd &n){
  db tmp = n[i];
  n[i] = n[j];
  n[j] = tmp;
  return n;
}

//eval move and return 1 if is tabu move otherwise return 0
int is_tabu_move(int x, int y, int pos_x, int pos_y){
  for(auto i : tabu_list){
    if( ((i.x==x && i.y==y) || (i.x==y && i.y==x)) &&
        ((i.pos_x==pos_x && i.pos_y==pos_y) || (i.pos_x==pos_y && i.pos_y==pos_x))) return 1;
  }
  return 0;
}

//generate all neighbors and evuale each of them
void get_neighbor(int i, int &it, int &flag,db &int_best_cost, graph & g){
  db local_best_cost=1<<30;
  db x=0,y=0;                
  int pos_x=0, pos_y=0;      
  vector<db>best_neighbor;
  int l=1;

  //exchange all positions evaluating and choosing best neigbor
  for(db i=0; i<path.size(); i++){
    for(db j=0; j<path.size();j++){
      vector<db> tmp = path;
      db a = tmp[i];
      db b = tmp[j];
      if( a!=1 && b!=1 && a!=b){

      	vd teste = exchange(i,j,tmp);

        db tmp_cost = neighbor_cost(g, teste); 
        

        if(!is_tabu_move(a,b,i,j)){
          if(tmp_cost < local_best_cost && tmp_cost!=0){
            local_best_cost = tmp_cost;
            best_neighbor = tmp;
            x=a,y=b;
            pos_x=i, pos_y=j;
          }
        }
        // else if(tmp_cost < local_best_cost){
        //   local_best_cost = tmp_cost;
        //   best_neighbor = tmp;
        //   x=a,y=b;
        //   pos_x=i, pos_y=j;
        // }
      }
    }
  }
  if(local_best_cost < best_cost){
    best_cost = local_best_cost;
    best_sol = best_neighbor;
    it = i;
  }
  if(local_best_cost < int_best_cost){
    path = best_neighbor;
    add_tabu_nodes(x,y,pos_x,pos_y);
  }else {
    flag=0;
  };

}

#endif