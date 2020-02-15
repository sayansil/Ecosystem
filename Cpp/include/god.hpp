#ifndef GOD_HPP
#define GOD_HPP

#include <DatabaseManager.hpp>
#include <animal.hpp>
#include <vector>
#include <iostream>
#include <unordered_map>

class God
{
public:
    God();
    void spawnAnimal(const std::string& kind, const std::string& chromosome, const chromosome_map_type& chromosome_structure, const unsigned int& generation, const std::string& name);
    void killAnimal(const std::string&);
    ~God();
private:
    void produceAnimal(const std::string& kind, const std::string& chromosome, const chromosome_map_type& chromosome_structure, const unsigned int& generation, const std::string& name, const unsigned int& X, const unsigned int& Y);
    void mate(const std::string&, const std::string&);
    void happyNewYear();
    DatabaseManager db;
    std::unordered_map<std::string, Animal> animals;
};

#endif // GOD_HPP
