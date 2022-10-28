#include <StopWatch.hpp>

namespace timer {
void StopWatch::start() {
    status = true;
    t_start = std::chrono::steady_clock::now();
}

void StopWatch::stop() {
    status = false;
    t_end = std::chrono::steady_clock::now();
    cur_time =
        std::chrono::duration_cast<std::chrono::nanoseconds>(t_end - t_start)
            .count();
    avg_time = (avg_time * count + cur_time) / (count + 1);
    count++;
}

void StopWatch::reset() {
    status = false;
    count = 0;
    avg_time = 0;
}

StopWatch::~StopWatch() {
    if (status) {
        stop();
    }
}
};  // namespace timer
