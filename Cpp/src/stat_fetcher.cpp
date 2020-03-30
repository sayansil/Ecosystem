#include <stat_fetcher.hpp>

std::vector<ENTITY> getOrganismVector(const ENTITY_MAP_TYPE &organisms)
{
    std::vector<ENTITY> organisms_vec;
    for (auto &organism : organisms)
    {
        organisms_vec.push_back(organism.second);
    }

    return organisms_vec;
}

namespace stat_fetcher
{
    double getGenderRatio(const ENTITY_MAP_TYPE &organisms, const std::string  &kind)
    {
        unsigned int M = 0, F = 0;
        for (auto organism : organisms)
        {
            if (kind != "" && kind != organism.second->get_kind())
                continue;

            if (organism.second->get_gender() == MALE)
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

    unsigned int getPopulation(const ENTITY_MAP_TYPE &organisms, const std::string &kind)
    {
        unsigned int count = 0;
        for (const auto &organism: organisms)
        {
            if (kind != "" && kind != organism.second->get_kind())
                continue;

            count++;
        }

        return count;
    }

    std::pair<unsigned int, unsigned int> getMatablePopulation(const ENTITY_MAP_TYPE &organisms, const std::string &kind)
    {
        unsigned int M = 0, F = 0;

        for (const auto &organism: organisms)
        {
            if (kind != "" && kind != organism.second->get_kind())
                continue;

            if (organism.second->get_age() >= organism.second->get_mating_age_start() && organism.second->get_age() <= organism.second->get_mating_age_end())
            {
                if (organism.second->get_gender() == MALE)
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

    std::pair<double, double> getStatGap(const ENTITY_MAP_TYPE &organisms, const std::string &attribute, const std::string &kind)
    {
        double low = 0.0, high = 0.0, value;
        STAT current_attribute;
        bool uninitialized = true;
        for (const auto &organism : organisms)
        {
            if (kind != "" && kind != organism.second->get_kind())
                continue;

            current_attribute = organism.second->get_stat(attribute);
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

    std::unordered_map<std::string, unsigned int> getKindDistribution(const ENTITY_MAP_TYPE &organisms)
    {
        std::unordered_map<std::string, unsigned int> kindDistribution;

        for (const auto &organism: organisms)
        {
            if(kindDistribution.find(organism.second->get_kind()) == kindDistribution.end())
            {
                kindDistribution[organism.second->get_kind()] = 1;
            }
            else
            {
                kindDistribution[organism.second->get_kind()]++;
            }
        }

        return kindDistribution;
    }

    double getStatAverage(const ENTITY_MAP_TYPE &organisms, const std::string &attribute, const std::string &kind)
    {
        double average = 0.0, n = 0.0, value = 0.0;

        // avg(n+1) = (n / (n + 1)) * avg(n) + (1 / (n + 1)) * kn

        for (const auto &organism: organisms)
        {
            if (kind != "" && kind != organism.second->get_kind())
                continue;

            average = (n / (n + 1)) * average + ((double)organism.second->get_stat(attribute) / (n + 1));
            n++;
        }

        return average;
    }

    std::vector<STAT> getOneStat(const ENTITY_MAP_TYPE &organisms, const std::string &attribute, const std::string &kind)
    {
        std::vector<STAT> attributeList;

        for (const auto &organism : organisms)
        {
            if (kind != "" && kind != organism.second->get_kind())
                continue;

            attributeList.push_back(organism.second->get_stat(attribute));
        }

        return attributeList;
    }

    std::unordered_map<std::string, double> getAllGenericStats(const ENTITY_MAP_TYPE &organisms, const std::string &kind)
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
        stat_display_map["average_static_fitness"] = 0.0;
        stat_display_map["average_death_factor"] = 0.0;

        for (const auto &organism: organisms)
        {
            if (kind != "" && kind != organism.second->get_kind())
                continue;

            if (organism.second->get_gender() == MALE)
            {
                stat_display_map["male"]++;

                if (organism.second->get_age() >= organism.second->get_mating_age_start() && organism.second->get_age() <= organism.second->get_mating_age_end())
                {
                    stat_display_map["matable_male"]++;
                }
            }
            else
            {
                stat_display_map["female"]++;

                if (organism.second->get_age() >= organism.second->get_mating_age_start() && organism.second->get_age() <= organism.second->get_mating_age_end())
                {
                    stat_display_map["matable_female"]++;
                }
            }

            tmp_d = organism.second->get_stat("conceiving_probability"); stat_display_map["conceiving_probability"] = tmp_d;
            tmp_i = organism.second->get_stat("mating_age_start"); stat_display_map["mating_age_start"] = (double)tmp_i;
            tmp_i = organism.second->get_stat("mating_age_end"); stat_display_map["mating_age_end"] = (double)tmp_i;
            tmp_i = organism.second->get_stat("max_age"); stat_display_map["max_age"] = (double)tmp_i;
            tmp_d = organism.second->get_stat("mutation_probability"); stat_display_map["mutation_probability"] = tmp_d;
            tmp_d = organism.second->get_stat("offsprings_factor"); stat_display_map["offsprings_factor"] = tmp_d;
            tmp_d = organism.second->get_stat("age_on_death"); stat_display_map["age_on_death"] = tmp_d;
            tmp_d = organism.second->get_stat("fitness_on_death"); stat_display_map["fitness_on_death"] = tmp_d;
            tmp_d = organism.second->get_stat("age_fitness_on_death_ratio"); stat_display_map["age_fitness_on_death_ratio"] = tmp_d;
            tmp_d = organism.second->get_stat("sleep_restore_factor"); stat_display_map["sleep_restore_factor"] = tmp_d;
            tmp_i = organism.second->get_stat("generation"); stat_display_map["average_generation"] = (count / (count + 1)) * stat_display_map["average_generation"] + ((double)tmp_i / (count + 1));
            tmp_d = organism.second->get_stat("immunity"); stat_display_map["average_immunity"] = (count / (count + 1)) * stat_display_map["average_immunity"] + (tmp_d / (count + 1));
            tmp_i = organism.second->get_stat("age"); stat_display_map["average_age"] = (count / (count + 1)) * stat_display_map["average_age"] + ((double)tmp_i / (count + 1));
            tmp_d = organism.second->get_stat("height"); stat_display_map["average_height"] = (count / (count + 1)) * stat_display_map["average_height"] + (tmp_d / (count + 1));
            tmp_d = organism.second->get_stat("weight"); stat_display_map["average_weight"] = (count / (count + 1)) * stat_display_map["average_weight"] + (tmp_d / (count + 1));
            tmp_d = organism.second->get_stat("static_fitness"); stat_display_map["average_static_fitness"] = (count / (count + 1)) * stat_display_map["average_static_fitness"] + (tmp_d / (count + 1));
            tmp_d = organism.second->get_stat("death_factor"); stat_display_map["average_death_factor"] = (count / (count + 1)) * stat_display_map["average_death_factor"] + (tmp_d / (count + 1));

            count++;
        }

        return stat_display_map;
    }

    std::string prepareDataForSimulation(const ENTITY_MAP_TYPE &organisms)
    {
        nlohmann::json response;
        response["animal"] = nlohmann::json::array();
        response["plant"] = nlohmann::json::array();
        response["status"] = "failure";
        response["log"] = "";
        if(organisms.size() < 1)
        {
            response["log"] = "Underpopulation";
            return response.dump();
        }
        if(organisms.size() > 35000)
        {
            response["log"] = "Overpopulation";
            return response.dump();
        }
        for(const auto &organism : organisms)
        {
            if(organism.second->get_monitor_in_simulation())
            {
                if(organism.second->get_kingdom() == "animal")
                {
                    Animal *obj = static_cast<Animal*>(organism.second.get());
                    response["animal"].push_back({
                                {"name", obj->name},
                                {"food_chain_rank", obj->food_chain_rank},
                                {"vision_radius", obj->vision_radius},
                                {"max_speed_at_age", obj->max_speed_at_age},
                                {"max_vitality_at_age", obj->max_vitality_at_age},
                                {"max_appetite_at_age", obj->max_appetite_at_age},
                                {"X", obj->X},
                                {"Y", obj->Y}
                            });
                }
                else if(organism.second->get_kingdom() == "plant")
                {
                    Plant *obj = static_cast<Plant*>(organism.second.get());
                    response["plant"].push_back({
                                {"name", obj->name},
                                {"food_chain_rank", obj->food_chain_rank},
                                {"max_vitality_at_age", obj->max_vitality_at_age},
                                {"X", obj->X},
                                {"Y", obj->Y}
                            });
                }
                else
                {
                    std::cout << "Kingdom not supported\n";
                }
            }
        }
        response["status"] = "success";
        return response.dump();
    }
};
