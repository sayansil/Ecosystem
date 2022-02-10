#include <database_manager.hpp>
#include <iostream>
#include <string>
#include <utility>
#include <unordered_map>

static std::vector<std::string> gRowsMaster;

static std::vector<std::string> items;

static int callback_read_master(void *data, int argc, char **argv, char **colName)
{
    gRowsMaster.push_back(argv[0]);
    return 0;
}

DatabaseManager::DatabaseManager(const std::filesystem::path &path)
{
    db_path = std::filesystem::canonical(path);
    sqlite3_open(db_path.c_str(), &db);
    begin_transaction();
}

DatabaseManager::~DatabaseManager()
{
    end_transaction();
    sqlite3_close(db);
}

void DatabaseManager::begin_transaction()
{
    sqlite3_exec(db, "BEGIN TRANSACTION", nullptr, nullptr, nullptr);
}

void DatabaseManager::end_transaction()
{
    sqlite3_exec(db, "END TRANSACTION", nullptr, nullptr, nullptr);
}

void DatabaseManager::insert_rows(const std::vector<flatbuffers::DetachedBuffer> &rows)
{
    for (const auto &row : rows)
    {
        std::string blob = ""; // TODO from buffer pointer to string

        std::string sql_command = "INSERT INTO ECOSYSTEM_MASTER VALUES (" + blob + ")";
        sqlite3_exec(db, sql_command.c_str(), nullptr, 0, nullptr);
    }
}

std::vector<std::vector<uint8_t>> DatabaseManager::read_all_rows()
{
    gRowsMaster.clear();
    gRowsMaster.shrink_to_fit();
    std::string sql_command = "SELECT * FROM ECOSYSTEM_MASTER";
    sqlite3_exec(db, sql_command.c_str(), callback_read_master, 0, nullptr);

    std::vector<std::vector<uint8_t>> rows;
    for (auto &gRow : gRowsMaster)
    {
        std::vector<uint8_t> row;
        std::copy(gRow.begin(), gRow.end(), std::back_inserter(row));

        rows.push_back(row);
    }

    return rows;
}

void DatabaseManager::clear_database()
{
    std::string sql_command = "DELETE FROM ECOSYSTEM_MASTER;";
    sqlite3_exec(db, sql_command.c_str(), nullptr, 0, nullptr);
}
