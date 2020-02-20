#ifndef GOD_HPP
#define GOD_HPP

#include <DatabaseManager.hpp>
#include <animal.hpp>
#include <vector>
#include <iostream>
#include <unordered_map>
#include <execution>
#include <algorithm>
#include <utility>

class God
{
public:

    std::unordered_map<std::string, Animal> animals;

    God();
    void spawnAnimal(const Animal&);
    void killAnimal(const std::string&);
    ~God();
    void happyNewYear();
    void catastrophe();

    template <typename Comp>
    std::vector<Animal> animalSort(Comp comp)
    {
        std::vector<Animal> animal_vec;
        for(const auto& i : animals)
            animal_vec.push_back(i.second);
        animal_vec.shrink_to_fit();
        std::sort(animal_vec.begin(), animal_vec.end(), comp);
        return animal_vec;
    }

    template <typename Comp>
    std::unordered_map<std::string, std::vector<Animal>> animalSortByKind(Comp comp)
    {
        std::unordered_map<std::string, std::vector<Animal>> animal_map;
        for(const auto& i : animals)
            animal_map[i.second.kind].push_back(i.second);
        for(const auto& i : animal_map)
            std::sort(i.second.begin(), i.second.end(), comp);
        return animal_map;
    }

private:
    void mate(const std::string&, const std::string&);
    DatabaseManager db;
    const int maxMateTrials = 100;
};

#endif // GOD_HPP
