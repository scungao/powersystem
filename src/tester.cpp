#include <iostream>
#include <fstream>
#include "tester.h"
#include "power.h"
#include <chrono>
#include <stdlib.h>     /* srand, rand */
#include <time.h> 


using namespace std;

tester::tester(table* st):
	symbol_table(st), converter(st) {}


void tester::testall() {
	//cout<< test_ast1()->print_prefix()<<endl;
	//cout<<test_ast2()->print_tree();
	//cout<< test_ast2()->print_smt2(true)<<endl;
	//test_ast2();
	//pwf();
	//simple();
	//ipc();
	powertest2();
	//powertest3();
	//ufuk2();
} 

void tester::ufuk2() {
	vector<ast*> x;
	vector<ast*> f;

	x.push_back(var("x1"));
	x[0] -> set_bounds(-100,100);
	ast* x1 = x[0];

	x.push_back(var("x2"));
	x[1] -> set_bounds(-100,100);

	vector<ast*> p;


	ast* x2 = x[1];

	ast* f1 = x[1];

	ast* f21 = mul(div(num(1),num(3)),pow(x[0],num(3)));
	ast* f22 = x[0];
	ast* f23 = x[1];

	ast* f2 = sub(f21,add(f21,f23));

	ast* a1 = var("a1");
	//a1 = num(-13);
	a1 -> set_bounds(-10,8);

	//ast* a2 = num(7);
	ast* a2 = var("a2");
	a2 -> set_bounds(5,8);

	//ast* a3 = num(16);
	ast* a3 = var("a3");
	a3 -> set_bounds(13,18);

	//ast* a4 = num(-6);
	ast* a4 = var("a4");
	a4 -> set_bounds(-4,-7);

	//ast* a5 = num((double)7/6);
	ast* a5 = var("a5");
	a5 -> set_bounds(0,1);

	//ast* a6 = num(-3);
	ast* a6 = var("a6");
	a6 -> set_bounds(0,5);

	//ast* a7 = num(12);
	ast* a7 = var("a7");
	a7 -> set_bounds(10,15);

	//ast* a8 = num((double)-12/3);
	ast* a8 = var("a8");
	a8 -> set_bounds(-8,-2);
	
	p.push_back(a1);
	p.push_back(a2);
	p.push_back(a3);
	p.push_back(a4);
	p.push_back(a5);
	p.push_back(a6);
	p.push_back(a7);
	p.push_back(a8);

	ast* B1 = add(a1,mul(a2,pow2(x1)));
	ast* B2 = add(B1, mul(a3,pow2(x2)) );
	ast* B3 = add(B2, mul(a4, mul(pow2(x1), pow2(x2)) ) );
	ast* B4 = add(B3, mul(a5, pow(x1,num(4))));
	ast* B5 = add(B4, mul(a6, mul(x1, pow(x2, num(3)))));
	ast* B6 = add(B5, mul(a7, mul(x1,x2)));
	ast* B7 = add(B6, mul(pow(x1,num(3)),x2));

	ast* B = B7;
	ast* Bcondition = geq(B,num(0));

	ast* dB1 = mul(partial(B,x1), f1);
	ast* dB2 = mul(partial(B,x2), f2);
	ast* dB = add(dB1, dB2);
	ast* induction = leq(dB,num(0));

	ast* init1 = pow2(sub(x1,num(1.5)));
	ast* init = leq(add(init1,pow2(x2)),num(0.1));

	ast* unsafe1 = pow2(add(x1,num(1)));
	ast* unsafe2 = pow2(add(x2,num(1)));

	ast* unsafe = leq(add(unsafe1, unsafe2), num(0.16));
//	ast* safe = lnot()

	ast* final1 = lor(land(init, lnot(Bcondition)),
					  land(unsafe, Bcondition));

	ast* final2 = lnot(induction);
//	ast* final2 = induction;
	//ast* extra = geq(add(pow2(sub(x1,num(0))),pow2(sub(x2,num(2)))),num(5));
	ast* final = lor(final1,final2);

	map<symbol*, symbol*> sol;
	//get_dreal_solutions(final, sol, true);


	cegis(lnot(final),x,p,sol,0.1);

}


