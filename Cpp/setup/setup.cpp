#include <filesystem>
#include <iostream>
#include <fstream>
#include <sqlite3.h>
#include <string>

const std::filesystem::path master_table_path = "../../data/ecosystem_master.db";
const std::filesystem::path json_data_path = "../../data/json";

static void attach_missing_json_files()
{
    std::filesystem::path filepath;

    for (const auto &entry : std::filesystem::directory_iterator(json_data_path))
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
    }
}

static void create_master_table()
{
    sqlite3 *db;
    sqlite3_open(master_table_path.c_str(), &db);
    std::string command = "CREATE TABLE ECOSYSTEM_MASTER("  \
                          "NAME         TEXT        PRIMARY KEY     NOT NULL," \
                          "KIND         TEXT                        NOT NULL," \
                          "CHROMOSOME   TEXT                        NOT NULL," \
                          "GENERATION   INT                         NOT NULL," \
                          "IMMUNITY     FLOAT                       NOT NULL," \
                          "GENDER       INT                         NOT NULL," \
                          "AGE          INT                         NOT NULL," \
                          "HEIGHT       FLOAT                       NOT NULL," \
                          "WEIGHT       FLOAT                       NOT NULL," \
                          "FITNESS      FLOAT                       NOT NULL);";
    int rc = sqlite3_exec(db, command.c_str(), nullptr, 0, nullptr);
    if( rc != SQLITE_OK )
    {
        std::cout << "Master table was not created\n";
    }
    else
    {
        std::cout << "Master table created successfully\n";
    }
}

int main()
{
    if(!std::filesystem::exists(master_table_path))
    {
        create_master_table();
    }
    else
    {
        std::cout << "Using existing table at " << master_table_path << '\n';
    }
    if(std::filesystem::exists(json_data_path))
    {
        attach_missing_json_files();
    }
}
