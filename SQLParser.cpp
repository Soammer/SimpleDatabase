#include "SQLParser.h"
#include <sstream>
#include <string>
#include <iostream>
constexpr auto SynError = "ERROR: You have an error in your SQL syntax!";

SQLParser::SQLParser(const string & command):m_state(State::GOOD)
{
    parser(command);
}

SQLParser::SQLType SQLParser::get_sql_type(void) const
{
    return m_type;
}

const string & SQLParser::get_name(void) const
{
    return m_name;
}

SQLParser::State SQLParser::get_state(void) const
{
    return m_state;
}
SQLParser::Address SQLParser::get_address(void) const
{
    return m_address;
}


void SQLParser::parser(const string& command)
{
    if (command.size() < 4)
    {
        m_type = SQLType::UNKNOWN;
        return;
    }
    istringstream iss(command);
    string operatorWord;
    string operatorAddress;
    string operatorSyn = iss.str();
    iss >> operatorWord;
    if (operatorWord == "create")
    {
        m_type = SQLType::CREATE;
        iss >> operatorAddress >> m_name;
        if (operatorAddress == "database")
        {
            if (m_name.empty())
            {
                m_state = State::BAD;
                cout << "name does not exist!" << endl;
            }
            else
            {
                m_address = Address::DATABASE;
            }
        }
        else if (operatorAddress == "table") 
        {
            if (m_name.empty())
            {
                m_state = State::BAD;
                cout << "name does not exist!" << endl;
            }
            else
            {
                int left = -1, right = -1;
                for (int i = 0; i < operatorSyn.size(); i++)
                {
                    if (operatorSyn[i] == '(')
                    {
                        if(left == -1 && right == -1) left = i;
                        else 
                        {
                            cout << SynError << endl;
                            m_state = State::BAD;
                            return;
                        }
                    }
                    else if (operatorSyn[i] == ')') 
                    {
                        if(right == -1 && left != -1) right = i;
                        else
                        {
                            cout << SynError << endl;
                            m_state = State::BAD;
                            return;
                        }
                    }
                }
                if (left == -1 || right - left == 1) 
                {
                    cout << "No parameter included! " << endl;
                    m_state = State::BAD;
                    return;
                }
                else 
                {
                    operatorSyn = operatorSyn.substr(left + 1, right - left - 1);
                    createInfo = retInfo(operatorSyn);
                    if (retInfo::err) 
                    {
                        cout << SynError << endl;
                        m_state = State::BAD;
                        return;
                    }
                }
                m_address = Address::TABLE;
            }
        }
        else
        {
            m_state = State::BAD;
            cout << SynError << endl;
        }
    }
    else if(operatorWord == "drop")
    {
        m_type = SQLType::DROP;
        iss >> operatorAddress >> m_name;
        if (operatorAddress == "database")
        {
            if (m_name.empty())
            {
                m_state = State::BAD;
                cout << "name does not exist!" << endl;
            }
            else
            {
                m_address = Address::DATABASE;
            }
        }
        else if (operatorAddress == "table") {
            if (m_name.empty())
            {
                m_state = State::BAD;
                cout << "name does not exist!" << endl;
            }
            else
            {
                m_address = Address::TABLE;
            }
        }
        else
        {
            m_state = State::BAD;
            cout << SynError << endl;
        }
    }
    else if (operatorWord == "use")
    {
        m_type = SQLType::USE;
        iss >> m_name;
        if (m_name.empty())
        {
            m_state = State::BAD;
            cout << "name does not exist!" << endl;
        }
    }
    else if (operatorWord == "insert")
    {
        m_type = SQLType::INSERT;
        string values;
        iss >> m_name >> values;
        if (m_name.empty() || values.empty() || values.rfind("values", 0) != 0)
        {
            m_state = State::BAD;
            cout << SynError << endl;
            return;
        }
        int left = -1, right = -1;
        for (int i = 0; i < operatorSyn.size(); i++)
        {
            if (operatorSyn[i] == '(')
            {
                if (left == -1 && right == -1) left = i;
                else
                {
                    cout << SynError << endl;
                    m_state = State::BAD;
                    return;
                }
            }
            else if (operatorSyn[i] == ')')
            {
                if (right == -1 && left != -1) right = i;
                else
                {
                    cout << SynError << endl;
                    m_state = State::BAD;
                    return;
                }
            }
        }
        if (left == -1 || right - left == 1)
        {
            cout << "No parameter included! " << endl;
            m_state = State::BAD;
            return;
        }
        operatorSyn = operatorSyn.substr(left + 1, right - left - 1);
        vector<int> commaPlace = findChar(operatorSyn, ',');
        commaPlace.insert(commaPlace.begin(), -1);
        commaPlace.push_back(operatorSyn.size());
        auto tmpVec = splitString(operatorSyn, commaPlace);
        for (size_t i = 0; i < tmpVec.size(); i++)
        {
            insertValues.push_back(strip(tmpVec[i]));
        }
    }
    else if (operatorWord == "select")
    {
        m_type = SQLType::SELECT;
        vector<int> commaPlace = findChar(operatorSyn, ' ');
        commaPlace.insert(commaPlace.begin(), -1);
        commaPlace.push_back(operatorSyn.size());
        condValues = splitString(operatorSyn, commaPlace);
        if (condValues.size() != 8 && condValues.size() != 4)
        {
            cout << SynError << endl;
            return;
        }
        if((condValues.size() == 8 && condValues[4] != "where") || condValues[2] != "from")
        {
            m_state = State::BAD;
            cout << SynError << endl;
            return;
        }
    }
    else if (operatorWord == "delete")
    {
        m_type = SQLType::DELETE;
        vector<int> commaPlace = findChar(operatorSyn, ' ');
        commaPlace.insert(commaPlace.begin(), -1);
        commaPlace.push_back(operatorSyn.size());
        condValues = splitString(operatorSyn, commaPlace);
        if (condValues.size() != 6)
        {
            cout << SynError << endl;
            return;
        }
        if (condValues[2] != "where")
        {
            m_state = State::BAD;
            cout << SynError << endl;
            return;
        }
    }
    else if (operatorWord == "exit")
    {
        m_type = SQLType::EXIT;
    }
    else
    {
        cout << iss.str()<< SynError << endl;
        m_type = SQLType::UNKNOWN;
        m_state = State::BAD;
    }
}


