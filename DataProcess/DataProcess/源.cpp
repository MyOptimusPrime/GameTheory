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
	in.close();
	for (int i = 0; i < ReList.size(); i++) {
		for (int j = 0; j < PointList.size(); j++) {
			if (PointList[j] == ReList[i]) PointList[j] = std::to_string(i);
		}
	}
	/*int NameID = 0;
	for (int i = 0; i < PointList.size() - 1; i++) {
		if (PointList[i] == PointList[i + 1]) PointList[i] = std::to_string(NameID);
		else {
			PointList[i] = std::to_string(NameID);
			NameID++;
		}
	}*/
	ofstream out;
	out.open("PL.txt");
	for (int j = 0; j < PointList.size(); j++) {
		out << PointList[j] << endl;
	}
	//getchar();
	return 0;
}