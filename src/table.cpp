#include "table.h"

table::table() {
	add(new symbol("+", sfunction, 2));
	add(new symbol("-", sfunction, 2));
	add(new symbol("*", sfunction, 2));
	add(new symbol("/", sfunction, 2));
	add(new symbol("^", sfunction, 2));
	add(new symbol("sqrt", sfunction, 1));
	add(new symbol("sin", sfunction, 1));
	add(new symbol("cos", sfunction, 1));
	add(new symbol("tan", sfunction, 1));

	add(new symbol("and", connective));
	add(new symbol("or", connective));
	add(new symbol("not", connective, 1));
	add(new symbol("implies", connective, 2));

	add(new symbol(">", relation, 2));
	add(new symbol("<", relation, 2));
	add(new symbol(">=", relation, 2));
	add(new symbol("<=", relation, 2));
	add(new symbol("=", relation, 2));
	add(new symbol("true", relation, 0));
	add(new symbol("false", relation, 0));

	add(new symbol("(", aux, 0));
	add(new symbol(")", aux, 0));

	add(new symbol("d/dt", sfunction, 1));

	add(new symbol("ite", pconnective, 3));
	add(new symbol("seq", pconnective, 10000));

	add(new symbol("0", constant, 0));
	add(new symbol("1", constant, 0));
}


table::~table() {
	map<string, symbol*>::iterator loc_s;
	//map<string, ast*>::iterator loc_a;
	for (loc_s = symbol_table.begin(); loc_s!= symbol_table.end(); loc_s++) {
		delete loc_s->second;
	}
	//for (loc_a = ast_map.begin(); loc_a!= ast_map.end(); loc_a++) {
	//	delete loc_a->second;
	//}
	symbol_table.clear();
}

symbol* table::locate_symbol(string name) {
	map<string, symbol*>::iterator	loc;
	loc = symbol_table.find(name);
	if (loc!=symbol_table.end()) 
		return loc -> second;
	else 
		return NULL;
}

ast* table::locate_label(string name) {
	map<string, ast*>::iterator	loc;
	loc = label_table.find(name);
	if (loc!=label_table.end()) 
		return loc -> second;
	else 
		return NULL;
}

void table::update_label(string s, ast* a) {
	map<string, ast*>::iterator	loc;
	loc = label_table.find(s);
	assert(loc!= label_table.end());
	loc->second = a;
}



