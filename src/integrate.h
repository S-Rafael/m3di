/*
 *   Copyright (C) 2019-2020 Rafael M. Siejakowski
 *   License information at the end of the file.
 */

#ifndef __INTEGRALS_H__
#define __INTEGRALS_H__

#include <json/json.h>
#include <string>
#include "transcendental.h"
#include "precompute.h"
#include "manifold.h"

class integration_domain
{
	int dimension, samples;
	int* froms;
	int* tos;
	bool* set;
	bool allocated;
	public:
	integration_domain(int dim, int sam);
	~integration_domain();
	bool all_set();
	void set_interval(int variable, double from, double to);
	unsigned int get_from(int variable);
	unsigned int get_to(int variable);
	inline int get_samples() {return samples;}
	inline double step_length() {return (samples)? 1.0/static_cast<double>(samples): 1.0;}
};

void integrate_mode(int argc, char** argv);
void subslice(std::complex<double>* output, int level, 
	unsigned int* initial_indices, integration_domain* D, mani_data* M);
std::complex<double> GK_integral(mani_data* M, std::complex<double> hbar, int samples);
int decide_thread_count(int N, int samples);
int make_divisible(int n, int d);

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
