#ifndef DP_H
#define DP_H
#include<iostream>
#include<stdio.h>
#include<vector>
#include "conway.h"
#include "Dijkstra.h"
using namespace std;
#define INF 10000
#define pi 3.14159265358
#define L 200.0
#define N1  100
vector<conway> conways;
/*�����ö���������ʾ�����Ӧ��������Ϊ������±� */
//Dij[i][j] Ϊ��i�㾭��j���ϵ�Դ��Ļ���
int Dij[N1][1 << 20];
// routine[i][j] Ϊ��i�㾭��j���ϵ�Դ�������i->..
int routine[N1][1 << 20];

int Distance[N1][N1];

int CityNum;
long start;
double gaussrand(int speed) {
	static double V1, V2, S;
	static int phase = 0;
	double X;

	if (phase == 0) {
		do {
			double U1 = (double) rand() / RAND_MAX;
			double U2 = (double) rand() / RAND_MAX;

			V1 = 2 * U1 - 1;
			V2 = 2 * U2 - 1;
			S = V1 * V1 + V2 * V2;
		} while (S >= 1 || S == 0);

		X = V1 * sqrt(-2 * log(S) / S);
	} else
		X = V2 * sqrt(-2 * log(S) / S);

	phase = 1 - phase;
	if ((X * 10 + speed) < 0)
		return (-1) * (X * 10 + speed);
	else
		return X * 10 + speed;
}

vector<int> cal(QVector<QPoint> p, node pl, int& min_distance, int flag,
		int speed, QVector<QLine> ql, int distance1[100][100]) {
	memset(Distance, 0, sizeof(Distance));
	vector<int> result;
	int i, j, k, max, min, rout, curpos;
	int temp[N1];

	CityNum = p.size();

	if (flag == 1) {
		for (int i = 0; i < CityNum; i++) {
			for (int j = 0; j < CityNum; j++) {
				Distance[i][j] = sqrt(
						(p[i].x() - p[j].x()) * (p[i].x() - p[j].x())
								+ (p[i].y() - p[j].y())
										* (p[i].y() - p[j].y()));

			}
			printf("\n");
		}
	} else if (flag == 2) {
		for (int i = 0; i < CityNum; i++) {
			for (int j = i; j < CityNum; j++) {
				Distance[i][j] = sqrt(
						(p[i].x() - p[j].x()) * (p[i].x() - p[j].x())
								+ (p[i].y() - p[j].y())
										* (p[i].y() - p[j].y()));
				Distance[i][j] = Distance[i][j] / gaussrand(speed);
				Distance[j][i] = Distance[i][j];

			}

		}
	} else if (flag == 3) {

		for (int i = 0; i < CityNum; i++) {
			for (int j = 0; j < CityNum; j++) {
				Distance[i][j] = sqrt(
						(p[i].x() - p[j].x()) * (p[i].x() - p[j].x())
								+ (p[i].y() - p[j].y())
										* (p[i].y() - p[j].y()));

			}

		}
		Distance[0][CityNum - 1] = 0;
		Distance[CityNum - 1][0] = 0;
		Distance[pl.x][pl.y] = pl.distance;
		Distance[pl.y][pl.x] = pl.distance;

	} else if (flag == 4) {
		for (int i = 0; i < CityNum; i++) {
			for (int j = 0; j < CityNum; j++) {

				Distance[i][j] = distance1[i][j];
			}
		}

	}

	max = (1 << (CityNum - 1)) - 1; /*2^(n-1)-1*/
	for (i = 1; i < CityNum; i++)  //i->0 (iֱ�ӵ�0)�Ļ���
		Dij[i][0] = Distance[i][0];
	/* ex: CityNum=4 max=7,�������1->[2,3]->0,2-[1,3]->0,3-[1,2]->0��ֵ*/
	for (i = 1; i < max; i++) {   //ȫ�����ϳ���111..111����
		for (j = 1; j <= CityNum - 1; j++)  //��һ�����  0->j+Dij[j][I]
			if ((i >> (j - 1) & 1) == 0) {   //j�㲻�ڼ���i��
				min = INF;
				for (k = 1; k <= CityNum; k++) {
					if ((i >> (k - 1) & 1) != 0) {  //k���ڼ���i��
						Dij[j][i] = Distance[j][k] + Dij[k][i - (1 << (k - 1))]; //���ƹ�ʽ
						if (Dij[j][i] < min) {
							min = Dij[j][i];
							rout = k;
						}
					}
				}
				Dij[j][i] = min;
				routine[j][i] = rout;
			}
	}
	//i>>(j-1)&1 �ж��Ƿ� jλ��1 ��i-(1<<(k-1)) ��i������ɾ��kλ�ϵ�ֵ
	/*��������·�������ٻ���*/
	min = INF;
	for (i = 1; i < CityNum; i++) {
		k = Distance[0][i] + Dij[i][max - (1 << (i - 1))];
		if (k < min) {   //t(11..11) ������ɾȥλ��i�ϵĵ�
			min = k;
			curpos = i;  //���·���ϵĵ�һ�����
		}
	}

	min_distance = min;

	result.push_back(0);

	result.push_back(curpos);

	temp[1] = 1;   //�������·������temp��
	temp[2] = curpos + 1;

	i = 3;
	k = max; //111..11
	for (j = CityNum - 1; j > 0; j--) { //������������
		k = k - (1 << (curpos - 1));  //ֱ�ӷŽ������ھ�������
		curpos = routine[curpos][k];
		result.push_back(curpos);

		temp[i++] = curpos + 1;
	}

	return result;

}

#endif // DP_H
