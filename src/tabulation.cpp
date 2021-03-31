/*
 *   Copyright (C) 2019-2021 Rafael M. Siejakowski
 *   License information at the end of the file.
 */
#include "tabulation.h"

/*
    Implementation of member functions of class tabulation.
*/
// =============================================================================================
tabulation::tabulation(double initial_a, CC hbar, int samples):
	length {samples}, ready {false}, iteration {nullptr}
/*
	Constructs the object and immediately launches the precomputation.
*/
{
	if (length < 1)
		return;
	step = twopi / static_cast<double>(length);
	buffer.reserve(length); // allocate memory for the table of values
	q_real = (hbar.imag() == 0.0);
	q = exp(hbar); // q is of complex type but if q_real then we can ignore its imaginary part
	startangle = initial_a * pi;
	prefactor = exp(hbar * initial_a); // when q_real, prefactor is real too
	// Everything is set up, so we can start the precomputation thread:
	iteration = std::make_unique<std::thread>(thread_worker, this); 
}
// ---------------------------------------------------------------------------------------------
void tabulation::thread_worker(tabulation* obj)
/*
	This is a static member function serving as the thread main
	for the precomputation thread.
	It tabulates the values of G_q() for a particular quad.
*/
{
	if (obj->q_real) // TODO: replace with compile-time logic (templates)
	{
		// Special case of tabulation: obj->q has imaginary part == 0
		// and we only consider the real part of q;
		// likewise, obj->prefactor.imag() == 0.
		double q = obj->q.real();
		double r = obj->prefactor.real();
		for (int k=0; k<obj->length; k++)
		{
			obj->buffer.push_back(
				G_q(q, std::polar<double>(r, static_cast<double>(k) * obj->step)));
		}
	}
	else
	{
		// General case of complex q and complex prefactor
		CC q = obj->q;
		CC r = obj->prefactor;
		for (int k=0; k<obj->length; k++)
		{
			obj->buffer.push_back(
				G_q(q, r * std::polar<double>(1.0, static_cast<double>(k) * obj->step)));
		}
	}
}
// ---------------------------------------------------------------------------------------------
std::complex<double> tabulation::get(int position) const
/*
	Retrieves the precomputed value at the given position.
	There's a simple bounds check, so 'position' is essentially
	reduced mod 'length' to yield a valid index.
*/
{ 
	while (position > length)
		position -= length;
	while (position < 0)
		position += length;
	return buffer[position];
}
// ---------------------------------------------------------------------------------------------
void tabulation::finish()
/*
	Waits for the precomputation thread to join before
	returning control to the parent thread.
*/
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
// =============================================================================================
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
