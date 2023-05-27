#include <flatbuffers/minireflect.h>
#include <fmt/core.h>

#include <god.hpp>
#include <helper.hpp>
#include <nlohmann/json.hpp>
#include <organism.hpp>
#include <profiler.hpp>
#include <range/v3/view.hpp>
#include <set>
#include <species_constants.hpp>
#include <stat_fetcher.hpp>

namespace rv = ranges::views;

static XoshiroCpp::Xoshiro128PlusPlus rng{std::random_device()()};

static double get_value_from_chromosome(
    const std::vector<uint8_t> &chromosome,
    const std::map<std::string, std::map<std::string, int>> &c_structure,
    const std::string &code, const double &multiplier,
    const uint16_t &chromosome_number);

static std::string getValueAsStr(const nlohmann::json &attributes,
                                 const std::string &key) {
    return attributes.find(key) != attributes.end()
               ? attributes[key].get<std::string>()
               : "";
}

static uint64_t getValueAsUlong(const nlohmann::json &attributes,
                                const std::string &key) {
    return attributes.find(key) != attributes.end()
               ? attributes[key].get<uint64_t>()
               : 0;
}

static uint32_t getValueAsUint(const nlohmann::json &attributes,
                               const std::string &key) {
    return attributes.find(key) != attributes.end()
               ? attributes[key].get<uint32_t>()
               : 0;
}

static float getValueAsFloat(const nlohmann::json &attributes,
                             const std::string &key) {
    return attributes.find(key) != attributes.end()
               ? attributes[key].get<float>()
               : 0.0;
}

static uint8_t getValueAsByte(const nlohmann::json &attributes,
                              const std::string &key) {
    return attributes.find(key) != attributes.end()
               ? attributes[key].get<uint8_t>()
               : 0;
}

static uint16_t getValueAsUshort(const nlohmann::json &attributes,
                                 const std::string &key) {
    return attributes.find(key) != attributes.end()
               ? attributes[key].get<uint16_t>()
               : 0;
}

static double get_value_from_chromosome(
    const std::vector<uint8_t> &chromosome,
    const std::map<std::string, std::map<std::string, int>> &c_structure,
    const std::string &code, const double &multiplier,
    const uint16_t &chromosome_number) {
    auto it = c_structure.find(code);
    if (it == c_structure.end()) return 0;
    std::string chromosome_str = helper::bytevector_to_string(
        chromosome.data(), chromosome.size(), chromosome_number);
    int start = it->second.find("start")->second;
    int len = it->second.find("length")->second;
    if (len == 0) return 0;
    return (helper::to_decimal(chromosome_str.substr(start, len)) /
            static_cast<double>(1 << len)) *
           multiplier;
}

static double updateStat(double base, double p_range) {
    std::uniform_real_distribution<double> dis(0.0, p_range * 2);
    const double x = p_range - dis(rng);

    return base * (1 + x);
}

God::God(const std::filesystem::path &ecosystem_root, const bool gods_eye) {
    this->gods_eye = gods_eye;
    this->ecosystem_root = ecosystem_root;
    constants::init(this->ecosystem_root);
    builder.ForceDefaults(true);
    db = std::make_unique<DatabaseManager>(this->ecosystem_root / "data" /
                                           "ecosystem_master.db");

    fmt::print("God created!\n");
}

God::~God() { fmt::print("God is dead...\n"); }

void God::createWorld(
    std::vector<std::unordered_map<std::string, std::string>> &organisms) {
    /* World creation begins */

    std::vector<flatbuffers::Offset<Ecosystem::Species>> stdvecSpecies;

    std::set<std::pair<std::string, std::string>> unique_species;
    for (const auto &organism : organisms) {
        unique_species.insert(
            {organism.find("kingdom")->second, organism.find("kind")->second});
    }

    for (const auto &[kingdom, kind] : unique_species) {
        std::vector<flatbuffers::Offset<Ecosystem::Organism>> stdvecOrganisms;

        for (auto &organism : organisms) {
            if (organism.find("kind")->second == kind) {
                flatbuffers::Offset<Ecosystem::Organism> current_organism;
                bool spawn_killed = false;

                do {
                    // Retry for spawn-kill situations
                    current_organism = createOrganism(
                        builder, organism["kind"], organism["kingdom"],
                        std::stoul(organism["age"]),
                        organism.find("monitor") != organism.end()
                            ? std::stoi(organism["monitor"])
                            : 0);
                    spawn_killed = !organism_opts::is_normal_child(
                        helper::get_pointer_from_offset(builder,
                                                        current_organism));
                } while (spawn_killed);

                stdvecOrganisms.push_back(current_organism);
            }
        }

        stdvecSpecies.push_back(Ecosystem::CreateSpecies(
            builder, builder.CreateString(kind.c_str()),
            (Ecosystem::KingdomE)std::stoi(kingdom),
            builder.CreateVectorOfSortedTables(stdvecOrganisms.data(),
                                               stdvecOrganisms.size())));
    }

    auto world_species = builder.CreateVectorOfSortedTables(
        stdvecSpecies.data(), stdvecSpecies.size());

    /* World creation ends */

    builder.Finish(Ecosystem::CreateWorld(builder, year, world_species));
    buffer = builder.Release();
    builder.Clear();
}

