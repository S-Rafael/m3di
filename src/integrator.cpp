/*
 *   Copyright (C) 2019-2021 Rafael M. Siejakowski.
 *   All rights reserved.
 *   License information at the end of the file.
 */

#include <json/json.h>
#include <string>
#include <thread>
#include <iostream>

#include "manifold.h"
#include "io.h"
#include "kahan.h"
#include "stats.h"

#include "integrator.h"

/**
 * @file
 * Implementation of member functions of the class `integrator`
*/
// ================================================================================================
/**
 * @brief
 * Constructor of class `integrator`.
 */
integrator::integrator(mani_data& Triangulation,
					   std::complex<double> given_hbar,
					   unsigned sam) :
	num_threads {1},
	hbar {given_hbar}
{
	if (sam < 1) sam = 1; // Make sure there's at least one sample point
	M = &Triangulation;   // Store a pointer to the triangulation data
	int N = M->num_tetrahedra();
	int k = M->num_cusps();
	nesting = N-k; // N-k nested integrals

	num_threads = std::thread::hardware_concurrency();
	if (num_threads > 1)
		samples = make_divisible(sam, num_threads);
	else
		samples = sam;
	step_length = 1.0/static_cast<double>(samples);
}
// ------------------------------------------------------------------------------------------------
/**
 * @brief
 * Computates the state integral using all of the data stored in the object.
 * Along the way, we inform the object Statistics about our progress.
 */
std::complex<double> integrator::compute_integral(stats& Statistics)
{
	Statistics.set_num_threads(num_threads); // Inform stats about num_threads
	M->tabulate(hbar, samples); // Tabulate the factors of the integrand
	Statistics.signal(stats::messages::finish_tabulation);

	// Prepare parameters needed to compute the integral
	std::complex<double> integral {0.0};
	unsigned samples_per_thread = samples/num_threads; // divisible by design
	std::vector<std::thread> threads(num_threads);
	std::vector< std::complex<double> > thread_results(num_threads);

	// We split the integration domain between the threads:
	for (unsigned t = 0; t < num_threads; t++)
	{
		unsigned from = t     * samples_per_thread;
		unsigned to   = (t+1) * samples_per_thread;
		// Launch integration thread with output pointer
		// set to t+thread_results.data()
		threads[t] = std::thread(thread_main,
								 this,
								 t + thread_results.data(),
								 from,
								 to);
	}
	// Threads are now running in parallel.
	for (auto& th : threads)
	{
		if (th.joinable())
			th.join();
		else
			std::cerr << "Error: unable to join a thread!" << std::endl;
	}

	// Threads are joined, we may now read the output array
	KN_accumulator thread_sum;
	thread_sum.accumulate(thread_results);
	integral = thread_sum;

	// The result is the integral times the constant prefactor:
	return integral * M->get_prefactor();
}
// ------------------------------------------------------------------------------------------------
/**
 * @brief
 *	Recursively computes a multidimensional Riemann sum over a cube of arbitrary dimension.
 */
/*  Indices of sample points have the form
 *	(v[0], v[1], ..., v[size()-1], k, ...),
 *	where v = initial_indices, and k runs from 'from' to 'to'.
 *	(In general, this allows us to run over a subset of a range, as in a thread worker.)
 *	If all indices are defined (there are not "dots" at the end), this is just a plain
 *	1-dimensional Riemann sum, where k plays the role of the summation index.
 *	Otherwise, we use a recursive call (Fubini's theorem).
 */
std::complex<double> integrator::Fubini_recursion(std::vector<unsigned>& initial_indices,
	unsigned from, unsigned to) const
{
	if (from >= to) // Nothing to compute
		return 0.0;

	// Copy initial indices
	std::vector<unsigned> indices = initial_indices;
	unsigned last_index = initial_indices.size();
	indices.resize(last_index + 1); // make sure there's an extra item at the end

	// Compute the Riemann sum
	KN_accumulator sum;
	if (last_index + 1 == nesting)
	/*
	 * We are iterating over the last index.
	 * Since all indices will be known, we can actually compute the Riemann sum.
	*/
	{	// Loop while changing the last index
		for (unsigned k = from; k < to; k++)
		{
			indices[last_index] = k;
			sum += M->get_integrand_value(indices);
		}	
	}
	else
	/*
		Not all indices are known yet; we must use recursion.
	*/
	{	// Loop while changing the last index
		for (unsigned k = from; k < to; k++)
		{
			indices[last_index] = k;
			sum += Fubini_recursion(indices, 0, samples);
		}	
	}
	// Multiply the sum of values by the length of the sample interval
	return step_length * std::complex<double>(sum);
}
// ------------------------------------------------------------------------------------------------
/**
 * This static member function serves as the thread main for
 * the integration threads.
 */
void integrator::thread_main(integrator* obj, std::complex<double>* output,
	 unsigned from, unsigned to)
{
	std::vector<unsigned> empty {};
	*output = obj->Fubini_recursion(empty, from, to);
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
