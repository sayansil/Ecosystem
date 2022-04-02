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
#include <database_manager.hpp>

class God
{
public:
    flatbuffers::DetachedBuffer buffer;
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

    void createWorld(std::vector<std::unordered_map<std::string, std::string>> &organisms);
    flatbuffers::Offset<Ecosystem::Organism> createOrganism(
        flatbuffers::FlatBufferBuilder &builder,
        Ecosystem::OrganismBuilder &organism_builder,
        const std::string &kind,
        const std::string &kingdom,
        const uint64_t &age,
        const std::string &name,
        const std::string &chromosome,
        const uint64_t &generation,
        const std::pair<uint64_t, uint64_t> &XY,
        const int8_t &monitor);

    flatbuffers::Offset<Ecosystem::Organism> createOrganism(
        flatbuffers::FlatBufferBuilder &builder,
        Ecosystem::OrganismBuilder &organism_builder,
        const std::string &kind,
        const std::string &kingdom,
        const uint64_t &age,
        const int8_t &monitor = 0);

    void displayWorldMetadata();

    // void catastrophe();
    void cleanSlate();
    void happy_new_year(const bool &log = false);
    // void kill_organisms(const std::vector<std::string>&);
    bool spawn_organism(Ecosystem::Organism *);
    // bool spawn_organism(ENTITY&&, std::vector<std::pair<std::string, ENTITY>>&);
    // void remember_species(const std::string&);
    // std::string get_annual_data(const std::string&);
    // std::vector<std::map<std::string, std::string>> get_live_data();

    // std::unordered_map<std::string, std::vector<double>> test_organism(ENTITY &&, const std::vector<std::string>&, const int&);

    // std::vector<ENTITY> organism_sort(bool (*comp)(const ENTITY&, const ENTITY&));
    // std::unordered_map<std::string, std::vector<ENTITY>> organism_sort_by_kind(bool (*comp)(const ENTITY&, const ENTITY&));
    // std::unordered_map<std::string, std::unordered_map<StatGroup, std::vector<std::string>>> statistics;

protected:
    double killer_function(const double &, const double &) const;
    int creator_function(const double &) const;

private:
    flatbuffers::FlatBufferBuilder builder;

    DatabaseManager db;
    bool gods_eye = false;
    const int max_mate_trials = 100;

    unsigned int spawn_count = 0;

    /****************************************
     *         Unavailable to users         *
     ****************************************/

    // bool mate(Ecosystem::Organism *, Ecosystem::Organism *, const nlohmann::json &= nlohmann::json());
    void update_species(const std::string &);
    flatbuffers::Offset<Ecosystem::Organism> clone_organism(
        flatbuffers::FlatBufferBuilder &builder,
        Ecosystem::OrganismBuilder &organism_builder,
        const Ecosystem::Organism *previous_organism);
    // template <typename T>
    // void assignBaseStats(
    //     const std::string &kingdom,
    //     const std::string &kind,
    //     const T *basePtr)
    // {
    // }
};

#endif // GOD_HPP
