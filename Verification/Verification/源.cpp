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
#define pb push_back

// 总共的人数
const static int person = 79;

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
int sum_consume;
vector<vector<int>> EmployerBenefit;

void update(int i) {
	cur_con_sum -= gamer[i] ? Consume[i] * p[i]  : -Consume[i] * p[i];
	for (int j = 0; j < PointP[i].size(); j++) {
		cur_sa_sum -= gamer[i] ? PointP[i][j].second: -PointP[i][j].second;

		cur_sa_sum_2 -= cur_sa_per_sum[PointP[i][j].first] * cur_sa_per_sum[PointP[i][j].first];
		cur_sa_per_sum[PointP[i][j].first] -= gamer[i] ? PointP[i][j].second : -PointP[i][j].second;
		cur_sa_sum_2 += cur_sa_per_sum[PointP[i][j].first] * cur_sa_per_sum[PointP[i][j].first];
	}
	gamer[i] = !gamer[i];
}

void initialize() {
	cur_sa_sum = cur_con_sum = cur_sa_sum_2 = 0;
	cur_sa_per_sum.clear();
	cur_sa_per_sum.resize(person);
	gamer.clear();
	p.clear();
	for (int i = 0; i < Consume.size(); i++) {
		gamer.pb(false);
		p.pb(1);
	}
	for (int i = 0; i < gamer.size(); i++) update(i);
}

void read_data() {
	PointP.clear();
	Consume.clear();
	int ReID, NameID, consume;
	double satisfy;
	vector<pair<int, double>> ReID_statisy;
	ReID_statisy.clear();
	sum_consume = 0;
	
	ifstream in("requirements_and_cost.txt");
	if (!in.is_open()) return;
	sum_consume = 0;
	while (in >> ReID >> consume) {
		Consume.pb(consume);
		sum_consume += consume;
	}
	in.close();

	//cout << Consume.size() << endl;
	in.open("PointP-SReq.txt");
	if (!in.is_open()) return;
	int i = 0;
	in >> NameID >> ReID >> satisfy;
	while (i <= Consume.size()) {
		//cout << i << '\t';
		if (ReID == i) {
			ReID_statisy.pb(mp(NameID, satisfy));
			if (!(in >> NameID >> ReID >> satisfy)) ReID = -1;
		}
		else {
			PointP.pb(ReID_statisy);
			ReID_statisy.clear();
			i++;
		}
	}
	in.close();
	return;
}

double calc_p() {
	double varia = cur_sa_sum_2 / person - cur_sa_sum * cur_sa_sum / person / person;
	return A * cur_sa_sum / person - B / 0.25 * varia - C * cur_con_sum / sum_consume;
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
	initialize();//初始化
	
	bool flag = false;
	//long long step = 0;
	//double result_tmp = 0;
	double tmp = 0;
	while (!flag) {
		flag = true;
		for (int i = 0; i < Consume.size(); i++) {
			tmp = check_pimax(i);
			if (tmp > -9) {
				update(i);
				//step++;
				flag = false;
				/*if (tmp - result_tmp > 0.01) {
				result_tmp = tmp;
				cout << "score reached " << result_tmp << " using " << step << " step!" << endl;
				}*/
			}
		}
	}
	for (int i = 0; i < Consume.size(); i++) {
		cout << PointP[i].size() << '\t' << gamer[i] << endl;
	}
	getchar();
	return 0;
}
