/*
 *   Copyright (C) 2019-2020 Rafael M. Siejakowski
 *   License information at the end of the file.
 */
#ifndef __MODES_H__
#define __MODES_H__

#include <iostream>
#include <fstream>
#include <string>
#include "manifold.h"
#include "integrate.h"
#include "write.h"

enum program_mode {MODE_INTEGRATE, MODE_USAGE, MODE_HELP, MODE_WRITE};
const std::string MODE_INTEGRATE_STRING = std::string("integrate");
const std::string MODE_HELP_STRING_1    = std::string("help");
const std::string MODE_HELP_STRING_2    = std::string("--help");
const std::string MODE_WRITE_STRING     = std::string("write");

void display_usage();
void display_help();

program_mode decide_mode(int argc, char** argv);
bool validate_q_and_samples(double Rehbar, double Imhbar, int samples);
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
