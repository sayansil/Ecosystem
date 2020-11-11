#include <god.hpp>

God::God(const bool &gods_eye)
{

    constants::init();
    this->gods_eye = gods_eye;
    helper::rng.seed(std::random_device()());

    statistics["animal"][StatGroup::FIX] = {
        "conceiving_probability",
        "mating_probability",
        "mating_age_start",
        "mating_age_end",
        "max_age",
        "mutation_probability",
        "offsprings_factor",
        "age_fitness_on_death_ratio",
        "height_on_speed",
        "height_on_stamina",
        "height_on_vitality",
        "weight_on_speed",
        "weight_on_stamina",
        "weight_on_vitality",
        "vitality_on_appetite",
        "vitality_on_speed",
        "stamina_on_appetite",
        "stamina_on_speed",
        "theoretical_maximum_base_appetite",
        "theoretical_maximum_base_height",
        "theoretical_maximum_base_speed",
        "theoretical_maximum_base_stamina",
        "theoretical_maximum_base_vitality",
        "theoretical_maximum_base_weight",
        "theoretical_maximum_height",
        "theoretical_maximum_speed",
        "theoretical_maximum_weight",
        "theoretical_maximum_height_multiplier",
        "theoretical_maximum_speed_multiplier",
        "theoretical_maximum_stamina_multiplier",
        "theoretical_maximum_vitality_multiplier",
        "theoretical_maximum_weight_multiplier",
        "sleep_restore_factor"
    };
    statistics["animal"][StatGroup::MEAN] = {
        "generation",
        "immunity",
        "age",
        "height",
        "weight",
        "max_appetite_at_age",
        "max_speed_at_age",
        "max_stamina_at_age",
        "max_vitality_at_age",
        "static_fitness",
        "age_death_factor",
        "fitness_death_factor",
        "death_factor",
        "vision_radius"
    };
    statistics["animal"][StatGroup::MISC] = {
        "year",
        "male_population",
        "female_population",
        "matable_male_population",
        "matable_female_population"
    };

    statistics["plant"][StatGroup::FIX] = {
        "conceiving_probability",
        "mating_probability",
        "mating_age_start",
        "mating_age_end",
        "max_age",
        "mutation_probability",
        "offsprings_factor",
        "age_fitness_on_death_ratio",
        "height_on_vitality",
        "weight_on_vitality",
        "theoretical_maximum_base_height",
        "theoretical_maximum_base_vitality",
        "theoretical_maximum_base_weight",
        "theoretical_maximum_height",
        "theoretical_maximum_weight",
        "theoretical_maximum_height_multiplier",
        "theoretical_maximum_vitality_multiplier",
        "theoretical_maximum_weight_multiplier"
    };
    statistics["plant"][StatGroup::MEAN] = {
        "generation",
        "immunity",
        "age",
        "height",
        "weight",
        "max_vitality_at_age",
        "static_fitness",
        "age_death_factor",
        "fitness_death_factor",
        "death_factor"
    };
    statistics["plant"][StatGroup::MISC] = {
        "year",
        "population",
        "matable_population"
    };

    catastrophe();
}

God::~God()
{
}

void God::catastrophe()
{
    if(gods_eye)
    {
        db.clear_database();
    }
    organisms.clear();
}

void God::reset_species(const std::string &full_species_name)
{
    std::string species_name = full_species_name.substr(full_species_name.find('/') + 1);
    std::string kingdom_name = full_species_name.substr(0, full_species_name.find('/'));
    std::ifstream in( helper::get_ecosystem_root() / "data" / "json" / kingdom_name / species_name / "base.json");
    nlohmann::json tmp;
    in >> tmp;
    in.close();
    constants::species_constants_map[species_name] = tmp;

    db.clear_table(species_name);
}

