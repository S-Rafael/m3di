#include "modes.h"

//==========================================================================================
program_mode decide_mode(int argc, char** argv)
/*
	Based on argc, argv, decide which mode the program should run in
*/
{
	if (argc < 2) // Not enough parameters
	{
		return MODE_USAGE;
	}
	std::string mode_string = std::string(argv[1]);
	if (mode_string == MODE_INTEGRATE_STRING)
	{
		// for MODE_INTEGRATE, we expect 4 more positional params:
		// infile
		// Re(hbar)
		// Im(hbar)
		// samples
		if (argc < 4+2)
			return MODE_USAGE;
		else
			return MODE_INTEGRATE;
	}
	else if (mode_string == MODE_WRITE_STRING)
	{
		// for MODE_WRITE, we expect 4 more positional params:
		// infile
		// Re(hbar)
		// Im(hbar)
		// samples
		if (argc < 4+2)
			return MODE_USAGE;
		else
			return MODE_WRITE; 
	}
	else if (mode_string == MODE_HELP_STRING_1 || mode_string == MODE_HELP_STRING_2)
		return MODE_HELP;
	else
		return MODE_USAGE;
}
//==========================================================================================
void integrate_mode(int argc, char** argv)
/*
	This function implements the integration mode, which is the main mode of the program.
*/
{
	// Parse hbar and sample count
	double Rehbar = parse_double(argv[3]);
	double Imhbar = parse_double(argv[4]);
	int samples = parse_int(argv[5]);
	if (samples < 1)
	{
		std::cerr << "Error: The number of samples must be a positive integer!" << std::endl;
		return;
	}
	std::complex<double> hbar = std::complex<double>(Rehbar, Imhbar);
	double a = exp(Rehbar); // a = |q|
	if (a < DBL_MIN || a >= 1.0)
	{
		std::cerr << "Error: The value of q specified does not satisfy 0<|q|<1!" << std::endl;
		return;
	}
	// Read in manifold info
	mani_data M = mani_data(argv[2]); 
	if (!M.is_valid())
	{
		std::cerr << "File '" << argv[2] << "' doesn't contain a valid "
		             "manifold specification!" << std::endl;
		return;
	}
	// compute the meromorphic 3D-index
	std::complex<double> GK = GK_integral(&M, hbar, samples);
	// Format the given hbar_value
	std::ostringstream hbar_given;
	hbar_given << argv[3] << (Imhbar<0.0? "":"+") << argv[4] << "i";
	// Create JSON representation of output
	Json::Value output;
	output["hbar_given"] = hbar_given.str();
	output["samples"] = samples;
	output["hbar_real_part"] = Rehbar;
	output["hbar_imag_part"] = Imhbar;
	output["int_real_part"] = GK.real();
	output["int_imag_part"] = GK.imag();
	// Output data
	Json::StreamWriterBuilder builder;
	builder["indentation"] = "\t";
	builder.settings_["precision"] = 320;
	std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
	writer->write(output, &std::cout);
	std::cout << std::endl;
}
//==========================================================================================
void display_usage()
{
	using namespace std;
	cout 
	          << "Usage:" << endl << endl
	          << "m3di MODE PARAMETERS" << endl << endl
	          << "Avaliable modes are:" << endl
	          << MODE_INTEGRATE_STRING << endl
	          << MODE_WRITE_STRING << endl
	          << MODE_HELP_STRING_1 << endl << endl
	          << "Type \"m3di " << MODE_HELP_STRING_1 << "\" for help." << endl;
}
//==========================================================================================
void display_help()
{
	std::cout << 
"m3di - a program for computing the meromorphic 3D-index\n\n"
"Commandline syntax:\n"
"m3di COMMAND PARAMETERS\n\n"
"Available COMMANDs:\n"
"integrate\n"
"          The integrate command is used to compute the total meromorphic 3D-index.\n"
"          The syntax for this mode is:\n"
"              m3di integrate <file> <Re_hbar> <Im_hbar> <samples>\n"
"          The meaning of the parameters is as follows:\n"
"          <file>    - The path to a JSON file containing combinatorial information\n"
"                      about the triangulated 3-manifold.\n"
"          <Re_hbar> - The real part of the parameter hbar of meromorphic 3D-index.\n"
"          <Im_hbar> - The imaginary part of the parameter hbar.\n"
"          <samples> - A positive integer specifying how many sample points are to be\n"
"                      taken in each iterated integral. A higher sample count generally\n"
"                      results in a higher accuracy of the result but also in a slower\n"
"                      computation. For q not too close to the boundary of the unit disc,\n"
"                      a value of <samples> in the range 5000 to 10000 usually suffices.\n\n"
"write\n"
"          This command does not compute the state integral, but rather writes out sampled\n"
"          values of the integrand as JSON data to the standard output.\n"
"          The syntax for this mode is:\n"
"              m3di write <file> <Re_hbar> <Im_hbar> <samples>\n"
"          The meaning of the parameters is as follows:\n"
"          <file>    - The path to a JSON file containing combinatorial information\n"
"                      about the triangulated 3-manifold.\n"
"          <Re_hbar> - The real part of the parameter hbar of meromorphic 3D-index.\n"
"          <Im_hbar> - The imaginary part of the parameter hbar.\n"
"          <samples> - A positive integer specifying how many sample points are to be\n"
"                      taken in each iterated integral. A higher sample count generally\n"
"                      results in a higher accuracy of the result but also in a slower\n"
"                      computation.\n\n";
}
//==========================================================================================
