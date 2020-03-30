#include <god.hpp>
#include <iostream>
#include <memory>
#include <prakhar1989/ProgressBar.hpp>
#include <stat_fetcher.hpp>

int main()
{
    unsigned int initial_organism_count = 500;

    God allah;
    allah.monitor_offsprings = true;

    allah.reset_species("deer");

    while (initial_organism_count--)
    {
        allah.spawn_organism(std::make_shared<Animal>("deer", 10, true));
    }

    allah.start_listening_for_simulation();
}
