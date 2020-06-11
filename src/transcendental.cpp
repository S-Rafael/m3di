#include "transcendental.h"

// ================================================================================================
// 
// ================================================================================================
std::complex<double> G_q(std::complex<double> q, std::complex<double> z) noexcept 
// This function returns G_q(z) for |q|<1.
{
	if (!nonzero(z))
		return INFTY; // at z=0, G_q(z) has an essential singularity.
	std::complex<double> one_minus_z = 1.0 - z;
	if (!nonzero(one_minus_z))
		return INFTY; // at z=1, G_q(z) has a pole.
	register std::complex<double> result = 1.0 / one_minus_z; // = 1/(1-z)
	// at first, we take the terms q^n / z, q^n * z with n=1,
	register std::complex<double> q_to_n_times_z = q * z; 
	register std::complex<double> q_to_n_over_z = q / z; // safe as z != 0
	while (!nonzero(q_to_n_over_z)) // main loop runs until q_to_n_over_z is indistinguishable from 0.
	{
		result *= (1.0 + q_to_n_over_z)
				/               // Costly call to __divdc3 :-/
			(1.0 - q_to_n_times_z);
		q_to_n_over_z *= q;
		q_to_n_times_z *= q;
	}
	return result;
}
// ================================================================================================
std::complex<double> c(std::complex<double> q) noexcept
// Returns c(q)
{
	std::complex<double> result = 1.0;
	std::complex<double> q_to_n = q;
	while (nonzero(q_to_n))
	{
		result *= square(1.0 - q_to_n)
				/
			(1.0 - square(q_to_n));
		q_to_n *= q;
	}
	return result;
}
// ================================================================================================
std::complex<double> kn_sum(std::complex<double>* array, int length)
{
	// Adds up the complex numbers in the array 'array' of length 'length'
	// using the Kahan–Neumaier running compensation algorithm.
	if (length < 1)
		return 0.0;
	register double re_sum = array[0].real(); 
	register double im_sum = array[0].imag(); // set sums to first element
	register double re_compensation = 0.0;
	register double im_compensation = 0.0;
	register double re_tentative, im_tentative;
	for (register int i=1; i<length; i++) // start from 1, since 0 is already in the sum
	{
		re_tentative = re_sum + array[i].real(); 
		im_tentative = im_sum + array[i].imag(); // tentatively add naively
		re_compensation += // update compensation depending on the relative magnitudes
			( abs(re_sum) >= abs(array[i].real()) )? // sum may swamp out the increment
				(re_sum - re_tentative) + array[i].real(): //gives array[i] a chance
				(array[i].real() - re_tentative) + re_sum; //gives the sum a chance
		im_compensation += // update compensation depending on the relative magnitudes
			( abs(im_sum) >= abs(array[i].imag()) )? // sum may swamp out the increment
				(im_sum - im_tentative) + array[i].imag(): //gives array[i] a chance
				(array[i].imag() - im_tentative) + im_sum; //gives the sum a chance
		re_sum = re_tentative;
		im_sum = im_tentative;
	}
	return std::complex<double>(re_sum + re_compensation, im_sum + im_compensation);
}
// ================================================================================================
double parse_double(char* input)
{
	try
	{
		return std::stod(std::string(input));
	}
	catch (std::invalid_argument& e)
	{
		std::cerr << "Invalid floating point number: '" << input << "'!" << std::endl;
		return 0.0;
	}
}
// ================================================================================================
int parse_int(char* input)
{
	try
	{
		return std::stoi(std::string(input));
	}
	catch (std::invalid_argument& e)
	{
		std::cerr << "Invalid integer number: '" << input << "'!" << std::endl;
		return 0;
	}
}
// ================================================================================================

