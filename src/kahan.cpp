/*
 *   Copyright (C) 2019-2021 Rafael M. Siejakowski.
 *   All rights reserved.
 *   License information at the end of the file.
 */

#include <complex>
#include <vector>

#include "kahan.h"

/**
 * @file Implementation of the KN_accumulator class
 */

// =============================================================================================
/**
 * @brief
 * Resets the KN_accumulator to the initial state.
*/
void KN_accumulator::reset(void)
{
	re_sum = im_sum = 0.0;
	re_compensation = im_compensation = 0.0;
}
// ---------------------------------------------------------------------------------------------
/**
 * @brief
 * Adds in an entire vector's worth of values
 */
void KN_accumulator::accumulate(const std::vector<CC>& v)

{
	for (CC z : v)
		operator+=(z);
}
// ---------------------------------------------------------------------------------------------
/**
 * @brief
 * Adds a new value to the accumulator, using a complex version
 * of the Kahan-Neumaier (KN) running compensation algorithm.
*/
void KN_accumulator::operator+= (CC increment)
{
	double re_increment = increment.real();
	double im_increment = increment.imag();
	// We tentatively add the real and imaginary parts
	double re_tentative = re_sum + re_increment;
	double im_tentative = im_sum + im_increment;
	// Now, we update the compensation depending on the error of the above additions.
	re_compensation += // update compensation depending on the relative magnitudes
		(std::abs(re_sum) >= std::abs(re_increment))? // sum dominates ?
			(re_sum - re_tentative) + re_increment: // use sum for the cancellation
			(re_increment - re_tentative) + re_sum; // use increment for the cancellation
	im_compensation += // as above, but for imaginary parts
		(std::abs(im_sum) >= std::abs(im_increment))?
			(im_sum - im_tentative) + im_increment:
			(im_increment - im_tentative) + im_sum;
	re_sum = re_tentative;
	im_sum = im_tentative;
}
// ---------------------------------------------------------------------------------------------
/**
 * @brief Conversion operator for getting a complex number out
 */
KN_accumulator::operator CC(void)
{
	return CC(re_sum + re_compensation, im_sum + im_compensation);
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
