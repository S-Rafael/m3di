/*
 *   Copyright (C) 2019-2021 Rafael M. Siejakowski.
 *   All rights reserved.
 *   License information at the end of the file.
 */
#ifndef __MANIFOLD_H__
#define __MANIFOLD_H__

#include <json/json.h>
#include <complex>
#include <vector>

#include "tabulation.h"

#define TRIM_LTD // Makes the program store only the first N-k rows of the LTD matrix

/**
 * @remarks
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
 * tabulate(hbar, samples)         - Precomputes the values of G_q(...) occurring as factors
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

/**
 * @brief The mani_data class stores information about a triangulated 3-manifold
 */
class mani_data
{
private:
	int nesting=1; // dimension of integration domain
	int num_quads=6; // Number of quads
	std::vector<int> LTD; // Leading-trailing matrix as a flattened vector
	std::vector<double> angles; //initial angle structure (in units of pi)
	std::vector< std::shared_ptr<tabulation> > G_q_tables; // tabulated values of G_q
	std::complex<double> prefactor; // [c(q)]^N
	int k=1; // Number of cusps; currently always 1
	int N=2; // Number of tetrahedra
	bool valid_state=false, valid_tabulation=false; // state variables

	// private IO member functions
	bool read_json(const char* filepath, Json::Value* root);
	bool populate(const char* filepath);

public:
	// cdtors
	mani_data(const char* filepath);
	~mani_data() = default;
	// Tabulation routine
	void tabulate(std::complex<double> hbar, int samples);
	// Some inline getters:
	inline unsigned int num_tetrahedra() const {return N;}
	inline unsigned int num_cusps() const {return k;}
	inline bool is_valid() const {return valid_state;}
	inline bool ready() const {return (valid_state && valid_tabulation);}
	inline std::complex<double> get_prefactor() const {return prefactor;}
	// -------------------------------------------------------------------------
	/**
	 * @brief Computes the dot product with l(quad)
	 *        (column of the LTD matrix coresponding to the quad)
	 * @param indices - the vector of indices
	 * @param quad - index of the quad
	 * @return dot product of indices and the column of L at index `quad`
	 */
	inline int ltd_exponent(std::vector<unsigned int>& indices, int quad) const
	{
		int sum = indices[0] * LTD[quad]; // edge == 0
		for (int edge=1; edge<nesting; edge++)
			sum += indices[edge] * LTD[(num_quads*edge) + quad];
		return sum;
	}
	// -------------------------------------------------------------------------
	/**
	 * @brief
	 * Computes the value of the integrand at the prescribed indices
	 */
	inline std::complex<double> get_integrand_value(std::vector<unsigned int>& indices) const
	{
		std::complex<double> prod = G_q_tables[0]->get(ltd_exponent(indices, 0));
		for (int quad = 1; quad < num_quads; quad++)
			prod *= G_q_tables[quad]->get(ltd_exponent(indices, quad));
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
