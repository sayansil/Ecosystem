#include <fmt/core.h>
#include <setup.hpp>

static const std::vector<std::pair<std::string, std::string>>
    schemaMaster{{"YEAR", "NUMBER"},
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

static void create_master_table(sqlite3 * db) {
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

static void parse_species_directories(std::string subdirectory,
        const std::filesystem::path& json_data_path,
        const std::filesystem::path& json_template_path) {
    std::filesystem::path filepath;

    auto copy_contents = [](const std::filesystem::path &source,
                            const std::filesystem::path &sink) {
        if (!std::filesystem::exists(sink)) {
            std::ifstream to_read(source, std::ios::binary);
            std::ofstream to_create(sink, std::ios::binary);

            to_create << to_read.rdbuf();

            to_read.close();
            to_create.close();
        }
    };

    for (const auto &entry :
         std::filesystem::directory_iterator(json_data_path / subdirectory)) {
        // Creating base.json from template
        copy_contents(json_template_path / subdirectory / "base.json",
                      entry.path() / "base.json");

        // Creating modify.json from template
        copy_contents(json_template_path / subdirectory / "modify.json",
                      entry.path() / "modify.json");
    }
}

namespace setup {
    std::filesystem::path setup() {

    std::filesystem::path ecosystem_root = helper::get_ecosystem_root();
    sqlite3 *db;

    const std::filesystem::path master_db_path =
        ecosystem_root / "data/ecosystem_master.db";
    const std::filesystem::path json_data_path =
        ecosystem_root / "data/json";
    const std::filesystem::path json_template_path =
        ecosystem_root / "data/templates/json";

    if (!std::filesystem::exists(master_db_path)) {
        sqlite3_open(master_db_path.string().c_str(), &db);
        create_master_table(db);
    } else {
        sqlite3_open(master_db_path.string().c_str(), &db);
        fmt::print("Using existing db at {}\n\n", master_db_path.string());
    }

    if (std::filesystem::exists(json_data_path / "animal")) {
        parse_species_directories("animal", json_data_path, json_template_path);
    }
    if (std::filesystem::exists(json_data_path / "plant")) {
        parse_species_directories("plant", json_data_path, json_template_path);
    }
    sqlite3_close(db);
    return ecosystem_root;
}
};  // namespace setup
