#include <algorithm>  // sort()
#include <dirent.h>  // opendir()
#include <fstream>
#include <iostream>
#include <unistd.h>  // access()
#include <vector>

using namespace std;

bool cORcpp(string path_name) {
    int loc = path_name.find_last_of('.');
    string suffix = path_name.substr(loc + 1);
    return suffix == "h" || suffix == "hpp" || suffix == "c" || suffix == "cpp" || suffix == "cc";
}

// 将 c 或 cpp 文件的头文件排序
void handle(const char* filename, int u) {
	if (access(filename, F_OK)) {
		cout << "第" << u << "个路径无效" << endl;
		return;
	}

    cout << filename << endl;

	ifstream inFile;
	inFile.open(filename);
	int start = -1, cnt = 0;
	string cur;
	vector<string> includesOfQuotes, includesOfAngleBrackets, other;
	while (getline(inFile, cur)) {
		if (cur.substr(0, 10) == "#include <") {
			includesOfAngleBrackets.push_back(cur);
			if (start == -1) start = cnt;
		} else if (cur.substr(0, 10) == "#include \"") {
            includesOfQuotes.push_back(cur);
            if (start == -1) start = cnt;
		} else other.push_back(cur);
		cnt ++ ;
	}
	sort(includesOfQuotes.begin(), includesOfQuotes.end());
    sort(includesOfAngleBrackets.begin(), includesOfAngleBrackets.end());
	inFile.close();

	ofstream outFile;
	outFile.open(filename);
	for (int i = 0; i < start; i ++ ) outFile << other[i] << endl;
	for (string s: includesOfAngleBrackets) outFile << s << endl;
	for (string s: includesOfQuotes) outFile << s << endl;
	for (int i = start; i < other.size(); i ++ ) outFile << other[i] << endl;
	outFile.close();
}

// 处理目录
void getAllFiles(string dir_name, int u) {
 	DIR* dir;
	dir = opendir(dir_name.c_str());
	if(dir == nullptr) {
		cout << "第" << u << "个路径无效" << endl;
		return;
    }
	
	/* read all the files in the dir ~ */
	struct dirent * filename;
	while((filename = readdir(dir)) != nullptr ) {
        string file_name = filename->d_name;

		// ignore "." and ".."
		if(file_name == "." || file_name == "..") continue;

        string path_name = dir_name + '/' + file_name;
        DIR* dir = opendir(path_name.c_str());
        if (dir) getAllFiles(path_name, u);  // 一个有效的目录
        else if (cORcpp(path_name)) handle(path_name.c_str(), u);  // 一个有效的c(cpp)文件 或 无效路径
	}
}

int main(int argc, char* argv[]) {
	if (argc < 2) {
		cout << "参数个数错误" << endl;
		return 1;
	}

	for (int i = 1; i < argc; i ++ ) {
        DIR* dir = opendir(argv[i]);
        if (dir) getAllFiles(argv[i], i);  // 一个有效的目录
        else if (cORcpp(argv[i])) handle(argv[i], i);  // 一个有效的c(cpp)文件 或 无效路径
    }
	
	cout << "finished..." << endl;
	return 0;
}
