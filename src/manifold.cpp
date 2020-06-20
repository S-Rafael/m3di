/*
 *   Copyright (C) 2019-2020 Rafael M. Siejakowski
 *   License information at the end of the file.
 */
#include "manifold.h"

// ================================================================================================
bool mani_data::read_json(char* filepath, Json::Value* root)
/*
	Reads and parses a JSON data file, filling in the structure fields accordingly.
	Returns true on success, false on error.
*/
{
	std::ifstream file(filepath, std::ifstream::in);
	if (!file.good())
	{
		std::cerr << "Error: file '" << filepath << "' cannot be opened for reading!" << std::endl;
		return false;
	}
	// File is good, we initialize the json parser
	Json::CharReaderBuilder parser;
	std::string error;
	// We parse the data using Json::parseFromStream
	bool success = Json::parseFromStream(parser, file, root, &error);
	file.close();
	if (!success)
	{
		std::cerr << "Error: file '" << filepath << "' is not a valid JSON document!" << std::endl;
		std::cerr << "--- error details:" << std::endl << error << std::endl;;
		return false;
	}
	return true;
}
// ================================================================================================
bool mani_data::populate(char* json_file)
/*
	Populates the structure with manifold data from the json file
	with specified path. Returns true on success, false on failure.
*/
{
	Json::Value json_data; // Data structure to be filled
	if (!read_json(json_file, &json_data))
		return false;
	// Try interpreting the number of tetrahedra:
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
			std::cerr << "Error: multiple cusps are not supported by this version of the program." 
			<< std::endl;
			return false;
		}
		else if (k<1)
		{
			std::cerr << "Error: the manifold must have at least one boundary component." 
			<< std::endl;
			return false;
		}
		for (unsigned int i=0; i<nrows; i++)
		{
			if (Ltemp[i].size() != ncols) // check if the matrix is rectangular
			{
				std::cerr << "Error: file '" << json_file << 
				"' does not specify the key \"L\" correctly." << std::endl
				<< "Please make sure that the key \"L\" is an array of matrix rows (the matrix rows "
				"themselves being arrays of integers of length "<< ncols << ")." << std::endl;
				return false;
			}
		}
	}
	catch (Json::LogicError& exception)
	{
		std::cerr << "Error: file '" << json_file << "' does not specify the key \"L\" correctly." 
		<< std::endl
		<< "Please make sure that the key \"L\" contains an integer matrix." << std::endl;
		return false;
	}
	// Read and validate the initial angle structure a in units of pi
	Json::Value atemp;
	try
	{
		atemp = json_data["a"];
		if (atemp.size() != ncols) // check if the vector has the correct length
		{
			std::cerr << "Error: file '" << json_file << 
			"' does not specify the key \"a\" correctly." << std::endl
			<< "Please make sure that the key \"a\" is an array of length "
			<< ncols << "." << std::endl;
			return false;
		}
	} 
	catch (Json::LogicError &exception)
	{
		std::cerr << "Error: file '" << json_file <<
		"' does not specify the key \"a\" correctly." << std::endl;
		return false;
	}
	// Temporarily store the entries of a and L
	vector<int> LTD_(ncols * nrows);
	vector<double> angles_(ncols);
	try
	{
		for (unsigned int r = 0; r < nrows; r++)
			for (unsigned int c=0; c < ncols; c++)
				LTD_[ncols*r + c] = Ltemp[r][c].asInt();
	}
	catch (Json::LogicError &exception)
	{
		std::cerr << "Error: file '" << json_file <<
		"' does not specify the key \"L\" correctly." << std::endl
		<< "Some of the entries could not be interpreted as integers." << std::endl;
		return false;
	}
	try
	{
		for (unsigned int c=0; c < ncols; c++)
			angles_[c] = atemp[c].asDouble();
	}
	catch (Json::LogicError &exception)
	{
		std::cerr << "Error: file '" << json_file <<
		"' does not specify the key \"a\" correctly." << std::endl
		<< "Some of the entries could not be interpreted as floating point numbers." << std::endl;
		return false;
	}
	// All is good, we move LTD_ to LTD and angles_ to angles
	LTD = std::move(LTD_);
	angles = std::move(angles_);
	return true;
}
// ================================================================================================
mani_data::mani_data(char* filepath): valid_state(false), valid_precomp(false)
/*
	Constructor of class mani_data.
	Takes the path of the JSON file with manifold description.
*/
{
	valid_state = populate(filepath);
	if (valid_state)
		precomputed_quads = new precomputed*[3*N];
	else std::cerr << "Could not load triangulation info." << std::endl;
}
// ================================================================================================
mani_data::~mani_data()
{
	if (valid_state)
	{
		//deallocate precomputed data
		if (valid_precomp)
		{
			for (int quad=0; quad<3*N; quad++)
				delete precomputed_quads[quad];
			delete [] precomputed_quads;
		}
	}
}
// ================================================================================================
void mani_data::precompute(std::complex<double> hbar, int samples)
/*
	This function precomputes the values of the individual
	G_q factors of the integrand. Each factor is evaluated
	on the unit circle with the given number of samples.
*/
{
	// TODO there's gotta be a better way to create thread objects, perhaps using jthread?
	//precompute c(q)
	cq_factor = c(exp(hbar));
	for (int quad=0; quad<3*N; quad++)
	{
		// Launch precomputation for each G_q factor
		precomputed_quads[quad] = new precomputed(angles[quad], hbar, samples);
	}
	// Precomputation threads are now running in parallel.
	for (int quad=0; quad<3*N; quad++)
		precomputed_quads[quad]->finish();
	valid_precomp = true;
}
// ================================================================================================
int mani_data::ltd_exponent(unsigned int* indices, int quad)
/*
	Given variable indices, this function returns the sample index of the
	product of N-1 edge vars at quad.
	In other words, this function realizes the dot product with l(quad).
*/
{
	int sum=0;
	for (int edge=0; edge<N-1; edge++) //last edge variable omitted
		sum += indices[edge] * LTD[(3*N*edge) + quad];
	return sum;
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

