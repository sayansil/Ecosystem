#include <god.hpp>
#include <set>
#include <species_constants.hpp>
#include <helper.hpp>
#include <fmt/core.h>
#include <flatbuffers/minireflect.h>
#include <nlohmann/json.hpp>

static flatbuffers::Offset<Ecosystem::Organism> createOrganism(
    flatbuffers::FlatBufferBuilder &builder,
    Ecosystem::OrganismBuilder &organism_builder,
    std::unordered_map<std::string, std::string> attributes);

static std::string getValueAsStr(const nlohmann::json &attributes, const std::string &key)
{
    return attributes.find(key) != attributes.end() ? attributes[key].get<std::string>() : "";
}

static uint64_t getValueAsUlong(const nlohmann::json &attributes, const std::string &key)
{
    return attributes.find(key) != attributes.end() ? attributes[key].get<uint64_t>() : 0;
}

static float getValueAsFloat(const nlohmann::json &attributes, const std::string &key)
{
    return attributes.find(key) != attributes.end() ? attributes[key].get<float>() : 0.0;
}

static uint8_t getValueAsByte(const nlohmann::json &attributes, const std::string &key)
{
    return attributes.find(key) != attributes.end() ? attributes[key].get<uint8_t>() : 0;
}

static uint16_t getValueAsUshort(const nlohmann::json &attributes, const std::string &key)
{
    return attributes.find(key) != attributes.end() ? attributes[key].get<uint16_t>() : 0;
}

God::God()
{
    constants::init();
    fmt::print("God created!\n");
}

God::~God()
{
    fmt::print("God is dead...\n");
}

void God::createWorld(std::vector<std::unordered_map<std::string, std::string>> &organisms)
{
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

        for (auto &organism : organisms)
        {
            if (organism.find("kind")->second == kind)
            {
                stdvecOrganisms.push_back(createOrganism(
                    builder,
                    organism_builder,
                    organism["kind"],
                    organism["kingdom"],
                    std::stoul(organism["age"]),
                    organism.find("monitor") != organism.end() ? std::stoi(organism["monitor"]) : 0));
            }
        }

        species_builder.add_kingdom((Ecosystem::KingdomE)std::stoi(kingdom));
        species_builder.add_kind(builder.CreateString(kind.c_str()));
        species_builder.add_organism(builder.CreateVectorOfSortedTables(stdvecOrganisms.data(), stdvecOrganisms.size()));

        stdvecSpecies.push_back(species_builder.Finish());
    }

    flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Ecosystem::Species>>> species_vec = builder.CreateVectorOfSortedTables(stdvecSpecies.data(), stdvecSpecies.size());
    world_builder.add_species(species_vec);
    world_builder.add_year(year);
    builder.Finish(world_builder.Finish());
    buffer = builder.Release();
    builder.Clear();
}

void God::displayWorldMetadata()
{
    fmt::print("\nBuffer Size: {} bytes \n", buffer.size());

    flatbuffers::ToStringVisitor visitor("", true, "", true);
    flatbuffers::IterateFlatBuffer(buffer.data(), Ecosystem::WorldTypeTable(), &visitor);
    nlohmann::json json_data = nlohmann::json::parse(visitor.s);
    fmt::print("Parsed JSON:\n{}\n", json_data.dump(4));
}

