#ifndef DIJKSTRA_H
#define DIJKSTRA_H
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
using namespace std;

void Dijkstra(int n, int v, int dist[], int prev[], int **cost) {
	int i;
	int j;
	int maxint = 65535; //����һ��������ֵ����Ϊ�������������ڵ�Ĵ���Ȩֵ
	int *s; //����������·���Ľڵ��Ӽ�s
	s = (int *) malloc(sizeof(int) * n);
	//��ʼ����С·�����ۺ�ǰһ���ڵ�ֵ
	for (i = 1; i <= n; i++) {
		dist[i] = cost[v][i];
		s[i] = 0;
		if (dist[i] == maxint) {
			prev[i] = 0;
		} else {
			prev[i] = v;
		}
	}
	dist[v] = 0;
	s[v] = 1; //Դ�ڵ���Ϊ�����s�Ӽ�

	for (i = 1; i < n; i++) {
		int temp = maxint;
		int u = v;
		//���������С���۵��ھӽڵ㵽s�Ӽ�
		for (j = 1; j <= n; j++) {
			if ((!s[j]) && (dist[j] < temp)) {
				u = j;
				temp = dist[j];
			}
		}
		s[u] = 1;
		//��������µĽڵ�󣬸���·��ʹ��������������
		for (j = 1; j <= n; j++) {
			if ((!s[j]) && (cost[u][j] < maxint)) {
				int newdist = dist[u] + cost[u][j];
				if (newdist < dist[j]) {
					dist[j] = newdist;
					prev[j] = u;
				}
			}
		}
	}
}
//չʾ���·������
vector<int> ShowPath(int n, int v, int u, int *dist, int *prev) {

	int j = 0;
	int w = u;
	int count = 0;
	int *way;
	way = (int *) malloc(sizeof(int) * (n + 1));
	//����·��
	while (w != v) {
		count++;
		way[count] = prev[w];
		w = prev[w];
	}
	//���·��
	vector<int> vec;

	for (j = count; j >= 1; j--) {

		vec.push_back(way[j] - 1);

	}
	vec.push_back(u - 1);

	return vec;
}
//����������Ҫ�������������
vector<int> caldij(int n, int v, int u, int a[][100], int &min_distance) {
	int i, j, t;

	int **cost; //���۾���
	int *dist; //���·������
	int *prev; //ǰһ���ڵ�ռ�

	cost = (int **) malloc(sizeof(int) * (n + 1));

	for (i = 1; i <= n; i++) {
		cost[i] = (int *) malloc(sizeof(int) * (n + 1));
	}
	//������۾���
	for (j = 1; j <= n; j++) {
		for (t = 1; t <= n; t++) {
			cost[j][t] = a[j - 1][t - 1];
		}
	}

	dist = (int *) malloc(sizeof(int) * n);
	prev = (int *) malloc(sizeof(int) * n);

	//����dijkstra�㷨
	Dijkstra(n, v, dist, prev, cost);
	vector<int> temp;
	temp = ShowPath(n, v, u, dist, prev);
	min_distance = dist[u];

	return temp;
}

#endif // DIJKSTRA_H
