#include <StopWatch.hpp>

namespace timer 
{
    void StopWatch::start()
    {
        status = true;
        t_start = std::chrono::steady_clock::now();
    }

    void StopWatch::stop()
    {
        // avg(n+1) = (n / (n + 1)) * avg(n) + (1 / (n + 1)) * kn
        status = false;
        t_end = std::chrono::steady_clock::now();
        cur_time = std::chrono::duration_cast<std::chrono::nanoseconds>(t_end - t_start).count();
        //avg_time = ( avg_time * count + cur_time ) / (count + 1);
        avg_time = ((double)count / (count + 1)) * avg_time + (1.0 / (count + 1)) * cur_time;
        count++;
    }

    void StopWatch::reset()
    {
        status = false;
        count = 0;
        avg_time = 0;
    }

    StopWatch::~StopWatch()
    {
        if(status)
        {
            stop();
        }
    }
};
