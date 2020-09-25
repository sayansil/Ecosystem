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
            if(kind != "" && kind != organism.second->get_kind())
                continue;

            if(organism.second->get_gender() == MALE)
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

        for (const auto &organism : organisms)
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

        for (const auto &organism : organisms)
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
        bool uninitialized = true;

        for (const auto &organism : organisms)
        {
            const auto& a_map = organism.second->get_attribute_raw_map();

            if (kind != "" && kind != organism.second->get_kind())
                continue;

            const auto& current_attribute = a_map[attribute];
            if(current_attribute.getIndex() == PStatType::INT)
            {
                //value = current_attribute.getInt();
                value = std::stoi(current_attribute.getString());
            }
            else if(current_attribute.getIndex() == PStatType::DOUBLE)
            {
                //value = current_attribute.getDouble();
                value = std::stod(current_attribute.getString());
            }
            else if(current_attribute.getIndex() == PStatType::UINT)
            {
                //value = current_attribute.getUnsignedInt();
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
            if (kindDistribution.find(organism.second->get_kind()) == kindDistribution.end())
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
            const auto& a_map = organism.second->get_attribute_raw_map();

            if (kind != "" && kind != organism.second->get_kind())
                continue;

            average = (n / (n + 1)) * average + ((std::stod(a_map[attribute].getString())) / (n + 1));
            n++;
        }

        return average;
    }

    std::vector<PStat> get_one_stat(const ENTITY_MAP_TYPE &organisms, const std::string &attribute, const std::string &kind)
    {
        std::vector<PStat> attributeList;

        for (const auto &organism : organisms)
        {
            const auto& a_map = organism.second->get_attribute_raw_map();

            if (kind != "" && kind != organism.second->get_kind())
                continue;

            attributeList.push_back(a_map[attribute]);
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

        std::vector<std::string> members{
            "name",
            "food_chain_rank",
            "vision_radius",
            "max_speed_at_age",
            "max_vitality_at_age",
            "max_appetite_at_age",
            "max_stamina_at_age",
            "height",
            "weight"
        };

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
            if (organism.second->get_monitor_in_simulation())
            {
                const auto &a_map = organism.second->get_attribute_raw_map();

                std::unordered_map<std::string, std::string> temp;
                for (const auto &i : members)
                {
                    if (a_map.map.find(i) != a_map.map.end())
                    {
                        temp.insert({i, a_map[i].getString()});
                    }
                }

                response[organism.second->get_kingdom()].push_back(temp);

            }
        }
        response["status"] = "success";
        return response.dump();
    }

    std::vector<std::map<std::string, std::string>> prepare_data_for_simulation_2(const ENTITY_MAP_TYPE &organisms)
    {
        std::vector<std::map<std::string, std::string>> representatives;

        std::vector<std::string> members{
            "name",
            "kingdom",
            "kind",
            "gender",
            "age",
            "height",
            "weight",
            "food_chain_rank",
            "vision_radius",
            "max_appetite_at_age",
            "max_speed_at_age",
            "max_stamina_at_age",
            "max_vitality_at_age"
        };

        for (const auto &organism : organisms)
        {
            const auto& a_map = organism.second->get_attribute_raw_map();

            if (organism.second->get_monitor_in_simulation())
            {
                std::map<std::string, std::string> temp;

                for (const auto &i : members)
                {
                    if (a_map.map.find(i) != a_map.map.end())
                    {
                        temp.insert({i, a_map[i].getString()});
                    }
                    else
                    {
                        temp.insert({i, std::to_string(0)});
                    }
                }

                representatives.push_back(temp);
            }
        }

        return representatives;
    }

    std::vector<DBType> get_db_row(const ENTITY_MAP_TYPE &organisms, const std::string &kind, const std::string &kingdom, const unsigned int &year, std::unordered_map<std::string, std::unordered_map<StatGroup, std::vector<std::string>>> &statistics)
    {
        std::vector<DBType> db_row;

        if (kingdom == "animal")
        {
            std::unordered_map<std::string, double> stat_db_map; // TODO : string -> DBType

            unsigned int count = 0;

            // Used in StatGroup::MISC

            for (const auto& var_name : statistics[kingdom][StatGroup::MISC])
            {
                stat_db_map[var_name] = 0.0;
            }

            // Used in StatGroup::MEAN

            for (const auto &var_name : statistics[kingdom][StatGroup::MEAN])
            {
                stat_db_map["average_" + var_name] = 0.0;
            }

            for (const auto &organism: organisms)
            {
                const auto& a_map = organism.second->get_attribute_raw_map();

                if (kind != organism.second->get_kind())
                    continue;

                if (organism.second->get_gender() == MALE)
                {
                    stat_db_map["male_population"]++;

                    if (organism.second->get_age() >= organism.second->get_mating_age_start() && organism.second->get_age() <= organism.second->get_mating_age_end())
                    {
                        stat_db_map["matable_male_population"]++;
                    }
                }
                else
                {
                    stat_db_map["female_population"]++;

                    if (organism.second->get_age() >= organism.second->get_mating_age_start() && organism.second->get_age() <= organism.second->get_mating_age_end())
                    {
                        stat_db_map["matable_female_population"]++;
                    }
                }

                // Used in StatGroup::FIX

                for (const auto &var_name : statistics[kingdom][StatGroup::FIX])
                {
                    stat_db_map[var_name] = std::stod(a_map[var_name].getString());
                }

                // Used in StatGroup::MEAN

                for (const auto &var_name : statistics[kingdom][StatGroup::MEAN])
                {
                    stat_db_map["average_" + var_name] = ((double)count / (count + 1)) * stat_db_map[var_name] + (std::stod(a_map[var_name].getString()) / (count + 1));
                }

                count++;
            }

            for (const auto &[colName, colType] : schema::schemaAnimal)
            {
                if (colName == "year")
                {
                    db_row.emplace_back(DBType(SQLType::INT, std::to_string(year)));
                }
                else
                {
                    db_row.emplace_back(DBType(SQLType::FLOAT, std::to_string(stat_db_map[colName])));
                }
            }

        }
        else if (kingdom == "plant")
        {
            std::unordered_map<std::string, double> stat_db_map;

            unsigned int count = 0;

            // Used in StatGroup::MISC

            for (const auto &var_name : statistics[kingdom][StatGroup::MISC])
            {
                stat_db_map[var_name] = 0.0;
            }

            // Used in StatGroup::MEAN

            for (const auto &var_name : statistics[kingdom][StatGroup::MEAN])
            {
                stat_db_map[var_name] = 0.0;
            }

            for (const auto &organism: organisms)
            {
                const auto& a_map = organism.second->get_attribute_raw_map();

                if (kind != organism.second->get_kind())
                    continue;

                stat_db_map["population"]++;

                if (organism.second->get_age() >= organism.second->get_mating_age_start() && organism.second->get_age() <= organism.second->get_mating_age_end())
                {
                    stat_db_map["matable_population"]++;
                }

                // Used in StatGroup::FIX

                for (const auto &var_name : statistics[kingdom][StatGroup::FIX])
                {
                    stat_db_map[var_name] = std::stod(a_map[var_name].getString());
                }

                // Used in StatGroup::MEAN

                for (const auto &var_name : statistics[kingdom][StatGroup::MEAN])
                {
                    stat_db_map[var_name] = (count / (count + 1)) * stat_db_map[var_name] + (std::stod(a_map[var_name].getString()) / (count + 1));
                }

                count++;
            }

            for (const auto &[colName, colType] : schema::schemaPlant)
            {
                if (colName == "year")
                {
                    db_row.emplace_back(DBType(SQLType::INT, std::to_string(year)));
                }
                else
                {
                    db_row.emplace_back(DBType(SQLType::FLOAT, std::to_string(stat_db_map[colName])));
                }
            }
        }

        return db_row;
    }
};
