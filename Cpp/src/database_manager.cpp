#include <database_manager.hpp>

struct RowType
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

DatabaseManager::DatabaseManager(const std::filesystem::path& path)
{
    db_path = path;
    sqlite3_open(db_path.c_str(), &db);
}

DatabaseManager::~DatabaseManager()
{
    sqlite3_close(db);
}

void DatabaseManager::insertRows(const std::vector<std::vector<STAT_TYPE>>& rows)
{
    for(const auto& row : rows)
    {
        std::string values = "";
        for(const auto& value : row)
        {
            if(value.index() == 0)
            {
                values += std::to_string(std::get<unsigned int>(value)) + ",";
            }
            if(value.index() == 1)
            {
                values += std::to_string(std::get<double>(value)) + ",";
            }
            if(value.index() == 2)
            {
                values += "\'" + std::get<std::string>(value) + "\',";
            }
            if(value.index() == 3)
            {
                bool tmp = std::get<bool>(value);
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

void DatabaseManager::deleteRows(const std::vector<std::string>& names)
{
    std::string values = "";
    for(const auto& name : names)
        values += "\'" + name + "\',";
    values = values.substr(0, values.length() - 1);
    std::string sql_command = "DELETE FROM ECOSYSTEM_MASTER "\
                              "WHERE NAME IN (" + values + ");";
    sqlite3_exec(db, sql_command.c_str(), nullptr, 0, nullptr);
}

std::vector<std::vector<STAT_TYPE>> DatabaseManager::readRows(const std::string& colName, const std::vector<std::string>& names)
{
    gRows.clear(); gRows.shrink_to_fit();
    std::string values = "";
    for(const auto& name : names)
        values += "\'" + name + "\',";
    values = values.substr(0, values.length() - 1);
    std::string sql_command = "SELECT * FROM ECOSYSTEM_MASTER "\
                              "WHERE " + colName +  " IN (" + values + ");";

    sqlite3_exec(db, sql_command.c_str(), callback_read, 0, nullptr);
    std::vector<std::vector<STAT_TYPE>> rows;
    for(const auto& gRow : gRows)
    {
        std::vector<STAT_TYPE> row;
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

void DatabaseManager::updateRows(const std::vector<std::vector<STAT_TYPE>>& rows)
{
    for(const auto& row : rows)
    {
        std::string values = "";
        values += "KIND = \'" + std::get<std::string>(row[1]) + "\',";
        values += "CHROMOSOME = \'" + std::get<std::string>(row[2]) + "\',";
        values += "GENERATION = " + std::to_string(std::get<unsigned int>(row[3])) + ",";
        values += "IMMUNITY = " + std::to_string(std::get<double>(row[4])) + ",";
        values += "GENDER = " + std::to_string(std::get<unsigned int>(row[5])) + ",";
        values += "AGE = " + std::to_string(std::get<unsigned int>(row[6])) + ",";
        values += "HEIGHT = " + std::to_string(std::get<double>(row[7])) + ",";
        values += "WEIGHT = " + std::to_string(std::get<double>(row[8])) + ",";
        values += "FITNESS = " + std::to_string(std::get<double>(row[9]));
        std::string sql_command = "UPDATE ECOSYSTEM_MASTER SET " + values + \
                                  " WHERE NAME = \'" + std::get<std::string>(row[0]) + "\';";
        sqlite3_exec(db, sql_command.c_str(), nullptr, 0, nullptr);
    }
}

void DatabaseManager::clearDatabase()
{
    std::string sql_command = "DELETE FROM ECOSYSTEM_MASTER;";
    sqlite3_exec(db, sql_command.c_str(), nullptr, 0, nullptr);
}

std::unordered_map<std::string, std::vector<std::vector<STAT_TYPE>>> DatabaseManager::groupByKind()
{
    items.clear(); items.shrink_to_fit();
    std::string sql_command = "SELECT DISTINCT KIND FROM ECOSYSTEM_MASTER;";
    sqlite3_exec(db, sql_command.c_str(), callback_group, 0, nullptr);
    std::unordered_map<std::string, std::vector<std::vector<STAT_TYPE>>> rowMap;
    for(const auto& item : items)
    {
        auto rows = readRows("KIND", {item});
        rowMap[item] = rows;
    }
    return rowMap;
}
