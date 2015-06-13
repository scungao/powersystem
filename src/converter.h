#ifndef CONVERTER_H
#define CONVERTER_H

#include "table.h"

class converter {
	table*	symbol_table;
public:
	inline converter() {}
	inline converter(table* t):
		symbol_table(t) {}
	inline ~converter() {}
	inline table*	get_table() {return symbol_table;}
//	ast* replace(ast*, symbol*, symbol*); //substitute symbol by symbol
//	ast* replace(ast*, ast*, ast*);
//	ast* copy_replace(ast*, symbol*, symbol*);
//	ast* copy_replace(ast*, ast*, ast*);
//	ast* replace(ast*, symbol*, ast*); //substitute symbol by ast

	inline void set_head_name(ast* a, string s) {
		symbol* ss = symbol_table->locate_symbol(s);
		a->set_head_symbol(ss);
	}

	ast* substitute(ast*, symbol*, symbol*); //need consecutive substitution

	ast* substitute(ast* source, ast* old, ast* neu) {
		return substitute(source, old->get_head_symbol(), neu->get_head_symbol());
	}

	inline ast* substitute(ast* source, map<ast*, ast*>& m) {
		map<ast*, ast*>::iterator it;
		for (it=m.begin(); it!=m.end(); it++) {
			source = substitute(source, it->first, it->second);
		}
		return source;
	}

	inline ast* substitute(ast* source, map<symbol*, symbol*>& m) {
		map<symbol*, symbol*>::iterator it;
		for (it=m.begin(); it!=m.end(); it++) {
			source = substitute(source, it->first, it->second);
		}
		return source;
	}
	ast*	process_to_formula(ast*);
	ast*	dup(ast*);	
	bool	get_dreal_solutions(ast*, map<symbol*, symbol*>&, bool);
	bool	get_dreal_solutions(ast*, map<symbol*, symbol*>&, bool, double, string, int);
	void	simplify(ast*);//compress constants

	ast*	partial(ast*, symbol*);
	inline ast* partial(ast* a, ast* s) {
		return partial(a, s->get_head_symbol());
	}

	ast*	lyapunov(vector<ast*>&, vector<ast*>&, ast*);
	bool	cegis(ast*, vector<ast*>&, vector<ast*>&, map<symbol*, symbol*>&, double);
	
	inline ast*	add(ast* a1, ast* a2) { 
		ast* result = new ast(symbol_table->locate_symbol("+")); 
		result -> add_child(a1);
		result -> add_child(a2);
		return result;
	}

	inline ast*	sub(ast* a1, ast* a2) { 
		ast* result = new ast(symbol_table->locate_symbol("-")); 
		result -> add_child(a1);
		result -> add_child(a2);
		return result;
	}

	inline ast*	mul(ast* a1, ast* a2) { 
		ast* result = new ast(symbol_table->locate_symbol("*")); 
		result -> add_child(a1);
		result -> add_child(a2);
		return result;
	}
	
	inline ast*	div(ast* a1, ast* a2) { 
		ast* result = new ast(symbol_table->locate_symbol("/")); 
		result -> add_child(a1);
		result -> add_child(a2);
		return result;
	}

	inline ast*	sqrt(ast* a1) { 
		ast* result = new ast(symbol_table->locate_symbol("sqrt")); 
		result -> add_child(a1);
		return result;
	}

	inline ast*	sin(ast* a1) { 
		ast* result = new ast(symbol_table->locate_symbol("sin")); 
		result -> add_child(a1);
		return result;
	}	

	inline ast*	cos(ast* a1) { 
		ast* result = new ast(symbol_table->locate_symbol("cos")); 
		result -> add_child(a1);
		return result;
	}

	inline ast*	tan(ast* a1) { 
		ast* result = new ast(symbol_table->locate_symbol("tan")); 
		result -> add_child(a1);
		return result;
	}	

	inline ast*	pow(ast* a1, ast* a2) { 
		ast* result = new ast(symbol_table->locate_symbol("^")); 
		result -> add_child(a1);
		result -> add_child(a2);
		return result;
	}

	inline ast* pow2(ast* a) {
		return pow(a, num("2"));
	}

	inline ast* gt(ast* a1, ast* a2) {
		ast* result = new ast(symbol_table->locate_symbol(">"));
		result -> add_child(a1);
		result -> add_child(a2);
		return result;
	} 

	inline ast* lt(ast* a1, ast* a2) {
		ast* result = new ast(symbol_table->locate_symbol("<"));
		result -> add_child(a1);
		result -> add_child(a2);
		return result;
	} 


