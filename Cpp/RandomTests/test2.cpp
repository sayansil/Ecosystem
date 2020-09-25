#include <god.hpp>
#include <iostream>
#include <helper.hpp>
#include <map_maker.hpp>

int main()
{
    unsigned int initial_organism_count = 50;
    unsigned int years_to_simulate = 10;

    God allah(true);
    allah.monitor_offsprings = true;
    allah.reset_species("plant/bamboo");

    while (initial_organism_count--)
    {
        allah.spawn_organism(std::make_shared<Plant>("bamboo", 10, true));
    }

    while (years_to_simulate--)
    {
        allah.happy_new_year(true);
    }

    auto data = allah.get_live_data();

    std::cout << data.size() << "\n";
    for (const auto &i : data)
    {
        std::cout << "**********************\n";

        for (const auto &[j, k] : i)
        {
            std::cout << j << " : " << k << "\n";
        }

        std::cout << "**********************\n";
    }
}
