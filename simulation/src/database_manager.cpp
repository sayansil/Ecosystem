#include <fmt/core.h>

#include <database_manager.hpp>
#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>
#include <algorithm> // Required for std::sort
#include <vector>    // Required for std::vector

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
    for (const auto &row_views : rows) { // Renamed 'row' to 'row_views' for clarity
        if (row_views.empty()) continue;

        const auto &avg_world_view = row_views[0];
        const auto &population_view = row_views[1];

        // Determine year from avg_world_view, assuming it's a valid World flatbuffer
        // This part is a bit risky if avg_world_view.data is not a valid World buffer.
        // However, Ecosystem::GetWorld is used elsewhere, so we follow the pattern.
        // A more robust way might be to pass the year explicitly to insert_rows.
        int year = Ecosystem::GetWorld(avg_world_view.data)->year();

        std::string sql_command;
        if (row_views.size() >= 3) {
            const auto &raw_world_view = row_views[2];
            sql_command = fmt::format(
                "INSERT INTO ECOSYSTEM_MASTER (YEAR, AVG_WORLD, POPULATION_WORLD, RAW_WORLD) VALUES ({}, ZEROBLOB({}), ZEROBLOB({}), ZEROBLOB({}))",
                year, avg_world_view.size, population_view.size, raw_world_view.size);
        } else {
            sql_command = fmt::format(
                "INSERT INTO ECOSYSTEM_MASTER (YEAR, AVG_WORLD, POPULATION_WORLD) VALUES ({}, ZEROBLOB({}), ZEROBLOB({}))",
                year, avg_world_view.size, population_view.size);
        }

        int rc = sqlite3_exec(db, sql_command.c_str(), nullptr, 0, nullptr);
        if (rc != SQLITE_OK) {
            std::cerr << "Failed to execute insert statement: " << sqlite3_errmsg(db) << std::endl;
            // Consider how to handle this error; maybe throw an exception or return a status
            continue;
        }

        auto last_insert_row = sqlite3_last_insert_rowid(db);

        sqlite3_blob *blob = nullptr;

        // Write AVG_WORLD
        rc = sqlite3_blob_open(db, "main", "ECOSYSTEM_MASTER", "AVG_WORLD", last_insert_row, 1, &blob);
        if (rc != SQLITE_OK) {
            std::cerr << "Failed to open blob for AVG_WORLD: " << sqlite3_errmsg(db) << std::endl;
            continue;
        }
        sqlite3_blob_write(blob, avg_world_view.data, avg_world_view.size, 0);
        sqlite3_blob_close(blob);

        // Write POPULATION_WORLD
        rc = sqlite3_blob_open(db, "main", "ECOSYSTEM_MASTER", "POPULATION_WORLD", last_insert_row, 1, &blob);
        if (rc != SQLITE_OK) {
            std::cerr << "Failed to open blob for POPULATION_WORLD: " << sqlite3_errmsg(db) << std::endl;
            continue;
        }
        sqlite3_blob_write(blob, population_view.data, population_view.size, 0);
        sqlite3_blob_close(blob);

        // Write RAW_WORLD if provided
        if (row_views.size() >= 3) {
            const auto &raw_world_view = row_views[2];
            if (raw_world_view.size > 0) { // Only write if there's data
                rc = sqlite3_blob_open(db, "main", "ECOSYSTEM_MASTER", "RAW_WORLD", last_insert_row, 1, &blob);
                if (rc != SQLITE_OK) {
                    std::cerr << "Failed to open blob for RAW_WORLD: " << sqlite3_errmsg(db) << std::endl;
                    continue;
                }
                sqlite3_blob_write(blob, raw_world_view.data, raw_world_view.size, 0);
                sqlite3_blob_close(blob);
            }
        }
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

std::vector<ByteArray> DatabaseManager::read_row_by_year(int year) {
    std::vector<ByteArray> row_data_list; // Changed name for clarity
    sqlite3_stmt *stmt;
    // Select RAW_WORLD as the third column.
    // Older rows might have NULL for RAW_WORLD if the column was added later.
    std::string sql = "SELECT AVG_WORLD, POPULATION_WORLD, RAW_WORLD FROM ECOSYSTEM_MASTER WHERE YEAR = ?;";

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return row_data_list; // Return empty vector on error
    }

    sqlite3_bind_int(stmt, 1, year);

    int rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        row_data_list.resize(3); // Expecting 3 blobs now

        // Read AVG_WORLD blob (column 0)
        const void *avg_blob_ptr = sqlite3_column_blob(stmt, 0);
        int avg_blob_size = sqlite3_column_bytes(stmt, 0);
        if (avg_blob_ptr && avg_blob_size > 0) {
            row_data_list[0].resize(avg_blob_size);
            memcpy(row_data_list[0].data(), avg_blob_ptr, avg_blob_size);
        }

        // Read POPULATION_WORLD blob (column 1)
        const void *pop_blob_ptr = sqlite3_column_blob(stmt, 1);
        int pop_blob_size = sqlite3_column_bytes(stmt, 1);
        if (pop_blob_ptr && pop_blob_size > 0) {
            row_data_list[1].resize(pop_blob_size);
            memcpy(row_data_list[1].data(), pop_blob_ptr, pop_blob_size);
        }

        // Read RAW_WORLD blob (column 2)
        // Check for SQLITE_NULL before trying to read blob data, as older rows might not have this column filled.
        if (sqlite3_column_type(stmt, 2) != SQLITE_NULL) {
            const void *raw_blob_ptr = sqlite3_column_blob(stmt, 2);
            int raw_blob_size = sqlite3_column_bytes(stmt, 2);
            if (raw_blob_ptr && raw_blob_size > 0) {
                row_data_list[2].resize(raw_blob_size);
                memcpy(row_data_list[2].data(), raw_blob_ptr, raw_blob_size);
            }
        } else {
            // RAW_WORLD is NULL, row_data_list[2] will remain an empty ByteArray by default.
        }

    } else if (rc == SQLITE_DONE) {
        // No data found for the year
        std::cout << "No data found for year: " << year << std::endl;
        // Return an empty vector, which God::load_snapshot should handle.
    } else {
        std::cerr << "Failed to execute statement: " << sqlite3_errmsg(db) << std::endl;
        // Return an empty vector on error.
    }

    sqlite3_finalize(stmt);
    return row_data_list;
}

std::vector<int> DatabaseManager::get_available_years() {
    std::vector<int> years;
    sqlite3_stmt *stmt;
    std::string sql = "SELECT DISTINCT YEAR FROM ECOSYSTEM_MASTER ORDER BY YEAR;";

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return years; // Return empty vector on error
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        years.push_back(sqlite3_column_int(stmt, 0));
    }

    sqlite3_finalize(stmt);
    return years;
}
