/*matrix.cpp*/
#include "matrix.h"

namespace matrix
{
	void Matrix::Initialization(int size, int k)
	{
		int i, j, s, l, f;

		n = size;
		m = k-2;
		for (i = 0, f = 0, l = -m-2; i < 5; i++, l++, f++)
		{
			ia[i] += l;
			// количество элементов в i-ой диагонали
			s = n - abs(l);
			matr[i].resize(s);
			//если прошли 3 диагонали, смещаем индексы на m
			if (f == 0 || f == 3) l += m;
		}
	}

	void Matrix::MultiplyAx(const vector <double> a, vector <double> &result)
	{
		int  j, k, l;
		for (int i = 0; i < result.size(); i++)
			result[i] = 0;
		for (int i = 0; i < 5; i++)
		{
			k = ia[i];
			//нижний треугольник
			if (k < 0)
			{
				k = abs(k);
				for (j = k, l = 0; j < n; j++, l++)
					result[j] += matr[i][l] * a[l];
			}
			//верхний треугольник + диагональ
			else
				for (j = 0, l = k; j < n - k; j++, l++)
					result[j] += matr[i][j] * a[l];
		}

	}
}

