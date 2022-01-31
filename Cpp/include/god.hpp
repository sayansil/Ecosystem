#ifndef GOD_HPP
#define GOD_HPP

#include <algorithm>
#include <iostream>
#include <random>
#include <unordered_map>
#include <utility>
#include <vector>
#include <string>
#include <execution>
#include <world_generated.h>
class God
{
public:
    unsigned int year = 0;

    unsigned int recent_deaths = 0;
    unsigned int recent_births = 0;

    bool monitor_offsprings = false;
    bool disable_deaths = false;

    /******************************
     *  Constructor / Destructor  *
     ******************************/

    God();
    ~God();

    /**************************************
     *         Available to users         *
     **************************************/

    void createWorld(const std::vector<std::unordered_map<std::string, std::string>> &organisms);
    void displayWorldMetadata();

    // void catastrophe();
    // void reset_species(const std::string&);
    // void happy_new_year(const bool &log = false);
    // void kill_organisms(const std::vector<std::string>&);
    // bool spawn_organism(ENTITY&&);
    // bool spawn_organism(ENTITY&&, std::vector<std::pair<std::string, ENTITY>>&);
    // void remember_species(const std::string&);
    // std::string get_annual_data(const std::string&);
    // std::vector<std::map<std::string, std::string>> get_live_data();

    // std::unordered_map<std::string, std::vector<double>> test_organism(ENTITY &&, const std::vector<std::string>&, const int&);

    // std::vector<ENTITY> organism_sort(bool (*comp)(const ENTITY&, const ENTITY&));
    // std::unordered_map<std::string, std::vector<ENTITY>> organism_sort_by_kind(bool (*comp)(const ENTITY&, const ENTITY&));
    // std::unordered_map<std::string, std::unordered_map<StatGroup, std::vector<std::string>>> statistics;

protected:
    // double killer_function(const double &, const double &) const;
    // int creator_function(const double &) const;

private:
    flatbuffers::FlatBufferBuilder builder;

    // DatabaseManager db;
    bool gods_eye = false;
    const int max_mate_trials = 100;

    unsigned int planet_age = 0;
    unsigned int spawn_count = 0;

    /****************************************
     *         Unavailable to users         *
     ****************************************/

    // bool mate(const std::string&, const std::string&, std::vector<std::pair<std::string, ENTITY>>&, const nlohmann::json & = nlohmann::json());
    // void update_species(const std::string&);
};

#endif // GOD_HPP
