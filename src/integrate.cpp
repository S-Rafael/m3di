/*
 *   Copyright (C) 2019-2020 Rafael M. Siejakowski
 *   License information at the end of the file.
 */
#include "integrate.h"

//==========================================================================================
void integrate_mode(int argc, char** argv)
/*
	This function implements the integration mode, which is the main mode of the program.
*/
{
	// Parse hbar and sample count
	double Rehbar = parse_double(argv[3]);
	double Imhbar = parse_double(argv[4]);
	int samples = parse_int(argv[5]);
	if (samples < 1)
	{
		std::cerr << "Error: The number of samples must be a positive integer!" << std::endl;
		return;
	}
	std::complex<double> hbar = std::complex<double>(Rehbar, Imhbar);
	double a = exp(Rehbar); // a = |q|
	if (a < DBL_MIN || a >= 1.0)
	{
		std::cerr << "Error: The value of q specified does not satisfy 0<|q|<1!" << std::endl;
		return;
	}
	// Read in manifold info
	mani_data M = mani_data(argv[2]); 
	if (!M.is_valid())
	{
		std::cerr << "File '" << argv[2] << "' doesn't contain a valid "
		             "manifold specification!" << std::endl;
		return;
	}
	// compute the meromorphic 3D-index
	std::complex<double> GK = GK_integral(&M, hbar, samples);
	// Format the given hbar_value
	std::ostringstream hbar_given;
	hbar_given << argv[3] << (Imhbar<0.0? "":"+") << argv[4] << "i";
	// Create JSON representation of output
	Json::Value output;
	output["hbar_given"] = hbar_given.str();
	output["samples"] = samples;
	output["hbar_real_part"] = Rehbar;
	output["hbar_imag_part"] = Imhbar;
	output["int_real_part"] = GK.real();
	output["int_imag_part"] = GK.imag();
	// Output data
	Json::StreamWriterBuilder builder;
	builder["indentation"] = "\t";
	builder.settings_["precision"] = 320;
	std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
	writer->write(output, &std::cout);
	std::cout << std::endl;
}
//============================================================================================
// function subslice()
// – computes an iterated integral over the torus.
//
// Arguments:
// output	  – where the function writes its output
// level	  – how many further integrals exist under this integral?
//                  (zero means we are at the deepest-nested integral)
// initial_indces – array containing indices of sample points. They are fixed
//                  (up to some point) by higher (containing) integrations;
//                  exactly one is modifiable by us, followed by the as-of-yet
//                  undetermined indices
// D              – a pointer to an integration_domain object containing a description
//                  of the domain of integration. This domain is the whole LTD-angle space
//                  in INTEGRATE_MODE and a subcube in SUBCUBE_MODE.
// M              – a pointer to a mani_data object containing all of the combinatorial
//                  information needed.
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
void subslice(std::complex<double>* output, int level, unsigned int* indices, integration_domain* D, mani_data* M)
{	// the last (deepest nested) integral is level 0
	// The integral runs over *indices; each invocation of subslice()
	// is only allowed to access indices[level]
	// (parallel threads have separate *indices)
	unsigned int from = D->get_from(level); //integration limits for our level of nesting
	unsigned int to   = D->get_to(level);
	unsigned int len = to - from;
	std::complex<double>* buffer = new std::complex<double>[len]; //stores function values
	if (level) 
		// we are NOT at the deepest-nested integral: use Fubini recursion to level-1:
		for (indices[level] = from; indices[level] < to; indices[level]++)
			subslice(&(buffer[indices[level] - from]), level-1, indices, D, M);
	else // we ARE at the deepest-nested integral: we can actually do the work
	     // (in particular, all indices in 'indices' are set).
		for (indices[level] = from; indices[level] < to; indices[level]++)
			buffer[indices[level] - from] = M->get_integrand_value(indices);
	// write to output:
	*output = D->step_length() * kn_sum(buffer, len);
	delete [] buffer;
}
// ================================================================================================
std::complex<double> GK_integral(mani_data* M, std::complex<double> hbar, int samples)
	/* Returns the approximate value of the state integral
	 * for the manifold specification stored in M and
	 * for the value of parameter hbar given. 
	 * The number of sample points on
	 * the circle is at least 'samples'
	 */
{
	// Prepare parameters needed to compute the integral
	int N = M->num_tetrahedra();
	int number_of_threads = decide_thread_count(N, samples);
	bool use_threads = (number_of_threads > 1);
	if (use_threads) // make sure the number of samples is divisible by the number of threads
		samples = make_divisible(samples, number_of_threads);
	// Precompute factors of the integrand
	M->precompute(hbar, samples);
	int integral_nesting = N-1; // how many nested integrals we need to compute
	int top_level = integral_nesting-1; // top level (so that the deepest integral has level 0)
	std::complex<double> result;
	// --------------------- Actual computation! --------------------------------------
	if (use_threads)
	{
		// Prepare threads and intermediate result array:
		std::thread** th = new std::thread*[number_of_threads];
		// Allocate a buffer for the threads' subdomain integrals
		std::complex<double>* intermediate = new std::complex<double>[number_of_threads];
		// Prepare integration domains and indices
		integration_domain** dom = new integration_domain*[number_of_threads];
		unsigned int** integration_indices = new unsigned int*[number_of_threads];
		for (int t=0; t<number_of_threads; t++)
		{
			// Create indices and subdomain for this thread
			integration_indices[t] = new unsigned int[integral_nesting];
			dom[t] = new integration_domain(integral_nesting, samples);
			// split the domain of integration for the 0th variable
			double from = static_cast<double>(t)/static_cast<double>(number_of_threads);
			double to = static_cast<double>(t+1)/static_cast<double>(number_of_threads);
			dom[t]->set_interval(0, from, to);
			th[t] = new std::thread(subslice, &(intermediate[t]), top_level,
			                        integration_indices[t], dom[t], M);
		}
		// Now all threads are working! After they finish, we clean them up:
		for (int t=0; t<number_of_threads; t++)
		{
			if (th[t]->joinable())
				th[t]->join();
			else
				std::cerr << "Error: something is wrong with thread #" << t << "!" << std::endl;
			delete th[t];
			delete dom[t];
			delete [] integration_indices[t];
		}
		result = kn_sum(intermediate, number_of_threads);
		delete [] intermediate;
		delete [] integration_indices;
		delete [] th;
		delete [] dom;
	}
	else // direct calculation without threads
	{		
		unsigned int* integration_indices = new unsigned int[integral_nesting];
		integration_domain* D = new integration_domain(integral_nesting, samples);
		subslice(&result, top_level, integration_indices, D, M);
		delete [] integration_indices;
		delete D;
	}
	return result;
}
// ================================================================================================
int decide_thread_count(int N, int samples)
	/*    Function decide_thread_count()
	 *    N       – number of tetrahedra
	 *    samples – number of sample points per circle
	 *
	 *    Returns an optimal number of threads to split the calculation into.
	 */
{	
	const int thread_base = 5; // take 1 thread per 10^(thread_base) data points.
	const int reasonable_concurrency = 8; // in case the processor won't tell us exactly
	const int overload = 8; // we may exceed hardware concurrency by this factor
	// 
	int concurrency = std::thread::hardware_concurrency();
	if (concurrency < reasonable_concurrency)
		concurrency = reasonable_concurrency;
	// There are N-1 nested integrals to compute, so we
	// need to estimate (samples)^(N-1); we divide it by 10^thread_base
	double s = std::pow(samples, N-1) * std::pow(10, -thread_base);
	int optimal_threads = static_cast<int>(std::ceil(s));
	if (optimal_threads > concurrency * overload)
		optimal_threads = concurrency * overload;
	return optimal_threads;
}
// ================================================================================================
// function make_divisible(int n, int d)
//
// Returns the smallest integer greater or equal n and divisible by d
//
int make_divisible(int n, int d)
{
	if (d<0)
		d = -d;
	int r = (n%d);
	if (r == 0) //already divisible
		return n;
	else
		return n + (d-r);
}
// ================================================================================================
// class integration_domain.
// TODO: Maybe we can do away with it!
//
// Stores starting and ending indices of integration over a subcube
// 
// -------------------------------------------------------------------------------------------------
// Class constructor
integration_domain::integration_domain(int d, int s):
dimension(d), samples(s), allocated(false)
{
	if (dimension < 1)
		return;
	// Allocate memory
	froms = new int[dimension];
	tos = new int[dimension];
	set = new bool[dimension];
	allocated = true;
	for (int i=0; i<dimension; i++)
		set[i] = false;
}
// -------------------------------------------------------------------------------------------------
integration_domain::~integration_domain()
{
	if (allocated)
	{
		delete [] froms;
		delete [] tos;
		delete [] set;
	}
}
// -------------------------------------------------------------------------------------------------
void integration_domain::set_interval(int variable, double from, double to)
{
	if (!allocated)
		return;
	if (variable < 0 || variable >= dimension)
		return;
	if (from > to) // limits swapped, need to get them right
	{
		double swap = from;
		from = to;
		to = swap;
	}
	double s = static_cast<double>(samples);
	int first_index = static_cast<int> (std::ceil(from*s));
	int last_index  = static_cast<int> (std::ceil(  to*s)) - 1;
	if (first_index < 0)
		first_index = 0;
	if (last_index > samples-1)
		last_index = samples-1;
	froms[variable] = first_index;
	tos[variable] = last_index+1;
	set[variable] = true;
}
// -------------------------------------------------------------------------------------------------
unsigned int integration_domain::get_from(int variable)
{
	if (!allocated)
		return 0;
	if (variable < 0 || variable >= dimension)
		return 0;
	if (!set[variable])
		return 0; // default from index is 0
	return froms[variable];
}
// -------------------------------------------------------------------------------------------------
unsigned int integration_domain::get_to(int variable)
{
	if (!allocated)
		return samples;
	if (variable < 0 || variable >= dimension)
		return samples;
	if (!set[variable])
		return samples; // default to index is samples
	return tos[variable];
}
// -------------------------------------------------------------------------------------------------
bool integration_domain::all_set()
{
	if (!allocated)
		return false;
	int i=0;
	while(i<dimension && set[i])
		i++;
	return (i == dimension);
}
// -------------------------------------------------------------------------------------------------
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
