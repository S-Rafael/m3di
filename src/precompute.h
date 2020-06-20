/*
 *   Copyright (C) 2019-2020 Rafael M. Siejakowski
 *   License information at the end of the file.
 */
#ifndef __PRECOMPUTE_H__
#define __PRECOMPUTE_H__

#include <thread>
#include "transcendental.h"

class precomputed
{
	std::complex<double>* buffer;
	std::complex<double> prefactor;
	std::complex<double> q;
	double startangle;
	double step;
	int length;
	static void thread_worker(precomputed* obj);
	bool ready;
	std::thread* iteration;
	public:
	precomputed(double initial_a, std::complex<double> hbar, int samples);
	~precomputed();
	std::complex<double> get(int position);
	void finish();
};

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
