#include <iostream>
#include <iomanip>
#include <vector>
#include <iterator>
#include <algorithm>
#include <fstream>
#include <math.h>
#include <time.h>
#include "conway.h"
int NCITY = 100;
double Time[200][200];
double Price[200][200];

struct CityCoor {
	int sn, x, y;
};
struct Sequence {
	double distance;
	std::vector<int> vSeqnc;
};

std::vector<CityCoor> vCityCoors(NCITY);
std::vector<std::vector<double> > vDistanceMatrix(NCITY);

double computDistance(const std::vector<int>& v);

namespace ga {
int POPSIZE = NCITY * 5;
int GENERATION = 5000;				//在变化显著的情况下为5000代
int SIMILAR_TIMES = 200;
double DIFF = 1e-15;					//量化变化不显著
double P_CROSSOVER = 0.3;				//交叉概率
double P_MUTATION = 0.1;				//变异概率
int nGen = 0;

std::vector<int> vCitySequence(NCITY);
std::vector<Sequence> vChromsome(POPSIZE + 1);	//第一个染色体只记录上一代遗传的最优解而不参与遗传运算
std::vector<double> vProbability(POPSIZE + 1);

namespace itor {

inline void itorDistance(Sequence& se) {
	se.distance = computDistance(se.vSeqnc);
}
inline void initChromsome(Sequence& v);

}
inline bool less_second(const Sequence &a, const Sequence &b) {
	return a.distance < b.distance;
}
}	//ga::

//////////////////////////////////////////////////////////////////////////
//迭代器
typedef std::vector<CityCoor>::iterator vitorCityCoor;
typedef std::vector<int>::iterator vitorSn;

typedef std::vector<int>::size_type vintSize;

typedef std::vector<CityCoor>::const_iterator cvitorCityCoor;
typedef std::vector<int>::const_iterator cvitorSn;
typedef std::vector<Sequence>::const_iterator cvitorChromsome;

//////////////////////////////////////////////////////////////////////////
std::vector<Sequence> vTemp(ga::POPSIZE + 1);

//////////////////////////////////////////////////////////////////////////
//全局函数，具体作用可见Definition
bool readTSP_Data(QVector<QPoint> p);
inline double myrand(double, double);
void crossover();
void mutation();
void selection();
void calcDistanceMatrix(QVector<QPoint> p, int speed, double pr1, double pr2,
		double pr3, QVector<QLine> ql, int flag);

bool isValid(std::vector<int> v) {
	sort(v.begin(), v.end());

	return true;
}
double gaussrand(int speed, int fc) {
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
	if ((X * fc + speed) < 0)
		return (-1) * (X * fc + speed);
	else
		return X * fc + speed;
}
vector<int> cal2(QVector<QPoint> p, node pl, int& min_distance, int speed,
		double pr1, double pr2, double pr3, QVector<QLine> ql, int flag1,
		int POPSIZE1 = 500, int Generaton1 = 5000, double crosscover1 = 0.3,
		double matation = 0.1) {
	//////////////////////////////////////////////////////////////////////////
	ga::POPSIZE = POPSIZE1;
	ga::GENERATION = Generaton1;
	ga::P_CROSSOVER = crosscover1;
	ga::P_MUTATION = matation;

	NCITY = p.size();
	readTSP_Data(p);

	clock_t startTime = clock();

	calcDistanceMatrix(p, speed, pr1, pr2, pr3, ql, flag1);
	if (flag1 == 2) {
		vDistanceMatrix[pl.x][pl.y] = pl.distance;
		vDistanceMatrix[pl.y][pl.x] = pl.distance;
		Distance[0][p.size() - 1] = 0;
		Distance[p.size() - 1][0] = 0;
	}

	std::cout << " OK\n";

	//求适应函数
	ga::vProbability[0] = 0.05;
	double a = 0.05;
	for (int i = 1; i <= ga::POPSIZE; ++i) {
		a *= 0.95;
		ga::vProbability[i] = ga::vProbability[i - 1] + a;
	}
	//////////////////////////////////////////////////////////////////////////
	std::cout << "#初始化" << ga::POPSIZE << "个染色体：";
	for_each(ga::vChromsome.begin() + 1, ga::vChromsome.end(),
			ga::itor::initChromsome);
	std::cout << " OK\n";

	ga::vChromsome[0] = ga::vChromsome[1];
	//先淘汰一次，以优化初值
	sort(ga::vChromsome.begin() + 1, ga::vChromsome.end(), ga::less_second);

	//////////////////////////////////////////////////////////////////////////
	std::cout << "\n#开始遗传算法，结束条件为连续" << ga::SIMILAR_TIMES << "代变化小于" << ga::DIFF
			<< "";

	int generWidth = static_cast<int>(ceil(
			log10(static_cast<double>(ga::GENERATION))));
	for (int i = 1; i <= ga::GENERATION; ++i) {
		selection();
		double prevDistance = ga::vChromsome[0].distance;

		srand(static_cast<unsigned>(time(NULL)));
		crossover();
		mutation();

		//对每个染色体求距离
		for_each(ga::vChromsome.begin() + 1, ga::vChromsome.end(),
				ga::itor::itorDistance);
		//进化，好解(小)在前，上一代的最优解也带入
		sort(ga::vChromsome.begin(), ga::vChromsome.end(), ga::less_second);

		if (prevDistance - ga::vChromsome[0].distance < ga::DIFF)
			++ga::nGen;
		else
			ga::nGen = 0;

		if (ga::SIMILAR_TIMES < ga::nGen)
			break;
	}
	min_distance = ga::vChromsome[0].distance;

	isValid(ga::vChromsome[0].vSeqnc);

	vector<int> result;
	vector<int> temp = ga::vChromsome[0].vSeqnc;
	int flag;
	for (int i = 0; i < temp.size(); i++) {
		if (temp[i] == 1) {
			flag = i;
			break;
		}
	}
	if (temp[flag - 1] > temp[flag + 1]) {
		for (int i = flag; i < temp.size(); i++) {
			result.push_back(temp[i] - 1);
		}
		for (int i = 0; i < flag; i++) {
			result.push_back(temp[i] - 1);
		}
	} else {
		for (int i = flag; i >= 0; i--) {
			result.push_back(temp[i] - 1);
		}
		for (int i = temp.size() - 1; i >= flag + 1; i--) {
			result.push_back(temp[i] - 1);
		}
	}
	result.push_back(0);
	return result;
}

