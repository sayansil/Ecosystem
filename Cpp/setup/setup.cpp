#include <filesystem>
#include <iostream>
#include <fstream>
#include <sqlite3.h>
#include <string>

const std::filesystem::path master_db_path = "../../data/ecosystem_master.db";
const std::filesystem::path json_data_path = "../../data/json";

sqlite3 *db;

static void parse_species_directories(std::string subdirectory)
{
    std::filesystem::path filepath;

    for (const auto &entry : std::filesystem::directory_iterator(json_data_path / subdirectory))
    {
        filepath = "base.json";
        if (!std::filesystem::exists(entry.path() / filepath))
        {
            std::ofstream to_create(entry.path() / filepath);
            to_create << "{}";
            to_create.close();
        }

        filepath = "current.json";
        if (!std::filesystem::exists(entry.path() / filepath))
        {
            std::ofstream to_create(entry.path() / filepath);

            std::ifstream to_copy(entry.path() / "base.json");
            std::string tmp((std::istreambuf_iterator<char>(to_copy)),
                            std::istreambuf_iterator<char>());
            to_create << tmp;

            to_copy.close();
            to_create.close();
        }

        filepath = "modify.json";
        if (!std::filesystem::exists(entry.path() / filepath))
        {
            std::ofstream to_create(entry.path() / filepath);
            to_create << "{}";
            to_create.close();
        }

        std::cout << entry << '\n';
    }
}

static void create_master_table()
{
    std::string sql_command;
    int rc;

    sql_command = "CREATE TABLE ECOSYSTEM_MASTER("
                  "NAME         TEXT        PRIMARY KEY     NOT NULL,"
                  "KIND         TEXT                        NOT NULL,"
                  "CHROMOSOME   TEXT                        NOT NULL,"
                  "GENERATION   INT                         NOT NULL,"
                  "IMMUNITY     FLOAT                       NOT NULL,"
                  "GENDER       INT                         NOT NULL,"
                  "AGE          INT                         NOT NULL,"
                  "HEIGHT       FLOAT                       NOT NULL,"
                  "WEIGHT       FLOAT                       NOT NULL,"
                  "FITNESS      FLOAT                       NOT NULL);";
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

static void create_table(const std::string &path)
{
}

int main()
{
    sqlite3_open(master_db_path.c_str(), &db);

    if(!std::filesystem::exists(master_db_path))
    {
        create_master_table();
    }
    else
    {
        std::cout << "Using existing db at " << master_db_path << '\n';
    }

    if (std::filesystem::exists(json_data_path / "animal"))
    {
        parse_species_directories("animal");
    }
    if (std::filesystem::exists(json_data_path / "plant"))
    {
        parse_species_directories("plant");
    }
}
