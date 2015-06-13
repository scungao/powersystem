#include "power.h"
#include <assert.h>
#include <random>

power_grid::power_grid(table* t, int n) 
	: converter(t), size(n) {

	b.resize(size);
	g.resize(size);
	zp.resize(size);
	zq.resize(size);
	zph.resize(size);
	zqh.resize(size);
	ap.resize(size);
	aq.resize(size);
	sigma2.resize(size);

	for (int i = 0; i<size; i++) {
		string v = "v";
		string theta = "th";
		string v_hat = "vhat";
		string t_hat = "thhat";

		v += to_string(i);
		theta += to_string(i);
		v_hat += to_string(i);
		t_hat += to_string(i);
	//	cout<<v<<" "<<theta;
	
		ast* a1 = var(v);
		a1->set_bounds(0,2);

		ast* a2 = var(theta);
		a2 -> set_bounds(-2,2);

		ast* a3 = var(v_hat);
		a3 -> set_bounds(0,2);

		ast* a4 = var(t_hat);
		a4 -> set_bounds(-2,2);

		volts.push_back(a1);
		phasors.push_back(a2);
		vhat.push_back(a3);
		thehat.push_back(a4);

		x.push_back(a1);
		x.push_back(a1);
		xhat.push_back(a3);
		xhat.push_back(a4);

		b[i].resize(size,0.0);
		g[i].resize(size,0.0);
		sigma2[i].resize(size,0.00001);

		for (int j =0; j<size; j++) {
			string zpn = "zp_";
			string zqn = "zq_";
			string zphn = "zph_";
			string zqhn = "zqh_";
			string apn = "ap_";
			string aqn = "aq_";

			zpn += to_string(i);
			zqn += to_string(i);
			zpn += "_";
			zqn += "_";
			zpn += to_string(j);
			zqn += to_string(j);

			zphn += to_string(i);
			zqhn += to_string(i);
			zphn += "_";
			zqhn += "_";
			zphn += to_string(j);
			zqhn += to_string(j);

			apn += to_string(i);
			aqn += to_string(i);
			apn += "_";
			aqn += "_";
			apn += to_string(j);
			aqn += to_string(j);

			ast* zpv = var(zpn);
			zpv -> set_bounds(-3,3);

			ast* zqv = var(zqn);
			zqv -> set_bounds(-3,3);
	
			ast* zphv = var(zphn);
			zphv -> set_bounds(-3,3);
	
			ast* zqhv = var(zqhn);
			zqhv -> set_bounds(-3,3);

			ast* apv = var(apn);
			apv -> set_bounds(-5,5);

			ast* aqv = var(aqn);
			aqv -> set_bounds(-5,5);

			zp[i].push_back(zpv);
			zq[i].push_back(zqv);
			zph[i].push_back(zphv);
			zqh[i].push_back(zqhv);
			aq[i].push_back(aqv);
			ap[i].push_back(apv);
		}

	}	
}



ast* power_grid::cegar(int bus, int n) { //gradually add in n ctrs
	ast* result = top();
		
	int i = bus;
	bool protect_bus = false;
	bool protect_line = true;
	int mem; //previous bus

	for (int k=0; k<n; k++) {

		if (protect_bus) {
			ast* a1 = eq( zp[i][i], p(i,volts,phasors));  
//			ast* a2 = eq( zq[i][i], q(i,volts,phasors)); 
//			result = land(result, land(a1,a2));

//			ast* s1 = eq(zp[i][i], num("1"));
//			ast* s2 = eq(zq[i][i], num("1"));
//			result = land(result, land(a1,s1));
			result = land(result, a1);
		}

		if (k%2 == 0) protect_bus = true;

		if (neighbors[i]!=NULL) {
			int j;
			for(set<int>::iterator it = neighbors[i]->begin();
				it != neighbors[i]->end(); it++) {
					j = *it;
					if (j!= mem) break;
			}
			//cout<<"new bus: "<<j<<endl;
				//if (j > i) {
			//if (j == i) 
			//	j = *(it++); 
			if (protect_line) {
				ast* a3 = eq(zp[i][j],p(i,j,volts,phasors));
				//ast* a4 = eq(zq[i][j],q(i,j,volts,phasors));	
				//result = land(result, land(a3, a4));	
				result = land(result, a3);
			}

			mem = i;
			i = j;			
		}
	}

	simplify(result);
	return (result);
}




