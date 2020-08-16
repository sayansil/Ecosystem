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
        map_maker maker;

        for (auto organism : organisms)
        {
            auto a_map = maker.raw_var_map_banana(organism.second);

            if (kind != "" && kind != a_map["kind"].getString())
                continue;

            if (a_map["gender"].getUnsignedInt() == MALE)
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
        map_maker maker;

        for (const auto &organism : organisms)
        {
            auto a_map = maker.raw_var_map_banana(organism.second);

            if (kind != "" && kind != a_map["kind"].getString())
                continue;

            count++;
        }

        return count;
    }

    std::pair<unsigned int, unsigned int> get_matable_population(const ENTITY_MAP_TYPE &organisms, const std::string &kind)
    {
        unsigned int M = 0, F = 0;
        map_maker maker;

        for (const auto &organism : organisms)
        {
            auto a_map = maker.raw_var_map_banana(organism.second);

            if (kind != "" && kind != a_map["kind"].getString())
                continue;

            if (a_map["age"].getUnsignedInt() >= a_map["mating_age_start"].getUnsignedInt() && a_map["age"].getUnsignedInt() <= a_map["mating_age_end"].getUnsignedInt())
            {
                if (a_map["gender"].getUnsignedInt() == MALE)
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
        map_maker maker;
        PStat current_attribute;
        bool uninitialized = true;

        for (const auto &organism : organisms)
        {
            auto a_map = maker.raw_var_map_banana(organism.second);

            if (kind != "" && kind != a_map["kind"].getString())
                continue;

            current_attribute = a_map[attribute];
            if(current_attribute.getIndex() == 0)
            {
                value = current_attribute.getInt();
            }
            else if(current_attribute.getIndex() == 1)
            {
                value = current_attribute.getDouble();
            }
            else if(current_attribute.getIndex() == 6)
            {
                value = current_attribute.getUnsignedInt();
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
        map_maker maker;

        for (const auto &organism: organisms)
        {
            auto a_map = maker.raw_var_map_banana(organism.second);
            if (kindDistribution.find(a_map["kind"].getString()) == kindDistribution.end())
            {
                kindDistribution[a_map["kind"].getString()] = 1;
            }
            else
            {
                kindDistribution[a_map["kind"].getString()]++;
            }
        }

        return kindDistribution;
    }

    double get_stat_average(const ENTITY_MAP_TYPE &organisms, const std::string &attribute, const std::string &kind)
    {
        double average = 0.0, n = 0.0, value = 0.0;
        map_maker maker;

        // avg(n+1) = (n / (n + 1)) * avg(n) + (1 / (n + 1)) * kn

        for (const auto &organism: organisms)
        {
            auto a_map = maker.raw_var_map_banana(organism.second);

            if (kind != "" && kind != a_map["kind"].getString())
                continue;

            average = (n / (n + 1)) * average + (a_map[attribute].getDouble() / (n + 1));
            n++;
        }

        return average;
    }

    std::vector<PStat> get_one_stat(const ENTITY_MAP_TYPE &organisms, const std::string &attribute, const std::string &kind)
    {
        map_maker maker;
        std::vector<PStat> attributeList;

        for (const auto &organism : organisms)
        {
            auto a_map = maker.raw_var_map_banana(organism.second);

            if (kind != "" && kind != a_map["kind"].getString())
                continue;

            attributeList.push_back(a_map[attribute]);
        }

        return attributeList;
    }

    std::string prepare_data_for_simulation(const ENTITY_MAP_TYPE &organisms)
    {
        map_maker maker;

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
            auto a_map = maker.raw_var_map_banana(organism.second);
            if (a_map["monitor_in_simulation"].getBool())
            {
                if(a_map["kingdom"].getString() == "animal")
                {
                    response["animal"].push_back({
                        {"name", a_map["name"].getString()},
                        {"food_chain_rank", a_map["food_chain_rank"].getUnsignedInt()},
                        {"vision_radius", a_map["vision_radius"].getDouble()},
                        {"max_speed_at_age", a_map["max_speed_at_age"].getDouble()},
                        {"max_vitality_at_age", a_map["max_vitality_at_age"].getDouble()},
                        {"max_appetite_at_age", a_map["max_appetite_at_age"].getDouble()},
                        {"max_stamina_at_age", a_map["max_stamina_at_age"].getDouble()},
                        {"height", a_map["height"].getDouble()},
                        {"weight", a_map["weight"].getDouble()}
                    });
                }
                else if (a_map["kingdom"].getString() == "plant")
                {
                    response["plant"].push_back({
                        {"name", a_map["name"].getString()},
                        {"food_chain_rank", a_map["food_chain_rank"].getUnsignedInt()},
                        {"max_vitality_at_age", a_map["max_vitality_at_age"].getDouble()},
                        {"height", a_map["height"].getDouble()},
                        {"weight", a_map["weight"].getDouble()}
                    });
                }
                else
                {
                    std::cout << "Kingdom " << a_map["kingdom"].getString() << " not supported\n";
                }
            }
        }
        response["status"] = "success";
        return response.dump();
    }

    std::vector<std::map<std::string, std::string>> prepare_data_for_simulation_2(const ENTITY_MAP_TYPE &organisms)
    {
        std::vector<std::map<std::string, std::string>> representatives;
        map_maker maker;

        for (const auto &organism : organisms)
        {
            auto a_map = maker.raw_var_map_banana(organism.second);
            if (a_map["monitor_in_simulation"].getBool())
            {
                if (a_map["kingdom"].getString() == "animal")
                {
                    std::map<std::string, std::string> temp;

                    temp.insert({"name", a_map["name"].getString()});
                    temp.insert({"kingdom", a_map["kingdom"].getString()});
                    temp.insert({"species", a_map["species"].getString()});

                    temp.insert({"gender", a_map["gender"].getString()});
                    temp.insert({"age", a_map["age"].getString()});
                    temp.insert({"height", a_map["height"].getString()});
                    temp.insert({"weight", a_map["weight"].getString()});

                    temp.insert({"food_chain_rank", a_map["food_chain_rank"].getString()});
                    temp.insert({"vision_radius", a_map["vision_radius"].getString()});

                    temp.insert({"max_appetite_at_age", a_map["max_appetite_at_age"].getString()});
                    temp.insert({"max_speed_at_age", a_map["max_speed_at_age"].getString()});
                    temp.insert({"max_stamina_at_age", a_map["max_stamina_at_age"].getString()});
                    temp.insert({"max_vitality_at_age", a_map["max_vitality_at_age"].getString()});

                    representatives.push_back(temp);
                }
                else if (a_map["kingdom"].getString() == "plant")
                {
                    std::map<std::string, std::string> temp;

                    temp.insert({"name", a_map["name"].getString()});
                    temp.insert({"kingdom", a_map["kingdom"].getString()});
                    temp.insert({"species", a_map["species"].getString()});

                    temp.insert({"gender", a_map["gender"].getString()});
                    temp.insert({"age", a_map["age"].getString()});
                    temp.insert({"height", a_map["height"].getString()});
                    temp.insert({"weight", a_map["weight"].getString()});

                    temp.insert({"food_chain_rank", a_map["food_chain_rank"].getString()});
                    temp.insert({"vision_radius", std::to_string(0)});

                    temp.insert({"max_appetite_at_age", std::to_string(0)});
                    temp.insert({"max_speed_at_age", std::to_string(0)});
                    temp.insert({"max_stamina_at_age", std::to_string(0)});
                    temp.insert({"max_vitality_at_age", a_map["max_vitality_at_age"].getString()});

                    representatives.push_back(temp);
                }
                else
                {
                    std::cout << "Kingdom " << a_map["kingdom"].getString() << " not supported\n";
                }
            }
        }

        return representatives;
    }

    std::vector<STAT> get_db_row(const ENTITY_MAP_TYPE &organisms, const std::string &kind, const std::string &kingdom, const unsigned int &year)
    {
        std::vector<STAT> db_row;
        map_maker maker;

        if (kingdom == "animal")
        {
            std::unordered_map<std::string, double> stat_db_map;

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
                auto a_map = maker.raw_var_map_banana(organism.second);

                if (kind != a_map["kind"].getString())
                    continue;

                if (a_map["gender"].getUnsignedInt() == MALE)
                {
                    stat_db_map["MALE"]++;

                    if (a_map["age"].getUnsignedInt() >= a_map["mating_age_start"].getUnsignedInt() && a_map["age"].getUnsignedInt() <= a_map["mating_age_end"].getUnsignedInt())
                    {
                        stat_db_map["M_MALE"]++;
                    }
                }
                else
                {
                    stat_db_map["FEMALE"]++;

                    if (a_map["age"].getUnsignedInt() >= a_map["mating_age_start"].getUnsignedInt() && a_map["age"].getUnsignedInt() <= a_map["mating_age_end"].getUnsignedInt())
                    {
                        stat_db_map["M_FEMALE"]++;
                    }
                }

                stat_db_map["M_AGE_START"] = a_map["mating_age_start"].getDouble();
                stat_db_map["M_AGE_END"] = a_map["mating_age_end"].getDouble();
                stat_db_map["MX_AGE"] = a_map["max_age"].getDouble();
                stat_db_map["C_PROB"] = a_map["conceiving_probability"].getDouble();
                stat_db_map["MT_PROB"] = a_map["mutation_probability"].getDouble();
                stat_db_map["OF_FACTOR"] = a_map["offsprings_factor"].getDouble();
                stat_db_map["AGE_DTH"] = a_map["age_on_death"].getDouble();
                stat_db_map["FIT_DTH"] = a_map["fitness_on_death"].getDouble();
                stat_db_map["AFR_DTH"] = a_map["age_fitness_on_death_ratio"].getDouble();
                stat_db_map["HT_SP"] = a_map["height_on_speed"].getDouble();
                stat_db_map["HT_ST"] = a_map["height_on_stamina"].getDouble();
                stat_db_map["HT_VT"] = a_map["height_on_vitality"].getDouble();
                stat_db_map["WT_SP"] = a_map["weight_on_speed"].getDouble();
                stat_db_map["WT_ST"] = a_map["weight_on_stamina"].getDouble();
                stat_db_map["WT_VT"] = a_map["weight_on_vitality"].getDouble();
                stat_db_map["VT_AP"] = a_map["vitality_on_appetite"].getDouble();
                stat_db_map["VT_SP"] = a_map["vitality_on_speed"].getDouble();
                stat_db_map["ST_AP"] = a_map["stamina_on_appetite"].getDouble();
                stat_db_map["ST_SP"] = a_map["stamina_on_speed"].getDouble();
                stat_db_map["TMB_AP"] = a_map["theoretical_maximum_base_appetite"].getDouble();
                stat_db_map["TMB_HT"] = a_map["theoretical_maximum_base_height"].getDouble();
                stat_db_map["TMB_SP"] = a_map["theoretical_maximum_base_speed"].getDouble();
                stat_db_map["TMB_ST"] = a_map["theoretical_maximum_base_stamina"].getDouble();
                stat_db_map["TMB_VT"] = a_map["theoretical_maximum_base_vitality"].getDouble();
                stat_db_map["TMB_WT"] = a_map["theoretical_maximum_base_weight"].getDouble();
                stat_db_map["TM_HT"] = a_map["theoretical_maximum_height"].getDouble();
                stat_db_map["TM_SP"] = a_map["theoretical_maximum_speed"].getDouble();
                stat_db_map["TM_WT"] = a_map["theoretical_maximum_weight"].getDouble();
                stat_db_map["TMM_HT"] = a_map["theoretical_maximum_height_multiplier"].getDouble();
                stat_db_map["TMM_SP"] = a_map["theoretical_maximum_speed_multiplier"].getDouble();
                stat_db_map["TMM_ST"] = a_map["theoretical_maximum_stamina_multiplier"].getDouble();
                stat_db_map["TMM_VT"] = a_map["theoretical_maximum_vitality_multiplier"].getDouble();
                stat_db_map["TMM_WT"] = a_map["theoretical_maximum_weight_multiplier"].getDouble();
                stat_db_map["SL_FACTOR"] = a_map["sleep_restore_factor"].getDouble();

                stat_db_map["AVG_GEN"] = (count / (count + 1)) * stat_db_map["AVG_GEN"] + (a_map["generation"].getDouble() / (count + 1));
                stat_db_map["AVG_IMM"] = (count / (count + 1)) * stat_db_map["AVG_IMM"] + (a_map["immunity"].getDouble() / (count + 1));
                stat_db_map["AVG_AGE"] = (count / (count + 1)) * stat_db_map["AVG_AGE"] + (a_map["age"].getDouble() / (count + 1));
                stat_db_map["AVG_HT"] = (count / (count + 1)) * stat_db_map["AVG_HT"] + (a_map["height"].getDouble() / (count + 1));
                stat_db_map["AVG_WT"] = (count / (count + 1)) * stat_db_map["AVG_WT"] + (a_map["weight"].getDouble() / (count + 1));
                stat_db_map["AVGMA_AP"] = (count / (count + 1)) * stat_db_map["AVGMA_AP"] + (a_map["max_appetite_at_age"].getDouble() / (count + 1));
                stat_db_map["AVGMA_SP"] = (count / (count + 1)) * stat_db_map["AVGMA_SP"] + (a_map["max_speed_at_age"].getDouble() / (count + 1));
                stat_db_map["AVGMA_ST"] = (count / (count + 1)) * stat_db_map["AVGMA_ST"] + (a_map["max_stamina_at_age"].getDouble() / (count + 1));
                stat_db_map["AVGMA_VT"] = (count / (count + 1)) * stat_db_map["AVGMA_VT"] + (a_map["max_vitality_at_age"].getDouble() / (count + 1));
                stat_db_map["AVG_SFIT"] = (count / (count + 1)) * stat_db_map["AVG_SFIT"] + (a_map["static_fitness"].getDouble() / (count + 1));
                stat_db_map["AVG_DTHF"] = (count / (count + 1)) * stat_db_map["AVG_DTHF"] + (a_map["death_factor"].getDouble() / (count + 1));
                stat_db_map["AVG_VIS"] = (count / (count + 1)) * stat_db_map["AVG_VIS"] + (a_map["vision_radius"].getDouble() / (count + 1));

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
                auto a_map = maker.raw_var_map_banana(organism.second);

                if (kind != a_map["kind"].getString())
                    continue;

                stat_db_map["POP"]++;

                if (a_map["age"].getUnsignedInt() >= a_map["mating_age_start"].getUnsignedInt() && a_map["age"].getUnsignedInt() <= a_map["mating_age_end"].getUnsignedInt())
                {
                    stat_db_map["M_POP"]++;
                }

                stat_db_map["C_PROB"] = a_map["conceiving_probability"].getDouble();
                stat_db_map["M_AGE_START"] = a_map["mating_age_start"].getDouble();
                stat_db_map["M_AGE_END"] = a_map["mating_age_end"].getDouble();
                stat_db_map["MX_AGE"] = a_map["max_age"].getDouble();
                stat_db_map["MT_PROB"] = a_map["mutation_probability"].getDouble();
                stat_db_map["OF_FACTOR"] = a_map["offsprings_factor"].getDouble();
                stat_db_map["AGE_DTH"] = a_map["age_on_death"].getDouble();
                stat_db_map["FIT_DTH"] = a_map["fitness_on_death"].getDouble();
                stat_db_map["AFR_DTH"] = a_map["age_fitness_on_death_ratio"].getDouble();
                stat_db_map["HT_VT"] = a_map["height_on_vitality"].getDouble();
                stat_db_map["WT_VT"] = a_map["weight_on_vitality"].getDouble();
                stat_db_map["TMB_HT"] = a_map["theoretical_maximum_base_height"].getDouble();
                stat_db_map["TMB_VT"] = a_map["theoretical_maximum_base_vitality"].getDouble();
                stat_db_map["TMB_WT"] = a_map["theoretical_maximum_base_weight"].getDouble();
                stat_db_map["TM_HT"] = a_map["theoretical_maximum_height"].getDouble();
                stat_db_map["TM_WT"] = a_map["theoretical_maximum_weight"].getDouble();
                stat_db_map["TMM_HT"] = a_map["theoretical_maximum_height_multiplier"].getDouble();
                stat_db_map["TMM_VT"] = a_map["theoretical_maximum_vitality_multiplier"].getDouble();
                stat_db_map["TMM_WT"] = a_map["theoretical_maximum_weight_multiplier"].getDouble();

                stat_db_map["AVG_GEN"] = (count / (count + 1)) * stat_db_map["AVG_GEN"] + (a_map["generation"].getDouble() / (count + 1));
                stat_db_map["AVG_IMM"] = (count / (count + 1)) * stat_db_map["AVG_IMM"] + (a_map["immunity"].getDouble() / (count + 1));
                stat_db_map["AVG_AGE"] = (count / (count + 1)) * stat_db_map["AVG_AGE"] + (a_map["age"].getDouble() / (count + 1));
                stat_db_map["AVG_HT"] = (count / (count + 1)) * stat_db_map["AVG_HT"] + (a_map["height"].getDouble() / (count + 1));
                stat_db_map["AVG_WT"] = (count / (count + 1)) * stat_db_map["AVG_WT"] + (a_map["weight"].getDouble() / (count + 1));
                stat_db_map["AVGMA_VT"] = (count / (count + 1)) * stat_db_map["AVGMA_VT"] + (a_map["max_vitality_at_age"].getDouble() / (count + 1));
                stat_db_map["AVG_SFIT"] = (count / (count + 1)) * stat_db_map["AVG_SFIT"] + (a_map["static_fitness"].getDouble() / (count + 1));
                stat_db_map["AVG_DTHF"] = (count / (count + 1)) * stat_db_map["AVG_DTHF"] + (a_map["death_factor"].getDouble() / (count + 1));

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
