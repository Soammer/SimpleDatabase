#pragma once
#include <string>
#include <map>
#include<vector>
#include "Condition.h"
#include "retInfo.h"
using namespace std;


class SQLParser
{
public:
    enum class SQLType
    {
        UNKNOWN,
        CREATE,
        DROP,
        USE,
        SELECT,
        INSERT,
        DELETE,
        EXIT,
    };
    enum class Address
    {
        UNKNOWN,
        DATABASE,
        TABLE,
    };
    enum class State
    {
        GOOD,
        BAD,
    };
    SQLParser(const string& command);
    SQLType get_sql_type(void) const;
    const string& get_name(void) const;
    State get_state(void) const;
    Address get_address(void) const;
    vector<string> insertValues;
    vector<string> condValues;
    vector<Condition> Values;
    retInfo createInfo;
private:
    void parser(const string& command);
    SQLType m_type;
    string m_name;
    State m_state;
    Address m_address;
};

