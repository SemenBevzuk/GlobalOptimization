#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "headers_direct.h"

void setdim(int *n);
void setbounds(int n, double* lb, double* ub);
double funct(int n, double* x);

int main(int argc, int **argv){
	int i, n, maxint;
	double *lb, *ub, *xott;
	double fbest, fglob;

	int Number_of_experiments = 1;	//для тестов
	int Function_number = 2;		//для тестов
	
	setdim(&n);
	lb    = (double *)malloc(n*sizeof(double));
	ub    = (double *)malloc(n*sizeof(double));
	xott  = (double *)malloc(n*sizeof(double));

	printf("problem dimension %d\n\n",n);

	setbounds(n, lb, ub);

	for(i=0;i<n;i++) {
		printf("lb[%d]=%f ub[%d]=%f\n",i,lb[i],i,ub[i]);
	}
	printf("\n");
	fglob = -100.0;
	maxint = 15000;

	direct(n, lb, ub, maxint, fglob, xott, &fbest, funct, &Number_of_experiments, Function_number);
	fglob = fbest;
	//direct(n,lb,ub,maxint,fglob,xott,&fbest,funct);

	printf("fbest=%f\n",fbest);
	for(i=0;i<n;i++) {
		printf("xbest[%d]=%f\n",i,xott[i]);
	}
	printf("Number_of_experiments = %d\n", Number_of_experiments);
	return 0;
}

