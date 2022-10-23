#include <database_manager.hpp>
#include <iostream>
#include <string>
#include <utility>
#include <unordered_map>
#include <fmt/core.h>

static std::vector<std::string> items;

static int callback_numrows(void *count, int argc, char **argv, char **azColName)
{
    int *c = (int *)count;
    *c = atoi(argv[0]);
    return 0;
}

DatabaseManager::DatabaseManager(const std::filesystem::path &path)
{
    db_path = std::filesystem::canonical(path);
    sqlite3_open(db_path.string().c_str(), &db);
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

void DatabaseManager::insert_rows(const std::vector<FBuffer> &rows)
{
    for (const auto &row : rows)
    {
        std::string sql_command = fmt::format(
            "INSERT INTO ECOSYSTEM_MASTER VALUES ({}, ZEROBLOB({}))",
            Ecosystem::GetWorld(row.data())->year(),
            row.size());
        sqlite3_exec(db, sql_command.c_str(), nullptr, 0, nullptr);

        sqlite3_blob *newBlob = 0;
        sqlite3_blob_open(db, "main", "ECOSYSTEM_MASTER", "AVG_WORLD", sqlite3_last_insert_rowid(db), 1, &newBlob);
        sqlite3_blob_write(newBlob, row.data(), row.size(), 0);
        sqlite3_blob_close(newBlob);
    }
}

std::vector<FBuffer> DatabaseManager::read_all_rows()
{
    int count = 0;
    sqlite3_exec(db, "SELECT COUNT(YEAR) FROM ECOSYSTEM_MASTER", callback_numrows, &count, nullptr);

    std::vector<FBuffer> rows;
    rows.reserve(count);

    for (int i = 0; i < count; i++)
    {
        sqlite3_blob *blob = 0;
        sqlite3_blob_open(db, "main", "ECOSYSTEM_MASTER", "AVG_WORLD", i + 1, 0, &blob);
        int size = sqlite3_blob_bytes(blob);

        FBuffer data(size);
        sqlite3_blob_read(blob, data.data(), data.size(), 0);
        sqlite3_blob_close(blob);
        rows.push_back(data);
    }
    return rows;
}

void DatabaseManager::clear_database()
{
    std::string sql_command = "DELETE FROM ECOSYSTEM_MASTER;";
    sqlite3_exec(db, sql_command.c_str(), nullptr, 0, nullptr);
}
