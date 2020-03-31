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

    std::vector<std::vector<STAT>> read_rows(const std::string &, const std::vector<std::string> &);
    void delete_rows(const std::vector<std::string> &);
    void insert_rows(const std::vector<std::vector<STAT>> &);
    void update_rows(const std::vector<std::vector<STAT>> &);

    void insert_stat_row(const std::vector<STAT> &, const std::string &, const std::string &);

    /******************************
     *  Miscellaneous operations  *
     ******************************/

    std::unordered_map<std::string, std::vector<std::vector<STAT>>> groupby_kind();
    void clear_database();
    void clear_table(const std::string &);
};

#endif // DATABASEMANAGER_HPP
