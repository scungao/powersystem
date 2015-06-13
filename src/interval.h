#ifndef INTERVAL_H
#define INTERVAL_H

class interval {
	double	lower_bound;
	double	upper_bound;
public:
	inline interval() {}
	inline interval(double lb, double ub)
		:lower_bound(lb), upper_bound(ub) {}
	inline ~interval() {}
	inline double get_length() 
		{ return (upper_bound - lower_bound) ; }
	inline double get_lower () 
		{ return lower_bound; }
	inline double get_upper ()
		{ return upper_bound; }
	inline void set_lower (double lb) { lower_bound = lb; }
	inline void set_upper (double ub) { upper_bound = ub; }
};

#endif