#ifndef __INTEGRALS_H__
#define __INTEGRALS_H__

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

///std::complex<double> integral(double n, int samples);
void subslice(std::complex<double>* output, int level, unsigned int* initial_indices, integration_domain* D, mani_data* M);
std::complex<double> GK_integral(mani_data* M, std::complex<double> hbar, int samples);
int decide_thread_count(int N, int samples);
int make_divisible(int n, int d);

#endif
