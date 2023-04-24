#include <fmt/core.h>
#include <fmt/ranges.h>
#include <world_generated.h>

#include <inspector.hpp>
#include <optional>
#include <range/v3/view.hpp>

namespace rv = ranges::views;

EcosystemInspector::EcosystemInspector(
    const uint8_t* input_buffer, const nlohmann::ordered_json& input_query) {
    buffer = input_buffer;  // intentional shallow copy
    world_type_table = Ecosystem::WorldTypeTable();
    species_type_table = Ecosystem::SpeciesTypeTable();
    organism_type_table = Ecosystem::OrganismTypeTable();
    query = input_query;
}

std::optional<nlohmann::json> EcosystemInspector::GetMatchingSpeciesQuery(
    const nlohmann::json& query, const nlohmann::json& species_item) {
    for (const auto& [current_species_index, current_species] : query.items()) {
        if (current_species["kind"] == species_item["kind"] &&
            current_species["kingdom"] == species_item["kingdom"]) {
            return current_species;
        }
    }
    return {};
};

void EcosystemInspector::Inspect() {
    const Ecosystem::World* world = Ecosystem::GetWorld(buffer);
    fmt::print("query: {}\n", query.dump(4));
    for (const Ecosystem::Species* species : *world->species()) {
        nlohmann::json species_json;
        species_json["kind"] = species->kind()->str();
        species_json["kingdom"] =
            Ecosystem::EnumNameKingdomE(species->kingdom());
        auto matchingSpeciesQuery =
            GetMatchingSpeciesQuery(query["species"], species_json);
        if (matchingSpeciesQuery.has_value()) {
            for (size_t organism_index :
                 rv::iota(0u, species->organism()->size())) {
                const uint8_t* organism_ptr =
                    species->organism()->GetAs<uint8_t>(organism_index);
                flatbuffers::ToStringVisitor visitor("", true, "", false);
                flatbuffers::IterateObject(
                    organism_ptr, Ecosystem::OrganismTypeTable(), &visitor);
                nlohmann::json current_organism_json =
                    nlohmann::json::parse(visitor.s);
                if (query["type"] == "extract") {
                    for (const auto& [key, value] :
                         (*matchingSpeciesQuery)["organism"].items()) {
                        species_json["organism"][value].emplace_back(
                            current_organism_json[value]);
                    }
                } else if (query["type"] == "filter") {
                    if (processOrganismQuery(
                            (*matchingSpeciesQuery)["organism"],
                            current_organism_json)) {
                        species_json["organism"].emplace_back(
                            current_organism_json);
                    }
                }
            }
            output["species"].emplace_back(species_json);
        }
    }
}

bool EcosystemInspector::processOrganismQuery(
    const nlohmann::json& organism_queries,
    const nlohmann::json& organism_json) {
    for (const auto& item : organism_queries.items()) {
        for (const auto& [query_stat_name, query_stat_value] :
             item.value().items()) {
            if (query_stat_value.is_structured()) {
                float lower_bound = std::numeric_limits<float>::min();
                float upper_bound = std::numeric_limits<float>::max();
                lower_bound = query_stat_value["low"].get<float>();
                upper_bound = query_stat_value["high"].get<float>();
                if (organism_json[query_stat_name] < lower_bound ||
                    organism_json[query_stat_name] > upper_bound) {
                    return false;
                }
            }
        }
    }
    return true;
}
