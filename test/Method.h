#include <vector>
#include <set>
#define _USE_MATH_DEFINES
#include <math.h>
#define MaxFuncs 10
#define  PI 3.141592653
typedef double(*pFunc)(double);
//����� ���������
struct CTrial {
	double x;
	double Value;
	int index;
};
inline bool operator<(const CTrial& t1, const CTrial& t2) { return (t1.x < t2.x); } typedef std::set<CTrial>::iterator pTrial;
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
	//���������� �� ���������
	CMethod() {}
	//���������� �� ���������
	~CMethod() {}
	//������ ������
	void Run();
	//������� ������
	pFunc Funcs[MaxFuncs];
	//������ ���������
	CTrial BestTrial;
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
	//���������� ���������� ���������
	CTrial MakeTrial(double);
	//������� ����������� ���������� ��������� 
	bool InsertTrial(CTrial);


};

//������� ������� ���������� ������
void CMethod::Run() {
	// ������������� �������� ������
	Init();
	//������� ��������� �� ���������
	bool stop = false;
	CTrial trial;
	int j = 2;
	while (!stop) {
		// ����������� ��������� I
		CalculateI();
		// ���������� �������� ������������� ��������� 
		CalculateM();
		// ���������� �������� Z
		CalculateZ();
		// ����� ��������� � ������������ ��������������� 
		pTrial t = FindMaxR();
		//���������� ��������� � ��������� No t
		pTrial t1 = t;
		t1--;
		if (t->index != t1->index) {
			trial = MakeTrial(0.5*(t->x + t1->x));
		}
		else {
			trial = MakeTrial(0.5*(t->x + t1->x) - (t->Value - t1->Value) / (M[t->index] * 2 * r[t->index]));
		}
		std::cout << "#" << j << " x = " << trial.x << " MaxIndex = " << MaxIndex << "\n";
		//������� ����������� ���������� ���������
		stop = InsertTrial(trial);
		j++;
	}
}

// ������������� �������� ������
void CMethod::Init() {
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
	std::cout << "#" << 1 << " x = " << trial.x << " MaxIndex = " << MaxIndex << "\n";
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
		for (unsigned i = 1; i < I[v].size(); i++) {
			tM = fabs(I[v][i]->Value - I[v][i - 1]->Value) / (I[v][i]->x - I[v][i - 1]->x);
			if (MaxM < tM)MaxM = tM;
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
		double deltax = i->x - i1->x;
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

//���������� ���������� ��������� 
CTrial CMethod::MakeTrial(double x) {
	CTrial Trial;
	Trial.x = x;
	//�������� �����������
	for (int i = 0; i < NumFuncs; i++) {
		Trial.Value = Funcs[i](x); if (i == NumFuncs - 1 || Trial.Value > 0) {
			Trial.index = i;
			break;
		}
	}
	return Trial;
}

//������� ����������� ���������� ��������� 
bool CMethod::InsertTrial(CTrial Trial) {
	//������� �����������
	std::pair<pTrial, bool> ins;
	ins = Trials.insert(Trial);
	//����� �������� ������� - ����
	if (!ins.second)return true;
	if (Trials.size() > 2) {
		pTrial j = ins.first, j1 = ins.first;
		j++;
		j1--;
		//��������� ������� ���������
		if ((j->x - j1->x) < eps)
			return true;
	}
	//������ �������� 
	if (Trial.index > MaxIndex || Trial.index == MaxIndex&&Trial.Value < BestTrial.Value) {
		BestTrial = Trial;
		MaxIndex = Trial.index;
	}
	return false;
}