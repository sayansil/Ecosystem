#include <god.hpp>
#include <prakhar1989/ProgressBar.hpp>
#include <stat_fetcher.hpp>

void init_csv(const std::string &kind)
{
    const std::vector<std::string> columns({
        "male",
        "female",
        "matable_male",
        "matable_female",
        "conceiving_probability",
        "mating_age_start",
        "mating_age_end",
        "max_age",
        "mutation_probability",
        "offsprings_factor",
        "age_on_death",
        "fitness_on_death",
        "age_fitness_on_death_ratio",
        "height_on_speed",
        "height_on_stamina",
        "height_on_vitality",
        "weight_on_speed",
        "weight_on_stamina",
        "weight_on_vitality",
        "vitality_on_appetite",
        "vitality_on_speed",
        "stamina_on_appetite",
        "stamina_on_speed",
        "theoretical_maximum_base_appetite",
        "theoretical_maximum_base_height",
        "theoretical_maximum_base_speed",
        "theoretical_maximum_base_stamina",
        "theoretical_maximum_base_vitality",
        "theoretical_maximum_base_weight",
        "theoretical_maximum_height",
        "theoretical_maximum_speed",
        "theoretical_maximum_weight",
        "theoretical_maximum_height_multiplier",
        "theoretical_maximum_speed_multiplier",
        "theoretical_maximum_stamina_multiplier",
        "theoretical_maximum_vitality_multiplier",
        "theoretical_maximum_weight_multiplier",
        "sleep_restore_factor",

        "average_generation",
        "average_immunity",
        "average_age",
        "average_height",
        "average_weight",
        "average_max_appetite_at_age",
        "average_max_speed_at_age",
        "average_max_stamina_at_age",
        "average_max_vitality_at_age",
        "average_static_fitness",
        "average_death_factor",
        "average_vision_radius"});
    std::string header = "";
    for (const auto &column : columns)
        header = header + ',' + column;
    header = header.substr(1) + '\n';

    helper::saveCSV(header, "../../data/csv/yearly_" + kind + ".csv");
}

std::string get_row(std::unordered_map<std::string, double> stat_display_map)
{
    std::vector<double> row({
        stat_display_map["male"],
        stat_display_map["female"],
        stat_display_map["matable_male"],
        stat_display_map["matable_female"],
        stat_display_map["conceiving_probability"],
        stat_display_map["mating_age_start"],
        stat_display_map["mating_age_end"],
        stat_display_map["max_age"],
        stat_display_map["mutation_probability"],
        stat_display_map["offsprings_factor"],
        stat_display_map["age_on_death"],
        stat_display_map["fitness_on_death"],
        stat_display_map["age_fitness_on_death_ratio"],
        stat_display_map["height_on_speed"],
        stat_display_map["height_on_stamina"],
        stat_display_map["height_on_vitality"],
        stat_display_map["weight_on_speed"],
        stat_display_map["weight_on_stamina"],
        stat_display_map["weight_on_vitality"],
        stat_display_map["vitality_on_appetite"],
        stat_display_map["vitality_on_speed"],
        stat_display_map["stamina_on_appetite"],
        stat_display_map["stamina_on_speed"],
        stat_display_map["theoretical_maximum_base_appetite"],
        stat_display_map["theoretical_maximum_base_height"],
        stat_display_map["theoretical_maximum_base_speed"],
        stat_display_map["theoretical_maximum_base_stamina"],
        stat_display_map["theoretical_maximum_base_vitality"],
        stat_display_map["theoretical_maximum_base_weight"],
        stat_display_map["theoretical_maximum_height"],
        stat_display_map["theoretical_maximum_speed"],
        stat_display_map["theoretical_maximum_weight"],
        stat_display_map["theoretical_maximum_height_multiplier"],
        stat_display_map["theoretical_maximum_speed_multiplier"],
        stat_display_map["theoretical_maximum_stamina_multiplier"],
        stat_display_map["theoretical_maximum_vitality_multiplier"],
        stat_display_map["theoretical_maximum_weight_multiplier"],
        stat_display_map["sleep_restore_factor"],
        stat_display_map["average_generation"],
        stat_display_map["average_immunity"],
        stat_display_map["average_age"],
        stat_display_map["average_height"],
        stat_display_map["average_weight"],
        stat_display_map["average_max_appetite_at_age"],
        stat_display_map["average_max_speed_at_age"],
        stat_display_map["average_max_stamina_at_age"],
        stat_display_map["average_max_vitality_at_age"],
        stat_display_map["average_static_fitness"],
        stat_display_map["average_death_factor"],
        stat_display_map["average_vision_radius"]});

    std::string row_str = "";
    for (const auto &value : row)
        row_str = row_str + ',' + std::to_string(value);
    row_str = row_str.substr(1) + '\n';

    return row_str;
}

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
    init_csv("deer");
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

        // allah.sendDataToPy();

        ++progressBar;
        if (i % 10 == 0)
            progressBar.display();

        // evaluation(allah);

        std::string row = get_row(stat_fetcher::getAllStats(allah.animals, "deer"));
        helper::appendToCSV(row, "../../data/csv/yearly_deer.csv");

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