void tester::powertest(){

	vector<int> test_size;
	vector<double> test_time;
	vector<int>	test_degree;
	vector<int> test_nattacks;
	vector< map<symbol*, symbol*> > sols;

//collect data
	for (int i=0; i<30; i++) {
		int size = 10 + 5*i;
		for (int j=1; j<5; j++) {
			for (int k=1; k<5; k++) {

			power_grid	grid(symbol_table, size);

			test_degree.push_back(j);
			test_size.push_back(size);
			test_nattacks.push_back(k);

			grid.random_config(j);

			map<symbol*, symbol*> sol;

//			get_dreal_solutions(f1,sol,true);
			sols.push_back(sol);	//todo: save solutions somewhere
			//test_time.push_back(time);
			}
		}
	}
}
//generate julia code from the statistics that can run



/*
	int size = 10;
	power_grid	grid(symbol_table, size);
	grid.random_config(4);
	grid.dump();

	ast* f1 = land(grid.mf(),grid.attack());
	f1 = land(f1, grid.monitor(0.5));
	f1 = land(f1, grid.esth());
	f1 = land(f1, grid.unsafe(5));

	map<symbol*, symbol*>	sol;
//	get_dreal_solutions(f1,sol,true);
//	cout<< grid.p(1) -> print_infix() <<endl ;
//	cout<< grid.q(1) -> print_infix() <<endl ;



//generate julia code from the statistics that can run


*/

void tester::powertest2(){

	vector<int> test_size;
	vector<double> test_time;
	vector<int>	test_degree;
	vector<int> test_nattacks;
	vector<bool>	test_result;
	vector<int>	nvar;
	vector<int> nnonlinear;
	//vector< vector<double> > stats;
	//vector< string > stats_name;

	vector< map<symbol*, symbol*> > sols;
    std::chrono::time_point<std::chrono::system_clock> start, end;
	ofstream	stats_file;
	string stats_name = "stats_";
	string label = "";
	string label2 = "prec";

	double precision = 0.1;
	int index=0;
	double tau = 0.5;
	double eps = 10;

	int size;
	int degree;
	int nattack;

	ast* f;
//collect data
//for (int i1=0; i1<20; i1++) {
//	for (int j=1; j<5; j++) {

for (int i1=5; i1<6; i1++) {
	for (int j=1; j<10; j++) {

		cout<<"size: ";
		cin>>i1;

		cout<<"degree: ";
		cin>>j;
		//size = 10+i1*5;
		size = i1*5;

		degree = j;

		srand(time(NULL));
		nattack = 1;

		power_grid	grid(symbol_table, size);
		test_degree.push_back(degree);
		test_size.push_back(size);
		test_nattacks.push_back(nattack);

		grid.random_config(degree);
		//grid.dump();

		int targeti = rand()%size;

		for (int i = 0 ; i<15; i++){
//			int bound;
		//	cout<<"how many?(>=1): ";
		//	cin>> bound;

//			bound = i;
			cout<<"depth: "<<i+1<<endl;
			f = grid.cegar(targeti, i+1);

			map<symbol*, symbol*> sol;
			int numb = f->count("sin")+f->count("cos")+f->count("*")+f->count("/");

    		label += to_string(degree);
		
  			start = std::chrono::system_clock::now();
		//	test_result.push_back(get_dreal_solutions(f, sol, true));
			label.clear();

			end = std::chrono::system_clock::now();
			cout<<"#nl: "<<numb<<endl;
    		std::chrono::duration<double> elapsed_seconds = end-start;
			test_time.push_back(elapsed_seconds.count());
			cout<<"time: "<<elapsed_seconds.count()<<endl;
		}

	}
/*
    label2 = to_string(precision);
    stats_name += label2;
    stats_file.open(stats_name);


    stats_file<<"size = [";
    for (int i=0; i<test_size.size(); i++)
    	stats_file<<test_size[i]<<" ";
    stats_file<<"]"<<endl;


    stats_file<<"time = [";
    for (int i=0; i<test_time.size(); i++)
    	stats_file<<test_time[i]<<" ";
    stats_file<<"]"<<endl;

    stats_file<<"degree = [";
    for (int i=0; i<test_degree.size(); i++)
    	stats_file<<test_degree[i]<<" ";
    stats_file<<"]"<<endl;

    stats_file<<"nattacks = [";
    for (int i=0; i<test_nattacks.size(); i++)
    	stats_file<<test_nattacks[i]<<" ";
    stats_file<<"]"<<endl;

    stats_file.close();
    stats_name= "stats_";
*/
}



}


