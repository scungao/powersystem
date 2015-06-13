#ifndef POWER_H
#define POWER_H

#include <iostream>
#include "converter.h"
#include <map>
#include <vector>

class power_grid : public converter {
 	int	size;
	table* symbol_table;
	vector<ast*> volts;
	vector<ast*> phasors;
	vector<ast*> x;
	vector<ast*> vhat;
	vector<ast*> thehat;
	vector<ast*> xhat;
	vector< vector<ast*> > zp; //the number of z depends on the graph strcture
	vector< vector<ast*> > zq;
	vector< vector<ast*> > zph; //z hats
	vector< vector<ast*> > zqh;
	map<int,set<int>*>	neighbors; 

	vector< vector<ast*> > ap;
	vector< vector<ast*> > aq;

	vector< vector<double> >	b;
	vector< vector<double> >	g;
	vector< vector<double> > sigma2; //p and q share sigma for now

	vector< vector<int> >	cluster_list;
	//vector<vector<int>> b;
	//vector<vector<int>> g;

public:
	power_grid(table*, int);
	void random_config(int);
	inline ~power_grid() {}
	inline int get_size() { return size; }
	void add_line(int i, int j);
	inline void add_line(map<int, int>& m) {
		for (map<int, int>::iterator it = m.begin(); it != m.end(); it++) {
			add_line(it->first, it->second);
		}  
	}
	inline void set_b(int i, int j, double a) { b[i][j] = a; b[j][i] = a; }
	inline void set_g(int i, int j, double a) { b[i][j] = a; b[j][i] = a; }
	inline double get_b(int i, int j) { return b[i][j]; }
	inline double get_g(int i, int j) { return g[i][j]; }

	inline int z_size() { 
		int num = 0; 
		for (map<int, set<int>*>::iterator it=neighbors.begin(); it!= neighbors.end(); it++ ) {
			num += it->second->size();
		}
		return num + 2*size ; //p,q for each line and each bus
	}

	ast* p(int i, vector<ast*>&, vector<ast*>&); //power injection P at i 
	ast* q(int i, vector<ast*>&, vector<ast*>&); //power injection Q at i
	ast* p(int i, int j, vector<ast*>&, vector<ast*>&); //power flow P
	ast* q(int i, int j, vector<ast*>&, vector<ast*>&); //power flow Q

	ast* mf();
	ast* mf(int, int, bool);
	ast* mf(int);
	ast* est();
	ast* esth(); //estimator with z hat
	ast* esth(int, int);
	ast* attack();
	ast* attack(int, int, bool);
	ast* monitor(double);
	ast* monitor(double, int, int, bool);
	ast* unsafe(double);
	ast* unsafe(double, int, int);
	ast* fdi(int,int,double,double);
	ast* fdi(int,double,double);

	ast* cegar(int, int);
	void dump();

};


#endif