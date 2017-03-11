/*slae.cpp*/
#include "slae.h"

namespace slae
{
	void SLAE::CalculateMatrix(){
		double hx1, hx2, hy1, hy2;
		for (int i = 0; i < grid.nodes.size(); i++) {
			//���� ���� �� �������� ���������
			if (!grid.FictNode(grid.nodes[i].x, grid.nodes[i].y)) {
				hx1 = grid.nodes[i].x - grid.nodes[i - 1].x;//hxi-1
				hx2 = grid.nodes[i+1].x - grid.nodes[i].x;//hxi
				hy1 = grid.nodes[i].y - grid.nodes[i - 1].y;
				hy2 = grid.nodes[i + 1].y - grid.nodes[i].y;

				//���� ������ �������
				if (grid.InNode(grid.nodes[i].x, grid.nodes[i].y)){
					A.matr[0][i - A.m - 2] = -lambda / hy1 / (hy1 + hy2);// A.matr[4][i] = pow((grid.nodes[i].y - grid.nodes[i - 1].y), 2);
					A.matr[1][i - 1] = -lambda / hx1 / (hx1 + hx2); // A.matr[3][i] = pow((grid.nodes[i].x - grid.nodes[i - 1].x), 2);
					
					A.matr[2][i] = (1 / hx1 / hx2 + 1 / hy1 / hy2) * 2 * lambda ; //1 / pow((grid.nodes[i].y - grid.nodes[i - 1].y), 2) +1 / pow((grid.nodes[i].x - grid.nodes[i - 1].x), 2);
					gamma = CalculateGamma(grid.nodes[i].x, grid.nodes[i].y);
					A.matr[2][i] += gamma;

					A.matr[3][i] = -lambda / hx2 / (hx1 + hx2);
					A.matr[4][i] = -lambda / hy2 / (hy1 + hy2);

					F[i] = CalculateF(grid.nodes[i].x, grid.nodes[i].y);
				}
				//���� �� �������
				else {
					int borderNumber = grid.BorderNode(grid.nodes[i].x, grid.nodes[i].y);
					int ku = grid.area.ku[borderNumber];
					if (ku == 1) {
						A.matr[2][i] = 1;
						F[i] = CalculateF1(grid.nodes[i].x, grid.nodes[i].y);
					}
					else if (ku == 2) {
						if (borderNumber == 0) {
							A.matr[3][i] = -lambda / (grid.nodes[i+1].x - grid.nodes[i].x);
							A.matr[2][i] = lambda / (grid.nodes[i+1].x - grid.nodes[i].x);
						}
						else if (borderNumber == 1 || borderNumber == 2) {
							A.matr[1][i - 1] = -lambda / (grid.nodes[i].x - grid.nodes[i - 1].x);
							A.matr[2][i] = lambda / (grid.nodes[i].x - grid.nodes[i - 1].x);
						}
						else if (borderNumber == 3 || borderNumber == 4){
							 A.matr[4][i] = -lambda /(grid.nodes[i+1].y - grid.nodes[i].y);
							 A.matr[2][i] = lambda / (grid.nodes[i+1].y - grid.nodes[i].y);
						}
						else if (borderNumber == 5){
							A.matr[0][i - A.m - 2] = -lambda / (grid.nodes[i].y - grid.nodes[i - 1].y);
							A.matr[2][i] = lambda / (grid.nodes[i].y - grid.nodes[i - 1].y);
						}
						F[i] = CalculateF2(grid.nodes[i].x, grid.nodes[i].y, borderNumber);
					}
				}
			}
			//���� ��������� 
			else {
				A.matr[2][i] = 1;
				F[i] = 0;
			}
		}
	}

	//���������� ������ ����� (�����)
	double SLAE::CalculateF(double x, double y){
		gamma = CalculateGamma(x, y);
		switch(test){
		case 0: return gamma * (x + y);
		case 1: return -2 * lambda + gamma * (x*x + y*y);
		case 3: return -lambda * exp(x) + gamma * exp(x);
		}
		return 0;
	}

	//���������� ������������ ����� (�����)
	double SLAE::CalculateGamma(double x, double y){
		switch (test) {
		case 0: return 1;
		case 1: return 1;
		case 3: return 1;
		}
		return 0;
	}