	inline ast* geq(ast* a1, ast* a2) {
		ast* result = new ast(symbol_table->locate_symbol(">="));
		result -> add_child(a1);
		result -> add_child(a2);
		return result;
	} 

	inline ast* leq(ast* a1, ast* a2) {
		ast* result = new ast(symbol_table->locate_symbol("<="));
		result -> add_child(a1);
		result -> add_child(a2);
		return result;
	} 

	inline ast* eq(ast* a1, ast* a2) {
		ast* result = new ast(symbol_table->locate_symbol("="));
		result -> add_child(a1);
		result -> add_child(a2);
		return result;
	} 

	inline ast*	lnot(ast* a1) { 
		ast* result = new ast(symbol_table->locate_symbol("not")); 
		result -> add_child(a1);
		return result;
	}

	inline ast* land(ast* a1, ast* a2) {
		ast* result = new ast(symbol_table->locate_symbol("and"));
		result -> add_child(a1);
		result -> add_child(a2);
		return result;
	} 

	inline ast* lor(ast* a1, ast* a2) {
		ast* result = new ast(symbol_table->locate_symbol("or"));
		result -> add_child(a1);
		result -> add_child(a2);
		return result;
	} 

	inline ast* limplies(ast* a1, ast* a2) {
		ast* result = new ast(symbol_table->locate_symbol("implies"));
		result -> add_child(a1);
		result -> add_child(a2);
		return result;
	} 

	inline ast* top() {
		ast* a = new ast(symbol_table->locate_symbol("true"));
		return a;		
	} 

	inline ast* bot() {
		ast* a = new ast(symbol_table->locate_symbol("false"));
		return a;		
	} 

	inline ast* num(string s) {
		symbol* new_num = symbol_table->locate_symbol(s);
		if (new_num == NULL){ 
			new_num = new symbol(s, constant, 0);
			symbol_table -> add(new_num);
		}
		ast* a = new ast(new_num);
		return a;
	}

	inline ast* num(double c) {
		string s = to_string(c);
		return num(s);
	}

	inline ast* parse_num(char* c) {
		string s(c);
		return num(s);
	}


	inline symbol* num_sym(string s) {
		symbol* new_num = symbol_table->locate_symbol(s);
		if (new_num == NULL){ 
			new_num = new symbol(s, constant, 0);
			symbol_table -> add(new_num);
		}
		return new_num;
	}

	inline symbol* num_sym(double c) {
		string s = to_string(c);
		symbol* new_num = symbol_table->locate_symbol(s);
		if (new_num == NULL){ 
			new_num = new symbol(s, constant, 0);
			symbol_table -> add(new_num);
		}
		return new_num;
	}

	inline ast* var(string s) {
		symbol* new_var = symbol_table->locate_symbol(s);
		if (new_var == NULL) {
			new_var = new symbol(s, variable, 0);
			symbol_table -> add(new_var);
		}
		ast* a = new ast(new_var);
		return a;
	}

	inline void mklabel(string s, ast* a) {
		ast* a_old = symbol_table->locate_label(s);
		if ( a_old == NULL) {
			symbol_table -> add_label(s, a);
		}
		else {
			symbol_table -> update_label(s, a);
		}
	}

	inline ast* find_label(string s) {
		return symbol_table -> locate_label(s);
	}

	inline bool is_var(string s) {
		symbol* holder = symbol_table->locate_symbol(s);
		if (holder!= NULL) {
			return holder -> get_stype() == variable;
		}
		else {
			return false;
		}

	}

	inline ast* get_var(string s) {
		symbol* holder = symbol_table->locate_symbol(s);
		if (holder!= NULL) {
			if (holder -> get_stype() == variable)
				return var(s);
		}
		return NULL;
	}


	inline bool is_formula(ast* a) {
		return (a->get_head_type()==formula);
	}

	inline bool is_term(ast* a) {
		return (a->get_head_type() == term);
	}

	inline ast* parse_var(char* c) {
		assert(c!=NULL);
		string s(c);
		return var(s);
	} 


	inline ast* param(string s) {
		symbol* new_param = symbol_table->locate_symbol(s);
		if (new_param == NULL) {
			new_param = new symbol(s, parameter, 0);
			symbol_table -> add(new_param);
		}
		ast* a = new ast(new_param);
		return a;
	}

	inline ast* monomial(vector<ast*> x, vector<ast*> e, ast* c) {
		ast* result = c;
		for (int i=0; i<x.size(); i++) {
			result = mul(result, pow(x[i], e[i]));
		}
		return result;
	}

};







#endif