flatbuffers::Offset<Ecosystem::Organism> God::createOrganism(
    flatbuffers::FlatBufferBuilder &builder,
    Ecosystem::OrganismBuilder &organism_builder,
    const std::string &kind,
    const std::string &kingdom,
    const uint64_t &age,
    const std::string &name,
    const std::string &chromosome,
    const uint64_t &generation,
    const std::pair<uint64_t, uint64_t> &XY,
    const int8_t &monitor)
{

    // Assign base stats of species
    std::vector<flatbuffers::Offset<Ecosystem::ChromosomeStrand>> stdvecCStrand;
    std::map<std::string, std::map<std::string, int>> cStructure = constants::species_constants_map[kind]["chromosome_structure"];
    for (auto &cStrand : cStructure)
    {
        Ecosystem::ChromosomeStrandBuilder cStrandBuilder(builder);
        cStrandBuilder.add_code(builder.CreateString(cStrand.first.c_str()));
        cStrandBuilder.add_start(cStrand.second["start"]);
        cStrandBuilder.add_length(cStrand.second["length"]);
        stdvecCStrand.push_back(cStrandBuilder.Finish());
    }

    organism_builder.add_chromosome_structure(builder.CreateVectorOfSortedTables(stdvecCStrand.data(), stdvecCStrand.size()));
    organism_builder.add_chromosome_number(getValueAsUshort(constants::species_constants_map[kind], "species_chromosome_number"));
    organism_builder.add_mating_age_start(getValueAsUlong(constants::species_constants_map[kind], "mating_age_start"));
    organism_builder.add_mating_age_end(getValueAsUlong(constants::species_constants_map[kind], "mating_age_end"));
    organism_builder.add_max_age(getValueAsUlong(constants::species_constants_map[kind], "species_max_age"));
    organism_builder.add_offsprings_factor(getValueAsFloat(constants::species_constants_map[kind], "offsprings_factor"));
    organism_builder.add_is_asexual((Ecosystem::Reproduction)getValueAsByte(constants::species_constants_map[kind], "is_asexual"));
    organism_builder.add_mutation_probability(getValueAsFloat(constants::species_constants_map[kind], "mutation_probability"));
    organism_builder.add_conceiving_probability(getValueAsFloat(constants::species_constants_map[kind], "conceiving_probability"));
    organism_builder.add_mating_probability(getValueAsFloat(constants::species_constants_map[kind], "mating_probability"));
    organism_builder.add_age_fitness_on_death_ratio(getValueAsFloat(constants::species_constants_map[kind], "species_age_fitness_on_death_ratio"));
    organism_builder.add_height_on_speed(getValueAsFloat(constants::species_constants_map[kind], "species_height_on_speed"));
    organism_builder.add_height_on_stamina(getValueAsFloat(constants::species_constants_map[kind], "species_height_on_stamina"));
    organism_builder.add_height_on_vitality(getValueAsFloat(constants::species_constants_map[kind], "species_height_on_vitality"));
    organism_builder.add_weight_on_speed(getValueAsFloat(constants::species_constants_map[kind], "species_weight_on_speed"));
    organism_builder.add_weight_on_stamina(getValueAsFloat(constants::species_constants_map[kind], "species_weight_on_stamina"));
    organism_builder.add_weight_on_vitality(getValueAsFloat(constants::species_constants_map[kind], "species_weight_on_vitality"));
    organism_builder.add_vitality_on_appetite(getValueAsFloat(constants::species_constants_map[kind], "species_vitality_on_appetite"));
    organism_builder.add_vitality_on_speed(getValueAsFloat(constants::species_constants_map[kind], "species_vitality_on_speed"));
    organism_builder.add_stamina_on_appetite(getValueAsFloat(constants::species_constants_map[kind], "species_stamina_on_appetite"));
    organism_builder.add_stamina_on_speed(getValueAsFloat(constants::species_constants_map[kind], "species_stamina_on_speed"));
    organism_builder.add_theoretical_maximum_base_appetite(getValueAsFloat(constants::species_constants_map[kind], "species_theoretical_maximum_base_appetite"));
    organism_builder.add_theoretical_maximum_base_height(getValueAsFloat(constants::species_constants_map[kind], "species_theoretical_maximum_base_height"));
    organism_builder.add_theoretical_maximum_base_speed(getValueAsFloat(constants::species_constants_map[kind], "species_theoretical_maximum_base_speed"));
    organism_builder.add_theoretical_maximum_base_stamina(getValueAsFloat(constants::species_constants_map[kind], "species_theoretical_maximum_base_stamina"));
    organism_builder.add_theoretical_maximum_base_vitality(getValueAsFloat(constants::species_constants_map[kind], "species_theoretical_maximum_base_vitality"));
    organism_builder.add_theoretical_maximum_base_weight(getValueAsFloat(constants::species_constants_map[kind], "species_theoretical_maximum_base_weight"));
    organism_builder.add_theoretical_maximum_height_multiplier(getValueAsFloat(constants::species_constants_map[kind], "species_theoretical_maximum_height_multiplier"));
    organism_builder.add_theoretical_maximum_speed_multiplier(getValueAsFloat(constants::species_constants_map[kind], "species_theoretical_maximum_speed_multiplier"));
    organism_builder.add_theoretical_maximum_stamina_multiplier(getValueAsFloat(constants::species_constants_map[kind], "species_theoretical_maximum_stamina_multiplier"));
    organism_builder.add_theoretical_maximum_vitality_multiplier(getValueAsFloat(constants::species_constants_map[kind], "species_theoretical_maximum_vitality_multiplier"));
    organism_builder.add_theoretical_maximum_weight_multiplier(getValueAsFloat(constants::species_constants_map[kind], "species_theoretical_maximum_weight_multiplier"));
    organism_builder.add_theoretical_maximum_height(getValueAsFloat(constants::species_constants_map[kind], "species_theoretical_maximum_height"));
    organism_builder.add_theoretical_maximum_speed(getValueAsFloat(constants::species_constants_map[kind], "species_theoretical_maximum_speed"));
    organism_builder.add_theoretical_maximum_weight(getValueAsFloat(constants::species_constants_map[kind], "species_theoretical_maximum_weight"));
    organism_builder.add_sleep_restore_factor(getValueAsFloat(constants::species_constants_map[kind], "species_sleep_restore_factor"));
    organism_builder.add_food_chain_rank(getValueAsUlong(constants::species_constants_map[kind], "food_chain_rank"));
    organism_builder.add_vision_radius(getValueAsFloat(constants::species_constants_map[kind], "vision_radius"));

    // Assign passed default attributes
    organism_builder.add_kind(builder.CreateString(kind.c_str()));
    organism_builder.add_kingdom((Ecosystem::KingdomE)std::stoi(kingdom));
    organism_builder.add_generation(generation);
    organism_builder.add_age(age);
    organism_builder.add_monitor((Ecosystem::Monitor)monitor);

    std::string tmp_str;

    tmp_str = tmp_str.length() != 0 ? name : fmt::format("{}-orphan-{}", kind, helper::random_name(16));
    organism_builder.add_name(builder.CreateString(tmp_str.c_str()));
    tmp_str.clear();

    tmp_str = tmp_str.length() != 0 ? chromosome : helper::random_binary((int)getValueAsUlong(constants::species_constants_map[kind], "species_chromosome_number"));
    std::vector<uint8_t> chromosome_vec = helper::string_to_bytevector(tmp_str);
    tmp_str.clear();
    organism_builder.add_chromosome(builder.CreateVector(chromosome_vec.data(), chromosome_vec.size()));

    organism_builder.add_X(XY.first);
    organism_builder.add_Y(XY.second);
    organism_builder.add_asleep(Ecosystem::Sleep::Awake);

    // TODO Assign dynamic values
    //this->vitality = this->max_vitality_at_age;
    //this->stamina = this->max_stamina_at_age;
    //this->appetite = this->max_appetite_at_age;
    //this->speed = this->max_speed_at_age;

    //TODO increment_age();
    //TODO evaluate_static_fitness();
    //TODO evaluate_dynamic_fitness();

    return organism_builder.Finish();
}
    
flatbuffers::Offset<Ecosystem::Organism> God::createOrganism(
        flatbuffers::FlatBufferBuilder &builder,
        Ecosystem::OrganismBuilder &organism_builder,
        const std::string &kind,
        const std::string &kingdom,
        const uint64_t &age,
        const int8_t &monitor)
{
    return createOrganism(builder, organism_builder, kind, kingdom, age, "", "", 0, helper::random_location(), monitor);
}
