/*matrix.h*/
#pragma once
#include <algorithm>
#include "grid.h"
using namespace grid;
namespace matrix{
	struct Matrix{
		//������� ����
		//5-�������������� ������� 
		array<vector<double>, 5> matr;
		//������ ����������
		array<int, 5> ia;
		//����������� �������
		int n;
		//���������� ������� ����������
		int m;

		void Initialization(int size, int k);
		//��������� ������� �� ������
		void MultiplyAx(const vector <double> &a, vector<double>& result);
	};
}
