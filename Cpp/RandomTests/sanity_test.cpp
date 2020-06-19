#include <god.hpp>
#include <iostream>

int main()
{
    unsigned int initial_organism_count = 200;
    unsigned int years_to_simulate = 100;

    God allah(true);
    allah.reset_species("plant/grass");
    while (initial_organism_count--)
    {
        allah.spawn_organism(std::make_shared<Plant>("grass", 10));
    }
    
    while (years_to_simulate--)
    {
        allah.happy_new_year(true);
        allah.remember_species("plant/grass");
    }
}
