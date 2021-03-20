/*
 *   Copyright (C) 2019-2021 Rafael M. Siejakowski
 *   License information at the end of the file.
 */
#ifndef __KAHAN_H__
#define __KAHAN_H__

#include <complex>
#include <vector>
/*
 * class KN_accumulator
 * 
 * Provides an accumulator for computing sums of complex numbers of the type
 * std::complex<double>. Internally, the class uses the Kahan-Neumaier running
 * compensation algorithm to achieve the best possible numerical accuracy.
 * 
 * The Kahan-Neumaier algorithm is applied separately to real and imaginary
 * parts.
 *
 * Member functions:
 * [trivial constructor and destructor],
 * reset()                          - zeroes out the internal state, thus
 *                                    clearing the object to allow reuse
 *                                    for another summation.
 * operator+=(std::complex<double>) - adds a new value to the accumulator.
 * std::complex<double> total()     - returns the total accumulated value.
 *
*/

using CC = std::complex<double>;

class KN_accumulator
{
	private:
	double re_sum {0.0}; // C+11 required for this style of class member initialization
	double im_sum {0.0}; 
	double re_compensation {0.0};
	double im_compensation {0.0};
	public:
	KN_accumulator() = default;
	~KN_accumulator() = default;
	void reset(void);
	void operator+= (CC increment);
	void accumulate(const std::vector<CC>& v);
	CC total(void);
};

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
