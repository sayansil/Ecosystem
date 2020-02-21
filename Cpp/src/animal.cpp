#include <animal.hpp>

Animal::Animal(const std::string& kind, const std::string& chromosome, const unsigned int& generation, const std::string& name, const std::pair<unsigned int, unsigned int>& XY)
{
    this->kind = kind;

    if(name.length() == 0)
    {
        this->name = kind + "-" + helper::random_name(16);
    }
    else
        this->name = name;

    const std::string filepath = "../../data/json/" + kind + ".json";

    std::ifstream in(filepath);

    nlohmann::json json_file; in >> json_file;

    this->chromosome_structure  = CHROMOSOME_MAP_TYPE(json_file["chromosome_structure"]);
    this->chromosome_number     = json_file["species_chromosome_number"];

    this->mating_age_start  = json_file["mating_age"]["start"];
    this->mating_age_end    = json_file["mating_age"]["end"];
    this->max_age           = json_file["species_max_age"];
    this->offsprings_factor = json_file["offsprings_factor"];

    this->mutation_probability = json_file["mutation_probability"];
    this->conceiving_probability    = json_file["conceiving_probability"];

    this->age_on_death                  = json_file["species_age_on_death"];
    this->fitness_on_death              = json_file["species_fitness_on_death"];
    this->age_fitness_on_death_ratio    = json_file["species_age_fitness_on_death_ratio"];

    this->height_on_speed       = json_file["species_height_on_speed"];
    this->height_on_stamina     = json_file["species_height_on_stamina"];
    this->height_on_vitality    = json_file["species_height_on_vitality"];
    this->weight_on_speed       = json_file["species_weight_on_speed"];
    this->weight_on_stamina     = json_file["species_weight_on_stamina"];
    this->weight_on_vitality    = json_file["species_weight_on_vitality"];

    this->vitality_on_appetite  = json_file["species_vitality_on_appetite"];
    this->vitality_on_speed     = json_file["species_vitality_on_speed"];
    this->stamina_on_appetite   = json_file["species_stamina_on_appetite"];
    this->stamina_on_speed      = json_file["species_stamina_on_speed"];

    this->theoretical_maximum_base_appetite = json_file["species_theoretical_maximum_base_appetite"];
    this->theoretical_maximum_base_height   = json_file["species_theoretical_maximum_base_height"];
    this->theoretical_maximum_base_speed    = json_file["species_theoretical_maximum_base_speed"];
    this->theoretical_maximum_base_stamina  = json_file["species_theoretical_maximum_base_stamina"];
    this->theoretical_maximum_base_vitality = json_file["species_theoretical_maximum_base_vitality"];
    this->theoretical_maximum_base_weight   = json_file["species_theoretical_maximum_base_weight"];

    this->theoretical_maximum_height_multiplier     = json_file["species_theoretical_maximum_height_multiplier"];
    this->theoretical_maximum_speed_multiplier      = json_file["species_theoretical_maximum_speed_multiplier"];
    this->theoretical_maximum_stamina_multiplier    = json_file["species_theoretical_maximum_stamina_multiplier"];
    this->theoretical_maximum_vitality_multiplier   = json_file["species_theoretical_maximum_vitality_multiplier"];
    this->theoretical_maximum_weight_multiplier     = json_file["species_theoretical_maximum_weight_multiplier"];

    this->theoretical_maximum_height    = json_file["species_theoretical_maximum_height"];
    this->theoretical_maximum_speed     = json_file["species_theoretical_maximum_speed"];
    this->theoretical_maximum_weight    = json_file["species_theoretical_maximum_weight"];

    this->sleep_restore_factor = json_file["species_sleep_restore_factor"];

    in.close();

    if(chromosome.length() == 0)
    {
        unsigned int chromosome_length = 0;

        // Do not disturb this loop...

        for(auto i = chromosome_structure.begin(); i != chromosome_structure.end(); i++)
        {
            chromosome_length += i->second["length"];
        }
        this->chromosome = helper::random_binary(chromosome_length);
    }
    else
    {
        this->chromosome = chromosome;
    }

    this->immunity      = get_immunity();
    this->generation    = generation;
    this->gender        = get_gender();

    this->age = 0;

    this->max_vitality_at_age   = get_base_vitality();
    this->max_stamina_at_age    = get_base_stamina();
    this->max_speed_at_age      = get_base_speed();
    this->max_appetite_at_age   = get_base_appetite();

    this->height = get_base_height();
    this->weight = get_base_weight();

    this->vitality  = this->max_vitality_at_age;
    this->stamina   = this->max_stamina_at_age;
    this->appetite  = this->max_appetite_at_age;
    this->speed     = this->max_speed_at_age;

    std::tie(this->X, this->Y) = helper::random_location();
    this->asleep = false;

    increment_age();

    evaluate_static_fitness();
    evaluate_dynamic_fitness();
}

double Animal::get_base_vitality() const
{
    return helper::get_value_from_chromosome(chromosome,
                      this->chromosome_structure.at("bv").at("start"),
                      this->chromosome_structure.at("bv").at("length"),
                      theoretical_maximum_base_vitality);
}

