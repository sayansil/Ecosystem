#include <god.hpp>
#include <prakhar1989/ProgressBar.hpp>
#include <stat_fetcher.hpp>

void evaluation(God god)
{
    std::cout << "Population: " << stat_fetcher::getPopulation(god.animals) << '\n';

    stat_type low, high;
    std::string attribute;

    attribute = "age";
    std::tie(low, high) = stat_fetcher::getStatGap(god.animals, attribute);
    std::cout << attribute << " - Lowest: " << std::get<unsigned int>(low) << " | Highest: " << std::get<unsigned int>(high) << '\n';

    attribute = "generation";
    std::tie(low, high) = stat_fetcher::getStatGap(god.animals, "generation");
    std::cout << attribute << " - Lowest: " << std::get<unsigned int>(low) << " | Highest: " << std::get<unsigned int>(high) << '\n';

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

    ProgressBar progressBar(years_to_simulate, 70, '#', '-');

    while (initial_animal_count--) allah.spawnAnimal(Animal("deer"));

    std::cout << "\n\nINITIAL EVALUATION:\n\n";
    evaluation(allah);

    std::cout << "Simulating " << years_to_simulate << " years\n";
    while (years_to_simulate--)
    {
        allah.happyNewYear();

        ++progressBar;
        if (years_to_simulate%10 == 0)
            progressBar.display();
    }

    std::cout << "\n\nFINAL EVALUATION:\n\n";
    evaluation(allah);
}


