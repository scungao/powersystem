#ifndef SYMBOL_H
#define SYMBOL_H

#include <string>
#include <assert.h>

using namespace std;

//symbol type
enum s_type {
	constant,
	variable,
	parameter,
	relation,
	connective,
	pconnective, //program connective
	sfunction, //"signature function" to avoid conflict with keyword
	aux,
	label //labels can be given to processes, formulas, etc. 
};

//domain type
enum d_type {
	real,
	integer,
	boolean,
	undef
};

class symbol {
	const string name;
	s_type	type;
	int	arity;	
	d_type	domain;	
	double	upper;
	double	lower;	
	int	prec; //precedence
public:
	inline symbol(string s, s_type t)
		:name(s), type(t) {}
	inline symbol(string s, s_type t, int a)
		:name(s), type(t), arity(a) {}
	inline symbol(string s, s_type t, int a, int p)
		:name(s), type(t), arity(a), prec(p) {}
	inline ~symbol() {}
	inline string	get_name()	{ return name; }
	inline s_type	get_stype()	{ return type; }
	inline int	get_arity()	{ return arity; }	
	inline void set_stype(s_type st) { type = st; }
	inline void set_dtype(d_type dt) { domain = dt; }
	inline double get_upper ()	{
		assert(domain == real || domain == integer);
		return upper;
	}
	inline double get_lower () {
		assert(domain == real || domain == integer);
		return lower;
	}
	inline void set_upper (double ub) { upper = ub; }
	inline void set_lower (double lb) { lower = lb; }
	inline double get_value() {
		if (type == constant)
			return stod(name);
		else
			printf("get_value is invalid.\n");
			return 0.0;
	}
	inline bool match (string s) { return (name.compare(s) == 0);}
};

#endif