void power_grid::add_line(int i, int j) {
	map<int, set<int>*>::iterator it;
	set<int>* nb;

	it = neighbors.find(i);
	if (it!= neighbors.end())
		it->second->insert(j);
	else {
		nb = new set<int>;
		nb -> insert(j);
		neighbors.insert(pair<int,set<int>*>(i,nb));
	}
//undirected
	it = neighbors.find(j);
	if (it!= neighbors.end())
		it->second->insert(i);
	else {
		nb = new set<int>;
		nb -> insert(i);
		neighbors.insert(pair<int,set<int>*>(j,nb));
	}
}

void power_grid::random_config(int degree) {

	cluster_list.resize(degree+1);

	//assert(degree>3);
	for (int i=0; i<size ; i++) {
    	std::random_device rd;
    	std::mt19937 gen(rd());
    	std::uniform_int_distribution<> dis(1, degree);
    	std::uniform_real_distribution<> dis2(0, 1);

//		int d = dis(gen);
    	int d = degree;
		sigma2[i][i] = dis2(gen) * 0.001;

		if (neighbors[i]==NULL)
			neighbors[i] = new set<int>;

		int actd = 0; //actual degree
		for (int j=0; j<d ; j++) {
	    	std::uniform_int_distribution<> dis3(0, size-1);
			int n = dis3(gen);

//			cout<<"for bus "<<i<<" picked "<<n<<endl;

			if (n!=i) {
				if (neighbors[n] != NULL) {
					if (neighbors[n]->size() >= degree) 
						continue;
				}
				else {
					neighbors[n] = new set<int>;
				}
				actd++;
				neighbors[i]->insert(n);
				neighbors[n]->insert(i);

				b[i][n] = dis2(gen) * 0.2;
				b[n][i] = b[i][n];

				g[i][n] = dis2(gen) * 0.2;
				g[n][i] = g[i][n];

				sigma2[i][n] = dis2(gen) * 0.001;
				sigma2[n][i] = sigma2[i][n];
			} 
		}
		cluster_list[actd].push_back(i);
	}
}


ast* power_grid::p(int i, vector<ast*>& v, vector<ast*>& t) {
	ast* result;
	result = num("0");
	
	if (neighbors[i]==NULL){
		return result;
	}

	for(set<int>::iterator it = neighbors[i]->begin();
								it != neighbors[i]->end(); it++) 
	{
		int j = *it ;
		result = add(
					result,
					mul(v[j],
						sub(sub(num("0"), 
					 	 	 	mul( num(g[i][j]),cos(sub(t[i], t[j])))
								),
							mul(num(b[i][j]), sin(sub(t[i], t[j])))
				 			)
				 		)
					);
	}

	result = mul(v[i], result);
	simplify(result);
	return result;
}


ast* power_grid::q(int i,vector<ast*>& v, vector<ast*>& t) {
	ast* result;
	result = num("0");

	if (neighbors[i]==NULL){
		return result;
	}

	for(set<int>::iterator it = neighbors[i]->begin();
								it != neighbors[i]->end(); it++) 
	{
		int j = *it ;
		result = add(result, 
					 mul(v[j],
					 	 sub( 
							mul(num(b[i][j]), cos(sub(t[i], t[j]))),
							mul(num(g[i][j]),sin(sub(t[i], t[j])))
							)
						)
					);
	}
	result = mul(v[i], result);
	simplify(result);
	return result;
}

ast* power_grid::p(int i, int j, vector<ast*>& v, vector<ast*>& t) {
	ast* result;
	result = mul(pow(volts[i],num("2")),num(g[i][j]));
	result = sub(result,mul(v[i],mul(v[j],
				mul(num(g[i][j]),cos(sub(t[i],t[j]))))));
	result = sub(result, mul(v[i],mul(v[j],
				mul(num(b[i][j]),sin(sub(t[i],t[j]))))));
	return result;
}


ast* power_grid::q(int i, int j, vector<ast*>& v, vector<ast*>& t) {
	ast* result;
	result = sub(num("0"),mul(pow(v[i],num("2")),num(g[i][j])));
	result = sub(result,mul(v[i],mul(v[j],
				mul(num(b[i][j]),cos(sub(t[i],t[j]))))));
	result = sub(result, mul(v[i],mul(v[j],
				mul(num(g[i][j]),sin(sub(t[i],t[j]))))));
	return result;
}

