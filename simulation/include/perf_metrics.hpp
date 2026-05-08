#ifndef PERF_METRICS_HPP
#define PERF_METRICS_HPP

#include <chrono>
#include <cstdint>
#include <cstdio>
#include <fmt/core.h>

struct PerfMetrics {
    double select_us = 0;
    double clone_us = 0;
    double mate_us = 0;
    double stats_us = 0;
    double pop_stats_us = 0;
    double db_us = 0;
    double total_us = 0;

    uint32_t organisms_before = 0;
    uint32_t deaths = 0;
    uint32_t survivors = 0;
    uint32_t mating_attempts = 0;
    uint32_t births = 0;
    uint32_t abnormal_rejects = 0;

    size_t builder_bytes = 0;
    size_t buffer_bytes = 0;
    size_t avg_buffer_bytes = 0;
    size_t population_stats_bytes = 0;

    uint32_t chromosome_decode_calls = 0;
    double chromosome_decode_us = 0;

    double db_write_us = 0;

    uint32_t clone_count = 0;

    double cumulative_select_us = 0;
    double cumulative_clone_us = 0;
    double cumulative_mate_us = 0;
    double cumulative_stats_us = 0;
    double cumulative_total_us = 0;
    uint32_t cumulative_survivors = 0;
    uint32_t cumulative_births = 0;
    uint32_t cumulative_deaths = 0;

    void reset() {
        select_us = 0;
        clone_us = 0;
        mate_us = 0;
        stats_us = 0;
        pop_stats_us = 0;
        db_us = 0;
        total_us = 0;
        organisms_before = 0;
        deaths = 0;
        survivors = 0;
        mating_attempts = 0;
        births = 0;
        abnormal_rejects = 0;
        builder_bytes = 0;
        buffer_bytes = 0;
        avg_buffer_bytes = 0;
        population_stats_bytes = 0;
        chromosome_decode_calls = 0;
        chromosome_decode_us = 0;
        db_write_us = 0;
        clone_count = 0;
    }

    void accumulate() {
        cumulative_select_us += select_us;
        cumulative_clone_us += clone_us;
        cumulative_mate_us += mate_us;
        cumulative_stats_us += stats_us;
        cumulative_total_us += total_us;
        cumulative_survivors += survivors;
        cumulative_births += births;
        cumulative_deaths += deaths;
    }

    void print(uint32_t year) const {
        double total_ms = total_us / 1000.0;
        double select_ms = select_us / 1000.0;
        double clone_ms = clone_us / 1000.0;
        double mate_ms = mate_us / 1000.0;
        double stats_ms = stats_us / 1000.0;
        double pop_stats_ms = pop_stats_us / 1000.0;
        double db_ms = db_us / 1000.0;
        double chromo_ms = chromosome_decode_us / 1000.0;

        fmt::print("[PERF] year={} total_ms={:.1f} select_ms={:.1f} clone_ms={:.1f} "
                   "mate_ms={:.1f} stats_ms={:.1f} pop_stats_ms={:.1f} db_ms={:.1f} "
                   "chromo_decode_ms={:.1f}(calls={}) "
                   "pop_before={} deaths={} survivors={} births={} rejects={} "
                   "mating_attempts={} "
                   "builder_kb={} buffer_kb={} avg_kb={} popstats_kb={}\n",
                   year, total_ms, select_ms, clone_ms, mate_ms, stats_ms,
                   pop_stats_ms, db_ms, chromo_ms, chromosome_decode_calls,
                   organisms_before, deaths, survivors, births, abnormal_rejects,
                   mating_attempts,
                   builder_bytes / 1024, buffer_bytes / 1024,
                   avg_buffer_bytes / 1024, population_stats_bytes / 1024);
    }

    void print_cumulative() const {
        double sel = cumulative_select_us / 1000.0;
        double clo = cumulative_clone_us / 1000.0;
        double mat = cumulative_mate_us / 1000.0;
        double sta = cumulative_stats_us / 1000.0;
        double tot = cumulative_total_us / 1000.0;
        fmt::print("[PERF-CUMULATIVE] select_ms={:.1f} clone_ms={:.1f} "
                   "mate_ms={:.1f} stats_ms={:.1f} total_ms={:.1f} "
                   "survivors={} births={} deaths={}\n",
                   sel, clo, mat, sta, tot,
                   cumulative_survivors, cumulative_births, cumulative_deaths);
        if (cumulative_survivors > 0) {
            fmt::print("[PERF-PER-ORG] clone_us_per_survivor={:.2f} "
                       "select_us_per_survivor={:.2f}\n",
                       cumulative_clone_us / cumulative_survivors,
                       cumulative_select_us / cumulative_survivors);
        }
        if (cumulative_births > 0) {
            fmt::print("[PERF-PER-BIRTH] mate_us_per_birth={:.2f} "
                       "total_us_per_birth={:.2f}\n",
                       cumulative_mate_us / cumulative_births,
                       cumulative_total_us / cumulative_births);
        }
    }
};

class ScopedPhaseTimer {
public:
    explicit ScopedPhaseTimer(double &target)
        : m_target(target), m_start(std::chrono::steady_clock::now()) {}

    ~ScopedPhaseTimer() {
        auto end = std::chrono::steady_clock::now();
        double elapsed = std::chrono::duration<double, std::micro>(end - m_start).count();
        m_target += elapsed;
    }

    ScopedPhaseTimer(const ScopedPhaseTimer &) = delete;
    ScopedPhaseTimer &operator=(const ScopedPhaseTimer &) = delete;

private:
    double &m_target;
    std::chrono::time_point<std::chrono::steady_clock> m_start;
};

#endif // PERF_METRICS_HPP
