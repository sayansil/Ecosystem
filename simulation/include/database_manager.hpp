#ifndef DATABASEMANAGER_HPP
#define DATABASEMANAGER_HPP

#include <filesystem>
#include <sqlite3.h>
#include <vector>
#include <helper.hpp>
#include <ecosystem_types.hpp>
#include <world_generated.h>

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

    DatabaseManager(const std::filesystem::path &filename = helper::ecosystem_root / "data/ecosystem_master.db");
    ~DatabaseManager();

    /******************************
     *  Standard DBMS operations  *
     ******************************/

    std::vector<std::vector<ByteArray>> read_all_rows();
    void insert_rows(const std::vector<std::vector<FBufferView>> &);

    /******************************
     *  Miscellaneous operations  *
     ******************************/

    void begin_transaction();
    void end_transaction();
    void clear_database();
};

#endif // DATABASEMANAGER_HPP
