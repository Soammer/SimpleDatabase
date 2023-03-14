#include "Condition.h"
Condition::Condition(const string& str, const string type, const bool pri,vector<string>& vec):column(str),m_type(type),primary(pri), Contents(vec)
{
}