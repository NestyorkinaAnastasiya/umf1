/*slae.h*/
#pragma once
#include "matrix.h"
using namespace matrix;

namespace slae
{
	class SLAE
	{
		//Сетка, элементы которой узлы
		Grid grid;
		//Номер зашитого теста
		int test;
		//Размерность задачи
		int n;
		//Максимальное количество итераций
		const int maxiter = 10000;
		//Точность решения СЛАУ
		const double eps = 1e-14;
		//Коэфициенты задачи 
		double lambda = 2;
		double gamma;
		//Глобальная матрица
		Matrix A;
		//Локальный вектор правой части
		array <double, 9> locF;
		//Глобальный вектор правой части
		vector <double> F;
		//Норма вектора правой части
		double normF;

		void CalculateMatrix();

		//Нахождение правой части
		double CalculateF(double x, double y);
		double CalculateGamma(double x, double y);
		//Нахождение правой части при первом краевом условии
		double CalculateF1(double x, double y);
		//Нахождение правой части при втором краевом условии
		double CalculateF2(double x, double y, int numberBorder);

		//Приближенное значение искомого вектора
		vector <double> x;
		//Вектор невязки
		vector <double> r;
		//Вектор спуска
		vector <double> z;

		//Вычисление нормы вектора
		double Norm(const vector<double>& x);
		//Скалярное произведение векторов
		double Scalar(const vector<double>& x, const vector<double>& y);


		double Residual();
		double Sum(int i, const vector<double> &_new, const vector<double> &old);
		
	public:
		//параметр релаксации
		double w = 0.1;
		SLAE();
		void GaussSeidel();
		~SLAE() {};
	};
}
