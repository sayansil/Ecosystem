#include <god.hpp>
#include <iostream>
#include <helper.hpp>

int main()
{
    unsigned int initial_organism_count = 200;
    unsigned int years_to_simulate = 500;
    std::string kingdom = "animal";
    std::string kind = "deer";

    God allah(true);
    allah.reset_species(helper::generate_full_species_name(kingdom, kind));
    while (initial_organism_count--)
    {
        allah.spawn_organism(std::make_shared<Animal>(kind, 10));
    }

    while (years_to_simulate--)
    {
        allah.happy_new_year(true);
        allah.remember_species(helper::generate_full_species_name(kingdom, kind));
    }

    std::cout << "Simulation complete. Ready to generate report for " << helper::generate_full_species_name(kingdom, kind) << "\n";
}
