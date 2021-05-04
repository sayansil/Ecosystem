#ifndef PLANT_HPP
#define PLANT_HPP

#include <climits>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <map>
#include <math.h>
#include <variant>
#include <filesystem>

#include <ecosystem_types.hpp>
#include <helper.hpp>
#include <nlohmann/json.hpp>
#include <organism.hpp>

class Plant : public Organism<Plant>
{
public:

    /***************************************
     *         Fixed for a species         *
    ****************************************/

    // Height dependance factors
    double height_on_vitality;

    // Weight dependance factors
    double weight_on_vitality;

    // Maximum base (starting) stats
    double theoretical_maximum_base_height;
    double theoretical_maximum_base_vitality;
    double theoretical_maximum_base_weight;

    // Maximum attainable stats
    double theoretical_maximum_height;
    double theoretical_maximum_weight;

    // Stat multiplier (with age)
    double theoretical_maximum_height_multiplier;
    double theoretical_maximum_vitality_multiplier;
    double theoretical_maximum_weight_multiplier;

    /***************************
     *  Fixed for an organism  *
     ***************************/
    
    double base_height;
    double base_vitality;
    double base_weight;
    double immunity_from_chromosome;
    unsigned int gender_from_chromosome;
    double height_multiplier;
    double vitality_multiplier;
    double weight_multiplier;
    double max_height;
    double max_weight;

    /*************************************************************
     *  Changes slowly - once every year (during mating season)  *
    **************************************************************/

    // Stats affected by age
    double max_vitality_at_age;


    /**************************************
     *  Changes dynamically every moment  *
    ***************************************/

    // Physical attributes
    double vitality;


    // Attribute map

    ATTRIBUTE_RAW_MAP attribute_raw_map;


    /******************************
     *  Constructor / Destructor  *
    *******************************/

    Plant() = default;
    Plant(
        const std::string &kind,
        const std::string &name = "",
        const unsigned int &age = 0,
        const bool &monitor_in_simulation = false,
        const std::string &chromosome = "",
        const unsigned int &generation = 1,
        const std::pair<unsigned int, unsigned int> &XY = helper::random_location(),
        const nlohmann::json &species_constants = nlohmann::json());
    ~Plant();

    /***********************
     *  Virtual functions  *
     ***********************/

    std::shared_ptr<Entity> clone() const;
    std::shared_ptr<Entity> clone(
                const std::string &kind,
                const std::string &name = "",
                const unsigned int &age = 0,
                const bool &monitor_in_simulation = false,
                const std::string &chromosome = "",
                const unsigned int &generation = 1,
                const std::pair<unsigned int, unsigned int> &XY = helper::random_location(),
                const nlohmann::json &species_constants = nlohmann::json()
            ) const;
    double get_base_height() const;
    double get_base_vitality() const;
    double get_base_weight() const;
    double get_immunity_from_chromosome() const;
    unsigned int get_gender_from_chromosome() const;
    double get_height_multiplier() const;
    double get_vitality_multiplier() const;
    double get_weight_multiplier() const;
    double get_max_height() const;
    double get_max_weight() const;
    ATTRIBUTE_RAW_MAP& get_attribute_raw_map();
    void init_from_json(const nlohmann::json &);
    void evaluate_dynamic_fitness();
    void evaluate_static_fitness();
    void generate_death_factor();
    void increment_age();
    void decrement_vitality_by(const double &);
    void increment_vitality_by(const double&);
    std::string get_kingdom() const;
    bool is_normal_child() const;

};

REFL_AUTO(
    type(Plant, bases<Organism<Plant>>),
    field(height_on_vitality, fixed_for_species()),
    field(weight_on_vitality, fixed_for_species()),
    field(theoretical_maximum_base_height, fixed_for_species()),
    field(theoretical_maximum_base_vitality, fixed_for_species()),
    field(theoretical_maximum_base_weight, fixed_for_species()),
    field(theoretical_maximum_height, fixed_for_species()),
    field(theoretical_maximum_weight, fixed_for_species()),
    field(theoretical_maximum_height_multiplier, fixed_for_species()),
    field(theoretical_maximum_vitality_multiplier, fixed_for_species()),
    field(theoretical_maximum_weight_multiplier, fixed_for_species()),

    field(max_vitality_at_age, changes_every_year()),

    field(vitality, changes_every_moment())
)

#endif /* PLANT_HPP */
