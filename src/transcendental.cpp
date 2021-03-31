/*
 *   Copyright (C) 2019-2021 Rafael M. Siejakowski
 *   License information at the end of the file.
 */
#include "transcendental.h"

// =============================================================================================
//  Implementation of the functions computing G_q and c_q.
// =============================================================================================
CC G_q(CC q, CC z) noexcept
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
 * This method of computing the function G_q(z) may, at a first glance,
 * seem to be wasteful because multiplication is considerably slower than addition.
 * However, the advantage is that the terms in the infinite product are
 * themselves easier to compute than the terms of the infinite series.
 * In particular, they don't require division (which is even slower than multiplication).
 *
 * There's a total of four complex multiplications in the inner loop, so hopefully they
 * take advantage of more than one pipelined ALU because the dependencies are weak.
 * However, the code could still be sped up for real q, because the multiplications
 * by q in the main loop would require fewer 'double' multiplications.
 *
 * In an earlier version, the formulae used the floating point literal "1.0"
 * which was implicitly converted to std::complex<double> but apparently,
 * all of these "1.0"-s were treated as separate by the compiler. So now we construct
 * a local constant object called "one". Inspection of assembly shows that this works.
 *
 */
{
	if (is_tiny(z))
		return INFTY; // at z=0, G_q(z) has an essential singularity.
	const CC one {1.0};
	CC numerator = one; // initial value 1
	CC denominator = one - z;  // initial value 1-z
	// at first, we take the terms q^n / z, q^n * z with n=1,
	CC q_to_n_times_z = q * z; 
	CC q_to_n_over_z = q / z; // safe as z != 0
	// main loop runs until q_to_n_over_z is indistinguishable from 0.
	while (!is_tiny(q_to_n_over_z))
	{
		numerator *= one + q_to_n_over_z;
		denominator *= one - q_to_n_times_z;
		q_to_n_over_z *= q;
		q_to_n_times_z *= q;
	}
	if (is_tiny(denominator))
		return INFTY;
	else
		return numerator/denominator;  // Costly complex division happens only once
}
// =============================================================================================
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
