#ifndef __MODES_H__
#define __MODES_H__

#include <iostream>
#include <fstream>
#include <string>
#include "manifold.h"
#include "integrals.h"
#include "write.h"

enum program_mode {MODE_INTEGRATE, MODE_USAGE, MODE_HELP, MODE_WRITE};
const std::string MODE_INTEGRATE_STRING = std::string("integrate");
const std::string MODE_HELP_STRING_1    = std::string("help");
const std::string MODE_HELP_STRING_2    = std::string("--help");
const std::string MODE_WRITE_STRING     = std::string("write");

void display_usage();
void display_help();

program_mode decide_mode(int argc, char** argv);
void integrate_mode(int argc, char** argv);

#endif
