/*grid.cpp*/
#include "grid.h"

namespace grid{
	Area::Area(){
		FILE *fo;
		fopen_s(&fo, "Area.txt", "r");

		for (int i = 0; i < 3; i++)
			fscanf_s(fo, "%lf", &x[i]);

		for (int i = 0; i < 3; i++)
			fscanf_s(fo, "%lf", &y[i]);

		for (int i = 0; i < 6; i++)
			fscanf_s(fo, "%lf", &ku[i]);

		fclose(fo);
	}

	Grid::Grid(){}

	Grid::~Grid() {}

	//��������� ���������� � ������ ��������� �� ���� ����������
	void Grid::Partition�oordinate(vector <double> &x, array <double, 3> areasLines,
		array <double, 2> coefficient, array <int, 2> nIntervals){
		int count;
		//����� ���������
		double l;
		//���
		double h;
		//����� ����������
		int nLines = areasLines.size();

		count = 0;
		for (int i = 0; i < nLines - 1; i++){
			x.push_back(areasLines[i]);
			count++;

			//����� ���������
			l = abs(areasLines[i + 1] - areasLines[i]);

			//������������ ������ ���
			//�����������
			if (abs(1.0 - coefficient[i]) < 1E-14)
				h = l / nIntervals[i];
			else {//�������������
					h = l * (1.0 - coefficient[i]);
					h /= 1.0 - pow(coefficient[i], nIntervals[i]);
				}

			//�������� ����� ������ ���������
			for (int j = 1; j < nIntervals[i]; j++)
			{
				if (j != 1) h *= coefficient[i];
				x.push_back(x[count - 1] + h);
				count++;
			}
		}
		x.push_back(areasLines[nLines - 1]);
	}

	//���������� ����
	void Grid::PushNode(double x, double y){
		Point p;
		p.y = y;
		p.x = x;
		//����� ���� � ����� ������� �����
		nodes.push_back(p);
	}

	//���������� �����
	void Grid::BuildNodes(){
		//��� i��� ��������� � ������ ����������
		//����������� ��������
		array <double, 2> xCoefficient;
		array <double, 2> yCoefficient;
		//����� �������������
		array <int, 2>  xIntervals;
		array <int, 2>  yIntervals;

		//���������� ������������ ����� �� x � y
		int xLines = 3;
		int yLines = 3;

		//�������������� ����� ��������� �� � � y
		vector <double> xi;
		vector <double> yj;

		//��������� ���������� ��� ����������
		int tmp1;
		//��������� ���������� ��� ����������
		double tmp2;

		//����� �����
		int nNodes;

		FILE *fo;
		fopen_s(&fo,"Intervals.txt", "r");
		//���� ���������� ���������� �� � � � � ������������� ��������
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
			//����� ���������� ���������� �� �
			nx += xIntervals[i];
		nx++;

		ny = 0;
		for (int j = 0; j < yLines - 1; j++)
			//����� ���������� ���������� �� �
			ny += yIntervals[j];
		ny++;

		xi.reserve(nx); yj.reserve(ny);

		//���������� ����� �� � � �
		Partition�oordinate(xi, area.x, xCoefficient, xIntervals);
		Partition�oordinate(yj, area.y, yCoefficient, yIntervals);

		nNodes = xi.size() * yj.size();

		nodes.reserve(nNodes);

		//��������� ������ �����
		for (int j = 0; j < yj.size(); j++)
			for (int i = 0; i < xi.size(); i++)
					PushNode(xi[i], yj[j]);
	}

	//�������� �� ����������� ����
	bool Grid::FictNode(double x, double y){
		if (x > area.x[1] && y < area.y[1]) return true;
		return false;
	}

	//�������� ���� �� �������������� � �������� ������� 
	bool Grid::InNode(double x, double y){
		if (x > area.x[0] && x < area.x[1] && y > area.y[0] && y < area.y[1]) return true;
		if (x > area.x[0] && x < area.x[2] && y > area.y[1] && y < area.y[2]) return true;
		return false;
	}

	//�������� �� �������������� ���� ����� �� ������
	int Grid::BorderNode(double x, double y){
		if (x == area.x[0] && y >= area.y[0] && y <= area.y[2]) return 0;
		if (x == area.x[1] && y >= area.y[0] && y <= area.y[1]) return 1;
		if (x == area.x[2] && y >= area.y[1] && y <= area.y[2]) return 2;
		if (x > area.x[0] && x < area.x[1] && y == area.y[0]) return 3;
		if (x > area.x[1] && x < area.x[2] && y == area.y[1]) return 4;
		if (x > area.x[0] && x < area.x[2] && y == area.y[2]) return 5;
		return -1;
	}
}
