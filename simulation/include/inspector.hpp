#ifndef INSPECTOR
#define INSPECTOR

#include <cstdint>
#include <nlohmann/json.hpp>
#include <flatbuffers/minireflect.h>

class EcosystemInspector {
public:
    EcosystemInspector(const uint8_t *input_buffer, const nlohmann::ordered_json &input_query);
    void Inspect();
private:
    bool processOrganismQuery(const nlohmann::json &organism_queries, const nlohmann::json &organism_json);
    std::optional<nlohmann::json> GetMatchingSpeciesQuery(const nlohmann::json& query, const nlohmann::json& species_item);
public:
    const uint8_t *buffer;
    nlohmann::json output;
    nlohmann::ordered_json query;

    const flatbuffers::TypeTable *world_type_table;
    const flatbuffers::TypeTable *species_type_table;
    const flatbuffers::TypeTable *organism_type_table;
};


#endif
