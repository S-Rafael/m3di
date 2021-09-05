/*
 *   Copyright (C) 2021 Rafael M. Siejakowski.
 *   All rights reserved.
 *   License information at the end of the file.
 */

#include "stats.h"

//==============================================================================================
/**
 * @brief stats::stats - class constructor; records the start time
 */
stats::stats() : num_threads {1}
{
    using namespace std::chrono;
    start = steady_clock::now(); // store construction time
    tabulation = integration = begin = start;
}
//==============================================================================================
/**
 * @brief Receives and processes a signal informing about the next stage of program execution
 * @param s - the signal
 */
void stats::signal(stats::messages s)
{
    using namespace std::chrono;
    switch (s)
    {
        case stats::messages::begin_computation:
            begin = steady_clock::now();
            [[fallthrough]]; // We always want tabulation to be >= than begin
        case stats::messages::finish_tabulation:
            tabulation = steady_clock::now();
            [[fallthrough]]; // We always want integration to be >= than tabulation
        case stats::messages::finish_integration:
            integration = steady_clock::now();
    }
}
//==============================================================================================
/**
 * @brief Fills the JSON structure with the statistics information
 * @param v - the JSON structure to be filled
 */
void stats::fill(Json::Value& v)
{
    using namespace Json;
    Value th = num_threads;
    interval_t setup_time = begin - start;
    interval_t tabulation_walltime = tabulation - begin;
    interval_t integration_walltime = integration - tabulation;
    interval_t total_walltime = integration - start;
    v["threads"] = th;
    v["setup walltime"] = setup_time.count();
    v["tabulation walltime"] = tabulation_walltime.count();
    v["integration walltime"] = integration_walltime.count();
    v["total walltime [s]"] = total_walltime.count();
}
//==============================================================================================
/*
 *
 * Copyright (C) 2021 Rafael M. Siejakowski
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation;
 * later versions of the GNU General Public Licence do NOT apply.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 *
 */