bool readTSP_Data(QVector<QPoint> p) {
	try {
		int num = 0;
		cout << p.size() << "  ";
		for (vitorCityCoor i = vCityCoors.begin(); i != vCityCoors.end(); ++i) {
			cout << num << " ";
			if (num == p.size())
				break;
			(*i).sn = num + 1;
			(*i).x = p[num].x();
			(*i).y = p[num].y();
			num++;
		}

	} catch (...) {
		return false;
	}
	return true;
}

//贪婪算法Right
int GreedyRight(std::vector<int> &cityrouter, int nCity) {
	bool bFindCity = false;
	vitorSn iter_city;
	for (iter_city = cityrouter.begin(); iter_city != cityrouter.end();
			++iter_city)
		if (*iter_city == nCity) {
			bFindCity = true;
			break;
		}

	if (bFindCity) {
		++iter_city;
		if (iter_city == cityrouter.end())
			iter_city = cityrouter.begin();

		return *iter_city;
	} else
		return -1;
}

//贪婪算法Left
int GreedyLeft(std::vector<int> &cityrouter, int nCity) {
	bool bFindCity = false;
	vitorSn iter_city;
	for (iter_city = cityrouter.begin(); iter_city != cityrouter.end();
			++iter_city)
		if (*iter_city == nCity) {
			bFindCity = true;
			break;
		}

	if (bFindCity) {
		if (iter_city == cityrouter.begin())
			return cityrouter.back();
		else {
			--iter_city;
			return *iter_city;
		}
	} else
		return -1;
}

void GreedyErase(std::vector<int> &cityrouter, int ndelcity) {
	bool bFindCity = false;
	vitorSn iter_city;
	for (iter_city = cityrouter.begin(); iter_city != cityrouter.end();
			++iter_city)
		if (*iter_city == ndelcity) {
			bFindCity = true;
			break;
		}

	if (bFindCity)
		cityrouter.erase(iter_city);
}

void doCrossover(int nFatherA, int nFatherB) {
	int randomcity, nowopcity, nextopcity, rightA, rightB, leftA, leftB;
	std::vector<int> SonA, SonB;

	randomcity = static_cast<int>(myrand(1, NCITY));

	nowopcity = randomcity;
	SonA.push_back(nowopcity);

	std::vector<int> FatherA = ga::vChromsome[nFatherA].vSeqnc;
	std::vector<int> FatherB = ga::vChromsome[nFatherB].vSeqnc;

	while (FatherA.size() > 1 && FatherB.size() > 1) {
		rightA = GreedyRight(FatherA, nowopcity);
		rightB = GreedyRight(FatherB, nowopcity);

		if (vDistanceMatrix[nowopcity - 1][rightA - 1]
				< vDistanceMatrix[nowopcity - 1][rightB - 1]) {
			SonA.push_back(rightA);
			nextopcity = rightA;
		} else {
			SonA.push_back(rightB);
			nextopcity = rightB;
		}

		GreedyErase(FatherA, nowopcity);
		GreedyErase(FatherB, nowopcity);
		nowopcity = nextopcity;
	}

	nowopcity = randomcity;
	SonB.push_back(nowopcity);
	FatherA = ga::vChromsome[nFatherA].vSeqnc;
	FatherB = ga::vChromsome[nFatherB].vSeqnc;

	while (FatherA.size() > 1 && FatherB.size() > 1) {
		leftA = GreedyLeft(FatherA, nowopcity);
		leftB = GreedyLeft(FatherB, nowopcity);

		if (vDistanceMatrix[nowopcity - 1][leftA - 1]
				< vDistanceMatrix[nowopcity - 1][leftB - 1]) {
			SonB.push_back(leftA);
			nextopcity = leftA;
		} else {
			SonB.push_back(leftB);
			nextopcity = leftB;
		}

		GreedyErase(FatherA, nowopcity);
		GreedyErase(FatherB, nowopcity);
		nowopcity = nextopcity;
	}

	swap(ga::vChromsome[nFatherA].vSeqnc, SonA);
	swap(ga::vChromsome[nFatherB].vSeqnc, SonB);
}

