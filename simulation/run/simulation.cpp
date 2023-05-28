#include "simulation.hpp"

#include <flatbuffers/minireflect.h>
#include <fmt/core.h>
#include <imgui.h>
#include <implot.h>
#include <population_generated.h>

#include <algorithm>
#include <cmath>
#include <numeric>
#include <stat_fetcher.hpp>
#include <unordered_map>
#include <vector>

#include "imspinner.h"

Simulation::Simulation() {
    simulation_lock = std::unique_lock(mu);

    /* Setup plots now */
}

void Simulation::RenderUI() {
    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

    ImGui::Begin("Ecosystem Health");

    ImGui::Dummy(ImVec2(0.0f, 5.0f));
    ImGui::Text(
        "\tThe Ecosystem will start with the following characteristics:");
    ImGui::Dummy(ImVec2(0.0f, 5.0f));
    ImGui::Text("\tSpecies: %s", species.c_str());
    ImGui::SliderInt("Initial Population", (int*)&initial_organism_count, 1,
                     1000000);
    ImGui::SliderInt("Years to simulate", (int*)&years_to_simulate, 1, 10000);
    ImGui::Dummy(ImVec2(0.0f, 20.0f));

    if (state == SimulationState::CREATING_WORLD) {
        ImSpinner::SpinnerRotateGear("SpinnerRotateGear", 16, 6,
                                     ImColor(255, 255, 255), 2.1f, 8);
        ImGui::Text(status_message.c_str());
    } else if (state == SimulationState::SIMULATION_RUNNING) {
        ImSpinner::SpinnerLemniscate("SpinnerLemniscate", 20, 3,
                                     ImColor(255, 255, 255), 2.1f, 3);
        ImGui::Text(status_message.c_str());
        ImGui::Dummy(ImVec2(0.0f, 5.0f));
    } else if (state == SimulationState::SIMULATION_COMPLETED) {
        ImSpinner::SpinnerFadePulsar("SpinnerFadePulsar2", 16,
                                     ImColor(255, 255, 255), 0.9f, 2);
        ImGui::Text(status_message.c_str());
    }

    if (started) {
        if (!stopped) {
            ImGui::Text("\t");
            ImGui::SameLine();
            stopped = ImGui::Button("Stop");
        } else {
            ImGui::Text("Stopped the Simulation.");
        }

        ImGui::Dummy(ImVec2(0.0f, 20.0f));

        ImGui::Dummy(ImVec2(0.0f, 20.0f));

        std::vector<float> x(years_to_simulate - k);
        std::iota(x.begin(), x.end(), 1);

        const int max_cols = 2;
        if (ImGui::BeginTable("Plots", max_cols)) {
            int index = 0, cols = 0;
            auto plot_it = all_plots.begin();
            while (index < all_plots.size()) {
                const auto& plot = plot_it->second;
                if (index % max_cols == 0) {
                    ImGui::TableNextRow();
                    cols = 0;
                }

                ImGui::TableSetColumnIndex(cols);
                ImPlot::SetNextAxesLimits(
                    1, years_to_simulate - (stopped ? k : 0),
                    plot.limits.first - plot.padding,
                    plot.limits.second + plot.padding, ImGuiCond_Always);
                if (ImPlot::BeginPlot(plot.name.c_str())) {
                    ImPlot::PushColormap(ImPlotColormap_Twilight);
                    ImPlot::PlotLine("", x.data(), plot.data.data(), x.size());
                    ImPlot::EndPlot();
                }
                ++index;
                ++cols;
                ++plot_it;
            }

            ImGui::EndTable();
        }
    } else {
        ImGui::Text("\t");
        ImGui::SameLine();
        started = ImGui::Button("Start");
        if (started) {
            state = SimulationState::CREATING_WORLD;
            simulation_lock.unlock();
            cond.notify_one();
        }
    }

    ImGui::Dummy(ImVec2(0.0f, 10.0f));
    ImGui::Text("%.1f fps", ImGui::GetIO().Framerate);
    ImGui::End();
}

void Simulation::run_ecosystem_simulation() {
    std::unique_lock<std::mutex> locker(mu);
    cond.wait(locker, [this]() { return started; });
    locker.unlock();

    k = years_to_simulate;
    /* Setup initial ecosystem */

    std::vector<std::unordered_map<std::string, std::string>> organisms;
    organisms.reserve(initial_organism_count);

    for (size_t i = 0; i < initial_organism_count; i++) {
        organisms.push_back(
            {{"kind", species},
             {"kingdom", std::to_string(static_cast<uint8_t>(kingdom))},
             {"age", std::to_string(initial_age)}});
        status_message = fmt::format("Storing metadata of {} organisms", i + 1);
    }

    status_message = "Clearing database";
    allah.cleanSlate();
    status_message = "Creating World";
    allah.createWorld(organisms);
    status_message = "Happy New Year starts";

    while (k && !stopped) {
        status_message = fmt::format("Current year of simulation: {}",
                                     years_to_simulate - k);
        allah.happy_new_year(false);

        {
            flatbuffers::ToStringVisitor visitor("", true, "", true);
            flatbuffers::IterateFlatBuffer(
                allah.avg_buffer.data(), Ecosystem::WorldTypeTable(), &visitor);
            nlohmann::json json_data = nlohmann::json::parse(visitor.s);
            state = SimulationState::SIMULATION_RUNNING;
            update_plots(json_data["species"][0]["organism"][0]);
        }

        {
            flatbuffers::ToStringVisitor visitor("", true, "", true);
            flatbuffers::IterateFlatBuffer(
                allah.population_stats.data(), Ecosystem::WorldPopulationTypeTable(),
                &visitor);
            nlohmann::json json_data = nlohmann::json::parse(visitor.s);

            nlohmann::json formatted_json;
            formatted_json["matable_population"] =
                (uint32_t)json_data["species_population"][0]
                                   ["matable_population"]["female_population"] +
                (uint32_t)json_data["species_population"][0]
                                   ["matable_population"]["male_population"];
            formatted_json["non_matable_population"] =
                (uint32_t)
                    json_data["species_population"][0]["non_matable_population"]
                             ["female_population"] +
                (uint32_t)
                    json_data["species_population"][0]["non_matable_population"]
                             ["male_population"];
            formatted_json["population"] =
                (uint32_t)formatted_json["matable_population"] +
                (uint32_t)formatted_json["non_matable_population"];

            update_plots(formatted_json);
        }

        k--;
    }
    status_message = "Simulation completed";
    state = SimulationState::SIMULATION_COMPLETED;
}

void Simulation::update_plots(const nlohmann::json& json_data) {
    for (const auto& [key, value] : json_data.items()) {
        if (value.is_number()) {
            if (all_plots.find(key) == all_plots.end()) {
                all_plots[key] = PlotAttribute(key, key, years_to_simulate);
            }

            all_plots[key].data.emplace_back(value);
            all_plots[key].limits = {std::min(all_plots[key].data.back(),
                                              all_plots[key].limits.first),
                                     std::max(all_plots[key].data.back(),
                                              all_plots[key].limits.second)};
            double tmp =
                (all_plots[key].limits.second - all_plots[key].limits.first) *
                0.1;
            all_plots[key].padding = tmp == 0.0 ? 1.0 : fabs(tmp);
        }
    }
}
