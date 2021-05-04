#ifndef ANIMAL_HPP
#define ANIMAL_HPP

#include <filesystem>

#include <ecosystem_types.hpp>
#include <helper.hpp>
#include <nlohmann/json.hpp>
#include <organism.hpp>

class Animal : public Organism<Animal>
{
public:

    /***************************************
     *         Fixed for a species         *
     ***************************************/

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
    double vision_radius;
    double sleep_restore_factor;

    
    /***************************
     *  Fixed for an organism  *
     ***************************/
    
    double base_appetite;
    double base_height;
    double base_speed;
    double base_stamina;
    double base_vitality;
    double base_weight;
    double immunity_from_chromosome;
    unsigned int gender_from_chromosome;
    double height_multiplier;
    double speed_multiplier;
    double stamina_multiplier;
    double vitality_multiplier;
    double weight_multiplier;
    double max_height;
    double max_weight;
    
    /*************************************************************
     *  Changes slowly - once every year (during mating season)  *
     *************************************************************/

    // Stats affected by age
    double max_appetite_at_age;
    double max_speed_at_age;
    double max_stamina_at_age;
    double max_vitality_at_age;


    /**************************************
     *  Changes dynamically every moment  *
     **************************************/

    // Physical attributes
    double appetite;
    double speed;
    double stamina;
    double vitality;

    // Miscellanous attributes
    bool asleep;

    // Attribute map

    ATTRIBUTE_RAW_MAP attribute_raw_map;

    /******************************
     *  Constructor / Destructor  *
     ******************************/

    Animal() = default;
    Animal(
        const std::string &kind,
        const std::string &name = "",
        const unsigned int &age = 0,
        const bool &monitor_in_simulation = false,
        const std::string &chromosome = "",
        const unsigned int &generation = 1,
        const std::pair<unsigned int, unsigned int> &XY = helper::random_location(),
        const nlohmann::json &species_constants = nlohmann::json());
    ~Animal();


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
    double get_base_appetite() const;
    double get_base_height() const;
    double get_base_speed() const;
    double get_base_stamina() const;
    double get_base_vitality() const;
    double get_base_weight() const;
    double get_immunity_from_chromosome() const;
    unsigned int get_gender_from_chromosome() const;
    double get_height_multiplier() const;
    double get_speed_multiplier() const;
    double get_stamina_multiplier() const;
    double get_vitality_multiplier() const;
    double get_weight_multiplier() const;
    double get_max_height() const;
    double get_max_weight() const;
    ATTRIBUTE_RAW_MAP& get_attribute_raw_map();
    void init_from_json(const nlohmann::json &);
    void evaluate_dynamic_fitness();
    void evaluate_static_fitness();
    void eat(const double &);
    void generate_death_factor();
    void increment_age();
    void sleep(const double &);
    void decrement_stamina_by(const double &);
    void decrement_vitality_by(const double &);
    void increment_stamina_by(const double &);
    void increment_vitality_by(const double&);
    std::string get_kingdom() const;
    bool is_normal_child() const;
};

REFL_AUTO(
    type(Animal, bases<Organism<Animal>>),
    field(height_on_speed, fixed_for_species()),
    field(height_on_stamina, fixed_for_species()),
    field(height_on_vitality, fixed_for_species()),
    field(weight_on_speed, fixed_for_species()),
    field(weight_on_stamina, fixed_for_species()),
    field(weight_on_vitality, fixed_for_species()),
    field(vitality_on_appetite, fixed_for_species()),
    field(vitality_on_speed, fixed_for_species()),
    field(stamina_on_appetite, fixed_for_species()),
    field(stamina_on_speed, fixed_for_species()),
    field(theoretical_maximum_base_appetite, fixed_for_species()),
    field(theoretical_maximum_base_height, fixed_for_species()),
    field(theoretical_maximum_base_speed, fixed_for_species()),
    field(theoretical_maximum_base_stamina, fixed_for_species()),
    field(theoretical_maximum_base_vitality, fixed_for_species()),
    field(theoretical_maximum_base_weight, fixed_for_species()),
    field(theoretical_maximum_height, fixed_for_species()),
    field(theoretical_maximum_speed, fixed_for_species()),
    field(theoretical_maximum_weight, fixed_for_species()),
    field(theoretical_maximum_height_multiplier, fixed_for_species()),
    field(theoretical_maximum_speed_multiplier, fixed_for_species()),
    field(theoretical_maximum_stamina_multiplier, fixed_for_species()),
    field(theoretical_maximum_vitality_multiplier, fixed_for_species()),
    field(theoretical_maximum_weight_multiplier, fixed_for_species()),
    field(vision_radius, fixed_for_species()),
    field(sleep_restore_factor, fixed_for_species()),


    field(max_appetite_at_age, changes_every_year()),
    field(max_speed_at_age, changes_every_year()),
    field(max_stamina_at_age, changes_every_year()),
    field(max_vitality_at_age, changes_every_year()),

    field(appetite, changes_every_moment()),
    field(speed, changes_every_moment()),
    field(stamina, changes_every_moment()),
    field(vitality, changes_every_moment()),
    field(asleep, changes_every_moment())
)


#endif // ANIMAL_HPP
