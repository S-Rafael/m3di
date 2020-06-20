/*
 *   Copyright (C) 2019-2020 Rafael M. Siejakowski
 *   License information at the end of the file.
 */
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

const std::complex<double> INFTY = std::complex<double>(std::numeric_limits<double>::infinity());
constexpr double pi = 3.14159265358979323846264338327950288419716939937510582097494459230781640628620899862803482534211706798214808651328230664709384460955058223172535940812848111745028410270193852110555964462294895493038196442881097566593344612847564823378678316527120190914564856692346034861;
constexpr double twopi = 2.0*pi;

std::complex<double> G_q(std::complex<double> q, std::complex<double> z) noexcept;
std::complex<double> c(std::complex<double> q) noexcept;

inline std::complex<double> square(std::complex<double> z) {return z*z;};
inline bool nonzero(complex<double> a)
{
	// This uses asm("cvttsd2si") so it's a lot faster than complex abs()
	return abs(a.real())>DBL_MIN || abs(a.imag())>DBL_MIN ;
}

std::complex<double> kn_sum(std::complex<double>* array, size_t length);
double parse_double(char* input);
int parse_int(char* input);

#endif

/*
 *
 * Copyright (C) 2019-2020 Rafael M. Siejakowski
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
