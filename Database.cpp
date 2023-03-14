#include "Database.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <direct.h>
#include<cstdio>
#include <io.h>
#include<cctype>
using namespace std;

bool Database::use(const string& filename)
{
    if (_access(filename.c_str(),0)!=0)
    {
        cout << "Can not open " << filename << endl;
        return false;
    }
    db_filepath = filename;
    cout << "Use " << filename << endl;
    return true;
}
bool Database::createdatabase(const string& filename)
{
    if (_mkdir(filename.c_str()) == 0) 
    {
        cout << filename << " created successfully!" << endl;
        return true;
    }
    else cout << "Created failed" << endl;
    return false;

}
bool Database::createtable(const string& tablename, retInfo& info) 
{
    if (db_filepath.empty()) 
    {
        cout << "no database used" << endl;
        return false;
    }
    string filename = db_filepath + "/" + tablename;
    ifstream fin(filename);
    if (fin.is_open())
    {
        cout << tablename << "already been created" << endl;
        return false;
    }
    fin.close();
    ofstream ofs(filename);
    if (!ofs)
    {
        cout << tablename << "created failed" << endl;
        return false;
    }
    ofs << info.primary.info[0] << " " << info.primary.info[1];
    for (int i = 0; i < info.arr.size(); i++)
    {
        ofs << "\n" << info.arr[i].info[0] << " " << info.arr[i].info[1];
    }
    cout << tablename << " created successfully!" << endl;
    ofs.close();
    return true;
}
bool Database::dropdatabase(const string& filename)
{
    if (_rmdir(filename.c_str()) == 0) 
    {
        cout << filename << " droped successfully!" << endl;
        return true;
    }
    else cout << "Droped failed" << endl;
    return false;
}
bool Database::droptable(const string& tablename)
{
    if (db_filepath.empty())
    {
        cout << "no database used" << endl;
        return false;
    }
    string filename = db_filepath + "/" + tablename;
    if (remove(filename.c_str()) == 0)
    {
        cout << tablename << "droped successfully!" << endl;
        return true;
    }
    cout << "Droped failed" << endl;
    return false;
}
bool Database::select(vector<string>& cond_val)
{
    if (db_filepath.empty())
    {
        cout << "No database used" << endl;
        return false;
    }
    ifstream fin(db_filepath + "/" + cond_val[3]);
    if (!fin) 
    {
        cout << "Table does not exist" << endl;
        return false;
    }
    if (cond_val.size() == 8 && (cond_val[6] != ">" && cond_val[6] != "=" && cond_val[6] != "<"))
    {
        cout << "Operator Illegal!" << endl;
        return false;
    }
    conds.clear();
    string data, name, typestr;
    int line = 1;
    bool pri = true;
    while (getline(fin, data))
    {
        vector<int> commaPlace = findChar(data, ',');
        commaPlace.insert(commaPlace.begin(), -1);
        commaPlace.push_back(data.size());
        auto tmpVecStr = splitString(data, commaPlace);
        vector<string> values;
        for (int i = 1; i < tmpVecStr.size(); i++)
        {
            values.push_back(tmpVecStr[i]);
        }
        istringstream iss(tmpVecStr[0]);
        iss >> name >> typestr;
        Condition cond(name, typestr, pri, values);
        conds.push_back(cond);
        pri = false;
        line++;
    }
    fin.close();
    if (cond_val.size() == 4)
    {
        if (cond_val[1] == "*")
        {
            for (auto& cond : conds)
            {
                cout << cond.column << " " << cond.m_type;
                for (string s : cond.Contents) cout << "," << s;
                cout << "\n";
            }
            return true;
        }
        else
        {
            for (auto& cond : conds)
            {
                if (cond.column == cond_val[1])
                {
                    cout << cond.column << " " << cond.m_type;
                    for (string s : cond.Contents) cout << "," << s;
                    cout << "\n";
                    return true;
                }
            }
        }
    }
    vector<int> fits;
    fits.clear();
    bool acrod = false;
    if(cond_val.size() == 8)
    {
        for (auto c : conds)
        {
            if (c.column == cond_val[5])
            {
                if (c.m_type == "int")
                {
                    int oCom = 0;
                    int tCom = atoi(cond_val[7].c_str());
                    for (int i = 0; i < c.Contents.size(); i++)
                    {
                        oCom = atoi(c.Contents[i].c_str());
                        if ((cond_val[6] == "<" && oCom < tCom) || (cond_val[6] == "=" && oCom == tCom) || (cond_val[6] == ">" && oCom > tCom))
                        {
                            fits.push_back(i);
                            acrod = true;
                        }
                    }
                }
                else 
                {
                    if (cond_val[6] != "=")
                    {
                        cout << "The parameter of this operation should not be string!" << endl;
                        return false;
                    }
                    for (int i = 0; i < c.Contents.size(); i++)
                    {
                        if (c.Contents[i] == cond_val[7])
                        {
                            fits.push_back(i);
                            acrod = true;
                        }
                    }
                }
            }
        }
        if (acrod && fits.size() != 0)
        {
            for (auto& cond : conds)
            {
                if (cond_val[1] == "*" || cond.column == cond_val[1])
                {
                    cout << cond.column << " " << cond.m_type;
                    for (int i : fits) cout << "," << cond.Contents[i];
                    cout << "\n";
                }
            }
        }
        else
        {
            cout << "No match!" << endl;
            return false;
        }
    }
}
void Database::insert(string tablename,vector<string>& insert_val)
{
    if (db_filepath.empty()) 
    {
        cout << "No database used!" << endl;
        return;
    }
    ifstream fin(db_filepath + "/" + tablename);
    if (!fin) 
    {
        cout << "Table does not exist" << endl;
        return;
    }
    conds.clear();
    string data,name,typestr;
    int line = 1;
    bool pri = true;
    while (getline(fin, data))
    {
        vector<int> commaPlace = findChar(data, ',');
        commaPlace.insert(commaPlace.begin(), -1);
        commaPlace.push_back(data.size());
        auto tmpVecStr = splitString(data, commaPlace);
        vector<string> values;
        for (int i = 1; i < tmpVecStr.size(); i++)
        {
            values.push_back(tmpVecStr[i]);
        }
        istringstream iss(tmpVecStr[0]);
        iss >> name >> typestr;
        Condition cond(name, typestr, pri, values);
        conds.push_back(cond);
        pri = false;
        line++;
    }
    if (conds.size() != insert_val.size()) 
    {
        cout << "Illegal parameter number!" << endl;
        conds.clear();
        return;
    }
    if (!isInt(insert_val[0]) && conds[0].m_type == "int")
    {
        cout << "Illegal parameter type1!" << endl;
        conds.clear();
        return;
    }
    for (int i = 0; i < conds[0].Contents.size(); i++)
    {
        if (insert_val[0] == conds[0].Contents[i])
        {
            cout << "Duplicate parameter!" << endl;
            conds.clear();
            return;
        }
    }
    conds[0].Contents.push_back(insert_val[0]);
    for (int i = 1; i < conds.size(); i++)
    {
        if (!isInt(insert_val[i]) && conds[i].m_type == "int")
        {
            cout << "Illegal parameter type2!" << endl;
            conds.clear();
            return;
        }
        conds[i].Contents.push_back(insert_val[i]);
    }
    ofstream fout(db_filepath + "/" + tablename);
    for (auto& cond : conds)
    {
        fout << cond.column << " " << cond.m_type;
        for (string s:cond.Contents)
        {
            fout << "," << s;
        }
        fout << "\n";
    }
    cout << "Insert success." << endl;
    conds.clear();
}

