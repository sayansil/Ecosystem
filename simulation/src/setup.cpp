#include <fmt/core.h>

#include <setup.hpp>

static const std::vector<std::pair<std::string, std::string>> schemaMaster{
    {"YEAR", "NUMBER"},
    {"AVG_WORLD", "LONGBLOB"},
    {"POPULATION_WORLD", "LONGBLOB"}};

static std::string sql_command_creator(
    const std::string &tableName,
    const std::vector<std::pair<std::string, std::string>> &schema) {
    std::string sql_command = "create table " + tableName + "(";

    bool primaryKey = true;
    for (const auto &[colName, colType] : schema) {
        sql_command += colName + " " + colType + " ";

        if (primaryKey) {
            sql_command += "PRIMARY KEY ";
            primaryKey = false;
        }

        sql_command += "NOT NULL,";
    }

    sql_command = sql_command.substr(0, sql_command.length() - 1);
    sql_command += ");";

    return sql_command;
}

static void create_master_table(sqlite3 *db) {
    int rc;

    std::string sql_command =
        sql_command_creator("ECOSYSTEM_MASTER", schemaMaster);

    rc = sqlite3_exec(db, sql_command.c_str(), nullptr, 0, nullptr);
    if (rc != SQLITE_OK) {
        fmt::print("Master table was not created\n\n");
    } else {
        fmt::print("Master table created successfully\n\n");
    }
}

namespace setup {
std::filesystem::path setup(std::filesystem::path ecosystem_root) {
    sqlite3 *db;

    const std::filesystem::path master_db_path =
        ecosystem_root / "data" / "ecosystem_master.db";

    if (!std::filesystem::exists(master_db_path)) {
        sqlite3_open(master_db_path.string().c_str(), &db);
        create_master_table(db);
    } else {
        sqlite3_open(master_db_path.string().c_str(), &db);
        fmt::print("Using existing db at {}\n\n", master_db_path.string());
    }

    sqlite3_close(db);
    return ecosystem_root;
}
};  // namespace setup
