/*
 *   Copyright (C) 2019-2021 Rafael M. Siejakowski
 *   License information at the end of the file.
 */
#include "io.h"
// =============================================================================================
void print_json(Json::OStream* destination, const Json::Value& data)
{
	if (!destination || !data)
	{
		std::cerr << "Error in JSON output!" << std::endl;
		return;
	}
	Json::StreamWriterBuilder builder;
	builder["indentation"] = "\t";
	builder.settings_["precision"] = 320;
	std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
	writer->write(data, destination);
	*destination << std::endl;
}
// =============================================================================================
cmdline_data parse_cmdline(const char** argv)
/*
 * Parses the command line arguments and fills the fields in a struct cmdline_data
 * which is returned to the caller.
 *
 * Arguments in argv and their conversions:
 * [0] : executable path   --> ignored
 * [1] : {integrate|write} --> already handled
 * [2] : JSON file path    --> const char*
 * [3] : Re(hbar)          --> double }
 * [4] : Im(hbar)          --> double } --> std::complex<double>
 * [5] : samples           --> int
 *
 */
{
	cmdline_data result;
	double Rehbar = parse_double(argv[3]);
	double Imhbar = parse_double(argv[4]);
	result.hbar = std::complex<double> {Rehbar,Imhbar};
	result.samples = parse_int(argv[5]);
	result.filepath = argv[2];
	result.valid = validate_q_and_samples(Rehbar, result.samples);
	return result;
}
// =============================================================================================
std::string format_complex_strings(const char* re, const char* im)
/*
 * Returns a string concatenating the textual representations
 * of the real and imaginary parts of a complex number.
 *
 * For example, the arguments ("42", "-0.69") output std::string("42-0.69i")
 *
 */
{
	std::ostringstream formatter;
	bool imag_nonnegative = (parse_double(im) >= 0);
	formatter << re << (imag_nonnegative? "+":"") << im << "i";
	return formatter.str();
}
// =============================================================================================
double parse_double(const char* input) noexcept
/*
	A simple wrapper around std:stod.
*/
{
	try
	{
		return std::stod(std::string(input));
	}
	catch (std::invalid_argument& e)
	{
		std::cerr << "Invalid floating point number: '" << input << "'!" << std::endl;
		return 0.0;
	}
	catch (...)
	{
		std::cerr << "Unhandled exception when converting a floating point number: '"
			<< input << "'!" << std::endl;
		return 0.0;
	}
}
// =============================================================================================
int parse_int(const char* input) noexcept
/*
	A simple wrapper around std::stoi.
*/
{
	try
	{
		return std::stoi(std::string(input));
	}
	catch (std::invalid_argument& e)
	{
		std::cerr << "Invalid integer number: '" << input << "'!" << std::endl;
		return 0;
	}
	catch (...)
	{
		std::cerr << "Unhandled exception when converting an integer: '"
			<< input << "'!" << std::endl;
		return 0;
	}
}
//==============================================================================================
bool validate_q_and_samples(double Rehbar, int samples)
/*
	Checks if the values of hbar and samples have been specified correctly.
	Returns true on valid data, false on invalid data.
*/
{
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
// =============================================================================================
int make_divisible(int n, int d)
/*
	Returns the smallest integer greater than |n| and divisible by d
*/
{
	if (n<0)
		n = -n;
	if (d==0)
		return n;
	else if (d<0)
		d = -d;
	if (n==0)
		return d;
	return (n/d + 1)*d;
}
// ================================================================================================
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
