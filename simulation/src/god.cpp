#include <flatbuffers/minireflect.h>
#include <fmt/core.h>

#include <chrono>
#include <god.hpp>
#include <helper.hpp>
#include <nlohmann/json.hpp>
#include <organism.hpp>
#include <profiler.hpp>
#include <set>
#include <species_constants.hpp>
#include <stat_fetcher.hpp>

static XoshiroCpp::Xoshiro128PlusPlus& get_rng() {
    static XoshiroCpp::Xoshiro128PlusPlus rng_instance{
        helper::benchmark_seed != 0
            ? helper::benchmark_seed
            : std::random_device()()};
    return rng_instance;
}
#define rng get_rng()

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

static ChrDecodeInfo extract_chr_info(
    const std::map<std::string, std::map<std::string, int>> &cs,
    const std::string &code) {
    ChrDecodeInfo info;
    auto it = cs.find(code);
    if (it != cs.end()) {
        info.start = it->second.find("start")->second;
        info.length = it->second.find("length")->second;
    }
    return info;
}

static SpeciesConstants extract_species_constants(const nlohmann::json &sp) {
    SpeciesConstants sc;
    sc.chromosome_number = getValueAsUshort(sp, "species_chromosome_number");
    sc.food_chain_rank = getValueAsUlong(sp, "food_chain_rank");
    sc.sexuality = getValueAsByte(sp, "sexuality");
    sc.age_fitness_on_death_ratio = getValueAsFloat(sp, "species_age_fitness_on_death_ratio");
    sc.conceiving_probability = getValueAsFloat(sp, "conceiving_probability");
    sc.mating_probability = getValueAsFloat(sp, "mating_probability");
    sc.mating_age_start = getValueAsUint(sp, "mating_age_start");
    sc.mating_age_end = getValueAsUint(sp, "mating_age_end");
    sc.species_max_age = getValueAsUint(sp, "species_max_age");
    sc.mutation_probability = getValueAsFloat(sp, "mutation_probability");
    sc.offsprings_factor = getValueAsFloat(sp, "offsprings_factor");
    sc.height_on_speed = getValueAsFloat(sp, "species_height_on_speed");
    sc.height_on_stamina = getValueAsFloat(sp, "species_height_on_stamina");
    sc.height_on_vitality = getValueAsFloat(sp, "species_height_on_vitality");
    sc.weight_on_speed = getValueAsFloat(sp, "species_weight_on_speed");
    sc.weight_on_stamina = getValueAsFloat(sp, "species_weight_on_stamina");
    sc.weight_on_vitality = getValueAsFloat(sp, "species_weight_on_vitality");
    sc.vitality_on_appetite = getValueAsFloat(sp, "species_vitality_on_appetite");
    sc.vitality_on_speed = getValueAsFloat(sp, "species_vitality_on_speed");
    sc.stamina_on_appetite = getValueAsFloat(sp, "species_stamina_on_appetite");
    sc.stamina_on_speed = getValueAsFloat(sp, "species_stamina_on_speed");
    sc.theoretical_maximum_base_appetite = getValueAsFloat(sp, "species_theoretical_maximum_base_appetite");
    sc.theoretical_maximum_base_height = getValueAsFloat(sp, "species_theoretical_maximum_base_height");
    sc.theoretical_maximum_base_speed = getValueAsFloat(sp, "species_theoretical_maximum_base_speed");
    sc.theoretical_maximum_base_stamina = getValueAsFloat(sp, "species_theoretical_maximum_base_stamina");
    sc.theoretical_maximum_base_vitality = getValueAsFloat(sp, "species_theoretical_maximum_base_vitality");
    sc.theoretical_maximum_base_weight = getValueAsFloat(sp, "species_theoretical_maximum_base_weight");
    sc.theoretical_maximum_height = getValueAsFloat(sp, "species_theoretical_maximum_height");
    sc.theoretical_maximum_speed = getValueAsFloat(sp, "species_theoretical_maximum_speed");
    sc.theoretical_maximum_weight = getValueAsFloat(sp, "species_theoretical_maximum_weight");
    sc.theoretical_maximum_height_multiplier = getValueAsFloat(sp, "species_theoretical_maximum_height_multiplier");
    sc.theoretical_maximum_speed_multiplier = getValueAsFloat(sp, "species_theoretical_maximum_speed_multiplier");
    sc.theoretical_maximum_stamina_multiplier = getValueAsFloat(sp, "species_theoretical_maximum_stamina_multiplier");
    sc.theoretical_maximum_vitality_multiplier = getValueAsFloat(sp, "species_theoretical_maximum_vitality_multiplier");
    sc.theoretical_maximum_weight_multiplier = getValueAsFloat(sp, "species_theoretical_maximum_weight_multiplier");
    sc.vision_radius = getValueAsFloat(sp, "vision_radius");
    sc.sleep_restore_factor = getValueAsFloat(sp, "species_sleep_restore_factor");
    sc.chromosome_structure = sp["chromosome_structure"];

    const auto &cs_map = sc.chromosome_structure;
    sc.chr_gn = extract_chr_info(cs_map, "gn");
    sc.chr_im = extract_chr_info(cs_map, "im");
    sc.chr_ba = extract_chr_info(cs_map, "ba");
    sc.chr_bh = extract_chr_info(cs_map, "bh");
    sc.chr_bp = extract_chr_info(cs_map, "bp");
    sc.chr_bs = extract_chr_info(cs_map, "bs");
    sc.chr_bv = extract_chr_info(cs_map, "bv");
    sc.chr_bw = extract_chr_info(cs_map, "bw");
    sc.chr_hm = extract_chr_info(cs_map, "hm");
    sc.chr_pm = extract_chr_info(cs_map, "pm");
    sc.chr_sm = extract_chr_info(cs_map, "sm");
    sc.chr_vm = extract_chr_info(cs_map, "vm");
    sc.chr_wm = extract_chr_info(cs_map, "wm");
    sc.chr_mh = extract_chr_info(cs_map, "mh");
    sc.chr_mw = extract_chr_info(cs_map, "mw");

    return sc;
}

