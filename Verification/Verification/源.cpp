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
vector<pair<int, int> > Data;
vector<double> p;
double punishfactor = 5;
ll sum_consume;
vector<vector<int>> EmployerBenefit;

void initialize() {
	C = 1;
	cur_sa_sum = cur_con_sum = cur_sa_sum_2 = 0;
	cur_sa_per_sum.clear();
	cur_sa_per_sum.resize(person);
	gamer.clear();
	p.clear();
	for (int i = 0; i < person*per_request; i++) {
		//gamer.pb(false);
		gamer.pb(true);
		p.pb(1);
	}
	int n = gamer.size();
	for (int i = 0; i < n; i++) {
		cur_sa_sum += gamer[i] ? Data[i].first : 0;
		cur_con_sum += gamer[i] ? Data[i].second * p[i] : 0;
		cur_sa_per_sum[i / per_request] += gamer[i] ? Data[i].first : 0;
	}
	for (int i = 0; i < n / per_request; i++)
		cur_sa_sum_2 += cur_sa_per_sum[i] * cur_sa_per_sum[i];
}

void update(int i) {
	cur_sa_sum -= gamer[i] ? Data[i].first : -Data[i].first;
	cur_con_sum -= gamer[i] ? Data[i].second * p[i] : -Data[i].second * p[i];

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
	ifstream in("requirements_and_cost.txt");
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
	//long long step = 0;
	//double result_tmp = 0;
	double tmp = 0;
	while (!flag) {
		flag = true;
		for (int i = 0; i < person * per_request; i++) {
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
}

int FindEB_PL(int i) {
	for (int j = 0; j < per_request; j++) {
		if (!gamer[i * per_request + j])
			return Data[i * per_request + j].first;
	}
	return 0;
}

int FindEB_NPO(int i, vector<bool> BackupGamer) {
	int EB = 0;
	for (int j = 0; j < per_request; j++) {
		if (!BackupGamer[i * per_request + j] && gamer[i * per_request + j])
			EB += Data[i * per_request + j].first;
		else if (BackupGamer[i * per_request + j] && !gamer[i * per_request + j])
			EB -= Data[i * per_request + j].first;
	}
	return EB;
}

void run_punish(int i, int j) {
	for (double pAdjust = 0.1; pAdjust < 1; pAdjust += 0.1) {
		for (int k = 0; k < per_request; k++) {
			p[i * per_request + k] -= (k == j) ? pAdjust : -punishfactor * pAdjust;
			cur_con_sum -= (k == j) ?
				Data[i * per_request + k].second * pAdjust * punishfactor :
				-Data[i * per_request + k].second * pAdjust * punishfactor;
		}
		run_game();
		if (gamer[i * per_request + j])
			break;
	}
}

int FindEB_PO(int i, vector<bool> BackupGamer) {
	int j;
	int EB = 0;
	for (j = 0; j < per_request; j++) {
		if (!BackupGamer[i * per_request + j] && gamer[i * per_request + j]) {
			EB = FindEB_NPO(i, BackupGamer);
			return EB;
		}
		else if (!BackupGamer[i * per_request + j] && !gamer[i * per_request + j])
			break;
	}

	run_punish(i, j);
	EB = FindEB_NPO(i, BackupGamer);
	return EB;
}

int main() {

	//for (int round = 1; round <= 5; round++) {
	//cout << round << endl;
	//产生随机矩阵
	if (!generate_rand()) {
		cout << "Generate rand matrix failed" << endl;
		return 0;
	}

	read_data();//读取随机矩阵

	initialize();//初始化

	run_game();//博弈初次运行获得原开发方案

	vector<int> PersonBenefit;
	vector<bool> BackupGamer = gamer;
	C = 0.5;
	run_game();
	ofstream out("EB.txt", ios::app);
	for (int i = 0; i < person; i++) {
		cout << i << '\t';
		PersonBenefit.clear();
		PersonBenefit.pb(0);
		PersonBenefit.pb(FindEB_PL(i));
		PersonBenefit.pb(FindEB_NPO(i, BackupGamer));
		/*out << BackupGamer[i * per_request + 0] << BackupGamer[i * per_request + 1]
		<< BackupGamer[i * per_request + 2] << BackupGamer[i * per_request + 3]
		<< BackupGamer[i * per_request + 4] << BackupGamer[i * per_request + 5]
		<< BackupGamer[i * per_request + 6] << BackupGamer[i * per_request + 7]
		<< BackupGamer[i * per_request + 8] << BackupGamer[i * per_request + 9] << '\t'
		<< gamer[i * per_request + 0] << gamer[i * per_request + 1]
		<< gamer[i * per_request + 2] << gamer[i * per_request + 3]
		<< gamer[i * per_request + 4] << gamer[i * per_request + 5]
		<< gamer[i * per_request + 6] << gamer[i * per_request + 7]
		<< gamer[i * per_request + 8] << gamer[i * per_request + 9] << '\t'
		<< PersonBenefit[0] << '\t' << PersonBenefit[1] << '\t' << PersonBenefit[2] << '\t';*/

		vector<bool> backup_gamer = gamer;
		double backup_cur_con_sum = cur_con_sum;
		vector<double> backup_p = p;

		PersonBenefit.pb(FindEB_PO(i, BackupGamer));

		/*out << gamer[i * per_request + 0] << gamer[i * per_request + 1]
		<< gamer[i * per_request + 2] << gamer[i * per_request + 3]
		<< gamer[i * per_request + 4] << gamer[i * per_request + 5]
		<< gamer[i * per_request + 6] << gamer[i * per_request + 7]
		<< gamer[i * per_request + 8] << gamer[i * per_request + 9] << '\t'
		<< PersonBenefit[3]<< endl;*/

		gamer = backup_gamer;
		cur_con_sum = backup_cur_con_sum;
		p = backup_p;

		out << PersonBenefit[0] << '\t' << PersonBenefit[1] << '\t'
			<< PersonBenefit[2] << '\t' << PersonBenefit[3] << endl;

		EmployerBenefit.pb(PersonBenefit);
	}
	out.close();
	//}
	return 0;
}
