#include <god.hpp>
#include <set>
#include <fmt/core.h>
#include <flatbuffers/minireflect.h>
#include <nlohmann/json.hpp>

static flatbuffers::Offset<Ecosystem::Organism> createOrganism(
    flatbuffers::FlatBufferBuilder &builder,
    Ecosystem::OrganismBuilder &organism_builder,
    std::unordered_map<std::string, std::string> attributes);

static std::string getValueAsStr(const std::unordered_map<std::string, std::string> &attributes, const std::string &key)
{
    auto it = attributes.find(key);
    return it != attributes.end() ? it->second : "";
}

static uint64_t getValueAsUlong(const std::unordered_map<std::string, std::string> &attributes, const std::string &key)
{
    auto it = attributes.find(key);
    return it != attributes.end() ? std::stoul(it->second) : 0;
}

static double getValueAsDouble(const std::unordered_map<std::string, std::string> &attributes, const std::string &key)
{
    auto it = attributes.find(key);
    return it != attributes.end() ? std::stod(it->second) : 0.0;
}

static int8_t getValueAsByte(const std::unordered_map<std::string, std::string> &attributes, const std::string &key)
{
    auto it = attributes.find(key);
    return it != attributes.end() ? (int8_t)std::stoi(it->second) : 0;
}

God::God()
{
    fmt::print("God created!\n");
}

God::~God()
{
    fmt::print("God is dead...\n");
}

void God::createWorld(const std::vector<std::unordered_map<std::string, std::string>> &organisms)
{
    flatbuffers::FlatBufferBuilder builder;
    Ecosystem::WorldBuilder world_builder(builder);

    std::vector<flatbuffers::Offset<Ecosystem::Species>> stdvecSpecies;
    Ecosystem::SpeciesBuilder species_builder(builder);

    std::set<std::pair<std::string, std::string>> unique_species;
    for (const auto &organism : organisms)
    {
        unique_species.insert({organism.find("kingdom")->second, organism.find("kind")->second});
    }

    for (const auto &[kingdom, kind] : unique_species)
    {
        std::vector<flatbuffers::Offset<Ecosystem::Organism>> stdvecOrganisms;
        Ecosystem::OrganismBuilder organism_builder(builder);

        for (const auto &organism : organisms)
        {
            if (organism.find("kind")->second == kind)
            {
                stdvecOrganisms.push_back(createOrganism(builder, organism_builder, organism));
            }
        }

        species_builder.add_kingdom(builder.CreateString(kingdom.c_str()));
        species_builder.add_kind(builder.CreateString(kind.c_str()));
        species_builder.add_organism(builder.CreateVectorOfSortedTables(stdvecOrganisms.data(), stdvecOrganisms.size()));

        stdvecSpecies.push_back(species_builder.Finish());
    }

    flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Ecosystem::Species>>> species_vec = builder.CreateVectorOfSortedTables(stdvecSpecies.data(), stdvecSpecies.size());
    world_builder.add_species(species_vec);
    world_builder.add_year(builder.CreateString(std::to_string(year).c_str()));
    builder.Finish(world_builder.Finish());
}

void God::displayWorldMetadata()
{
    fmt::print("\nBuffer Size: {} bytes \n", builder.GetSize());

    flatbuffers::ToStringVisitor visitor("", true, "", true);
    flatbuffers::IterateFlatBuffer(builder.GetBufferPointer(), Ecosystem::WorldTypeTable(), &visitor);
    nlohmann::json json_data = nlohmann::json::parse(visitor.s);
    fmt::print("Parsed JSON:\n{}\n", json_data.dump(4));
}

