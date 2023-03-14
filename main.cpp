#include "Database.h"
#include "SQLParser.h"
#include <iostream>

using namespace std;

int main()
{
    Database db;
    while (true)
    {
        cout<<"db>";
        string command;
        getline(cin, command);
        if (command.size() > 0)
        {
            SQLParser parser(command);
            if (parser.get_state() != SQLParser::State::GOOD)
            {
                continue;
            }
            switch (parser.get_sql_type())
            {
            case SQLParser::SQLType::CREATE:
                if(parser.get_address() == SQLParser::Address::DATABASE)db.createdatabase(parser.get_name());
                else if (parser.get_address() == SQLParser::Address::TABLE)db.createtable(parser.get_name(),parser.createInfo);
                break;
            case SQLParser::SQLType::DROP:
                if (parser.get_address() == SQLParser::Address::DATABASE)db.dropdatabase(parser.get_name());
                else if (parser.get_address() == SQLParser::Address::TABLE)db.droptable(parser.get_name());
                break;
            case SQLParser::SQLType::USE:
                db.use(parser.get_name()); break;
            case SQLParser::SQLType::INSERT:
                db.insert(parser.get_name(), parser.insertValues);
                parser.insertValues.clear();
                break;
            case SQLParser::SQLType::SELECT:
                db.select(parser.condValues);
                parser.condValues.clear();
                break;
            case SQLParser::SQLType::DELETE:
                db.delete_m(parser.condValues);
                parser.condValues.clear(); 
                break;
            case SQLParser::SQLType::EXIT:
                cout << "Database exited." << endl;
                return 0;
            default:
                cout << "SQL not valid!" << endl;
                break;
            }
        }
    }
    return 0;
}