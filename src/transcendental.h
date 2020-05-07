#ifndef __TRANSCENDENTAL_H__
#define __TRANSCENDENTAL_H__

#include <cmath>
#include <vector>
#include <complex>
#include <cfloat>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <limits>

using namespace std;

const std::complex<double> COMPLEX_ONE = std::complex<double>(1.0);
const std::complex<double> COMPLEX_ZERO = std::complex<double>(0.0);
const std::complex<double> COMPLEX_I = std::complex<double>(0.0, 1.0);
const std::complex<double> INFTY = std::complex<double>(std::numeric_limits<double>::infinity());
constexpr double pi = 3.14159265358979323846264338327950288419716939937510582097494459230781640628620899862803482534211706798214808651328230664709384460955058223172535940812848111745028410270193852110555964462294895493038196442881097566593344612847564823378678316527120190914564856692346034861;
constexpr double twopi = 2.0*pi;

std::complex<double> G_q(std::complex<double> q, std::complex<double> z);
std::complex<double> c(std::complex<double> q);
inline std::complex<double> square(std::complex<double> z) {return z*z;};
std::complex<double> kn_sum(std::complex<double>* array, int length);
double parse_double(char* input);
int parse_int(char* input);

#endif
