/*
 *   m3di - a program for computing the meromorphic 3D-index. 
 *              
 *   Author: Rafael M. Siejakowski; www.rs-math.net
 *
 * ------------------------------------------------------------------------------
 *
 *   For an explanation of what the meromorphic 3D-index is, see the
 *   paper https://arxiv.org/
 *
 * ------------------------------------------------------------------------------
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
#include "modes.h"

//=======================================================================================
int main(int argc, const char** argv)
{
	program_mode mode = decide_mode(argc, argv);
	int exit_code{0};
	switch (mode)
	{
		case MODE_INTEGRATE:
			exit_code = integrate_mode(argv);
			break;
		case MODE_HELP:
			exit_code = display_help(argc, argv);
			break;
		case MODE_WRITE:
			exit_code = write_mode(argv);
			break;
		case MODE_USAGE:
		default:
			exit_code = display_usage(argc, argv);
	}
	return exit_code;
}
//=======================================================================================

