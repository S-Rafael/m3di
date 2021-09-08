# encoding: UTF-8
#
# cmake file for m3di
#------------------------------------------------------------------------------
#
# © Copyright 2021 Rafał M. Siejakowski <rs@rs-math.net>
#   All rights reserved.
#   License information at the end of the file.
# 
#------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.5)

# Load the module for checking the existence of headers:
include(CheckIncludeFileCXX)
cmake_policy(SET CMP0075 NEW)

# Project name and C++ standard
project(m3di LANGUAGES CXX)
set(CMAKE_CXX_EXTENSIONS False)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add source files
add_executable(m3di
               integrator.cpp
               io.cpp
               kahan.cpp
               main.cpp
               manifold.cpp
               modes.cpp
               stats.cpp
               tabulation.cpp
               transcendental.cpp
               write.cpp)

# Require the library `jsoncpp`
set(CMAKE_REQUIRED_LIBRARIES "jsoncpp")
CHECK_INCLUDE_FILE_CXX("json/json.h" JSONCPP_PRESENT)
if (JSONCPP_PRESENT)
  message(STATUS "Successfully found headers for the library jsoncpp")
else()
  message(FATAL_ERROR "Fatal: Could not find the header file `json/json.h`.\n"
  "Make sure the jsoncpp library is installed and configured on your system.\n"
  "You can obtain it from [https://github.com/open-source-parsers/jsoncpp].")
endif()

# Set libraries to link
target_link_libraries(m3di jsoncpp pthread)

# Set compiler options:
# * aggressive but mathematically safe optimizations with vectorization
# * activate streaming extensions SSE v3
# * display all warnings
if (MSVC)
  target_compile_options(m3di PUBLIC /W4 /O2)
else()
  target_compile_options(m3di PUBLIC -Wall -O3 -msse3)
endif()

# Debug compiler flags
#get_target_property(DEBUG_COMPILE_FLAGS m3di COMPILE_OPTIONS)
#add_custom_command(TARGET m3di POST_BUILD COMMAND echo ${DEBUG_COMPILE_FLAGS})

#------------------------------------------------------------------------------
#
# (C) Copyright Rafał M. Siejakowski, 2021
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
