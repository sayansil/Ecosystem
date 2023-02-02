#include <imgui-SFML.h>
#include <imgui.h>
#include <implot.h>
#include <stdio.h>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <algorithm>
#include <god.hpp>
#include <iostream>
#include <limits>
#include <mutex>
#include <random>
#include <thread>
#include <vector>

#include "style.hpp"

struct PlotAttribute {
    std::vector<double> data;
    std::string name;
    std::pair<double, double> limits;
    std::string query_name;
    double padding;

    PlotAttribute(const std::string& name, const std::string& query_name,
                  const size_t& data_size) {
        this->name = name;
        this->query_name = query_name;
        this->data.reserve(data_size);
        this->limits = {std::numeric_limits<double>::max(),
                        -std::numeric_limits<double>::max()};
        this->padding = 0.0;
    }

    void fetchNextValue(ENTITY_MAP_TYPE& organisms) {
        if (query_name == "population")
            data.push_back(stat_fetcher::get_population(organisms));
        else
            data.push_back(
                stat_fetcher::get_stat_average(organisms, query_name));
    }
};

std::vector<PlotAttribute> all_plots;

const unsigned int initial_organism_count = 200;
const unsigned int years_to_simulate = 200;
const std::string kingdom = "animal";
const std::string species = "deer";

int k;

bool started, stopped;

std::mutex mu;
std::condition_variable cond;

int countloop = 0;
God allah;

void run_ecosystem_simulation() {
    std::unique_lock<std::mutex> locker(mu);
    cond.wait(locker, []() { return started; });
    locker.unlock();

    while (k && !stopped) {
        allah.happy_new_year(false);
        for (auto& i : all_plots) {
            i.fetchNextValue(allah.organisms);
            i.limits = {std::min(i.data.back(), i.limits.first),
                        std::max(i.data.back(), i.limits.second)};
            double tmp = (i.limits.second - i.limits.first) * 0.1;
            i.padding = tmp == 0.0 ? 1.0 : fabs(tmp);
        }
        k--;
    }
}

