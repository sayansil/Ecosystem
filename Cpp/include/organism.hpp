#ifndef ORGANISM_HPP
#define ORGANISM_HPP

#include <climits>
#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include <math.h>
#include <variant>
#include <memory>
#include <vector>

#include <EcosystemTypes.hpp>
#include <helper.hpp>
#include <nlohmann/json.hpp>

class Organism
{
public:

    /***************************************
     *         Fixed for a species         *
    ****************************************/

    std::string kind;
    CHROMOSOME_MAP_TYPE chromosome_structure;
    unsigned int chromosome_number;
    unsigned int food_chain_rank;
    unsigned int is_asexual;

    // Attributes related to death

    double age_on_death;
    double fitness_on_death;
    double age_fitness_on_death_ratio;

    // Attributes related to Mating

    double conceiving_probability;
    unsigned int mating_age_start;
    unsigned int mating_age_end;
    unsigned int max_age;
    double mutation_probability;
    double offsprings_factor;

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

    // Fitness based on yearly changing stats
    double static_fitness;

    // Death factor determined by present age and fitness
    double death_factor;

    // Miscellaneous attributes
    double vision_radius;

    /**************************************
     *  Changes dynamically every moment  *
    ***************************************/

    // Position
    unsigned int X;
    unsigned int Y;

    // Dynamic fitness value
    double dynamic_fitness;

    /*****************
     *  Get methods  *
    ******************/

    // Get base stats
    virtual double get_base_appetite() const = 0;
    virtual double get_base_height() const = 0;
    virtual double get_base_speed() const = 0;
    virtual double get_base_stamina() const = 0;
    virtual double get_base_vitality() const = 0;
    virtual double get_base_weight() const = 0;

    // Get multipliers
    virtual double get_height_multiplier() const = 0;
    virtual double get_speed_multiplier() const = 0;
    virtual double get_stamina_multiplier() const = 0;
    virtual double get_vitality_multiplier() const = 0;
    virtual double get_weight_multiplier() const = 0;

    // Get max stats
    virtual double get_max_height() const = 0;
    virtual double get_max_weight() const = 0;

    // Get miscellaneous stats
    double get_die_of_age_factor() const;
    double get_die_of_fitness_factor() const;
    double get_fitness() const;
    double get_immunity() const;
    unsigned int get_gender() const;
    virtual bool is_normal_child() const = 0;

    /********************
     *  Clone function  *
    *********************/
    
    virtual std::shared_ptr<Organism> clone() const = 0;
    virtual std::shared_ptr<Organism> clone(
                const std::string& kind,
                const unsigned int& age = 0,
                const std::string& chromosome = "",
                const unsigned int& generation = 1,
                const std::string& name = "",
                const std::pair<unsigned int, unsigned int>& XY = helper::random_location(),
                const nlohmann::json& species_constants = nlohmann::json()
            ) const = 0;
    
    /********************
     *  Update methods  *
     ********************/

    virtual void init_from_json(const nlohmann::json &) = 0;

    // Generate fitness values
    virtual void evaluate_dynamic_fitness() = 0;
    virtual void evaluate_static_fitness() = 0;

    // Miscellaneous methods
    virtual void eat(const double &) = 0;
    virtual void generate_death_factor() = 0;
    virtual void increment_age() = 0;
    virtual void sleep(const double &) = 0;

    // Update stats
    virtual void decrement_stamina_by(const double &) = 0;
    virtual void decrement_vitality_by(const double &) = 0;
    virtual void increment_stamina_by(const double &) = 0;
    virtual void increment_vitality_by(const double&) = 0;

    virtual STAT get_stat(const std::string &) const = 0;

    /************************
     *  Virtual Destructor  *
     ************************/

    virtual ~Organism();
};

using ORGANISM = std::shared_ptr<Organism>;

#endif /* ORGANISM_HPP */
