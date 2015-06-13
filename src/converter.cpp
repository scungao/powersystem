#include "converter.h"
#include <math.h>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

extern table* full_table;
extern converter* operators;

ast* converter::dup(ast* source) {
	ast* result = new ast();
	result -> set_head_symbol(source->get_head_symbol());
	result -> set_parent(source->get_parent());
	result -> set_height(source->get_height());
	result -> set_period(source->get_period());
	for (int i=0; i< source->get_degree(); i++)
		result -> add_child(dup(source->get_child(i)));
	return result;
}
/*
ast* converter::substitute(ast* source, ast* old, ast* neo) {
	ast* result = new ast();
	if (source->get_head_symbol() == old->get_head_symbol()) {
		result -> set_head_symbol(neo->get_head_symbol());	
	}
	else {
		result -> set_head_symbol(source->get_head_symbol());
	}
	for (int i=0; i<source->get_degree(); i++)
		result -> add_child(substitute(source->get_child(i), old, neo));
	return result;
}
*/

ast* converter::substitute(ast* source, symbol* old, symbol* neo) {
	ast* result = dup(source);
	result -> substitute(old, neo);
	return result;
}

bool converter::get_dreal_solutions(ast* phi, map<symbol*, 
									symbol*>& sol, bool polarity) {
/*
#include <iostream>
#include <fstream>
using namespace std;

int main () {
  ofstream myfile;
  myfile.open ("example.txt");
  myfile << "Writing this to a file.\n";
  myfile.close();
  return 0;
}
*/
	stringstream s;
	ofstream	dreal_file;
	dreal_file.open("dreal_file.smt2");

	sol.clear();

//	simplify(phi);
	dreal_file << phi->print_smt2(polarity);
	dreal_file.close();

	//cout<<"./dReal --proof --precision 0.01 dreal_file.smt2";
	system("./dReal --proof --precision 0.1 dreal_file.smt2");

	ifstream	dreal_result;
	dreal_result.open("dreal_file.smt2.proof");

	char cc;
	string cline;
	string	buffer;
	vector<string>	fields;

	getline(dreal_result, cline);

	if (cline.find("SAT") == 0) {
//		printf("sat answer is returned\n");
		while (dreal_result.get(cc)) { //collect the fields
			if ((isalnum(cc)||ispunct(cc))
			 		&& cc!=',' && cc!= ':' && cc!='[' && cc!= ']' && cc!= ';')
				buffer += cc;
			else {
				if (!buffer.empty())
					fields.push_back(buffer);
				buffer.clear();
			}
		}
		//cout<<endl;
		//for (int i=0; i<fields.size(); i++) cout<<i<<": "<<fields[i]<<endl;

		for(int i=0; i<fields.size(); i=i+3) {

			//avoid underflow
			if (fields[i+1][(fields[i+1]).size()-5]=='e'&& 
					fields[i+1][(fields[i+1]).size()-4]=='-')
				fields[i+1] = "0";

			if (fields[i+2][(fields[i+2]).size()-5]=='e'&& 
					fields[i+2][(fields[i+2]).size()-4]=='-')
//			if (fields[i+2].find("e-308")!= std::string::npos)
				fields[i+2] = "0";

		//	cout<<"fields_a: "<<fields[i+1]<<endl;
		//	cout<<"fields_b: "<<fields[i+2]<<endl;

			double a = stod(fields[i+1]);
			double b = stod(fields[i+2]);
			symbol* name = symbol_table->locate_symbol(fields[i]);
			symbol* value = num_sym(to_string(0.5*(a+b)));
			sol.insert(pair<symbol*, symbol*>(name,value));
			//	cout<<fields[i]<<"["<<fields[i+1]<<","<<fields[i+2]<<"]"<<endl;
		}

//print solutions
//		for(map<symbol*,symbol*>::iterator it=sol.begin(); it!=sol.end(); it++) {
//			cout<< it->first->get_name()<<" : "<<it->second->get_name()<<endl;
//		}	

		dreal_result.close();
		return true;
	}

	dreal_result.close();
	return false;
}

