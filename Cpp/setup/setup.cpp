#include <filesystem>
#include <iostream>
#include <fstream>
#include <sqlite3.h>
#include <string>

const std::filesystem::path master_db_path = "../../data/ecosystem_master.db";
const std::filesystem::path json_data_path = "../../data/json";

sqlite3 *db;

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

    // std::cout << "Creating " << table_name << " of type " << kingdom << '\n';

    std::string sql_command;

    if (kingdom == "animal")
    {
        sql_command = "CREATE TABLE " + table_name + "("
                      "YEAR             INT        PRIMARY KEY      NOT NULL,"
                      "MALE             FLOAT                       NOT NULL,"
                      "FEMALE           FLOAT                       NOT NULL,"
                      "M_MALE           FLOAT                       NOT NULL,"
                      "M_FEMALE         FLOAT                       NOT NULL,"
                      "C_PROB           FLOAT                       NOT NULL,"
                      "M_AGE_START      FLOAT                       NOT NULL,"
                      "M_AGE_END        FLOAT                       NOT NULL,"
                      "MX_AGE           FLOAT                       NOT NULL,"
                      "MT_PROB          FLOAT                       NOT NULL,"
                      "OF_FACTOR        FLOAT                       NOT NULL,"
                      "AGE_DTH          FLOAT                       NOT NULL,"
                      "FIT_DTH          FLOAT                       NOT NULL,"
                      "AFR_DTH          FLOAT                       NOT NULL,"
                      "HT_SP            FLOAT                       NOT NULL,"
                      "HT_ST            FLOAT                       NOT NULL,"
                      "HT_VT            FLOAT                       NOT NULL,"
                      "WT_SP            FLOAT                       NOT NULL,"
                      "WT_ST            FLOAT                       NOT NULL,"
                      "WT_VT            FLOAT                       NOT NULL,"
                      "VT_AP            FLOAT                       NOT NULL,"
                      "VT_SP            FLOAT                       NOT NULL,"
                      "ST_AP            FLOAT                       NOT NULL,"
                      "ST_SP            FLOAT                       NOT NULL,"
                      "TMB_AP           FLOAT                       NOT NULL,"
                      "TMB_HT           FLOAT                       NOT NULL,"
                      "TMB_SP           FLOAT                       NOT NULL,"
                      "TMB_ST           FLOAT                       NOT NULL,"
                      "TMB_VT           FLOAT                       NOT NULL,"
                      "TMB_WT           FLOAT                       NOT NULL,"
                      "TM_HT            FLOAT                       NOT NULL,"
                      "TM_SP            FLOAT                       NOT NULL,"
                      "TM_WT            FLOAT                       NOT NULL,"
                      "TMM_HT           FLOAT                       NOT NULL,"
                      "TMM_SP           FLOAT                       NOT NULL,"
                      "TMM_ST           FLOAT                       NOT NULL,"
                      "TMM_VT           FLOAT                       NOT NULL,"
                      "TMM_WT           FLOAT                       NOT NULL,"
                      "SL_FACTOR        FLOAT                       NOT NULL,"
                      "AVG_GEN          FLOAT                       NOT NULL,"
                      "AVG_IMM          FLOAT                       NOT NULL,"
                      "AVG_AGE          FLOAT                       NOT NULL,"
                      "AVG_HT           FLOAT                       NOT NULL,"
                      "AVG_WT           FLOAT                       NOT NULL,"
                      "AVGMA_AP         FLOAT                       NOT NULL,"
                      "AVGMA_SP         FLOAT                       NOT NULL,"
                      "AVGMA_ST         FLOAT                       NOT NULL,"
                      "AVGMA_VT         FLOAT                       NOT NULL,"
                      "AVG_SFIT         FLOAT                       NOT NULL,"
                      "AVG_DTHF         FLOAT                       NOT NULL,"
                      "AVG_VIS          FLOAT                       NOT NULL);";
    }
    else if (kingdom == "plant")
    {
        sql_command = "CREATE TABLE " + table_name + "("
                      "YEAR             INT        PRIMARY KEY      NOT NULL,"
                      "POP              FLOAT                       NOT NULL,"
                      "M_POP            FLOAT                       NOT NULL,"
                      "C_PROB           FLOAT                       NOT NULL,"
                      "M_AGE_START      FLOAT                       NOT NULL,"
                      "M_AGE_END        FLOAT                       NOT NULL,"
                      "MX_AGE           FLOAT                       NOT NULL,"
                      "MT_PROB          FLOAT                       NOT NULL,"
                      "OF_FACTOR        FLOAT                       NOT NULL,"
                      "AGE_DTH          FLOAT                       NOT NULL,"
                      "FIT_DTH          FLOAT                       NOT NULL,"
                      "AFR_DTH          FLOAT                       NOT NULL,"
                      "HT_VT            FLOAT                       NOT NULL,"
                      "WT_VT            FLOAT                       NOT NULL,"
                      "TMB_HT           FLOAT                       NOT NULL,"
                      "TMB_VT           FLOAT                       NOT NULL,"
                      "TMB_WT           FLOAT                       NOT NULL,"
                      "TM_HT            FLOAT                       NOT NULL,"
                      "TM_WT            FLOAT                       NOT NULL,"
                      "TMM_HT           FLOAT                       NOT NULL,"
                      "TMM_VT           FLOAT                       NOT NULL,"
                      "TMM_WT           FLOAT                       NOT NULL,"
                      "AVG_GEN          FLOAT                       NOT NULL,"
                      "AVG_IMM          FLOAT                       NOT NULL,"
                      "AVG_AGE          FLOAT                       NOT NULL,"
                      "AVG_HT           FLOAT                       NOT NULL,"
                      "AVG_WT           FLOAT                       NOT NULL,"
                      "AVGMA_VT         FLOAT                       NOT NULL,"
                      "AVG_SFIT         FLOAT                       NOT NULL,"
                      "AVG_DTHF         FLOAT                       NOT NULL);";
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

        // std::cout << entry.path() << '\n';
        create_species_table(entry.path());
    }
}

int main()
{

    if(!std::filesystem::exists(master_db_path))
    {
        sqlite3_open(master_db_path.c_str(), &db);
        create_master_table();
    }
    else
    {
        sqlite3_open(master_db_path.c_str(), &db);
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
