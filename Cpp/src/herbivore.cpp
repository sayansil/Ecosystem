#include <herbivore.hpp>

chromosome_map_type Deer::species_chromosome_structure;

Deer::Deer(const std::string& chromosome, const unsigned int& generation, const std::string& name, const unsigned int &X, const unsigned int& Y)
    : Animal(species_name, chromosome, species_chromosome_structure, generation, name, X, Y)
{
}

Deer::Deer(const std::string& chromosome, const unsigned int& generation, const std::string& name)
    : Animal(species_name, chromosome, species_chromosome_structure, generation, name)
{
}
