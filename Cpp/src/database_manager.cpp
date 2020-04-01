#include <database_manager.hpp>
#include <iostream>
#include <string>

struct RowTypeMaster
{
    std::string NAME;
    std::string KIND;
    std::string CHROMOSOME;
    unsigned int GENERATION;
    double IMMUNITY;
    unsigned int GENDER;
    unsigned int AGE;
    double HEIGHT;
    double WEIGHT;
    double FITNESS;
};

struct RowTypeAnimal
{
    std::string NAME;
    std::string KIND;
    std::string CHROMOSOME;
    unsigned int GENERATION;
    double IMMUNITY;
    unsigned int GENDER;
    unsigned int AGE;
    double HEIGHT;
    double WEIGHT;
    double FITNESS;
};

struct RowTypePlant
{
    std::string NAME;
    std::string KIND;
    std::string CHROMOSOME;
    unsigned int GENERATION;
    double IMMUNITY;
    unsigned int GENDER;
    unsigned int AGE;
    double HEIGHT;
    double WEIGHT;
    double FITNESS;
};

static std::vector<RowTypeMaster> gRowsMaster;
static std::vector<RowTypeAnimal> gRowsAnimal;
static std::vector<RowTypePlant> gRowsPlant;

static std::vector<std::string> items;

static int callback_read_master(void *data, int argc, char **argv, char **colName)
{
    RowTypeMaster gRow;
    gRow.NAME = argv[0];
    gRow.KIND = argv[1];
    gRow.CHROMOSOME = argv[2];
    gRow.GENERATION = (unsigned int)std::stoi(argv[3]);
    gRow.IMMUNITY = std::stod(argv[4]);
    gRow.GENDER = (unsigned int)std::stoi(argv[5]);
    gRow.AGE = (unsigned int)std::stoi(argv[6]);
    gRow.HEIGHT = std::stod(argv[7]);
    gRow.WEIGHT = std::stod(argv[8]);
    gRow.FITNESS = std::stod(argv[9]);
    gRowsMaster.push_back(gRow);

    return 0;
}

static int callback_read_animal(void *data, int argc, char **argv, char **colName)
{
    RowTypeMaster gRow;
    gRow.NAME = argv[0];
    gRow.KIND = argv[1];
    gRow.CHROMOSOME = argv[2];
    gRow.GENERATION = (unsigned int)std::stoi(argv[3]);
    gRow.IMMUNITY = std::stod(argv[4]);
    gRow.GENDER = (unsigned int)std::stoi(argv[5]);
    gRow.AGE = (unsigned int)std::stoi(argv[6]);
    gRow.HEIGHT = std::stod(argv[7]);
    gRow.WEIGHT = std::stod(argv[8]);
    gRow.FITNESS = std::stod(argv[9]);
    gRowsMaster.push_back(gRow);

    return 0;
}

static int callback_read_plant(void *data, int argc, char **argv, char **colName)
{
    RowTypeMaster gRow;
    gRow.NAME = argv[0];
    gRow.KIND = argv[1];
    gRow.CHROMOSOME = argv[2];
    gRow.GENERATION = (unsigned int)std::stoi(argv[3]);
    gRow.IMMUNITY = std::stod(argv[4]);
    gRow.GENDER = (unsigned int)std::stoi(argv[5]);
    gRow.AGE = (unsigned int)std::stoi(argv[6]);
    gRow.HEIGHT = std::stod(argv[7]);
    gRow.WEIGHT = std::stod(argv[8]);
    gRow.FITNESS = std::stod(argv[9]);
    gRowsMaster.push_back(gRow);

    return 0;
}

static int callback_group(void *data, int argc, char **argv, char **colName)
{
    std::string item = argv[0];
    items.push_back(item);

    return 0;
}

