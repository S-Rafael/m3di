/*
 *   Copyright (C) 2021 Rafael M. Siejakowski.
 *   All rights reserved.
 *   License information at the end of the file.
 */
#ifndef __STATS_H__
#define __STATS_H__

#include <chrono>
#include "json/json.h"

using interval_t = std::chrono::duration<double>;
using timept_t = std::chrono::time_point<std::chrono::steady_clock, interval_t>;

/**
 * @class
 * Keeps track of the statistics of computation
 *
 * @remark
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
public:
	enum class messages {begin_computation, finish_tabulation, finish_integration};

private:
	timept_t start, begin, tabulation, integration;
	int num_threads;

public:
	stats();
	inline void set_num_threads(int n) {num_threads=n;}
	void signal(stats::messages s);
	void fill(Json::Value& v);
};

#endif
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
