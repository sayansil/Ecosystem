#include <god.hpp>
#include <iostream>
#include <memory>
#include <stat_fetcher.hpp>

int main()
{
    unsigned int initial_organism_count_m = 500;
    unsigned int initial_organism_count_nm = 500;

    God allah(true);
    allah.monitor_offsprings = true;

    allah.reset_species("plant/bamboo");

    while (initial_organism_count_m--)
    {
        allah.spawn_organism(std::make_shared<Plant>("bamboo", 10, true));
    }
    while (initial_organism_count_nm--)
    {
        allah.spawn_organism(std::make_shared<Plant>("bamboo", 10, false));
    }

    allah.start_listening_for_simulation();
}
