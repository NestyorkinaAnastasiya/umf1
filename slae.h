/*slae.h*/
#pragma once
#include "tests.h"
using namespace matrix;
using namespace tests;

namespace slae{
	class SLAE {
		//����������� ������
		int n;
		//������������ ���������� ��������
		const int maxiter = 10000;
		//�������� ������� ����
		const double eps = 1e-14;
		double lambda;
		double gamma;
		//�����, �������� ������� ����
		Nodes grid;
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

		
	public:
		SLAE();
		~SLAE() {};
	};
}
