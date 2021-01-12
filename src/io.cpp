/*
 *   Copyright (C) 2019-2020 Rafael M. Siejakowski
 *   License information at the end of the file.
 */
#include "io.h"

// ================================================================================================
double parse_double(const char* input)
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
}
// ================================================================================================
int parse_int(const char* input)
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
}
//==========================================================================================
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
// ================================================================================================
int decide_thread_count(int N, int samples)
/*    
 *    N       – number of tetrahedra
 *    samples – number of sample points per circle
 *
 *    Returns an optimal number of threads to split the calculation into,
 *    based on the constants defined in the header file
 */
{	
	// Get hardware concurrency
	int concurrency = std::thread::hardware_concurrency();
	if (concurrency < reasonable_concurrency)
		concurrency = reasonable_concurrency;
	// There are at most N-1 nested integrals to compute, so we
	// need to estimate (samples)^(N-1); we divide it by 10^thread_base
	double s = std::pow(samples, N-1) * std::pow(10, -thread_base);
	int optimal_threads = static_cast<int>(std::ceil(s));
	if (optimal_threads > concurrency * overload) // we don't have that many cores...
		optimal_threads = concurrency * overload;
	return optimal_threads;
}
// ================================================================================================
int make_divisible(int n, int d)
/*
	Returns the smallest integer greater than |n| and divisible by d
*/
{
	if (d<0)
		d = -d;
	if (n<0)
		n = -n;
	if (d==0)
		return n;
	if (n==0)
		return d;
	return (n/d + 1)*d;
}
// ================================================================================================
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
