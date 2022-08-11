#include <iostream>
#include "format.h"

/*
* Analysis Recent Saved File from Register
* HKEY_CURRENT_USER\SOFTWARE\Microsoft\Windows\CurrentVersion\Explorer\ComDlg32\OpenSavePidlMRU
*/
using namespace std;

int main() {
	std::locale::global(std::locale(""));
	Parser obj;
	obj.GetMRU();
	wcout << L"Recent saved files:" << endl;
	unordered_map<wstring, vector<wstring>> res = obj.GetInfo();
	for (auto& it : res) {
		wcout << L"=>" << it.first << L":" << endl;
		for (auto& path : it.second) {
			wcout << L"\t" << path << endl;
		}
	}
	
	return 0;
}