bool God::spawn_organism(ENTITY &&current_organism)
{
    const std::string kingdom = current_organism->get_kingdom();
    if(kingdom == "animal")
    {
        Animal *obj = static_cast<Animal*>(current_organism.get());
        init(*obj);
    }
    else if(kingdom == "plant")
    {
        Plant *obj = static_cast<Plant*>(current_organism.get());
        init(*obj);
    }
    else
    {
        throw std::runtime_error(__func__ + std::string(": kingdom ") + kingdom + " is not supported\n");
    }

    current_organism->generate_death_factor();

    if (current_organism->is_normal_child())
    {
        // Add to memory
        organisms[current_organism->get_name()] = current_organism;

        if(gods_eye)
        {
            std::vector<DBType> tmp;

            const auto &a_map = current_organism->get_attribute_raw_map();

            for (const auto &[colName, colType] : schema::schemaMaster)
            {
                tmp.emplace_back(DBType(colType, a_map[colName].getString()));
            }

            // Add to database
            db.insert_rows(std::vector<std::vector<DBType>>{tmp});
        }
        return true;
    }

    return false;
}

void God::kill_organisms(const std::vector<std::string> &names)
{
    // Remove from database
    if(gods_eye)
    {
        db.delete_rows(names);
    }

    // Remove from memory
    for (const auto &lamb_to_slaughter : names)
        organisms.erase(lamb_to_slaughter);
}

bool God::mate(const std::string &name1, const std::string &name2, std::vector<std::pair<std::string, ENTITY>>& organisms_buffer, const nlohmann::json &species_constants)
{
    // ORGANISM objects of 2 parents
    const auto &parent1 = organisms[name1];
    const auto &parent2 = organisms[name2];

    // Generate chromosomes of the child
    auto child_chromosome = helper::get_random_mixture(parent1->get_chromosome(), parent2->get_chromosome());

    // Mutate chromosomes
    for(auto &bit : child_chromosome)
        if(helper::weighted_prob(parent1->get_mutation_probability()))
            bit = (bit == '1')?'0':'1';

    // Spawn child (if probable)
    if(helper::weighted_prob(std::min(parent1->get_conceiving_probability(), parent2->get_conceiving_probability())))
    {
        bool monitor_in_simulation = false;

        if (monitor_offsprings && (parent1->get_monitor_in_simulation() || parent2->get_monitor_in_simulation()))
            monitor_in_simulation = true;

        return spawn_organism(parent1->clone(
            parent1->get_kind(),
            1,
            monitor_in_simulation,
            child_chromosome,
            std::max(parent1->get_generation(), parent2->get_generation()) + 1,
            "",
            {(parent1->get_X() + parent2->get_X()) / 2,
                (parent1->get_Y() + parent2->get_Y()) / 2},
            species_constants));
    }
    return false;
}

double updateStat(double base, double p_range)
{
    std::uniform_real_distribution<double> dis(0.0, p_range * 2);
    const double x = p_range - dis(helper::rng);

    return base * (1 + x);
}

void God::update_species(const std::string &full_species_name)
{
    std::string kind = full_species_name.substr(full_species_name.find('/') + 1);
    std::string kingdom = full_species_name.substr(0, full_species_name.find('/'));

    const std::filesystem::path modify_filepath = helper::get_ecosystem_root() / "data" / "json" / kingdom / kind / "modify.json";

    std::ifstream modify_in(modify_filepath);

    nlohmann::json modify;
    modify_in >> modify;

    for (const auto [key, value]: modify.items())
    {
        constants::species_constants_map[kind][key] = updateStat((double)constants::species_constants_map[kind][key], (double)value);
    }

    modify_in.close();
}

double God::killer_function(const double &index, const double &size) const
{
    // return std::exp(-x / (s / 10.0))
    // return pow(x / s, 1 / 1.75)
    // return 1 - (1 / (1 + exp(-(10 * index - size) / pow(size, 0.5))));
    const double &ratio = 1.0 / 10.0;
    return 1 - (1 / (1 + exp((ratio * size - index) / (ratio * pow(size, 0.5)))));
}

int God::creator_function(const double &o_factor) const
{
    std::gamma_distribution<double> dis(1.5, o_factor);
    return std::round(dis(helper::rng));
}

