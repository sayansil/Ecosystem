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

class God
{
public:

    /***********************
     *  Public attributes  *
    ************************/

    std::unordered_map<std::string, Animal> animals;

    /******************************
     *  Constructor / Destructor  *
    *******************************/

    God();
    ~God();

    /**************************************
     *         Available to users         *
    ***************************************/

    void catastrophe();
    void happyNewYear();
    void killAnimals(const std::vector<std::string> &);
    void spawnAnimal(const Animal&);

    std::vector<Animal> animalSort(bool (*comp)(const Animal &, const Animal &));
    std::unordered_map<std::string, std::vector<Animal>> animalSortByKind(bool (*comp)(const Animal &, const Animal &));

private:

    /************************
     *  Private attributes  *
    *************************/

    DatabaseManager db;
    const int maxMateTrials = 100;

    /****************************************
     *         Unavailable to users         *
    *****************************************/

    void mate(const std::string&, const std::string&);
};

#endif // GOD_HPP
