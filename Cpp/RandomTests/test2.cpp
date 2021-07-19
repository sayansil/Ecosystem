#include <god.hpp>
#include <iostream>

int main()
{
    unsigned int initial_organism_count = 200;
    unsigned int years_to_simulate = 300;

    God allah(false);
    allah.reset_species("animal/deer");
    while (initial_organism_count--)
    {
        allah.spawn_organism(std::make_shared<Animal>("deer", 10, false, "OG-" + std::to_string(initial_organism_count)));
    }

    while (years_to_simulate--)
    {
        allah.happy_new_year(true);
        //allah.remember_species("animal/deer");
    }
}
