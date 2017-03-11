/*slae.h*/
#pragma once
#include "matrix.h"
using namespace matrix;

namespace slae{
	class SLAE {
		//����� �������� �����
		int test;
		//����������� ������
		int n;
		//������������ ���������� ��������
		const int maxiter = 10000;
		//�������� ������� ����
		const double eps = 1e-14;
		//����������� ������ 
		double lambda;
		double gamma;
		//�����, �������� ������� ����
		Grid grid;
		//���������� �������
		Matrix A;
		//��������� ������ ������ �����
		array <double, 9> locF;
		//���������� ������ ������ �����
		vector <double> F;
		//����� ������� ������ �����
		double normF;

		void CalculateMatrix();

		//���������� ������ �����
		double CalculateF(double x, double y);
		double CalculateGamma(double x, double y);
		//���������� ������ ����� ��� ������ ������� �������
		double CalculateF1(double x, double y);
		//���������� ������ ����� ��� ������ ������� �������
		double CalculateF2(double x, double y, int numberBorder);

		//������������ �������� �������� �������
		vector <double> x;
		//������ �������
		vector <double> r;
		//������ ������
		vector <double> z;

		//���������� ����� �������
		double Norm(const vector<double>& x);
		//��������� ������������ ��������
		double Scalar(const vector<double>& x, const vector<double>& y);


		double Residual();
		void IterativeSteps(vector<double>& _new, vector<double> old);
		double Sum(int i, vector<double> _new, vector<double> old);
		
	public:
		//�������� ����������
		double w = 1;
		SLAE();
		void GaussSeidel();
		~SLAE() {};
	};
}
