#ifndef GOD_HPP
#define GOD_HPP

#include <algorithm>
#include <execution>
#include <iostream>
#include <unordered_map>
#include <utility>
#include <vector>

#include <database_manager.hpp>
#include <organism.hpp>
#include <plant.hpp>
#include <stat_fetcher.hpp>

#include <zmq.hpp>

class God
{
public:

    /***********************
     *  Public attributes  *
    ************************/

    ORGANISM_MAP_TYPE organisms;
    unsigned int recent_deaths;
    unsigned int recent_births;

    /******************************
     *  Constructor / Destructor  *
    *******************************/

    God();
    ~God();

    /**************************************
     *         Available to users         *
    ***************************************/

    void catastrophe();
    void reset_species(const std::string &);
    void happyNewYear();
    void killOrganisms(const std::vector<std::string> &);
    bool spawnOrganism(const ORGANISM&);
    void sendDataToPy();

    std::vector<ORGANISM> organismSort(bool (*comp)(const ORGANISM&, const ORGANISM&));
    std::unordered_map<std::string, std::vector<ORGANISM>> organismSortByKind(bool (*comp)(const ORGANISM&, const ORGANISM&));

protected:
    double killerFunction(const double &index, const double &size) const;
    int creatorFunction(const double &value, const double &factor) const;

private:

    /************************
     *  Private attributes  *
    *************************/

    DatabaseManager db;
    const int maxMateTrials = 100;
    zmq::context_t context;
    zmq::socket_t socket;


    /****************************************
     *         Unavailable to users         *
    *****************************************/

    bool mate(const std::string&, const std::string&, const nlohmann::json& = nlohmann::json());
    void update_species(const std::string &);
};

#endif // GOD_HPP
