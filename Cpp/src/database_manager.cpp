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
    unsigned int YEAR;
    double MALE;
    double FEMALE;
    double M_MALE;
    double M_FEMALE;
    double C_PROB;
    double M_AGE_START;
    double M_AGE_END;
    double MX_AGE;
    double MT_PROB;
    double OF_FACTOR;
    double AGE_DTH;
    double FIT_DTH;
    double AFR_DTH;
    double HT_SP;
    double HT_ST;
    double HT_VT;
    double WT_SP;
    double WT_ST;
    double WT_VT;
    double VT_AP;
    double VT_SP;
    double ST_AP;
    double ST_SP;
    double TMB_AP;
    double TMB_HT;
    double TMB_SP;
    double TMB_ST;
    double TMB_VT;
    double TMB_WT;
    double TM_HT;
    double TM_SP;
    double TM_WT;
    double TMM_HT;
    double TMM_SP;
    double TMM_ST;
    double TMM_VT;
    double TMM_WT;
    double SL_FACTOR;
    double AVG_GEN;
    double AVG_IMM;
    double AVG_AGE;
    double AVG_HT;
    double AVG_WT;
    double AVGMA_AP;
    double AVGMA_SP;
    double AVGMA_ST;
    double AVGMA_VT;
    double AVG_SFIT;
    double AVG_DTHF;
    double AVG_VIS;
};

struct RowTypePlant
{
    unsigned int YEAR;
    double POP;
    double M_POP;
    double C_PROB;
    double M_AGE_START;
    double M_AGE_END;
    double MX_AGE;
    double MT_PROB;
    double OF_FACTOR;
    double AGE_DTH;
    double FIT_DTH;
    double AFR_DTH;
    double HT_VT;
    double WT_VT;
    double TMB_HT;
    double TMB_VT;
    double TMB_WT;
    double TM_HT;
    double TM_WT;
    double TMM_HT;
    double TMM_VT;
    double TMM_WT;
    double AVG_GEN;
    double AVG_IMM;
    double AVG_AGE;
    double AVG_HT;
    double AVG_WT;
    double AVGMA_VT;
    double AVG_SFIT;
    double AVG_DTHF;
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
    RowTypeAnimal gRow;

    gRow.YEAR = (unsigned int)std::stoi(argv[0]);
    gRow.MALE = std::stod(argv[1]);
    gRow.FEMALE = std::stod(argv[2]);
    gRow.M_MALE = std::stod(argv[3]);
    gRow.M_FEMALE = std::stod(argv[4]);
    gRow.C_PROB = std::stod(argv[5]);
    gRow.M_AGE_START = std::stod(argv[6]);
    gRow.M_AGE_END = std::stod(argv[7]);
    gRow.MX_AGE = std::stod(argv[8]);
    gRow.MT_PROB = std::stod(argv[9]);
    gRow.OF_FACTOR = std::stod(argv[10]);
    gRow.AGE_DTH = std::stod(argv[11]);
    gRow.FIT_DTH = std::stod(argv[12]);
    gRow.AFR_DTH = std::stod(argv[13]);
    gRow.HT_SP = std::stod(argv[14]);
    gRow.HT_ST = std::stod(argv[15]);
    gRow.HT_VT = std::stod(argv[16]);
    gRow.WT_SP = std::stod(argv[17]);
    gRow.WT_ST = std::stod(argv[18]);
    gRow.WT_VT = std::stod(argv[19]);
    gRow.VT_AP = std::stod(argv[20]);
    gRow.VT_SP = std::stod(argv[21]);
    gRow.ST_AP = std::stod(argv[22]);
    gRow.ST_SP = std::stod(argv[23]);
    gRow.TMB_AP = std::stod(argv[24]);
    gRow.TMB_HT = std::stod(argv[25]);
    gRow.TMB_SP = std::stod(argv[26]);
    gRow.TMB_ST = std::stod(argv[27]);
    gRow.TMB_VT = std::stod(argv[28]);
    gRow.TMB_WT = std::stod(argv[29]);
    gRow.TM_HT = std::stod(argv[30]);
    gRow.TM_SP = std::stod(argv[31]);
    gRow.TM_WT = std::stod(argv[32]);
    gRow.TMM_HT = std::stod(argv[33]);
    gRow.TMM_SP = std::stod(argv[34]);
    gRow.TMM_ST = std::stod(argv[35]);
    gRow.TMM_VT = std::stod(argv[36]);
    gRow.TMM_WT = std::stod(argv[37]);
    gRow.SL_FACTOR = std::stod(argv[38]);
    gRow.AVG_GEN = std::stod(argv[39]);
    gRow.AVG_IMM = std::stod(argv[40]);
    gRow.AVG_AGE = std::stod(argv[41]);
    gRow.AVG_HT = std::stod(argv[42]);
    gRow.AVG_WT = std::stod(argv[43]);
    gRow.AVGMA_AP = std::stod(argv[44]);
    gRow.AVGMA_SP = std::stod(argv[45]);
    gRow.AVGMA_ST = std::stod(argv[46]);
    gRow.AVGMA_VT = std::stod(argv[47]);
    gRow.AVG_SFIT = std::stod(argv[48]);
    gRow.AVG_DTHF = std::stod(argv[49]);
    gRow.AVG_VIS = std::stod(argv[50]);

    gRowsAnimal.push_back(gRow);

    return 0;
}

