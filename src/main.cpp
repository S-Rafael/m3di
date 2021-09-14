/*
 *   m3di - a program for computing the meromorphic 3D-index. 
 *              
 *   Author: Rafael M. Siejakowski; www.rs-math.net
 *
 * ------------------------------------------------------------------------------
 *
 *   For an explanation of what the meromorphic 3D-index is, see the
 *   paper https://arxiv.org/abs/2109.05355
 *
 * ------------------------------------------------------------------------------
 *
 * Copyright (C) 2019-2021 Rafael M. Siejakowski.
 * All rights reserved.
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
#include "modes.h"

//=======================================================================================
int main(int argc, const char** argv)
{
	auto mode = decide_mode(argc, argv);
	switch (mode)
	{
		case program_mode::integrate:
			return integrate_mode(argv);

		case program_mode::help:
			return display_help(argc, argv);

		case program_mode::write:
			return write_mode(argv);

		case program_mode::usage:
		default:
			return display_usage(argc, argv);
	}
}
//=======================================================================================