ast* power_grid::mf() {
	ast* result = top();
	for (int i=0; i<size; i++) {//todo: change this to e bounds
		ast* a1 = eq( zp[i][i], p(i,volts,phasors));  
		ast* a2 = eq( zq[i][i], q(i,volts,phasors));  
		result = land(result,land(a1, a2));
		if (neighbors[i]!=NULL) {
			for (set<int>::iterator it = neighbors[i]->begin();
					it != neighbors[i]->end(); it++) {
				int j = *it;
				if (j > i) {
					ast* a3 = eq(zp[i][j],p(i,j,volts,phasors));
					ast* a4 = eq(zq[i][j],q(i,j,volts,phasors));	
					result = land(result, land(a3, a4));				
				} 
			}
		}
	}
	simplify(result);
	return result;
}

ast* power_grid::mf(int bus) {//full neighborhood
	ast* result = top();
	for (int i=bus; i<bus+1; i++) {//todo: change this to e bounds
		ast* a1 = eq( zp[i][i], p(i,volts,phasors));  
		ast* a2 = eq( zq[i][i], q(i,volts,phasors));  
		result = land(result,land(a1, a2));
		if (neighbors[i]!=NULL) {
			for (set<int>::iterator it = neighbors[i]->begin();
					it != neighbors[i]->end(); it++) {
				int j = *it;
				//if (j > i) {
					ast* a3 = eq(zp[i][j],p(i,j,volts,phasors));
					ast* a4 = eq(zq[i][j],q(i,j,volts,phasors));	
					result = land(result, land(a3, a4));				
				//} 
			}
		}
	}
	simplify(result);
	return result;
}



ast* power_grid::mf(int i, int j, bool poq) {
	ast* result;
	if (i==j) {//bus
		if (poq) {//p
			result = eq( zp[i][i], p(i,volts,phasors));  
		}
		else {//q
			result = eq( zq[i][i], q(i,volts,phasors));  
		}
	}
	else {//line
		if (poq) {
			result = eq(zp[i][j],p(i,j,volts,phasors));
		}
		else {
			result = eq(zq[i][j],q(i,j,volts,phasors));	
		}
	}
	simplify(result);
	return result;
}

ast* power_grid::attack() {
	ast* result = top();
	for (int i=0; i<size; i++) {
		ast* a1 = eq( zph[i][i], add(zp[i][i], ap[i][i]));
		ast* a2 = eq( zqh[i][i], add(zq[i][i], aq[i][i]));
		result = land(result,land(a1, a2));
		if (neighbors[i]!=NULL) {
			for (set<int>::iterator it = neighbors[i]->begin();
					it != neighbors[i]->end(); it++) {
				int j = *it;
				if (j > i) {
					ast* a3 = eq(zph[i][j],add(zp[i][j], ap[i][j]));
					ast* a4 = eq(zqh[i][j],add(zq[i][j], aq[i][j]));	
					result = land(result, land(a3, a4));				
				} 
			}
		}		
	}
	simplify(result);
	return result;
}


ast* power_grid::attack(int i, int j, bool poq) {
	ast* result;
	if (i==j) {//bus
		if (poq) {//p
			result = eq( zph[i][i], add(zp[i][i], ap[i][i]));
		}
		else {//q
			result = eq( zqh[i][i], add(zq[i][i], aq[i][i]));
		}
	}
	else {//line
		if (poq) {
			result = eq(zph[i][j],add(zp[i][j], ap[i][j]));
		}
		else {
			result = eq(zqh[i][j],add(zq[i][j], aq[i][j]));	
		}
	}
	simplify(result);
	return result;
}

ast* power_grid::monitor(double tau) {
	ast* result = top();
	for (int i=0; i<size; i++) {//todo: change this to e bounds
		ast* a1 = sub( zph[i][i], p(i,vhat,thehat));  
		ast* a2 = sub( zqh[i][i], q(i,vhat,thehat));  
		ast* b1 = land(lt(a1,num(tau)), gt(a1, sub(num(0),num(tau))));
		ast* b2 = land(lt(a2,num(tau)), gt(a2, sub(num(0),num(tau))));
		result = land(result,land(b1, b2));
		if (neighbors[i]!=NULL) {
			for (set<int>::iterator it = neighbors[i]->begin();
					it != neighbors[i]->end(); it++) {
				int j = *it;
				if (j > i) {
					ast* a3 = sub(zph[i][j],p(i,j,vhat,thehat));
					ast* a4 = sub(zqh[i][j],q(i,j,vhat,thehat));	
					ast* b3 = land(lt(a3,num(tau)), gt(a3, sub(num(0),num(tau))));
					ast* b4 = land(lt(a4,num(tau)), gt(a4, sub(num(0),num(tau))));					
					result = land(result, land(b3, b4));				
				} 
			}
		}
	}
	simplify(result);
	return result;
}

