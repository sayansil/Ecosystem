#ifndef HERBIVORE_HPP
#define HERBIVORE_HPP

#include <organism.hpp>
#include <string>
#include <helper.hpp>

class Deer : public Animal
{
private:


    inline static std::string species_name = "deer";
    static chromosome_map_type species_chromosome_structure;
public:
    Deer(const std::string& chromosome, const unsigned int& generation, const std::string& name, const unsigned int &X, const unsigned int& Y);
    Deer(const std::string& chromosome, const unsigned int& generation, const std::string& name);


    static void initialize()
    {

        species_chromosome_structure["bv"] = {{"start", 0}, {"length", 10}};
        species_chromosome_structure["vm"] = {{"start", 10}, {"length", 8}};
        species_chromosome_structure["bs"] = {{"start", 18}, {"length", 10}};
        species_chromosome_structure["sm"] = {{"start", 28}, {"length", 8}};
        species_chromosome_structure["mh"] = {{"start", 36}, {"length", 10}};
        species_chromosome_structure["mw"] = {{"start", 46}, {"length", 10}};
        species_chromosome_structure["bh"] = {{"start", 56}, {"length", 10}};
        species_chromosome_structure["bw"] = {{"start", 66}, {"length", 10}};
        species_chromosome_structure["hm"] = {{"start", 76}, {"length", 8}};
        species_chromosome_structure["wm"] = {{"start", 84}, {"length", 8}};
        species_chromosome_structure["bp"] = {{"start", 92}, {"length", 10}};
        species_chromosome_structure["pm"] = {{"start", 102}, {"length", 8}};
        species_chromosome_structure["ba"] = {{"start", 110}, {"length", 10}};
        species_chromosome_structure["im"] = {{"start", 120}, {"length", 10}};
        species_chromosome_structure["gn"] = {{"start", 130}, {"length", 1}};
    }


    static std::string generate_random_chromosome()
    {
        unsigned int chromosome_length = 0;
        for(auto i = species_chromosome_structure.begin(); i != species_chromosome_structure.end(); i++)
            chromosome_length += i->second["length"];
        return helper::random_binary(chromosome_length);
    }

};


#endif