void God::happy_new_year(const bool &log)
{
    recent_births = 0;
    recent_deaths = 0;

    if (!disable_deaths)
    {
        /************************************
         *       Annual Killing Begins      *
         ************************************/

        // Vector for [ (ORGANISM, death_factor) ]
        std::vector<std::pair<ENTITY, double>>
            organisms_vec;

        for(auto &organism : organisms)
        {
            organism.second->generate_death_factor();
            organisms_vec.push_back({organism.second, 0.0});
        }

        // Sort organism_vec by death factor

        std::sort(std::execution::par, organisms_vec.begin(),
            organisms_vec.end(),
            [](const std::pair<ENTITY, double> &x, const std::pair<ENTITY, double> &y){
                return x.first->get_death_factor() > y.first->get_death_factor();
        });

        // Mark the organisms in organism_vec for death

        std::vector<size_t> indices(organisms_vec.size()); std::iota(indices.begin(), indices.end(), 0);
        std::vector<double> death_factors(indices.size());
        std::transform(std::execution::par, indices.begin(), indices.end(), death_factors.begin(), [this, &organisms_vec](const size_t& index){
            static thread_local std::uniform_real_distribution<double> par_dis(0.0, 1.0);
            static thread_local std::mt19937_64 par_rng{std::random_device()()};
            const double x = par_dis(par_rng);
            return (x <= killer_function(index, organisms_vec.size()) ? 1 : 0);
        });

        std::for_each(std::execution::par, indices.begin(), indices.end(), [&death_factors, &organisms_vec](const size_t& index){
            organisms_vec[index].second = death_factors[index];
        });

        indices.clear(); indices.shrink_to_fit();
        death_factors.clear(); death_factors.shrink_to_fit();

        // Remove the above marked organisms from the f****** universe

        std::vector<std::string> organisms_to_be_slaughtered;

        for(const auto &organism_tuple : organisms_vec)
        {
            if(organism_tuple.second == 1)
            {
                // Being dragged to the slaughterhouse
                organisms_to_be_slaughtered.push_back(organism_tuple.first->get_name());
            }
        }

        recent_deaths = organisms_to_be_slaughtered.size();
        kill_organisms(organisms_to_be_slaughtered);

        organisms_vec.clear(); organisms_vec.shrink_to_fit();
        organisms_to_be_slaughtered.clear(); organisms_to_be_slaughtered.shrink_to_fit();

    }

    /************************************
     *       Annual Ageing Begins      *
     ************************************/

    std::for_each(std::execution::par, organisms.begin(), organisms.end(), [](auto &x){
        x.second->increment_age();
    });

    year++;


    /***********************************
     *       Annual Mating Begins      *
     ***********************************/

    std::unordered_map<std::string, std::vector<ENTITY>> organismsByKind;
    for(const auto &organism : organisms)
        organismsByKind[organism.second->get_full_species_name()].push_back(organism.second);

    int index_parent;
    for(auto &organism_tuple : organismsByKind)
    {
        // Mating organisms of species organism_tuple.first

        auto &organism_list = organism_tuple.second;
        if (organism_list.size() == 0)
        {
            continue;
        }

        update_species(organism_tuple.first);

        std::string species_name = organism_tuple.first.substr(organism_tuple.first.find('/') + 1);
        const auto& species_constants = constants::species_constants_map[species_name];

        std::vector<ENTITY> mating_list1, mating_list2;

        bool current_kind_asexual = organism_list[0]->get_is_asexual();

        for(const auto &organism : organism_list)
        {
            if(current_kind_asexual)
            {
                if(helper::weighted_prob(0.5) == 0)
                {
                    mating_list1.push_back(organism);
                }
                else
                {
                    mating_list2.push_back(organism);
                }
            }
            else
            {
                if(organism->get_gender() == MALE)
                {
                    if(organism->get_age() >= organism->get_mating_age_start() && organism->get_age() <= organism->get_mating_age_end())
                    {
                        mating_list1.push_back(organism);
                    }
                }
                else
                {
                    if(organism->get_age() >= organism->get_mating_age_start() && organism->get_age() <= organism->get_mating_age_end())
                    {
                        mating_list2.push_back(organism);
                    }
                }

            }
        }

        static thread_local std::mt19937_64 rng{std::random_device()()};
        std::shuffle(mating_list1.begin(), mating_list1.end(), rng);
        std::shuffle(mating_list2.begin(), mating_list2.end(), rng);
        index_parent = 0;

        while (mating_list1.size() > index_parent && mating_list2.size() > index_parent)
        {
            const auto &parent1 = mating_list1[index_parent];
            const auto &parent2 = mating_list2[index_parent];

            if(helper::weighted_prob(std::min(parent1->get_mating_probability(), parent2->get_mating_probability())))
            {
                int n_children = creator_function(parent1->get_offsprings_factor());
                std::vector<std::pair<std::string, ENTITY>> organisms_buffer;
                organisms_buffer.reserve(n_children);
                while(n_children--)
                {
                    if (mate(parent1->get_name(), parent2->get_name(), organisms_buffer, species_constants))
                    {
                        recent_births++;
                    }
                }
            }
            index_parent++;
        }
    }

    /*********************
     *       Logging     *
     *********************/

    if (log)
    {
        std::cout << "Year: " << year << " - ";
        std::cout << "Recent births: " << recent_births << ' ';
        std::cout << "Recent deaths: " << recent_deaths << ' ';
        std::cout << "Population: " << organisms.size() << '\n';
    }

}

