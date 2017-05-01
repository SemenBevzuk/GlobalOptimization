#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "headers_direct.h"

//NLOPT см Direct

void setdim(int *n);
void setbounds(int n, double* lb, double* ub);
double funct(int n, double* x);

int main(int argc, int **argv){
	int i, n, maxint;
	double *lb, *ub, *xott;
	double fbest, fglob;

	int Number_of_experiments = 0;	//для тестов
	int Count_of_experiments = 0;
	int Function_number = 3;		//для тестов
	
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
	maxint = 1000;//15000;

	direct(n, lb, ub, maxint, fglob, xott, &fbest, funct, &Number_of_experiments, Function_number);
	fglob = fbest;
	Count_of_experiments += Number_of_experiments;
	direct(n, lb, ub, maxint, fglob, xott, &fbest, funct, &Number_of_experiments, Function_number);
	Count_of_experiments += Number_of_experiments;

	printf("fbest=%f\n",fbest);
	for(i=0;i<n;i++) {
		printf("xbest[%d]=%f\n",i,xott[i]);
	}
	printf("Number_of_experiments = %d\n", Count_of_experiments);

	char ch;
	scanf_s(&ch);
	return 0;
}