double Animal::get_base_stamina() const
{
    return helper::get_value_from_chromosome(chromosome,
                      this->chromosome_structure.at("bs").at("start"),
                      this->chromosome_structure.at("bs").at("length"),
                      theoretical_maximum_base_stamina);
}

double Animal::get_base_speed() const
{
    return helper::get_value_from_chromosome(chromosome,
                      this->chromosome_structure.at("bp").at("start"),
                      this->chromosome_structure.at("bp").at("length"),
                      theoretical_maximum_base_speed);
}

double Animal::get_base_appetite() const
{
    return helper::get_value_from_chromosome(chromosome,
                      this->chromosome_structure.at("ba").at("start"),
                      this->chromosome_structure.at("ba").at("length"),
                      theoretical_maximum_base_appetite);
}

double Animal::get_vitality_multiplier() const
{
    return helper::get_value_from_chromosome(chromosome,
                      this->chromosome_structure.at("vm").at("start"),
                      this->chromosome_structure.at("vm").at("length"),
                      theoretical_maximum_vitality_multiplier);
}

double Animal::get_stamina_multiplier() const
{
    return helper::get_value_from_chromosome(chromosome,
                      this->chromosome_structure.at("sm").at("start"),
                      this->chromosome_structure.at("sm").at("length"),
                      theoretical_maximum_stamina_multiplier);
}

double Animal::get_speed_multiplier() const
{
    return helper::get_value_from_chromosome(chromosome,
                      this->chromosome_structure.at("pm").at("start"),
                      this->chromosome_structure.at("pm").at("length"),
                      theoretical_maximum_speed_multiplier);
}

double Animal::get_max_height() const
{
    return helper::get_value_from_chromosome(chromosome,
                      this->chromosome_structure.at("mh").at("start"),
                      this->chromosome_structure.at("mh").at("length"),
                      theoretical_maximum_height);
}

double Animal::get_base_height() const
{
    return helper::get_value_from_chromosome(chromosome,
                      this->chromosome_structure.at("bh").at("start"),
                      this->chromosome_structure.at("bh").at("length"),
                      theoretical_maximum_base_height);
}

double Animal::get_height_multiplier() const
{
    return helper::get_value_from_chromosome(chromosome,
                      this->chromosome_structure.at("hm").at("start"),
                      this->chromosome_structure.at("hm").at("length"),
                      theoretical_maximum_height_multiplier);
}

double Animal::get_max_weight() const
{
    return helper::get_value_from_chromosome(chromosome,
                      this->chromosome_structure.at("mw").at("start"),
                      this->chromosome_structure.at("mw").at("length"),
                      theoretical_maximum_weight);
}

double Animal::get_base_weight() const
{
    return helper::get_value_from_chromosome(chromosome,
                      this->chromosome_structure.at("bw").at("start"),
                      this->chromosome_structure.at("bw").at("length"),
                      theoretical_maximum_base_weight);
}

double Animal::get_weight_multiplier() const
{
    return helper::get_value_from_chromosome(chromosome,
                      this->chromosome_structure.at("wm").at("start"),
                      this->chromosome_structure.at("wm").at("length"),
                      theoretical_maximum_weight_multiplier);
}

double Animal::get_immunity() const
{
    return helper::get_value_from_chromosome(chromosome,
                      this->chromosome_structure.at("im").at("start"),
                      this->chromosome_structure.at("im").at("length"),
                      1.0);
}

unsigned int Animal::get_gender() const
{
    return static_cast<unsigned int>(helper::get_value_from_chromosome(chromosome,
                      this->chromosome_structure.at("bv").at("start"),
                      this->chromosome_structure.at("bv").at("length"),
                      2.0));
}

void Animal::evaluate_static_fitness()
{
    static_fitness = immunity * ( 0.01 * max_vitality_at_age +
                       0.01 * max_stamina_at_age +
                       max_speed_at_age) / (3 * pow(age, 1.0 / 32));
}

void Animal::evaluate_dynamic_fitness()
{
    dynamic_fitness = 1;
}

double Animal::get_fitness() const
{
    return static_fitness * dynamic_fitness;
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
    evaluate_static_fitness();
}

void Animal::sleep(const double &duration)
{
    stamina = std::min(max_stamina_at_age, stamina + sleep_restore_factor * duration);
}

void Animal::eat(const double &nutrition)
{
    appetite = std::max(0.0, appetite - nutrition);
}

double Animal::get_die_of_age_factor() const
{
    return std::min(1.0, exp(age_on_death * (static_cast<double>(age) / max_age - 1)));
}

double Animal::get_die_of_fitness_factor() const
{
    return std::min(1.0, exp(-fitness_on_death * get_fitness()));
}

void Animal::generate_death_factor()
{
    age_fitness_on_death_ratio = age_fitness_on_death_ratio * (2 - get_die_of_age_factor());
    death_factor = helper::weighted_average({get_die_of_age_factor(), get_die_of_fitness_factor()}, {age_fitness_on_death_ratio, 1.0});
}

