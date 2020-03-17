#include "nlohmann/json_fwd.hpp"
#include <filesystem>
#include <stat_fetcher.hpp>
#include <string>

std::vector<Animal> getAnimalVector(const ANIMAL_MAP_TYPE &animals)
{
    std::vector<Animal> animals_vec;
    for (auto &animal : animals)
    {
        animals_vec.push_back(animal.second);
    }

    return animals_vec;
}

namespace stat_fetcher
{
    double getGenderRatio(const ANIMAL_MAP_TYPE &animals, const std::string & kind)
    {
        unsigned int M = 0, F = 0;
        for (auto animal : animals)
        {
            if (kind != "" && kind != animal.second.kind)
                continue;

            if (animal.second.gender == MALE)
            {
                M++;
            }
            else
            {
                F++;
            }
        }

        return (1.0 * M) / F;
    }

    unsigned int getPopulation(const ANIMAL_MAP_TYPE &animals, const std::string &kind)
    {
        unsigned int count = 0;
        for (const auto &animal: animals)
        {
            if (kind != "" && kind != animal.second.kind)
                continue;

            count++;
        }

        return count;
    }

    std::pair<unsigned int, unsigned int> getMatablePopulation(const ANIMAL_MAP_TYPE &animals, const std::string &kind)
    {
        unsigned int M = 0, F = 0;

        for (const auto &animal: animals)
        {
            if (kind != "" && kind != animal.second.kind)
                continue;

            if (animal.second.age >= animal.second.mating_age_start && animal.second.age <= animal.second.mating_age_end)
            {
                if (animal.second.gender == MALE)
                {
                    M++;
                }
                else
                {
                    F++;
                }
            }
        }

        return {M, F};
    }

    std::pair<double, double> getStatGap(const ANIMAL_MAP_TYPE &animals, const std::string &attribute, const std::string &kind)
    {
        double low = 0.0, high = 0.0, value;
        STAT current_attribute;
        bool uninitialized = true;
        for (const auto &animal : animals)
        {
            if (kind != "" && kind != animal.second.kind)
                continue;

            current_attribute = animal.second.get_stat(attribute);
            if(current_attribute.data.index() == 0)
            {
                value = (unsigned int)current_attribute;
            }
            else if(current_attribute.data.index() == 1)
            {
                value = current_attribute;
            }
            else
            {
                break;
            }
            if (uninitialized)
            {
                low = value;
                high = value;
                uninitialized = false;
            }
            else
            {
                if (low > value)
                {
                    low = value;
                }
                if (high < value)
                {
                    high = value;
                }
            }
        }

        return {low, high};
    }

    std::unordered_map<std::string, unsigned int> getKindDistribution(const ANIMAL_MAP_TYPE &animals)
    {
        std::unordered_map<std::string, unsigned int> kindDistribution;

        for (const auto &animal: animals)
        {
            if(kindDistribution.find(animal.second.kind) == kindDistribution.end())
            {
                kindDistribution[animal.second.kind] = 1;
            }
            else
            {
                kindDistribution[animal.second.kind]++;
            }
        }

        return kindDistribution;
    }

    double getStatAverage(const ANIMAL_MAP_TYPE &animals, const std::string &attribute, const std::string &kind)
    {
        double average = 0.0, n = 0.0, value = 0.0;

        // avg(n+1) = (n / (n + 1)) * avg(n) + (1 / (n + 1)) * kn

        for (const auto &animal: animals)
        {
            if (kind != "" && kind != animal.second.kind)
                continue;

            average = (n / (n + 1)) * average + ((double)animal.second.get_stat(attribute) / (n + 1));
            n++;
        }

        return average;
    }

    std::vector<STAT> getOneStat(const ANIMAL_MAP_TYPE &animals, const std::string &attribute, const std::string &kind)
    {
        std::vector<STAT> attributeList;

        for (const auto &animal : animals)
        {
            if (kind != "" && kind != animal.second.kind)
                continue;

            attributeList.push_back(animal.second.get_stat(attribute));
        }

        return attributeList;
    }

