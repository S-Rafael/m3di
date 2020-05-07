#include "write.h"

void write_mode(int argc, char** argv)
/*
	This function implements the write mode, which outputs the integrand values as JSON.
*/
{
	// Parse hbar and sample count
	double Rehbar = parse_double(argv[3]);
	double Imhbar = parse_double(argv[4]);
	int samples = parse_int(argv[5]);
	if (samples < 1)
	{
		std::cerr << "Error: The number of samples must be a positive integer!" << std::endl;
		return;
	}
	std::complex<double> hbar = std::complex<double>(Rehbar, Imhbar);
	double a = exp(Rehbar); // a = |q|
	if (a < DBL_MIN || a >= 1.0)
	{
		std::cerr << "Error: The value of q specified does not satisfy 0<|q|<1!" << std::endl;
		return;
	}
	// Read in manifold info
	mani_data M = mani_data(argv[2]); 
	if (!M.is_valid())
	{
		std::cerr << "File '" << argv[2] << "' doesn't contain a valid "
		             "manifold specification!" << std::endl;
		return;
	}
	M.precompute(hbar, samples);
	if (!M.ready())
	{
		std::cerr << "Error while computing integrand values." << std::endl;
		return;
	}
	// Create JSON representation of output
	Json::Value output;
	output["samples"] = samples;
	output["hbar_real_part"] = Rehbar;
	output["hbar_imag_part"] = Imhbar;
	// Compute the integrand values and store them in output
	store_integrand_values(output, M, samples);
	// Format the given hbar_value
	std::ostringstream hbar_given;
	hbar_given << argv[3] << (Imhbar<0.0? "":"+") << argv[4] << "i";
	output["hbar_given"] = hbar_given.str();
	// Output data
	Json::StreamWriterBuilder builder;
	builder["indentation"] = "\t";
	builder.settings_["precision"] = 320;
	std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
	writer->write(output, &std::cout);
	std::cout << std::endl;
}

// ================================================================================================
void store_integrand_values(Json::Value &target, mani_data &M, int samples)
/*
	This function fills target with values of the meromorphic
	3D-index integrand on M at sample points.
*/
{
	std::complex<double> val;
	int d =  M.num_tetrahedra() - 1;
	double* pts = new double[d];
	double step = twopi/static_cast<double>(samples);
	multi_iterator indices = multi_iterator(samples, d);
	do
	{
		Json::Value pts_array;
		val = M.get_integrand_value(indices.item());
		for (int i=0; i<d; i++)
			pts_array.append(step * static_cast<double>(indices.item()[i]));
		Json::Value point;
		point["t"] = pts_array;
		point["real"] = val.real();
		point["imag"] = val.imag();
		target["points"].append(point); 
	} while (indices.advance());
	delete [] pts;
}
// ================================================================================================
multi_iterator::multi_iterator(unsigned int length, unsigned int depth)
{
	len = length;
	d = depth;
	buffer = new unsigned int[depth];
	for (unsigned int i=0; i<depth; i++)
		buffer[i]=0;
}
// -------------------------------------------------------------------------------------------------
multi_iterator::~multi_iterator()
{
	delete [] buffer;
}
// -------------------------------------------------------------------------------------------------
bool multi_iterator::advance()
{
	unsigned int pos = 0;
	while (pos < d && buffer[pos] == len-1)
	{
		buffer[pos] = 0;
		pos++;
	}
	if (pos == d)
		return false;
	buffer[pos]++;
	return true;
}
// -------------------------------------------------------------------------------------------------
