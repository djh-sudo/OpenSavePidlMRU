#pragma once
#include <cstdlib>
#include <string>
#include <vector>
#include <unordered_map>
#include <Windows.h>
#include <shlobj.h>

#define MAX_LEN 1024

class Parser{
	
public:

	void ParseMRUValue(HKEY subKey, std::vector<std::wstring>& paths) {
		DWORD i = 0, szValueName = MAX_PATH, szBuffer = MAX_LEN;
		wchar_t valueName[MAX_PATH] = { 0 };
		char buffer[MAX_LEN] = { 0 };
		wchar_t filePath[MAX_PATH] = { 0 };
		while (RegEnumValueW(subKey, i++, valueName, &szValueName, 0, NULL, (LPBYTE)buffer, &szBuffer) == ERROR_SUCCESS) {
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
			memset(buffer, 0, MAX_LEN);
			memset(valueName, 0, MAX_PATH * sizeof(wchar_t));
		}
	}
	
	void GetMRU() {
		const wchar_t mruPath[] = L"Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\ComDlg32\\OpenSavePidlMRU";
		HKEY subKey = NULL, enumKey = NULL;
		DWORD szKeyName = MAX_PATH, i = 0;
		wchar_t keyName[MAX_PATH] = { 0 };

		LSTATUS status = ERROR_SUCCESS, check = ERROR_SUCCESS;
		status = RegOpenKeyExW(HKEY_CURRENT_USER, mruPath, 0, KEY_READ, &subKey);
		do {
			szKeyName = MAX_PATH;
			memset(keyName, 0, MAX_PATH * sizeof(wchar_t));

			if (status != ERROR_SUCCESS) {
				continue;
			}
			status = RegEnumKeyExW(subKey, i++, keyName, &szKeyName, 0, NULL, NULL, NULL);
			if (status != ERROR_SUCCESS) {
				continue;
			}
			check = RegOpenKeyExW(subKey, keyName, 0, KEY_READ, &enumKey);
			if (check != ERROR_SUCCESS) {
				continue;
			}
			structures[keyName] = std::vector<std::wstring>();
			ParseMRUValue(enumKey, structures[keyName]);
			RegCloseKey(enumKey);
		} while (status == ERROR_SUCCESS);

		RegCloseKey(subKey);
	}

	std::unordered_map<std::wstring, std::vector<std::wstring>>& GetInfo() {
		return structures;
	}

	Parser() = default;
	~Parser() = default;

private:
	std::unordered_map<std::wstring, std::vector<std::wstring>> structures;
};
