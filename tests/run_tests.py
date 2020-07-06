#!/usr/bin/env python3

#  Copyright (C) 2020 Rafael M. Siejakowski
#  License information at the end of the file.

import sys
if sys.version_info[0] < 3 or (sys.version_info[0] == 3  and sys.version_info[1] < 5):
    raise Exception("This program requires Python 3.5 or newer.")
import json;
import subprocess;

KnownIntegralValues = [
	{'json': "4_1.json",
	 'kappa': 10.0,
	 'value_real': -4.11238897846823903137192246504127979278564453125,
	 'value_imag': 0.0
	},
	{'json': "4_1.json",
	 'kappa': 20.0,
	 'value_real': -8.497616394720036936405449523590505123138427734375,
	 'value_imag': 0.0
	},
	{'json': "4_1.json",
	 'kappa': 30.0,
	 'value_real': 4.8085374720036409001977517618797719478607177734375,
	 'value_imag': 0.0
	},
	{'json': "5_2.json",
	 'hbar': -0.14084507042253521791330683754495112225413322448730468750,
	 'value_real': 1.9968134574009461790211616971646435558795928955078125,
	 'value_imag': 0.0
	},
	{'json': "5_2.json",
	 'hbar': -0.120481927710843372825166852635447867214679718017578125,
	 'value_real': 7.52313655517506862935306344297714531421661376953125,
	 'value_imag': 0.0
	},
	{'json': "5_2.json",
	 'hbar': -0.10869565217391304046135047656207461841404438018798828125,
	 'value_real': 4.59545783844553401564780870103277266025543212890625,
	 'value_imag': 0.0
	},
	]
#==================================================================================================
def test_NaN(executable):
	"""
	Tests the behaviour of the executable with a JSON file
	that produces a NaN value of the integral.
	"""
	nan_dict = {'json': "NaN.json",	 'hbar': -0.1}
	nan_samples = 20 # enough for us, for it suffices to hit one singularity
	test = subprocess.run([executable, 
	                       'integrate',
	                       nan_dict['json'],
	                       "%.60f"%nan_dict['hbar'], "0.0", "%d"%nan_samples],
	                       capture_output=True)
	# We expect retcode = 0
	if (test.returncode != 0):
		return False 
	# We expect no error
	if (test.stderr != b''):
		print("There were errors when running\n%s integrate %s %.60f 0.0 %d"%(
			executable, nan_dict['json'], nan_dict['hbar'], nan_samples))
		print("Error message:")
		print(test.stderr)
		return False
	# We expect the integral value to be in the result JSON
	output = json.loads(test.stdout)
	if (not 'int_real_part' in output) or (not 'int_imag_part' in output):
		print("Error: incorrect return format of %s:"%executable)
		print("Returned bytes:\n---")
		print(test.stdout)
		return False
	# Check if returned values are exactly equal to the known ones
	if (output['int_real_part'] == 'infinity or removable singularity' and
	    output['int_imag_part'] == 'infinity or removable singularity'):
		return True
	else:
		print("Error: NaN integral value reported as " + output['int_real_part'] + ", " 
		      + output['int_imag_part'] + ".")
		return False
#==================================================================================================
def test_singular(executable):
	"""
	Tests the cases when the integral is NaN or infinity
	"""
	print("Testing singular cases...")
	result = test_NaN(executable)
	if (result):
		print("NaN handling: " + green("PASS"))
	else:
		print("NaN handling: ERROR")
