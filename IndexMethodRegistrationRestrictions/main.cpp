#include <iostream>
#include <math.h>
#include "IndexMethod.h"

using namespace std;
// Первое ограничение
double g1(double x) {
	return exp(-0.5*x)*sin(6 * x - 1.5);
}
// Второе ограничение
double g2(double x) {
	return fabs(x)*sin(2 * x * PI - 0.5);
}
// Целевая функция
double func(double x) {
	return cos(18 * x - 3)*sin(10 * x - 7) + 1.5;
}

int main(int argc, char* argv[]) {
	IndexMethod Method;
	Method.SetInterval(0.6, 2.2);
	Method.SetAccuracy(0.001);
	Method.NumFuncs = 3;
	Method.Funcs[0] = g1;
	Method.Funcs[1] = g2;
	Method.Funcs[2] = func;
	for (int i = 0; i < 3; i++)
		Method.r[i] = 2;
	Method.Run();
	double opt_x;
	double opt_y;
	opt_x = Method.BestTrial.x;
	opt_y = Method.BestTrial.value;
	cout << "X = "<<opt_x<<endl;
	cout << "Y = " << opt_y << endl;
	return 0;
}