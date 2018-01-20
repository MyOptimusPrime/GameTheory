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
const static int person = 100;

// 每个人的需求数
const static int per_request = 10;

// 每个人满意度的和
const static int SUM_SATISFY = 100;

// 每个需求最大的耗费资源数
const static int MAX_CONSUME = 1000;


double A = 1;
double B = 5;
double C = 1;

double cur_sa_sum, cur_con_sum;
double cur_sa_sum_2;
vector<double> cur_sa_per_sum;
vector<bool> gamer;
vector<vector<pair<int, double>>> PointP;
vector<int> Consume;
vector<double> p;
double punishfactor = 5;
ll sum_consume;
vector<vector<int>> EmployerBenefit;

void initialize() {

}

void update(int i) {

}

void read_data() {
	PointP.clear();
	Consume.clear();
	int ReID, NameID, consume, i;
	double satisfy;
	vector<pair<int, double>> ReID_statisy;
	ReID_statisy.clear();
	ifstream in("requirements_and_cost.txt");
	if (!in.is_open()) return;
	sum_consume = 0;
	while (in >> ReID >> consume) {
		Consume.pb(consume);
		sum_consume += (ll)consume;
	}
	in.close();
	in.open("PointP-SReq.txt");
	if (!in.is_open()) return;
	i = 0;
	while (in >> NameID >> ReID >> satisfy) {
		if (NameID == i) ReID_statisy.pb(mp(ReID, satisfy));
		else {
			PointP.pb(ReID_statisy);
			ReID_statisy.clear();
			ReID_statisy.pb(mp(ReID, satisfy));
			i++;
		}
	}
	return;
}

double calc_p() {
	double varia = cur_sa_sum_2 / person - cur_sa_sum * cur_sa_sum / person / person;
	return A * cur_sa_sum / person / 100 - B / 2500 * varia - C * cur_con_sum / sum_consume;
}

double check_pimax(int i) {
	double p_former = calc_p();

	update(i);

	double p_later = calc_p();

	// recover
	update(i);

	if (p_former > p_later)
		return -10;
	else
		return p_later;
}

int main() {
	read_data();//读取随机矩阵
	for (int i = 0; i < PointP[0].size(); i++){
		cout << PointP[0][i].first << '\t' << PointP[0][i].second << endl;
    }
	getchar();
	return 0;
}
