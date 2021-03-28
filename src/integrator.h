/*
 *   Copyright (C) 2019-2021 Rafael M. Siejakowski
 *   License information at the end of the file.
 */

#ifndef __INTEGRATOR_H__
#define __INTEGRATOR_H__

#include <json/json.h>
#include <string>
#include <thread>
#include "manifold.h"
#include "io.h"
#include "kahan.h"

/*
 * class integrator
 *
 * This class stores the information specific to the computation
 * of the state integral for the meromorphic 3D-index and performs this
 * computation. Currently, only the quadrature via Riemann sums
 * ("rectangle rule") is implemented.
 *
 * Public member functions of class integrator:
 *
 * integrator(M, hbar, samples)            - class constructor. 
 *                                           Here, M is an object of mani_data type
 *                                           (see manifold.h), 
 *                                           'hbar' is the complex parameter of the state integral,
 *                                           'samples' is the number of sample points in each coordinate
 *                                           direction in the integration domain.
 * 
 * std::complex<double> compute_integral() - returns the value of the integral.
 *
 */

class integrator
{
	private:
	unsigned int samples; // how many sample points in each coordinate direction
	unsigned int num_threads; // how many concurrent threads to use for the integration
	unsigned int nesting; // dimension of the integration domain
	mani_data* M; // non-owning pointer to the manifold data object
	std::complex<double> hbar; // the complex parameter of the meromorphic 3D-index
	double step_length; // length of the base interval for Riemann sum
	public:
	integrator(mani_data& M, std::complex<double> hbar, unsigned int samples);
	~integrator() = default;
	std::complex<double> compute_integral(); // computes the value of the integrand
	private:
	std::complex<double> Fubini_recursion(std::vector<unsigned int>& initial_indices, 
		unsigned int from, unsigned int to) const; // performs Riemann summation recursively
	static void thread_worker(integrator* obj, std::complex<double>* output,
		 unsigned int from, unsigned int to); // static member function serving as thread main.
};

#endif
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