void God::displayWorldMetadata() {
    PROFILE_FUNCTION();
    fmt::print("\nBuffer Size: {} bytes \n", buffer.size());

    flatbuffers::ToStringVisitor visitor("", true, "", true);
    flatbuffers::IterateFlatBuffer(buffer.data(), Ecosystem::WorldTypeTable(),
                                   &visitor);
    nlohmann::json json_data = nlohmann::json::parse(visitor.s);
    fmt::print("Parsed JSON:\n{}\n", json_data.dump(4));
}

flatbuffers::Offset<Ecosystem::Organism> God::createOrganism(
    flatbuffers::FlatBufferBuilder &builder, const std::string &kind,
    const std::string &kingdom, const uint64_t &age, const std::string &name,
    const std::string &chromosome, const uint64_t &generation,
    const std::pair<uint64_t, uint64_t> &XY, const int8_t &monitor) {
    PROFILE_FUNCTION();

    /* Organism creation begins */

    // Assign base stats of species
    std::vector<flatbuffers::Offset<Ecosystem::ChromosomeStrand>> stdvecCStrand;
    std::map<std::string, std::map<std::string, int>> c_structure =
        constants::get_species_constants_map()[kind]["chromosome_structure"];
    for (auto &cStrand : c_structure) {
        stdvecCStrand.push_back(Ecosystem::CreateChromosomeStrand(
            builder, builder.CreateString(cStrand.first.c_str()),
            cStrand.second["start"], cStrand.second["length"]));
    }

    std::string tmp_str;
    float tmp_flt;

    tmp_str = name.length() != 0
                  ? name
                  : fmt::format("{}-orphan-{}", kind, helper::random_name(16));
    auto organism_name = builder.CreateString(tmp_str.c_str());
    tmp_str.clear();

    tmp_str = tmp_str.length() != 0
                  ? chromosome
                  : helper::random_binary((int)getValueAsUlong(
                        constants::get_species_constants_map()[kind],
                        "species_chromosome_number"));
    std::vector<uint8_t> chromosome_vec = helper::string_to_bytevector(tmp_str);
    tmp_str.clear();
    auto organism_chromosome =
        builder.CreateVector(chromosome_vec.data(), chromosome_vec.size());

    auto organism_offset = Ecosystem::CreateOrganism(
        builder, builder.CreateString(kind.c_str()),
        (Ecosystem::KingdomE)std::stoi(kingdom),
        getValueAsUshort(constants::get_species_constants_map()[kind],
                         "species_chromosome_number"),
        builder.CreateVectorOfSortedTables(stdvecCStrand.data(),
                                           stdvecCStrand.size()),
        getValueAsUlong(constants::get_species_constants_map()[kind],
                        "food_chain_rank"),
        (Ecosystem::Reproduction)getValueAsByte(
            constants::get_species_constants_map()[kind], "sexuality"),
        getValueAsFloat(constants::get_species_constants_map()[kind],
                        "species_age_fitness_on_death_ratio"),
        getValueAsFloat(constants::get_species_constants_map()[kind],
                        "conceiving_probability"),
        getValueAsFloat(constants::get_species_constants_map()[kind],
                        "mating_probability"),
        getValueAsUint(constants::get_species_constants_map()[kind],
                       "mating_age_start"),
        getValueAsUint(constants::get_species_constants_map()[kind],
                       "mating_age_end"),
        getValueAsUint(constants::get_species_constants_map()[kind],
                       "species_max_age"),
        getValueAsFloat(constants::get_species_constants_map()[kind],
                        "mutation_probability"),
        getValueAsFloat(constants::get_species_constants_map()[kind],
                        "offsprings_factor"),
        getValueAsFloat(constants::get_species_constants_map()[kind],
                        "species_height_on_speed"),
        getValueAsFloat(constants::get_species_constants_map()[kind],
                        "species_height_on_stamina"),
        getValueAsFloat(constants::get_species_constants_map()[kind],
                        "species_height_on_vitality"),
        getValueAsFloat(constants::get_species_constants_map()[kind],
                        "species_weight_on_speed"),
        getValueAsFloat(constants::get_species_constants_map()[kind],
                        "species_weight_on_stamina"),
        getValueAsFloat(constants::get_species_constants_map()[kind],
                        "species_weight_on_vitality"),
        getValueAsFloat(constants::get_species_constants_map()[kind],
                        "species_vitality_on_appetite"),
        getValueAsFloat(constants::get_species_constants_map()[kind],
                        "species_vitality_on_speed"),
        getValueAsFloat(constants::get_species_constants_map()[kind],
                        "species_stamina_on_appetite"),
        getValueAsFloat(constants::get_species_constants_map()[kind],
                        "species_stamina_on_speed"),
        getValueAsFloat(constants::get_species_constants_map()[kind],
                        "species_theoretical_maximum_base_appetite"),
        getValueAsFloat(constants::get_species_constants_map()[kind],
                        "species_theoretical_maximum_base_height"),
        getValueAsFloat(constants::get_species_constants_map()[kind],
                        "species_theoretical_maximum_base_speed"),
        getValueAsFloat(constants::get_species_constants_map()[kind],
                        "species_theoretical_maximum_base_stamina"),
        getValueAsFloat(constants::get_species_constants_map()[kind],
                        "species_theoretical_maximum_base_vitality"),
        getValueAsFloat(constants::get_species_constants_map()[kind],
                        "species_theoretical_maximum_base_weight"),
        getValueAsFloat(constants::get_species_constants_map()[kind],
                        "species_theoretical_maximum_height"),
        getValueAsFloat(constants::get_species_constants_map()[kind],
                        "species_theoretical_maximum_speed"),
        getValueAsFloat(constants::get_species_constants_map()[kind],
                        "species_theoretical_maximum_weight"),
        getValueAsFloat(constants::get_species_constants_map()[kind],
                        "species_theoretical_maximum_height_multiplier"),
        getValueAsFloat(constants::get_species_constants_map()[kind],
                        "species_theoretical_maximum_speed_multiplier"),
        getValueAsFloat(constants::get_species_constants_map()[kind],
                        "species_theoretical_maximum_stamina_multiplier"),
        getValueAsFloat(constants::get_species_constants_map()[kind],
                        "species_theoretical_maximum_vitality_multiplier"),
        getValueAsFloat(constants::get_species_constants_map()[kind],
                        "species_theoretical_maximum_weight_multiplier"),
        organism_name, organism_chromosome,
        (Ecosystem::Gender)get_value_from_chromosome(
            chromosome_vec, c_structure, "gn", 2.0,
            (int)getValueAsUlong(constants::get_species_constants_map()[kind],
                                 "species_chromosome_number")),
        generation,
        get_value_from_chromosome(
            chromosome_vec, c_structure, "im", 1.0,
            (int)getValueAsUlong(constants::get_species_constants_map()[kind],
                                 "species_chromosome_number")),
        get_value_from_chromosome(
            chromosome_vec, c_structure, "ba",
            getValueAsFloat(constants::get_species_constants_map()[kind],
                            "species_theoretical_maximum_base_appetite"),
            (int)getValueAsUlong(constants::get_species_constants_map()[kind],
                                 "species_chromosome_number")),
        get_value_from_chromosome(
            chromosome_vec, c_structure, "bh",
            getValueAsFloat(constants::get_species_constants_map()[kind],
                            "species_theoretical_maximum_base_height"),
            (int)getValueAsUlong(constants::get_species_constants_map()[kind],
                                 "species_chromosome_number")),
        get_value_from_chromosome(
            chromosome_vec, c_structure, "bp",
            getValueAsFloat(constants::get_species_constants_map()[kind],
                            "species_theoretical_maximum_base_speed"),
            (int)getValueAsUlong(constants::get_species_constants_map()[kind],
                                 "species_chromosome_number")),
        get_value_from_chromosome(
            chromosome_vec, c_structure, "bs",
            getValueAsFloat(constants::get_species_constants_map()[kind],
                            "species_theoretical_maximum_base_stamina"),
            (int)getValueAsUlong(constants::get_species_constants_map()[kind],
                                 "species_chromosome_number")),
        get_value_from_chromosome(
            chromosome_vec, c_structure, "bv",
            getValueAsFloat(constants::get_species_constants_map()[kind],
                            "species_theoretical_maximum_base_vitality"),
            (int)getValueAsUlong(constants::get_species_constants_map()[kind],
                                 "species_chromosome_number")),
        get_value_from_chromosome(
            chromosome_vec, c_structure, "bw",
            getValueAsFloat(constants::get_species_constants_map()[kind],
                            "species_theoretical_maximum_base_weight"),
            (int)getValueAsUlong(constants::get_species_constants_map()[kind],
                                 "species_chromosome_number")),
        get_value_from_chromosome(
            chromosome_vec, c_structure, "hm",
            getValueAsFloat(constants::get_species_constants_map()[kind],
                            "species_theoretical_maximum_height_multiplier"),
            (int)getValueAsUlong(constants::get_species_constants_map()[kind],
                                 "species_chromosome_number")),
        get_value_from_chromosome(
            chromosome_vec, c_structure, "pm",
            getValueAsFloat(constants::get_species_constants_map()[kind],
                            "species_theoretical_maximum_speed_multiplier"),
            (int)getValueAsUlong(constants::get_species_constants_map()[kind],
                                 "species_chromosome_number")),
        get_value_from_chromosome(
            chromosome_vec, c_structure, "sm",
            getValueAsFloat(constants::get_species_constants_map()[kind],
                            "species_theoretical_maximum_stamina_multiplier"),
            (int)getValueAsUlong(constants::get_species_constants_map()[kind],
                                 "species_chromosome_number")),
        get_value_from_chromosome(
            chromosome_vec, c_structure, "vm",
            getValueAsFloat(constants::get_species_constants_map()[kind],
                            "species_theoretical_maximum_vitality_multiplier"),
            (int)getValueAsUlong(constants::get_species_constants_map()[kind],
                                 "species_chromosome_number")),
        get_value_from_chromosome(
            chromosome_vec, c_structure, "wm",
            getValueAsFloat(constants::get_species_constants_map()[kind],
                            "species_theoretical_maximum_weight_multiplier"),
            (int)getValueAsUlong(constants::get_species_constants_map()[kind],
                                 "species_chromosome_number")),
        get_value_from_chromosome(
            chromosome_vec, c_structure, "mh",
            getValueAsFloat(constants::get_species_constants_map()[kind],
                            "species_theoretical_maximum_height"),
            (int)getValueAsUlong(constants::get_species_constants_map()[kind],
                                 "species_chromosome_number")),
        get_value_from_chromosome(
            chromosome_vec, c_structure, "mw",
            getValueAsFloat(constants::get_species_constants_map()[kind],
                            "species_theoretical_maximum_weight"),
            (int)getValueAsUlong(constants::get_species_constants_map()[kind],
                                 "species_chromosome_number")),
        age - 1,
        get_value_from_chromosome(
            chromosome_vec, c_structure, "bh",
            getValueAsFloat(constants::get_species_constants_map()[kind],
                            "species_theoretical_maximum_base_height"),
            (int)getValueAsUlong(constants::get_species_constants_map()[kind],
                                 "species_chromosome_number")),
        get_value_from_chromosome(
            chromosome_vec, c_structure, "bw",
            getValueAsFloat(constants::get_species_constants_map()[kind],
                            "species_theoretical_maximum_base_weight"),
            (int)getValueAsUlong(constants::get_species_constants_map()[kind],
                                 "species_chromosome_number")),
        0.0, /* static_fitness */
        get_value_from_chromosome(
            chromosome_vec, c_structure, "ba",
            getValueAsFloat(constants::get_species_constants_map()[kind],
                            "species_theoretical_maximum_base_appetite"),
            (int)getValueAsUlong(constants::get_species_constants_map()[kind],
                                 "species_chromosome_number")),
        get_value_from_chromosome(
            chromosome_vec, c_structure, "bp",
            getValueAsFloat(constants::get_species_constants_map()[kind],
                            "species_theoretical_maximum_base_speed"),
            (int)getValueAsUlong(constants::get_species_constants_map()[kind],
                                 "species_chromosome_number")),
        get_value_from_chromosome(
            chromosome_vec, c_structure, "bs",
            getValueAsFloat(constants::get_species_constants_map()[kind],
                            "species_theoretical_maximum_base_stamina"),
            (int)getValueAsUlong(constants::get_species_constants_map()[kind],
                                 "species_chromosome_number")),
        get_value_from_chromosome(
            chromosome_vec, c_structure, "bv",
            getValueAsFloat(constants::get_species_constants_map()[kind],
                            "species_theoretical_maximum_base_vitality"),
            (int)getValueAsUlong(constants::get_species_constants_map()[kind],
                                 "species_chromosome_number")),
        get_value_from_chromosome(
            chromosome_vec, c_structure, "ba",
            getValueAsFloat(constants::get_species_constants_map()[kind],
                            "species_theoretical_maximum_base_appetite"),
            (int)getValueAsUlong(constants::get_species_constants_map()[kind],
                                 "species_chromosome_number")),
        get_value_from_chromosome(
            chromosome_vec, c_structure, "bp",
            getValueAsFloat(constants::get_species_constants_map()[kind],
                            "species_theoretical_maximum_base_speed"),
            (int)getValueAsUlong(constants::get_species_constants_map()[kind],
                                 "species_chromosome_number")),
        get_value_from_chromosome(
            chromosome_vec, c_structure, "bs",
            getValueAsFloat(constants::get_species_constants_map()[kind],
                            "species_theoretical_maximum_base_stamina"),
            (int)getValueAsUlong(constants::get_species_constants_map()[kind],
                                 "species_chromosome_number")),
        get_value_from_chromosome(
            chromosome_vec, c_structure, "bv",
            getValueAsFloat(constants::get_species_constants_map()[kind],
                            "species_theoretical_maximum_base_vitality"),
            (int)getValueAsUlong(constants::get_species_constants_map()[kind],
                                 "species_chromosome_number")),
        XY.first, XY.second, 1.0, /* dynamic_fitness */
        getValueAsFloat(constants::get_species_constants_map()[kind],
                        "vision_radius"),
        getValueAsFloat(constants::get_species_constants_map()[kind],
                        "species_sleep_restore_factor"),
        Ecosystem::Sleep::awake, static_cast<Ecosystem::Monitor>(monitor));

    // TODO: Remove this
    Ecosystem::Organism *organism_ptr =
        helper::get_mutable_pointer_from_offset(builder, organism_offset);
    organism_opts::increment_age(organism_ptr);

    return organism_offset;
}

