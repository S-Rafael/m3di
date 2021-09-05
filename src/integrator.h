/*
 *   Copyright (C) 2019-2021 Rafael M. Siejakowski.
 *   All rights reserved.
 *   License information at the end of the file.
 */

#ifndef __INTEGRATOR_H__
#define __INTEGRATOR_H__

#include <complex>
#include <vector>

#include "manifold.h"
#include "stats.h"

/*
 * class integrator
 *
 * This class stores the information specific to the computation of
 * the state integral for the meromorphic 3D-index and performs this
 * computation. Currently, only the quadrature via Riemann sums
 * ("rectangle rule") is implemented.
 *
 */

class integrator
{
private:
	unsigned samples;          // how many sample points in each coordinate direction
	unsigned num_threads;      // how many concurrent threads to use for the integration
	unsigned nesting;          // dimension of the integration domain
	mani_data* M;              // non-owning pointer to the manifold data object
	std::complex<double> hbar; // the complex parameter of the meromorphic 3D-index
	double step_length;        // length of the base interval for Riemann sum
public:
	integrator(mani_data& M, std::complex<double> hbar, unsigned samples);
	~integrator() = default;
	std::complex<double> compute_integral(stats& S); // computes the value of the integrand
private:
	std::complex<double> Fubini_recursion(std::vector<unsigned>& initial_indices,
		unsigned from, unsigned to) const; // performs Riemann summation recursively
	static void thread_main(integrator* obj, std::complex<double>* output,
		 unsigned from, unsigned to); // static member function serving as thread main.
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
