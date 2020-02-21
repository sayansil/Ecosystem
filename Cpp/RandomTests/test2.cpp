#include <god.hpp>
#include <prakhar1989/ProgressBar.hpp>
#include <stat_fetcher.hpp>

void evaluation(God god)
{
    std::cout << "Population: " << stat_fetcher::getPopulation(god.animals) << '\n';

    std::cout << "Average height: " << stat_fetcher::getStatAverage(god.animals, "height") << '\n';
    std::cout << "Average weight: " << stat_fetcher::getStatAverage(god.animals, "weight") << '\n';
    std::cout << "Average max appetite: " << stat_fetcher::getStatAverage(god.animals, "max_appetite_at_age") << '\n';
    std::cout << "Average max speed: " << stat_fetcher::getStatAverage(god.animals, "max_speed_at_age") << '\n';
    std::cout << "Average max stamina: " << stat_fetcher::getStatAverage(god.animals, "max_stamina_at_age") << '\n';
    std::cout << "Average max vitality: " << stat_fetcher::getStatAverage(god.animals, "max_vitality_at_age") << '\n';

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

    // for (const auto &animal : allah.animals)
    // {
    //     if (animal.second.max_vitality_at_age != animal.second.max_vitality_at_age)
    //         std::cout << animal.second.get_base_vitality() << ',' << animal.second.get_vitality_multiplier() << ',' << animal.second.max_age << ',' << animal.second.age << ',' << animal.second.height_on_vitality << ',' << animal.second.height << ',' << animal.second.get_max_height() << ',' << animal.second.weight_on_vitality << ',' << animal.second.weight << ',' << animal.second.get_max_weight() << '\n';
    // }
    // std::cout <<'\n';



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


