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
	bool valid_state, valid_precomp;
	int N, k;
	double* angles; //initial angle structure
	std::complex<double> cq_factor; 
	int* LTD;
	precomputed** precomputed_quads;
	bool read_json(char* filepath);
	public:
	mani_data(char* filepath);
	~mani_data();
	int ltd_exponent(unsigned int* indices, int quad);
	void precompute(std::complex<double> hbar, int samples);
	inline int num_tetrahedra() {return N;}
	inline bool is_valid() {return valid_state;}
	inline bool ready() {return (valid_state && valid_precomp);}
	inline std::complex<double> get_integrand_value(unsigned int* yindices) {
			std::complex<double> prod = COMPLEX_ONE; // we multiply tetrahedral weigths
			for (int j = 0; j < N; j++) // weight of tetrahedron j:
				prod *= precomputed_quads[3*j  ]->get(ltd_exponent(yindices, 3*j  ))
				      * precomputed_quads[3*j+1]->get(ltd_exponent(yindices, 3*j+1))
				      * cq_factor
				      * precomputed_quads[3*j+2]->get(ltd_exponent(yindices, 3*j+2));
			return prod;
	}
};

#endif
