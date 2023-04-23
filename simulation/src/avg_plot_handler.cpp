#include <flatbuffers/minireflect.h>
#include <world_generated.h>

#include <avg_plot_handler.hpp>
#include <range/v3/view.hpp>

namespace rv = ranges::views;

void AveragePlotHandler::add_record(const FBuffer& avg_world_instance) {
    const Ecosystem::World* world =
        Ecosystem::GetWorld(avg_world_instance.data());
    for (const Ecosystem::Species* species : *world->species()) {
        auto& species_json = plot_history["species"][species->kind()->str()];
        species_json["kingdom"] =
            Ecosystem::EnumNameKingdomE(species->kingdom());
        auto& organism_json = species_json["organism"];
        for (size_t organism_index :
             rv::iota(0u, species->organism()->size())) {
            const uint8_t* organism_ptr =
                species->organism()->GetAs<uint8_t>(organism_index);
            flatbuffers::ToStringVisitor visitor("", true, "", false);
            flatbuffers::IterateObject(
                organism_ptr, Ecosystem::OrganismTypeTable(), &visitor);
            nlohmann::json current_organism_json =
                nlohmann::json::parse(visitor.s);
            for (const auto& [key, value] : current_organism_json.items()) {
                organism_json[key].emplace_back(value);
            }
        }
    }
}