static flatbuffers::Offset<flatbuffers::Vector<uint8_t>> create_chromosome_vector(
    flatbuffers::FlatBufferBuilder &builder, const std::string &binary_str) {
    size_t len = binary_str.length();
    size_t padded_len = len % 8 == 0 ? len : len + 8 - (len % 8);
    size_t byte_count = padded_len / 8;
    std::vector<uint8_t> bytes(byte_count, 0);
    size_t offset = padded_len - len;
    for (size_t i = 0; i < len; i++) {
        if (binary_str[i] == '1') {
            size_t bit_pos = offset + i;
            size_t byte_idx = bit_pos / 8;
            int bit_idx = 7 - (bit_pos % 8);
            bytes[byte_idx] |= (1 << bit_idx);
        }
    }
    return builder.CreateVector(bytes.data(), bytes.size());
}

static double get_value_from_chromosome(
    const std::vector<uint8_t> &chromosome,
    const std::map<std::string, std::map<std::string, int>> &c_structure,
    const std::string &code, const double &multiplier,
    const uint16_t &chromosome_number) {
    auto start = std::chrono::steady_clock::now();

    auto it = c_structure.find(code);
    if (it == c_structure.end()) return 0;
    std::string chromosome_str = helper::bytevector_to_string(
        chromosome.data(), chromosome.size(), chromosome_number);
    int start_pos = it->second.find("start")->second;
    int len = it->second.find("length")->second;
    if (len == 0) return 0;
    double result = (helper::to_decimal(chromosome_str.substr(start_pos, len)) /
            static_cast<double>(1 << len)) *
           multiplier;

    if (helper::active_perf) {
        auto end = std::chrono::steady_clock::now();
        helper::active_perf->chromosome_decode_us +=
            std::chrono::duration<double, std::micro>(end - start).count();
        helper::active_perf->chromosome_decode_calls++;
    }

    return result;
}

static double decode_chr_fast(
    const std::string &chromosome_str, const ChrDecodeInfo &info,
    const double &multiplier) {
    if (info.length == 0) return 0;
    return (helper::to_decimal(chromosome_str.substr(info.start, info.length)) /
            static_cast<double>(1 << info.length)) *
           multiplier;
}

