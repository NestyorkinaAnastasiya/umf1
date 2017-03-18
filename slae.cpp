/*slae.cpp*/
#include "slae.h"

namespace slae
{
	void SLAE::CalculateMatrix()
	{
		double hx1, hx2, hy1, hy2;
		for (int i = 0; i < grid.nodes.size(); i++) 
		{
			//если узел не является фиктивным
			if (!grid.FictNode(grid.nodes[i].x, grid.nodes[i].y)) 
			{
				//если внутри области
				if (grid.InNode(grid.nodes[i].x, grid.nodes[i].y)){
					hx1 = grid.nodes[i].x - grid.nodes[i - 1].x;//hxi-1
					hx2 = grid.nodes[i + 1].x - grid.nodes[i].x;//hxi
					hy1 = grid.nodes[i].y - grid.nodes[i - A.m -2].y;
					hy2 = grid.nodes[i + A.m + 2].y - grid.nodes[i].y;

					A.matr[0][i - A.m - 2] = -2*lambda / hy1 / (hy1 + hy2);// A.matr[4][i] = pow((grid.nodes[i].y - grid.nodes[i - 1].y), 2);
					A.matr[1][i - 1] = -2*lambda / hx1 / (hx1 + hx2); // A.matr[3][i] = pow((grid.nodes[i].x - grid.nodes[i - 1].x), 2);
					
					A.matr[2][i] = (1 / hx1 / hx2 + 1 / hy1 / hy2) * 2 * lambda ; //1 / pow((grid.nodes[i].y - grid.nodes[i - 1].y), 2) +1 / pow((grid.nodes[i].x - grid.nodes[i - 1].x), 2);
					gamma = CalculateGamma(grid.nodes[i].x, grid.nodes[i].y);
					A.matr[2][i] += gamma;

					A.matr[3][i] = -2*lambda / hx2 / (hx1 + hx2);
					A.matr[4][i] = -2*lambda / hy2 / (hy1 + hy2);

					F[i] = CalculateF(grid.nodes[i].x, grid.nodes[i].y);
				}
				//если на границе
				else 
				{
					int borderNumber = grid.BorderNode(grid.nodes[i].x, grid.nodes[i].y);
					int ku = -1;
					//if (borderNumber != -1)
						ku = grid.area.ku[borderNumber];
					if (ku == 1) 
					{
						A.matr[2][i] = 1;
						F[i] = CalculateF1(grid.nodes[i].x, grid.nodes[i].y);
					}
					else if (ku == 2) 
					{
						if (borderNumber == 0) 
						{
							A.matr[3][i] = -lambda / (grid.nodes[i+1].x - grid.nodes[i].x);
							A.matr[2][i] = lambda / (grid.nodes[i+1].x - grid.nodes[i].x);
						}
						else if (borderNumber == 1 || borderNumber == 2) 
						{
							A.matr[1][i - 1] = -lambda / (grid.nodes[i].x - grid.nodes[i - 1].x);
							A.matr[2][i] = lambda / (grid.nodes[i].x - grid.nodes[i - 1].x);
						}
						else if (borderNumber == 3 || borderNumber == 4)
						{
							A.matr[4][i] = -lambda / (grid.nodes[i + A.m + 2].y - grid.nodes[i].y);
							A.matr[2][i] = lambda / (grid.nodes[i + A.m + 2].y - grid.nodes[i].y);
						}
						else if (borderNumber == 5)
						{
							A.matr[0][i - A.m - 2] = -lambda / (grid.nodes[i].y - grid.nodes[i - A.m - 2].y);
							A.matr[2][i] = lambda / (grid.nodes[i].y - grid.nodes[i - A.m - 2].y);
						}
						F[i] = CalculateF2(grid.nodes[i].x, grid.nodes[i].y, borderNumber);
					}
				}
			}
			//узел фиктивный 
			else 
			{
				A.matr[2][i] = 1;
				F[i] = 0;
			}
		}
	}

	//Нахождение правой части (тесты)
	double SLAE::CalculateF(double x, double y)
	{
		gamma = CalculateGamma(x, y);
		switch(test)
		{
		case 0: return gamma * (x + y);
		case 1: return -2 * lambda + gamma * (x*x + y*y);
		case 3: return -lambda * exp(x) + gamma * exp(x);
		case 4: return 2;
		}
		return 0;
	}

