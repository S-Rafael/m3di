/*
 *   Copyright (C) 2019-2020 Rafael M. Siejakowski
 *   License information at the end of the file.
 */
#ifndef __PRECOMPUTE_H__
#define __PRECOMPUTE_H__

#include <thread>
#include <iostream>
#include <vector>
#include "transcendental.h"

/*
 *  class precomputed
 *
 * This class precomputes and stores the values of the factors G_q(w) at sample points of
 * the form w = e^(alpha*hbar/pi) * z, with |z|=1.
 *
 * Each 'precomputed' object stores a single sequence of values, with z ranging over the
 * points exp(2*pi*i * k/samples) for k=0,1,...,samples-1, and where alpha and hbar are
 * fixed. The computation is launched by the class constructor, which allocates a buffer
 * storing the results. The buffer is implemented as std::vector. Thus, deallocation is 
 * done automatically by the defaulted class destructor.
 * 
 * Other public member functions:
 *
 * std::complex<double> get(int position) - returns the precomputed value at the index
 *                                          given by 'position'. In other words, this value
 *                                          is G_q(e^(alpha*hbar/pi + 2*pi*i * k/samples)),
 *                                          where k=position.
 *
 * void finish()                          - finishes the precomputation. This function will
 *                                          block until the precomputation thread exits.
 *
 */

class precomputed
{
	private:
	std::complex<double> prefactor; // this will store the circle radius exp(hbar * a)
	std::complex<double> q; // the parameter q = exp(hbar)
	double startangle; // the initial angle
	double step; // distance between consecutive sample points
	int length; // number of sample points
	bool ready; // whether the computation is done
	std::vector<std::complex<double>> buffer; // buffer to store computed values

	static void thread_worker(precomputed* obj); // static member function serving as thread main
	std::unique_ptr<std::thread> iteration; // unique pointer to the thread object

	public:
	precomputed(double initial_a, std::complex<double> hbar, int samples); // custom constructor
	~precomputed() = default; // default destructor
	std::complex<double> get(int position) const; // retrieves the stored value at 'position'
	void finish(); // wait for the thread to join.
};

#endif

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