bool converter::get_dreal_solutions(ast* phi, map<symbol*, symbol*>& sol, bool polarity, double precision, string label, int index) {
/*
#include <iostream>
#include <fstream>
using namespace std;

int main () {
  ofstream myfile;
  myfile.open ("example.txt");
  myfile << "Writing this to a file.\n";
  myfile.close();
  return 0;
}
*/
	stringstream s;
	ofstream	dreal_file;

	string fname = "dreal_file_";
	fname += label;
	fname += "_";
	fname += to_string(index);
	fname += ".smt2";

	dreal_file.open(fname);

	sol.clear();

	simplify(phi);
	cout<< phi->print_smt2(polarity);
	dreal_file << phi->print_smt2(polarity);
	dreal_file.close();

	string cmd = "time ./dReal --proof --precision ";
	cmd += to_string(precision);
	cmd += " ";
	cmd += fname;

	system(cmd.c_str());

	ifstream	dreal_result;
	fname += ".proof";

	dreal_result.open(fname);

	char cc;
	string cline;
	string	buffer;
	vector<string>	fields;

	getline(dreal_result, cline);

	bool res = (cline.find("SAT") == 0);
	dreal_result.close();

	return res;
	
	/*
	if (cline.find("SAT") == 0) {
//		printf("sat answer is returned\n");
		while (dreal_result.get(cc)) { //collect the fields
			if ((isalnum(cc)||ispunct(cc))
			 		&& cc!=',' && cc!= ':' && cc!='[' && cc!= ']')
				buffer += cc;
			else {
				if (!buffer.empty())
					fields.push_back(buffer);
				buffer.clear();
			}
		}
		//for (int i=0; i<fields.size(); i++) cout<<fields[i]<<endl;

		for(int i=0; i<fields.size(); i=i+4) {

			//avoid underflow
			if (fields[i+1][(fields[i+1]).size()-5]=='e'&& 
					fields[i+1][(fields[i+1]).size()-4]=='-')
				fields[i+1] = "0";

			if (fields[i+2][(fields[i+2]).size()-5]=='e'&& 
					fields[i+2][(fields[i+2]).size()-4]=='-')
//			if (fields[i+2].find("e-308")!= std::string::npos)
				fields[i+2] = "0";

			double a = stod(fields[i+1]);
			double b = stod(fields[i+2]);
			symbol* name = symbol_table->locate_symbol(fields[i]);
			symbol* value = num_sym(to_string(0.5*(a+b)));
			sol.insert(pair<symbol*, symbol*>(name,value));
			//	cout<<fields[i]<<"["<<fields[i+1]<<","<<fields[i+2]<<"]"<<endl;
		}

//		for(map<symbol*,symbol*>::iterator it=sol.begin(); it!=sol.end(); it++) {
//			cout<< it->first->get_name()<<" : "<<it->second->get_name()<<endl;
//		}	

		dreal_result.close();
		return true;
	}

	dreal_result.close();
	return false;
	*/
}

