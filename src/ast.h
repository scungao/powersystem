#ifndef AST_H
#define AST_H

#include "symbol.h"
#include <string>
#include <vector>
#include <set>

//ast type
enum a_type {
	term,
	formula,
	statement,
	process, 
	other
};

//this ast is more general than the usual one
class ast {
	bool	terminal;
	symbol*		head_symbol;
	ast*		parent;
	vector<ast*>		children; //array of pointers to ASTs
	int	degree; //branching degree of head_symbol
	int	height;
	double	period; //for process
public:
	inline ast() {}
	ast(symbol*); // terminal leaf
	ast(symbol*, ast*); //unary tree
	ast(symbol*, ast*, ast*); //binary tree
	ast(symbol*, ast*, ast*, ast*); //ternary tree
	~ast();
	inline a_type get_head_type() { 	
		switch(head_symbol->get_stype()) {
			case variable:
			case parameter: 
			case constant:
			case sfunction:
				return term;
			case connective:
			case relation:
				return formula;
			case pconnective:
				return statement;
			case aux:
			case label:
				return other;
		}
	}
	inline bool is_formula() {
		return (get_head_type()==formula);
	}

	inline bool is_term() {
		return (get_head_type() == term);
	}

	inline symbol*	get_head_symbol()	{ return head_symbol; }
	inline bool	compare_head_symbol(symbol* s) 
		{ return s == get_head_symbol(); }
	inline string	get_head_name()	{ return head_symbol->get_name(); }
	inline string 	get_name() { return get_head_name(); }
	inline void 	set_head_symbol(symbol* s) { head_symbol = s; } 

	inline ast* 	get_child(int i)	{ return children[i]; }
	inline s_type 	get_child_type(int i) { 
		return children[i]-> get_head_symbol() -> get_stype(); }

	//this is only for constants
	inline double 	get_value() { return get_head_symbol()->get_value();}

	inline void clear_child(int i) { children.erase(children.begin()+i); }
	inline void clear_children() { 
	//	for (int i=0; i<get_degree(); i++) 
	//		delete(children[i]);
		children.clear(); 
	}
	void		add_child(ast*);
	inline int	get_degree()	{ return children.size(); }
	//inline void set_degree(int d) { degree=d; 

	inline int	get_height()	{ return height; }
	inline void set_height(int h) { height=h; }
	inline bool		is_terminal()	{ return terminal; }
//	inline string	get_flatname()	{ return flatname; }
	inline void		set_terminal()	{ terminal = true; }
	inline double	get_period()	{ return period; }
	inline void 	set_period(double p)	{ period = p; }
//	inline void	add_variable(symbol* v) { variables.insert(v); }
//	inline void	add_parameter(symbol* p) { variables.insert(p); }
//	inline set<symbol*>* get_variables_set() { return &variables; }
//	inline set<symbol*>* get_parameters_set() { return &parameters; }
	inline void set_parent(ast* a) { parent = a; }
	inline ast* get_parent() { return parent;}
	inline void set_upper(double a) {head_symbol->set_upper(a);}
	inline void set_lower(double a) {head_symbol->set_lower(a);}
	inline void set_bounds(double a, double b) {
		head_symbol->set_dtype(real);
		head_symbol->set_upper(b); head_symbol->set_lower(a);
	}
	void	substitute(symbol*, symbol*);
//	void normalize();
	void collect(set<symbol*>&, s_type);
	string print_tree();
	string print_prefix();
	string print_infix();
	string print_smt2(bool);//bool sets the formula or its negation
	int count(string s) {
		int r = 0;
		if (get_head_name().compare(s) == 0) {
			r++;
		}
		for (int i=0; i<children.size(); i++) {
			r += children[i]->count(s);
		}
		return r;
	}
	int count(s_type s) {
		int r = 0;
		if (get_head_symbol() -> get_stype() == s) {
			r++;
		}
		for (int i=0; i<children.size(); i++) {
			r += children[i]->count(s);
		}
		return r;
	}
};

#endif
