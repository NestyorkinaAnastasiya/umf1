/*grid.h*/
#pragma once
#include <stdio.h>
#include <vector>
#include <fstream>
#include <functional>
#include <array>
using namespace std;

namespace grid{
	//��������� ��� ��������� �������: 
	//����� �������, ����� �����

	//�������
	struct Area{
		array <double, 3> x;
		array <double, 3> y;

		//1 - 1 ����
		//2 - 2 ����
		//3 - 3 ����
		//-1 - ��� ������� �������
		array<int, 6> ku;
		//C����� ������� ������� �������
		//-1 - ��� ������� �������
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

	//��������� ����� �������
	class Nodes{
		//��������� ���������� � ������ ��������� �� ���� ����������
		void Partition�oordinate(vector <double> &x, array <double, 3> areasLines,
			array <double, 2> coefficient, array <int, 2> nIntervals);
		//���������� ����
		void PushNode(double x, double y);	
		
	public:
		Nodes();
		//������� �
		Area area;
		//������ �����
		vector <Point> nodes;
		//����� ���������� ���������� �� x � �� y
		//����� ���������� �����
		int nx, ny;

		//����������� ����������� ���� ����� ��������� �������
		bool FictNode(double x, double y);
		bool InNode(double x, double y);
		int BorderNode(double x, double y);

		//���������� �����
		void BuildNodes();

		~Nodes();
	};

}