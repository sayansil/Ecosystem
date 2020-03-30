#include <database_manager.hpp>

struct RowType
{
    DB_NAME NAME;
    DB_KIND KIND;
    DB_CHROMOSOME CHROMOSOME;
    DB_GENERATION GENERATION;
    DB_IMMUNITY IMMUNITY;
    DB_GENDER GENDER;
    DB_AGE AGE;
    DB_HEIGHT HEIGHT;
    DB_WEIGHT WEIGHT;
    DB_FITNESS FITNESS;
};

static std::vector<RowType> gRows;
static std::vector<std::string> items;

static int callback_read(void *data, int argc, char **argv, char **colName)
{
    RowType gRow;
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
    gRows.push_back(gRow);

    return 0;
}

static int callback_group(void *data, int argc, char **argv, char **colName)
{
    std::string item = argv[0];
    items.push_back(item);

    return 0;
}

DatabaseManager::DatabaseManager(const std::filesystem::path &path)
{
    db_path = path;
    sqlite3_open(db_path.c_str(), &db);
}

DatabaseManager::~DatabaseManager()
{
    sqlite3_close(db);
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
        std::string sql_command = "INSERT INTO ECOSYSTEM_MASTER (NAME,KIND,CHROMOSOME,GENERATION,IMMUNITY,GENDER,AGE,HEIGHT,WEIGHT,FITNESS) "\
                                  "VALUES (" + values + ");";
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
        sql_command = "INSERT INTO " + table_name + " (YEAR,ALL,M_ALL,C_PROB,M_AGE_START,M_AGE_END,MX_AGE,MT_PROB,OF_FACTOR,AGE_DTH,FIT_DTH,AFR_DTH,HT_VT,WT_VT,TMB_HT,TMB_VT,TMB_WT,TM_HT,TM_WT,TMM_HT,TMM_VT,TMM_WT,AVG_GEN,AVG_IMM,AVG_AGE,AVG_HT,AVG_WT,AVGMA_VT,AVG_SFIT,AVG_DTHF) "
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

std::vector<std::vector<STAT>> DatabaseManager::read_rows(const std::string &colName, const std::vector<std::string> &names)
{
    gRows.clear(); gRows.shrink_to_fit();
    std::string values = "";
    for(const auto &name : names)
        values += "\'" + name + "\',";
    values = values.substr(0, values.length() - 1);
    std::string sql_command = "SELECT * FROM ECOSYSTEM_MASTER "\
                              "WHERE " + colName +  " IN (" + values + ");";

    sqlite3_exec(db, sql_command.c_str(), callback_read, 0, nullptr);
    std::vector<std::vector<STAT>> rows;
    for(const auto &gRow : gRows)
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

void DatabaseManager::create_table(const std::string &kind, const std::string &kingdom)
{
    std::string table_name = "STATS_" + kind;
    for (auto & c: table_name) c = toupper(c);

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
                      "ALL              FLOAT                       NOT NULL,"
                      "M_ALL            FLOAT                       NOT NULL,"
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
        std::cout << kind << " table was not created\n";
    }
    else
    {
        std::cout << kind << " table created successfully\n";
    }
}

std::unordered_map<std::string, std::vector<std::vector<STAT>>> DatabaseManager::groupby_kind()
{
    items.clear(); items.shrink_to_fit();
    std::string sql_command = "SELECT DISTINCT KIND FROM ECOSYSTEM_MASTER;";
    sqlite3_exec(db, sql_command.c_str(), callback_group, 0, nullptr);
    std::unordered_map<std::string, std::vector<std::vector<STAT>>> rowMap;
    for(const auto &item : items)
    {
        auto rows = read_rows("KIND", {item});
        rowMap[item] = rows;
    }
    return rowMap;
}