flatbuffers::Offset<Ecosystem::Organism> God::createOrganism(
    flatbuffers::FlatBufferBuilder &builder, const std::string &kind,
    const std::string &kingdom, const uint64_t &age, const int8_t &monitor) {
    PROFILE_FUNCTION();
    return createOrganism(builder, kind, kingdom, age, "", "", 0,
                          helper::random_location(), monitor);
}

void God::cleanSlate() {
    for (const auto &entry : std::filesystem::directory_iterator(
             ecosystem_root / std::filesystem::path("data") /
             std::filesystem::path("json"))) {
        if (entry.path().filename().string()[0] == '.') {
            // Ignore hidden files
            continue;
        }

        for (const auto &inner_entry :
             std::filesystem::directory_iterator(entry.path())) {
            if (inner_entry.path().filename().string()[0] == '.') {
                // Ignore hidden files
                continue;
            }

            std::ifstream in(inner_entry.path() / "base.json");
            nlohmann::json tmp;
            in >> tmp;
            in.close();

            constants::get_species_constants_map()
                [inner_entry.path().filename().string()] = tmp;
        }
    }

    if (gods_eye) {
        db->clear_database();
    }
}

void God::update_species(const std::string &full_species_name) {
    std::string kind =
        full_species_name.substr(full_species_name.find('/') +
                                 1);  // Note to Darkstar1997: this is not path
    std::string kingdom =
        full_species_name.substr(0, full_species_name.find('/'));

    const std::filesystem::path modify_filepath =
        ecosystem_root / std::filesystem::path("data") /
        std::filesystem::path("json") / kingdom / kind /
        std::filesystem::path("modify.json");

    std::ifstream modify_in(modify_filepath);

    nlohmann::json modify;
    modify_in >> modify;

    for (const auto [key, value] : modify.items()) {
        constants::get_species_constants_map()[kind][key] = updateStat(
            (double)constants::get_species_constants_map()[kind][key],
            (double)value);
    }

    modify_in.close();
}

