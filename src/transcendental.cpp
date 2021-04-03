/*
 *   Copyright (C) 2019-2021 Rafael M. Siejakowski.
 *   All rights reserved.
 *   License information at the end of the file.
 */
#include "transcendental.h"

// =============================================================================================
//  Implementation of the functions computing G_q and c_q.
// =============================================================================================
template<typename q_t>
CC G_q(q_t q, CC z) noexcept
/* 
 * This function returns G_q(z) for |q|<1.
 * It's safe to declare it as noexcept because std::complex doesn't throw exceptions.
 *
 * However, if we hit a pole, the return value may be infinity.
 *
 * The computation is based on the infinite product representation of G_q(z).
 *
 * === Thoughts on performance. ===
 *
 * In an earlier version, the formulae used the floating point literal "1.0"
 * which was implicitly converted to std::complex<double> but apparently,
 * these "1.0"-s were treated as separate by the compiler. So now we construct
 * a local constant object called "one". Inspection of assembly shows that this works.
 *
 * In an optimization attempt, the loop termination condition was replaced by
 * a check whether the variable q_to_n_over_z has real part less than the machine epsilon
 * and the imaginary part less than DBL_MIN. The resulting code was  significantly slower
 * than the present version, so we stick with the current check, which is really fast
 * - checking if a double is "subnormal" can be done with a single x86-64 instruction.
 */
{
	if (is_subnormal(z))
		return INFTY; // at z=0, G_q(z) has an essential singularity.
	const CC one {1.0};
	CC numerator   = one; // initial value 1
	CC denominator = one - z;  // initial value 1-z
	// at first, we take the terms q^n / z, q^n * z with n=1,
	CC q_to_n_times_z = q * z; 
	CC q_to_n_over_z  = q / z; // safe as z != 0
	// main loop runs until q_to_n_over_z is indistinguishable from 0.
	while (!is_subnormal(q_to_n_over_z))
	{
		numerator      *= one + q_to_n_over_z;
		denominator    *= one - q_to_n_times_z;
		q_to_n_over_z  *= q;
		q_to_n_times_z *= q;
	}
	if (is_subnormal(denominator))
		return INFTY;
	else
		return numerator/denominator;  // Costly complex division happens only once
}
// Instantiate template for q_t in {double, CC}
template CC G_q<double>(double q, CC z) noexcept;
template CC G_q<CC>(CC q, CC z) noexcept;
// =============================================================================================
template<typename q_t>
CC c(q_t q) noexcept
// Returns c_q for |q| < 1.
{
	const CC one {1.0};
	CC numerator   = one;
	CC denominator = one;
	CC q_to_n      = q;
	while (!is_subnormal(q_to_n))
	{
		numerator   *= square(one - q_to_n);
		denominator *= one - square(q_to_n);
		q_to_n      *= q;
	}
	if (is_subnormal(denominator))
		return INFTY;
	else
		return numerator/denominator;
}
// Instantiate template for q_t in {double, CC}
template CC c<double>(double q) noexcept;
template CC c<CC>(CC q) noexcept;
// =============================================================================================
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
