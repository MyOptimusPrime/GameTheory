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


double A = 1;
double B = 5;
double C = 1;

double cur_sa_sum, cur_con_sum;
double cur_sa_sum_2;
vector<double> cur_sa_per_sum;
vector<bool> gamer;
vector<pair<int, int> > Data;
ll sum_consume;

void initialize() {
	cur_sa_sum = cur_con_sum = cur_sa_sum_2 = 0;
	cur_sa_per_sum.clear();
	cur_sa_per_sum.resize(person);
	gamer.clear();
	for (int i = 0; i < person*per_request; i++) {
		//gamer.pb(false);
		gamer.pb(true);
	}
	int n = gamer.size();
	for (int i = 0; i < n; i++) {
		cur_sa_sum += gamer[i] ? Data[i].first : 0;
		cur_con_sum += gamer[i] ? Data[i].second : 0;
		cur_sa_per_sum[i / per_request] += gamer[i] ? Data[i].first : 0;
	}
	for (int i = 0; i < n / per_request; i++)
		cur_sa_sum_2 += cur_sa_per_sum[i] * cur_sa_per_sum[i];
}

void update(int i) {
	cur_sa_sum -= gamer[i] ? Data[i].first : -Data[i].first;
	cur_con_sum -= gamer[i] ? Data[i].second : -Data[i].second;

	cur_sa_sum_2 -= cur_sa_per_sum[i / per_request] * cur_sa_per_sum[i / per_request];
	cur_sa_per_sum[i / per_request] -= gamer[i] ? Data[i].first : -Data[i].first;
	cur_sa_sum_2 += cur_sa_per_sum[i / per_request] * cur_sa_per_sum[i / per_request];
	gamer[i] = !gamer[i];
}

bool generate_rand() {

	ofstream out("rand.txt");

	if (!out.is_open()) return false;

	srand(time(NULL));

	for (int per = 1; per <= person; ++per) {
		vector<pair<int, int> > ev;
		ev.clear();
		int cur_sum = 0;
		for (int req = 1; req <= per_request; ++req) {
			int satisfy = rand() % (SUM_SATISFY - 20) + 20; //
			int consume = rand() % MAX_CONSUME + 1; // 1 - MAX_CONSUME
			cur_sum += satisfy;
			ev.pb(mp(satisfy, consume));
		}

		int left = SUM_SATISFY;
		for (int req = 1; req <= per_request; ++req) {
			int satisfy = int(ev[req - 1].first * 100.0 / cur_sum + 0.5);
			if (req == per_request) {
				satisfy = left;
			}
			ev[req - 1].first = satisfy;
			// out << satisfy << '\t' << ev[req - 1].second << endl;
			left -= satisfy;
		}
		sort(ev.begin(), ev.end());
		for (int req = per_request - 1; req >= 0; --req) {
		//for (int req = 0; req <= PER_REQUEST - 1; ++req) {
			out << ev[req].first << '\t' << ev[req].second << endl;
		}
	}
	out.close();
	return true;
}

void read_data() {
	Data.clear();
	ifstream in("rand.txt");
	if (!in.is_open()) return;
	int satisfy, consume;
	sum_consume = 0;
	while (in >> satisfy >> consume) {
		Data.pb(mp(satisfy, consume));
		sum_consume += (ll)consume;
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

void run_game() {
    bool flag = false;
	long long step = 0;
	double result_tmp = 0;
	double tmp = 0;
	while (!flag) {
		flag = true;
		for (int i = 0; i < person * per_request; i++) {
			tmp = check_pimax(i);
			if (tmp > -9) {
				update(i);
				step++;
				flag = false;
				if (tmp - result_tmp > 0.01) {
					result_tmp = tmp;
					cout << "score reached " << result_tmp << " using " << step << " step!" << endl;
				}
			}
		}
	}
}

int main() {
	//产生随机矩阵
	if (!generate_rand()) {
		cout << "Generate rand matrix failed" << endl;
		return 0;
	}

	read_data();//读取随机矩阵

	initialize();//初始化

	run_game();//博弈运行

	cout << calc_p() << endl;
	getchar();
	return 0;
}
