#ifndef STOPWATCH_HPP
#define STOPWATCH_HPP

#include <chrono>

// Milliseconds stopwatch

namespace timer
{
    struct StopWatch
    {
        unsigned long cur_time;
        double avg_time = 0.0;
        unsigned long count = 0;
        bool status = false;

        std::chrono::time_point<std::chrono::steady_clock> t_start;
        std::chrono::time_point<std::chrono::steady_clock> t_end;

        StopWatch() = default;
        ~StopWatch();

        void start();
        void stop();
        void reset();

    };
};

#endif /* STOPWATCH_HPP */
