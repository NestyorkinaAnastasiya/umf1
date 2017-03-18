/*grid.h*/
#pragma once
#include <stdio.h>
#include <vector>
#include <fstream>
#include <functional>
#include <array>
using namespace std;

namespace grid
{
	//Нумерация идёт следующим образом: 
	//слева направо, снизу вверх

	//Область
	struct Area
	{
		array <double, 3> x;
		array <double, 3> y;

		//1 - 1 рода
		//2 - 2 рода
		//3 - 3 рода
		//-1 - нет краевых условий
		array<int, 6> ku;
		Area();
	};

	struct Point
	{
		double x;
		double y;

		Point() {};
		~Point() {};
		Point(double xx, double yy)
		{
			x = xx;
			y = yy;
		}

		bool operator==(Point point)
		{
			if (point.x == x && point.y == y)
				return true;
			else
				return false;
		}

	};

	//Разбиение общей области
	class Grid
	{
		//Генерация координаты с учётом разбиения на всех интервалах
		void PartitionСoordinate(vector <double> &x, array <double, 3> areasLines,
			array <double, 2> coefficient, array <int, 2> nIntervals);
		//Добавление узла
		void PushNode(double x, double y);	
		
	public:
		Grid();
		//Область Г
		Area area;
		//Массив узлов
		vector <Point> nodes;
		//Общее количество интервалов по x и по y
		//после построения сетки
		int nx, ny;

		//Проверка на фиктивность узла
		bool FictNode(double x, double y);
		//Проверка узла на принадлежность к заданной области 
		bool InNode(double x, double y);
		//Проверка на принадлежность узла одной из границ
		int BorderNode(double x, double y);

		//Построение сетки
		void BuildNodes();

		~Grid();
	};

}
