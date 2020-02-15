#include <animal.hpp>

Animal::Animal(const std::string& kind, const std::string& chromosome, const chromosome_map_type& chromosome_structure, const unsigned int& generation, const std::string& name)
{
    this->name = name;
    this->kind = kind;
    if(this->name == "")
    {
        this->name = kind + "-" + helper::random_name(16);
    }
    this->chromosome_structure = chromosome_structure;

    const std::string filepath = "../../data/json/" + kind + ".json";

    std::ifstream in(filepath);

    nlohmann::json json_file; in >> json_file;
    this->max_age = json_file["species_max_age"];
    this->chromosome_number = json_file["species_chromosome_number"];
    this->age_on_death = json_file["species_age_on_death"];
    this->fitness_on_death = json_file["species_fitness_on_death"];
    this->age_fitness_on_death_ratio = json_file["species_age_fitness_on_death_ratio"];
    this->height_on_vitality = json_file["species_height_on_vitality"];
    this->weight_on_vitality = json_file["species_weight_on_vitality"];
    this->height_on_stamina = json_file["species_height_on_stamina"];
    this->weight_on_stamina = json_file["species_weight_on_stamina"];
    this->height_on_speed = json_file["species_height_on_speed"];
    this->weight_on_speed = json_file["species_weight_on_speed"];
    this->vitality_on_appetite = json_file["species_vitality_on_appetite"];
    this->stamina_on_appetite = json_file["species_stamina_on_appetite"];
    this->vitality_on_speed = json_file["species_vitality_on_speed"];
    this->stamina_on_speed = json_file["species_stamina_on_speed"];
    this->sleep_restore_factor = json_file["species_sleep_restore_factor"];
    this->theoretical_maximum_base_vitality = json_file["species_theoretical_maximum_base_vitality"];
    this->theoretical_maximum_vitality_multiplier = json_file["species_theoretical_maximum_vitality_multiplier"];
    this->theoretical_maximum_base_stamina = json_file["species_theoretical_maximum_base_stamina"];
    this->theoretical_maximum_stamina_multiplier = json_file["species_theoretical_maximum_stamina_multiplier"];
    this->theoretical_maximum_height = json_file["species_theoretical_maximum_height"];
    this->theoretical_maximum_weight = json_file["species_theoretical_maximum_weight"];
    this->theoretical_maximum_base_height = json_file["species_theoretical_maximum_base_height"];
    this->theoretical_maximum_base_weight = json_file["species_theoretical_maximum_base_weight"];
    this->theoretical_maximum_height_multiplier = json_file["species_theoretical_maximum_height_multiplier"];
    this->theoretical_maximum_weight_multiplier = json_file["species_theoretical_maximum_weight_multiplier"];
    this->theoretical_maximum_speed = json_file["species_theoretical_maximum_speed"];
    this->theoretical_maximum_base_speed = json_file["species_theoretical_maximum_base_speed"];
    this->theoretical_maximum_speed_multiplier = json_file["species_theoretical_maximum_speed_multiplier"];
    this->theoretical_maximum_base_appetite = json_file["species_theoretical_maximum_base_appetite"];

    in.close();

    this->chromosome = chromosome;
    this->immunity = get_immunity();
    this->generation = generation;
    this->gender = get_gender();

    this->age = 0;
    this->max_vitality_at_age = get_base_vitality();
    this->max_stamina_at_age = get_base_stamina();
    this->max_speed_at_age = get_base_speed();
    this->max_appetite_at_age = get_base_appetite();
    this->height = get_base_height();
    this->weight = get_base_weight();

    this->vitality = this->max_vitality_at_age;
    this->stamina = this->max_stamina_at_age;
    this->appetite = this->max_appetite_at_age;
    this->speed = this->max_speed_at_age;

    std::tie(this->X, this->Y) = helper::random_location();
    this->asleep = false;

    increment_age();
    evaluate_fitness();

    // todo: Insert row in table
}

Animal::Animal(const std::string& kind, const std::string& chromosome, const chromosome_map_type& chromosome_structure, const unsigned int& generation, const std::string& name, const unsigned int& X, const unsigned int& Y)
{
    Animal(kind, chromosome, chromosome_structure, generation, name);
    this->X = X;
    this->Y = Y;
}

