/*
 *   Copyright (C) 2019-2021 Rafael M. Siejakowski.
 *   All rights reserved.
 *   License information at the end of the file.
 */

#ifndef __IO_H__
#define __IO_H__

#include <iostream>
#include <string>
#include <cfloat>
#include <cmath>
#include <thread> 
#include <json/json.h>
#include <complex>

/*
 * This file declare miscellaneous I/O and data validation functions
 */
struct cmdline_data
{
    std::complex<double> hbar;
	std::string hbar_textual;
    int samples;
    const char* filepath;
    bool valid;
};

double parse_double(const char* input) noexcept;
int parse_int(const char* input) noexcept;
bool validate_q_and_samples(double Rehbar, int samples);
int make_divisible(int n, int d);
void print_json(Json::OStream* destination, const Json::Value& data);
cmdline_data parse_cmdline(const char** argv);
std::string format_complex_strings(const char* re, const char* im);

#endif

/*
 *
 * Copyright (C) 2019-2021 Rafael M. Siejakowski
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
