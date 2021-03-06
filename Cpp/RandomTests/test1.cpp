#include <god.hpp>
#include <iostream>
#include <memory>
#include <stat_fetcher.hpp>

void evaluation(God &god)
{
    std::cout << "Population: " << stat_fetcher::get_population(god.organisms) << '\n';
    std::cout << "Deaths: " << god.recent_deaths << '\n';
    std::cout << "Births: " << god.recent_births << '\n';

    std::cout << "Average height: " << stat_fetcher::get_stat_average(god.organisms, "height") << '\n';
    std::cout << "Average weight: " << stat_fetcher::get_stat_average(god.organisms, "weight") << '\n';

    double low, high;
    std::string attribute;

    attribute = "age";
    std::tie(low, high) = stat_fetcher::get_stat_gap(god.organisms, attribute);
    std::cout << attribute << " - Lowest: " << low << " | Highest: " << high << '\n';

    attribute = "generation";
    std::tie(low, high) = stat_fetcher::get_stat_gap(god.organisms, "generation");
    std::cout << attribute << " - Lowest: " << low << " | Highest: " << high << '\n';

    std::cout << "Kind Distribution:\n";
    for (const auto &kind : stat_fetcher::get_kind_distribution(god.organisms))
    {
        std::cout << kind.first << " : " << kind.second << '\n';
    }

    std::cout << "\n\n";
}

int main()
{
    unsigned int initial_organism_count = 500;
    unsigned int years_to_simulate = 50;

    God allah(true);
    allah.reset_species("animal/deer");

    while (initial_organism_count--)
    {
        allah.spawn_organism(std::make_shared<Animal>("deer", 10, true));
    }

    std::cout << "\n\nINITIAL EVALUATION:\n\n";
    evaluation(allah);

    std::cout << "Simulating " << years_to_simulate << " years\n";

    while (years_to_simulate--)
    {
        allah.happy_new_year(true);
        allah.remember_species("animal/deer");
    }

    std::cout << "\n\nFINAL EVALUATION:\n\n";
    evaluation(allah);
}
