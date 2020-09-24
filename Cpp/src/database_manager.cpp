#include <database_manager.hpp>
#include <utility>

static const std::vector<std::pair<std::string, SQLType>> schemaMaster {
    {"name", SQLType::TEXT},
    {"kind", SQLType::TEXT},
    {"chromosome", SQLType::TEXT},
    {"generation", SQLType::INT},
    {"immunity", SQLType::FLOAT},
    {"gender", SQLType::INT},
    {"age", SQLType::INT},
    {"height", SQLType::FLOAT},
    {"weight", SQLType::FLOAT},
    {"fitness", SQLType::FLOAT}
};

static const std::vector<std::pair<std::string, SQLType>> schemaAnimal {
    {"year", SQLType::INT},
    {"male_population", SQLType::FLOAT},
    {"female_population", SQLType::FLOAT},
    {"matable_male_population", SQLType::FLOAT},
    {"matable_female_population", SQLType::FLOAT},
    {"conceiving_probability", SQLType::FLOAT},
    {"mating_age_start", SQLType::FLOAT},
    {"mating_age_end", SQLType::FLOAT},
    {"max_age", SQLType::FLOAT},
    {"mutation_probability", SQLType::FLOAT},
    {"offsprings_factor", SQLType::FLOAT},
    {"age_on_death", SQLType::FLOAT},
    {"fitness_on_death", SQLType::FLOAT},
    {"age_fitness_on_death_ratio", SQLType::FLOAT},
    {"height_on_speed", SQLType::FLOAT},
    {"height_on_stamina", SQLType::FLOAT},
    {"height_on_vitality", SQLType::FLOAT},
    {"weight_on_speed", SQLType::FLOAT},
    {"weight_on_stamina", SQLType::FLOAT},
    {"weight_on_vitality", SQLType::FLOAT},
    {"vitality_on_appetite", SQLType::FLOAT},
    {"vitality_on_speed", SQLType::FLOAT},
    {"stamina_on_appetite", SQLType::FLOAT},
    {"stamina_on_speed", SQLType::FLOAT},
    {"theoretical_maximum_base_appetite", SQLType::FLOAT},
    {"theoretical_maximum_base_height", SQLType::FLOAT},
    {"theoretical_maximum_base_speed", SQLType::FLOAT},
    {"theoretical_maximum_base_stamina", SQLType::FLOAT},
    {"theoretical_maximum_base_vitality", SQLType::FLOAT},
    {"theoretical_maximum_base_weight", SQLType::FLOAT},
    {"theoretical_maximum_height", SQLType::FLOAT},
    {"theoretical_maximum_speed", SQLType::FLOAT},
    {"theoretical_maximum_weight", SQLType::FLOAT},
    {"theoretical_maximum_height_multiplier", SQLType::FLOAT},
    {"theoretical_maximum_speed_multiplier", SQLType::FLOAT},
    {"theoretical_maximum_stamina_multiplier", SQLType::FLOAT},
    {"theoretical_maximum_vitality_multiplier", SQLType::FLOAT},
    {"theoretical_maximum_weight_multiplier", SQLType::FLOAT},
    {"sleep_restore_factor", SQLType::FLOAT},
    {"average_generation", SQLType::FLOAT},
    {"average_immunity", SQLType::FLOAT},
    {"average_age", SQLType::FLOAT},
    {"average_height", SQLType::FLOAT},
    {"average_weight", SQLType::FLOAT},
    {"average_max_appetite_at_age", SQLType::FLOAT},
    {"average_max_speed_at_age", SQLType::FLOAT},
    {"average_max_stamina_at_age", SQLType::FLOAT},
    {"average_max_vitality_at_age", SQLType::FLOAT},
    {"average_static_fitness", SQLType::FLOAT},
    {"average_dynamic_fitness", SQLType::FLOAT},
    {"average_vision_radius", SQLType::FLOAT}
};

static const std::vector<std::pair<std::string, SQLType>> schemaPlant {
    {"year", SQLType::INT},
    {"population", SQLType::FLOAT},
    {"matable_population", SQLType::FLOAT},
    {"conceiving_probability", SQLType::FLOAT},
    {"mating_age_start", SQLType::FLOAT},
    {"mating_age_end", SQLType::FLOAT},
    {"max_age", SQLType::FLOAT},
    {"mutation_probability", SQLType::FLOAT},
    {"offsprings_factor", SQLType::FLOAT},
    {"age_on_death", SQLType::FLOAT},
    {"fitness_on_death", SQLType::FLOAT},
    {"age_fitness_on_death_ratio", SQLType::FLOAT},
    {"height_on_vitality", SQLType::FLOAT},
    {"weight_on_vitality", SQLType::FLOAT},
    {"theoretical_maximum_base_height", SQLType::FLOAT},
    {"theoretical_maximum_base_vitality", SQLType::FLOAT},
    {"theoretical_maximum_base_weight", SQLType::FLOAT},
    {"theoretical_maximum_height", SQLType::FLOAT},
    {"theoretical_maximum_weight", SQLType::FLOAT},
    {"theoretical_maximum_height_multiplier", SQLType::FLOAT},
    {"theoretical_maximum_vitality_multiplier", SQLType::FLOAT},
    {"theoretical_maximum_weight_multiplier", SQLType::FLOAT},
    {"average_generation", SQLType::FLOAT},
    {"average_immunity", SQLType::FLOAT},
    {"average_age", SQLType::FLOAT},
    {"average_height", SQLType::FLOAT},
    {"average_weight", SQLType::FLOAT},
    {"average_max_vitality_at_age", SQLType::FLOAT},
    {"average_static_fitness", SQLType::FLOAT},
    {"average_dynamic_fitness", SQLType::FLOAT},
};

static std::vector<std::unordered_map<std::string, DBType>> gRowsMaster;
static std::vector<std::unordered_map<std::string, DBType>> gRowsAnimal;
static std::vector<std::unordered_map<std::string, DBType>> gRowsPlant;

static std::vector<std::string> items;

static int callback_read_master(void *data, int argc, char **argv, char **colName)
{
    std::unordered_map<std::string, DBType> gRow;
    int i = 0;
    for (const auto &[colName, colType] : schemaMaster)
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
    for (const auto &[colName, colType] : schemaAnimal)
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
    for (const auto &[colName, colType] : schemaPlant)
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
    for(const auto &gRow : gRowsMaster)
    {
        std::vector<DBType> row;

        for (const auto &[colName, columnType] : schemaMaster)
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
    for(const auto &gRow : gRowsMaster)
    {
        std::vector<DBType> row;

        for (const auto &[colName, colType] : schemaMaster)
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

        for (const auto &gRow : gRowsAnimal)
        {
            std::vector<DBType> row;

            for (const auto &[colName, colType] : schemaAnimal)
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

        for (const auto &gRow : gRowsPlant)
        {
            std::vector<DBType> row;

            for (const auto &[colName, colType] : schemaPlant)
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

        for (int i = 1; i < schemaMaster.size(); i++)
        {
            auto &[colName, colType] = schemaMaster[i];

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