double God::killer_function(const double &index, const double &size) const {
    // return std::exp(-x / (s / 10.0))
    // return pow(x / s, 1 / 1.75)
    // return 1 - (1 / (1 + exp(-(10 * index - size) / pow(size, 0.5))));
    const double &ratio = 1.0 / 10.0;
    return 1 -
           (1 / (1 + exp((ratio * size - index) / (ratio * pow(size, 0.5)))));
}

int God::creator_function(const double &o_factor) const {
    std::gamma_distribution<double> dis(1.5, o_factor);
    return std::round(dis(rng));
}

std::string God::get_child_chromosome(const Ecosystem::OrganismT &parent1,
                                      const Ecosystem::OrganismT &parent2,
                                      const nlohmann::json &species_constants) {
    // Generate chromosomes of the child
    auto child_chromosome = helper::get_random_mixture(
        helper::bytevector_to_string(parent1.chromosome.data(),
                                     parent1.chromosome.size(),
                                     parent1.chromosome_number),
        helper::bytevector_to_string(parent2.chromosome.data(),
                                     parent2.chromosome.size(),
                                     parent2.chromosome_number));

    // Mutate chromosomes
    for (auto &bit : child_chromosome)
        if (helper::weighted_prob(std::max(parent1.mutation_probability,
                                           parent2.mutation_probability)))
            bit = (bit == '1') ? '0' : '1';

    return child_chromosome;
}

