#pragma once
#include <sstream>
#include <iostream>
#include <vector>
using namespace std;
class Condition
{
public:
	string column;
    bool primary;
    string m_type;
    vector<string> Contents;
    Condition(const string& str,const string type,const bool pri,vector<string>& vec);
};

