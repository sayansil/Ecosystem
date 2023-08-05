#include <flatbuffers/minireflect.h>
#include <fmt/core.h>
#include <world_generated.h>

#include <avg_plot_handler.hpp>
#include <helper.hpp>
#include <range/v3/view.hpp>

namespace rv = ranges::views;

static nlohmann::json flatten_population_stats(
    const FBuffer& world_population_stats) {
    nlohmann::json current_species_population;
    const Ecosystem::WorldPopulation* world_population =
        Ecosystem::GetWorldPopulation(world_population_stats.data());
    for (const Ecosystem::SpeciesPopulation* species_population :
         *world_population->species_population()) {
        current_species_population[species_population->kind()
                                       ->str()]["matable_population"] =
            species_population->matable_population()->male_population() +
            species_population->matable_population()->female_population();
        current_species_population[species_population->kind()
                                       ->str()]["non_matable_population"] =
            species_population->non_matable_population()->male_population() +
            species_population->non_matable_population()->female_population();
        current_species_population[species_population->kind()
                                       ->str()]["male_population"] =
            species_population->matable_population()->male_population() +
            species_population->non_matable_population()->male_population();
        current_species_population[species_population->kind()
                                       ->str()]["female_population"] =
            species_population->matable_population()->female_population() +
            species_population->non_matable_population()->female_population();
        current_species_population[species_population->kind()
                                       ->str()]["population"] =
            current_species_population[species_population->kind()->str()]
                                      ["male_population"]
                                          .get<uint32_t>() +
            current_species_population[species_population->kind()->str()]
                                      ["female_population"]
                                          .get<uint32_t>();
    }
    return current_species_population;
}

void AveragePlotHandler::add_record(const FBuffer& avg_world_instance,
                                    const FBuffer& world_population_stats) {
    const Ecosystem::World* world =
        Ecosystem::GetWorld(avg_world_instance.data());
    nlohmann::json population_stats_json =
        flatten_population_stats(world_population_stats);
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

            // add 0s to empty entries
            const flatbuffers::TypeTable* type_table =
                Ecosystem::OrganismTypeTable();
            for (size_t i = 0; i < type_table->num_elems; i++) {
                if (type_table->type_codes[i].base_type ==
                        flatbuffers::ET_FLOAT ||
                    type_table->type_codes[i].base_type ==
                        flatbuffers::ET_UINT) {
                    if (current_organism_json.find(type_table->names[i]) ==
                        current_organism_json.end()) {
                        organism_json[type_table->names[i]].emplace_back(0);
                    }
                }
            }
        }
        auto& population_json = species_json["organism"];
        for (const auto& [key, value] :
             population_stats_json[species->kind()->str()].items()) {
            population_json[key].emplace_back(value);
        }
    }
}