flatbuffers::Offset<Ecosystem::Organism> God::clone_organism(
    flatbuffers::FlatBufferBuilder &builder,
    const Ecosystem::Organism *previous_organism) {
    std::vector<flatbuffers::Offset<Ecosystem::ChromosomeStrand>>
        newStdvecCStrand;
    for (const auto &cStrand : *previous_organism->chromosome_structure()) {
        newStdvecCStrand.push_back(Ecosystem::CreateChromosomeStrand(
            builder, builder.CreateString(cStrand->code()), cStrand->start(),
            cStrand->length()));
    }

    return Ecosystem::CreateOrganism(
        builder, builder.CreateString(previous_organism->kind()),
        previous_organism->kingdom(), previous_organism->chromosome_number(),
        builder.CreateVectorOfSortedTables(newStdvecCStrand.data(),
                                           newStdvecCStrand.size()),
        previous_organism->food_chain_rank(), previous_organism->sexuality(),
        previous_organism->age_fitness_on_death_ratio(),
        previous_organism->conceiving_probability(),
        previous_organism->mating_probability(),
        previous_organism->mating_age_start(),
        previous_organism->mating_age_end(), previous_organism->max_age(),
        previous_organism->mutation_probability(),
        previous_organism->offsprings_factor(),
        previous_organism->height_on_speed(),
        previous_organism->height_on_stamina(),
        previous_organism->height_on_vitality(),
        previous_organism->weight_on_speed(),
        previous_organism->weight_on_stamina(),
        previous_organism->weight_on_vitality(),
        previous_organism->vitality_on_appetite(),
        previous_organism->vitality_on_speed(),
        previous_organism->stamina_on_appetite(),
        previous_organism->stamina_on_speed(),
        previous_organism->theoretical_maximum_base_appetite(),
        previous_organism->theoretical_maximum_base_height(),
        previous_organism->theoretical_maximum_base_speed(),
        previous_organism->theoretical_maximum_base_stamina(),
        previous_organism->theoretical_maximum_base_vitality(),
        previous_organism->theoretical_maximum_base_weight(),
        previous_organism->theoretical_maximum_height(),
        previous_organism->theoretical_maximum_speed(),
        previous_organism->theoretical_maximum_weight(),
        previous_organism->theoretical_maximum_height_multiplier(),
        previous_organism->theoretical_maximum_speed_multiplier(),
        previous_organism->theoretical_maximum_stamina_multiplier(),
        previous_organism->theoretical_maximum_vitality_multiplier(),
        previous_organism->theoretical_maximum_weight_multiplier(),
        builder.CreateString(previous_organism->name()),
        builder.CreateVector(previous_organism->chromosome()->data(),
                             previous_organism->chromosome()->size()),
        previous_organism->gender(), previous_organism->generation(),
        previous_organism->immunity(), previous_organism->base_appetite(),
        previous_organism->base_height(), previous_organism->base_speed(),
        previous_organism->base_stamina(), previous_organism->base_vitality(),
        previous_organism->base_weight(),
        previous_organism->height_multiplier(),
        previous_organism->speed_multiplier(),
        previous_organism->stamina_multiplier(),
        previous_organism->vitality_multiplier(),
        previous_organism->weight_multiplier(), previous_organism->max_height(),
        previous_organism->max_weight(), previous_organism->age(),
        previous_organism->height(), previous_organism->weight(),
        previous_organism->static_fitness(),
        previous_organism->max_appetite_at_age(),
        previous_organism->max_speed_at_age(),
        previous_organism->max_stamina_at_age(),
        previous_organism->max_vitality_at_age(), previous_organism->appetite(),
        previous_organism->speed(), previous_organism->stamina(),
        previous_organism->vitality(), previous_organism->X(),
        previous_organism->Y(), previous_organism->dynamic_fitness(),
        previous_organism->vision_radius(),
        previous_organism->sleep_restore_factor(), previous_organism->asleep(),
        previous_organism->monitor());
}