/*
void tester::powertest2(){
	//double b[5][5];
	//double g[5][5];
	power_grid	grid(symbol_table, 20);
	grid.add_line(1,2);
	grid.add_line(0,1);
	grid.add_line(0,2);
	grid.add_line(5,7);
	grid.add_line(9,4);
	grid.add_line(6,3);
	grid.add_line(2,8);

	ast* f1 = land(grid.mf(),grid.attack());
	f1 = land(f1, grid.monitor(0.5));
	f1 = land(f1, grid.esth());
	f1 = land(f1, grid.unsafe(5));

	cout<<f1->print_smt2(true)<<endl;	
	map<symbol*, symbol*>	sol;
	get_dreal_solutions(f1,sol,true);
//	cout<< grid.p(1) -> print_infix() <<endl ;
//	cout<< grid.q(1) -> print_infix() <<endl ;
}
*/
void tester::powertest3(){
	//double b[5][5];
	//double g[5][5];
	power_grid	grid(symbol_table, 100);
	grid.add_line(1,2);
	grid.add_line(0,1);
	grid.add_line(0,2);
	grid.add_line(5,7);
	grid.add_line(9,4);
	grid.add_line(6,3);
	grid.add_line(2,8);
	grid.add_line(5,10);
	grid.add_line(10,70);

	ast* f1 = land(grid.mf(),grid.est());

	cout<<f1->print_smt2(true)<<endl;
	
	map<symbol*, symbol*>	sol;
	get_dreal_solutions(f1,sol,true);
//	cout<< grid.p(1) -> print_infix() <<endl ;
//	cout<< grid.q(1) -> print_infix() <<endl ;
}


ast* tester::test_ast1() {

	symbol* var_x = new symbol("x", variable, 0);
	symbol* c1 = new symbol("2.1312", constant, 0);
	symbol* var_y = new symbol("y", variable, 0);
	symbol* par_a = new symbol("a", parameter, 0);

	symbol_table -> add(var_x);
	symbol_table -> add(var_y);
	symbol_table -> add(c1);

	ast* test_ast1 = new ast(symbol_table->locate_symbol("and"));
	ast* a_e2 = new ast(symbol_table->locate_symbol("="));
	ast* a_x1 = new ast(var_x);
	ast* a_y1 = new ast(var_y);
	ast* a_x2 = new ast(par_a);
	ast* a_x3 = new ast(c1);

	ast* a_e1 = eq(a_x1, a_y1);

	test_ast1 -> add_child(a_e1);
	test_ast1 -> add_child(a_e2);

	a_e2 -> add_child(a_x2);
	a_e2 -> add_child(a_x3);

	return test_ast1;
}

