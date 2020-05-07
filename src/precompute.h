#ifndef __PRECOMPUTE_H__
#define __PRECOMPUTE_H__

#include <thread>
#include "transcendental.h"

class precomputed
{
	std::complex<double>* buffer;
	std::complex<double> prefactor;
	std::complex<double> q;
	double startangle;
	double step;
	int length;
	static void thread_worker(precomputed* obj);
	bool ready;
	std::thread* iteration;
	public:
	precomputed(double initial_a, std::complex<double> hbar, int samples);
	~precomputed();
	std::complex<double> get(int position);
	void finish();
};


#endif
