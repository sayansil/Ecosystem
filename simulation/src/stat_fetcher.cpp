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

struct OrganismAvg {
    float offsprings_factor = 0;
    float mutation_probability = 0;
    float conceiving_probability = 0;
    float mating_probability = 0;
    float age_fitness_on_death_ratio = 0;
    float height_on_speed = 0;
    float height_on_stamina = 0;
    float height_on_vitality = 0;
    float weight_on_speed = 0;
    float weight_on_stamina = 0;
    float weight_on_vitality = 0;
    float vitality_on_appetite = 0;
    float vitality_on_speed = 0;
    float stamina_on_appetite = 0;
    float stamina_on_speed = 0;
    float theoretical_maximum_base_appetite = 0;
    float theoretical_maximum_base_height = 0;
    float theoretical_maximum_base_speed = 0;
    float theoretical_maximum_base_stamina = 0;
    float theoretical_maximum_base_vitality = 0;
    float theoretical_maximum_base_weight = 0;
    float theoretical_maximum_height_multiplier = 0;
    float theoretical_maximum_speed_multiplier = 0;
    float theoretical_maximum_stamina_multiplier = 0;
    float theoretical_maximum_vitality_multiplier = 0;
    float theoretical_maximum_weight_multiplier = 0;
    float theoretical_maximum_height = 0;
    float theoretical_maximum_speed = 0;
    float theoretical_maximum_weight = 0;
    float sleep_restore_factor = 0;
    float vision_radius = 0;
    float immunity = 0;
    float base_appetite = 0;
    float base_speed = 0;
    float base_stamina = 0;
    float base_vitality = 0;
    float base_weight = 0;
    float base_height = 0;
    float weight = 0;
    float height = 0;
    float max_weight = 0;
    float max_height = 0;
    float weight_multiplier = 0;
    float height_multiplier = 0;
    float speed_multiplier = 0;
    float stamina_multiplier = 0;
    float vitality_multiplier = 0;
    float vitality = 0;
    float stamina = 0;
    float appetite = 0;
    float speed = 0;
    float static_fitness = 0;
    float dynamic_fitness = 0;
    float mating_age_start = 0;
    float mating_age_end = 0;
    float max_age = 0;
    float generation = 0;
    float age = 0;
    float X = 0;
    float Y = 0;
    float max_appetite_at_age = 0;
    float max_speed_at_age = 0;
    float max_stamina_at_age = 0;
    float max_vitality_at_age = 0;
};

static inline void update_avg(float &avg, float val, int k) {
    avg = avg + (val - avg) / (k + 1);
}

