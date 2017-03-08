#include <iostream>
#include "IndexMethod.h"
// Первое ограничение
double f1(double x, double y) {
	return 0.01*(pow(x - 2.2, 2) + pow(y - 1.2, 2) - 2.25);
	//return x + y - 1;
}
// Второе ограничение
double f2(double x, double y) {
	return 100 * (1 - pow(x - 2, 2) / 1.44 - pow(0.5*y, 2));
	//return x - y + 1;
}
// Третье ограничение
double f3(double x, double y) {
	return 10 * (y - 1.5 - 1.5*sin(6.283*(x - 1.75)));
}
// Целевая функция
double fi(double x, double y) {
	return -1.5*x*x*exp(1 - x*x - 20.25*pow(x - y, 2.0)) - pow(0.5*(x - 1)*(y - 1), 4)*exp(2 - pow(0.5*(x - 1), 4) - pow(y - 1, 4));
	//return pow(x - 1, 2) + pow(y-1,2);
}
int main(int argc, char* argv[]) {
	CMethod im;
	im.a = 0;
	im.b = 1;
	im.eps = 0.001;
	im.N = 2;
	im.m = 10;

	im.NumFuncs = 4;
	im.SetSearchAreas(0,4,-1,3);
	im.Funcs[0] = f1;
	im.Funcs[1] = f2;
	im.Funcs[2] = f3;
	im.Funcs[3] = fi;
	for (int i = 0; i<im.NumFuncs; i++)
		im.r[i] = 2;
	im.Run();
	//std::cout << "Xmin = " << im.BestTrial.x << " Ymin = " << im.BestTrial.Value << std::endl; 
	return 0;
}