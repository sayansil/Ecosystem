#ifndef ORGANISM_HPP
#define ORGANISM_HPP

#include <string>
#include <map>

class Animal
{
private:
    std::string name;
    std::map<std::string, std::map<std::string, unsigned int>> chromosome_structure;

    // Fixed for a species

    unsigned int max_age;
    unsigned int chromosome_number;
    double age_on_death;
    double fitness_on_death;
    double age_fitness_on_death_ratio;
    double height_on_vitality;
    double weight_on_vitality;
    double height_on_stamina;
    double weight_on_stamina;
    double height_on_speed;
    double weight_on_speed;
    double vitality_on_appetite;
    double stamina_on_appetite;
    double vitality_on_speed;
    double stamina_on_speed;
    double sleep_restore_factor;
    double theoretical_maximum_base_vitality;
    double theoretical_maximum_vitality_multiplier;
    double theoretical_maximum_base_stamina;
    double theoretical_maximum_stamina_multiplier;
    double theoretical_maximum_height;
    double theoretical_maximum_weight;
    double theoretical_maximum_base_height;
    double theoretical_maximum_base_weight;
    double theoretical_maximum_height_multiplier;
    double theoretical_maximum_weight_multiplier;
    double theoretical_maximum_speed;
    double theoretical_maximum_base_speed;
    double theoretical_maximum_speed_multiplier;
    double theoretical_maximum_base_appetite;

    // Fixed for the organism throughout their lifetime

    std::string chromosome;
    unsigned int generation;
    double immunity;

    // Changes slowly - once every year (during mating season)

    unsigned int age;
    double max_vitality_at_age;
    double max_stamina_at_age;
    double max_speed_at_age;
    double max_appetite_at_age;
    double height;
    double weight;

    // Changes dynamically every moment

    double vitality;
    double stamina;
    double speed;
    double appetite;
    unsigned int X;
    unsigned int Y;
    bool asleep;

    double fitness;

public:
};


#endif // ORGANISM_HPP
