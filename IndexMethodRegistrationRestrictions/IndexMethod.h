#include <vector>
#include <set>
#include <math.h>
#define  PI 3.141592653

#define MaxFuncs 10
#define MaxTestPoints 1000
typedef double(*pFunc)(double);

struct TestPoint {
	double x;
	double value;
	int index;
};

class IndexMethod {
public:
	double a;
	double b;
	double eps;
	double r[MaxFuncs];
	int NumFuncs;
	void SetInterval(double a_, double b_);
	void SetAccuracy(double e);
	IndexMethod();
	~IndexMethod() {}
	void Run();
	pFunc Funcs[MaxFuncs];

	TestPoint BestTrial;
private:
	//Параметр Z 
	double Z[MaxFuncs];
	//Относительные первые разности
	double M[MaxFuncs];
	std::vector<TestPoint> I[MaxFuncs];
	TestPoint TestPoints[MaxTestPoints];
	int LastFreePoint = 0;
	int MaxIndex;

	TestPoint MakeTrial(double);
	void InsertTrial(TestPoint);
	void CalculateI();
	void FindMaxIndex();
	void CalculateM();
	void CalculateZ();
	int FindIntervalWhithMaxR();
	//void Initialization();
	void quickSort(int l, int r);
};

inline void IndexMethod::SetInterval(double a_, double b_) {
	a = a_; b = b_;
}

inline void IndexMethod::SetAccuracy(double e) {
	eps = e;
}

inline IndexMethod::IndexMethod()
{
	for (int i = 0; i < NumFuncs; i++)
	{
		I[i] = std::vector<TestPoint>();
	}
}

inline void IndexMethod::Run() {
	MaxIndex = -1;
	for (int v = 0; v < NumFuncs; v++)
		M[v] = 1;
	//Заносим в массив границы
	TestPoint trial;
	trial.x = a;
	trial.index = -1;
	InsertTrial(trial);
	trial.x = b;
	trial.index = -1;
	InsertTrial(trial);
	//считаем первую точку
	trial = MakeTrial((a + b) / 2.0);
	InsertTrial(trial);
	std::cout << "#" << 1 << " x = " << trial.x << " MaxIndex = " << MaxIndex << "\n";
	int j = 2;
	int CurrentInterval;
	bool flag = true;
	while (flag) {
		quickSort(0, LastFreePoint - 1);
		CalculateI();
		FindMaxIndex();
		CalculateM();
		CalculateZ();
		CurrentInterval = FindIntervalWhithMaxR();
		if (TestPoints[CurrentInterval].index != TestPoints[CurrentInterval-1].index) {
			trial = MakeTrial(0.5*(TestPoints[CurrentInterval].x + TestPoints[CurrentInterval-1].x));
		}
		else {
			trial = MakeTrial(0.5*(TestPoints[CurrentInterval].x + TestPoints[CurrentInterval - 1].x) 
							  - 0.5*(TestPoints[CurrentInterval].value - TestPoints[CurrentInterval - 1].value) / (M[TestPoints[CurrentInterval].index] * r[TestPoints[CurrentInterval].index]));
		}
		InsertTrial(trial);
		std::cout << "#" << j << " x = " << trial.x  <<" MaxIndex = "<< MaxIndex<< "\n";
		if (TestPoints[CurrentInterval].x - TestPoints[CurrentInterval - 1].x < eps)
		{
			BestTrial.x = trial.x;
			BestTrial.value = trial.value;
			BestTrial.index = trial.index;
			flag = false;
		}

		j++;
	}
}

inline TestPoint IndexMethod::MakeTrial(double x) {
	TestPoint test;
	test.x = x;
	for (int i = 0; i < NumFuncs; i++) {
		test.value = Funcs[i](x);
		if (test.value > 0) {
			test.index = i;
			break;
		}
	}
	return test;
}

