#include <god.hpp>
#include <set>
#include <organism.hpp>
#include <species_constants.hpp>
#include <helper.hpp>
#include <fmt/core.h>
#include <flatbuffers/minireflect.h>
#include <nlohmann/json.hpp>
#include <profiler.hpp>

static double get_value_from_chromosome(const std::vector<uint8_t> &chromosome, const std::map<std::string, std::map<std::string, int>> &c_structure, const std::string &code, const double &multiplier, const uint16_t &chromosome_number);

static std::string getValueAsStr(const nlohmann::json &attributes, const std::string &key)
{
    return attributes.find(key) != attributes.end() ? attributes[key].get<std::string>() : "";
}

static uint64_t getValueAsUlong(const nlohmann::json &attributes, const std::string &key)
{
    return attributes.find(key) != attributes.end() ? attributes[key].get<uint64_t>() : 0;
}

static uint32_t getValueAsUint(const nlohmann::json &attributes, const std::string &key)
{
    return attributes.find(key) != attributes.end() ? attributes[key].get<uint32_t>() : 0;
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

static double get_value_from_chromosome(const std::vector<uint8_t> &chromosome, const std::map<std::string, std::map<std::string, int>> &c_structure, const std::string &code, const double &multiplier, const uint16_t &chromosome_number)
{
    auto it = c_structure.find(code);
    if (it == c_structure.end())
        return 0;
    std::string chromosome_str = helper::bytevector_to_string(chromosome.data(), chromosome.size(), chromosome_number);
    int start = it->second.find("start")->second;
    int len = it->second.find("length")->second;
    if (len == 0)
        return 0;
    return (helper::to_decimal(chromosome_str.substr(start, len)) / static_cast<double>(1 << len)) * multiplier;
}

static double updateStat(double base, double p_range)
{
    std::uniform_real_distribution<double> dis(0.0, p_range * 2);
    const double x = p_range - dis(helper::rng);

    return base * (1 + x);
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
    /* World creation begins */

    std::vector<flatbuffers::Offset<Ecosystem::Species>> stdvecSpecies;

    std::set<std::pair<std::string, std::string>> unique_species;
    for (const auto &organism : organisms)
    {
        unique_species.insert({organism.find("kingdom")->second, organism.find("kind")->second});
    }

    for (const auto &[kingdom, kind] : unique_species)
    {
        std::vector<flatbuffers::Offset<Ecosystem::Organism>> stdvecOrganisms;

        for (auto &organism : organisms)
        {
            if (organism.find("kind")->second == kind)
            {
                flatbuffers::Offset<Ecosystem::Organism> current_organism;
                bool spawn_killed = false;

                do
                {
                    // Retry for spawn-kill situations
                    current_organism = createOrganism(
                        builder,
                        organism["kind"],
                        organism["kingdom"],
                        std::stoul(organism["age"]),
                        organism.find("monitor") != organism.end() ? std::stoi(organism["monitor"]) : 0);
                    spawn_killed = !organism_opts::is_normal_child(helper::get_pointer_from_offset(builder, current_organism));
                } while (spawn_killed);

                stdvecOrganisms.push_back(current_organism);
            }
        }

        stdvecSpecies.push_back(Ecosystem::CreateSpecies(builder,
            builder.CreateString(kind.c_str()),
            (Ecosystem::KingdomE)std::stoi(kingdom),
            builder.CreateVectorOfSortedTables(stdvecOrganisms.data(), stdvecOrganisms.size())
        ));
    }

    auto world_species = builder.CreateVectorOfSortedTables(stdvecSpecies.data(), stdvecSpecies.size());

    /* World creation ends */

    Ecosystem::WorldBuilder world_builder(builder);

    world_builder.add_species(world_species);
    world_builder.add_year(year);
    builder.Finish(world_builder.Finish());
    buffer = builder.Release();
    builder.Clear();
}

void God::displayWorldMetadata()
{
    PROFILE_FUNCTION();
    fmt::print("\nBuffer Size: {} bytes \n", buffer.size());

    flatbuffers::ToStringVisitor visitor("", true, "", true);
    flatbuffers::IterateFlatBuffer(buffer.data(), Ecosystem::WorldTypeTable(), &visitor);
    nlohmann::json json_data = nlohmann::json::parse(visitor.s);
    fmt::print("Parsed JSON:\n{}\n", json_data.dump(4));
}

flatbuffers::Offset<Ecosystem::Organism> God::createOrganism(
    flatbuffers::FlatBufferBuilder &builder,
    const std::string &kind,
    const std::string &kingdom,
    const uint64_t &age,
    const std::string &name,
    const std::string &chromosome,
    const uint64_t &generation,
    const std::pair<uint64_t, uint64_t> &XY,
    const int8_t &monitor)
{

    PROFILE_FUNCTION();

    /* Organism creation begins */

    // Assign base stats of species
    std::vector<flatbuffers::Offset<Ecosystem::ChromosomeStrand>> stdvecCStrand;
    std::map<std::string, std::map<std::string, int>> c_structure = constants::species_constants_map[kind]["chromosome_structure"];
    for (auto &cStrand : c_structure)
    {
        stdvecCStrand.push_back(Ecosystem::CreateChromosomeStrand(builder,
            builder.CreateString(cStrand.first.c_str()),
            cStrand.second["start"],
            cStrand.second["length"]
        ));
    }

    std::string tmp_str;
    float tmp_flt;

    tmp_str = tmp_str.length() != 0 ? name : fmt::format("{}-orphan-{}", kind, helper::random_name(16));
    auto organism_name = builder.CreateString(tmp_str.c_str());
    tmp_str.clear();

    tmp_str = tmp_str.length() != 0 ? chromosome : helper::random_binary((int)getValueAsUlong(constants::species_constants_map[kind], "species_chromosome_number"));
    std::vector<uint8_t> chromosome_vec = helper::string_to_bytevector(tmp_str);
    tmp_str.clear();
    auto organism_chromosome = builder.CreateVector(chromosome_vec.data(), chromosome_vec.size());

    /* Organism creation ends */

/*

    // Assign passed default attributes
*/


/*

    // Assign attributes from chromosome
*/

    auto organism_offset = Ecosystem::CreateOrganism(builder,
        builder.CreateString(kind.c_str()),
        (Ecosystem::KingdomE)std::stoi(kingdom),
        getValueAsUshort(constants::species_constants_map[kind], "species_chromosome_number"),
        builder.CreateVectorOfSortedTables(stdvecCStrand.data(), stdvecCStrand.size()),
        getValueAsUlong(constants::species_constants_map[kind], "food_chain_rank"),
        (Ecosystem::Reproduction)getValueAsByte(constants::species_constants_map[kind], "sexuality"),
        getValueAsFloat(constants::species_constants_map[kind], "species_age_fitness_on_death_ratio"),
        getValueAsFloat(constants::species_constants_map[kind], "conceiving_probability"),
        getValueAsFloat(constants::species_constants_map[kind], "mating_probability"),
        getValueAsUint(constants::species_constants_map[kind], "mating_age_start"),
        getValueAsUint(constants::species_constants_map[kind], "mating_age_end"),
        getValueAsUint(constants::species_constants_map[kind], "species_max_age"),
        getValueAsFloat(constants::species_constants_map[kind], "mutation_probability"),
        getValueAsFloat(constants::species_constants_map[kind], "offsprings_factor"),
        getValueAsFloat(constants::species_constants_map[kind], "species_height_on_speed"),
        getValueAsFloat(constants::species_constants_map[kind], "species_height_on_stamina"),
        getValueAsFloat(constants::species_constants_map[kind], "species_height_on_vitality"),
        getValueAsFloat(constants::species_constants_map[kind], "species_weight_on_speed"),
        getValueAsFloat(constants::species_constants_map[kind], "species_weight_on_stamina"),
        getValueAsFloat(constants::species_constants_map[kind], "species_weight_on_vitality"),
        getValueAsFloat(constants::species_constants_map[kind], "species_vitality_on_appetite"),
        getValueAsFloat(constants::species_constants_map[kind], "species_vitality_on_speed"),
        getValueAsFloat(constants::species_constants_map[kind], "species_stamina_on_appetite"),
        getValueAsFloat(constants::species_constants_map[kind], "species_stamina_on_speed"),
        getValueAsFloat(constants::species_constants_map[kind], "species_theoretical_maximum_base_appetite"),
        getValueAsFloat(constants::species_constants_map[kind], "species_theoretical_maximum_base_height"),
        getValueAsFloat(constants::species_constants_map[kind], "species_theoretical_maximum_base_speed"),
        getValueAsFloat(constants::species_constants_map[kind], "species_theoretical_maximum_base_stamina"),
        getValueAsFloat(constants::species_constants_map[kind], "species_theoretical_maximum_base_vitality"),
        getValueAsFloat(constants::species_constants_map[kind], "species_theoretical_maximum_base_weight"),
        getValueAsFloat(constants::species_constants_map[kind], "species_theoretical_maximum_height"),
        getValueAsFloat(constants::species_constants_map[kind], "species_theoretical_maximum_speed"),
        getValueAsFloat(constants::species_constants_map[kind], "species_theoretical_maximum_weight"),
        getValueAsFloat(constants::species_constants_map[kind], "species_theoretical_maximum_height_multiplier"),
        getValueAsFloat(constants::species_constants_map[kind], "species_theoretical_maximum_speed_multiplier"),
        getValueAsFloat(constants::species_constants_map[kind], "species_theoretical_maximum_stamina_multiplier"),
        getValueAsFloat(constants::species_constants_map[kind], "species_theoretical_maximum_vitality_multiplier"),
        getValueAsFloat(constants::species_constants_map[kind], "species_theoretical_maximum_weight_multiplier"),
        organism_name,
        organism_chromosome,
        (Ecosystem::Gender)get_value_from_chromosome(chromosome_vec, c_structure, "gn", 2.0, (int)getValueAsUlong(constants::species_constants_map[kind], "species_chromosome_number")),
        generation,
        get_value_from_chromosome(chromosome_vec, c_structure, "im", 1.0, (int)getValueAsUlong(constants::species_constants_map[kind], "species_chromosome_number")),
        get_value_from_chromosome(chromosome_vec, c_structure, "ba", getValueAsFloat(constants::species_constants_map[kind], "species_theoretical_maximum_base_appetite"), (int)getValueAsUlong(constants::species_constants_map[kind], "species_chromosome_number")),
        get_value_from_chromosome(chromosome_vec, c_structure, "bh", getValueAsFloat(constants::species_constants_map[kind], "species_theoretical_maximum_base_height"), (int)getValueAsUlong(constants::species_constants_map[kind], "species_chromosome_number")),
        get_value_from_chromosome(chromosome_vec, c_structure, "bp", getValueAsFloat(constants::species_constants_map[kind], "species_theoretical_maximum_base_speed"), (int)getValueAsUlong(constants::species_constants_map[kind], "species_chromosome_number")),
        get_value_from_chromosome(chromosome_vec, c_structure, "bs", getValueAsFloat(constants::species_constants_map[kind], "species_theoretical_maximum_base_stamina"), (int)getValueAsUlong(constants::species_constants_map[kind], "species_chromosome_number")),
        get_value_from_chromosome(chromosome_vec, c_structure, "bv", getValueAsFloat(constants::species_constants_map[kind], "species_theoretical_maximum_base_vitality"), (int)getValueAsUlong(constants::species_constants_map[kind], "species_chromosome_number")),
        get_value_from_chromosome(chromosome_vec, c_structure, "bw", getValueAsFloat(constants::species_constants_map[kind], "species_theoretical_maximum_base_weight"), (int)getValueAsUlong(constants::species_constants_map[kind], "species_chromosome_number")),
        get_value_from_chromosome(chromosome_vec, c_structure, "hm", getValueAsFloat(constants::species_constants_map[kind], "species_theoretical_maximum_height_multiplier"), (int)getValueAsUlong(constants::species_constants_map[kind], "species_chromosome_number")),
        get_value_from_chromosome(chromosome_vec, c_structure, "pm", getValueAsFloat(constants::species_constants_map[kind], "species_theoretical_maximum_speed_multiplier"), (int)getValueAsUlong(constants::species_constants_map[kind], "species_chromosome_number")),
        get_value_from_chromosome(chromosome_vec, c_structure, "sm", getValueAsFloat(constants::species_constants_map[kind], "species_theoretical_maximum_stamina_multiplier"), (int)getValueAsUlong(constants::species_constants_map[kind], "species_chromosome_number")),
        get_value_from_chromosome(chromosome_vec, c_structure, "vm", getValueAsFloat(constants::species_constants_map[kind], "species_theoretical_maximum_vitality_multiplier"), (int)getValueAsUlong(constants::species_constants_map[kind], "species_chromosome_number")),
        get_value_from_chromosome(chromosome_vec, c_structure, "wm", getValueAsFloat(constants::species_constants_map[kind], "species_theoretical_maximum_weight_multiplier"), (int)getValueAsUlong(constants::species_constants_map[kind], "species_chromosome_number")),
        get_value_from_chromosome(chromosome_vec, c_structure, "mh", getValueAsFloat(constants::species_constants_map[kind], "species_theoretical_maximum_height"), (int)getValueAsUlong(constants::species_constants_map[kind], "species_chromosome_number")),
        get_value_from_chromosome(chromosome_vec, c_structure, "mw", getValueAsFloat(constants::species_constants_map[kind], "species_theoretical_maximum_weight"), (int)getValueAsUlong(constants::species_constants_map[kind], "species_chromosome_number")),
        age - 1,
        get_value_from_chromosome(chromosome_vec, c_structure, "bh", getValueAsFloat(constants::species_constants_map[kind], "species_theoretical_maximum_base_height"), (int)getValueAsUlong(constants::species_constants_map[kind], "species_chromosome_number")),
        get_value_from_chromosome(chromosome_vec, c_structure, "bw", getValueAsFloat(constants::species_constants_map[kind], "species_theoretical_maximum_base_weight"), (int)getValueAsUlong(constants::species_constants_map[kind], "species_chromosome_number")),
        0.0, /* age_death_factor */
        0.0, /* fitness_death_factor */
        0.0, /* death_factor */
        0.0, /* static_fitness */
        get_value_from_chromosome(chromosome_vec, c_structure, "ba", getValueAsFloat(constants::species_constants_map[kind], "species_theoretical_maximum_base_appetite"), (int)getValueAsUlong(constants::species_constants_map[kind], "species_chromosome_number")),
        get_value_from_chromosome(chromosome_vec, c_structure, "bp", getValueAsFloat(constants::species_constants_map[kind], "species_theoretical_maximum_base_speed"), (int)getValueAsUlong(constants::species_constants_map[kind], "species_chromosome_number")),
        get_value_from_chromosome(chromosome_vec, c_structure, "bs", getValueAsFloat(constants::species_constants_map[kind], "species_theoretical_maximum_base_stamina"), (int)getValueAsUlong(constants::species_constants_map[kind], "species_chromosome_number")),
        get_value_from_chromosome(chromosome_vec, c_structure, "bv", getValueAsFloat(constants::species_constants_map[kind], "species_theoretical_maximum_base_vitality"), (int)getValueAsUlong(constants::species_constants_map[kind], "species_chromosome_number")),
        get_value_from_chromosome(chromosome_vec, c_structure, "ba", getValueAsFloat(constants::species_constants_map[kind], "species_theoretical_maximum_base_appetite"), (int)getValueAsUlong(constants::species_constants_map[kind], "species_chromosome_number")),
        get_value_from_chromosome(chromosome_vec, c_structure, "bp", getValueAsFloat(constants::species_constants_map[kind], "species_theoretical_maximum_base_speed"), (int)getValueAsUlong(constants::species_constants_map[kind], "species_chromosome_number")),
        get_value_from_chromosome(chromosome_vec, c_structure, "bs", getValueAsFloat(constants::species_constants_map[kind], "species_theoretical_maximum_base_stamina"), (int)getValueAsUlong(constants::species_constants_map[kind], "species_chromosome_number")),
        get_value_from_chromosome(chromosome_vec, c_structure, "bv", getValueAsFloat(constants::species_constants_map[kind], "species_theoretical_maximum_base_vitality"), (int)getValueAsUlong(constants::species_constants_map[kind], "species_chromosome_number")),
        XY.first,
        XY.second,
        0.0, /* dynamic_fitness */
        getValueAsFloat(constants::species_constants_map[kind], "vision_radius"),
        getValueAsFloat(constants::species_constants_map[kind], "species_sleep_restore_factor"),
        Ecosystem::Sleep::Awake,
        static_cast<Ecosystem::Monitor>(monitor)
    );

    Ecosystem::Organism *organism_ptr = helper::get_mutable_pointer_from_offset(builder, organism_offset);
    organism_opts::increment_age(organism_ptr);
    organism_opts::evaluate_static_fitness(organism_ptr);

    return organism_offset;
}

flatbuffers::Offset<Ecosystem::Organism> God::createOrganism(
    flatbuffers::FlatBufferBuilder &builder,
    const std::string &kind,
    const std::string &kingdom,
    const uint64_t &age,
    const int8_t &monitor)
{
    PROFILE_FUNCTION();
    return createOrganism(builder, kind, kingdom, age, "", "", 0, helper::random_location(), monitor);
}

void God::cleanSlate()
{
    for (const auto &entry : std::filesystem::directory_iterator(
             helper::ecosystem_root / std::filesystem::path("data") / std::filesystem::path("json")))
    {
        for (const auto &inner_entry : std::filesystem::directory_iterator(entry.path()))
        {
            std::ifstream in(inner_entry.path() / "base.json");
            nlohmann::json tmp;
            in >> tmp;
            in.close();

            constants::species_constants_map[inner_entry.path().filename().string()] = tmp;
        }
    }

    db.clear_database();
}

void God::update_species(const std::string &full_species_name)
{
    std::string kind = full_species_name.substr(full_species_name.find('/') + 1); // Note to Darkstar1997: this is not path
    std::string kingdom = full_species_name.substr(0, full_species_name.find('/'));

    const std::filesystem::path modify_filepath = helper::ecosystem_root / std::filesystem::path("data") / std::filesystem::path("json") / kingdom / kind / std::filesystem::path("modify.json");

    std::ifstream modify_in(modify_filepath);

    nlohmann::json modify;
    modify_in >> modify;

    for (const auto [key, value] : modify.items())
    {
        constants::species_constants_map[kind][key] = updateStat((double)constants::species_constants_map[kind][key], (double)value);
    }

    modify_in.close();
}

double God::killer_function(const double &index, const double &size) const
{
    // return std::exp(-x / (s / 10.0))
    // return pow(x / s, 1 / 1.75)
    // return 1 - (1 / (1 + exp(-(10 * index - size) / pow(size, 0.5))));
    const double &ratio = 1.0 / 10.0;
    return 1 - (1 / (1 + exp((ratio * size - index) / (ratio * pow(size, 0.5)))));
}

int God::creator_function(const double &o_factor) const
{
    std::gamma_distribution<double> dis(1.5, o_factor);
    return std::round(dis(helper::rng));
}

std::string God::get_child_chromosome(const Ecosystem::Organism *parent1, const Ecosystem::Organism *parent2, const nlohmann::json &species_constants)
{
    // Generate chromosomes of the child
    auto child_chromosome = helper::get_random_mixture(
        helper::bytevector_to_string(parent1->chromosome()->data(), parent1->chromosome()->size(), parent1->chromosome_number()),
        helper::bytevector_to_string(parent2->chromosome()->data(), parent2->chromosome()->size(), parent2->chromosome_number()));

    // Mutate chromosomes
    for (auto &bit : child_chromosome)
        if (helper::weighted_prob(std::max(parent1->mutation_probability(), parent2->mutation_probability())))
            bit = (bit == '1') ? '0' : '1';

    return child_chromosome;
}

flatbuffers::Offset<Ecosystem::Organism> God::clone_organism(
    flatbuffers::FlatBufferBuilder &builder,
    Ecosystem::OrganismBuilder &organism_builder,
    const Ecosystem::Organism *previous_organism)
{
    // Assign base stats of species

    std::vector<flatbuffers::Offset<Ecosystem::ChromosomeStrand>> newStdvecCStrand;
    for (const auto &cStrand : *previous_organism->chromosome_structure())
    {
        Ecosystem::ChromosomeStrandBuilder cStrandBuilder(builder);
        cStrandBuilder.add_code(builder.CreateString(cStrand->code()));
        cStrandBuilder.add_start(cStrand->start());
        cStrandBuilder.add_length(cStrand->length());
        newStdvecCStrand.push_back(cStrandBuilder.Finish());
    }

    organism_builder.add_chromosome_structure(builder.CreateVectorOfSortedTables(newStdvecCStrand.data(), newStdvecCStrand.size()));
    organism_builder.add_chromosome_number(previous_organism->chromosome_number());
    organism_builder.add_mating_age_start(previous_organism->mating_age_start());
    organism_builder.add_mating_age_end(previous_organism->mating_age_end());
    organism_builder.add_max_age(previous_organism->max_age());
    organism_builder.add_offsprings_factor(previous_organism->offsprings_factor());
    organism_builder.add_sexuality(previous_organism->sexuality());
    organism_builder.add_mutation_probability(previous_organism->mutation_probability());
    organism_builder.add_conceiving_probability(previous_organism->conceiving_probability());
    organism_builder.add_mating_probability(previous_organism->mating_probability());
    organism_builder.add_age_fitness_on_death_ratio(previous_organism->age_fitness_on_death_ratio());
    organism_builder.add_height_on_speed(previous_organism->height_on_speed());
    organism_builder.add_height_on_stamina(previous_organism->height_on_stamina());
    organism_builder.add_height_on_vitality(previous_organism->height_on_vitality());
    organism_builder.add_weight_on_speed(previous_organism->weight_on_speed());
    organism_builder.add_weight_on_stamina(previous_organism->weight_on_stamina());
    organism_builder.add_weight_on_vitality(previous_organism->weight_on_vitality());
    organism_builder.add_vitality_on_appetite(previous_organism->vitality_on_appetite());
    organism_builder.add_vitality_on_speed(previous_organism->vitality_on_speed());
    organism_builder.add_stamina_on_appetite(previous_organism->stamina_on_appetite());
    organism_builder.add_stamina_on_speed(previous_organism->stamina_on_speed());
    organism_builder.add_theoretical_maximum_base_appetite(previous_organism->theoretical_maximum_base_appetite());
    organism_builder.add_theoretical_maximum_base_height(previous_organism->theoretical_maximum_base_height());
    organism_builder.add_theoretical_maximum_base_speed(previous_organism->theoretical_maximum_base_speed());
    organism_builder.add_theoretical_maximum_base_stamina(previous_organism->theoretical_maximum_base_stamina());
    organism_builder.add_theoretical_maximum_base_vitality(previous_organism->theoretical_maximum_base_vitality());
    organism_builder.add_theoretical_maximum_base_weight(previous_organism->theoretical_maximum_base_weight());
    organism_builder.add_theoretical_maximum_height_multiplier(previous_organism->theoretical_maximum_height_multiplier());
    organism_builder.add_theoretical_maximum_speed_multiplier(previous_organism->theoretical_maximum_speed_multiplier());
    organism_builder.add_theoretical_maximum_stamina_multiplier(previous_organism->theoretical_maximum_stamina_multiplier());
    organism_builder.add_theoretical_maximum_vitality_multiplier(previous_organism->theoretical_maximum_vitality_multiplier());
    organism_builder.add_theoretical_maximum_weight_multiplier(previous_organism->theoretical_maximum_weight_multiplier());
    organism_builder.add_theoretical_maximum_height(previous_organism->theoretical_maximum_height());
    organism_builder.add_theoretical_maximum_speed(previous_organism->theoretical_maximum_speed());
    organism_builder.add_theoretical_maximum_weight(previous_organism->theoretical_maximum_weight());
    organism_builder.add_sleep_restore_factor(previous_organism->sleep_restore_factor());
    organism_builder.add_food_chain_rank(previous_organism->food_chain_rank());
    organism_builder.add_vision_radius(previous_organism->vision_radius());

    // Assign passed default attributes
    organism_builder.add_kind(builder.CreateString(previous_organism->kind()));
    organism_builder.add_kingdom(previous_organism->kingdom());
    organism_builder.add_generation(previous_organism->generation());
    organism_builder.add_age(previous_organism->age());
    organism_builder.add_monitor(previous_organism->monitor());

    std::vector<uint8_t> chromosome(
        previous_organism->chromosome()->data(),
        previous_organism->chromosome()->data() + previous_organism->chromosome()->size());
    organism_builder.add_chromosome(builder.CreateVector(chromosome));

    organism_builder.add_X(previous_organism->X());
    organism_builder.add_Y(previous_organism->Y());
    organism_builder.add_asleep(previous_organism->asleep());

    // Assign attributes from chromosome
    organism_builder.add_immunity(previous_organism->immunity());
    organism_builder.add_gender(previous_organism->gender());
    organism_builder.add_base_appetite(previous_organism->base_appetite());
    organism_builder.add_base_speed(previous_organism->base_speed());
    organism_builder.add_base_stamina(previous_organism->base_stamina());
    organism_builder.add_base_vitality(previous_organism->base_vitality());
    organism_builder.add_base_weight(previous_organism->base_weight());
    organism_builder.add_base_height(previous_organism->base_height());
    organism_builder.add_weight(previous_organism->weight());
    organism_builder.add_height(previous_organism->height());
    organism_builder.add_max_weight(previous_organism->max_weight());
    organism_builder.add_max_height(previous_organism->max_height());
    organism_builder.add_max_appetite_at_age(previous_organism->max_appetite_at_age());
    organism_builder.add_max_speed_at_age(previous_organism->max_speed_at_age());
    organism_builder.add_max_stamina_at_age(previous_organism->max_stamina_at_age());
    organism_builder.add_max_vitality_at_age(previous_organism->max_vitality_at_age());
    organism_builder.add_weight_multiplier(previous_organism->weight_multiplier());
    organism_builder.add_height_multiplier(previous_organism->height_multiplier());
    organism_builder.add_speed_multiplier(previous_organism->speed_multiplier());
    organism_builder.add_stamina_multiplier(previous_organism->stamina_multiplier());
    organism_builder.add_vitality_multiplier(previous_organism->vitality_multiplier());

    organism_builder.add_vitality(previous_organism->vitality());
    organism_builder.add_stamina(previous_organism->stamina());
    organism_builder.add_appetite(previous_organism->appetite());
    organism_builder.add_speed(previous_organism->speed());

    organism_builder.add_static_fitness(previous_organism->static_fitness());
    organism_builder.add_dynamic_fitness(previous_organism->dynamic_fitness());

    return organism_builder.Finish();
}

void God::happy_new_year(const bool &log)
{
    spawn_count = 0;
    recent_births = 0;
    recent_deaths = 0;
    recent_population = 0;

    Ecosystem::World *previous_world = Ecosystem::GetMutableWorld(buffer.data());

    std::vector<flatbuffers::Offset<Ecosystem::Species>> newStdvecSpecies;

    uint32_t num_species = previous_world->species()->size();

    std::mt19937_64 rng{std::random_device()()};

    for (uint32_t n = 0; n < num_species; n++)
    {
        Ecosystem::Species *species = previous_world->mutable_species()->GetMutableObject(n);
        auto kingdom = species->kingdom();
        auto kind = species->kind();

        std::string full_species_name = EcosystemTypes::get_kingdom_name[static_cast<uint8_t>(kingdom)] + "/" + kind->str();

        std::vector<flatbuffers::Offset<Ecosystem::Organism>> stdvecOrganisms;

        /***************************************************
         *       Annual Killing (un-selection) Begins      *
         ***************************************************/

        // Vector for [ (death_factor, index in vector) ]

        std::vector<std::pair<float, uint32_t>> organisms_vec;
        organisms_vec.reserve(species->organism()->size());

        for (uint32_t i = 0; i < species->organism()->size(); i++)
        {
            Ecosystem::Organism *organism = species->mutable_organism()->GetMutableObject(i);
            organism_opts::generate_death_factor(organism);
            organisms_vec.emplace_back(std::make_pair(organism->death_factor(), i));
        }

        // Sort organism_vec by death factor

        std::sort(
            organisms_vec.begin(), organisms_vec.end(),
            std::greater<std::pair<float, uint32_t>>());

        std::uniform_real_distribution<double> par_dis(0.0, 1.0);
        std::mt19937_64 par_rng{std::random_device()()};

        for (uint32_t index = 0; index < organisms_vec.size(); index++)
        {
            const double x = par_dis(par_rng);

            // Spare this organism
            if (disable_deaths || x >= killer_function(index, organisms_vec.size()))
            {
                // Annual ageing of this Organism
                organism_opts::increment_age(species->mutable_organism()->GetMutableObject(organisms_vec[index].second));

                // auto new_organism = clone_organism(
                //     builder,
                //     new_organism_builder,
                //     species->organism()->Get(organisms_vec[index].second));
                // stdvecOrganisms.push_back(new_organism);

                stdvecOrganisms.push_back(createOrganism(
                    builder,
                    species->organism()->Get(organisms_vec[index].second)->kind()->str(),
                    std::to_string((int)species->organism()->Get(organisms_vec[index].second)->kingdom()),
                    species->organism()->Get(organisms_vec[index].second)->age(),
                    (int8_t)species->organism()->Get(organisms_vec[index].second)->monitor()));
            }
            else
            {
                recent_deaths++;
            }
        }

        /***************************************************
         *       Annual Mating (spawning) Begins      *
         ***************************************************/

        if (stdvecOrganisms.size() > 0)
        {
            update_species(full_species_name);
            const auto &species_constants = constants::species_constants_map[kind->str()];

            std::vector<uint32_t> mating_list1, mating_list2;

            for (int index = 0; index < stdvecOrganisms.size(); index++)
            {
                auto desparate_organism = helper::get_pointer_from_offset(builder, stdvecOrganisms[index]);
                auto sexuality = desparate_organism->sexuality();

                if (sexuality == Ecosystem::Reproduction::Asexual)
                {
                    if (desparate_organism->age() >= desparate_organism->mating_age_start() &&
                        desparate_organism->age() <= desparate_organism->mating_age_end())
                    {
                        mating_list1.push_back(index);
                    }
                }
                else if (sexuality == Ecosystem::Reproduction::Sexual)
                {
                    if (desparate_organism->age() >= desparate_organism->mating_age_start() &&
                        desparate_organism->age() <= desparate_organism->mating_age_end())
                    {
                        if (desparate_organism->gender() == Ecosystem::Gender::Male)
                        {
                            mating_list1.push_back(index);
                        }
                        else
                        {
                            mating_list2.push_back(index);
                        }
                    }
                }
            }

            std::shuffle(mating_list1.begin(), mating_list1.end(), rng);
            std::shuffle(mating_list2.begin(), mating_list2.end(), rng);

            uint32_t index_parent = 0;

            while ((mating_list1.size() > index_parent && mating_list2.size() > index_parent &&
                    helper::get_pointer_from_offset(builder, stdvecOrganisms[index_parent])->sexuality() == Ecosystem::Reproduction::Sexual) ||
                   (mating_list1.size() > index_parent &&
                    helper::get_pointer_from_offset(builder, stdvecOrganisms[index_parent])->sexuality() == Ecosystem::Reproduction::Asexual))
            {
                const Ecosystem::Organism *parent1 = helper::get_pointer_from_offset(builder, stdvecOrganisms[mating_list1[index_parent]]);
                const Ecosystem::Organism *parent2 = helper::get_pointer_from_offset(builder, stdvecOrganisms[helper::get_pointer_from_offset(builder, stdvecOrganisms[index_parent])->sexuality() == Ecosystem::Reproduction::Sexual ? mating_list2[index_parent] : mating_list1[index_parent]]);

                if (helper::weighted_prob(std::min(parent1->mating_probability(), parent2->mating_probability())))
                {
                    int n_children = creator_function(std::min(parent1->offsprings_factor(), parent2->offsprings_factor()));

                    while (n_children--)
                    {
                        if (!helper::weighted_prob(std::min(parent1->conceiving_probability(), parent2->conceiving_probability())))
                            continue; // Conceiving probability too low this time

                        auto child_chromosome = get_child_chromosome(parent1, parent2, species_constants);
                        bool monitor_child = monitor_offsprings && (static_cast<bool>(parent1->monitor()) || static_cast<bool>(parent2->monitor()));

                        uint64_t child_X = (parent1->X() + parent2->X()) / 2, child_Y = (parent1->Y() + parent2->Y()) / 2;
                        std::string child_name = std::to_string(year) + "-" + std::to_string(spawn_count++);

                        auto child_offset = createOrganism(
                            builder,
                            parent1->kind()->str(),
                            std::to_string(static_cast<uint8_t>(kingdom)),
                            1,
                            child_name,
                            child_chromosome,
                            std::max(parent1->generation(), parent2->generation()) + 1,
                            std::make_pair(child_X, child_Y),
                            monitor_child);

                        Ecosystem::Organism *child_ptr = helper::get_mutable_pointer_from_offset(builder, child_offset);
                        organism_opts::generate_death_factor(child_ptr);

                        // Skip abnormal children (with weird stats)
                        if (organism_opts::is_normal_child(helper::get_pointer_from_offset(builder, child_offset)))
                        {
                            stdvecOrganisms.push_back(child_offset);
                            recent_births++;
                        }
                    }
                }
                index_parent++;
            }
        }

        recent_population += stdvecOrganisms.size();

        newStdvecSpecies.push_back(Ecosystem::CreateSpecies(builder,
            builder.CreateString(kind),
            kingdom,
            builder.CreateVectorOfSortedTables(stdvecOrganisms.data(), stdvecOrganisms.size())
        ));
    }

    flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Ecosystem::Species>>> species_vec = builder.CreateVectorOfSortedTables(newStdvecSpecies.data(), newStdvecSpecies.size());

    Ecosystem::WorldBuilder new_world_builder(builder);
    new_world_builder.add_year(year);
    new_world_builder.add_species(species_vec);
    builder.Finish(new_world_builder.Finish());

    /***************************
     *       World Updated     *
     ***************************/

    buffer = builder.Release();
    builder.Clear();

    // TODO push new buffer to db

    /*********************
     *       Logging     *
     *********************/

    if (log)
    {
        fmt::print("Year: {} - Recent births: {} - Recent deaths: {} - Population: {}", year, recent_births, recent_deaths, recent_population);
    }

    year++;
}
