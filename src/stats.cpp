/*
 *   Copyright (C) 2021 Rafael M. Siejakowski.
 *   All rights reserved.
 *   License information at the end of the file.
 */
#include "stats.h"

//==============================================================================================
stats::stats() : num_threads {1}
// Class constructor records the start time
{
    using namespace std::chrono;
    start = steady_clock::now(); // store construction time
    tabulation = integration = begin = start;
}
//==============================================================================================
void stats::signal(stats_signals s)
// Receives and processes a signal informing about the next stage of program execution
{
    using namespace std::chrono;
    switch (s)
    {
        case stats_signals::begin_computation:
            begin = steady_clock::now();
            [[fallthrough]]; // We always want tabulation to be >= than begin
        case stats_signals::finish_tabulation:
            tabulation = steady_clock::now();
            [[fallthrough]]; // We always want integration to be >= than tabulation
        case stats_signals::finish_integration:
            integration = steady_clock::now();
    }
}
//==============================================================================================
void stats::fill(Json::Value& v)
// Fills the Json::Value v passed by reference with the statistics information
{
    using namespace Json;
    Value th = num_threads;
    interval_t setup_time = begin - start;
    interval_t tabulation_walltime = tabulation - begin;
    interval_t integration_walltime = integration - tabulation;
    interval_t total_walltime = integration - start;
    v["threads"] = th;
    v["setup wtime [s]"] = setup_time.count();
    v["tabulation wtime [s]"] = tabulation_walltime.count();
    v["integration wtime [s]"] = integration_walltime.count();
    v["total wtime [s]"] = total_walltime.count();
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