ast* power_grid::monitor(double tau, int i, int j, bool poq) {
	ast* result;
	if (i==j) {//bus
		if (poq) {//p
			ast* a1 = sub(zph[i][i], p(i,vhat,thehat));  
			result = land(lt(a1,num(tau)), gt(a1, sub(num(0),num(tau))));
		}
		else {//q
			ast* a2 = sub( zqh[i][i], q(i,vhat,thehat));  
			result = land(lt(a2,num(tau)), gt(a2, sub(num(0),num(tau))));
		}
	}
	else {//line
		if (poq) {
			ast* a3 = sub(zph[i][j],p(i,j,vhat,thehat));
			result = land(lt(a3,num(tau)), gt(a3, sub(num(0),num(tau))));
		}
		else {
			ast* a4 = sub(zqh[i][j],q(i,j,vhat,thehat));	
			result = land(lt(a4,num(tau)), gt(a4, sub(num(0),num(tau))));
		}
	}
	simplify(result);
	return result;
}

ast* power_grid::unsafe(double eps) {
	ast* result = num(0);
	for (int i=0; i<size; i++) {
		ast* a1 = sub(volts[i], vhat[i]);
		ast* a2 = sub(phasors[i], thehat[i]);
		ast* a3 = add(pow(a1,num(2)),pow(a2,num(2)));
		result = add(result,a3);
	}
	result = geq(result,num(eps));
	simplify(result);
	return result;
}

ast* power_grid::unsafe(double eps, int i, int j) {
	ast* result = num(0);
	if (i==j){
		ast* a1 = sub(volts[i], vhat[i]);
		ast* a2 = sub(phasors[i], vhat[i]);
		result = add(pow(a1,num(2)),pow(a2,num(2)));
	}
	else {
		ast* b1 = sub(volts[i], vhat[i]);
		ast* b2 = sub(volts[j],vhat[j]);

		ast* b3 = sub(phasors[i], thehat[i]);
		ast* b4 = sub(phasors[j], vhat[j]);

		ast* b5 = add(add(pow(b1,num(2)),pow(b2,num(2))),
					  add(pow(b3,num(2)),pow(b4,num(2))));
		result = b5;
	}
	result = geq(result,num(eps));
	simplify(result);
	return result;
}


ast* power_grid::est() { 
	ast* result = top();
	ast* component = num("0");

	for (int i=0; i<size; i++) {
		for (int j=0; j<size ; j++) {
			component = add(
							component,
							div(
								mul(
									sub(zp[j][j],p(j,vhat,thehat)),
									partial(p(j,vhat,thehat),vhat[i])
									), 
								num(sigma2[j][j])
								)
							);
			component = add(
							component,
							div(
								mul(sub(zq[j][j],q(j,vhat,thehat)),
									partial(q(i,vhat,thehat),thehat[i])
									), 
								num(sigma2[j][j])
								)
							);
			if (neighbors[j]!=NULL) {
				for (set<int>::iterator it=neighbors[j]->begin();
									it != neighbors[j]->end(); it++) {
					int k = *it;
					if (k>j) {
						component = add(
										component,
										div(
											mul(
												sub(zp[j][k],p(j,k,vhat,thehat)),
												partial(p(j,k,vhat,thehat),vhat[i])
											), 
											num(sigma2[j][k])
										)
									);
						component = add(
										component,
										div(
											mul(sub(zq[j][k],q(j,k,vhat,thehat)),
											partial(q(j,k,vhat,thehat),thehat[i])
											), 
											num(sigma2[j][k])
										)
									);
					}
				}
			}
		}
		result = land(result, eq(component, num("0")));
		component = num("0");
	}
	simplify(result);
	return result;
}


