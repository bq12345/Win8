#ifndef CONWAY_H
#define CONWAY_H
#include <vector>
using namespace std;
struct conway {
	int x1;
	int x2;
	vector<int> replaceway;
	conway() {
	}
};
struct node {
	int x;
	int y;
	int distance;
	node() {
	}
	;
};
struct line {
	int x;
	int y;
	line(int x1, int y1) {
		x = x1;
		y = y1;
	}
};

#endif // CONWAY_H
