/*grid.cpp*/
#include "grid.h"

namespace grid{
	Area::Area()
	{
		FILE *fo;
		fopen_s(&fo, "Area.txt", "r");

		for (int i = 0; i < 3; i++)
			fscanf_s(fo, "%lf", &x[i]);

		for (int i = 0; i < 3; i++)
			fscanf_s(fo, "%lf", &y[i]);

		for (int i = 0; i < 6; i++)
			fscanf_s(fo, "%d", &ku[i]);

		fclose(fo);
	}

	Grid::Grid(){}

	Grid::~Grid() {}

	//Генерация координаты с учётом разбиения на всех интервалах
	void Grid::PartitionСoordinate(vector <double> &x, array <double, 3> areasLines,
		array <double, 2> coefficient, array <int, 2> nIntervals)
	{
		int count;
		//длина интервала
		double l;
		//шаг
		double h;
		//число интервалов
		int nLines = areasLines.size();

		count = 0;
		for (int i = 0; i < nLines - 1; i++)
		{
			x.push_back(areasLines[i]);
			count++;

			//длина интервала
			l = abs(areasLines[i + 1] - areasLines[i]);

			//рассчитываем первый шаг
			//равномерная
			if (abs(1.0 - coefficient[i]) < 1E-14)
				h = l / nIntervals[i];
			else {//неравномерная
					h = l * (1.0 - coefficient[i]);
					h /= 1.0 - pow(coefficient[i], nIntervals[i]);
				}

			//получаем сетку внутри интервала
			for (int j = 1; j < nIntervals[i]; j++)
			{
				if (j != 1) h *= coefficient[i];
				x.push_back(x[count - 1] + h);
				count++;
			}
		}
		x.push_back(areasLines[nLines - 1]);
	}

	//Добавление узла
	void Grid::PushNode(double x, double y)
	{
		Point p;
		p.y = y;
		p.x = x;
		//кладём узел в конец массива узлов
		nodes.push_back(p);
	}

	//Построение сетки
	void Grid::BuildNodes()
	{
		//для iого интервала и каждой координаты
		//коэффициент разрядки
		array <double, 2> xCoefficient;
		array <double, 2> yCoefficient;
		//число подинтервалов
		array <int, 2>  xIntervals;
		array <int, 2>  yIntervals;

		//количество координатных линий по x и y
		int xLines = 3;
		int yLines = 3;

		//геометрические линии разбиения по х и y
		vector <double> xi;
		vector <double> yj;

		//временная переменная для считывания
		int tmp1;
		//временная переменная для считывания
		double tmp2;

		//число узлов
		int nNodes;

		FILE *fo;
		fopen_s(&fo,"Intervals.txt", "r");
		//ввод количества интервалов по х и у и коэффициентов разрядки
		for (int i = 0; i < xLines - 1; i++)
		{
			fscanf_s(fo, "%d", &tmp1);
			xIntervals[i] = tmp1;
			fscanf_s(fo, "%lf", &tmp2);
			xCoefficient[i] = tmp2;
		}

		for (int i = 0; i < yLines - 1; i++)
		{
			fscanf_s(fo, "%d", &tmp1);
			yIntervals[i] = tmp1;
			fscanf_s(fo, "%lf", &tmp2);
			yCoefficient[i] = tmp2;
		}
		fclose(fo);

		nx = 0;
		for (int i = 0; i < xLines - 1; i++)
			//общее количество интервалов по х
			nx += xIntervals[i];
		nx++;

		ny = 0;
		for (int j = 0; j < yLines - 1; j++)
			//общее количество интервалов по у
			ny += yIntervals[j];
		ny++;

		xi.reserve(nx); yj.reserve(ny);

		//построение сеток по х и у
		PartitionСoordinate(xi, area.x, xCoefficient, xIntervals);
		PartitionСoordinate(yj, area.y, yCoefficient, yIntervals);

		nNodes = xi.size() * yj.size();

		nodes.reserve(nNodes);

		//заполняем список узлов
		for (int j = 0; j < yj.size(); j++)
			for (int i = 0; i < xi.size(); i++)
					PushNode(xi[i], yj[j]);
	}

	//Проверка на фиктивность узла
	bool Grid::FictNode(double x, double y)
	{
		if (x > area.x[1] && y < area.y[1]) return true;
		return false;
	}

	//Проверка узла на принадлежность к заданной области 
	bool Grid::InNode(double x, double y)
	{
		if (x > area.x[0] && x < area.x[1] && y > area.y[0] && y < area.y[2]) return true;
		if (x > area.x[0] && x < area.x[2] && y > area.y[1] && y < area.y[2]) return true;
		return false;
	}

	//Проверка на принадлежность узла одной из границ
	int Grid::BorderNode(double x, double y)
	{
		if (x == area.x[0] && y >= area.y[0] && y <= area.y[2]) return 0;
		if (x == area.x[1] && y >= area.y[0] && y <= area.y[1]) return 1;
		if (x == area.x[2] && y >= area.y[1] && y <= area.y[2]) return 2;
		if (x > area.x[0] && x < area.x[1] && y == area.y[0]) return 3;
		if (x > area.x[1] && x < area.x[2] && y == area.y[1]) return 4;
		if (x > area.x[0] && x < area.x[2] && y == area.y[2]) return 5;
		return -1;
	}
}