FBuffer create_avg_world(const flatbuffers::DetachedBuffer &world_buffer) {
    const Ecosystem::World *world_pointer =
        Ecosystem::GetWorld(world_buffer.data());

    flatbuffers::FlatBufferBuilder new_builder;
    std::vector<flatbuffers::Offset<Ecosystem::Species>> new_stdvecSpecies;

    for (const Ecosystem::Species *species : *(world_pointer->species())) {
        if (species->organism()->size() == 0) {
            continue;
        }

        std::vector<flatbuffers::Offset<Ecosystem::Organism>>
            new_stdvecOrganisms;

        OrganismAvg avg;
        int k = 0;

        auto chromosome_number =
            species->organism()->Get(0)->chromosome_number();
        auto sexuality = species->organism()->Get(0)->sexuality();
        auto food_chain_rank = species->organism()->Get(0)->food_chain_rank();
        auto kind =
            new_builder.CreateString(species->organism()->Get(0)->kind());
        auto kingdom = species->organism()->Get(0)->kingdom();
        auto name = new_builder.CreateString(
            species->organism()->Get(0)->kind()->str() + "_avg_organism");

        auto gender = species->organism()->Get(0)->gender();
        auto asleep = species->organism()->Get(0)->asleep();
        auto monitor = species->organism()->Get(0)->monitor();

        std::vector<uint8_t> chromosome_vec;
        auto chromosome = new_builder.CreateVector(chromosome_vec.data(),
                                                   chromosome_vec.size());

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
            update_avg(avg.offsprings_factor, organism->offsprings_factor(), k);
            update_avg(avg.mutation_probability, organism->mutation_probability(), k);
            update_avg(avg.conceiving_probability, organism->conceiving_probability(), k);
            update_avg(avg.mating_probability, organism->mating_probability(), k);
            update_avg(avg.age_fitness_on_death_ratio, organism->age_fitness_on_death_ratio(), k);
            update_avg(avg.height_on_speed, organism->height_on_speed(), k);
            update_avg(avg.height_on_stamina, organism->height_on_stamina(), k);
            update_avg(avg.height_on_vitality, organism->height_on_vitality(), k);
            update_avg(avg.weight_on_speed, organism->weight_on_speed(), k);
            update_avg(avg.weight_on_stamina, organism->weight_on_stamina(), k);
            update_avg(avg.weight_on_vitality, organism->weight_on_vitality(), k);
            update_avg(avg.vitality_on_appetite, organism->vitality_on_appetite(), k);
            update_avg(avg.vitality_on_speed, organism->vitality_on_speed(), k);
            update_avg(avg.stamina_on_appetite, organism->stamina_on_appetite(), k);
            update_avg(avg.stamina_on_speed, organism->stamina_on_speed(), k);
            update_avg(avg.theoretical_maximum_base_appetite, organism->theoretical_maximum_base_appetite(), k);
            update_avg(avg.theoretical_maximum_base_height, organism->theoretical_maximum_base_height(), k);
            update_avg(avg.theoretical_maximum_base_speed, organism->theoretical_maximum_base_speed(), k);
            update_avg(avg.theoretical_maximum_base_stamina, organism->theoretical_maximum_base_stamina(), k);
            update_avg(avg.theoretical_maximum_base_vitality, organism->theoretical_maximum_base_vitality(), k);
            update_avg(avg.theoretical_maximum_base_weight, organism->theoretical_maximum_base_weight(), k);
            update_avg(avg.theoretical_maximum_height_multiplier, organism->theoretical_maximum_height_multiplier(), k);
            update_avg(avg.theoretical_maximum_speed_multiplier, organism->theoretical_maximum_speed_multiplier(), k);
            update_avg(avg.theoretical_maximum_stamina_multiplier, organism->theoretical_maximum_stamina_multiplier(), k);
            update_avg(avg.theoretical_maximum_vitality_multiplier, organism->theoretical_maximum_vitality_multiplier(), k);
            update_avg(avg.theoretical_maximum_weight_multiplier, organism->theoretical_maximum_weight_multiplier(), k);
            update_avg(avg.theoretical_maximum_height, organism->theoretical_maximum_height(), k);
            update_avg(avg.theoretical_maximum_speed, organism->theoretical_maximum_speed(), k);
            update_avg(avg.theoretical_maximum_weight, organism->theoretical_maximum_weight(), k);
            update_avg(avg.sleep_restore_factor, organism->sleep_restore_factor(), k);
            update_avg(avg.vision_radius, organism->vision_radius(), k);
            update_avg(avg.immunity, organism->immunity(), k);
            update_avg(avg.base_appetite, organism->base_appetite(), k);
            update_avg(avg.base_speed, organism->base_speed(), k);
            update_avg(avg.base_stamina, organism->base_stamina(), k);
            update_avg(avg.base_vitality, organism->base_vitality(), k);
            update_avg(avg.base_weight, organism->base_weight(), k);
            update_avg(avg.base_height, organism->base_height(), k);
            update_avg(avg.weight, organism->weight(), k);
            update_avg(avg.height, organism->height(), k);
            update_avg(avg.max_weight, organism->max_weight(), k);
            update_avg(avg.max_height, organism->max_height(), k);
            update_avg(avg.weight_multiplier, organism->weight_multiplier(), k);
            update_avg(avg.height_multiplier, organism->height_multiplier(), k);
            update_avg(avg.speed_multiplier, organism->speed_multiplier(), k);
            update_avg(avg.stamina_multiplier, organism->stamina_multiplier(), k);
            update_avg(avg.vitality_multiplier, organism->vitality_multiplier(), k);
            update_avg(avg.vitality, organism->vitality(), k);
            update_avg(avg.stamina, organism->stamina(), k);
            update_avg(avg.appetite, organism->appetite(), k);
            update_avg(avg.speed, organism->speed(), k);
            update_avg(avg.static_fitness, organism->static_fitness(), k);
            update_avg(avg.dynamic_fitness, organism->dynamic_fitness(), k);
            update_avg(avg.mating_age_start, static_cast<float>(organism->mating_age_start()), k);
            update_avg(avg.mating_age_end, static_cast<float>(organism->mating_age_end()), k);
            update_avg(avg.max_age, static_cast<float>(organism->max_age()), k);
            update_avg(avg.generation, static_cast<float>(organism->generation()), k);
            update_avg(avg.age, static_cast<float>(organism->age()), k);
            update_avg(avg.X, static_cast<float>(organism->X()), k);
            update_avg(avg.Y, static_cast<float>(organism->Y()), k);
            update_avg(avg.max_appetite_at_age, organism->max_appetite_at_age(), k);
            update_avg(avg.max_speed_at_age, organism->max_speed_at_age(), k);
            update_avg(avg.max_stamina_at_age, organism->max_stamina_at_age(), k);
            update_avg(avg.max_vitality_at_age, organism->max_vitality_at_age(), k);
            k++;
        }

        new_stdvecOrganisms.push_back(Ecosystem::CreateOrganism(
            new_builder, kind, kingdom, chromosome_number, chromosome_structure,
            food_chain_rank, sexuality, avg.age_fitness_on_death_ratio,
            avg.conceiving_probability, avg.mating_probability,
            static_cast<uint32_t>(avg.mating_age_start),
            static_cast<uint32_t>(avg.mating_age_end),
            static_cast<uint32_t>(avg.max_age),
            avg.mutation_probability, avg.offsprings_factor,
            avg.height_on_speed, avg.height_on_stamina,
            avg.height_on_vitality, avg.weight_on_speed,
            avg.weight_on_stamina, avg.weight_on_vitality,
            avg.vitality_on_appetite, avg.vitality_on_speed,
            avg.stamina_on_appetite, avg.stamina_on_speed,
            avg.theoretical_maximum_base_appetite,
            avg.theoretical_maximum_base_height,
            avg.theoretical_maximum_base_speed,
            avg.theoretical_maximum_base_stamina,
            avg.theoretical_maximum_base_vitality,
            avg.theoretical_maximum_base_weight,
            avg.theoretical_maximum_height,
            avg.theoretical_maximum_speed,
            avg.theoretical_maximum_weight,
            avg.theoretical_maximum_height_multiplier,
            avg.theoretical_maximum_speed_multiplier,
            avg.theoretical_maximum_stamina_multiplier,
            avg.theoretical_maximum_vitality_multiplier,
            avg.theoretical_maximum_weight_multiplier, name, chromosome,
            gender, static_cast<uint32_t>(avg.generation),
            avg.immunity, avg.base_appetite,
            avg.base_height, avg.base_speed,
            avg.base_stamina, avg.base_vitality,
            avg.base_weight, avg.height_multiplier,
            avg.speed_multiplier, avg.stamina_multiplier,
            avg.vitality_multiplier, avg.weight_multiplier,
            avg.max_height, avg.max_weight,
            static_cast<uint32_t>(avg.age), avg.height,
            avg.weight, avg.static_fitness,
            avg.max_appetite_at_age, avg.max_speed_at_age,
            avg.max_stamina_at_age, avg.max_vitality_at_age,
            avg.vitality, avg.stamina, avg.appetite,
            avg.speed, static_cast<uint32_t>(avg.X),
            static_cast<uint32_t>(avg.Y), avg.dynamic_fitness,
            avg.vision_radius, avg.sleep_restore_factor, asleep,
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

std::unordered_map<uint32_t, uint32_t> get_generation_count(
    const flatbuffers::DetachedBuffer &world_buffer) {
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
