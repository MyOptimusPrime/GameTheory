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

int main(){
	//vector<string> ReList, PointList;
	//ReList.clear();
	//PointList.clear();
    //string ID;
	//ifstream in;
	//in.open("requirements_and_cost.txt");
	//while (in >> ID) {
	//	ReList.push_back(ID);
	//}
	//in.close();
	//in.open("PointP-SReq.txt");
	//while (in >> ID) {
	//	PointList.push_back(ID);
	//}
	//in.close();
	//for (int i = 0; i < ReList.size(); i++) {
	//	for (int j = 0; j < PointList.size(); j++) {
	//		if (PointList[j] == ReList[i]) PointList[j] = std::to_string(i);
	//	}
	//}
	///*int NameID = 0;
	//for (int i = 0; i < PointList.size() - 1; i++) {
	//	if (PointList[i] == PointList[i + 1]) PointList[i] = std::to_string(NameID);
	//	else {
	//		PointList[i] = std::to_string(NameID);
	//		NameID++;
	//	}
	//}*/
	vector<pair<string, int>> RateList;
	string ID;
	int rate;
	RateList.clear();
	ifstream in;
	in.open("RateP-SReq.txt");
	while (in >> ID >> rate) {
		RateList.push_back(make_pair(ID, rate));
	}
	in.close();
	vector<pair<string, int>> sum_rate;
	sum_rate.clear();
	rate = 0;
	int i = 0;
	for (i ; i < RateList.size() - 1; i++) {
		rate += RateList[i].second;
		if (RateList[i].first != RateList[i + 1].first) {
			sum_rate.push_back(make_pair(RateList[i].first, rate));
			rate = 0;
		}
	}
	rate += RateList[i].second;
	sum_rate.push_back(make_pair(RateList[i].first, rate));
	ofstream out;
	out.open("OutPut.txt");
	for (int j = 0; j < sum_rate.size(); j++) {
		out << sum_rate[j].first << '\t' << sum_rate[j].second << endl;
	}
	//getchar();
	return 0;
}