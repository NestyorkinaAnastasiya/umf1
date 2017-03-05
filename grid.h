/*grid.h*/
#pragma once
#include <stdio.h>
#include <vector>
#include <fstream>
#include <functional>
#include <array>
using namespace std;

namespace grid{
	//Нумерация идёт следующим образом: 
	//слева направо, снизу вверх

	//Область
	struct Area{
		array <double, 3> x;
		array <double, 3> y;

		//1 - 1 рода
		//2 - 2 рода
		//3 - 3 рода
		//-1 - нет краевых условий
		array<int, 6> ku;
		//Cпособ расчёта краевых условий
		//-1 - нет краевых условий
		array<int, 6> kuForm;
		Area();
	};

	struct Point{
		double x;
		double y;

		Point() {};
		~Point() {};
		Point(double xx, double yy){
			x = xx;
			y = yy;
		}

		bool operator==(Point point){
			if (point.x == x && point.y == y)
				return true;
			else
				return false;
		}

	};

	//Разбиение общей области
	class Nodes{
		//Генерация координаты с учётом разбиения на всех интервалах
		void PartitionСoordinate(vector <double> &x, array <double, 3> areasLines,
			array <double, 2> coefficient, array <int, 2> nIntervals);
		//Добавление узла
		void PushNode(double x, double y);	
		
	public:
		Nodes();
		//Область Г
		Area area;
		//Массив узлов
		vector <Point> nodes;
		//Общее количество интервалов по x и по y
		//после построения сетки
		int nx, ny;

		//Определение фиктивности узла через расчётные области
		bool FictNode(double x, double y);
		bool InNode(double x, double y);
		int BorderNode(double x, double y);

		//Построение сетки
		void BuildNodes();

		~Nodes();
	};

}