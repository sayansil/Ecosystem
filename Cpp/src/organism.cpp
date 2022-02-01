#include <organism.hpp>
#include <helper.hpp>

namespace organism_opts
{
    double get_value_from_chromosome(const Ecosystem::Organism *organism, const std::string &code, const double &multiplier)
    {
        std::string chromosome = organism->chromosome()->str();
        int start = organism->chromosome_structure()->structure()->LookupByKey(code.c_str())->start();
        int length = organism->chromosome_structure()->structure()->LookupByKey(code.c_str())->length();

        if (length == 0)
            return 0;
        return (helper::to_decimal(chromosome.substr(start, length)) / static_cast<double>(1 << length)) * multiplier;
    }

    double get_immunity(const Ecosystem::Organism *organism)
    {
        return get_value_from_chromosome(organism, "im", 1.0);
    }
    double get_base_vitality(const Ecosystem::Organism *organism)
    {
        return get_value_from_chromosome(organism, "bv", organism->theoretical_maximum_base_vitality());
    }
    double get_base_weight(const Ecosystem::Organism *organism)
    {
        return get_value_from_chromosome(organism, "bw", organism->theoretical_maximum_base_weight());
    }
    double get_height_multiplier(const Ecosystem::Organism *organism)
    {
        return get_value_from_chromosome(organism, "hm", organism->theoretical_maximum_height_multiplier());
    }
    double get_vitality_multiplier(const Ecosystem::Organism *organism)
    {
        return get_value_from_chromosome(organism, "vm", organism->theoretical_maximum_vitality_multiplier());
    }
    double get_weight_multiplier(const Ecosystem::Organism *organism)
    {
        return get_value_from_chromosome(organism, "wm", organism->theoretical_maximum_weight_multiplier());
    }
    double get_max_height(const Ecosystem::Organism *organism)
    {
        return get_value_from_chromosome(organism, "mh", organism->theoretical_maximum_height());
    }
    double get_max_weight(const Ecosystem::Organism *organism)
    {
        return get_value_from_chromosome(organism, "mw", organism->theoretical_maximum_weight());
    }
    int8_t get_gender(const Ecosystem::Organism *organism)
    {
        return (int8_t)get_value_from_chromosome(organism, "gn", 2.0);
    }
    double get_base_appetite(const Ecosystem::Organism *organism)
    {
        return get_value_from_chromosome(organism, "ba", organism->theoretical_maximum_base_appetite());
    }
    double get_base_height(const Ecosystem::Organism *organism)
    {
        return get_value_from_chromosome(organism, "bh", organism->theoretical_maximum_base_height());
    }
    double get_base_speed(const Ecosystem::Organism *organism)
    {
        return get_value_from_chromosome(organism, "bp", organism->theoretical_maximum_base_speed());
    }
    double get_base_stamina(const Ecosystem::Organism *organism)
    {
        return get_value_from_chromosome(organism, "bs", organism->theoretical_maximum_base_stamina());
    }
    double get_speed_multiplier(const Ecosystem::Organism *organism)
    {
        return get_value_from_chromosome(organism, "pm", organism->theoretical_maximum_speed_multiplier());
    }
    double get_stamina_multiplier(const Ecosystem::Organism *organism)
    {
        return get_value_from_chromosome(organism, "sm", organism->theoretical_maximum_stamina_multiplier());
    }
};
