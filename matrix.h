/*matrix.h*/
#pragma once
#include <algorithm>
#include "grid.h"
using namespace grid;
namespace matrix
{
	struct Matrix
	{
		//Матрица СЛАУ
		//5-тидиагональная матрица 
		array<vector<double>, 5> matr;
		//сдвиги диагоналей
		array<int, 5> ia;
		//размерность матрицы
		int n;
		//количество нулевых диагоналей
		int m;

		void Initialization(int size, int k);
		//Умножение матрицы на вектор
		void MultiplyAx(const vector <double> a, vector<double>& result);
	};
}
