#include <god.hpp>
#include <iostream>
#include <memory>
#include <prakhar1989/ProgressBar.hpp>
#include <stat_fetcher.hpp>

int main()
{
    unsigned int initial_organism_count = 1000;

    God allah;
    allah.reset_species("deer");

    while (initial_organism_count--)
    {
        allah.spawnOrganism(std::make_shared<Animal>("deer", 10, true));
    }

    allah.startListeningForSimulationPing();
}
