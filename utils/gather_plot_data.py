#!/usr/bin/python
#
# (C) Copyright 2018-2021 by Rafael M. Siejakowski
# All rights reserved.
#
# License information at the end of the file. 
#
###############################################################################
# 
# This is a Python script for collecting multiple JSON output files
# produced by the program `m3di` and assembling them into a single
# list, suitable for plotting e.g. with SageMath.
#
###############################################################################
try:
    import glob
    import sys
    import json
except ImportError:
    print("Error! Could not import one of the required packages.")

ZERO_PAIR = tuple([0, 0])

class JsonError(BaseException):
    pass
#end

def extract_pair(packet):
    """
    Takes a JSON data object representing the output of `m3di`
    and tries to read off and return the pair (kappa, I(kappa)) from it.
    In case of error, throws the JsonError exception.
    """
    try:
        hbar_real = packet['input']['hbar_real']
        hbar_imag = packet['input']['hbar_imag']
        m3di = packet['output']['real']
    except KeyError:
        raise JsonError

    if hbar_imag != 0.0:
        print("Error: encountered hbar with non-zero imaginary part!")
        raise JsonError
    
    kappa = -1/hbar_real
    return tuple([kappa, m3di])
#end

def process_json(json_filepath):
    """
    Opens a JSON file created by `m3di` and returns the pair
    (kappa, I(kappa)) extracted from it. In case of error,
    it returns ZERO_PAIR instead.
    """
    file_handle = None

    try:
        file_handle = open(json_filepath, "r")
    except IOError:
        file_handle = None

    if file_handle is None:
        print(f"Error: cannot open file '{file}' for reading!")
        return ZERO_PAIR

    data = json.load(file_handle)
    file_handle.close()

    try:
        pair = extract_pair(data)
    except JsonError:
        print(f"Error: file '{file}' doesn't have the expected format!")
        return ZERO_PAIR

    return pair
#end  

def gather_jsons(directory):
    """
    Extracts the pairs (kappa, I(kappa)) from all JSON files
    in the given directory, puts them into a list, and returns
    a string containing a Python assignment instruction which
    puts this list into a variable `out`.
    """
    # Make sure we have a trailing slash
    if directory[-1] != '/' and directory[-1] != '\\':
        directory = directory + '/'
    wildcard = directory + '*.json'
    points = []
    # Process all JSON files matching the regexp:
    for file in glob.glob(wildcard):
        points.append(process_json(file))
    # We sort the points by the kappa coordinate
    points.sort(key=lambda p: p[0])
    serialized = repr(points).replace("), ", "),\n       ") #nicer indents
    return "out = " + serialized + "\n"
#end

def write_data(data, file):
    """
    Writes the string `data` into the output file with name `file`.
    """
    file_handle = None
    try:
        file_handle = open(file, "w")
    except IOError:
        file_handle = None
    if file_handle is None:
        print(f"Error: cannot open file '{file}' for writing!")
        return
    file_handle.write(data)
    file_handle.close()
#end

def print_usage(myself):
    """
    Prints a help string about the script and how to use it.
    """
    print(f"\nUsage:\n\t{myself} <Directory> <Output>\n")
    print("Reads all JSON files (with extension *.json) in the given <Directory>\n"
          "and records the values input.hbar_real and output.real found in these\n"
          "files. After converting each value of hbar to kappa := -1/hbar, a new\n"
          "list is written to the <Output> file.  This list consists of pairs of\n"
          "the form (kappa, output.real) assembled from the JSON files. The list\n"
          "is put into a Python assignment instruction defining a variable `out`\n"
          "so that the variable can be directly passed into list_plot() function\n"
          "available in SageMath.")
#end

def main(argc, argv):
    """
    Main routine of the script; checks the command line parameters
    and launches the data processing based on their values.
    """
    if argc < 3:
        print_usage(argv[0])
    else:
        # Enough arguments were passed
        json_directory = argv[1]
        output_file = argv[2]
        plot_data = gather_jsons(json_directory)
        write_data(plot_data, output_file)
#end

if __name__ == '__main__':
    main(len(sys.argv), sys.argv)

###############################################################################
#
# Copyright (C) 2018-2021 Rafael M. Siejakowski
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