	//Нахождение коэффициента гамма (тесты)
	double SLAE::CalculateGamma(double x, double y)
	{
		switch (test) {
		case 0: return 1;
		case 1: return 1;
		case 3: return 1;
		case 4: return 1;
		}
		return 0;
	}

	//Правая часть при первых краевых условиях (тесты)
	double SLAE::CalculateF1(double x, double y)
	{
		switch (test) 
		{
		case 0: return x+y;
		case 1: return x*x+y*y;
		case 3: return exp(x);
		case 4: return 2;
		}
		return 0;
	}

	//Правая часть при вторых краевых условиях (тесты)
	double SLAE::CalculateF2(double x, double y, int numberBorder)
	{
		switch (test) 
		{
		case 0: 
		{
			switch(numberBorder)
			{
			case 0: return -lambda;
			case 1: return lambda;
			case 2: return lambda;
			case 3: return -lambda;
			case 4: return -lambda;
			case 5: return lambda;
			}
		}
		case 1: 
		{
			switch (numberBorder) 
			{
			case 0: return -lambda * 2 * x;
			case 1: return lambda * 2 * x;
			case 2: return lambda * 2 * x;
			case 3: return -lambda * 2 * y;
			case 4: return -lambda * 2 * y;
			case 5: return lambda * 2 * y;
			}
		}
		case 3: 
		{
			switch (numberBorder) 
			{
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

	SLAE::SLAE()
	{
		//Построение сетки
		grid.BuildNodes();
		//Размерность задачи
		n = grid.nx *grid.ny;

		F.resize(n);
		x.resize(n);
		r.resize(n);
		z.resize(n);
		FILE *fo;
		fopen_s(&fo,"test.txt", "r");
		fscanf_s(fo, "%d", &test);
		fclose(fo);

		//Генерация портрета матрицы и её инициализация
		A.Initialization(n, grid.nx);
		//Сборка матрицы
		CalculateMatrix();
		
		normF = Norm(F);
	}


	//Вычисление нормы вектора
	double SLAE::Norm(const vector<double> &x) 
	{
		double norm = 0;
		int size = x.size();

		for (int i = 0; i < size; i++)
			norm += x[i] * x[i];

		return sqrt(norm);
	}

	//Скалярное произведение векторов
	double SLAE::Scalar(const vector<double> &x, const vector<double> &y) 
	{
		double sum = 0;
		int size = x.size();
		for (int i = 0; i < size; i++)
			sum += x[i] * y[i];

		return sum;
	}

	double SLAE::Residual()
	{
		double res;
		vector <double> Ax;
		Ax.resize(n);

		A.MultiplyAx(x, Ax);
		for (int i = 0; i < n; i++)
			Ax[i] = F[i] - Ax[i];
		res = Norm(Ax) / normF;

		return res;
	}

	double SLAE::Sum(int i, const vector <double> &_new, const vector <double> &old)
	{
		double sum = 0;

		for (int k = 0, l; k < 5; k++)
		{
			//смещение
			l = abs(A.ia[k]);
			//нижний треугольник
			if (k < 2)
			{
				if (l <= i && i < n)
				{
					int p = 0;
					for (int m = l; i != m; m++) p++;
					sum += A.matr[k][p] * _new[p];
				}
			}
			//верхний треугольник + диагональ
			else
				if (0 <= i && i < n - l)
				{
					int p = l;
					for (int m = 0; i != m; m++) p++;
					sum += A.matr[k][i] * old[p];
				}
		}
		return sum;
	}

	void SLAE::GaussSeidel()
	{	//отн. невязка
		double res;
		vector <double> old;
		double sum = 0;
		old.resize(n);
		int iter;
		res = Residual();

		FILE *fr;
		fopen_s(&fr, "GaussSeidel.txt", "w");

		for (iter = 0; (res >= eps) && (iter < maxiter); iter++)
		{
			old = x;
			for (int i = 0; i < n; i++) 
			{
				sum = Sum(i, x, old);
				x[i] += w / A.matr[2][i] * (F[i] - sum);
			}
			res = Residual();
		}

		fprintf(fr, "[%d]\t", iter);
		fprintf(fr, "%e\t", res);
		fprintf(fr, "%.14lf\n", x[0]);
		for (int i = 1; i<n; i++)
			fprintf(fr, "\t\t\t\t\t%.14lf\n", x[i]);
		fprintf(fr, "\n");

		fclose(fr);
	}
}
