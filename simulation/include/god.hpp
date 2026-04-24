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
#include <perf_metrics.hpp>

struct ParentData {
    float mating_probability;
    float offsprings_factor;
    float conceiving_probability;
    float mutation_probability;
    Ecosystem::Monitor monitor;
    uint64_t X, Y;
    uint64_t generation;
    uint16_t chromosome_number;
    std::string kind;
    std::string chromosome_str;
};

class God
{
public:
    flatbuffers::DetachedBuffer buffer;
    flatbuffers::DetachedBuffer avg_buffer;
    flatbuffers::DetachedBuffer population_stats;
    uint32_t year = 1;

    bool monitor_offsprings = false;
    bool disable_deaths = false;
    std::filesystem::path ecosystem_root;

    PerfMetrics perf;
    uint32_t perf_log_interval = 10;

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

    flatbuffers::Offset<Ecosystem::Organism> createChildOrganism(
        flatbuffers::FlatBufferBuilder &builder,
        const std::string &kind,
        const std::string &kingdom,
        const uint64_t &age,
        const std::string &name,
        const std::string &chromosome_str,
        const std::vector<uint8_t> &chromosome_bytes,
        flatbuffers::Offset<flatbuffers::String> kind_offset,
        flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Ecosystem::ChromosomeStrand>>> chr_structure_offset,
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

    std::string get_child_chromosome(const std::string &chr1, uint16_t chr1_num,
                                      const std::string &chr2, uint16_t chr2_num,
                                      double mutation_prob);
    void update_species(const std::string &);
    void update_species_cached(const std::string &full_species_name);
    std::unordered_map<std::string, nlohmann::json> species_modify_cache;
    flatbuffers::Offset<Ecosystem::Organism> clone_organism(
        flatbuffers::FlatBufferBuilder &builder,
        const Ecosystem::Organism *previous_organism,
        flatbuffers::Offset<flatbuffers::String> kind_offset,
        flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Ecosystem::ChromosomeStrand>>> chr_structure_offset);
    flatbuffers::Offset<Ecosystem::Organism> clone_organism(
        flatbuffers::FlatBufferBuilder &builder,
        const Ecosystem::Organism *previous_organism);
};

#endif // GOD_HPP
