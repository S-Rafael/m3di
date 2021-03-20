/*
 *   Copyright (C) 2019-2021 Rafael M. Siejakowski
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

/*
 * class mani_data
 * 
 * This is the main class representing the necessary triangulation data and storing
 * precomputed values of the functions G_q which appear as factors of the integrand.
 *
 * Public member functions:
 *
 * mani_data(char* filepath)       - class constructor. Takes in the path to a JSON file.
 * 
 * ~mani_data()                    - destructor
 * 
 * int ltd_exponent(indices, quad) - returns t*l(□), where t is the vector 'indices' and
 *                                   □ is the normal quad with the index 'quad'.
 *
 * precompute(hbar, samples)       - Precomputes the values of G_q(...) occurring as factors
 *                                 - of the integrand.
 *
 * unsigned int num_tetrahedra()   - returns the number of tetrahedra in the triangulation
 * 
 * bool is_valid()                 - tells whether the object has been initialized correctly
 *                                   and is in a valid state
 *
 * bool ready()                    - tells whether the values of G_q(...) factors have been
 *                                   precomputed successfully, so that the integrand can be
 *                                   evaluated
 *
 * std::complex<double> get_integrand_value(indices) 
 *                                 - returns the value of the integrand at the point defined
 *                                   by the indices. Each index runs from 0 to samples.
 *
 */

class mani_data
{
	private:
	int N, k; // Number of tetrahedra and cusps
	std::vector<int> LTD; // Leading-trailing matrix as a flattened vector
	std::vector<double> angles; //initial angle structure (in units of pi)
	std::vector< std::shared_ptr<precomputed> > precomputed_quads;
	std::complex<double> cq_factor; // c(q)
	bool valid_state, valid_precomp; // state variables
	// private IO member functions
	bool read_json(const char* filepath, Json::Value* root);
	bool populate(const char* filepath);

	public:
	// cdtors
	mani_data(const char* filepath);
	~mani_data() = default;

	inline int ltd_exponent(std::vector<unsigned int>& indices, int quad) const
	/*
	 *	Returns the dot product of the indices with l(quad)
	 *  (column of the LTD matrix coresponding to the quad).
	 */
	{
		int sum=0;
		for (int edge=0; edge<N-1; edge++) // last edge variable omitted
			sum += indices[edge] * LTD[(3*N*edge) + quad];
		return sum;

	}
	void precompute(std::complex<double> hbar, int samples);
	// Some inline getters:
	inline unsigned int num_tetrahedra() const {return N;}
	inline unsigned int num_cusps() const {return k;}
	inline bool is_valid() const {return valid_state;}
	inline bool ready() const {return (valid_state && valid_precomp);}
	inline std::complex<double> get_integrand_value(std::vector<unsigned int>& indices) const
	/*
	 *	Computes the value of the integrand at the prescribed indices
	 *	TODO: Measure performance and optimize this loop
	 */
	{
			std::complex<double> prod = 1.0;
			// Assemble integrand value by multiplying tetrahedral weigths:
			for (int j = 0; j < N; j++) // weight of tetrahedron j:
				prod *= precomputed_quads[3*j  ]->get(ltd_exponent(indices, 3*j  ))
				      * precomputed_quads[3*j+1]->get(ltd_exponent(indices, 3*j+1))
					  * cq_factor // maybe putting this here increases numerical stability?
				      * precomputed_quads[3*j+2]->get(ltd_exponent(indices, 3*j+2));
			return prod;
	}
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
