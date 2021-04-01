/*
 *   Copyright (C) 2019-2021 Rafael M. Siejakowski
 *   License information at the end of the file.
 */
#include "modes.h"

//==========================================================================================
program_mode decide_mode(int argc, const char** argv)
/*
	Based on argc, argv, return the mode in which the program should run.
*/
{
	if (argc < 2) // Not enough parameters
	{
		return program_mode::usage;
	}
	std::string mode_string(argv[1]);
	if (mode_string == MODE_INTEGRATE_STRING)
	{
		// for MODE_INTEGRATE, we expect 4 more positional params:
		// infile, Re(hbar), Im(hbar), samples
		if (argc < 4+2)
			return program_mode::usage;
		else
			return program_mode::integrate;
	}
	else if (mode_string == MODE_WRITE_STRING)
	{
		// for MODE_WRITE, we expect 4 more positional params:
		// infile, Re(hbar), Im(hbar), samples
		if (argc < 4+2)
			return program_mode::usage;
		else
			return program_mode::write;
	}
	else if (mode_string == MODE_HELP_STRING_1 || mode_string == MODE_HELP_STRING_2)
		return program_mode::help;
	else
		return program_mode::usage;
}
//==========================================================================================
int integrate_mode(const char** argv)
/*
	This function implements the integration mode,
	which is the main mode of the program.
*/
{
	cmdline_data args = parse_cmdline(argv);
	if (!args.valid)
		return 1;
	mani_data M = mani_data(args.filepath);
	if (!M.is_valid())
	{
		std::cerr << "File '" << args.filepath << "' doesn't contain a valid "
		             "manifold specification!" << std::endl;
		return 1;
	}
	// ==== Compute the state integral of the meromorphic 3D-index ====
	stats Statistics;
	integrator I(M, args.hbar, args.samples);
	Statistics.signal(stats_signals::begin_computation);
	std::complex<double> integral = I.compute_integral(Statistics);
	Statistics.signal(stats_signals::finish_integration);
	// ==== Format output ====
	Json::Value output;
	// Check if the returned value of the integral is infinity or NaN
	if (integral == INFTY)
	{
		output["int_real_part"] = "infinity";
		output["int_imag_part"] = "infinity";
	}
	else if (std::isnan(integral.real()) || std::isnan(integral.imag())) 
	{
		output["int_real_part"] = "infinity or removable singularity";
		output["int_imag_part"] = "infinity or removable singularity";
	}
	else
	{
		output["int_real_part"] = integral.real();
		output["int_imag_part"] = integral.imag();
	}
	// Fill out remaining fields
	Json::Value s;
	Statistics.fill(s);
	output["statistics"] = s;
	output["hbar_given"] = format_complex_strings(argv[3], argv[4]); // Re(hbar), Im(hbar)
	output["samples"] = args.samples;
	output["hbar_real_part"] = args.hbar.real();
	output["hbar_imag_part"] = args.hbar.imag();
	// Output data; TODO: implement output to file instead of std::cout
	print_json(&(std::cout), output);
	return 0;
}
//==========================================================================================
int write_mode(const char** argv)
/*
	This function implements the write mode,
	which outputs the integrand values as JSON data.
*/
{
	cmdline_data args = parse_cmdline(argv);
	if (!args.valid)
		return 1;
	mani_data M = mani_data(args.filepath);
	if (!M.is_valid())
	{
		std::cerr << "File '" << args.filepath << "' doesn't contain a valid "
		             "manifold specification!" << std::endl;
		return 1;
	}
	// M is OK, we launch precomputation
	M.tabulate(args.hbar, args.samples);
	if (!M.ready())
	{
		std::cerr << "Error while computing integrand values." << std::endl;
		return 1;
	}
	// Create JSON representation of output
	Json::Value output;
	output["samples"] = args.samples;
	output["hbar_real_part"] = args.hbar.real();
	output["hbar_imag_part"] = args.hbar.imag();
	// Compute the integrand values and store them in output
	store_integrand_values(output, M, args.samples);
	output["hbar_given"] = format_complex_strings(argv[3], argv[4]); // Re(hbar), Im(hbar)
	// Output data; TODO: implement output to file instead of std::cout
	print_json(&(std::cout), output);
	return 0;
}
//==========================================================================================
int display_usage(int argc, const char** argv)
/*
	Outputs to stdout a brief message about the usage of the tool.
 */
{
	using namespace std;
	string executable((argc)? argv[0]: "m3di");
	cout << "Usage:" << endl << endl
		 << executable << " MODE PARAMETERS" << endl << endl
		 << "Avaliable modes are:" << endl
		 << MODE_INTEGRATE_STRING << endl
		 << MODE_WRITE_STRING << endl
		 << MODE_HELP_STRING_1 << endl << endl
		 << "Type \"" << executable << " "
		 << MODE_HELP_STRING_1 << "\" for help." << endl;
	return 0;
}
//==========================================================================================
int display_help(int argc, const char** argv)
{
	std::string executable((argc)? argv[0]: "m3di");
	std::cout << 
executable <<
" - a program for computing the meromorphic 3D-index\n\n"
"Command line syntax:\n"
<< executable <<
" COMMAND PARAMETERS\n\n"
"Available COMMANDs:\n"
"integrate\n"
"          The integrate command is used to compute the total meromorphic 3D-index.\n"
"          The syntax for this mode is:\n"
"              " << executable << " integrate <file> <Re_hbar> <Im_hbar> <samples>\n"
"          The meaning of the parameters is as follows:\n"
"          <file>    - Path to a JSON file containing combinatorial information\n"
"                      about the triangulated 3-manifold.\n"
"          <Re_hbar> - The real part of the parameter 'hbar' of meromorphic 3D-index.\n"
"          <Im_hbar> - The imaginary part of the parameter 'hbar'.\n"
"          <samples> - A positive integer specifying how many sample points are to be\n"
"                      taken in each iterated integral. A higher sample count generally\n"
"                      results in a higher accuracy of the result but also in a slower\n"
"                      computation. For q not too close to the boundary of the unit disc,\n"
"                      a value of <samples> in the range 5000 to 10000 usually suffices.\n\n"
"write\n"
"          This command does not compute the state integral, but rather writes out sampled\n"
"          values of the integrand as JSON data to the standard output.\n"
"          The syntax for this mode is:\n"
"              " << executable << " write <file> <Re_hbar> <Im_hbar> <samples>\n"
"          The meaning of the parameters is identical as in the integrate mode.\n\n";
	return 0;
}
//==========================================================================================
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
