/*
 *   Copyright (C) 2019-2020 Rafael M. Siejakowski
 *   License information at the end of the file.
 */
#ifndef __WRITE_H__
#define __WRITE_H__

#include <fstream>
#include <vector>
#include "manifold.h"
#include "modes.h"

void write_mode(int argc, char** argv);
void store_integrand_values(Json::Value &target, mani_data &M, int samples);

// This class iterates over a multidimensional array
class multi_iterator
{
	unsigned int len;
	unsigned int d;
	unsigned int* buffer;
	public:
	multi_iterator(unsigned int length, unsigned int depth);
	~multi_iterator();
	inline unsigned int* item() {return buffer;};
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
