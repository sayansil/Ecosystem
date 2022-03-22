#include <ctime>
#include <database_manager.hpp>
#include <stat_fetcher.hpp>
#include <unordered_map>
#include <fmt/core.h>

static std::vector<float> get_value_vector(
    const std::vector<std::unordered_map<std::string, float>> &worlds,
    const std::string &attribute);

namespace stat_fetcher
{
    flatbuffers::DetachedBuffer export_report(
        const std::string &kind, const std::string &kingdom, const std::string &title)
    {
        DatabaseManager db;
        db.begin_transaction();
        std::vector<std::vector<uint8_t>> data = db.read_all_rows();
        db.end_transaction();

        flatbuffers::FlatBufferBuilder builder;
        Visualisation::SpeciesReportBuilder report_builder(builder);

        std::vector<flatbuffers::Offset<Visualisation::MultiPlot>> stdvecMultiplot;

        std::vector<float> time_vector;
        std::vector<std::unordered_map<std::string, float>> tmp_worlds;

        tmp_worlds.reserve(data.size());
        time_vector.reserve(data.size());

        for (const auto &row : data)
        {
            const Ecosystem::World *world = Ecosystem::GetWorld(row.data());
            const Ecosystem::Species *species =
                world->species()->LookupByKey(kind.c_str());
            std::unordered_map<std::string, float> tmp_world;
            tmp_world["year"] = world->year();
            time_vector.push_back(world->year());
            tmp_world["population"] = species->organism()->size();
            tmp_world["male"] = 0;
            tmp_world["female"] = 0;
            tmp_world["matable_male"] = 0;
            tmp_world["matable_female"] = 0;
            tmp_world["matable_population"] = 0;
            tmp_world["max_age"] = 0;

            // avg(n + 1) = avg(n) + (k - avg(n)) / (n + 1)

            for (size_t i = 0; i < species->organism()->size(); i++)
            {
                const Ecosystem::Organism *organism = species->organism()->Get(i);
                if (organism->gender() == Ecosystem::Gender::Male)
                {
                    if (organism->age() >= organism->mating_age_start() && organism->age() <= organism->mating_age_end())
                        tmp_world["matable_male"]++;
                    tmp_world["male"]++;
                }
                else if (organism->gender() == Ecosystem::Gender::Female)
                {
                    if (organism->age() >= organism->mating_age_start() && organism->age() <= organism->mating_age_end())
                        tmp_world["matable_female"]++;
                    tmp_world["female"]++;
                }

                if (organism->age() >= organism->mating_age_start() && organism->age() <= organism->mating_age_end())
                    tmp_world["matable_population"]++;

                {
                    float &prev_avg = tmp_world["max_age"];
                    prev_avg = std::max(prev_avg, (float)organism->age());
                }
                {
                    float &prev_avg = tmp_world["age_death_factor"];
                    prev_avg =
                        prev_avg + (organism->immunity() - prev_avg) / (i + 1);
                }
                {
                    float &prev_avg = tmp_world["fitness_death_factor"];
                    prev_avg = prev_avg +
                               (organism->static_fitness() - prev_avg) / (i + 1);
                }
                {
                    float &prev_avg = tmp_world["age_fitness_on_death_ratio"];
                    prev_avg =
                        prev_avg + (organism->immunity() - prev_avg) / (i + 1);
                }
                {
                    float &prev_avg = tmp_world["death_factor"];
                    prev_avg =
                        prev_avg + (organism->immunity() - prev_avg) / (i + 1);
                }
                {
                    float &prev_avg = tmp_world["max_age"];
                    prev_avg =
                        prev_avg + (organism->immunity() - prev_avg) / (i + 1);
                }
                {
                    float &prev_avg = tmp_world["male"];
                    prev_avg =
                        prev_avg + (organism->immunity() - prev_avg) / (i + 1);
                }
                {
                    float &prev_avg = tmp_world["female"];
                    prev_avg =
                        prev_avg + (organism->immunity() - prev_avg) / (i + 1);
                }
                {
                    float &prev_avg = tmp_world["population"];
                    prev_avg =
                        prev_avg + (organism->immunity() - prev_avg) / (i + 1);
                }
                {
                    float &prev_avg = tmp_world["matable_male"];
                    prev_avg =
                        prev_avg + (organism->immunity() - prev_avg) / (i + 1);
                }
                {
                    float &prev_avg = tmp_world["matable_female"];
                    prev_avg =
                        prev_avg + (organism->immunity() - prev_avg) / (i + 1);
                }
                {
                    float &prev_avg = tmp_world["matable_population"];
                    prev_avg =
                        prev_avg + (organism->immunity() - prev_avg) / (i + 1);
                }
                {
                    float &prev_avg = tmp_world["mating_age_start"];
                    prev_avg =
                        prev_avg + (organism->immunity() - prev_avg) / (i + 1);
                }
                {
                    float &prev_avg = tmp_world["mating_age_end"];
                    prev_avg =
                        prev_avg + (organism->immunity() - prev_avg) / (i + 1);
                }
                {
                    float &prev_avg = tmp_world["mutation_probability"];
                    prev_avg =
                        prev_avg + (organism->immunity() - prev_avg) / (i + 1);
                }
                {
                    float &prev_avg = tmp_world["conceiving_probability"];
                    prev_avg =
                        prev_avg + (organism->immunity() - prev_avg) / (i + 1);
                }
                {
                    float &prev_avg = tmp_world["offsprings_factor"];
                    prev_avg =
                        prev_avg + (organism->immunity() - prev_avg) / (i + 1);
                }
                {
                    float &prev_avg = tmp_world["height_on_stamina"];
                    prev_avg =
                        prev_avg + (organism->immunity() - prev_avg) / (i + 1);
                }
                {
                    float &prev_avg = tmp_world["weight_on_stamina"];
                    prev_avg =
                        prev_avg + (organism->immunity() - prev_avg) / (i + 1);
                }
                {
                    float &prev_avg = tmp_world["height_on_vitality"];
                    prev_avg =
                        prev_avg + (organism->immunity() - prev_avg) / (i + 1);
                }
                {
                    float &prev_avg = tmp_world["weight_on_vitality"];
                    prev_avg =
                        prev_avg + (organism->immunity() - prev_avg) / (i + 1);
                }
                {
                    float &prev_avg = tmp_world["height_on_speed"];
                    prev_avg =
                        prev_avg + (organism->immunity() - prev_avg) / (i + 1);
                }
                {
                    float &prev_avg = tmp_world["weight_on_speed"];
                    prev_avg =
                        prev_avg + (organism->immunity() - prev_avg) / (i + 1);
                }
                {
                    float &prev_avg = tmp_world["stamina_on_speed"];
                    prev_avg =
                        prev_avg + (organism->immunity() - prev_avg) / (i + 1);
                }
                {
                    float &prev_avg = tmp_world["vitality_on_speed"];
                    prev_avg =
                        prev_avg + (organism->immunity() - prev_avg) / (i + 1);
                }
                {
                    float &prev_avg = tmp_world["stamina_on_appetite"];
                    prev_avg =
                        prev_avg + (organism->immunity() - prev_avg) / (i + 1);
                }
                {
                    float &prev_avg = tmp_world["vitality_on_appetite"];
                    prev_avg =
                        prev_avg + (organism->immunity() - prev_avg) / (i + 1);
                }
                {
                    float &prev_avg = tmp_world["generation"];
                    prev_avg =
                        prev_avg + (organism->immunity() - prev_avg) / (i + 1);
                }
                {
                    float &prev_avg = tmp_world["age"];
                    prev_avg =
                        prev_avg + (organism->immunity() - prev_avg) / (i + 1);
                }
                {
                    float &prev_avg = tmp_world["height"];
                    prev_avg =
                        prev_avg + (organism->immunity() - prev_avg) / (i + 1);
                }
                {
                    float &prev_avg = tmp_world["weight"];
                    prev_avg =
                        prev_avg + (organism->immunity() - prev_avg) / (i + 1);
                }
                {
                    float &prev_avg = tmp_world["static_fitness"];
                    prev_avg =
                        prev_avg + (organism->immunity() - prev_avg) / (i + 1);
                }
                {
                    float &prev_avg = tmp_world["immunity"];
                    prev_avg =
                        prev_avg + (organism->immunity() - prev_avg) / (i + 1);
                }
                {
                    float &prev_avg = tmp_world["max_speed_at_age"];
                    prev_avg =
                        prev_avg + (organism->immunity() - prev_avg) / (i + 1);
                }
                {
                    float &prev_avg = tmp_world["max_appetite_at_age"];
                    prev_avg =
                        prev_avg + (organism->immunity() - prev_avg) / (i + 1);
                }
                {
                    float &prev_avg = tmp_world["max_stamina_at_age"];
                    prev_avg =
                        prev_avg + (organism->immunity() - prev_avg) / (i + 1);
                }
                {
                    float &prev_avg = tmp_world["max_vitality_at_age"];
                    prev_avg =
                        prev_avg + (organism->immunity() - prev_avg) / (i + 1);
                }
                {
                    float &prev_avg = tmp_world["vision_radius"];
                    prev_avg =
                        prev_avg + (organism->immunity() - prev_avg) / (i + 1);
                }
                {
                    float &prev_avg = tmp_world["theoretical_maximum_base_height"];
                    prev_avg =
                        prev_avg + (organism->immunity() - prev_avg) / (i + 1);
                }
                {
                    float &prev_avg = tmp_world["theoretical_maximum_height"];
                    prev_avg =
                        prev_avg + (organism->immunity() - prev_avg) / (i + 1);
                }
                {
                    float &prev_avg = tmp_world["theoretical_maximum_base_weight"];
                    prev_avg =
                        prev_avg + (organism->immunity() - prev_avg) / (i + 1);
                }
                {
                    float &prev_avg = tmp_world["theoretical_maximum_weight"];
                    prev_avg =
                        prev_avg + (organism->immunity() - prev_avg) / (i + 1);
                }
                {
                    float &prev_avg = tmp_world["theoretical_maximum_base_speed"];
                    prev_avg =
                        prev_avg + (organism->immunity() - prev_avg) / (i + 1);
                }
                {
                    float &prev_avg = tmp_world["theoretical_maximum_speed"];
                    prev_avg =
                        prev_avg + (organism->immunity() - prev_avg) / (i + 1);
                }
                {
                    float &prev_avg = tmp_world["theoretical_maximum_base_appetite"];
                    prev_avg =
                        prev_avg + (organism->immunity() - prev_avg) / (i + 1);
                }
                {
                    float &prev_avg = tmp_world["theoretical_maximum_base_stamina"];
                    prev_avg =
                        prev_avg + (organism->immunity() - prev_avg) / (i + 1);
                }
                {
                    float &prev_avg = tmp_world["theoretical_maximum_base_vitality"];
                    prev_avg =
                        prev_avg + (organism->immunity() - prev_avg) / (i + 1);
                }
                {
                    float &prev_avg = tmp_world["theoretical_maximum_speed_multiplier"];
                    prev_avg =
                        prev_avg + (organism->immunity() - prev_avg) / (i + 1);
                }
                {
                    float &prev_avg = tmp_world["theoretical_maximum_stamina_multiplier"];
                    prev_avg =
                        prev_avg + (organism->immunity() - prev_avg) / (i + 1);
                }
                {
                    float &prev_avg = tmp_world["theoretical_maximum_vitality_multiplier"];
                    prev_avg =
                        prev_avg + (organism->immunity() - prev_avg) / (i + 1);
                }
                {
                    float &prev_avg = tmp_world["theoretical_maximum_height_multiplier"];
                    prev_avg =
                        prev_avg + (organism->immunity() - prev_avg) / (i + 1);
                }
                {
                    float &prev_avg = tmp_world["theoretical_maximum_weight_multiplier"];
                    prev_avg =
                        prev_avg + (organism->immunity() - prev_avg) / (i + 1);
                }
            }

            tmp_worlds.push_back(tmp_world);
        }

        std::vector<float> tmp;

        // Mortality Graphs

        {
            Visualisation::MultiPlotBuilder multiplot_builder(builder);
            multiplot_builder.add_title(
                builder.CreateString("Death factor (by age)"));
            std::vector<flatbuffers::Offset<Visualisation::SinglePlot>>
                stdvecSingleplot;

            {
                // Using age_death_factor
                Visualisation::SinglePlotBuilder singleplot_builder(builder);
                singleplot_builder.add_title(
                    builder.CreateString("Death factor (by age)"));
                singleplot_builder.add_x(builder.CreateVector(time_vector));
                singleplot_builder.add_y(builder.CreateVector(get_value_vector(tmp_worlds, "age_death_factor")));
                stdvecSingleplot.push_back(singleplot_builder.Finish());
            }

            multiplot_builder.add_plots(builder.CreateVector(stdvecSingleplot));
            multiplot_builder.add_xlabel(builder.CreateString("year"));
            multiplot_builder.add_ylabel(builder.CreateString(""));
            stdvecMultiplot.push_back(multiplot_builder.Finish());
        }

        {
            Visualisation::MultiPlotBuilder multiplot_builder(builder);
            multiplot_builder.add_title(
                builder.CreateString("Death factor (by fitness)"));
            std::vector<flatbuffers::Offset<Visualisation::SinglePlot>>
                stdvecSingleplot;

            {
                // Using fitness_death_factor
                Visualisation::SinglePlotBuilder singleplot_builder(builder);
                singleplot_builder.add_title(
                    builder.CreateString("Death factor (by fitness)"));
                singleplot_builder.add_x(builder.CreateVector(time_vector));
                singleplot_builder.add_y(builder.CreateVector(get_value_vector(tmp_worlds, "fitness_death_factor")));
                stdvecSingleplot.push_back(singleplot_builder.Finish());
            }

            multiplot_builder.add_plots(builder.CreateVector(stdvecSingleplot));
            multiplot_builder.add_xlabel(builder.CreateString("year"));
            multiplot_builder.add_ylabel(builder.CreateString(""));
            stdvecMultiplot.push_back(multiplot_builder.Finish());
        }

        {
            Visualisation::MultiPlotBuilder multiplot_builder(builder);
            multiplot_builder.add_title(builder.CreateString("Age vs. Fitness"));
            std::vector<flatbuffers::Offset<Visualisation::SinglePlot>>
                stdvecSingleplot;

            {
                // Using age_fitness_on_death_ratio
                Visualisation::SinglePlotBuilder singleplot_builder(builder);
                singleplot_builder.add_title(
                    builder.CreateString("Age vs. Fitness"));
                singleplot_builder.add_x(builder.CreateVector(time_vector));
                singleplot_builder.add_y(builder.CreateVector(get_value_vector(tmp_worlds, "age_fitness_on_death_ratio")));
                stdvecSingleplot.push_back(singleplot_builder.Finish());
            }

            multiplot_builder.add_plots(builder.CreateVector(stdvecSingleplot));
            multiplot_builder.add_xlabel(builder.CreateString("year"));
            multiplot_builder.add_ylabel(builder.CreateString("ratio"));
            stdvecMultiplot.push_back(multiplot_builder.Finish());
        }

        {
            Visualisation::MultiPlotBuilder multiplot_builder(builder);
            multiplot_builder.add_title(
                builder.CreateString("Average Death Factor"));
            std::vector<flatbuffers::Offset<Visualisation::SinglePlot>>
                stdvecSingleplot;

            {
                // Using death_factor
                Visualisation::SinglePlotBuilder singleplot_builder(builder);
                singleplot_builder.add_title(
                    builder.CreateString("Average Death Factor"));
                singleplot_builder.add_x(builder.CreateVector(time_vector));
                singleplot_builder.add_y(builder.CreateVector(get_value_vector(tmp_worlds, "death_factor")));
                stdvecSingleplot.push_back(singleplot_builder.Finish());
            }

            multiplot_builder.add_plots(builder.CreateVector(stdvecSingleplot));
            multiplot_builder.add_xlabel(builder.CreateString("year"));
            multiplot_builder.add_ylabel(builder.CreateString(""));
            stdvecMultiplot.push_back(multiplot_builder.Finish());
        }

        {
            Visualisation::MultiPlotBuilder multiplot_builder(builder);
            multiplot_builder.add_title(builder.CreateString("Max age"));
            std::vector<flatbuffers::Offset<Visualisation::SinglePlot>>
                stdvecSingleplot;

            {
                Visualisation::SinglePlotBuilder singleplot_builder(builder);
                singleplot_builder.add_title(builder.CreateString("Max age"));
                singleplot_builder.add_x(builder.CreateVector(time_vector));
                singleplot_builder.add_y(builder.CreateVector(get_value_vector(tmp_worlds, "max_age")));
                stdvecSingleplot.push_back(singleplot_builder.Finish());
            }

            multiplot_builder.add_plots(builder.CreateVector(stdvecSingleplot));
            multiplot_builder.add_xlabel(builder.CreateString("year"));
            multiplot_builder.add_ylabel(builder.CreateString("age"));
            stdvecMultiplot.push_back(multiplot_builder.Finish());
        }

        // Demographic Graphs

        if (kind == "animal")
        {
            Visualisation::MultiPlotBuilder multiplot_builder(builder);
            multiplot_builder.add_title(builder.CreateString("Overall Population"));
            std::vector<flatbuffers::Offset<Visualisation::SinglePlot>>
                stdvecSingleplot;

            {
                // Using [Organism]
                Visualisation::SinglePlotBuilder singleplot_builder(builder);
                singleplot_builder.add_title(builder.CreateString("Male"));
                singleplot_builder.add_x(builder.CreateVector(time_vector));
                singleplot_builder.add_y(builder.CreateVector(get_value_vector(tmp_worlds, "male")));
                stdvecSingleplot.push_back(singleplot_builder.Finish());
            }

            {
                // Using [Organism]
                Visualisation::SinglePlotBuilder singleplot_builder(builder);
                singleplot_builder.add_title(builder.CreateString("Female"));
                singleplot_builder.add_x(builder.CreateVector(time_vector));
                singleplot_builder.add_y(builder.CreateVector(get_value_vector(tmp_worlds, "female")));
                stdvecSingleplot.push_back(singleplot_builder.Finish());
            }

            multiplot_builder.add_plots(builder.CreateVector(stdvecSingleplot));
            multiplot_builder.add_xlabel(builder.CreateString("year"));
            multiplot_builder.add_ylabel(builder.CreateString("population"));
            stdvecMultiplot.push_back(multiplot_builder.Finish());
        }
        else if (kind == "plant")
        {
            Visualisation::MultiPlotBuilder multiplot_builder(builder);
            multiplot_builder.add_title(builder.CreateString("Overall Population"));
            std::vector<flatbuffers::Offset<Visualisation::SinglePlot>>
                stdvecSingleplot;

            {
                // Using [Organism]
                Visualisation::SinglePlotBuilder singleplot_builder(builder);
                singleplot_builder.add_title(
                    builder.CreateString("Overall Population"));
                singleplot_builder.add_x(builder.CreateVector(time_vector));
                singleplot_builder.add_y(builder.CreateVector(get_value_vector(tmp_worlds, "population")));
                stdvecSingleplot.push_back(singleplot_builder.Finish());
            }

            multiplot_builder.add_plots(builder.CreateVector(stdvecSingleplot));
            multiplot_builder.add_xlabel(builder.CreateString("year"));
            multiplot_builder.add_ylabel(builder.CreateString("population"));
            stdvecMultiplot.push_back(multiplot_builder.Finish());
        }

        // Copulation Graphs

        if (kind == "animal")
        {
            Visualisation::MultiPlotBuilder multiplot_builder(builder);
            multiplot_builder.add_title(builder.CreateString("Matable Population"));
            std::vector<flatbuffers::Offset<Visualisation::SinglePlot>>
                stdvecSingleplot;

            {
                Visualisation::SinglePlotBuilder singleplot_builder(builder);
                singleplot_builder.add_title(builder.CreateString("Male"));
                singleplot_builder.add_x(builder.CreateVector(time_vector));
                singleplot_builder.add_y(builder.CreateVector(get_value_vector(tmp_worlds, "matable_male")));
                stdvecSingleplot.push_back(singleplot_builder.Finish());
            }

            {
                Visualisation::SinglePlotBuilder singleplot_builder(builder);
                singleplot_builder.add_title(builder.CreateString("Female"));
                singleplot_builder.add_x(builder.CreateVector(time_vector));
                singleplot_builder.add_y(builder.CreateVector(get_value_vector(tmp_worlds, "matable_female")));
                stdvecSingleplot.push_back(singleplot_builder.Finish());
            }

            multiplot_builder.add_plots(builder.CreateVector(stdvecSingleplot));
            multiplot_builder.add_xlabel(builder.CreateString("year"));
            multiplot_builder.add_ylabel(builder.CreateString("population"));
            stdvecMultiplot.push_back(multiplot_builder.Finish());
        }
        else if (kind == "plant")
        {
            Visualisation::MultiPlotBuilder multiplot_builder(builder);
            multiplot_builder.add_title(builder.CreateString("Matable Population"));
            std::vector<flatbuffers::Offset<Visualisation::SinglePlot>>
                stdvecSingleplot;

            {
                Visualisation::SinglePlotBuilder singleplot_builder(builder);
                singleplot_builder.add_title(
                    builder.CreateString("Matable Population"));
                singleplot_builder.add_x(builder.CreateVector(time_vector));
                singleplot_builder.add_y(builder.CreateVector(get_value_vector(tmp_worlds, "matable_population")));
                stdvecSingleplot.push_back(singleplot_builder.Finish());
            }

            multiplot_builder.add_plots(builder.CreateVector(stdvecSingleplot));
            multiplot_builder.add_xlabel(builder.CreateString("year"));
            multiplot_builder.add_ylabel(builder.CreateString("population"));
            stdvecMultiplot.push_back(multiplot_builder.Finish());
        }

        {
            Visualisation::MultiPlotBuilder multiplot_builder(builder);
            multiplot_builder.add_title(builder.CreateString("Mating age"));
            std::vector<flatbuffers::Offset<Visualisation::SinglePlot>>
                stdvecSingleplot;

            {
                // Using mating_age_start
                Visualisation::SinglePlotBuilder singleplot_builder(builder);
                singleplot_builder.add_title(builder.CreateString("Starting age"));
                singleplot_builder.add_x(builder.CreateVector(time_vector));
                singleplot_builder.add_y(builder.CreateVector(get_value_vector(tmp_worlds, "mating_age_start")));
                stdvecSingleplot.push_back(singleplot_builder.Finish());
            }

            {
                // Using mating_age_end
                Visualisation::SinglePlotBuilder singleplot_builder(builder);
                singleplot_builder.add_title(builder.CreateString("Ending age"));
                singleplot_builder.add_x(builder.CreateVector(time_vector));
                singleplot_builder.add_y(builder.CreateVector(get_value_vector(tmp_worlds, "mating_age_end")));
                stdvecSingleplot.push_back(singleplot_builder.Finish());
            }

            multiplot_builder.add_plots(builder.CreateVector(stdvecSingleplot));
            multiplot_builder.add_xlabel(builder.CreateString("year"));
            multiplot_builder.add_ylabel(builder.CreateString("age"));
            stdvecMultiplot.push_back(multiplot_builder.Finish());
        }

        {
            Visualisation::MultiPlotBuilder multiplot_builder(builder);
            multiplot_builder.add_title(
                builder.CreateString("Mutation Probability"));
            std::vector<flatbuffers::Offset<Visualisation::SinglePlot>>
                stdvecSingleplot;

            {
                // Using mutation_probability
                Visualisation::SinglePlotBuilder singleplot_builder(builder);
                singleplot_builder.add_title(
                    builder.CreateString("Mutation Probability"));
                singleplot_builder.add_x(builder.CreateVector(time_vector));
                singleplot_builder.add_y(builder.CreateVector(get_value_vector(tmp_worlds, "mutation_probability")));
                stdvecSingleplot.push_back(singleplot_builder.Finish());
            }

            multiplot_builder.add_plots(builder.CreateVector(stdvecSingleplot));
            multiplot_builder.add_xlabel(builder.CreateString("year"));
            multiplot_builder.add_ylabel(builder.CreateString(""));
            stdvecMultiplot.push_back(multiplot_builder.Finish());
        }

        {
            Visualisation::MultiPlotBuilder multiplot_builder(builder);
            multiplot_builder.add_title(
                builder.CreateString("Conceiving Probability"));
            std::vector<flatbuffers::Offset<Visualisation::SinglePlot>>
                stdvecSingleplot;

            {
                // Using conceiving_probability
                Visualisation::SinglePlotBuilder singleplot_builder(builder);
                singleplot_builder.add_title(
                    builder.CreateString("Conceiving Probability"));
                singleplot_builder.add_x(builder.CreateVector(time_vector));
                singleplot_builder.add_y(builder.CreateVector(get_value_vector(tmp_worlds, "conceiving_probability")));
                stdvecSingleplot.push_back(singleplot_builder.Finish());
            }

            multiplot_builder.add_plots(builder.CreateVector(stdvecSingleplot));
            multiplot_builder.add_xlabel(builder.CreateString("year"));
            multiplot_builder.add_ylabel(builder.CreateString(""));
            stdvecMultiplot.push_back(multiplot_builder.Finish());
        }

        {
            Visualisation::MultiPlotBuilder multiplot_builder(builder);
            multiplot_builder.add_title(builder.CreateString("Offspring Factor"));
            std::vector<flatbuffers::Offset<Visualisation::SinglePlot>>
                stdvecSingleplot;

            {
                // Using offsprings_factor
                Visualisation::SinglePlotBuilder singleplot_builder(builder);
                singleplot_builder.add_title(
                    builder.CreateString("Offspring Factor"));
                singleplot_builder.add_x(builder.CreateVector(time_vector));
                singleplot_builder.add_y(builder.CreateVector(get_value_vector(tmp_worlds, "offsprings_factor")));
                stdvecSingleplot.push_back(singleplot_builder.Finish());
            }

            multiplot_builder.add_plots(builder.CreateVector(stdvecSingleplot));
            multiplot_builder.add_xlabel(builder.CreateString("year"));
            multiplot_builder.add_ylabel(builder.CreateString(""));
            stdvecMultiplot.push_back(multiplot_builder.Finish());
        }

        // Dependency Graphs

        if (kind == "animal")
        {
            Visualisation::MultiPlotBuilder multiplot_builder(builder);
            multiplot_builder.add_title(
                builder.CreateString("Factors affecting Stamina"));
            std::vector<flatbuffers::Offset<Visualisation::SinglePlot>>
                stdvecSingleplot;

            {
                // Using height_on_stamina
                Visualisation::SinglePlotBuilder singleplot_builder(builder);
                singleplot_builder.add_title(
                    builder.CreateString("Height dependence"));
                singleplot_builder.add_x(builder.CreateVector(time_vector));
                singleplot_builder.add_y(builder.CreateVector(get_value_vector(tmp_worlds, "height_on_stamina")));
                stdvecSingleplot.push_back(singleplot_builder.Finish());
            }

            {
                // Using weight_on_stamina
                Visualisation::SinglePlotBuilder singleplot_builder(builder);
                singleplot_builder.add_title(
                    builder.CreateString("Weight dependence"));
                singleplot_builder.add_x(builder.CreateVector(time_vector));
                singleplot_builder.add_y(builder.CreateVector(get_value_vector(tmp_worlds, "weight_on_stamina")));
                stdvecSingleplot.push_back(singleplot_builder.Finish());
            }

            multiplot_builder.add_plots(builder.CreateVector(stdvecSingleplot));
            multiplot_builder.add_xlabel(builder.CreateString("year"));
            multiplot_builder.add_ylabel(builder.CreateString(""));
            stdvecMultiplot.push_back(multiplot_builder.Finish());
        }

        {
            Visualisation::MultiPlotBuilder multiplot_builder(builder);
            multiplot_builder.add_title(
                builder.CreateString("Factors affecting Vitality"));
            std::vector<flatbuffers::Offset<Visualisation::SinglePlot>>
                stdvecSingleplot;

            {
                // Using height_on_vitality
                Visualisation::SinglePlotBuilder singleplot_builder(builder);
                singleplot_builder.add_title(
                    builder.CreateString("Height dependence"));
                singleplot_builder.add_x(builder.CreateVector(time_vector));
                singleplot_builder.add_y(builder.CreateVector(get_value_vector(tmp_worlds, "height_on_vitality")));
                stdvecSingleplot.push_back(singleplot_builder.Finish());
            }

            {
                // Using weight_on_vitality
                Visualisation::SinglePlotBuilder singleplot_builder(builder);
                singleplot_builder.add_title(
                    builder.CreateString("Weight dependence"));
                singleplot_builder.add_x(builder.CreateVector(time_vector));
                singleplot_builder.add_y(builder.CreateVector(get_value_vector(tmp_worlds, "weight_on_vitality")));
                stdvecSingleplot.push_back(singleplot_builder.Finish());
            }

            multiplot_builder.add_plots(builder.CreateVector(stdvecSingleplot));
            multiplot_builder.add_xlabel(builder.CreateString("year"));
            multiplot_builder.add_ylabel(builder.CreateString(""));
            stdvecMultiplot.push_back(multiplot_builder.Finish());
        }

        if (kind == "animal")
        {
            Visualisation::MultiPlotBuilder multiplot_builder(builder);
            multiplot_builder.add_title(
                builder.CreateString("Factors affecting Speed"));
            std::vector<flatbuffers::Offset<Visualisation::SinglePlot>>
                stdvecSingleplot;

            {
                // Using height_on_speed
                Visualisation::SinglePlotBuilder singleplot_builder(builder);
                singleplot_builder.add_title(
                    builder.CreateString("Height dependence"));
                singleplot_builder.add_x(builder.CreateVector(time_vector));
                singleplot_builder.add_y(builder.CreateVector(get_value_vector(tmp_worlds, "height_on_speed")));
                stdvecSingleplot.push_back(singleplot_builder.Finish());
            }

            {
                // Using weight_on_speed
                Visualisation::SinglePlotBuilder singleplot_builder(builder);
                singleplot_builder.add_title(
                    builder.CreateString("Weight dependence"));
                singleplot_builder.add_x(builder.CreateVector(time_vector));
                singleplot_builder.add_y(builder.CreateVector(get_value_vector(tmp_worlds, "weight_on_speed")));
                stdvecSingleplot.push_back(singleplot_builder.Finish());
            }

            {
                // Using stamina_on_speed
                Visualisation::SinglePlotBuilder singleplot_builder(builder);
                singleplot_builder.add_title(
                    builder.CreateString("Stamina dependence"));
                singleplot_builder.add_x(builder.CreateVector(time_vector));
                singleplot_builder.add_y(builder.CreateVector(get_value_vector(tmp_worlds, "stamina_on_speed")));
                stdvecSingleplot.push_back(singleplot_builder.Finish());
            }

            {
                // Using vitality_on_speed
                Visualisation::SinglePlotBuilder singleplot_builder(builder);
                singleplot_builder.add_title(
                    builder.CreateString("Vitality dependence"));
                singleplot_builder.add_x(builder.CreateVector(time_vector));
                singleplot_builder.add_y(builder.CreateVector(get_value_vector(tmp_worlds, "vitality_on_speed")));
                stdvecSingleplot.push_back(singleplot_builder.Finish());
            }

            multiplot_builder.add_plots(builder.CreateVector(stdvecSingleplot));
            multiplot_builder.add_xlabel(builder.CreateString("year"));
            multiplot_builder.add_ylabel(builder.CreateString(""));
            stdvecMultiplot.push_back(multiplot_builder.Finish());
        }

        if (kind == "animal")
        {
            Visualisation::MultiPlotBuilder multiplot_builder(builder);
            multiplot_builder.add_title(
                builder.CreateString("Factors affecting Appetite"));
            std::vector<flatbuffers::Offset<Visualisation::SinglePlot>>
                stdvecSingleplot;

            {
                // Using stamina_on_appetite
                Visualisation::SinglePlotBuilder singleplot_builder(builder);
                singleplot_builder.add_title(
                    builder.CreateString("Stamina dependence"));
                singleplot_builder.add_x(builder.CreateVector(time_vector));
                singleplot_builder.add_y(builder.CreateVector(get_value_vector(tmp_worlds, "stamina_on_appetite")));
                stdvecSingleplot.push_back(singleplot_builder.Finish());
            }

            {
                // Using vitality_on_appetite
                Visualisation::SinglePlotBuilder singleplot_builder(builder);
                singleplot_builder.add_title(
                    builder.CreateString("Vitality dependence"));
                singleplot_builder.add_x(builder.CreateVector(time_vector));
                singleplot_builder.add_y(builder.CreateVector(get_value_vector(tmp_worlds, "vitality_on_appetite")));
                stdvecSingleplot.push_back(singleplot_builder.Finish());
            }

            multiplot_builder.add_plots(builder.CreateVector(stdvecSingleplot));
            multiplot_builder.add_xlabel(builder.CreateString("year"));
            multiplot_builder.add_ylabel(builder.CreateString(""));
            stdvecMultiplot.push_back(multiplot_builder.Finish());
        }

        // Average graphs

        {
            Visualisation::MultiPlotBuilder multiplot_builder(builder);
            multiplot_builder.add_title(builder.CreateString("Average Generation"));
            std::vector<flatbuffers::Offset<Visualisation::SinglePlot>>
                stdvecSingleplot;

            {
                // Using generation
                Visualisation::SinglePlotBuilder singleplot_builder(builder);
                singleplot_builder.add_title(
                    builder.CreateString("Average Generation"));
                singleplot_builder.add_x(builder.CreateVector(time_vector));
                singleplot_builder.add_y(builder.CreateVector(get_value_vector(tmp_worlds, "generation")));
                stdvecSingleplot.push_back(singleplot_builder.Finish());
            }

            multiplot_builder.add_plots(builder.CreateVector(stdvecSingleplot));
            multiplot_builder.add_xlabel(builder.CreateString("year"));
            multiplot_builder.add_ylabel(builder.CreateString("generation"));
            stdvecMultiplot.push_back(multiplot_builder.Finish());
        }

        {
            Visualisation::MultiPlotBuilder multiplot_builder(builder);
            multiplot_builder.add_title(builder.CreateString("Average Age"));
            std::vector<flatbuffers::Offset<Visualisation::SinglePlot>>
                stdvecSingleplot;

            {
                // Using age
                Visualisation::SinglePlotBuilder singleplot_builder(builder);
                singleplot_builder.add_title(builder.CreateString("Average Age"));
                singleplot_builder.add_x(builder.CreateVector(time_vector));
                singleplot_builder.add_y(builder.CreateVector(get_value_vector(tmp_worlds, "age")));
                stdvecSingleplot.push_back(singleplot_builder.Finish());
            }

            multiplot_builder.add_plots(builder.CreateVector(stdvecSingleplot));
            multiplot_builder.add_xlabel(builder.CreateString("year"));
            multiplot_builder.add_ylabel(builder.CreateString("age"));
            stdvecMultiplot.push_back(multiplot_builder.Finish());
        }

        {
            Visualisation::MultiPlotBuilder multiplot_builder(builder);
            multiplot_builder.add_title(builder.CreateString("Average Height"));
            std::vector<flatbuffers::Offset<Visualisation::SinglePlot>>
                stdvecSingleplot;

            {
                // Using height
                Visualisation::SinglePlotBuilder singleplot_builder(builder);
                singleplot_builder.add_title(
                    builder.CreateString("Average Height"));
                singleplot_builder.add_x(builder.CreateVector(time_vector));
                singleplot_builder.add_y(builder.CreateVector(get_value_vector(tmp_worlds, "height")));
                stdvecSingleplot.push_back(singleplot_builder.Finish());
            }

            multiplot_builder.add_plots(builder.CreateVector(stdvecSingleplot));
            multiplot_builder.add_xlabel(builder.CreateString("year"));
            multiplot_builder.add_ylabel(builder.CreateString("height"));
            stdvecMultiplot.push_back(multiplot_builder.Finish());
        }

        {
            Visualisation::MultiPlotBuilder multiplot_builder(builder);
            multiplot_builder.add_title(builder.CreateString("Average Weight"));
            std::vector<flatbuffers::Offset<Visualisation::SinglePlot>>
                stdvecSingleplot;

            {
                // Using weight
                Visualisation::SinglePlotBuilder singleplot_builder(builder);
                singleplot_builder.add_title(
                    builder.CreateString("Average Weight"));
                singleplot_builder.add_x(builder.CreateVector(time_vector));
                singleplot_builder.add_y(builder.CreateVector(get_value_vector(tmp_worlds, "weight")));
                stdvecSingleplot.push_back(singleplot_builder.Finish());
            }

            multiplot_builder.add_plots(builder.CreateVector(stdvecSingleplot));
            multiplot_builder.add_xlabel(builder.CreateString("year"));
            multiplot_builder.add_ylabel(builder.CreateString("weight"));
            stdvecMultiplot.push_back(multiplot_builder.Finish());
        }

        {
            Visualisation::MultiPlotBuilder multiplot_builder(builder);
            multiplot_builder.add_title(builder.CreateString("Average Fitness"));
            std::vector<flatbuffers::Offset<Visualisation::SinglePlot>>
                stdvecSingleplot;

            {
                // Using static_fitness
                Visualisation::SinglePlotBuilder singleplot_builder(builder);
                singleplot_builder.add_title(
                    builder.CreateString("Average Fitness"));
                singleplot_builder.add_x(builder.CreateVector(time_vector));
                singleplot_builder.add_y(builder.CreateVector(get_value_vector(tmp_worlds, "static_fitness")));
                stdvecSingleplot.push_back(singleplot_builder.Finish());
            }

            multiplot_builder.add_plots(builder.CreateVector(stdvecSingleplot));
            multiplot_builder.add_xlabel(builder.CreateString("year"));
            multiplot_builder.add_ylabel(builder.CreateString(""));
            stdvecMultiplot.push_back(multiplot_builder.Finish());
        }

        {
            Visualisation::MultiPlotBuilder multiplot_builder(builder);
            multiplot_builder.add_title(builder.CreateString("Average Immunity"));
            std::vector<flatbuffers::Offset<Visualisation::SinglePlot>>
                stdvecSingleplot;

            {
                // Using immunity
                Visualisation::SinglePlotBuilder singleplot_builder(builder);
                singleplot_builder.add_title(
                    builder.CreateString("Average Immunity"));
                singleplot_builder.add_x(builder.CreateVector(time_vector));
                singleplot_builder.add_y(builder.CreateVector(get_value_vector(tmp_worlds, "immunity")));
                stdvecSingleplot.push_back(singleplot_builder.Finish());
            }

            multiplot_builder.add_plots(builder.CreateVector(stdvecSingleplot));
            multiplot_builder.add_xlabel(builder.CreateString("year"));
            multiplot_builder.add_ylabel(builder.CreateString(""));
            stdvecMultiplot.push_back(multiplot_builder.Finish());
        }

        if (kind == "animal")
        {
            Visualisation::MultiPlotBuilder multiplot_builder(builder);
            multiplot_builder.add_title(
                builder.CreateString("Average current max Speed"));
            std::vector<flatbuffers::Offset<Visualisation::SinglePlot>>
                stdvecSingleplot;

            {
                // Using max_speed_at_age
                Visualisation::SinglePlotBuilder singleplot_builder(builder);
                singleplot_builder.add_title(
                    builder.CreateString("Average current max Speed"));
                singleplot_builder.add_x(builder.CreateVector(time_vector));
                singleplot_builder.add_y(builder.CreateVector(get_value_vector(tmp_worlds, "max_speed_at_age")));
                stdvecSingleplot.push_back(singleplot_builder.Finish());
            }

            multiplot_builder.add_plots(builder.CreateVector(stdvecSingleplot));
            multiplot_builder.add_xlabel(builder.CreateString("year"));
            multiplot_builder.add_ylabel(builder.CreateString("speed"));
            stdvecMultiplot.push_back(multiplot_builder.Finish());
        }

        if (kind == "animal")
        {
            Visualisation::MultiPlotBuilder multiplot_builder(builder);
            multiplot_builder.add_title(
                builder.CreateString("Average current max Appetite"));
            std::vector<flatbuffers::Offset<Visualisation::SinglePlot>>
                stdvecSingleplot;

            {
                // Using max_appetite_at_age
                Visualisation::SinglePlotBuilder singleplot_builder(builder);
                singleplot_builder.add_title(
                    builder.CreateString("Average current max Appetite"));
                singleplot_builder.add_x(builder.CreateVector(time_vector));
                singleplot_builder.add_y(builder.CreateVector(get_value_vector(tmp_worlds, "max_appetite_at_age")));
                stdvecSingleplot.push_back(singleplot_builder.Finish());
            }

            multiplot_builder.add_plots(builder.CreateVector(stdvecSingleplot));
            multiplot_builder.add_xlabel(builder.CreateString("year"));
            multiplot_builder.add_ylabel(builder.CreateString("appetite"));
            stdvecMultiplot.push_back(multiplot_builder.Finish());
        }

        if (kind == "animal")
        {
            Visualisation::MultiPlotBuilder multiplot_builder(builder);
            multiplot_builder.add_title(
                builder.CreateString("Average current max Stamina"));
            std::vector<flatbuffers::Offset<Visualisation::SinglePlot>>
                stdvecSingleplot;

            {
                // Using max_stamina_at_age
                Visualisation::SinglePlotBuilder singleplot_builder(builder);
                singleplot_builder.add_title(
                    builder.CreateString("Average current max Stamina"));
                singleplot_builder.add_x(builder.CreateVector(time_vector));
                singleplot_builder.add_y(builder.CreateVector(get_value_vector(tmp_worlds, "max_stamina_at_age")));
                stdvecSingleplot.push_back(singleplot_builder.Finish());
            }

            multiplot_builder.add_plots(builder.CreateVector(stdvecSingleplot));
            multiplot_builder.add_xlabel(builder.CreateString("year"));
            multiplot_builder.add_ylabel(builder.CreateString("stamina"));
            stdvecMultiplot.push_back(multiplot_builder.Finish());
        }

        {
            Visualisation::MultiPlotBuilder multiplot_builder(builder);
            multiplot_builder.add_title(
                builder.CreateString("Average current max Vitality"));
            std::vector<flatbuffers::Offset<Visualisation::SinglePlot>>
                stdvecSingleplot;

            {
                // Using max_vitality_at_age
                Visualisation::SinglePlotBuilder singleplot_builder(builder);
                singleplot_builder.add_title(
                    builder.CreateString("Average current max Vitality"));
                singleplot_builder.add_x(builder.CreateVector(time_vector));
                singleplot_builder.add_y(builder.CreateVector(get_value_vector(tmp_worlds, "max_vitality_at_age")));
                stdvecSingleplot.push_back(singleplot_builder.Finish());
            }

            multiplot_builder.add_plots(builder.CreateVector(stdvecSingleplot));
            multiplot_builder.add_xlabel(builder.CreateString("year"));
            multiplot_builder.add_ylabel(builder.CreateString("vitality"));
            stdvecMultiplot.push_back(multiplot_builder.Finish());
        }

        if (kind == "animal")
        {
            Visualisation::MultiPlotBuilder multiplot_builder(builder);
            multiplot_builder.add_title(
                builder.CreateString("Average Vision Radius"));
            std::vector<flatbuffers::Offset<Visualisation::SinglePlot>>
                stdvecSingleplot;

            {
                // Using vision_radius
                Visualisation::SinglePlotBuilder singleplot_builder(builder);
                singleplot_builder.add_title(
                    builder.CreateString("Average Vision Radius"));
                singleplot_builder.add_x(builder.CreateVector(time_vector));
                singleplot_builder.add_y(builder.CreateVector(get_value_vector(tmp_worlds, "vision_radius")));
                stdvecSingleplot.push_back(singleplot_builder.Finish());
            }

            multiplot_builder.add_plots(builder.CreateVector(stdvecSingleplot));
            multiplot_builder.add_xlabel(builder.CreateString("year"));
            multiplot_builder.add_ylabel(builder.CreateString("distance"));
            stdvecMultiplot.push_back(multiplot_builder.Finish());
        }

        // Theoretical Graphs

        {
            Visualisation::MultiPlotBuilder multiplot_builder(builder);
            multiplot_builder.add_title(
                builder.CreateString("Theoretical Maximum Height"));
            std::vector<flatbuffers::Offset<Visualisation::SinglePlot>>
                stdvecSingleplot;

            {
                // Using theoretical_maximum_base_height
                Visualisation::SinglePlotBuilder singleplot_builder(builder);
                singleplot_builder.add_title(builder.CreateString("Base Height"));
                singleplot_builder.add_x(builder.CreateVector(time_vector));
                singleplot_builder.add_y(builder.CreateVector(get_value_vector(tmp_worlds, "theoretical_maximum_base_height")));
                stdvecSingleplot.push_back(singleplot_builder.Finish());
            }

            {
                // Using theoretical_maximum_height
                Visualisation::SinglePlotBuilder singleplot_builder(builder);
                singleplot_builder.add_title(
                    builder.CreateString("Attainable Height"));
                singleplot_builder.add_x(builder.CreateVector(time_vector));
                singleplot_builder.add_y(builder.CreateVector(get_value_vector(tmp_worlds, "theoretical_maximum_height")));
                stdvecSingleplot.push_back(singleplot_builder.Finish());
            }

            multiplot_builder.add_plots(builder.CreateVector(stdvecSingleplot));
            multiplot_builder.add_xlabel(builder.CreateString("year"));
            multiplot_builder.add_ylabel(builder.CreateString("height"));
            stdvecMultiplot.push_back(multiplot_builder.Finish());
        }

        {
            Visualisation::MultiPlotBuilder multiplot_builder(builder);
            multiplot_builder.add_title(
                builder.CreateString("Theoretical Maximum Weight"));
            std::vector<flatbuffers::Offset<Visualisation::SinglePlot>>
                stdvecSingleplot;

            {
                // Using theoretical_maximum_base_weight
                Visualisation::SinglePlotBuilder singleplot_builder(builder);
                singleplot_builder.add_title(builder.CreateString("Base Weight"));
                singleplot_builder.add_x(builder.CreateVector(time_vector));
                singleplot_builder.add_y(builder.CreateVector(get_value_vector(tmp_worlds, "theoretical_maximum_base_weight")));
                stdvecSingleplot.push_back(singleplot_builder.Finish());
            }

            {
                // Using theoretical_maximum_weight
                Visualisation::SinglePlotBuilder singleplot_builder(builder);
                singleplot_builder.add_title(
                    builder.CreateString("Attainable Weight"));
                singleplot_builder.add_x(builder.CreateVector(time_vector));
                singleplot_builder.add_y(builder.CreateVector(get_value_vector(tmp_worlds, "theoretical_maximum_weight")));
                stdvecSingleplot.push_back(singleplot_builder.Finish());
            }

            multiplot_builder.add_plots(builder.CreateVector(stdvecSingleplot));
            multiplot_builder.add_xlabel(builder.CreateString("year"));
            multiplot_builder.add_ylabel(builder.CreateString("weight"));
            stdvecMultiplot.push_back(multiplot_builder.Finish());
        }

        if (kind == "animal")
        {
            Visualisation::MultiPlotBuilder multiplot_builder(builder);
            multiplot_builder.add_title(
                builder.CreateString("Theoretical Maximum Speed"));
            std::vector<flatbuffers::Offset<Visualisation::SinglePlot>>
                stdvecSingleplot;

            {
                // Using theoretical_maximum_base_speed
                Visualisation::SinglePlotBuilder singleplot_builder(builder);
                singleplot_builder.add_title(builder.CreateString("Base Speed"));
                singleplot_builder.add_x(builder.CreateVector(time_vector));
                singleplot_builder.add_y(builder.CreateVector(get_value_vector(tmp_worlds, "theoretical_maximum_base_speed")));
                stdvecSingleplot.push_back(singleplot_builder.Finish());
            }

            {
                // Using theoretical_maximum_speed
                Visualisation::SinglePlotBuilder singleplot_builder(builder);
                singleplot_builder.add_title(
                    builder.CreateString("Attainable Speed"));
                singleplot_builder.add_x(builder.CreateVector(time_vector));
                singleplot_builder.add_y(builder.CreateVector(get_value_vector(tmp_worlds, "theoretical_maximum_speed")));
                stdvecSingleplot.push_back(singleplot_builder.Finish());
            }

            multiplot_builder.add_plots(builder.CreateVector(stdvecSingleplot));
            multiplot_builder.add_xlabel(builder.CreateString("year"));
            multiplot_builder.add_ylabel(builder.CreateString("speed"));
            stdvecMultiplot.push_back(multiplot_builder.Finish());
        }

        if (kind == "animal")
        {
            Visualisation::MultiPlotBuilder multiplot_builder(builder);
            multiplot_builder.add_title(
                builder.CreateString("Theoretical Maximum Base Appetite"));
            std::vector<flatbuffers::Offset<Visualisation::SinglePlot>>
                stdvecSingleplot;

            {
                // Using theoretical_maximum_base_appetite
                Visualisation::SinglePlotBuilder singleplot_builder(builder);
                singleplot_builder.add_title(
                    builder.CreateString("Theoretical Maximum Base Appetite"));
                singleplot_builder.add_x(builder.CreateVector(time_vector));
                singleplot_builder.add_y(builder.CreateVector(get_value_vector(tmp_worlds, "theoretical_maximum_base_appetite")));
                stdvecSingleplot.push_back(singleplot_builder.Finish());
            }

            multiplot_builder.add_plots(builder.CreateVector(stdvecSingleplot));
            multiplot_builder.add_xlabel(builder.CreateString("year"));
            multiplot_builder.add_ylabel(builder.CreateString("appetite"));
            stdvecMultiplot.push_back(multiplot_builder.Finish());
        }

        if (kind == "animal")
        {
            Visualisation::MultiPlotBuilder multiplot_builder(builder);
            multiplot_builder.add_title(
                builder.CreateString("Theoretical Maximum Base Stamina"));
            std::vector<flatbuffers::Offset<Visualisation::SinglePlot>>
                stdvecSingleplot;

            {
                // Using theoretical_maximum_base_stamina
                Visualisation::SinglePlotBuilder singleplot_builder(builder);
                singleplot_builder.add_title(
                    builder.CreateString("Theoretical Maximum Base Stamina"));
                singleplot_builder.add_x(builder.CreateVector(time_vector));
                singleplot_builder.add_y(builder.CreateVector(get_value_vector(tmp_worlds, "theoretical_maximum_base_stamina")));
                stdvecSingleplot.push_back(singleplot_builder.Finish());
            }

            multiplot_builder.add_plots(builder.CreateVector(stdvecSingleplot));
            multiplot_builder.add_xlabel(builder.CreateString("year"));
            multiplot_builder.add_ylabel(builder.CreateString("stamina"));
            stdvecMultiplot.push_back(multiplot_builder.Finish());
        }

        {
            Visualisation::MultiPlotBuilder multiplot_builder(builder);
            multiplot_builder.add_title(
                builder.CreateString("Theoretical Maximum Base Vitality"));
            std::vector<flatbuffers::Offset<Visualisation::SinglePlot>>
                stdvecSingleplot;

            {
                // Using theoretical_maximum_base_vitality
                Visualisation::SinglePlotBuilder singleplot_builder(builder);
                singleplot_builder.add_title(
                    builder.CreateString("Theoretical Maximum Base Vitality"));
                singleplot_builder.add_x(builder.CreateVector(time_vector));
                singleplot_builder.add_y(builder.CreateVector(get_value_vector(tmp_worlds, "theoretical_maximum_base_vitality")));
                stdvecSingleplot.push_back(singleplot_builder.Finish());
            }

            multiplot_builder.add_plots(builder.CreateVector(stdvecSingleplot));
            multiplot_builder.add_xlabel(builder.CreateString("year"));
            multiplot_builder.add_ylabel(builder.CreateString("vitality"));
            stdvecMultiplot.push_back(multiplot_builder.Finish());
        }

        if (kind == "animal")
        {
            Visualisation::MultiPlotBuilder multiplot_builder(builder);
            multiplot_builder.add_title(
                builder.CreateString("Theoretical Maximum Speed Multiplier"));
            std::vector<flatbuffers::Offset<Visualisation::SinglePlot>>
                stdvecSingleplot;

            {
                // Using theoretical_maximum_speed_multiplier
                Visualisation::SinglePlotBuilder singleplot_builder(builder);
                singleplot_builder.add_title(
                    builder.CreateString("Theoretical Maximum Speed Multiplier"));
                singleplot_builder.add_x(builder.CreateVector(time_vector));
                singleplot_builder.add_y(builder.CreateVector(get_value_vector(tmp_worlds, "theoretical_maximum_speed_multiplier")));
                stdvecSingleplot.push_back(singleplot_builder.Finish());
            }

            multiplot_builder.add_plots(builder.CreateVector(stdvecSingleplot));
            multiplot_builder.add_xlabel(builder.CreateString("year"));
            multiplot_builder.add_ylabel(builder.CreateString(""));
            stdvecMultiplot.push_back(multiplot_builder.Finish());
        }

        if (kind == "animal")
        {
            Visualisation::MultiPlotBuilder multiplot_builder(builder);
            multiplot_builder.add_title(
                builder.CreateString("Theoretical Maximum Stamina Multiplier"));
            std::vector<flatbuffers::Offset<Visualisation::SinglePlot>>
                stdvecSingleplot;

            {
                // Using theoretical_maximum_stamina_multiplier
                Visualisation::SinglePlotBuilder singleplot_builder(builder);
                singleplot_builder.add_title(
                    builder.CreateString("Theoretical Maximum Stamina Multiplier"));
                singleplot_builder.add_x(builder.CreateVector(time_vector));
                singleplot_builder.add_y(builder.CreateVector(get_value_vector(tmp_worlds, "theoretical_maximum_stamina_multiplier")));
                stdvecSingleplot.push_back(singleplot_builder.Finish());
            }

            multiplot_builder.add_plots(builder.CreateVector(stdvecSingleplot));
            multiplot_builder.add_xlabel(builder.CreateString("year"));
            multiplot_builder.add_ylabel(builder.CreateString(""));
            stdvecMultiplot.push_back(multiplot_builder.Finish());
        }

        {
            Visualisation::MultiPlotBuilder multiplot_builder(builder);
            multiplot_builder.add_title(
                builder.CreateString("Theoretical Maximum Vitality Multiplier"));
            std::vector<flatbuffers::Offset<Visualisation::SinglePlot>>
                stdvecSingleplot;

            {
                // Using theoretical_maximum_vitality_multiplier
                Visualisation::SinglePlotBuilder singleplot_builder(builder);
                singleplot_builder.add_title(builder.CreateString(
                    "Theoretical Maximum Vitality Multiplier"));
                singleplot_builder.add_x(builder.CreateVector(time_vector));
                singleplot_builder.add_y(builder.CreateVector(get_value_vector(tmp_worlds, "theoretical_maximum_vitality_multiplier")));
                stdvecSingleplot.push_back(singleplot_builder.Finish());
            }

            multiplot_builder.add_plots(builder.CreateVector(stdvecSingleplot));
            multiplot_builder.add_xlabel(builder.CreateString("year"));
            multiplot_builder.add_ylabel(builder.CreateString(""));
            stdvecMultiplot.push_back(multiplot_builder.Finish());
        }

        {
            Visualisation::MultiPlotBuilder multiplot_builder(builder);
            multiplot_builder.add_title(
                builder.CreateString("Theoretical Maximum Height Multiplier"));
            std::vector<flatbuffers::Offset<Visualisation::SinglePlot>>
                stdvecSingleplot;

            {
                // Using theoretical_maximum_height_multiplier
                Visualisation::SinglePlotBuilder singleplot_builder(builder);
                singleplot_builder.add_title(
                    builder.CreateString("Theoretical Maximum Height Multiplier"));
                singleplot_builder.add_x(builder.CreateVector(time_vector));
                singleplot_builder.add_y(builder.CreateVector(get_value_vector(tmp_worlds, "theoretical_maximum_height_multiplier")));
                stdvecSingleplot.push_back(singleplot_builder.Finish());
            }

            multiplot_builder.add_plots(builder.CreateVector(stdvecSingleplot));
            multiplot_builder.add_xlabel(builder.CreateString("year"));
            multiplot_builder.add_ylabel(builder.CreateString(""));
            stdvecMultiplot.push_back(multiplot_builder.Finish());
        }

        {
            Visualisation::MultiPlotBuilder multiplot_builder(builder);
            multiplot_builder.add_title(
                builder.CreateString("Theoretical Maximum Weight Multiplier"));
            std::vector<flatbuffers::Offset<Visualisation::SinglePlot>>
                stdvecSingleplot;

            {
                // Using theoretical_maximum_weight_multiplier
                Visualisation::SinglePlotBuilder singleplot_builder(builder);
                singleplot_builder.add_title(
                    builder.CreateString("Theoretical Maximum Weight Multiplier"));
                singleplot_builder.add_x(builder.CreateVector(time_vector));
                singleplot_builder.add_y(builder.CreateVector(get_value_vector(tmp_worlds, "theoretical_maximum_weight_multiplier")));
                stdvecSingleplot.push_back(singleplot_builder.Finish());
            }

            multiplot_builder.add_plots(builder.CreateVector(stdvecSingleplot));
            multiplot_builder.add_xlabel(builder.CreateString("year"));
            multiplot_builder.add_ylabel(builder.CreateString(""));
            stdvecMultiplot.push_back(multiplot_builder.Finish());
        }

        report_builder.add_title(builder.CreateString(title.c_str()));
        report_builder.add_species(builder.CreateString(kind.c_str()));
        report_builder.add_timestamp(
            builder.CreateString(std::to_string(std::time(nullptr)).c_str()));
        report_builder.add_plots(builder.CreateVectorOfSortedTables(
            stdvecMultiplot.data(), stdvecMultiplot.size()));
        builder.Finish(report_builder.Finish());

        auto report_buffer = builder.Release();
        builder.Clear();

        return report_buffer;
    }