STAT_TYPE Animal::get_stat(const std::string &attribute) const
{
    if (attribute == "kind")
    {
        return kind;
    }
    if (attribute == "chromosome_number")
    {
        return chromosome_number;
    }
    if (attribute == "conceiving_probability")
    {
        return conceiving_probability;
    }
    if (attribute == "mating_age_start")
    {
        return mating_age_start;
    }
    if (attribute == "mating_age_end")
    {
        return mating_age_end;
    }
    if (attribute == "max_age")
    {
        return max_age;
    }
    if (attribute == "mutation_probability")
    {
        return mutation_probability;
    }
    if (attribute == "offsprings_factor")
    {
        return offsprings_factor;
    }
    if (attribute == "age_on_death")
    {
        return age_on_death;
    }
    if (attribute == "fitness_on_death")
    {
        return fitness_on_death;
    }
    if (attribute == "age_fitness_on_death_ratio")
    {
        return age_fitness_on_death_ratio;
    }
    if (attribute == "height_on_speed")
    {
        return height_on_speed;
    }
    if (attribute == "height_on_stamina")
    {
        return height_on_stamina;
    }
    if (attribute == "height_on_vitality")
    {
        return height_on_vitality;
    }
    if (attribute == "weight_on_speed")
    {
        return weight_on_speed;
    }
    if (attribute == "weight_on_stamina")
    {
        return weight_on_stamina;
    }
    if (attribute == "weight_on_vitality")
    {
        return weight_on_vitality;
    }
    if (attribute == "vitality_on_appetite")
    {
        return vitality_on_appetite;
    }
    if (attribute == "vitality_on_speed")
    {
        return vitality_on_speed;
    }
    if (attribute == "stamina_on_appetite")
    {
        return stamina_on_appetite;
    }
    if (attribute == "stamina_on_speed")
    {
        return stamina_on_speed;
    }
    if (attribute == "theoretical_maximum_base_appetite")
    {
        return theoretical_maximum_base_appetite;
    }
    if (attribute == "theoretical_maximum_base_height")
    {
        return theoretical_maximum_base_height;
    }
    if (attribute == "theoretical_maximum_base_speed")
    {
        return theoretical_maximum_base_speed;
    }
    if (attribute == "theoretical_maximum_base_stamina")
    {
        return theoretical_maximum_base_stamina;
    }
    if (attribute == "theoretical_maximum_base_vitality")
    {
        return theoretical_maximum_base_vitality;
    }
    if (attribute == "theoretical_maximum_base_weight")
    {
        return theoretical_maximum_base_weight;
    }
    if (attribute == "theoretical_maximum_height")
    {
        return theoretical_maximum_height;
    }
    if (attribute == "theoretical_maximum_speed")
    {
        return theoretical_maximum_speed;
    }
    if (attribute == "theoretical_maximum_weight")
    {
        return theoretical_maximum_weight;
    }
    if (attribute == "theoretical_maximum_height_multiplier")
    {
        return theoretical_maximum_height_multiplier;
    }
    if (attribute == "theoretical_maximum_speed_multiplier")
    {
        return theoretical_maximum_speed_multiplier;
    }
    if (attribute == "theoretical_maximum_stamina_multiplier")
    {
        return theoretical_maximum_stamina_multiplier;
    }
    if (attribute == "theoretical_maximum_vitality_multiplier")
    {
        return theoretical_maximum_vitality_multiplier;
    }
    if (attribute == "theoretical_maximum_weight_multiplier")
    {
        return theoretical_maximum_weight_multiplier;
    }
    if (attribute == "sleep_restore_factor")
    {
        return sleep_restore_factor;
    }
    if (attribute == "chromosome")
    {
        return chromosome;
    }
    if (attribute == "gender")
    {
        return gender;
    }
    if (attribute == "generation")
    {
        return generation;
    }
    if (attribute == "immunity")
    {
        return immunity;
    }
    if (attribute == "name")
    {
        return name;
    }
    if (attribute == "age")
    {
        return age;
    }
    if (attribute == "height")
    {
        return height;
    }
    if (attribute == "weight")
    {
        return weight;
    }
    if (attribute == "max_appetite_at_age")
    {
        return max_appetite_at_age;
    }
    if (attribute == "max_speed_at_age")
    {
        return max_speed_at_age;
    }
    if (attribute == "max_stamina_at_age")
    {
        return max_stamina_at_age;
    }
    if (attribute == "max_vitality_at_age")
    {
        return max_vitality_at_age;
    }
    if (attribute == "static_fitness")
    {
        return static_fitness;
    }
    if (attribute == "death_factor")
    {
        return death_factor;
    }
    if (attribute == "vision_radius")
    {
        return vision_radius;
    }
    if (attribute == "appetite")
    {
        return appetite;
    }
    if (attribute == "speed")
    {
        return speed;
    }
    if (attribute == "stamina")
    {
        return stamina;
    }
    if (attribute == "vitality")
    {
        return vitality;
    }
    if (attribute == "X")
    {
        return X;
    }
    if (attribute == "Y")
    {
        return Y;
    }
    if (attribute == "dynamic_fitness")
    {
        return dynamic_fitness;
    }
    if (attribute == "asleep")
    {
        return asleep;
    }

    return "null";
}
