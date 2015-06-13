#ifndef NODE_H
#define NODE_H

#include "symbol.h"
#include <string>

//node type
enum n_type {
	term,
	formula, //eval to boolean
	assignment,
	sequence, 
	ite
};

class node {
	const string	name;
	const n_type	type;
	const unsigned	arity;
public:
	inline node (string s, n_type t, unsigned a)
		: name(s), type(t), arity(a) {};
	~node();
	inline string	get_name()	{ return name; }
	inline n_type	get_type()	{ return type; }
	inline unsigned	get_arity()	{ return arity; }	
};

#endif