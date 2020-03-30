#ifndef DATABASEMANAGER_HPP
#define DATABASEMANAGER_HPP

#include <filesystem>
#include <iostream>
#include <sqlite3.h>
#include <string>
#include <unordered_map>
#include <vector>
#include <animal.hpp>

struct DatabaseManager
{
    /***************
     *  Attributes *
     ***************/

    sqlite3 *db;
    std::filesystem::path db_path;


    /******************************
     *  Constructor / Destructor  *
     ******************************/

    DatabaseManager(const std::filesystem::path &path = "../../data/ecosystem_master.db");
    ~DatabaseManager();


    /******************************
     *  Standard DBMS operations  *
     ******************************/

    std::vector<std::vector<STAT>> readRows(const std::string &, const std::vector<std::string> &);
    void deleteRows(const std::vector<std::string> &);
    void insertRows(const std::vector<std::vector<STAT>>&);
    void updateRows(const std::vector<std::vector<STAT>>&);


    /******************************
     *  Miscellaneous operations  *
     ******************************/

    std::unordered_map<std::string, std::vector<std::vector<STAT>>> groupByKind();
    void clearDatabase();
};

#endif // DATABASEMANAGER_HPP