	//������ ����� ��� ������ ������� �������� (�����)
	double SLAE::CalculateF1(double x, double y){
		switch (test) {
		case 0: return x+y;
		case 1: return x*x+y*y;
		case 3: return exp(x);
		}
		return 0;
	}

	//������ ����� ��� ������ ������� �������� (�����)
	double SLAE::CalculateF2(double x, double y, int numberBorder){
		switch (test) {
		case 0: {
			switch(numberBorder){
			case 0: return -lambda;
			case 1: return lambda;
			case 2: return lambda;
			case 3: return -lambda;
			case 4: return -lambda;
			case 5: return lambda;
			}
		}
		case 1: {
			switch (numberBorder) {
			case 0: return -lambda * 2 * x;
			case 1: return lambda * 2 * x;
			case 2: return lambda * 2 * x;
			case 3: return -lambda * 2 * y;
			case 4: return -lambda * 2 * y;
			case 5: return lambda * 2 * y;
			}
		}
		case 3: {
			switch (numberBorder) {
			case 0: return -lambda * exp(x);
			case 1: return lambda * exp(x);
			case 2: return lambda * exp(x);
			case 3: return 0;
			case 4: return 0;
			case 5: return 0;
			}
		}
		}
		return 0;
	}

	SLAE::SLAE(){
		//���������� �����
		grid.BuildNodes();
		//����������� ������
		n = grid.nx *grid.ny;

		F.resize(n);
		x.resize(n);
		r.resize(n);
		z.resize(n);

		FILE *fo = fopen("test.txt", "r");
		fscanf(fo, "%d", &test);
		fclose(fo);

		//��������� �������� ������� � � �������������
		A.Initialization(n, grid.nx);
		//������ �������
		CalculateMatrix();
		
		normF = Norm(F);
	}


	//���������� ����� �������
	double SLAE::Norm(const vector<double> &x) {
		double norm = 0;
		int size = x.size();

		for (int i = 0; i < size; i++)
			norm += x[i] * x[i];

		return sqrt(norm);
	}

	//��������� ������������ ��������
	double SLAE::Scalar(const vector<double> &x, const vector<double> &y) {
		double sum = 0;
		int size = x.size();
		for (int i = 0; i < size; i++)
			sum += x[i] * y[i];

		return sum;
	}

	double SLAE::Residual(){
		double res;
		vector <double> Ax, k;
		Ax.resize(n);
		k.resize(n);
		A.MultiplyAx(x, Ax);
		for (int i = 0; i < n; i++)
			k[i] = F[i] - Ax[i];
		double p = Norm(k);
		p = Norm(F);
		res = Norm(k) / Norm(F);
		return res;
	}

	double SLAE::Sum(int i, vector <double> _new, vector <double> old){
		double sum = 0;

		for (int k = 0, l; k < 5; k++){
			//��������
			l = abs(A.ia[k]);
			//������ �����������
			if (k < 4){
				if (l <= i && i < n){
					int p = 0;
					for (int m = l; i != m; m++) p++;
					sum += A.matr[k][p] * _new[p];//
				}
			}
			//������� ����������� + ���������
			else
				if (0 <= i && i < n - l){
					int p = l;
					for (int m = 0; i != m; m++) p++;
					sum += A.matr[k][i] * old[p];
				}
		}
		return sum;
	}

	void SLAE::IterativeSteps(vector <double> &_new, vector <double> old) {
		double sum = 0;
		for (int i = 0; i < n; i++) {
			sum = Sum(i, _new, old);
			x[i] += w / A.matr[2][i] * (F[i] - sum);
		}
	}

	void SLAE::GaussSeidel(){
		//���. �������,
		double res;
		//���������� ��������
		vector <double> old;

		old.resize(n);
		for (int i = 0; i<n; i++)
			x[i] = 0;
		int iter;
		res = Residual();
		for (iter = 0; (res >= eps) && (iter < maxiter); iter++){
			for (int i = 0; i < n; i++)
				old[i] = x[i];
			IterativeSteps(x, old);
			res = Residual();
		}

		FILE *fr = fopen("GaussSeidel.txt", "a+b");
		fprintf(fr, "%.2lf\t", w);
		fprintf(fr, "%.15lf\t", x[0]);
		fprintf(fr, "%d\t", iter);
		fprintf(fr, "%.15lf\t", res);
		for (int i = 1; i<n; i++)
			fprintf(fr, "\t\t%.15lf\n", x[i]);
		fclose(fr);
	}
}