void converter::simplify(ast* a) {

	for (int i=0; i<a->get_degree(); i++) {
		simplify(a -> get_child(i));
	}

	if(a->get_degree()==0) return;

	if (a->get_head_name() == "+") {
		if (a->get_child_type(0) == constant 
				&& a->get_child_type(1) == constant) {
			double c1 = a->get_child(0)->get_value();
			double c2 = a->get_child(1)->get_value();
			double c = c1+c2;
			a -> set_head_symbol(num_sym(c));
			a -> clear_children();
		}
	}
	else if (a->get_head_name() == "*") {
		if ( (a->get_child_type(0) == constant 
				&& a->get_child(0)->get_value() == 0) ||
			 (a->get_child_type(1) == constant 
				&& a->get_child(1)->get_value() == 0) ) {
//		 (a->get_child(0)->get_name() == "0"
//			|| a->get_child(0)->get_name() == "0.0"
//			|| a->get_child(1)->get_name() == "0"
//			|| a->get_child(1)->get_name() == "0.0"){
			a->set_head_symbol(num_sym(0));
			a->clear_children();
		}

		if (a->get_child_type(0) == constant 
				&& a->get_child_type(1) == constant)
				 {
			double c1 = a->get_child(0)->get_value();
			double c2 = a->get_child(1)->get_value();
			double c = c1*c2;
			a -> set_head_symbol(num_sym(c));
			a -> clear_children();
		}
	}
	else if (a->get_head_name() == "-") {
		if (a->get_child_type(0) == constant 
				&& a->get_child_type(1) == constant) {
			double c1 = a->get_child(0)->get_value();
			double c2 = a->get_child(1)->get_value();
			double c = c1-c2;
			a -> set_head_symbol(num_sym(c));
			a -> clear_children();
		}
	}
	else if (a->get_head_name() == "/") {
		if (a->get_child_type(0) == constant 
				&& a->get_child_type(1) == constant) {
			double c1 = a->get_child(0)->get_value();
			double c2 = a->get_child(1)->get_value();
			double c = c1/c2;
			a -> set_head_symbol(num_sym(c));
			a -> clear_children();
		}
	}
	else if (a->get_head_name() == "^") {
		if (a->get_child_type(0) == constant 
				&& a->get_child_type(1) == constant) {
			double c1 = a->get_child(0)->get_value();
			double c2 = a->get_child(1)->get_value();
			double c = ::pow(c1,c2);
			a -> set_head_symbol(num_sym(c));
			a -> clear_children();
		}
	}
	else if (a->get_head_name() == "sin") {
		if (a->get_child_type(0) == constant) {
			double c1 = a->get_child(0)->get_value();
			double c = ::sin(c1);
			a -> set_head_symbol(num_sym(c));
			a -> clear_children();
		}
	}
	else if (a->get_head_name() == "cos") {
		if (a->get_child_type(0) == constant) {
			double c1 = a->get_child(0)->get_value();
			double c = ::cos(c1);
			a -> set_head_symbol(num_sym(c));
			a -> clear_children();
		}
	}	
	else if (a->get_head_name() == "tan") {
		if (a->get_child_type(0) == constant) {
			double c1 = a->get_child(0)->get_value();
			double c = ::tan(c1);
			a -> set_head_symbol(num_sym(c));
			a -> clear_children();
		}
	}
	else if (a->get_head_name() == "=") {
		if ( a->get_child_type(0) == constant 
				&& a->get_child_type(1) == constant ) {
			if (a->get_child(0)->get_value() == a->get_child(1)->get_value())
				a -> set_head_symbol(symbol_table->locate_symbol("true"));
				a -> clear_children();
		}
	}
	else if (a->get_head_name() == "<") {
		if ( a->get_child_type(0) == constant 
				&& a->get_child_type(1) == constant ) {
			if (a->get_child(0)->get_value() < a->get_child(1)->get_value())
				a -> set_head_symbol(symbol_table->locate_symbol("true"));
				a -> clear_children();
		}
	}	
	else if (a->get_head_name() == ">") {
		if ( a->get_child_type(0) == constant 
				&& a->get_child_type(1) == constant ) {
			if (a->get_child(0)->get_value() > a->get_child(1)->get_value())
				a -> set_head_symbol(symbol_table->locate_symbol("true"));
				a -> clear_children();
		}
	}
	else if (a->get_head_name() == "<=") {
		if ( a->get_child_type(0) == constant 
				&& a->get_child_type(1) == constant ) {
			if (a->get_child(0)->get_value() <= a->get_child(1)->get_value())
				a -> set_head_symbol(symbol_table->locate_symbol("true"));
				a -> clear_children();
		}
	}	
	else if (a->get_head_name() == ">=") {
		if ( a->get_child_type(0) == constant 
				&& a->get_child_type(1) == constant ) {
			if (a->get_child(0)->get_value() >= a->get_child(1)->get_value())
				a -> set_head_symbol(symbol_table->locate_symbol("true"));
				a -> clear_children();
		}
	}	
	//return a;

}

ast* converter::partial(ast* V, symbol* x) { //change the places that need to be changed, then simplify
	ast* result;
	if (V->get_head_symbol() == x) {
		result = num("1");
	}
	else if (V->get_head_symbol()->get_stype() == variable 
				|| V->get_head_symbol()->get_stype() == parameter
				|| V->get_head_symbol()->get_stype() == constant ) {
		result = num("0");
	}
	else if ( V-> get_head_name()== "+") {
		result = add(	partial(V->get_child(0), x)
						, 
						partial(V->get_child(1), x)
					);
	}
	else if ( V-> get_head_name() == "-") {
		result = sub(	partial(V->get_child(0), x)
						, 
						partial(V->get_child(1), x)
					);
	}
	else if ( V-> get_head_name() == "*") {
			result = add(
							mul(partial(V->get_child(1), x), V->get_child(0))
							,
							mul(partial(V->get_child(0), x), V->get_child(1))
						);
	}
	else if ( V-> get_head_name() == "/") {
			result = mul(
							sub(
								mul(partial(V->get_child(0), x), V->get_child(1))
								,
								mul(partial(V->get_child(1), x), V->get_child(0))
								)
							,
							pow(V->get_child(1),num("-2"))
						);
	}
	else if ( V-> get_head_name() == "^") {
		result = mul(
						mul(V->get_child(1), pow(V->get_child(0), sub(V->get_child(1), num("1"))))
						,
						partial(V->get_child(0),x) //chain rule
					);
	}
	else if (V -> get_head_name() == "sin") {
		result = mul(
						cos(V->get_child(0)),
						partial(V->get_child(0),x)
					);
	}
	else if (V -> get_head_name() == "cos") {
		result = mul(
						sub(num("0"), sin(V->get_child(0))),
						partial(V->get_child(0),x)
					);
	}
	else if (V -> get_head_name() == "tan") {
		result = mul(
						add(num("1"), pow(tan(V->get_child(0)), num("2"))),
						partial(V->get_child(0),x)
					);
	}
	else {
		result = dup(V);
	}
	simplify(result);
	return result;
}


