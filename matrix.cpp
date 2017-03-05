/*matrix.cpp*/
#include "matrix.h"

namespace matrix{
	void Matrix::Initialization(int size, int k){
		int i, j, s, l, f;

		n = size;
		m = k;
		for (i = 0, f = 0, l = -m - 4; i < 5; i++, l++, f++){
			ia[i] += l;
			// ���������� ��������� � i-�� ���������
			s = n - abs(l);
			matr[i].resize(s);
			//���� ������ 3 ���������, ������� ������� �� m
			if (f == 0 || f == 3) l += m;
		}
	}

	void Matrix::MultiplyAx(const vector <double> &a, vector <double> &result){
		int  j, k, l;
		for (int i = 0; i < 5; i++){
			k = ia[i];
			//������ �����������
			if (k < 0){
				k = abs(k);
				for (j = k, l = 0; j < n; j++, l++)
					result[j] += matr[i][l] * a[l];
			}
			//������� ����������� + ���������
			else
				for (j = 0, l = k; j < n - k; j++, l++)
					result[j] += matr[i][j] * a[l];
		}

	}
}