    std::unordered_map<std::string, double> getAllStats(const ANIMAL_MAP_TYPE &animals, const std::string &kind)
    {
        std::unordered_map<std::string, double> stat_display_map;
        unsigned int tmp_i;
        double tmp_d;

        unsigned int count = 0;

        stat_display_map["male"] = 0.0;
        stat_display_map["female"] = 0.0;
        stat_display_map["matable_male"] = 0.0;
        stat_display_map["matable_female"] = 0.0;
        stat_display_map["average_generation"] = 0.0;
        stat_display_map["average_immunity"] = 0.0;
        stat_display_map["average_age"] = 0.0;
        stat_display_map["average_height"] = 0.0;
        stat_display_map["average_weight"] = 0.0;
        stat_display_map["average_max_appetite_at_age"] = 0.0;
        stat_display_map["average_max_speed_at_age"] = 0.0;
        stat_display_map["average_max_stamina_at_age"] = 0.0;
        stat_display_map["average_max_vitality_at_age"] = 0.0;
        stat_display_map["average_static_fitness"] = 0.0;
        stat_display_map["average_death_factor"] = 0.0;
        stat_display_map["average_vision_radius"] = 0.0;

        for (const auto &animal: animals)
        {
            if (kind != "" && kind != animal.second.kind)
                continue;

            if (animal.second.gender == MALE)
            {
                stat_display_map["male"]++;

                if (animal.second.age >= animal.second.mating_age_start && animal.second.age <= animal.second.mating_age_end)
                {
                    stat_display_map["matable_male"]++;
                }
            }
            else
            {
                stat_display_map["female"]++;

                if (animal.second.age >= animal.second.mating_age_start && animal.second.age <= animal.second.mating_age_end)
                {
                    stat_display_map["matable_female"]++;
                }
            }

            tmp_d = animal.second.get_stat("conceiving_probability"); stat_display_map["conceiving_probability"] = tmp_d;
            tmp_i = animal.second.get_stat("mating_age_start"); stat_display_map["mating_age_start"] = (double)tmp_i;
            tmp_i = animal.second.get_stat("mating_age_end"); stat_display_map["mating_age_end"] = (double)tmp_i;
            tmp_i = animal.second.get_stat("max_age"); stat_display_map["max_age"] = (double)tmp_i;
            tmp_d = animal.second.get_stat("mutation_probability"); stat_display_map["mutation_probability"] = tmp_d;
            tmp_d = animal.second.get_stat("offsprings_factor"); stat_display_map["offsprings_factor"] = tmp_d;
            tmp_d = animal.second.get_stat("age_on_death"); stat_display_map["age_on_death"] = tmp_d;
            tmp_d = animal.second.get_stat("fitness_on_death"); stat_display_map["fitness_on_death"] = tmp_d;
            tmp_d = animal.second.get_stat("age_fitness_on_death_ratio"); stat_display_map["age_fitness_on_death_ratio"] = tmp_d;
            tmp_d = animal.second.get_stat("height_on_speed"); stat_display_map["height_on_speed"] = tmp_d;
            tmp_d = animal.second.get_stat("height_on_stamina"); stat_display_map["height_on_stamina"] = tmp_d;
            tmp_d = animal.second.get_stat("height_on_vitality"); stat_display_map["height_on_vitality"] = tmp_d;
            tmp_d = animal.second.get_stat("weight_on_speed"); stat_display_map["weight_on_speed"] = tmp_d;
            tmp_d = animal.second.get_stat("weight_on_stamina"); stat_display_map["weight_on_stamina"] = tmp_d;
            tmp_d = animal.second.get_stat("weight_on_vitality"); stat_display_map["weight_on_vitality"] = tmp_d;
            tmp_d = animal.second.get_stat("vitality_on_appetite"); stat_display_map["vitality_on_appetite"] = tmp_d;
            tmp_d = animal.second.get_stat("vitality_on_speed"); stat_display_map["vitality_on_speed"] = tmp_d;
            tmp_d = animal.second.get_stat("stamina_on_appetite"); stat_display_map["stamina_on_appetite"] = tmp_d;
            tmp_d = animal.second.get_stat("stamina_on_speed"); stat_display_map["stamina_on_speed"] = tmp_d;
            tmp_d = animal.second.get_stat("theoretical_maximum_base_appetite"); stat_display_map["theoretical_maximum_base_appetite"] = tmp_d;
            tmp_d = animal.second.get_stat("theoretical_maximum_base_height"); stat_display_map["theoretical_maximum_base_height"] = tmp_d;
            tmp_d = animal.second.get_stat("theoretical_maximum_base_speed"); stat_display_map["theoretical_maximum_base_speed"] = tmp_d;
            tmp_d = animal.second.get_stat("theoretical_maximum_base_stamina"); stat_display_map["theoretical_maximum_base_stamina"] = tmp_d;
            tmp_d = animal.second.get_stat("theoretical_maximum_base_vitality"); stat_display_map["theoretical_maximum_base_vitality"] = tmp_d;
            tmp_d = animal.second.get_stat("theoretical_maximum_base_weight"); stat_display_map["theoretical_maximum_base_weight"] = tmp_d;
            tmp_d = animal.second.get_stat("theoretical_maximum_height"); stat_display_map["theoretical_maximum_height"] = tmp_d;
            tmp_d = animal.second.get_stat("theoretical_maximum_speed"); stat_display_map["theoretical_maximum_speed"] = tmp_d;
            tmp_d = animal.second.get_stat("theoretical_maximum_weight"); stat_display_map["theoretical_maximum_weight"] = tmp_d;
            tmp_d = animal.second.get_stat("theoretical_maximum_height_multiplier"); stat_display_map["theoretical_maximum_height_multiplier"] = tmp_d;
            tmp_d = animal.second.get_stat("theoretical_maximum_speed_multiplier"); stat_display_map["theoretical_maximum_speed_multiplier"] = tmp_d;
            tmp_d = animal.second.get_stat("theoretical_maximum_stamina_multiplier"); stat_display_map["theoretical_maximum_stamina_multiplier"] = tmp_d;
            tmp_d = animal.second.get_stat("theoretical_maximum_vitality_multiplier"); stat_display_map["theoretical_maximum_vitality_multiplier"] = tmp_d;
            tmp_d = animal.second.get_stat("theoretical_maximum_weight_multiplier"); stat_display_map["theoretical_maximum_weight_multiplier"] = tmp_d;
            tmp_d = animal.second.get_stat("sleep_restore_factor"); stat_display_map["sleep_restore_factor"] = tmp_d;

            tmp_i = animal.second.get_stat("generation"); stat_display_map["average_generation"] = (count / (count + 1)) * stat_display_map["average_generation"] + ((double)tmp_i / (count + 1));
            tmp_d = animal.second.get_stat("immunity"); stat_display_map["average_immunity"] = (count / (count + 1)) * stat_display_map["average_immunity"] + (tmp_d / (count + 1));
            tmp_i = animal.second.get_stat("age"); stat_display_map["average_age"] = (count / (count + 1)) * stat_display_map["average_age"] + ((double)tmp_i / (count + 1));
            tmp_d = animal.second.get_stat("height"); stat_display_map["average_height"] = (count / (count + 1)) * stat_display_map["average_height"] + (tmp_d / (count + 1));
            tmp_d = animal.second.get_stat("weight"); stat_display_map["average_weight"] = (count / (count + 1)) * stat_display_map["average_weight"] + (tmp_d / (count + 1));
            tmp_d = animal.second.get_stat("max_appetite_at_age"); stat_display_map["average_max_appetite_at_age"] = (count / (count + 1)) * stat_display_map["average_max_appetite_at_age"] + (tmp_d / (count + 1));
            tmp_d = animal.second.get_stat("max_speed_at_age"); stat_display_map["average_max_speed_at_age"] = (count / (count + 1)) * stat_display_map["average_max_speed_at_age"] + (tmp_d / (count + 1));
            tmp_d = animal.second.get_stat("max_stamina_at_age"); stat_display_map["average_max_stamina_at_age"] = (count / (count + 1)) * stat_display_map["average_max_stamina_at_age"] + (tmp_d / (count + 1));
            tmp_d = animal.second.get_stat("max_vitality_at_age"); stat_display_map["average_max_vitality_at_age"] = (count / (count + 1)) * stat_display_map["average_max_vitality_at_age"] + (tmp_d / (count + 1));
            tmp_d = animal.second.get_stat("static_fitness"); stat_display_map["average_static_fitness"] = (count / (count + 1)) * stat_display_map["average_static_fitness"] + (tmp_d / (count + 1));
            tmp_d = animal.second.get_stat("death_factor"); stat_display_map["average_death_factor"] = (count / (count + 1)) * stat_display_map["average_death_factor"] + (tmp_d / (count + 1));
            tmp_d = animal.second.get_stat("vision_radius"); stat_display_map["average_vision_radius"] = (count / (count + 1)) * stat_display_map["average_vision_radius"] + (tmp_d / (count + 1));

            count++;
        }

        return stat_display_map;
    }
    
    std::string generateDataForPy(const ANIMAL_MAP_TYPE& animals)
    {
        nlohmann::json master_data;

        std::filesystem::path species_folder = "../../data/json";

        for(auto file : std::filesystem::directory_iterator(species_folder))
        {
            nlohmann::json species_json;
            std::string current_species = file.path();
            current_species = current_species.substr(current_species.find_last_of('/') + 1);
            const std::string filepath = file / std::filesystem::path("current.json"); 
            std::ifstream in(filepath);
            nlohmann::json json_file;
            in >> json_file;
            species_json["food_chain_rank"] = json_file["food_chain_rank"];
            species_json["vision_radius"] = json_file["vision_radius"];
            species_json["theoretical_maximum_speed"] = json_file["theoretical_maximum_speed"];
            species_json["theoretical_maximum_base_vitality"] = json_file["theoretical_maximum_base_vitality"];
            species_json["species_theoretical_maximum_base_appetite"] = json_file["theoretical_maximum_base_appetite"];
            species_json["population"] = getPopulation(animals, current_species);
            in.close();
            master_data[current_species] = species_json;
        }
        return master_data.dump();
    }
};
