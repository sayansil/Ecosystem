#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <memory>
#include <nlohmann/json.hpp>
#include <helper.hpp>
#include <ecosystem_types.hpp>

class Entity
{
public:

    /********************
     *      Getters     *
     ********************/

    virtual double get_base_appetite() const = 0;
    virtual double get_base_height() const = 0;
    virtual double get_base_speed() const = 0;
    virtual double get_base_stamina() const = 0;
    virtual double get_base_vitality() const = 0;
    virtual double get_base_weight() const = 0;
    virtual double get_height_multiplier() const = 0;
    virtual double get_speed_multiplier() const = 0;
    virtual double get_stamina_multiplier() const = 0;
    virtual double get_vitality_multiplier() const = 0;
    virtual double get_weight_multiplier() const = 0;
    virtual double get_max_height() const = 0;
    virtual double get_max_weight() const = 0;
    virtual double get_die_of_age_factor() const = 0;
    virtual double get_die_of_fitness_factor() const = 0;
    virtual double get_fitness() const = 0;
    virtual double get_immunity() const = 0;
    virtual double get_height() const = 0;
    virtual double get_weight() const = 0;
    virtual double get_static_fitness() const = 0;
    virtual double get_mutation_probability() const = 0;
    virtual double get_conceiving_probability() const = 0;
    virtual double get_death_factor() const = 0;
    virtual double get_offsprings_factor() const = 0;
    virtual unsigned int get_gender() const = 0;
    virtual unsigned int get_mating_age_start() const = 0;
    virtual unsigned int get_mating_age_end() const = 0;
    virtual unsigned int get_age() const = 0;
    virtual unsigned int get_generation() const = 0;
    virtual unsigned int get_X() const = 0;
    virtual unsigned int get_Y() const = 0;
    virtual unsigned int get_is_asexual() const = 0;
    virtual unsigned int get_food_chain_rank() const = 0;
    virtual std::string get_kind() const = 0;
    virtual std::string get_full_species_name() const = 0;
    virtual std::string get_name() const = 0;
    virtual std::string get_chromosome() const = 0;
    virtual std::string get_kingdom() const = 0;
    virtual STAT get_stat(const std::string &data) const = 0;
    virtual bool get_monitor_in_simulation() const = 0;


    /**************************
     *      Miscellaneous     *
     **************************/

    virtual void generate_death_factor() = 0;
    virtual void increment_age() = 0;
    virtual bool is_normal_child() const = 0;

    virtual std::shared_ptr<Entity> clone() const = 0;
    virtual std::shared_ptr<Entity> clone(
                const std::string &kind,
                const unsigned int &age = 0,
                const bool &monitor_in_simulation = false,
                const std::string &chromosome = "",
                const unsigned int &generation = 1,
                const std::string &name = "",
                const std::pair<unsigned int, unsigned int> &XY = helper::random_location(),
                const nlohmann::json &species_constants = nlohmann::json()
            ) const = 0;
};

using ENTITY = std::shared_ptr<Entity>;

#endif /* ENTITY_HPP */
