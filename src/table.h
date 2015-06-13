#ifndef TABLE_H
#define TABLE_H

#include "ast.h"
#include <string>
#include <vector>
#include <assert.h>
#include <map>

using namespace std;

class table {
	map<string, symbol*>	symbol_table;
	map<string, ast*>		label_table; //only keeps top-level ast
	vector<int>	variable_id_list;
	vector<int>	constant_id_list; //faster access
	vector<int> process_id_list;
public:
	table();
	~table();
	inline unsigned get_size() { return symbol_table.size();}
	inline void add(symbol* s) 
		{ symbol_table.insert(pair<string,symbol*>(s->get_name(), s)); }
	inline void add_label(string s, ast* a)
		{ label_table.insert(pair<string, ast*>(s, a)); }	
	void update_label(string s, ast* a);
	symbol*	locate_symbol(string);
	ast* locate_label(string);

	//node*	locate_node(string);
	//ast*	locate_ast(string);
};

#endif