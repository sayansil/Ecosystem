#include <database_manager.hpp>
#include <utility>

static std::vector<std::unordered_map<std::string, DBType>> gRowsMaster;
static std::vector<std::unordered_map<std::string, DBType>> gRowsAnimal;
static std::vector<std::unordered_map<std::string, DBType>> gRowsPlant;

static std::vector<std::string> items;

static int callback_read_master(void *data, int argc, char **argv, char **colName)
{
    std::unordered_map<std::string, DBType> gRow;
    int i = 0;
    for (const auto &[colName, colType] : schema::schemaMaster)
    {
        gRow[colName] = DBType(colType, argv[i++]);
    }

    gRowsMaster.push_back(gRow);

    return 0;
}

static int callback_read_animal(void *data, int argc, char **argv, char **colName)
{
    std::unordered_map<std::string, DBType> gRow;
    int i = 0;
    for (const auto &[colName, colType] : schema::schemaAnimal)
    {
        gRow[colName] = DBType(colType, argv[i++]);
    }

    gRowsAnimal.push_back(gRow);

    return 0;
}

static int callback_read_plant(void *data, int argc, char **argv, char **colName)
{
    std::unordered_map<std::string, DBType> gRow;
    int i = 0;
    for (const auto &[colName, colType] : schema::schemaPlant)
    {
        gRow[colName] = DBType(colType, argv[i++]);
    }

    gRowsPlant.push_back(gRow);

    return 0;
}

static int callback_group(void *data, int argc, char **argv, char **colName)
{
    std::string item = argv[0];
    items.push_back(item);

    return 0;
}

DatabaseManager::DatabaseManager(const std::experimental::filesystem::path& path)
{
    db_path = std::experimental::filesystem::canonical(path);
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

void DatabaseManager::insert_rows(const std::vector<std::vector<DBType>> &rows)
{

    for(const auto &row : rows)
    {
        std::string values = "";

        for(const auto &value : row)
        {
            if (value.dtype == SQLType::TEXT)
            {
                values += "\'" + value.data + "\',";
            }
            else
            {
                values += value.data + ",";
            }
        }
        values = values.substr(0, values.length() - 1);

        std::string sql_command = "INSERT INTO ECOSYSTEM_MASTER VALUES (" + values + ")";
        sqlite3_exec(db, sql_command.c_str(), nullptr, 0, nullptr);
    }
}

void DatabaseManager::insert_stat_row(const std::vector<DBType> &row, const std::string &kind)
{
    std::string table_name = "STATS_" + kind;
    for (auto & c: table_name) c = toupper(c);

    std::string values = "";
    for (const auto &value : row)
    {
        if (value.dtype == SQLType::TEXT)
        {
            values += "\'" + value.data + "\',";
        }
        else
        {
            values += value.data + ",";
        }
    }
    values = values.substr(0, values.length() - 1);

    std::string sql_command;
    sql_command = "INSERT INTO " + table_name + " VALUES (" + values + ");";

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

std::vector<std::vector<DBType>> DatabaseManager::read_rows_master(const std::string &columnName, const std::vector<std::string> &names)
{
    gRowsMaster.clear(); gRowsMaster.shrink_to_fit();
    std::string values = "";
    for(const auto &name : names)
        values += "\'" + name + "\',";
    values = values.substr(0, values.length() - 1);
    std::string sql_command = "SELECT * FROM ECOSYSTEM_MASTER "\
                              "WHERE " + columnName +  " IN (" + values + ");";

    sqlite3_exec(db, sql_command.c_str(), callback_read_master, 0, nullptr);
    std::vector<std::vector<DBType>> rows;
    for(auto &gRow : gRowsMaster)
    {
        std::vector<DBType> row;

        for (const auto &[colName, columnType] : schema::schemaMaster)
        {
            row.push_back(gRow[colName]);
        }

        rows.push_back(row);
    }
    return rows;
}

std::vector<std::vector<DBType>> DatabaseManager::read_all_rows_master()
{
    gRowsMaster.clear(); gRowsMaster.shrink_to_fit();
    std::string sql_command = "SELECT * FROM ECOSYSTEM_MASTER";

    sqlite3_exec(db, sql_command.c_str(), callback_read_master, 0, nullptr);
    std::vector<std::vector<DBType>> rows;
    for(auto &gRow : gRowsMaster)
    {
        std::vector<DBType> row;

        for (const auto &[colName, colType] : schema::schemaMaster)
        {
            row.push_back(gRow[colName]);
        }

        rows.push_back(row);
    }
    return rows;
}

std::vector<std::vector<DBType>> DatabaseManager::read_all_rows_stats(const std::string &full_species_name)
{
    std::string kind = full_species_name.substr(full_species_name.find('/') + 1);
    std::string kingdom = full_species_name.substr(0, full_species_name.find('/'));
    std::vector<std::vector<DBType>> rows;
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

        for (auto &gRow : gRowsAnimal)
        {
            std::vector<DBType> row;

            for (const auto &[colName, colType] : schema::schemaAnimal)
            {
                row.push_back(gRow[colName]);
            }

            rows.push_back(row);
        }
    }
    else if(kingdom == "plant")
    {
        gRowsPlant.clear();
        gRowsPlant.shrink_to_fit();
        std::string sql_command = "SELECT * FROM " + table_name;
        sqlite3_exec(db, sql_command.c_str(), callback_read_plant, 0, nullptr);

        for (auto &gRow : gRowsPlant)
        {
            std::vector<DBType> row;

            for (const auto &[colName, colType] : schema::schemaPlant)
            {
                row.push_back(gRow[colName]);
            }

            rows.push_back(row);
        }
    }
    return rows;
}

void DatabaseManager::update_rows(const std::vector<std::vector<DBType>> &rows)
{
    for(const auto &row : rows)
    {
        std::string values = "";

        for (int i = 1; i < schema::schemaMaster.size(); i++)
        {
            auto &[colName, colType] = schema::schemaMaster[i];

            if (colType == SQLType::TEXT)
            {
                values += colName + " = \'" + row[i].data + "\',";
            }
            else
            {
                values += colName + " = " + row[i].data + ",";
            }
        }

        values = values.substr(0, values.length() - 1);

        std::string sql_command = "UPDATE ECOSYSTEM_MASTER SET " + values +
                                    " WHERE name = \'" + row[0].data + "\';";
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

std::unordered_map<std::string, std::vector<std::vector<DBType>>> DatabaseManager::groupby_kind()
{
    items.clear(); items.shrink_to_fit();
    std::string sql_command = "SELECT DISTINCT kind FROM ECOSYSTEM_MASTER;";
    sqlite3_exec(db, sql_command.c_str(), callback_group, 0, nullptr);

    std::unordered_map<std::string, std::vector<std::vector<DBType>>> rowMap;
    for(const auto &item : items)
    {
        auto rows = read_rows_master("kind", {item});
        rowMap[item] = rows;
    }
    return rowMap;
}
