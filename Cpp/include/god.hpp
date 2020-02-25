#ifndef GOD_HPP
#define GOD_HPP

#include <algorithm>
#include <execution>
#include <iostream>
#include <unordered_map>
#include <utility>
#include <vector>

#include <database_manager.hpp>
#include <animal.hpp>
#include <stat_fetcher.hpp>

using ANIMAL_MAP_TYPE = std::unordered_map<std::string, Animal>;

class God
{
public:

    /***********************
     *  Public attributes  *
    ************************/

    ANIMAL_MAP_TYPE animals;
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
    void killAnimals(const std::vector<std::string> &);
    bool spawnAnimal(const Animal&);

    std::vector<Animal> animalSort(bool (*comp)(const Animal &, const Animal &));
    std::unordered_map<std::string, std::vector<Animal>> animalSortByKind(bool (*comp)(const Animal &, const Animal &));

protected:
    double killerFunction(const double &index, const double &size) const;
    int creatorFunction(const double &value, const double &factor) const;

private:

    /************************
     *  Private attributes  *
    *************************/

    DatabaseManager db;
    const int maxMateTrials = 100;

    /****************************************
     *         Unavailable to users         *
    *****************************************/

    bool mate(const std::string&, const std::string&, const nlohmann::json& = nlohmann::json());
    void update_species(const std::string &);
};

#endif // GOD_HPP
