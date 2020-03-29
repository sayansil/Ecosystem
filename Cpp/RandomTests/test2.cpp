#include <god.hpp>
#include <iostream>
#include <memory>
#include <prakhar1989/ProgressBar.hpp>
#include <stat_fetcher.hpp>

int main()
{
    unsigned int initial_organism_count = 200;
    unsigned int years_to_simulate = 100;

    God allah;
    allah.reset_species("bamboo");

    ProgressBar progressBar(years_to_simulate, 70, '#', '-');

    while (initial_organism_count--)
    {
        allah.spawnOrganism(std::make_shared<Plant>("bamboo", 10, true));
    }

    allah.startListeningForSimulationPing();
}
