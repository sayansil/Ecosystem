#include <filesystem>
#include <iostream>
#include <fstream>
#include <sqlite3.h>
#include <string>
#include <helper.hpp>
#include <schema.hpp>

const std::experimental::filesystem::path master_db_path = helper::get_ecosystem_root() / "data/ecosystem_master.db";
const std::experimental::filesystem::path json_data_path = helper::get_ecosystem_root() / "data/json";
const std::experimental::filesystem::path json_template_path = helper::get_ecosystem_root() / "data/templates/json";

sqlite3 *db;

static std::string sql_command_creator(const std::string &tableName, const std::vector<std::pair<std::string, SQLType>> &schema)
{
    std::string sql_command = "create table " + tableName + "(";

    bool primaryKey = true;
    for (const auto &[colName, colType] : schema)
    {
        sql_command += colName + " ";
        if (colType == SQLType::TEXT)
            sql_command += "TEXT ";
        else if (colType == SQLType::FLOAT)
            sql_command += "FLOAT ";
        else if (colType == SQLType::INT)
            sql_command += "INT ";

        if (primaryKey)
        {
            sql_command += "PRIMARY KEY ";
            primaryKey = false;
        }

        sql_command += "NOT NULL,";
    }

    sql_command = sql_command.substr(0, sql_command.length() - 1);
    sql_command += ");";

    return sql_command;
}

static void create_master_table()
{
    int rc;

    std::string sql_command = sql_command_creator("ECOSYSTEM_MASTER", schema::schemaMaster);

    rc = sqlite3_exec(db, sql_command.c_str(), nullptr, 0, nullptr);
    if (rc != SQLITE_OK)
    {
        std::cout << "Master table was not created\n";
    }
    else
    {
        std::cout << "Master table created successfully\n";
    }
}

static void create_species_table(const std::string &path)
{
    std::string kind, kingdom;
    kind = path.substr(path.rfind('/') + 1);
    kingdom = path.substr(0, path.rfind('/'));
    if (kingdom.rfind('/') != std::string::npos)
    {
        kingdom = kingdom.substr(kingdom.rfind('/') + 1);
    }

    std::string table_name = "STATS_" + kind;
    for (auto & c: table_name) c = toupper(c);

    std::string sql_command;

    if (kingdom == "animal")
    {
        sql_command = sql_command_creator(table_name, schema::schemaAnimal);
    }
    else if (kingdom == "plant")
    {
        sql_command = sql_command_creator(table_name, schema::schemaPlant);
    }

    int rc = sqlite3_exec(db, sql_command.c_str(), nullptr, 0, nullptr);
    if (rc != SQLITE_OK)
    {
        std::cout << "New " << table_name << " table cannot be created\n";
    }
    else
    {
        std::cout << table_name << " table created successfully\n";
    }
}

static void parse_species_directories(std::string subdirectory)
{
    std::experimental::filesystem::path filepath;

    auto copy_contents = [](const std::experimental::filesystem::path &source, const std::experimental::filesystem::path &sink) {
        if (!std::experimental::filesystem::exists(sink))
        {
            std::ifstream to_read(source, std::ios::binary);
            std::ofstream to_create(sink, std::ios::binary);

            to_create << to_read.rdbuf();

            to_read.close();
            to_create.close();
        }
    };

    for (const auto &entry : std::experimental::filesystem::directory_iterator(json_data_path / subdirectory))
    {
        // Creating base.json from template
        copy_contents(
            json_template_path / subdirectory / "base.json",
            entry.path() / "base.json");

        // Creating modify.json from template
        copy_contents(
            json_template_path / subdirectory / "modify.json",
            entry.path() / "modify.json");

        // Creating current.json from base.json
        copy_contents(
            entry.path() / "base.json",
            entry.path() / "current.json");

        create_species_table(entry.path());
    }
}

int main()
{

    if(!std::experimental::filesystem::exists(master_db_path))
    {
        sqlite3_open(master_db_path.c_str(), &db);
        create_master_table();
    }
    else
    {
        sqlite3_open(master_db_path.c_str(), &db);
        std::cout << "Using existing db at " << master_db_path << '\n';
    }

    if (std::experimental::filesystem::exists(json_data_path / "animal"))
    {
        parse_species_directories("animal");
    }
    if (std::experimental::filesystem::exists(json_data_path / "plant"))
    {
        parse_species_directories("plant");
    }
}
