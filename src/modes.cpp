/*
 *   Copyright (C) 2019-2020 Rafael M. Siejakowski
 *   License information at the end of the file.
 */
#include "modes.h"

//==========================================================================================
program_mode decide_mode(int argc, const char** argv)
/*
	Based on argc, argv, return the mode in which the program should run in.
*/
{
	if (argc < 2) // Not enough parameters
	{
		return MODE_USAGE;
	}
	std::string mode_string(argv[1]);
	if (mode_string == MODE_INTEGRATE_STRING)
	{
		// for MODE_INTEGRATE, we expect 4 more positional params:
		// infile [Json]
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
int integrate_mode(const char** argv)
/*
	This function implements the integration mode,
	which is the main mode of the program.
*/
{
	// Parse hbar and sample count (positional command line parameters)
	double Rehbar = parse_double(argv[3]);
	double Imhbar = parse_double(argv[4]);
	int samples = parse_int(argv[5]);
	// Validate command line input
	if (!validate_q_and_samples(Rehbar, samples)) return 1;
	// Prepare basic data
	std::complex<double> hbar {Rehbar, Imhbar};
	mani_data M = mani_data(argv[2]); 
	if (!M.is_valid())
	{
		std::cerr << "File '" << argv[2] << "' doesn't contain a valid "
		             "manifold specification!" << std::endl;
		return 1;
	}
	// ==== Compute the state integral of the meromorphic 3D-index ====
	integrator I(M, hbar, samples);
	std::complex<double> integral = I.compute_integral();
	// ==== Format output ====
	// Format the given hbar_value
	std::ostringstream hbar_given;
	hbar_given << argv[3] << (Imhbar<0.0? "":"+") << argv[4] << "i";
	// Create JSON representation of output
	Json::Value output;
	output["hbar_given"] = hbar_given.str();
	output["samples"] = samples;
	output["hbar_real_part"] = Rehbar;
	output["hbar_imag_part"] = Imhbar;
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
	// Output data
	Json::StreamWriterBuilder builder;
	builder["indentation"] = "\t";
	builder.settings_["precision"] = 320;
	std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
	writer->write(output, &std::cout); // TODO: implement output to file
	std::cout << std::endl;
	return 0;
}
//==========================================================================================
int write_mode(const char** argv)
/*
	This function implements the write mode,
	which outputs the integrand values as JSON data.
*/
{
	// Parse hbar and sample count
	double Rehbar = parse_double(argv[3]);
	double Imhbar = parse_double(argv[4]);
	int samples = parse_int(argv[5]);
	if (!validate_q_and_samples(Rehbar, samples)) return 1;
	// Set up basic data
	std::complex<double> hbar {Rehbar, Imhbar};
	mani_data M = mani_data(argv[2]); 
	if (!M.is_valid())
	{
		std::cerr << "File '" << argv[2] << "' doesn't contain a valid "
		             "manifold specification!" << std::endl;
		return 1;
	}
	// M is OK, we launch precomputation
	M.precompute(hbar, samples);
	if (!M.ready())
	{
		std::cerr << "Error while computing integrand values." << std::endl;
		return 1;
	}
	// Create JSON representation of output
	Json::Value output;
	output["samples"] = samples;
	output["hbar_real_part"] = Rehbar;
	output["hbar_imag_part"] = Imhbar;
	// Compute the integrand values and store them in output
	store_integrand_values(output, M, samples);
	// Format the given hbar_value
	std::ostringstream hbar_given;
	hbar_given << argv[3] << (Imhbar<0.0? "":"+") << argv[4] << "i";
	// Output data
	output["hbar_given"] = hbar_given.str();
	Json::StreamWriterBuilder builder;
	builder["indentation"] = "\t";
	builder.settings_["precision"] = 64;
	std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
	writer->write(output, &std::cout); // TODO: allow output to file
	std::cout << std::endl;
	return 0;
}
//==========================================================================================
int display_usage(int argc, const char** argv)
/*
	Outputs to stdout a brief message about the usage of the tool.
 */
{
	using namespace std;
	cout 
	          << "Usage:" << endl << endl;
	if (argc)
		cout << argv[0];
	else
		cout << "m3di"; //default executable name
        cout
	          << " MODE PARAMETERS" << endl << endl
	          << "Avaliable modes are:" << endl
	          << MODE_INTEGRATE_STRING << endl
	          << MODE_WRITE_STRING << endl
	          << MODE_HELP_STRING_1 << endl << endl
	          << "Type \"m3di " << MODE_HELP_STRING_1 << "\" for help." << endl;
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
