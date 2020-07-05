/*
 *   Copyright (C) 2019-2020 Rafael M. Siejakowski
 *   License information at the end of the file.
 */
#ifndef __WRITE_H__
#define __WRITE_H__

#include <fstream>
#include <vector>
#include <json/json.h>
#include "manifold.h"
#include "constants.h"

/*
	This file declares the class multi_iterator and
	the function store_integrand_values.
	Both of them are specific to the 'write' mode.

	class multi_iterator is a simple iterator which
	iterates over the set [0,1,...,s-1]^d,
	where d and s are arbitrary positive integers.
	
	store_integrand_values computes the values of the integrand
	of the meromorphic 3D-index at sample points with prescribed
	density and stores them in a Json::Value data structure.
*/

void store_integrand_values(Json::Value& target, mani_data& M, int samples);

class multi_iterator
{
	private:
	unsigned int len; // size of each dimension
	unsigned int d; // number of dimensions
	std::vector<unsigned int> buffer; // buffer to store the current indices
	public:
	multi_iterator(unsigned int length, unsigned int depth);
	inline std::vector<unsigned int> item() {return buffer;};
	bool advance();
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
