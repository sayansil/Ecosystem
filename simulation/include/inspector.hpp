#ifndef INSPECTOR
#define INSPECTOR

#include <cstdint>
#include <nlohmann/json.hpp>
#include <flatbuffers/minireflect.h>

class EcosystemInspector {
public:
    EcosystemInspector(const uint8_t *input_buffer);

    void IterateWorld(flatbuffers::ToStringVisitor *visitor);
private:
    nlohmann::json IterateSpecies(flatbuffers::ElementaryType type, const uint8_t *val,
            const flatbuffers::TypeTable *type_table,
            flatbuffers::ToStringVisitor visitor);
public:
    const uint8_t *buffer;
    nlohmann::json output;

    const flatbuffers::TypeTable *world_type_table;
    const flatbuffers::TypeTable *species_type_table;
    const flatbuffers::TypeTable *organism_type_table;
};


#endif
