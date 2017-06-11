// Force.cpp : 定义控制台应用程序的入口点。
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
#include <bitset>

using namespace std;
//using std::bitset;

#define mp make_pair
#define pb push_back
#define ll long long


// 总共的人数
const static int PERSON = 2;

// 每个人的需求数
const static int PER_REQUEST = 5;

// 每个人满意度的和
const static int SUM_SATISFY = 100;

// 每个需求最大的耗费资源数
const static int MAX_CONSUME = 1000;
//
const static double A = 1;
const static double B = 5;
const static double C = 0.7;

// for debug
template<typename T>
void print(const vector<T> &in) {
	int n = in.size();
	for (int i = 0; i < n; ++i) {
		cout << in[i] << ' ';
	}
	cout << endl;
}

// 生成随机数矩阵，存到文件rand.txt中
bool generate_rand() {

	ofstream out("rand.txt");

	if (!out.is_open()) return false;

	srand(time(NULL));

	for (int per = 1; per <= PERSON; ++per) {
		vector<pair<int, int> > ev;
		ev.clear();
		int cur_sum = 0;
		for (int req = 1; req <= PER_REQUEST; ++req) {
			int satisfy = rand() % SUM_SATISFY + 1; //
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

// 从rand.txt中读取数据
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

// 计算方差
double variance(const vector<int> &in) {
	double sum = 0, sum2 = 0;
	int n = in.size();

	if (!n) return 0;

	for (int i = 0; i < n; ++i) {
		sum += in[i];
		sum2 += in[i] * in[i];
	}
	sum /= n, sum2 /= n;
	// D(x) = E(x^2) - E(x)^2
	return sum2 - sum * sum;
}

double sum(const vector<int> &in) {
	double sum = 0;
	int n = in.size();
	for (int i = 0; i < n; ++i) {
		sum += in[i];
	}
	return sum;
}

double average(const vector<int> &in) {
	if (in.size() == 0) return 0;
	return sum(in) / in.size();
}

double calc_P(const vector<int> &satisfy, const vector<int> &consume,
	const ll &sum_consume) {
	return A * average(satisfy) / 100 - B / 2500 * variance(satisfy) -
		C * sum(consume) / sum_consume;
}

double calc_P_local(const vector<int> & satisfy, const vector<int> &consume,
	const ll &sum_consume) {
	return A * sum(satisfy) / PERSON / 100.0 - B / 2500.0 * variance(satisfy) -
		C * average(consume) * PERSON / sum_consume;
}

bool cmp(pair<int, double> A, pair<int, double> B) {
	if (A.second == B.second) return A.first < B.first;
	return A.second > B.second;
}

int main() {
	/*if (!generate_rand()) {
		cout << "Generate rand matrix failed" << endl;
		return 0;
	}*/
	ll sum_consume = 0;
	vector<pair<int, int> > data = read_data(sum_consume);
	cout << sum_consume << endl;
	ll max_iter = 1LL << (PERSON * PER_REQUEST);

	// brute force
	int ans_pos = -1;
	double ans_max = INT_MIN * 1.0;
	ofstream out("brute.txt");
	if (!out.is_open()) {
		cout << "Failed to open brute.txt" << endl;
		return 0;
	}
	vector<pair<int, double> > sort_ans;
	sort_ans.clear();
	for (ll i = 0; i < max_iter; ++i) {
		vector<int> satisfy, consume;  // 每个人的满意度以及消耗的资源
		satisfy.clear(), consume.clear();
		for (int index = 0; index < PERSON; ++index) {
			satisfy.pb(0), consume.pb(0);
		}
		for (int index = 0; index < (PERSON * PER_REQUEST); ++index) {
			if ((i >> index) & 1) {
				satisfy[index / PER_REQUEST] += data[index].first;
				consume[index / PER_REQUEST] += data[index].second;
			}
		}
		// cout << i << '\t'  << calc_P(satisfy, consume) << endl;
		double tmp = calc_P(satisfy, consume, sum_consume);
		if (tmp > ans_max) {
			ans_max = tmp;
			ans_pos = i;
		}
		out << i << '\t' << tmp << endl;
		sort_ans.pb(mp(i, tmp));
	}
	out.close();
	// 对结果排序并写入sort_out.txt
	ofstream sort_out("sort_out.txt");
	if (!sort_out.is_open()) {
		cout << "Failed to open sort_out.txt" << endl;
		return 0;
	}
	sort(sort_ans.begin(), sort_ans.end(), cmp);
	for (size_t i = 0; i < sort_ans.size(); ++i) {
	//for (size_t i = 0; i < 1000; ++i) {
		sort_out << i + 1 << '\t' << bitset<32>(sort_ans[i].first) << '\t' << sort_ans[i].second << endl;
	}
	sort_out.close();
	return 0;
}

