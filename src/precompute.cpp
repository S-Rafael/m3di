/*
 *   Copyright (C) 2019-2020 Rafael M. Siejakowski
 *   License information at the end of the file.
 */
#include "precompute.h"

/*
	Implementation of member functions of class 'precomputed'.
*/
// ================================================================================================
precomputed::precomputed(double initial_a, std::complex<double> hbar, int samples):
	length {samples}, ready {false}, iteration {nullptr}
/*
	Constructs the object and immediately launches the precomputation.
*/
{
	if (length < 1)
		return;
	//Initialize variables needed for the pre-computation
	step = twopi / static_cast<double>(length);
	buffer.resize(length);
	q = exp(hbar);
	startangle = initial_a * pi;
	prefactor = exp(hbar * initial_a);
	// Everything is set up, so we can start the precomputation thread:
	iteration = std::make_unique<std::thread>(thread_worker, this); 
}
// ------------------------------------------------------------------------------------------------
void precomputed::thread_worker(precomputed* obj)
/*
	This is a static member function serving as the thread main
	for the precomputation thread.
*/
{
	// Compute the values of G_q() for a particular quad
	for (int k=0; k<obj->length; k++)
		obj->buffer[k] = G_q(obj->q, 
		                     obj->prefactor 
		                     * std::polar<double>(1.0,
		                                          obj->startangle + 
		                                          (static_cast<double>(k) * obj->step)));
}
// ------------------------------------------------------------------------------------------------
std::complex<double> precomputed::get(int position) const
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
// ------------------------------------------------------------------------------------------------
void precomputed::finish()
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
