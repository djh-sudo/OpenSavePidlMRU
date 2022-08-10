#pragma once
#include <cstdlib>
#include <string>
#include <vector>
#include <Windows.h>
#include <shlobj.h>

#define MAX_LEN 1024

class Parser{
	
public:
	bool ParseMRUValue(HKEY subKey) {
		DWORD i = 0, szValueName = MAX_PATH, szBuffer = MAX_LEN;
		wchar_t valueName[MAX_PATH] = { 0 };
		char buffer[MAX_LEN] = { 0 };
		wchar_t filePath[MAX_PATH] = { 0 };
		while (RegEnumValueW(subKey, ++i, valueName, &szValueName, 0, NULL, (LPBYTE)buffer, &szBuffer)) {
			if (wcscmp(valueName, L"MRUListEx") != 0) {
				LPITEMIDLIST file_pidl = (LPITEMIDLIST)new BYTE[szBuffer + 1];
				if (file_pidl == NULL) {
					continue;
				}
				else {
					memcpy(file_pidl, buffer, szBuffer);
					memset(filePath, 0, MAX_PATH);
					if (SHGetPathFromIDListW(file_pidl, filePath)) {
						paths.push_back(filePath);
					}
					delete[] file_pidl;
				}
			}
			
			szValueName = MAX_PATH;
			szBuffer = MAX_LEN;
		}
	}

private:
	std::vector<std::wstring>paths;
};
