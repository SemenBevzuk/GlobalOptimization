#include <math.h>
#include "vagris.h"

void setdim(int *n){
	*n = 2;
	return;
};

void setbounds(int n, double* lb, double* ub){
	int i;
	for(i=0;i < n; i++){
		lb[i] = 0.0;
		ub[i] = 1.001;
	}
	return;
};

double funct(int n, double* x, int Function_number) {
	int i;
	double f;

	set_random(Function_number);

	f = random_func(x[0],x[1]);

	return(f);
}

