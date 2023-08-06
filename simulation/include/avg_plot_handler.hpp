#pragma once

#include <nlohmann/json.hpp>
#include <ecosystem_types.hpp>

struct AveragePlotHandler {
    AveragePlotHandler() = default;
    void add_record(const FBuffer& avg_world_instance, const FBuffer& world_population_stats);

    nlohmann::json plot_history;
};
