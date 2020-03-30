#ifndef GOD_HPP
#define GOD_HPP

#include <algorithm>
#include <execution>
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

    ENTITY_MAP_TYPE organisms;  // < name, Entity >
    unsigned int recent_deaths = 0;
    unsigned int recent_births = 0;

    bool monitor_offsprings = false;

    /******************************
     *  Constructor / Destructor  *
     ******************************/

    God();
    ~God();


    /**************************************
     *         Available to users         *
     **************************************/

    void catastrophe();
    void reset_species(const std::string&);
    void happyNewYear(const bool &log = false);
    void killOrganisms(const std::vector<std::string>&);
    bool spawnOrganism(const ENTITY&);
    void sendDataToPy();
    bool listenForSimulationPing();
    void startListeningForSimulationPing();

    std::vector<ENTITY> organismSort(bool (*comp)(const ENTITY&, const ENTITY&));
    std::unordered_map<std::string, std::vector<ENTITY>> organismSortByKind(bool (*comp)(const ENTITY&, const ENTITY&));

protected:
    double killerFunction(const double &index, const double &size) const;
    int creatorFunction(const double &value, const double &factor) const;

private:

    DatabaseManager db;
    const int maxMateTrials = 100;
    zmq::context_t context;
    zmq::socket_t socket;


    /****************************************
     *         Unavailable to users         *
     ****************************************/

    bool mate(const std::string&, const std::string&, const nlohmann::json & = nlohmann::json());
    void update_species(const std::string&);
};

#endif // GOD_HPP
