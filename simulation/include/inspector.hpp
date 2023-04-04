#ifndef INSPECTOR
#define INSPECTOR

#include <cstdint>
#include <nlohmann/json.hpp>
#include <flatbuffers/minireflect.h>

class EcosystemInspector {
public:
    EcosystemInspector(const uint8_t *input_buffer, const nlohmann::ordered_json &input_query);

    void IterateWorld();
private:
    nlohmann::json IterateSpecies(const uint8_t *val,
            const flatbuffers::TypeTable *type_table,
            flatbuffers::ToStringVisitor visitor);
    nlohmann::json IterateOrganism(const uint8_t *obj, const flatbuffers::TypeTable *type_table,
                          flatbuffers::ToStringVisitor visitor);
    int processSpeciesQuery(const nlohmann::ordered_json &species_queries, flatbuffers::ToStringVisitor visitor);
    bool processOrganismQuery(const nlohmann::json &organism_queries, const nlohmann::json &organism_json);
public:
    const uint8_t *buffer;
    nlohmann::json output;
    nlohmann::ordered_json query;

    const flatbuffers::TypeTable *world_type_table;
    const flatbuffers::TypeTable *species_type_table;
    const flatbuffers::TypeTable *organism_type_table;
};


#endif
