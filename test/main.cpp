//Решение тестового примера индексным методом 
#include <iostream>
#include "method.h"
#include <iostream>

// Первое ограничение
double f1(double x) {
	return exp(-0.5*x)*sin(6 * x - 1.5);
}
// Второе ограничение
double f2(double x) {
	return fabs(x)*sin(2 * x * PI - 0.5);
}
// Целевая функция
double fi(double x) {
	return cos(18 * x - 3)*sin(10 * x - 7) + 1.5;
}

using namespace std;

int main(int argc, char* argv[]) {
	CMethod im;
	im.a = 0.6;
	im.b = 2.2;
	im.eps = 0.001;
	im.NumFuncs = 3;
	im.Funcs[0] = f1;
	im.Funcs[1] = f2;
	im.Funcs[2] = fi;
	for (int i = 0; i<3; i++)
		im.r[i] = 2;
	im.Run();
	cout << "Xmin = " << im.BestTrial.x <<endl << "Ymin = " << im.BestTrial.Value << endl;
	return 0;
}