ast* power_grid::esth() { 
	ast* result = top();
	ast* component = num("0");

	for (int i=0; i<size; i++) {
		for (int j=0; j<size ; j++) {
			component = add(
							component,
							div(
								mul(
									sub(zph[j][j],p(j,vhat,thehat)),
									partial(p(j,vhat,thehat),vhat[i])
									), 
								num(sigma2[j][j])
								)
							);
			component = add(
							component,
							div(
								mul(sub(zqh[j][j],q(j,vhat,thehat)),
									partial(q(i,vhat,thehat),thehat[i])
									), 
								num(sigma2[j][j])
								)
							);
			if (neighbors[j]!=NULL) {
				for (set<int>::iterator it=neighbors[j]->begin();
									it != neighbors[j]->end(); it++) {
					int k = *it;
					if (k>j) {
						component = add(
										component,
										div(
											mul(
												sub(zph[j][k],p(j,k,vhat,thehat)),
												partial(p(j,k,vhat,thehat),vhat[i])
											), 
											num(sigma2[j][k])
										)
									);
						component = add(
										component,
										div(
											mul(sub(zqh[j][k],q(j,k,vhat,thehat)),
											partial(q(j,k,vhat,thehat),thehat[i])
											), 
											num(sigma2[j][k])
										)
									);
					}
				}
			}
		}
		result = land(result, eq(component, num("0")));
		component = num("0");
	}
	simplify(result);
	return result;
}


//ast* fdi(int i, int j, bool poq) {//true is p, false is q

//}


ast* power_grid::esth(int i, int j) { 
	ast* result = top();
	ast* sum_p = num(0);
	ast* sum_q = num(0);
	ast* component = num(0);

	if (i!=j) {//bus
		sum_p = add(
						div(
							mul(
								sub(zph[i][j],p(i,j,vhat,thehat)),
								partial(p(i,j,vhat,thehat),vhat[i])
								), 
							num(sigma2[j][j])
							),
						div(
							mul(
								sub(zph[i][j],p(i,j,vhat,thehat)),
								partial(p(i,j,vhat,thehat),vhat[j])
								), 
							num(sigma2[j][j])
							)
					);

		sum_p = add(sum_p, add(
						div(
							mul(
								sub(zph[i][j],p(i,j,vhat,thehat)),
								partial(p(i,j,vhat,thehat),thehat[i])
								), 
							num(sigma2[j][j])
							),
						div(
							mul(
								sub(zph[i][j],p(i,j,vhat,thehat)),
								partial(p(i,j,vhat,thehat),thehat[j])
								), 
							num(sigma2[j][j])
							)
						)
				);
		sum_q = add(
						div(
							mul(
								sub(zqh[i][j],q(i,j,vhat,thehat)),
								partial(q(i,j,vhat,thehat),vhat[i])
								), 
							num(sigma2[j][j])
							),
						div(
							mul(
								sub(zqh[i][j],q(i,j,vhat,thehat)),
								partial(q(i,j,vhat,thehat),vhat[j])
								), 
							num(sigma2[j][j])
							)
					);

		sum_p = add(sum_p, add(
						div(
							mul(
								sub(zqh[i][j],q(i,j,vhat,thehat)),
								partial(q(i,j,vhat,thehat),thehat[i])
								), 
							num(sigma2[j][j])
							),
						div(
							mul(
								sub(zqh[i][j],q(i,j,vhat,thehat)),
								partial(q(i,j,vhat,thehat),thehat[j])
								), 
							num(sigma2[j][j])
							)
						)
				);
		result = land(eq(sum_p,num(0)), eq(sum_q,num(0)));
	}
	else {
		if (neighbors[i]!=NULL) {
			sum_p = div(
						mul(
							sub(zph[i][i],p(i,vhat,thehat)),
							partial(p(i,vhat,thehat),vhat[i])
							), 
						num(sigma2[i][i])
						);
			sum_q = div(
						mul(
							sub(zqh[i][i],q(i,vhat,thehat)),
							partial(q(i,vhat,thehat),vhat[i])
							), 
						num(sigma2[i][i])
						);
			for (set<int>::iterator it=neighbors[j]->begin();
									it != neighbors[j]->end(); it++) {
					int k = *it;
					sum_p = add(
								sum_p,
								div(
									mul(
										sub(zph[i][i],p(i,vhat,thehat)),
										partial(p(i,vhat,thehat),vhat[k])
										), 
									num(sigma2[j][k])
									)
								);
					sum_p = add(
								sum_p,
								div(
									mul(
										sub(zph[j][k],p(j,k,vhat,thehat)),
										partial(p(j,k,vhat,thehat),thehat[i])
										), 
									num(sigma2[j][k])
									)
								);
					sum_q = add(
								sum_q,
								div(
									mul(
										sub(zqh[i][i],q(i,vhat,thehat)),
										partial(q(i,vhat,thehat),vhat[k])
										), 
									num(sigma2[j][k])
									)
								);
					sum_p = add(
								sum_p,
								div(
									mul(
										sub(zqh[j][k],q(j,k,vhat,thehat)),
										partial(q(j,k,vhat,thehat),thehat[i])
										), 
									num(sigma2[j][k])
									)
								);
			}
			result = land(eq(sum_p,num(0)),eq(sum_q,num(0)));
		}
	}
	simplify(result);
	return result;
}

