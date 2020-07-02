/*
 *   Copyright (C) 2019-2020 Rafael M. Siejakowski
 *   License information at the end of the file.
 */
#include "transcendental.h"

// ================================================================================================
//  Implementation of the functions computing G_q and c_q.
// ================================================================================================
CC G_q(CC q, CC z) noexcept 
/* 
 * This function returns G_q(z) for |q|<1.
 * It's safe to declare it as noexcept because std::complex doesn't throw exceptions.
 *
 * However, if we hit a pole, the return value may be infinity
 */
{
	if (is_tiny(z))
		return INFTY; // at z=0, G_q(z) has an essential singularity.
	CC numerator = 1.0; // initial value 1
	CC denominator = 1.0 - z;  // initial value 1-z
	// at first, we take the terms q^n / z, q^n * z with n=1,
	CC q_to_n_times_z = q * z; 
	CC q_to_n_over_z = q / z; // safe as z != 0
	while (!is_tiny(q_to_n_over_z)) // main loop runs until q_to_n_over_z is indistinguishable from 0.
	{
		numerator *= 1.0 + q_to_n_over_z;
		denominator *= 1.0 - q_to_n_times_z;
		q_to_n_over_z *= q;
		q_to_n_times_z *= q;
	}
	if (is_tiny(denominator))
		return INFTY;
	else
		return numerator/denominator;  // Costly call to __divdc3 happens only once
}
// ================================================================================================
CC c(CC q) noexcept
// Returns c(q) for |q| < 1.
{
	CC numerator = 1.0;
	CC denominator = 1.0;
	CC q_to_n = q;
	while (!is_tiny(q_to_n))
	{
		numerator *= square(1.0 - q_to_n);
		denominator *= (1.0 - square(q_to_n));
		q_to_n *= q;
	}
	if (is_tiny(denominator))
		return INFTY;
	else
		return numerator/denominator;
}
// ================================================================================================
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