inline void IndexMethod::InsertTrial(TestPoint trial) {
	TestPoints[LastFreePoint] = trial;
	LastFreePoint++;
}

inline void IndexMethod::CalculateI() {
	for (int i = 0; i < NumFuncs; i++) {
		I[i].clear();
	}
	MaxIndex = 0;
	for (int v = 0; v < NumFuncs; v++) {
		for (int i = 1; i < LastFreePoint-1; i++) 
		{
			if (TestPoints[i].index == v)
			{
				I[v].push_back(TestPoints[i]);
			}
		}
	}
}

inline void IndexMethod::FindMaxIndex()
{
	MaxIndex = TestPoints[0].index;
	for (int i = 1; i < LastFreePoint; i++)
	{
		if (TestPoints[i].index>MaxIndex)
		{
			MaxIndex = TestPoints[i].index;
		}
	}
}

inline void IndexMethod::CalculateM()
{
	for (int v = 0; v < NumFuncs; v++) 
	{
		if (I[v].size() < 2)
		{
			M[v] = 1;
			continue;
		}

		double Max = -HUGE_VAL;
		double temp;
		for (int i = 1; i < I[v].size(); i++) {
			temp = abs(I[v][i].value - I[v][i - 1].value) / (I[v][i].x - I[v][i - 1].x);
			if (Max < temp)
			{
				Max = temp;
			}
		}
		if (Max > 0)
		{
			M[v] = Max;
		}
		else
		{
			M[v] = 1;
		}
	}
}

inline void IndexMethod::CalculateZ()
{
	for (int v = 0; v < NumFuncs; v++) {
		if (v < MaxIndex) {
			Z[v] = 0;
			continue;
		}
		if (I[v].size() != 0)
		{
			double min = I[v][0].value;
			for (int i = 1; i < I[v].size(); i++) {
				if (I[v][i].value<min) {
					min = I[v][i].value;
				}
			}
			Z[v] = min;
		}
		else
		{
			Z[v] = 0;
		}
	}
}

inline int IndexMethod::FindIntervalWhithMaxR()
{
	double MaxR = -HUGE_VAL;
	double R;
	int index = 1;
	for (int i = 1; i < LastFreePoint; i++) 
	{
		double deltaX = TestPoints[i].x - TestPoints[i-1].x;
		if (TestPoints[i].index == TestPoints[i-1].index) {
			int v = TestPoints[i].index;
			R = deltaX + (TestPoints[i].value - TestPoints[i - 1].value)*(TestPoints[i].value - TestPoints[i - 1].value)/(deltaX*M[v] * M[v] * r[v] * r[v])
				- 2.0*(TestPoints[i].value + TestPoints[i - 1].value - 2.0 * Z[v]) / (r[v] * M[v]);
		}
		if (TestPoints[i].index > TestPoints[i - 1].index) {
			int v = TestPoints[i].index;;
			R = 2.0 * deltaX - 4.0 * (TestPoints[i].value -Z[v]) / (r[v] * M[v]);
		}
		if (TestPoints[i].index < TestPoints[i - 1].index) {
			int v = TestPoints[i-1].index;
			R = 2 * deltaX - 4.0 * (TestPoints[i-1].value - Z[v]) / (r[v] * M[v]);
		}
		if (R>MaxR) {
			MaxR = R;
			index = i;
		}
	}
	return index;
}

inline void IndexMethod::quickSort(int l, int r) {
	double x = TestPoints[l + (r - l) / 2].x;
	int i = l;
	int j = r;
	while (i <= j) {
		while (TestPoints[i].x < x) i++;
		while (TestPoints[j].x > x) j--;
		if (i <= j) {
			TestPoint temp = TestPoints[i];
			TestPoints[i] = TestPoints[j];
			TestPoints[j] = temp;
			i++;
			j--;
		}
	}
	if (i < r)
		quickSort(i, r);

	if (l < j)
		quickSort(l, j);
}