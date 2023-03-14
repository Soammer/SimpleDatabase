#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
using namespace std;

inline string strip(string& s)
{
	if (s.empty())
		return s;
	s.erase(0, s.find_first_not_of(" "));
	s.erase(s.find_last_not_of(" ") + 1);
	return s;
}
inline vector<int> findChar(const string& s, char target)
{
	vector<int> ret;
	for (int i = 0; i < s.size(); i++)
		if (s[i] == target) ret.push_back(i);
	return ret;
}
inline vector<string> splitString(const string& s, const vector<int>& splitPlace)
{
	vector<string> ret;
	for (int i = 1; i < splitPlace.size(); i++) {
		string tmpStr = s.substr(splitPlace[i - 1] + 1, splitPlace[i] - splitPlace[i - 1] - 1);
		ret.push_back(tmpStr);
	}
	return ret;
}
inline bool isInt(const string& s)
{
	for (char i : s) if (!isdigit(i)) return false;
	return true;
}
class retInfo 
{
	class tmpInfo
	{
	public:
		static int err;
		vector<string> info;
		tmpInfo(string tmpStr);
		tmpInfo();
	};
private:
	int checkPrimary();
public:
	vector<tmpInfo> arr;
	tmpInfo primary;
	static int err;
	retInfo(string cmd);
	retInfo();
};