static int callback_read_plant(void *data, int argc, char **argv, char **colName)
{
    RowTypePlant gRow;

    gRow.YEAR = (unsigned int)std::stoi(argv[0]);
    gRow.POP = std::stod(argv[1]);
    gRow.M_POP = std::stod(argv[2]);
    gRow.C_PROB = std::stod(argv[3]);
    gRow.M_AGE_START = std::stod(argv[4]);
    gRow.M_AGE_END = std::stod(argv[5]);
    gRow.MX_AGE = std::stod(argv[6]);
    gRow.MT_PROB = std::stod(argv[7]);
    gRow.OF_FACTOR = std::stod(argv[8]);
    gRow.AGE_DTH = std::stod(argv[9]);
    gRow.FIT_DTH = std::stod(argv[10]);
    gRow.AFR_DTH = std::stod(argv[11]);
    gRow.HT_VT = std::stod(argv[12]);
    gRow.WT_VT = std::stod(argv[13]);
    gRow.TMB_HT = std::stod(argv[14]);
    gRow.TMB_VT = std::stod(argv[15]);
    gRow.TMB_WT = std::stod(argv[16]);
    gRow.TM_HT = std::stod(argv[17]);
    gRow.TM_WT = std::stod(argv[18]);
    gRow.TMM_HT = std::stod(argv[19]);
    gRow.TMM_VT = std::stod(argv[20]);
    gRow.TMM_WT = std::stod(argv[21]);
    gRow.AVG_GEN = std::stod(argv[22]);
    gRow.AVG_IMM = std::stod(argv[23]);
    gRow.AVG_AGE = std::stod(argv[24]);
    gRow.AVG_HT = std::stod(argv[25]);
    gRow.AVG_WT = std::stod(argv[26]);
    gRow.AVGMA_VT = std::stod(argv[27]);
    gRow.AVG_SFIT = std::stod(argv[28]);
    gRow.AVG_DTHF = std::stod(argv[29]);

    gRowsPlant.push_back(gRow);

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
            row.push_back(gRow.YEAR);
            row.push_back(gRow.MALE);
            row.push_back(gRow.FEMALE);
            row.push_back(gRow.M_MALE);
            row.push_back(gRow.M_FEMALE);
            row.push_back(gRow.C_PROB);
            row.push_back(gRow.M_AGE_START);
            row.push_back(gRow.M_AGE_END);
            row.push_back(gRow.MX_AGE);
            row.push_back(gRow.MT_PROB);
            row.push_back(gRow.OF_FACTOR);
            row.push_back(gRow.AGE_DTH);
            row.push_back(gRow.FIT_DTH);
            row.push_back(gRow.AFR_DTH);
            row.push_back(gRow.HT_SP);
            row.push_back(gRow.HT_ST);
            row.push_back(gRow.HT_VT);
            row.push_back(gRow.WT_SP);
            row.push_back(gRow.WT_ST);
            row.push_back(gRow.WT_VT);
            row.push_back(gRow.VT_AP);
            row.push_back(gRow.VT_SP);
            row.push_back(gRow.ST_AP);
            row.push_back(gRow.ST_SP);
            row.push_back(gRow.TMB_AP);
            row.push_back(gRow.TMB_HT);
            row.push_back(gRow.TMB_SP);
            row.push_back(gRow.TMB_ST);
            row.push_back(gRow.TMB_VT);
            row.push_back(gRow.TMB_WT);
            row.push_back(gRow.TM_HT);
            row.push_back(gRow.TM_SP);
            row.push_back(gRow.TM_WT);
            row.push_back(gRow.TMM_HT);
            row.push_back(gRow.TMM_SP);
            row.push_back(gRow.TMM_ST);
            row.push_back(gRow.TMM_VT);
            row.push_back(gRow.TMM_WT);
            row.push_back(gRow.SL_FACTOR);
            row.push_back(gRow.AVG_GEN);
            row.push_back(gRow.AVG_IMM);
            row.push_back(gRow.AVG_AGE);
            row.push_back(gRow.AVG_HT);
            row.push_back(gRow.AVG_WT);
            row.push_back(gRow.AVGMA_AP);
            row.push_back(gRow.AVGMA_SP);
            row.push_back(gRow.AVGMA_ST);
            row.push_back(gRow.AVGMA_VT);
            row.push_back(gRow.AVG_SFIT);
            row.push_back(gRow.AVG_DTHF);
            row.push_back(gRow.AVG_VIS);
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
            row.push_back(gRow.YEAR);
            row.push_back(gRow.POP);
            row.push_back(gRow.M_POP);
            row.push_back(gRow.C_PROB);
            row.push_back(gRow.M_AGE_START);
            row.push_back(gRow.M_AGE_END);
            row.push_back(gRow.MX_AGE);
            row.push_back(gRow.MT_PROB);
            row.push_back(gRow.OF_FACTOR);
            row.push_back(gRow.AGE_DTH);
            row.push_back(gRow.FIT_DTH);
            row.push_back(gRow.AFR_DTH);
            row.push_back(gRow.HT_VT);
            row.push_back(gRow.WT_VT);
            row.push_back(gRow.TMB_HT);
            row.push_back(gRow.TMB_VT);
            row.push_back(gRow.TMB_WT);
            row.push_back(gRow.TM_HT);
            row.push_back(gRow.TM_WT);
            row.push_back(gRow.TMM_HT);
            row.push_back(gRow.TMM_VT);
            row.push_back(gRow.TMM_WT);
            row.push_back(gRow.AVG_GEN);
            row.push_back(gRow.AVG_IMM);
            row.push_back(gRow.AVG_AGE);
            row.push_back(gRow.AVG_HT);
            row.push_back(gRow.AVG_WT);
            row.push_back(gRow.AVGMA_VT);
            row.push_back(gRow.AVG_SFIT);
            row.push_back(gRow.AVG_DTHF);
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
