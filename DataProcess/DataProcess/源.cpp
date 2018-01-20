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
	vector<string> ReList, PointList;
	ReList.clear();
	PointList.clear();
    string ID;
	ifstream in;
	in.open("requirements_and_cost.txt");
	while (in >> ID) {
		ReList.push_back(ID);
	}
	in.close();
	in.open("PointP-SReq.txt");
	while (in >> ID) {
		PointList.push_back(ID);
	}
	char buffer[10];
	for (int i = 1; i <= ReList.size(); i++) {
		replace(PointList.begin(), PointList.end(), '2', '1');
	}
	getchar();
	return 0;
}