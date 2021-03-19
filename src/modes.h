/*
 *   Copyright (C) 2019-2020 Rafael M. Siejakowski
 *   License information at the end of the file.
 */
#ifndef __MODES_H__
#define __MODES_H__

#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include "manifold.h"
#include "integrator.h"
#include "write.h"
#include "io.h"
#include "constants.h"

/*
 * This file declares the subroutines which serve as sub-'main'-s of 
 * several different modes of the program, such as:
 *
 * integrate_mode(argc, argv),
 * write_mode(argc, argv)
 * and possibly others in the future.
 *
 * Additionally, we declare the function decide_mode() which
 * tells us which mode the program should run in.
 *
 * Also, we have helper functions display_usage() and display_help().
 *
 */

enum class program_mode {integrate, write, usage, help};
const std::string MODE_INTEGRATE_STRING {"integrate"};
const std::string MODE_HELP_STRING_1    {"help"};
const std::string MODE_HELP_STRING_2    {"--help"};
const std::string MODE_WRITE_STRING     {"write"};

program_mode decide_mode(int argc, const char** argv);
int integrate_mode(const char** argv);
int write_mode(const char** argv);

int display_usage(int argc, const char** argv);
int display_help(int argc, const char** argv);

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
