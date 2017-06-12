// Nash.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <algorithm>
#include <fstream>
#include <climits>
#include <ctime>

using namespace std;

#define mp make_pair
#define ll long long
#define pb push_back


// 总共的人数
const static int person = 1000;

// 每个人的需求数
const static int per_request = 10;

// 每个人满意度的和
const static int SUM_SATISFY = 100;

// 每个需求最大的耗费资源数
const static int MAX_CONSUME = 1000;


const static double A = 1;
const static double B = 5;
const static double C = 1;

double cur_sa_sum, cur_con_sum;
double cur_sa_sum_2;
vector<double> cur_sa_per_sum;

void initialize(const vector<pair<int, int> > & data, const vector<bool> & gamer, int PER_REQUEST) {
	int n = gamer.size();
	for (int i = 0; i < n; i++) {
		cur_sa_sum += gamer[i] ? data[i].first : 0;
		cur_con_sum += gamer[i] ? data[i].second : 0;
		cur_sa_per_sum[i / PER_REQUEST] += gamer[i] ? data[i].first : 0;
	}
	for (int i = 0; i < n / PER_REQUEST; i++)
		cur_sa_sum_2 += cur_sa_per_sum[i] * cur_sa_per_sum[i];
}

bool generate_rand(int PERSON, int PER_REQUEST) {

	ofstream out("rand.txt");

	if (!out.is_open()) return false;

	srand(time(NULL));

	for (int per = 1; per <= PERSON; ++per) {
		vector<pair<int, int> > ev;
		ev.clear();
		int cur_sum = 0;
		for (int req = 1; req <= PER_REQUEST; ++req) {
			int satisfy = rand() % (SUM_SATISFY-20) + 20; //
			int consume = rand() % MAX_CONSUME + 1; // 1 - MAX_CONSUME
			cur_sum += satisfy;
			ev.pb(mp(satisfy, consume));
		}

		int left = SUM_SATISFY;
		for (int req = 1; req <= PER_REQUEST; ++req) {
			int satisfy = int(ev[req - 1].first * 100.0 / cur_sum + 0.5);
			if (req == PER_REQUEST) {
				satisfy = left;
			}
			out << satisfy << '\t' << ev[req - 1].second << endl;
			left -= satisfy;
		}
	}
	out.close();
	return true;
}

vector<pair<int, int> > read_data(ll &consume_sum) {
	vector<pair<int, int> > ans;
	ans.clear();
	ifstream in("rand.txt");
	if (!in.is_open()) return ans;
	int satisfy, consume;
	consume_sum = 0;
	while (in >> satisfy >> consume) {
		ans.pb(mp(satisfy, consume));
		consume_sum += (ll)consume;
	}
	return ans;
}

double calc_p(const vector<pair<int, int> > & data, int sum_consume, int PERSON, int PER_REQUEST) {
	double varia = cur_sa_sum_2 / PERSON - cur_sa_sum * cur_sa_sum / PERSON / PERSON;
	return A * cur_sa_sum / PERSON / 100 - B / 2500 * varia - C * cur_con_sum / sum_consume;
}

double check_pimax(const vector<pair<int, int> > & data, bool gameri, int i, int sum_consume, int PERSON, int PER_REQUEST) {
	double p_former = calc_p(data, sum_consume, PERSON, PER_REQUEST);

	cur_sa_sum -= gameri ? data[i].first : -data[i].first;
	cur_con_sum -= gameri ? data[i].second : -data[i].second;

	cur_sa_sum_2 -= cur_sa_per_sum[i / PER_REQUEST] * cur_sa_per_sum[i / PER_REQUEST];
	cur_sa_per_sum[i / PER_REQUEST] -= gameri ? data[i].first : -data[i].first;
	cur_sa_sum_2 += cur_sa_per_sum[i / PER_REQUEST] * cur_sa_per_sum[i / PER_REQUEST];

	double p_later = calc_p(data, sum_consume, PERSON, PER_REQUEST);

	// recover
	cur_sa_sum += gameri ? data[i].first : -data[i].first;
	cur_con_sum += gameri ? data[i].second : -data[i].second;

	cur_sa_sum_2 -= cur_sa_per_sum[i / PER_REQUEST] * cur_sa_per_sum[i / PER_REQUEST];
	cur_sa_per_sum[i / PER_REQUEST] += gameri ? data[i].first : -data[i].first;
	cur_sa_sum_2 += cur_sa_per_sum[i / PER_REQUEST] * cur_sa_per_sum[i / PER_REQUEST];

	if (p_former > p_later)
		return -10;
	else
		return p_later;
}

void dealwith(int PERSON, int PER_REQUEST) {
	//产生随机矩阵
	/*if (!generate_rand(PERSON, PER_REQUEST)) {
		cout << "Generate rand matrix failed" << endl;
		return;
	}*/
	//读取随机矩阵
	ll sum_consume = 0;
	vector<pair<int, int> > data = read_data(sum_consume);
	//寻找过程

	//初始化
	vector<bool> gamer;
	gamer.clear();
	for (int i = 0; i < PERSON*PER_REQUEST; i++) {
		//gamer.pb(false);
		gamer.pb(true);
	}
	bool flag = false;
	long long step = 0;
	double result_tmp = 0;
	cur_sa_sum = cur_con_sum = cur_sa_sum_2 = 0;
	cur_sa_per_sum.clear();
	cur_sa_per_sum.resize(PERSON);
	initialize(data, gamer, PER_REQUEST);

	//主循环迭代
	while (!flag) {
		flag = true;
		for (int i = 0; i < PERSON * PER_REQUEST; i++) {
			double tmp = check_pimax(data, gamer[i], i, sum_consume, PERSON, PER_REQUEST);
			if (tmp > -9) {

				cur_sa_sum -= gamer[i] ? data[i].first : -data[i].first;
				cur_con_sum -= gamer[i] ? data[i].second : -data[i].second;

				cur_sa_sum_2 -= cur_sa_per_sum[i / PER_REQUEST] * cur_sa_per_sum[i / PER_REQUEST];
				cur_sa_per_sum[i / PER_REQUEST] -= gamer[i] ? data[i].first : -data[i].first;
				cur_sa_sum_2 += cur_sa_per_sum[i / PER_REQUEST] * cur_sa_per_sum[i / PER_REQUEST];

				gamer[i] = !gamer[i];
				step++;
				flag = false;
				if (tmp - result_tmp > 0.01) {
					result_tmp = tmp;
					cout << "score reached " << result_tmp << " using " << step << " step!" << endl;
				}
			}
		}
	}
	double result = calc_p(data, sum_consume, PERSON, PER_REQUEST);
	cout << result << endl;
	getchar();
}

int main() {
	
	return 0;
}