void crossover() {
	std::vector<int> vecCrossoverIndexs;
	double random;

	for (int i = 1; i <= ga::POPSIZE; i++) {
		random = static_cast<double>(myrand(0, 1));
		if (random < ga::P_CROSSOVER)
			vecCrossoverIndexs.push_back(i);
	}

	size_t CrossoverNumber = vecCrossoverIndexs.size();
	if (CrossoverNumber % 2 != 0)
		vecCrossoverIndexs.pop_back();

	CrossoverNumber = vecCrossoverIndexs.size();
	for (size_t i = 0; i < CrossoverNumber; i += 2) {
		int nFatherA = vecCrossoverIndexs[i];
		int nFatherB = vecCrossoverIndexs[i + 1];

		doCrossover(nFatherA, nFatherB);
	}
}

void ga::itor::initChromsome(Sequence& v) {
	v.vSeqnc.resize(NCITY);

	for (int i = 1; i <= NCITY; ++i)
		v.vSeqnc[i - 1] = i;

	std::random_shuffle(v.vSeqnc.begin(), v.vSeqnc.end());
	v.distance = computDistance(v.vSeqnc);
	std::cout << ".";
}

double myrand(double a, double b) {
	double y;
	if (a > b) {
		printf("\nThe first parameter should be less than the second!");
		exit(1);
	}
	//////////////////////////////////////////////////////////////////////////
	//rand() can reture a number in [0, RAND_MAX]
	y = static_cast<double>(rand()) / RAND_MAX;

	return (a + (b - a) * y);
}

void mutation() {
	vitorSn it;
	for (int i = 1; i <= ga::POPSIZE; ++i)
		if (ga::P_MUTATION > myrand(0, 1)) {
			int left = static_cast<int>(myrand(0, NCITY / 2));
			int right = static_cast<int>(myrand(NCITY / 2, NCITY));
			it = ga::vChromsome[i].vSeqnc.begin();

			std::random_shuffle(it + left, it + right);
		}
}

double computDistance(const std::vector<int>& v) {
	double tmp = 0.0;

	for (int it = 0; it < NCITY - 1; ++it)
		tmp += vDistanceMatrix[v[it] - 1][v[it + 1] - 1];

	tmp += vDistanceMatrix[v[NCITY - 1] - 1][v[0] - 1];
	return tmp;
}

void selection() {
	double r;
	int label;

	vTemp[0] = ga::vChromsome[0];	//第一个仅仅作记录用
	for (int i = 1; i <= ga::POPSIZE; ++i) {
		r = myrand(0, ga::vProbability[ga::POPSIZE]);
		label = 0;
		for (int j = 0; j <= ga::POPSIZE; ++j) {
			if (r <= ga::vProbability[j]) {
				label = j;
				break;
			}
		}
		vTemp[i] = ga::vChromsome[label];
	}

	swap(ga::vChromsome, vTemp);
}

void calcDistanceMatrix(QVector<QPoint> p, int speed, double pr1, double pr2,
		double pr3, QVector<QLine> ql, int flag) {
	double dltX, dltY;

	std::vector<std::vector<double> >::iterator it = vDistanceMatrix.begin();
	for (; it != vDistanceMatrix.end(); ++it)
		(*it).resize(NCITY);

	for (int i = 0; i < NCITY; ++i)
		for (int j = i; j < NCITY; ++j)
			if (i == j)
				vDistanceMatrix[i][j] = 0.0;
			else {
				dltX = vCityCoors[i].x - vCityCoors[j].x;
				dltY = vCityCoors[i].y - vCityCoors[j].y;
				vDistanceMatrix[i][j] = sqrt(
						static_cast<double>(dltX * dltX + dltY * dltY));
				vDistanceMatrix[j][i] = vDistanceMatrix[i][j];
			}

	for (int i = 0; i < NCITY; ++i)
		for (int j = 0; j < NCITY; ++j) {
			if (i == j)
				Time[i][j] = 0.0;
			Time[i][j] = vDistanceMatrix[i][j] / gaussrand(speed, 10) * 50;

		}

	for (int i = 0; i < NCITY; ++i)
		for (int j = 0; j < NCITY; ++j) {
			if (i == j)
				Price[i][j] = 0.0;
			Price[i][j] = vDistanceMatrix[i][j] * gaussrand(30, 5) / 100;

		}
	for (int i = 0; i < NCITY; ++i)
		for (int j = 0; j < NCITY; ++j) {
			vDistanceMatrix[i][j] = vDistanceMatrix[i][j] * pr1
					+ Time[i][j] * pr2 + Price[i][j] * pr3;

		}
	for (int i = 0; i < p.size(); i++) {
		for (int j = 0; j < p.size(); j++) {
			cout << vDistanceMatrix[i][j] << " ";
		}
		cout << endl;
	}

}
