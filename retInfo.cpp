#include "retInfo.h"
int retInfo::err = 0;
int retInfo::tmpInfo::err = 0;
retInfo::tmpInfo::tmpInfo(string tmpStr)
{
	tmpStr = strip(tmpStr);
	auto splitPlace = findChar(tmpStr, ' ');
	splitPlace.insert(splitPlace.begin(), -1);
	splitPlace.push_back(tmpStr.size());
	info = splitString(tmpStr, splitPlace);
	if (info.size() < 2 || info.size() > 4) err = 1;
	return;
}

retInfo::tmpInfo::tmpInfo()
{
	return;
}

int retInfo::checkPrimary() 
{
	int cnt = 0, place = -1;
	for (int p = 0; p < arr.size(); p++)
	{
		auto& i = arr[p];
		if (i.info.size() > 2 && i.info[2] == "primary")
		{
			cnt++, primary = i, place = p;
			while (i.info.size() > 2)
				i.info.pop_back();
		}
	}
	if (cnt > 1 || cnt < 1) return -1;
	arr.erase(arr.begin() + place);
	return cnt;
}

retInfo::retInfo(string cmd)
{
	tmpInfo::err = 0;
	vector<int> commaPlace = findChar(cmd, ',');

	commaPlace.insert(commaPlace.begin(), -1);//º”Õ∑
	commaPlace.push_back(cmd.size());//º”Œ≤
	auto tmpVecStr = splitString(cmd, commaPlace);
	for (auto i : tmpVecStr) arr.push_back(tmpInfo(i));
	int primaryNum = checkPrimary();
	if (primaryNum != 1 || tmpInfo::err) err = 1;
	return;
}

retInfo::retInfo() 
{
	return;
}
