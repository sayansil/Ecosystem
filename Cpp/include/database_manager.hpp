#ifndef DATABASEMANAGER_HPP
#define DATABASEMANAGER_HPP

#include <experimental/filesystem>
#include <iostream>
#include <sqlite3.h>
#include <string>
#include <unordered_map>
#include <vector>
#include <ecosystem_types.hpp>
#include <animal.hpp>
#include <schema.hpp>

struct DatabaseManager
{
    /***************
     *  Attributes *
     ***************/

    sqlite3 *db;
    std::experimental::filesystem::path db_path;


    /******************************
     *  Constructor / Destructor  *
     ******************************/

    DatabaseManager(const std::experimental::filesystem::path& filename = helper::get_ecosystem_root() / "data/ecosystem_master.db");
    ~DatabaseManager();


    /******************************
     *  Standard DBMS operations  *
     ******************************/

    std::vector<std::vector<DBType>> read_rows_master(const std::string &, const std::vector<std::string> &);
    std::vector<std::vector<DBType>> read_all_rows_stats(const std::string &);
    std::vector<std::vector<DBType>> read_all_rows_master();
    void delete_rows(const std::vector<std::string> &);
    void insert_rows(const std::vector<std::vector<DBType>> &);
    void update_rows(const std::vector<std::vector<DBType>> &);

    void insert_stat_row(const std::vector<DBType> &, const std::string &);

    /******************************
     *  Miscellaneous operations  *
     ******************************/

    void begin_transaction();
    void end_transaction();
    std::unordered_map<std::string, std::vector<std::vector<DBType>>> groupby_kind();
    void clear_database();
    void clear_table(const std::string &);
};

#endif // DATABASEMANAGER_HPP
