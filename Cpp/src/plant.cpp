#include <plant.hpp>

Plant::Plant(const std::string &kind, const unsigned int &age, const bool &monitor_in_simulation, const std::string &chromosome, const unsigned int &generation, const std::string &name, const std::pair<unsigned int, unsigned int> &XY, const nlohmann::json &species_constants)
{
    this->monitor_in_simulation = monitor_in_simulation;
    this->kind = kind;
    this->kingdom = "plant";
    this->full_species_name = get_kingdom() + "/" + kind;

    this->generation = generation;

    if (name.length() == 0)
    {
        this->name = kind + "-" + helper::random_name(16);
    }
    else
        this->name = name;

    if (species_constants.empty())
    {
        nlohmann::json tmp = constants::species_constants_map[kind];
        init_from_json(tmp);
    }
    else
    {
        init_from_json(species_constants);
    }

    this->vitality = this->max_vitality_at_age;

    std::tie(this->X, this->Y) = helper::random_location();

    this->age = age - 1;
    increment_age();

    evaluate_static_fitness();
    evaluate_dynamic_fitness();
}

Plant::~Plant()
{
}

std::shared_ptr<Entity> Plant::clone() const
{
    return std::make_shared<Plant>();
}

std::shared_ptr<Entity> Plant::clone(
    const std::string &kind,
    const unsigned int &age,
    const bool &monitor_in_simulation,
    const std::string &chromosome,
    const unsigned int &generation,
    const std::string &name,
    const std::pair<unsigned int, unsigned int> &XY,
    const nlohmann::json &species_constants) const
{
    return std::make_shared<Plant>(kind, age, monitor_in_simulation, chromosome, generation, name, XY, species_constants);
}

void Plant::init_from_json(const nlohmann::json &json_file)
{
    this->chromosome_structure = CHROMOSOME_MAP_TYPE(json_file["chromosome_structure"]);
    this->chromosome_number = json_file["species_chromosome_number"];

    this->mating_age_start = json_file["mating_age_start"];
    this->mating_age_end = json_file["mating_age_end"];
    this->max_age = json_file["species_max_age"];
    this->offsprings_factor = json_file["offsprings_factor"];
    this->is_asexual = json_file["is_asexual"];

    this->mutation_probability = json_file["mutation_probability"];
    this->conceiving_probability = json_file["conceiving_probability"];
    this->mating_probability = json_file["mating_probability"];

    this->age_fitness_on_death_ratio = json_file["species_age_fitness_on_death_ratio"];

    this->height_on_vitality = json_file["species_height_on_vitality"];
    this->weight_on_vitality = json_file["species_weight_on_vitality"];

    this->theoretical_maximum_base_height = json_file["species_theoretical_maximum_base_height"];
    this->theoretical_maximum_base_vitality = json_file["species_theoretical_maximum_base_vitality"];
    this->theoretical_maximum_base_weight = json_file["species_theoretical_maximum_base_weight"];

    this->theoretical_maximum_height_multiplier = json_file["species_theoretical_maximum_height_multiplier"];
    this->theoretical_maximum_vitality_multiplier = json_file["species_theoretical_maximum_vitality_multiplier"];
    this->theoretical_maximum_weight_multiplier = json_file["species_theoretical_maximum_weight_multiplier"];

    this->theoretical_maximum_height = json_file["species_theoretical_maximum_height"];
    this->theoretical_maximum_weight = json_file["species_theoretical_maximum_weight"];

    this->food_chain_rank = json_file["food_chain_rank"];

    if (chromosome.length() == 0)
    {
        unsigned int chromosome_length = 0;

        // Do not disturb this loop...

        for (auto i = chromosome_structure.begin(); i != chromosome_structure.end(); i++)
        {
            chromosome_length += i->second["length"];
        }
        this->chromosome = helper::random_binary(chromosome_length);
    }
    else
    {
        this->chromosome = chromosome;
    }

    this->immunity = get_immunity_from_chromosome();
    this->gender = get_gender_from_chromosome();

    this->max_vitality_at_age = get_base_vitality();

    this->height = get_base_height();
    this->weight = get_base_weight();
}

unsigned int Plant::get_gender_from_chromosome() const
{
    return static_cast<unsigned int>(helper::get_value_from_chromosome(chromosome,
                      this->chromosome_structure.at("gn").at("start"),
                      this->chromosome_structure.at("gn").at("length"),
                      2.0));
}

double Plant::get_immunity_from_chromosome() const
{
    return helper::get_value_from_chromosome(chromosome,
                      this->chromosome_structure.at("im").at("start"),
                      this->chromosome_structure.at("im").at("length"),
                      1.0);
}

