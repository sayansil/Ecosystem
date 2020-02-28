#include <stat_fetcher.hpp>
#include <vector>
#include <helper.hpp>

void init_csv()
{
    const std::vector<std::string> columns({
        "kind",
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

        "average_gender",
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
        "average_vision_radius"
    });
    std::string header = "";
    for (const auto& column: columns) header = header + ',' + column;
    header = header.substr(1) + '\n';

    helper::saveCSV(header, "../../data/csv/yearly.csv");
}

int main()
{
    init_csv();
}
