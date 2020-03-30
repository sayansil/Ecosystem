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

#include <ecosystem_types.hpp>
#include <helper.hpp>
#include <nlohmann/json.hpp>
#include <entity.hpp>

template <typename T>
class Organism : public Entity
{
public:

    bool monitor_in_simulation;

    /***************************************
     *         Fixed for a species         *
     ***************************************/

    std::string kind;
    std::string full_species_name;
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
     ******************************************************/

    // Chromosome (binary string)
    std::string chromosome;

    // Miscellaneous attributes
    unsigned int gender; // (0 - male, 1 - female)
    unsigned int generation;
    double immunity;
    std::string name;


    /*************************************************************
     *  Changes slowly - once every year (during mating season)  *
     *************************************************************/

    // Physical changes (in appearance)
    unsigned int age;
    double height;
    double weight;

    // Fitness based on yearly changing stats
    double static_fitness;

    // Death factor determined by present age and fitness
    double death_factor;


    /**************************************
     *  Changes dynamically every moment  *
     **************************************/

    // Position
    unsigned int X;
    unsigned int Y;

    // Dynamic fitness value
    double dynamic_fitness;


    /********************
     *      Getters     *
     ********************/

    bool get_monitor_in_simulation() const
    {
        return monitor_in_simulation;
    }

    // Get base stats
    double get_base_appetite() const
    {
        return static_cast<const T&>(*this).get_base_appetite();
    }
    double get_base_height() const
    {
        return static_cast<const T&>(*this).get_base_height();
    }
    double get_base_speed() const
    {
        return static_cast<const T&>(*this).get_base_speed();
    }
    double get_base_stamina() const
    {
        return static_cast<const T&>(*this).get_base_stamina();
    }
    double get_base_vitality() const
    {
        return static_cast<const T&>(*this).get_base_vitality();
    }
    double get_base_weight() const
    {
        return static_cast<const T&>(*this).get_base_weight();
    }

    // Get multipliers
    double get_height_multiplier() const
    {
        return static_cast<const T&>(*this).get_height_multiplier();
    }
    double get_speed_multiplier() const
    {
        return static_cast<const T&>(*this).get_speed_multiplier();
    }
    double get_stamina_multiplier() const
    {
        return static_cast<const T&>(*this).get_stamina_multiplier();
    }
    double get_vitality_multiplier() const
    {
        return static_cast<const T&>(*this).get_vitality_multiplier();
    }
    double get_weight_multiplier() const
    {
        return static_cast<const T&>(*this).get_weight_multiplier();
    }

    // Get max stats
    double get_max_height() const
    {
        return static_cast<const T&>(*this).get_max_height();
    }
    double get_max_weight() const
    {
        return static_cast<const T&>(*this).get_max_weight();
    }

    // Get other stats
    std::string get_kind() const
    {
        return kind;
    }
    std::string get_full_species_name() const
    {
        return full_species_name;
    }
    std::string get_name() const
    {
        return name;
    }
    std::string get_chromosome() const
    {
        return chromosome;
    }
    double get_immunity() const
    {
        return immunity;
    }
    double get_die_of_age_factor() const
    {
        return std::min(1.0, exp(age_on_death * (static_cast<double>(age) / max_age - 1)));
    }
    double get_die_of_fitness_factor() const
    {
        return std::min(1.0, exp(-fitness_on_death * get_fitness()));
    }
    double get_fitness() const
    {
        return static_fitness * dynamic_fitness;
    }
    double get_height() const
    {
        return height;
    }
    double get_weight() const
    {
        return weight;
    }
    double get_static_fitness() const
    {
        return static_fitness;
    }
    double get_mutation_probability() const
    {
        return mutation_probability;
    }
    double get_conceiving_probability() const
    {
        return conceiving_probability;
    }
    double get_death_factor() const
    {
        return death_factor;
    }
    double get_offsprings_factor() const
    {
        return offsprings_factor;
    }
    unsigned int get_gender() const
    {
        return gender;
    }
    unsigned int get_mating_age_start() const
    {
        return mating_age_start;
    }
    unsigned int get_mating_age_end() const
    {
        return mating_age_end;
    }
    unsigned int get_age() const
    {
        return age;
    }
    unsigned int get_generation() const
    {
        return generation;
    }
    unsigned int get_X() const
    {
        return X;
    }
    unsigned int get_Y() const
    {
        return Y;
    }
    unsigned int get_is_asexual() const
    {
        return is_asexual;
    }
    unsigned int get_food_chain_rank() const
    {
        return food_chain_rank;
    }
    bool is_normal_child() const
    {
        return static_cast<const T&>(*this).is_normal_child();
    }
    std::string get_kingdom() const
    {
        return static_cast<const T &>(*this).get_kingdom();
    }
    STAT get_stat(const std::string &data) const
    {
        return static_cast<const T &>(*this).get_stat(data);
    }


    /********************
     *  Clone function  *
     ********************/

    std::shared_ptr<Entity> clone() const
    {
        return static_cast<const T&>(*this).clone();
    }
    std::shared_ptr<Entity> clone(
                const std::string &kind,
                const unsigned int &age = 0,
                const bool &monitor_in_simulation = false,
                const std::string &chromosome = "",
                const unsigned int &generation = 1,
                const std::string &name = "",
                const std::pair<unsigned int, unsigned int> &XY = helper::random_location(),
                const nlohmann::json &species_constants = nlohmann::json()
            ) const
    {
        return static_cast<const T&>(*this).clone(kind, age, monitor_in_simulation, chromosome, generation, name, XY, species_constants);
    }


    /********************
     *  Update methods  *
     ********************/

    void init_from_json(const nlohmann::json &data)
    {
        static_cast<T&>(*this).init_from_json(data);
    }

    // Generate fitness values
    void evaluate_dynamic_fitness()
    {
        static_cast<T&>(*this).evaluate_dynamic_fitness();
    }
    void evaluate_static_fitness()
    {
        static_cast<T&>(*this).evaluate_static_fitness();
    }

    // Miscellaneous methods
    void eat(const double &data)
    {
        static_cast<T&>(*this).eat(data);
    }
    void generate_death_factor()
    {
        static_cast<T&>(*this).generate_death_factor();
    }
    void increment_age()
    {
        static_cast<T&>(*this).increment_age();
    }
    void sleep(const double &data)
    {
        static_cast<T&>(*this).sleep(data);
    }

    // Update stats
    void decrement_stamina_by(const double &data)
    {
        static_cast<T&>(*this).decrement_stamina_by(data);
    }
    void decrement_vitality_by(const double &data)
    {
        static_cast<T&>(*this).decrement_vitality_by(data);
    }
    void increment_stamina_by(const double &data)
    {
        static_cast<T&>(*this).increment_stamina_by(data);
    }
    void increment_vitality_by(const double&data)
    {
        static_cast<T&>(*this).increment_vitality_by(data);
    }
};

#endif /* ORGANISM_HPP */
