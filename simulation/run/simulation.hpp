#pragma once

#include <vector>
#include <string>
#include <limits>
#include <mutex>
#include <condition_variable>
#include <god.hpp>
#include <world_generated.h>

enum class SimulationState {
    TAKING_INPUT,
    CREATING_WORLD,
    SIMULATION_RUNNING,
    SIMULATION_COMPLETED
};

struct PlotAttribute {
    std::vector<float> data;
    std::string name;
    std::pair<float, float> limits;
    std::string query_name;
    float padding;

    PlotAttribute() = default;
    PlotAttribute(const std::string& name, const std::string& query_name, const size_t& data_size) {
        this->name = name;
        this->query_name = query_name;
        this->data.reserve(data_size);
        this->limits = {std::numeric_limits<float>::max(), -std::numeric_limits<float>::max()};
        this->padding = 0.0f;
    }
};

struct Simulation {
    std::unordered_map<std::string, PlotAttribute> all_plots;
    SimulationState state = SimulationState::TAKING_INPUT;
    uint32_t initial_organism_count = 4000;
    uint32_t years_to_simulate = 200;
    const Ecosystem::KingdomE kingdom = Ecosystem::KingdomE::animal;
    const std::string species = "deer";
    const uint32_t initial_age = 20;
    std::mutex mu;
    std::unique_lock<std::mutex> simulation_lock;
    std::condition_variable cond;
    bool started = false;
    bool stopped = false;
    uint32_t k = years_to_simulate;
    God allah;
    std::string status_message = "";

    Simulation();
    void RenderUI();
    void run_ecosystem_simulation();
    void update_plots(const nlohmann::json & json_data);
};