flatbuffers::Offset<Ecosystem::Organism> createOrganism(
    flatbuffers::FlatBufferBuilder &builder,
    Ecosystem::OrganismBuilder &organism_builder,
    std::unordered_map<std::string, std::string> attributes)
{
    organism_builder.add_kind(builder.CreateString(getValueAsStr(attributes, "kind").c_str()));
    organism_builder.add_kingdom(builder.CreateString(getValueAsStr(attributes, "kingdom").c_str()));
    organism_builder.add_chromosome_number(getValueAsUlong(attributes, "chromosome_number"));
    organism_builder.add_food_chain_rank(getValueAsUlong(attributes, "food_chain_rank"));
    organism_builder.add_is_asexual(Ecosystem::Reproduction(getValueAsByte(attributes, "is_asexual")));
    organism_builder.add_age_fitness_on_death_ratio(getValueAsDouble(attributes, "age_fitness_on_death_ratio"));
    organism_builder.add_conceiving_probability(getValueAsDouble(attributes, "conceiving_probability"));
    organism_builder.add_mating_probability(getValueAsDouble(attributes, "mating_probability"));
    organism_builder.add_mating_age_start(getValueAsDouble(attributes, "mating_age_start"));
    organism_builder.add_mating_age_end(getValueAsDouble(attributes, "mating_age_end"));
    organism_builder.add_max_age(getValueAsDouble(attributes, "max_age"));
    organism_builder.add_mutation_probability(getValueAsDouble(attributes, "mutation_probability"));
    organism_builder.add_offsprings_factor(getValueAsDouble(attributes, "offsprings_factor"));
    organism_builder.add_height_on_speed(getValueAsDouble(attributes, "height_on_speed"));
    organism_builder.add_height_on_stamina(getValueAsDouble(attributes, "height_on_stamina"));
    organism_builder.add_height_on_vitality(getValueAsDouble(attributes, "height_on_vitality"));
    organism_builder.add_weight_on_speed(getValueAsDouble(attributes, "weight_on_speed"));
    organism_builder.add_weight_on_stamina(getValueAsDouble(attributes, "weight_on_stamina"));
    organism_builder.add_weight_on_vitality(getValueAsDouble(attributes, "weight_on_vitality"));
    organism_builder.add_vitality_on_appetite(getValueAsDouble(attributes, "vitality_on_appetite"));
    organism_builder.add_vitality_on_speed(getValueAsDouble(attributes, "vitality_on_speed"));
    organism_builder.add_stamina_on_appetite(getValueAsDouble(attributes, "stamina_on_appetite"));
    organism_builder.add_stamina_on_speed(getValueAsDouble(attributes, "stamina_on_speed"));
    organism_builder.add_theoretical_maximum_base_appetite(getValueAsDouble(attributes, "theoretical_maximum_base_appetite"));
    organism_builder.add_theoretical_maximum_base_height(getValueAsDouble(attributes, "theoretical_maximum_base_height"));
    organism_builder.add_theoretical_maximum_base_speed(getValueAsDouble(attributes, "theoretical_maximum_base_speed"));
    organism_builder.add_theoretical_maximum_base_stamina(getValueAsDouble(attributes, "theoretical_maximum_base_stamina"));
    organism_builder.add_theoretical_maheximum_base_vitality(getValueAsDouble(attributes, "theoretical_maheximum_base_vitality"));
    organism_builder.add_theoretical_maximum_base_weight(getValueAsDouble(attributes, "theoretical_maximum_base_weight"));
    organism_builder.add_theoretical_maximum_height(getValueAsDouble(attributes, "theoretical_maximum_height"));
    organism_builder.add_theoretical_maximum_speed(getValueAsDouble(attributes, "theoretical_maximum_speed"));
    organism_builder.add_theoretical_maximum_weight(getValueAsDouble(attributes, "theoretical_maximum_weight"));
    organism_builder.add_theoretical_maximum_height_multiplier(getValueAsDouble(attributes, "theoretical_maximum_height_multiplier"));
    organism_builder.add_theoretical_maximum_speed_multiplier(getValueAsDouble(attributes, "theoretical_maximum_speed_multiplier"));
    organism_builder.add_theoretical_maximum_stamina_multiplier(getValueAsDouble(attributes, "theoretical_maximum_stamina_multiplier"));
    organism_builder.add_theoretical_maximum_vitality_multiplier(getValueAsDouble(attributes, "theoretical_maximum_vitality_multiplier"));
    organism_builder.add_theoretical_maximum_weight_multiplier(getValueAsDouble(attributes, "theoretical_maximum_weight_multiplier"));
    organism_builder.add_name(builder.CreateString(getValueAsStr(attributes, "name").c_str()));
    organism_builder.add_chromosome(builder.CreateString(getValueAsStr(attributes, "chromosome").c_str()));
    organism_builder.add_gender(Ecosystem::Gender(getValueAsByte(attributes, "gender")));
    organism_builder.add_generation(getValueAsUlong(attributes, "generation"));
    organism_builder.add_immunity(getValueAsDouble(attributes, "immunity"));
    organism_builder.add_base_appetite(getValueAsDouble(attributes, "base_appetite"));
    organism_builder.add_base_height(getValueAsDouble(attributes, "base_height"));
    organism_builder.add_base_speed(getValueAsDouble(attributes, "base_speed"));
    organism_builder.add_base_stamina(getValueAsDouble(attributes, "base_stamina"));
    organism_builder.add_base_vitality(getValueAsDouble(attributes, "base_vitality"));
    organism_builder.add_base_weight(getValueAsDouble(attributes, "base_weight"));
    organism_builder.add_height_multiplier(getValueAsDouble(attributes, "height_multiplier"));
    organism_builder.add_speed_multiplier(getValueAsDouble(attributes, "speed_multiplier"));
    organism_builder.add_stamina_multiplier(getValueAsDouble(attributes, "stamina_multiplier"));
    organism_builder.add_vitality_multiplier(getValueAsDouble(attributes, "vitality_multiplier"));
    organism_builder.add_weight_multiplier(getValueAsDouble(attributes, "weight_multiplier"));
    organism_builder.add_max_height(getValueAsDouble(attributes, "max_height"));
    organism_builder.add_max_weight(getValueAsDouble(attributes, "max_weight"));
    organism_builder.add_age(getValueAsUlong(attributes, "age"));
    organism_builder.add_height(getValueAsDouble(attributes, "height"));
    organism_builder.add_weight(getValueAsDouble(attributes, "weight"));
    organism_builder.add_age_death_factor(getValueAsDouble(attributes, "age_death_factor"));
    organism_builder.add_fitness_death_factor(getValueAsDouble(attributes, "fitness_death_factor"));
    organism_builder.add_death_factor(getValueAsDouble(attributes, "death_factor"));
    organism_builder.add_static_fitness(getValueAsDouble(attributes, "static_fitness"));
    organism_builder.add_max_appetite_at_age(getValueAsDouble(attributes, "max_appetite_at_age"));
    organism_builder.add_max_speed_at_age(getValueAsDouble(attributes, "max_speed_at_age"));
    organism_builder.add_max_stamina_at_age(getValueAsDouble(attributes, "max_stamina_at_age"));
    organism_builder.add_max_vitality_at_age(getValueAsDouble(attributes, "max_vitality_at_age"));
    organism_builder.add_vision_radius(getValueAsDouble(attributes, "vision_radius"));
    organism_builder.add_sleep_restore_factor(getValueAsDouble(attributes, "sleep_restore_factor"));

    return organism_builder.Finish();
}