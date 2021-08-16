/*
 *   Copyright (C) 2019-2021 Rafael M. Siejakowski.
 *   All rights reserved.
 *   License information at the end of the file.
 */
#include "manifold.h"
/*
 *   Implementation of the class mani_data.
 */
// =============================================================================================
bool mani_data::read_json(const char* path, Json::Value* root)
/*
	Reads and parses a JSON data file, filling in the JSON structure 'root' accordingly.
	Returns true on success, false on error.
*/
{
	Json::CharReaderBuilder parser;
	std::string error;
	std::string infile(path);
	bool success = false;
	if (infile == "-") // Read JSON from stdin
	{
		success = Json::parseFromStream(parser, std::cin, root, &error);
	}
	else // Read JSON from file
	{
		std::ifstream file(infile, std::ifstream::in);
		if (!file.good())
		{
			std::cerr << "Error: file '" << infile << "' cannot be opened for reading!"
				  << std::endl;
			return false;
		}
		success = Json::parseFromStream(parser, file, root, &error);
		file.close();
	}
	if (success)
	{
		return true;
	}
	else
	{
		std::cerr << "Error: invalid JSON data!";
		std::cerr << std::endl << "--- error details:" << std::endl << error << std::endl;
		return false;
	}
}
// =============================================================================================
bool mani_data::populate(const char* json_file)
/*
	Populates the data members of mani_data with manifold data, based
	on the JSON file with specified path: 'json_file'.
	Returns true on success, false on failure.
*/
{
	Json::Value json_data; // Data structure to be filled
	if (!read_json(json_file, &json_data))
		return false;
	// Try reading in the number of tetrahedra:
	try
	{
		N = json_data.get("N", 0).asInt();
	}
	catch (Json::LogicError &exception)
	{
		std::cerr << "Error: file '" << json_file << 
		"' does not specify the key \"N\" correctly." << std::endl
		<< "Please make sure that the key \"N\" has a positive integer value equal to the "
		"number of tetrahedra in the triangulation." << std::endl;
		return false;
	}
	// Validate number of tetrahedra
	if (N<1)
	{
		std::cerr << "Error: file '" << json_file << "' specifies \"N\" as equal to " << N
		<< ", which is not a valid number of tetrahedra in a triangulation." << std::endl;
		return false;
	}
	// Try reading in the matrix of leading-trailing deformations
	Json::Value Ltemp;
	unsigned int nrows;
	unsigned int ncols;
	try
	{
		Ltemp = json_data["L"];
		nrows = Ltemp.size();
		ncols = 3*N; // At least this is what we expect; we check it below.
		k = (nrows-N)/2;
		if (k>1) // For now, we generate an error if there are multiple cusps
		{
			std::cerr << "Error: multiple cusps are not supported by this version of the "
			"program." << std::endl;
			return false;
		}
		else if (k<1)
		{
			std::cerr << "Error: the manifold must have at least one boundary component."
			" Please ensure that the matrix specified in the key \"L\" has at least "
			<< N+2 << " rows." << std::endl;
			return false;
		}
		for (unsigned int i=0; i<nrows; i++)
		{
			if (Ltemp[i].size() != ncols) // check if the matrix is rectangular
			{
				std::cerr << "Error: file '" << json_file << 
				"' does not specify the key \"L\" correctly." << std::endl
				<< "Please make sure that the key \"L\" is an array of matrix rows (the matrix "
				"rows themselves being arrays of integers of length "<< ncols << ")."
				<< std::endl;
				return false;
			}
		}
	}
	catch (Json::LogicError& exception)
	{
		std::cerr << "Error: file '" <<json_file<< "' does not specify the key \"L\" correctly."
		<< std::endl
		<< "Please make sure that the key \"L\" contains an integer matrix." << std::endl;
		return false;
	}
	// Read and validate the initial angle structure "a" (in units of pi)
	Json::Value atemp;
	try
	{
		atemp = json_data["a"];
		if (atemp.size() != ncols) // check if the vector has the correct length
		{
			std::cerr << "Error: file '" << json_file << 
			"' does not specify the key \"a\" correctly." << std::endl
			<< "Please make sure that the key \"a\" is an array of numbers of length "
			<< ncols << "." << std::endl;
			return false;
		}
	} 
	catch (Json::LogicError& exception)
	{
		std::cerr << "Error: file '" << json_file <<
		"' does not specify the key \"a\" correctly." << std::endl
		<< exception.what() << std::endl;
		return false;
	}
	nesting = N-k;
#ifdef TRIM_LTD
	nrows = nesting; // store only the first N-k rows of the matrix
#endif
	// Temporarily store the entries of a and L
	std::vector<int> LTD_(ncols * nrows);
	std::vector<double> angles_(ncols);
	try
	{
		for (unsigned int r = 0; r < nrows; r++)
			for (unsigned int c=0; c < ncols; c++)
				LTD_[ncols*r + c] = Ltemp[r][c].asInt();
	}
	catch (Json::LogicError& exception)
	{
		std::cerr << "Error: file '" << json_file <<
		"' does not specify the key \"L\" correctly." << std::endl
		<< "Some of the entries could not be interpreted as integers." << std::endl
		<< "Detailed error description:\n\"" << exception.what() << "\"\n";
		return false;
	}
	try
	{
		for (unsigned int c=0; c < ncols; c++)
			angles_[c] = atemp[c].asDouble();
	}
	catch (Json::LogicError& exception)
	{
		std::cerr << "Error: file '" << json_file <<
		"' does not specify the key \"a\" correctly." << std::endl
		<< "Some of the entries could not be interpreted as floating point numbers."<< std::endl
		<< "Detailed error description:\n\"" << exception.what() << "\"\n";
		return false;
	}
	// All is good, we move LTD_ to LTD and angles_ to angles
	LTD = std::move(LTD_);
	angles = std::move(angles_);
	return true;
}
// =============================================================================================
mani_data::mani_data(const char* filepath)
/*
	Constructor of class mani_data.
	Takes the path of the JSON file with manifold description.
*/
{
	valid_state = populate(filepath);
	if (valid_state)
	{
		num_quads = 3*N;
		// Allocate the vector for shared_ptr's to tabulations of factors:
		G_q_tables.resize(num_quads);
	}

	else std::cerr << "Could not load triangulation info." << std::endl;
}
// =============================================================================================
void mani_data::tabulate(std::complex<double> hbar, int samples)
/*
	This function precomputes the values of the individual
	G_q(...) factors of the integrand. Each factor is evaluated
	on a circle with the given number of samples;
	the radius of the circle depends on the corresponding entry of 'angles'.
	TODO: instead of 1 thread per quad, decide thread count more intelligently.
*/
{
	if (!valid_state)
		return;
	//Compute the constant prefactor [c(q)]^N
	prefactor = std::pow(c(exp(hbar)), N);
	for (int quad=0; quad < num_quads; quad++)
	{
		// Launch tabulation for each G_q factor
		G_q_tables[quad] = std::make_shared<tabulation>(angles[quad], hbar, samples);
	}
	// Tabulation threads are now running in parallel.
	for (auto& quad : G_q_tables)
		quad->finish();
	valid_tabulation = true;
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

