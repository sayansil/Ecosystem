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

void DatabaseManager::insert_rows(const std::vector<std::vector<FBuffer>> &rows)
{
    for (const auto &row : rows)
    {
        auto avg_world = row[0];
        auto population = row[1];
        std::string sql_command = fmt::format(
            "INSERT INTO ECOSYSTEM_MASTER VALUES ({}, ZEROBLOB({}), ZEROBLOB({}))",
            Ecosystem::GetWorld(avg_world.data())->year(),
            avg_world.size(),
            population.size());
        sqlite3_exec(db, sql_command.c_str(), nullptr, 0, nullptr);


        auto last_insert_row = sqlite3_last_insert_rowid(db);

        sqlite3_blob *avgBlob = 0;
        sqlite3_blob_open(db, "main", "ECOSYSTEM_MASTER", "AVG_WORLD", last_insert_row, 1, &avgBlob);
        sqlite3_blob_write(avgBlob, avg_world.data(), avg_world.size(), 0);
        sqlite3_blob_close(avgBlob);

        sqlite3_blob *populationBlob = 0;
        sqlite3_blob_open(db, "main", "ECOSYSTEM_MASTER", "POPULATION_WORLD", last_insert_row, 1, &populationBlob);
        sqlite3_blob_write(populationBlob, population.data(), population.size(), 0);
        sqlite3_blob_close(populationBlob);
    }
}

std::vector<std::vector<FBuffer>> DatabaseManager::read_all_rows()
{
    int count = 0;
    sqlite3_exec(db, "SELECT COUNT(YEAR) FROM ECOSYSTEM_MASTER", callback_numrows, &count, nullptr);

    std::vector<std::vector<FBuffer>> rows;
    rows.reserve(count);

    for (int i = 0; i < count; i++)
    {
        std::vector<FBuffer> row;

        sqlite3_blob *avgBlob = 0;
        sqlite3_blob_open(db, "main", "ECOSYSTEM_MASTER", "AVG_WORLD", i + 1, 0, &avgBlob);
        int size = sqlite3_blob_bytes(avgBlob);

        FBuffer avg_data(size);
        sqlite3_blob_read(avgBlob, avg_data.data(), avg_data.size(), 0);
        sqlite3_blob_close(avgBlob);
        row.push_back(avg_data);

        sqlite3_blob *populationBlob = 0;
        sqlite3_blob_open(db, "main", "ECOSYSTEM_MASTER", "POPULATION_WORLD", i + 1, 0, &populationBlob);
        size = sqlite3_blob_bytes(populationBlob);

        FBuffer population_data(size);
        sqlite3_blob_read(populationBlob, population_data.data(), population_data.size(), 0);
        sqlite3_blob_close(populationBlob);
        row.push_back(population_data);

        rows.push_back(row);
    }
    return rows;
}

void DatabaseManager::clear_database()
{
    std::string sql_command = "DELETE FROM ECOSYSTEM_MASTER;";
    sqlite3_exec(db, sql_command.c_str(), nullptr, 0, nullptr);
}
