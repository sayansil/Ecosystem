#include <god.hpp>

God::God(const bool &gods_eye)
{
    this->gods_eye = gods_eye;
    context = zmq::context_t(1);
    socket = zmq::socket_t(context, zmq::socket_type::dealer);
    socket.bind("tcp://*:5556");
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
    const std::string base_filepath = "../../data/json/" + full_species_name + "/base.json";
    const std::string current_filepath = "../../data/json/" + full_species_name + "/current.json";

    std::ifstream base_in(base_filepath);
    std::ofstream current_out(current_filepath);

    nlohmann::json base;
    base_in >> base;

    current_out << base;

    base_in.close();
    current_out.close();
    
    std::string kind = full_species_name.substr(full_species_name.find('/') + 1);
    db.clear_table(kind);
}

bool God::spawn_organism(const ENTITY &current_organism)
{
    if (current_organism->is_normal_child())
    {
        // Add to memory
        organisms[current_organism->get_name()] = current_organism;
        
        if(gods_eye)
        {
            std::vector<std::vector<STAT>> tmp;
            tmp.emplace_back(std::vector<STAT>{
                STAT(current_organism->get_name()),
                STAT(current_organism->get_kind()),
                STAT(current_organism->get_chromosome()),
                STAT(current_organism->get_generation()),
                STAT(current_organism->get_immunity()),
                STAT(current_organism->get_gender()),
                STAT(current_organism->get_age()),
                STAT(current_organism->get_height()),
                STAT(current_organism->get_weight()),
                STAT(current_organism->get_static_fitness())});

            // Add to database
            db.insert_rows(tmp);
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

bool God::mate(const std::string &name1, const std::string &name2, const nlohmann::json &species_constants)
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
    if(helper::weighted_prob(parent1->get_conceiving_probability()))
    {
        bool monitor_in_simulation = false;

        if (monitor_offsprings && (parent1->get_monitor_in_simulation() || parent2->get_monitor_in_simulation()))
            monitor_in_simulation = true;

        return spawn_organism(parent1->clone(parent1->get_kind(),
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
    std::mt19937_64 rng; rng.seed(std::random_device()());
    std::uniform_real_distribution<double> dis(0.0, p_range * 2);
    const double x = p_range - dis(rng);

    return base * (1 + x);
}

void God::update_species(const std::string &kind)
{
    const std::string current_filepath = "../../data/json/" + kind + "/current.json";
    const std::string modify_filepath = "../../data/json/" + kind + "/modify.json";

    std::ifstream current_in(current_filepath);
    std::ifstream modify_in(modify_filepath);

    nlohmann::json current, modify;
    current_in >> current;
    modify_in >> modify;

    for (const auto [key, value]: modify.items())
    {
        current[key] = updateStat((double)current[key], (double)value);
    }

    current_in.close();
    modify_in.close();

    std::ofstream current_out(current_filepath);

    current_out << current;

    current_out.close();
}

double God::killer_function(const double &index, const double &size) const
{
    // return std::exp(-x / (s / 10.0))
    // return pow(x / s, 1 / 1.75)
    // return 1 - (1 / (1 + exp(-(10 * index - size) / pow(size, 0.5))));
    const double &ratio = 1.0 / 10.0;
    return 1 - (1 / (1 + exp((ratio * size - index) / (ratio * pow(size, 0.5)))));
}

int God::creator_function(const double &value, const double &factor) const
{
    return floor(1 - 1 / (factor * (value - 1)));
}

void God::happy_new_year(const bool &log)
{
    recent_births = 0;
    recent_deaths = 0;


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

    __gnu_parallel::sort(organisms_vec.begin(),
        organisms_vec.end(),
        [](const std::pair<ENTITY, double> &x, const std::pair<ENTITY, double> &y){
            return x.first->get_death_factor() > y.first->get_death_factor();
    });

    // Mark the organisms in organism_vec for death

    int tmp_i = 0;
    __gnu_parallel::for_each(organisms_vec.begin(), organisms_vec.end(), [this, &tmp_i, &organisms_vec](std::pair<ENTITY, double> &x) {
        x.second = helper::weighted_prob(
            // killerFunction(x.first.get_fitness(), organisms_vec.size())
            killer_function(tmp_i++, organisms_vec.size())
        );
    });

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


    /***********************************
     *       Annual Mating Begins      *
     ***********************************/

    std::unordered_map<std::string, std::vector<ENTITY>> organismsByKind;
    for(const auto &organism : organisms)
        organismsByKind[organism.second->get_full_species_name()].push_back(organism.second);

    int index_parent1, index_parent2;
    for(auto &organism_tuple : organismsByKind)
    {
        // Mating organisms of species organism_tuple.first

        auto &organism_list = organism_tuple.second;
        if (organism_list.size() == 0)
        {
            continue;
        }

        update_species(organism_tuple.first);

        const std::string current_filepath = "../../data/json/" + organism_tuple.first + "/current.json";
        std::ifstream current_in(current_filepath);
        nlohmann::json species_constants;
        current_in >> species_constants;
        current_in.close();

        std::vector<ENTITY> mating_list1, mating_list2;

        if(organism_list.empty())
            continue;

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

        std::mt19937_64 rng; rng.seed(std::random_device()());

        while (mating_list1.size() > 0 && mating_list2.size() > 0)
        {
            std::uniform_int_distribution<int> dis_parent(0, std::min(mating_list2.size(), mating_list1.size()) - 1);
            index_parent1 = dis_parent(rng);
            index_parent2 = dis_parent(rng);
            const auto &parent1 = mating_list1[index_parent1];
            const auto &parent2 = mating_list2[index_parent2];

            std::uniform_real_distribution<double> dis_children(0.0, 1.0);
            int n_children = creator_function(dis_children(rng), parent1->get_offsprings_factor());
            while(n_children--)
            {
                if (mate(parent1->get_name(), parent2->get_name(), species_constants))
                {
                    recent_births++;
                }
            }

            mating_list1.erase(mating_list1.begin() + index_parent1);
            mating_list2.erase(mating_list2.begin() + index_parent2);
            mating_list1.shrink_to_fit();
            mating_list2.shrink_to_fit();
        }
    }


    /************************************
     *       Annual Ageing Begins      *
     ************************************/

    // todo: parallel v sequential performance test

    __gnu_parallel::for_each(organisms.begin(), organisms.end(), [](auto &x){
        x.second->increment_age();
    });

    year++;

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

    std::string table_name = "STATS_" + kind;
    for (auto & c: table_name) c = toupper(c);

    std::vector<STAT> db_row = stat_fetcher::get_db_row(organisms, kind, kingdom, year);
    db.insert_stat_row(db_row, kind, kingdom);
}

void God::send_data_to_simulation()
{
    socket.send(zmq::buffer(stat_fetcher::prepare_data_for_simulation(organisms)), zmq::send_flags::dontwait);
}

bool God::listen_for_simulation_once()
{
    std::string positive = "SEND";
    std::string negative = "STOP";
    zmq::message_t response;
    socket.recv(response);
    std::string response_str = response.to_string();
    if(response_str == positive)
    {
        send_data_to_simulation();
        happy_new_year(true);
        return true;
    }
    else if(response_str == negative)
    {
        return false;
    }
    else
    {
        std::cout << "Error: " << response_str << " received\n";
        return false;
    }
}

void God::start_listening_for_simulation()
{
    std::cout << "God starts listening...\n";
    while(listen_for_simulation_once());
    std::cout << "Alas! God stopped listening.\n";
}
