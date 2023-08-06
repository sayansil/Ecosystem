#include <fmt/core.h>

#include <database_manager.hpp>
#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>

static std::vector<std::string> items;

static int callback_numrows(void *count, int argc, char **argv,
                            char **azColName) {
    int *c = (int *)count;
    *c = atoi(argv[0]);
    return 0;
}

DatabaseManager::DatabaseManager(const std::filesystem::path &path) {
    db_path = std::filesystem::canonical(path);
    sqlite3_open(db_path.string().c_str(), &db);
    sqlite3_exec(db, "PRAGMA synchronous = OFF", NULL, NULL, NULL);
    sqlite3_exec(db, "PRAGMA journal_mode = MEMORY", NULL, NULL, NULL);
    begin_transaction();
}

DatabaseManager::~DatabaseManager() {
    end_transaction();
    sqlite3_close(db);
}

void DatabaseManager::begin_transaction() {
    sqlite3_exec(db, "BEGIN TRANSACTION", nullptr, nullptr, nullptr);
}

void DatabaseManager::end_transaction() {
    sqlite3_exec(db, "END TRANSACTION", nullptr, nullptr, nullptr);
}

void DatabaseManager::insert_rows(
    const std::vector<std::vector<FBufferView>> &rows) {
    for (const auto &row : rows) {
        auto &avg_world = row[0];
        auto &population = row[1];
        std::string sql_command = fmt::format(
            "INSERT INTO ECOSYSTEM_MASTER VALUES ({}, ZEROBLOB({}), "
            "ZEROBLOB({}))",
            Ecosystem::GetWorld(avg_world.data)->year(), avg_world.size,
            population.size);
        sqlite3_exec(db, sql_command.c_str(), nullptr, 0, nullptr);

        auto last_insert_row = sqlite3_last_insert_rowid(db);

        sqlite3_blob *avgBlob = 0;
        sqlite3_blob_open(db, "main", "ECOSYSTEM_MASTER", "AVG_WORLD",
                          last_insert_row, 1, &avgBlob);
        sqlite3_blob_write(avgBlob, avg_world.data, avg_world.size, 0);
        sqlite3_blob_close(avgBlob);

        sqlite3_blob *populationBlob = 0;
        sqlite3_blob_open(db, "main", "ECOSYSTEM_MASTER", "POPULATION_WORLD",
                          last_insert_row, 1, &populationBlob);
        sqlite3_blob_write(populationBlob, population.data, population.size, 0);
        sqlite3_blob_close(populationBlob);
    }
}

std::vector<std::vector<ByteArray>> DatabaseManager::read_all_rows() {
    int count = 0;
    sqlite3_exec(db, "SELECT COUNT(YEAR) FROM ECOSYSTEM_MASTER",
                 callback_numrows, &count, nullptr);

    std::vector<std::vector<ByteArray>> rows;
    rows.reserve(count);

    for (int i = 0; i < count; i++) {
        std::vector<ByteArray> row(2);

        sqlite3_blob *avgBlob = 0;
        sqlite3_blob_open(db, "main", "ECOSYSTEM_MASTER", "AVG_WORLD", i + 1, 0,
                          &avgBlob);
        int size = sqlite3_blob_bytes(avgBlob);

        auto &avg_data = row[0];
        avg_data = ByteArray(size);
        sqlite3_blob_read(avgBlob, avg_data.data(), avg_data.size(), 0);
        sqlite3_blob_close(avgBlob);

        sqlite3_blob *populationBlob = 0;
        sqlite3_blob_open(db, "main", "ECOSYSTEM_MASTER", "POPULATION_WORLD",
                          i + 1, 0, &populationBlob);
        size = sqlite3_blob_bytes(populationBlob);

        auto &population_data = row[1];
        population_data = ByteArray(size);
        sqlite3_blob_read(populationBlob, population_data.data(),
                          population_data.size(), 0);
        sqlite3_blob_close(populationBlob);

        rows.emplace_back(row);
    }
    return rows;
}

void DatabaseManager::clear_database() {
    std::string sql_command = "DELETE FROM ECOSYSTEM_MASTER;";
    sqlite3_exec(db, sql_command.c_str(), nullptr, 0, nullptr);
}