ast* converter::lyapunov(vector<ast*>& f, vector<ast*>& x, ast* v) {
	ast* result;
	ast* pos_v;
	ast* neg_dv = num("0");

	pos_v = geq(v, num("0"));

	for (int i=0; i< f.size(); i++) {
		ast* term = mul(partial(v, x[i]), f[i]);
		neg_dv = add(term, neg_dv);
	}

	neg_dv = leq(neg_dv, num("0"));
	result = land(pos_v, neg_dv);

	simplify(result);
	return result;
}


bool converter::cegis(ast* phi, vector<ast*>& x, 
						vector<ast*>& p, map<symbol*,symbol*>& sol, double delta) {
/*
	1. start with choosing some random values in p
	2. sub in values in p, solve for the negation of x
	3. sub in values of x, solve for p
	4. loop 2-3, but add new values (a ball around it) of x each time into constraint. 
	5. terminate whenever unsat. 
*/
	//vector<double> sol_values;
	ast* phi_temp = phi;
	ast* phi_sol = leq(num(0),num(1)); //top
	sol.clear();
	for(int i=0; i<p.size(); i++) {
//		cout<<p[i]->get_head_name()<<" ";
		double l = p[i]->get_head_symbol()->get_lower();
		double u = p[i]->get_head_symbol()->get_upper();
		//sol_values.push_back(0.5*(l+u));
		sol.insert(pair<symbol*,symbol*>(p[i]->get_head_symbol(), num_sym(0.5*(l+u))));
		phi_temp = substitute(phi_temp, p[i], num(0.5*(l+u)));
	}
//	cout<<endl;
	phi_temp = lnot(phi_temp);
//	cout<<phi_temp->print_infix()<<endl;
//	cout<<phi_temp->print_smt2(true)<<endl;

	vector<ast*>	different_x_instances;
	vector<ast*>	explored_ctx; //explored counterexamples
	//last bool sends negation:
	int counter=0;

	//debugging, if was while
	while(get_dreal_solutions(phi_temp, sol, true)) {
//		cout<<"Found counterexamples with: "<<phi_temp -> print_smt2(true);
		//cin.get();
		//sol now keeps the counterexample for x
		phi_temp = phi;
		phi_sol = top(); //phi_sol will encode conditions for excluding explored examples

		cout<<"[counterexamples] ";
		for(map<symbol*,symbol*>::iterator it=sol.begin(); it!=sol.end(); it++) {
			cout<< it->first->get_name()<<": "<<it->second->get_name()<<" ";
			phi_temp = substitute(phi_temp, it->first, it->second);
/*
			phi_sol = land(
						lor( 
							geq(sub(var(it->first->get_name()), num(it->second->get_name())), 
								num(delta)),
							geq(num(-delta), 
								sub(var(it->first->get_name()), num(it->second->get_name())))
				  	  	),
				  	  	phi_sol
				  	  );	
*/
			phi_sol = land(
							geq( pow(sub(var(it->first->get_name()), num(it->second->get_name())),num(2)), 
								num(delta) ),
				  	  		phi_sol
				  	  );	
		}
		cout<<endl;

		different_x_instances.push_back(phi_temp);
		explored_ctx.push_back(phi_sol); //store the negation
		//take conjunction of all learned points
		for(int i=0; i<different_x_instances.size()-1; i++) { //no need to redo the last one
			phi_temp = land(different_x_instances[i], phi_temp);
		}

		simplify(phi_temp);
//		cout<<"Find parameters with: "<<phi_temp -> print_smt2(true);

		if (!get_dreal_solutions(phi_temp, sol, true)) return false;
		//sol now keeps candidates for p

		phi_temp = phi;

		cout<<"[candidates] ";

		for(map<symbol*,symbol*>::iterator it=sol.begin(); it!=sol.end(); it++) {
			cout<< it->first->get_name()<<": "<<it->second->get_name()<<" ";
			phi_temp = substitute(phi_temp, it->first, it->second);
		}
		phi_temp = lnot(phi_temp);

		for(int i=0; i<explored_ctx.size(); i++) { //need the last one
			phi_temp = land(explored_ctx[i], phi_temp);
		}
		cout<<endl;
		cout<<"--- Round "<<counter++<<" ----"<<endl;
	}
	return true;	
}


ast* operator+(ast& a1, ast& a2) { 
	return operators->add(&a1,&a2);
}

ast* operator-(ast& a1, ast& a2) { 
	return operators->sub(&a1,&a2);
}

ast* operator*(ast& a1, ast& a2) {
	return operators->mul(&a1,&a2);
}
	
ast* operator/(ast& a1, ast& a2) {
	return operators->div(&a1,&a2);
}





