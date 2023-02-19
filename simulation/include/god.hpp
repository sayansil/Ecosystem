#ifndef GOD_HPP
#define GOD_HPP

#include <algorithm>
#include <iostream>
#include <random>
#include <unordered_map>
#include <utility>
#include <vector>
#include <string>
#include <world_generated.h>
#include <database_manager.hpp>
#include <nlohmann/json.hpp>

class God
{
public:
    flatbuffers::DetachedBuffer buffer;
    uint32_t year = 1;

    bool monitor_offsprings = false;
    bool disable_deaths = false;
    std::filesystem::path ecosystem_root;

    /******************************
     *  Constructor / Destructor  *
     ******************************/

    God(const std::filesystem::path& ecosystem_root = helper::get_ecosystem_root(), const bool gods_eye = false);
    ~God();

    /**************************************
     *         Available to users         *
     **************************************/

    void createWorld(std::vector<std::unordered_map<std::string, std::string>> &organisms);
    flatbuffers::Offset<Ecosystem::Organism> createOrganism(
        flatbuffers::FlatBufferBuilder &builder,
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
        const std::string &kind,
        const std::string &kingdom,
        const uint64_t &age,
        const int8_t &monitor = 0);

    void displayWorldMetadata();

    void cleanSlate();
    void happy_new_year(const bool &log = false);

protected:
    double killer_function(const double &, const double &) const;
    int creator_function(const double &) const;

private:
    flatbuffers::FlatBufferBuilder builder;

    std::unique_ptr<DatabaseManager> db;
    bool gods_eye = false;

    /****************************************
     *         Unavailable to users         *
     ****************************************/

    std::string get_child_chromosome(const Ecosystem::OrganismT &, const Ecosystem::OrganismT &, const nlohmann::json & = nlohmann::json());
    void update_species(const std::string &);
    flatbuffers::Offset<Ecosystem::Organism> clone_organism(
        flatbuffers::FlatBufferBuilder &builder,
        const Ecosystem::Organism *previous_organism);
};

#endif // GOD_HPP
