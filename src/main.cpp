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
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
*/
#include "main.h"

//=======================================================================================
int main(int argc, char** argv)
{
	// Decide which mode the program should run in.
	program_mode mode = decide_mode(argc, argv);
	switch (mode)
	{
		case MODE_INTEGRATE:
			integrate_mode(argc, argv);
			break;
		case MODE_HELP:
			display_help();
			break;
		case MODE_WRITE:
			write_mode(argc, argv);
			break;
		case MODE_USAGE:
		default:
			display_usage();
	}
	return 0;
}
//=======================================================================================

