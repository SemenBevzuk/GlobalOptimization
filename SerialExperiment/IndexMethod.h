#include <vector>
#include <set>
#define _USE_MATH_DEFINES
#include <math.h>
#include <fstream>

#include "vagris.h"

#define MaxFuncs 10
#include "map.h"
#include <string>

typedef double(*pFunc)(double, double);

double sign(double val) {
	return (val>0) ? (1) : ((val<0) ? (-1) : (0));
}

struct Result {
	double x;
	double y;
	double Value;
	int points;
};
//����� ���������
struct CTrial {
	double Value;	//�������� ������� � �����
	int index;		//������ �����
	double x;		//����� ����� �������, �������� ���������� ����� FindReal_...
};

struct SearchArea {
	double a;
	double b;
};

inline bool operator<(const CTrial& t1, const CTrial& t2) 
{ 
	return (t1.x < t2.x); 
} 

typedef std::set<CTrial>::iterator pTrial;
//��������� �����
class CMethod {
public:
	//����� ������� ��������� ������
	double a;
	//������ ������� ��������� ������
	double b;
	//�������� ������
	double eps;
	//�������� ����������
	double r[MaxFuncs];
	//����� ������� ������
	int NumFuncs;
	//�����������
	int N;
	//��������� ��������
	int m;
	//���������� ������� ������
	void SetSearchAreas(double a1, double b1, double a2, double b2);
	//���������� �� ���������
	CMethod() {}
	//���������� �� ���������
	~CMethod() {}
	//������ ������
	Result Run(int nfunc);
	//������ ������ c ������������ ��������� ������ ��������������� 
	Result Run(int nfunc, double eps);
	//������� ������
	pFunc Funcs[MaxFuncs];
	//������ ���������
	CTrial BestTrial;

	//������� ��������
	void Clean();
private:
	//�������� Z ���������
	double Z[MaxFuncs];
	//������������� ������ ��������
	double M[MaxFuncs];
	//��������� �� ����� ��������� � ������������� �������� 
	std::vector<pTrial> I[MaxFuncs];
	//����� ���������
	std::set<CTrial> Trials;
	//������������ ������
	int MaxIndex;
	// ������������ ������� � ������������, ��
	double max_Length_SearchArea = -1;
	//����������� �� ������� ������
	SearchArea SearchAreas[2];


	//������������� �������� ������
	void Init();
	// ����������� ��������� I
	void CalculateI(void);
	//���������� �������� ������������� ��������� 
	void CalculateM();
	//���������� �������� Z
	void CalculateZ();
	// ����� ��������� � ������������ ���������������
	pTrial FindMaxR(void);
	//����� �������� X �� ����� �� [0,1]
	double FindRealX(double x);
	//����� �������� Y �� ����� �� [0,1]
	double FindRealY(double x);
	//���������� ���������� ���������
	CTrial MakeTrial(double);
	//������� ����������� ���������� ��������� 
	bool InsertTrial(CTrial);
	//����� ������������ ����� ��������� �� ������� ������
	double FindMaxLength();

	//����� ����� ��������� ������
	void PrintPoints(int nf);

};

//������� ������� ���������� ������
Result CMethod::Run(int nfunc){
	// ������������� �������� ������
	Init();
	//������� ��������� �� ���������
	bool stop = false;
	CTrial trial;
	int count = 1;
	while (!stop) {
		// ����������� ��������� I
		CalculateI();
		// ���������� �������� ������������� ��������� 
		CalculateM();
		// ���������� �������� Z
		CalculateZ();
		// ����� ��������� � ������������ ��������������� 
		pTrial t = FindMaxR();
		//���������� ��������� � ��������� t
		pTrial t1 = t;
		t1--;
		if (t->index != t1->index) {
			trial = MakeTrial(0.5*(t->x + t1->x));
		}
		else {
			trial = MakeTrial(0.5*(t->x + t1->x) - sign(t->Value - t1->Value)*pow(fabs(t->Value - t1->Value) / M[t->index], N)*(1.0 / (2 * r[t->index])));
		}
		count++;
		//������� ����������� ���������� ���������
		stop = InsertTrial(trial);
	}
	//fclose(f);
	Result res;
	res.x = FindRealX(BestTrial.x);
	res.y = FindRealY(BestTrial.x);
	res.Value = BestTrial.Value;
	res.points = Trials.size();
	//std::cout << "BEST:" << " X = " << FindRealX(BestTrial.x) << " Y = " << FindRealY(BestTrial.x) << " Z = " << BestTrial.Value << "\n\n";
	PrintPoints(nfunc);

	Clean();
	return res;
}

