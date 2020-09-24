#include <experimental/filesystem>
#include <iostream>
#include <fstream>
#include <sqlite3.h>
#include <string>
#include <helper.hpp>

const std::experimental::filesystem::path master_db_path = helper::get_ecosystem_root() / "data/ecosystem_master.db";
const std::experimental::filesystem::path json_data_path = helper::get_ecosystem_root() / "data/json";
const std::experimental::filesystem::path json_template_path = helper::get_ecosystem_root() / "data/templates/json";

sqlite3 *db;

static void create_master_table()
{
    std::string sql_command;
    int rc;

    sql_command = "CREATE TABLE ECOSYSTEM_MASTER("
                  "name         TEXT        PRIMARY KEY     NOT NULL,"
                  "kind         TEXT                        NOT NULL,"
                  "chromosome   TEXT                        NOT NULL,"
                  "generation   INT                         NOT NULL,"
                  "immunity     FLOAT                       NOT NULL,"
                  "gender       INT                         NOT NULL,"
                  "age          INT                         NOT NULL,"
                  "height       FLOAT                       NOT NULL,"
                  "weight       FLOAT                       NOT NULL,"
                  "fitness      FLOAT                       NOT NULL);";
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
            "year                                          INT        PRIMARY KEY      NOT NULL,"
            "male_population                               FLOAT                       NOT NULL,"
            "female_population                             FLOAT                       NOT NULL,"
            "matable_male_population                       FLOAT                       NOT NULL,"
            "matable_female_population                     FLOAT                       NOT NULL,"
            "conceiving_probability                        FLOAT                       NOT NULL,"
            "mating_age_start                              FLOAT                       NOT NULL,"
            "mating_age_end                                FLOAT                       NOT NULL,"
            "max_age                                       FLOAT                       NOT NULL,"
            "mutation_probability                          FLOAT                       NOT NULL,"
            "offsprings_factor                             FLOAT                       NOT NULL,"
            "age_on_death                                  FLOAT                       NOT NULL,"
            "fitness_on_death                              FLOAT                       NOT NULL,"
            "age_fitness_on_death_ratio                    FLOAT                       NOT NULL,"
            "height_on_speed                               FLOAT                       NOT NULL,"
            "height_on_stamina                             FLOAT                       NOT NULL,"
            "height_on_vitality                            FLOAT                       NOT NULL,"
            "weight_on_speed                               FLOAT                       NOT NULL,"
            "weight_on_stamina                             FLOAT                       NOT NULL,"
            "weight_on_vitality                            FLOAT                       NOT NULL,"
            "vitality_on_appetite                          FLOAT                       NOT NULL,"
            "vitality_on_speed                             FLOAT                       NOT NULL,"
            "stamina_on_appetite                           FLOAT                       NOT NULL,"
            "stamina_on_speed                              FLOAT                       NOT NULL,"
            "theoretical_maximum_base_appetite             FLOAT                       NOT NULL,"
            "theoretical_maximum_base_height               FLOAT                       NOT NULL,"
            "theoretical_maximum_base_speed                FLOAT                       NOT NULL,"
            "theoretical_maximum_base_stamina              FLOAT                       NOT NULL,"
            "theoretical_maximum_base_vitality             FLOAT                       NOT NULL,"
            "theoretical_maximum_base_weight               FLOAT                       NOT NULL,"
            "theoretical_maximum_height                    FLOAT                       NOT NULL,"
            "theoretical_maximum_speed                     FLOAT                       NOT NULL,"
            "theoretical_maximum_weight                    FLOAT                       NOT NULL,"
            "theoretical_maximum_height_multiplier         FLOAT                       NOT NULL,"
            "theoretical_maximum_speed_multiplier          FLOAT                       NOT NULL,"
            "theoretical_maximum_stamina_multiplier        FLOAT                       NOT NULL,"
            "theoretical_maximum_vitality_multiplier       FLOAT                       NOT NULL,"
            "theoretical_maximum_weight_multiplier         FLOAT                       NOT NULL,"
            "sleep_restore_factor                          FLOAT                       NOT NULL,"
            "average_generation                            FLOAT                       NOT NULL,"
            "average_immunity                              FLOAT                       NOT NULL,"
            "average_age                                   FLOAT                       NOT NULL,"
            "average_height                                FLOAT                       NOT NULL,"
            "average_weight                                FLOAT                       NOT NULL,"
            "average_max_appetite_at_age                   FLOAT                       NOT NULL,"
            "average_max_speed_at_age                      FLOAT                       NOT NULL,"
            "average_max_stamina_at_age                    FLOAT                       NOT NULL,"
            "average_max_vitality_at_age                   FLOAT                       NOT NULL,"
            "average_static_fitness                        FLOAT                       NOT NULL,"
            "average_dynamic_fitness                       FLOAT                       NOT NULL,"
            "average_vision_radius                         FLOAT                       NOT NULL);";
    }
    else if (kingdom == "plant")
    {
        sql_command = "CREATE TABLE " + table_name + "("
            "year                                          INT        PRIMARY KEY      NOT NULL,"
            "population                                    FLOAT                       NOT NULL,"
            "matable_population                            FLOAT                       NOT NULL,"
            "conceiving_probability                        FLOAT                       NOT NULL,"
            "mating_age_start                              FLOAT                       NOT NULL,"
            "mating_age_end                                FLOAT                       NOT NULL,"
            "max_age                                       FLOAT                       NOT NULL,"
            "mutation_probability                          FLOAT                       NOT NULL,"
            "offsprings_factor                             FLOAT                       NOT NULL,"
            "age_on_death                                  FLOAT                       NOT NULL,"
            "fitness_on_death                              FLOAT                       NOT NULL,"
            "age_fitness_on_death_ratio                    FLOAT                       NOT NULL,"
            "height_on_vitality                            FLOAT                       NOT NULL,"
            "weight_on_vitality                            FLOAT                       NOT NULL,"
            "theoretical_maximum_base_height               FLOAT                       NOT NULL,"
            "theoretical_maximum_base_vitality             FLOAT                       NOT NULL,"
            "theoretical_maximum_base_weight               FLOAT                       NOT NULL,"
            "theoretical_maximum_height                    FLOAT                       NOT NULL,"
            "theoretical_maximum_weight                    FLOAT                       NOT NULL,"
            "theoretical_maximum_height_multiplier         FLOAT                       NOT NULL,"
            "theoretical_maximum_vitality_multiplier       FLOAT                       NOT NULL,"
            "theoretical_maximum_weight_multiplier         FLOAT                       NOT NULL,"
            "average_generation                            FLOAT                       NOT NULL,"
            "average_immunity                              FLOAT                       NOT NULL,"
            "average_age                                   FLOAT                       NOT NULL,"
            "average_height                                FLOAT                       NOT NULL,"
            "average_weight                                FLOAT                       NOT NULL,"
            "average_max_vitality_at_age                   FLOAT                       NOT NULL,"
            "average_static_fitness                        FLOAT                       NOT NULL,"
            "average_dynamic_fitness                       FLOAT                       NOT NULL);";
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
    std::experimental::filesystem::path filepath;

    auto copy_contents = [](const std::experimental::filesystem::path &source, const std::experimental::filesystem::path &sink) {
        if (!std::experimental::filesystem::exists(sink))
        {
            std::ifstream to_read(source, std::ios::binary);
            std::ofstream to_create(sink, std::ios::binary);

            to_create << to_read.rdbuf();

            to_read.close();
            to_create.close();
        }
    };

    for (const auto &entry : std::experimental::filesystem::directory_iterator(json_data_path / subdirectory))
    {
        // Creating base.json from template
        copy_contents(
            json_template_path / subdirectory / "base.json",
            entry.path() / "base.json");

        // Creating modify.json from template
        copy_contents(
            json_template_path / subdirectory / "modify.json",
            entry.path() / "modify.json");

        // Creating current.json from base.json
        copy_contents(
            entry.path() / "base.json",
            entry.path() / "current.json");

        create_species_table(entry.path());
    }
}

int main()
{

    if(!std::experimental::filesystem::exists(master_db_path))
    {
        sqlite3_open(master_db_path.c_str(), &db);
        create_master_table();
    }
    else
    {
        sqlite3_open(master_db_path.c_str(), &db);
        std::cout << "Using existing db at " << master_db_path << '\n';
    }

    if (std::experimental::filesystem::exists(json_data_path / "animal"))
    {
        parse_species_directories("animal");
    }
    if (std::experimental::filesystem::exists(json_data_path / "plant"))
    {
        parse_species_directories("plant");
    }
}