//line case
ast* power_grid::fdi(int i, int j, double tau, double eps) {

	ast* result;

//	ast* f2 = land(monitor(tau,i,j,true),monitor(tau,i,j,false));

//	cout<<"monitor:"<<endl<<f2 -> print_infix()<<endl;

	//ast* f3 = land(attack(i,j,true),attack(i,j,false));
	//cout<<"attack:"<<endl<<f3 -> print_infix()<<endl;

	//ast* f4 = land(esth(i,j),unsafe(eps,i,j));
	//cout<<"estimation and unsafe:"<<endl<<f4 -> print_infix()<<endl;

//	result = land(land(f1, land(f2, f3)),f5);
//	result = land(f1, land(f2, land(f3, f4)));

	//result = land(f2, land(f3,f4));

	result = mf(); 
	simplify(result);
	return result;
}

//bus case
ast* power_grid::fdi(int i, double tau, double eps) {
	ast* result = top();
	
//	ast* f1 = land(mf(i,j,true),mf(i,j,false));
//	cout<<"measurement function:"<<endl<<f1 -> print_infix()<<endl;

	ast* f;

	f = land(
				land(monitor(tau,i,i,true),monitor(tau,i,i,false)),
				land(
					land(attack(i,i,true),attack(i,i,false)),
					esth(i,i)
					)
			);
	
	result = land(result, f);

	if (neighbors[i]!=NULL) {
		for (set<int>::iterator it=neighbors[i]->begin(); 					
								it != neighbors[i]->end() ; it++ ) 
		{
			int j = *it;

			f = land(
					land(monitor(tau,i,j,true),monitor(tau,i,j,false)),
					land(
						land(attack(i,j,true),attack(i,j,false)),
						esth(i,j)
						)
				);
			result = land(result, f);

		}
		cout<<".";
	}
/*
	ast* f2 = land(monitor(tau,i,j,true),monitor(tau,i,j,false));
	cout<<"monitor:"<<endl<<f2 -> print_infix()<<endl;

	ast* f3 = land(attack(i,j,true),attack(i,j,false));
	cout<<"attack:"<<endl<<f3 -> print_infix()<<endl;

	ast* f4 = land(esth(i,j),unsafe(eps,i,j));
	cout<<"estimation and unsafe:"<<endl<<f4 -> print_infix()<<endl;

//	result = land(land(f1, land(f2, f3)),f5);
//	result = land(f1, land(f2, land(f3, f4)));

	result = land(f2, land(f3,f4));
*/

	simplify(result);
	return result;
}



void power_grid::dump() {	
	for (int i=0; i<size ; i++) {
		if (neighbors[i]!=NULL) {
			cout<<"Bus "<<i<<" is connected to: ";
			for (set<int>::iterator it=neighbors[i]->begin(); 
					it != neighbors[i]->end() ; it++ ) {
				cout<<"Bus "<<*it<<" ";
			}
			cout<<".";
		}
		else
			cout<<"Bus "<<i<<" is isolated.";

		cout<<endl;

		for (int j=0; j<size; j++) {
			cout <<"b"<<i<<j<<":"<<b[i][j]<<" ";
		}
		cout<<endl;

		for (int j=0; j<size; j++) {
			cout <<"g"<<i<<j<<":"<<g[i][j]<<" ";		
		}
		cout<<endl;

		for (int j=0; j<size; j++) {
			cout <<"sigma2"<<i<<j<<":"<<sigma2[i][j]<<" ";		
		}
		cout<<endl;

	}
}


