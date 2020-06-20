/*
 *   Copyright (C) 2019-2020 Rafael M. Siejakowski
 *   License information at the end of the file.
 */
#ifndef __MANIFOLD_H__
#define __MANIFOLD_H__
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <iomanip>
#include <json/json.h>
#include "precompute.h"

// Main class representing the combinatorics of a triangulation.
class mani_data
{
	private:
	bool valid_state, valid_precomp; // state variables
	int N, k; // Number of tetrahedra and cusps
	vector<double> angles; //initial angle structure (in units of pi)
	std::complex<double> cq_factor; // c(q)
	vector<int> LTD; // Leading-trailing matrix as a flattened vector
	precomputed** precomputed_quads; // to store precomputed G_q values
	bool read_json(char* filepath, Json::Value* root);
	bool populate(char* filepath);

	public:
	mani_data(char* filepath);
	~mani_data();
	int ltd_exponent(unsigned int* indices, int quad);
	void precompute(std::complex<double> hbar, int samples);
	// Some inline getters
	inline unsigned int num_tetrahedra() {return N;}
	inline bool is_valid() {return valid_state;}
	inline bool ready() {return (valid_state && valid_precomp);}
	// this one is more complicated but the loop has N iterations only
	inline std::complex<double> get_integrand_value(unsigned int* yindices) {
			std::complex<double> prod = 1.0; // we multiply tetrahedral weigths
			for (int j = 0; j < N; j++) // weight of tetrahedron j:
				prod *= precomputed_quads[3*j  ]->get(ltd_exponent(yindices, 3*j  ))
				      * precomputed_quads[3*j+1]->get(ltd_exponent(yindices, 3*j+1))
				      * cq_factor
				      * precomputed_quads[3*j+2]->get(ltd_exponent(yindices, 3*j+2));
			return prod;
	}
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
