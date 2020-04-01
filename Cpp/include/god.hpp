#ifndef GOD_HPP
#define GOD_HPP

#include <parallel/algorithm>
#include <iostream>
#include <random>
#include <unordered_map>
#include <utility>
#include <vector>
#include <string>

#include <database_manager.hpp>
#include <zmq.hpp>
#include <organism.hpp>
#include <helper.hpp>
#include <stat_fetcher.hpp>

#include <zmq.hpp>

class God
{
public:

    unsigned int year = 0;

    ENTITY_MAP_TYPE organisms;  // < name, Entity >
    unsigned int recent_deaths = 0;
    unsigned int recent_births = 0;

    bool monitor_offsprings = false;

    /******************************
     *  Constructor / Destructor  *
     ******************************/

    God(const bool &gods_eye = false);
    ~God();


    /**************************************
     *         Available to users         *
     **************************************/

    void catastrophe();
    void reset_species(const std::string&);
    void happy_new_year(const bool &log = false);
    void kill_organisms(const std::vector<std::string>&);
    bool spawn_organism(const ENTITY&);
    void remember_species(const std::string &);
    void send_data_to_simulation();
    bool listen_for_simulation_once();
    void start_listening_for_simulation();

    std::vector<ENTITY> organism_sort(bool (*comp)(const ENTITY&, const ENTITY&));
    std::unordered_map<std::string, std::vector<ENTITY>> organism_sort_by_kind(bool (*comp)(const ENTITY&, const ENTITY&));

protected:
    double killer_function(const double &index, const double &size) const;
    int creator_function(const double &value, const double &factor) const;

private:

    DatabaseManager db;
    bool gods_eye = false;
    const int max_mate_trials = 100;
    zmq::context_t context;
    zmq::socket_t socket;


    /****************************************
     *         Unavailable to users         *
     ****************************************/

    bool mate(const std::string&, const std::string&, const nlohmann::json & = nlohmann::json());
    void update_species(const std::string&);
};

#endif // GOD_HPP