void tester::test_ast2() { 

//the four labels of functions
	symbol* lf[5]; //just to make the index clear
	lf[1] = new symbol("f1", label, 1);
	lf[2] = new symbol("f2", label, 1);
	lf[3] = new symbol("f3", label, 1);
	lf[4] = new symbol("f4", label, 1);

	ast* m = num("1.0");
	ast* M = num("2.0");
	ast* L = num("1.0");
	ast* g = num("9.8");
	ast* c2 = num("2");
	ast* c0 = num("0");

	vector<ast*> x;

//the four variables
	ast* x1 = var("x");
	x1 -> set_bounds(-10, 10);

	ast* x2 = var("xd");
	x2 -> set_bounds(-10, 10);

	ast* x3 = var("theta");
	x3 -> set_bounds(-4, 4);

	ast* x4 = var("thetad");
	x4 -> set_bounds(-10, 10);

	ast* u = param("u");
	u -> set_bounds(-10, 10);

	x.push_back(x1);
	x.push_back(x2);
	x.push_back(x3);
	x.push_back(x4);


//define the functions
	vector<ast*> f; 

	//f1
	 f.push_back(x2);

	//f2
	ast* term21 = mul(mul(mul(m,L),sin(x3)),pow(x4,c2));
	ast* term22 = mul(m, mul(g, mul( cos(x3), sin(x3))));
	ast* term23 = add(mul(m, pow(sin(x3), c2)), M);
	f.push_back( div(add(add(term21,term22),u),term23) );

	//f3
	f.push_back( x4 );

	//f4
	ast* term41 = mul(mul(mul(mul(m,L),sin(x3)),cos(x3)),pow(x4,c2));
	ast* term42 = mul(mul(add(m,M),g),sin(x3));
	ast* term43 = mul(cos(x3),u);
	ast* term44 = mul(sub(c0,L),add(mul(m,pow(sin(x3),c2)),M));
	f.push_back( div(add(add(term41,term42),term43),term44) );

	ast* formula1 = land(land(eq(f[2],c0),land(eq(f[0],c0),eq(f[1],c0))),eq(f[3],c0));



	ast* formula2 = dup(formula1);
	ast* formula3 = substitute(formula1, symbol_table->locate_symbol("thetad"), 
							symbol_table->locate_symbol("0"));
	ast* formula41 = substitute(x2, x3, m);
	ast* formula4 = add(x2, mul(x3, div(x1, x2)));
//	ast* formula4 = copy_replace(formula1, symbol_table->locate_symbol("xd"), 
//							symbol_table->locate_symbol("1"));

	cout<< formula1 -> print_smt2(true) <<endl;
//	cout<< formula2 -> print_smt2(true) << endl;
	cout<< formula3 -> print_smt2(true) <<endl;
//	cout<< substitute(x2, x2, m) -> print_smt2(true) <<endl;
//	cout<< formula4 -> print_smt2(true) <<endl;

	map<symbol*, symbol*> sol;

	get_dreal_solutions(formula1, sol, true);

	for (map<symbol*, symbol*>::iterator it = sol.begin(); it!=sol.end(); it++)
		formula1 = substitute(formula1, it->first, it->second);

	cout<<"original: "<<formula1 -> print_smt2(true)<<endl;
	simplify(formula1);

	cout<<"simplified: "<<formula1 -> print_smt2(false)<<endl;

	ast* formula5 = partial(formula4, symbol_table->locate_symbol("xd"));
	cout<<"derivative of: "<<formula4 -> print_prefix()<<" is "
		<<formula5 -> print_prefix()<<endl;

	ast* formula6 = div(x2, x3);
	cout << partial(formula6, x2)->print_prefix()<<endl;

	cout<<formula5 -> print_smt2(true)<<endl;

	ast* formula7 = mul(sin(x2),cos(x2));
	cout<< partial(formula7, x2)->print_prefix()<<endl;

	ast* v = x[2];
	ast* lcondition = lyapunov(f, x, v);

	cout<<lcondition->print_smt2(true)<<endl;

	vector<ast*> p;
	p.push_back(u);

	if (cegis(lcondition, x, p, sol, 0.1))
		cout<<"cegis succeeded"<<endl;
	else
		cout<<"cegies found no solution"<<endl;
}

void tester::pwf() {//pendulum with friction
	vector<ast*> x;
	vector<ast*> f;

	ast* m = num("1.0");
//	ast* m = var("m");
	ast* M = num("2.0");
	ast* L = num("1.0");
//	ast* L = var("L");
	ast* g = num("9.8");
//	ast* g = var("g");		
	ast* c2 = num("2");
	ast* c0 = num("0");
	ast* k = num("1.5");
//	ast* k = var("k");

	x.push_back(var("x1"));
	x[0]->set_bounds(-1,1);

	x.push_back(var("x2"));
	x[1]->set_bounds(-1,1);

	f.push_back(x[1]); //f1

	ast* f2part1 = sub(num("0"),mul(div(g,L),sin(x[0])));
	ast* f2part2 = mul(div(k,m),x[1]);
	f.push_back(sub(f2part1,f2part2));//f2

	vector<ast*> p;

	p.push_back(var("a"));
	p[0] -> set_bounds(6,100);

	p.push_back(var("b"));
	p[1] -> set_bounds(5, 200);

	ast* v; //energy function
	v = add(mul(p[0],pow(x[1],num(2))), mul(p[1],sub(num("1"),cos(x[0]))));
//	v = add(mul(p[0],pow(x[1],num(2))), mul(p[1],pow(sub(num("1"),cos(x[0])),num("3"))));


	ast* lcondition = lyapunov(f, x, v);

	map<symbol*, symbol*> sol;

	if (cegis(lcondition, x, p, sol, 0.1)) {
		cout<<"cegis succeeded"<<endl;
	}
	else
		cout<<"cegies found no solution"<<endl;


}


