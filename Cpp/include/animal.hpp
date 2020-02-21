#ifndef ANIMAL_HPP
#define ANIMAL_HPP

#include <climits>
#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include <math.h>
#include <variant>

#include <EcosystemTypes.hpp>
#include <helper.hpp>
#include <nlohmann/json.hpp>

#define MALE 0
#define FEMALE 1

class Animal
{
public:

    /***************************************
     *         Fixed for a species         *
    ****************************************/

    std::string kind;
    CHROMOSOME_MAP_TYPE chromosome_structure;
    unsigned int chromosome_number;

    // Attributes related to Mating
    double conceiving_probability;
    unsigned int mating_age_start;
    unsigned int mating_age_end;
    unsigned int max_age;
    double mutation_probability;
    double offsprings_factor;

    // Attributes related to death
    double age_on_death;
    double fitness_on_death;
    double age_fitness_on_death_ratio;

    // Height dependance factors
    double height_on_speed;
    double height_on_stamina;
    double height_on_vitality;

    // Weight dependance factors
    double weight_on_speed;
    double weight_on_stamina;
    double weight_on_vitality;

    // Vitality dependance factors
    double vitality_on_appetite;
    double vitality_on_speed;

    // Stamina dependance factors
    double stamina_on_appetite;
    double stamina_on_speed;

    // Maximum base (starting) stats
    double theoretical_maximum_base_appetite;
    double theoretical_maximum_base_height;
    double theoretical_maximum_base_speed;
    double theoretical_maximum_base_stamina;
    double theoretical_maximum_base_vitality;
    double theoretical_maximum_base_weight;

    // Maximum attainable stats
    double theoretical_maximum_height;
    double theoretical_maximum_speed;
    double theoretical_maximum_weight;

    // Stat multiplier (with age)
    double theoretical_maximum_height_multiplier;
    double theoretical_maximum_speed_multiplier;
    double theoretical_maximum_stamina_multiplier;
    double theoretical_maximum_vitality_multiplier;
    double theoretical_maximum_weight_multiplier;

    // Miscellaneous attributes
    double sleep_restore_factor;

    /******************************************************
     *  Fixed for the organism throughout their lifetime  *
    *******************************************************/

    // Chromosome (binary string)
    std::string chromosome;

    // Miscellaneous attributes
    unsigned int gender; // (0 - male, 1 - female)
    unsigned int generation;
    double immunity;
    std::string name;

    /*************************************************************
     *  Changes slowly - once every year (during mating season)  *
    **************************************************************/

    // Physical changes (in appearance)
    unsigned int age;
    double height;
    double weight;

    // Stats affected by age
    double max_appetite_at_age;
    double max_speed_at_age;
    double max_stamina_at_age;
    double max_vitality_at_age;

    // Fitness based on yearly changing stats
    double static_fitness;

    // Death factor determined by present age and fitness
    double death_factor;

    // Miscellaneous attributes
    double vision_radius; // TODO

    /**************************************
     *  Changes dynamically every moment  *
    ***************************************/

    // Physical attributes
    double appetite;
    double speed;
    double stamina;
    double vitality;

    // Position
    unsigned int X;
    unsigned int Y;

    // Dynamic fitness value
    double dynamic_fitness;

    // Miscellanous attributes
    bool asleep;

    /******************************
     *  Constructor / Destructor  *
    *******************************/

    Animal() = default;
    Animal(
        const std::string& kind,
        const std::string& chromosome = "",
        const unsigned int& generation = 1,
        const std::string& name = "",
        const std::pair<unsigned int, unsigned int>& XY = helper::random_location());

    /*****************
     *  Get methods  *
    ******************/

    // Get base stats
    double get_base_appetite() const;
    double get_base_height() const;
    double get_base_speed() const;
    double get_base_stamina() const;
    double get_base_vitality() const;
    double get_base_weight() const;

    // Get multipliers
    double get_height_multiplier() const;
    double get_speed_multiplier() const;
    double get_stamina_multiplier() const;
    double get_vitality_multiplier() const;
    double get_weight_multiplier() const;

    // Get max stats
    double get_max_height() const;
    double get_max_weight() const;

    // Get miscellaneous stats
    double get_die_of_age_factor() const;
    double get_die_of_fitness_factor() const;
    double get_fitness() const;
    double get_immunity() const;
    unsigned int get_gender() const;

    /********************
     *  Update methods  *
    *********************/

    // Generate fitness values
    void evaluate_dynamic_fitness();
    void evaluate_static_fitness();

    // Miscellaneous methods
    void eat(const double &);
    void generate_death_factor();
    void increment_age();
    void sleep(const double &);

    // Update stats
    void decrement_stamina_by(const double &);
    void decrement_vitality_by(const double &);
    void increment_stamina_by(const double &);
    void increment_vitality_by(const double&);

    STAT_TYPE get_stat(const std::string &) const;
};

#endif // ANIMAL_HPP
