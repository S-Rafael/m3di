/*
 *   Copyright (C) 2019-2021 Rafael M. Siejakowski.
 *   All rights reserved.
 *   License information at the end of the file.
 */

#include <iostream>
#include <complex>

#include "tabulation.h"

/**
 * @file
 * Implementation of member functions of class 'tabulation'
*/
// ================================================================================================
/**
 * @brief
 * Constructs the object and immediately launches the tabulation
*/
tabulation::tabulation(double initial_a, std::complex<double> hbar, int samples):
	length {samples}, ready {false}, iteration {nullptr}
{
	if (length < 1)
		return;
	//Initialize variables needed for the tabulation
	step = twopi / static_cast<double>(length);
	buffer.reserve(length);
	q = std::exp(hbar);
	startangle = initial_a * π;
	radius = std::exp(hbar * initial_a);
	bool is_real = (hbar.imag() == 0);
	// Everything is set up, so we can start the precomputation thread:
	iteration = std::make_unique<std::thread>(thread_main, this, is_real);
}
// ------------------------------------------------------------------------------------------------
/**
 * @brief
 * A static member function serving as the thread main for the tabulation thread.
 * @param
 * real_q - informs us whether the parameter hbar is real.
 * @remark
 * Note that when hbar is real, then q and "radius" are also real,
 * and this speeds up computations.
*/
void tabulation::thread_main(tabulation* obj, bool real_q)
{
	double alpha = obj->startangle;
	double step = obj->step;
	int len = obj->length;
	if (real_q)
	{   // Special case of real hbar, q and radius
		double q = obj->q.real();
		double r = obj->radius.real();
		for (int k=0; k<len; k++)
		{
			obj->buffer.push_back(
						G_q<double>(q,
		/* z: */     std::polar<double>(r, alpha + (static_cast<double>(k) * step))
								   )
								 );
		}
	}
	else
	{   // General case of complex hbar; may be slower than otherwise
		std::complex<double> q = obj->q;
		std::complex<double> r = obj->radius;
		for (int k=0; k<obj->length; k++)
		{
			obj->buffer.push_back(
				G_q< std::complex<double> >(q,
						r * std::polar<double>(1.0,
							alpha + (static_cast<double>(k) * step)
											  )
										   )
								 );
		}
	}
}
// ------------------------------------------------------------------------------------------------
/**
 * @brief
 * Retrieves the precomputed value at the given position.
 * There's a simple bounds check, so 'position' is essentially
 * reduced mod 'length' to yield a valid index.
*/
std::complex<double> tabulation::get(int position) const
{ 
	while (position > length)
		position -= length;
	while (position < 0)
		position += length;
	return buffer[position];
}
// ------------------------------------------------------------------------------------------------
/**
 * @brief
 * Waits for the precomputation thread to join before
 * returning control to the parent thread.
*/
void tabulation::finish()
{
	if (ready)
		return;
	if (iteration->joinable())
	{
		iteration->join();
		ready = true;
	}
	else
		std::cerr << "Error in a precomputation thread!" << std::endl;
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
