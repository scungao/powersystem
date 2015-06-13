#include "ast.h"
#include <iostream>
#include <sstream>
#include <string>

ast::ast(symbol* s)
	:	head_symbol(s), terminal(true), 
		degree(0), height(0), parent(NULL)
{}

ast::ast(symbol* s, ast* l)
	:	head_symbol(s), terminal(false), degree(1), parent(NULL)
{
//	flatname = s->get_name();
	children.push_back(l);
//	flatname += "(";
//	flatname += l->flatname;
//	flatname += ")";
//	height = (children[0]->height)+1;
}


ast::ast(symbol* s, ast* l1, ast* l2)
	:	head_symbol(s), terminal(false), degree(2), parent(NULL)
{
//	flatname = s->get_name();

	children.push_back(l1);
	children.push_back(l2);

//	flatname += "(";
	height = 0;
	for (int i=0; i<2; i++) {
		if ( children[i]->height > height ) 
			height = children[i]->height;
//		flatname += children[i]->flatname;
//		flatname += ",";
	}
//	flatname.pop_back(); //delete trailing ","
//	flatname += ")";
	height += 1;
}

ast::ast(symbol* s, ast* l1, ast* l2, ast* l3)
	:	head_symbol(s), terminal(false), degree(3), parent(NULL) 
{
//	flatname = s->get_name();
	
	children.push_back(l1);
	children.push_back(l2);
	children.push_back(l3);

//	flatname += "(";
	height = 0;
	for (int i=0; i<3; i++) {
		if ( children[i]->height > height ) 
			height = children[i]->height;
//		flatname += children[i]->flatname;
//		flatname += ",";
	}
//	flatname.pop_back(); //delete trailing ","
//	flatname += ")";
	height += 1;
}

void ast::add_child(ast* a){ 
	children.push_back(a); 
	a->set_parent(this); 
//	set<symbol*>::iterator it;
//	for(it = a->get_variables_set()->begin(); 
//			it != a->get_variables_set()->end(); it++)
//		add_variable(*it); 
//	for(it = a->get_parameters_set()->begin(); 
//			it != a->get_parameters_set()->end(); it++)
//		add_parameter(*it);
}


void ast::substitute(symbol* old, symbol* neo) {
	if (get_head_symbol() == old)
		set_head_symbol(neo);
	for (int i=0; i< get_degree(); i++)
		get_child(i)->substitute(old,neo);
}

string ast::print_prefix() {
	stringstream result;
	if (get_degree() !=0) result<<"(";
	result<<get_head_name();
	for (int i=0; i< children.size(); i++) {
		result <<" "<<children[i]->print_prefix();
	}
	if (get_degree() !=0) result<<")";
	return result.str();
}

string ast::print_infix() {
	stringstream result;
	if (get_degree() > 1 ){
		result<<"(";
	} 

	if (get_degree()<=1)
		result<<get_head_name();

	if (get_degree()==1)
		result<<"(";

	for (int i=0; i<children.size(); i++) {
		result <<children[i]->print_infix();
		if (i<children.size()-1) result<< get_head_name();
	}
	if (get_degree() >= 1 ) result<<")";
	return result.str();
}

string ast::print_tree() {
	stringstream result;
	result<<get_head_name()<<endl;
	for (int i=0; i< children.size(); i++) {
		result <<" "<<children[i]->print_tree();
	}
	return result.str();
}

string ast::print_smt2(bool polar) {
/*
(set-logic QF_NRA)
(declare-fun x1 () Real)
(declare-fun x2 () Real)
(assert (<= 3.0 x1))
(assert (<= x1 3.14))
(assert (<= -7.0 x2))
(assert (<= x2 5.0))
(assert (<= (- (* 2.0 3.14159265) (* 2.0 (* x1 (arcsin (* (cos 0.797) (sin (/ 3.14159265 x1)))))))
(+ (- 0.591 (* 0.0331 x2)) (+ 0.506 1.0))))
(check-sat)
(exit)
*/
	stringstream result;
	stringstream vtemp; //keep bounds on vars
	stringstream ptemp;

	result<<"(set-logic QF_NRA)"<<endl;

	set<symbol*> variable_set;

	collect(variable_set, variable);
	set<symbol*>::iterator it;

	cout<<"#vars: "<<variable_set.size()<<endl;

	for (it = variable_set.begin(); it!= variable_set.end(); it++) {
		result<<"(declare-fun "<<(*it)->get_name()<<" () Real)"<<endl;
		vtemp<<"(assert (<= "<<(*it)->get_name()<<" "<<(*it)->get_upper()<<"))"<<endl;
		vtemp<<"(assert (>= "<<(*it)->get_name()<<" "<<(*it)->get_lower()<<"))"<<endl;			
	}

	set<symbol*> parameter_set;
	collect(parameter_set, parameter);
	for (it = parameter_set.begin(); it!= parameter_set.end(); it++) {
		result<<"(declare-fun "<<(*it)->get_name()<<" () Real)"<<endl;
		ptemp<<"(assert (<= "<<(*it)->get_name()<<" "<<(*it)->get_upper()<<"))"<<endl;
		ptemp<<"(assert (>= "<<(*it)->get_name()<<" "<<(*it)->get_lower()<<"))"<<endl;
	}

	result<<vtemp.str()<<ptemp.str();

	if (polar)
		result<<"(assert "<<print_prefix()<<")"<<endl;
	else //give negation
		result<<"(assert (not "<<print_prefix()<<"))"<<endl;
	result<<"(check-sat)"<<endl;
	result<<"(exit)"<<endl;

	return result.str();
}

void ast::collect(set<symbol*>& vs, s_type s) {
	if (head_symbol -> get_stype() == s)
		vs.insert(head_symbol);
	for (int i=0; i<get_degree(); i++)
		get_child(i) -> collect(vs,s);
}

ast::~ast() 
{
}