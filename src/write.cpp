/*
 *   Copyright (C) 2019-2020 Rafael M. Siejakowski
 *   License information at the end of the file.
 */
#include "write.h"

// =================================================================================================
void store_integrand_values(Json::Value& target, mani_data& M, int samples)
/*
	This function fills 'target' with values of the meromorphic
	3D-index integrand for M at sample points.
	'samples' is the number of evenly spacesd sample points 
	in each coordinate direction of the integration domain.
*/
{
	std::complex<double> val; // to store integrand value
	unsigned int d =  M.num_tetrahedra() - M.num_cusps(); // dimension of integration domain
	double step = twopi/static_cast<double>(samples); // distance between adjacent samples
	multi_iterator indices = multi_iterator(samples, d); // d-dimensional iterator
	do
	{
		Json::Value pts_array;
		std::vector<unsigned int> current_indices = indices.item();
		val = M.get_integrand_value(current_indices);
		// compute actual coordinates of the sample point:
		for (unsigned int i=0; i<d; i++)
			pts_array.append(step * static_cast<double>(current_indices[i]));
		// Fill in a Json::Value point structure
		Json::Value point;
		point["t"] = pts_array;
		if (val == INFTY)
		{
			point["real"] = "infinity";
			point["imag"] = "infinity";
		}
		else
		{
			point["real"] = val.real();
			point["imag"] = val.imag();
		}
		target["points"].append(point); 
	} while (indices.advance());
}
// =================================================================================================
/*
	Implementation of member function of class multi_iterator
*/
// =================================================================================================
multi_iterator::multi_iterator(unsigned int length, unsigned int depth)
/*
	Class constructor
*/
: len {length}, d {depth}, buffer {depth, 0} {}
// -------------------------------------------------------------------------------------------------
bool multi_iterator::advance()
/*
	Advances the iterator by a step.
	Returns true if iteration can be continued,
	false if the iteration has reached its end.
*/
{
	unsigned int pos = 0; // currently incremented position
	while (pos < d && buffer[pos] == len-1) // about to overflow: increase pos
	{
		buffer[pos] = 0;
		pos++;
	}
	if (pos == d) // reached the end
		return false;
	buffer[pos]++;
	return true;
}
// =================================================================================================
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
