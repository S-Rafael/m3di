#ifndef __STATS_H__
#define __STATS_H__

#include <chrono>
#include "json/json.h"

using interval_t = std::chrono::duration<double>;
using timept_t = std::chrono::time_point<std::chrono::steady_clock, interval_t>;

enum class stats_signals {begin_computation, finish_tabulation, finish_integration};

/*
 * class stats
 *
 * The job of an object of this class is to store the durations of the two
 * steps in the computation: the tabulation phase and the integration phase.
 *
 * We get the two walltimes from the std::cthrono::steady_clock
 * in the unit of seconds (internal representation is double).
 *
 * In addition to the times, we also record the number of threads used.
 *
 */
class stats
{
    private:
        timept_t start, begin, tabulation, integration;
        int num_threads;
    public:
        stats();
        inline void set_num_threads(int n) {num_threads=n;}
        void signal(stats_signals s);
        void fill(Json::Value& v);
};

#endif
