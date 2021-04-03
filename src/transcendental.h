/*
 *   Copyright (C) 2019-2021 Rafael M. Siejakowski.
 *   All rights reserved.
 *   License information at the end of the file.
 */
#ifndef __TRANSCENDENTAL_H__
#define __TRANSCENDENTAL_H__

#include <complex>
#include <cfloat>
#include <cmath>
#include "constants.h"

using CC = std::complex<double>;
/*
	Here we declare the key functions G_q, c used for the
	numerical evaluation of the transcendental functions G_q(z) and c_q.
*/
template<typename q_t>
CC G_q(q_t q, CC z) noexcept;

template<typename q_t>
CC c(q_t q) noexcept;

// Inline helper functions:
inline CC square(CC z) {return z*z;};

inline bool is_subnormal(CC z)
/*
 *  Checks whether the real and imaginary parts of z are so small that
 *  they're "subnormal doubles", i.e., they have leading zeros in the
 *  mantissa because we have minned out (is that a word?) the exponent.
 *
 */
{
    return (std::abs(z.real()) < DBL_MIN && std::abs(z.imag()) < DBL_MIN);
}

#endif

/*
 *
 * Copyright (C) 2019-2021 Rafael M. Siejakowski
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
