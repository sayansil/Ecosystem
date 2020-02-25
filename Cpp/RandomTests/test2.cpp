#include <god.hpp>
#include <prakhar1989/ProgressBar.hpp>
#include <stat_fetcher.hpp>

void evaluation(const God &god)
{
    std::cout << "Population: " << stat_fetcher::getPopulation(god.animals) << '\n';
    std::cout << "Deaths: " << god.recent_deaths << '\n';
    std::cout << "Births: " << god.recent_births << '\n';

    std::cout << "Average height: " << stat_fetcher::getStatAverage(god.animals, "height") << '\n';
    std::cout << "Average weight: " << stat_fetcher::getStatAverage(god.animals, "weight") << '\n';
    std::cout << "Average max appetite: " << stat_fetcher::getStatAverage(god.animals, "max_appetite_at_age") << '\n';
    std::cout << "Average max speed: " << stat_fetcher::getStatAverage(god.animals, "max_speed_at_age") << '\n';
    std::cout << "Average max stamina: " << stat_fetcher::getStatAverage(god.animals, "max_stamina_at_age") << '\n';
    std::cout << "Average max vitality: " << stat_fetcher::getStatAverage(god.animals, "max_vitality_at_age") << '\n';

    double low, high;
    std::string attribute;

    attribute = "age";
    std::tie(low, high) = stat_fetcher::getStatGap(god.animals, attribute);
    std::cout << attribute << " - Lowest: " << low << " | Highest: " << high << '\n';

    attribute = "generation";
    std::tie(low, high) = stat_fetcher::getStatGap(god.animals, "generation");
    std::cout << attribute << " - Lowest: " << low << " | Highest: " << high << '\n';

    std::cout << "Kind Distribution:\n";
    for (const auto &kind : stat_fetcher::getKindDistribution(god.animals))
    {
        std::cout << kind.first << " : " << kind.second << '\n';
    }

    std::cout << "\n\n";
}

int main()
{
    unsigned int initial_animal_count = 500;
    unsigned int years_to_simulate = 100;

    God allah;
    allah.reset_species("deer");

    ProgressBar progressBar(years_to_simulate, 70, '#', '-');

    while (initial_animal_count--) allah.spawnAnimal(Animal("deer", 10));

    std::cout << "\n\nINITIAL EVALUATION:\n\n";
    evaluation(allah);

    std::cout << "Simulating " << years_to_simulate << " years\n";
    unsigned int i = 0;
    while (i++ < years_to_simulate)
    {
        allah.happyNewYear();

        ++progressBar;
        if (i % 10 == 0)
            progressBar.display();

        // evaluation(allah);

        if (allah.animals.size() == 0)
        {
            std::cout << "Population extinct at year #" << i << "\n";
            break;
        }
        if (allah.animals.size() >= 35000)
        {
            std::cout << "Overpopulated at year #" << i << "\n";
            break;
        }
    }

    std::cout << "\n\nFINAL EVALUATION:\n\n";
    evaluation(allah);
}


