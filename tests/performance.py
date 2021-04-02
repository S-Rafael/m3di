#!/usr/bin/env python3
#
#  Copyright (C) 2021 Rafael M. Siejakowski.
#  All rights reserved.
#  License information at the end of the file.

import json
import subprocess

test_executable = '../src/m3di'
test_file = '5_2.json'
test_hbar = '-0.1'
test_samples = 20000

def time_run():
# Runs m3di with the test settings and returns the times
	process = subprocess.run([test_executable, 'integrate', test_file, test_hbar, "0.0", 
	                          "%d"%test_samples], capture_output=True)
	data = json.loads(process.stdout)
	stats = data["statistics"]
	tabtime = stats["tabulation walltime"]
	inttime = stats["integration walltime"]
	totaltime = stats["total walltime [s]"]
	return tuple([tabtime, inttime, totaltime])

def average(array):
	result = 0
	for a in array:
		result += a
	return result/len(array)

def collect_stats(num_runs = 30):
	tabtimes = []
	inttimes = []
	totals   = []
	for r in range(num_runs):
		(tabt, intt, total) = time_run()
		tabtimes.append(tabt)
		inttimes.append(intt)
		totals.append(total)
		print("Run #%d – tab=%fs, int=%fs, total=%fs"%(r+1,tabt,intt,total))
	print("--- Averages of %d runs: ---"%num_runs)
	print("Tabulation  : %f" % average(tabtimes))
	print("Integration : %f" % average(inttimes))
	print("Total       : %f" % average(totals))

collect_stats(50)
# ---------------------------------------------------------
#
# Copyright (C) 2020 Rafael M. Siejakowski.
# 
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License 
# version 2 as published by the Free Software Foundation; 
# later versions of the GNU General Public Licence do NOT apply.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 
# 02110-1301, USA.
#