void Database::delete_m(vector<string>& cond_val)
{
    if (db_filepath.empty())
    {
        cout << "No database used" << endl;
        return;
    }
    ifstream fin(db_filepath + "/" + cond_val[1]);
    if (!fin)
    {
        cout << "Table does not exist" << endl;
        return;
    }
    if (cond_val[4] != ">" && cond_val[4] != "=" && cond_val[4] != "<")
    {
        cout << "Operator Illegal!" << endl;
        return;
    }
    conds.clear();
    string data, name, typestr;
    int line = 1;
    bool pri = true;
    while (getline(fin, data))
    {
        vector<int> commaPlace = findChar(data, ',');
        commaPlace.insert(commaPlace.begin(), -1);
        commaPlace.push_back(data.size());
        auto tmpVecStr = splitString(data, commaPlace);
        vector<string> values;
        for (int i = 1; i < tmpVecStr.size(); i++) values.push_back(tmpVecStr[i]);
        istringstream iss(tmpVecStr[0]);
        iss >> name >> typestr;
        Condition cond(name, typestr, pri, values);
        conds.push_back(cond);
        pri = false;
        line++;
    }
    fin.close();
    vector<int> fits;
    fits.clear();
    bool acrod = false;
    for (auto c : conds)
    {
        if (c.column == cond_val[3])
        {
            if (c.m_type == "int")
            {
                int oCom = 0;
                int tCom = atoi(cond_val[5].c_str());
                for (int i = 0; i < c.Contents.size(); i++)
                {
                    oCom = atoi(c.Contents[i].c_str());
                    if ((cond_val[4] == "<" && oCom < tCom) || (cond_val[4] == "=" && oCom == tCom) || (cond_val[4] == ">" && oCom > tCom))
                    {
                        fits.push_back(i);
                        acrod = true;
                    }
                }
            }
            else
            {
                if (cond_val[6] != "=")
                {
                    cout << "The parameter of this operation should not be string!" << endl;
                    return;
                }
                for (int i = 0; i < c.Contents.size(); i++)
                {
                    if (c.Contents[i] == cond_val[5])
                    {
                        fits.push_back(i);
                        acrod = true;
                    }
                }
            }
        }
    }
    if (acrod && fits.size() != 0)
    {
        for (auto& cond : conds)
        {
            for (int i = fits.size() - 1; i >= 0; i--)
            {
                cond.Contents.erase(cond.Contents.begin() + fits[i]);
            }
        }
    }
    else
    {
        cout << "No match!" << endl;
        return;
    }
    ofstream fout(db_filepath + "/" + cond_val[1]);
    for (auto& cond : conds)
    {
        fout << cond.column << " " << cond.m_type;
        for (string s : cond.Contents)
        {
            fout << "," << s;
        }
        fout << "\n";
    }
    cout << "Delete success." << endl;
}

