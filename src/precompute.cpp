#include "precompute.h"

// ================================================================================================
//  precomputed class -- precomputes and stores values of G_q at sample points of
//  the form e^(alpha*hbar/pi) * z, with |z|=1.
// ================================================================================================
precomputed::precomputed(double initial_a, std::complex<double> hbar, int samples):
	length(samples), ready(false), iteration(NULL)
{
	if (!length)
		return;
	//Initialize variables needed for the pre-computation
	step = twopi / static_cast<double>(length);
	buffer = new std::complex<double>[length]; //will store the values
	q = exp(hbar);
	startangle = initial_a * pi;
	prefactor = exp(hbar * initial_a);
	iteration = new std::thread(thread_worker, this); //starts precomputation thread.
}
// ------------------------------------------------------------------------------------------------
void precomputed::thread_worker(precomputed* obj)
{
	// Computes the values of G_q() for a particular quad
	for (int k=0; k<obj->length; k++)
		obj->buffer[k] = G_q(obj->q, 
		        obj->prefactor 
		          * std::polar<double>(1.0, obj->startangle + static_cast<double>(k) * obj->step));
}
// ------------------------------------------------------------------------------------------------
precomputed::~precomputed()
{
	if (length)
		delete [] buffer;
}
// ------------------------------------------------------------------------------------------------
std::complex<double> precomputed::get(int position)
{ 
	int index = position % length;
	while (index < 0)
		index += length;
	return buffer[index];
}
// ------------------------------------------------------------------------------------------------
void precomputed::finish()
{
	// Finish all branched threads and return control to the parent thread
	if (ready)
		return;
	if (iteration->joinable())
	{
		iteration->join();
		ready = true;
		delete iteration;
	}
	else
		std::cerr << "Error in a precomputation thread!" << std::endl;
}
// ------------------------------------------------------------------------------------------------