int main(int, char**) {
    allah.reset_species(kingdom + "/" + species);

    for (int i = 0; i < initial_organism_count; i++) {
        if (kingdom == "animal")
            allah.spawn_organism(std::make_shared<Animal>(
                species, 10, false, "OG-" + std::to_string(i)));
        else if (kingdom == "plant")
            allah.spawn_organism(std::make_shared<Plant>(
                species, 10, false, "OG-" + std::to_string(i)));
    }

    k = years_to_simulate;
    std::vector<std::pair<double, double>> limits(
        4, {std::numeric_limits<double>::max(),
            std::numeric_limits<double>::min()});

    all_plots.emplace_back("Population", "population", years_to_simulate);
    all_plots.emplace_back("Average Age", "age", years_to_simulate);
    all_plots.emplace_back("Average Height", "height", years_to_simulate);
    all_plots.emplace_back("Average Weight", "weight", years_to_simulate);
    all_plots.emplace_back("Average Maximum Vision Radius", "vision_radius",
                           years_to_simulate);
    all_plots.emplace_back("Average Maximum Speed", "max_speed_at_age",
                           years_to_simulate);
    all_plots.emplace_back("Average Maximum Appetite", "max_appetite_at_age",
                           years_to_simulate);
    all_plots.emplace_back("Average Maximum Vitality", "max_vitality_at_age",
                           years_to_simulate);
    all_plots.emplace_back("Average Maximum Stamina", "max_stamina_at_age",
                           years_to_simulate);
    all_plots.emplace_back("Average Mutation Probability",
                           "mutation_probability", years_to_simulate);
    all_plots.emplace_back("Average Conceiving Probability",
                           "conceiving_probability", years_to_simulate);
    all_plots.emplace_back("Average Death Factor By Age", "age_death_factor",
                           years_to_simulate);
    all_plots.emplace_back("Average Death Factor By Fitness",
                           "fitness_death_factor", years_to_simulate);
    all_plots.emplace_back("Average Age v Fitness",
                           "age_fitness_on_death_ratio", years_to_simulate);
    all_plots.emplace_back("Average Death Factor", "death_factor",
                           years_to_simulate);
    all_plots.emplace_back("Average Generation", "generation",
                           years_to_simulate);
    all_plots.emplace_back("Average Static Fitness", "static_fitness",
                           years_to_simulate);
    all_plots.emplace_back("Average Immunity", "immunity", years_to_simulate);
    all_plots.emplace_back("Start of Mating Age", "mating_age_start",
                           years_to_simulate);
    all_plots.emplace_back("End of Mating Age", "mating_age_end",
                           years_to_simulate);
    all_plots.emplace_back("Average Offsprings Factor", "offsprings_factor",
                           years_to_simulate);
    all_plots.emplace_back("Height On Stamina Dependancy", "height_on_stamina",
                           years_to_simulate);
    all_plots.emplace_back("Weight On Stamina Dependancy", "weight_on_stamina",
                           years_to_simulate);
    all_plots.emplace_back("Height On Vitality Dependancy",
                           "height_on_vitality", years_to_simulate);
    all_plots.emplace_back("Weight On Vitality Dependancy",
                           "weight_on_vitality", years_to_simulate);
    all_plots.emplace_back("Height On Speed Dependancy", "height_on_speed",
                           years_to_simulate);
    all_plots.emplace_back("Weight On Speed Dependancy", "weight_on_speed",
                           years_to_simulate);
    all_plots.emplace_back("Vitality On Speed Dependancy", "vitality_on_speed",
                           years_to_simulate);
    all_plots.emplace_back("Vitality On Appetite Dependancy",
                           "vitality_on_appetite", years_to_simulate);
    all_plots.emplace_back("Stamina On Appetite Dependancy",
                           "stamina_on_appetite", years_to_simulate);
    all_plots.emplace_back("Theoretical Maximum Age", "max_age",
                           years_to_simulate);
    all_plots.emplace_back("Theoretical Maximum Base Height",
                           "theoretical_maximum_base_height",
                           years_to_simulate);
    all_plots.emplace_back("Theoretical Maximum Base Weight",
                           "theoretical_maximum_base_weight",
                           years_to_simulate);
    all_plots.emplace_back("Theoretical Maximum Height",
                           "theoretical_maximum_height", years_to_simulate);
    all_plots.emplace_back("Theoretical Maximum Weight",
                           "theoretical_maximum_weight", years_to_simulate);
    all_plots.emplace_back("Theoretical Maximum Base Speed",
                           "theoretical_maximum_base_speed", years_to_simulate);
    all_plots.emplace_back("Theoretical Maximum Base Appetite",
                           "theoretical_maximum_base_appetite",
                           years_to_simulate);
    all_plots.emplace_back("Theoretical Maximum Base Stamina",
                           "theoretical_maximum_base_stamina",
                           years_to_simulate);
    all_plots.emplace_back("Theoretical Maximum Base Vitality",
                           "theoretical_maximum_base_vitality",
                           years_to_simulate);
    all_plots.emplace_back("Theoretical Maximum Speed",
                           "theoretical_maximum_speed", years_to_simulate);
    all_plots.emplace_back("Theoretical Maximum Speed Multiplier",
                           "theoretical_maximum_speed_multiplier",
                           years_to_simulate);
    all_plots.emplace_back("Theoretical Maximum Stamina Multiplier",
                           "theoretical_maximum_stamina_multiplier",
                           years_to_simulate);
    all_plots.emplace_back("Theoretical Maximum Vitality Multiplier",
                           "theoretical_maximum_vitality_multiplier",
                           years_to_simulate);
    all_plots.emplace_back("Theoretical Maximum Height Multiplier",
                           "theoretical_maximum_height_multiplier",
                           years_to_simulate);
    all_plots.emplace_back("Theoretical Maximum Weight Multiplier",
                           "theoretical_maximum_weight_multiplier",
                           years_to_simulate);

    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Ecosystem",
                            sf::Style::Fullscreen);
    window.setFramerateLimit(60);
    ImGui::SFML::Init(window);

    ImGui::CreateContext();
    ImPlot::CreateContext();

    ImGui::SetupImGuiStyle(true, 0.5);

    started = false;
    stopped = false;

    ImVec4 clear_color = ImVec4(1.00f, 1.00f, 1.00f, 0.00f);

    std::thread runner(run_ecosystem_simulation);

    std::unique_lock<std::mutex> locker(mu);

    sf::Clock deltaClock;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        ImGui::SFML::Update(window, deltaClock.restart());

        const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(
            ImVec2(main_viewport->WorkPos.x, main_viewport->WorkPos.y));
        ImGuiStyle& style = ImGui::GetStyle();
        style.WindowMenuButtonPosition = ImGuiDir_None;

        ImGui::Begin("Ecosystem Health");

        ImGui::Dummy(ImVec2(0.0f, 5.0f));
        ImGui::Text(
            "\tThe Ecosystem will start with the following characteristics:");
        ImGui::Dummy(ImVec2(0.0f, 5.0f));
        ImGui::Text("\tSpecies: %s", species.c_str());
        ImGui::Text("\tInitial Population: %d", initial_organism_count);
        ImGui::Text("\tYears to simulate: %d", years_to_simulate);
        ImGui::Dummy(ImVec2(0.0f, 20.0f));

        if (started) {
            if (!stopped) {
                ImGui::Text("\t");
                ImGui::SameLine();
                stopped = ImGui::Button("Stop");
            } else {
                ImGui::Text("Stopped the Simulation.");
            }
            ImGui::Dummy(ImVec2(0.0f, 20.0f));

            ImGui::Text("Current Year of Simulation: %d",
                        years_to_simulate - k);
            ImGui::Dummy(ImVec2(0.0f, 20.0f));

            std::vector<double> x(years_to_simulate - k);
            std::iota(x.begin(), x.end(), 1);

            const int max_cols = 2;

            if (ImGui::BeginTable("Plots", max_cols)) {
                for (int index = 0, cols = 0; index < all_plots.size();
                     index++, cols++) {
                    if (index % max_cols == 0) {
                        ImGui::TableNextRow();
                        cols = 0;
                    }

                    ImGui::TableSetColumnIndex(cols);
                    ImPlot::SetNextPlotLimits(
                        1, years_to_simulate - (stopped ? k : 0),
                        all_plots[index].limits.first -
                            all_plots[index].padding,
                        all_plots[index].limits.second +
                            all_plots[index].padding,
                        ImGuiCond_Always);
                    if (ImPlot::BeginPlot(all_plots[index].name.c_str())) {
                        ImPlot::PushColormap(ImPlotColormap_Twilight);
                        ImPlot::PlotLine("", x.data(),
                                         all_plots[index].data.data(),
                                         x.size());
                        ImPlot::EndPlot();
                    }
                }

                ImGui::EndTable();
            }
        } else {
            ImGui::Text("\t");
            ImGui::SameLine();
            started = ImGui::Button("Start");
            if (started) {
                locker.unlock();
                cond.notify_one();
            }
        }

        ImGui::Dummy(ImVec2(0.0f, 10.0f));
        ImGui::Text("%.1f fps", ImGui::GetIO().Framerate);
        ImGui::End();

        window.clear();
        ImGui::SFML::Render(window);
        window.display();
        int display_w, display_h;
    }

    runner.join();
    ImGui::SFML::Shutdown();

    return 0;
}
