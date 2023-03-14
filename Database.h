#include <string>
using namespace std;
#include <vector>
#include "retInfo.h"
#include "Condition.h"
class Database
{
public:
    bool use(const string& database_file);
    bool createdatabase(const string& database_file);
    bool dropdatabase(const string& database_file);
    bool createtable(const string& table_file, retInfo& info);
    bool droptable(const string& table_file);
    bool select(vector<string>& cond_val);
    void insert(string table, vector<string>& insert_val);
    void delete_m(vector<string>& cond_val);
private:
    string db_filepath;
    vector<Condition> conds;
};