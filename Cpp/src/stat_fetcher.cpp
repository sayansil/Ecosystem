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
    double get_gender_ratio(const ENTITY_MAP_TYPE &organisms, const std::string  &kind)
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

    unsigned int get_population(const ENTITY_MAP_TYPE &organisms, const std::string &kind)
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

    std::pair<unsigned int, unsigned int> get_matable_population(const ENTITY_MAP_TYPE &organisms, const std::string &kind)
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

    std::pair<double, double> get_stat_gap(const ENTITY_MAP_TYPE &organisms, const std::string &attribute, const std::string &kind)
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

    std::unordered_map<std::string, unsigned int> get_kind_distribution(const ENTITY_MAP_TYPE &organisms)
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

    double get_stat_average(const ENTITY_MAP_TYPE &organisms, const std::string &attribute, const std::string &kind)
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

    std::vector<STAT> get_one_stat(const ENTITY_MAP_TYPE &organisms, const std::string &attribute, const std::string &kind)
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

    std::string prepare_data_for_simulation(const ENTITY_MAP_TYPE &organisms)
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
                                {"max_stamina_at_age", obj->max_stamina_at_age},
                                {"height", obj->height},
                                {"weight", obj->weight}
                            });
                }
                else if(organism.second->get_kingdom() == "plant")
                {
                    Plant *obj = static_cast<Plant*>(organism.second.get());
                    response["plant"].push_back({
                                {"name", obj->name},
                                {"food_chain_rank", obj->food_chain_rank},
                                {"max_vitality_at_age", obj->max_vitality_at_age},
                                {"height", obj->height},
                                {"weight", obj->weight}
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

    std::vector<STAT> get_db_row(const ENTITY_MAP_TYPE &organisms, const std::string &kind, const std::string &kingdom, const unsigned int &year)
    {
        std::vector<STAT> db_row;

        if (kingdom == "animal")
        {
            std::unordered_map<std::string, double> stat_db_map;
            unsigned int tmp_i;
            double tmp_d;

            unsigned int count = 0;

            stat_db_map["MALE"] = 0.0;
            stat_db_map["FEMALE"] = 0.0;
            stat_db_map["M_MALE"] = 0.0;
            stat_db_map["M_FEMALE"] = 0.0;
            stat_db_map["AVG_GEN"] = 0.0;
            stat_db_map["AVG_IMM"] = 0.0;
            stat_db_map["AVG_AGE"] = 0.0;
            stat_db_map["AVG_HT"] = 0.0;
            stat_db_map["AVG_WT"] = 0.0;
            stat_db_map["AVGMA_AP"] = 0.0;
            stat_db_map["AVGMA_SP"] = 0.0;
            stat_db_map["AVGMA_ST"] = 0.0;
            stat_db_map["AVGMA_VT"] = 0.0;
            stat_db_map["AVG_SFIT"] = 0.0;
            stat_db_map["AVG_DTHF"] = 0.0;
            stat_db_map["AVG_VIS"] = 0.0;

            for (const auto &organism: organisms)
            {
                const Animal *obj = static_cast<const Animal *>(organism.second.get());

                if (kind != obj->kind)
                    continue;

                if (obj->gender == MALE)
                {
                    stat_db_map["MALE"]++;

                    if (obj->age >= obj->mating_age_start && obj->age <= obj->mating_age_end)
                    {
                        stat_db_map["M_MALE"]++;
                    }
                }
                else
                {
                    stat_db_map["FEMALE"]++;

                    if (obj->age >= obj->mating_age_start && obj->age <= obj->mating_age_end)
                    {
                        stat_db_map["M_FEMALE"]++;
                    }
                }

                tmp_d = obj->get_stat("conceiving_probability"); stat_db_map["C_PROB"] = tmp_d;
                tmp_i = obj->get_stat("mating_age_start"); stat_db_map["M_AGE_START"] = (double)tmp_i;
                tmp_i = obj->get_stat("mating_age_end"); stat_db_map["M_AGE_END"] = (double)tmp_i;
                tmp_i = obj->get_stat("max_age"); stat_db_map["MX_AGE"] = (double)tmp_i;
                tmp_d = obj->get_stat("mutation_probability"); stat_db_map["MT_PROB"] = tmp_d;
                tmp_d = obj->get_stat("offsprings_factor"); stat_db_map["OF_FACTOR"] = tmp_d;
                tmp_d = obj->get_stat("age_on_death"); stat_db_map["AGE_DTH"] = tmp_d;
                tmp_d = obj->get_stat("fitness_on_death"); stat_db_map["FIT_DTH"] = tmp_d;
                tmp_d = obj->get_stat("age_fitness_on_death_ratio"); stat_db_map["AFR_DTH"] = tmp_d;
                tmp_d = obj->get_stat("height_on_speed"); stat_db_map["HT_SP"] = tmp_d;
                tmp_d = obj->get_stat("height_on_stamina"); stat_db_map["HT_ST"] = tmp_d;
                tmp_d = obj->get_stat("height_on_vitality"); stat_db_map["HT_VT"] = tmp_d;
                tmp_d = obj->get_stat("weight_on_speed"); stat_db_map["WT_SP"] = tmp_d;
                tmp_d = obj->get_stat("weight_on_stamina"); stat_db_map["WT_ST"] = tmp_d;
                tmp_d = obj->get_stat("weight_on_vitality"); stat_db_map["WT_VT"] = tmp_d;
                tmp_d = obj->get_stat("vitality_on_appetite"); stat_db_map["VT_AP"] = tmp_d;
                tmp_d = obj->get_stat("vitality_on_speed"); stat_db_map["VT_SP"] = tmp_d;
                tmp_d = obj->get_stat("stamina_on_appetite"); stat_db_map["ST_AP"] = tmp_d;
                tmp_d = obj->get_stat("stamina_on_speed"); stat_db_map["ST_SP"] = tmp_d;
                tmp_d = obj->get_stat("theoretical_maximum_base_appetite"); stat_db_map["TMB_AP"] = tmp_d;
                tmp_d = obj->get_stat("theoretical_maximum_base_height"); stat_db_map["TMB_HT"] = tmp_d;
                tmp_d = obj->get_stat("theoretical_maximum_base_speed"); stat_db_map["TMB_SP"] = tmp_d;
                tmp_d = obj->get_stat("theoretical_maximum_base_stamina"); stat_db_map["TMB_ST"] = tmp_d;
                tmp_d = obj->get_stat("theoretical_maximum_base_vitality"); stat_db_map["TMB_VT"] = tmp_d;
                tmp_d = obj->get_stat("theoretical_maximum_base_weight"); stat_db_map["TMB_WT"] = tmp_d;
                tmp_d = obj->get_stat("theoretical_maximum_height"); stat_db_map["TM_HT"] = tmp_d;
                tmp_d = obj->get_stat("theoretical_maximum_speed"); stat_db_map["TM_SP"] = tmp_d;
                tmp_d = obj->get_stat("theoretical_maximum_weight"); stat_db_map["TM_WT"] = tmp_d;
                tmp_d = obj->get_stat("theoretical_maximum_height_multiplier"); stat_db_map["TMM_HT"] = tmp_d;
                tmp_d = obj->get_stat("theoretical_maximum_speed_multiplier"); stat_db_map["TMM_SP"] = tmp_d;
                tmp_d = obj->get_stat("theoretical_maximum_stamina_multiplier"); stat_db_map["TMM_ST"] = tmp_d;
                tmp_d = obj->get_stat("theoretical_maximum_vitality_multiplier"); stat_db_map["TMM_VT"] = tmp_d;
                tmp_d = obj->get_stat("theoretical_maximum_weight_multiplier"); stat_db_map["TMM_WT"] = tmp_d;
                tmp_d = obj->get_stat("sleep_restore_factor"); stat_db_map["SL_FACTOR"] = tmp_d;

                tmp_i = obj->get_stat("generation"); stat_db_map["AVG_GEN"] = (count / (count + 1)) * stat_db_map["AVG_GEN"] + ((double)tmp_i / (count + 1));
                tmp_d = obj->get_stat("immunity"); stat_db_map["AVG_IMM"] = (count / (count + 1)) * stat_db_map["AVG_IMM"] + (tmp_d / (count + 1));
                tmp_i = obj->get_stat("age"); stat_db_map["AVG_AGE"] = (count / (count + 1)) * stat_db_map["AVG_AGE"] + ((double)tmp_i / (count + 1));
                tmp_d = obj->get_stat("height"); stat_db_map["AVG_HT"] = (count / (count + 1)) * stat_db_map["AVG_HT"] + (tmp_d / (count + 1));
                tmp_d = obj->get_stat("weight"); stat_db_map["AVG_WT"] = (count / (count + 1)) * stat_db_map["AVG_WT"] + (tmp_d / (count + 1));
                tmp_d = obj->get_stat("max_appetite_at_age"); stat_db_map["AVGMA_AP"] = (count / (count + 1)) * stat_db_map["AVGMA_AP"] + (tmp_d / (count + 1));
                tmp_d = obj->get_stat("max_speed_at_age"); stat_db_map["AVGMA_SP"] = (count / (count + 1)) * stat_db_map["AVGMA_SP"] + (tmp_d / (count + 1));
                tmp_d = obj->get_stat("max_stamina_at_age"); stat_db_map["AVGMA_ST"] = (count / (count + 1)) * stat_db_map["AVGMA_ST"] + (tmp_d / (count + 1));
                tmp_d = obj->get_stat("max_vitality_at_age"); stat_db_map["AVGMA_VT"] = (count / (count + 1)) * stat_db_map["AVGMA_VT"] + (tmp_d / (count + 1));
                tmp_d = obj->get_stat("static_fitness"); stat_db_map["AVG_SFIT"] = (count / (count + 1)) * stat_db_map["AVG_SFIT"] + (tmp_d / (count + 1));
                tmp_d = obj->get_stat("death_factor"); stat_db_map["AVG_DTHF"] = (count / (count + 1)) * stat_db_map["AVG_DTHF"] + (tmp_d / (count + 1));
                tmp_d = obj->get_stat("vision_radius"); stat_db_map["AVG_VIS"] = (count / (count + 1)) * stat_db_map["AVG_VIS"] + (tmp_d / (count + 1));

                count++;
            }

            db_row.push_back((STAT)year); // year
            db_row.push_back((STAT)stat_db_map["MALE"]); // male
            db_row.push_back((STAT)stat_db_map["FEMALE"]); // female
            db_row.push_back((STAT)stat_db_map["M_MALE"]); // matable_male
            db_row.push_back((STAT)stat_db_map["M_FEMALE"]); // matable_female
            db_row.push_back((STAT)stat_db_map["C_PROB"]); // conceiving_probability
            db_row.push_back((STAT)stat_db_map["M_AGE_START"]); // mating_age_start
            db_row.push_back((STAT)stat_db_map["M_AGE_END"]); // mating_age_end
            db_row.push_back((STAT)stat_db_map["MX_AGE"]); // max_age
            db_row.push_back((STAT)stat_db_map["MT_PROB"]); // mutation_probability
            db_row.push_back((STAT)stat_db_map["OF_FACTOR"]); // offsprings_factor
            db_row.push_back((STAT)stat_db_map["AGE_DTH"]); // age_on_death
            db_row.push_back((STAT)stat_db_map["FIT_DTH"]); // fitness_on_death
            db_row.push_back((STAT)stat_db_map["AFR_DTH"]); // age_fitness_on_death_ratio
            db_row.push_back((STAT)stat_db_map["HT_SP"]); // height_on_speed
            db_row.push_back((STAT)stat_db_map["HT_ST"]); // height_on_stamina
            db_row.push_back((STAT)stat_db_map["HT_VT"]); // height_on_vitality
            db_row.push_back((STAT)stat_db_map["WT_SP"]); // weight_on_speed
            db_row.push_back((STAT)stat_db_map["WT_ST"]); // weight_on_stamina
            db_row.push_back((STAT)stat_db_map["WT_VT"]); // weight_on_vitality
            db_row.push_back((STAT)stat_db_map["VT_AP"]); // vitality_on_appetite
            db_row.push_back((STAT)stat_db_map["VT_SP"]); // vitality_on_speed
            db_row.push_back((STAT)stat_db_map["ST_AP"]); // stamina_on_appetite
            db_row.push_back((STAT)stat_db_map["ST_SP"]); // stamina_on_speed
            db_row.push_back((STAT)stat_db_map["TMB_AP"]); // theoretical_maximum_base_appetite
            db_row.push_back((STAT)stat_db_map["TMB_HT"]); // theoretical_maximum_base_height
            db_row.push_back((STAT)stat_db_map["TMB_SP"]); // theoretical_maximum_base_speed
            db_row.push_back((STAT)stat_db_map["TMB_ST"]); // theoretical_maximum_base_stamina
            db_row.push_back((STAT)stat_db_map["TMB_VT"]); // theoretical_maximum_base_vitality
            db_row.push_back((STAT)stat_db_map["TMB_WT"]); // theoretical_maximum_base_weight
            db_row.push_back((STAT)stat_db_map["TM_HT"]); // theoretical_maximum_height
            db_row.push_back((STAT)stat_db_map["TM_SP"]); // theoretical_maximum_speed
            db_row.push_back((STAT)stat_db_map["TM_WT"]); // theoretical_maximum_weight
            db_row.push_back((STAT)stat_db_map["TMM_HT"]); // theoretical_maximum_height_multiplier
            db_row.push_back((STAT)stat_db_map["TMM_SP"]); // theoretical_maximum_speed_multiplier
            db_row.push_back((STAT)stat_db_map["TMM_ST"]); // theoretical_maximum_stamina_multiplier
            db_row.push_back((STAT)stat_db_map["TMM_VT"]); // theoretical_maximum_vitality_multiplier
            db_row.push_back((STAT)stat_db_map["TMM_WT"]); // theoretical_maximum_weight_multiplier
            db_row.push_back((STAT)stat_db_map["SL_FACTOR"]); // sleep_restore_factor
            db_row.push_back((STAT)stat_db_map["AVG_GEN"]); // average_generation
            db_row.push_back((STAT)stat_db_map["AVG_IMM"]); // average_immunity
            db_row.push_back((STAT)stat_db_map["AVG_AGE"]); // average_age
            db_row.push_back((STAT)stat_db_map["AVG_HT"]); // average_height
            db_row.push_back((STAT)stat_db_map["AVG_WT"]); // average_weight
            db_row.push_back((STAT)stat_db_map["AVGMA_AP"]); // average_max_appetite_at_age
            db_row.push_back((STAT)stat_db_map["AVGMA_SP"]); // average_max_speed_at_age
            db_row.push_back((STAT)stat_db_map["AVGMA_ST"]); // average_max_stamina_at_age
            db_row.push_back((STAT)stat_db_map["AVGMA_VT"]); // average_max_vitality_at_age
            db_row.push_back((STAT)stat_db_map["AVG_SFIT"]); // average_static_fitness
            db_row.push_back((STAT)stat_db_map["AVG_DTHF"]); // average_death_factor
            db_row.push_back((STAT)stat_db_map["AVG_VIS"]); // average_vision_radius
        }
        else if (kingdom == "plant")
        {
            std::unordered_map<std::string, double> stat_db_map;
            unsigned int tmp_i;
            double tmp_d;

            unsigned int count = 0;

            stat_db_map["POP"] = 0.0;
            stat_db_map["M_POP"] = 0.0;
            stat_db_map["AVG_GEN"] = 0.0;
            stat_db_map["AVG_IMM"] = 0.0;
            stat_db_map["AVG_AGE"] = 0.0;
            stat_db_map["AVG_HT"] = 0.0;
            stat_db_map["AVG_WT"] = 0.0;
            stat_db_map["AVGMA_VT"] = 0.0;
            stat_db_map["AVG_SFIT"] = 0.0;
            stat_db_map["AVG_DTHF"] = 0.0;

            for (const auto &organism: organisms)
            {
                Plant *obj = static_cast<Plant *>(organism.second.get());

                if (kind != obj->kind)
                    continue;

                stat_db_map["POP"]++;

                if (obj->age >= obj->mating_age_start && obj->age <= obj->mating_age_end)
                {
                    stat_db_map["M_POP"]++;
                }

                tmp_d = obj->get_stat("conceiving_probability"); stat_db_map["C_PROB"] = tmp_d;
                tmp_i = obj->get_stat("mating_age_start"); stat_db_map["M_AGE_START"] = (double)tmp_i;
                tmp_i = obj->get_stat("mating_age_end"); stat_db_map["M_AGE_END"] = (double)tmp_i;
                tmp_i = obj->get_stat("max_age"); stat_db_map["MX_AGE"] = (double)tmp_i;
                tmp_d = obj->get_stat("mutation_probability"); stat_db_map["MT_PROB"] = tmp_d;
                tmp_d = obj->get_stat("offsprings_factor"); stat_db_map["OF_FACTOR"] = tmp_d;
                tmp_d = obj->get_stat("age_on_death"); stat_db_map["AGE_DTH"] = tmp_d;
                tmp_d = obj->get_stat("fitness_on_death"); stat_db_map["FIT_DTH"] = tmp_d;
                tmp_d = obj->get_stat("age_fitness_on_death_ratio"); stat_db_map["AFR_DTH"] = tmp_d;
                tmp_d = obj->get_stat("height_on_vitality"); stat_db_map["HT_VT"] = tmp_d;
                tmp_d = obj->get_stat("weight_on_vitality"); stat_db_map["WT_VT"] = tmp_d;
                tmp_d = obj->get_stat("theoretical_maximum_base_height"); stat_db_map["TMB_HT"] = tmp_d;
                tmp_d = obj->get_stat("theoretical_maximum_base_vitality"); stat_db_map["TMB_VT"] = tmp_d;
                tmp_d = obj->get_stat("theoretical_maximum_base_weight"); stat_db_map["TMB_WT"] = tmp_d;
                tmp_d = obj->get_stat("theoretical_maximum_height"); stat_db_map["TM_HT"] = tmp_d;
                tmp_d = obj->get_stat("theoretical_maximum_weight"); stat_db_map["TM_WT"] = tmp_d;
                tmp_d = obj->get_stat("theoretical_maximum_height_multiplier"); stat_db_map["TMM_HT"] = tmp_d;
                tmp_d = obj->get_stat("theoretical_maximum_vitality_multiplier"); stat_db_map["TMM_VT"] = tmp_d;
                tmp_d = obj->get_stat("theoretical_maximum_weight_multiplier"); stat_db_map["TMM_WT"] = tmp_d;

                tmp_i = obj->get_stat("generation"); stat_db_map["AVG_GEN"] = (count / (count + 1)) * stat_db_map["AVG_GEN"] + ((double)tmp_i / (count + 1));
                tmp_d = obj->get_stat("immunity"); stat_db_map["AVG_IMM"] = (count / (count + 1)) * stat_db_map["AVG_IMM"] + (tmp_d / (count + 1));
                tmp_i = obj->get_stat("age"); stat_db_map["AVG_AGE"] = (count / (count + 1)) * stat_db_map["AVG_AGE"] + ((double)tmp_i / (count + 1));
                tmp_d = obj->get_stat("height"); stat_db_map["AVG_HT"] = (count / (count + 1)) * stat_db_map["AVG_HT"] + (tmp_d / (count + 1));
                tmp_d = obj->get_stat("weight"); stat_db_map["AVG_WT"] = (count / (count + 1)) * stat_db_map["AVG_WT"] + (tmp_d / (count + 1));
                tmp_d = obj->get_stat("max_vitality_at_age"); stat_db_map["AVGMA_VT"] = (count / (count + 1)) * stat_db_map["AVGMA_VT"] + (tmp_d / (count + 1));
                tmp_d = obj->get_stat("static_fitness"); stat_db_map["AVG_SFIT"] = (count / (count + 1)) * stat_db_map["AVG_SFIT"] + (tmp_d / (count + 1));
                tmp_d = obj->get_stat("death_factor"); stat_db_map["AVG_DTHF"] = (count / (count + 1)) * stat_db_map["AVG_DTHF"] + (tmp_d / (count + 1));

                count++;
            }

            db_row.push_back((STAT)year); // year
            db_row.push_back((STAT)stat_db_map["POP"]); // population
            db_row.push_back((STAT)stat_db_map["M_POP"]); // matable_population
            db_row.push_back((STAT)stat_db_map["C_PROB"]); // conceiving_probability
            db_row.push_back((STAT)stat_db_map["M_AGE_START"]); // mating_age_start
            db_row.push_back((STAT)stat_db_map["M_AGE_END"]); // mating_age_end
            db_row.push_back((STAT)stat_db_map["MX_AGE"]); // max_age
            db_row.push_back((STAT)stat_db_map["MT_PROB"]); // mutation_probability
            db_row.push_back((STAT)stat_db_map["OF_FACTOR"]); // offsprings_factor
            db_row.push_back((STAT)stat_db_map["AGE_DTH"]); // age_on_death
            db_row.push_back((STAT)stat_db_map["FIT_DTH"]); // fitness_on_death
            db_row.push_back((STAT)stat_db_map["AFR_DTH"]); // age_fitness_on_death_ratio
            db_row.push_back((STAT)stat_db_map["HT_VT"]); // height_on_vitality
            db_row.push_back((STAT)stat_db_map["WT_VT"]); // weight_on_vitality
            db_row.push_back((STAT)stat_db_map["TMB_HT"]); // theoretical_maximum_base_height
            db_row.push_back((STAT)stat_db_map["TMB_VT"]); // theoretical_maximum_base_vitality
            db_row.push_back((STAT)stat_db_map["TMB_WT"]); // theoretical_maximum_base_weight
            db_row.push_back((STAT)stat_db_map["TM_HT"]); // theoretical_maximum_height
            db_row.push_back((STAT)stat_db_map["TM_WT"]); // theoretical_maximum_weight
            db_row.push_back((STAT)stat_db_map["TMM_HT"]); // theoretical_maximum_height_multiplier
            db_row.push_back((STAT)stat_db_map["TMM_VT"]); // theoretical_maximum_vitality_multiplier
            db_row.push_back((STAT)stat_db_map["TMM_WT"]); // theoretical_maximum_weight_multiplier
            db_row.push_back((STAT)stat_db_map["AVG_GEN"]); // average_generation
            db_row.push_back((STAT)stat_db_map["AVG_IMM"]); // average_immunity
            db_row.push_back((STAT)stat_db_map["AVG_AGE"]); // average_age
            db_row.push_back((STAT)stat_db_map["AVG_HT"]); // average_height
            db_row.push_back((STAT)stat_db_map["AVG_WT"]); // average_weight
            db_row.push_back((STAT)stat_db_map["AVGMA_VT"]); // average_max_vitality_at_age
            db_row.push_back((STAT)stat_db_map["AVG_SFIT"]); // average_static_fitness
            db_row.push_back((STAT)stat_db_map["AVG_DTHF"]); // average_death_factor
        }

        return db_row;
    }
};
