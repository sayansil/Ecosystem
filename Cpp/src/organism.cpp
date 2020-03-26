#include <organism.hpp>

double Organism::get_die_of_age_factor() const
{
    return std::min(1.0, exp(age_on_death * (static_cast<double>(age) / max_age - 1)));
}

double Organism::get_die_of_fitness_factor() const
{
    return std::min(1.0, exp(-fitness_on_death * get_fitness()));
}

double Organism::get_fitness() const
{
    return static_fitness * dynamic_fitness;
}



double Organism::get_immunity() const
{
    return helper::get_value_from_chromosome(chromosome,
                      this->chromosome_structure.at("im").at("start"),
                      this->chromosome_structure.at("im").at("length"),
                      1.0);
}

unsigned int Organism::get_gender() const
{
    return static_cast<unsigned int>(helper::get_value_from_chromosome(chromosome,
                      this->chromosome_structure.at("bv").at("start"),
                      this->chromosome_structure.at("bv").at("length"),
                      2.0));
}

Organism::~Organism()
{

}