double Animal::get_base_vitality()
{
    return helper::get_value_from_chromosome(chromosome,
                      this->chromosome_structure["bv"]["start"],
                      this->chromosome_structure["bv"]["length"],
                      theoretical_maximum_base_vitality);
}

double Animal::get_base_stamina()
{
    return helper::get_value_from_chromosome(chromosome,
                      this->chromosome_structure["bs"]["start"],
                      this->chromosome_structure["bs"]["length"],
                      theoretical_maximum_base_stamina);
}

double Animal::get_base_speed()
{
    return helper::get_value_from_chromosome(chromosome,
                      this->chromosome_structure["bp"]["start"],
                      this->chromosome_structure["bp"]["length"],
                      theoretical_maximum_base_speed);
}

double Animal::get_base_appetite()
{
    return helper::get_value_from_chromosome(chromosome,
                      this->chromosome_structure["ba"]["start"],
                      this->chromosome_structure["ba"]["length"],
                      theoretical_maximum_base_appetite);
}

double Animal::get_vitality_multiplier()
{
    return helper::get_value_from_chromosome(chromosome,
                      this->chromosome_structure["vm"]["start"],
                      this->chromosome_structure["vm"]["length"],
                      theoretical_maximum_vitality_multiplier);
}

double Animal::get_stamina_multiplier()
{
    return helper::get_value_from_chromosome(chromosome,
                      this->chromosome_structure["sm"]["start"],
                      this->chromosome_structure["sm"]["length"],
                      theoretical_maximum_stamina_multiplier);
}

double Animal::get_speed_multiplier()
{
    return helper::get_value_from_chromosome(chromosome,
                      this->chromosome_structure["pm"]["start"],
                      this->chromosome_structure["pm"]["length"],
                      theoretical_maximum_speed_multiplier);
}

double Animal::get_max_height()
{
    return helper::get_value_from_chromosome(chromosome,
                      this->chromosome_structure["mh"]["start"],
                      this->chromosome_structure["mh"]["length"],
                      theoretical_maximum_height);
}

double Animal::get_base_height()
{
    return helper::get_value_from_chromosome(chromosome,
                      this->chromosome_structure["bh"]["start"],
                      this->chromosome_structure["bh"]["length"],
                      theoretical_maximum_base_height);
}

double Animal::get_height_multiplier()
{
    return helper::get_value_from_chromosome(chromosome,
                      this->chromosome_structure["hm"]["start"],
                      this->chromosome_structure["hm"]["length"],
                      theoretical_maximum_height_multiplier);
}

double Animal::get_max_weight()
{
    return helper::get_value_from_chromosome(chromosome,
                      this->chromosome_structure["mw"]["start"],
                      this->chromosome_structure["mw"]["length"],
                      theoretical_maximum_weight);
}

double Animal::get_base_weight()
{
    return helper::get_value_from_chromosome(chromosome,
                      this->chromosome_structure["bw"]["start"],
                      this->chromosome_structure["bw"]["length"],
                      theoretical_maximum_base_weight);
}

double Animal::get_weight_multiplier()
{
    return helper::get_value_from_chromosome(chromosome,
                      this->chromosome_structure["wm"]["start"],
                      this->chromosome_structure["wm"]["length"],
                      theoretical_maximum_weight_multiplier);
}

double Animal::get_immunity()
{
    return helper::get_value_from_chromosome(chromosome,
                      this->chromosome_structure["im"]["start"],
                      this->chromosome_structure["im"]["length"],
                      1.0);
}

unsigned int Animal::get_gender()
{
    return static_cast<unsigned int>(helper::get_value_from_chromosome(chromosome,
                      this->chromosome_structure["bv"]["start"],
                      this->chromosome_structure["bv"]["length"],
                      2.0));
}

void Animal::evaluate_fitness()
{
    fitness = immunity * ( 0.01 * max_vitality_at_age +
                       0.01 * max_stamina_at_age +
                       max_speed_at_age) / (3 * pow(age, 1.0 / 32));
}