void tester::simple() {
	vector<ast*> x;
	vector<ast*> f;
	vector<ast*> p;
	ast* v;

	x.push_back(var("x"));
	x[0]->set_bounds(-1,1);

	p.push_back(var("a"));
	p[0]->set_bounds(0, 10);

	p.push_back(var("b"));
	p[1]->set_bounds(-5,10);

	f.push_back(sub(
					add(pow(x[0],num("3")),pow(x[0],num("2"))), 
					mul(p[0],x[0])
					)
				);

	v = add(p[1], pow(x[0],num("4")));

	ast* lcondition = lyapunov(f, x, v);

	map<symbol*, symbol*> sol;

	if (cegis(lcondition, x, p, sol, 0.1)) {
		cout<<"cegis succeeded"<<endl;
	}
	else
		cout<<"cegis found no solution"<<endl;
}

void tester::ufuk() {
	vector<ast*> x;
	vector<ast*> f;
	vector<ast*> p;

	x.push_back(var("x"));
	x[0]->set_bounds(-90,0);

	x.push_back(var("y"));
	x[1]->set_bounds(-90,0);

	x.push_back(var("z"));
	x[2]->set_bounds(-90,0);

	p.push_back(var("gamma"));
	p[0]->set_bounds(-9,0.5);

	p.push_back(var("a"));
	p[1]->set_bounds(-9,10);

	p.push_back(var("b"));
	p[2]->set_bounds(-9,10);

	p.push_back(var("c"));
	p[3]->set_bounds(-9,10);

//	cout<<"p size: "<<p.size();

	ast* fx = sub(x[2],x[0]);
//	ast* fx = x[0];
	ast* fy = add( 
					sub(mul(x[0],num(2)),
						mul(x[1],num(2))),
					x[2]
	//				x[1]
				);

	ast* v = add(
				add(mul(p[1],pow(x[0],num(2))),mul(p[3],mul(x[0],x[1]))),
				mul(p[2],pow(x[1],num(2)))
				);

	ast* dv = add(mul(partial(v,x[0]),fx),mul(partial(v,x[1]),fy));

	ast* rhs = sub(mul(p[0],pow(x[2],num(2))),
				   pow(mul(num(0.4),add(x[0],x[1])),num(2)));

	ast* final = land(geq(v,num(0)),leq(dv,rhs));

	map<symbol*, symbol*> sol;

	if (cegis(final, x, p, sol, 0.1)) {
		cout<<"cegis finished"<<endl;
	}
	else
		cout<<"cegies found no solution"<<endl;	
}



