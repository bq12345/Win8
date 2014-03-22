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
	int maxint = 65535; //定义一个最大的数值，作为不相连的两个节点的代价权值
	int *s; //定义具有最短路径的节点子集s
	s = (int *) malloc(sizeof(int) * n);
	//初始化最小路径代价和前一跳节点值
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
	s[v] = 1; //源节点作为最初的s子集

	for (i = 1; i < n; i++) {
		int temp = maxint;
		int u = v;
		//加入具有最小代价的邻居节点到s子集
		for (j = 1; j <= n; j++) {
			if ((!s[j]) && (dist[j] < temp)) {
				u = j;
				temp = dist[j];
			}
		}
		s[u] = 1;
		//计算加入新的节点后，更新路径使得其产生代价最短
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
//展示最佳路径函数
vector<int> ShowPath(int n, int v, int u, int *dist, int *prev) {

	int j = 0;
	int w = u;
	int count = 0;
	int *way;
	way = (int *) malloc(sizeof(int) * (n + 1));
	//回溯路径
	while (w != v) {
		count++;
		way[count] = prev[w];
		w = prev[w];
	}
	//输出路径
	vector<int> vec;

	for (j = count; j >= 1; j--) {

		vec.push_back(way[j] - 1);

	}
	vec.push_back(u - 1);

	return vec;
}
//主函数，主要做输入输出工作
vector<int> caldij(int n, int v, int u, int a[][100], int &min_distance) {
	int i, j, t;

	int **cost; //代价矩阵
	int *dist; //最短路径代价
	int *prev; //前一跳节点空间

	cost = (int **) malloc(sizeof(int) * (n + 1));

	for (i = 1; i <= n; i++) {
		cost[i] = (int *) malloc(sizeof(int) * (n + 1));
	}
	//输入代价矩阵
	for (j = 1; j <= n; j++) {
		for (t = 1; t <= n; t++) {
			cost[j][t] = a[j - 1][t - 1];
		}
	}

	dist = (int *) malloc(sizeof(int) * n);
	prev = (int *) malloc(sizeof(int) * n);

	//调用dijkstra算法
	Dijkstra(n, v, dist, prev, cost);
	vector<int> temp;
	temp = ShowPath(n, v, u, dist, prev);
	min_distance = dist[u];

	return temp;
}

#endif // DIJKSTRA_H
