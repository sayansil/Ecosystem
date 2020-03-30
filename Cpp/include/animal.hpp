#ifndef ANIMAL_HPP
#define ANIMAL_HPP

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


    /******************************
     *  Constructor / Destructor  *
     ******************************/

    Animal() = default;
    Animal(
        const std::string &kind,
        const unsigned int &age = 0,
        const bool &monitor_in_simulation = false,
        const std::string &chromosome = "",
        const unsigned int &generation = 1,
        const std::string &name = "",
        const std::pair<unsigned int, unsigned int> &XY = helper::random_location(),
        const nlohmann::json &species_constants = nlohmann::json());
    ~Animal();


    /***********************
     *  Virtual functions  *
     ***********************/

    std::shared_ptr<Entity> clone() const;
    std::shared_ptr<Entity> clone(
                const std::string &kind,
                const unsigned int &age = 0,
                const bool &monitor_in_simulation = false,
                const std::string &chromosome = "",
                const unsigned int &generation = 1,
                const std::string &name = "",
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
    STAT get_stat(const std::string &) const;
    std::string get_kingdom() const;
    bool is_normal_child() const;
};

#endif // ANIMAL_HPP