Result CMethod::Run(int nfunc, double eps) {
	// ������������� �������� ������
	Init();
	//������� ��������� �� ���������
	bool stop = false;
	CTrial trial;
	int count = 1;

	set_random(nfunc);
	double real_x, real_y, real_value;
	double delta_value;

	while (!stop) {
		// ����������� ��������� I
		CalculateI();
		// ���������� �������� ������������� ��������� 
		CalculateM();
		// ���������� �������� Z
		CalculateZ();
		// ����� ��������� � ������������ ��������������� 
		pTrial t = FindMaxR();
		//���������� ��������� � ��������� t
		pTrial t1 = t;
		t1--;
		if (t->index != t1->index) {
			trial = MakeTrial(0.5*(t->x + t1->x));
		}
		else {
			trial = MakeTrial(0.5*(t->x + t1->x) - sign(t->Value - t1->Value)*pow(fabs(t->Value - t1->Value) / M[t->index], N)*(1.0 / (2 * r[t->index])));
		}
		count++;
		//������� ����������� ���������� ���������
		stop = InsertTrial(trial);

		real_x = rand_minimums[(nfunc - 1) * 2];
		real_y = rand_minimums[(nfunc - 1) * 2 + 1];
		real_value = random_func(real_x, real_y);
		delta_value = abs(trial.Value - real_value);
		if (delta_value<eps)
		{
			BestTrial = trial;
			stop = true;
		}
	}
	Result res;
	res.x = FindRealX(BestTrial.x);
	res.y = FindRealY(BestTrial.x);
	res.Value = BestTrial.Value;
	res.points = Trials.size();

	Clean();
	return res;
}

// ������������� �������� ������
void CMethod::Init() {
	//fopen_s(&f, "data.csv", "w");

	//������� ������������ ������ �� ���������
	MaxIndex = -1;
	//�������������� ������������ �������������� �������� 
	for (int v = 0; v < NumFuncs; v++)
		M[v] = 1;
	//������� � ������ ��������� ��������� �����
	CTrial trial;
	trial.x = a;
	trial.index = -1;
	InsertTrial(trial);
	trial.x = b;
	trial.index = -1;
	InsertTrial(trial);
	//�������� ��������e �� ���������� �����, ��������, � ���������� 
	trial = MakeTrial((a + b) / 2);
	InsertTrial(trial);
	double x = FindRealX(trial.x);
	double y = FindRealY(trial.x);
	//fprintf(f, "%f;%f;\n", x, y);
}

// ����������� ��������� I
void CMethod::CalculateI(void) {
	//��������������� ������� ��������� I
	for (int v = 0; v < NumFuncs; v++) {
		I[v].clear();
	}
	//���������� ��������� I
	for (int v = 0; v < NumFuncs; v++) {
		pTrial i;
		for (i = Trials.begin(); i != Trials.end(); i++) {
			if (i->index == v) {
				I[v].push_back(i);
			}
		}
	}
}

// ���������� �������� ������������� ��������� 
void CMethod::CalculateM() {
	for (int v = 0; v < NumFuncs; v++) {
		if (I[v].size() < 2) {
			M[v] = 1;
			continue;
		}
		double MaxM = -HUGE_VAL;
		double tM;
		for (int i = 1; i < I[v].size(); i++) {
			tM = fabs(I[v][i]->Value - I[v][i - 1]->Value) / pow(fabs(I[v][i]->x - I[v][i - 1]->x), 1.0 / N);
			if (MaxM < tM) MaxM = tM;
		}
		if (MaxM > 0)M[v] = MaxM;
		else M[v] = 1;
	}
}

// ���������� �������� Z
void CMethod::CalculateZ() {
	for (int v = 0; v < NumFuncs; v++) {
		if (v < MaxIndex) {
			Z[v] = 0;
			continue;
		}
		Z[v] = BestTrial.Value;
		break;
	}
}

