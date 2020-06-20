/*
 *   Copyright (C) 2019-2020 Rafael M. Siejakowski
 *   License information at the end of the file.
 */
#include "modes.h"

//==========================================================================================
program_mode decide_mode(int argc, char** argv)
/*
	Based on argc, argv, decide which mode the program should run in
*/
{
	if (argc < 2) // Not enough parameters
	{
		return MODE_USAGE;
	}
	std::string mode_string = std::string(argv[1]);
	if (mode_string == MODE_INTEGRATE_STRING)
	{
		// for MODE_INTEGRATE, we expect 4 more positional params:
		// infile
		// Re(hbar)
		// Im(hbar)
		// samples
		if (argc < 4+2)
			return MODE_USAGE;
		else
			return MODE_INTEGRATE;
	}
	else if (mode_string == MODE_WRITE_STRING)
	{
		// for MODE_WRITE, we expect 4 more positional params:
		// infile
		// Re(hbar)
		// Im(hbar)
		// samples
		if (argc < 4+2)
			return MODE_USAGE;
		else
			return MODE_WRITE; 
	}
	else if (mode_string == MODE_HELP_STRING_1 || mode_string == MODE_HELP_STRING_2)
		return MODE_HELP;
	else
		return MODE_USAGE;
}
//==========================================================================================
void display_usage()
{
	using namespace std;
	cout 
	          << "Usage:" << endl << endl
	          << "m3di MODE PARAMETERS" << endl << endl
	          << "Avaliable modes are:" << endl
	          << MODE_INTEGRATE_STRING << endl
	          << MODE_WRITE_STRING << endl
	          << MODE_HELP_STRING_1 << endl << endl
	          << "Type \"m3di " << MODE_HELP_STRING_1 << "\" for help." << endl;
}
//==========================================================================================
void display_help()
{
	std::cout << 
"m3di - a program for computing the meromorphic 3D-index\n\n"
"Commandline syntax:\n"
"m3di COMMAND PARAMETERS\n\n"
"Available COMMANDs:\n"
"integrate\n"
"          The integrate command is used to compute the total meromorphic 3D-index.\n"
"          The syntax for this mode is:\n"
"              m3di integrate <file> <Re_hbar> <Im_hbar> <samples>\n"
"          The meaning of the parameters is as follows:\n"
"          <file>    - The path to a JSON file containing combinatorial information\n"
"                      about the triangulated 3-manifold.\n"
"          <Re_hbar> - The real part of the parameter hbar of meromorphic 3D-index.\n"
"          <Im_hbar> - The imaginary part of the parameter hbar.\n"
"          <samples> - A positive integer specifying how many sample points are to be\n"
"                      taken in each iterated integral. A higher sample count generally\n"
"                      results in a higher accuracy of the result but also in a slower\n"
"                      computation. For q not too close to the boundary of the unit disc,\n"
"                      a value of <samples> in the range 5000 to 10000 usually suffices.\n\n"
"write\n"
"          This command does not compute the state integral, but rather writes out sampled\n"
"          values of the integrand as JSON data to the standard output.\n"
"          The syntax for this mode is:\n"
"              m3di write <file> <Re_hbar> <Im_hbar> <samples>\n"
"          The meaning of the parameters is as follows:\n"
"          <file>    - The path to a JSON file containing combinatorial information\n"
"                      about the triangulated 3-manifold.\n"
"          <Re_hbar> - The real part of the parameter hbar of meromorphic 3D-index.\n"
"          <Im_hbar> - The imaginary part of the parameter hbar.\n"
"          <samples> - A positive integer specifying how many sample points are to be\n"
"                      taken in each iterated integral. A higher sample count generally\n"
"                      results in a higher accuracy of the result but also in a slower\n"
"                      computation.\n\n";
}
//==========================================================================================
bool validate_q_and_samples(double Rehbar, double Imhbar, int samples)
{
	// Checks if the values have been specified correctly
	// Returns true on valid data, false on invalid.
	if (samples < 1)
	{
		std::cerr << "Error: The number of samples must be a positive integer!" << std::endl;
		return false;
	}
	double a = exp(Rehbar); // a = |q|
	if (a < DBL_MIN || a >= 1.0)
	{
		std::cerr << "Error: The value of q specified does not satisfy 0<|q|<1!" << std::endl;
		return false;
	}
	return true;
}
//==========================================================================================
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
