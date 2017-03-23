#include <iostream>
#include "IndexMethod.h"
#include "vagris.h"

int main(int argc, char* argv[]) {
	FILE *f;
	fopen_s(&f, "log\\results.csv", "w");

	int nf;
	CMethod im;
	im.a = 0;
	im.b = 1;
	im.eps = 0.002;
	im.N = 2;
	im.m = 10;
	im.NumFuncs = 1;
	im.SetSearchAreas(0, 1.001, 0, 1.001);
	im.Funcs[0] = random_func;
	im.r[0] = 4;
	Result res;
	int right = 0;
	bool IsCorrect;
	double real_x, real_y, real_value;
	double eps = 0.01; 
	int Correct[101];
	double delta[101];
	Result results[101];
	for (nf = 1; nf <= 100; nf++) {
		IsCorrect = false;
		set_random(nf);
		//res = im.Run();
		res = im.Run(nf, eps);
		results[nf] = res;
		real_x = rand_minimums[(nf - 1) * 2];
		real_y = rand_minimums[(nf - 1) * 2 + 1];
		real_value = random_func(real_x,real_y);

		delta[nf] = abs(real_value - res.Value);
		Correct[nf] = 0;
		if ((real_x-res.x<eps) && (real_y-res.y<eps))
		{
			right++;
			IsCorrect = true;
			Correct[nf] = 1;
		}
		
		//std::cout << "#" << nf << std::endl;
		//std::cout << "	Min is : (" <<res.x << "; " << res.y << "), Value = "<<res.Value <<" Points: " << res.points <<std::endl;
		//std::cout << "	Global min is : (" << real_x << "; " << real_y << "), Value = " << real_value << std::endl;

		fprintf(f, "%f;%f;%f;%f;%f;%f;%d;%d;\n", res.x, res.y, res.Value, real_x, real_y, real_value, res.points, IsCorrect);
	} 
	std::cout << "Ready!" << std::endl;
	std::cout << "Correct = " << right << "%" << std::endl << std::endl;
	for (int i = 1; i <= 100; i++)
	{
		if (Correct[i] == 0)
		{
			set_random(i);
			std::cout << "#" << i << " Delta_value = " << delta[i] << std::endl;
			std::cout << "	Calculated: (x,y) = ( " << results[i].x << "; " << results[i].y << "); Value = " << results[i].Value << std::endl;
			std::cout << "	Global	  : (x,y) = ( " << rand_minimums[(i - 1) * 2] << "; " << rand_minimums[(i - 1) * 2 + 1]
				<< "); Value = " << random_func(rand_minimums[(i - 1) * 2], rand_minimums[(i - 1) * 2 + 1]) << std::endl;
			std::cout << std::endl;
		}
	}
	fclose(f);
	return 0;
}