void tester::ipc() {
	ast* m = num("1.0");
	ast* M = num("2.0");
	ast* L = num("1.0");
	ast* g = num("9.8");
	ast* c2 = num("2");
	ast* c0 = num("0");

	vector<ast*> x;

//the four variables
	ast* x1 = var("x");
	x1 -> set_bounds(-1.5, 1.5);

	ast* x2 = var("xd");
	x2 -> set_bounds(-1, 1);

	ast* x3 = var("theta");
	x3 -> set_bounds(-0.5, 0.5);

	ast* x4 = var("thetad");
	x4 -> set_bounds(-1, 1);

	x.push_back(x1);
	x.push_back(x2);
	x.push_back(x3);
	x.push_back(x4);

	vector<ast*> p;

	ast* kp = var("kp");
	kp->set_bounds(0,30);

	ast* kl = var("kl");
	kl->set_bounds(0,25);

	ast* ki = var("ki");
	ki->set_bounds(0,20);


	p.push_back(kp);
	p.push_back(kl);
	p.push_back(ki);


	ast* ksi = add(div(x1,L),mul(kp,sin(x3)));
	cout<<"ksi: "<<ksi->print_infix()<<endl;

	ast* delta = div(M,m);

	ast* pksi = add(div(x2,L),mul(mul(kp,x4),cos(x3)));
	cout<<"pksi: "<<pksi->print_infix()<<endl;
	
	
	ast* itax = add(mul(ki,ksi),mul(kp,mul(sin(x3), 
							sub(cos(x3), pow(x4,num(2))) )));
	cout<<"itax: "<<itax->print_infix()<<endl;
		
	ast* deltatheta = sub(add(num(1), kl), mul(kp, pow(cos(x3),num(2))));
	cout<<"deltatheta: "<<deltatheta->print_infix()<<endl;
	

	ast* veta = sub(num(0),
			   div(add(pksi,itax),deltatheta));
	cout<<"veta: "<<veta->print_infix()<<endl;

	ast* u = add(
			sub(mul(cos(x3),sin(x3)),mul(pow(x4,num(2)),sin(x3))),
			mul(veta,add(pow(sin(x3),num(2)),delta))
			);
	simplify(u);
	cout<<"u: "<<u->print_infix()<<endl;

	ast* phix = add(
					sub(mul(kp,sub(num(1),cos(x3))),
						mul(num(0.5),mul(kp,pow(x4,num(2))))),
					mul(num(0.5),pow(div(x2,L),num(2)))
					);
	cout<<"phix: "<<phix->print_infix()<<endl;

	ast* V = add(add(mul(mul(num(0.5),ki),pow(ksi,num(2))),
				 	 mul(num(0.5),pow(pksi,num(2)))),
				 mul(kl, phix)
				);
	cout<<"V: "<<V->print_infix()<<endl;

//define the functions
	vector<ast*> f; 

	//f1
	 f.push_back(x2);

	//f2
	ast* term21 = mul(mul(mul(m,L),sin(x3)),pow(x4,c2));
	ast* term22 = mul(m, mul(g, mul(cos(x3), sin(x3))));
	ast* term23 = add(mul(m, pow(sin(x3), c2)), M);
	f.push_back( div(add(add(term21,sub(num("0"),term22)),u),term23) );
	simplify(f[1]);
	cout<<"f2: "<<f[1]->print_infix()<<endl;

	//f3
	f.push_back( x4 );

	//f4
	ast* term41 = mul(mul(mul(mul(m,L),sin(x3)),cos(x3)),pow(x4,c2));
	ast* term42 = mul(mul(add(m,M),g),sin(x3));
	ast* term43 = mul(cos(x3),u);
	ast* term44 = mul(L,add(mul(m,pow(sin(x3),c2)),M));
	f.push_back(div(add(add(sub(num("0"),term41),term42),sub(num("0"),term43)),term44) );
	simplify(f[3]);
	cout<<"f4: "<<f[3]->print_infix()<<endl;


//	ast* formula1 = land(land(eq(f[2],c0),land(eq(f[0],c0),eq(f[1],c0))),eq(f[3],c0));
//	simplify(formula1);
//	cout<<formula1->print_infix()<<endl;
//	get_dreal_solutions(formula1, sol, true);

	ast* lcondition = lyapunov(f, x, V);
//	ast* lcondition = gt(x1,num(0));

	map<symbol*, symbol*> sol;

	if (cegis(lcondition, x, p, sol, 0.1)) {
		cout<<"cegis succeeded"<<endl;
	}
	else
		cout<<"cegis found no solution"<<endl;

}


void tester::cubli() {
	vector<ast*> x;
	vector<ast*> f;
	vector<ast*> p;

	ast* phi = var("phi");
	x.push_back(phi);
	phi->set_bounds(-1,1);

	ast* p_phi = var("p_phi");
	x.push_back(p_phi);
	p_phi->set_bounds(-5,5);

	ast* p_psi = var("p_psi");
	x.push_back(p_psi);
	p_psi->set_bounds(-5,5);

}