void Animal::increment_vitality_by(const double &units)
{
    vitality = std::min(vitality + units, max_vitality_at_age);
    appetite = appetite * ( 1 + vitality_on_appetite * vitality / max_vitality_at_age );
    speed = speed * (1 + vitality_on_speed * vitality / max_vitality_at_age);
}

void Animal::decrement_vitality_by(const double &units)
{
    vitality = std::max(vitality - units, 0.0);
    appetite = appetite * ( 1 + vitality_on_appetite * vitality / max_vitality_at_age );
    speed = speed * (1 + vitality_on_speed * vitality / max_vitality_at_age);
}

void Animal::increment_stamina_by(const double &units)
{
    stamina = std::min(stamina + units, max_stamina_at_age);
    appetite = appetite * ( 1 + stamina_on_appetite * stamina / max_stamina_at_age );
    speed = speed * (1 + stamina_on_speed * stamina / max_stamina_at_age);
}

void Animal::decrement_stamina_by(const double &units)
{
    stamina = std::max(stamina - units, 0.0);
    appetite = appetite * ( 1 + stamina_on_appetite * stamina / max_stamina_at_age );
    speed = speed * (1 + stamina_on_speed * stamina / max_stamina_at_age);
}

void Animal::increment_age()
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
    max_stamina_at_age = get_base_stamina() * ( get_stamina_multiplier() * 0.5 * pow(max_age, 0.5) *
                                                  exp(-pow(age - max_age * 0.5, 2) / (max_age * get_base_stamina())) + 1);
    max_speed_at_age = get_speed_multiplier() * 100 * exp((-1 / (get_speed_multiplier() * pow(max_age, 1.5))) * pow(age - max_age / 2.5, 2)) + get_base_speed();
    max_appetite_at_age = get_base_appetite() + get_base_appetite() * exp((-0.5 / pow(max_age, 1.25)) * pow(age - max_age / 3.0, 2));

    // change stats dependently

    max_vitality_at_age = max_vitality_at_age * (1 +
                                                 height_on_vitality * height / get_max_height() +
                                                 weight_on_vitality * weight / get_max_weight());
    max_stamina_at_age = max_stamina_at_age * (1 +
                                               height_on_stamina * height / get_max_height() +
                                               weight_on_stamina * weight / get_max_weight());
    max_speed_at_age = max_speed_at_age * (1 +
                                           height_on_speed * height / get_max_height() +
                                           weight_on_speed * weight / get_max_weight());
    evaluate_fitness();

    // TODO: update a row in table
}

void Animal::sleep(const double &duration)
{
    stamina = std::min(max_stamina_at_age, stamina + sleep_restore_factor * duration);
}

void Animal::eat(const double &nutrition)
{
    appetite = std::max(0.0, appetite - nutrition);
}

double Animal::die_of_age_factor()
{
    return std::min(1.0, exp(age_on_death * (static_cast<double>(age) / max_age - 1)));
}

double Animal::die_of_fitness_factor()
{
    return std::min(1.0, exp(-fitness_on_death * fitness));
}

double Animal::death_factor()
{
    age_fitness_on_death_ratio = age_fitness_on_death_ratio * (2 - die_of_age_factor());
    return helper::weighted_average({die_of_age_factor(), die_of_fitness_factor()}, {age_fitness_on_death_ratio, 1.0});
}

std::map<std::string, stat_type> Animal::get_stats()
{
    std::map<std::string, stat_type> stats;
    stats["name"] = name;
    stats["age"] = age;
//    stats["max_vitality_at_age"] = max_vitality_at_age;
//    stats["max_stamina_at_age"] = max_stamina_at_age;
//    stats["max_speed_at_age"] = max_speed_at_age;
//    stats["max_appetite_at_age"] = max_appetite_at_age;
    stats["height"] = height;
    stats["weight"] = weight;
    stats["death_factor"] = death_factor();
//    stats["vitality"] = vitality;
//    stats["stamina"] = stamina;
//    stats["speed"] = speed;
//    stats["appetite"] = appetite;
    stats["fitness"] = fitness;
    return stats;
}