double Plant::get_base_height() const
{
    return helper::get_value_from_chromosome(chromosome,
                      this->chromosome_structure.at("bh").at("start"),
                      this->chromosome_structure.at("bh").at("length"),
                      theoretical_maximum_base_height);
}

double Plant::get_base_vitality() const
{
    return helper::get_value_from_chromosome(chromosome,
                      this->chromosome_structure.at("bv").at("start"),
                      this->chromosome_structure.at("bv").at("length"),
                      theoretical_maximum_base_vitality);
}

double Plant::get_base_weight() const
{
    return helper::get_value_from_chromosome(chromosome,
                      this->chromosome_structure.at("bw").at("start"),
                      this->chromosome_structure.at("bw").at("length"),
                      theoretical_maximum_base_weight);
}

double Plant::get_height_multiplier() const
{
    return helper::get_value_from_chromosome(chromosome,
                      this->chromosome_structure.at("hm").at("start"),
                      this->chromosome_structure.at("hm").at("length"),
                      theoretical_maximum_height_multiplier);
}

double Plant::get_vitality_multiplier() const
{
    return helper::get_value_from_chromosome(chromosome,
                      this->chromosome_structure.at("vm").at("start"),
                      this->chromosome_structure.at("vm").at("length"),
                      theoretical_maximum_vitality_multiplier);
}

double Plant::get_weight_multiplier() const
{
    return helper::get_value_from_chromosome(chromosome,
                      this->chromosome_structure.at("wm").at("start"),
                      this->chromosome_structure.at("wm").at("length"),
                      theoretical_maximum_weight_multiplier);
}

double Plant::get_max_height() const
{
    return helper::get_value_from_chromosome(chromosome,
                      this->chromosome_structure.at("mh").at("start"),
                      this->chromosome_structure.at("mh").at("length"),
                      theoretical_maximum_height);
}

double Plant::get_max_weight() const
{
    return helper::get_value_from_chromosome(chromosome,
                      this->chromosome_structure.at("mw").at("start"),
                      this->chromosome_structure.at("mw").at("length"),
                      theoretical_maximum_weight);
}

void Plant::evaluate_dynamic_fitness()
{
    dynamic_fitness = 1;
}

void Plant::evaluate_static_fitness()
{
    static_fitness = immunity * ( 0.03 * max_vitality_at_age) / (3 * pow(age, 1.0 / 32));
}

void Plant::generate_death_factor()
{
    const double tmp = std::exp((std::log(1 / 199) / max_age) * age);
    age_death_factor = (1 - tmp) / (1 + tmp);

    fitness_death_factor = 1.0 / get_fitness();

    death_factor = helper::weighted_average({age_death_factor, fitness_death_factor}, {age_fitness_on_death_ratio, 1.0});
}

void Plant::increment_age()
{
    age += 1;

    // change height and weight independently

    height = std::min(std::max(0.5 * (1 + get_height_multiplier()) * log(age + 1) - pow(static_cast<double>(age) / max_age, 2) + get_base_height(),
                               get_base_height()),
                      get_max_height());
    weight = std::min(std::max(get_weight_multiplier() * max_age * log(age + 1) - (0.5 / max_age) * pow(age, 2 * (get_weight_multiplier() + 0.75)) + get_base_weight(),
                               get_base_weight()),
                      get_max_weight());

    // change stats independently

    max_vitality_at_age = get_base_vitality() * ( get_vitality_multiplier() * 0.5 * pow(max_age, 0.5) *
                                                  exp(-pow(age - max_age * 0.5, 2) / (max_age * get_base_vitality())) + 1);

    // change stats dependently

    max_vitality_at_age = max_vitality_at_age * (1 +
                                                 height_on_vitality * height / get_max_height() +
                                                 weight_on_vitality * weight / get_max_weight());
    evaluate_static_fitness();
}

void Plant::increment_vitality_by(const double &units)
{
    vitality = std::min(vitality + units, max_vitality_at_age);
}

void Plant::decrement_vitality_by(const double &units)
{
    vitality = std::max(vitality - units, 0.0);
}

bool Plant::is_normal_child() const
{

    std::vector<double> checklist {
        height,
        weight,
        immunity,
        max_vitality_at_age,
        get_base_height(),
        get_base_vitality(),
        get_base_weight(),
        get_max_height(),
        get_max_weight(),
        get_height_multiplier(),
        get_vitality_multiplier(),
        get_weight_multiplier()
    };

    for (const auto &i : checklist)
        if (!helper::is_nonzero_nonnan(i))
            return false;

    return true;
}

std::string Plant::get_kingdom() const
{
    return kingdom;
}

ATTRIBUTE_RAW_MAP& Plant::get_attribute_raw_map()
{
    return attribute_raw_map;
}
