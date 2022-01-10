#include <algorithm>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <vector>

using namespace std;

void handle(char* filename, int u) {
	if (access(filename, F_OK)) {
		cout << "第" << u << "个文件不存在" << endl;
		return;
	}

	ifstream inFile;
	inFile.open(filename);
	int start = -1, cnt = 0;
	string cur;
	vector<string> includes, other;
	while (getline(inFile, cur)) {
		if (cur.substr(0, 8) == "#include") {
			includes.push_back(cur);
			if (start == -1) start = cnt;
		}
		else other.push_back(cur);
		cnt ++ ;
	}
	sort(includes.begin(), includes.end());
	inFile.close();

	ofstream outFile;
	outFile.open(filename);
	for (int i = 0; i < start; i ++ ) outFile << other[i] << endl;
	for (string s: includes) outFile << s << endl;
	for (int i = start; i < other.size(); i ++ ) outFile << other[i] << endl;
	outFile.close();
}

int main(int argc, char* argv[]) {
	if (argc < 2) {
		cout << "参数个数错误" << endl;
		return 1;
	}

	for (int i = 1; i < argc; i ++ ) handle(argv[i], i);
	
	cout << "finish..." << endl;
	return 0;
}
