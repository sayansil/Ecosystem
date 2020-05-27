#include <god.hpp>
#include <iostream>

int main()
{
    unsigned int initial_organism_count = 200;
    unsigned int years_to_simulate = 100;

    God allah(true);
    std::cout << "reached HERE1\n";
    allah.reset_species("animal/deer");
    std::cout << "reached HERE2\n";
    while (initial_organism_count--)
    {
        allah.spawn_organism(std::make_shared<Animal>("deer", 10));
    }

    while (years_to_simulate--)
    {
        allah.happy_new_year(true);
        allah.remember_species("animal/deer");
    }
}
