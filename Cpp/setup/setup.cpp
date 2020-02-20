#include <filesystem>
#include <iostream>
#include <sqlite3.h>
#include <string>

const std::filesystem::path master_table_path = "../../data/ecosystem_master.db";

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
}