std::vector<ENTITY> God::organism_sort(bool (*comp)(const ENTITY&, const ENTITY&))
{
    std::vector<ENTITY> organism_vec;
    for (auto &i : organisms)
    {
        organism_vec.push_back(i.second);
    }
    organism_vec.shrink_to_fit();
    std::sort(organism_vec.begin(), organism_vec.end(), comp);
    return organism_vec;
}

std::unordered_map<std::string, std::vector<ENTITY>> God::organism_sort_by_kind(bool (*comp)(const ENTITY&, const ENTITY&))
{
    std::unordered_map<std::string, std::vector<ENTITY>> organism_map;
    for (const auto &i : organisms)
    {
        organism_map[i.second->get_kind()].push_back(i.second);
    }
    for (auto &i : organism_map)
    {
        std::sort(i.second.begin(), i.second.end(), comp);
    }
    return organism_map;
}

void God::remember_species(const std::string &full_species_name)
{
    std::string kind = full_species_name.substr(full_species_name.find('/') + 1);
    std::string kingdom = full_species_name.substr(0, full_species_name.find('/'));

    std::vector<DBType> db_row = stat_fetcher::get_db_row(organisms, kind, kingdom, year, statistics);
    db.insert_stat_row(db_row, kind);
}

std::string God::get_annual_data(const std::string &full_species_name)
{
    auto data = db.read_all_rows_stats(full_species_name);

    std::string final_data = "";

    for(const auto& row: data)
    {
        for(const auto& item: row)
        {
            final_data += item.data + ",";
        }
        if (final_data.length() > 0)
            final_data = final_data.substr(0, final_data.length() - 1);
        final_data += '\n';
    }

    return final_data;
}

std::vector<std::map<std::string, std::string>> God::get_live_data()
{
    return stat_fetcher::prepare_data_for_simulation_2(organisms);
}

std::unordered_map<std::string, std::vector<double>> God::test_organism(ENTITY &&current_organism, const std::vector<std::string>& vars, const int &years)
{
    std::unordered_map<std::string, std::vector<double>> res;
    for (const std::string &var : vars)
    {
        res[var] = {};
    }

    std::string name = helper::random_name(10);
    spawn_organism(current_organism->clone(
        current_organism->get_kind(),
        1,
        false,
        current_organism->get_chromosome(),
        current_organism->get_generation(),
        name));

    if (organisms.find(name) == organisms.end())
    {
        std::cout << "Could not create organism. Population 0.\n";
        return res;
    }

    const auto &a_map = organisms[name]->get_attribute_raw_map();

    int i = 0;
    while (organisms.find(name) != organisms.end() && i++ < years)
    {

        for (const std::string &var : vars)
        {
            res[var].push_back(std::stod(a_map[var].getString()));
        }

        happy_new_year();
    }

    return res;
}