void God::happy_new_year(const bool &log) {
    uint32_t spawn_count = 0;
    uint32_t recent_births = 0;
    uint32_t recent_deaths = 0;
    uint32_t recent_population = 0;

    Ecosystem::World *previous_world =
        Ecosystem::GetMutableWorld(buffer.data());

    std::vector<flatbuffers::Offset<Ecosystem::Species>> newStdvecSpecies;

    uint32_t num_species = previous_world->species()->size();

    std::mt19937_64 rng{std::random_device()()};

    for (uint32_t n : rv::iota(0u, num_species)) {
        Ecosystem::Species *species =
            previous_world->mutable_species()->GetMutableObject(n);
        auto kingdom = species->kingdom();
        auto kind = species->kind();

        std::string full_species_name =
            std::string(Ecosystem::EnumNameKingdomE(kingdom)) + "/" +
            kind->str();

        std::vector<flatbuffers::Offset<Ecosystem::Organism>> stdvecOrganisms;

        /***************************************************
         *       Annual Killing (un-selection) Begins      *
         ***************************************************/

        // Vector for [ (death_factor, index in vector) ]

        std::vector<std::pair<float, uint32_t>> organisms_vec;
        organisms_vec.reserve(species->organism()->size());

        for (uint32_t i : rv::iota(0u, species->organism()->size())) {
            const Ecosystem::Organism *organism = species->organism()->Get(i);
            const float death_factor =
                organism_opts::generate_death_factor(organism);
            organisms_vec.emplace_back(std::make_pair(death_factor, i));
        }

        // Sort organism_vec by death factor

        std::sort(organisms_vec.begin(), organisms_vec.end(),
                  std::greater<std::pair<float, uint32_t>>());

        std::uniform_real_distribution<double> par_dis(0.0, 1.0);
        std::mt19937_64 par_rng{std::random_device()()};

        for (uint32_t index : rv::iota(0u, organisms_vec.size())) {
            const double x = par_dis(par_rng);

            // Spare this organism
            if (disable_deaths ||
                x >= killer_function(index, organisms_vec.size())) {
                // Annual ageing of this Organism
                organism_opts::increment_age(
                    species->mutable_organism()->GetMutableObject(
                        organisms_vec[index].second));

                auto new_organism = clone_organism(
                    builder,
                    species->organism()->Get(organisms_vec[index].second));
                stdvecOrganisms.push_back(new_organism);
            } else {
                recent_deaths++;
            }
        }

        /***************************************************
         *       Annual Mating (spawning) Begins      *
         ***************************************************/

        if (stdvecOrganisms.size() > 0) {
            update_species(full_species_name);
            const auto &species_constants =
                constants::get_species_constants_map()[kind->str()];

            std::vector<uint32_t> mating_list1, mating_list2;

            for (size_t index : rv::iota(0u, stdvecOrganisms.size())) {
                auto desparate_organism = helper::get_pointer_from_offset(
                    builder, stdvecOrganisms[index]);
                auto sexuality = desparate_organism->sexuality();

                if (sexuality == Ecosystem::Reproduction::asexual) {
                    if (desparate_organism->age() >=
                            desparate_organism->mating_age_start() &&
                        desparate_organism->age() <=
                            desparate_organism->mating_age_end()) {
                        mating_list1.push_back(index);
                    }
                } else if (sexuality == Ecosystem::Reproduction::sexual) {
                    if (desparate_organism->age() >=
                            desparate_organism->mating_age_start() &&
                        desparate_organism->age() <=
                            desparate_organism->mating_age_end()) {
                        if (desparate_organism->gender() ==
                            Ecosystem::Gender::male) {
                          mating_list1.push_back(index);
                        } else {
                          mating_list2.push_back(index);
                        }
                    }
                }
            }

            if (mating_list1.size() > 0) {
                std::shuffle(mating_list1.begin(), mating_list1.end(), rng);
                std::shuffle(mating_list2.begin(), mating_list2.end(), rng);

                uint32_t index_parent = 0;
                Ecosystem::Reproduction sexuality =
                    helper::get_pointer_from_offset(
                        builder, stdvecOrganisms[mating_list1[index_parent]])
                        ->sexuality();

                while ((mating_list1.size() > index_parent &&
                        mating_list2.size() > index_parent &&
                        sexuality == Ecosystem::Reproduction::sexual) ||
                       (mating_list1.size() > index_parent &&
                        sexuality == Ecosystem::Reproduction::asexual)) {
                    /*
                       BIG CAUTION HERE!!! Please use flatbuffer object API for
                       holding intermediate data only. Don't use for
                       serializing/deserializing entire buffers, else there will
                       be a big drop in performance
                       */

                    Ecosystem::OrganismT parent1, parent2;

                    {
                        const Ecosystem::Organism *parent1_ptr =
                            helper::get_pointer_from_offset(
                                builder,
                                stdvecOrganisms[mating_list1[index_parent]]);
                        const Ecosystem::Organism *parent2_ptr =
                            sexuality == Ecosystem::Reproduction::sexual
                                ? helper::get_pointer_from_offset(
                                      builder, stdvecOrganisms
                                                   [mating_list2[index_parent]])
                                : parent1_ptr;

                        /* Assign copies of data as the parent pointers get
                         * shifted during organism creation */

                        parent1.mating_probability =
                            parent1_ptr->mating_probability();
                        parent1.offsprings_factor =
                            parent1_ptr->offsprings_factor();
                        parent1.conceiving_probability =
                            parent1_ptr->conceiving_probability();
                        parent1.monitor = parent1_ptr->monitor();
                        parent1.X = parent1_ptr->X();
                        parent1.Y = parent1_ptr->Y();
                        parent1.kind = parent1_ptr->kind()->str();
                        parent1.generation = parent1_ptr->generation();
                        parent1.mutation_probability =
                            parent1_ptr->mutation_probability();
                        size_t chromosome1_size =
                            parent1_ptr->chromosome()->size();
                        parent1.chromosome.reserve(chromosome1_size);
                        std::copy(parent1_ptr->chromosome()->data(),
                                  parent1_ptr->chromosome()->data() +
                                      chromosome1_size,
                                  std::back_inserter(parent1.chromosome));
                        parent1.chromosome_number =
                            parent1_ptr->chromosome_number();

                        parent2.mating_probability =
                            parent2_ptr->mating_probability();
                        parent2.offsprings_factor =
                            parent2_ptr->offsprings_factor();
                        parent2.conceiving_probability =
                            parent2_ptr->conceiving_probability();
                        parent2.monitor = parent2_ptr->monitor();
                        parent2.X = parent2_ptr->X();
                        parent2.Y = parent2_ptr->Y();
                        parent2.kind = parent2_ptr->kind()->str();
                        parent2.generation = parent2_ptr->generation();
                        parent2.mutation_probability =
                            parent2_ptr->mutation_probability();
                        size_t chromosome2_size =
                            parent2_ptr->chromosome()->size();
                        parent2.chromosome.reserve(chromosome2_size);
                        std::copy(parent2_ptr->chromosome()->data(),
                                  parent2_ptr->chromosome()->data() +
                                      chromosome1_size,
                                  std::back_inserter(parent2.chromosome));
                        parent2.chromosome_number =
                            parent2_ptr->chromosome_number();
                    }

                    if (helper::weighted_prob(
                            std::min(parent1.mating_probability,
                                     parent2.mating_probability))) {
                        int n_children = creator_function(
                            std::min(parent1.offsprings_factor,
                                     parent2.offsprings_factor));

                        while (n_children--) {
                            if (!helper::weighted_prob(
                                    std::min(parent1.conceiving_probability,
                                             parent2.conceiving_probability)))
                                continue;  // Conceiving probability too low
                                           // this time

                            auto child_chromosome = get_child_chromosome(
                                parent1, parent2, species_constants);
                            bool monitor_child =
                                monitor_offsprings &&
                                (static_cast<bool>(parent1.monitor) ||
                                 static_cast<bool>(parent2.monitor));

                            uint64_t child_X = (parent1.X + parent2.X) / 2,
                                     child_Y = (parent1.Y + parent2.Y) / 2;
                            std::string child_name = fmt::format(
                                "{}-{}-{}", kind->str(), year, spawn_count++);

                            auto child_offset = createOrganism(
                                builder, parent1.kind,
                                std::to_string(static_cast<uint8_t>(kingdom)),
                                1, child_name, child_chromosome,
                                std::max(parent1.generation,
                                         parent2.generation) +
                                    1,
                                std::make_pair(child_X, child_Y),
                                monitor_child);

                            const Ecosystem::Organism *child_ptr =
                                helper::get_pointer_from_offset(builder,
                                                                child_offset);

                            // Skip abnormal children (with weird stats)
                            if (organism_opts::is_normal_child(child_ptr)) {
                                stdvecOrganisms.push_back(child_offset);
                                recent_births++;
                            }
                        }
                    }
                    index_parent++;
                }
            }
        }

        recent_population += stdvecOrganisms.size();

        newStdvecSpecies.push_back(Ecosystem::CreateSpecies(
            builder, builder.CreateString(kind), kingdom,
            builder.CreateVectorOfSortedTables(stdvecOrganisms.data(),
                                               stdvecOrganisms.size())));
    }

    flatbuffers::Offset<
        flatbuffers::Vector<flatbuffers::Offset<Ecosystem::Species>>>
        species_vec = builder.CreateVectorOfSortedTables(
            newStdvecSpecies.data(), newStdvecSpecies.size());

    builder.Finish(Ecosystem::CreateWorld(builder, year, species_vec));

    /***************************
     *       World Updated     *
     ***************************/

    buffer = builder.Release();
    builder.Clear();

    /*********************
     *       Logging     *
     *********************/

    if (log) {
        fmt::print(
            "Year: {} - Recent births: {} - Recent deaths: {} - Population: "
            "{}\n",
            year, recent_births, recent_deaths, recent_population);
    }

    /* Create the avg buffer everytime. This would be used anyway for plotting
       from Flutter. This avoids repeated redundant calls to create_avg_world */

    avg_buffer = stat_fetcher::create_avg_world(buffer);

    if (gods_eye) {
        // Save average stats and population for every species in DB
        FBuffer world_population = stat_fetcher::get_population_stats(buffer);
        std::vector<std::vector<FBufferView>> rows(1);
        rows[0].emplace_back(FBufferView(avg_buffer.data(), avg_buffer.size()));
        rows[0].emplace_back(
            FBufferView(world_population.data(), world_population.size()));
        db->insert_rows(rows);
    }

    year++;
}
