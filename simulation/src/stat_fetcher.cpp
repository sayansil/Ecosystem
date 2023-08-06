#include <fmt/core.h>
#include <population_generated.h>
#include <world_generated.h>

#include <ctime>
#include <database_manager.hpp>
#include <stat_fetcher.hpp>
#include <unordered_map>

static std::vector<float> get_value_vector(
    const std::vector<std::unordered_map<std::string, float>> &worlds,
    const std::string &attribute);

namespace stat_fetcher {

std::unordered_map<uint32_t, uint32_t> get_generation_count(
    const flatbuffers::DetachedBuffer &world_buffer) {
    // Map of (generation: live count of that generation)
    std::unordered_map<uint32_t, uint32_t> gen_map;

    const Ecosystem::World *world_pointer =
        Ecosystem::GetWorld(world_buffer.data());
    for (const Ecosystem::Species *species : *(world_pointer->species())) {
        for (const Ecosystem::Organism *organism : *(species->organism())) {
            uint32_t generation = organism->generation();
            gen_map[generation] = gen_map[generation] + 1;
        }
    }

    return gen_map;
}

std::unordered_map<uint32_t, uint32_t> get_age_count(
    const flatbuffers::DetachedBuffer &world_buffer) {
    // Map of (age: live count of that age)
    std::unordered_map<uint32_t, uint32_t> age_map;

    const Ecosystem::World *world_pointer =
        Ecosystem::GetWorld(world_buffer.data());
    for (const Ecosystem::Species *species : *(world_pointer->species())) {
        for (const Ecosystem::Organism *organism : *(species->organism())) {
            uint32_t age = organism->age();
            age_map[age] = age_map[age] + 1;
        }
    }

    return age_map;
}

FBuffer get_population_stats(const flatbuffers::DetachedBuffer &world_buffer) {
    const Ecosystem::World *world_pointer =
        Ecosystem::GetWorld(world_buffer.data());

    flatbuffers::FlatBufferBuilder new_builder;

    std::vector<flatbuffers::Offset<Ecosystem::SpeciesPopulation>>
        new_stdvecSpeciesPopulation;

    for (const Ecosystem::Species *species : *(world_pointer->species())) {
        if (species->organism()->size() == 0) {
            continue;
        }

        uint32_t male_matable_population = 0;
        uint32_t female_matable_population = 0;
        uint32_t male_non_matable_population = 0;
        uint32_t female_non_matable_population = 0;

        for (const Ecosystem::Organism *organism : *(species->organism())) {
            if (organism->age() >= organism->mating_age_start() &&
                organism->age() <= organism->mating_age_end()) {
                if (organism->gender() == Ecosystem::Gender::male) {
                    male_matable_population++;
                } else if (organism->gender() == Ecosystem::Gender::female) {
                    female_matable_population++;
                }
            } else {
                if (organism->gender() == Ecosystem::Gender::male) {
                    male_non_matable_population++;
                } else if (organism->gender() == Ecosystem::Gender::female) {
                    female_non_matable_population++;
                }
            }
        }

        auto matablePopulation = Ecosystem::RawPopulation(
            male_matable_population, female_matable_population);
        auto nonMatablePopulation = Ecosystem::RawPopulation(
            male_non_matable_population, female_non_matable_population);

        new_stdvecSpeciesPopulation.push_back(
            Ecosystem::CreateSpeciesPopulation(
                new_builder, new_builder.CreateString(species->kind()),
                static_cast<uint8_t>(species->kingdom()), &matablePopulation,
                &nonMatablePopulation));
    }

    new_builder.Finish(Ecosystem::CreateWorldPopulation(
        new_builder, world_pointer->year(),
        new_builder.CreateVector(new_stdvecSpeciesPopulation.data(),
                                 new_stdvecSpeciesPopulation.size())));

    FBuffer new_buffer = new_builder.Release();

    new_builder.Clear();
    return new_buffer;
}

FBuffer create_avg_world(const flatbuffers::DetachedBuffer &world_buffer) {
    const Ecosystem::World *world_pointer =
        Ecosystem::GetWorld(world_buffer.data());

    flatbuffers::FlatBufferBuilder new_builder;
    std::vector<flatbuffers::Offset<Ecosystem::Species>> new_stdvecSpecies;

    // Do avg stuff here

    for (const Ecosystem::Species *species : *(world_pointer->species())) {
        if (species->organism()->size() == 0) {
            continue;
        }

        std::vector<flatbuffers::Offset<Ecosystem::Organism>>
            new_stdvecOrganisms;
        // Store a single organism with avg stats in this vector

        std::unordered_map<std::string, float> org_map;

        int k = 0;

        // Constant (or dummy) stats for average for all organisms in a species

        auto chromosome_number =
            species->organism()->Get(0)->chromosome_number();
        auto sexuality = species->organism()->Get(0)->sexuality();
        auto food_chain_rank = species->organism()->Get(0)->food_chain_rank();
        auto kind =
            new_builder.CreateString(species->organism()->Get(0)->kind());
        auto kingdom = species->organism()->Get(0)->kingdom();
        auto name = new_builder.CreateString(
            species->organism()->Get(0)->kind()->str() + "_avg_organism");

        auto gender = species->organism()->Get(0)->gender();    // dummy
        auto asleep = species->organism()->Get(0)->asleep();    // dummy
        auto monitor = species->organism()->Get(0)->monitor();  // dummy

        std::vector<uint8_t> chromosome_vec;  // dummy
        auto chromosome = new_builder.CreateVector(chromosome_vec.data(),
                                                   chromosome_vec.size());

        // Calculate average stats for all organisms in a species

        std::vector<flatbuffers::Offset<Ecosystem::ChromosomeStrand>>
            newStdvecCStrand;
        for (const auto &cStrand :
             *species->organism()->Get(0)->chromosome_structure()) {
            newStdvecCStrand.push_back(Ecosystem::CreateChromosomeStrand(
                new_builder, new_builder.CreateString(cStrand->code()),
                cStrand->start(), cStrand->length()));
        }
        auto chromosome_structure = new_builder.CreateVectorOfSortedTables(
            newStdvecCStrand.data(), newStdvecCStrand.size());

        for (const Ecosystem::Organism *organism : *(species->organism())) {
            {
                float &prev_avg = org_map["offsprings_factor"];
                prev_avg = prev_avg +
                           (organism->offsprings_factor() - prev_avg) / (k + 1);
            }
            {
                float &prev_avg = org_map["mutation_probability"];
                prev_avg =
                    prev_avg +
                    (organism->mutation_probability() - prev_avg) / (k + 1);
            }
            {
                float &prev_avg = org_map["conceiving_probability"];
                prev_avg =
                    prev_avg +
                    (organism->conceiving_probability() - prev_avg) / (k + 1);
            }
            {
                float &prev_avg = org_map["mating_probability"];
                prev_avg =
                    prev_avg +
                    (organism->mating_probability() - prev_avg) / (k + 1);
            }
            {
                float &prev_avg = org_map["age_fitness_on_death_ratio"];
                prev_avg = prev_avg +
                           (organism->age_fitness_on_death_ratio() - prev_avg) /
                               (k + 1);
            }
            {
                float &prev_avg = org_map["height_on_speed"];
                prev_avg = prev_avg +
                           (organism->height_on_speed() - prev_avg) / (k + 1);
            }
            {
                float &prev_avg = org_map["height_on_stamina"];
                prev_avg = prev_avg +
                           (organism->height_on_stamina() - prev_avg) / (k + 1);
            }
            {
                float &prev_avg = org_map["height_on_vitality"];
                prev_avg =
                    prev_avg +
                    (organism->height_on_vitality() - prev_avg) / (k + 1);
            }
            {
                float &prev_avg = org_map["weight_on_speed"];
                prev_avg = prev_avg +
                           (organism->weight_on_speed() - prev_avg) / (k + 1);
            }
            {
                float &prev_avg = org_map["weight_on_stamina"];
                prev_avg = prev_avg +
                           (organism->weight_on_stamina() - prev_avg) / (k + 1);
            }
            {
                float &prev_avg = org_map["weight_on_vitality"];
                prev_avg =
                    prev_avg +
                    (organism->weight_on_vitality() - prev_avg) / (k + 1);
            }
            {
                float &prev_avg = org_map["vitality_on_appetite"];
                prev_avg =
                    prev_avg +
                    (organism->vitality_on_appetite() - prev_avg) / (k + 1);
            }
            {
                float &prev_avg = org_map["vitality_on_speed"];
                prev_avg = prev_avg +
                           (organism->vitality_on_speed() - prev_avg) / (k + 1);
            }
            {
                float &prev_avg = org_map["stamina_on_appetite"];
                prev_avg =
                    prev_avg +
                    (organism->stamina_on_appetite() - prev_avg) / (k + 1);
            }
            {
                float &prev_avg = org_map["stamina_on_speed"];
                prev_avg = prev_avg +
                           (organism->stamina_on_speed() - prev_avg) / (k + 1);
            }
            {
                float &prev_avg = org_map["theoretical_maximum_base_appetite"];
                prev_avg =
                    prev_avg +
                    (organism->theoretical_maximum_base_appetite() - prev_avg) /
                        (k + 1);
            }
            {
                float &prev_avg = org_map["theoretical_maximum_base_height"];
                prev_avg =
                    prev_avg +
                    (organism->theoretical_maximum_base_height() - prev_avg) /
                        (k + 1);
            }
            {
                float &prev_avg = org_map["theoretical_maximum_base_speed"];
                prev_avg =
                    prev_avg +
                    (organism->theoretical_maximum_base_speed() - prev_avg) /
                        (k + 1);
            }
            {
                float &prev_avg = org_map["theoretical_maximum_base_stamina"];
                prev_avg =
                    prev_avg +
                    (organism->theoretical_maximum_base_stamina() - prev_avg) /
                        (k + 1);
            }
            {
                float &prev_avg = org_map["theoretical_maximum_base_vitality"];
                prev_avg =
                    prev_avg +
                    (organism->theoretical_maximum_base_vitality() - prev_avg) /
                        (k + 1);
            }
            {
                float &prev_avg = org_map["theoretical_maximum_base_weight"];
                prev_avg =
                    prev_avg +
                    (organism->theoretical_maximum_base_weight() - prev_avg) /
                        (k + 1);
            }
            {
                float &prev_avg =
                    org_map["theoretical_maximum_height_multiplier"];
                prev_avg = prev_avg +
                           (organism->theoretical_maximum_height_multiplier() -
                            prev_avg) /
                               (k + 1);
            }
            {
                float &prev_avg =
                    org_map["theoretical_maximum_speed_multiplier"];
                prev_avg = prev_avg +
                           (organism->theoretical_maximum_speed_multiplier() -
                            prev_avg) /
                               (k + 1);
            }
            {
                float &prev_avg =
                    org_map["theoretical_maximum_stamina_multiplier"];
                prev_avg = prev_avg +
                           (organism->theoretical_maximum_stamina_multiplier() -
                            prev_avg) /
                               (k + 1);
            }
            {
                float &prev_avg =
                    org_map["theoretical_maximum_vitality_multiplier"];
                prev_avg =
                    prev_avg +
                    (organism->theoretical_maximum_vitality_multiplier() -
                     prev_avg) /
                        (k + 1);
            }
            {
                float &prev_avg =
                    org_map["theoretical_maximum_weight_multiplier"];
                prev_avg = prev_avg +
                           (organism->theoretical_maximum_weight_multiplier() -
                            prev_avg) /
                               (k + 1);
            }
            {
                float &prev_avg = org_map["theoretical_maximum_height"];
                prev_avg = prev_avg +
                           (organism->theoretical_maximum_height() - prev_avg) /
                               (k + 1);
            }
            {
                float &prev_avg = org_map["theoretical_maximum_speed"];
                prev_avg = prev_avg +
                           (organism->theoretical_maximum_speed() - prev_avg) /
                               (k + 1);
            }
            {
                float &prev_avg = org_map["theoretical_maximum_weight"];
                prev_avg = prev_avg +
                           (organism->theoretical_maximum_weight() - prev_avg) /
                               (k + 1);
            }
            {
                float &prev_avg = org_map["sleep_restore_factor"];
                prev_avg =
                    prev_avg +
                    (organism->sleep_restore_factor() - prev_avg) / (k + 1);
            }
            {
                float &prev_avg = org_map["vision_radius"];
                prev_avg =
                    prev_avg + (organism->vision_radius() - prev_avg) / (k + 1);
            }
            {
                float &prev_avg = org_map["immunity"];
                prev_avg =
                    prev_avg + (organism->immunity() - prev_avg) / (k + 1);
            }
            {
                float &prev_avg = org_map["base_appetite"];
                prev_avg =
                    prev_avg + (organism->base_appetite() - prev_avg) / (k + 1);
            }
            {
                float &prev_avg = org_map["base_speed"];
                prev_avg =
                    prev_avg + (organism->base_speed() - prev_avg) / (k + 1);
            }
            {
                float &prev_avg = org_map["base_stamina"];
                prev_avg =
                    prev_avg + (organism->base_stamina() - prev_avg) / (k + 1);
            }
            {
                float &prev_avg = org_map["base_vitality"];
                prev_avg =
                    prev_avg + (organism->base_vitality() - prev_avg) / (k + 1);
            }
            {
                float &prev_avg = org_map["base_weight"];
                prev_avg =
                    prev_avg + (organism->base_weight() - prev_avg) / (k + 1);
            }
            {
                float &prev_avg = org_map["base_height"];
                prev_avg =
                    prev_avg + (organism->base_height() - prev_avg) / (k + 1);
            }
            {
                float &prev_avg = org_map["weight"];
                prev_avg = prev_avg + (organism->weight() - prev_avg) / (k + 1);
            }
            {
                float &prev_avg = org_map["height"];
                prev_avg = prev_avg + (organism->height() - prev_avg) / (k + 1);
            }
            {
                float &prev_avg = org_map["max_weight"];
                prev_avg =
                    prev_avg + (organism->max_weight() - prev_avg) / (k + 1);
            }
            {
                float &prev_avg = org_map["max_height"];
                prev_avg =
                    prev_avg + (organism->max_height() - prev_avg) / (k + 1);
            }
            {
                float &prev_avg = org_map["weight_multiplier"];
                prev_avg = prev_avg +
                           (organism->weight_multiplier() - prev_avg) / (k + 1);
            }
            {
                float &prev_avg = org_map["height_multiplier"];
                prev_avg = prev_avg +
                           (organism->height_multiplier() - prev_avg) / (k + 1);
            }
            {
                float &prev_avg = org_map["speed_multiplier"];
                prev_avg = prev_avg +
                           (organism->speed_multiplier() - prev_avg) / (k + 1);
            }
            {
                float &prev_avg = org_map["stamina_multiplier"];
                prev_avg =
                    prev_avg +
                    (organism->stamina_multiplier() - prev_avg) / (k + 1);
            }
            {
                float &prev_avg = org_map["vitality_multiplier"];
                prev_avg =
                    prev_avg +
                    (organism->vitality_multiplier() - prev_avg) / (k + 1);
            }
            {
                float &prev_avg = org_map["vitality"];
                prev_avg =
                    prev_avg + (organism->vitality() - prev_avg) / (k + 1);
            }
            {
                float &prev_avg = org_map["stamina"];
                prev_avg =
                    prev_avg + (organism->stamina() - prev_avg) / (k + 1);
            }
            {
                float &prev_avg = org_map["appetite"];
                prev_avg =
                    prev_avg + (organism->appetite() - prev_avg) / (k + 1);
            }
            {
                float &prev_avg = org_map["speed"];
                prev_avg = prev_avg + (organism->speed() - prev_avg) / (k + 1);
            }
            {
                float &prev_avg = org_map["static_fitness"];
                prev_avg = prev_avg +
                           (organism->static_fitness() - prev_avg) / (k + 1);
            }
            {
                float &prev_avg = org_map["dynamic_fitness"];
                prev_avg = prev_avg +
                           (organism->dynamic_fitness() - prev_avg) / (k + 1);
            }
            {
                float &prev_avg = org_map["mating_age_start"];
                prev_avg = prev_avg +
                           (organism->mating_age_start() - prev_avg) / (k + 1);
            }
            {
                float &prev_avg = org_map["mating_age_end"];
                prev_avg = prev_avg +
                           (organism->mating_age_end() - prev_avg) / (k + 1);
            }
            {
                float &prev_avg = org_map["max_age"];
                prev_avg =
                    prev_avg + (organism->max_age() - prev_avg) / (k + 1);
            }
            {
                float &prev_avg = org_map["generation"];
                prev_avg =
                    prev_avg + (organism->generation() - prev_avg) / (k + 1);
            }
            {
                float &prev_avg = org_map["age"];
                prev_avg = prev_avg + (organism->age() - prev_avg) / (k + 1);
            }
            {
                float &prev_avg = org_map["X"];
                prev_avg = prev_avg + (organism->X() - prev_avg) / (k + 1);
            }
            {
                float &prev_avg = org_map["Y"];
                prev_avg = prev_avg + (organism->Y() - prev_avg) / (k + 1);
            }
            {
                float &prev_avg = org_map["max_appetite_at_age"];
                prev_avg =
                    prev_avg +
                    (organism->max_appetite_at_age() - prev_avg) / (k + 1);
            }
            {
                float &prev_avg = org_map["max_speed_at_age"];
                prev_avg = prev_avg +
                           (organism->max_speed_at_age() - prev_avg) / (k + 1);
            }
            {
                float &prev_avg = org_map["max_stamina_at_age"];
                prev_avg =
                    prev_avg +
                    (organism->max_stamina_at_age() - prev_avg) / (k + 1);
            }
            {
                float &prev_avg = org_map["max_vitality_at_age"];
                prev_avg =
                    prev_avg +
                    (organism->max_vitality_at_age() - prev_avg) / (k + 1);
            }
            {
                float &prev_avg = org_map["sleep_restore_factor"];
                prev_avg =
                    prev_avg +
                    (organism->sleep_restore_factor() - prev_avg) / (k + 1);
            }

            k++;
        }

        new_stdvecOrganisms.push_back(Ecosystem::CreateOrganism(
            new_builder, kind, kingdom, chromosome_number, chromosome_structure,
            food_chain_rank, sexuality, org_map["age_fitness_on_death_ratio"],
            org_map["conceiving_probability"], org_map["mating_probability"],
            static_cast<uint32_t>(org_map["mating_age_start"]),
            static_cast<uint32_t>(org_map["mating_age_end"]),
            static_cast<uint32_t>(org_map["max_age"]),
            org_map["mutation_probability"], org_map["offsprings_factor"],
            org_map["height_on_speed"], org_map["height_on_stamina"],
            org_map["height_on_vitality"], org_map["weight_on_speed"],
            org_map["weight_on_stamina"], org_map["weight_on_vitality"],
            org_map["vitality_on_appetite"], org_map["vitality_on_speed"],
            org_map["stamina_on_appetite"], org_map["stamina_on_speed"],
            org_map["theoretical_maximum_base_appetite"],
            org_map["theoretical_maximum_base_height"],
            org_map["theoretical_maximum_base_speed"],
            org_map["theoretical_maximum_base_stamina"],
            org_map["theoretical_maximum_base_vitality"],
            org_map["theoretical_maximum_base_weight"],
            org_map["theoretical_maximum_height"],
            org_map["theoretical_maximum_speed"],
            org_map["theoretical_maximum_weight"],
            org_map["theoretical_maximum_height_multiplier"],
            org_map["theoretical_maximum_speed_multiplier"],
            org_map["theoretical_maximum_stamina_multiplier"],
            org_map["theoretical_maximum_vitality_multiplier"],
            org_map["theoretical_maximum_weight_multiplier"], name, chromosome,
            gender, static_cast<uint32_t>(org_map["generation"]),
            org_map["immunity"], org_map["base_appetite"],
            org_map["base_height"], org_map["base_speed"],
            org_map["base_stamina"], org_map["base_vitality"],
            org_map["base_weight"], org_map["height_multiplier"],
            org_map["speed_multiplier"], org_map["stamina_multiplier"],
            org_map["vitality_multiplier"], org_map["weight_multiplier"],
            org_map["max_height"], org_map["max_weight"],
            static_cast<uint32_t>(org_map["age"]), org_map["height"],
            org_map["weight"], org_map["static_fitness"],
            org_map["max_appetite_at_age"], org_map["max_speed_at_age"],
            org_map["max_stamina_at_age"], org_map["max_vitality_at_age"],
            org_map["vitality"], org_map["stamina"], org_map["appetite"],
            org_map["speed"], static_cast<uint32_t>(org_map["X"]),
            static_cast<uint32_t>(org_map["Y"]), org_map["dynamic_fitness"],
            org_map["vision_radius"], org_map["sleep_restore_factor"], asleep,
            monitor));

        new_stdvecSpecies.push_back(Ecosystem::CreateSpecies(
            new_builder, kind, kingdom,
            new_builder.CreateVectorOfSortedTables(
                new_stdvecOrganisms.data(), new_stdvecOrganisms.size())));
    }

    auto new_world_species = new_builder.CreateVectorOfSortedTables(
        new_stdvecSpecies.data(), new_stdvecSpecies.size());

    Ecosystem::WorldBuilder new_world_builder(new_builder);

    new_world_builder.add_species(new_world_species);
    new_world_builder.add_year(world_pointer->year());
    new_builder.Finish(new_world_builder.Finish());

    FBuffer new_buffer = new_builder.Release();

    new_builder.Clear();
    return new_buffer;
}
};  // namespace stat_fetcher

std::vector<float> get_value_vector(
    const std::vector<std::unordered_map<std::string, float>> &worlds,
    const std::string &attribute) {
    std::vector<float> values;
    values.reserve(worlds.size());
    for (const auto &world : worlds) {
        auto it = world.find(attribute);
        if (it == world.end()) throw std::runtime_error("Invalid attribute");
        values.push_back(it->second);
    }
    return values;
}