DatabaseManager::DatabaseManager(const std::filesystem::path& path)
{
    db_path = std::filesystem::canonical(path);
    sqlite3_open(db_path.c_str(), &db);
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

void DatabaseManager::insert_rows(const std::vector<std::vector<STAT>> &rows)
{
    
    for(const auto &row : rows)
    {
        std::string values = "";
        for(const auto &value : row)
        {
            if(value.data.index() == 0)
            {
                values += std::to_string(static_cast<unsigned int>(value)) + ",";
            }
            if(value.data.index() == 1)
            {
                values += std::to_string(static_cast<double>(value)) + ",";
            }
            if(value.data.index() == 2)
            {
                values += "\'" + static_cast<std::string>(value) + "\',";
            }
            if(value.data.index() == 3)
            {
                bool tmp = static_cast<bool>(value);
                if(tmp)
                    values += "\'true\',";
                else
                    values += "\'false\',";
            }

        }
        values = values.substr(0, values.length() - 1);
        std::string sql_command = "INSERT INTO ECOSYSTEM_MASTER (NAME,KIND,CHROMOSOME,GENERATION,IMMUNITY,GENDER,AGE,HEIGHT,WEIGHT,FITNESS) "
                                  "VALUES (" + values + ")";
        sqlite3_exec(db, sql_command.c_str(), nullptr, 0, nullptr);
    }
}

void DatabaseManager::insert_stat_row(const std::vector<STAT> &row, const std::string &kind, const std::string &kingdom)
{
    std::string table_name = "STATS_" + kind;
    for (auto & c: table_name) c = toupper(c);

    std::string values = "";
    for (const auto &value : row)
    {
        if (value.data.index() == 0)
        {
            values += std::to_string(static_cast<unsigned int>(value)) + ",";
        }
        if (value.data.index() == 1)
        {
            values += std::to_string(static_cast<double>(value)) + ",";
        }
        if (value.data.index() == 2)
        {
            values += "\'" + static_cast<std::string>(value) + "\',";
        }
        if (value.data.index() == 3)
        {
            bool tmp = static_cast<bool>(value);
            if (tmp)
                values += "\'true\',";
            else
                values += "\'false\',";
        }
    }
    values = values.substr(0, values.length() - 1);

    std::string sql_command;
    if (kingdom == "animal")
    {
        sql_command = "INSERT INTO " + table_name + " (YEAR,MALE,FEMALE,M_MALE,M_FEMALE,C_PROB,M_AGE_START,M_AGE_END,MX_AGE,MT_PROB,OF_FACTOR,AGE_DTH,FIT_DTH,AFR_DTH,HT_SP,HT_ST,HT_VT,WT_SP,WT_ST,WT_VT,VT_AP,VT_SP,ST_AP,ST_SP,TMB_AP,TMB_HT,TMB_SP,TMB_ST,TMB_VT,TMB_WT,TM_HT,TM_SP,TM_WT,TMM_HT,TMM_SP,TMM_ST,TMM_VT,TMM_WT,SL_FACTOR,AVG_GEN,AVG_IMM,AVG_AGE,AVG_HT,AVG_WT,AVGMA_AP,AVGMA_SP,AVGMA_ST,AVGMA_VT,AVG_SFIT,AVG_DTHF,AVG_VIS) "
                      "VALUES (" + values + ");";
    }
    else if (kingdom == "plant")
    {
        sql_command = "INSERT INTO " + table_name + " (YEAR,POP,M_POP,C_PROB,M_AGE_START,M_AGE_END,MX_AGE,MT_PROB,OF_FACTOR,AGE_DTH,FIT_DTH,AFR_DTH,HT_VT,WT_VT,TMB_HT,TMB_VT,TMB_WT,TM_HT,TM_WT,TMM_HT,TMM_VT,TMM_WT,AVG_GEN,AVG_IMM,AVG_AGE,AVG_HT,AVG_WT,AVGMA_VT,AVG_SFIT,AVG_DTHF) "
                      "VALUES (" + values + ");";
    }
    sqlite3_exec(db, sql_command.c_str(), nullptr, 0, nullptr);
}

void DatabaseManager::delete_rows(const std::vector<std::string> &names)
{
    std::string values = "";
    for(const auto &name : names)
        values += "\'" + name + "\',";
    values = values.substr(0, values.length() - 1);
    std::string sql_command = "DELETE FROM ECOSYSTEM_MASTER "\
                              "WHERE NAME IN (" + values + ");";
    sqlite3_exec(db, sql_command.c_str(), nullptr, 0, nullptr);
}

std::vector<std::vector<STAT>> DatabaseManager::read_rows_master(const std::string &colName, const std::vector<std::string> &names)
{
    gRowsMaster.clear(); gRowsMaster.shrink_to_fit();
    std::string values = "";
    for(const auto &name : names)
        values += "\'" + name + "\',";
    values = values.substr(0, values.length() - 1);
    std::string sql_command = "SELECT * FROM ECOSYSTEM_MASTER "\
                              "WHERE " + colName +  " IN (" + values + ");";

    sqlite3_exec(db, sql_command.c_str(), callback_read_master, 0, nullptr);
    std::vector<std::vector<STAT>> rows;
    for(const auto &gRow : gRowsMaster)
    {
        std::vector<STAT> row;
        row.push_back(gRow.NAME);
        row.push_back(gRow.KIND);
        row.push_back(gRow.CHROMOSOME);
        row.push_back(gRow.GENERATION);
        row.push_back(gRow.IMMUNITY);
        row.push_back(gRow.GENDER);
        row.push_back(gRow.AGE);
        row.push_back(gRow.HEIGHT);
        row.push_back(gRow.WEIGHT);
        row.push_back(gRow.FITNESS);
        rows.push_back(row);
    }
    return rows;
}

std::vector<std::vector<STAT>> DatabaseManager::read_all_rows_master()
{
    gRowsMaster.clear(); gRowsMaster.shrink_to_fit();
    std::string sql_command = "SELECT * FROM ECOSYSTEM_MASTER";

    sqlite3_exec(db, sql_command.c_str(), callback_read_master, 0, nullptr);
    std::vector<std::vector<STAT>> rows;
    for(const auto &gRow : gRowsMaster)
    {
        std::vector<STAT> row;
        row.push_back(gRow.NAME);
        row.push_back(gRow.KIND);
        row.push_back(gRow.CHROMOSOME);
        row.push_back(gRow.GENERATION);
        row.push_back(gRow.IMMUNITY);
        row.push_back(gRow.GENDER);
        row.push_back(gRow.AGE);
        row.push_back(gRow.HEIGHT);
        row.push_back(gRow.WEIGHT);
        row.push_back(gRow.FITNESS);
        rows.push_back(row);
    }
    return rows;
}

std::vector<std::vector<STAT>> DatabaseManager::read_all_rows_stats(const std::string &full_species_name)
{
    std::string kind = full_species_name.substr(full_species_name.find('/') + 1);
    std::string kingdom = full_species_name.substr(0, full_species_name.find('/'));
    std::vector<std::vector<STAT>> rows;
    std::string table_name = "STATS_" + kind;
    
    for (auto & c: table_name)
    {
        c = toupper(c);
    }
    if(kingdom == "animal")
    {    
        gRowsAnimal.clear();
        gRowsAnimal.shrink_to_fit();
        std::string sql_command = "SELECT * FROM " + table_name;
        sqlite3_exec(db, sql_command.c_str(), callback_read_animal, 0, nullptr);
        for (const auto &gRow : gRowsAnimal)
        {
            std::vector<STAT> row;
            row.push_back(gRow.NAME);
            row.push_back(gRow.KIND);
            row.push_back(gRow.CHROMOSOME);
            row.push_back(gRow.GENERATION);
            row.push_back(gRow.IMMUNITY);
            row.push_back(gRow.GENDER);
            row.push_back(gRow.AGE);
            row.push_back(gRow.HEIGHT);
            row.push_back(gRow.WEIGHT);
            row.push_back(gRow.FITNESS);
            rows.push_back(row);
        }
    }
    else if(kingdom == "plant")
    {    
        gRowsPlant.clear();
        gRowsPlant.shrink_to_fit();
        std::string sql_command = "SELECT * FROM " + table_name;
        sqlite3_exec(db, sql_command.c_str(), callback_read_plant, 0, nullptr);
        for (const auto &gRow : gRowsPlant)
        {
            std::vector<STAT> row;
            row.push_back(gRow.NAME);
            row.push_back(gRow.KIND);
            row.push_back(gRow.CHROMOSOME);
            row.push_back(gRow.GENERATION);
            row.push_back(gRow.IMMUNITY);
            row.push_back(gRow.GENDER);
            row.push_back(gRow.AGE);
            row.push_back(gRow.HEIGHT);
            row.push_back(gRow.WEIGHT);
            row.push_back(gRow.FITNESS);
            rows.push_back(row);
        }
    }
    return rows;
}

void DatabaseManager::update_rows(const std::vector<std::vector<STAT>> &rows)
{
    for(const auto &row : rows)
    {
        std::string values = "";
        values += "KIND = \'" + static_cast<std::string>(row[1]) + "\',";
        values += "CHROMOSOME = \'" + static_cast<std::string>(row[2]) + "\',";
        values += "GENERATION = " + std::to_string(static_cast<unsigned int>(row[3])) + ",";
        values += "IMMUNITY = " + std::to_string(static_cast<double>(row[4])) + ",";
        values += "GENDER = " + std::to_string(static_cast<unsigned int>(row[5])) + ",";
        values += "AGE = " + std::to_string(static_cast<unsigned int>(row[6])) + ",";
        values += "HEIGHT = " + std::to_string(static_cast<double>(row[7])) + ",";
        values += "WEIGHT = " + std::to_string(static_cast<double>(row[8])) + ",";
        values += "FITNESS = " + std::to_string(static_cast<double>(row[9]));
        std::string sql_command = "UPDATE ECOSYSTEM_MASTER SET " + values + \
                                  " WHERE NAME = \'" + static_cast<std::string>(row[0]) + "\';";
        sqlite3_exec(db, sql_command.c_str(), nullptr, 0, nullptr);
    }
}

void DatabaseManager::clear_database()
{
    std::string sql_command = "DELETE FROM ECOSYSTEM_MASTER;";
    sqlite3_exec(db, sql_command.c_str(), nullptr, 0, nullptr);
}

void DatabaseManager::clear_table(const std::string &kind)
{
    std::string table_name = "STATS_" + kind;
    for (auto & c: table_name) c = toupper(c);

    std::string sql_command = "DELETE FROM " + table_name + ";";
    sqlite3_exec(db, sql_command.c_str(), nullptr, 0, nullptr);
}

std::unordered_map<std::string, std::vector<std::vector<STAT>>> DatabaseManager::groupby_kind()
{
    items.clear(); items.shrink_to_fit();
    std::string sql_command = "SELECT DISTINCT KIND FROM ECOSYSTEM_MASTER;";
    sqlite3_exec(db, sql_command.c_str(), callback_group, 0, nullptr);
    std::unordered_map<std::string, std::vector<std::vector<STAT>>> rowMap;
    for(const auto &item : items)
    {
        auto rows = read_rows_master("KIND", {item});
        rowMap[item] = rows;
    }
    return rowMap;
}
