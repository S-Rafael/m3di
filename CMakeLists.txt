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
project(m3di LANGUAGES CXX)

if (NOT BUILD_DIRECTORY)
  set(BUILD_DIRECTORY build)
endif()

add_subdirectory(src ${BUILD_DIRECTORY})
install(FILES ${BUILD_DIRECTORY}/m3di TYPE BIN)

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