    FBuffer create_avg_world(const flatbuffers::DetachedBuffer &world_buffer)
    {

        const Ecosystem::World *world_pointer = Ecosystem::GetWorld(world_buffer.data());

        flatbuffers::FlatBufferBuilder new_builder;
        Ecosystem::WorldBuilder new_world_builder(new_builder);

        std::vector<flatbuffers::Offset<Ecosystem::Species>> new_stdvecSpecies;
        Ecosystem::SpeciesBuilder new_species_builder(new_builder);

        // Do avg stuff here

        for (const Ecosystem::Species *species : *(world_pointer->species()))
        {
            std::vector<flatbuffers::Offset<Ecosystem::Organism>> new_stdvecOrganisms;
            Ecosystem::OrganismBuilder new_organism_builder(new_builder);

            std::unordered_map<std::string, float> org_map;

            bool common_values_set = false;
            int k = 0;

            for (const Ecosystem::Organism *organism : *(species->organism()))
            {
                if (!common_values_set)
                {
                    new_organism_builder.add_chromosome_number(organism->chromosome_number());
                    new_organism_builder.add_is_asexual(organism->is_asexual());
                    new_organism_builder.add_food_chain_rank(organism->food_chain_rank());
                    new_organism_builder.add_kind(new_builder.CreateString(organism->kind()));
                    new_organism_builder.add_kingdom(organism->kingdom());
                    new_organism_builder.add_name(new_builder.CreateString(organism->kind()->str() + "_avg_organism"));
                    // new_organism_builder.add_name(new_builder.CreateVector(chromosome_vec.data(), chromosome_vec.size()));

                    common_values_set = true;
                }

                {
                    float &prev_avg = org_map["offsprings_factor"];
                    prev_avg = prev_avg + (organism->offsprings_factor() - prev_avg) / (k + 1);
                }
                {
                    float &prev_avg = org_map["mutation_probability"];
                    prev_avg = prev_avg + (organism->mutation_probability() - prev_avg) / (k + 1);
                }
                {
                    float &prev_avg = org_map["conceiving_probability"];
                    prev_avg = prev_avg + (organism->conceiving_probability() - prev_avg) / (k + 1);
                }
                {
                    float &prev_avg = org_map["mating_probability"];
                    prev_avg = prev_avg + (organism->mating_probability() - prev_avg) / (k + 1);
                }
                {
                    float &prev_avg = org_map["age_fitness_on_death_ratio"];
                    prev_avg = prev_avg + (organism->age_fitness_on_death_ratio() - prev_avg) / (k + 1);
                }
                {
                    float &prev_avg = org_map["height_on_speed"];
                    prev_avg = prev_avg + (organism->height_on_speed() - prev_avg) / (k + 1);
                }
                {
                    float &prev_avg = org_map["height_on_stamina"];
                    prev_avg = prev_avg + (organism->height_on_stamina() - prev_avg) / (k + 1);
                }
                {
                    float &prev_avg = org_map["height_on_vitality"];
                    prev_avg = prev_avg + (organism->height_on_vitality() - prev_avg) / (k + 1);
                }
                {
                    float &prev_avg = org_map["weight_on_speed"];
                    prev_avg = prev_avg + (organism->weight_on_speed() - prev_avg) / (k + 1);
                }
                {
                    float &prev_avg = org_map["weight_on_stamina"];
                    prev_avg = prev_avg + (organism->weight_on_stamina() - prev_avg) / (k + 1);
                }
                {
                    float &prev_avg = org_map["weight_on_vitality"];
                    prev_avg = prev_avg + (organism->weight_on_vitality() - prev_avg) / (k + 1);
                }
                {
                    float &prev_avg = org_map["vitality_on_appetite"];
                    prev_avg = prev_avg + (organism->vitality_on_appetite() - prev_avg) / (k + 1);
                }
                {
                    float &prev_avg = org_map["vitality_on_speed"];
                    prev_avg = prev_avg + (organism->vitality_on_speed() - prev_avg) / (k + 1);
                }
                {
                    float &prev_avg = org_map["stamina_on_appetite"];
                    prev_avg = prev_avg + (organism->stamina_on_appetite() - prev_avg) / (k + 1);
                }
                {
                    float &prev_avg = org_map["stamina_on_speed"];
                    prev_avg = prev_avg + (organism->stamina_on_speed() - prev_avg) / (k + 1);
                }
                {
                    float &prev_avg = org_map["theoretical_maximum_base_appetite"];
                    prev_avg = prev_avg + (organism->theoretical_maximum_base_appetite() - prev_avg) / (k + 1);
                }
                {
                    float &prev_avg = org_map["theoretical_maximum_base_height"];
                    prev_avg = prev_avg + (organism->theoretical_maximum_base_height() - prev_avg) / (k + 1);
                }
                {
                    float &prev_avg = org_map["theoretical_maximum_base_speed"];
                    prev_avg = prev_avg + (organism->theoretical_maximum_base_speed() - prev_avg) / (k + 1);
                }
                {
                    float &prev_avg = org_map["theoretical_maximum_base_stamina"];
                    prev_avg = prev_avg + (organism->theoretical_maximum_base_stamina() - prev_avg) / (k + 1);
                }
                {
                    float &prev_avg = org_map["theoretical_maximum_base_vitality"];
                    prev_avg = prev_avg + (organism->theoretical_maximum_base_vitality() - prev_avg) / (k + 1);
                }
                {
                    float &prev_avg = org_map["theoretical_maximum_base_weight"];
                    prev_avg = prev_avg + (organism->theoretical_maximum_base_weight() - prev_avg) / (k + 1);
                }
                {
                    float &prev_avg = org_map["theoretical_maximum_height_multiplier"];
                    prev_avg = prev_avg + (organism->theoretical_maximum_height_multiplier() - prev_avg) / (k + 1);
                }
                {
                    float &prev_avg = org_map["theoretical_maximum_speed_multiplier"];
                    prev_avg = prev_avg + (organism->theoretical_maximum_speed_multiplier() - prev_avg) / (k + 1);
                }
                {
                    float &prev_avg = org_map["theoretical_maximum_stamina_multiplier"];
                    prev_avg = prev_avg + (organism->theoretical_maximum_stamina_multiplier() - prev_avg) / (k + 1);
                }
                {
                    float &prev_avg = org_map["theoretical_maximum_vitality_multiplier"];
                    prev_avg = prev_avg + (organism->theoretical_maximum_vitality_multiplier() - prev_avg) / (k + 1);
                }
                {
                    float &prev_avg = org_map["theoretical_maximum_weight_multiplier"];
                    prev_avg = prev_avg + (organism->theoretical_maximum_weight_multiplier() - prev_avg) / (k + 1);
                }
                {
                    float &prev_avg = org_map["theoretical_maximum_height"];
                    prev_avg = prev_avg + (organism->theoretical_maximum_height() - prev_avg) / (k + 1);
                }
                {
                    float &prev_avg = org_map["theoretical_maximum_speed"];
                    prev_avg = prev_avg + (organism->theoretical_maximum_speed() - prev_avg) / (k + 1);
                }
                {
                    float &prev_avg = org_map["theoretical_maximum_weight"];
                    prev_avg = prev_avg + (organism->theoretical_maximum_weight() - prev_avg) / (k + 1);
                }
                {
                    float &prev_avg = org_map["sleep_restore_factor"];
                    prev_avg = prev_avg + (organism->sleep_restore_factor() - prev_avg) / (k + 1);
                }
                {
                    float &prev_avg = org_map["vision_radius"];
                    prev_avg = prev_avg + (organism->vision_radius() - prev_avg) / (k + 1);
                }
                {
                    float &prev_avg = org_map["immunity"];
                    prev_avg = prev_avg + (organism->immunity() - prev_avg) / (k + 1);
                }
                {
                    float &prev_avg = org_map["base_appetite"];
                    prev_avg = prev_avg + (organism->base_appetite() - prev_avg) / (k + 1);
                }
                {
                    float &prev_avg = org_map["base_speed"];
                    prev_avg = prev_avg + (organism->base_speed() - prev_avg) / (k + 1);
                }
                {
                    float &prev_avg = org_map["base_stamina"];
                    prev_avg = prev_avg + (organism->base_stamina() - prev_avg) / (k + 1);
                }
                {
                    float &prev_avg = org_map["base_vitality"];
                    prev_avg = prev_avg + (organism->base_vitality() - prev_avg) / (k + 1);
                }
                {
                    float &prev_avg = org_map["base_weight"];
                    prev_avg = prev_avg + (organism->base_weight() - prev_avg) / (k + 1);
                }
                {
                    float &prev_avg = org_map["base_height"];
                    prev_avg = prev_avg + (organism->base_height() - prev_avg) / (k + 1);
                }
                {
                    float &prev_avg = org_map["weight"];
                    prev_avg = prev_avg + (organism->weight() - prev_avg) / (k + 1);
                }
                {
                    float &prev_avg = org_map["height"];
                    prev_avg = prev_avg + (organism->height() - prev_avg) / (k + 1);
                }
                {
                    float &prev_avg = org_map["max_weight"];
                    prev_avg = prev_avg + (organism->max_weight() - prev_avg) / (k + 1);
                }
                {
                    float &prev_avg = org_map["max_height"];
                    prev_avg = prev_avg + (organism->max_height() - prev_avg) / (k + 1);
                }
                {
                    float &prev_avg = org_map["weight_multiplier"];
                    prev_avg = prev_avg + (organism->weight_multiplier() - prev_avg) / (k + 1);
                }
                {
                    float &prev_avg = org_map["height_multiplier"];
                    prev_avg = prev_avg + (organism->height_multiplier() - prev_avg) / (k + 1);
                }
                {
                    float &prev_avg = org_map["speed_multiplier"];
                    prev_avg = prev_avg + (organism->speed_multiplier() - prev_avg) / (k + 1);
                }
                {
                    float &prev_avg = org_map["stamina_multiplier"];
                    prev_avg = prev_avg + (organism->stamina_multiplier() - prev_avg) / (k + 1);
                }
                {
                    float &prev_avg = org_map["vitality_multiplier"];
                    prev_avg = prev_avg + (organism->vitality_multiplier() - prev_avg) / (k + 1);
                }
                {
                    float &prev_avg = org_map["vitality"];
                    prev_avg = prev_avg + (organism->vitality() - prev_avg) / (k + 1);
                }
                {
                    float &prev_avg = org_map["stamina"];
                    prev_avg = prev_avg + (organism->stamina() - prev_avg) / (k + 1);
                }
                {
                    float &prev_avg = org_map["appetite"];
                    prev_avg = prev_avg + (organism->appetite() - prev_avg) / (k + 1);
                }
                {
                    float &prev_avg = org_map["speed"];
                    prev_avg = prev_avg + (organism->speed() - prev_avg) / (k + 1);
                }
                {
                    float &prev_avg = org_map["static_fitness"];
                    prev_avg = prev_avg + (organism->static_fitness() - prev_avg) / (k + 1);
                }
                {
                    float &prev_avg = org_map["dynamic_fitness"];
                    prev_avg = prev_avg + (organism->dynamic_fitness() - prev_avg) / (k + 1);
                }
                {
                    float &prev_avg = org_map["mating_age_start"];
                    prev_avg = prev_avg + (organism->mating_age_start() - prev_avg) / (k + 1);
                }
                {
                    float &prev_avg = org_map["mating_age_end"];
                    prev_avg = prev_avg + (organism->mating_age_end() - prev_avg) / (k + 1);
                }
                {
                    float &prev_avg = org_map["max_age"];
                    prev_avg = prev_avg + (organism->max_age() - prev_avg) / (k + 1);
                }
                {
                    float &prev_avg = org_map["generation"];
                    prev_avg = prev_avg + (organism->generation() - prev_avg) / (k + 1);
                }
                {
                    float &prev_avg = org_map["age"];
                    prev_avg = prev_avg + (organism->age() - prev_avg) / (k + 1);
                }
                {
                    float &prev_avg = org_map["X"];
                    prev_avg = prev_avg + (organism->X() - prev_avg) / (k + 1);
                }
                {
                    float &prev_avg = org_map["Y"];
                    prev_avg = prev_avg + (organism->Y() - prev_avg) / (k + 1);
                }

                k++;
            }

            new_organism_builder.add_offsprings_factor(org_map["offsprings_factor"]);
            new_organism_builder.add_mutation_probability(org_map["mutation_probability"]);
            new_organism_builder.add_conceiving_probability(org_map["conceiving_probability"]);
            new_organism_builder.add_mating_probability(org_map["mating_probability"]);
            new_organism_builder.add_age_fitness_on_death_ratio(org_map["age_fitness_on_death_ratio"]);
            new_organism_builder.add_height_on_speed(org_map["height_on_speed"]);
            new_organism_builder.add_height_on_stamina(org_map["height_on_stamina"]);
            new_organism_builder.add_height_on_vitality(org_map["height_on_vitality"]);
            new_organism_builder.add_weight_on_speed(org_map["weight_on_speed"]);
            new_organism_builder.add_weight_on_stamina(org_map["weight_on_stamina"]);
            new_organism_builder.add_weight_on_vitality(org_map["weight_on_vitality"]);
            new_organism_builder.add_vitality_on_appetite(org_map["vitality_on_appetite"]);
            new_organism_builder.add_vitality_on_speed(org_map["vitality_on_speed"]);
            new_organism_builder.add_stamina_on_appetite(org_map["stamina_on_appetite"]);
            new_organism_builder.add_stamina_on_speed(org_map["stamina_on_speed"]);
            new_organism_builder.add_theoretical_maximum_base_appetite(org_map["theoretical_maximum_base_appetite"]);
            new_organism_builder.add_theoretical_maximum_base_height(org_map["theoretical_maximum_base_height"]);
            new_organism_builder.add_theoretical_maximum_base_speed(org_map["theoretical_maximum_base_speed"]);
            new_organism_builder.add_theoretical_maximum_base_stamina(org_map["theoretical_maximum_base_stamina"]);
            new_organism_builder.add_theoretical_maximum_base_vitality(org_map["theoretical_maximum_base_vitality"]);
            new_organism_builder.add_theoretical_maximum_base_weight(org_map["theoretical_maximum_base_weight"]);
            new_organism_builder.add_theoretical_maximum_height_multiplier(org_map["theoretical_maximum_height_multiplier"]);
            new_organism_builder.add_theoretical_maximum_speed_multiplier(org_map["theoretical_maximum_speed_multiplier"]);
            new_organism_builder.add_theoretical_maximum_stamina_multiplier(org_map["theoretical_maximum_stamina_multiplier"]);
            new_organism_builder.add_theoretical_maximum_vitality_multiplier(org_map["theoretical_maximum_vitality_multiplier"]);
            new_organism_builder.add_theoretical_maximum_weight_multiplier(org_map["theoretical_maximum_weight_multiplier"]);
            new_organism_builder.add_theoretical_maximum_height(org_map["theoretical_maximum_height"]);
            new_organism_builder.add_theoretical_maximum_speed(org_map["theoretical_maximum_speed"]);
            new_organism_builder.add_theoretical_maximum_weight(org_map["theoretical_maximum_weight"]);
            new_organism_builder.add_sleep_restore_factor(org_map["sleep_restore_factor"]);
            new_organism_builder.add_vision_radius(org_map["vision_radius"]);
            new_organism_builder.add_immunity(org_map["immunity"]);
            new_organism_builder.add_base_appetite(org_map["base_appetite"]);
            new_organism_builder.add_base_speed(org_map["base_speed"]);
            new_organism_builder.add_base_stamina(org_map["base_stamina"]);
            new_organism_builder.add_base_vitality(org_map["base_vitality"]);
            new_organism_builder.add_base_weight(org_map["base_weight"]);
            new_organism_builder.add_base_height(org_map["base_height"]);
            new_organism_builder.add_weight(org_map["weight"]);
            new_organism_builder.add_height(org_map["height"]);
            new_organism_builder.add_max_weight(org_map["max_weight"]);
            new_organism_builder.add_max_height(org_map["max_height"]);
            new_organism_builder.add_weight_multiplier(org_map["weight_multiplier"]);
            new_organism_builder.add_height_multiplier(org_map["height_multiplier"]);
            new_organism_builder.add_speed_multiplier(org_map["speed_multiplier"]);
            new_organism_builder.add_stamina_multiplier(org_map["stamina_multiplier"]);
            new_organism_builder.add_vitality_multiplier(org_map["vitality_multiplier"]);
            new_organism_builder.add_vitality(org_map["vitality"]);
            new_organism_builder.add_stamina(org_map["stamina"]);
            new_organism_builder.add_appetite(org_map["appetite"]);
            new_organism_builder.add_speed(org_map["speed"]);
            new_organism_builder.add_static_fitness(org_map["static_fitness"]);
            new_organism_builder.add_dynamic_fitness(org_map["dynamic_fitness"]);

            new_organism_builder.add_mating_age_start(static_cast<uint32_t>(org_map["mating_age_start"]));
            new_organism_builder.add_mating_age_end(static_cast<uint32_t>(org_map["mating_age_end"]));
            new_organism_builder.add_max_age(static_cast<uint32_t>(org_map["max_age"]));
            new_organism_builder.add_generation(static_cast<uint32_t>(org_map["generation"]));
            new_organism_builder.add_age(static_cast<uint32_t>(org_map["age"]));

            new_organism_builder.add_X(static_cast<uint32_t>(org_map["X"]));
            new_organism_builder.add_Y(static_cast<uint32_t>(org_map["Y"]));

            new_stdvecOrganisms.push_back(new_organism_builder.Finish());

            new_species_builder.add_organism(new_builder.CreateVectorOfSortedTables(new_stdvecOrganisms.data(), new_stdvecOrganisms.size()));
            new_species_builder.add_kingdom(species->kingdom());
            new_species_builder.add_kind(new_builder.CreateString(species->kind()));

            new_stdvecSpecies.push_back(new_species_builder.Finish());
        }

        new_world_builder.add_species(new_builder.CreateVectorOfSortedTables(new_stdvecSpecies.data(), new_stdvecSpecies.size()));
        new_world_builder.add_year(world_pointer->year());
        new_builder.Finish(new_world_builder.Finish());

        FBuffer new_buffer(new_builder.GetSize());
        uint8_t *new_buffer_ptr = new_builder.GetBufferPointer();

        for (int i = 0; i < new_buffer.size(); i++)
            new_buffer[i] = new_buffer_ptr[i];

        return new_buffer;
    }
}; // namespace stat_fetcher

std::vector<float> get_value_vector(
    const std::vector<std::unordered_map<std::string, float>> &worlds,
    const std::string &attribute)
{
    std::vector<float> values;
    values.reserve(worlds.size());
    for (const auto &world : worlds)
    {
        auto it = world.find(attribute);
        if (it == world.end())
            throw std::runtime_error("Invalid attribute");
        values.push_back(it->second);
    }
    return values;
}
