#include <math.h>
#include <stdio.h>
#include <stdlib.h>

double funcA(double a){
	return a * a * a - 5 * a -9;
}

double derivFuncA(double a){
	return 3 * a * a - 5;
}

double funcB(double x){
	return exp(x) - 3 * x;
}


double derivFuncB(double a){
	return exp(a) - 3;
}

double funcC(double x){
	return cos(x);
}


double derivFuncC(double a){
	return -sin(a);
}

double NewtonRaphson(double (*function)(double), double (*derivativeFunction)(double), double x, double epsilon){
	double  diff;
	do{
		diff = (*function)(x)/(*derivativeFunction)(x);
		x = x - diff;
	}while(abs(diff) >= epsilon);

	return x;
}


int main() {
	printf("%f\n", NewtonRaphson(funcC, derivFuncC, 0.5, 0.001));	
}

