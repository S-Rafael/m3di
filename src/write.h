#ifndef __WRITE_H__
#define __WRITE_H__

#include <fstream>
#include <vector>
#include "manifold.h"

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
