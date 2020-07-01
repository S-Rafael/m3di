/*
 *   Copyright (C) 2019-2020 Rafael M. Siejakowski
 *   License information at the end of the file.
 */
#ifndef __TRANSCENDENTAL_H__
#define __TRANSCENDENTAL_H__

#include <complex>
#include <cfloat>
#include <limits>

/*
	Here we declare some constants and the functions G_q, c
	needed for numerical evaluation of the transcendental functions G_q(z) and c_q.
*/

// Type name aliases
using CC = std::complex<double>;

// Compile-time constants:
const CC INFTY = CC(std::numeric_limits<double>::infinity());
constexpr double pi = 3.14159265358979323846264338327950288419716939937510582097494459230781640628620899862803482534211706798214808651328230664709384460955058223172535940812848111745028410270193852110555964462294895493038196442881097566593344612847564823378678316527120190914564856692346034861;
constexpr double twopi = 2.0*pi;

// Function declarations:
CC G_q(CC q, CC z) noexcept;
CC c(CC q) noexcept;

// Inline helper functions:
inline CC square(CC z) {return z*z;};
inline bool nonzero(CC a)
{
	// This is a lot faster than complex abs()
	return abs(a.real())>DBL_MIN || abs(a.imag())>DBL_MIN ;
}

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