#==================================================================================================		
def test_integrate(known_dict, executable, samples, precision):
	"""
	Tests the value of the integral returned by 'm3di integrate' against
	a known value. Numerical error must be less than 'precision' with the
	given number of samples. Returns True on success, False on failure.
	"""
	if 'hbar' in known_dict:
		hbar = known_dict['hbar']
	elif 'kappa' in known_dict:
		hbar = -1.0/known_dict['kappa']
	else:
		return False
	test = subprocess.run([executable, 
	                       'integrate',
	                       known_dict['json'],
	                       "%.60f"%hbar, "0.0", "%d"%samples],
	                       capture_output=True)
	if (test.returncode != 0):
		return False
	if (test.stderr != b''):
		print("There were errors when running\n%s integrate %s %.60f 0.0 %d"%(
			executable, known_dict['json'], hbar, samples))
		print("Error message:")
		print(test.stderr)
		return False
	output = json.loads(test.stdout)
	if (not 'int_real_part' in output) or (not 'int_imag_part' in output):
		print("Error: incorrect return format of %s:"%executable)
		print("Returned bytes:\n---")
		print(test.stdout)
		return False
	# Check if returned values are exactly equal to the known ones
	if (output['int_real_part'] == known_dict['value_real'] and
	    output['int_imag_part'] == known_dict['value_imag']):
		return True
	# otherwise, we check the numerical error
	real_error = output['int_real_part'] - known_dict['value_real']
	imag_error = output['int_imag_part'] - known_dict['value_imag']
	if (abs(real_error) > precision):
		print("Failed to attain required precision with %d samples:"%samples)
		print("Computed real_part: %.60f"%output['int_real_part'])
		print("Expected real_part: %.60f"%known_dict['value_real'])
		print("Error in real_part: %.60f"%real_error)
		print("Absolute error = %.60f > %.60f = precision"%(abs(real_error),precision))
		return False
	if (abs(imag_error) > precision):
		print("Failed to attain required precision with %d samples:"%samples)
		print("Computed imag_part: %.60f"%output['int_imag_part'])
		print("Expected imag_part: %.60f"%known_dict['value_imag'])
		print("Error in imag_part: %.60f"%imag_error)
		print("Absolute error = %.60f > %.60f = precision"%(abs(imag_error),precision))
		return False
	return True
#==================================================================================================
def green(text):
	""" 
	Surrounds 'text' with POSIX shell green color begin and end commands
	"""
	return "\033[0;32m" + text + "\033[0m"
#==================================================================================================
def get_executable_path():
	"""
	Returns the path to the m3di executable
	"""
	local = subprocess.run(['ls', '../src/m3di'], capture_output=True)
	if (local.returncode == 0):
		return '../src/m3di'
	which = subprocess.run(['which', 'm3di'], capture_output=True)
	if (which.returncode == 0):
		return which.stdout
	raise Exception("Error: couldn't locate 'm3di' either in $PATH or in '../src'.")
#==================================================================================================
def test_known_values(executable):
	"""
	Tests against known values in the array KnownIntegralValues
	"""
	precision = 1e-8 # reasonable precision for now
	sample_counts = [5000, 10000, 15000]
	test_number = 1
	print("Testing against known values...")
	for K in KnownIntegralValues:
		for S in sample_counts:
			result = test_integrate(K, executable, S, precision)
			if (result):
				print(("Test %02d:\t"%test_number) + green("PASS") +
				       " [to within %f]"%precision)
			test_number += 1
#==================================================================================================
def test_malformed_JSON(executable, path):
	"""
	Tests the behaviour of 'executable' when passed the 'path'
	to a malformed JSON data file.
	"""
	test = subprocess.run([executable, 
	                       'integrate',
	                       path,
	                       "-0.1", "0.0", "10"], #just 10 samples (shouldn't run anyway)
	                       capture_output=True)
	if (test.returncode == 0):
		return False # retcode == 0 means error wasn't detected
	else:
		return True # retcode != 0 means the program correctly detected the error
#==================================================================================================
def test_malformed_input(executable):
	"""
	Tests against malformed JSON input
	"""
	malformed_count = 15 # increase when you add extra tests
	print("Testing program behaviour with malformed JSON input...")
	test_number = 1
	for m in range(malformed_count):
		malformed_file = "malformed%d.json"%m
		result = test_malformed_JSON(executable, malformed_file)
		if (result):
			print(("Test %02d: '%s':\t"%(test_number,malformed_file)) + green("PASS"))
		test_number += 1
#==================================================================================================
def run_tests():
	"""
	Runs all tests.
	"""
	program = get_executable_path()
	print("Testing executable at %s ..."%program)
	test_malformed_input(program)
	test_singular(program)
	test_known_values(program)
#==================================================================================================
run_tests()
#==================================================================================================
#
# Copyright (C) 2020 Rafael M. Siejakowski
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
