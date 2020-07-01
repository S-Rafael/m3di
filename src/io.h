/*
 *   Copyright (C) 2019-2020 Rafael M. Siejakowski
 *   License information at the end of the file.
 */

#ifndef __IO_H__
#define __IO_H__

#include <iostream>
#include <string>
#include <cfloat>
#include <cmath>
#include <thread> 

/*
 * This file declare miscellaneous I/O and data validation functions
 * as well as functions responsible for detecting the available hardware concurrency.
 */
const int thread_base = 5; // assume 1 thread per 10^(thread_base) data points
const int reasonable_concurrency = 8; // default number of cores if unable to get exact value 
const int overload = 4; // maximum threads per core 

double parse_double(const char* input);
int parse_int(const char* input);
bool validate_q_and_samples(double Rehbar, int samples);
int decide_thread_count(int N, int samples);
int make_divisible(int n, int d);

#endif

/*
 *
 * Copyright (C) 2019-2020 Rafael M. Siejakowski
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
