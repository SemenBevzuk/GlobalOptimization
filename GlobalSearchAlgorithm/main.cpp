#include <stdlib.h>
#include <iostream>

#define MaximumNumberOfExperiments 200
#define EPS 0.1

using namespace std;

double ObjectiveFunction(double x) //целевая функция
{
	return x*x-cos(18*x);//(x*x+1); 
}

class SearchInformationElement
{
	private:
		double x;
		double z;
	public:
		SearchInformationElement() {
			x = 0;
			z = 0;
		}
		SearchInformationElement(double point)
		{
			x = point;
			z = ObjectiveFunction(point);
		}
		void SetPoint(double point)
		{
			x = point;
			z = ObjectiveFunction(point);
		}
		double GetPoint()
		{
			return x;
		}
		double GetFunctionValue() {
			return z;
		}
};

class GlobalSearchAlgorithm
{
	private:
		SearchInformationElement SearchInformation[MaximumNumberOfExperiments];
		//list<SearchInformationElement> OrderedInformation;
		double IntervalCharacteristic[MaximumNumberOfExperiments];//кроме нуля
		int IndexMaxIntervalCharacteristic;
		int LastFreeExperiment = 0;

		double r = 2.0;
		double M;
		double m;

		void SetExperiment(double x) {
			if (LastFreeExperiment<MaximumNumberOfExperiments) {
				SearchInformation[LastFreeExperiment].SetPoint(x);
				LastFreeExperiment++;

				//OrderedInformation.push_back(SearchInformationElement(x));
				//OrderedInformation.sort();
			}
			else {
				cout << "Выработан ресурс!";
			}
		}

		void quickSort(int l, int r) {
			double x = SearchInformation[l + (r - l) / 2].GetPoint();
			int i = l;
			int j = r;
			while (i <= j) {
				while (SearchInformation[i].GetPoint() < x) i++;
				while (SearchInformation[j].GetPoint() > x) j--;
				if (i <= j) {
					swap(SearchInformation[i], SearchInformation[j]);
					i++;
					j--;
				}
			}
			if (i<r)
				quickSort(i, r);

			if (l<j)
				quickSort(l, j);
		}

		void SortSearchInformation() {
			quickSort(0, LastFreeExperiment-1);
		}

		void FindmMaximumAbsoluteValue() //поиск M
		{
			double tempM;
			for (int i = 1; i < LastFreeExperiment; i++)
			{
				tempM = abs((SearchInformation[i].GetFunctionValue() - SearchInformation[i - 1].GetFunctionValue()) / 
							 (SearchInformation[i].GetPoint() - SearchInformation[i - 1].GetPoint()));
				if (tempM > M)
				{
					M = tempM;
				}
			}
		}
		void Set_m()
		{
			if (M == 0)
				m = 1;
			if (M > 0)
				m = r*M;
		}
		void SetIntervalCharacteristic(int i)//i>=1 и i<LastFreeExperiment
		{
			double a = m*(SearchInformation[i].GetPoint() - SearchInformation[i - 1].GetPoint());
			double b = (SearchInformation[i].GetFunctionValue() - SearchInformation[i - 1].GetFunctionValue()) *
					   (SearchInformation[i].GetFunctionValue() - SearchInformation[i - 1].GetFunctionValue());
			b = b / (m*(SearchInformation[i].GetPoint() - SearchInformation[i - 1].GetPoint()));
			double c = 2.0 * (SearchInformation[i].GetFunctionValue() + SearchInformation[i - 1].GetFunctionValue());
			IntervalCharacteristic[i] = a + b - c;
		}
		void FindIntervalCharacteristic()
		{
			double max;
			IndexMaxIntervalCharacteristic = 1;
			SetIntervalCharacteristic(1);
			max = IntervalCharacteristic[1];
			//cout << "LastFreeExperiment = " << LastFreeExperiment << "\n";
			for (int i = 2; i < LastFreeExperiment; i++)
			{
				SetIntervalCharacteristic(i);
				if (max < IntervalCharacteristic[i]) {
					max = IntervalCharacteristic[i];
					IndexMaxIntervalCharacteristic = i;
				}
			}
		}
		double FindNextPoint()
		{
			double a = SearchInformation[IndexMaxIntervalCharacteristic].GetPoint() + SearchInformation[IndexMaxIntervalCharacteristic - 1].GetPoint();
			double b = SearchInformation[IndexMaxIntervalCharacteristic].GetFunctionValue() - SearchInformation[IndexMaxIntervalCharacteristic - 1].GetFunctionValue();
			double c = 0.5*a - (0.5*b)*(1.0 / m);
			cout << "x = " << c << " a = " << SearchInformation[IndexMaxIntervalCharacteristic - 1].GetPoint() << " b = " << SearchInformation[IndexMaxIntervalCharacteristic].GetPoint() << " Len = " << SearchInformation[IndexMaxIntervalCharacteristic].GetPoint() - SearchInformation[IndexMaxIntervalCharacteristic-1].GetPoint() << endl;
			return c;
		}
		int FindPositionMinInSearchInformation()
		{
			double min = SearchInformation[0].GetFunctionValue();
			int position = 0;
			for (int i = 0; i < LastFreeExperiment; i++)
			{
				if (SearchInformation[i].GetFunctionValue()<min)
				{
					min = SearchInformation[i].GetFunctionValue();
					position = i;
				}
			}
			return position;
		}
		bool NeedToStop() //критерий останова
		{
			double x1 = SearchInformation[IndexMaxIntervalCharacteristic].GetPoint();
			double x0 = SearchInformation[IndexMaxIntervalCharacteristic - 1].GetPoint();
			//cout << "		Length = " << x1 - x0 << "\n";
			if ((x1-x0)< EPS)
			{
				
				return true;
			}
			else
			{
				return false;
			}
		}

	public:
		GlobalSearchAlgorithm(double a, double b) //изначально считаем функцию в крайних точках отрезка
		{
			LastFreeExperiment = 0;
			SetExperiment(a);
			SetExperiment(b);
			M = abs((SearchInformation[1].GetFunctionValue() - SearchInformation[0].GetFunctionValue()) /
					(SearchInformation[1].GetPoint() - SearchInformation[0].GetPoint()));
			Set_m();
			IndexMaxIntervalCharacteristic = 1;
		}
		double FindMin()
		{
			int j = 2;
			while (!NeedToStop() && j<200)
			{

				SortSearchInformation();
				FindmMaximumAbsoluteValue();
				Set_m();
				FindIntervalCharacteristic();
				SetExperiment(FindNextPoint());
				j++;	
			}
			cout << "\nIteretion = " << j << "\n";

			//минимальный отрезок
			double min = 10;
			double len;
			SortSearchInformation();
			for (int i = 1; i < LastFreeExperiment; i++)
			{
				len = SearchInformation[i].GetPoint() - SearchInformation[i - 1].GetPoint();
				if (min>len)
				{
					min = len;
				}
			}
			cout << "Min len = " << min << "\n";
			
			int min_position = FindPositionMinInSearchInformation();
			return SearchInformation[min_position].GetPoint();
		}
};
void main()
{
	GlobalSearchAlgorithm Algorithm(-0.5, 1.5);
	double x = Algorithm.FindMin();
	cout << "\nx = " << x <<"\n";
	cout << "f(x) = " << ObjectiveFunction(x) << "\n";
}