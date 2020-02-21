#include <god.hpp>
#include <prakhar1989/ProgressBar.hpp>
#include <stat_fetcher.hpp>

int main()
{
    unsigned int initial_animal_count = 500;
    unsigned int years_to_simulate = 100;

    God allah;

    ProgressBar progressBar(years_to_simulate, 70, '#', '-');

    while (initial_animal_count--) allah.spawnAnimal(Animal("deer"));
    std::cout << "Initially: " << stat_fetcher::getPopulation(allah.animals) << "\n\n";

    while (years_to_simulate--)
    {
        allah.happyNewYear();

        ++progressBar;
        if (years_to_simulate%10 == 0)
            progressBar.display();
    }
    std::cout << "\n\nFinally:\n";

    stat_type low, high;

    std::tie(low, high) = stat_fetcher::getStatGap(allah.animals, "age");
    std::cout << "AGE - Lowest: " << std::get<unsigned int>(low) << " Highest: " << std::get<unsigned int>(high) << '\n';
}
