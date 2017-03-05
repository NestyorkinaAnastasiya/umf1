/*slae.cpp*/
#include "slae.h"

namespace slae
{
	void SLAE::CalculateMatrix(){
		for (int i = 0; i < grid.nodes.size(); i++) {
			//если узел не является фиктивным
			if (!grid.FictNode(grid.nodes[i].x, grid.nodes[i].y)) {
				//если внутри области
				if (grid.InNode(grid.nodes[i].x, grid.nodes[i].y)){
					A.matr[0][i - A.m - 2] = A.matr[4][i] = 
						-lambda / pow((grid.nodes[i].y - grid.nodes[i - 1].y), 2);
					A.matr[1][i-1]  = A.matr[3][i] = 
						-lambda / pow((grid.nodes[i].x - grid.nodes[i - 1].x), 2);
					A.matr[2][i] = 1 / pow((grid.nodes[i].y - grid.nodes[i - 1].y), 2) +
						1 / pow((grid.nodes[i].x - grid.nodes[i - 1].x), 2);
					A.matr[2][i] *= 2 * lambda;
					A.matr[2][i] += gamma;
					F[i] = CalculateF(grid.nodes[i].x, grid.nodes[i].y);
				}
				//если на границе
				else {
					int borderNumber = grid.BorderNode(grid.nodes[i].x, grid.nodes[i].y);
					int ku = grid.area.ku[borderNumber];
					if (ku == 1) {
						A.matr[2][i] = 1;
						F[i] = CalculateF1(grid.nodes[i].x, grid.nodes[i].y);
					}
					else if (ku == 2) {
						if (borderNumber == 0) {
							A.matr[4][i+1] = -lambda / (grid.nodes[i+1].x - grid.nodes[i].x);
							A.matr[2][i] = lambda / (grid.nodes[i+1].x - grid.nodes[i].x);
						}
						else if (borderNumber == 1 || borderNumber == 2) {
							A.matr[4][i - 1] = -lambda / (grid.nodes[i].x - grid.nodes[i - 1].x);
							A.matr[2][i] = lambda / (grid.nodes[i].x - grid.nodes[i - 1].x);
						}
						else if (borderNumber == 3 || borderNumber == 4){
							 A.matr[4][i] = -lambda /(grid.nodes[i+1].y - grid.nodes[i].y);
							 A.matr[2][i] = lambda / (grid.nodes[i+1].y - grid.nodes[i].y);
						}
						else if (borderNumber == 5){
							A.matr[4][i - A.m - 2] = -lambda / (grid.nodes[i].y - grid.nodes[i - 1].y);
							A.matr[2][i] = lambda / (grid.nodes[i].y - grid.nodes[i - 1].y);
						}
						F[i] = CalculateF2(grid.nodes[i].x, grid.nodes[i].y, borderNumber);
					}
				}
			}
			//узел фиктивный 
			else {
				A.matr[2][i] = 1;
				F[i] = 0;
			}
		}
	}
	//Нахождение правой части
	double SLAE::CalculateF(double x, double y){
		return 0;
	}

	double SLAE::CalculateF1(double x, double y){
		return 0;
	}

	double SLAE::CalculateF2(double x, double y, int numberBorder){
		return 0;
	}
	
	//Вычисление нормы вектора
	double SLAE::Norm(const vector<double> &x){
		double norm = 0;
		int size = x.size();

		for (int i = 0; i < size; i++)
			norm += x[i] * x[i];

		return sqrt(norm);
	}

	//Скалярное произведение векторов
	double SLAE::Scalar(const vector<double> &x, const vector<double> &y){
		double sum = 0;
		int size = x.size();
		for (int i = 0; i < size; i++)
			sum += x[i] * y[i];

		return sum;
	}

	SLAE::SLAE(){
		//Построение сетки
		grid.BuildNodes();
		//Размерность задачи
		n = grid.nx *grid.ny;

		F.resize(n);
		x.resize(n);
		r.resize(n);
		z.resize(n);

		//Генерация портрета матрицы и её инициализация
		A.Initialization(n, grid.nx);
		CalculateMatrix();
		
		normF = Norm(F);
	}
}