#include <vector>
#include <set>
#define _USE_MATH_DEFINES
#include <math.h>
#define MaxFuncs 10
#define  PI 3.141592653
typedef double(*pFunc)(double);
//Точка испытания
struct CTrial {
	double x;
	double Value;
	int index;
};
inline bool operator<(const CTrial& t1, const CTrial& t2) { return (t1.x < t2.x); } typedef std::set<CTrial>::iterator pTrial;
//Индексный метод
class CMethod {
public:
	//Левая граница интервала поиска
	double a;
	//Правая граница интервала поиска
	double b;
	//Точность поиска
	double eps;
	//Параметр надежности
	double r[MaxFuncs];
	//Число функций задачи
	int NumFuncs;
	//Контруктор по умолчанию
	CMethod() {}
	//Деструктор по умолчанию
	~CMethod() {}
	//Запуск метода
	void Run();
	//Функции задачи
	pFunc Funcs[MaxFuncs];
	//Лучшее испытание
	CTrial BestTrial;
private:
	//Параметр Z алгоритма
	double Z[MaxFuncs];
	//Относительные первые разности
	double M[MaxFuncs];
	//Указатели на точки испытаний с фиксированным индексом 
	std::vector<pTrial> I[MaxFuncs];
	//Точки испытаний
	std::set<CTrial> Trials;
	//Максимальный индекс
	int MaxIndex;
	//Инициализация процесса поиска
	void Init();
	// Определение множества I
	void CalculateI(void);
	//Вычисление значений относительных разностей 
	void CalculateM();
	//Вычисление значений Z
	void CalculateZ();
	// Поиск интервала с максимальной характеристикой
	pTrial FindMaxR(void);
	//Проведение очередного испытания
	CTrial MakeTrial(double);
	//Вставка результатов очередного испытания 
	bool InsertTrial(CTrial);


};

//Главная функция индексного метода
void CMethod::Run() {
	// Инициализация процесса поиска
	Init();
	//Условие остановки не выполнено
	bool stop = false;
	CTrial trial;
	int j = 2;
	while (!stop) {
		// Определение множества I
		CalculateI();
		// Вычисление значений относительных разностей 
		CalculateM();
		// Вычисление значений Z
		CalculateZ();
		// Поиск интервала с максимальной характеристикой 
		pTrial t = FindMaxR();
		//Проведение испытания в интервале No t
		pTrial t1 = t;
		t1--;
		if (t->index != t1->index) {
			trial = MakeTrial(0.5*(t->x + t1->x));
		}
		else {
			trial = MakeTrial(0.5*(t->x + t1->x) - (t->Value - t1->Value) / (M[t->index] * 2 * r[t->index]));
		}
		std::cout << "#" << j << " x = " << trial.x << " MaxIndex = " << MaxIndex << "\n";
		//Вставка результатов очередного испытания
		stop = InsertTrial(trial);
		j++;
	}
}

// Инициализация процесса поиска
void CMethod::Init() {
	//Сначала максимальный индекс не определен
	MaxIndex = -1;
	//Первоначальное формирование информационных множеств 
	for (int v = 0; v < NumFuncs; v++)
		M[v] = 1;
	//Заносим в массив испытаний граничные точки
	CTrial trial;
	trial.x = a;
	trial.index = -1;
	InsertTrial(trial);
	trial.x = b;
	trial.index = -1;
	InsertTrial(trial);
	//Проводим испытаниe во внутренней точке, например, в серединной 
	trial = MakeTrial((a + b) / 2);
	InsertTrial(trial);
	std::cout << "#" << 1 << " x = " << trial.x << " MaxIndex = " << MaxIndex << "\n";
}

// Определение множества I
void CMethod::CalculateI(void) {
	//Предварительная очистка множества I
	for (int v = 0; v < NumFuncs; v++) {
		I[v].clear();
	}
	//Заполнение множества I
	for (int v = 0; v < NumFuncs; v++) {
		pTrial i;
		for (i = Trials.begin(); i != Trials.end(); i++) {
			if (i->index == v) {
				I[v].push_back(i);
			}
		}
	}
}

// вычисление значений относительных разностей 
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

// Вычисление значений Z
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

// Поиск интервала с максимальной характеристикой 
pTrial CMethod::FindMaxR(void) {
	pTrial t;
	pTrial i1 = Trials.begin();
	pTrial i = Trials.begin();
	i++;
	double MaxR = -HUGE_VAL;
	double R;
	//Вычисление характеристик
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

//Проведение очередного испытания 
CTrial CMethod::MakeTrial(double x) {
	CTrial Trial;
	Trial.x = x;
	//проверка ограничений
	for (int i = 0; i < NumFuncs; i++) {
		Trial.Value = Funcs[i](x); if (i == NumFuncs - 1 || Trial.Value > 0) {
			Trial.index = i;
			break;
		}
	}
	return Trial;
}

//Вставка результатов очередного испытания 
bool CMethod::InsertTrial(CTrial Trial) {
	//Вставка результатов
	std::pair<pTrial, bool> ins;
	ins = Trials.insert(Trial);
	//точки итераций совпали - стоп
	if (!ins.second)return true;
	if (Trials.size() > 2) {
		pTrial j = ins.first, j1 = ins.first;
		j++;
		j1--;
		//Выполнено условие остановки
		if ((j->x - j1->x) < eps)
			return true;
	}
	//Оценка значений 
	if (Trial.index > MaxIndex || Trial.index == MaxIndex&&Trial.Value < BestTrial.Value) {
		BestTrial = Trial;
		MaxIndex = Trial.index;
	}
	return false;
}