static double decode_chromosome(
    const std::string &chromosome_str,
    const std::map<std::string, std::map<std::string, int>> &c_structure,
    const std::string &code, const double &multiplier) {
    auto it = c_structure.find(code);
    if (it == c_structure.end()) return 0;
    int start_pos = it->second.find("start")->second;
    int len = it->second.find("length")->second;
    if (len == 0) return 0;
    return (helper::to_decimal(chromosome_str.substr(start_pos, len)) /
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
    if (benchmark_seed != 0) {
        helper::benchmark_seed = benchmark_seed;
    }

    fmt::print("God created!\n");
}

God::~God() {
    perf.print_cumulative();
    fmt::print("God is dead...\n");
}

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

    const auto &sp = constants::get_species_constants_map()[kind];

    std::vector<flatbuffers::Offset<Ecosystem::ChromosomeStrand>> stdvecCStrand;
    std::map<std::string, std::map<std::string, int>> c_structure =
        sp["chromosome_structure"];
    for (auto &cStrand : c_structure) {
        stdvecCStrand.push_back(Ecosystem::CreateChromosomeStrand(
            builder, builder.CreateString(cStrand.first.c_str()),
            cStrand.second["start"], cStrand.second["length"]));
    }

    std::string tmp_str;
    tmp_str = name.length() != 0
                  ? name
                  : fmt::format("{}-orphan-{}", kind, helper::random_name(16));
    auto organism_name = builder.CreateString(tmp_str.c_str());
    tmp_str.clear();

    uint16_t chr_num = getValueAsUshort(sp, "species_chromosome_number");
    tmp_str = chromosome.empty()
                  ? helper::random_binary((int)chr_num)
                  : chromosome;
    std::vector<uint8_t> chromosome_vec = helper::string_to_bytevector(tmp_str);
    tmp_str.clear();
    auto organism_chromosome =
        builder.CreateVector(chromosome_vec.data(), chromosome_vec.size());

    std::string chr_str = helper::bytevector_to_string(
        chromosome_vec.data(), chromosome_vec.size(), chr_num);

    auto organism_offset = Ecosystem::CreateOrganism(
        builder, builder.CreateString(kind.c_str()),
        (Ecosystem::KingdomE)std::stoi(kingdom),
        chr_num,
        builder.CreateVectorOfSortedTables(stdvecCStrand.data(),
                                           stdvecCStrand.size()),
        getValueAsUlong(sp, "food_chain_rank"),
        (Ecosystem::Reproduction)getValueAsByte(sp, "sexuality"),
        getValueAsFloat(sp, "species_age_fitness_on_death_ratio"),
        getValueAsFloat(sp, "conceiving_probability"),
        getValueAsFloat(sp, "mating_probability"),
        getValueAsUint(sp, "mating_age_start"),
        getValueAsUint(sp, "mating_age_end"),
        getValueAsUint(sp, "species_max_age"),
        getValueAsFloat(sp, "mutation_probability"),
        getValueAsFloat(sp, "offsprings_factor"),
        getValueAsFloat(sp, "species_height_on_speed"),
        getValueAsFloat(sp, "species_height_on_stamina"),
        getValueAsFloat(sp, "species_height_on_vitality"),
        getValueAsFloat(sp, "species_weight_on_speed"),
        getValueAsFloat(sp, "species_weight_on_stamina"),
        getValueAsFloat(sp, "species_weight_on_vitality"),
        getValueAsFloat(sp, "species_vitality_on_appetite"),
        getValueAsFloat(sp, "species_vitality_on_speed"),
        getValueAsFloat(sp, "species_stamina_on_appetite"),
        getValueAsFloat(sp, "species_stamina_on_speed"),
        getValueAsFloat(sp, "species_theoretical_maximum_base_appetite"),
        getValueAsFloat(sp, "species_theoretical_maximum_base_height"),
        getValueAsFloat(sp, "species_theoretical_maximum_base_speed"),
        getValueAsFloat(sp, "species_theoretical_maximum_base_stamina"),
        getValueAsFloat(sp, "species_theoretical_maximum_base_vitality"),
        getValueAsFloat(sp, "species_theoretical_maximum_base_weight"),
        getValueAsFloat(sp, "species_theoretical_maximum_height"),
        getValueAsFloat(sp, "species_theoretical_maximum_speed"),
        getValueAsFloat(sp, "species_theoretical_maximum_weight"),
        getValueAsFloat(sp, "species_theoretical_maximum_height_multiplier"),
        getValueAsFloat(sp, "species_theoretical_maximum_speed_multiplier"),
        getValueAsFloat(sp, "species_theoretical_maximum_stamina_multiplier"),
        getValueAsFloat(sp, "species_theoretical_maximum_vitality_multiplier"),
        getValueAsFloat(sp, "species_theoretical_maximum_weight_multiplier"),
        organism_name, organism_chromosome,
        (Ecosystem::Gender)decode_chromosome(chr_str, c_structure, "gn", 2.0),
        generation,
        decode_chromosome(chr_str, c_structure, "im", 1.0),
        decode_chromosome(chr_str, c_structure, "ba",
            getValueAsFloat(sp, "species_theoretical_maximum_base_appetite")),
        decode_chromosome(chr_str, c_structure, "bh",
            getValueAsFloat(sp, "species_theoretical_maximum_base_height")),
        decode_chromosome(chr_str, c_structure, "bp",
            getValueAsFloat(sp, "species_theoretical_maximum_base_speed")),
        decode_chromosome(chr_str, c_structure, "bs",
            getValueAsFloat(sp, "species_theoretical_maximum_base_stamina")),
        decode_chromosome(chr_str, c_structure, "bv",
            getValueAsFloat(sp, "species_theoretical_maximum_base_vitality")),
        decode_chromosome(chr_str, c_structure, "bw",
            getValueAsFloat(sp, "species_theoretical_maximum_base_weight")),
        decode_chromosome(chr_str, c_structure, "hm",
            getValueAsFloat(sp, "species_theoretical_maximum_height_multiplier")),
        decode_chromosome(chr_str, c_structure, "pm",
            getValueAsFloat(sp, "species_theoretical_maximum_speed_multiplier")),
        decode_chromosome(chr_str, c_structure, "sm",
            getValueAsFloat(sp, "species_theoretical_maximum_stamina_multiplier")),
        decode_chromosome(chr_str, c_structure, "vm",
            getValueAsFloat(sp, "species_theoretical_maximum_vitality_multiplier")),
        decode_chromosome(chr_str, c_structure, "wm",
            getValueAsFloat(sp, "species_theoretical_maximum_weight_multiplier")),
        decode_chromosome(chr_str, c_structure, "mh",
            getValueAsFloat(sp, "species_theoretical_maximum_height")),
        decode_chromosome(chr_str, c_structure, "mw",
            getValueAsFloat(sp, "species_theoretical_maximum_weight")),
        age - 1,
        decode_chromosome(chr_str, c_structure, "bh",
            getValueAsFloat(sp, "species_theoretical_maximum_base_height")),
        decode_chromosome(chr_str, c_structure, "bw",
            getValueAsFloat(sp, "species_theoretical_maximum_base_weight")),
        0.0,
        decode_chromosome(chr_str, c_structure, "ba",
            getValueAsFloat(sp, "species_theoretical_maximum_base_appetite")),
        decode_chromosome(chr_str, c_structure, "bp",
            getValueAsFloat(sp, "species_theoretical_maximum_base_speed")),
        decode_chromosome(chr_str, c_structure, "bs",
            getValueAsFloat(sp, "species_theoretical_maximum_base_stamina")),
        decode_chromosome(chr_str, c_structure, "bv",
            getValueAsFloat(sp, "species_theoretical_maximum_base_vitality")),
        decode_chromosome(chr_str, c_structure, "ba",
            getValueAsFloat(sp, "species_theoretical_maximum_base_appetite")),
        decode_chromosome(chr_str, c_structure, "bp",
            getValueAsFloat(sp, "species_theoretical_maximum_base_speed")),
        decode_chromosome(chr_str, c_structure, "bs",
            getValueAsFloat(sp, "species_theoretical_maximum_base_stamina")),
        decode_chromosome(chr_str, c_structure, "bv",
            getValueAsFloat(sp, "species_theoretical_maximum_base_vitality")),
        XY.first, XY.second, 1.0,
        getValueAsFloat(sp, "vision_radius"),
        getValueAsFloat(sp, "species_sleep_restore_factor"),
        Ecosystem::Sleep::awake, static_cast<Ecosystem::Monitor>(monitor));

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

flatbuffers::Offset<Ecosystem::Organism> God::createChildOrganism(
    flatbuffers::FlatBufferBuilder &builder,
    const SpeciesConstants &sc,
    const std::string &kingdom,
    const uint64_t &age,
    flatbuffers::Offset<flatbuffers::String> name_offset,
    const std::string &chromosome_str,
    flatbuffers::Offset<flatbuffers::Vector<uint8_t>> chromosome_offset,
    flatbuffers::Offset<flatbuffers::String> kind_offset,
    flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Ecosystem::ChromosomeStrand>>> chr_structure_offset,
    const uint64_t &generation,
    const std::pair<uint64_t, uint64_t> &XY,
    const int8_t &monitor) {
    const auto &c_str = chromosome_str;

    auto organism_offset = Ecosystem::CreateOrganism(
        builder, kind_offset,
        (Ecosystem::KingdomE)std::stoi(kingdom),
        sc.chromosome_number,
        chr_structure_offset,
        sc.food_chain_rank,
        (Ecosystem::Reproduction)sc.sexuality,
        sc.age_fitness_on_death_ratio,
        sc.conceiving_probability,
        sc.mating_probability,
        sc.mating_age_start,
        sc.mating_age_end,
        sc.species_max_age,
        sc.mutation_probability,
        sc.offsprings_factor,
        sc.height_on_speed,
        sc.height_on_stamina,
        sc.height_on_vitality,
        sc.weight_on_speed,
        sc.weight_on_stamina,
        sc.weight_on_vitality,
        sc.vitality_on_appetite,
        sc.vitality_on_speed,
        sc.stamina_on_appetite,
        sc.stamina_on_speed,
        sc.theoretical_maximum_base_appetite,
        sc.theoretical_maximum_base_height,
        sc.theoretical_maximum_base_speed,
        sc.theoretical_maximum_base_stamina,
        sc.theoretical_maximum_base_vitality,
        sc.theoretical_maximum_base_weight,
        sc.theoretical_maximum_height,
        sc.theoretical_maximum_speed,
        sc.theoretical_maximum_weight,
        sc.theoretical_maximum_height_multiplier,
        sc.theoretical_maximum_speed_multiplier,
        sc.theoretical_maximum_stamina_multiplier,
        sc.theoretical_maximum_vitality_multiplier,
        sc.theoretical_maximum_weight_multiplier,
        name_offset, chromosome_offset,
        (Ecosystem::Gender)decode_chr_fast(c_str, sc.chr_gn, 2.0),
        generation,
        decode_chr_fast(c_str, sc.chr_im, 1.0),
        decode_chr_fast(c_str, sc.chr_ba, sc.theoretical_maximum_base_appetite),
        decode_chr_fast(c_str, sc.chr_bh, sc.theoretical_maximum_base_height),
        decode_chr_fast(c_str, sc.chr_bp, sc.theoretical_maximum_base_speed),
        decode_chr_fast(c_str, sc.chr_bs, sc.theoretical_maximum_base_stamina),
        decode_chr_fast(c_str, sc.chr_bv, sc.theoretical_maximum_base_vitality),
        decode_chr_fast(c_str, sc.chr_bw, sc.theoretical_maximum_base_weight),
        decode_chr_fast(c_str, sc.chr_hm, sc.theoretical_maximum_height_multiplier),
        decode_chr_fast(c_str, sc.chr_pm, sc.theoretical_maximum_speed_multiplier),
        decode_chr_fast(c_str, sc.chr_sm, sc.theoretical_maximum_stamina_multiplier),
        decode_chr_fast(c_str, sc.chr_vm, sc.theoretical_maximum_vitality_multiplier),
        decode_chr_fast(c_str, sc.chr_wm, sc.theoretical_maximum_weight_multiplier),
        decode_chr_fast(c_str, sc.chr_mh, sc.theoretical_maximum_height),
        decode_chr_fast(c_str, sc.chr_mw, sc.theoretical_maximum_weight),
        age - 1,
        decode_chr_fast(c_str, sc.chr_bh, sc.theoretical_maximum_base_height),
        decode_chr_fast(c_str, sc.chr_bw, sc.theoretical_maximum_base_weight),
        0.0,
        decode_chr_fast(c_str, sc.chr_ba, sc.theoretical_maximum_base_appetite),
        decode_chr_fast(c_str, sc.chr_bp, sc.theoretical_maximum_base_speed),
        decode_chr_fast(c_str, sc.chr_bs, sc.theoretical_maximum_base_stamina),
        decode_chr_fast(c_str, sc.chr_bv, sc.theoretical_maximum_base_vitality),
        decode_chr_fast(c_str, sc.chr_ba, sc.theoretical_maximum_base_appetite),
        decode_chr_fast(c_str, sc.chr_bp, sc.theoretical_maximum_base_speed),
        decode_chr_fast(c_str, sc.chr_bs, sc.theoretical_maximum_base_stamina),
        decode_chr_fast(c_str, sc.chr_bv, sc.theoretical_maximum_base_vitality),
        XY.first, XY.second, 1.0,
        sc.vision_radius,
        sc.sleep_restore_factor,
        Ecosystem::Sleep::awake, static_cast<Ecosystem::Monitor>(monitor));

    Ecosystem::Organism *organism_ptr =
        helper::get_mutable_pointer_from_offset(builder, organism_offset);
    organism_opts::increment_age(organism_ptr);

    return organism_offset;
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

void God::update_species_cached(const std::string &full_species_name) {
    std::string kind =
        full_species_name.substr(full_species_name.find('/') + 1);

    auto it = species_modify_cache.find(kind);
    if (it == species_modify_cache.end()) {
        std::string kingdom =
            full_species_name.substr(0, full_species_name.find('/'));

        const std::filesystem::path modify_filepath =
            ecosystem_root / std::filesystem::path("data") /
            std::filesystem::path("json") / kingdom / kind /
            std::filesystem::path("modify.json");

        std::ifstream modify_in(modify_filepath);
        nlohmann::json modify;
        modify_in >> modify;
        modify_in.close();

        species_modify_cache[kind] = modify;
        it = species_modify_cache.find(kind);
    }

    for (const auto [key, value] : it->second.items()) {
        constants::get_species_constants_map()[kind][key] = updateStat(
            (double)constants::get_species_constants_map()[kind][key],
            (double)value);
    }
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

std::string God::get_child_chromosome(const std::string &chr1, uint16_t chr1_num,
                                      const std::string &chr2, uint16_t chr2_num,
                                      double mutation_prob) {
    auto child_chromosome = helper::get_random_mixture(chr1, chr2);

    for (auto &bit : child_chromosome)
        if (helper::weighted_prob(mutation_prob))
            bit = (bit == '1') ? '0' : '1';

    return child_chromosome;
}

flatbuffers::Offset<Ecosystem::Organism> God::clone_organism(
    flatbuffers::FlatBufferBuilder &builder,
    const Ecosystem::Organism *previous_organism,
    flatbuffers::Offset<flatbuffers::String> kind_offset,
    flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Ecosystem::ChromosomeStrand>>> chr_structure_offset) {
    return Ecosystem::CreateOrganism(
        builder, kind_offset,
        previous_organism->kingdom(), previous_organism->chromosome_number(),
        chr_structure_offset,
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

flatbuffers::Offset<Ecosystem::Organism> God::clone_organism(
    flatbuffers::FlatBufferBuilder &builder,
    const Ecosystem::Organism *previous_organism) {
    auto kind_offset = builder.CreateString(previous_organism->kind());

    std::vector<flatbuffers::Offset<Ecosystem::ChromosomeStrand>>
        newStdvecCStrand;
    for (const auto &cStrand : *previous_organism->chromosome_structure()) {
        newStdvecCStrand.push_back(Ecosystem::CreateChromosomeStrand(
            builder, builder.CreateString(cStrand->code()), cStrand->start(),
            cStrand->length()));
    }
    auto chr_structure_offset = builder.CreateVectorOfSortedTables(
        newStdvecCStrand.data(), newStdvecCStrand.size());

    return clone_organism(builder, previous_organism, kind_offset,
                          chr_structure_offset);
}

void God::happy_new_year(const bool &log) {
    perf.reset();
    ScopedPhaseTimer total_timer(perf.total_us);

    helper::active_perf = &perf;

    uint32_t spawn_count = 0;
    uint32_t recent_births = 0;
    uint32_t recent_deaths = 0;
    uint32_t recent_population = 0;

    Ecosystem::World *previous_world =
        Ecosystem::GetMutableWorld(buffer.data());

    std::vector<flatbuffers::Offset<Ecosystem::Species>> newStdvecSpecies;

    uint32_t num_species = previous_world->species()->size();

    for (uint32_t n = 0; n < num_species; n++) {
        perf.organisms_before += previous_world->species()->Get(n)->organism()->size();
    }

    uint64_t tick_seed = helper::benchmark_seed != 0
                             ? helper::benchmark_seed + year
                             : std::random_device()();
    std::mt19937_64 tick_rng{tick_seed};

    for (uint32_t n = 0; n < num_species; n++) {
        Ecosystem::Species *species =
            previous_world->mutable_species()->GetMutableObject(n);
        auto kingdom = species->kingdom();
        auto kind = species->kind();

        std::string full_species_name =
            std::string(Ecosystem::EnumNameKingdomE(kingdom)) + "/" +
            kind->str();

        std::vector<flatbuffers::Offset<Ecosystem::Organism>> stdvecOrganisms;

        auto kind_offset = builder.CreateString(kind->str());

        std::vector<flatbuffers::Offset<Ecosystem::ChromosomeStrand>>
            sharedChrStrand;
        if (species->organism()->size() > 0) {
            for (const auto &cStrand :
                 *species->organism()->Get(0)->chromosome_structure()) {
                sharedChrStrand.push_back(Ecosystem::CreateChromosomeStrand(
                    builder, builder.CreateString(cStrand->code()),
                    cStrand->start(), cStrand->length()));
            }
        }
        auto chr_structure_offset = builder.CreateVectorOfSortedTables(
            sharedChrStrand.data(), sharedChrStrand.size());

        /***************************************************
         *       Annual Killing (un-selection) Begins      *
         ***************************************************/

        {
            ScopedPhaseTimer select_timer(perf.select_us);

            std::vector<std::pair<float, uint32_t>> organisms_vec;
            organisms_vec.reserve(species->organism()->size());

            for (uint32_t i = 0; i < species->organism()->size(); i++) {
                const Ecosystem::Organism *organism = species->organism()->Get(i);
                const float death_factor =
                    organism_opts::generate_death_factor(organism);
                organisms_vec.emplace_back(std::make_pair(death_factor, i));
            }

            std::sort(organisms_vec.begin(), organisms_vec.end(),
                      std::greater<std::pair<float, uint32_t>>());

            std::uniform_real_distribution<double> par_dis(0.0, 1.0);
            std::mt19937_64 par_rng{tick_seed + 1};

            for (uint32_t index = 0; index < organisms_vec.size(); index++) {
                const double x = par_dis(par_rng);

                if (disable_deaths ||
                    x >= killer_function(index, organisms_vec.size())) {
                    ScopedPhaseTimer clone_timer(perf.clone_us);
                    organism_opts::increment_age(
                        species->mutable_organism()->GetMutableObject(
                            organisms_vec[index].second));

                    auto new_organism = clone_organism(
                        builder,
                        species->organism()->Get(organisms_vec[index].second),
                        kind_offset, chr_structure_offset);
                    stdvecOrganisms.push_back(new_organism);
                    perf.survivors++;
                } else {
                    recent_deaths++;
                }
            }
        }

        /***************************************************
         *       Annual Mating (spawning) Begins      *
         ***************************************************/

        {
            ScopedPhaseTimer mate_timer(perf.mate_us);

        if (stdvecOrganisms.size() > 0) {
            update_species_cached(full_species_name);
            const auto &species_json =
                constants::get_species_constants_map()[kind->str()];
            const std::string kind_str = kind->str();
            SpeciesConstants sc = extract_species_constants(species_json);

            std::vector<uint32_t> mating_list1, mating_list2;

            for (size_t index = 0; index < stdvecOrganisms.size(); index++) {
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
                std::shuffle(mating_list1.begin(), mating_list1.end(), tick_rng);
                std::shuffle(mating_list2.begin(), mating_list2.end(), tick_rng);

                uint32_t index_parent = 0;
                Ecosystem::Reproduction sexuality =
                    helper::get_pointer_from_offset(
                        builder, stdvecOrganisms[mating_list1[index_parent]])
                        ->sexuality();

                std::string kingdom_str = std::to_string(static_cast<uint8_t>(kingdom));

                while ((mating_list1.size() > index_parent &&
                        mating_list2.size() > index_parent &&
                        sexuality == Ecosystem::Reproduction::sexual) ||
                       (mating_list1.size() > index_parent &&
                        sexuality == Ecosystem::Reproduction::asexual)) {

                    ParentData parent1, parent2;

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
                        parent1.chromosome_number =
                            parent1_ptr->chromosome_number();
                        parent1.chromosome_str = helper::bytevector_to_string(
                            parent1_ptr->chromosome()->data(),
                            parent1_ptr->chromosome()->size(),
                            parent1.chromosome_number);

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
                        parent2.chromosome_number =
                            parent2_ptr->chromosome_number();
                        parent2.chromosome_str = helper::bytevector_to_string(
                            parent2_ptr->chromosome()->data(),
                            parent2_ptr->chromosome()->size(),
                            parent2.chromosome_number);
                    }

                    if (helper::weighted_prob(
                            std::min(parent1.mating_probability,
                                     parent2.mating_probability))) {
                        perf.mating_attempts++;
                        int n_children = creator_function(
                            std::min(parent1.offsprings_factor,
                                     parent2.offsprings_factor));

                        double max_mutation = std::max(parent1.mutation_probability,
                                                       parent2.mutation_probability);

                        while (n_children--) {
                            if (!helper::weighted_prob(
                                    std::min(parent1.conceiving_probability,
                                             parent2.conceiving_probability)))
                                continue;

                            auto child_chromosome = get_child_chromosome(
                                parent1.chromosome_str, parent1.chromosome_number,
                                parent2.chromosome_str, parent2.chromosome_number,
                                max_mutation);

                            bool monitor_child =
                                monitor_offsprings &&
                                (static_cast<bool>(parent1.monitor) ||
                                 static_cast<bool>(parent2.monitor));

                            uint64_t child_X = (parent1.X + parent2.X) / 2,
                                     child_Y = (parent1.Y + parent2.Y) / 2;

                            auto child_offset = createChildOrganism(
                                builder, sc,
                                kingdom_str,
                                1,
                                builder.CreateString(fmt::format(
                                    "{}-{}-{}", kind_str, year, spawn_count++)),
                                child_chromosome,
                                create_chromosome_vector(builder, child_chromosome),
                                kind_offset, chr_structure_offset,
                                std::max(parent1.generation,
                                         parent2.generation) +
                                    1,
                                std::make_pair(child_X, child_Y),
                                monitor_child);

                            const Ecosystem::Organism *child_ptr =
                                helper::get_pointer_from_offset(builder,
                                                                child_offset);

                            if (organism_opts::is_normal_child(child_ptr)) {
                                stdvecOrganisms.push_back(child_offset);
                                recent_births++;
                            } else {
                                perf.abnormal_rejects++;
                            }
                        }
                    }
                    index_parent++;
                }
            }
        }

        } // end mate_timer scope

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

    perf.buffer_bytes = buffer.size();
    perf.builder_bytes = builder.GetSize();

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

    {
        ScopedPhaseTimer stats_timer(perf.stats_us);
        avg_buffer = stat_fetcher::create_avg_world(buffer);
    }
    perf.avg_buffer_bytes = avg_buffer.size();

    if (gods_eye) {
        // Save average stats and population for every species in DB
        {
            ScopedPhaseTimer pop_timer(perf.pop_stats_us);
            population_stats = stat_fetcher::get_population_stats(buffer);
        }
        perf.population_stats_bytes = population_stats.size();

        {
            ScopedPhaseTimer db_timer(perf.db_us);
            std::vector<std::vector<FBufferView>> rows(1);
            rows[0].emplace_back(FBufferView(avg_buffer.data(), avg_buffer.size()));
            rows[0].emplace_back(
                FBufferView(population_stats.data(), population_stats.size()));
            db->insert_rows(rows);
        }
    }

    perf.deaths = recent_deaths;
    perf.births = recent_births;

    if (year % perf_log_interval == 0) {
        perf.print(year);
    }

    perf.accumulate();

    helper::active_perf = nullptr;

    year++;
}