// ����� ��������� � ������������ ��������������� 
pTrial CMethod::FindMaxR(void) {
	pTrial t;
	pTrial i1 = Trials.begin();
	pTrial i = Trials.begin();
	i++;
	double MaxR = -HUGE_VAL;
	double R;
	//���������� �������������
	for (i; i != Trials.end(); i++, i1++) {
		double deltax = pow(fabs(i->x - i1->x), 1.0 / N);
		if (i->index == i1->index) {
			int v = i->index;
			R = deltax + (i->Value - i1->Value)*(i->Value - i1->Value) / (deltax*M[v] * M[v] * r[v] * r[v]) - 2 * (i->Value + i1->Value - 2 * Z[v]) / (r[v] * M[v]);
		}
		if (i->index > i1->index) {
			int v = i->index;
			R = 2 * deltax - 4 * (i->Value - Z[v]) / (r[v] * M[v]);
		}
		if (i->index<i1->index) {
			int v = i1->index; R = 2 * deltax - 4 * (i1->Value - Z[v]) / (r[v] * M[v]);
		}
		if (R>MaxR) {
			MaxR = R;
			t = i;
		}
	}
	return t;
}

inline double CMethod::FindRealX(double x)
{
	double temp_x;
	double coords[2];
	mapd(x, m, coords, N, 1);
	double p_x = coords[0] + 1 / 2.0;
	//temp_x = coords[0] * max_Length_SearchArea + 0.5*(SearchAreas[0].a + SearchAreas[0].b);
	temp_x = p_x*(SearchAreas[0].b - SearchAreas[0].a) + SearchAreas[0].a;
	return temp_x;
}


inline double CMethod::FindRealY(double x) 
{
	double temp_y;
	double coords[2];
	mapd(x, m, coords, N, 1);
	double p_y = coords[1] + 1 / 2.0;
	//temp_y = coords[1] * max_Length_SearchArea + 0.5*(SearchAreas[1].a + SearchAreas[1].b);
	temp_y = p_y*(SearchAreas[1].b - SearchAreas[1].a) + SearchAreas[1].a;
	return temp_y;
}

inline void  CMethod::SetSearchAreas(double a1, double b1, double a2, double b2) {
	SearchAreas[0].a = a1;
	SearchAreas[0].b = b1;
	SearchAreas[1].a = a2;
	SearchAreas[1].b = b2;
	max_Length_SearchArea = FindMaxLength();
}

inline double  CMethod::FindMaxLength() {
	double temp;
	double maxLength = -1;
	for (int i = 0; i < 2; i++) {
		temp = SearchAreas[i].b - SearchAreas[i].a;
		if (temp>maxLength) {
			maxLength = temp;
		}
	}
	return maxLength;
}

//���������� ���������� ��������� 
CTrial CMethod::MakeTrial(double x) {
	CTrial Trial;
	Trial.x = x;
	//�������� �����������
	for (int i = 0; i < NumFuncs; i++) {
		Trial.Value = Funcs[i](FindRealX(x), FindRealY(x)); 
		if (i == NumFuncs - 1 || Trial.Value > 0) {
			Trial.index = i;
			break;
		}
	}
	//fprintf(f, "%f;%f;\n", FindRealX(Trial.x), FindRealY(Trial.x));
	return Trial;
}

//������� ����������� ���������� ��������� 
bool CMethod::InsertTrial(CTrial Trial) {
	//������� �����������
	std::pair<pTrial, bool> ins;
	ins = Trials.insert(Trial);
	//����� �������� ������� - ����
	if (!ins.second)
		return true;
	if (Trials.size() > 2) {
		pTrial j = ins.first, j1 = ins.first;
		j++;
		j1--;

		//��������� ������� ���������
		//std::cout << pow(fabs(j->x - j1->x), 1.0 / N) << "\n";
		if (pow(fabs(j->x - j1->x), 1.0/N) < eps)
			return true;
	}
	//������ �������� 
	if (Trial.index > MaxIndex || Trial.index == MaxIndex && Trial.Value < BestTrial.Value) {
		BestTrial = Trial;
		MaxIndex = Trial.index;
	}
	return false;
}

inline void CMethod::PrintPoints(int nf)
{
	std::string file_name = "log\\function_points_" + std::to_string(nf) + ".csv";
	std::ofstream current_func_points(file_name);
	double temp_x, temp_y;

	for (std::set<CTrial>::const_iterator iter = Trials.begin(); iter != Trials.end(); iter++) {
		temp_x = FindRealX(iter->x);
		temp_y = FindRealY(iter->x);
		current_func_points << temp_x << ';' << temp_y << std::endl;
	}

	current_func_points.close();
}

inline void CMethod::Clean()
{
	Trials.clear();
	for (int i = 0; i < MaxFuncs; i++)
		I[i].clear();
}
